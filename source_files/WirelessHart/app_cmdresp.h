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
* Name:         app_cmdresp.h
* Author:       Nivis LLC
* Date:         August 2008
* Description:  Application Layer, HART commands: parse the request, compose the response
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_APP_CMDRESP_H_
#define _NIVIS_APP_CMDRESP_H_

#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/PhysicalLayerCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/NetworkLayerCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/DataLinkLayerCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/WirelessApplicationCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/DeviceSpecificWirelessOnlyCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/DeviceSpecificCommands.h"
#else
  #include "ApplicationLayer/Model/PhysicalLayerCommands.h"
  #include "ApplicationLayer/Model/NetworkLayerCommands.h"
  #include "ApplicationLayer/Model/DataLinkLayerCommands.h"
  #include "ApplicationLayer/Model/WirelessApplicationCommands.h"
  #include "ApplicationLayer/Model/DeviceSpecificWirelessOnlyCommands.h"
  #include "ApplicationLayer/Model/DeviceSpecificCommands.h"
#endif


#include "../typedef.h"       // for SHORT_ADDR definition

#define MAINTENANCE_PORT_MASK     0x01  // with "control" parameter
#define PUBLISHED_RESP_MASK       0x80

#define APP_DEVRESET_INACTIVE         0x00
#define APP_DEVRESET_C42_WIRELESS     0x02
#define APP_DEVRESET_C42_WIRED        0x03
#define APP_DEVRESET_C771_WIRELESS    0x04
#define APP_DEVRESET_C771_WIRED       0x05

#define APP_DEVRESET_MASK_WIRED       0x01
#define APP_DEVRESET_MASK_C771        0x04

// when set => perform a reset
extern uint8 g_ucDoDeviceInitFromCmd;  
extern uint32 g_ulDelayInitFromCmd;

/*
  SET_CONFIG_CHANGED sets the Configuration Changed bit and increments the Configuration Change Counter.
  SET_CONFIG_CHANGED should be used in the commands that alter a device's configuration
  (see WirelessHART command summary table in the addendum to Wireless Command Specification)
*/
void APP_SetDeviceStatusFlag(uint8_t ucBitMask);

void APP_ResetDeviceStatusFlag(SHORT_ADDR unSrcAddr, uint8_t ucControl, uint8_t ucBitMask);

uint8_t APP_GetDeviceStatus(uint8_t ucSrvIdx);

#define SET_CONFIG_CHANGED {APP_SetDeviceStatusFlag(CONFIG_CHANGED);}

#ifdef USE_WRITE_PROTECT_MODE   // check write protect mode
  #define CK_WP_MODE {if(g_stProv.ucWriteProtectCode == WriteProtectCodes_Yes) return RCS_E07_InWriteProtectMode;} 
#else
  #define CK_WP_MODE 
#endif

#ifdef USE_LOCK_DEVICE_MODE   // check lock device status
  #define CK_LOCK_STATUS {if(IS_LOCKED_FOR_THIS_MASTER(control, srcAddress)) return RCS_E16_AccessRestricted;} 
#else
  #define CK_LOCK_STATUS 
#endif


/*
* Execute functions prototype:
* 
* typedef uint16_t (*ExecuteFunction)(void* request, void* response, 
*                                     SHORT_ADDR srcAddress,
*                                     uint8_t control, uint8_t remainingBytes);
* 
* Parameters:
* @request: pointer to struct of command's specific request params (input data)
* @response: pointer to struct of command's specific response params (output data)
* @srcAddress: requesting device
* @control: bit0 = 1 -request issued via the maintenance port (wired network) , 0 -via the wireless network (use MAINTENANCE_PORT_MASK)
*           bit7 = 1 -published response, 0 -requested command (use PUBLISHED_RESP_MASK)
* @remainingBytes: number of free bytes left in response buffer
* 
* Returned value: Command-specific response code
*/


