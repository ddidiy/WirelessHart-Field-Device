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
* Name:         provision.c
* Author:       NIVIS LLC, 
* Date:         July 2008
* Description:  This file holds the provisioning / non-volatile data
* Changes:
* Revisions:
****************************************************************************************************/
#include "datalink_layer.h"
#include "datalink_layer_dm.h"
#include "network_layer.h"
#include "transport_layer.h"
#include "app_cmdresp.h"

#include "provision.h"
#include "../spif_interface.h"

PROVISION_T g_stProv;   // holds the provisioned data

NV_BURST_MODE_T g_stBurstModeTableNvData[MAX_BURST_NO];

#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
  #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
    NV_DEVICE_VARIABLE_T g_stDeviceVariableNvData[ANALOG_CH_NO + DIGITAL_CH_NO];
  #else
    NV_DEVICE_VARIABLE_T g_stDeviceVariableNvData[LOCAL_ANALOG_CH_NO + ANALOG_CH_NO];
  #endif   
#else
  //due to mapping dynamic on device vars, no use in keeping them doubled!!!
  NV_DEVICE_VARIABLE_T g_stDeviceVariableNvData[NoOfDeviceVariables + 2];
#endif  
  
NV_EVENT_NOTIFICATION_T g_stEventsTableNvData[MAX_EVENTS_NO];  

//const UNIQUE_ID c_aJoinDevUID_BE = { 0xF8, 0x70, 0x00, NET_ID>>8, DEV_ID };  // provisioned UID of the JD // UP_DEV_ID and DEV_ID are defined like: 0x1a
//const UNIQUE_ID c_aJoinDevUID_LE = { DEV_ID, NET_ID>>8, 0x00, 0x70, 0xF8 };  // 

#if defined( BACKBONE_SUPPORT )
  #define EXPANDED_DEVICE_TYPE 0xF870
#elif ( BOARD_TYPE == BOARD_TYPE_DEVELOPMENT )
  #if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API ) 
    #define EXPANDED_DEVICE_TYPE 0xF872
  #else
    #define EXPANDED_DEVICE_TYPE 0xF871
  #endif
#elif ((BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT))
  #if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API ) 
    #define EXPANDED_DEVICE_TYPE 0xF876
  #elif ( _UAP_TYPE == UAP_TYPE_WHART_API )
    #define EXPANDED_DEVICE_TYPE 0xF877
  #else
      #define EXPANDED_DEVICE_TYPE 0xF875
    #endif
#endif


// default MAC address
const UNIQUE_ID c_aDefaultUID_BE = { EXPANDED_DEVICE_TYPE>>8, (uint8)EXPANDED_DEVICE_TYPE, 0xFF, 0xFF, 0xFF };
const UNIQUE_ID c_aDefaultUID_LE = { 0xFF, 0xFF, 0xFF, (uint8)EXPANDED_DEVICE_TYPE, EXPANDED_DEVICE_TYPE>>8 }; 


UNIQUE_ID g_aJoinDevUID_BE, g_aJoinDevUID_LE; 

const uint8 c_aJDNickname[2] = { 0x01, 0x23 };        // the nickname that will be allocated to JD
const uint8 c_aUnicastSessionKey[16] =    { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF };
const uint8 c_aBroadcastSessionKey[16] =  { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF };
const uint8 c_aNetworkKey[16] =           { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF };
  //const uint8 c_aJoinSessionKey[16] =       { 0x23, 0x10, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // Voicu
  //const uint8 c_aJoinSessionKey[16] =       { 0xFF, 0xFD, 0xFB, 0xF9, 0xF7, 0xF5, 0xF3, 0xF1, 0xEF, 0xED, 0xEB, 0xE9, 0xE7, 0xE5, 0xE3, 0xE1 }; // Hannover
