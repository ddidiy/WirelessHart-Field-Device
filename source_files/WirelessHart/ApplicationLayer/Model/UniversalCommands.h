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

#ifndef UNIVERSALCOMMANDS_H_
#define UNIVERSALCOMMANDS_H_

#include "CommonTables.h"

//possible sizes for cmd 48:
#define C048_SIZE_9   9
#define C048_SIZE_14  14
#define C048_SIZE_25  25

enum
{
	CMDID_C000_ReadUniqueIdentifier                       = 0,
	CMDID_C001_ReadPrimaryVariable                        = 1,
	CMDID_C002_ReadLoopCurrentAndPercentOfRange           = 2,
	CMDID_C003_ReadDynamicVariablesAndLoopCurrent         = 3,
	CMDID_C006_WritePollingAddress                        = 6,
	CMDID_C007_ReadLoopConfiguration                      = 7,
	CMDID_C008_ReadDynamicVariableClassifications         = 8,
	CMDID_C009_ReadDeviceVariablesWithStatus              = 9,
	CMDID_C011_ReadUniqueIdentifierAssociatedWithTag      = 11,
	CMDID_C012_ReadMessage                                = 12,
	CMDID_C013_ReadTagDescriptorDate                      = 13,
	CMDID_C014_ReadPrimaryVariableTransducerInformation   = 14,
	CMDID_C015_ReadDeviceInformation                      = 15,
	CMDID_C016_ReadFinalAssemblyNumber                    = 16,
	CMDID_C017_WriteMessage                               = 17,
	CMDID_C018_WriteTagDescriptorDate                     = 18,
	CMDID_C019_WriteFinalAssemblyNumber                   = 19,
	CMDID_C020_ReadLongTag                                = 20,
	CMDID_C021_ReadUniqueIdentifierAssociatedWithLongTag  = 21,
	CMDID_C022_WriteLongTag                               = 22,
	CMDID_C038_ResetConfigurationChangedFlag              = 38,
	CMDID_C048_ReadAdditionalDeviceStatus                 = 48
};

enum
{
  C000_ReqSize =  0,
  C001_ReqSize =  0,
  C002_ReqSize =  0,
  C003_ReqSize =  0,
  C006_ReqSize =  1,  //2, // or 1, if the master is HART rev.5
  C007_ReqSize =  0,
  C008_ReqSize =  0,
  C009_ReqSize =  1, // or 2, 3, ..., 8 (variable size)
  C011_ReqSize =  0, // broadcast command (normally 6)
  C012_ReqSize =  0,
  C013_ReqSize =  0,
  C014_ReqSize =  0,
  C015_ReqSize =  0,
  C016_ReqSize =  0,
  C017_ReqSize = 24,
  C018_ReqSize = 21,
  C019_ReqSize =  3,
  C020_ReqSize =  0,
  C021_ReqSize =  0, // broadcast command (normally 32)
  C022_ReqSize = 32,
  C038_ReqSize =  0,  //2 if master is HART rev.7,
  C048_ReqSize =  0   //25  // or none, if master is HART rev. 5 or 6
};

enum
{
  C000_RespSize = 22,
  C001_RespSize =  5,
  C002_RespSize =  8,
  C003_RespSize = 24,
  C006_RespSize =  2,
  C007_RespSize =  2,
  C008_RespSize =  4,
  C009_RespSize = 13, // or 21, 29, ..., 69 (variable size)
  C011_RespSize = 22,
  C012_RespSize = 24,
  C013_RespSize = 21,
  C014_RespSize = 16,
  C015_RespSize = 18,
  C016_RespSize =  3,
  C017_RespSize = 24,
  C018_RespSize = 21,
  C019_RespSize =  3,
  C020_RespSize = 32,
  C021_RespSize = 22,
  C022_RespSize = 32,
  C038_RespSize =  2,
  C048_RespSize = 25  // or less, depending on Field Device implementation
};

