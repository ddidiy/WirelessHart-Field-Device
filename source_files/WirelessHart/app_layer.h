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
* Name:         app_layer.h
* Author:       Nivis LLC
* Date:         July 2008
* Description:  Application Layer 
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_APP_H_
#define _NIVIS_APP_H_

/*===========================[ public includes ]=============================*/
#include "../typedef.h"
#include "provision.h"
#include "../global.h"
#include "../spi.h"

#include "uap_data.h"
#include "network_layer_dm.h"
#include "transport_layer.h"
#include "datalink_layer.h"
#include "datalink_layer_dm.h"
#include "ParseComposeCommand.h"
#include "ParseComposeCommandTables.h"
#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/UniversalCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/CommonPracticeCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/WirelessApplicationCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/DataLinkLayerCommands.h"
#else
  #include "ApplicationLayer/Model/UniversalCommands.h"
  #include "ApplicationLayer/Model/CommonPracticeCommands.h"
  #include "ApplicationLayer/Model/WirelessApplicationCommands.h"
  #include "ApplicationLayer/Model/DataLinkLayerCommands.h"
#endif

#if(WIHARTDEV_TYPE != WIHARTDEV_TYPE_ACCPOINT)
#include "apphart.h"
#endif

#define MAX_PENDING_C963_NO     MAX_SESSIONS_NO
#define MAX_PENDING_C965_NO     DLL_MAX_SUPERFRAMES

#define DUMMY_UINT8     0xFF
#define DUMMY_UINT16  0xFFFF
#define C780_NEIGHBOR     10 // length of neighbor properties in Cmd780
#define C787_NEIGHBOR      3 // length of neighbor properties in Cmd787

//device specific cmd 48 size
#define C048_SIZE     C048_SIZE_14 

// burst/event message handle 
extern uint16 g_unBurstAndEventsHandle; 
#define SET_BURST_EVENT_HANDLER(arg) ( (arg) = ((arg) == 0x9FFF) ? 0x9000 : ((arg) + 1) )

typedef union
{
  C048_ReadAdditionalDeviceStatus_Req data;  
  uint8 byte[25];
} Cmd48Data;

// additional device status structure
extern Cmd48Data g_stCmd48Data; 

// when set => perform the self test function
extern uint8 g_ucDoSelfTest; 

// HealthReports
enum
{
  HEALTH_REPORT_FIRST_NPDU,
  HEALTH_REPORT_NEXT_NPDU,
}; // HealthReport StateMachine status

typedef struct 
{
  uint8 m_ucIdx;
  uint8 m_ucSize;
} REQ_T; // Cmd780/787 request format 


typedef union
{
  uint32  m_ulValue;
  float   m_fValue;
} SPECIAL_FLOAT_T; 

extern const SPECIAL_FLOAT_T c_stNoValue;


#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )

  #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
 
    // analog data from DAQ
    #define TEMP_CODE     UAP_DATA_TEMP - 1
    #define HUMID_CODE    UAP_DATA_HUMIDITY -1
    #define DEW_CODE      UAP_DATA_DEWPOINT -1
    #define ADUMMY_CODE   UAP_DATA_ANALOG1 -1
    
    // digital data from DAQ
    #define D1DUMMY_CODE  UAP_DATA_DIGITAL1 - 1
    #define D2DUMMY_CODE  UAP_DATA_DIGITAL2 - 1
    #define D3DUMMY_CODE  UAP_DATA_DIGITAL3 - 1
    #define D4DUMMY_CODE  UAP_DATA_DIGITAL4 - 1

  #elif (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)

    extern uint8 g_aHartDVCodes[LOCAL_ANALOG_CH_NO];

    // local data
    #define ANALOG_INPUT       UAP_DATA_LOCAL_ANALOG_INPUT - 1
    #define TEMP_CODE          UAP_DATA_LOCAL_TEMP - 1
    #define HUMID_CODE         UAP_DATA_LOCAL_HUMIDITY - 1
    #define DEW_CODE           UAP_DATA_LOCAL_DEWPOINT - 1
    #define PERCENT_OF_RANGE   UAP_DATA_LOCAL_PERCENT_OF_RANGE - 1   
    #define LOOP_CURRENT       UAP_DATA_LOCAL_LOOP_CURRENT - 1

    // analog data from user board
    #define USER_DATA1_CODE    UAP_DATA_ANALOG1 - 1
    #define USER_DATA2_CODE    UAP_DATA_ANALOG2 - 1
    #define USER_DATA3_CODE    UAP_DATA_ANALOG3 - 1
    #define USER_DATA4_CODE    UAP_DATA_ANALOG4 - 1
    #define USER_DATA5_CODE    UAP_DATA_ANALOG5 - 1
    #define USER_DATA6_CODE    UAP_DATA_ANALOG6 - 1
    #define USER_DATA7_CODE    UAP_DATA_ANALOG7 - 1
    #define USER_DATA8_CODE    UAP_DATA_ANALOG8 - 1 

  #endif

