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

#include "C782_ReadSessionEntries.h"

uint8_t Compose_C782_ReadSessionEntries_Req ( C782_ReadSessionEntries_Req* request,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucSessionIndex);
  STREAM_WRITE_UINT8(toStream, request->m_ucNoOfEntriesToRead);

  return RCS_N00_Success; // success
}

uint8_t Parse_C782_ReadSessionEntries_Req ( C782_ReadSessionEntries_Req* request,
                                    BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &request->m_ucSessionIndex);
  STREAM_READ_UINT8(fromStream, &request->m_ucNoOfEntriesToRead);

  return RCS_N00_Success; // success
}

uint8_t Compose_C782_ReadSessionEntries_Resp ( C782_ReadSessionEntries_Resp* response,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucSessionIndex);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfEntriesRead);
  STREAM_WRITE_UINT8(toStream, response->m_unNoOfActiveSessions);

  for(uint8_t i = 0; i < response->m_ucNoOfEntriesRead; i++)
  {
    STREAM_WRITE_UINT8(toStream, response->m_aSessions[i].sessionType);
    STREAM_WRITE_UINT16(toStream, response->m_aSessions[i].peerDeviceNickname);
    STREAM_WRITE_BYTES(toStream, response->m_aSessions[i].peerDeviceUniqueId, 5);
    STREAM_WRITE_UINT32(toStream, response->m_aSessions[i].peerDeviceNonceCounterVal);
    STREAM_WRITE_UINT32(toStream, response->m_aSessions[i].theDeviceNonceCounterVal);
  }

  return RCS_N00_Success; // success
}

uint8_t Parse_C782_ReadSessionEntries_Resp ( C782_ReadSessionEntries_Resp* response,
                                    BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &response->m_ucSessionIndex);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfEntriesRead);
  STREAM_READ_UINT8(fromStream, &response->m_unNoOfActiveSessions);

  if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfEntriesRead * 16)
    return RCS_E05_TooFewDataBytesReceived;

  for(uint8_t i = 0; i < response->m_ucNoOfEntriesRead; i++)
  {
    STREAM_READ_UINT8(fromStream, &response->m_aSessions[i].sessionType);
    STREAM_READ_UINT16(fromStream, &response->m_aSessions[i].peerDeviceNickname);
    STREAM_READ_BYTES(fromStream, &response->m_aSessions[i].peerDeviceUniqueId, 5);
    STREAM_READ_UINT32(fromStream, &response->m_aSessions[i].peerDeviceNonceCounterVal);
    STREAM_READ_UINT32(fromStream, &response->m_aSessions[i].theDeviceNonceCounterVal);
  }

  return RCS_N00_Success; // success
}