#define C000_ReadUniqueIdentifier_ReqSize                       C000_ReqSize
#define C001_ReadPrimaryVariable_ReqSize                        C001_ReqSize
#define C002_ReadLoopCurrentAndPercentOfRange_ReqSize           C002_ReqSize
#define C003_ReadDynamicVariablesAndLoopCurrent_ReqSize         C003_ReqSize
#define C006_WritePollingAddress_ReqSize                        C006_ReqSize
#define C007_ReadLoopConfiguration_ReqSize                      C007_ReqSize
#define C008_ReadDynamicVariableClassifications_ReqSize         C008_ReqSize
#define C009_ReadDeviceVariablesWithStatus_ReqSize              C009_ReqSize
#define C011_ReadUniqueIdentifierAssociatedWithTag_ReqSize      C011_ReqSize
#define C012_ReadMessage_ReqSize                                C012_ReqSize
#define C013_ReadTagDescriptorDate_ReqSize                      C013_ReqSize
#define C014_ReadPrimaryVariableTransducerInformation_ReqSize   C014_ReqSize
#define C015_ReadDeviceInformation_ReqSize                      C015_ReqSize
#define C016_ReadFinalAssemblyNumber_ReqSize                    C016_ReqSize
#define C017_WriteMessage_ReqSize                               C017_ReqSize
#define C018_WriteTagDescriptorDate_ReqSize                     C018_ReqSize
#define C019_WriteFinalAssemblyNumber_ReqSize                   C019_ReqSize
#define C020_ReadLongTag_ReqSize                                C020_ReqSize
#define C021_ReadUniqueIdentifierAssociatedWithLongTag_ReqSize  C021_ReqSize
#define C022_WriteLongTag_ReqSize                               C022_ReqSize
#define C038_ResetConfigurationChangedFlag_ReqSize              C038_ReqSize
#define C048_ReadAdditionalDeviceStatus_ReqSize                 C048_ReqSize

#define C000_ReadUniqueIdentifier_RespSize                      C000_RespSize
#define C001_ReadPrimaryVariable_RespSize                       C001_RespSize
#define C002_ReadLoopCurrentAndPercentOfRange_RespSize          C002_RespSize
#define C003_ReadDynamicVariablesAndLoopCurrent_RespSize        C003_RespSize
#define C006_WritePollingAddress_RespSize                       C006_RespSize
#define C007_ReadLoopConfiguration_RespSize                     C007_RespSize
#define C008_ReadDynamicVariableClassifications_RespSize        C008_RespSize
#define C009_ReadDeviceVariablesWithStatus_RespSize             C009_RespSize
#define C011_ReadUniqueIdentifierAssociatedWithTag_RespSize     C011_RespSize
#define C012_ReadMessage_RespSize                               C012_RespSize
#define C013_ReadTagDescriptorDate_RespSize                     C013_RespSize
#define C014_ReadPrimaryVariableTransducerInformation_RespSize  C014_RespSize
#define C015_ReadDeviceInformation_RespSize                     C015_RespSize
#define C016_ReadFinalAssemblyNumber_RespSize                   C016_RespSize
#define C017_WriteMessage_RespSize                              C017_RespSize
#define C018_WriteTagDescriptorDate_RespSize                    C018_RespSize
#define C019_WriteFinalAssemblyNumber_RespSize                  C019_RespSize
#define C020_ReadLongTag_RespSize                               C020_RespSize
#define C021_ReadUniqueIdentifierAssociatedWithLongTag_RespSize C021_RespSize
#define C022_WriteLongTag_RespSize                              C022_RespSize
#define C038_ResetConfigurationChangedFlag_RespSize             C038_RespSize
#define C048_ReadAdditionalDeviceStatus_RespSize                C048_RespSize




/******************** CMD 000 *************************/

typedef EmptyCommand_Req C000_ReadUniqueIdentifier_Req;

