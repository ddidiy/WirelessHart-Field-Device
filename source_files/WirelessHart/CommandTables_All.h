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

#ifndef DEVICECOMMANDTABLES_ALL_H_
#define DEVICECOMMANDTABLES_ALL_H_

//used just for testing purpose


#include "../system.h"


//#define INCLUDE_RECOMMENDED_COMMON_COMMANDS
//#define INCLUDE_I_O_ADAPTER_COMMANDS
//#define INCLUDE_RECOMMENDED_WIRELESS_COMMANDS



// device is acting as a Master for the following commands
const ParserComposerEntry g_composeReqParseResp[] = {
	CREATE_COMPOSER_REQ_PARSER_RESP_ENTRY         (C770_RequestActiveAdvertise),    // master & slave
  CREATE_COMPOSER_REQ_PARSER_RESP_ENTRY         (C799_RequestService),            // master
	CREATE_COMPOSER_REQ_PARSER_RESP_ENTRY         (C801_DeleteService),             // master & slave
	{0xffff, 0, 0} //nicu.dascalu - only for testing, to detect size of array
};


const ParserComposerEntry g_parseReqComposeResp[] = {
  // Universal Commands - all Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C000_ReadUniqueIdentifier),               
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C001_ReadPrimaryVariable),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C002_ReadLoopCurrentAndPercentOfRange),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C003_ReadDynamicVariablesAndLoopCurrent),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C006_WritePollingAddress),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C007_ReadLoopConfiguration),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C008_ReadDynamicVariableClassifications),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C009_ReadDeviceVariablesWithStatus),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C011_ReadUniqueIdentifierAssociatedWithTag),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C012_ReadMessage),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C013_ReadTagDescriptorDate),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C014_ReadPrimaryVariableTransducerInformation),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C015_ReadDeviceInformation),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C016_ReadFinalAssemblyNumber),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C017_WriteMessage),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C018_WriteTagDescriptorDate),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C019_WriteFinalAssemblyNumber),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C020_ReadLongTag),                        
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C021_ReadUniqueIdentifierAssociatedWithLongTag),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C022_WriteLongTag),

  // Common Practice Commands
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C033_ReadDeviceVariables),                            // not required
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C034_WritePrimaryVariableDampingValue),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C035_WritePrimaryVariableRangeValues),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C036_SetPrimaryVariableUpperRangeValue),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C037_SetPrimaryVariableLowerRangeValue),

  // Universal Commands - all Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C038_ResetConfigurationChangedFlag),                  // Mandatory

  // Common Practice Commands
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C040_EnterExitFixedCurrentMode),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C041_PerformSelfTest),                                // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C042_PerformDeviceReset),                             // Mandatory
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C043_SetPrimaryVariableZero),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C044_WritePrimaryVariableUnits),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C045_TrimLoopCurrentZero),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C046_TrimLoopCurrentGain),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C047_WritePrimaryVariableTransferFunction),

  // Universal Commands - all Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C048_ReadAdditionalDeviceStatus),                     // Mandatory

  // Common Practice Commands
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C049_WritePrimaryVariableTransducerSerialNumber),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C050_ReadDynamicVariableAssignments),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C051_WriteDynamicVariableAssignments),
#ifdef INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C052_SetDeviceVariableZero),                          // Recommended - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C053_WriteDeviceVariableUnits),                       // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C054_ReadDeviceVariableInformation),                  // Mandatory
#ifdef INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C055_WriteDeviceVariableDampingValue),                // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_COMMON_COMMANDS
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C056_WriteDeviceVariableTransducerSerialNo),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C059_WriteNumberOfResponsePreambles),                 // Mandatory
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C060_ReadAnalogChannelAndPercentOfRange),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C062_ReadAnalogChannels),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C063_ReadAnalogChannelInformation),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C064_WriteAnalogChannelAdditionalDampingValue),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C065_WriteAnalogChannelRangeValues),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C066_EnterExitFixedAnalogChannelMode),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C067_TrimAnalogChannelZero),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C068_TrimAnalogChannelGain),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C069_WriteAnalogChannelTransferFunction),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C070_ReadAnalogChannelEndpointValues),
#ifdef INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C071_LockDevice),                                     // Recommended - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C072_Squawk),                                         // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_COMMON_COMMANDS
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C073_FindDevice),                                     // should never return an error code or response when device not armed
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C074_ReadIOSystemCapabilities),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C075_PollSubDevice),
#ifdef INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C076_ReadLockDeviceState),                            // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_COMMON_COMMANDS
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C077_SendCommandToSubDevice),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C078_ReadAggregatedCommands),                         // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C079_WriteDeviceVariable),                            // Mandatory - TBD
#ifdef INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C080_ReadDeviceVariableTrimPoints),                   // Recommended - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C081_ReadDeviceVariableTrimGuidelines),               // Recommended - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C082_WriteDeviceVariableTrimPoint),                   // Recommended - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C083_ResetDeviceVariableTrim),                        // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_COMMON_COMMANDS
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C084_ReadSubDeviceIdentitySummary),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C085_ReadIOChannelStatistics),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C086_ReadSubDeviceStatistics),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C087_WriteIOSystemMasterMode),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C088_WriteIOSystemRetryCount),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C089_SetRealTimeClock),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C090_ReadRealTimeClock),                              // Mandatory - TBD
#ifdef INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C091_ReadTrendConfiguration),                         // Recommended - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C092_WriteTrendConfiguration),                        // Recommended - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C093_ReadTrend),                                      // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_COMMON_COMMANDS
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C094_ReadIOSystemClientSideCommunicationStatistics),
#ifdef INCLUDE_RECOMMENDED_COMMON_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C095_ReadDeviceCommunicationsStatistics),             // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_COMMON_COMMANDS
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C096_ReadSynchronousAction),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C097_ConfigureSynchronousAction),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C098_ReadCommandAction),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C099_ConfigureCommandAction),
#ifdef INCLUDE_I_O_ADAPTER_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C101_ReadSubDeviceToBurstMessageMap),                 // Adapter Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C102_MapSubDeviceToBurstMessage),                     // Adapter Mandatory
#endif // INCLUDE_I_O_ADAPTER_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C103_WriteBurstPeriod),                               // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C104_WriteBurstTrigger),                              // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C105_ReadBurstModeConfiguration),                     // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C106_FlushDelayedResponses),                          // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C107_WriteBurstDeviceVariables),                      // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C108_WriteBurstModeCommandNumber),                    // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C109_BurstModeControl),                               // Mandatory
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C113_CatchDeviceVariable),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C114_ReadCaughtDeviceVariable),
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C115_ReadEventNotificationSummary),                   // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C116_WriteEventNotificationBitMask),                  // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C117_WriteEventNotificationTiming),                   // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C118_EventNotificationControl),                       // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C119_AcknowledgeEventNotification),                   // Mandatory - TBD
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C512_ReadCountryCode),
//	CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C513_WriteCountryCode),
        
        // device specific commands
        CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C133_ReadFirmwareVersion),
        
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C768_WriteJoinKey),                                   // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C769_ReadJoinStatus),                                 // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C770_RequestActiveAdvertise),                         // Mandatory - TBD  // master & slave
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C771_ForceJoin),                                      // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C772_ReadJoinModeConfiguration),                      // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C773_WriteNetworkId),                                 // Mandatory - (partially) - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C774_ReadNetworkId),                                  // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C775_WriteNetworkTag),                                // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C776_ReadNetworkTag),                                 // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C777_ReadWirelessDeviceInformation),                  // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C778_ReadBatteryLife),                                // Mandatory - TBD
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C779_ReportDeviceHealth),                             // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C780_ReportNeighborHealthList),                       // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C781_ReadDeviceNicknameAddress),                      // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C782_ReadSessionEntries),                             // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C783_ReadSuperframeList),                             // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C784_ReadLinkList),                                   // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C785_ReadGraphList),                                  // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C786_ReadNeighborPropertyFlag),                       // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C787_ReportNeighborSignalLevels),                     // Mandatory
	CREATE_NULLCOMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C788_AlarmPathDown),                  // will only be published, must respond "Access Restricted"
	CREATE_NULLCOMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C789_AlarmSourceRouteFailed),         // will only be published, must respond "Access Restricted"
	CREATE_NULLCOMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C790_AlarmGraphRouteFailed),          // will only be published, must respond "Access Restricted"
	CREATE_NULLCOMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(C791_AlarmTransportLayerFailed),      // will only be published, must respond "Access Restricted"
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C793_WriteUTCTime),                                   // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C794_ReadUTCTime),                                    // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C795_WriteTimerInterval),                             // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C796_ReadTimerInterval),                              // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C797_WriteRadioPower),                                // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C798_ReadRadioPower),                                 // Mandatory
//	CREATE_NULLCOMPOSER_RESP_NULLPARSER_REQ_NULLEXECUTE_ENTRY(C799_RequestService),             // required on NM only
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C800_ReadServiceList),                                // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C801_DeleteService),                                  // Mandatory // master & slave
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C802_ReadRouteList),                                  // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C803_ReadSourceRoute),                                // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C804_ReadRadioCCAMode),                               // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C805_WriteRadioCCAMode),                              // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C806_ReadHandheldSuperframe),                         // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C807_RequestHandheldSuperframeMode),                  // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C808_ReadTimeToLive),                                 // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C809_WriteTimeToLive),                                // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C810_ReadJoinPriority),                               // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C811_WriteJoinPriority),                              // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C812_ReadPacketReceivePriority),                      // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C813_WritePacketReceivePriority),                     // Mandatory
#ifdef INCLUDE_RECOMMENDED_WIRELESS_COMMANDS
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C814_ReadDeviceListEntries),                          // Recommended - TBD
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C815_AddDeviceListTableEntry),                        // Recommended - TBD
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C816_DeleteDeviceListTableEntry),                     // Recommended - TBD
#endif // INCLUDE_RECOMMENDED_WIRELESS_COMMANDS
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C817_ReadChannelBlacklist),                           // Mandatory
//CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C818_WriteChannelBlacklist),              // maybe could be used on Transceiver???
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C819_ReadBackOffExponent),                            // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C820_WriteBackOffExponent),                           // Mandatory

