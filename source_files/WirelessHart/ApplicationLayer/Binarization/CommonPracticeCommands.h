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

#ifndef COMMONPRACTICECOMMANDS_BINARIZATION_H_
#define COMMONPRACTICECOMMANDS_BINARIZATION_H_

#include "CommonPracticeCommands/C033_ReadDeviceVariables.h"
#include "CommonPracticeCommands/C034_WritePrimaryVariableDampingValue.h"
#include "CommonPracticeCommands/C035_WritePrimaryVariableRangeValues.h"
#include "CommonPracticeCommands/C036_SetPrimaryVariableUpperRangeValue.h"
#include "CommonPracticeCommands/C037_SetPrimaryVariableLowerRangeValue.h"
#include "CommonPracticeCommands/C040_EnterExitFixedCurrentMode.h"
#include "CommonPracticeCommands/C041_PerformSelfTest.h"
#include "CommonPracticeCommands/C042_PerformDeviceReset.h"
#include "CommonPracticeCommands/C043_SetPrimaryVariableZero.h"
#include "CommonPracticeCommands/C044_WritePrimaryVariableUnits.h"
#include "CommonPracticeCommands/C045_TrimLoopCurrentZero.h"
#include "CommonPracticeCommands/C046_TrimLoopCurrentGain.h"
#include "CommonPracticeCommands/C047_WritePrimaryVariableTransferFunction.h"
#include "CommonPracticeCommands/C049_WritePrimaryVariableTransducerSerialNumber.h"
#include "CommonPracticeCommands/C050_ReadDynamicVariableAssignments.h"
#include "CommonPracticeCommands/C051_WriteDynamicVariableAssignments.h"
#include "CommonPracticeCommands/C052_SetDeviceVariableZero.h"
#include "CommonPracticeCommands/C053_WriteDeviceVariableUnits.h"
#include "CommonPracticeCommands/C054_ReadDeviceVariableInformation.h"
#include "CommonPracticeCommands/C055_WriteDeviceVariableDampingValue.h"
#include "CommonPracticeCommands/C056_WriteDeviceVariableTransducerSerialNo.h"
#include "CommonPracticeCommands/C059_WriteNumberOfResponsePreambles.h"
#include "CommonPracticeCommands/C060_ReadAnalogChannelAndPercentOfRange.h"
#include "CommonPracticeCommands/C062_ReadAnalogChannels.h"
#include "CommonPracticeCommands/C063_ReadAnalogChannelInformation.h"
#include "CommonPracticeCommands/C064_WriteAnalogChannelAdditionalDampingValue.h"
#include "CommonPracticeCommands/C065_WriteAnalogChannelRangeValues.h"
#include "CommonPracticeCommands/C066_EnterExitFixedAnalogChannelMode.h"
#include "CommonPracticeCommands/C067_TrimAnalogChannelZero.h"
#include "CommonPracticeCommands/C068_TrimAnalogChannelGain.h"
#include "CommonPracticeCommands/C069_WriteAnalogChannelTransferFunction.h"
#include "CommonPracticeCommands/C070_ReadAnalogChannelEndpointValues.h"
#include "CommonPracticeCommands/C071_LockDevice.h"
#include "CommonPracticeCommands/C072_Squawk.h"
#include "CommonPracticeCommands/C073_FindDevice.h"
#include "CommonPracticeCommands/C074_ReadIOSystemCapabilities.h"
#include "CommonPracticeCommands/C075_PollSubDevice.h"
#include "CommonPracticeCommands/C076_ReadLockDeviceState.h"
#include "CommonPracticeCommands/C077_SendCommandToSubDevice.h"
#include "CommonPracticeCommands/C078_ReadAggregatedCommands.h"
#include "CommonPracticeCommands/C079_WriteDeviceVariable.h"
#include "CommonPracticeCommands/C080_ReadDeviceVariableTrimPoints.h"
#include "CommonPracticeCommands/C081_ReadDeviceVariableTrimGuidelines.h"
#include "CommonPracticeCommands/C082_WriteDeviceVariableTrimPoint.h"
#include "CommonPracticeCommands/C083_ResetDeviceVariableTrim.h"
#include "CommonPracticeCommands/C084_ReadSubDeviceIdentitySummary.h"
#include "CommonPracticeCommands/C085_ReadIOChannelStatistics.h"
#include "CommonPracticeCommands/C086_ReadSubDeviceStatistics.h"
#include "CommonPracticeCommands/C087_WriteIOSystemMasterMode.h"
#include "CommonPracticeCommands/C088_WriteIOSystemRetryCount.h"
#include "CommonPracticeCommands/C089_SetRealTimeClock.h"
#include "CommonPracticeCommands/C090_ReadRealTimeClock.h"
#include "CommonPracticeCommands/C091_ReadTrendConfiguration.h"
#include "CommonPracticeCommands/C092_WriteTrendConfiguration.h"
#include "CommonPracticeCommands/C093_ReadTrend.h"
#include "CommonPracticeCommands/C094_ReadIOSystemClientSideCommunicationStatistics.h"
#include "CommonPracticeCommands/C095_ReadDeviceCommunicationsStatistics.h"
#include "CommonPracticeCommands/C096_ReadSynchronousAction.h"
#include "CommonPracticeCommands/C097_ConfigureSynchronousAction.h"
#include "CommonPracticeCommands/C098_ReadCommandAction.h"
#include "CommonPracticeCommands/C099_ConfigureCommandAction.h"
#include "CommonPracticeCommands/C101_ReadSubDeviceToBurstMessageMap.h"
#include "CommonPracticeCommands/C102_MapSubDeviceToBurstMessage.h"
#include "CommonPracticeCommands/C103_WriteBurstPeriod.h"
#include "CommonPracticeCommands/C104_WriteBurstTrigger.h"
#include "CommonPracticeCommands/C105_ReadBurstModeConfiguration.h"
#include "CommonPracticeCommands/C106_FlushDelayedResponses.h"
#include "CommonPracticeCommands/C107_WriteBurstDeviceVariables.h"
#include "CommonPracticeCommands/C108_WriteBurstModeCommandNumber.h"
#include "CommonPracticeCommands/C109_BurstModeControl.h"
#include "CommonPracticeCommands/C113_CatchDeviceVariable.h"
#include "CommonPracticeCommands/C114_ReadCaughtDeviceVariable.h"
#include "CommonPracticeCommands/C115_ReadEventNotificationSummary.h"
#include "CommonPracticeCommands/C116_WriteEventNotificationBitMask.h"
#include "CommonPracticeCommands/C117_WriteEventNotificationTiming.h"
#include "CommonPracticeCommands/C118_EventNotificationControl.h"
#include "CommonPracticeCommands/C119_AcknowledgeEventNotification.h"
#include "CommonPracticeCommands/C512_ReadCountryCode.h"
#include "CommonPracticeCommands/C513_WriteCountryCode.h"

#endif /* COMMONPRACTICECOMMANDS_BINARIZATION_H_ */
