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
* Name:         apphart.c
* Author:       Nivis LLC
* Date:         October 2009
* Description:  
* Changes:
* Revisions:
****************************************************************************************************/
#include "..\system.h"
#include "tmr_util.h"

#if ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )
#include <string.h>

#include "apphart.h"
#include "transport_layer.h" //for COLD_START
#include "app_cmdresp.h"
#include "app_layer.h"
#include "drm.h"

#define SYNC_EPSILON  20 /*?CHECK!!!*/
#define TIME24H      100*60*60*24 // (((10 ms units * 100 => 1s) * 60 => 1min) *60 => 1h) * 24 => 24h

#define APPHART_TASK_FREQ     USEC_TO_FRACTION2( 10000 ) // each 10ms
#define APPHART_RETRY_FREQ    USEC_TO_FRACTION2( 73000 ) // retry after 73 ms

#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Binarization/CommonPracticeCommands.h"
#else
  #include "ApplicationLayer/Binarization/CommonPracticeCommands.h"
#endif

SUBDEV_ID g_aSubDev[MAX_NOF_SUBDEV];
uint8     g_ucNofSubDev;

uint8_t g_ucSyncSubdevices;

APPHART_QUEUE_ELEMENT g_aAPPHARTQueue[APPHART_QUEUE_SIZE];

extern DR_SLAVE_BUFFER_STRUCT* findDRBufByCmdId(uint16 p_unCmdId);

void APPHART_InitQueue()
{
  memset(g_aAPPHARTQueue, 0, sizeof(g_aAPPHARTQueue));
}

void APPHART_Task()
{
  static uint32 ulLastRTCCounter = 0;
  
  if(RTC_COUNT - ulLastRTCCounter < APPHART_TASK_FREQ )
  {
    return;
  }
  ulLastRTCCounter = RTC_COUNT; 
  
  static uint8 ucOldIdx = 0; //last sent req index
  uint8 ucCrtIdx = 0;
  uint8 ucSend = 0; //flag - if found a sendable req in queue
  APPHART_QUEUE_ELEMENT* pEntry;
  
  for(uint8 ucIdx=0; ucIdx<APPHART_QUEUE_SIZE; ucIdx++)
  {
    ucCrtIdx = (ucOldIdx+ucIdx)%APPHART_QUEUE_SIZE;
    pEntry = g_aAPPHARTQueue + ucCrtIdx;
    if( RTC_COUNT - pEntry->m_ulTimeout >  APPHART_RETRY_FREQ)
    {
      pEntry->m_ucRetryCnt--;
      pEntry->m_ulTimeout = 0;
    }
   
    switch(pEntry->m_ucStatus){
      case APPHART_SEND:
        {
          if(!ucSend)
            ucSend = 1;
        }
        break;
      case APPHART_WAIT:
        {
          if(pEntry->m_ucRetryCnt)
          {
            if(!ucSend)
              ucSend = 1;
          }
          else 
            pEntry->m_ucStatus = APPHART_ABORT;
        }
        break;
      case APPHART_SUCCESS:
        {
          //TODO: do (if) whatever you need to do with the response
          pEntry->m_ucStatus = APPHART_INACTIVE;
        }
        break;        
      case APPHART_ABORT:
        {
          if(pEntry->m_ucFromDR)
          {
            DR_SLAVE_BUFFER_STRUCT* pDRBuf = findDRBufByCmdId(pEntry->m_unCmdId);
            if(pDRBuf) //should always be true
              pDRBuf->m_ucDRStatus = DRSLAVE_DEAD;
          }
          pEntry->m_ucStatus = APPHART_INACTIVE;
        }
        break;
      default:
        break;
    }//end switch
  } //end for
  
  if(ucSend) //found a sendable req in queue
  {
    ucOldIdx = ucCrtIdx;
    if(CMDID_C089_SetRealTimeClock == pEntry->m_unCmdId)
    {
      C089_SetRealTimeClock_Req* pstReqC089 = (C089_SetRealTimeClock_Req*)pEntry->m_aReqData;
      if(TimeSetCode_ReadReceiveTime == pstReqC089->timeSetCode)
      {
        TIME_DATE_T stCurrentUTC;
        //current Time and Date
        APP_TimeToDate(g_sDllASN.m_ullASN, &stCurrentUTC);
       
        pstReqC089->date.day = stCurrentUTC.m_ucDay;
        pstReqC089->date.month = stCurrentUTC.m_ucMonth;
        pstReqC089->date.year = stCurrentUTC.m_ucYear;
        pstReqC089->time.u32 = stCurrentUTC.m_ulTimeOfDay;
        pstReqC089->nothingSpecial = 0;
          
      }
    }
    uint8 aReqBuffer[HART_BUFFER_MAX_LEN];
    BinaryStream stream;
    STREAM_INIT(&stream, aReqBuffer, HART_BUFFER_MAX_LEN);
    ComposeWiredRequestCommmand((void*)pEntry->m_aReqData, 
                                  pEntry->m_unCmdId, 
                                  &stream);  
    
    DLLHART_TRANSMIT_Request(pEntry->m_ucAddrType,
                     pEntry->m_aAddr,
                     //(pEntry->m_unCmdId > 255) ? 31 : pEntry->m_unCmdId,
                     //stream.nextByte - aReqBuffer,
                     aReqBuffer
                    );
    pEntry->m_ucStatus = APPHART_WAIT;
    pEntry->m_ulTimeout = 0;  
    pEntry->m_ucRetryCnt--;
  }
}