typedef struct
{
  //byte 0 = 254 hardcoded
  uint32_t deviceID;  // uint24_t
  uint16_t expandedDeviceType;            // ExpandedDeviceTypeCodes
  uint16_t configChangeCounter;
  uint16_t manufacturerIDCode;            // ManufacturerIdentificationCodes
  uint16_t privateLabelDistributorCode;   // PrivateLabelDistributorCodes
  uint8_t minReqPreamblesNo;
  uint8_t protocolMajorRevNo;     // e.g. 7 for HART rev. 7
  uint8_t deviceRevisionLevel;
  uint8_t softwareRevisionLevel;
  uint8_t hardwareRevisionLevel;  // bits 7 to 3 of byte 7
  uint8_t physicalSignalingCode;  // bits 2 to 0 of byte 7; CommonTable10, PhysicalSignalingCodes
  uint8_t flags;
  uint8_t minRespPreamblesNo;
  uint8_t maxNoOfDeviceVars;
  uint8_t extendedFieldDeviceStatus;      // CommonTable17
  uint8_t deviceProfile;                  // DeviceProfileCodes
}C000_ReadUniqueIdentifier_Resp;

typedef enum
{
  C000_N00 = RCS_N00_Success
}C000_ReadUniqueIdentifier_RespCodes;


/******************** CMD 001 *************************/

typedef EmptyCommand_Req C001_ReadPrimaryVariable_Req;

typedef struct
{
	UnitsCodes primaryVariableUnit; //CHECKME [george.petrehus]not found documentation
	float primaryVariable;

}C001_ReadPrimaryVariable_Resp;

typedef enum
{
  C001_N00 = RCS_N00_Success,
	C001_E06 = RCS_E06_DeviceSpecificCommandError,
	C001_W08 = RCM_W08_UpdateFailure,
	C001_E16 = RCS_E16_AccessRestricted

}C001_ReadPrimaryVariable_RespCodes;


/******************** CMD 002 *************************/

typedef EmptyCommand_Req C002_ReadLoopCurrentAndPercentOfRange_Req;

typedef struct
{
	float primaryVariableLoopCurrent;
	float primaryVariablePercentOfRange;

}C002_ReadLoopCurrentAndPercentOfRange_Resp;

typedef enum
{
  C002_N00 = RCS_N00_Success,
  C002_E06 = RCS_E06_DeviceSpecificCommandError,
	C002_W08 = RCM_W08_UpdateFailure,
  C002_E16 = RCS_E16_AccessRestricted

}C002_ReadLoopCurrentAndPercentOfRange_RespCodes;


/******************** CMD 003 *************************/

typedef EmptyCommand_Req C003_ReadDynamicVariablesAndLoopCurrent_Req;

typedef struct
{
  uint8_t noOfVariables;
  float primaryVariableLoopCurrent;
  Variable variables[4];
}C003_ReadDynamicVariablesAndLoopCurrent_Resp;

typedef enum
{
  C003_N00 = RCS_N00_Success,
  C003_E06 = RCS_E06_DeviceSpecificCommandError,
  C003_W08 = RCM_W08_UpdateFailure,
  C003_E16 = RCS_E16_AccessRestricted

}C003_ReadDynamicVariablesAndLoopCurrent_RespCodes;

// = 4 reserved
// = 5 reserved


/******************** CMD 006 *************************/

typedef struct
{
	uint8_t pollingAddressDevice;
	LoopCurrentModeCodes loopCurrentModeCode;
  uint8_t ucBackwardCompatMode;
}C006_WritePollingAddress_Req;

typedef C006_WritePollingAddress_Req C006_WritePollingAddress_Resp;

typedef enum
{
  C006_N00 = RCS_N00_Success,
	C006_E02 = RCS_E02_InvalidSelection,
	C006_E05 = RCS_E05_TooFewDataBytesReceived,
	C006_E06 = RCS_E06_DeviceSpecificCommandError,
	C006_E07 = RCS_E07_InWriteProtectMode,
	C006_E12 = RCM_E12_InvalidModeSelection,
	C006_E16 = RCS_E16_AccessRestricted,
	C006_E32 = RCS_E32_Busy

}C006_WritePollingAddress_RespCodes;


/******************** CMD 007 *************************/

typedef EmptyCommand_Req C007_ReadLoopConfiguration_Req;