/*************************************************************************/
/*********************** Universal Commands ******************************/
/*************************************************************************/
uint16_t Execute_C000_ReadUniqueIdentifier(C000_ReadUniqueIdentifier_Req* request, 
                                     C000_ReadUniqueIdentifier_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C001_ReadPrimaryVariable(C001_ReadPrimaryVariable_Req* request, 
                                     C001_ReadPrimaryVariable_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C002_ReadLoopCurrentAndPercentOfRange(C002_ReadLoopCurrentAndPercentOfRange_Req* request, 
                                     C002_ReadLoopCurrentAndPercentOfRange_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C003_ReadDynamicVariablesAndLoopCurrent(C003_ReadDynamicVariablesAndLoopCurrent_Req* request, 
                                     C003_ReadDynamicVariablesAndLoopCurrent_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C006_WritePollingAddress(C006_WritePollingAddress_Req* request, 
                                     C006_WritePollingAddress_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C007_ReadLoopConfiguration(C007_ReadLoopConfiguration_Req* request, 
                                     C007_ReadLoopConfiguration_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C008_ReadDynamicVariableClassifications(C008_ReadDynamicVariableClassifications_Req* request, 
                                     C008_ReadDynamicVariableClassifications_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C009_ReadDeviceVariablesWithStatus(C009_ReadDeviceVariablesWithStatus_Req* request, 
                                     C009_ReadDeviceVariablesWithStatus_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C011_ReadUniqueIdentifierAssociatedWithTag(C011_ReadUniqueIdentifierAssociatedWithTag_Req* request, 
                                     C011_ReadUniqueIdentifierAssociatedWithTag_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C012_ReadMessage(C012_ReadMessage_Req* request, 
                                     C012_ReadMessage_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C013_ReadTagDescriptorDate(C013_ReadTagDescriptorDate_Req* request, 
                                     C013_ReadTagDescriptorDate_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C014_ReadPrimaryVariableTransducerInformation(C014_ReadPrimaryVariableTransducerInformation_Req* request, 
                                     C014_ReadPrimaryVariableTransducerInformation_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C015_ReadDeviceInformation(C015_ReadDeviceInformation_Req* request, 
                                     C015_ReadDeviceInformation_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C016_ReadFinalAssemblyNumber(C016_ReadFinalAssemblyNumber_Req* request, 
                                     C016_ReadFinalAssemblyNumber_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C017_WriteMessage(C017_WriteMessage_Req* request, 
                                     C017_WriteMessage_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C018_WriteTagDescriptorDate(C018_WriteTagDescriptorDate_Req* request, 
                                     C018_WriteTagDescriptorDate_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C019_WriteFinalAssemblyNumber(C019_WriteFinalAssemblyNumber_Req* request, 
                                     C019_WriteFinalAssemblyNumber_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C020_ReadLongTag(C020_ReadLongTag_Req* request, 
                                     C020_ReadLongTag_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C021_ReadUniqueIdentifierAssociatedWithLongTag(C021_ReadUniqueIdentifierAssociatedWithLongTag_Req* request, 
                                     C021_ReadUniqueIdentifierAssociatedWithLongTag_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C022_WriteLongTag(C022_WriteLongTag_Req* request, 
                                     C022_WriteLongTag_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C033_ReadDeviceVariables(C033_ReadDeviceVariables_Req* request, 
                                     C033_ReadDeviceVariables_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C034_WritePrimaryVariableDampingValue(C034_WritePrimaryVariableDampingValue_Req* request, 
                                     C034_WritePrimaryVariableDampingValue_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C035_WritePrimaryVariableRangeValues(C035_WritePrimaryVariableRangeValues_Req* request, 
                                     C035_WritePrimaryVariableRangeValues_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C036_SetPrimaryVariableUpperRangeValue(C036_SetPrimaryVariableUpperRangeValue_Req* request, 
                                     C036_SetPrimaryVariableUpperRangeValue_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C037_SetPrimaryVariableLowerRangeValue(C037_SetPrimaryVariableLowerRangeValue_Req* request, 
                                     C037_SetPrimaryVariableLowerRangeValue_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

//Exception: this is a universal command
uint16_t Execute_C038_ResetConfigurationChangedFlag(C038_ResetConfigurationChangedFlag_Req* request, 
                                     C038_ResetConfigurationChangedFlag_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C040_EnterExitFixedCurrentMode(C040_EnterExitFixedCurrentMode_Req* request, 
                                     C040_EnterExitFixedCurrentMode_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C041_PerformSelfTest(C041_PerformSelfTest_Req* request, 
                                     C041_PerformSelfTest_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C042_PerformDeviceReset(C042_PerformDeviceReset_Req* request, 
                                     C042_PerformDeviceReset_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C043_SetPrimaryVariableZero(C043_SetPrimaryVariableZero_Req* request, 
                                     C043_SetPrimaryVariableZero_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C044_WritePrimaryVariableUnits(C044_WritePrimaryVariableUnits_Req* request, 
                                     C044_WritePrimaryVariableUnits_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C045_TrimLoopCurrentZero(C045_TrimLoopCurrentZero_Req* request, 
                                     C045_TrimLoopCurrentZero_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C046_TrimLoopCurrentGain(C046_TrimLoopCurrentGain_Req* request, 
                                     C046_TrimLoopCurrentGain_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C047_WritePrimaryVariableTransferFunction(C047_WritePrimaryVariableTransferFunction_Req* request, 
                                     C047_WritePrimaryVariableTransferFunction_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

//Exception: this is a universal command
uint16_t Execute_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Req* request, 
                                     C048_ReadAdditionalDeviceStatus_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C049_WritePrimaryVariableTransducerSerialNumber(C049_WritePrimaryVariableTransducerSerialNumber_Req* request, 
                                     C049_WritePrimaryVariableTransducerSerialNumber_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Req* request, 
                                     C050_ReadDynamicVariableAssignments_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Req* request, 
                                     C051_WriteDynamicVariableAssignments_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C052_SetDeviceVariableZero(C052_SetDeviceVariableZero_Req* request, 
                                     C052_SetDeviceVariableZero_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C053_WriteDeviceVariableUnits(C053_WriteDeviceVariableUnits_Req* request, 
                                     C053_WriteDeviceVariableUnits_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C054_ReadDeviceVariableInformation(C054_ReadDeviceVariableInformation_Req* request, 
                                     C054_ReadDeviceVariableInformation_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C055_WriteDeviceVariableDampingValue(C055_WriteDeviceVariableDampingValue_Req* request, 
                                     C055_WriteDeviceVariableDampingValue_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C056_WriteDeviceVariableTransducerSerialNo(C056_WriteDeviceVariableTransducerSerialNo_Req* request, 
                                     C056_WriteDeviceVariableTransducerSerialNo_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Req* request, 
                                     C059_WriteNumberOfResponsePreambles_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C060_ReadAnalogChannelAndPercentOfRange(C060_ReadAnalogChannelAndPercentOfRange_Req* request, 
                                     C060_ReadAnalogChannelAndPercentOfRange_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C062_ReadAnalogChannels(C062_ReadAnalogChannels_Req* request, 
                                     C062_ReadAnalogChannels_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C063_ReadAnalogChannelInformation(C063_ReadAnalogChannelInformation_Req* request, 
                                     C063_ReadAnalogChannelInformation_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C064_WriteAnalogChannelAdditionalDampingValue(C064_WriteAnalogChannelAdditionalDampingValue_Req* request, 
                                     C064_WriteAnalogChannelAdditionalDampingValue_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C065_WriteAnalogChannelRangeValues(C065_WriteAnalogChannelRangeValues_Req* request, 
                                     C065_WriteAnalogChannelRangeValues_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C066_EnterExitFixedAnalogChannelMode(C066_EnterExitFixedAnalogChannelMode_Req* request, 
                                     C066_EnterExitFixedAnalogChannelMode_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C067_TrimAnalogChannelZero(C067_TrimAnalogChannelZero_Req* request, 
                                     C067_TrimAnalogChannelZero_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C068_TrimAnalogChannelGain(C068_TrimAnalogChannelGain_Req* request, 
                                     C068_TrimAnalogChannelGain_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C069_WriteAnalogChannelTransferFunction(C069_WriteAnalogChannelTransferFunction_Req* request, 
                                     C069_WriteAnalogChannelTransferFunction_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C070_ReadAnalogChannelEndpointValues(C070_ReadAnalogChannelEndpointValues_Req* request, 
                                     C070_ReadAnalogChannelEndpointValues_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C071_LockDevice(C071_LockDevice_Req* request, 
                                     C071_LockDevice_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C072_Squawk(C072_Squawk_Req* request, 
                                     C072_Squawk_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C073_FindDevice(C073_FindDevice_Req* request, 
                                     C073_FindDevice_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C074_ReadIOSystemCapabilities(C074_ReadIOSystemCapabilities_Req* request, 
                                     C074_ReadIOSystemCapabilities_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C075_PollSubDevice(C075_PollSubDevice_Req* request, 
                                     C075_PollSubDevice_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C076_ReadLockDeviceState(C076_ReadLockDeviceState_Req* request, 
                                     C076_ReadLockDeviceState_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C077_SendCommandToSubDevice(C077_SendCommandToSubDevice_Req* request, 
                                     C077_SendCommandToSubDevice_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C078_ReadAggregatedCommands(C078_ReadAggregatedCommands_Req* request, 
                                     C078_ReadAggregatedCommands_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Req* request, 
                                     C079_WriteDeviceVariable_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C080_ReadDeviceVariableTrimPoints(C080_ReadDeviceVariableTrimPoints_Req* request, 
                                     C080_ReadDeviceVariableTrimPoints_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C081_ReadDeviceVariableTrimGuidelines(C081_ReadDeviceVariableTrimGuidelines_Req* request, 
                                     C081_ReadDeviceVariableTrimGuidelines_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C082_WriteDeviceVariableTrimPoint(C082_WriteDeviceVariableTrimPoint_Req* request, 
                                     C082_WriteDeviceVariableTrimPoint_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C083_ResetDeviceVariableTrim(C083_ResetDeviceVariableTrim_Req* request, 
                                     C083_ResetDeviceVariableTrim_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C084_ReadSubDeviceIdentitySummary(C084_ReadSubDeviceIdentitySummary_Req* request, 
                                     C084_ReadSubDeviceIdentitySummary_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C085_ReadIOChannelStatistics(C085_ReadIOChannelStatistics_Req* request, 
                                     C085_ReadIOChannelStatistics_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C086_ReadSubDeviceStatistics(C086_ReadSubDeviceStatistics_Req* request, 
                                     C086_ReadSubDeviceStatistics_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C087_WriteIOSystemMasterMode(C087_WriteIOSystemMasterMode_Req* request, 
                                     C087_WriteIOSystemMasterMode_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C088_WriteIOSystemRetryCount(C088_WriteIOSystemRetryCount_Req* request, 
                                     C088_WriteIOSystemRetryCount_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C089_SetRealTimeClock(C089_SetRealTimeClock_Req* request, 
                                     C089_SetRealTimeClock_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C090_ReadRealTimeClock(C090_ReadRealTimeClock_Req* request, 
                                     C090_ReadRealTimeClock_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C091_ReadTrendConfiguration(C091_ReadTrendConfiguration_Req* request, 
                                     C091_ReadTrendConfiguration_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C092_WriteTrendConfiguration(C092_WriteTrendConfiguration_Req* request, 
                                     C092_WriteTrendConfiguration_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C093_ReadTrend(C093_ReadTrend_Req* request, 
                                     C093_ReadTrend_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C094_ReadIOSystemClientSideCommunicationStatistics(C094_ReadIOSystemClientSideCommunicationStatistics_Req* request, 
                                     C094_ReadIOSystemClientSideCommunicationStatistics_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C095_ReadDeviceCommunicationsStatistics(C095_ReadDeviceCommunicationsStatistics_Req* request, 
                                     C095_ReadDeviceCommunicationsStatistics_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C096_ReadSynchronousAction(C096_ReadSynchronousAction_Req* request, 
                                     C096_ReadSynchronousAction_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C097_ConfigureSynchronousAction(C097_ConfigureSynchronousAction_Req* request, 
                                     C097_ConfigureSynchronousAction_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C098_ReadCommandAction(C098_ReadCommandAction_Req* request, 
                                     C098_ReadCommandAction_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C099_ConfigureCommandAction(C099_ConfigureCommandAction_Req* request, 
                                     C099_ConfigureCommandAction_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C101_ReadSubDeviceToBurstMessageMap(C101_ReadSubDeviceToBurstMessageMap_Req* request, 
                                     C101_ReadSubDeviceToBurstMessageMap_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C102_MapSubDeviceToBurstMessage(C102_MapSubDeviceToBurstMessage_Req* request, 
                                     C102_MapSubDeviceToBurstMessage_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Req* request, 
                                     C103_WriteBurstPeriod_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Req* request, 
                                     C104_WriteBurstTrigger_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C105_ReadBurstModeConfiguration(C105_ReadBurstModeConfiguration_Req* request, 
                                     C105_ReadBurstModeConfiguration_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C106_FlushDelayedResponses(C106_FlushDelayedResponses_Req* request, 
                                     C106_FlushDelayedResponses_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C107_WriteBurstDeviceVariables(C107_WriteBurstDeviceVariables_Req* request, 
                                     C107_WriteBurstDeviceVariables_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C108_WriteBurstModeCommandNumber(C108_WriteBurstModeCommandNumber_Req* request, 
                                     C108_WriteBurstModeCommandNumber_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C109_BurstModeControl(C109_BurstModeControl_Req* request, 
                                     C109_BurstModeControl_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C113_CatchDeviceVariable(C113_CatchDeviceVariable_Req* request, 
                                     C113_CatchDeviceVariable_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C114_ReadCaughtDeviceVariable(C114_ReadCaughtDeviceVariable_Req* request, 
                                     C114_ReadCaughtDeviceVariable_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Req* request, 
                                     C115_ReadEventNotificationSummary_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Req* request, 
                                     C116_WriteEventNotificationBitMask_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Req* request, 
                                     C117_WriteEventNotificationTiming_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C118_EventNotificationControl(C118_EventNotificationControl_Req* request, 
                                     C118_EventNotificationControl_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Req* request, 
                                     C119_AcknowledgeEventNotification_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

//--------------- device specific commands, begin -----------------//

uint16_t Execute_C133_ReadFirmwareVersion( C133_ReadFirmwareVersion_Req* request,
                                           C133_ReadFirmwareVersion_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control,
                                           uint8_t remainingBytes );

//--------------- device specific commands, end -------------------//

uint16_t Execute_C512_ReadCountryCode(C512_ReadCountryCode_Req* request, 
                                     C512_ReadCountryCode_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C513_WriteCountryCode(C513_WriteCountryCode_Req* request, 
                                     C513_WriteCountryCode_Resp* response,
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

/*************************************************************************/
/************************ Wireless Commands ******************************/
/*************************************************************************/

uint16_t Execute_C768_WriteJoinKey(C768_WriteJoinKey_Req* request,
                                  C768_WriteJoinKey_Resp* response,
                                  SHORT_ADDR srcAddr,
                                  uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C769_ReadJoinStatus(C769_ReadJoinStatus_Req* request,
                                    C769_ReadJoinStatus_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C770_RequestActiveAdvertise(C770_RequestActiveAdvertise_Req* request,
                                            C770_RequestActiveAdvertise_Resp* response,
                                            SHORT_ADDR srcAddr,
                                            uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C771_ForceJoin(C771_ForceJoin_Req* request,
                               C771_ForceJoin_Resp* response,
                               SHORT_ADDR srcAddr,
                               uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C772_ReadJoinModeConfiguration(C772_ReadJoinModeConfiguration_Req* request,
                                               C772_ReadJoinModeConfiguration_Resp* response,
                                               SHORT_ADDR srcAddr,
                                               uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C773_WriteNetworkId( C773_WriteNetworkId_Req* request, 
                                     C773_WriteNetworkId_Resp* response,                                       
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C774_ReadNetworkId( C774_ReadNetworkId_Req* request, 
                                    C774_ReadNetworkId_Resp* response,                                       
                                    SHORT_ADDR srcAddr, 
                                    uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C775_WriteNetworkTag( C775_WriteNetworkTag_Req* request, 
                                      C775_WriteNetworkTag_Resp* response,                                       
                                      SHORT_ADDR srcAddr, 
                                      uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C776_ReadNetworkTag( C776_ReadNetworkTag_Req* request, 
                                     C776_ReadNetworkTag_Resp* response,                                       
                                     SHORT_ADDR srcAddr, 
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C777_ReadWirelessDeviceInformation( C777_ReadWirelessDeviceInformation_Req* request, 
                                         C777_ReadWirelessDeviceInformation_Resp* response,                                       
                                         SHORT_ADDR srcAddr, 
                                         uint8_t control, 
                                         uint8_t remainingBytes );

uint16_t Execute_C778_ReadBatteryLife(C778_ReadBatteryLife_Req* request,
                                               C778_ReadBatteryLife_Resp* response,
                                               SHORT_ADDR srcAddr,
                                               uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C779_ReportDeviceHealth( C779_ReportDeviceHealth_Req* request, 
                                         C779_ReportDeviceHealth_Resp* response,                                       
                                         SHORT_ADDR srcAddr, 
                                         uint8_t control, 
                                         uint8_t remainingBytes );

uint16_t Execute_C780_ReportNeighborHealthList( C780_ReportNeighborHealthList_Req* request, 
                                               C780_ReportNeighborHealthList_Resp* response,                                       
                                               SHORT_ADDR srcAddr, 
                                               uint8_t control, 
                                               uint8_t remainingBytes );

uint16_t Execute_C781_ReadDeviceNicknameAddress( C781_ReadDeviceNicknameAddress_Req* request, 
                                                C781_ReadDeviceNicknameAddress_Resp* response,                                       
                                                SHORT_ADDR srcAddr, 
                                                uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C782_ReadSessionEntries(C782_ReadSessionEntries_Req* request,
                                        C782_ReadSessionEntries_Resp* response,
                                        SHORT_ADDR srcAddr,
                                        uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C783_ReadSuperframeList( C783_ReadSuperframeList_Req* request, 
                                         C783_ReadSuperframeList_Resp* response,                                       
                                         SHORT_ADDR srcAddr, 
                                         uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C784_ReadLinkList( C784_ReadLinkList_Req* request, 
                                   C784_ReadLinkList_Resp* response,                                       
                                   SHORT_ADDR srcAddr, 
                                   uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C785_ReadGraphList( C785_ReadGraphList_Req* request, 
                                    C785_ReadGraphList_Resp* response,                                       
                                    SHORT_ADDR srcAddr, 
                                    uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C786_ReadNeighborPropertyFlag( C786_ReadNeighborPropertyFlag_Req* request, 
                                               C786_ReadNeighborPropertyFlag_Resp* response,                                       
                                               SHORT_ADDR srcAddr, 
                                               uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C787_ReportNeighborSignalLevels( C787_ReportNeighborSignalLevels_Req* request, 
                                        C787_ReportNeighborSignalLevels_Resp* response,                                       
                                        SHORT_ADDR srcAddr, 
                                        uint8_t control, uint8_t remainingBytes);

//uint16_t Execute_C788_AlarmPathDown( C788_AlarmPathDown_Req* request, 
//                                    C788_AlarmPathDown_Resp* response,                                       
//                                    SHORT_ADDR srcAddr, 
//                                    uint8_t control, uint8_t remainingBytes);
//
//uint16_t Execute_C789_AlarmSourceRouteFailed(C789_AlarmSourceRouteFailed_Req* request,
//                                            C789_AlarmSourceRouteFailed_Resp* response,
//                                            SHORT_ADDR srcAddr,
//                                            uint8_t control, uint8_t remainingBytes);
//
//uint16_t Execute_C790_AlarmGraphRouteFailed(C790_AlarmGraphRouteFailed_Req* request,
//                                           C790_AlarmGraphRouteFailed_Resp* response,
//                                           SHORT_ADDR srcAddr,
//                                           uint8_t control, uint8_t remainingBytes);
//
//uint16_t Execute_C791_AlarmTransportLayerFailed(C791_AlarmTransportLayerFailed_Req* request,
//                                               C791_AlarmTransportLayerFailed_Resp* response,
//                                               SHORT_ADDR srcAddr,
//                                               uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C793_WriteUTCTime( C793_WriteUTCTime_Req* request, 
                                         C793_WriteUTCTime_Resp* response,                                       
                                         SHORT_ADDR srcAddr, 
                                         uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C794_ReadUTCTime( C794_ReadUTCTime_Req* request, 
                                         C794_ReadUTCTime_Resp* response,                                       
                                         SHORT_ADDR srcAddr, 
                                         uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C795_WriteTimerInterval( C795_WriteTimerInterval_Req* request, 
                                         C795_WriteTimerInterval_Resp* response,                                       
                                         SHORT_ADDR srcAddr, 
                                         uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C796_ReadTimerInterval( C796_ReadTimerInterval_Req* request, 
                                        C796_ReadTimerInterval_Resp* response,                                       
                                        SHORT_ADDR srcAddr, 
                                        uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C797_WriteRadioPower(C797_WriteRadioPower_Req* request,
                                     C797_WriteRadioPower_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C798_ReadRadioPower(C798_ReadRadioPower_Req* request,
                                    C798_ReadRadioPower_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes);

/*
uint16_t Execute_C799_RequestService(C799_RequestService_Req* request,
                                    C799_RequestService_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes);
*/

uint16_t Execute_C800_ReadServiceList(C800_ReadServiceList_Req* request,
                                     C800_ReadServiceList_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C801_DeleteService(C801_DeleteService_Req* request,
                                   C801_DeleteService_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C802_ReadRouteList(C802_ReadRouteList_Req* request,
                                   C802_ReadRouteList_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C803_ReadSourceRoute(C803_ReadSourceRoute_Req* request,
                                     C803_ReadSourceRoute_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C804_ReadRadioCCAMode(C804_ReadRadioCCAMode_Req* request,
                                      C804_ReadRadioCCAMode_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C805_WriteRadioCCAMode(C805_WriteRadioCCAMode_Req* request,
                                       C805_WriteRadioCCAMode_Resp* response,
                                       SHORT_ADDR srcAddr,
                                       uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C806_ReadHandheldSuperframe( C806_ReadHandheldSuperframe_Req* request, 
                                                   C806_ReadHandheldSuperframe_Resp* response,                                       
                                                   SHORT_ADDR srcAddr, 
                                                   uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C807_RequestHandheldSuperframeMode( C807_RequestHandheldSuperframeMode_Req* request, 
                                               C807_RequestHandheldSuperframeMode_Resp* response,                                       
                                               SHORT_ADDR srcAddr, 
                                               uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C808_ReadTimeToLive(C808_ReadTimeToLive_Req* request,
                                    C808_ReadTimeToLive_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C809_WriteTimeToLive(C809_WriteTimeToLive_Req* request,
                                     C809_WriteTimeToLive_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C810_ReadJoinPriority( C810_ReadJoinPriority_Req* request, 
                                       C810_ReadJoinPriority_Resp* response,                                       
                                       SHORT_ADDR srcAddr, 
                                       uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C811_WriteJoinPriority( C811_WriteJoinPriority_Req* request, 
                                        C811_WriteJoinPriority_Resp* response,                                       
                                        SHORT_ADDR srcAddr, 
                                        uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C812_ReadPacketReceivePriority( C812_ReadPacketReceivePriority_Req* request, 
                                         C812_ReadPacketReceivePriority_Resp* response,                                       
                                         SHORT_ADDR srcAddr, 
                                         uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C813_WritePacketReceivePriority( C813_WritePacketReceivePriority_Req* request, 
                                          C813_WritePacketReceivePriority_Resp* response,                                       
                                          SHORT_ADDR srcAddr, 
                                          uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C814_ReadDeviceListEntries(C814_ReadDeviceListEntries_Req* request,
                                           C814_ReadDeviceListEntries_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C815_AddDeviceListTableEntry(C815_AddDeviceListTableEntry_Req* request,
                                             C815_AddDeviceListTableEntry_Resp* response,
                                             SHORT_ADDR srcAddr,
                                             uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C816_DeleteDeviceListTableEntry(C816_DeleteDeviceListTableEntry_Req* request,
                                                C816_DeleteDeviceListTableEntry_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C817_ReadChannelBlacklist(C817_ReadChannelBlacklist_Req* request,
                                          C817_ReadChannelBlacklist_Resp* response,
                                          SHORT_ADDR srcAddr,
                                          uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C818_WriteChannelBlacklist(C818_WriteChannelBlacklist_Req* request,
                                          C818_WriteChannelBlacklist_Resp* response,
                                          SHORT_ADDR srcAddr,
                                          uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C819_ReadBackOffExponent( C819_ReadBackOffExponent_Req* request, 
                                          C819_ReadBackOffExponent_Resp* response,                                       
                                          SHORT_ADDR srcAddr, 
                                          uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C820_WriteBackOffExponent( C820_WriteBackOffExponent_Req* request, 
                                           C820_WriteBackOffExponent_Resp* response,                                       
                                           SHORT_ADDR srcAddr, 
                                           uint8_t control, uint8_t remainingBytes);

/*  C821 has to be implemented only by the Gateway
uint16_t Execute_C821_WriteNetworkAccessMode(C821_WriteNetworkAccessMode_Req* request,
C821_WriteNetworkAccessMode_Resp* response,
SHORT_ADDR srcAddr,
uint8_t control, uint8_t remainingBytes);
*/

/*  C822 has to be implemented only by the Gateway
uint16_t Execute_C822_ReadNetworkAccessMode(C822_ReadNetworkAccessMode_Req* request,
C822_ReadNetworkAccessMode_Resp* response,
SHORT_ADDR srcAddr,
uint8_t control, uint8_t remainingBytes);
*/

/*  C823 has to be implemented only by the Gateway
uint16_t Execute_C823_RequestSession(C823_RequestSession_Req* request,
C823_RequestSession_Resp* response,
SHORT_ADDR srcAddr,
uint8_t control, uint8_t remainingBytes);
*/


/*********************************************/
/*********** GW and NM Commands **************/

//Execute_C832_ReadNetworkDeviceIdentity
//Execute_C833_ReadNetworkDeviceNeighbourHealth
//Execute_C834_ReadNetworkTopologyInformation
//Execute_C835_ReadBurstMessageList
//Execute_C836_FlushCacheResponses
//Execute_C837_WriteUpdateNotificationBitMask
//Execute_C838_ReadUpdateNotificationBitMask
//Execute_C839_ChangeNotification
//Execute_C840_ReadDeviceStatistics
//Execute_C841_ReadNetworkDeviceIdentityByNickname
//Execute_C842_WriteDeviceSchedulingFlags
//Execute_C843_ReadDeviceSchedulingFlags
//Execute_C844_ReadNetworkConstraints
//Execute_C845_WriteNetworkConstraints


/*********************************************/
/***** Network Management Config Commands ****/

uint16_t Execute_C960_DisconnectDevice(C960_DisconnectDevice_Req* request,
                                      C960_DisconnectDevice_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C961_WriteNetworkKey(C961_WriteNetworkKey_Req* request,
                                     C961_WriteNetworkKey_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C962_WriteDeviceNicknameAddress(C962_WriteDeviceNicknameAddress_Req* request,
                                                C962_WriteDeviceNicknameAddress_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C963_WriteSession(C963_WriteSession_Req* request,
                                  C963_WriteSession_Resp* response,
                                  SHORT_ADDR srcAddr,
                                  uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C964_DeleteSession(C964_DeleteSession_Req* request,
                                   C964_DeleteSession_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C965_WriteSuperframe(C965_WriteSuperframe_Req* request,
                                     C965_WriteSuperframe_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C966_DeleteSuperframe(C966_DeleteSuperframe_Req* request,
                                      C966_DeleteSuperframe_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C967_WriteLink(C967_WriteLink_Req* request,
                               C967_WriteLink_Resp* response,
                               SHORT_ADDR srcAddr,
                               uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C968_DeleteLink(C968_DeleteLink_Req* request,
                                C968_DeleteLink_Resp* response,
                                SHORT_ADDR srcAddr,
                                uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C969_WriteGraphNeighbourPair(C969_WriteGraphNeighbourPair_Req* request,
                                             C969_WriteGraphNeighbourPair_Resp* response,
                                             SHORT_ADDR srcAddr,
                                             uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C970_DeleteGraphConnection(C970_DeleteGraphConnection_Req* request,
                                           C970_DeleteGraphConnection_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C971_WriteNeighbourPropertyFlag(C971_WriteNeighbourPropertyFlag_Req* request,
                                                C971_WriteNeighbourPropertyFlag_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C972_SuspendDevices(C972_SuspendDevices_Req* request,
                                    C972_SuspendDevices_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C973_WriteService(C973_WriteService_Req* request,
                                  C973_WriteService_Resp* response,
                                  SHORT_ADDR srcAddr,
                                  uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C974_WriteRoute(C974_WriteRoute_Req* request,
                                C974_WriteRoute_Resp* response,
                                SHORT_ADDR srcAddr,
                                uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C975_DeleteRoute(C975_DeleteRoute_Req* request,
                                 C975_DeleteRoute_Resp* response,
                                 SHORT_ADDR srcAddr,
                                 uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C976_WriteSourceRoute(C976_WriteSourceRoute_Req* request,
                                      C976_WriteSourceRoute_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes);

uint16_t Execute_C977_DeleteSourceRoute(C977_DeleteSourceRoute_Req* request,
                                       C977_DeleteSourceRoute_Resp* response,
                                       SHORT_ADDR srcAddr,
                                       uint8_t control, uint8_t remainingBytes);


/**************************************************/
/***** Device Specific Wireless-only Commands *****/

uint16_t Execute_C64512_ReadWirelessModuleRevision(C64512_ReadWirelessModuleRevision_Req* request,
                                       C64512_ReadWirelessModuleRevision_Resp* response,
                                       SHORT_ADDR srcAddr,
                                       uint8_t control, uint8_t remainingBytes);


#endif /* _NIVIS_APP_CMDRESP_H_ */