APPHART_QUEUE_ELEMENT* APPHART_getQueueEntry()
{
  uint8 ucIdx = 0;
  APPHART_QUEUE_ELEMENT* p = g_aAPPHARTQueue;
  for(; ucIdx<APPHART_QUEUE_SIZE; ucIdx++, p++)
  {
    if(APPHART_INACTIVE ==  p->m_ucStatus)
    {
      memset(p, 0, sizeof(APPHART_QUEUE_ELEMENT));
      p->m_ucStatus = APPHART_SEND;
      p->m_ucRetryCnt = g_stProv.ucIOSystemRetryCount;
      return p;
    }
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APPHART_TRANSMIT_confirm
// Author: NIVIS LLC
// Description: It is invoked by HART Master DLL to communicate to the Master Device APP Layer 
//                the result of a previously executed TRANSMIT_request
// Parameters:
//                p_ucStatus   - local status (from Rx communication routines)
//                p_ucAddrType - 0 -Polling (1 byte) Address, 1 -Unique (5 byte) Address
//                p_pSlaveAddr - slave address 
//                p_pData      - data buffer
// Return: None
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APPHART_TRANSMIT_confirm(uint8 p_ucStatus,
                              uint8 p_ucAddrType,
                              const uint8 *p_pSlaveAddr,
                              const uint8 *p_pData)
{
  uint16 unCmdId = *p_pData;
  uint8 ucDataLen = *(p_pData+1);
  BinaryStream stStream;
  STREAM_INIT(&stStream, p_pData+2, ucDataLen);  
  
  if(31 == unCmdId)
  {
    if(ucDataLen < 2)
      return;
    STREAM_READ_UINT16(&stStream, &unCmdId);
  }
    
  uint8 ucIdx = 0;
  APPHART_QUEUE_ELEMENT* p = g_aAPPHARTQueue;
  for(; ucIdx<APPHART_QUEUE_SIZE; ucIdx++, p++)
  {
    //TODO: should use parser here for covering 2-byte cmd format
    
    if((APPHART_WAIT == p->m_ucStatus) &&
       (p->m_unCmdId == unCmdId) &&
       (p->m_ucAddrType == p_ucAddrType) &&
       !memcmp(p_pSlaveAddr, p->m_aAddr, p_ucAddrType*4 + 1))
    {
     
      switch(unCmdId)
      {
        case CMDID_C089_SetRealTimeClock:
          {
            C089_SetRealTimeClock_Resp* pstRspC089 = (C089_SetRealTimeClock_Resp*) p->m_aRspData;

                
            Parse_C089_SetRealTimeClock_Resp(pstRspC089, &stStream);            
            
            if(TimeSetCode_ReadReceiveTime == pstRspC089->timeSetCode)
            {

              unsigned long long ullRecvTime; // = process response data & time using APP_DateToTime();
              unsigned long long ullReqTime;  // = process request data & time using APP_DateToTime();        
              TIME_DATE_T stTmpUTC;
              
              stTmpUTC.m_ucDay = pstRspC089->date.day;
              stTmpUTC.m_ucMonth = pstRspC089->date.month;
              stTmpUTC.m_ucYear = pstRspC089->date.year;
              stTmpUTC.m_ulTimeOfDay = pstRspC089->time.u32;
              APP_DateToTime(&stTmpUTC, &ullRecvTime);
              
              C089_SetRealTimeClock_Req* pstReqC089 = (C089_SetRealTimeClock_Req*)p->m_aReqData;
              stTmpUTC.m_ucDay = pstReqC089->date.day;
              stTmpUTC.m_ucMonth = pstReqC089->date.month;
              stTmpUTC.m_ucYear = pstReqC089->date.year;
              stTmpUTC.m_ulTimeOfDay = pstReqC089->time.u32;
              APP_DateToTime(&stTmpUTC, &ullReqTime);        
              
      
              unsigned long long ullCrtTime = g_sDllASN.m_ullASN;
              unsigned long long ullDiff = (ullCrtTime - ullReqTime) / 2;
              if((ullRecvTime >= (ullReqTime + SYNC_EPSILON)) && 
                 (ullRecvTime <= (ullReqTime - SYNC_EPSILON))) 
              {
                //may use the same queue entry (same cmd id, same addr.)
                //must only set Time, Date, write flag and queue entry status
                // TODO: check if it is absolutely vital 
                  // to adjust g_aSubDev[ucIdx].m_ullLastSyncTime = ullCrtTime + ullDiff; it was also set as g_sDllASN.m_ullASN at queue-add-time
                APP_TimeToDate(ullCrtTime + ullDiff, &stTmpUTC);
                
                pstReqC089->timeSetCode = TimeSetCode_WriteDateTime;
                pstReqC089->date.day = stTmpUTC.m_ucDay;
                pstReqC089->date.month = stTmpUTC.m_ucMonth;
                pstReqC089->date.year = stTmpUTC.m_ucYear;
                pstReqC089->time.u32 = stTmpUTC.m_ulTimeOfDay;
                p->m_ucStatus = APPHART_SEND;
                p->m_ulTimeout = 0; 
                p->m_ucRetryCnt = g_stProv.ucIOSystemRetryCount;
                return;
              }
            }            
          }
          break;
        default:
          break;
      }//end switch


      
      //if from DR context, pass response
      if(p->m_ucFromDR)
      {
        DR_SLAVE_BUFFER_STRUCT* pDRBuf = findDRBufByCmdId(unCmdId);
        if(pDRBuf) //should always be true
        {
          memcpy(pDRBuf->m_aResp + pDRBuf->m_ucRespSize, p_pData, ucDataLen+2);
          pDRBuf->m_ucRespSize += (ucDataLen + 2); //for cmd 77 the "wireless part" of the response should have already been added
          pDRBuf->m_ucDRStatus = DRSLAVE_END;
        }
      }
        
      p->m_ucStatus = APPHART_SUCCESS;
    }
  
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Name: APPHART_TRANSMIT_indicate
// Author: NIVIS LLC
// Description: It is invoked by HART Slave DLL to notify the Field Device APP Layer 
//                that a valid message has been received from the specified Master Device
// Parameters:
//                p_ucSrcAddr - source address (0 -Secondary, 1 -Primary Master)
//                p_pData     - data buffer
// Return: None
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void APPHART_TRANSMIT_indicate(uint8 p_ucSrcAddr,
                               const uint8 *p_pData)
{
  uint8 ucDataLen = *(p_pData+1);
  BinaryStream stInputStream;
  STREAM_INIT(&stInputStream, p_pData, ucDataLen+2);
  
  uint8 aOutputBuffer[MAX_APDU_SIZE];  
 
  BinaryStream stOutputStream;
  STREAM_INIT(&stOutputStream, aOutputBuffer, sizeof(aOutputBuffer));
  
  uint16 unRespCode = APP_ProcessCmdBuf( 0xff,
                                         0,
                                         WIRED_CMD,
                                         CD_REQUEST,
                                         p_ucSrcAddr,
                                         &stInputStream,
                                         &stOutputStream
                                       );

  if(unRespCode == RCS_N255_MakeNoResponse)
    return;   // ??? this request will return no response!!! (e.g. C011, C021)  
  
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )   
  if(RCS_N258_ReturnToAppQueue == unRespCode)
  {
    
      APP_QUEUE_ELEMENT* pQueue = APP_getQueueElement();
      //add queue element
      if(NULL != pQueue) //this was actually checked inside APP_ProcessCmdBuf. else should never happen!
      {
        pQueue->m_ucStatus = AQE_PENDING; 
     
        //g_aAPPQueue[ucIdx].m_ucPriority = p_ucPriority;
        
        pQueue->m_unSrcAddr = p_ucSrcAddr;
        pQueue->m_ucReqSize = ucDataLen+2;
        pQueue->m_ucReqCrtPos = stInputStream.nextByte - (uint8*)p_pData;
        pQueue->m_ucRspCrtPos = 0;
        pQueue->m_ulTimeout = RTC_COUNT;
        memcpy(pQueue->m_aReqBuffer, p_pData,  ucDataLen+2);
        
      }
  
      return;
  }  
#endif  
  DLLHART_TRANSMIT_Response(stOutputStream.nextByte - aOutputBuffer, aOutputBuffer);
  APP_ResetDeviceStatusFlag(p_ucSrcAddr, WIRED_CMD, COLD_START);
}

void APPHART_SYNC_SUBDEVICES_task()
{

  if(g_stUTCatASN0.m_ucUTCReceived) 
  {
    if(g_ucSyncSubdevices)
    {
      //force each subdevice to resync.
      for(uint8 ucIdx=0; ucIdx<g_ucNofSubDev; ucIdx++)
      {
        g_aSubDev[ucIdx].m_ullLastSyncTime = g_sDllASN.m_ullASN - TIME24H;
      }
      g_ucSyncSubdevices = 0;
    }
    for(uint8 ucIdx=0; ucIdx<g_ucNofSubDev; ucIdx++)
    {
      if((g_sDllASN.m_ullASN - g_aSubDev[ucIdx].m_ullLastSyncTime) >= TIME24H)
      {
        APPHART_QUEUE_ELEMENT* pEntry = APPHART_getQueueEntry();
        if(NULL != pEntry)
        {
          pEntry->m_ucAddrType = 5;
          pEntry->m_aAddr[0] = g_aSubDev[ucIdx].m_unExpandedDeviceTypeCode >> 8;
          pEntry->m_aAddr[1] = g_aSubDev[ucIdx].m_unExpandedDeviceTypeCode;
          memcpy(pEntry->m_aAddr+2, g_aSubDev[ucIdx].m_aDeviceId, 3);
          pEntry->m_unCmdId = CMDID_C089_SetRealTimeClock;
          ((C089_SetRealTimeClock_Req*)pEntry->m_aReqData)->timeSetCode = TimeSetCode_ReadReceiveTime;
          g_aSubDev[ucIdx].m_ullLastSyncTime = g_sDllASN.m_ullASN; //!will  be adjusted at cmd 89 confirmation
          pEntry->m_ucFromDR = 0;
        }
        else
        {
          //nothing to do. this subdev must wait until queue entry available; 
        }
      }
    }
  }
}

#endif //( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )
