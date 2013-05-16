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

#ifndef PHYSICAL_LAYER_COMMANDS_H_
#define PHYSICAL_LAYER_COMMANDS_H_

#include "CommonTables.h"

enum
{
	CMDID_C797_WriteRadioPower    = 797,
	CMDID_C798_ReadRadioPower     = 798,
	CMDID_C804_ReadRadioCCAMode   = 804,
	CMDID_C805_WriteRadioCCAMode  = 805,
};

enum
{
  C797_ReqSize =  1,
  C798_ReqSize =  0,
  C804_ReqSize =  0,
  C805_ReqSize =  1
};

enum
{
  C797_RespSize =  1,
  C798_RespSize =  1,
  C804_RespSize =  1,
  C805_RespSize =  1
};

#define C797_WriteRadioPower_ReqSize    C797_ReqSize
#define C798_ReadRadioPower_ReqSize     C798_ReqSize
#define C804_ReadRadioCCAMode_ReqSize   C804_ReqSize
#define C805_WriteRadioCCAMode_ReqSize  C805_ReqSize

#define C797_WriteRadioPower_RespSize   C797_RespSize
#define C798_ReadRadioPower_RespSize    C798_RespSize
#define C804_ReadRadioCCAMode_RespSize  C804_RespSize
#define C805_WriteRadioCCAMode_RespSize C805_RespSize


/******************** CMD 797 *************************/

typedef struct
{
	int8_t outputPower;
}C797_WriteRadioPower_Req;

typedef struct
{
	int8_t outputPower;
}C797_WriteRadioPower_Resp;

typedef enum
{
  C797_N00 = RCS_N00_Success,
  C797_E03 = RCS_E03_PassedParameterTooLarge,
  C797_E04 = RCS_E04_PassedParameterTooSmall,
  C797_E05 = RCS_E05_TooFewDataBytesReceived,
  C797_E06 = RCS_E06_DeviceSpecificCommandError,
  C797_E07 = RCS_E07_InWriteProtectMode,
  C797_W08 = RCM_W08_SetToNearestPossibleValue,
  C797_E16 = RCS_E16_AccessRestricted,
  C797_E32 = RCS_E32_Busy,
  C797_E33 = RCS_E33_DelayedResponseInitiated,
  C797_E34 = RCS_E34_DelayedResponseRunning,
  C797_E35 = RCS_E35_DelayedResponseDead,
  C797_E36 = RCS_E36_DelayedResponseConflict
}C797_WriteRadioPower_RespCodes;


/******************** CMD 798 *************************/

typedef EmptyCommand_Req C798_ReadRadioPower_Req;

typedef struct {
	int8_t outputPower;
}C798_ReadRadioPower_Resp;

typedef enum
{
  C798_N00 = RCS_N00_Success,
  C798_E06 = RCS_E06_DeviceSpecificCommandError,
  C798_E32 = RCS_E32_Busy
}C798_ReadRadioPower_RespCodes;


/******************** CMD 804 *************************/

typedef EmptyCommand_Req C804_ReadRadioCCAMode_Req;

typedef struct
{
	uint8_t CCAMode;
}C804_ReadRadioCCAMode_Resp;

typedef enum
{
  C804_N00 = RCS_N00_Success,
  C804_E06 = RCS_E06_DeviceSpecificCommandError,
  C804_E32 = RCS_E32_Busy
}C804_ReadRadioCCAMode_RespCodes;


/******************** CMD 805 *************************/

typedef struct
{
	uint8_t CCAMode;
}C805_WriteRadioCCAMode_Req;

typedef struct
{
	uint8_t CCAMode;
}C805_WriteRadioCCAMode_Resp;

typedef enum
{
  C805_N00 = RCS_N00_Success,
  C805_E02 = RCS_E02_InvalidSelection,
  C805_E05 = RCS_E05_TooFewDataBytesReceived,
  C805_E06 = RCS_E06_DeviceSpecificCommandError,
  C805_E07 = RCS_E07_InWriteProtectMode,
  C805_W08 = RCM_W08_SetToNearestPossibleValue,
  C805_E16 = RCS_E16_AccessRestricted,
  C805_E32 = RCS_E32_Busy,
  C805_E33 = RCS_E33_DelayedResponseInitiated,
  C805_E34 = RCS_E34_DelayedResponseRunning,
  C805_E35 = RCS_E35_DelayedResponseDead,
  C805_E36 = RCS_E36_DelayedResponseConflict
}C805_WriteRadioCCAMode_RespCodes;

#endif /*PHYSICAL_LAYER_COMMANDS_H_*/