#endif

#define DEVICE_VARIABLE_CODE1  DeviceVariableCodes_BatteryLife     // battery life(float in days)
#define DEVICE_VARIABLE_CODE2  DeviceVariableCodes_PercentRange    // percent of range(for PV)
#define DEVICE_VARIABLE_CODE3  DeviceVariableCodes_LoopCurrent     // loop current
#define DEVICE_VARIABLE_CODE4  DeviceVariableCodes_PrimaryVariable // primary variable(PV) 

#if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )
  // hard-coded positions in device variable table
  //#define BATT_LIFE_IDX  3        // battery life index
  #ifndef TEST_ADC_COUNTS
    #define PERCENT_OF_RANGE_IDX  4 // percent of range index
    #define LOOP_CURRENT_IDX  5     // loop current index
  #else
    #define PERCENT_OF_RANGE_IDX  8 // percent of range index
    #define LOOP_CURRENT_IDX  9     // loop current index
  #endif
#else
  // hard-coded positions in device variable table
  //#define BATT_LIFE_IDX  3        // battery life index
  #define PERCENT_OF_RANGE_IDX  3 // percent of range index
  #define LOOP_CURRENT_IDX  4     // loop current index
#endif /* BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT */

#define PV_IDX  0               // temperature
#define SV_IDX  1               // humidity
#define TV_IDX  2               // dew-point
#define QV_IDX  3


#define MAX_REQ_BYTES 33 // tbd !!! 

//#define BURST_PIPE_100MS
#ifdef BURST_PIPE_100MS
  // 100ms pipe structure
  typedef struct 
  {
    uint8 m_ucPipeIsActive; // 0xFF -> inactive, 1 -> active
    uint8 m_ucIdx; // pipe index
    uint8 m_ucSlotsUntilBurst; // number of slots until bursting 
    
    uint32 m_ulLastRTCCount; 
  } PIPE_100MS;
  
  // 100ms pipe, this is supposed to be ONLY ONE for burst/event !!!
  extern PIPE_100MS g_stBurstPipe100ms, g_stEventPipe100ms;
#endif

/* APDU buffer */
extern uint8 g_aucBuffer[92];
//extern uint8 g_aucBuffer[100];
extern BinaryStream g_stBuffStream;

// burst pipe structure
typedef struct 
{
  //uint8 m_ucMsgNo; // number of burst messages
  uint8 m_ucServiceId; // the service id received from NM for this pipe
  uint8 m_ucFirstBurstSent; // flag set when this pipe is active and the first burst was sent; flag is cleared when the pipe is closed
} BURST_MODE_T;

// device burst table (contains ONLY non-volatile data)
extern BURST_MODE_T g_stBurstModeTable[MAX_BURST_NO]; 