const uint8 c_aJoinSessionKey[16] =       { 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // HCF

uint8 c_aJDLongTag[32] = JD_LONG_TAG;     // the long tag that will be allocated to JD (version, UP_DEV_ID and DEV_ID will be updated later) 

const NV_DEVICE_VARIABLE_T cDevVarInitData[] =
  {
#if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT))
    //current data:
    { 
      0, //m_ucStatus
      DeviceVariableCodes_PrimaryVariable, //m_ucCode
      0, //m_ulTransducerSN
      UnitsCodes_Milliamperes, //m_ucUnit
      25, //m_fUpperLimit
      0, //m_fLowerLimit
      0, //m_fDampingValue
      0, //m_fMinSpan
      DeviceVariableClassificationCodes_Current, //m_ucClassification 
      DeviceVariableFamilyCodes_NotUsed, //m_ucFamily
      30*100 //30s //m_ulUpdateTimePeriod
    }, 
    #ifdef TEST_ADC_COUNTS    
      //output counts 1
      { 
        0, //m_ucStatus
        1, //m_ucCode
        0, //m_ulTransducerSN
        UnitsCodes_NotUsed, //m_ucUnit
        4096, //m_fUpperLimit
        0, //m_fLowerLimit
        0, //m_fDampingValue
        0, //m_fMinSpan
        DeviceVariableClassificationCodes_NotUsed, //m_ucClassification 
        DeviceVariableFamilyCodes_NotUsed, //m_ucFamily
        30*100 //30s //m_ulUpdateTimePeriod
      }, 
      //output counts 2
      { 
        0, //m_ucStatus
        2, //m_ucCode
        0, //m_ulTransducerSN
        UnitsCodes_NotUsed, //m_ucUnit
        4096, //m_fUpperLimit
        0, //m_fLowerLimit
        0, //m_fDampingValue
        0, //m_fMinSpan
        DeviceVariableClassificationCodes_NotUsed, //m_ucClassification 
        DeviceVariableFamilyCodes_NotUsed, //m_ucFamily
        30*100 //30s //m_ulUpdateTimePeriod
      }, 
      //output counts 3
      { 
        0, //m_ucStatus
        3, //m_ucCode
        0, //m_ulTransducerSN
        UnitsCodes_NotUsed, //m_ucUnit
        4096, //m_fUpperLimit
        0, //m_fLowerLimit
        0, //m_fDampingValue
        0, //m_fMinSpan
        DeviceVariableClassificationCodes_NotUsed, //m_ucClassification 
        DeviceVariableFamilyCodes_NotUsed, //m_ucFamily
        30*100 //30s //m_ulUpdateTimePeriod
      }, 
      //output counts 4
      { 
        0, //m_ucStatus
        4, //m_ucCode
        0, //m_ulTransducerSN
        UnitsCodes_NotUsed, //m_ucUnit
        4096, //m_fUpperLimit
        0, //m_fLowerLimit
        0, //m_fDampingValue
        0, //m_fMinSpan
        DeviceVariableClassificationCodes_NotUsed, //m_ucClassification 
        DeviceVariableFamilyCodes_NotUsed, //m_ucFamily
        30*100 //30s //m_ulUpdateTimePeriod
      },
    #endif //TEST_ADC_COUNTS
#endif //(BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) 

#if ( BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT )
    //temperature data:
    { 
      0, //m_ucStatus
#if (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) 
      DeviceVariableCodes_SecondaryVariable, //m_ucCode
#else   
      DeviceVariableCodes_PrimaryVariable, //m_ucCode
#endif
      0, //m_ulTransducerSN
      UnitsCodes_DegreesCelsius, //m_ucUnit
      40, //m_fUpperLimit
      0, //m_fLowerLimit
      0, //m_fDampingValue
      0, //m_fMinSpan
      DeviceVariableClassificationCodes_Temperature, //m_ucClassification 
      DeviceVariableFamilyCodes_Temperature, //m_ucFamily
      30*100 //30s //m_ulUpdateTimePeriod
    },
    //humidity data:
    { //humidity data
      0,
#if (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) 
      DeviceVariableCodes_TertiaryVariable, //m_ucCode
#else   
      DeviceVariableCodes_SecondaryVariable, //m_ucCode
#endif
      0, //m_ulTransducerSN
      UnitsCodes_Percent, //m_ucUnit
      100, //m_fUpperLimit
      0, //m_fLowerLimit
      0, //m_fDampingValue
      0, //m_fMinSpan
      DeviceVariableClassificationCodes_Analytical, //m_ucClassification 
      DeviceVariableFamilyCodes_NotUsed, //m_ucFamily
      30*100 //30s //m_ulUpdateTimePeriod
    },  
  //dew point data:
    { 
      0, //m_ucStatus
#if (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT)
      DeviceVariableCodes_QuaternaryVariable, //m_ucCode
#else   
      DeviceVariableCodes_TertiaryVariable, //m_ucCode
#endif
      0, //m_ulTransducerSN
      UnitsCodes_DegreesCelsius, //m_ucUnit
      40, //m_fUpperLimit
      0, //m_fLowerLimit
      0, //m_fDampingValue
      0, //m_fMinSpan
      DeviceVariableClassificationCodes_Temperature, //m_ucClassification 
      DeviceVariableFamilyCodes_Temperature, //m_ucFamily
      30*100 //30s //m_ulUpdateTimePeriod
    },
#endif

    //percent data:
    { 
      0, //m_ucStatus
      DeviceVariableCodes_PercentRange, //m_ucCode
      0, //m_ulTransducerSN
      UnitsCodes_Percent, //m_ucUnit
      100, //m_fUpperLimit
      0, //m_fLowerLimit
      0, //m_fDampingValue
      0, //m_fMinSpan
      DeviceVariableClassificationCodes_Level, //m_ucClassification 
      DeviceVariableFamilyCodes_Level, //m_ucFamily
      30*100 //30s //m_ulUpdateTimePeriod
    },
    //loop current data:
    { 
      0, //m_ucStatus
      DeviceVariableCodes_LoopCurrent, //m_ucCode
      0, //m_ulTransducerSN
      UnitsCodes_Milliamperes, //m_ucUnit
      20, //m_fUpperLimit
      4, //m_fLowerLimit
      0, //m_fDampingValue
      0, //m_fMinSpan
      DeviceVariableClassificationCodes_Level, //m_ucClassification 
      DeviceVariableFamilyCodes_Level, //m_ucFamily
      30*100 //30s //m_ulUpdateTimePeriod
    } 
  };


// persistent data table
#pragma diag_suppress=Pe767 // optimisation reasons(used space) -> disable Warning[Pe767]: conversion from pointer to smaller integer
const NVM_TABLE_T c_aProvNvmTable[]={
/* [PROVISION_T] */
/* 1-byte variables/arrays */
  { offsetof(PROVISION_T, ucDevicePollingAddr),          sizeof(g_stProv.ucDevicePollingAddr)           }, // Id=1 (Id=0 -> full record)
  { offsetof(PROVISION_T, ucIOSystemMasterMode),         sizeof(g_stProv.ucIOSystemMasterMode)          }, // Id=2
  { offsetof(PROVISION_T, ucIOSystemRetryCount),         sizeof(g_stProv.ucIOSystemRetryCount)          }, // Id=3
  { offsetof(PROVISION_T, ucLoopCurrentMode),            sizeof(g_stProv.ucLoopCurrentMode)             }, // Id=4
  { offsetof(PROVISION_T, ucMinRespPreamblesNo),         sizeof(g_stProv.ucMinRespPreamblesNo)          }, // Id=5
  { offsetof(PROVISION_T, ucDefaultTTL),                 sizeof(g_stProv.ucDefaultTTL)                  }, // Id=6  // after reset TTL must be 32 (default), no need to be kept in NVM -see TML211A
  { offsetof(PROVISION_T, ucJoinModeCode),               sizeof(g_stProv.ucJoinModeCode)                }, // Id=7
  { offsetof(PROVISION_T, cRSLThresholdGoodConnection),  sizeof(g_stProv.cRSLThresholdGoodConnection)   }, // Id=8
  { offsetof(PROVISION_T, ucHardwareRevisionLevel),      sizeof(g_stProv.ucHardwareRevisionLevel)       }, // Id=9
  { offsetof(PROVISION_T, ucMaxNoOfDeviceVars),          sizeof(g_stProv.ucMaxNoOfDeviceVars)           }, // Id=10
  { offsetof(PROVISION_T, ucDeviceProfile),              sizeof(g_stProv.ucDeviceProfile)               }, // Id=11
  { offsetof(PROVISION_T, ucPowerSource),                sizeof(g_stProv.ucPowerSource)                 }, // Id=12
  { offsetof(PROVISION_T, ucLockDeviceCode),             sizeof(g_stProv.ucLockDeviceCode)              }, // Id=13
  { offsetof(PROVISION_T, ucWriteProtectCode),           sizeof(g_stProv.ucWriteProtectCode)            }, // Id=14

  { offsetof(PROVISION_T, aDate),                        sizeof(g_stProv.aDate)                         }, // Id=15
  { offsetof(PROVISION_T, aTag),                         sizeof(g_stProv.aTag)                          }, // Id=16
  { offsetof(PROVISION_T, aDescriptor),                  sizeof(g_stProv.aDescriptor)                   }, // Id=17  
  { offsetof(PROVISION_T, aJoinSessionKey),              sizeof(g_stProv.aJoinSessionKey)               }, // Id=18
  { offsetof(PROVISION_T, aLongTag),                     sizeof(g_stProv.aLongTag)                      }, // Id=19
  { offsetof(PROVISION_T, aNetworkTag),                  sizeof(g_stProv.aNetworkTag)                   }, // Id=20 
  { offsetof(PROVISION_T, aMessage),                     sizeof(g_stProv.aMessage)                      }, // Id=21
  { offsetof(PROVISION_T, aDeviceStatus),                sizeof(g_stProv.aDeviceStatus)                 }, // Id=22
  { offsetof(PROVISION_T, aRestartReason),               sizeof(g_stProv.aRestartReason)                }, // Id=23
  //---------> Andy's request
  { offsetof(PROVISION_T, ucDLLQueueSize),               sizeof(g_stProv.ucDLLQueueSize)                }, // Id=24
// reserve(10)
  EMPTY_ENTRY, // Id=25
  EMPTY_ENTRY, // Id=26
  EMPTY_ENTRY, // Id=27
  EMPTY_ENTRY, // Id=28
  EMPTY_ENTRY, // Id=29
  EMPTY_ENTRY, // Id=30
  EMPTY_ENTRY, // Id=31
  EMPTY_ENTRY, // Id=32
  EMPTY_ENTRY, // Id=33
  EMPTY_ENTRY, // Id=34
    
/* 2-byte variables */ 
  { offsetof(PROVISION_T, unNetworkId),                  sizeof(g_stProv.unNetworkId)                   }, // Id=35
  { offsetof(PROVISION_T, unConfigChangeCounter),        sizeof(g_stProv.unConfigChangeCounter)         }, // Id=36
  { offsetof(PROVISION_T, unManufacturerIDCode),         sizeof(g_stProv.unManufacturerIDCode)          }, // Id=37
  { offsetof(PROVISION_T, unPrivateLabelDistributorCode),sizeof(g_stProv.unPrivateLabelDistributorCode) }, // Id=38
  { offsetof(PROVISION_T, unMaxNoOfNeighbors),           sizeof(g_stProv.unMaxNoOfNeighbors)            }, // Id=39
  { offsetof(PROVISION_T, unMaxNoOfPacketBuffers),       sizeof(g_stProv.unMaxNoOfPacketBuffers)        }, // Id=40
  { offsetof(PROVISION_T, unDllTxTimeouts),              sizeof(g_stProv.unDllTxTimeouts)               }, // Id=41
  { offsetof(PROVISION_T, unDllRxTimeouts),              sizeof(g_stProv.unDllRxTimeouts)               }, // Id=42
// reserve(10)
  EMPTY_ENTRY, // Id=43
  EMPTY_ENTRY, // Id=44
  EMPTY_ENTRY, // Id=45
  EMPTY_ENTRY, // Id=46
  EMPTY_ENTRY, // Id=47
  EMPTY_ENTRY, // Id=48
  EMPTY_ENTRY, // Id=49
  EMPTY_ENTRY, // Id=50
  EMPTY_ENTRY, // Id=51
  EMPTY_ENTRY, // Id=52

/* 4-byte variables */ 
  { offsetof(PROVISION_T, ulFinalAssemblyNmb),            sizeof(g_stProv.ulFinalAssemblyNmb)           }, // Id=53
  { offsetof(PROVISION_T, ulActiveSearchShedTime),        sizeof(g_stProv.ulActiveSearchShedTime)       }, // Id=54
  { offsetof(PROVISION_T, ulDllKeepAlive),                sizeof(g_stProv.ulDllKeepAlive)               }, // Id=55
  { offsetof(PROVISION_T, ulDllPathFail),                 sizeof(g_stProv.ulDllPathFail)                }, // Id=56
  { offsetof(PROVISION_T, ulDllAdvertise),                sizeof(g_stProv.ulDllAdvertise)               }, // Id=57
  { offsetof(PROVISION_T, ulDllDiscovery),                sizeof(g_stProv.ulDllDiscovery)               }, // Id=58
  { offsetof(PROVISION_T, ulHealthReportTime),            sizeof(g_stProv.ulHealthReportTime)           }, // Id=59
  { offsetof(PROVISION_T, ulBcastReplyTime),              sizeof(g_stProv.ulBcastReplyTime)             }, // Id=60
  { offsetof(PROVISION_T, ulAdWaitTimeout),               sizeof(g_stProv.ulAdWaitTimeout)              }, // Id=61
  { offsetof(PROVISION_T, ulMaxPacketAge),                sizeof(g_stProv.ulMaxPacketAge)               }, // Id=62
  { offsetof(PROVISION_T, tDurationAtPeakPacketLoad),     sizeof(g_stProv.tDurationAtPeakPacketLoad)    }, // Id=63
  { offsetof(PROVISION_T, tTimeToRecoverFromPowerDrain),  sizeof(g_stProv.tTimeToRecoverFromPowerDrain) }, // Id=64
  { offsetof(PROVISION_T, tMinRequiredKeepAliveTime),     sizeof(g_stProv.tMinRequiredKeepAliveTime)    }, // Id=65
  { offsetof(PROVISION_T, fPeakPacketsPerSecond),         sizeof(g_stProv.fPeakPacketsPerSecond)        }, // Id=66
// reserve uint32(5)  
  EMPTY_ENTRY, // Id=67
  EMPTY_ENTRY, // Id=68
  EMPTY_ENTRY, // Id=69
  EMPTY_ENTRY, // Id=70
  EMPTY_ENTRY, // Id=71
// reserve float(5)  
  EMPTY_ENTRY, // Id=72
  EMPTY_ENTRY, // Id=73
  EMPTY_ENTRY, // Id=74
  EMPTY_ENTRY, // Id=75
  EMPTY_ENTRY, // Id=76

/* [NV_BURST_MODE_T] */
/* Burst 0 */
  { offsetof(NV_BURST_MODE_T, m_ulMinUpdateTime),              2*sizeof(uint32)                         }, // Cmd 103, Id=77 
  { offsetof(NV_BURST_MODE_T, m_fTriggerValue),                sizeof(uint32) + 3*sizeof(uint8)         }, // Cmd 104, Id=78
  { offsetof(NV_BURST_MODE_T, m_aucDeviceVariableCodes),       8*sizeof(uint8)                          }, // Cmd 107, Id=79   
  { offsetof(NV_BURST_MODE_T, m_unCmdNo),                      sizeof(uint16)                           }, // Cmd 108, Id=80  
  { offsetof(NV_BURST_MODE_T, m_ucBurstModeControlCode),       sizeof(uint8)                            }, // Cmd 109, Id=81

/* Burst 1 */
  { x_offsetof(NV_BURST_MODE_T, 1, m_ulMinUpdateTime),         2*sizeof(uint32)                         }, // Cmd 103, Id=82
  { x_offsetof(NV_BURST_MODE_T, 1, m_fTriggerValue),           sizeof(uint32) + 3*sizeof(uint8)         }, // Cmd 104, Id=83
  { x_offsetof(NV_BURST_MODE_T, 1, m_aucDeviceVariableCodes),  8*sizeof(uint8)                          }, // Cmd 107, Id=84   
  { x_offsetof(NV_BURST_MODE_T, 1, m_unCmdNo),                 sizeof(uint16)                           }, // Cmd 108, Id=85  
  { x_offsetof(NV_BURST_MODE_T, 1, m_ucBurstModeControlCode),  sizeof(uint8)                            }, // Cmd 109, Id=86

/* Burst 2 */  
  { x_offsetof(NV_BURST_MODE_T, 2, m_ulMinUpdateTime),         2*sizeof(uint32)                         }, // Cmd 103, Id=87
  { x_offsetof(NV_BURST_MODE_T, 2, m_fTriggerValue),           sizeof(uint32) + 3*sizeof(uint8)         }, // Cmd 104, Id=88
  { x_offsetof(NV_BURST_MODE_T, 2, m_aucDeviceVariableCodes),  8*sizeof(uint8)                          }, // Cmd 107, Id=89   
  { x_offsetof(NV_BURST_MODE_T, 2, m_unCmdNo),                 sizeof(uint16)                           }, // Cmd 108, Id=90  
  { x_offsetof(NV_BURST_MODE_T, 2, m_ucBurstModeControlCode),  sizeof(uint8)                            }, // Cmd 109, Id=91
// reserve
  EMPTY_ENTRY, // Id=92
  EMPTY_ENTRY, // Id=93
  EMPTY_ENTRY, // Id=94
  EMPTY_ENTRY, // Id=95
  EMPTY_ENTRY, // Id=96
  EMPTY_ENTRY, // Id=97
  EMPTY_ENTRY, // Id=98
  EMPTY_ENTRY, // Id=99
  EMPTY_ENTRY, // Id=100
  EMPTY_ENTRY, // Id=101
  EMPTY_ENTRY, // Id=102
  EMPTY_ENTRY, // Id=103
  EMPTY_ENTRY, // Id=104
  EMPTY_ENTRY, // Id=105
  EMPTY_ENTRY, // Id=106


/* [NV_EVENT_NOTIFICATION_T] */ 
  { offsetof(NV_EVENT_NOTIFICATION_T, m_ucMaskSize),           (1+26)*sizeof(uint8)                     }, // Cmd 116, Id=107
  { offsetof(NV_EVENT_NOTIFICATION_T, m_ulRetryTime),          3*sizeof(uint32)                         }, // Cmd 117, Id=108                                                                   
  { offsetof(NV_EVENT_NOTIFICATION_T, m_ucEventControlCode),   sizeof(uint8)                            }, // Cmd 118, Id=109
// reserve
  EMPTY_ENTRY, // Id=110
  EMPTY_ENTRY, // Id=111
  EMPTY_ENTRY, // Id=112
  EMPTY_ENTRY, // Id=113
  EMPTY_ENTRY, // Id=114
  EMPTY_ENTRY, // Id=115
};  

// global offset within the current NVM-area
PROV_ADDR_TYPE g_ulNvmAreaOffset;

// HART Unique Id
uint8 g_aHART_UID[3];

uint8 g_ucPAValue = DEFAULT_TXPOWER;    // radio Tx power value

void BuildLongTag (void)
{
#ifdef FORCE_LONGTAG_HCF_DUT  
  {
    memcpy(g_stProv.aLongTag, "HCF-DUT", 7);
    memset(g_stProv.aLongTag + 7, 0, sizeof(g_stProv.aLongTag) - 7);
  }
#else
  
  c_aJDLongTag[20] = MAJOR_VERSION + '0';
  c_aJDLongTag[21] = MINOR_VERSION + '0';
  uint8 c = BUILD >> 4;                   // BUILD is defined like: 0x11, which means decimal 11
  c_aJDLongTag[22] = c + '0';
  c = BUILD & 0x0F;
  c_aJDLongTag[23] = c + '0';
  c_aJDLongTag[24] = BUILD_LETTER;
  
  c = g_aHART_UID[0] >> 4;                  
  c_aJDLongTag[26] = (c > 9) ? c + '7' : c + '0';
  c = g_aHART_UID[0] & 0x0F;
  c_aJDLongTag[27] = (c > 9) ? c + '7' : c + '0';
  
  c = g_aHART_UID[1] >> 4;                  
  c_aJDLongTag[28] = (c > 9) ? c + '7' : c + '0';
  c = g_aHART_UID[1] & 0x0F;
  c_aJDLongTag[29] = (c > 9) ? c + '7' : c + '0';
  
  c = g_aHART_UID[2] >> 4;                  
  c_aJDLongTag[30] = (c > 9) ? c + '7' : c + '0';
  c = g_aHART_UID[2] & 0x0F;
  c_aJDLongTag[31] = (c > 9) ? c + '7' : c + '0';
  
  memcpy(g_stProv.aLongTag, c_aJDLongTag, 32);
#endif
}

void ReadManufacturing(void)
{
  uint8 aVersion[2];
  
  // default MAC address
  memcpy( g_aJoinDevUID_BE, c_aDefaultUID_BE, 5);
  memcpy( g_aJoinDevUID_LE, c_aDefaultUID_LE, 5);
  
  // read format version
  NVM_ReadPersistentData( aVersion, MANUFACTURING_START_ADDR, 2);
  
  if( (*(uint16*)aVersion & 0xFEFE) == 0 ) // valid version?
  { // HART_UID = the last 3 bytes of m_aMAC
    NVM_ReadPersistentData( g_aHART_UID, MANUFACTURING_START_ADDR + offsetof(T_MANUFACTURING, m_aMAC) + 5, 3 );
 
    // TBD: device manufacturing for NIVIS customers   
    #ifdef AUTO_MANUFACTURING    
        if( (g_aHART_UID[0] == 0xFF) && (g_aHART_UID[1] == 0xFF) && (g_aHART_UID[2] == 0xFF) )
        { // no device ID
          NVM_ReadPersistentData( g_aHART_UID, MANUFACTURING_HART_START_ADDR, 3 );
          // only one entire NVM-sector can be written, not enough RAM for that!!
          //NVM_WritePersistentData( g_aHART_UID, MANUFACTURING_START_ADDR + offsetof(T_MANUFACTURING, m_aMAC) + 5, 3);
        }  
    #endif    
    
    // copy the manufactured MAC address 
    memcpy( g_aJoinDevUID_BE + 2, g_aHART_UID, 3);
    InverseMemcpy( g_aJoinDevUID_LE, g_aHART_UID, 3); 
  }   
}

///////////////////////////////////////////////////////////////////////////////////
// Name: InitProvisionedData
// Author: NIVIS LLC
// Description: Init function for the provisioned data
// Parameters: none
// Return: None
// Obs:
//      Access level: user level, before the other HART stack initializations
///////////////////////////////////////////////////////////////////////////////////
void InitProvisionedData (void)
{  
  // Physical Layer 
  // ... nothing for the moment    
  
  // Data Link Layer  
//  g_stProv.unNetworkId = 0x09FB; //doinel.alban - for Voicu
//  g_stProv.unNetworkId = 0xE002; //for RD    // DLL, Table 2: 0xE000 - 0xEFFF for non-public networks used by device manufacturers
//  g_stProv.unNetworkId = 0x2747; //doinel.alban - for Hannover    
//  g_stProv.unNetworkId = 0x1234; //doinel.alban - for HCF test
//  g_stProv.unNetworkId = 0x4321; //doinel.alban - for second test setup
//  g_stProv.unNetworkId = 0x3333; //doinel.alban - for the third test setup
//  g_stProv.unNetworkId = 0x5555; //doinel.alban - for the QA setup
  
#ifndef NET_ID
  #define NET_ID  0x3333
#endif
  
  g_stProv.unNetworkId = NET_ID;    // NET_ID=0xnnnn, defined in project options
  g_unNetworkId = g_stProv.unNetworkId; // the actual value used by the DLL
  
  g_stProv.ulActiveSearchShedTime = DLL_ACTIVE_SEARCH_TIME; // 4000s  //

  // use the default values in c_tTMinMax[].ulDefault
//  g_stProv.ulDllDiscovery = 1000;   // 10s   
//  g_stProv.ulDllAdvertise = 90;     // 90*10ms = 900ms
//  g_stProv.ulDllKeepAlive = 3000;   // 3000*10ms = 30s  
//  g_stProv.ulDllPathFail  = 100*100; // 100*100*10ms = 100s
//  g_stProv.ulHealthReportTime = 15*60*4;  // 15*60*4*250ms = 15min. 
//  g_stProv.ulBcastReplyTime = 6000;         // 60*100*10ms = 60s
//  g_stProv.ulMaxPacketAge = 30000;          // 300*100*10ms = 300s
  
  // init the 7 timer intervals written by Cmd795 (see CommonTable 43)
  for (uint8 i=0; i < (sizeof(c_tTMinMax) / sizeof(c_tTMinMax[0])); i++)
  {
    *c_tTMinMax[i].pStore = c_tTMinMax[i].ulDefault;
  }

  //g_stProv.ulHealthReportTime = 3*60*4;     // 3 min !!! for testing only !!!
#ifdef BACKBONE_SUPPORT
  #ifdef TEST_09FB
  g_stProv.ulDllAdvertise = 5;     // 5*10ms = 50ms
  #else
  g_stProv.ulDllAdvertise = 177;     // 177*10ms = 1770ms
  #endif
#endif
  
  // Network Layer 
//  uint8 ucIdx;
//  for (ucIdx=0; ucIdx<16; ucIdx++)
//    g_stProv.aJoinSessionKey[ucIdx]=0xFF-2*ucIdx;
  memcpy(g_stProv.aJoinSessionKey, c_aJoinSessionKey, 16); //doinel.alban - for HCF Join test
  
  
#ifdef FOR_SALE_MANUFACTURING
  memset(g_stProv.aLongTag, '?', 32);
  g_stProv.ucJoinModeCode = JoinModeCode_DontAttemptToJoin; 
  g_ucJoinMode = JoinModeCode_DontAttemptToJoin;
#else
  BuildLongTag(); 
  g_stProv.ucJoinModeCode = JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset; 
  g_ucJoinMode = JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset;
#endif
  
  //memcpy(g_stProv.aUniqueIdBE, g_aJoinDevUID_BE, 5);
  //memcpy(g_stProv.aUniqueIdLE, g_aJoinDevUID_LE, 5);
  
  memset(g_stProv.aTag, 0xff, sizeof(g_stProv.aTag));  // TODO: non-volatile memory
  memset(g_stProv.aMessage, 0xff, sizeof(g_stProv.aMessage));
  memset(g_stProv.aDescriptor, 0xff, sizeof(g_stProv.aDescriptor));
  
  g_stProv.ulAdWaitTimeout = 30*4;           // the normal value is 30*4*250ms = 30s //[doinel.alban] TODO: move it out from NVM area as a constant
  g_stProv.ucDefaultTTL = 32;           // after reset TTL must be 32 (default), no need to be kept in NVM -see TML211A //[doinel.alban] TODO: move it out from NVM area as a constant
  
  // Application Layer 
#ifdef USE_WRITE_PROTECT_MODE 
  g_stProv.ucWriteProtectCode = WriteProtectCodes_NO;         // should be get from non-volatile memory
#else
  g_stProv.ucWriteProtectCode = WriteProtectCodes_None;       // WriteProtect Not Implemented
#endif 
  
  memset(g_stProv.aDeviceStatus, 0, sizeof(g_stProv.aDeviceStatus));
  
  APP_SetDeviceStatusFlag(COLD_START);
  
#ifdef USE_LOOP_CONTROL
  g_stProv.ucDevicePollingAddr = 0; 
  g_stProv.ucLoopCurrentMode = LoopCurrentModeCodes_Enabled; // CommonTable16
#else
  g_stProv.ucDevicePollingAddr = 0x01;  //g_aHART_UID[2] & 0x3F; 
  g_stProv.ucLoopCurrentMode = LoopCurrentModeCodes_Disabled; // CommonTable16
  #ifndef HCF_LOOP_CURRENT_WA    
  APP_SetDeviceStatusFlag(LOOP_CURRENT_FIXED);
  #endif //#ifndef HCF_LOOP_CURRENT_WA    
#endif 

  g_stProv.unConfigChangeCounter = 0;       // Cmd_000
  g_stProv.ucHardwareRevisionLevel = 1;     // Cmd_000                                    //[doinel.alban] TODO: move it out from NVM area as a manufacturing data?
  g_stProv.ucMinRespPreamblesNo = 5;        // Cmd_000, Cmd_059                           //[doinel.alban] TODO: move it out from NVM area as a constant
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )  
  g_stProv.ucMaxNoOfDeviceVars = 0; //to be filled up at GetInitialInfo
#else  
  g_stProv.ucMaxNoOfDeviceVars = NoOfDeviceVariables;         // Cmd_000,                                   //[doinel.alban] TODO: differentiate this val by device type and move it out from NVM area as a constant
#endif
  
  // Nivis internal, fake codes
  g_stProv.unManufacturerIDCode = 0x00E1; // Cmd_000 // CommonTable8          //[doinel.alban] TODO: move it out from NVM area as a constant 
  g_stProv.unPrivateLabelDistributorCode = 0x00E2; // Cmd_000 // CommonTable8 //[doinel.alban] TODO: move it out from NVM area as a constant or manufacturing data?
    
#if( WIHARTDEV_TYPE == WIHARTDEV_TYPE_FIELDDEV )                                          //[doinel.alban] TODO: differentiate this val by device type and move it out from NVM area as a constant
  g_stProv.ucDeviceProfile = DeviceProfileCodes_WIRELESSHART_PROCESS_AUTOMATION_DEVICE;   // CommonTable57 
#elif( WIHARTDEV_TYPE == WIHARTDEV_TYPE_ACCPOINT )
  g_stProv.ucDeviceProfile = DeviceProfileCodes_WIRELESSHART_GATEWAY;                     // CommonTable57 
#else
  #error "Unsupported WirelessHART device type"
//  g_stProv.ucDeviceProfile = 0;   // invalid value! 
#endif
  
  // configurable constant data (cannot be set by HART commands) //[doinel.alban] TODO: differentiate this vals by device type (if necessary) and move them out from NVM area as constants
  g_stProv.ucPowerSource = C777_DEVICE_POWER_SOURCE;
  g_stProv.fPeakPacketsPerSecond = C777_DEVICE_PEAK_PACKETS_PER_SECOND;
  g_stProv.tDurationAtPeakPacketLoad = C777_DURATION_AT_PEAK_PACKET_LOAD;
  g_stProv.tTimeToRecoverFromPowerDrain = C777_TIME_TO_RECOVER_FROM_POWER_DRAIN;
  g_stProv.cRSLThresholdGoodConnection = C777_RSL_THRESHOLD_GOOD_CONNECTION;
  g_stProv.tMinRequiredKeepAliveTime = C777_MIN_REQUIRED_KEEP_ALIVE_TIME;
  g_stProv.unMaxNoOfNeighbors = DLL_MAX_NEIGHBORS;
  g_stProv.unMaxNoOfPacketBuffers = DLL_MAX_PACKET_BUFFERS;
  g_stProv.ucIOSystemRetryCount = CMD088_MIN_IOSYS_RETRY_COUNTER;
  g_stProv.ucIOSystemMasterMode = 1; //primary IOSys master  
  // TBD
}
  
