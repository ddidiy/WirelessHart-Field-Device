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

#ifndef COMMONPRACTICECOMMANDS_H_
#define COMMONPRACTICECOMMANDS_H_

#include "CommonTables.h"
#include "UniversalCommands.h"

enum
{
	ShortFieldAddressSize = 1, LongFieldAddressSize = 5, MaxEmbeddedCommandsSize = 128, MaxNoOfEmbededCommands = 7
};

//TODO: define upperLowerRangeUnitsCode enum


enum
{
	CMDID_C033_ReadDeviceVariables                                = 33,
	CMDID_C034_WritePrimaryVariableDampingValue                   = 34,
	CMDID_C035_WritePrimaryVariableRangeValues                    = 35,
	CMDID_C036_SetPrimaryVariableUpperRangeValue                  = 36,
	CMDID_C037_SetPrimaryVariableLowerRangeValue                  = 37,
	CMDID_C039_EEPROMControl                                      = 39, //INFO: [Ovidiu] - (Command Not Recommended)
	CMDID_C040_EnterExitFixedCurrentMode                          = 40,
	CMDID_C041_PerformSelfTest                                    = 41,
	CMDID_C042_PerformDeviceReset                                 = 42,
	CMDID_C043_SetPrimaryVariableZero                             = 43,
	CMDID_C044_WritePrimaryVariableUnits                          = 44,
	CMDID_C045_TrimLoopCurrentZero                                = 45,
	CMDID_C046_TrimLoopCurrentGain                                = 46,
	CMDID_C047_WritePrimaryVariableTransferFunction               = 47,
	CMDID_C049_WritePrimaryVariableTransducerSerialNumber         = 49,
	CMDID_C050_ReadDynamicVariableAssignments                     = 50,
	CMDID_C051_WriteDynamicVariableAssignments                    = 51,
	CMDID_C052_SetDeviceVariableZero                              = 52,
	CMDID_C053_WriteDeviceVariableUnits                           = 53,
	CMDID_C054_ReadDeviceVariableInformation                      = 54,
	CMDID_C055_WriteDeviceVariableDampingValue                    = 55,
	CMDID_C056_WriteDeviceVariableTransducerSerialNo              = 56,
	CMDID_C057_ReadUnitTagDescriptorDate                          = 57, //INFO: [Ovidiu] - (Command Not Recommended)
	CMDID_C058_WriteUnitTagDescriptorDate                         = 58, //INFO: [Ovidiu] - (Command Not Recommended)
	CMDID_C059_WriteNumberOfResponsePreambles                     = 59,
	CMDID_C060_ReadAnalogChannelAndPercentOfRange                 = 60,
	CMDID_C061_ReadDynamicVariablesAndPrimaryVariableAnalogChannel= 61,//INFO: [Ovidiu] - (Command Not Recommended)
	CMDID_C062_ReadAnalogChannels                                 = 62,
	CMDID_C063_ReadAnalogChannelInformation                       = 63,
	CMDID_C064_WriteAnalogChannelAdditionalDampingValue           = 64,
	CMDID_C065_WriteAnalogChannelRangeValues                      = 65,
	CMDID_C066_EnterExitFixedAnalogChannelMode                    = 66,
	CMDID_C067_TrimAnalogChannelZero                              = 67,
	CMDID_C068_TrimAnalogChannelGain                              = 68,
	CMDID_C069_WriteAnalogChannelTransferFunction                 = 69,
	CMDID_C070_ReadAnalogChannelEndpointValues                    = 70,
	CMDID_C071_LockDevice                                         = 71,
	CMDID_C072_Squawk                                             = 72,
	CMDID_C073_FindDevice                                         = 73,
	CMDID_C074_ReadIOSystemCapabilities                           = 74,
	CMDID_C075_PollSubDevice                                      = 75,
	CMDID_C076_ReadLockDeviceState                                = 76,
	CMDID_C077_SendCommandToSubDevice                             = 77,
	CMDID_C078_ReadAggregatedCommands                             = 78,
	CMDID_C079_WriteDeviceVariable                                = 79,
	CMDID_C080_ReadDeviceVariableTrimPoints                       = 80,
	CMDID_C081_ReadDeviceVariableTrimGuidelines                   = 81,
	CMDID_C082_WriteDeviceVariableTrimPoint                       = 82,
	CMDID_C083_ResetDeviceVariableTrim                            = 83,
	CMDID_C084_ReadSubDeviceIdentitySummary                       = 84,
	CMDID_C085_ReadIOChannelStatistics                            = 85,
	CMDID_C086_ReadSubDeviceStatistics                            = 86,
	CMDID_C087_WriteIOSystemMasterMode                            = 87,
	CMDID_C088_WriteIOSystemRetryCount                            = 88,
	CMDID_C089_SetRealTimeClock                                   = 89, //INFO: [Doinel] - (Should not be implemented in WirelessHART devices)
	CMDID_C090_ReadRealTimeClock                                  = 90,
	CMDID_C091_ReadTrendConfiguration                             = 91,
	CMDID_C092_WriteTrendConfiguration                            = 92,
	CMDID_C093_ReadTrend                                          = 93,
	CMDID_C094_ReadIOSystemClientSideCommunicationStatistics      = 94,
	CMDID_C095_ReadDeviceCommunicationsStatistics                 = 95,
	CMDID_C096_ReadSynchronousAction                              = 96,
	CMDID_C097_ConfigureSynchronousAction                         = 97,
	CMDID_C098_ReadCommandAction                                  = 98,
	CMDID_C099_ConfigureCommandAction                             = 99,
	CMDID_C101_ReadSubDeviceToBurstMessageMap                     = 101,
	CMDID_C102_MapSubDeviceToBurstMessage                         = 102,
	CMDID_C103_WriteBurstPeriod                                   = 103,
	CMDID_C104_WriteBurstTrigger                                  = 104,
	CMDID_C105_ReadBurstModeConfiguration                         = 105,
	CMDID_C106_FlushDelayedResponses                              = 106,
	CMDID_C107_WriteBurstDeviceVariables                          = 107,
	CMDID_C108_WriteBurstModeCommandNumber                        = 108,
	CMDID_C109_BurstModeControl                                   = 109,
	CMDID_C110_ReadAllDynamicVariables                            = 110, //INFO: [Ovidiu] - (Command Not Recommended)
	CMDID_C111_TransferServiceControl                             = 111,
	CMDID_C112_TransferService                                    = 112,
	CMDID_C113_CatchDeviceVariable                                = 113, //INFO: [Doinel] - (Makes no sense in a wireless network)
	CMDID_C114_ReadCaughtDeviceVariable                           = 114, //INFO: [Doinel] - (Makes no sense in a wireless network)
	CMDID_C115_ReadEventNotificationSummary                       = 115,
	CMDID_C116_WriteEventNotificationBitMask                      = 116,
	CMDID_C117_WriteEventNotificationTiming                       = 117,
	CMDID_C118_EventNotificationControl                           = 118,
	CMDID_C119_AcknowledgeEventNotification                       = 119,
	CMDID_C512_ReadCountryCode                                    = 512,
	CMDID_C513_WriteCountryCode                                   = 513
};

enum
{
	C033_ReqSize =  1, // or 2, 3, 4 (variable size)
	C034_ReqSize =  4,
	C035_ReqSize =  9,
	C036_ReqSize =  0,
	C037_ReqSize =  0,
	C039_ReqSize =  1,
	C040_ReqSize =  4,
	C041_ReqSize =  0,
	C042_ReqSize =  0,
	C043_ReqSize =  0,
	C044_ReqSize =  1,
	C045_ReqSize =  4,
	C046_ReqSize =  4,
	C047_ReqSize =  1,
	C049_ReqSize =  3,
	C050_ReqSize =  0,
	C051_ReqSize =  1, // or 2, 3, 4 (variable size)
	C052_ReqSize =  1,
	C053_ReqSize =  2,
	C054_ReqSize =  1,
	C055_ReqSize =  5,
	C056_ReqSize =  4,
	C057_ReqSize =  0,
	C058_ReqSize = 21,
	C059_ReqSize =  1,
	C060_ReqSize =  1,
	C061_ReqSize =  0,
	C062_ReqSize =  4,
	C063_ReqSize =  1,
	C064_ReqSize =  5,
	C065_ReqSize = 10,
	C066_ReqSize =  6,
	C067_ReqSize =  6,
	C068_ReqSize =  6,
	C069_ReqSize =  2,
	C070_ReqSize =  1,
	C071_ReqSize =  1,
	C072_ReqSize =  0,
	C073_ReqSize =  0,
	C074_ReqSize =  0,
	C075_ReqSize =  3,
	C076_ReqSize =  0,
	C077_ReqSize =  7,  // up to ??? max. bytes (variable size)
	C078_ReqSize =  1,  // at least one byte (number of commands)
	C079_ReqSize =  8,
	C080_ReqSize =  1,
	C081_ReqSize =  1,
	C082_ReqSize =  7,
	C083_ReqSize =  1,
	C084_ReqSize =  2,
	C085_ReqSize =  2,
	C086_ReqSize =  2,
	C087_ReqSize =  1,
	C088_ReqSize =  1,
	C089_ReqSize = 10,
	C090_ReqSize =  0,
	C091_ReqSize =  1,
	C092_ReqSize =  7,
	C093_ReqSize =  1,
	C094_ReqSize =  0,
	C095_ReqSize =  0,
	C096_ReqSize =  1,
	C097_ReqSize = 12,
	C098_ReqSize =  1,
	C099_ReqSize =  4,  // up to ??? max. bytes (variable size)
  C101_ReqSize =  1,
  C102_ReqSize =  3,
  C103_ReqSize =  9,
  C104_ReqSize =  8,
  C105_ReqSize =  0,
  C106_ReqSize =  0,
  C107_ReqSize =  1,  // or 2, 3, 4 (if master is HART rev. 5 or 6); 9  (if master is HART rev. 7)
  C108_ReqSize =  1,
  C109_ReqSize =  1,  // (if master is HART rev. 5 or 6); or 2 (if master is HART rev. 7
	C110_ReqSize =  0,
	C111_ReqSize =  5,
	C112_ReqSize =  7,  // up to ??? max. bytes (variable size)
	C113_ReqSize = 13,
	C114_ReqSize =  1,
	C115_ReqSize =  1,
	C116_ReqSize =  1,  // up to 28, // can be truncated
	C117_ReqSize = 13,
	C118_ReqSize =  2,
	C119_ReqSize =  1,  // up to 33, // can be truncated
	C512_ReqSize =  0,
	C513_ReqSize =  3
};

enum
{
	C033_RespSize =  6, // or 12, 18, 24 (variable size)
	C034_RespSize =  4,
	C035_RespSize =  9,
	C036_RespSize =  0,
	C037_RespSize =  0,
	C039_RespSize =  1,
	C040_RespSize =  4,
	C041_RespSize =  0,
	C042_RespSize =  0,
	C043_RespSize =  0,
	C044_RespSize =  1,
	C045_RespSize =  4,
	C046_RespSize =  4,
	C047_RespSize =  1,
	C049_RespSize =  3,
	C050_RespSize =  4,
	C051_RespSize =  4,
	C052_RespSize =  1,
	C053_RespSize =  2,
	C054_RespSize = 27,
	C055_RespSize =  5,
	C056_RespSize =  4,
	C057_RespSize = 21,
	C058_RespSize = 21,
	C059_RespSize =  1,
	C060_RespSize = 10,
	C061_RespSize = 24,
	C062_RespSize = 24,
	C063_RespSize = 17,
	C064_RespSize =  5,
	C065_RespSize = 10,
	C066_RespSize =  6,
	C067_RespSize =  6,
	C068_RespSize =  6,
	C069_RespSize =  2,
	C070_RespSize = 10,
	C071_RespSize =  1,
	C072_RespSize =  0,
	C073_RespSize = 22,
	C074_RespSize =  8,
	C075_RespSize = 22,
	C076_RespSize =  1,
	C077_RespSize =  7,  // up to ??? max. bytes (variable size)
	C078_RespSize =  1,   
	C079_RespSize =  8,
	C080_RespSize = 10,
	C081_RespSize = 23,
	C082_RespSize =  7,
	C083_RespSize =  1,
	C084_RespSize = 44,
	C085_RespSize = 13,
	C086_RespSize =  7,
	C087_RespSize =  1,
	C088_RespSize =  1,
	C089_RespSize =  8,
	C090_RespSize = 15,
	C091_RespSize =  8,
	C092_RespSize =  7,
	C093_RespSize = 75,
	C094_RespSize = 16,
	C095_RespSize =  6,
	C096_RespSize = 13,
	C097_RespSize = 12,
	C098_RespSize =  4,  // up to ??? max. bytes (variable size)
	C099_RespSize =  4,  // up to ??? max. bytes (variable size)
  C101_RespSize =  3,
  C102_RespSize =  3,
  C103_RespSize =  9,
  C104_RespSize =  8,
  C105_RespSize = 29,
  C106_RespSize =  0,
  C107_RespSize =  9,
  C108_RespSize =  1,
  C109_RespSize =  2, // 2-byte fixed composed response size (as a slave); 1 or 2 bytes when receiveng a response (backward compatibility)
	C110_RespSize = 20,
	C111_RespSize =  7,
	C112_RespSize =  7,  // up to ??? max. bytes (variable size)
	C113_RespSize = 13,
	C114_RespSize = 13,
	C115_RespSize = 19, // up to 45, // can be truncated
	C116_RespSize =  1, // up to 28, // ??? can be truncated ???
	C117_RespSize = 13,
	C118_RespSize =  2,
	C119_RespSize =  9, // up to 33, // can be truncated
	C512_RespSize =  3,
	C513_RespSize =  3
};

