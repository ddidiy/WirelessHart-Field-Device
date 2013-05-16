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
* Name:         provision.h
* Author:       NIVIS LLC, 
* Date:         July 2008
* Description:  This file holds the provisioning data
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_PROVISION_H_
#define _NIVIS_PROVISION_H_

#include "config.h"
#include "uap_data.h"
#include "../typedef.h"
#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/CommonPracticeCommands.h"
#else
  #include "ApplicationLayer/Model/CommonPracticeCommands.h"
#endif


#if( DEVICE_TYPE == DEV_TYPE_MC13225 )

  #include "../spif_interface.h"    

  typedef uint32 PROV_ADDR_TYPE;

  #define SECTOR_SIZE (4*1024)
  #define NVM_AREA_SIZE 2*SECTOR_SIZE


  // persistent data sectors (sector 31 is reserved for factory)


  #define MANUFACTURING_SECTOR_IDX      30  // the same manufacturing-sector for ISA and HART 
  #define MANUFACTURING_SECTOR_NO       (1UL << MANUFACTURING_SECTOR_IDX)
  #define MANUFACTURING_START_ADDR      (SECTOR_SIZE * MANUFACTURING_SECTOR_IDX)
//  #define MANUFACTURING_ISA_START_ADDR  (MANUFACTURING_START_ADDR)
//  #define MANUFACTURING_ISA_FREE        (1910) //(2048 - sizeof(ISA manufacturing data))   
  #define MANUFACTURING_HART_START_ADDR (MANUFACTURING_START_ADDR + 2048)

  // manufacturing info
  typedef struct 
  { 
    uint16 m_unFormatVersion; 
    uint8  m_aMAC[8];              
    uint16 m_unVRef;    
    uint8  m_ucMaxPA;        
    uint8  m_ucCristal; // m_ucCTune
//  uint8  m_ucFTune;
//  uint8  m_Reserved[13];
    uint8  m_aProvKEY[16];
    uint8  m_aProvSecManager[8];
    uint8  m_aPAProfile0[17];
    uint8  m_aPAProfile1[17];
    uint8  m_aPAProfile2[17];
    uint8  m_aPAProfile3[17];
    uint16 m_unCountryCode[16];
    //uint8  m_ucReserve[MANUFACTURING_ISA_FREE];   
  } T_MANUFACTURING; 

/*  
  typedef struct 
  {
    // HART manufacturing segment(2k)
    
    uint8  m_aUID_HART[3];
    
  } T_MANUFACTURING_HART;
*/
  
  
  #define NVM_HIGH_SECTOR_IDX         23  
  #define NVM_MIDDLE_HIGH_SECTOR_IDX  22  
  #define NVM_MIDDLE_LOW_SECTOR_IDX   21  
  #define NVM_LOW_SECTOR_IDX          20
  
  #define NVM_SIGNATURE    0xA5
  
  #define NVM_HIGH_SECTOR_NO          (1UL << NVM_HIGH_SECTOR_IDX)  
  #define NVM_MIDDLE_HIGH_SECTOR_NO   (1UL << NVM_MIDDLE_HIGH_SECTOR_IDX)  
  #define NVM_MIDDLE_LOW_SECTOR_NO    (1UL << NVM_MIDDLE_LOW_SECTOR_IDX) 
  #define NVM_LOW_SECTOR_NO           (1UL << NVM_LOW_SECTOR_IDX) 

  #define NVM_HIGH_SECTOR_START_ADDR          (SECTOR_SIZE * NVM_HIGH_SECTOR_IDX)
  #define NVM_MIDDLE_HIGH_SECTOR_START_ADDR   (SECTOR_SIZE * NVM_MIDDLE_HIGH_SECTOR_IDX)
  #define NVM_MIDDLE_LOW_SECTOR_START_ADDR    (SECTOR_SIZE * NVM_MIDDLE_LOW_SECTOR_IDX) 
  #define NVM_LOW_SECTOR_START_ADDR           (SECTOR_SIZE * NVM_LOW_SECTOR_IDX)

  #define NVM_AREA1_START_ADDR                NVM_LOW_SECTOR_START_ADDR
  #define NVM_AREA2_START_ADDR                NVM_MIDDLE_HIGH_SECTOR_START_ADDR

  #define NVM_AREA1_NO                        (NVM_LOW_SECTOR_NO | NVM_MIDDLE_LOW_SECTOR_NO)
  #define NVM_AREA2_NO                        (NVM_MIDDLE_HIGH_SECTOR_NO | NVM_HIGH_SECTOR_NO)   