typedef struct
{
	uint8_t pollingAddressDevice;
	LoopCurrentModeCodes loopCurrentModeCode;
}C007_ReadLoopConfiguration_Resp;

typedef enum
{
  C007_N00 = RCS_N00_Success,
	C007_E16 = RCS_E16_AccessRestricted

}C007_ReadLoopConfiguration_RespCodes;


/******************** CMD 008 *************************/

typedef EmptyCommand_Req C008_ReadDynamicVariableClassifications_Req;

typedef struct
{
	DeviceVariableClassificationCodes primaryVariableClassification;
	DeviceVariableClassificationCodes secondaryVariableClassification;
	DeviceVariableClassificationCodes tertiaryVariableClassification;
	DeviceVariableClassificationCodes quaternaryVariableClassification;

}C008_ReadDynamicVariableClassifications_Resp;

typedef enum
{
  C008_N00 = RCS_N00_Success,
	C008_E16 = RCS_E16_AccessRestricted

}C008_ReadDynamicVariableClassifications_RespCodes;


/******************** CMD 009 *************************/

typedef struct
{
	uint8_t variablesSize;
	DeviceVariableCodes slotDVC[8];

}C009_ReadDeviceVariablesWithStatus_Req;

typedef struct
{
	uint8_t variablesSize;
	uint8_t extendedFieldDeviceStatus;
	Slot slots[8];
	_time_t slot0TimeStamp;

}C009_ReadDeviceVariablesWithStatus_Resp;

typedef enum
{
  C009_N00 = RCS_N00_Success,
  C009_E02 = RCS_E02_InvalidSelection,
  C009_E05 = RCS_E05_TooFewDataBytesReceived,
  C009_E06 = RCS_E06_DeviceSpecificCommandError,
	C009_W08 = RCM_W08_UpdateFailure,
	C009_W14 = RCM_W14_DynamicVarsReturnedForDeviceVars,
  C009_E16 = RCS_E16_AccessRestricted,
	C009_W30 = RCM_W30_CommandResponseTruncated

}C009_ReadDeviceVariablesWithStatus_RespCodes;


/******************** CMD 011 *************************/

typedef struct
{
	uint8_t tag[6];
}C011_ReadUniqueIdentifierAssociatedWithTag_Req;

typedef C000_ReadUniqueIdentifier_Resp C011_ReadUniqueIdentifierAssociatedWithTag_Resp;


/******************** CMD 012 *************************/

typedef EmptyCommand_Req C012_ReadMessage_Req;

typedef struct
{
	uint8_t message[24];
}C012_ReadMessage_Resp;

typedef enum
{
  C012_N00 = RCS_N00_Success,
	C012_E16 = RCS_E16_AccessRestricted,
	C012_E32 = RCS_E32_Busy

}C012_ReadMessage_RespCodes;


/******************** CMD 013 *************************/

typedef EmptyCommand_Req C013_ReadTagDescriptorDate_Req;

typedef struct
{
	uint8_t tag[6];
	uint8_t descriptor[12];
	_date_t dateCode;
}C013_ReadTagDescriptorDate_Resp;

typedef enum
{
  C013_N00 = RCS_N00_Success,
	C013_E16 = RCS_E16_AccessRestricted,
	C013_E32 = RCS_E32_Busy

}C013_ReadTagDescriptorDate_RespCodes;


/******************** CMD 014 *************************/

typedef EmptyCommand_Req C014_ReadPrimaryVariableTransducerInformation_Req;

typedef struct
{
	uint32_t transducerSerialNumber;
	UnitsCodes transducerLimitsMinimumSpanUnitsCode;
	float upperTransducerLimit;
	float lowerTransducerLimit;
	float minimumSpan;
}C014_ReadPrimaryVariableTransducerInformation_Resp;

typedef enum
{
  C014_N00 = RCS_N00_Success,
	C014_E16 = RCS_E16_AccessRestricted,
	C014_E32 = RCS_E32_Busy

}C014_ReadPrimaryVariableTransducerInformation_RespCodes;


/******************** CMD 015 *************************/