#define C033_ReadDeviceVariables_ReqSize                                  C033_ReqSize
#define C034_WritePrimaryVariableDampingValue_ReqSize                     C034_ReqSize
#define C035_WritePrimaryVariableRangeValues_ReqSize                      C035_ReqSize
#define C036_SetPrimaryVariableUpperRangeValue_ReqSize                    C036_ReqSize
#define C037_SetPrimaryVariableLowerRangeValue_ReqSize                    C037_ReqSize
#define C039_EEPROMControl_ReqSize                                        C039_ReqSize
#define C040_EnterExitFixedCurrentMode_ReqSize                            C040_ReqSize
#define C041_PerformSelfTest_ReqSize                                      C041_ReqSize
#define C042_PerformDeviceReset_ReqSize                                   C042_ReqSize
#define C043_SetPrimaryVariableZero_ReqSize                               C043_ReqSize
#define C044_WritePrimaryVariableUnits_ReqSize                            C044_ReqSize
#define C045_TrimLoopCurrentZero_ReqSize                                  C045_ReqSize
#define C046_TrimLoopCurrentGain_ReqSize                                  C046_ReqSize
#define C047_WritePrimaryVariableTransferFunction_ReqSize                 C047_ReqSize
#define C049_WritePrimaryVariableTransducerSerialNumber_ReqSize           C049_ReqSize
#define C050_ReadDynamicVariableAssignments_ReqSize                       C050_ReqSize
#define C051_WriteDynamicVariableAssignments_ReqSize                      C051_ReqSize
#define C052_SetDeviceVariableZero_ReqSize                                C052_ReqSize
#define C053_WriteDeviceVariableUnits_ReqSize                             C053_ReqSize
#define C054_ReadDeviceVariableInformation_ReqSize                        C054_ReqSize
#define C055_WriteDeviceVariableDampingValue_ReqSize                      C055_ReqSize
#define C056_WriteDeviceVariableTransducerSerialNo_ReqSize                C056_ReqSize
#define C057_ReadUnitTagDescriptorDate_ReqSize                            C057_ReqSize
#define C058_WriteUnitTagDescriptorDate_ReqSize                           C058_ReqSize
#define C059_WriteNumberOfResponsePreambles_ReqSize                       C059_ReqSize
#define C060_ReadAnalogChannelAndPercentOfRange_ReqSize                   C060_ReqSize
#define C061_ReadDynamicVariablesAndPrimaryVariableAnalogChannel_ReqSize  C061_ReqSize
#define C062_ReadAnalogChannels_ReqSize                                   C062_ReqSize
#define C063_ReadAnalogChannelInformation_ReqSize                         C063_ReqSize
#define C064_WriteAnalogChannelAdditionalDampingValue_ReqSize             C064_ReqSize
#define C065_WriteAnalogChannelRangeValues_ReqSize                        C065_ReqSize
#define C066_EnterExitFixedAnalogChannelMode_ReqSize                      C066_ReqSize
#define C067_TrimAnalogChannelZero_ReqSize                                C067_ReqSize
#define C068_TrimAnalogChannelGain_ReqSize                                C068_ReqSize
#define C069_WriteAnalogChannelTransferFunction_ReqSize                   C069_ReqSize
#define C070_ReadAnalogChannelEndpointValues_ReqSize                      C070_ReqSize
#define C071_LockDevice_ReqSize                                           C071_ReqSize
#define C072_Squawk_ReqSize                                               C072_ReqSize
#define C073_FindDevice_ReqSize                                           C073_ReqSize
#define C074_ReadIOSystemCapabilities_ReqSize                             C074_ReqSize
#define C075_PollSubDevice_ReqSize                                        C075_ReqSize
#define C076_ReadLockDeviceState_ReqSize                                  C076_ReqSize
#define C077_SendCommandToSubDevice_ReqSize                               C077_ReqSize
#define C078_ReadAggregatedCommands_ReqSize                               C078_ReqSize
#define C079_WriteDeviceVariable_ReqSize                                  C079_ReqSize
#define C080_ReadDeviceVariableTrimPoints_ReqSize                         C080_ReqSize
#define C081_ReadDeviceVariableTrimGuidelines_ReqSize                     C081_ReqSize
#define C082_WriteDeviceVariableTrimPoint_ReqSize                         C082_ReqSize
#define C083_ResetDeviceVariableTrim_ReqSize                              C083_ReqSize
#define C084_ReadSubDeviceIdentitySummary_ReqSize                         C084_ReqSize
#define C085_ReadIOChannelStatistics_ReqSize                              C085_ReqSize
#define C086_ReadSubDeviceStatistics_ReqSize                              C086_ReqSize
#define C087_WriteIOSystemMasterMode_ReqSize                              C087_ReqSize
#define C088_WriteIOSystemRetryCount_ReqSize                              C088_ReqSize
#define C089_SetRealTimeClock_ReqSize                                     C089_ReqSize
#define C090_ReadRealTimeClock_ReqSize                                    C090_ReqSize
#define C091_ReadTrendConfiguration_ReqSize                               C091_ReqSize
#define C092_WriteTrendConfiguration_ReqSize                              C092_ReqSize
#define C093_ReadTrend_ReqSize                                            C093_ReqSize
#define C094_ReadIOSystemClientSideCommunicationStatistics_ReqSize        C094_ReqSize
#define C095_ReadDeviceCommunicationsStatistics_ReqSize                   C095_ReqSize
#define C096_ReadSynchronousAction_ReqSize                                C096_ReqSize
#define C097_ConfigureSynchronousAction_ReqSize                           C097_ReqSize
#define C098_ReadCommandAction_ReqSize                                    C098_ReqSize
#define C099_ConfigureCommandAction_ReqSize                               C099_ReqSize
#define C101_ReadSubDeviceToBurstMessageMap_ReqSize                       C101_ReqSize
#define C102_MapSubDeviceToBurstMessage_ReqSize                           C102_ReqSize
#define C103_WriteBurstPeriod_ReqSize                                     C103_ReqSize
#define C104_WriteBurstTrigger_ReqSize                                    C104_ReqSize
#define C105_ReadBurstModeConfiguration_ReqSize                           C105_ReqSize
#define C106_FlushDelayedResponses_ReqSize                                C106_ReqSize
#define C107_WriteBurstDeviceVariables_ReqSize                            C107_ReqSize
#define C108_WriteBurstModeCommandNumber_ReqSize                          C108_ReqSize
#define C109_BurstModeControl_ReqSize                                     C109_ReqSize
#define C110_ReadAllDynamicVariables_ReqSize                              C110_ReqSize
#define C111_TransferServiceControl_ReqSize                               C111_ReqSize
#define C112_TransferService_ReqSize                                      C112_ReqSize
#define C113_CatchDeviceVariable_ReqSize                                  C113_ReqSize
#define C114_ReadCaughtDeviceVariable_ReqSize                             C114_ReqSize
#define C115_ReadEventNotificationSummary_ReqSize                         C115_ReqSize
#define C116_WriteEventNotificationBitMask_ReqSize                        C116_ReqSize
#define C117_WriteEventNotificationTiming_ReqSize                         C117_ReqSize
#define C118_EventNotificationControl_ReqSize                             C118_ReqSize
#define C119_AcknowledgeEventNotification_ReqSize                         C119_ReqSize
#define C512_ReadCountryCode_ReqSize                                      C512_ReqSize
#define C513_WriteCountryCode_ReqSize                                     C513_ReqSize


#define C033_ReadDeviceVariables_RespSize                                 C033_RespSize
#define C034_WritePrimaryVariableDampingValue_RespSize                    C034_RespSize
#define C035_WritePrimaryVariableRangeValues_RespSize                     C035_RespSize
#define C036_SetPrimaryVariableUpperRangeValue_RespSize                   C036_RespSize
#define C037_SetPrimaryVariableLowerRangeValue_RespSize                   C037_RespSize
#define C039_EEPROMControl_RespSize                                       C039_RespSize
#define C040_EnterExitFixedCurrentMode_RespSize                           C040_RespSize
#define C041_PerformSelfTest_RespSize                                     C041_RespSize
#define C042_PerformDeviceReset_RespSize                                  C042_RespSize
#define C043_SetPrimaryVariableZero_RespSize                              C043_RespSize
#define C044_WritePrimaryVariableUnits_RespSize                           C044_RespSize
#define C045_TrimLoopCurrentZero_RespSize                                 C045_RespSize
#define C046_TrimLoopCurrentGain_RespSize                                 C046_RespSize
#define C047_WritePrimaryVariableTransferFunction_RespSize                C047_RespSize
#define C049_WritePrimaryVariableTransducerSerialNumber_RespSize          C049_RespSize
#define C050_ReadDynamicVariableAssignments_RespSize                      C050_RespSize
#define C051_WriteDynamicVariableAssignments_RespSize                     C051_RespSize
#define C052_SetDeviceVariableZero_RespSize                               C052_RespSize
#define C053_WriteDeviceVariableUnits_RespSize                            C053_RespSize
#define C054_ReadDeviceVariableInformation_RespSize                       C054_RespSize
#define C055_WriteDeviceVariableDampingValue_RespSize                     C055_RespSize
#define C056_WriteDeviceVariableTransducerSerialNo_RespSize               C056_RespSize
#define C057_ReadUnitTagDescriptorDate_RespSize                           C057_RespSize
#define C058_WriteUnitTagDescriptorDate_RespSize                          C058_RespSize
#define C059_WriteNumberOfResponsePreambles_RespSize                      C059_RespSize
#define C060_ReadAnalogChannelAndPercentOfRange_RespSize                  C060_RespSize
#define C061_ReadDynamicVariablesAndPrimaryVariableAnalogChannel_RespSize C061_RespSize
#define C062_ReadAnalogChannels_RespSize                                  C062_RespSize
#define C063_ReadAnalogChannelInformation_RespSize                        C063_RespSize
#define C064_WriteAnalogChannelAdditionalDampingValue_RespSize            C064_RespSize
#define C065_WriteAnalogChannelRangeValues_RespSize                       C065_RespSize
#define C066_EnterExitFixedAnalogChannelMode_RespSize                     C066_RespSize
#define C067_TrimAnalogChannelZero_RespSize                               C067_RespSize
#define C068_TrimAnalogChannelGain_RespSize                               C068_RespSize
#define C069_WriteAnalogChannelTransferFunction_RespSize                  C069_RespSize
#define C070_ReadAnalogChannelEndpointValues_RespSize                     C070_RespSize
#define C071_LockDevice_RespSize                                          C071_RespSize
#define C072_Squawk_RespSize                                              C072_RespSize
#define C073_FindDevice_RespSize                                          C073_RespSize
#define C074_ReadIOSystemCapabilities_RespSize                            C074_RespSize
#define C075_PollSubDevice_RespSize                                       C075_RespSize
#define C076_ReadLockDeviceState_RespSize                                 C076_RespSize
#define C077_SendCommandToSubDevice_RespSize                              C077_RespSize
#define C078_ReadAggregatedCommands_RespSize                              C078_RespSize
#define C079_WriteDeviceVariable_RespSize                                 C079_RespSize
#define C080_ReadDeviceVariableTrimPoints_RespSize                        C080_RespSize
#define C081_ReadDeviceVariableTrimGuidelines_RespSize                    C081_RespSize
#define C082_WriteDeviceVariableTrimPoint_RespSize                        C082_RespSize
#define C083_ResetDeviceVariableTrim_RespSize                             C083_RespSize
#define C084_ReadSubDeviceIdentitySummary_RespSize                        C084_RespSize
#define C085_ReadIOChannelStatistics_RespSize                             C085_RespSize
#define C086_ReadSubDeviceStatistics_RespSize                             C086_RespSize
#define C087_WriteIOSystemMasterMode_RespSize                             C087_RespSize
#define C088_WriteIOSystemRetryCount_RespSize                             C088_RespSize
#define C089_SetRealTimeClock_RespSize                                    C089_RespSize
#define C090_ReadRealTimeClock_RespSize                                   C090_RespSize
#define C091_ReadTrendConfiguration_RespSize                              C091_RespSize
#define C092_WriteTrendConfiguration_RespSize                             C092_RespSize
#define C093_ReadTrend_RespSize                                           C093_RespSize
#define C094_ReadIOSystemClientSideCommunicationStatistics_RespSize       C094_RespSize
#define C095_ReadDeviceCommunicationsStatistics_RespSize                  C095_RespSize
#define C096_ReadSynchronousAction_RespSize                               C096_RespSize
#define C097_ConfigureSynchronousAction_RespSize                          C097_RespSize
#define C098_ReadCommandAction_RespSize                                   C098_RespSize
#define C099_ConfigureCommandAction_RespSize                              C099_RespSize
#define C101_ReadSubDeviceToBurstMessageMap_RespSize                      C101_RespSize
#define C102_MapSubDeviceToBurstMessage_RespSize                          C102_RespSize
#define C103_WriteBurstPeriod_RespSize                                    C103_RespSize
#define C104_WriteBurstTrigger_RespSize                                   C104_RespSize
#define C105_ReadBurstModeConfiguration_RespSize                          C105_RespSize
#define C106_FlushDelayedResponses_RespSize                               C106_RespSize
#define C107_WriteBurstDeviceVariables_RespSize                           C107_RespSize
#define C108_WriteBurstModeCommandNumber_RespSize                         C108_RespSize
#define C109_BurstModeControl_RespSize                                    C109_RespSize
#define C110_ReadAllDynamicVariables_RespSize                             C110_RespSize
#define C111_TransferServiceControl_RespSize                              C111_RespSize
#define C112_TransferService_RespSize                                     C112_RespSize
#define C113_CatchDeviceVariable_RespSize                                 C113_RespSize
#define C114_ReadCaughtDeviceVariable_RespSize                            C114_RespSize
#define C115_ReadEventNotificationSummary_RespSize                        C115_RespSize
#define C116_WriteEventNotificationBitMask_RespSize                       C116_RespSize
#define C117_WriteEventNotificationTiming_RespSize                        C117_RespSize
#define C118_EventNotificationControl_RespSize                            C118_RespSize
#define C119_AcknowledgeEventNotification_RespSize                        C119_RespSize
#define C512_ReadCountryCode_RespSize                                     C512_RespSize
#define C513_WriteCountryCode_RespSize                                    C513_RespSize


