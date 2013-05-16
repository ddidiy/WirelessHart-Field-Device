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

#ifndef COMMON_RESPONSE_CODE_H_
#define COMMON_RESPONSE_CODE_H_

//see Command Response Code Specification.pdf

// RCS_Nxx - ResponseCode-SingleDefinition-Notification #xx
// RCS_Exx - ResponseCode-SingleDefinition-Error        #xx
// RCS_Wxx - ResponseCode-SingleDefinition-Warning      #xx
// RCM_Exx - ResponseCode-MultipleDefinition-Error      #xx
// RCM_Wxx - ResponseCode-MultipleDefinition-Warning    #xx

typedef enum
{
	RCS_N00_Success = 0,

	//Errors: 1-7, 16-23, 32-64
	RCS_E01_Undefined1 = 1,
	RCS_E02_InvalidSelection = 2,
	RCS_E03_PassedParameterTooLarge = 3,
	RCS_E04_PassedParameterTooSmall = 4,
	RCS_E05_TooFewDataBytesReceived = 5,
	RCS_E06_DeviceSpecificCommandError = 6,
	RCS_E07_InWriteProtectMode = 7,

  RCM_W08_UpdateFailure = 8,
  RCM_W08_UpdateTimesAdjusted = 8,
  RCM_W08_UpdatePeriodIncreased = 8,
  RCM_W08_SetToNearestPossibleValue = 8,
  RCM_W08_AllButRunningDelayedResponsesFlushed = 8,
  RCM_W08_DeviceFamilyStatusBitNotSet = 8,
  RCM_W08_TrendNotActive = 8,
  RCM_W08_SamplingTimeAdjusted = 8,
  RCM_W08_BurstConditionConflict = 8,
  RCM_W08_UpdatePeriodOrDebounceIntervalAdjusted = 8,
  RCM_W08_NotAllEventsCleared = 8,
  RCM_W08_NetworkIDChangePending = 8,
  RCM_W08_DeviceAlreadyInList = 8,
  RCM_W08_UpdateInProgress = 8,
  RCM_W08_BroadcastAddrDeletedInResp = 8,
  RCM_W08_UnsupportedPropertyFlag = 8,
  RCS_W08_NotAllEventsCleared = 8,

  RCM_E09_LowerRangeValueTooHigh = 9,
  RCM_E09_AppliedProcessTooHigh = 9,
  RCM_E09_InvalidBurstMessage = 9,
  RCM_E09_InvalidEventMessage = 9,
  RCM_E09_ConfigurationChangeCounterMismatch = 9,
  RCM_E09_InsufficientBandwidth = 9,
  RCM_E09_NotInProperCurrentMode = 9,
  RCM_E09_IncorrectLoopCurrentModeOrValue = 9,
  RCM_E09_NotInProperAnalogChannelMode = 9,
  RCM_E09_UnableToSquawk = 9,
  RCM_E09_NoSubDeviceFound = 9,
  RCM_E09_InvalidCommandRequested = 9,
  RCM_E09_BadTriggerAction = 9,
  RCM_E09_InvalidSubDeviceIndex = 9,
  RCM_E09_NoHandheldSuperframe = 9,
  RCM_E09_InvalidDateCodeDetected = 9,
  RCM_E09_InvalidPropertyFlag = 9,

  RCM_E10_LowerRangeValueTooLow = 10,
  RCM_E10_AppliedProcessTooLow = 10,
  RCM_E10_InvalidLockCode = 10,
  RCM_E10_InvalidWriteDeviceVariableCode = 10,
  RCM_E10_InvalidDate = 10,
  RCM_E10_BadCommandData = 10,
  RCM_E10_InvalidCaptureModeCode = 10,

  RCM_E11_UpperRangeValueTooHigh = 11,
  RCM_E11_InMultidropMode = 11,
  RCM_E11_InvalidDeviceVariableCode = 11,
  RCM_E11_InvalidDeviceVariableClassification = 11,
  RCM_E11_TrimErrorExcessCorrectionAttempted = 11,
  RCM_E11_CannotLockDevice = 11,
  RCM_E11_LoopCurrentNotActive = 11,
  RCM_E11_InvalidTrendNumber = 11,
  RCM_E11_InvalidTime = 11,
  RCM_E11_InvalidSlotNumber = 11,               // !!! C113

  RCM_E12_UpperRangeValueTooLow = 12,
  RCM_E12_InvalidUnitsCode = 12,
  RCM_E12_InvalidModeSelection = 12,
  RCM_E12_InvalidDeviceVariable = 12,
  RCM_E12_CommandNumberNotSupported = 12,       // !!! C113

  RCM_E13_InvalidTransferFunctionCode = 13,
  RCM_E13_UpperAndLowerRangeValuesOutOfLimits = 13,
  RCM_E13_ComputationError = 13,
  RCM_E13_CommandNumberNotSupported = 13,       // !!! C097, C099
  RCM_E13_InvalidBurstTriggerModeSelectionCode = 13,

  RCM_W14_SpanTooSmall = 14,
  RCM_W14_DynamicVarsReturnedForDeviceVars = 14,
  RCM_W14_UpdateRateUncertain = 14,
  RCM_W14_NewLowerRangeValuePushedUpperRangeOver = 14,
  RCM_W14_CommBandwidthLessThanRequested = 14,
  RCM_W14_StatusBytesMismatch = 14,

  RCM_E15_InvalidAnalogChannelCodeNumber = 15,

	RCS_E16_AccessRestricted = 16,
	RCS_E17_InvalidDeviceVariableIndex = 17,
	RCS_E18_InvalidUnitsCode = 18,
	RCS_E19_DeviceVariableIndexNotAllowed = 19,
	RCS_E20_InvalidExtendedCommandNumber = 20,
	RCS_E21_InvalidIOCardNumber = 21,
	RCS_E22_InvalidChannelNumber = 22,
	RCS_E23_SubdeviceResponseTooLong = 23,

	// reserved for future: W24-W27
	RCS_W24_ReservedForFuture24 = 24,
	RCS_W27_ReservedForFuture27 = 27,

	RCM_E28_InvalidRangeUnitsCode = 28,
	RCM_E29_InvalidSpan = 29,

	RCM_W30_CommandResponseTruncated = 30,
	RCM_W31_NotDefined31 = 31,

	RCS_E32_Busy = 32,
	RCS_E33_DelayedResponseInitiated = 33,
	RCS_E34_DelayedResponseRunning = 34,
	RCS_E35_DelayedResponseDead = 35,
	RCS_E36_DelayedResponseConflict = 36,

	// reserved for future: E37-E59
	RCS_E37_ReservedForFuture37 = 37,
	RCS_E59_ReservedForFuture59 = 59,

	RCS_E60_PayloadTooLong = 60,
	RCS_E61_NoBuffersAvailable = 61,
	RCS_E62_NoAlarmEventBuffersAvailable = 62,
	RCS_E63_PriorityTooLow = 63,
	RCS_E64_CommandNotImplemented = 64,

	RCM_E65_Declined = 65,
	RCM_E65_ForceJoinDeclined = 65,
	RCM_E65_InvalidNetworkID = 65,
	RCM_E65_InvalidTimerType = 65,
	RCM_E65_InvalidTimeToLive = 65,
	RCM_E65_InvalidJoinPriority = 65,
	RCM_E65_UnknownNickname = 65,             // !!! C786, C823, C971
	RCM_E65_InvalidNickname = 65,             // !!! C962
	RCM_E65_ServiceRequestDenied = 65,
	RCM_E65_LinkNotFound = 65,
	RCM_E65_NoMoreEntriesAvailable = 65,
	RCM_E65_EntryNotFound = 65,
	RCM_E65_IllegalFrequencyChannelBits = 65,
	RCM_E65_UnknownUID = 65,
	RCM_E65_KeyChangeFailed = 65,
	RCM_E65_SessionWithGivenPeerDoesntExist = 65,
	RCM_E65_InvalidSuspendTime = 65,
	RCM_E65_InvalidNeighborTableIndex = 65,
	RCM_E65_InvalidRouteID = 65,              // !!! C975

	RCM_E66_InvalidNumberOfSlots = 66,
	RCM_E66_InvalidExecutionTime = 66,
	RCM_E66_UnknownNotificationFlag = 66,
	RCM_E66_DeviceListConflict = 66,
	RCM_E66_InvalidNeighborProperty = 66,
	RCM_E66_InvalidServiceID = 66,
	RCM_E66_InvalidResumeTime = 66,
	RCM_E66_LinkAlreadyExists = 66,
	RCM_E66_InvalidReasonCode = 66,
	RCM_E66_UnknownServiceFlag = 66,
	RCM_E66_UnknownNickname = 66,             // !!! C969
	RCM_E66_InvalidNickname = 66,             // !!! C974
	RCM_E66_InvalidRouteID = 66,              // !!! C976, C977
	RCM_E66_PeerHasInsufficientCapacityForAnotherSession = 66,

	RCM_E67_InvalidGraphID = 67,
	RCM_E67_UnknownSuperframeID = 67,
	RCM_E67_ReasonCodeRejected = 67,
	RCM_E67_UnknownApplicationDomain = 67,
	RCM_E67_InvalidNickname = 67,             // !!! C976
	RCM_E67_SessionTypeInvalid = 67,
	RCM_E67_InvalidNoOfSlots = 67,
	RCM_E67_CantAddEdgeWhenSuperframeUsedAsGraphEquiv = 67,
	RCM_E67_OpenTransactionPending = 67,

	RCM_E68_UnknownNickname = 68,             // !!! C799
	RCM_E68_InvalidSlotNumber = 68,
	RCM_E68_InvalidSuperframeMode = 68,
	RCM_E68_DeleteNotAllowed = 68,
	RCM_E68_InvalidApplicationDomain = 68,
	RCM_E68_InvalidNoOfHops = 68,

	RCM_E69_InvalidLinkOptions = 69,
	RCM_E69_InvalidServiceId = 69,
	RCM_E69_UnknownCorrespNickname = 69,

	RCM_E70_InvalidChannelOffset = 70,
	RCM_E70_InvalidRouteID = 70,              // !!! C973

	RCM_E71_InvalidLinkType = 71,
	RCM_E71_CorrespNicknameAndRouteCorrespMismatch = 71,

	RCM_E72_NoMoreNeighborsAvailable = 72,

	// reserved for future: W96-W111
	RCS_W96_ReservedForFuture96 = 96,
	RCS_W111_ReservedForFuture111 = 111,

  RCS_N255_MakeNoResponse = 255, // Nivis defined Response Code - the request will return no response (e.g. C011, C021)
  RCS_N256_ReturnWithDR = 256, //needed for delayed response mechanism
  RCS_N257_ReturnFromDR = 257, //needed for delayed response mechanism
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
  RCS_N258_ReturnToAppQueue = 258, //needed cmd tunneling
  RCS_N2598_ReturnFromSrvcReq = 259
#endif    
} CommandResponseCodes;

//FIXME [nicu.dascalu] - check if c99 supports inline functions

/*
#define IsResponseCodeError(rc)\
	((1 <= rc && rc <= 7) || (16 <= rc && rc <= 23) || (32 <= rc && rc <= 64)\
        || (9 <= rc && rc <= 13) || (rc == 15) || (rc == 28) || (rc == 29) || (65 <= rc && rc <= 95))
*/
//CHECME [nicu.dascalu] - it is correct impl the warning checking function ???
#define IsResponseCodeWarning(rc)\
	(rc != 0 && !IsResponseCodeError(rc))


/*
inline
uint8_t IsResponseCodeError(uint8_t rc)
{
	return
		//single def
		(1 <= rc && rc <=7) || (16 <= rc && rc <=23) || (32 <= rc && rc <= 64)
		// multi def
		|| (9 <= rc && rc <= 13) || (rc == 15) || (rc == 28) || (rc == 29) || (65 <= rc && rc <= 95);
}

inline
uint8_t IsResponseCodeWarning(uint8_t rc)
{
	return 	rc != 0 && !IsResponseCodeError(rc);
}
*/



#endif /* COMMON_RESPONSE_CODE_H_ */
