/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/***************************************************************************************************
* Name:         app_layer.c
* Author:       Nivis LLC
* Date:         November 2007
* Description:  Application Layer
* Changes:
* Revisions:
****************************************************************************************************/

#include <string.h>
#include "datalink_layer.h"
//#include "datalink_layer_dm.h"
#include "app_layer.h"
//#include "..\WiredHart\DataLinkLayer.h"
#include "app_cmdreq.h"
#include "app_cmdresp.h"
#include "tmr_util.h"

#include "drm.h"
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
#include "..\CommonAPI\Common_API.h"
#endif
#define APPSRVC_TASK_FREQ     USEC_TO_FRACTION2( 10000 ) // each 10ms


// HART commands 961, 963 and 965 may have the execution time set in the future, in which case they're stored
//   for processing at a later (appropriate) time. Command 972 is handled using the same mechanism
uint8               g_ucPendingCmds;        // set if there are is at least one command awaiting execution
PENDING_CMDS_DATA   g_sPendingCmdsData;     // command data for C961, C963, C965, C972 and some control flags
uint8               g_ucSrvcTaskFlag; 

uint8 g_ucMarkDisconnectHandle;
  
#ifdef PUBLISH_CMD784
static uint16 unLnkIdx = 0;
#endif

// the Service ID requested by wireless device, must be in range 0x00 ... 0x7F
uint8     g_ucRequestedServiceId; 

// additional device status structure
Cmd48Data g_stCmd48Data; 

// when set => perform the self test function
uint8 g_ucDoSelfTest; 
// HealthReports
static void APP_InitHealthReports( void );

/* this flag is set while the construction of a HealthReport is active */
static uint8 g_ucGenerateHealthReport=0;

/* this flag is set if a new NPDU is needed (when transmitting HealthReports) */
static uint8 g_ucNextNPDU=0;

/* HealthReport StateMachine status */
static uint8 g_ucSendHealthReportStatus;

/* hold the next neighbor index (for HealthReports transmitted in multiple NPDUs) */
static uint8 g_ucNextIndex, g_ucNextIndexOld;

/* NPDU counter (starting with 0) for HealthReports */
static uint8 g_ucNPDUCnt;

/* APDU buffer */
uint8 g_aucBuffer[92];
//uint8 g_aucBuffer[100];   // just as a precaution, but still must not use more than 92 bytes for APDU 
BinaryStream g_stBuffStream;

/* the composed command */
//uint8 g_aucComposedCommand[92];
uint8 g_aucComposedCommand[8 + sizeof(C787_ReportNeighborSignalLevels_Resp)]; // max size of response structure of any HealthReport/Burst/Event command!!!

/* compose offset */
//uint8 g_ucComposeOffset;

/* Health Reports handle */
uint16 g_unHealthReportHandle;

// Burst Mode

#ifdef BURST_PIPE_100MS
  // 100ms pipe, this is supposed to be ONLY ONE for burst/event !!!
  PIPE_100MS g_stBurstPipe100ms, g_stEventPipe100ms;
#endif

// set when the first burst message is sent
uint8 g_ucBurstIsActive;

// test counter
//float g_fValue;

/* !!!TBD: c_aVarMap should not be constant when cmd 51 will be implemented! */
// Dynamic Variable Mapper (device variable - dynamic variable) - the loop board has only the PV
  const uint8 c_aVarMap[NoOfDynamicVariables] =
  {
    PV_IDX, // index of device variable associated with the PV
    SV_IDX, // index of device variable associated with the SV
    TV_IDX, // index of device variable associated with the TV
#if (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)
    QV_IDX  // index of device variable associated with the QV
#endif
  };
 
// device burst table
BURST_MODE_T g_stBurstModeTable[MAX_BURST_NO];

// device variables buffer
#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
  #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
    DEVICE_VARIABLE_T g_stDeviceVariables[ANALOG_CH_NO + DIGITAL_CH_NO];
  #else
    DEVICE_VARIABLE_T g_stDeviceVariables[LOCAL_ANALOG_CH_NO + ANALOG_CH_NO];
    uint8 g_aHartDVCodes[LOCAL_ANALOG_CH_NO];
  #endif
#else 
  //due to mapping dynamic on device vars, no use in keeping them doubled!!!
  DEVICE_VARIABLE_T g_stDeviceVariables[NoOfDeviceVariables + 2];
#endif

// burst/event message handle 
uint16 g_unBurstAndEventsHandle; 

// timers for min. update period
TIME_10MS MinUpdateTimer[MAX_BURST_NO];

// timers for max. update period
TIME_10MS MaxUpdateTimer[MAX_BURST_NO];

// Event Notification 
EVENT_NOTIFICATION_T g_stEventsTable[MAX_EVENTS_NO];

// EVENT QUEUE
EVENT_PENDING_T g_stEventQueue[MAX_PENDING_EVENTS];
EVENT_PENDING_T* g_pEventQueueEnd;

static uint32 g_ulLastRTC; // old value of RTC_COUNTER, used for triggering events
static uint8  g_ucLastScanIndex; // last scan index in g_aAllDeviceStatus 
static uint8  g_aAllDeviceStatus[3][26]; // scan history (last three scans)
static uint16 g_aCCCounterHistory[3]; // configuration changed counter attached to scan history

// this table gives the scan order within the scan history
// example: g_ucLastScanIndex=1 -> oldest scan index = 2, mid-scan index = 0, last scan index = 1;
static const uint8 c_aScanIndexTable[3][3] = { {1,2,0}, {2,0,1}, {0,1,2} };  

// acknowledge for an event -> set if Cmd119 request is received
uint8 g_ucIsEventAck;

// timer table for retry period of an event
TIME_10MS RetryTimer[MAX_EVENTS_NO];

// timer table for update period of an event
TIME_10MS UpdateTimer[MAX_EVENTS_NO];

// Time & Date struct
TIME_DATE_T g_stUTCatASN0;

const uint16 c_aDays4Month[13]  = {0,31,59,90,120,151,181,212,243,273,304,334,365}; // normal year
const uint16 c_aDays4MonthL[13] = {0,31,60,91,121,152,182,213,244,274,305,335,366}; // leap year

// when a device/dynamic variable requested is not supported -> the corresponding value must be set to 0x7FA00000
const SPECIAL_FLOAT_T c_stNoValue = {0x7FA00000}; 

// Alarms
uint16 g_unAlarmHandle;
uint8  g_ucAlarmBuffLen;
uint8  g_aAlarmBuff[APP_MAX_ALARM_BUFF_LEN]; // contains pending, aggregated alarm command responses to be published

extern uint16 g_unDiscPktHandle;


static void APP_InitEventScanTable( void );

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_Init
// Author: NIVIS LLC
// Description: Init function for the Application Layer
// Parameters: none
// Return: None
// Obs:
//      Access level: user level, before entering the main loop
///////////////////////////////////////////////////////////////////////////////////
void APP_Init()
{
  
  g_ucRequestedServiceId = 0;
  g_ucPendingCmds = 0;
  g_ucSrvcTaskFlag = 1;
  g_ucAlarmBuffLen = 0;
  
  memset(&g_stCmd48Data.byte[0], 0x00, 25);
  
  g_ucDoSelfTest = 0;
  g_ucDoDeviceInitFromCmd = APP_DEVRESET_INACTIVE;
  g_ulDelayInitFromCmd = 0;
  
 
  // Time & Date initialisation
  g_stUTCatASN0.m_ucUTCReceived = 0x00;
  g_stUTCatASN0.m_ucYear        = 0x00;   // year 1900
  g_stUTCatASN0.m_ucMonth       = 0x01;   // January
  g_stUTCatASN0.m_ucDay         = 0x01;   // 1st 
  g_stUTCatASN0.m_ulTimeOfDay   = 0;      // 00:00:00 
  g_stUTCatASN0.m_ullASN        = 0;      
  
  /*
  g_stTimeDate.m_ucHour = 0x00;  // 00:00:00   
  g_stTimeDate.m_ucMin = 0x00;
  g_stTimeDate.m_ucSec = 0x00;
  */
  
  APP_InitHealthReports(); // init HealthReports

  APP_InitBurstAndEvent(); // init BurstMode / Event Notification
  
  g_unAlarmHandle = 0xA000; // init Alarms handle

  #ifdef DR_USE_MORE_BUFFERS
    memset(g_aDRSlaveBuf, 0, sizeof(g_aDRSlaveBuf));
  #else 
    memset(&g_stDRSlaveBuf, 0, sizeof(g_stDRSlaveBuf));
  #endif //DR_USE_MORE_BUFFERS
    
  memset(g_aAPPQueue, 0, sizeof(g_aAPPQueue));    

  memset(g_aReqNMQueue, 0, sizeof(g_aReqNMQueue));     
  
  g_ucMarkDisconnectHandle = 0;
  g_unDiscPktHandle = 0xFFFF; // this is an intentionally invalid value for a packet handle
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_DateToTime()
// Author: NIVIS LLC
// Description: convert a certain Time and Date into 10ms tics (since 01/01/1900 00:00:00)
// Parameters: p_pTimeDate -> Time & Date
//             p_p10msTics -> computed time 
// Return: None
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_DateToTime( const TIME_DATE_T* p_pTimeDate, unsigned long long* p_p10msTics )
{
    
  *p_p10msTics = (unsigned long long)((
                 (p_pTimeDate->m_ucMonth > 2) ? (p_pTimeDate->m_ucYear/4 + 1) : 
                ((p_pTimeDate->m_ucYear >= 1) ? ((p_pTimeDate->m_ucYear-1)/4 + 1) : 0) +                     
                  p_pTimeDate->m_ucYear * 365 + 
                  c_aDays4Month[p_pTimeDate->m_ucMonth - 1] + 
                 (p_pTimeDate->m_ucDay - 1))*24*3600*100 + (p_pTimeDate->m_ulTimeOfDay/(10*32))); //[10 ms]
} 

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_TimeToDate()
// Author: NIVIS LLC
// Description: set the actual time and date based on the current ASN
// Parameters: p_ullASN -> current ASN
//             p_pTimeDate -> computed Time & Date        
// Return: None
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_TimeToDate(unsigned long long p_ullASN, TIME_DATE_T* p_pTimeDate)
{ 
  uint16 unDayWithinYear;
  
  unsigned long long ullTmp = p_ullASN/100; // time in seconds since ASN0                             
  ullTmp /= 3600*24; // time in days since ASN0
  
  // time in 1/32 ms within a day
  p_pTimeDate->m_ulTimeOfDay = (p_ullASN%(100*3600*24))*10*32;     //[1/32 ms]
  
  for( p_pTimeDate->m_ucYear = 58; // years from 1900 (supposing that TAI0 = 01.01.1958)
       ullTmp >= (p_pTimeDate->m_ucYear % 4 ? 365 : 366); 
       ullTmp -= (p_pTimeDate->m_ucYear % 4 ? 365 : 366), p_pTimeDate->m_ucYear++ ); // present year 

  unDayWithinYear = ullTmp; // calculate days since January 1st
  
  if (p_pTimeDate->m_ucYear % 4 != 0)          
  { // no leap year
    for( p_pTimeDate->m_ucMonth = 11; 
         unDayWithinYear < c_aDays4Month[p_pTimeDate->m_ucMonth]; 
         p_pTimeDate->m_ucMonth-- ); 
  
    p_pTimeDate->m_ucDay = unDayWithinYear - c_aDays4Month[p_pTimeDate->m_ucMonth] + 1; // present day
  }
  else                                  
  { // leap year
    for( p_pTimeDate->m_ucMonth = 11; 
         unDayWithinYear < c_aDays4MonthL[p_pTimeDate->m_ucMonth]; 
         p_pTimeDate->m_ucMonth-- );
    
    p_pTimeDate->m_ucDay = unDayWithinYear - c_aDays4MonthL[p_pTimeDate->m_ucMonth] + 1; //present day
  }
  
  // present month
  p_pTimeDate->m_ucMonth ++;
 
/* OLD VARIANT -> maintain the Time & Date every one second 
  
  static uint8 ucIdx=0; // month index in c_aDays4Month / c_aDays4MonthL
  static uint16 unDayWithinYear=1; // absolute day (within a year)
  
  
  if( ++g_stTimeDate.m_ucSec > 59 )
  { // every minute
    g_stTimeDate.m_ucSec = 0;
    if( ++g_stTimeDate.m_ucMin > 59 )
    { // every hour
      g_stTimeDate.m_ucMin = 0;
      if( ++g_stTimeDate.m_ucHour > 23 )
      { // every day
        g_stTimeDate.m_ucHour = 0;
   
        // normal/leap year
        if( ++unDayWithinYear > (((g_stTimeDate.m_ucYear + 1900) % 4) ? c_aDays4Month[ucIdx + 1] : c_aDays4MonthL[ucIdx + 1]) )
        { // every month
          ucIdx ++;
            
          if( ++g_stTimeDate.m_ucMonth > 12 )
          { // every year
            g_stTimeDate.m_ucMonth = 1; 
            unDayWithinYear = 1;
            ucIdx = 0;
            g_stTimeDate.m_ucYear ++; // no check for the year (0->1900, 255->2155)
          }  
        }
        
        // day within the current month
        g_stTimeDate.m_ucDay = unDayWithinYear - (((g_stTimeDate.m_ucYear + 1900) % 4) ? c_aDays4Month[ucIdx] : c_aDays4MonthL[ucIdx]);
      }  
    }  
  } 
*/  
}  

#define UPDATE_SIZE 10
// allowed update periods in ms -> see Common Practice Command Specification / Update periods
const uint16 g_unUpdatePeriodsAllowed[UPDATE_SIZE]={100, 250, 500, 1000, 2000, 4000, 8000, 16000, 32000, 60000};

#define ADJUST_UPDATE_TIME(arg) ((arg<=46000)?((arg<=24000)?((arg<=12000)?((arg<=6000)?\
                                ((arg<=3000)?((arg<=1500)?((arg<=750)?((arg<=375)?\
                                ((arg<=175)?100:250):500):1000):2000):4000):8000):16000):32000):60000)