/******************** CMD 033 *************************/

typedef struct
{
	uint8_t variablesSize;
	uint8_t variables[4];
}C033_ReadDeviceVariables_Req;
typedef struct
{
	uint8_t variablesSize;
	struct
	{
		uint8_t code;
		UnitsCodes unitsCode;
		float value;
	} variables[4];
}C033_ReadDeviceVariables_Resp;
typedef enum
{
      C033_NOO = RCS_N00_Success,
      C033_E02 = RCS_E02_InvalidSelection,
      C033_E05 = RCS_E05_TooFewDataBytesReceived,
      C033_E06 = RCS_E06_DeviceSpecificCommandError,
      C033_W08 = RCM_W08_UpdateFailure,
      C033_E16 = RCS_E16_AccessRestricted
}C033_ReadDeviceVariables_RespCodes;


/******************** CMD 034 *************************/

typedef struct
{
	float value; //units of seconds
}C034_WritePrimaryVariableDampingValue_Req;
typedef C034_WritePrimaryVariableDampingValue_Req C034_WritePrimaryVariableDampingValue_Resp;
typedef enum
{
      C034_NOO = RCS_N00_Success,
      C034_E03 = RCS_E03_PassedParameterTooLarge,
      C034_E04 = RCS_E04_PassedParameterTooSmall,
      C034_E05 = RCS_E05_TooFewDataBytesReceived,
      C034_E06 = RCS_E06_DeviceSpecificCommandError,
      C034_E07 = RCS_E07_InWriteProtectMode,
      C034_W08 = RCM_W08_SetToNearestPossibleValue,
      C034_E16 = RCS_E16_AccessRestricted,
      C034_E32 = RCS_E32_Busy
}C034_WritePrimaryVariableDampingValue_RespCodes;


/******************** CMD 035 *************************/

typedef struct
{
	UnitsCodes unitsCode;
	float upperValue;
	float lowerValue;
}C035_WritePrimaryVariableRangeValues_Req;

typedef C035_WritePrimaryVariableRangeValues_Req C035_WritePrimaryVariableRangeValues_Resp; // same format

typedef enum
{
      C035_NOO = RCS_N00_Success,
      C035_E02 = RCS_E02_InvalidSelection,
      C035_E05 = RCS_E05_TooFewDataBytesReceived,
      C035_E06 = RCS_E06_DeviceSpecificCommandError,
      C035_E07 = RCS_E07_InWriteProtectMode,
      C035_W08 = RCM_W08_SetToNearestPossibleValue,  // (Upper or Lower Range Pushed)
      C035_E09 = RCM_E09_LowerRangeValueTooHigh,
      C035_E10 = RCM_E10_LowerRangeValueTooLow,
      C035_E11 = RCM_E11_UpperRangeValueTooHigh,
      C035_E12 = RCM_E12_UpperRangeValueTooLow,
      C035_E13 = RCM_E13_UpperAndLowerRangeValuesOutOfLimits,
      C035_W14 = RCM_W14_SpanTooSmall,  // (Device Accuracy May Be Impaired)
      C035_E16 = RCS_E16_AccessRestricted,
      C035_E18 = RCS_E18_InvalidUnitsCode,
      C035_E29 = RCM_E29_InvalidSpan,
      C035_E32 = RCS_E32_Busy
}C035_WritePrimaryVariableRangeValues_RespCodes;


/******************** CMD 036 *************************/

typedef EmptyCommand_Req C036_SetPrimaryVariableUpperRangeValue_Req;
typedef EmptyCommand_Resp C036_SetPrimaryVariableUpperRangeValue_Resp;
typedef enum
{
      C036_NOO = RCS_N00_Success,
      C036_E06 = RCS_E06_DeviceSpecificCommandError,
      C036_E07 = RCS_E07_InWriteProtectMode,
      C036_W08 = RCM_W08_SetToNearestPossibleValue,  // (Upper Range Pushed)
      C036_E09 = RCM_E09_AppliedProcessTooHigh,
      C036_E10 = RCM_E10_AppliedProcessTooLow,
      C036_W14 = RCM_W14_SpanTooSmall,  // (Device Accuracy May Be Impaired)
      C036_E16 = RCS_E16_AccessRestricted,
      C036_E29 = RCM_E29_InvalidSpan,
      C036_E32 = RCS_E32_Busy
}C036_SetPrimaryVariableUpperRangeValue_RespCodes;


/******************** CMD 037 *************************/

typedef EmptyCommand_Req C037_SetPrimaryVariableLowerRangeValue_Req;
typedef EmptyCommand_Resp C037_SetPrimaryVariableLowerRangeValue_Resp;
typedef enum
{
      C037_NOO = RCS_N00_Success,  
      C037_E06 = RCS_E06_DeviceSpecificCommandError,
      C037_E07 = RCS_E07_InWriteProtectMode,
      C037_E09 = RCM_E09_AppliedProcessTooHigh,
      C037_E10 = RCM_E10_AppliedProcessTooLow,
      C037_W14 = RCM_W14_NewLowerRangeValuePushedUpperRangeOver,
      C037_E16 = RCS_E16_AccessRestricted,
      C037_E29 = RCM_E29_InvalidSpan,
      C037_E32 = RCS_E32_Busy
}C037_SetPrimaryVariableLowerRangeValue_RespCodes;


/******************** CMD 040 *************************/

typedef struct
{
	float level; //units of milliamperes
}C040_EnterExitFixedCurrentMode_Req;
typedef C040_EnterExitFixedCurrentMode_Req C040_EnterExitFixedCurrentMode_Resp; // same format

typedef enum
{
      C040_NOO = RCS_N00_Success,  
      C040_E03 = RCS_E03_PassedParameterTooLarge,
      C040_E04 = RCS_E04_PassedParameterTooSmall,
      C040_E05 = RCS_E05_TooFewDataBytesReceived,
      C040_E06 = RCS_E06_DeviceSpecificCommandError,
      C040_E07 = RCS_E07_InWriteProtectMode,
      C040_E11 = RCM_E11_LoopCurrentNotActive,  // (Device in Multidrop Mode)
      C040_E16 = RCS_E16_AccessRestricted,
      C040_E32 = RCS_E32_Busy
}C040_EnterExitFixedCurrentMode_RespCodes;


/******************** CMD 041 *************************/

typedef EmptyCommand_Req C041_PerformSelfTest_Req;
typedef EmptyCommand_Resp C041_PerformSelfTest_Resp;
typedef enum
{
      C041_NOO = RCS_N00_Success,      
      C041_E06 = RCS_E06_DeviceSpecificCommandError,
      C041_E16 = RCS_E16_AccessRestricted,
      C041_E32 = RCS_E32_Busy
}C041_PerformSelfTest_RespCodes;


/******************** CMD 042 *************************/

typedef EmptyCommand_Req C042_PerformDeviceReset_Req;
typedef EmptyCommand_Resp C042_PerformDeviceReset_Resp;
typedef enum
{
      C042_NOO = RCS_N00_Success,    
      C042_E06 = RCS_E06_DeviceSpecificCommandError,
      C042_E16 = RCS_E16_AccessRestricted,
      C042_E32 = RCS_E32_Busy
}C042_PerformDeviceReset_RespCodes;


/******************** CMD 043 *************************/

typedef EmptyCommand_Req C043_SetPrimaryVariableZero_Req;
typedef EmptyCommand_Req C043_SetPrimaryVariableZero_Resp;
typedef enum
{
      C043_NOO = RCS_N00_Success,   
      C043_E06 = RCS_E06_DeviceSpecificCommandError,
      C043_E07 = RCS_E07_InWriteProtectMode,
      C043_E09 = RCM_E09_AppliedProcessTooHigh,
      C043_E10 = RCM_E10_AppliedProcessTooLow,
      C043_E16 = RCS_E16_AccessRestricted,
      C043_E32 = RCS_E32_Busy
}C043_SetPrimaryVariableZero_RespCodes;


/******************** CMD 044 *************************/

typedef struct
{
	UnitsCodes unitsCode;
}C044_WritePrimaryVariableUnits_Req;
typedef C044_WritePrimaryVariableUnits_Req C044_WritePrimaryVariableUnits_Resp;
typedef enum
{
      C044_NOO = RCS_N00_Success,   
      C044_E02 = RCS_E02_InvalidSelection,
      C044_E05 = RCS_E05_TooFewDataBytesReceived,
      C044_E06 = RCS_E06_DeviceSpecificCommandError,
      C044_E07 = RCS_E07_InWriteProtectMode,
      C044_E16 = RCS_E16_AccessRestricted,
      C044_E32 = RCS_E32_Busy
}C044_WritePrimaryVariableUnits_RespCodes;


/******************** CMD 045 *************************/

typedef struct
{
	float level; //units of milliamperes
}C045_TrimLoopCurrentZero_Req;
typedef struct
{
	float actualLevel; //units of milliamperes
}C045_TrimLoopCurrentZero_Resp;
typedef enum
{
      C045_NOO = RCS_N00_Success,   
      C045_E03 = RCS_E03_PassedParameterTooLarge,
      C045_E04 = RCS_E04_PassedParameterTooSmall,
      C045_E05 = RCS_E05_TooFewDataBytesReceived,
      C045_E06 = RCS_E06_DeviceSpecificCommandError,
      C045_E07 = RCS_E07_InWriteProtectMode,
      C045_E09 = RCM_E09_IncorrectLoopCurrentModeOrValue,
      C045_E11 = RCM_E11_LoopCurrentNotActive,  // (Device in Multidrop Mode)
      C045_E16 = RCS_E16_AccessRestricted,
      C045_E32 = RCS_E32_Busy
}C045_TrimLoopCurrentZero_RespCodes;


/******************** CMD 046 *************************/

typedef struct
{
	float level; //units of milliamperes
}C046_TrimLoopCurrentGain_Req;
typedef struct
{
	float actualLevel; //units of milliamperes
}C046_TrimLoopCurrentGain_Resp;
typedef enum
{
      C046_NOO = RCS_N00_Success,    
      C046_E03 = RCS_E03_PassedParameterTooLarge,
      C046_E04 = RCS_E04_PassedParameterTooSmall,
      C046_E05 = RCS_E05_TooFewDataBytesReceived,
      C046_E06 = RCS_E06_DeviceSpecificCommandError,
      C046_E07 = RCS_E07_InWriteProtectMode,
      C046_E09 = RCM_E09_IncorrectLoopCurrentModeOrValue,
      C046_E11 = RCM_E11_LoopCurrentNotActive,  // (Device in Multidrop Mode)
      C046_E16 = RCS_E16_AccessRestricted,
      C046_E32 = RCS_E32_Busy
}C046_TrimLoopCurrentGain_RespCodes;


/******************** CMD 047 *************************/

typedef struct
{
	TransferFunctionCodes transferFunctionCode; //CommonTable3
}C047_WritePrimaryVariableTransferFunction_Req;
typedef C047_WritePrimaryVariableTransferFunction_Req C047_WritePrimaryVariableTransferFunction_Resp;
typedef enum
{
      C047_NOO = RCS_N00_Success, 
      C047_E02 = RCS_E02_InvalidSelection,
      C047_E05 = RCS_E05_TooFewDataBytesReceived,
      C047_E06 = RCS_E06_DeviceSpecificCommandError,
      C047_E07 = RCS_E07_InWriteProtectMode,
      C047_E16 = RCS_E16_AccessRestricted,
      C047_E32 = RCS_E32_Busy
}C047_WritePrimaryVariableTransferFunction_RespCodes;


/******************** CMD 049 *************************/

typedef struct
{
	uint32_t serialNo; //_uint24_t
}C049_WritePrimaryVariableTransducerSerialNumber_Req;
typedef C049_WritePrimaryVariableTransducerSerialNumber_Req C049_WritePrimaryVariableTransducerSerialNumber_Resp;
typedef enum
{
      C049_NOO = RCS_N00_Success,   
      C049_E05 = RCS_E05_TooFewDataBytesReceived,
      C049_E06 = RCS_E06_DeviceSpecificCommandError,
      C049_E07 = RCS_E07_InWriteProtectMode,
      C049_E16 = RCS_E16_AccessRestricted,
      C049_E32 = RCS_E32_Busy
}C049_WritePrimaryVariableTransducerSerialNumber_RespCodes;


/******************** CMD 050 *************************/