#endif


#define CMD000_MIN_REQUIRED_PREAMBLES_FROM_MASTER     5
#define CMD000_HART_PROTOCOL_MAJOR_REV_NO             7
#define CMD000_DEVICE_REVISION_LEVEL                  1
#define CMD000_SOFTWARE_REVISION_LEVEL                1
#define CMD000_PHYSICAL_SIGNALING_CODE                PhysicalSignalingCodes_WIRELESS // CommonTable10
#define CMD000_FLAGS                                  (FlagAssignmentsMask_OQPSKModulation) // CommonTable11

#define C777_DEVICE_POWER_SOURCE                      DevicePowerSource_LinePower // CommonTable44
#define C777_DEVICE_PEAK_PACKETS_PER_SECOND           20.
#define C777_DURATION_AT_PEAK_PACKET_LOAD             (24UL*60*60*1000*32)    // 24h  // Time type = 1/32ms [hours*min*sec*ms*32]
#define C777_TIME_TO_RECOVER_FROM_POWER_DRAIN         0                       //      // Time type = 1/32ms [hours*min*sec*ms*32]
#define C777_RSL_THRESHOLD_GOOD_CONNECTION            -65   // [dBm] To Be Defined!
#define C777_MIN_REQUIRED_KEEP_ALIVE_TIME             (10*1000*32)            // 10s  // Time type = 1/32ms [hours*min*sec*ms*32]


#define CMD088_MIN_IOSYS_RETRY_COUNTER                2
#define CMD088_MAX_IOSYS_RETRY_COUNTER                6

#define CMD64512_DEVICE_REVISION_LEVEL                1
#define CMD64512_SOFTWARE_REVISION_LEVEL              1

#define JD_LONG_TAG   "Nivis WirelessHART v0000x 000000"    // 32 chars - version and DeviceID will be overwritten

//extern const UNIQUE_ID c_aJoinDevUID_BE;  // provisioned UID of the JD
//extern const UNIQUE_ID c_aJoinDevUID_LE;  // provisioned UID of the JD

extern UNIQUE_ID g_aJoinDevUID_BE, g_aJoinDevUID_LE;  
  
extern const uint8 c_aJDNickname[2];      // the nickname that will be allocated to JD
extern uint8 c_aJDLongTag[32];            // the long tag that will be allocated to JD
extern const uint8 c_aUninitializedTag[8];
extern const uint8 c_aUnicastSessionKey[16];
extern const uint8 c_aBroadcastSessionKey[16];
extern const uint8 c_aNetworkKey[16];

#define TXPOWER_10dBm       0x0C    // as ISA100 - for 10dBm
#define TXPOWER_5dBm        0x0B    // 
#define TXPOWER_0dBm        0x0A    // 

#define DEFAULT_TXPOWER       TXPOWER_10dBm
extern uint8 g_ucPAValue;


// RestartReason codes: g_stProv.aRestartReason[3]
// byte0 -the higher nibble contains the restart reason as the following enum
// byte0 -the lower nibble contains the value of g_stC769.wirelessMode upon restart (see CommonTable51)
// bytes1&2 -contain the value of g_stC769.joinStatus upon restart (see CommonTable52)
enum
{
  //RREASON_PWUP_RESET_WDOG = 0, //TODO: in case this reason is needed, please assign non 0 value!!!
  RREASON_NONE,
  RREASON_TOUT_TIMESRC,
  RREASON_TOUT_LOOSELY,
  RREASON_TOUT_RECEPTION,
  RREASON_CMD42,
  RREASON_CMD771,
  RREASON_CMD960,
  RREASON_CMD972,
  RREASON_NO_SUPERFRAME,
  RREASON_TOUT_BBR,
  RREASON_RESET_BBR,
  RREASON_OLD_ADVERTISE,
  RREASON_JOIN_REQ_EXCEEDED,
  RREASON_NO_ACK,
  RREASON_LOOP_DETECTED,
  RREASON_STACK_CORRUPTION,
  