// device variable data
typedef struct 
{
  float m_fValue;  
  
  // not in spec
  float m_fLastValue;
  // see Common Practice Commands, Cmd79 Write Device Variable
  uint8 m_ucNormalOperation; // =0 -> normal mode; != 0 -> force the engineering value to a certain value  
} DEVICE_VARIABLE_T; 

// device variable buffer (contains ONLY non-volatile data)
// 3 -> battery life, percent of range and loop current are MANDATORY
#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
  #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
   extern DEVICE_VARIABLE_T g_stDeviceVariables[ANALOG_CH_NO + DIGITAL_CH_NO];
  #else
   extern DEVICE_VARIABLE_T g_stDeviceVariables[LOCAL_ANALOG_CH_NO + ANALOG_CH_NO];
  #endif 
#else
  //due to mapping dynamic on device vars, no use in keeping them doubled!!!
  extern DEVICE_VARIABLE_T g_stDeviceVariables[NoOfDeviceVariables + 2];
#endif
  
// Dynamic Variable Mapper (device variable - dynamic variable)
extern const uint8 c_aVarMap[NoOfDynamicVariables];

// timers for min. update period of a burst pipe
extern TIME_10MS MinUpdateTimer[MAX_BURST_NO];

// timers for max. update period of a burst pipe
extern TIME_10MS MaxUpdateTimer[MAX_BURST_NO];

// event configuration
typedef struct 
{  
  uint8 m_ucServiceId; // the service id received from NM for this event
} EVENT_NOTIFICATION_T;

// events table (contains ONLY non-volatile data)
extern EVENT_NOTIFICATION_T g_stEventsTable[MAX_EVENTS_NO];

// event pending queue, see Addendum to Common Practice Command Spec
typedef struct 
{
  TIME       m_ulTimestamp;         // time within the day [1/32 ms]
  uint16     m_ucCCCounter;         // configuration changed counter
  uint8      m_ucEventNmb;          // event number
  uint8      m_aData[26];           // device status byte + Cmd48 data

// -----------------------------------------------------------------------------
// until here the structure is identical with C119_AcknowledgeEventNotification_Req  
// -----------------------------------------------------------------------------  
  
  uint8      m_ucEventStatus;       // event status(not shifted !)
  uint8      m_ucAckReceived;       // 0xFF -> not triggered, !=0xFF -> triggered
} EVENT_PENDING_T;

#define MAX_PENDING_EVENTS 5
extern EVENT_PENDING_T g_stEventQueue[MAX_PENDING_EVENTS];
extern EVENT_PENDING_T* g_pEventQueueEnd;

#define EVENT_QUEUE_IS_FULL ( (g_pEventQueueEnd < g_stEventQueue + MAX_PENDING_EVENTS) ? 0 : 1 )
#define EVENT_QUEUE_NOT_EMPTY ( (g_pEventQueueEnd > g_stEventQueue) ? 1 : 0 )

// this flag is set if Cmd119 request is received
extern uint8 g_ucIsEventAck;

// timer table for retry period of an event
extern TIME_10MS RetryTimer[MAX_EVENTS_NO];

// timer table for update period of an event
extern TIME_10MS UpdateTimer[MAX_EVENTS_NO];

// Time/Date structure
typedef struct
{
  uint8   m_ucUTCReceived; // 0 -> UTC not received; 1 -> UTC received  
  
  uint8   m_ucYear;  // years since 1900          [0,255]  
  uint8   m_ucMonth; // months since Janurary     [1,12]
  uint8   m_ucDay;   // day of month              [1,31]
  uint32  m_ulTimeOfDay; // time within a day in 1/32 ms
  /*
  uint8 m_ucHour;  // hours since midnight      [0,23]    
  uint8 m_ucMin;   // minutes after the hour    [0,59]
  uint8 m_ucSec;   // seconds after the minute  [0,59]
  */
  
  unsigned long long m_ullASN; // current ASN when UTC was received
}TIME_DATE_T;

// UTC0
extern TIME_DATE_T g_stUTCatASN0;