typedef EmptyCommand_Req C050_ReadDynamicVariableAssignments_Req;
typedef struct
{
	uint8_t variables[4];
}C050_ReadDynamicVariableAssignments_Resp;
typedef enum
{
      C050_NOO = RCS_N00_Success,
      C050_E06 = RCS_E06_DeviceSpecificCommandError,
      C050_E16 = RCS_E16_AccessRestricted
}C050_ReadDynamicVariableAssignments_RespCodes;


/******************** CMD 051 *************************/

typedef struct
{
	uint8_t variablesSize;
	uint8_t variables[4];
}C051_WriteDynamicVariableAssignments_Req;
typedef struct
{
	uint8_t variables[4];
}C051_WriteDynamicVariableAssignments_Resp;
typedef enum
{
      C051_NOO = RCS_N00_Success,
      C051_E02 = RCS_E02_InvalidSelection,
      C051_E05 = RCS_E05_TooFewDataBytesReceived,
      C051_E06 = RCS_E06_DeviceSpecificCommandError,
      C051_E07 = RCS_E07_InWriteProtectMode,
      C051_E16 = RCS_E16_AccessRestricted,
      C051_E32 = RCS_E32_Busy
}C051_WriteDynamicVariableAssignments_RespCodes;


/******************** CMD 052 *************************/

typedef struct
{
	uint8_t variableToBeZeroed;
}C052_SetDeviceVariableZero_Req;
typedef C052_SetDeviceVariableZero_Req C052_SetDeviceVariableZero_Resp;
typedef enum
{
      C052_NOO = RCS_N00_Success,  
      C052_E02 = RCS_E02_InvalidSelection,
      C052_E05 = RCS_E05_TooFewDataBytesReceived,
      C052_E06 = RCS_E06_DeviceSpecificCommandError,
      C052_E07 = RCS_E07_InWriteProtectMode,
      C052_E09 = RCM_E09_AppliedProcessTooHigh,
      C052_E10 = RCM_E10_AppliedProcessTooLow,
      C052_E16 = RCS_E16_AccessRestricted,
      C052_E32 = RCS_E32_Busy
}C052_SetDeviceVariableZero_RespCodes;


/******************** CMD 053 *************************/

typedef struct
{
	uint8_t variable;
	UnitsCodes unitsCode;
}C053_WriteDeviceVariableUnits_Req;
typedef C053_WriteDeviceVariableUnits_Req C053_WriteDeviceVariableUnits_Resp;
typedef enum
{
      C053_NOO = RCS_N00_Success,    
      C053_E05 = RCS_E05_TooFewDataBytesReceived,
      C053_E06 = RCS_E06_DeviceSpecificCommandError,
      C053_E07 = RCS_E07_InWriteProtectMode,
      C053_E11 = RCM_E11_InvalidDeviceVariableCode,
      C053_E12 = RCM_E12_InvalidUnitsCode,
      C053_E16 = RCS_E16_AccessRestricted,
      C053_E32 = RCS_E32_Busy
}C053_WriteDeviceVariableUnits_RespCodes;


/******************** CMD 054 *************************/

typedef struct
{
	uint8_t variableCode;
}C054_ReadDeviceVariableInformation_Req;
typedef struct
{
	uint8_t variableCode;
	uint32_t serialNo; //_uint24_t
	UnitsCodes spanUnits;
	float upperTransducerLimit;
	float lowerTransducerLimit;
	float dampingValue;
	float minimumSpan;
	DeviceVariableClassificationCodes classificationCode;
	DeviceVariableFamilyCodes familyCode;
	_time_t updateTime;
}C054_ReadDeviceVariableInformation_Resp;
typedef enum
{
      C054_NOO = RCS_N00_Success,
      C054_E02 = RCS_E02_InvalidSelection,
      C054_E05 = RCS_E05_TooFewDataBytesReceived,
      C054_E06 = RCS_E06_DeviceSpecificCommandError,
      C054_E16 = RCS_E16_AccessRestricted,
      C054_E32 = RCS_E32_Busy
}C054_ReadDeviceVariableInformation_RespCodes;


/******************** CMD 055 *************************/

typedef struct
{
	uint8_t variableCode;
	float value; //units of seconds
}C055_WriteDeviceVariableDampingValue_Req;
typedef C055_WriteDeviceVariableDampingValue_Req C055_WriteDeviceVariableDampingValue_Resp;
typedef enum
{
      C055_NOO = RCS_N00_Success,  
      C055_E02 = RCS_E02_InvalidSelection,
      C055_E03 = RCS_E03_PassedParameterTooLarge,
      C055_E04 = RCS_E04_PassedParameterTooSmall,
      C055_E05 = RCS_E05_TooFewDataBytesReceived,
      C055_E06 = RCS_E06_DeviceSpecificCommandError,
      C055_E07 = RCS_E07_InWriteProtectMode,
      C55_W08 = RCM_W08_SetToNearestPossibleValue,
      C055_E16 = RCS_E16_AccessRestricted,
      C055_E32 = RCS_E32_Busy
}C055_WriteDeviceVariableDampingValue_RespCodes;


/******************** CMD 056 *************************/

typedef struct
{
	uint8_t variableCode;
	uint32_t serialNo; //_uint24_t
}C056_WriteDeviceVariableTransducerSerialNo_Req;
typedef C056_WriteDeviceVariableTransducerSerialNo_Req C056_WriteDeviceVariableTransducerSerialNo_Resp;
typedef enum
{
      C056_NOO = RCS_N00_Success,   
      C056_E02 = RCS_E02_InvalidSelection,
      C056_E05 = RCS_E05_TooFewDataBytesReceived,
      C056_E06 = RCS_E06_DeviceSpecificCommandError,
      C056_E07 = RCS_E07_InWriteProtectMode,
      C056_E16 = RCS_E16_AccessRestricted,
      C056_E32 = RCS_E32_Busy
}C056_WriteDeviceVariableTransducerSerialNo_RespCodes;


/******************** CMD 059 *************************/

typedef struct
{
	uint8_t noOfPreambles;
}C059_WriteNumberOfResponsePreambles_Req;
typedef C059_WriteNumberOfResponsePreambles_Req C059_WriteNumberOfResponsePreambles_Resp;
typedef enum
{
      C059_NOO = RCS_N00_Success,  
      C059_E03 = RCS_E03_PassedParameterTooLarge,
      C059_E04 = RCS_E04_PassedParameterTooSmall,
      C059_E05 = RCS_E05_TooFewDataBytesReceived,
      C059_E06 = RCS_E06_DeviceSpecificCommandError,
      C059_E07 = RCS_E07_InWriteProtectMode,
      C059_W08 = RCM_W08_SetToNearestPossibleValue,
      C059_E16 = RCS_E16_AccessRestricted,
      C059_E32 = RCS_E32_Busy
}C059_WriteNumberOfResponsePreambles_RespCodes;


/******************** CMD 060 *************************/

typedef struct
{
	uint8_t channelNumberCode;
}C060_ReadAnalogChannelAndPercentOfRange_Req;
typedef struct
{
	uint8_t channelNumberCode;
	UnitsCodes unitsCode;
	float level;
	float percentOfRange; //units of percent
}C060_ReadAnalogChannelAndPercentOfRange_Resp;
typedef enum
{
      C060_NOO = RCS_N00_Success,    
      C060_E02 = RCS_E02_InvalidSelection,
      C060_E05 = RCS_E05_TooFewDataBytesReceived,
      C060_E06 = RCS_E06_DeviceSpecificCommandError,
      C060_W08 = RCM_W08_UpdateFailure,
      C060_E16 = RCS_E16_AccessRestricted
}C060_ReadAnalogChannelAndPercentOfRange_RespCodes;


/******************** CMD 062 *************************/

typedef struct
{
	uint8_t noOfChannelsNumberCodes;
	AnalogChannelNumberCodes channelsNumberCodes[4];
}C062_ReadAnalogChannels_Req;
typedef struct
{
	uint8_t noOfChannelsNumberCodes; //no need to assign this memeber (harcoded = 4)
	struct
	{
		AnalogChannelNumberCodes channelNumberCode;
		UnitsCodes unitsCode;
		float channelLevel;
	} channels[4];
}C062_ReadAnalogChannels_Resp;
typedef enum
{
      C062_NOO = RCS_N00_Success,  
      C062_E02 = RCS_E02_InvalidSelection,
      C062_E05 = RCS_E05_TooFewDataBytesReceived,
      C062_E06 = RCS_E06_DeviceSpecificCommandError,
      C062_W08 = RCM_W08_UpdateFailure
}C062_ReadAnalogChannels_RespCodes;


/******************** CMD 063 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
}C063_ReadAnalogChannelInformation_Req;
typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	AlarmSelectionCodes alarmSelectionCode;
	TransferFunctionCodes transferFunctionCode;
	UnitsCodes upperLowerRangeUnitsCode;
	float upperRangeValue;
	float lowerRangeValue;
	float dampingValue;
	uint8_t analogChannelFlags;
}C063_ReadAnalogChannelInformation_Resp;
typedef enum
{
      C063_NOO = RCS_N00_Success,  
      C063_E02 = RCS_E02_InvalidSelection,
      C063_E05 = RCS_E05_TooFewDataBytesReceived,
      C063_E06 = RCS_E06_DeviceSpecificCommandError,
      C063_E16 = RCS_E16_AccessRestricted,
      C063_E32 = RCS_E32_Busy
}C063_ReadAnalogChannelInformation_RespCodes;


/******************** CMD 064 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	float dampingValue; //units of seconds
}C064_WriteAnalogChannelAdditionalDampingValue_Req;
typedef C064_WriteAnalogChannelAdditionalDampingValue_Req C064_WriteAnalogChannelAdditionalDampingValue_Resp;
typedef enum
{
      C064_NOO = RCS_N00_Success,    
      C064_E02 = RCS_E02_InvalidSelection,
      C064_E03 = RCS_E03_PassedParameterTooLarge,
      C064_E04 = RCS_E04_PassedParameterTooSmall,
      C064_E05 = RCS_E05_TooFewDataBytesReceived,
      C064_E06 = RCS_E06_DeviceSpecificCommandError,
      C064_E07 = RCS_E07_InWriteProtectMode,
      C064_W08 = RCM_W08_SetToNearestPossibleValue,
      C064_E16 = RCS_E16_AccessRestricted,
      C064_E32 = RCS_E32_Busy
}C064_WriteAnalogChannelAdditionalDampingValue_RespCodes;


/******************** CMD 065 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	UnitsCodes upperLowerRangeUnitsCode;
	float upperRangeValue;
	float lowerRangeValue;
}C065_WriteAnalogChannelRangeValues_Req;
typedef C065_WriteAnalogChannelRangeValues_Req C065_WriteAnalogChannelRangeValues_Resp;
typedef enum
{
      C065_NOO = RCS_N00_Success,    
      C065_E05 = RCS_E05_TooFewDataBytesReceived,
      C065_E06 = RCS_E06_DeviceSpecificCommandError,
      C065_E07 = RCS_E07_InWriteProtectMode,
      C065_W08 = RCM_W08_SetToNearestPossibleValue,  // (Upper or Lower Range Pushed)
      C065_E09 = RCM_E09_LowerRangeValueTooHigh,
      C065_E10 = RCM_E10_LowerRangeValueTooLow,
      C065_E11 = RCM_E11_UpperRangeValueTooHigh,
      C065_E12 = RCM_E12_UpperRangeValueTooLow,
      C065_E13 = RCM_E13_UpperAndLowerRangeValuesOutOfLimits,
      C065_W14 = RCM_W14_SpanTooSmall,  // (Device Accuracy May Be Impaired)
      C065_E15 = RCM_E15_InvalidAnalogChannelCodeNumber,
      C065_E16 = RCS_E16_AccessRestricted,
      C065_E28 = RCM_E28_InvalidRangeUnitsCode,
      C065_E29 = RCM_E29_InvalidSpan,
      C065_E32 = RCS_E32_Busy
}C065_WriteAnalogChannelRangeValues_RespCodes;


/******************** CMD 066 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	UnitsCodes unitsCode;
	float channelLevel;
}C066_EnterExitFixedAnalogChannelMode_Req;
typedef C066_EnterExitFixedAnalogChannelMode_Req C066_EnterExitFixedAnalogChannelMode_Resp;
typedef enum
{
      C066_NOO = RCS_N00_Success,     
      C066_E03 = RCS_E03_PassedParameterTooLarge,
      C066_E04 = RCS_E04_PassedParameterTooSmall,
      C066_E05 = RCS_E05_TooFewDataBytesReceived,
      C066_E06 = RCS_E06_DeviceSpecificCommandError,
      C066_E07 = RCS_E07_InWriteProtectMode,
      C066_E11 = RCM_E11_InMultidropMode,
      C066_E12 = RCM_E12_InvalidUnitsCode,
      C066_E15 = RCM_E15_InvalidAnalogChannelCodeNumber,
      C066_E16 = RCS_E16_AccessRestricted,
      C066_E32 = RCS_E32_Busy
}C066_EnterExitFixedAnalogChannelMode_RespCodes;


/******************** CMD 067 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	UnitsCodes unitsCode;
	float channelLevel;
}C067_TrimAnalogChannelZero_Req;
typedef C067_TrimAnalogChannelZero_Req C067_TrimAnalogChannelZero_Resp;
typedef enum
{
      C067_NOO = RCS_N00_Success,   
      C067_E03 = RCS_E03_PassedParameterTooLarge,
      C067_E04 = RCS_E04_PassedParameterTooSmall,
      C067_E05 = RCS_E05_TooFewDataBytesReceived,
      C067_E06 = RCS_E06_DeviceSpecificCommandError,
      C067_E07 = RCS_E07_InWriteProtectMode,
      C067_E09 = RCM_E09_NotInProperAnalogChannelMode,
      C067_E11 = RCM_E11_InMultidropMode,
      C067_E12 = RCM_E12_InvalidUnitsCode,
      C067_E15 = RCM_E15_InvalidAnalogChannelCodeNumber,
      C067_E16 = RCS_E16_AccessRestricted,
      C067_E32 = RCS_E32_Busy
}C067_TrimAnalogChannelZero_RespCodes;


/******************** CMD 068 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	UnitsCodes unitsCode;
	float channelLevel;
}C068_TrimAnalogChannelGain_Req;
typedef C068_TrimAnalogChannelGain_Req C068_TrimAnalogChannelGain_Resp;
typedef enum
{
      C068_NOO = RCS_N00_Success,  
      C068_E03 = RCS_E03_PassedParameterTooLarge,
      C068_E04 = RCS_E04_PassedParameterTooSmall,
      C068_E05 = RCS_E05_TooFewDataBytesReceived,
      C068_E06 = RCS_E06_DeviceSpecificCommandError,
      C068_E07 = RCS_E07_InWriteProtectMode, 
      C068_E09 = RCM_E09_NotInProperAnalogChannelMode,
      C068_E11 = RCM_E11_InMultidropMode,
      C068_E12 = RCM_E12_InvalidUnitsCode,
      C068_E15 = RCM_E15_InvalidAnalogChannelCodeNumber,
      C068_E16 = RCS_E16_AccessRestricted,
      C068_E32 = RCS_E32_Busy      
}C068_TrimAnalogChannelGain_RespCodes;


/******************** CMD 069 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	TransferFunctionCodes transferFunctionCode;
}C069_WriteAnalogChannelTransferFunction_Req;
typedef C069_WriteAnalogChannelTransferFunction_Req C069_WriteAnalogChannelTransferFunction_Resp;
typedef enum
{
      C069_NOO = RCS_N00_Success,  
      C069_E05 = RCS_E05_TooFewDataBytesReceived,
      C069_E06 = RCS_E06_DeviceSpecificCommandError,
      C069_E07 = RCS_E07_InWriteProtectMode,  
      C069_E13 = RCM_E13_InvalidTransferFunctionCode,
      C069_E15 = RCM_E15_InvalidAnalogChannelCodeNumber,
      C069_E16 = RCS_E16_AccessRestricted,
      C069_E32 = RCS_E32_Busy         
}C069_WriteAnalogChannelTransferFunction_RespCodes;


/******************** CMD 070 *************************/

typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
}C070_ReadAnalogChannelEndpointValues_Req;
typedef enum
{
      C070_NOO = RCS_N00_Success,  
      C070_E02 = RCS_E02_InvalidSelection,
      C070_E05 = RCS_E05_TooFewDataBytesReceived,
      C070_E06 = RCS_E06_DeviceSpecificCommandError,
      C070_E16 = RCS_E16_AccessRestricted,
      C070_E32 = RCS_E32_Busy         
}C070_ReadAnalogChannelEndpointValues_RespCodes;
typedef struct
{
	AnalogChannelNumberCodes channelNumberCode;
	UnitsCodes upperLowerRangeUnitsCode;
	float upperEndpointValue;
	float lowerEndpointValue;
}C070_ReadAnalogChannelEndpointValues_Resp;


/******************** CMD 071 *************************/

typedef struct
{
	LockDeviceCodes lockCode;
}C071_LockDevice_Req;
typedef C071_LockDevice_Req C071_LockDevice_Resp;
typedef enum
{
      C071_NOO = RCS_N00_Success,   
      C071_E05 = RCS_E05_TooFewDataBytesReceived,
      C071_E06 = RCS_E06_DeviceSpecificCommandError,
      C071_E10 = RCM_E10_InvalidLockCode,
      C071_E11 = RCM_E11_CannotLockDevice,
      C071_E16 = RCS_E16_AccessRestricted,
      C071_E32 = RCS_E32_Busy,
      C071_E33 = RCS_E33_DelayedResponseInitiated,
      C071_E34 = RCS_E34_DelayedResponseRunning,
      C071_E35 = RCS_E35_DelayedResponseDead,
      C071_E36 = RCS_E36_DelayedResponseConflict
}C071_LockDevice_RespCodes;


/******************** CMD 072 *************************/

typedef EmptyCommand_Req C072_Squawk_Req;
typedef EmptyCommand_Resp C072_Squawk_Resp;
typedef enum
{
      C072_NOO = RCS_N00_Success,   
      C072_E06 = RCS_E06_DeviceSpecificCommandError,      
      C072_E09 = RCM_E09_UnableToSquawk,
      C072_E16 = RCS_E16_AccessRestricted,
      C072_E32 = RCS_E32_Busy,      
}C072_Squawk_RespCodes;


/******************** CMD 073 *************************/

typedef EmptyCommand_Req C073_FindDevice_Req;
typedef C000_ReadUniqueIdentifier_Resp C073_FindDevice_Resp;
typedef enum
{
      C073_NOO = RCS_N00_Success
}C073_FindDevice_RespCodes;


/******************** CMD 074 *************************/

typedef EmptyCommand_Req C074_ReadIOSystemCapabilities_Req;
typedef struct
{
	uint8_t noOfIOCards;
	uint8_t noOfChannelsPerIOCard;
	uint8_t noOfSubdevicesPerChannel;
	uint16_t noOfDevicesDetected;
	uint8_t noOfDelayedResponsesSupported;
	uint8_t masterMode; //0 = Secondary Master; 1 = Primary Master (default)
	uint8_t sendRetryCount; //to use when sending commands to an sub-device. Valid range is 2 to 5. (default= 3 retries)
}C074_ReadIOSystemCapabilities_Resp;
typedef enum
{
      C074_NOO = RCS_N00_Success,
      C074_E06 = RCS_E06_DeviceSpecificCommandError,
      C074_E16 = RCS_E16_AccessRestricted,
      C074_E32 = RCS_E32_Busy
}C074_ReadIOSystemCapabilities_RespCodes;


/******************** CMD 075 *************************/

typedef struct
{
	uint8_t ioCard;
	uint8_t channel;
	uint8_t pollingAddress;
}C075_PollSubDevice_Req;
typedef C000_ReadUniqueIdentifier_Resp C075_PollSubDevice_Resp;
typedef enum
{
      C075_NOO = RCS_N00_Success,  
      C075_E02 = RCS_E02_InvalidSelection,
      C075_E05 = RCS_E05_TooFewDataBytesReceived,
      C075_E06 = RCS_E06_DeviceSpecificCommandError,
      C075_E09 = RCM_E09_NoSubDeviceFound,
      C075_E16 = RCS_E16_AccessRestricted,
      C075_E21 = RCS_E21_InvalidIOCardNumber,
      C075_E22 = RCS_E22_InvalidChannelNumber,
      C075_E32 = RCS_E32_Busy,
      C075_E33 = RCS_E33_DelayedResponseInitiated,
      C075_E34 = RCS_E34_DelayedResponseRunning,
      C075_E35 = RCS_E35_DelayedResponseDead,
      C075_E36 = RCS_E36_DelayedResponseConflict      
}C075_PollSubDevice_RespCodes;


/******************** CMD 076 *************************/

typedef EmptyCommand_Req C076_ReadLockDeviceState_Req;
typedef struct
{
	uint8_t lockStatus;
}C076_ReadLockDeviceState_Resp;
typedef enum
{
      C076_NOO = RCS_N00_Success,  
      C076_E06 = RCS_E06_DeviceSpecificCommandError,
      C076_E16 = RCS_E16_AccessRestricted,
      C076_E32 = RCS_E32_Busy
}C076_ReadLockDeviceState_RespCodes;


/******************** CMD 077 *************************/

typedef struct
{
	uint8_t ioCard;
	uint8_t channel;
	uint8_t transmitPreambleCount; //Valid range is 5 to 20
	//INFO: Delimiter values : Token-Passing Data Link Layer Specification.pdf
	//Bits 0-2 (0x07) indicate the Frame Type and they can only have a value of(1,2,6): 1=BACK (Burst Frame),	2=STX (Master to Field Device),	6=ACK (Field Device to Master)
	//Bits 3-4 (0x18) indicate the Physical Layer 0=Asynchronous, 1=Synchronous (i.e., PSK)
	//Bits 5-6 (0x60) are used for frame expansion. Since these bits are reserved for Foundation use and not currently assigned 0(zero) is the only legal value.
	//Bit 7 (0x80) indicates Address Type: 0=Poling (1 Byte) Address, 1=Unique (5 Byte) Address
	uint8_t frameType;
	uint8_t physicalLayerType;
	uint8_t noOfExpansionBytes; //not used: always zero
	uint8_t addressType; //0=1 Byte Addres, 1=5 Byte Address INFO: Command 0 is unique in that it is the only command that still supports short frame messages.
	uint8_t fieldAddress[LongFieldAddressSize];
	uint8_t commandOfEmbeddedCommand;
	uint8_t embeddedCommandSize;
	uint8_t embeddedCommand[MaxEmbeddedCommandsSize];
}C077_SendCommandToSubDevice_Req;
typedef struct
{
	uint8_t ioCard;
	uint8_t channel;
	//embededCommandDelimiter;
	uint8_t frameType;
	uint8_t physicalLayerType;
	uint8_t noOfExpansionBytes; //not used: always zero
	uint8_t addressType; //0=1 Byte Addres, 1=5 Byte Address INFO: Command 0 is unique in that it is the only command that still supports short frame messages.

	uint8_t fieldAddress[LongFieldAddressSize];
	uint8_t commandOfEmbeddedCommand;
	uint8_t embeddedCommandSize;
	uint8_t embeddedCommand[MaxEmbeddedCommandsSize];
}C077_SendCommandToSubDevice_Resp;
typedef enum
{
      C077_NOO = RCS_N00_Success,  
      C077_E02 = RCS_E02_InvalidSelection,
      C077_E05 = RCS_E05_TooFewDataBytesReceived,
      C077_E06 = RCS_E06_DeviceSpecificCommandError,
      C077_E21 = RCS_E21_InvalidIOCardNumber,
      C077_E22 = RCS_E22_InvalidChannelNumber,
      C077_E23 = RCS_E23_SubdeviceResponseTooLong,
      C077_E32 = RCS_E32_Busy,
      C077_E33 = RCS_E33_DelayedResponseInitiated,
      C077_E34 = RCS_E34_DelayedResponseRunning,
      C077_E35 = RCS_E35_DelayedResponseDead,
      C077_E36 = RCS_E36_DelayedResponseConflict      
}C077_SendCommandToSubDevice_RespCodes;


/******************** CMD 078 *************************/

typedef struct
{
	uint8_t noOfCommands; // noOfCommands <= MaxNoOfEmbededCommands 
        uint8_t cmdSize;
	uint8_t cmdData[90];
        
}C078_ReadAggregatedCommands_Req;
typedef struct
{
	uint8_t extendedDeviceStatus;
	uint8_t noOfCommands; // noOfCommands <= MaxNoOfEmbededCommands
        uint8_t cmdSize;
	uint8_t cmdData[90];
}C078_ReadAggregatedCommands_Resp;
typedef enum
{
      C078_NOO = RCS_N00_Success,  
      C078_E02 = RCS_E02_InvalidSelection,  
      C078_E05 = RCS_E05_TooFewDataBytesReceived,
      C078_E06 = RCS_E06_DeviceSpecificCommandError,
      C078_W08 = RCM_W08_UpdateFailure,
      C078_E09 = RCM_E09_InvalidCommandRequested,
      C078_W30 = RCM_W30_CommandResponseTruncated,
      C078_E32 = RCS_E32_Busy,
      C078_E33 = RCS_E33_DelayedResponseInitiated,
      C078_E34 = RCS_E34_DelayedResponseRunning,
      C078_E35 = RCS_E35_DelayedResponseDead,
      C078_E36 = RCS_E36_DelayedResponseConflict          
}C078_ReadAggregatedCommands_RespCodes;


/******************** CMD 079 *************************/

typedef struct
{
	DeviceVariableCodes deviceVariableCode;
	WriteDeviceVariableCodes writeCommandCode;
	UnitsCodes unitsCode;
	float value;
	uint8_t deviceVariableStatus; //TODO:[Ovidiu] - find and define this enum DeviceVariableStatus
}C079_WriteDeviceVariable_Req;
typedef C079_WriteDeviceVariable_Req C079_WriteDeviceVariable_Resp;
typedef enum
{
      C079_NOO = RCS_N00_Success,   
      C079_E02 = RCS_E02_InvalidSelection,
      C079_E05 = RCS_E05_TooFewDataBytesReceived,
      C079_E06 = RCS_E06_DeviceSpecificCommandError,
      C079_E07 = RCS_E07_InWriteProtectMode,
      C079_W08 = RCM_W08_DeviceFamilyStatusBitNotSet,
      C079_E10 = RCM_E10_InvalidWriteDeviceVariableCode,
      C079_E16 = RCS_E16_AccessRestricted,
      C079_E17 = RCS_E17_InvalidDeviceVariableIndex,
      C079_E18 = RCS_E18_InvalidUnitsCode,
      C079_E19 = RCS_E19_DeviceVariableIndexNotAllowed,
      C079_E32 = RCS_E32_Busy,
      C079_E33 = RCS_E33_DelayedResponseInitiated,
      C079_E34 = RCS_E34_DelayedResponseRunning,
      C079_E35 = RCS_E35_DelayedResponseDead,
      C079_E36 = RCS_E36_DelayedResponseConflict
}C079_WriteDeviceVariable_RespCodes;


