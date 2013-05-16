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

#include "C814_ReadDeviceListEntries.h"

uint8_t Compose_C814_ReadDeviceListEntries_Req ( C814_ReadDeviceListEntries_Req* request,
                                    
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucDeviceListCode);
  STREAM_WRITE_UINT8(toStream, request->m_ucNoOfListEntriesToRead);
  STREAM_WRITE_UINT16(toStream, request->m_unStartingListIndex);

  return RCS_N00_Success; // success
}

uint8_t Parse_C814_ReadDeviceListEntries_Req ( C814_ReadDeviceListEntries_Req* request,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C814_ReqSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &request->m_ucDeviceListCode);
  STREAM_READ_UINT8(fromStream, &request->m_ucNoOfListEntriesToRead);
  STREAM_READ_UINT16(fromStream, &request->m_unStartingListIndex);

  return RCS_N00_Success; // success
}

uint8_t Compose_C814_ReadDeviceListEntries_Resp ( C814_ReadDeviceListEntries_Resp* response,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucDeviceListCode);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfListEntriesRead);
  STREAM_WRITE_UINT16(toStream, response->m_unStartingListIndex);
  STREAM_WRITE_UINT16(toStream, response->m_unTotalNoOfEntriesInList);

  for(uint8_t i = 0; i < response->m_ucNoOfListEntriesRead; i++)
  {
    STREAM_WRITE_BYTES(toStream, response->m_aDeviceUniqueIds[i], 5);
  }

  return RCS_N00_Success; // success
}

uint8_t Parse_C814_ReadDeviceListEntries_Resp ( C814_ReadDeviceListEntries_Resp* response,
                                    BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &response->m_ucDeviceListCode);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfListEntriesRead);
  STREAM_READ_UINT16(fromStream, &response->m_unStartingListIndex);
  STREAM_READ_UINT16(fromStream, &response->m_unTotalNoOfEntriesInList);

  if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfListEntriesRead * 5)
    return RCS_E05_TooFewDataBytesReceived;
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > response->m_ucNoOfListEntriesRead * 5)
    return RCS_E01_Undefined1; // too many bytes
#endif

  for(uint8_t i = 0; i < response->m_ucNoOfListEntriesRead; i++)
  {
    STREAM_READ_BYTES(fromStream, &response->m_aDeviceUniqueIds[i], 5);
  }

  return RCS_N00_Success; // success
}