typedef EmptyCommand_Req C015_ReadDeviceInformation_Req;

typedef struct
{
	AlarmSelectionCodes pvAlarmSelectionCodes;
	TransferFunctionCodes pvTransferFunctionCode;
	UnitsCodes pvUpperLowerRangeValuesUnitsCode;
	float pvUpperRangeValue;
	float pvLowerRangeValue;
	float pvDampingValue;
	WriteProtectCodes writeProtectCode;
	uint8_t pvAnalogChannelFlags;
}C015_ReadDeviceInformation_Resp;

typedef enum
{
  C015_N00 = RCS_N00_Success,
	C015_E16 = RCS_E16_AccessRestricted,
	C015_E32 = RCS_E32_Busy

}C015_ReadDeviceInformation_RespCodes;


/******************** CMD 016 *************************/

typedef EmptyCommand_Req  C016_ReadFinalAssemblyNumber_Req;

typedef struct
{
//	uint8_t finalAssemblyNumber[3];
	uint32_t finalAssemblyNumber; //_uint24_t
}C016_ReadFinalAssemblyNumber_Resp;

typedef enum
{
  C016_N00 = RCS_N00_Success,
	C016_E16 = RCS_E16_AccessRestricted,
	C016_E32 = RCS_E32_Busy

}C016_ReadFinalAssemblyNumber_RespCodes;


/******************** CMD 017 *************************/

typedef struct
{
	uint8_t messageString[24];
}C017_WriteMessage_Req;

typedef C017_WriteMessage_Req C017_WriteMessage_Resp;

typedef enum
{
  C017_N00 = RCS_N00_Success,
  C017_E05 = RCS_E05_TooFewDataBytesReceived,
  C017_E06 = RCS_E06_DeviceSpecificCommandError,
  C017_E07 = RCS_E07_InWriteProtectMode,
  C017_E16 = RCS_E16_AccessRestricted,
  C017_E32 = RCS_E32_Busy

}C017_WriteMessage_RespCodes;


/******************** CMD 018 *************************/

typedef struct
{
	uint8_t tag[6];
	uint8_t descriptorUsedByTheMasterForRecordKeeping[12];
	_date_t dateCodeUsedByTheMasterForRecordKeeping;
}C018_WriteTagDescriptorDate_Req;

typedef C018_WriteTagDescriptorDate_Req C018_WriteTagDescriptorDate_Resp;

typedef enum
{
  C018_N00 = RCS_N00_Success,
  C018_E05 = RCS_E05_TooFewDataBytesReceived,
  C018_E06 = RCS_E06_DeviceSpecificCommandError,
  C018_E07 = RCS_E07_InWriteProtectMode,
	C018_E09 = RCM_E09_InvalidDateCodeDetected,
  C018_E16 = RCS_E16_AccessRestricted,
  C018_E32 = RCS_E32_Busy

}C018_WriteTagDescriptorDate_RespCodes;


/******************** CMD 019 *************************/

typedef struct
{
//	uint8_t finalAssemblyNumber[3];
	uint32_t finalAssemblyNumber; //_uint24_t
}C019_WriteFinalAssemblyNumber_Req;

typedef C019_WriteFinalAssemblyNumber_Req C019_WriteFinalAssemblyNumber_Resp;

typedef enum
{
  C019_N00 = RCS_N00_Success,
  C019_E05 = RCS_E05_TooFewDataBytesReceived,
  C019_E06 = RCS_E06_DeviceSpecificCommandError,
  C019_E07 = RCS_E07_InWriteProtectMode,
  C019_E16 = RCS_E16_AccessRestricted,
  C019_E32 = RCS_E32_Busy

}C019_WriteFinalAssemblyNumber_RespCodes;


/******************** CMD 020 *************************/

typedef EmptyCommand_Req C020_ReadLongTag_Req;

typedef struct
{
	uint8_t longTag[32];
}C020_ReadLongTag_Resp;