uint8 BurstEventCorrection( uint32* p_ulValue )
{
  uint8 ucByte=0;
  
  if( *p_ulValue < 100*60 )
  { 
    // UpdateTime
    for(; ucByte<UPDATE_SIZE; ucByte++)
    {
      if( g_unUpdatePeriodsAllowed[ucByte] == (*p_ulValue)*10 )
        break; // no correction
    }
  
    if( ucByte >= UPDATE_SIZE )
    { // need correction
      *p_ulValue = ADJUST_UPDATE_TIME((*p_ulValue)*10) / 10;
      return 1;
    }
  } 
  else
  {
    // round to seconds
    ucByte =  *p_ulValue % 100;
    if( ucByte )
    {
      *p_ulValue -= ucByte;
      return 1; 
    }  
  }  
  
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_DoBurstEventTimerCorrection()
// Author: NIVIS LLC
// Description: corrects the timing parameters for a burst/event pipe according to subsection 6.9.2 from Common Practice Cmds 
// Parameters: for burst: min/max update time; for events: retry/ max update time 
// Return: 1 -> need correction, 0 -> no correction 
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
uint8 APP_DoBurstEventTimerCorrection(uint32* p_pulParam1, uint32* p_pulParam2)
{
  uint8  ucCorrection=0;
  
  // *p_pulParam1 <= *p_pulParam12
  if( *p_pulParam1 > *p_pulParam2 )
  {  
    *p_pulParam1 = *p_pulParam2;
    ucCorrection = 1;
  }
  
  if( *p_pulParam1 > 100*3600 ) // > 3600s not allowed
  {  
    *p_pulParam1 = 100*3600;
    ucCorrection = 1;
  }
  else
  {
    ucCorrection = BurstEventCorrection(p_pulParam1); 
  }  
      
  if( *p_pulParam2 > 100*3600 ) // > 3600s not allowed
  {
    *p_pulParam2 = 100*3600;
    ucCorrection = 1;
  }
  else
  {
    ucCorrection = BurstEventCorrection(p_pulParam2); 
  } 
  
  return ucCorrection;
}

/* MASTER_THREAD_OLD 
///////////////////////////////////////////////////////////////////////////////////
// Name: ASL_IsSlaveDelayedResp
// Author: NIVIS LLC
// Description: slave delayed response, interface with master
// Parameters:
//                p_hHandle  - packet handler
//                p_ucPriority  - message priority
//                p_unSrcAddr - source address
//                p_ucSequenceNumber  - sequence nr.
//                p_unApduDataLen - APDU data length
//                p_pApduData - APDU data buffer
// Return: return code
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
uint8 ASL_IsSlaveDelayedRsp(HANDLE           p_hHandle,
                            uint8            p_ucPriority,
                            SHORT_ADDR       p_unSrcAddr,
                            uint8            p_ucSequenceNumber,
                            uint16           p_unApduDataLen,
                            const void*      p_pApduData)
{
  // store the request
  memcpy(&g_pSlaveDelayedRsp->aReq, p_pApduData, p_unApduDataLen);
  g_pSlaveDelayedRsp->unHandler = p_hHandle;
  g_pSlaveDelayedRsp->unDstAddr = p_unSrcAddr; 
  g_pSlaveDelayedRsp->ucSequenceNumber = p_ucSequenceNumber;
  // ... TBD 
    
  return 0;
}
~MASTER_THREAD_OLD */

void APP_TransmitIndicate(HANDLE                  p_hHandle,
                          uint8                   p_ucPriority,
                          SHORT_ADDR              p_unSrcAddr,
                          uint8                   p_ucSequenceNumber,
                          uint8                   p_ucTransportType,
                          uint8                   p_ucApduDataLen,
                          const void *            p_pApduData)
{
  // inc Cmd779 counter
  g_stC779.m_unNoOfPacketsTerminatedByDevice++;

 
//TODO: received Device Status and Extended Device Status bytes should be considered here
// or passed to the ResponseAnalyzer function, together with the RC???

  CommandDirection ucDirection;
  switch(p_ucTransportType)
  {
  // CHECKME
    case BLOCK_DATA_TRANSFER_REQ:
    case REQ_UNICAST:
    case SEARCH_BROADCAST:
    case REQ_BROADCAST:
      ucDirection = CD_REQUEST;   // received a request message from a master
      break;
      
  // CHECKME
    case BLOCK_DATA_TRANSFER_RESP:
    case RESP_UNICAST:
    case PUBLISH_BROADCAST:
    case RESP_BROADCAST:
    case PUBLISH_NOTIFY:
      ucDirection = CD_RESPONSE;  // received an un-solicited (published) response message
      break;
      
    default:
      return; // this should never happen
  }

 
  BinaryStream stInputStream;
  STREAM_INIT(&stInputStream, p_pApduData, p_ucApduDataLen);
  uint8 aOutputBuffer[MAX_APDU_SIZE];  
 
  BinaryStream stOutputStream;
  STREAM_INIT(&stOutputStream, aOutputBuffer, sizeof(aOutputBuffer));
  uint16 unRespCode = APP_ProcessCmdBuf( 0xff,
                                         0,
                                         WIRELESS_CMD,
                                         ucDirection,
                                         p_unSrcAddr,
                                         &stInputStream,
                                         &stOutputStream
                                       );

  if((RCS_N256_ReturnWithDR == unRespCode)
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )   
      || (RCS_N258_ReturnToAppQueue == unRespCode)
#endif    
     )
  {
    
      APP_QUEUE_ELEMENT* pQueue = APP_getQueueElement();
      //add queue element
      if(NULL != pQueue) //this was actually checked inside APP_ProcessCmdBuf. else should never happen!
      {
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
        pQueue->m_ucStatus = AQE_PENDING; 
#else        
        pQueue->m_ucStatus = AQE_USED;
#endif        
        //g_aAPPQueue[ucIdx].m_ucPriority = p_ucPriority;
        
        pQueue->m_hHandle = p_hHandle;
        pQueue->m_unSrcAddr = p_unSrcAddr;
        pQueue->m_ucDirection = ucDirection;
        pQueue->m_ucSequenceNumber = p_ucSequenceNumber;
        pQueue->m_ucTransportType = p_ucTransportType;
        pQueue->m_ucReqSize = p_ucApduDataLen;
        pQueue->m_ucReqCrtPos = stInputStream.nextByte - (uint8*)p_pApduData;
        pQueue->m_ucRspCrtPos = stOutputStream.nextByte - aOutputBuffer;
        pQueue->m_ulTimeout = RTC_COUNT;
        memcpy(pQueue->m_aReqBuffer, p_pApduData,  p_ucApduDataLen);
        memcpy(pQueue->m_aRspBuffer, aOutputBuffer, pQueue->m_ucRspCrtPos);
      }
  
      return;
  }

  // TBD
  switch(p_ucTransportType)
  {

// CD_REQUEST cases    
    case BLOCK_DATA_TRANSFER_REQ:
      // no TransmitRsp ? TBD
      break;
      
    case SEARCH_BROADCAST:
    case REQ_BROADCAST:
    case REQ_UNICAST:
      if(unRespCode == RCS_N255_MakeNoResponse)
      {
        TL_TransmitRsp(p_hHandle, 0, aOutputBuffer, p_ucSequenceNumber, 0/*dummy service ID*/);
        break;
      }
      if(g_ucMarkDisconnectHandle)
        g_unDiscPktHandle = p_hHandle;
 
      unRespCode = TL_TransmitRsp(p_hHandle, 
                                  (stOutputStream.nextByte - aOutputBuffer), 
                                  aOutputBuffer, 
                                  p_ucSequenceNumber, 
                                  0/*dummy service ID*/);
        
      if(SUCCESS == unRespCode)
      {
        APP_ResetDeviceStatusFlag(p_unSrcAddr, WIRELESS_CMD, COLD_START);
      }
      else
      { //TODO: manage the possible error conditions returned by TL, NL, DLL or UART_LINK
      }
      g_ucMarkDisconnectHandle = 0;
      break;

      
// CD_RESPONSE cases    
    case BLOCK_DATA_TRANSFER_RESP:
    case RESP_UNICAST:
    case PUBLISH_BROADCAST:
    case RESP_BROADCAST:
    case PUBLISH_NOTIFY:
      // no TransmitRsp
      break;
  } // end switch(p_ucTransportType)
  
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_TransmitConfirm
// Author: NIVIS LLC
// Description: It is invoked by Transport Layer to notify the client about a received packet which is
//                the result of a TransmitRequest.
// Parameters:
//                p_hHandle  - packet handler
//                p_ucLocalStatus - status of the transaction
//                p_ucApduDataLen - APDU data length
//                p_pApduData - APDU data buffer
// Return: None
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
/*
void APP_TransmitConfirm(HANDLE             p_hHandle,
                         uint8              p_ucLocalStatus,
                         uint8              p_ucApduDataLen,
                         const void*        p_pApduData)
{
  // TODO: implement standard TransmitConfirm for JD
  
  if( TL_SUCCESS == p_ucLocalStatus )
  {  
  
#define me ((uint8*)p_pApduData)  
  
    uint16 unCmdId  = *me >> 8;
           unCmdId += *(me + 1);       
           
    uint8 ucRC = *(me + 3),
          ucServiceId = *(me + 4), 
          ucIdx;
    
    NET_SERVICE_ATTRIBUTES* p = g_aServicesTable;
    
    switch( unCmdId )
    {
      case 770: // parse Cmd770 response, TBD  
                break;
          
      case 799: // parse Cmd799 response 
                ucIdx = FindServiceIndex( ucServiceId, NL_SEARCH_BY_SERVICE_ID ); 
                if( ucIdx == NET_INVALID_IDX )
                { // add service
                  p += g_ucServicesNo;
                  g_ucServicesNo ++;
                }
                else
                { // overide service
                  p += ucIdx;
                } 
                
                p->m_ucServiceID = ucServiceId;
                p->m_ucServiceFlags = *(me + 5);
                p->m_ucApplicationDomain = *(me + 6);
                p->m_unPeerNickname = *(uint16*)(me + 7);
                p->m_tPeriod.u32 = *(uint32*)(me + 9);  
                p->m_ucRouteID = *(uint16*)(me + 13);
                
                break;
                     
      case 801: // parse Cmd801 response 
                ucIdx = FindServiceIndex( ucServiceId, NL_SEARCH_BY_SERVICE_ID );
                p += ucIdx;
                if( ucIdx < (g_ucServicesNo - 1) ) // not the last element
                  memmove( p, p + 1, sizeof(NET_SERVICE_ATTRIBUTES)*(g_ucServicesNo - ucIdx) );  
        
                g_ucServicesNo --; 
                break;           
    };
    
    if( !ucRC && ((unCmdId == 770) || (unCmdId == 799) || (unCmdId == 801)) )
    { // master reques t confirmed with SUCCESS -> clear the request from the master queue 
      APP_SetConfirmStatusInMasterQueue(unCmdId);
    }
    
    // inc Cmd779 counter
    g_stC779.m_unNoOfPacketsTerminatedByDevice++;
    
#undef me    
    
  }  
}
*/

  
                
                
void APP_TransmitConfirm(HANDLE             p_hHandle,
                         uint8              p_ucLocalStatus,
                         uint8              p_ucApduDataLen,
                         const void*        p_pApduData)
{
  // inc Cmd779 counter
  g_stC779.m_unNoOfPacketsTerminatedByDevice++;    

  if((p_hHandle < REQNM_HANDLE_BASE) || (p_hHandle >= (REQNM_HANDLE_BASE + REQNM_QUEUE_SIZE)))
     return;
      
  uint16 unCmdId;
  BinaryStream stStream;
  uint8 ucRetFlag = 0;      // =1 decrement RetryCnt before return, =2 don't (keep retrying)
  uint8 ucByteCnt;
  uint8 ucRC;
  REQNM_QEUE_T* pReqNMQueue = g_aReqNMQueue + (uint8)p_hHandle; // truncation of handle (p_hHAndle == 0x7000 + index)
  DR_SLAVE_BUFFER_STRUCT* pDRBuf = pReqNMQueue->m_pDRBuf;
#ifdef DRM_REDESIGN  
  if(REQNM_SUCCESS == pReqNMQueue->m_ucStatus) //actually the srvc req/del initiator was canceled!
  {
    if(NULL != pDRBuf)
    {
      //did this elsewhere
      /*
      if(DRSLAVE_START == pDRBuf->m_ucDRStatus)
        pDRBuf->m_ucDRStatus = DRSLAVE_CONFLICT;  ////very rare posibility; might happen only in a 2-masters scenario; actually this forces a possible pending app queue to clean itself in it's next task pass
      else
      {
        pDRBuf->m_ucDRStatus = DRSLAVE_END; 
        
        BinaryStream stStreamDR;
        STREAM_INIT(&stStreamDR, pDRBuf->m_aResp, sizeof(pDRBuf->m_aResp));    
        //compose DR response
        ComposeResponseToCommmand(pDRBuf->m_unSrcAddr,                 
                                  NULL, 
                                  pDRBuf->m_unCmdId, 
                                  NULL, 
                                  RCS_E33_DelayedResponseInitiated,
                                  &stStreamDR
                                 );    
        pDRBuf->m_ucRespSize = stStreamDR.nextByte - pDRBuf->m_aResp;
      }
      */ 
    }
    memset(pReqNMQueue, 0, sizeof(REQNM_QEUE_T));
    return;
  }
#endif  
  
  #if ( _UAP_TYPE == UAP_TYPE_WHART_API ) 
    uint32 burstPeriod=0;
  #else
    C103_WriteBurstPeriod_Resp stRsp103;
    C109_BurstModeControl_Resp stRsp109;
  #endif
   
    void* pRspData = NULL;

    C117_WriteEventNotificationTiming_Resp stRsp117;
    C118_EventNotificationControl_Resp stRsp118;
  
  
  
  //!!! we consider that local status may only be TL_SUCCESS or TL_MAX_RETRIES_EXCEEDED
  // data len must be at least 4 (2 bytes for cmd id, 1 for byte cnt and 1 for RC)
  if((p_ucApduDataLen < 4) || (TL_SUCCESS != p_ucLocalStatus) ) // actually only TL_MAX_RETRIES_EXCEEDED might happen
  {
    ucRetFlag = 1;      // decrement RetryCnt before return
  }
  else
  {
    STREAM_INIT(&stStream, p_pApduData, p_ucApduDataLen);
    STREAM_READ_UINT16(&stStream, &unCmdId);
    STREAM_READ_UINT8(&stStream, &ucByteCnt);
    STREAM_READ_UINT8(&stStream, &ucRC);
  
   
    uint8 ucErrorFlag = IsResponseCodeError(ucRC);
    
    switch( unCmdId )
    {
    case CMDID_C799_RequestService:
      {
        if(ucErrorFlag)
        {
          switch(ucRC)
          {
          case RCS_E05_TooFewDataBytesReceived:
          case RCS_E32_Busy:            
          case RCS_E35_DelayedResponseDead:
          case RCS_E36_DelayedResponseConflict:            
            ucRetFlag = 1;  // retry the request
            break;

          case RCS_E33_DelayedResponseInitiated:
          case RCS_E34_DelayedResponseRunning:
            ucRetFlag = 2;  // retry the request forever
            break;
          case RCS_E04_PassedParameterTooSmall: // update period
          case RCS_E06_DeviceSpecificCommandError:
          case RCS_E16_AccessRestricted:
          case RCM_E65_ServiceRequestDenied:
          case RCM_E66_UnknownServiceFlag:
          case RCM_E67_UnknownApplicationDomain:
          case RCM_E68_UnknownNickname:
          case RCM_E69_InvalidServiceId:
            if(NULL == pDRBuf)
            { // initial request was not from a DR context, no response to be composed
              if(JoinProcessStatusMask_NegotiatingNetworkProperties == 
                 (g_stC769.joinStatus & 
                  (JoinProcessStatusMask_NegotiatingNetworkProperties | JoinProcessStatusMask_NormalOperationCommencing)) )
              { // join succeeded, device is negotiating BW for provisioned Burst/Event 
                //TODO: -signal the ERR response to the Burst task 
                // -set "Capacity Denied" flag
                // at the end, free g_aReqNMQueue 
              }
              else
              { // was a C103/C117 request to increase update period (already responded to)
                // nothing to do here, the service remains as it was
                // at the end, free g_aReqNMQueue 
              }
            }
            else
            { // respond with the appropriate err or warn code to the initial command in DR buf
              
            }
            break;
          } // end switch(ucRC)
          break; // end case of 799
        } // end if(ucErrorFlag)            
        else if(ucByteCnt < (C799_RequestService_RespSize + 1))
        { // not error RC, but too few bytes in NM's response
          ucRetFlag = 1;  // retry
          break;
        }
        
        // so (ucErrorFlag == 0) ... now must parse and analyze the NM's response to C799,
        //  take the appropriate actions, 
        //  and compose a response to the initial C109/118/103/117 request if DR context
        //TODO: rework the following sequence (C799 response)    
        
        uint8 ucValue = 0; 
        C799_RequestService_Resp stRsp;
        Parse_C799_RequestService_Resp(&stRsp, &stStream);
        
        uint8 ucIdxSrvc = FindServiceIndex( stRsp.m_ucServiceId, NL_SEARCH_BY_SERVICE_ID );
        if( ucIdxSrvc == NET_INVALID_IDX )
        { // add service
          //!!! i'm considering that (g_ucServicesNo < MAX_SERVICES_NO) was checked at request time !!!
          ucIdxSrvc = g_ucServicesNo++; 
          g_aServicesTable[ucIdxSrvc].m_ucServiceID = stRsp.m_ucServiceId;
        }
        
        g_aServicesTable[ucIdxSrvc].m_tPeriod = stRsp.m_tPeriod;
        g_aServicesTable[ucIdxSrvc].m_unPeerNickname = stRsp.m_unNicknameOfPeer;
        g_aServicesTable[ucIdxSrvc].m_ucServiceFlags = stRsp.m_ucServiceRequestFlags;
        g_aServicesTable[ucIdxSrvc].m_ucApplicationDomain = stRsp.m_ucServiceApplicationDomain;
        g_aServicesTable[ucIdxSrvc].m_ucRouteID = stRsp.m_ucRouteId;
      #if ( _UAP_TYPE == UAP_TYPE_WHART_API )
        burstPeriod = stRsp.m_tPeriod.u32;
      #endif  
        if(NULL != pDRBuf)
        {
          switch(pDRBuf->m_unCmdId)
          {
       #if ( _UAP_TYPE != UAP_TYPE_WHART_API )   
          case CMDID_C103_WriteBurstPeriod:
            {
              BinaryStream stStreamReq;
              STREAM_INIT(&stStreamReq, pDRBuf->m_aReq, MAX_DR_SLAVE_REQ_LEN);
              C103_WriteBurstPeriod_Req stReq103; 
              uint16 unCmdId; 
              ParserComposerEntry* pCmdEntry;
              ParseWirelessCommmand(CD_REQUEST, &stStreamReq, &stReq103, &unCmdId, &pCmdEntry);
               
              ucValue = stRsp.m_ucServiceId - 1;
              TIME_10MS ulPeriod = stRsp.m_tPeriod.u32 / (32 * 10); // [10ms]
              TIME_10MS ulMaxPeriod = stReq103.maxUpdatePeriod.u32 / (32 * 10); // [10ms]
              
              // apply correction
              APP_DoBurstEventTimerCorrection(&ulPeriod, &ulMaxPeriod); 
              
              g_stBurstModeTableNvData[ucValue].m_ulMinUpdateTime = ulPeriod;
              g_stBurstModeTableNvData[ucValue].m_ulMaxUpdateTime = ulMaxPeriod;
              MinUpdateTimer[ucValue] = ulPeriod;
              MaxUpdateTimer[ucValue] = ulMaxPeriod;
             
              stRsp103.burstMessage = ucValue;
              stRsp103.updatePeriod.u32 = ulPeriod * (32 * 10);
              stRsp103.maxUpdatePeriod.u32 = ulMaxPeriod * (32 * 10); 
              pRspData = &stRsp103;

              if((stReq103.updatePeriod.u32 != stRsp103.updatePeriod.u32) || 
                 (stReq103.maxUpdatePeriod.u32 != stRsp103.maxUpdatePeriod.u32))
              {
                if(stReq103.updatePeriod.u32 < stRsp103.updatePeriod.u32)
                {
                  g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_CapacityDenied;
                }
                ucRC = RCM_W08_UpdateTimesAdjusted;
              }
                           
              SET_CONFIG_CHANGED 
               
              // NVM -> store the individual record into FLASH 
              ucValue = 77 + 5*(stRsp.m_ucServiceId - 1);  
              NVM_WriteIndividualRecord( ucValue, 
                                         (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucValue-1].m_unOffset, 
                                         c_aProvNvmTable[ucValue-1].m_ucSize);
             
              ucValue = 1; // timing correction done
            }
            break;
            
          case CMDID_C109_BurstModeControl:
            {
              stRsp109.burstModeCode = BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer;
              stRsp109.burstMessage = stRsp.m_ucServiceId - 1; 
              pRspData = &stRsp109;
              g_stBurstModeTableNvData[stRsp109.burstMessage].m_ucBurstModeControlCode = BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer;
           
              SET_CONFIG_CHANGED
      
              // NVM -> store the individual record into FLASH
              ucValue = 81 + 5*stRsp109.burstMessage;  
              NVM_WriteIndividualRecord( ucValue, 
                                         (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucValue-1].m_unOffset, 
                                         c_aProvNvmTable[ucValue-1].m_ucSize);
              
              ucValue = 0; // need timing correction
            }
            break;
        #endif    
          case CMDID_C117_WriteEventNotificationTiming:
            {
              BinaryStream stStreamReq;
              STREAM_INIT(&stStreamReq, pDRBuf->m_aReq, MAX_DR_SLAVE_REQ_LEN);
              C117_WriteEventNotificationTiming_Req stReq117; 
              uint16 unCmdId; 
              ParserComposerEntry* pCmdEntry;
              ParseWirelessCommmand(CD_REQUEST, &stStreamReq, &stReq117, &unCmdId, &pCmdEntry);

              ucValue = stRsp.m_ucServiceId - MAX_BURST_NO - 1;
              TIME_10MS ulRetryTime = stRsp.m_tPeriod.u32 / (32 * 10); // [10ms]
              TIME_10MS ulMaxUpdateTime = stReq117.maximumUpdateTime.u32 / (32 * 10); // [10ms]
              TIME_10MS ulDebounceInterval = stReq117.eventDeBounceInterval.u32 / (32 * 10); // [10ms]
              
              // apply correction
              APP_DoBurstEventTimerCorrection(&ulRetryTime, &ulMaxUpdateTime);
              BurstEventCorrection(&ulDebounceInterval);
 
              g_stEventsTableNvData[ucValue].m_ulRetryTime = ulRetryTime;
              g_stEventsTableNvData[ucValue].m_ulMaxUpdateTime = ulMaxUpdateTime;
              g_stEventsTableNvData[ucValue].m_ulDebounceInterval = ulDebounceInterval;
              RetryTimer[ucValue]  = ulRetryTime;
              UpdateTimer[ucValue] = ulMaxUpdateTime;
              
              stRsp117.eventNo = ucValue;
              stRsp117.eventNotificationRetryTime.u32 = ulRetryTime * (32 * 10);
              stRsp117.maximumUpdateTime.u32 = ulMaxUpdateTime * (32 * 10);
              stRsp117.eventDeBounceInterval.u32 = ulDebounceInterval * (32 * 10);
              pRspData = &stRsp117;

              if((stReq117.eventNotificationRetryTime.u32 != stRsp117.eventNotificationRetryTime.u32) ||
                 (stReq117.maximumUpdateTime.u32 != stRsp117.maximumUpdateTime.u32) ||
                 (stReq117.eventDeBounceInterval.u32 != stRsp117.eventDeBounceInterval.u32))  
              {
                if(stReq117.eventNotificationRetryTime.u32 < stRsp117.eventNotificationRetryTime.u32)
                {
                  g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_CapacityDenied;
                }
                ucRC = RCM_W08_UpdatePeriodOrDebounceIntervalAdjusted;
              }
              
              SET_CONFIG_CHANGED

              // NVM -> store the individual record into FLASH 
              ucValue = 108 + 3*(stRsp.m_ucServiceId - MAX_BURST_NO - 1);  
              NVM_WriteIndividualRecord( ucValue, 
                                         (uint8*)g_stEventsTableNvData + c_aProvNvmTable[ucValue-1].m_unOffset, 
                                         c_aProvNvmTable[ucValue-1].m_ucSize);
           
              ucValue = 1; // timing correction done
            }
            break;
            
          case CMDID_C118_EventNotificationControl:
            {
              stRsp118.eventNo = stRsp.m_ucServiceId  - MAX_BURST_NO - 1;
              stRsp118.eventNotificationControlCode = EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer;
              pRspData = &stRsp118;
              g_stEventsTableNvData[stRsp118.eventNo].m_ucEventControlCode = EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer;
            
              SET_CONFIG_CHANGED
      
              // NVM -> store the individual record into FLASH 
              ucValue = 109 + 3*stRsp118.eventNo;  
              NVM_WriteIndividualRecord( ucValue, 
                                         (uint8*)g_stEventsTableNvData + c_aProvNvmTable[ucValue-1].m_unOffset, 
                                         c_aProvNvmTable[ucValue-1].m_ucSize);
              
              ucValue = 0; // need timing correction
            }
            
          default: 
            break;
          }
        }
        else 
        {                   
          if(stRsp.m_tPeriod.u32 > pReqNMQueue->m_stC799Req.m_tPeriod.u32)
          {
            g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_CapacityDenied;
          }
        }
      
        
        // ##DP: when a bandwidth request occurs for an already opened pipe(burst or event) => do timings correction 
           
        if(MAX_BURST_NO >= stRsp.m_ucServiceId) //? ServiceApplicationDomain_Publish == m_ucServiceApplicationDomain ?
        { 
          #if ( _UAP_TYPE != UAP_TYPE_WHART_API )
          g_stBurstModeTable[stRsp.m_ucServiceId - 1].m_ucServiceId = stRsp.m_ucServiceId;
          
          if(!ucValue)
          { // do timings correction
            ucValue = stRsp.m_ucServiceId - 1;
            
            g_stBurstModeTableNvData[ucValue].m_ulMinUpdateTime = g_aServicesTable[ucIdxSrvc].m_tPeriod.u32 / (32 * 10); // [10ms];
            
            APP_DoBurstEventTimerCorrection(&g_stBurstModeTableNvData[ucValue].m_ulMinUpdateTime, 
                                            &g_stBurstModeTableNvData[ucValue].m_ulMaxUpdateTime);
   
            MinUpdateTimer[ucValue] = g_stBurstModeTableNvData[ucValue].m_ulMinUpdateTime;
            MaxUpdateTimer[ucValue] = g_stBurstModeTableNvData[ucValue].m_ulMaxUpdateTime;  
          } 
          #endif 
        }
        else if((MAX_EVENTS_NO + MAX_BURST_NO) >= stRsp.m_ucServiceId) //? ServiceApplicationDomain_Event == m_ucServiceApplicationDomain ?
        { 
          g_stEventsTable[stRsp.m_ucServiceId - MAX_BURST_NO - 1].m_ucServiceId = stRsp.m_ucServiceId;
          
          if(!ucValue)
          { // do timings correction
            ucValue = stRsp.m_ucServiceId - MAX_BURST_NO - 1;
            
            g_stEventsTableNvData[ucValue].m_ulRetryTime = g_aServicesTable[ucIdxSrvc].m_tPeriod.u32 / (32 * 10); // [10ms];
            
            APP_DoBurstEventTimerCorrection(&g_stEventsTableNvData[ucValue].m_ulRetryTime, 
                                            &g_stEventsTableNvData[ucValue].m_ulMaxUpdateTime);
            
            RetryTimer[ucValue]  = g_stEventsTableNvData[ucValue].m_ulRetryTime;
            UpdateTimer[ucValue] = g_stEventsTableNvData[ucValue].m_ulMaxUpdateTime; 
          }  
        }
        else
        {
          //block transfer data mode table, etc
        }
        
        g_stCmd48Data.data.standardizedStatus3 &= ~StandardizedStatus3Mask_BandwidthAllocationPending;
        APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
      }
      break;
      
    case CMDID_C770_RequestActiveAdvertise:
      {
        if(!ucErrorFlag && (ucByteCnt < (C770_RequestActiveAdvertise_RespSize + 1)) )
        {
          ucRetFlag = 1;  // decrement RetryCnt
          break;
        }
        
        if(ucErrorFlag && 
           (ucRC == RCS_E32_Busy ||
            ucRC == RCS_E33_DelayedResponseInitiated ||
              ucRC == RCS_E34_DelayedResponseRunning ) )
        {
          ucRetFlag = 2;  // don't decrement RetryCnt, keep retrying
          break;
        }
        
        // forward the received response as is, don't need to parse and re-compose it
        if(NULL != pDRBuf)
        {
          pDRBuf->m_ucRespSize = (p_ucApduDataLen > sizeof(pDRBuf->m_aResp)) ? sizeof(pDRBuf->m_aResp) : p_ucApduDataLen;
          memcpy(pDRBuf->m_aResp, p_pApduData, pDRBuf->m_ucRespSize);
          pDRBuf->m_ucDRStatus = DRSLAVE_END; 
        }
      
        //free the req queue
        //g_aReqNMQueue[ucIdx].m_ucStatus = REQNM_INACTIVE;
        memset(pReqNMQueue, 0, sizeof(REQNM_QEUE_T));
        return;
      }
      
    case CMDID_C801_DeleteService:
      if(!ucErrorFlag)
      {
        if(ucByteCnt < (C801_DeleteService_RespSize + 1))
        {
          ucRetFlag = 1;
          break;
        }  
        C801_DeleteService_Resp stRsp;
        Parse_C801_DeleteService_Resp(&stRsp, &stStream);
        
        // check out the response params to comply with my request
        if(stRsp.m_ucServiceId != pReqNMQueue->m_stC801Req.m_ucServiceId)
        {
          ucRetFlag = 1;
          break;
        }  
        
        // delete service if exists
        uint8 ucIdxSrvc = FindServiceIndex( stRsp.m_ucServiceId, NL_SEARCH_BY_SERVICE_ID );
        if(NET_INVALID_IDX != ucIdxSrvc)
        {
          g_ucServicesNo --;
          if( ucIdxSrvc < g_ucServicesNo ) // not the last element
          {
            memmove(g_aServicesTable + ucIdxSrvc, 
                    g_aServicesTable + ucIdxSrvc + 1, 
                    sizeof(NET_SERVICE_ATTRIBUTES)*(g_ucServicesNo - ucIdxSrvc) );  
          }
        }
           
        if(MAX_BURST_NO >= stRsp.m_ucServiceId)
        {
          #if ( _UAP_TYPE != UAP_TYPE_WHART_API )
          g_stBurstModeTable[stRsp.m_ucServiceId - 1].m_ucServiceId = 0;
          #endif
        }
        else if((MAX_EVENTS_NO + MAX_BURST_NO) >= stRsp.m_ucServiceId)
        {
          g_stEventsTable[stRsp.m_ucServiceId - MAX_BURST_NO - 1].m_ucServiceId = 0;
        }
        else
        {
          // block transfer data mode table, etc
        }
      }
      // C801 cannot come from a DR context :)
      // if error response code - makes no sense to repeat the C801 request, just free the request queue entry
      break;
      
    default:
      return;
    }
  }
 
  if(ucRetFlag)
  {
    if(ucRetFlag == 2)
    {
          pReqNMQueue->m_ucRetryCnt = MAX_REQNM_RETRY_CNT; // just keep trying
    }
    else
    {
      if(RCS_E35_DelayedResponseDead == ucRC)
      {
        //g_aReqNMQueue[ucIdx].m_ucStatus = REQNM_ABORT;
        pReqNMQueue->m_ucRetryCnt = 0;
        if(NULL != pDRBuf)
        {
          pDRBuf->m_ucDRStatus = DRSLAVE_DEAD;
        }        
      }
      if(RCS_E36_DelayedResponseConflict == ucRC) 
      {
        pReqNMQueue->m_ucRetryCnt = 0;
        if(NULL != pDRBuf)
        {
          pDRBuf->m_ucDRStatus = DRSLAVE_CONFLICT;
        }
      }
      if(pReqNMQueue->m_ucRetryCnt)
       pReqNMQueue->m_ucRetryCnt--;
    }
      
    if(pReqNMQueue->m_ucRetryCnt)
    { // the corresponding ReqNMQueue element must be notified to re-send request
      pReqNMQueue->m_ucStatus = REQNM_SEND;
      pReqNMQueue->m_ulTimestamp = RTC_COUNT;
    }
    else
    { // the corresponding ReqNMQueue element must be notified to abort the request
      pReqNMQueue->m_ucStatus = REQNM_ABORT;
    }
    return;
  }            
  
  if(NULL != pDRBuf)
  {
    pDRBuf->m_ucDRStatus = DRSLAVE_END; 
  #if ( _UAP_TYPE == UAP_TYPE_WHART_API )
  if(NULL == pRspData)  
    memcpy(pDRBuf->m_aResp, (uint8*)&burstPeriod, sizeof(uint32));
  else
#endif
    {
  
      BinaryStream stStreamDR;
      STREAM_INIT(&stStreamDR, pDRBuf->m_aResp, sizeof(pDRBuf->m_aResp));    
      //pDRBuf->m_ucRC = ucRC;
      //compose DR response
      ComposeResponseToCommmand(pDRBuf->m_unSrcAddr,                 
                                pRspData, 
                                pDRBuf->m_unCmdId, 
                                NULL, 
                                ucRC,
                                &stStreamDR
                               );    
      pDRBuf->m_ucRespSize = stStreamDR.nextByte - pDRBuf->m_aResp;
    }
  }
  //free the req queue
//  g_aReqNMQueue[ucIdx].m_ucStatus = REQNM_INACTIVE;
  memset(pReqNMQueue, 0, sizeof(REQNM_QEUE_T));  
}



///////////////////////////////////////////////////////////////////////////////////
// Name: DLL_CheckCmdExecTime
// Author: NIVIS LLC
// Description: called by Datalink Layer to periodically inspect the exec time of
//              any pending commands (961, 963, 965 or 972) and execute them when
//              their execution time matches the current ASN
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void DLL_CheckCmdExecTime()
{
  // TODO: call this function (after testing g_ucPendingCmds)
  static uint8 ucOldState;
  uint8 cmpRes;

  g_sPendingCmdsData.m_bPendingCommands = 0;

  //  C961
  if(g_sPendingCmdsData.m_bC961)
  {
    uint8 aASN[5];
    DLL_GetASN(aASN);
    cmpRes = memcmp(g_sPendingCmdsData.m_sC961_Data.m_tExecutionTime, aASN, 5);
    if(cmpRes > 0)
    { // execution time is still in the future
      g_sPendingCmdsData.m_bPendingCommands = 1;
    }
    else
    { // exec time has arrived

      // execute command 961

      DLL_LM_Request(DLL_SERV_WRITE_NETWORK_KEY, &g_sPendingCmdsData.m_sC961_Data);

      g_sPendingCmdsData.m_bC961 = 0;
    }
  }

  //  C963
  if(g_sPendingCmdsData.m_bC963)
  {
    // g_sPendingCmdsData.m_sC963_Idx contains indices in m_aC963_Data sorted by exec time (descending)
    for(int8 i = g_sPendingCmdsData.m_ucNoOfSessions - 1; i >= 0; i--)
    {
      uint8 ucIdx = g_sPendingCmdsData.m_aC963_Idx[i];
      uint8 aASN[5];
      DLL_GetASN(aASN);
      cmpRes = memcmp(g_sPendingCmdsData.m_aC963_Data[ucIdx].m_tExecutionTime, aASN, 5);
      if(cmpRes > 0)
      { // execution time is in the future
        g_sPendingCmdsData.m_bPendingCommands = 1;
        break;  // stop looping; remaining commands have exec times in the future
      }
      else
      { // exec time has arrived

        // execute command 963

        NET_Request(NL_WRITE_SESSION_KEY, &g_sPendingCmdsData.m_aC963_Data[ucIdx]);

        g_sPendingCmdsData.m_ucNoOfSessions--;

        if(ucIdx < g_sPendingCmdsData.m_ucNoOfSessions)
        {
          memcpy(g_sPendingCmdsData.m_aC963_Data + ucIdx,
                 g_sPendingCmdsData.m_aC963_Data + (ucIdx + 1),
                 (g_sPendingCmdsData.m_ucNoOfSessions - ucIdx) * sizeof(C963_WriteSession_Resp));
        }
      }
    }
    if(g_sPendingCmdsData.m_ucNoOfSessions == 0)
      g_sPendingCmdsData.m_bC963 = 0;
  }

  //  C965
  if(g_sPendingCmdsData.m_bC965)
  {
    // g_sPendingCmdsData.m_sC965_Data is sorted by exec time (descending)
    for(int8 i = g_sPendingCmdsData.m_ucNoOfSuperframes - 1; i >= 0; i--)
    {
      uint8 aASN[5];
      DLL_GetASN(aASN);
      cmpRes = memcmp(g_sPendingCmdsData.m_aC965_Data[i].m_tExecutionTime, aASN, 5);
      if(cmpRes > 0)
      { // execution time is in the future
        g_sPendingCmdsData.m_bPendingCommands = 1;
        break;  // stop evaluating; remaining commands have exec times in the future
      }
      else
      { // exec time has arrived

        // execute command 965

        DLL_LM_Request(DLL_SERV_WRITE_SUPERFRAME, &g_sPendingCmdsData.m_aC965_Data[i]);

        g_sPendingCmdsData.m_ucNoOfSuperframes--;
      }
    }
    if(g_sPendingCmdsData.m_ucNoOfSuperframes == 0)
      g_sPendingCmdsData.m_bC965 = 0;
  }

  //  C972
  if(g_sPendingCmdsData.m_bC972)
  {
    if(g_sPendingCmdsData.m_bWaitingToSuspend)
    { // waiting to suspend
      uint8 aASN[5];
      DLL_GetASN(aASN);
      cmpRes = memcmp(g_sPendingCmdsData.m_sC972_Data.m_tTimeToSuspend, aASN, 5);
      if(cmpRes > 0)
      { // execution time is still in the future
        g_sPendingCmdsData.m_bPendingCommands = 1;
      }
      else
      { // exec time has arrived

        // give suspend command
        ucOldState = g_ucNetState;  // save current state before suspending
        g_ucNetState = NET_STATE_SUSPENDED;

        g_sPendingCmdsData.m_bWaitingToSuspend = 0;
      }
    }
    else
    { // waiting to resume
      uint8 aASN[5];
      DLL_GetASN(aASN);
      cmpRes = memcmp(g_sPendingCmdsData.m_sC972_Data.m_tTimeToResume, aASN, 5);
      if(cmpRes > 0)
      { // execution time is still in the future
        g_sPendingCmdsData.m_bPendingCommands = 1;
      }
      else
      { // exec time has arrived

        // give resume command
        g_ucNetState = ucOldState;

        g_sPendingCmdsData.m_bC972 = 0;
      }
    }
  }
  g_ucPendingCmds = g_sPendingCmdsData.m_bPendingCommands;
}

/*************************** HEALTH REPORTS BEGIN *****************************/

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_InitHealthReports
// Author: NIVIS LLC
// Description: initialisations for HealthReports
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_InitHealthReports( void )
{
  g_ucSendHealthReportStatus = HEALTH_REPORT_FIRST_NPDU;

  g_unHealthReportHandle = 0x8000;
  
  g_ucNextIndex = 0;
  g_ucNextIndexOld = 0;
  g_ucNPDUCnt = 0;
//  g_ucComposeOffset = 0;
  STREAM_INIT(&g_stBuffStream, g_aucBuffer, sizeof(g_aucBuffer));
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_ComposeResponse
// Author: NIVIS LLC
// Description: compose and binarize the response for a certain command;
//              the result is put in g_aucBuffer using g_stBuffStream
// Parameters: p_unCmdId - command id
//             p_pReq    - request
//             p_pResp   - response
// Return: status
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_ComposeResponse( uint16 p_unCmdId, void* p_pReq, void* p_pResp )
{
  ExecuteFunction exec;

  exec = FindRequestExecuter(p_unCmdId);

  // execute Cmd and compose the response
  // -> assuming here that always the response code is 0 or 8, so RCS_N00_Success(=0) is given further
  (*exec)( p_pReq,
           p_pResp,
           DUMMY_UINT16,
           DUMMY_UINT8,
           STREAM_GetRemainingSize(&g_stBuffStream) );

  // Cmd response binarization
  // -> this function returns a response code, ignore it
  ComposeResponseToCommmand( 0xFF,  // wireless-format command
                            p_pResp,
                            p_unCmdId,
                            NULL,
                            RCS_N00_Success,
                            &g_stBuffStream );
  
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_PrepareRequest
// Author: NIVIS LLC
// Description:
// Parameters: p_ucNmbOfNeighbors - remaining neighbors
//             p_ucNeighborLength - length of neighbor properties
// Return: request for a certain command
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
REQ_T APP_PrepareRequest( uint8 p_ucNoNeighbors, uint8 p_ucEntryLength )
{
  REQ_T req;

  req.m_ucIdx = g_ucNextIndex; // start neighbor index

  // -> add 3 bytes: see Cmd780/787 response format
  // -> subtract 4 bytes: 2 for cmdID, 1 for dataLen, 1 for respCode
  if( (3 + p_ucNoNeighbors * p_ucEntryLength) <= (STREAM_GetRemainingSize(&g_stBuffStream) - 4) )
  { // copy all linked/unlinked neighbors
    req.m_ucSize = p_ucNoNeighbors;
  }
  else
  { // copy first x linked/unlinked neighbors
    req.m_ucSize = (STREAM_GetRemainingSize(&g_stBuffStream) - 7) / p_ucEntryLength;

    g_ucNextNPDU = 1; // buffer full, transmit this NPDU
    g_ucNPDUCnt ++;
  }

  g_ucNextIndex += req.m_ucSize; // set the next index

  return req;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_780_787_Concatenation
// Author: NIVIS LLC
// Description: adds the response for Cmd787 to the same NPDU (when the response for Cmd780 completes and there is enough space)
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_780_787_Concatenation( void )
{
  if( !g_ucNextNPDU )
  { // buffer yet not full

    // test if remainingBytes >= 10 (10 = 4 + 6, 4 = CmdHdrSize, 6 = Cmd787PayloadMin)
    if( STREAM_GetRemainingSize(&g_stBuffStream) >= 10 )
    {
      // CMD787: prepare request, execute and compose the response
      
      // don't send CMD787(within a health report) if there are no unlinked neighbors
      if( g_ucDllLkdNeighborsSize < g_ucDllAllNeighborsSize )
      { // there is at least one unlinked neighbor 
        REQ_T req = APP_PrepareRequest(g_ucDllAllNeighborsSize - g_ucDllLkdNeighborsSize, C787_NEIGHBOR);
        APP_ComposeResponse(CMDID_C787_ReportNeighborSignalLevels, &req, &g_aucComposedCommand);
      }  
    }
    else
    {
      g_ucNextNPDU = 1; // buffer full, transmit this NPDU
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_SendHealthReports
// Author: NIVIS LLC
// Description: this function sends periodically HealthReports once the device reaches the quarantine state
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_SendHealthReports( void )
{
  uint8 ucRespCode;
  REQ_T req;

  if( START_HEALTH_REPORTS == g_ucHealthReportState )
  {
    if( g_ucDllAllNeighborsSize >= 3 )
    {
      g_ulHealthReportTimer = 1; // force to send the first health report
    }
  }

  if( g_ucGenerateHealthReport || (--g_ulHealthReportTimer == 0) )
  {
    g_ucHealthReportState = SEND_HEALTH_REPORTS;

    g_ucGenerateHealthReport = 1; // construction of HealthReport has begun

    STREAM_INIT(&g_stBuffStream, g_aucBuffer, sizeof(g_aucBuffer));

    // send health report
    switch( g_ucSendHealthReportStatus )
    {

      case HEALTH_REPORT_FIRST_NPDU:

                       g_ucNextNPDU = 0;

                       // CMD779: prepare request, execute and compose the response
                       APP_ComposeResponse(CMDID_C779_ReportDeviceHealth, NULL, &g_aucComposedCommand);

                       // CMD780: prepare request, execute and compose the response
                       req = APP_PrepareRequest(g_ucDllLkdNeighborsSize, C780_NEIGHBOR);
                       APP_ComposeResponse(CMDID_C780_ReportNeighborHealthList, &req, &g_aucComposedCommand);

                       APP_780_787_Concatenation();

                       break;

      case HEALTH_REPORT_NEXT_NPDU:

                       // send the next NPDU (if exist)
                       if( g_ucNextNPDU )
                       {
                         g_ucNextNPDU = 0;
                         g_ucNextIndexOld = g_ucNextIndex;

                         if( g_ucNextIndex < g_ucDllLkdNeighborsSize )
                         {
                           // CMD780: prepare request, execute and compose the response
                           req = APP_PrepareRequest(g_ucDllLkdNeighborsSize - g_ucNextIndex, C780_NEIGHBOR);
                           APP_ComposeResponse(CMDID_C780_ReportNeighborHealthList, &req, &g_aucComposedCommand);

                           APP_780_787_Concatenation();
                         }
                         else if( g_ucNextIndex < g_ucDllAllNeighborsSize )
                         {
                           // CMD787: prepare request, execute and compose the response
                           req = APP_PrepareRequest(g_ucDllAllNeighborsSize - g_ucNextIndex, C787_NEIGHBOR);
                           APP_ComposeResponse(CMDID_C787_ReportNeighborSignalLevels, &req, &g_aucComposedCommand);
                         }
                       }
                       else
                       { // construction of HealthReport is done
                         g_ucGenerateHealthReport = 0;
                       }

                       break;

    }; // END SWITCH

    if( g_ucGenerateHealthReport )
    {
      // send NPDU
      ucRespCode = TL_TransmitRequest( g_unHealthReportHandle,
                                       NET_ADDR_SHORT_ADDR,
                                       c_aNetManagerAddr,
                                       DLL_PACKET_PRIORITY_DATA,
                                       0,
                                       PUBLISH_NOTIFY,
                                       (g_stBuffStream.nextByte - g_aucBuffer),
                                       g_aucBuffer
                                     );
      
      // continue or restart according to response code
      if(!ucRespCode)
      {
        // send the next NPDU or terminate
        g_ucSendHealthReportStatus = HEALTH_REPORT_NEXT_NPDU;
        
        // update the HR-handle
        g_unHealthReportHandle = (g_unHealthReportHandle == 0x8FFF) ? 0x8000 : (g_unHealthReportHandle + 1);
      }
      else
      {
        if( g_ucSendHealthReportStatus == HEALTH_REPORT_NEXT_NPDU )
        { // not first NPDU -> retry last NPDU
          g_ucNextNPDU = 1;
          g_ucNextIndex = g_ucNextIndexOld;
        }
        else
        { // first NPDU -> restart the HR
          g_ucNextIndex = 0;
          g_ucNextIndexOld = 0;
          g_ucNPDUCnt = 0;
        }
        
        // the HR-packet has not reached the DLL queue -> retry in 1s
        g_ucGenerateHealthReport = 0;
        g_ulHealthReportTimer = 4;         
      }      
    }

    if( !g_ucGenerateHealthReport && (HEALTH_REPORT_NEXT_NPDU == g_ucSendHealthReportStatus) )
    { // the last HealthReport NPDU was successfully transmitted
      
      // initialise the HealthReport timer
      g_ulHealthReportTimer = g_stProv.ulHealthReportTime;
      
      // other inits
      g_ucSendHealthReportStatus = HEALTH_REPORT_FIRST_NPDU; 
      g_ucNextIndex = 0;
      g_ucNextIndexOld = 0;
      g_ucNPDUCnt = 0;
    }
  }
}

/*************************** HEALTH REPORTS END *******************************/

/***************** BURST MODE / EVENT NOTIFICATION BEGIN **********************/

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_InitBurstMode
// Author: NIVIS LLC
// Description: burst mode initialisations
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_InitBurstAndEvent( void )
{ 
#ifdef BURST_PIPE_100MS
  // init the 100ms pipe
  memset((void*)&g_stBurstPipe100ms, 0xFF, sizeof(g_stBurstPipe100ms));
  memset((void*)&g_stEventPipe100ms, 0xFF, sizeof(g_stEventPipe100ms));
#endif  
  // various inits
  g_unBurstAndEventsHandle = 0x9000;
  
  // reset all burst and event tables
  memset(g_stBurstModeTable,  0x00, sizeof(g_stBurstModeTable));
  memset(g_stEventsTable,     0x00, sizeof(g_stEventsTable));
  memset(g_stDeviceVariables, 0x00, sizeof(g_stDeviceVariables));
 
  //memset(g_stBurstModeTableNvData, 0x00, sizeof(g_stBurstModeTableNvData));
  //memset(g_stEventsTableNvData,    0x00, sizeof(g_stEventsTableNvData));
  memset(g_stDeviceVariableNvData, 0x00, sizeof(g_stDeviceVariableNvData));
  
  // reset burst timers
  memset(MinUpdateTimer, 0x00, sizeof(MinUpdateTimer));
  memset(MaxUpdateTimer, 0x00, sizeof(MaxUpdateTimer));
  
  // reset event timers
  memset(RetryTimer,  0x00, sizeof(RetryTimer));
  memset(UpdateTimer, 0x00, sizeof(UpdateTimer));
  
  g_ucBurstIsActive=0;
  
#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )

  #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
  
    //dev var codes
    g_stDeviceVariableNvData[0].m_ucCode = TEMP_CODE;      //temperature
    g_stDeviceVariableNvData[1].m_ucCode = HUMID_CODE;     //humidity
    g_stDeviceVariableNvData[2].m_ucCode = DEW_CODE;       //dew point 
    g_stDeviceVariableNvData[3].m_ucCode = ADUMMY_CODE;    //dummy analog
    
    g_stDeviceVariableNvData[4].m_ucCode = D1DUMMY_CODE;   //dummy digital 1
    g_stDeviceVariableNvData[5].m_ucCode = D2DUMMY_CODE;   //dummy digital 2
    g_stDeviceVariableNvData[6].m_ucCode = D3DUMMY_CODE;   //dummy digital 3
    g_stDeviceVariableNvData[7].m_ucCode = D4DUMMY_CODE;   //dummy digital 4 
    
    //engineering unit codes
    g_stDeviceVariableNvData[0].m_ucUnit = UnitsCodes_DegreesCelsius;
    g_stDeviceVariableNvData[1].m_ucUnit = UnitsCodes_Percent;
    g_stDeviceVariableNvData[2].m_ucUnit = UnitsCodes_DegreesCelsius;
    g_stDeviceVariableNvData[3].m_ucUnit = UnitsCodes_NotUsed;
    
    g_stDeviceVariableNvData[4].m_ucUnit = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[5].m_ucUnit = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[6].m_ucUnit = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[7].m_ucUnit = UnitsCodes_NotUsed;
    
    //dev var classification codes
    g_stDeviceVariableNvData[0].m_ucClassification = DeviceVariableClassificationCodes_Temperature;
    g_stDeviceVariableNvData[1].m_ucClassification = DeviceVariableClassificationCodes_Analytical;
    g_stDeviceVariableNvData[2].m_ucClassification = DeviceVariableClassificationCodes_Temperature;  
    g_stDeviceVariableNvData[3].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
    
    g_stDeviceVariableNvData[4].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[5].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[6].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[7].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
  
  #else // -> BOARD_TYPE_HART_DEV_KIT
  
    //API DV codes
    g_stDeviceVariableNvData[0].m_ucCode  = ANALOG_INPUT;     //4-20mA analog input
    g_stDeviceVariableNvData[1].m_ucCode  = TEMP_CODE;        //temperature
    g_stDeviceVariableNvData[2].m_ucCode  = HUMID_CODE;       //humidity
    g_stDeviceVariableNvData[3].m_ucCode  = DEW_CODE;         //dew point 
    g_stDeviceVariableNvData[4].m_ucCode  = PERCENT_OF_RANGE; //percent of range
    g_stDeviceVariableNvData[5].m_ucCode  = LOOP_CURRENT;     //loop current
    //HART DV codes
    g_aHartDVCodes[0] = DeviceVariableCodes_PrimaryVariable;    //4-20mA analog input
    g_aHartDVCodes[1] = DeviceVariableCodes_SecondaryVariable;  //temperature
    g_aHartDVCodes[2] = DeviceVariableCodes_TertiaryVariable;   //humidity
    g_aHartDVCodes[3] = DeviceVariableCodes_QuaternaryVariable; //dew point 
    g_aHartDVCodes[4] = DeviceVariableCodes_PercentRange;       //percent of range
    g_aHartDVCodes[5] = DeviceVariableCodes_LoopCurrent;        //loop current
    
    g_stDeviceVariableNvData[6].m_ucCode  = USER_DATA1_CODE;   //user data 1
    g_stDeviceVariableNvData[7].m_ucCode  = USER_DATA2_CODE;   //user data 2
    g_stDeviceVariableNvData[8].m_ucCode  = USER_DATA3_CODE;   //user data 3
    g_stDeviceVariableNvData[9].m_ucCode  = USER_DATA4_CODE;   //user data 4 
    g_stDeviceVariableNvData[10].m_ucCode = USER_DATA5_CODE;   //user data 5
    g_stDeviceVariableNvData[11].m_ucCode = USER_DATA6_CODE;   //user data 6
    g_stDeviceVariableNvData[12].m_ucCode = USER_DATA7_CODE;   //user data 7
    g_stDeviceVariableNvData[13].m_ucCode = USER_DATA8_CODE;   //user data 8
    
    //engineering unit codes
    g_stDeviceVariableNvData[0].m_ucUnit  = UnitsCodes_Milliamperes;
    g_stDeviceVariableNvData[1].m_ucUnit  = UnitsCodes_DegreesCelsius;
    g_stDeviceVariableNvData[2].m_ucUnit  = UnitsCodes_Percent;
    g_stDeviceVariableNvData[3].m_ucUnit  = UnitsCodes_DegreesCelsius;
    g_stDeviceVariableNvData[4].m_ucUnit  = UnitsCodes_Percent;
    g_stDeviceVariableNvData[5].m_ucUnit  = UnitsCodes_Milliamperes;
    
    g_stDeviceVariableNvData[6].m_ucUnit  = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[7].m_ucUnit  = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[8].m_ucUnit  = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[9].m_ucUnit  = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[10].m_ucUnit = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[11].m_ucUnit = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[12].m_ucUnit = UnitsCodes_NotUsed;
    g_stDeviceVariableNvData[13].m_ucUnit = UnitsCodes_NotUsed;
    
    //dev var classification codes
    g_stDeviceVariableNvData[0].m_ucClassification =  DeviceVariableClassificationCodes_Current;
    g_stDeviceVariableNvData[1].m_ucClassification =  DeviceVariableClassificationCodes_Temperature;
    g_stDeviceVariableNvData[2].m_ucClassification =  DeviceVariableClassificationCodes_Analytical;  
    g_stDeviceVariableNvData[3].m_ucClassification =  DeviceVariableClassificationCodes_Temperature;
    g_stDeviceVariableNvData[4].m_ucClassification =  DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[5].m_ucClassification =  DeviceVariableClassificationCodes_NotUsed;
    
    g_stDeviceVariableNvData[6].m_ucClassification =  DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[7].m_ucClassification =  DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[8].m_ucClassification =  DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[9].m_ucClassification =  DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[10].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[11].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[12].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
    g_stDeviceVariableNvData[13].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
  
  #endif
  
#else
  memcpy(g_stDeviceVariableNvData, cDevVarInitData, sizeof(g_stDeviceVariableNvData));

  /* we do not support the loop current */
  g_stDeviceVariables[LOOP_CURRENT_IDX].m_fValue = c_stNoValue.m_fValue;  

  
#endif  // _UAP_TYPE == UAP_TYPE_SIMPLE_API
  
  // init for event scan table
  APP_InitEventScanTable();
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_InitEventScanTable
// Author: NIVIS LLC
// Description: performs the initialisation of the event scan history
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_InitEventScanTable( void )
{
  // clear event queue 
  memset(g_stEventQueue, 0x00, sizeof(g_stEventQueue));
  g_pEventQueueEnd = g_stEventQueue;
  
  g_aAllDeviceStatus[0][0] = g_stProv.aDeviceStatus[MASTER_GW_IDX];
  memcpy(&g_aAllDeviceStatus[0][1], &g_stCmd48Data.byte[0], 25);
   
  memcpy(&g_aAllDeviceStatus[1][0], g_aAllDeviceStatus, 26);
  memcpy(&g_aAllDeviceStatus[2][0], g_aAllDeviceStatus, 26);
    
  g_ucLastScanIndex = 0; 
  
  g_ulLastRTC = RTC_COUNT; // save actual time
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_CopyChannel
// Author: NIVIS LLC
// Description: copies the four device variables from SPI buffer into the appl. buffer (Cmd33)
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_CopyChannel( void )
{
  for(uint8 ucIdx=0; ucIdx < NoOfDynamicVariables; ucIdx++)
  { 
    // emulate the process connections (only for device var. 1->4, mapped on PV,SV,TV and QV, the Cmd33 will then publish them)
    if( !g_stDeviceVariables[c_aVarMap[ucIdx]].m_ucNormalOperation )
    {  
      // save the old value
      g_stDeviceVariables[c_aVarMap[ucIdx]].m_fLastValue = g_stDeviceVariables[c_aVarMap[ucIdx]].m_fValue; 
      g_stDeviceVariables[ucIdx].m_fLastValue = g_stDeviceVariables[ucIdx].m_fValue; 
      
      if( g_stDeviceVariables[c_aVarMap[ucIdx]].m_fValue < g_stDeviceVariableNvData[c_aVarMap[ucIdx]].m_fUpperLimit )
      {  
        g_stDeviceVariables[c_aVarMap[ucIdx]].m_fValue += 0.001; // dynamic variable
        g_stDeviceVariables[ucIdx].m_fValue = g_stDeviceVariables[c_aVarMap[ucIdx]].m_fValue; // device variable
      }  
      else
      {
        g_stDeviceVariables[c_aVarMap[ucIdx]].m_fValue = g_stDeviceVariableNvData[c_aVarMap[ucIdx]].m_fLowerLimit; 
        g_stDeviceVariables[ucIdx].m_fValue = g_stDeviceVariables[c_aVarMap[ucIdx]].m_fValue;
      }
    }  
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_ReadChannel
// Author: NIVIS LLC
// Description: reads the actual value of a certain published channel
// Parameters: p_ucCode - device variable code
// Return: ptr. to device variable
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
NV_DEVICE_VARIABLE_T* APP_ReadChannel( uint8 p_ucCode )
{
#if ( _UAP_TYPE != UAP_TYPE_SIMPLE_API )
  // 0 -> PV, 1 -> SV, and so on
  if( p_ucCode < NoOfDynamicVariables )
    return &g_stDeviceVariableNvData[c_aVarMap[p_ucCode]];  
#else
  for(uint8 ucIdx=0; ucIdx<LOCAL_ANALOG_CH_NO; ucIdx++)
  {
    #if (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)
      if (p_ucCode == g_aHartDVCodes[ucIdx])
        return (g_stDeviceVariableNvData + ucIdx);
    #endif
  }
  #if (BOARD_TYPE_HART_DEV_KIT != BOARD_TYPE)
    if ((p_ucCode >= DeviceVariableCodes_PrimaryVariable ) && (p_ucCode <= DeviceVariableCodes_TertiaryVariable ))
        p_ucCode -=  DeviceVariableCodes_PrimaryVariable;       
  #endif
#endif  
    
  NV_DEVICE_VARIABLE_T *pCrtVar = g_stDeviceVariableNvData;
  for(; pCrtVar < (g_stDeviceVariableNvData+sizeof(g_stDeviceVariableNvData)/sizeof(NV_DEVICE_VARIABLE_T)); pCrtVar++)
  {
    if( p_ucCode == pCrtVar->m_ucCode )
      return pCrtVar;
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_SendBurstMsg
// Author: NIVIS LLC
// Description: sends burst message 0 / aggregated burst messages
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_SendBurstMsg(uint8 p_ucServiceId)
{ 
  // send the aggregated burst messages 
  if( g_stBuffStream.nextByte - g_aucBuffer ) 
  {   
    TL_TransmitRequest( g_unBurstAndEventsHandle,
                        NET_ADDR_SHORT_ADDR,
                        c_aGatewayAddr,
                        (p_ucServiceId <= MAX_BURST_NO) ? DLL_PACKET_PRIORITY_DATA : DLL_PACKET_PRIORITY_ALARM,
                        p_ucServiceId,
                        PUBLISH_NOTIFY,
                        (g_stBuffStream.nextByte - g_aucBuffer),
                        g_aucBuffer
                      );
    
    // update handle
    SET_BURST_EVENT_HANDLER(g_unBurstAndEventsHandle);
    //g_unBurstAndEventsHandle = (g_unBurstAndEventsHandle == 0x9FFF) ? 0x9000 : (g_unBurstAndEventsHandle + 1);
    
    // clear offset
//    g_ucComposeOffset = 0;
    STREAM_INIT(&g_stBuffStream, g_aucBuffer, sizeof(g_aucBuffer));
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_PrepareBurstMsg
// Author: NIVIS LLC
// Description: composing a burst message
// Parameters: pBurstMsg - current burst message to be sent
//             ucDVIdx - Device Variable(Slot 0) index 
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_PrepareBurstMsg( NV_BURST_MODE_T *pBurstMsg, uint8 ucDVIdx )
{
  uint8 aucReq[MAX_REQ_BYTES],
        *pReq = aucReq;
#ifndef BACKBONE_SUPPORT // just to avoid compiler errors
  uint8 ucIdx;
#endif

  if( !g_ucBurstIsActive )
  { // flag for the first message
    g_ucBurstIsActive = 1;
  }

  if( CMDID_C033_ReadDeviceVariables == pBurstMsg->m_unCmdNo )
  {
    // build Cmd33 request
#ifndef BACKBONE_SUPPORT // just to avoid compiler errors
    for(ucIdx=0; ucIdx<4; ucIdx++)
    {
      if(DeviceVariableCodes_NotUsed == pBurstMsg->m_aucDeviceVariableCodes[ucIdx])
        break;
      ((C033_ReadDeviceVariables_Req *)pReq)->variables[ucIdx] = pBurstMsg->m_aucDeviceVariableCodes[ucIdx];
    }
    ((C033_ReadDeviceVariables_Req *)pReq)->variablesSize = ucIdx;
#endif
  }
  else if( CMDID_C009_ReadDeviceVariablesWithStatus == pBurstMsg->m_unCmdNo )
  {
    // build Cmd9 request
    
#ifndef BACKBONE_SUPPORT // just to avoid compiler errors
    for(ucIdx=0; ucIdx<8; ucIdx++)
    {
      if(DeviceVariableCodes_NotUsed == pBurstMsg->m_aucDeviceVariableCodes[ucIdx])
        break;      
      ((C009_ReadDeviceVariablesWithStatus_Req *)pReq)->slotDVC[ucIdx] = pBurstMsg->m_aucDeviceVariableCodes[ucIdx];
    }
    ((C009_ReadDeviceVariablesWithStatus_Req *)pReq)->variablesSize = ucIdx;
#endif
  }

#ifdef PUBLISH_CMD784  
  if( CMDID_C784_ReadLinkList == pBurstMsg->m_unCmdNo )
  { 
    ((C784_ReadLinkList_Req *)pReq)->m_unLinkIndex = unLnkIdx;
    ((C784_ReadLinkList_Req *)pReq)->m_ucNoOfLinksToRead = 0xFF;
    
    if( g_ucDllAllLinksSize - unLnkIdx > 10 )
      unLnkIdx += 10;  
    else
      unLnkIdx = 0; 
  }  
#endif
  
  // tbd: prepare the request (depending on command)
  APP_ComposeResponse(pBurstMsg->m_unCmdNo, pReq, &g_aucComposedCommand);

  //if( pBurstMsg == g_stBurstModeTableNvData )
  //{ // Burst message 0 must never be aggregated
    APP_SendBurstMsg(g_stBurstModeTable[pBurstMsg - g_stBurstModeTableNvData].m_ucServiceId);
  //} 
  
  // save the published value
  g_stDeviceVariables[ucDVIdx].m_fLastValue = g_stDeviceVariables[ucDVIdx].m_fValue;
  
  // re-init both timers
  MaxUpdateTimer[pBurstMsg - g_stBurstModeTableNvData] = pBurstMsg->m_ulMaxUpdateTime;
  MinUpdateTimer[pBurstMsg - g_stBurstModeTableNvData] = pBurstMsg->m_ulMinUpdateTime;
}

void Execute100msPipe(void)
{
#ifdef BURST_PIPE_100MS  
  
  if( g_stBurstPipe100ms.m_ucIdx < MAX_BURST_NO )
  {
    NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData + g_stBurstPipe100ms.m_ucIdx; 
    if( pCrtBurst->m_ucBurstModeControlCode == BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer )
    { // 100ms pipe active
    
      if( g_stBurstPipe100ms.m_ucSlotsUntilBurst == (RTC_COUNT - g_stBurstPipe100ms.m_ulLastRTCCount)/(32*10) )
      { // time to burst
        NV_DEVICE_VARIABLE_T *p = APP_ReadChannel( pCrtBurst->m_aucDeviceVariableCodes[0] ); // assume that p != NULL
        uint8 ucIdx = p - g_stDeviceVariableNvData; // device variable index
        float fValue = g_stDeviceVariables[ucIdx].m_fValue,
              fLastValue = g_stDeviceVariables[ucIdx].m_fLastValue;
  
        if( BurstMessageTriggerMode_Window == pCrtBurst->m_ucBurstTriggerModeCode )
        { // implement ABS function if continuous trigger mode
          fValue = (fValue > fLastValue) ? (fValue - fLastValue) : (fLastValue - fValue);
          
          //condition not fulfilled -> discard burst
          if( fValue < pCrtBurst->m_fTriggerValue )
            return;
        }
  
        if(  (pCrtBurst->m_ucBurstTriggerModeCode == BurstMessageTriggerMode_Continuous) // continuous
           ||(pCrtBurst->m_ucBurstTriggerModeCode == BurstMessageTriggerMode_Window)     // windowed
           ||(fValue != fLastValue) )                                                    // others, condition is fulfilled
        { // send burst
          
          APP_PrepareBurstMsg(pCrtBurst, ucIdx);
  
          // save the published value
          g_stDeviceVariables[ucIdx].m_fLastValue = fValue;
  
          g_stBurstPipe100ms.m_ulLastRTCCount = RTC_COUNT;
          
          g_stBurstPipe100ms.m_ucSlotsUntilBurst = 10;
          
          // re-init both timers
          MaxUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] = pCrtBurst->m_ulMaxUpdateTime;
          MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] = pCrtBurst->m_ulMinUpdateTime;
          
          // send the burst message
          APP_SendBurstMsg(g_stBurstModeTable[pCrtBurst - g_stBurstModeTableNvData].m_ucServiceId);
        } 
      }  
    }  
  } 
  
  if( g_stEventPipe100ms.m_ucIdx < MAX_EVENTS_NO )
  {
    NV_EVENT_NOTIFICATION_T *pCrtEvent = g_stEventsTableNvData + g_stEventPipe100ms.m_ucIdx; 
    if ( pCrtEvent->m_ucEventControlCode == EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer )
    { // event pipe is active 
      if(  (g_stEventPipe100ms.m_ucIdx == g_stEventQueue[0].m_ucEventNmb)
         &&(g_stEventPipe100ms.m_ucSlotsUntilBurst == (RTC_COUNT - g_stEventPipe100ms.m_ulLastRTCCount)/(32*10)) )
      { 
        // send the event
        APP_ComposeResponse(119, (C119_AcknowledgeEventNotification_Req*)g_stEventQueue, &g_aucComposedCommand);
              
        g_stEventPipe100ms.m_ulLastRTCCount = RTC_COUNT;
          
        g_stEventPipe100ms.m_ucSlotsUntilBurst = 10;
        
        // re-init timers
        RetryTimer[pCrtEvent - g_stEventsTableNvData] = pCrtEvent->m_ulRetryTime;
        UpdateTimer[pCrtEvent - g_stEventsTableNvData] = pCrtEvent->m_ulMaxUpdateTime;
      } 
    }
  }

#endif  
}  

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_BurstModeAndEventNotificationTask
// Author: NIVIS LLC
// Description: handling of burst messages / events
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_BurstModeAndEventNotificationTask(void)
{
  // handling of burst pipes  
  if(( NET_STATE_OPERATIONAL == g_ucNetState ) && 
     (g_stC769.joinStatus & JoinProcessStatusMask_NormalOperationCommencing))
  {
//    g_ucComposeOffset = 0; // clear offset
    STREAM_INIT(&g_stBuffStream, g_aucBuffer, sizeof(g_aucBuffer));
    
    NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData;
    for(; pCrtBurst < g_stBurstModeTableNvData + MAX_BURST_NO; pCrtBurst++)
    { // parse the burst table
      if (  (pCrtBurst->m_ucBurstModeControlCode == BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer) 
          && g_stBurstModeTable[pCrtBurst - g_stBurstModeTableNvData].m_ucServiceId )
      {
        if( MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] )
        { // decrement the timer if not elapsed
          MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] --;
        }
        else
        { // time to burst
          if( pCrtBurst->m_ucBurstTriggerModeCode == BurstMessageTriggerMode_Continuous )
          { // minimum update period expired -> publish the burst message if continuous trigger mode
            
            // send the burst
            APP_PrepareBurstMsg(pCrtBurst, 0);
  
            // set the burst flag(NIVIS intern)
            if( !g_stBurstModeTable[pCrtBurst - g_stBurstModeTableNvData].m_ucFirstBurstSent )
              g_stBurstModeTable[pCrtBurst - g_stBurstModeTableNvData].m_ucFirstBurstSent = 1;
          }
        }
        
        if( pCrtBurst->m_ucBurstTriggerModeCode != BurstMessageTriggerMode_Continuous )
        {
          NV_DEVICE_VARIABLE_T *p = APP_ReadChannel( pCrtBurst->m_aucDeviceVariableCodes[0] ); // assume that p != NULL
          uint8 ucIdx = p - g_stDeviceVariableNvData; // device variable index
          float fValue = g_stDeviceVariables[ucIdx].m_fValue,
                fLastValue = g_stDeviceVariables[ucIdx].m_fLastValue;
          
          // set the burst flag(NIVIS intern)
          if( !g_stBurstModeTable[pCrtBurst - g_stBurstModeTableNvData].m_ucFirstBurstSent )
          {
            // send the burst
            APP_PrepareBurstMsg(pCrtBurst, ucIdx);
            
            g_stBurstModeTable[pCrtBurst - g_stBurstModeTableNvData].m_ucFirstBurstSent = 1;
          }
          else
          {
            switch( pCrtBurst->m_ucBurstTriggerModeCode )
            {
              
              case BurstMessageTriggerMode_Window: 
                     // | value - last value | >= trigger level -> send burst                 
                     fValue = (fValue > fLastValue) ? (fValue - fLastValue) : (fLastValue - fValue);  
                     if( (fValue >= pCrtBurst->m_fTriggerValue) && !MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] )
                     {
                       // send the burst
                       APP_PrepareBurstMsg(pCrtBurst, ucIdx); 
                     }  
                     break;
            
                    
              case BurstMessageTriggerMode_Rising:  
                     // value > trigger level -> send burst 
                     if( (fValue > pCrtBurst->m_fTriggerValue) && !MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] )
                     {
                       // send the burst
                       APP_PrepareBurstMsg(pCrtBurst, ucIdx); 
                     }          
                     break;            
            
                   
              case BurstMessageTriggerMode_Falling:  
                     // value < trigger level -> send burst 
                     if( (fValue < pCrtBurst->m_fTriggerValue) && !MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] )
                     {
                       // send the burst
                       APP_PrepareBurstMsg(pCrtBurst, ucIdx); 
                     }
                     break;            


              case BurstMessageTriggerMode_OnChange:  
                     // value != last value -> send burst
                     if( (fValue != fLastValue) && !MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] )
                     {
                       // send the burst
                       APP_PrepareBurstMsg(pCrtBurst, ucIdx); 
                     }
                     break;                                                 
            
            };
          }
          
          if( MaxUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] )
          { // decrement the timer if not elapsed
            MaxUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] --;
          }
          
          else
          { // timeout MaxUpdateTimer -> send a burst message
            NV_DEVICE_VARIABLE_T *p = APP_ReadChannel( pCrtBurst->m_aucDeviceVariableCodes[0] );
  
            APP_PrepareBurstMsg(pCrtBurst, p - g_stDeviceVariableNvData);
          }
        }
        
      }
    } 
 
    // handling of event pipes
    if( EVENT_QUEUE_NOT_EMPTY )
    { 
      EVENT_PENDING_T* pEvent = g_stEventQueue; // this event will be published
            
      if( RetryTimer[pEvent->m_ucEventNmb] )
      {
        RetryTimer[pEvent->m_ucEventNmb] --; 
      }      
         
      if( !RetryTimer[pEvent->m_ucEventNmb] )
      {// publish the Cmd119 response, until the ACK(Cmd119 request) is received
       
        if( pEvent->m_ucAckReceived ) 
        { // acknowlegde received 
          
          // compact the event queue(memmove has no effect if there is only one event on queue)
          memmove(pEvent, pEvent+1, (g_pEventQueueEnd - g_stEventQueue - 1)*sizeof(EVENT_PENDING_T)); 
          g_pEventQueueEnd --;
          
          // empty queue -> re-init the pipe UpdateTimer 
          if( !EVENT_QUEUE_NOT_EMPTY )
          {  
            UpdateTimer[pEvent->m_ucEventNmb] =   g_stEventsTableNvData[pEvent->m_ucEventNmb].m_ulMaxUpdateTime 
                                                - g_stEventsTableNvData[pEvent->m_ucEventNmb].m_ulRetryTime;
          }  
        }
        else
        {  
          // compose the Cmd119 response
          APP_ComposeResponse(119, (C119_AcknowledgeEventNotification_Req*)g_stEventQueue, &g_aucComposedCommand);
              
          // send the event
          APP_SendBurstMsg(g_stEventsTable[pEvent->m_ucEventNmb].m_ucServiceId);
          
          // re-init timers
          RetryTimer[pEvent->m_ucEventNmb] = g_stEventsTableNvData[pEvent->m_ucEventNmb].m_ulRetryTime;  
        }   
      }         
    }      
    else
    {
      NV_EVENT_NOTIFICATION_T *pCrtEvent = g_stEventsTableNvData;
      for(; pCrtEvent < g_stEventsTableNvData + MAX_EVENTS_NO; pCrtEvent++)
      { // parse the events table
        if (  ( pCrtEvent->m_ucEventControlCode == EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer )
            && g_stEventsTable[pCrtEvent - g_stEventsTableNvData].m_ucServiceId )
        { // event pipe is active 
          
          uint8 ucCrtEvent = pCrtEvent - g_stEventsTableNvData;
          C119_AcknowledgeEventNotification_Req C119_ReqBuff;
          
          if( !UpdateTimer[ucCrtEvent] )
          { // send the event
             
            // compose the Cmd119 request 
            C119_ReqBuff.eventNo = ucCrtEvent;
            C119_ReqBuff.firstUnacknowledged.u32 = 0xFFFFFFFF; // event not triggered -> timestamp = -1
            C119_ReqBuff.configurationChangedCounter = g_stProv.unConfigChangeCounter;
            C119_ReqBuff.deviceStatus = g_stProv.aDeviceStatus[MASTER_GW_IDX];
            memcpy(C119_ReqBuff.command48Data, &g_stCmd48Data.byte[0], MaxLengthOf48Command); 
  
            // compose the Cmd119 response
            APP_ComposeResponse(119, (void*)&C119_ReqBuff, &g_aucComposedCommand);
              
            // send the event
            APP_SendBurstMsg(g_stEventsTable[ucCrtEvent].m_ucServiceId);
            
            // re-init timers
            UpdateTimer[ucCrtEvent] = pCrtEvent->m_ulMaxUpdateTime;
          }
          else
          {             
            UpdateTimer[ucCrtEvent] --;
          }       
        }
      }
    }  

    // TBD: send the aggregated burst messages; the problem is: on which service ??? 
    //APP_SendBurstMsg(pCrtBurst);
  }
}


///////////////////////////////////////////////////////////////////////////////////
// Name: APP_TriggerEvents
// Author: NIVIS LLC
// Description: trigger events on all event pipes
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_TriggerEvents(void)  
{   
  uint8 ucT0, ucT1, ucT2, ucIdx, aCrtScan[26];
  TIME_DATE_T stCrtTimeDate;
  
  
  NV_EVENT_NOTIFICATION_T* pCrtEvent = g_stEventsTableNvData;
  for(; pCrtEvent < g_stEventsTableNvData + MAX_EVENTS_NO; pCrtEvent++)
  { // parse the events table
    if (  ( pCrtEvent->m_ucEventControlCode == EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer )
        && g_stEventsTable[pCrtEvent - g_stEventsTableNvData].m_ucServiceId )
    { // event pipe is active   
     
      if( RTC_COUNT - g_ulLastRTC >= pCrtEvent->m_ulDebounceInterval*10*16 ) // RTC - LastRTC >= Tdebounce/2 ?
      { // new scan
          
        g_ulLastRTC = RTC_COUNT; // save RTC_COUNT 
        
        // save current scan
        aCrtScan[0] = g_stProv.aDeviceStatus[MASTER_GW_IDX];
        memcpy(aCrtScan + 1, &g_stCmd48Data.byte[0], 25);
          
        // get the scan index within the scan history
        ucT0 = c_aScanIndexTable[g_ucLastScanIndex][0];
        ucT1 = c_aScanIndexTable[g_ucLastScanIndex][1];
        ucT2 = c_aScanIndexTable[g_ucLastScanIndex][2];  
          
        for(ucIdx = 0; ucIdx < pCrtEvent->m_ucMaskSize; ucIdx++)
        {
          if( (g_aAllDeviceStatus[ucT0][ucIdx] ^ g_aAllDeviceStatus[ucT1][ucIdx])
             &(g_aAllDeviceStatus[ucT0][ucIdx] ^ g_aAllDeviceStatus[ucT2][ucIdx])
             &(g_aAllDeviceStatus[ucT0][ucIdx] ^ aCrtScan[ucIdx])  
             & pCrtEvent->m_aucEventMasks[ucIdx] ) 
          { // event triggered, store it
              
            // event queue full -> override the last event
            if( EVENT_QUEUE_IS_FULL )
              g_pEventQueueEnd --;  
              
            g_pEventQueueEnd->m_ucEventNmb = pCrtEvent - g_stEventsTableNvData;   
            g_pEventQueueEnd->m_ucCCCounter = g_aCCCounterHistory[ucT1];
            memcpy(g_pEventQueueEnd->m_aData, aCrtScan, 26); 
  
            // calculate the current time and date
            APP_TimeToDate(g_sDllASN.m_ullASN, &stCrtTimeDate);
 
            // event timestamp = time within the present day
            g_pEventQueueEnd->m_ulTimestamp = stCrtTimeDate.m_ulTimeOfDay; 
            
            // reset the ACK flag     
            g_pEventQueueEnd->m_ucAckReceived = 0;
            
            // save event status
            g_pEventQueueEnd->m_ucEventStatus = 0;
            if( !ucIdx )
            { // device status byte triggered
              if( (aCrtScan[0] & CONFIG_CHANGED) ^ (g_aAllDeviceStatus[ucT0][0] & CONFIG_CHANGED) )
                g_pEventQueueEnd->m_ucEventStatus |= EventStatusCodesMask_ConfigurationChangedEventPending; 
                
              if( (aCrtScan[0] & MORE_STATUS_AVAILABLE) ^ (g_aAllDeviceStatus[ucT0][0] & MORE_STATUS_AVAILABLE) )
                g_pEventQueueEnd->m_ucEventStatus |= EventStatusCodesMask_MoreStatusAvailableEventPending;
            }
            else
            { // Cmd48 data triggered
              g_pEventQueueEnd->m_ucEventStatus = EventStatusCodesMask_DeviceStatusEventPending; 
            }  
              
            // update queue size
            g_pEventQueueEnd ++;
          }  
        }  
          
        // save the current scan
        memcpy(&g_aAllDeviceStatus[ucT0][0], aCrtScan, 26);
          
        // save the CC-Counter
        g_aCCCounterHistory[ucT0] = g_stProv.unConfigChangeCounter;;
         
        // index of current scan
        g_ucLastScanIndex = ucT0;
          
      }  
    }
    else if(  (  (pCrtEvent->m_ucEventControlCode == EventNotificationControlCodes_Off)  // event off
               || !g_stEventsTable[pCrtEvent - g_stEventsTableNvData].m_ucServiceId)     // no service
            && EVENT_QUEUE_NOT_EMPTY  )
    {
      // clear event queue 
      memset(g_stEventQueue, 0x00, sizeof(g_stEventQueue));
      g_pEventQueueEnd = g_stEventQueue;  
    }  
  }        
}  

/***************** BURST MODE / EVENT NOTIFICATION END ************************/

///////////////////////////////////////////////////////////////////////////////////
// Name: APP_AddToAlarmBuffer
// Author: NIVIS LLC, Doinel Alban
// Description: adds an aggregated alarm report to the global alarm commands buffer
// Parameters:
//  @ucAlarmCode: LSB of the Alarm Command number (APP_ALARM_C788, APP_ALARM_C789, APP_ALARM_C790, APP_ALARM_C791)
//  @unDestId:    Nickname / GraphId
//  @p_aNetMIC:   NL MIC of the failed packet, only for Cmd789
// Return: none
// Obs:
//      Access level: user level & interrupt level
///////////////////////////////////////////////////////////////////////////////////
void APP_AddToAlarmBuffer(uint8 ucAlarmCode, uint16 unDestId, const uint8* p_aNetMIC)
{
  if((g_ucNetState == NET_STATE_OPERATIONAL) || (g_ucNetState == NET_STATE_QUARANTINED))
  {
    uint8 ucUsedBytes = 6;    //  6 = 2Cmd+1Len+1RespCode+2Data
    
    if(ucAlarmCode == APP_ALARM_C789)
      ucUsedBytes = 10;       // 10 = 2Cmd+1Len+1RespCode+2Data+4MIC
    
    MONITOR_ENTER();     // the function could be called from DLL interrupts, too
    
    if(g_ucAlarmBuffLen <= APP_MAX_ALARM_BUFF_LEN - ucUsedBytes)
    { // implemented for Little Endian only!
      uint8*  p = g_aAlarmBuff + g_ucAlarmBuffLen;
      *(p++) = APP_ALARM_MSB;                         // MSB is the same for all the Alarm commands
      *(p++) = ucAlarmCode;                           // LSB of AlarmCmd
      *(p++) = ucUsedBytes - 3;                       // 1RespCode+2Data [+4MIC]
      *(p++) = RCS_N00_Success;            // RespCode
      *(p++) = ((uint8*)&unDestId)[1];                // MSB of Data
      *(p++) = ((uint8*)&unDestId)[0];                // LSB of Data
      g_ucAlarmBuffLen += 6;
      if(ucUsedBytes == 10)
      {
        memcpy(p, p_aNetMIC, 4);                      // MIC
        g_ucAlarmBuffLen += 4;
      }
    }
    
    MONITOR_EXIT();
  }
}


///////////////////////////////////////////////////////////////////////////////////
// Name: APP_PublishAlarmBuffer
// Author: NIVIS LLC, Doinel Alban
// Description: Publish an alarm report to the NM if the alarm buffer does contain any
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APP_PublishAlarmBuffer(void)
{
  if(g_ucAlarmBuffLen)
  {
    // store the buffer length at this moment; other alarms could be added at the end by DLL interrupts
    uint8 ucBuffLen = g_ucAlarmBuffLen;

//    if(
    TL_TransmitRequest( g_unAlarmHandle,
                        NET_ADDR_SHORT_ADDR,
                        c_aNetManagerAddr,
                        DLL_PACKET_PRIORITY_ALARM,
                        0,                         // ServiceID
                        PUBLISH_NOTIFY,            // TransportType
                        ucBuffLen,
                        g_aAlarmBuff
                      );
//    == SUCCESS)
    {
      // update handle
      g_unAlarmHandle = (g_unAlarmHandle == 0xAFFF) ? 0xA000 : (g_unAlarmHandle + 1);
      
      // check if other alarms have been added from DLL interrupts mean time
      MONITOR_ENTER();
      if(g_ucAlarmBuffLen != ucBuffLen)
      {
        ucBuffLen = g_ucAlarmBuffLen - ucBuffLen; // new added alarms length
        if(ucBuffLen < APP_MAX_ALARM_BUFF_LEN)   // just a precaution...
        { // move the new alarms to buffer beginning
          memmove(g_aAlarmBuff, g_aAlarmBuff + g_ucAlarmBuffLen, ucBuffLen);
          g_ucAlarmBuffLen = ucBuffLen;
          MONITOR_EXIT();
          return;
        }
      }
      g_ucAlarmBuffLen = 0;
      MONITOR_EXIT();
    }
//    else if(particular_error_condition)
//    {
//      TBD
//    }
  }
}

void APP_SrvcTask()
{
  if(!g_ucSrvcTaskFlag)
    return;
 
  if(NET_STATE_OPERATIONAL != g_ucNetState)  //probably better if(g_stC769.joinStatus & JoinProcessStatusMask_NormalOperationCommencing)
    return;
  
  //if(g_sPendingCmdsData.m_bPendingCommands)
    //return;
  uint8 ucIdx=0;
  for(; ucIdx<MAX_SESSIONS_NO; ucIdx++)
  {

//    if((UNUSED_ENTRY != g_aSessionsTable[ucIdx].m_ucSessionFlags) && (GW_NICKNAME == g_aSessionsTable[ucIdx].m_unPeerNickname)) // wrong condition, since m_ucSessionFlags can also have BIT6 set!!!
    if(((UNUSED_ENTRY & g_aSessionsTable[ucIdx].m_ucSessionFlags) == 0) // this session entry is in use
       && (GW_NICKNAME == g_aSessionsTable[ucIdx].m_unPeerNickname))
      break;
  }
  if(MAX_SESSIONS_NO == ucIdx) //no session with gw found
    return;

/*  
  static uint32 ulLastRTCCounter = 0;
  
  if(RTC_COUNT - ulLastRTCCounter < APPSRVC_TASK_FREQ )
  {
    return;
  }
  ulLastRTCCounter = RTC_COUNT; 
*/
  //TEST SRVC AFTER RESET
#ifdef TEST_APP_SRVC
  static uint8 ucStart = 1;
  if(ucStart)
  {
    ucStart = 0;  
    
    g_stBurstModeTableNvData[1].m_ucBurstModeControlCode = 2;
    g_stBurstModeTableNvData[1].m_unCmdNo = 1;
    g_stBurstModeTableNvData[1].m_ulMinUpdateTime = 800; // *10ms => 4 sec
    g_stBurstModeTableNvData[1].m_ulMaxUpdateTime = 1600;
    g_stBurstModeTable[1].m_ucServiceId = 0;

/*    
    g_stBurstModeTableNvData[2].m_ucBurstModeControlCode = 2;
    g_stBurstModeTableNvData[2].m_unCmdNo = 2;
    g_stBurstModeTableNvData[2].m_ulMinUpdateTime = 400; // *10ms => 4 sec
    g_stBurstModeTableNvData[2].m_ulMaxUpdateTime = 1600;
    g_stBurstModeTable[2].m_ucServiceId = 0;

    g_stEventsTableNvData[0].m_ucEventControlCode = 2;
    g_stEventsTableNvData[0].m_aucEventMasks[0] = 0xFF;
    g_stEventsTableNvData[0].m_ulRetryTime = 800; // *10ms => 4 sec
    g_stEventsTableNvData[0].m_ulMaxUpdateTime = 3200;
    g_stEventsTable[0].m_ucServiceId = 0;
*/    
  }
#endif //TEST_APP_SRVC
  //~TEST SRVC

#if ( _UAP_TYPE != UAP_TYPE_WHART_API )   
    for(uint8 ucIdx=0; ucIdx<MAX_BURST_NO; ucIdx++)
    {
      if(BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer == g_stBurstModeTableNvData[ucIdx].m_ucBurstModeControlCode)
        if(!g_stBurstModeTable[ucIdx].m_ucServiceId)
        {
          if(NULL != REQNM_FindSrvcCmdPending(ucIdx + 1))
             return;
        
          uint8 ucQIdx;
          REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
          if(!p)
            return; // no entry available, cannot send the request to NM
          
          MinUpdateTimer[ucIdx] = g_stBurstModeTableNvData[ucIdx].m_ulMinUpdateTime;
          MaxUpdateTimer[ucIdx] = g_stBurstModeTableNvData[ucIdx].m_ulMaxUpdateTime;
          p->m_unCmdId = CMDID_C799_RequestService;
          p->m_pDRBuf = NULL;
          p->m_stC799Req.m_ucServiceRequestFlags = ServiceRequestFlagsMask_Source; 
          p->m_stC799Req.m_ucServiceApplicationDomain = ServiceApplicationDomain_Publish;
          p->m_stC799Req.m_unNicknameOfPeer = GW_NICKNAME;   
          p->m_stC799Req.m_tPeriod.u32 = g_stBurstModeTableNvData[ucIdx].m_ulMinUpdateTime * (32 * 10) ;
          p->m_stC799Req.m_ucServiceId = ucIdx + 1;
          
          g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_BandwidthAllocationPending;
          
          return;
        }
    }
#endif    
    for(uint8 ucIdx=0; ucIdx<MAX_EVENTS_NO; ucIdx++)
    {
      if(EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer == g_stEventsTableNvData[ucIdx].m_ucEventControlCode)
        if(!g_stEventsTable[ucIdx].m_ucServiceId)
        {
          if(NULL != REQNM_FindSrvcCmdPending(ucIdx + MAX_BURST_NO + 1))
             return;
          
          uint8 ucQIdx;
          REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
          if(!p)
            return; // no entry available, cannot send the request to NM
          
          RetryTimer[ucIdx] = g_stEventsTableNvData[ucIdx].m_ulRetryTime;
          UpdateTimer[ucIdx] = g_stEventsTableNvData[ucIdx].m_ulMaxUpdateTime;    
          p->m_unCmdId = CMDID_C799_RequestService;
          p->m_pDRBuf = NULL;
          p->m_stC799Req.m_ucServiceRequestFlags = ServiceRequestFlagsMask_Source/*|ServiceRequestFlagsMask_Sink*/; 
          p->m_stC799Req.m_ucServiceApplicationDomain = ServiceApplicationDomain_Event;
          p->m_stC799Req.m_unNicknameOfPeer = GW_NICKNAME;   
          p->m_stC799Req.m_tPeriod.u32 = g_stEventsTableNvData[ucIdx].m_ulRetryTime * (32 * 10) ;
          p->m_stC799Req.m_ucServiceId = ucIdx + MAX_BURST_NO + 1;
          
          g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_BandwidthAllocationPending;
            
          return;        
         }
    }
  
#if ( _UAP_TYPE == UAP_TYPE_WHART_API ) 
    if (NO_ERR != SendMessage(STACK_SPECIFIC, HART_NOTIFY_JOIN, API_REQ, 0, 0, NULL /* &g_ucJoinStatus */ ))
      return;
#endif
    g_ucSrvcTaskFlag = 0;
    g_stC769.joinStatus |= JoinProcessStatusMask_NormalOperationCommencing;  
}

#if ( _UAP_TYPE == UAP_TYPE_WHART_API ) 
///////////////////////////////////////////////////////////////////////////////////
// Name: APP_IsCmdForMe
// Author: NIVIS LLC
// Description: cmd execute decision 
// Parameters: p_unCmdId - command id
// Return: 1 = cmd for me, 0 = fwd cmd to user board
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
uint8 APP_IsCmdForMe(uint16 p_unCmdId)
{
  //device specific range
  if((128 <= p_unCmdId) && (253 >= p_unCmdId))
     return 0;
      
  switch (p_unCmdId)
  {
    
    //universal cmds:
    case CMDID_C001_ReadPrimaryVariable: 
    case CMDID_C002_ReadLoopCurrentAndPercentOfRange:
    case CMDID_C003_ReadDynamicVariablesAndLoopCurrent:
    case CMDID_C008_ReadDynamicVariableClassifications:
    case CMDID_C009_ReadDeviceVariablesWithStatus:
    case CMDID_C014_ReadPrimaryVariableTransducerInformation:
    case CMDID_C015_ReadDeviceInformation:  
    //case CMDID_C016_ReadFinalAssemblyNumber:  
      
    //common practice cmds:  
    case CMDID_C033_ReadDeviceVariables:
    case CMDID_C034_WritePrimaryVariableDampingValue:
    case CMDID_C035_WritePrimaryVariableRangeValues:
    case CMDID_C036_SetPrimaryVariableUpperRangeValue:
    case CMDID_C037_SetPrimaryVariableLowerRangeValue:  
    case CMDID_C040_EnterExitFixedCurrentMode:
    case CMDID_C041_PerformSelfTest:
    case CMDID_C043_SetPrimaryVariableZero:
    case CMDID_C044_WritePrimaryVariableUnits:
    case CMDID_C045_TrimLoopCurrentZero:
    case CMDID_C046_TrimLoopCurrentGain:
    case CMDID_C047_WritePrimaryVariableTransferFunction:
    case CMDID_C049_WritePrimaryVariableTransducerSerialNumber:
    case CMDID_C050_ReadDynamicVariableAssignments:  
    case CMDID_C051_WriteDynamicVariableAssignments:
    case CMDID_C052_SetDeviceVariableZero:
    case CMDID_C053_WriteDeviceVariableUnits:
    case CMDID_C054_ReadDeviceVariableInformation:
    case CMDID_C055_WriteDeviceVariableDampingValue:
    case CMDID_C056_WriteDeviceVariableTransducerSerialNo:
    case CMDID_C060_ReadAnalogChannelAndPercentOfRange:
    case CMDID_C061_ReadDynamicVariablesAndPrimaryVariableAnalogChannel:
    case CMDID_C062_ReadAnalogChannels:
    case CMDID_C063_ReadAnalogChannelInformation:
    case CMDID_C064_WriteAnalogChannelAdditionalDampingValue:
    case CMDID_C065_WriteAnalogChannelRangeValues:
    case CMDID_C066_EnterExitFixedAnalogChannelMode:
    case CMDID_C067_TrimAnalogChannelZero:
    case CMDID_C068_TrimAnalogChannelGain:
    case CMDID_C069_WriteAnalogChannelTransferFunction:
    case CMDID_C070_ReadAnalogChannelEndpointValues:
    case CMDID_C071_LockDevice:
    case CMDID_C072_Squawk:
    case CMDID_C076_ReadLockDeviceState:
    //case CMDID_C078_ReadAggregatedCommands:
    case CMDID_C079_WriteDeviceVariable:
    case CMDID_C080_ReadDeviceVariableTrimPoints:
    case CMDID_C081_ReadDeviceVariableTrimGuidelines:
    case CMDID_C082_WriteDeviceVariableTrimPoint:
    case CMDID_C083_ResetDeviceVariableTrim:
    case CMDID_C091_ReadTrendConfiguration:
    case CMDID_C092_WriteTrendConfiguration:
    case CMDID_C093_ReadTrend:
    case CMDID_C096_ReadSynchronousAction:
    case CMDID_C097_ConfigureSynchronousAction:
    case CMDID_C098_ReadCommandAction:  
    case CMDID_C099_ConfigureCommandAction:
    case CMDID_C103_WriteBurstPeriod:
    case CMDID_C104_WriteBurstTrigger:
    case CMDID_C105_ReadBurstModeConfiguration:                                         
    case CMDID_C107_WriteBurstDeviceVariables:                   
    case CMDID_C108_WriteBurstModeCommandNumber:                  
    case CMDID_C109_BurstModeControl:
      
    case CMDID_C110_ReadAllDynamicVariables:
    case CMDID_C113_CatchDeviceVariable:
    case CMDID_C114_ReadCaughtDeviceVariable:
    //case CMDID_C115_ReadEventNotificationSummary:                
    //case CMDID_C116_WriteEventNotificationBitMask:                    
    //case CMDID_C117_WriteEventNotificationTiming:
    //case CMDID_C118_EventNotificationControl:                          
    //case CMDID_C119_AcknowledgeEventNotification:
    /*  
    case 128: //read persistent memory
    case 129: //write persistent memory
    case 130: //erase persistent memory  
    case 131: //app-proc tasks status
    case 132: //reads msp fw version, e.g. 01 05 04
    */
   // not for me, send it to the user board
      return 0;
      
    default:
      
      // cmd for me
      return 1;   
  } // end SWITCH
}


#endif  
    
