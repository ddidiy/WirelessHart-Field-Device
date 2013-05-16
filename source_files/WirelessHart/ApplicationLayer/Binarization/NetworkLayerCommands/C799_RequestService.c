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

#include "C799_RequestService.h"

uint8_t Compose_C799_RequestService_Req ( C799_RequestService_Req* request,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucServiceId);
  STREAM_WRITE_UINT8(toStream, request->m_ucServiceRequestFlags);
  STREAM_WRITE_UINT8(toStream, request->m_ucServiceApplicationDomain);
  STREAM_WRITE_UINT16(toStream, request->m_unNicknameOfPeer);
  STREAM_WRITE_TIME(toStream, request->m_tPeriod);

  return RCS_N00_Success; // success
}

uint8_t Parse_C799_RequestService_Req ( C799_RequestService_Req* request,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C799_ReqSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &request->m_ucServiceId);
  STREAM_READ_UINT8(fromStream, &request->m_ucServiceRequestFlags);
  STREAM_READ_UINT8(fromStream, &request->m_ucServiceApplicationDomain);
  STREAM_READ_UINT16(fromStream, &request->m_unNicknameOfPeer);
  STREAM_READ_TIME(fromStream, &request->m_tPeriod);

  return RCS_N00_Success; // success
}

uint8_t Compose_C799_RequestService_Resp ( C799_RequestService_Resp* response,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucServiceId);
  STREAM_WRITE_UINT8(toStream, response->m_ucServiceRequestFlags);
  STREAM_WRITE_UINT8(toStream, response->m_ucServiceApplicationDomain);
  STREAM_WRITE_UINT16(toStream, response->m_unNicknameOfPeer);
  STREAM_WRITE_TIME(toStream, response->m_tPeriod);
  STREAM_WRITE_UINT8(toStream, response->m_ucRouteId);

  return RCS_N00_Success; // success
}

uint8_t Parse_C799_RequestService_Resp ( C799_RequestService_Resp* response,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C799_RespSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &response->m_ucServiceId);
  STREAM_READ_UINT8(fromStream, &response->m_ucServiceRequestFlags);
  STREAM_READ_UINT8(fromStream, &response->m_ucServiceApplicationDomain);
  STREAM_READ_UINT16(fromStream, &response->m_unNicknameOfPeer);
  STREAM_READ_TIME(fromStream, &response->m_tPeriod);
  STREAM_READ_UINT8(fromStream, &response->m_ucRouteId);

  return RCS_N00_Success; // success
}