/******************** CMD 080 *************************/

typedef struct
{
	DeviceVariableCodes deviceVariableCode;
}C080_ReadDeviceVariableTrimPoints_Req;
typedef struct
{
	DeviceVariableCodes deviceVariableCode;
	UnitsCodes trimUnitsCode;
	float lowerTrimPoint;
	float upperTrimPoint;
}C080_ReadDeviceVariableTrimPoints_Resp;
typedef enum
{
      C080_NOO = RCS_N00_Success,   
      C080_E05 = RCS_E05_TooFewDataBytesReceived,
      C080_E06 = RCS_E06_DeviceSpecificCommandError,
      C080_E17 = RCS_E17_InvalidDeviceVariableIndex,
      C080_E18 = RCS_E18_InvalidUnitsCode,
      C080_E19 = RCS_E19_DeviceVariableIndexNotAllowed,
      C080_E32 = RCS_E32_Busy
}C080_ReadDeviceVariableTrimPoints_RespCodes;


/******************** CMD 081 *************************/

typedef struct
{
	DeviceVariableCodes deviceVariableCode;
}C081_ReadDeviceVariableTrimGuidelines_Req;
typedef struct
{
	DeviceVariableCodes deviceVariableCode;
	TrimPointCodes trimPointsSupported;
	UnitsCodes trimUnitsCode; //TODO: search and define this
	float minLowerTrimPoint;
	float maxLowerTrimPoint;
	float minUpperTrimPoint;
	float maxUpperTrimPoint;
	float minDifferential; //minimum acceptable difference between upper and lower trim points
}C081_ReadDeviceVariableTrimGuidelines_Resp;
typedef enum
{
      C081_NOO = RCS_N00_Success,   
      C081_E05 = RCS_E05_TooFewDataBytesReceived,
      C081_E06 = RCS_E06_DeviceSpecificCommandError,
      C081_E17 = RCS_E17_InvalidDeviceVariableIndex,
      C081_E18 = RCS_E18_InvalidUnitsCode,
      C081_E19 = RCS_E19_DeviceVariableIndexNotAllowed,
      C081_E32 = RCS_E32_Busy
}C081_ReadDeviceVariableTrimGuidelines_RespCodes;


/******************** CMD 082 *************************/
typedef struct
{
	DeviceVariableCodes deviceVariableCode;
	TrimPointCodes trimPoint;
	UnitsCodes trimUnitsCode;
	float value;
}C082_WriteDeviceVariableTrimPoint_Req;
typedef C082_WriteDeviceVariableTrimPoint_Req C082_WriteDeviceVariableTrimPoint_Resp;
typedef enum
{
      C082_NOO = RCS_N00_Success,   
      C082_E02 = RCS_E02_InvalidSelection,
      C082_E03 = RCS_E03_PassedParameterTooLarge,
      C082_E04 = RCS_E04_PassedParameterTooSmall,
      C082_E05 = RCS_E05_TooFewDataBytesReceived,
      C082_E06 = RCS_E06_DeviceSpecificCommandError,
      C082_E07 = RCS_E07_InWriteProtectMode,
      C082_E09 = RCM_E09_AppliedProcessTooHigh,
      C082_E10 = RCM_E10_AppliedProcessTooLow,
      C082_E11 = RCM_E11_TrimErrorExcessCorrectionAttempted,
      C082_E13 = RCM_E13_ComputationError, // TrimValuesWereNotChanged
      C082_W14 = RCM_W14_SpanTooSmall,
      C082_E16 = RCS_E16_AccessRestricted,
      C082_E17 = RCS_E17_InvalidDeviceVariableIndex,
      C082_E18 = RCS_E18_InvalidUnitsCode,
      C082_E19 = RCS_E19_DeviceVariableIndexNotAllowed,
      C082_E32 = RCS_E32_Busy,
      C082_E33 = RCS_E33_DelayedResponseInitiated,
      C082_E34 = RCS_E34_DelayedResponseRunning,
      C082_E35 = RCS_E35_DelayedResponseDead,
      C082_E36 = RCS_E36_DelayedResponseConflict
}C082_WriteDeviceVariableTrimPoint_RespCodes;


/******************** CMD 083 *************************/

typedef struct
{
	DeviceVariableCodes deviceVariableCode;
}C083_ResetDeviceVariableTrim_Req;
typedef C083_ResetDeviceVariableTrim_Req C083_ResetDeviceVariableTrim_Resp;
typedef enum
{
      C083_NOO = RCS_N00_Success,   
      C083_E02 = RCS_E02_InvalidSelection,
      C083_E05 = RCS_E05_TooFewDataBytesReceived,
      C083_E07 = RCS_E07_InWriteProtectMode,
      C083_E16 = RCS_E16_AccessRestricted,
      C083_E17 = RCS_E17_InvalidDeviceVariableIndex,
      C083_E19 = RCS_E19_DeviceVariableIndexNotAllowed,
      C083_E32 = RCS_E32_Busy,
      C083_E33 = RCS_E33_DelayedResponseInitiated,
      C083_E34 = RCS_E34_DelayedResponseRunning,
      C083_E35 = RCS_E35_DelayedResponseDead,
      C083_E36 = RCS_E36_DelayedResponseConflict
}C083_ResetDeviceVariableTrim_RespCodes;


/******************** CMD 084 *************************/

typedef struct
{
	uint16_t subDeviceIndex;
}C084_ReadSubDeviceIdentitySummary_Req;
typedef struct
{
	uint16_t subDeviceIndex;
	uint8_t ioCard;
	uint8_t channel;
	uint16_t manufacturerID;
	ExpandedDeviceTypeCodes expandedDeviceType; //uint16_t
	uint32_t deviceID; //CHECKME:[Ovidiu] sure is Unsigned-25 and not Unsigned 24???
	uint8_t revisionLevel;
	uint8_t tag[32];
}C084_ReadSubDeviceIdentitySummary_Resp;
typedef enum
{
      C084_NOO = RCS_N00_Success,   
      C084_E02 = RCS_E02_InvalidSelection,
      C084_E05 = RCS_E05_TooFewDataBytesReceived,
      C084_E06 = RCS_E06_DeviceSpecificCommandError
}C084_ReadSubDeviceIdentitySummary_RespCodes;


/******************** CMD 085 *************************/

typedef struct
{
	uint8_t ioCard;
	uint8_t channel;
}C085_ReadIOChannelStatistics_Req;
typedef struct
{
	uint8_t ioCard;
	uint8_t channel;
	uint16_t stxMessagesSentCount;
	uint16_t ackMessagesReceivedCount;
	uint16_t ostxMessagesReceivedCount;
	uint16_t oackMessagesReceivedCount;
	uint16_t backMessagesReceivedCount;
}C085_ReadIOChannelStatistics_Resp;
typedef enum
{
      C085_NOO = RCS_N00_Success,   
      C085_E02 = RCS_E02_InvalidSelection,
      C085_E05 = RCS_E05_TooFewDataBytesReceived,
      C085_E06 = RCS_E06_DeviceSpecificCommandError,
      C085_E21 = RCS_E21_InvalidIOCardNumber,
      C085_E22 = RCS_E22_InvalidChannelNumber,
      C085_E32 = RCS_E32_Busy
}C085_ReadIOChannelStatistics_RespCodes;


/******************** CMD 086 *************************/

typedef struct
{
	uint16_t subDeviceIndex;
}C086_ReadSubDeviceStatistics_Req;
typedef struct
{
	uint16_t subDeviceIndex;
	uint16_t stxMessagesSentCount;
	uint16_t ackMessagesReceivedCount;
	uint16_t backMessagesReceivedCount;
}C086_ReadSubDeviceStatistics_Resp;
typedef enum
{
      C086_NOO = RCS_N00_Success,   
      C086_E02 = RCS_E02_InvalidSelection,
      C086_E05 = RCS_E05_TooFewDataBytesReceived,
      C086_E06 = RCS_E06_DeviceSpecificCommandError,
}C086_ReadSubDeviceStatistics_RespCodes;


/******************** CMD 087 *************************/

typedef struct
{
	uint8_t masterMode; //0 = Secondary Master; 1 = Primary Master
}C087_WriteIOSystemMasterMode_Req;
typedef C087_WriteIOSystemMasterMode_Req C087_WriteIOSystemMasterMode_Resp;
typedef enum
{
      C087_NOO = RCS_N00_Success,   
      C087_E02 = RCS_E02_InvalidSelection,
      C087_E05 = RCS_E05_TooFewDataBytesReceived,
      C087_E06 = RCS_E06_DeviceSpecificCommandError,
      C087_E07 = RCS_E07_InWriteProtectMode,
      C087_E16 = RCS_E16_AccessRestricted
}C087_WriteIOSystemMasterMode_RespCodes;


/******************** CMD 088 *************************/

typedef struct
{
	uint8_t retryCount;
}C088_WriteIOSystemRetryCount_Req;
typedef C088_WriteIOSystemRetryCount_Req C088_WriteIOSystemRetryCount_Resp;
typedef enum
{
      C088_NOO = RCS_N00_Success,   
      C088_E03 = RCS_E03_PassedParameterTooLarge,
      C088_E04 = RCS_E04_PassedParameterTooSmall,
      C088_E05 = RCS_E05_TooFewDataBytesReceived,
      C088_E06 = RCS_E06_DeviceSpecificCommandError,
      C088_E07 = RCS_E07_InWriteProtectMode,
      C088_E16 = RCS_E16_AccessRestricted
}C088_WriteIOSystemRetryCount_RespCodes;


/******************** CMD 089 *************************/

typedef struct
{
	TimeSetCodes timeSetCode;
	_date_t date;
	_time_t time;
	uint16_t nothingSpecial; //Should be set to 0. Two bytes to ensure request and response take equal	amounts of time
}C089_SetRealTimeClock_Req;
typedef struct
{
	TimeSetCodes timeSetCode;
	_date_t date;
	_time_t time;
}C089_SetRealTimeClock_Resp;
typedef enum
{
      C089_NOO = RCS_N00_Success,   
      C089_E02 = RCS_E02_InvalidSelection,
      C089_E03 = RCS_E03_PassedParameterTooLarge,
      C089_E05 = RCS_E05_TooFewDataBytesReceived,
      C089_E06 = RCS_E06_DeviceSpecificCommandError,
      C089_E07 = RCS_E07_InWriteProtectMode,
      C089_E16 = RCS_E16_AccessRestricted
}C089_SetRealTimeClock_RespCodes;


/******************** CMD 090 *************************/

typedef EmptyCommand_Req C090_ReadRealTimeClock_Req;
typedef struct
{
	_date_t currentDate;
	_time_t currentTime;
	_date_t lastDateSet;
	_time_t lastTimeSet;
	uint8_t rtcFlags;
}C090_ReadRealTimeClock_Resp;
typedef enum
{
      C090_NOO = RCS_N00_Success,   
      C090_E06 = RCS_E06_DeviceSpecificCommandError
}C090_ReadRealTimeClock_RespCodes;


/******************** CMD 091 *************************/

typedef struct
{
	uint8_t trendNo;
}C091_ReadTrendConfiguration_Req;
typedef struct
{
	uint8_t trendNo;
	uint8_t totalNoOfTrendsSupported;
	TrendControlCodes trendControlCode;
	DeviceVariableCodes deviceVariableCode;
	_time_t trendSampleInterval; //maximum is 2h: one trend per day 0x0DBBA000
}C091_ReadTrendConfiguration_Resp;
typedef enum
{
      C091_NOO = RCS_N00_Success,  
      C091_E05 = RCS_E05_TooFewDataBytesReceived,
      C091_E06 = RCS_E06_DeviceSpecificCommandError,      
      C091_E11 = RCM_E11_InvalidTrendNumber,
}C091_ReadTrendConfiguration_RespCodes;


/******************** CMD 092 *************************/

typedef struct
{
	uint8_t trendNo;
	TrendControlCodes trendControlCode;
	DeviceVariableCodes deviceVariableCode;
	_time_t trendSamplePeriod; //maximum is 2h: one trend per day 0x0DBBA000
}C092_WriteTrendConfiguration_Req;
typedef C092_WriteTrendConfiguration_Req C092_WriteTrendConfiguration_Resp;
typedef enum
{
      C092_NOO = RCS_N00_Success,    
      C092_E02 = RCS_E02_InvalidSelection,
      C092_E03 = RCS_E03_PassedParameterTooLarge,
      C092_E04 = RCS_E04_PassedParameterTooSmall,
      C092_E05 = RCS_E05_TooFewDataBytesReceived,
      C092_E06 = RCS_E06_DeviceSpecificCommandError,   
      C092_E07 = RCS_E07_InWriteProtectMode,
      C092_W08 = RCM_W08_SetToNearestPossibleValue,
      C092_E11 = RCM_E11_InvalidTrendNumber,
      C092_E16 = RCS_E16_AccessRestricted,
      C092_E17 = RCS_E17_InvalidDeviceVariableIndex
}C092_WriteTrendConfiguration_RespCodes;


/******************** CMD 093 *************************/

enum
{
	C093_NoOfTrendValues = 12
};
typedef struct
{
	uint8_t trendNo;
}C093_ReadTrend_Req;
typedef struct
{
	uint8_t trendNo;
	DeviceVariableCodes deviceVariableCode;
	DeviceVariableClassificationCodes deviceVariableClassificationCode;
	uint8_t deviceVariableUnitCode; //TODO:[Ovidiu] - define Device Variable Unit Code
	_date_t dateOfTrendValue0;
	_time_t timeOfTrendValue0;
	_time_t sampleIterval;
	struct
	{
		float trendValue;
		uint8_t trendValueStatus;// TODO[Ovidiu]: define Trend Value n Status
	} trendValues[C093_NoOfTrendValues];
}C093_ReadTrend_Resp;
typedef enum
{
      C093_NOO = RCS_N00_Success,   
      C093_E05 = RCS_E05_TooFewDataBytesReceived,
      C093_E06 = RCS_E06_DeviceSpecificCommandError,     
      C093_W08 = RCM_W08_TrendNotActive,
      C093_E11 = RCM_E11_InvalidTrendNumber
}C093_ReadTrend_RespCodes;


/******************** CMD 094 *************************/

typedef EmptyCommand_Req C094_ReadIOSystemClientSideCommunicationStatistics_Req;
typedef struct
{
	uint16_t noOfMessagesReceivedThroughHostSystem;
	uint16_t noOfMessagesReturnedThroughHostSystem;
	uint16_t noOfRequestsForwardedToIOSystem;
	uint16_t noOfResponsesReturnedFromIOSystem;
}C094_ReadIOSystemClientSideCommunicationStatistics_Resp;
typedef enum
{
      C094_NOO = RCS_N00_Success,   
      C094_E06 = RCS_E06_DeviceSpecificCommandError,     
      C094_E16 = RCS_E16_AccessRestricted
}C094_ReadIOSystemClientSideCommunicationStatistics_RespCodes;


/******************** CMD 095 *************************/

typedef EmptyCommand_Req C095_ReadDeviceCommunicationsStatistics_Req;
typedef struct
{
	uint16_t stxMessagesReceivedCount;
	uint16_t ackMessagesSentCount;
	uint16_t backMessagesSentCount;
}C095_ReadDeviceCommunicationsStatistics_Resp;
typedef enum
{
      C095_NOO = RCS_N00_Success,   
      C095_E06 = RCS_E06_DeviceSpecificCommandError
}C095_ReadDeviceCommunicationsStatistics_RespCodes;


/******************** CMD 096 *************************/

typedef struct
{
	uint8_t actionNo;
}C096_ReadSynchronousAction_Req;
typedef struct
{
	uint8_t actionNo;
	uint8_t totalNoOfActions;
	uint8_t actionControl;
	DeviceVariableCodes deviceVariableCode;
	uint16_t commandNo; //If action is sampling a Device Variable, the Command Number must be set to 0xFFFF.
	_date_t triggerDate;
	_time_t triggerTime;
}C096_ReadSynchronousAction_Resp;
typedef enum
{
      C096_NOO = RCS_N00_Success,   
      C096_E02 = RCS_E02_InvalidSelection,
      C096_E05 = RCS_E05_TooFewDataBytesReceived,
      C096_E06 = RCS_E06_DeviceSpecificCommandError
}C096_ReadSynchronousAction_RespCodes;


/******************** CMD 097 *************************/

typedef struct
{
	uint8_t actionNo;
	uint8_t actionControl;
	DeviceVariableCodes deviceVariableCode;
	uint16_t commandNo; //If action is sampling a Device Variable, the Command Number must be set to 0xFFFF.
	_date_t triggerDate;
	_time_t triggerTime;
}C097_ConfigureSynchronousAction_Req;
typedef C097_ConfigureSynchronousAction_Req C097_ConfigureSynchronousAction_Resp;
typedef enum
{
      C097_NOO = RCS_N00_Success,  
      C097_E02 = RCS_E02_InvalidSelection,
      C097_E05 = RCS_E05_TooFewDataBytesReceived,
      C097_E06 = RCS_E06_DeviceSpecificCommandError,
      C097_E07 = RCS_E07_InWriteProtectMode,
      C097_W08 = RCM_W08_SamplingTimeAdjusted,
      C097_E09 = RCM_E09_BadTriggerAction,
      C097_E10 = RCM_E10_InvalidDate,
      C097_E11 = RCM_E11_InvalidTime,
      C097_E12 = RCM_E12_InvalidDeviceVariable,
      C097_E13 = RCM_E13_CommandNumberNotSupported,
      C097_E16 = RCS_E16_AccessRestricted,
      C097_E32 = RCS_E32_Busy,
      C097_E33 = RCS_E33_DelayedResponseInitiated,
      C097_E34 = RCS_E34_DelayedResponseRunning,
      C097_E35 = RCS_E35_DelayedResponseDead,
      C097_E36 = RCS_E36_DelayedResponseConflict
}C097_ConfigureSynchronousAction_RespCodes;


/******************** CMD 098 *************************/

typedef struct
{
	uint8_t triggerAction;
}C098_ReadCommandAction_Req;
typedef struct
{
	uint8_t triggerAction;
	uint16_t commandNo;
	uint8_t commandSize;
	uint8_t commandData[MaxEmbeddedCommandsSize];
}C098_ReadCommandAction_Resp;
typedef enum
{
      C098_NOO = RCS_N00_Success,   
      C098_E02 = RCS_E02_InvalidSelection,
      C098_E05 = RCS_E05_TooFewDataBytesReceived,
      C098_E06 = RCS_E06_DeviceSpecificCommandError
}C098_ReadCommandAction_RespCodes;


/******************** CMD 099 *************************/

typedef struct
{
	uint8_t triggerAction;
	uint16_t commandNo;
	uint8_t commandSize;
	uint8_t commandData[MaxEmbeddedCommandsSize];
}C099_ConfigureCommandAction_Req;
typedef C099_ConfigureCommandAction_Req C099_ConfigureCommandAction_Resp;
typedef enum
{
      C099_NOO = RCS_N00_Success,  
      C099_E02 = RCS_E02_InvalidSelection,
      C099_E05 = RCS_E05_TooFewDataBytesReceived,
      C099_E06 = RCS_E06_DeviceSpecificCommandError,
      C099_E07 = RCS_E07_InWriteProtectMode,
      C099_W08 = RCM_W08_SamplingTimeAdjusted,
      C099_E09 = RCM_E09_BadTriggerAction,
      C099_E10 = RCM_E10_BadCommandData,
      C099_E13 = RCM_E13_CommandNumberNotSupported,
      C099_E16 = RCS_E16_AccessRestricted,
      C099_E32 = RCS_E32_Busy,
      C099_E33 = RCS_E33_DelayedResponseInitiated,
      C099_E34 = RCS_E34_DelayedResponseRunning,
      C099_E35 = RCS_E35_DelayedResponseDead,
      C099_E36 = RCS_E36_DelayedResponseConflict
}C099_ConfigureCommandAction_RespCodes;


/******************** CMD 101 *************************/

typedef struct
{
	uint8_t burstMessage;
}C101_ReadSubDeviceToBurstMessageMap_Req;
typedef struct
{
	uint8_t burstMessage;
	uint16_t subDeviceIndex; //Index 0 indicates the I/O System itself
}C101_ReadSubDeviceToBurstMessageMap_Resp;
typedef enum
{
      C101_NOO = RCS_N00_Success,  
      C101_E02 = RCS_E02_InvalidSelection,
      C101_E05 = RCS_E05_TooFewDataBytesReceived,
      C101_E06 = RCS_E06_DeviceSpecificCommandError,
}C101_ReadSubDeviceToBurstMessageMap_RespCodes;


/******************** CMD 102 *************************/

typedef struct
{
	uint8_t burstMessage;
	uint16_t subDeviceIndex; //Index 0 indicates the I/O System itself
}C102_MapSubDeviceToBurstMessage_Req;
typedef C102_MapSubDeviceToBurstMessage_Req C102_MapSubDeviceToBurstMessage_Resp;
typedef enum
{
      C102_NOO = RCS_N00_Success,    
      C102_E02 = RCS_E02_InvalidSelection,
      C102_E05 = RCS_E05_TooFewDataBytesReceived,
      C102_E06 = RCS_E06_DeviceSpecificCommandError,
      C102_E07 = RCS_E07_InWriteProtectMode,
      C102_E09 = RCM_E09_InvalidSubDeviceIndex,
      C102_E16 = RCS_E16_AccessRestricted
}C102_MapSubDeviceToBurstMessage_RespCodes;


/******************** CMD 103 *************************/

typedef struct
{
	uint8_t burstMessage;
	_time_t updatePeriod; //Update Period must not exceed 3600 seconds.
	_time_t maxUpdatePeriod; //Update Period must not exceed 3600 seconds.
}C103_WriteBurstPeriod_Req;
typedef C103_WriteBurstPeriod_Req C103_WriteBurstPeriod_Resp;
typedef enum
{
      C103_NOO = RCS_N00_Success,    
      C103_E05 = RCS_E05_TooFewDataBytesReceived,
      C103_E06 = RCS_E06_DeviceSpecificCommandError,
      C103_E07 = RCS_E07_InWriteProtectMode,
      C103_W08 = RCM_W08_UpdateTimesAdjusted,
      C103_E09 = RCM_E09_InvalidBurstMessage,
      C103_E16 = RCS_E16_AccessRestricted,
      C103_E32 = RCS_E32_Busy,
      C103_E33 = RCS_E33_DelayedResponseInitiated,
      C103_E34 = RCS_E34_DelayedResponseRunning,
      C103_E35 = RCS_E35_DelayedResponseDead,
      C103_E36 = RCS_E36_DelayedResponseConflict
}C103_WriteBurstPeriod_RespCodes;


/******************** CMD 104 *************************/

typedef struct
{
	uint8_t burstMessage;
	BurstMessageTriggerMode selectionCode;
	DeviceVariableClassificationCodes classificationCode;
	UnitsCodes unitsCode;
	float triggerLevel;
}C104_WriteBurstTrigger_Req;
typedef C104_WriteBurstTrigger_Req C104_WriteBurstTrigger_Resp;
typedef enum
{
      C104_NOO = RCS_N00_Success, 
      C104_E02 = RCS_E02_InvalidSelection,
      C104_E03 = RCS_E03_PassedParameterTooLarge,
      C104_E04 = RCS_E04_PassedParameterTooSmall,
      C104_E05 = RCS_E05_TooFewDataBytesReceived,
      C104_E06 = RCS_E06_DeviceSpecificCommandError,
      C104_E07 = RCS_E07_InWriteProtectMode,
      C104_E09 = RCM_E09_InvalidBurstMessage,
      C104_E11 = RCM_E11_InvalidDeviceVariableClassification,
      C104_E12 = RCM_E12_InvalidUnitsCode,
      C104_E13 = RCM_E13_InvalidBurstTriggerModeSelectionCode,
      C104_E16 = RCS_E16_AccessRestricted,
      C104_E32 = RCS_E32_Busy,
      C104_E33 = RCS_E33_DelayedResponseInitiated,
      C104_E34 = RCS_E34_DelayedResponseRunning,
      C104_E35 = RCS_E35_DelayedResponseDead,
      C104_E36 = RCS_E36_DelayedResponseConflict      
}C104_WriteBurstTrigger_RespCodes;


/******************** CMD 105 *************************/

enum
{
	MaxNoOfDeviceVariables = 8
};
typedef struct
{
	uint8_t burstMessage;
	uint8_t requestIsEmpty; //0 - if is request is empty (for backward compability); >0 - otherwise
}C105_ReadBurstModeConfiguration_Req;

typedef struct
{
	BurstModeControlCodes burstMode;
	uint8_t commandNo;
	DeviceVariableCodes deviceVariableCode[8];
	uint8_t burstMessage;
	uint8_t maxBurstMessagesSupported;
        uint16_t extendedCommandNumber;
	_time_t updateTime;
	_time_t maxUpdateTime;
	BurstMessageTriggerMode burstTriggerMode;
	DeviceVariableClassificationCodes classificationCode;
	UnitsCodes unitsCode;
	float triggerValue;
}C105_ReadBurstModeConfiguration_Resp;
typedef enum
{
      C105_NOO = RCS_N00_Success,   	
      C105_E06 = RCS_E06_DeviceSpecificCommandError,
      C105_E09 = RCM_E09_InvalidBurstMessage,
      C105_E32 = RCS_E32_Busy
}C105_ReadBurstModeConfiguration_RespCodes;


/******************** CMD 106 *************************/

typedef EmptyCommand_Req C106_FlushDelayedResponses_Req;
typedef EmptyCommand_Resp C106_FlushDelayedResponses_Resp;
typedef enum
{
      C106_NOO = RCS_N00_Success,   	  
      C106_E06 = RCS_E06_DeviceSpecificCommandError,
      C106_W08 = RCM_W08_AllButRunningDelayedResponsesFlushed,
      C106_E16 = RCS_E16_AccessRestricted,
      C106_E32 = RCS_E32_Busy
}C106_FlushDelayedResponses_RespCodes;


/******************** CMD 107 *************************/

typedef struct
{
	uint8_t noOfDeviceVariables;
	DeviceVariableCodes deviceVariableCode[8]; 
	uint8_t burstMessage;
	uint8_t backComp; // 1 - if is request contains only 1, 2, 3 or 4 bytes (for backward compability); 0 - otherwise
}C107_WriteBurstDeviceVariables_Req;
typedef struct
{
	DeviceVariableCodes deviceVariableCode[8];
	uint8_t burstMessage;
}C107_WriteBurstDeviceVariables_Resp;
typedef enum
{
      C107_NOO = RCS_N00_Success,   	  
      C107_E02 = RCS_E02_InvalidSelection,
      C107_E05 = RCS_E05_TooFewDataBytesReceived,
      C107_E06 = RCS_E06_DeviceSpecificCommandError,
      C107_E07 = RCS_E07_InWriteProtectMode,
      C107_W08 = RCM_W08_BurstConditionConflict,
      C107_E09 = RCM_E09_InvalidBurstMessage
}C107_WriteBurstDeviceVariables_RespCodes;


