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

#include "C802_ReadRouteList.h"

uint8_t Compose_C802_ReadRouteList_Req ( C802_ReadRouteList_Req* request,
                                    
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucRouteIndex);
  STREAM_WRITE_UINT8(toStream, request->m_ucNoOfEntriesToRead);

  return RCS_N00_Success; // success
}

uint8_t Parse_C802_ReadRouteList_Req ( C802_ReadRouteList_Req* request,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C802_ReqSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &request->m_ucRouteIndex);
  STREAM_READ_UINT8(fromStream, &request->m_ucNoOfEntriesToRead);

  return RCS_N00_Success; // success
}

uint8_t Compose_C802_ReadRouteList_Resp ( C802_ReadRouteList_Resp* response,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucRouteIndex);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfEntriesRead);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfActiveRoutes);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfRoutesRemaining);

  for(uint8_t i = 0; i < response->m_ucNoOfEntriesRead; i++)
  {
    STREAM_WRITE_UINT8(toStream, response->m_aRoutes[i].routeId);
    STREAM_WRITE_UINT16(toStream, response->m_aRoutes[i].destinationNickname);
    STREAM_WRITE_UINT16(toStream, response->m_aRoutes[i].graphId);
    STREAM_WRITE_UINT8(toStream, response->m_aRoutes[i].sourceRouteAttached);
  }

  return RCS_N00_Success; // success
}

uint8_t Parse_C802_ReadRouteList_Resp ( C802_ReadRouteList_Resp* response,
                                    BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &response->m_ucRouteIndex);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfEntriesRead);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfActiveRoutes);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfRoutesRemaining);

  if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfEntriesRead * 6)
    return RCS_E05_TooFewDataBytesReceived;
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > response->m_ucNoOfEntriesRead * 6)
    return RCS_E01_Undefined1; // too many bytes
#endif

  for(uint8_t i = 0; i < response->m_ucNoOfEntriesRead; i++)
  {
    STREAM_READ_UINT8(fromStream, &response->m_aRoutes[i].routeId);
    STREAM_READ_UINT16(fromStream, &response->m_aRoutes[i].destinationNickname);
    STREAM_READ_UINT16(fromStream, &response->m_aRoutes[i].graphId);
    STREAM_READ_UINT8(fromStream, &response->m_aRoutes[i].sourceRouteAttached);
  }

  return RCS_N00_Success; // success
}