// the Service ID requested by wireless device, must be in range 0x00 ... 0x7F
extern uint8 g_ucRequestedServiceId; 

typedef struct {
  
  
  uint8_t   m_bC961:  1;  // set if there are commands that need to be executed in the future
  uint8_t   m_bC963:  1;
  uint8_t   m_bC965:  1;
  uint8_t   m_bC972:  1;
  uint8_t   m_bPendingCommands:   1;
  uint8_t   m_bWaitingToSuspend:  1;   // identifies one of the 2 phases when executing C972
  
  C961_WriteNetworkKey_Resp   m_sC961_Data;
  
  uint8_t   m_ucNoOfSessions;
  uint8_t   m_ucNoOfSuperframes;
  uint8_t   m_aC963_Idx[MAX_PENDING_C963_NO];
  C963_WriteSession_Resp      m_aC963_Data[MAX_PENDING_C963_NO];
  C965_WriteSuperframe_Resp   m_aC965_Data[MAX_PENDING_C965_NO];
  
  
  C972_SuspendDevices_Resp    m_sC972_Data;
} PENDING_CMDS_DATA;

extern uint8               g_ucPendingCmds;
extern PENDING_CMDS_DATA   g_sPendingCmdsData;
extern uint8               g_ucSrvcTaskFlag;



#if ( _UAP_TYPE == UAP_TYPE_WHART_API )  // Full API stuff


uint8 APP_IsCmdForMe(uint16 p_unCmdId);

#endif           


extern uint8 g_ucMarkDisconnectHandle;

#define APP_ALARM_C788    ((uint8)CMDID_C788_AlarmPathDown)
#define APP_ALARM_C789    ((uint8)CMDID_C789_AlarmSourceRouteFailed)
#define APP_ALARM_C790    ((uint8)CMDID_C790_AlarmGraphRouteFailed)
#define APP_ALARM_C791    ((uint8)CMDID_C791_AlarmTransportLayerFailed)
#define APP_ALARM_MSB     ((uint8)(CMDID_C788_AlarmPathDown >> 8))

#define APP_MAX_ALARM_BUFF_LEN  MIN_APDU_SIZE



void APP_Init();
void APP_TransmitIndicate(HANDLE                p_hHandle,                      
                          uint8                 p_ucPriority,
                          SHORT_ADDR            p_unSrcAddr,
                          uint8                 p_ucSequenceNumber,
                          uint8                 p_ucTransportType, 
                          uint8                 p_ucApduDataLen,
                          const void *          p_pApduData);

void APP_TransmitConfirm(HANDLE             p_hHandle,
                         uint8              p_ucLocalStatus,
                         uint8              p_ucApduDataLen,
                         const void*        p_pApduData);


void DLL_CheckCmdExecTime();

void APP_SendHealthReports(void);
void APP_InitBurstAndEvent(void);
void APP_CopyChannel(void);
NV_DEVICE_VARIABLE_T* APP_ReadChannel(uint8 p_ucCode);
void APP_TriggerEvents(void); 
void APP_BurstModeAndEventNotificationTask(void);
void APP_AddToAlarmBuffer(uint8 ucAlarmCode, uint16 unDestId, const uint8* p_aNetMIC);
void APP_PublishAlarmBuffer(void);
void APP_ComposeResponse( uint16 p_unCmdId, void* p_pReq, void* p_pResp );
void APP_DateToTime( const TIME_DATE_T* p_pTimeDate, unsigned long long* p_pASN );
void APP_TimeToDate( unsigned long long p_ullASN, TIME_DATE_T* p_pTimeDate );
uint8 APP_DoBurstEventTimerCorrection( uint32* p_pulParam1, uint32* p_pulParam2 );
uint8 BurstEventCorrection( uint32* p_ulValue );
void Execute100msPipe(void);


void APP_SrvcTask();

#endif /* _NIVIS_APP_H_ */