  RREASON_NUM
};


// device status byte 
enum
{
  MASTER_WIRED_SEC_IDX = 0,
  MASTER_WIRED_PRIMARY_IDX,
  MASTER_NM_IDX,
  MASTER_GW_IDX,
  HANDHELD_IDX,
  MASTER_NO
};


typedef struct
{
/***************** 4-byte variables *****************/
  
  TIME_10MS ulDllKeepAlive;
  TIME_10MS ulDllPathFail;
  TIME_10MS ulDllAdvertise;
  TIME_10MS ulDllDiscovery;
  
  TIME_10MS ulHealthReportTime;
  TIME_10MS ulBcastReplyTime;
  TIME_10MS ulAdWaitTimeout;  // Ad -> Advertising
  TIME_10MS ulMaxPacketAge;
 
  TIME tDurationAtPeakPacketLoad;         // [1/32ms]
  TIME tTimeToRecoverFromPowerDrain;      // [1/32ms]
  TIME tMinRequiredKeepAliveTime;         // [1/32ms]
  
  uint32 ulFinalAssemblyNmb;     // Cmd19
  uint32 ulActiveSearchShedTime; // Cmd771
 
  float fPeakPacketsPerSecond;
  
  // RESERVE !!!
  uint32 aulReserve[5]; // ## DP(09.06.10): 5 Ids / 20 bytes reserved 
  float  afReserve[5];  // ## DP(09.06.10): 5 Ids / 20 bytes reserved 
  
/**************** 2-byte variables *****************/ 
  
  uint16 unConfigChangeCounter;           // configuration change counter
  uint16 unNetworkId;
  uint16 unManufacturerIDCode;            // CommonTable8
  uint16 unPrivateLabelDistributorCode;   // CommonTable8
  uint16 unMaxNoOfNeighbors;  
  uint16 unMaxNoOfPacketBuffers; 
  uint16 unDllTxTimeouts;                 // total number of abandoned transmissions (calculated Delay < warm-up time)  
  uint16 unDllRxTimeouts;                 // total number of late receptions
 
  // RESERVE !!!
  uint16 aunReserve[10]; // ## DP(09.06.10): 10 Ids / 20 bytes reserved 
  
/**************** 1-byte variables/arrays ***********/  
  
  uint8 ucDevicePollingAddr; // Cmd006
  uint8 ucDefaultTTL;
   
  uint8 aJoinSessionKey[16];
  uint8 aNetworkTag[32];
  uint8 aLongTag[32];
  uint8 aTag[6]; // Cmd18
  uint8 aMessage[24]; // device message (see universal Cmd12 and Cmd17)
  uint8 aDescriptor[12]; // device's descriptor, Cmd18
  uint8 aDate[3]; // Cmd18
//  uint8 aFinalAssemblyNmb[3]; // Cmd19
 
//doinel.alban - constant value:  uint8 ucMinReqPreamblesNo;  // Cmd0 - min required preambles for the request message from Master to Slave
//doinel.alban - constant value:  uint8 ucProtocolMajorRevNo;     // e.g. 7 for HART rev. 7
//doinel.alban - constant value:  uint8 ucDeviceRevisionLevel;
//doinel.alban - constant value:  uint8 ucSoftwareRevisionLevel;
  uint8 ucHardwareRevisionLevel;  // bits 7 to 3 of byte 7  // can have the same stack on different HW platform
//doinel.alban - constant value:  uint8 ucPhysicalSignalingCode;  // bits 2 to 0 of byte 7;  PhysicalSignalingCodes
//doinel.alban - constant value:  uint8 ucDeviceFlags;                    // CommonTable11
  uint8 ucMinRespPreamblesNo;             // Cmd0, Cmd59 - min preambles to be sent with the response message from Slave to Master
  uint8 ucMaxNoOfDeviceVars;
  uint8 ucDeviceProfile;                  // CommonTable57 DeviceProfileCodes
  uint8 ucPowerSource;                    // CommonTable44; bits 7-4 must be set to 0
  int8 cRSLThresholdGoodConnection;       // dB 
  
  uint8 ucJoinModeCode; // Cmd771
  uint8 ucWriteProtectCode; // CommonTable7 
  uint8 ucLockDeviceCode; // CommonTable18 
  uint8 ucLoopCurrentMode;   // see Common Table 16; 0=disabled; 1=enabled;
  uint8 ucIOSystemMasterMode; // 1 = primary master / 0 = secondary master
  uint8 ucIOSystemRetryCount; // number of retries to subdevice(valid range is 2 to 5)
  uint8 aDeviceStatus[MASTER_NO]; // device status byte
  
  uint8 aRestartReason[3];      // Reason for restart of device (b7-b4 of byte 0); 
                                // WirelessOperationMode value upon restart (b3-b0 of byte 0)-CommonTable51
                                // JoinStatus (bytes 1-2) -CommonTable52
  
  uint8 ucDLLQueueSize;         // Andy's request
  
  // RESERVE !!!  
  uint8 aucReserve[29]; // ## DP(03.02.11): 10 Ids / 29 bytes reserved 
  
} PROVISION_T;

