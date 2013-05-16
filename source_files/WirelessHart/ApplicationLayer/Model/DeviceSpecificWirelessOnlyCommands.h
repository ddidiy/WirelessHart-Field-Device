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

#ifndef DEVICE_SPECIFIC_WIRELESS_ONLY_COMMANDS_H_
#define DEVICE_SPECIFIC_WIRELESS_ONLY_COMMANDS_H_

#include "CommonTables.h"

enum
{
  CMDID_C64512_ReadWirelessModuleRevision   = 64512
};

enum
{
  C64512_ReqSize = 0
};

enum
{
  C64512_RespSize = 7
};

#define C64512_ReadWirelessModuleRevision_ReqSize           C64512_ReqSize

#define C64512_ReadWirelessModuleRevision_RespSize          C64512_RespSize

/******************** CMD 64512 *************************/

typedef EmptyCommand_Req C64512_ReadWirelessModuleRevision_Req;

typedef struct
{
  uint16_t expandedDeviceType;            // ExpandedDeviceTypeCodes, CommonTable1
  uint16_t manufacturerIDCode;            // ManufacturerIdentificationCodes, CommonTable8
  uint8_t deviceRevisionLevel;
  uint8_t softwareRevisionLevel;
  uint8_t hardwareRevisionLevel;          // Unsigned-5: bits 4 to 0
}C64512_ReadWirelessModuleRevision_Resp;  // Wireless transceiver module identification data, not device identification data


typedef enum
{
  C64512_NOO = RCS_N00_Success
}C64512_ReadWirelessModuleRevision_RespCodes;



#endif /* DEVICE_SPECIFIC_WIRELESS_ONLY_COMMANDS_H_ */