//  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C821_WriteNetworkAccessMode),
//  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C822_ReadNetworkAccessMode),
//  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C823_RequestSession),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C832_ReadNetworkDeviceIdentity),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C833_ReadNetworkDeviceNeighbourHealth),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C834_ReadNetworkTopologyInformation),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C835_ReadBurstMessageList),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C836_FlushCacheResponses),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C837_WriteUpdateNotificationBitMask),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C838_ReadUpdateNotificationBitMask),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C839_ChangeNotification),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C840_ReadDeviceStatistics),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C841_ReadNetworkDeviceIdentityByNickname),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C842_WriteDeviceSchedulingFlags),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C843_ReadDeviceSchedulingFlags),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C844_ReadNetworkConstraints),
//	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C845_WriteNetworkConstraints),

  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C960_DisconnectDevice),                               // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C961_WriteNetworkKey),                                // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C962_WriteDeviceNicknameAddress),                     // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C963_WriteSession),                                   // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C964_DeleteSession),                                  // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C965_WriteSuperframe),                                // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C966_DeleteSuperframe),                               // Mandatory
  CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C967_WriteLink),                                      // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C968_DeleteLink),                                     // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C969_WriteGraphNeighbourPair),                        // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C970_DeleteGraphConnection),                          // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C971_WriteNeighbourPropertyFlag),                     // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C972_SuspendDevices),                                 // Mandatory - TBD
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C973_WriteService),                                   // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C974_WriteRoute),                                     // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C975_DeleteRoute),                                    // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C976_WriteSourceRoute),                               // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C977_DeleteSourceRoute),                              // Mandatory
	CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(C64512_ReadWirelessModuleRevision),                   // Mandatory?
	{0xffff, 0, 0} //nicu.dascalu - only for testing, to detect end of array
};


#endif /* DEVICECOMMANDTABLES_ALL_H_ */