// Sub-device identity summary(it belongs to I/O System)
typedef struct
{
  uint16   m_unSubdeviceIdx;
  uint8    m_ucIOCard;
  uint8    m_ucChannel;
  uint16   m_unManufacturerId;
  uint16   m_unExpandedDeviceTypeCode;
  uint8    m_aDeviceId[3];
  uint8    m_ucRevisionLevel; 
  uint8    m_aLongTag[32];
} SUBDEVICE_IDENTITY_T; 


// Burst persistent data structures BEGIN
typedef struct
{
  uint8     m_ucBurstModeControlCode; // see CommonTable 9
  uint8     m_aucDeviceVariableCodes[8]; // the published variables(Cmd9 or 33) 
  //uint8     m_ucBurstMsg; // burst message
  uint16    m_unCmdNo; // the published command number 
  TIME_10MS m_ulMinUpdateTime; // normal update interval
  TIME_10MS m_ulMaxUpdateTime; // max. update interval
  float     m_fTriggerValue; // trigger value
  uint8     m_ucBurstTriggerModeCode; // tigger mode, see Table 33
  uint8     m_ucClassification; // device variable classification for trigger value
  uint8     m_ucUnit; // units code
} NV_BURST_MODE_T; // persistent data for a burst pipe  

#if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )
// number of device variables supported: PV(4-20mA analog input), SV(temp), TV(humid) and QV(dew-point)
  #ifndef TEST_ADC_COUNTS
    #define NoOfDeviceVariables   4
  #else
    #define NoOfDeviceVariables   8
  #endif //TEST_ADC_COUNTS
  // number of dynamic variables supported
  #define NoOfDynamicVariables  4
#else
  // number of device variables supported
  #define NoOfDeviceVariables   3
  // number of dynamic variables supported
  #define NoOfDynamicVariables  3
#endif

// Burst Mode
#define MAX_BURST_NO 3

typedef struct 
{
  uint8       m_ucStatus;
  uint8       m_ucCode;
  uint32      m_ulTransducerSN;
  uint8       m_ucUnit;
  float       m_fUpperLimit;
  float       m_fLowerLimit;
  float       m_fDampingValue;
  float       m_fMinSpan;
  uint8       m_ucClassification;
  uint8       m_ucFamily;
  TIME_10MS   m_ulUpdateTimePeriod;
} NV_DEVICE_VARIABLE_T; // persistent data for a certain device variable 
// Burst persistent data structures END

// Event Notification persistent data structure BEGIN

// Event Notification
#define MAX_EVENTS_NO 1

typedef struct 
{
  TIME_10MS  m_ulRetryTime;         // retry time
  TIME_10MS  m_ulMaxUpdateTime;     // maximum update time
  TIME_10MS  m_ulDebounceInterval;  // event debounce interval
  uint8      m_ucEventControlCode;  // event notification control code(see Common Table 35)
  uint8      m_ucMaskSize;          // max. 26
  uint8      m_aucEventMasks[26];   // concatenation of event masks 
} NV_EVENT_NOTIFICATION_T;
// Event Notification persistent data structure BEGIN





