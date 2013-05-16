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

#include "C64512_ReadWirelessModuleRevision.h"


uint8_t Compose_C64512_ReadWirelessModuleRevision_Req ( C64512_ReadWirelessModuleRevision_Req* request,
                                    BinaryStream* toStream )
{
  return RCS_N00_Success; // success
}

uint8_t Parse_C64512_ReadWirelessModuleRevision_Req ( C64512_ReadWirelessModuleRevision_Req* request,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > 0)
    return RCS_E01_Undefined1; // too many bytes
#endif

  return RCS_N00_Success; // success
}

uint8_t Compose_C64512_ReadWirelessModuleRevision_Resp ( C64512_ReadWirelessModuleRevision_Resp* response,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT16(toStream, response->expandedDeviceType);
  STREAM_WRITE_UINT16(toStream, response->manufacturerIDCode);
  STREAM_WRITE_UINT8(toStream, response->deviceRevisionLevel);
  STREAM_WRITE_UINT8(toStream, response->softwareRevisionLevel);
  STREAM_WRITE_UINT8(toStream, response->hardwareRevisionLevel);

  return RCS_N00_Success; // success
}

uint8_t Parse_C64512_ReadWirelessModuleRevision_Resp ( C64512_ReadWirelessModuleRevision_Resp* response,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C64512_RespSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT16(fromStream, &response->expandedDeviceType);
  STREAM_READ_UINT16(fromStream, &response->manufacturerIDCode);
  STREAM_READ_UINT8(fromStream, &response->deviceRevisionLevel);
  STREAM_READ_UINT8(fromStream, &response->softwareRevisionLevel);
  STREAM_READ_UINT8(fromStream, &response->hardwareRevisionLevel);

  return RCS_N00_Success; // success
}
