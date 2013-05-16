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

#ifndef DEVICE_SPECIFIC_COMMANDS_H_
#define DEVICE_SPECIFIC_COMMANDS_H_

#include "CommonTables.h"

enum
{
  CMDID_C133_ReadFirmwareVersion   = 133
};

enum
{
  C133_ReqSize = 0
};

enum
{
  C133_RespSize = 4
};

#define C133_ReadFirmwareVersion_ReqSize           C133_ReqSize

#define C133_ReadFirmwareVersion_RespSize          C133_RespSize

/******************** CMD 133 *************************/

typedef EmptyCommand_Req C133_ReadFirmwareVersion_Req;

typedef struct
{
  uint8_t MajorVersion;  // MAJOR_VERSION
  uint8_t MinorVersion;  // MINOR_VERSION
  uint8_t Build;         // BUILD
  uint8_t Letter;        // LETTER
}C133_ReadFirmwareVersion_Resp;  


typedef enum
{
  C133_NOO = RCS_N00_Success
}C133_ReadFirmwareVersion_RespCodes;



#endif /* DEVICE_SPECIFIC_WIRELESS_ONLY_COMMANDS_H_ */