typedef enum
{
  C020_N00 = RCS_N00_Success,
  C020_E16 = RCS_E16_AccessRestricted,
  C020_E32 = RCS_E32_Busy

}C020_ReadLongTag_RespCodes;


/******************** CMD 021 *************************/

typedef struct
{
	uint8_t longTag[32];
}C021_ReadUniqueIdentifierAssociatedWithLongTag_Req;

typedef C000_ReadUniqueIdentifier_Resp C021_ReadUniqueIdentifierAssociatedWithLongTag_Resp;

//typedef enum
//{
//}C021_ReadUniqueIdentifierAssociatedWithLongTag_RespCodes;


/******************** CMD 022 *************************/

typedef struct
{
	uint8_t longTag[32];
}C022_WriteLongTag_Req;

typedef C022_WriteLongTag_Req C022_WriteLongTag_Resp;

typedef enum
{
  C022_N00 = RCS_N00_Success,
  C022_E05 = RCS_E05_TooFewDataBytesReceived,
  C022_E06 = RCS_E06_DeviceSpecificCommandError,
  C022_E07 = RCS_E07_InWriteProtectMode,
  C022_E16 = RCS_E16_AccessRestricted,
  C022_E32 = RCS_E32_Busy, // a DR could not be started
  C022_E33 = RCS_E33_DelayedResponseInitiated,
  C022_E34 = RCS_E34_DelayedResponseRunning,
  C022_E35 = RCS_E35_DelayedResponseDead,
  C022_E36 = RCS_E36_DelayedResponseConflict

}C022_WriteLongTag_RespCodes;


/******************** CMD 038 *************************/

typedef struct
{
	uint16_t configurationChangeCounter;
        uint8_t backComp; //1 - if is request is empty (for backward compability); 0 - otherwise
}C038_ResetConfigurationChangedFlag_Req;

typedef C038_ResetConfigurationChangedFlag_Req C038_ResetConfigurationChangedFlag_Resp;

typedef enum
{
  C038_N00 = RCS_N00_Success,
  C038_E05 = RCS_E05_TooFewDataBytesReceived,
  C038_E06 = RCS_E06_DeviceSpecificCommandError,
  C038_E07 = RCS_E07_InWriteProtectMode,
	C038_E09 = RCM_E09_ConfigurationChangeCounterMismatch,
  C038_E16 = RCS_E16_AccessRestricted

}C038_ResetConfigurationChangedFlag_RespCodes;


/******************** CMD 048 *************************/

typedef struct
{
	uint8_t deviceSpecificStatus1[6];
	uint8_t extendedDeviceStatus;
	/*
	 * Table 14. Operating Mode Codes
	 * Code Operating Mode Description
	 * This Table is Reserved
	 */
	//CHECKME [george.petrehus] Device Operating Mode -> Byte 7
	uint8_t deviceOperatingMode;
	uint8_t standardizedStatus0;
	/*
	 * 5.30 Table 30. Standardized Status 1
	 * Code Description
	 * This Table is Reserved
	 */
	//CHECKME [george.petrehus] 9 Bits Standardized Status 1 (refer to Common Table 30)
	uint8_t standardizedStatus1;
	uint8_t analogChannelSaturatedCode;
	uint8_t standardizedStatus2;
	uint8_t standardizedStatus3;
	uint8_t analogChannelFixedCode;
	uint8_t deviceSpecificStatus2[11];
        
        uint8_t commandSize;
}C048_ReadAdditionalDeviceStatus_Req;

typedef C048_ReadAdditionalDeviceStatus_Req C048_ReadAdditionalDeviceStatus_Resp;

typedef enum
{
  C048_N00 = RCS_N00_Success,
	C048_E05 = RCS_E05_TooFewDataBytesReceived,
	C048_E06 = RCS_E06_DeviceSpecificCommandError,
	C048_W08 = RCM_W08_UpdateInProgress,
	C048_W14 = RCM_W14_StatusBytesMismatch,
	C048_E16 = RCS_E16_AccessRestricted

}C048_ReadAdditionalDeviceStatus_RespCodes;

#endif /*UNIVERSALCOMMANDS_H_*/