/******************** CMD 108 *************************/

typedef struct
{
	uint16_t commandNo;
	uint8_t backComp; // 1 - if is request is empty (for backward compability); 0 - otherwise
	uint8_t burstMessage;
}C108_WriteBurstModeCommandNumber_Req;
typedef C108_WriteBurstModeCommandNumber_Req C108_WriteBurstModeCommandNumber_Resp;
typedef enum
{
      C108_NOO = RCS_N00_Success,   	    
      C108_E02 = RCS_E02_InvalidSelection,
      C108_E05 = RCS_E05_TooFewDataBytesReceived,
      C108_E06 = RCS_E06_DeviceSpecificCommandError,
      C108_E07 = RCS_E07_InWriteProtectMode,
      C108_W08 = RCM_W08_BurstConditionConflict,
      C108_E09 = RCM_E09_InvalidBurstMessage
}C108_WriteBurstModeCommandNumber_RespCodes;


/******************** CMD 109 *************************/

typedef struct
{
	uint8_t backComp; //1 - if is request is empty (for backward compability); 0 - otherwise
	BurstModeControlCodes burstModeCode;
	uint8_t burstMessage;
}C109_BurstModeControl_Req;
typedef C109_BurstModeControl_Req C109_BurstModeControl_Resp;
typedef enum
{
      C109_NOO = RCS_N00_Success,   	    
      C109_E02 = RCS_E02_InvalidSelection,
      C109_E05 = RCS_E05_TooFewDataBytesReceived,
      C109_E06 = RCS_E06_DeviceSpecificCommandError,
      C109_E07 = RCS_E07_InWriteProtectMode,  
      C109_W08 = RCM_W08_UpdatePeriodIncreased,
      C109_E09 = RCM_E09_InsufficientBandwidth,
      C109_E16 = RCS_E16_AccessRestricted,
      C109_E32 = RCS_E32_Busy,
      C109_E33 = RCS_E33_DelayedResponseInitiated,
      C109_E34 = RCS_E34_DelayedResponseRunning,
      C109_E35 = RCS_E35_DelayedResponseDead,
      C109_E36 = RCS_E36_DelayedResponseConflict         
}C109_BurstModeControl_RespCodes;


/******************** CMD 111 *************************/

typedef EmptyCommand_Req C111_TransferServiceControl_Req;
//TODO:[Ovidiu] - See Block Transfer Specification and implement this!!!


typedef EmptyCommand_Resp C111_TransferServiceControl_Resp;
//TODO:[Ovidiu] - See Block Transfer Specification and implement this!!!


/******************** CMD 112 *************************/

typedef EmptyCommand_Req C112_TransferService_Req;
//TODO:[Ovidiu] - See Block Transfer Specification and implement this!!!

typedef EmptyCommand_Resp C112_TransferService_Resp;
//TODO:[Ovidiu] - See Block Transfer Specification and implement this!!!


/******************** CMD 113 *************************/

typedef struct
{
	DeviceVariableCodes destinationDeviceVariableCode;
	CaptureModeCodes captureModeCode;
	ExpandedDeviceTypeCodes sourceSlaveExpandedDeviceTypeCode; // (Slave Address Byte 0-1)
	uint32_t sourceSlaveDeviceID; // _uint24_t Slave Address Byte 2-4
	uint8_t sourceCommandNo;
	uint8_t sourceSlotNo;
	float shedTime; //in Seconds
}C113_CatchDeviceVariable_Req;
typedef struct
{
	DeviceVariableCodes destinationDeviceVariableCode;
	CaptureModeCodes captureModeCode;
	_device_address_t sourceSlaveAddress;
	uint8_t sourceCommandNo;
	uint8_t sourceSlotNo;
	float shedTime; //in Seconds
}C113_CatchDeviceVariable_Resp;
typedef enum
{
      C113_NOO = RCS_N00_Success,   	    
      C113_E05 = RCS_E05_TooFewDataBytesReceived,
      C113_E06 = RCS_E06_DeviceSpecificCommandError,  
      C113_E10 = RCM_E10_InvalidCaptureModeCode,
      C113_E11 = RCM_E11_InvalidSlotNumber,
      C113_E12 = RCM_E12_CommandNumberNotSupported,
      C113_E16 = RCS_E16_AccessRestricted,
      C113_E17 = RCS_E17_InvalidDeviceVariableIndex,
      C113_E19 = RCS_E19_DeviceVariableIndexNotAllowed,
      C113_E32 = RCS_E32_Busy,
      C113_E33 = RCS_E33_DelayedResponseInitiated,
      C113_E34 = RCS_E34_DelayedResponseRunning,
      C113_E35 = RCS_E35_DelayedResponseDead,
      C113_E36 = RCS_E36_DelayedResponseConflict           
}C113_CatchDeviceVariable_RespCodes;


/******************** CMD 114 *************************/

typedef struct
{
	DeviceVariableCodes destinationDeviceVariableCode;
}C114_ReadCaughtDeviceVariable_Req;
typedef struct
{
	DeviceVariableCodes destinationDeviceVariableCode;
	CaptureModeCodes captureModeCode;
	_device_address_t sourceSlaveAddress;
	uint32_t sourceSlaveDeviceID; //_uint24_t Slave Address Byte 2-4
	uint8_t sourceCommandNo;
	uint8_t sourceSlotNo;
	float shedTime; //in Seconds
}C114_ReadCaughtDeviceVariable_Resp;
typedef enum
{
      C114_NOO = RCS_N00_Success,   	    
      C114_E05 = RCS_E05_TooFewDataBytesReceived,
      C114_E06 = RCS_E06_DeviceSpecificCommandError,  
      C114_E17 = RCS_E17_InvalidDeviceVariableIndex,
      C114_E19 = RCS_E19_DeviceVariableIndexNotAllowed,
      C114_E32 = RCS_E32_Busy,
      C114_E33 = RCS_E33_DelayedResponseInitiated,
      C114_E34 = RCS_E34_DelayedResponseRunning,
      C114_E35 = RCS_E35_DelayedResponseDead,
      C114_E36 = RCS_E36_DelayedResponseConflict           
}C114_ReadCaughtDeviceVariable_RespCodes;


/******************** CMD 115 *************************/

enum
{
	MaxNoOfReadEvents = 26
};
typedef struct
{
	uint8_t eventNo;
}C115_ReadEventNotificationSummary_Req;
typedef struct
{
	uint8_t eventNo;
	uint8_t noOfEventsSupported;
	//eventStatus and eventNotificationControlCode share same byte
	uint8_t eventStatus; //Most Significant 4 Bits
	EventNotificationControlCodes eventNotificationControlCode;// Least Significant 4 bits
	_time_t firstUnacknowledged; //(must be set to 0xFFFFFFFF when no events are pending)
	_time_t eventNotificationRetryTime;
	_time_t maximumUpdateTime;
	_time_t eventDeBounceInterval;
	uint8_t noOfReadEvents;
	uint8_t eventMask[MaxNoOfReadEvents];
}C115_ReadEventNotificationSummary_Resp;
typedef enum
{
      C115_NOO = RCS_N00_Success,   	    
      C115_E02 = RCS_E02_InvalidSelection,
      C115_E05 = RCS_E05_TooFewDataBytesReceived,
      C115_E06 = RCS_E06_DeviceSpecificCommandError   
}C115_ReadEventNotificationSummary_RespCodes;


/******************** CMD 116 *************************/

enum
{
	MaxNoOfWriteEvents = 26
};
typedef struct
{
	uint8_t eventNo;
	uint8_t noOfWriteEvents;
	uint8_t eventMask[MaxNoOfWriteEvents];
}C116_WriteEventNotificationBitMask_Req;
typedef C116_WriteEventNotificationBitMask_Req C116_WriteEventNotificationBitMask_Resp;
typedef enum
{
      C116_NOO = RCS_N00_Success,   	    
      C116_E02 = RCS_E02_InvalidSelection,
      C116_E05 = RCS_E05_TooFewDataBytesReceived,
      C116_E06 = RCS_E06_DeviceSpecificCommandError,
      C116_E07 = RCS_E07_InWriteProtectMode,
      C116_E16 = RCS_E16_AccessRestricted
}C116_WriteEventNotificationBitMask_RespCodes;


/******************** CMD 117 *************************/

typedef struct
{
	uint8_t eventNo;
	_time_t eventNotificationRetryTime;
	_time_t maximumUpdateTime;
	_time_t eventDeBounceInterval;
}C117_WriteEventNotificationTiming_Req;
typedef C117_WriteEventNotificationTiming_Req C117_WriteEventNotificationTiming_Resp;
typedef enum
{
      C117_NOO = RCS_N00_Success,   	    
      C117_E05 = RCS_E05_TooFewDataBytesReceived,
      C117_E06 = RCS_E06_DeviceSpecificCommandError,
      C117_E07 = RCS_E07_InWriteProtectMode,  
      C117_W08 = RCM_W08_UpdatePeriodOrDebounceIntervalAdjusted,
      C117_E09 = RCM_E09_InvalidBurstMessage,
      C117_E16 = RCS_E16_AccessRestricted,
      C117_E32 = RCS_E32_Busy,
      C117_E33 = RCS_E33_DelayedResponseInitiated,
      C117_E34 = RCS_E34_DelayedResponseRunning,
      C117_E35 = RCS_E35_DelayedResponseDead,
      C117_E36 = RCS_E36_DelayedResponseConflict        
}C117_WriteEventNotificationTiming_RespCodes;


/******************** CMD 118 *************************/

typedef struct
{
	uint8_t eventNo;
	EventNotificationControlCodes eventNotificationControlCode;
}C118_EventNotificationControl_Req;
typedef C118_EventNotificationControl_Req C118_EventNotificationControl_Resp;
typedef enum
{
      C118_NOO = RCS_N00_Success,   	    
      C118_E02 = RCS_E02_InvalidSelection,
      C118_E05 = RCS_E05_TooFewDataBytesReceived,
      C118_E06 = RCS_E06_DeviceSpecificCommandError,
      C118_E07 = RCS_E07_InWriteProtectMode,    
      C118_W08 = RCM_W08_UpdateTimesAdjusted,
      C118_W14 = RCM_W14_UpdateRateUncertain,
      C118_E16 = RCS_E16_AccessRestricted,
      C118_E32 = RCS_E32_Busy,
      C118_E33 = RCS_E33_DelayedResponseInitiated,
      C118_E34 = RCS_E34_DelayedResponseRunning,
      C118_E35 = RCS_E35_DelayedResponseDead,
      C118_E36 = RCS_E36_DelayedResponseConflict          
}C118_EventNotificationControl_RespCodes;


/******************** CMD 119 *************************/

enum
{
	MaxLengthOf48Command = 25
};
typedef struct
{
        _time_t  firstUnacknowledged;
        uint16_t configurationChangedCounter;
        uint8_t  eventNo;
        uint8_t  deviceStatus;
	uint8_t  command48Data[MaxLengthOf48Command];
}C119_AcknowledgeEventNotification_Req;
typedef C119_AcknowledgeEventNotification_Req C119_AcknowledgeEventNotification_Resp;
typedef enum
{
      C119_NOO = RCS_N00_Success,   	    
      C119_E02 = RCS_E02_InvalidSelection,
      C119_E05 = RCS_E05_TooFewDataBytesReceived,
      C119_E06 = RCS_E06_DeviceSpecificCommandError,
      C119_E07 = RCS_E07_InWriteProtectMode,    
      C119_W08 = RCM_W08_NotAllEventsCleared,
}C119_AcknowledgeEventNotification_RespCodes;


/******************** CMD 512 *************************/

typedef EmptyCommand_Req C512_ReadCountryCode_Req;
typedef struct
{
	char countryCode[2];
	SIUnitsControlCodes siUnitsOnly;
}C512_ReadCountryCode_Resp;
typedef enum
{
      C512_NOO = RCS_N00_Success,   	    
      C512_E06 = RCS_E06_DeviceSpecificCommandError
}C512_ReadCountryCode_RespCodes;


/******************** CMD 513 *************************/

typedef struct
{
	char countryCode[2];
	SIUnitsControlCodes siUnitsOnly;
}C513_WriteCountryCode_Req;
typedef C513_WriteCountryCode_Req C513_WriteCountryCode_Resp;
typedef enum
{
      C513_NOO = RCS_N00_Success,   	    
      C513_E02 = RCS_E02_InvalidSelection,
      C513_E05 = RCS_E05_TooFewDataBytesReceived,
      C513_E06 = RCS_E06_DeviceSpecificCommandError,
      C513_E07 = RCS_E07_InWriteProtectMode,    
      C513_E16 = RCS_E16_AccessRestricted,
      C513_E32 = RCS_E32_Busy,
      C513_E33 = RCS_E33_DelayedResponseInitiated,
      C513_E34 = RCS_E34_DelayedResponseRunning,
      C513_E35 = RCS_E35_DelayedResponseDead,
      C513_E36 = RCS_E36_DelayedResponseConflict          
}C513_WriteCountryCode_RespCodes;



#endif /*COMMONPRACTICECOMMANDS_H_*/