////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: NVM_ReadPersistentData
// Author: NIVIS LLC  
// Description: Generic function for read non-volatile data 
// Parameters: p_pucDst  - destination buffer
//             p_uAddr   - address 
//             p_unSize  - number of bytes to read
// Return: none
// Obs:
//      Access level: user level
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void NVM_ReadPersistentData( uint8 *p_pucDst, PROV_ADDR_TYPE p_uAddr, uint16 p_unSize )
{ 
#if( DEVICE_TYPE == DEV_TYPE_MC13225 )
  NVM_FlashRead(p_pucDst, p_uAddr, p_unSize);
#endif  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: NVM_WritePersistentData
// Author: NIVIS LLC  
// Description: Generic function for write non-volatile data  
// Parameters: p_pucSrc - source buffer
//             p_uAddr  - address
//             p_unSize - number of bytes to write
// Return: none
// Obs:
//      Access level: user level
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void NVM_WritePersistentData( uint8 *p_pucSrc, PROV_ADDR_TYPE p_uAddr, uint16 p_unSize )
{ 
#if( DEVICE_TYPE == DEV_TYPE_MC13225 )
  NVM_FlashWrite((void*)p_pucSrc, p_uAddr, p_unSize); 
#endif  
}
  

#if( DEVICE_TYPE == DEV_TYPE_MC13225 )

////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: NVM_EraseSector
// Author: NIVIS LLC    
// Description: Generic function for clear non-volatile data  
// Parameters: p_ulSectorNmb - sector number 
// Return: none
// Obs:
//      Access level: user level
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void NVM_EraseSector( uint32 p_ulSectorNmb )
{  
  uint32 ulAddr = (p_ulSectorNmb == NVM_AREA1_NO) ? NVM_AREA1_START_ADDR : NVM_AREA2_START_ADDR; 
  
  if( NVM_FlashBlankCheck(ulAddr, SECTOR_SIZE) == gNvmErrBlankCheckError_c ) 
    NVM_FlashErase(p_ulSectorNmb);  
}

#endif

///////////////////////////////////////////////////////////////////////////////////
// Name: NVM_GetChecksum
// Author: NIVIS LLC
// Description: Computes the checksum of a certain memory object.
//              The implemented algorithm rotates the current sum (starting
//              with zero suggested) one bit to left (true bit rotation) 
//              then xor the result with the data byte
// Parameters: p_pStart - Pointer to the first byte of the memory object
//             p_unSize - Size of the memory object
//             p_unCksInit - initial value for checksum algo
// Return: checksum
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
CKS_CHECKSUM_T NVM_GetChecksum( void *p_pStart, uint16 p_unSize, uint16 p_unCksInit )
{ 
  uint8  *p;
  uint16 sum = p_unCksInit;

  p=p_pStart;
  while(p_unSize--)
  {
    CKS_ALGORITHM( &sum, *p );
    p++;
  }
  return sum;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: DoCristalCalibration
// Author: NIVIS LLC
// Description: calibrate the 24MHz clock
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void DoCristalCalibration (void)
{
  
//  uint8 ucOrigCTune = CRM_XTAL_CNTL.cTune;
  uint8 ucFinalCTune;
  uint8 aVersion[2];
  
  NVM_ReadPersistentData(aVersion, MANUFACTURING_START_ADDR, 2);
  
  if( (*(uint16*)aVersion != 0xFFFF) && ((*(uint16*)aVersion & 0xFEFE) == 0) ) // valid manufacturing version?
  {    
    NVM_ReadPersistentData(&ucFinalCTune, MANUFACTURING_START_ADDR + offsetof(T_MANUFACTURING,m_ucCristal), 1);  
    
    ucFinalCTune = (0x18 + ucFinalCTune) & 0x001F; // m_ucCristal
    CRM_XTAL_CNTL.fTune = 0x0F;
    
//    if( (ucOrigCTune ^ ucFinalCTune) & 0x10 ) // 4 pF capacitor is different, activate it first
    {
      DelayLoop(10*8000); // 10 ms
//      CRM_XTAL_CNTL.cTune = (ucFinalCTune & 0x10) | (ucOrigCTune & 0x0F);
      CRM_XTAL_CNTL.bulktune = (ucFinalCTune >> 4) & 0x01; // set bulk tune first (4 pF Capacitor)
    }
      
    DelayLoop(10*8000); // 10 ms
    CRM_XTAL_CNTL.cTune = ucFinalCTune & 0x0F;
  }
  
}  

///////////////////////////////////////////////////////////////////////////////////
// Name: NVM_WriteIndividualRecord
// Author: NIVIS LLC
// Description: write a certain individual record in FLASH
// Parameters: p_ucId   - NVM Identifier
//             p_pStart - Pointer to the first byte of the memory object
//             p_ucSize - Size of the memory object
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void NVM_WriteIndividualRecord( uint8 p_ucId, uint8 *p_pStart, uint8 p_ucSize )
{
  uint8  aucRecord[MAX_INDIVIDUAL_RECORD], 
         *p;
  uint16 unCks;
  uint32 ulValue = g_ulNvmAreaOffset;
  
   if( g_ulNvmAreaOffset < NVM_AREA2_START_ADDR )
   {
     ulValue -= NVM_AREA1_START_ADDR;
   }
   else
   {
     ulValue -= NVM_AREA2_START_ADDR;
   }  
  
  // place-check for the new individual record
  if( ulValue <= (NVM_AREA_SIZE - p_ucSize - 5) )
  {  
    p = aucRecord;
    *p++ = p_ucId;  // Id
    *p++ = g_stProv.unConfigChangeCounter >> 8; // CC
    *p++ = g_stProv.unConfigChangeCounter; 
    memcpy(p, p_pStart, p_ucSize); // Data
    p += p_ucSize;
    unCks = NVM_GetChecksum(aucRecord, p_ucSize + 3, 0);
    *p++ = unCks >> 8;  // Checksum
    *p   = unCks; 
    
    // individual record -> flash
    NVM_WritePersistentData(aucRecord, g_ulNvmAreaOffset, p_ucSize + 5);
    
    // update the current offset
    g_ulNvmAreaOffset += p_ucSize + 5;
  }  
  else
  {
    // Write the current full record into the next NVM-area and clear this NVM-area
    g_ulNvmAreaOffset = (g_ulNvmAreaOffset <= NVM_AREA2_START_ADDR) ? NVM_AREA2_START_ADDR : NVM_AREA1_START_ADDR;
    NVM_WriteFullRecord();
  } 
 
}

///////////////////////////////////////////////////////////////////////////////////
// Name: NVM_WriteFullRecord
// Author: NIVIS LLC
// Description: write a full record in FLASH
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void NVM_WriteFullRecord( void )
{
  uint8  aucRecord[4];
  uint16 unValue;
  
  aucRecord[0] = NVM_SIGNATURE;
  aucRecord[1] = 0;
  
  unValue = FULL_RECORD_SIZE;
  aucRecord[2] = unValue >> 8; // big endian
  aucRecord[3] = unValue; 
  
  // write: signature + id + size
  NVM_WritePersistentData(aucRecord, g_ulNvmAreaOffset, 4);
  g_ulNvmAreaOffset += 4;
  
  // write main persistent data
  NVM_WritePersistentData((uint8*)&g_stProv, g_ulNvmAreaOffset, sizeof(g_stProv));
  unValue = NVM_GetChecksum(&g_stProv, sizeof(g_stProv), 0);
  g_ulNvmAreaOffset += sizeof(g_stProv);
  
  // write burst pipes(3)
  NVM_WritePersistentData((uint8*)g_stBurstModeTableNvData, g_ulNvmAreaOffset, sizeof(g_stBurstModeTableNvData));
  unValue = NVM_GetChecksum(g_stBurstModeTableNvData, sizeof(g_stBurstModeTableNvData), unValue);
  g_ulNvmAreaOffset += sizeof(g_stBurstModeTableNvData);
  
  // write event pipe
  NVM_WritePersistentData((uint8*)g_stEventsTableNvData, g_ulNvmAreaOffset, sizeof(g_stEventsTableNvData));
  unValue = NVM_GetChecksum(g_stEventsTableNvData, sizeof(g_stEventsTableNvData), unValue);
  g_ulNvmAreaOffset += sizeof(g_stEventsTableNvData);
  
  // write checksum
  aucRecord[0] = unValue >> 8; // big endian
  aucRecord[1] = unValue;
  NVM_WritePersistentData(aucRecord, g_ulNvmAreaOffset, 2);
  g_ulNvmAreaOffset += 2; 
  
  // erase the next NVM-area
  if( g_ulNvmAreaOffset < NVM_AREA2_START_ADDR )
    NVM_EraseSector(NVM_AREA2_NO);
  else
    NVM_EraseSector(NVM_AREA1_NO);
  
}

///////////////////////////////////////////////////////////////////////////////////
// Name: NVM_LoadDefaultData
// Author: NIVIS LLC
// Description: initialise the NVM RAM with default values
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void NVM_LoadDefaultData( void )
{  
  // init main provision data
  InitProvisionedData();
  
  // init burst
  NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData;
  for(; pCrtBurst < g_stBurstModeTableNvData + MAX_BURST_NO; pCrtBurst++)
  {
    pCrtBurst->m_ucBurstModeControlCode = BurstModeControlCodes_Off;
    
    for(uint8 ucIdx=0; ucIdx<8; ucIdx++)
      pCrtBurst->m_aucDeviceVariableCodes[ucIdx] = DeviceVariableCodes_NotUsed;
      
    pCrtBurst->m_unCmdNo = 1;
    
    pCrtBurst->m_ulMinUpdateTime = 6000;
    pCrtBurst->m_ulMaxUpdateTime = 6000;
    
    pCrtBurst->m_fTriggerValue = 0;
    pCrtBurst->m_ucBurstTriggerModeCode = 0; // continuous
    pCrtBurst->m_ucClassification = DeviceVariableClassificationCodes_NotUsed; // PV -> Temp(assumption)
    pCrtBurst->m_ucUnit = UnitsCodes_NotUsed;
  }
 
  // PUBLISH_CMD784, Nivis custom
#ifdef PUBLISH_CMD784  
  g_stBurstModeTableNvData[0].m_unCmdNo = 784;
  g_stBurstModeTableNvData[0].m_ucBurstModeControlCode = BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer; 
#endif    
  
  // init event
  NV_EVENT_NOTIFICATION_T *pCrtEvent = g_stEventsTableNvData;
  for(; pCrtEvent < g_stEventsTableNvData + MAX_EVENTS_NO; pCrtEvent++)
  {
    pCrtEvent->m_ulRetryTime = 6000; 
    pCrtEvent->m_ulMaxUpdateTime = 6000;
    pCrtEvent->m_ulDebounceInterval = 100;
    
    pCrtEvent->m_ucEventControlCode = EventNotificationControlCodes_Off;
    
    pCrtEvent->m_ucMaskSize = 1;
    pCrtEvent->m_aucEventMasks[0] = 0xFF;
    for(uint8 ucIdx=1; ucIdx<26; ucIdx++)
      pCrtEvent->m_aucEventMasks[ucIdx] = 0x00; 
  }
  
  // write persistent data into FLASH
  NVM_WriteFullRecord();  
  
  // calibrate the 24MHz clock
  DoCristalCalibration();
}

///////////////////////////////////////////////////////////////////////////////////
// Name: NVM_ReadRecords
// Author: NIVIS LLC
// Description: read the persistent data from FLASH
// Parameters: none
// Return: none
// Obs:
//      Access level: user level
///////////////////////////////////////////////////////////////////////////////////
void NVM_ReadRecords( void )
{
  uint8  ucValue;
  uint8  aucRecord[MAX_INDIVIDUAL_RECORD];
  uint16 unValue;  
  
  // read manufacturing info
  ReadManufacturing();
  
  // full record handling
  
  // find the NVM-area and validate the signature
  NVM_ReadPersistentData(&ucValue, NVM_AREA1_START_ADDR, 1);
  if( NVM_SIGNATURE != ucValue )
  {
    NVM_ReadPersistentData(&ucValue, NVM_AREA2_START_ADDR, 1); 
    if( NVM_SIGNATURE == ucValue )
    { // area 2 found
      g_ulNvmAreaOffset = NVM_AREA2_START_ADDR;
    } 
    else
    {
      NVM_EraseSector(NVM_AREA1_NO);
      g_ulNvmAreaOffset = NVM_AREA1_START_ADDR;
      NVM_LoadDefaultData(); 
      return;
    } 
  } 
  else
  { // area 1 found
    g_ulNvmAreaOffset = NVM_AREA1_START_ADDR;
  }  
  
  // validate the Id and size
  NVM_ReadPersistentData(&ucValue, g_ulNvmAreaOffset + 1, 1);
  NVM_ReadPersistentData(aucRecord, g_ulNvmAreaOffset + 2, 2);
  unValue = 256*aucRecord[0] + aucRecord[1];
  if(  ucValue    // wrong Id(full record Id must be 0) 
     ||(unValue != FULL_RECORD_SIZE) ) // wrong data size
  {  
    // erase the wrong NVM-area
    if( g_ulNvmAreaOffset < NVM_AREA2_START_ADDR )    
      NVM_EraseSector(NVM_AREA1_NO);
    else  
      NVM_EraseSector(NVM_AREA2_NO);
    
    g_ulNvmAreaOffset = NVM_AREA1_START_ADDR;
    NVM_LoadDefaultData();
    return;
  }
  
  // update the offset
  g_ulNvmAreaOffset += 4;
    
  // read main persistent data
  NVM_ReadPersistentData((uint8*)&g_stProv, g_ulNvmAreaOffset, sizeof(g_stProv));
  unValue = NVM_GetChecksum(&g_stProv, sizeof(g_stProv), 0);
  g_ulNvmAreaOffset += sizeof(g_stProv);

  // read burst pipes(3)
  NVM_ReadPersistentData((uint8*)g_stBurstModeTableNvData, g_ulNvmAreaOffset, sizeof(g_stBurstModeTableNvData));
  unValue = NVM_GetChecksum(g_stBurstModeTableNvData, sizeof(g_stBurstModeTableNvData), unValue);
  g_ulNvmAreaOffset += sizeof(g_stBurstModeTableNvData);
  
  // read event pipe
  NVM_ReadPersistentData((uint8*)g_stEventsTableNvData, g_ulNvmAreaOffset, sizeof(g_stEventsTableNvData));
  unValue = NVM_GetChecksum(g_stEventsTableNvData, sizeof(g_stEventsTableNvData), unValue);
  g_ulNvmAreaOffset += sizeof(g_stEventsTableNvData);
  
  // checksum ok ?
  NVM_ReadPersistentData(aucRecord, g_ulNvmAreaOffset, 2);
  if(  unValue != (256*aucRecord[0] + aucRecord[1]) )
  {
    // erase the wrong NVM-area
    if( g_ulNvmAreaOffset < NVM_AREA2_START_ADDR )  
      NVM_EraseSector(NVM_AREA1_NO);
    else  
      NVM_EraseSector(NVM_AREA2_NO);
   
    g_ulNvmAreaOffset = NVM_AREA1_START_ADDR;
    NVM_LoadDefaultData(); 
    return;
  }  
  g_ulNvmAreaOffset += 2; 

  
  // individual record handling
  for(;;)
  {
    const NVM_TABLE_T* pCrtEntry;
    
    // read Id
    NVM_ReadPersistentData(&ucValue, g_ulNvmAreaOffset, 1);
    
    pCrtEntry = NVM_GET_ENTRY(ucValue);
    if( pCrtEntry != NULL )
    {
      // Id ok, update offset
      g_ulNvmAreaOffset ++;
      
      // Checksum is ok ? 
      NVM_ReadPersistentData(aucRecord, g_ulNvmAreaOffset + 2 + pCrtEntry->m_ucSize, 2);
      unValue = 256*aucRecord[0] + aucRecord[1]; // stored Cks
      NVM_ReadPersistentData(aucRecord, g_ulNvmAreaOffset - 1, 1 + 2 + pCrtEntry->m_ucSize + 2);
      
      
      if( unValue == NVM_GetChecksum(aucRecord, 1 + 2 + pCrtEntry->m_ucSize, 0) )
      { // Cks is OK
        
        // override the CC
        g_stProv.unConfigChangeCounter = 256*aucRecord[1] + aucRecord[2];   
        
        // override the NVM-RAM
        if( ucValue <= PROVISION_LAST_ID )
        {
          memcpy((uint8*)&g_stProv + pCrtEntry->m_unOffset, aucRecord + 3, pCrtEntry->m_ucSize); 
        }
        else if( ucValue <= BURST_LAST_ID )
        {
          memcpy((uint8*)g_stBurstModeTableNvData + pCrtEntry->m_unOffset, aucRecord + 3, pCrtEntry->m_ucSize); 
        }
        else if( ucValue <= EVENT_LAST_ID )
        {
          memcpy((uint8*)g_stEventsTableNvData + pCrtEntry->m_unOffset, aucRecord + 3, pCrtEntry->m_ucSize); 
        }
        
        // update offset
        g_ulNvmAreaOffset += pCrtEntry->m_ucSize + 4;
      }
      else
      { // wrong Cks
        g_ulNvmAreaOffset = (g_ulNvmAreaOffset < NVM_AREA2_START_ADDR) ? NVM_AREA2_START_ADDR : NVM_AREA1_START_ADDR;
        NVM_WriteFullRecord();  
        break;       
      }
    } 
    else
    {  
      if( ucValue == 0xFF )
      { // this is no record
        break;
      }  
      else
      { // Id corrupted 
        g_ulNvmAreaOffset = (g_ulNvmAreaOffset < NVM_AREA2_START_ADDR) ? NVM_AREA2_START_ADDR : NVM_AREA1_START_ADDR;
        NVM_WriteFullRecord();  
        break;
      } 
    }
  }

/* this point is reached during every uController initialisation */
/* by now, the RAM image (g_stProv) of the provision structure has been loaded with the values taken from NVM */
/* it is now safe to over-write the number of device variables; we need to do this in order not to use an old value from NVM, in case it had not been erased */
/* this is only a temporary work-around, the nb of device variable is HW dependant and, therefore, should stay in RAM */
  
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )  
  g_stProv.ucMaxNoOfDeviceVars = 0;
#else  
  g_stProv.ucMaxNoOfDeviceVars = NoOfDeviceVariables; /* erase these lines when the nb of dev. var. are moved to RAM */
#endif  

  
#if ( (BOARD_TYPE == BOARD_TYPE_DEVELOPMENT) || (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )  
  // overwrite the Network Id with the flash hardcoded value (disabled for boards with hart modem)
  if( g_stProv.unNetworkId != NET_ID )
  {
    g_stProv.unNetworkId = NET_ID; 
    
    NVM_WriteIndividualRecord( 35, 
                               (uint8*)&g_stProv + c_aProvNvmTable[34].m_unOffset, 
                               c_aProvNvmTable[34].m_ucSize);
  }
#endif

#ifdef FORCE_LONGTAG_HCF_DUT  
  if(memcmp(g_stProv.aLongTag, "HCF-DUT", 7))
  {
    memcpy(g_stProv.aLongTag, "HCF-DUT", 7);
    memset(g_stProv.aLongTag + 7, 0, sizeof(g_stProv.aLongTag) - 7);
    NVM_WriteIndividualRecord( 19, 
                               (uint8*)&g_stProv + c_aProvNvmTable[18].m_unOffset, 
                               c_aProvNvmTable[18].m_ucSize);
  }    
#endif
  
  // masking aDeviceStatus
  for(uint8 ucIdx = 0; ucIdx < MASTER_NO - 1; ucIdx++)
  {
    g_stProv.aDeviceStatus[ucIdx] &= CONFIG_CHANGED;  // CONFIG_CHANGED flag is persistent
    g_stProv.aDeviceStatus[ucIdx] |= COLD_START;      // set the COLD_START flag
  }
  
  // perform other initialisations  
  g_unNetworkId = g_stProv.unNetworkId; // the actual value used by the DLL
  g_ucJoinMode = g_stProv.ucJoinModeCode;
  g_stProv.ucDefaultTTL = 32;           // after reset TTL must be 32 (default), no need to be kept in NVM -see TML211A

  //if(TXPOWER_10dBm > TXPOWER_10dBm)   // TODO: add Tx power to manufacturing/NVM area -same way as ISA100 
    g_ucPAValue = DEFAULT_TXPOWER;

  
// the commented lines are not necessary  
//  memset(g_stProv.aDeviceStatus, 0, sizeof(g_stProv.aDeviceStatus));
//  APP_SetDeviceStatusFlag(COLD_START);
  
#ifdef USE_LOOP_CONTROL
//  g_stProv.ucDevicePollingAddr = 0; 
//  g_stProv.ucLoopCurrentMode = LoopCurrentModeCodes_Enabled; // CommonTable16
#else
//  g_stProv.ucDevicePollingAddr = (DEV_ID) & 0x3F; 
//  g_stProv.ucLoopCurrentMode = LoopCurrentModeCodes_Disabled; // CommonTable16
  #ifndef HCF_LOOP_CURRENT_WA  
  APP_SetDeviceStatusFlag(LOOP_CURRENT_FIXED);
  #endif //#ifndef HCF_LOOP_CURRENT_WA   
#endif
  
  // calibrate the 24MHz clock: C-Tune adjustment (based on the pre-calculated m_ucCristal from manufacturing sector)
  DoCristalCalibration();
 
#ifdef BACKBONE_SUPPORT
  // TR: NetId and JoinKey will be received from AP every time after a reset, no need to keep them in NVM(for TR)
  g_unNetworkId = g_stProv.unNetworkId = 0xFFFF;
  memset(g_stProv.aJoinSessionKey, 0x00, 16); 
#endif
}


// save the RestartReason and wait for a watchdog reset
//care must be taken if this function is called from an interrupt, because a FLASH transfer could be already pending from outside the interrupt!!!
void NVM_SaveBeforeRestart(uint8 ucReason)
{
    
    #ifdef C779_SEND_TXTIMEOUTS
      FEED_WDT();
      // save Tx timeouts
      NVM_WriteIndividualRecord(41, (uint8*)&g_stProv + c_aProvNvmTable[40].m_unOffset, 
                                c_aProvNvmTable[40].m_ucSize);
      // save Rx timeouts
      NVM_WriteIndividualRecord(42, (uint8*)&g_stProv + c_aProvNvmTable[41].m_unOffset, 
                                c_aProvNvmTable[41].m_ucSize);
    #endif
      
    #ifdef C000_SEND_RREASON
      FEED_WDT();
      
      //Andy's request
      g_stProv.ucDLLQueueSize = g_ucDllAllQueueSize;
 
      NVM_WriteIndividualRecord(24, (uint8*)&g_stProv + c_aProvNvmTable[23].m_unOffset, 
                                c_aProvNvmTable[23].m_ucSize);
      
      g_stProv.aRestartReason[0] = (ucReason << 4) | (g_stC769.wirelessMode & 0x0F);  // see CommonTable51
      g_stProv.aRestartReason[1] = g_stC769.joinStatus >> 8;    // see CommonTable52
      g_stProv.aRestartReason[2] = g_stC769.joinStatus & 0xFF;  // see CommonTable52
      
      NVM_WriteIndividualRecord(23, (uint8*)&g_stProv + c_aProvNvmTable[22].m_unOffset, 
                                c_aProvNvmTable[22].m_ucSize);
    #endif  
}