typedef struct
{
  uint16     m_unOffset;  // offset of a certain NVM variable within its structure
  uint8      m_ucSize;    // Size of a certain NVM variable 
} NVM_TABLE_T;  

// last Id for PROVISION / BURST / EVENT 
#define PROVISION_LAST_ID   76
#define BURST_LAST_ID      106
#define EVENT_LAST_ID      115

// NVM table, empty entry definition
#define  EMPTY_ENTRY  {0,0}

// size of the NVM full record
#define  FULL_RECORD_SIZE  (sizeof(PROVISION_T) + 3*sizeof(NV_BURST_MODE_T) + sizeof(NV_EVENT_NOTIFICATION_T))

// the longest NVM individual record
#define MAX_INDIVIDUAL_RECORD 40 

// offset of a member "m" within a structure "s"
#define offsetof(s,m)    (uint16)&(((s *)0)->m)

// offset of a member "m" within an array of structures "s[i]"
#define x_offsetof(s,i,m)    ((uint16)&(((s *)0)->m) + i*sizeof(s))

//  The checksum algorithm for NVM: rotate the current sum (starting
//  with zero) one bit to left (true bit rotation) then xor the result
//  with the data byte.
typedef uint16 CKS_CHECKSUM_T;

#define CKS_CHECKSUM_MSB  (1UL<<((sizeof(CKS_CHECKSUM_T)*8)-1))

#define CKS_ALGORITHM(pSum,value)   do                                \
                                    {                                 \
                                      if(*(pSum)&CKS_CHECKSUM_MSB)    \
                                      {                               \
                                        *(pSum)<<=1;                  \
                                        *(pSum)|=1;                   \
                                      }                               \
                                      else                            \
                                      {                               \
                                        *(pSum)<<=1;                  \
                                      }                               \
                                      *(pSum)^=value;                 \
                                    }                                 \
                                    while(0)




// global offset within the current NVM-area
extern PROV_ADDR_TYPE g_ulNvmAreaOffset; 


extern PROVISION_T g_stProv;

// burst persistent data
extern NV_BURST_MODE_T g_stBurstModeTableNvData[MAX_BURST_NO];

// device variable persistent data
#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
  #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
   extern NV_DEVICE_VARIABLE_T g_stDeviceVariableNvData[ANALOG_CH_NO + DIGITAL_CH_NO];
  #else
   extern NV_DEVICE_VARIABLE_T g_stDeviceVariableNvData[LOCAL_ANALOG_CH_NO + ANALOG_CH_NO];
  #endif
#else
  //due to mapping dynamic on device vars, no use in keeping them doubled!!!
  extern NV_DEVICE_VARIABLE_T g_stDeviceVariableNvData[NoOfDeviceVariables + 2];
#endif 

// event notification persistent data
extern NV_EVENT_NOTIFICATION_T g_stEventsTableNvData[MAX_EVENTS_NO];  


// persistent data table
extern const NVM_TABLE_T c_aProvNvmTable[];

// Number of entries for NVM table 
#define NVM_TABLE_SIZE        (sizeof(c_aProvNvmTable)/sizeof(NVM_TABLE_T))

// Retrive pointer to NVM entry
#define NVM_GET_ENTRY(index)   ((index <= NVM_TABLE_SIZE) ? &c_aProvNvmTable[index-1] : NULL)

extern const NV_DEVICE_VARIABLE_T cDevVarInitData[];

void InitProvisionedData (void);

void NVM_WriteFullRecord( void );

void NVM_WriteIndividualRecord( uint8 p_ucId, uint8 *p_pStart, uint8 p_ucSize );

void NVM_ReadRecords( void );

void DoCristalCalibration (void);

void NVM_ReadPersistentData( uint8 *p_pucDst, PROV_ADDR_TYPE p_uAddr, uint16 p_unSize );

void NVM_WritePersistentData( uint8 *p_pucSrc, PROV_ADDR_TYPE p_uAddr, uint16 p_unSize );

#if( DEVICE_TYPE == DEV_TYPE_MC13225 )
  void NVM_EraseSector( uint32 p_ulSectorNmb );
#endif  

void NVM_SaveBeforeRestart(uint8 ucReason);

#endif // _NIVIS_PROVISION_H_
