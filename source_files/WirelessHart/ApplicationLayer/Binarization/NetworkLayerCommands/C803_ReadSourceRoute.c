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

#include "C803_ReadSourceRoute.h"

uint8_t Compose_C803_ReadSourceRoute_Req ( C803_ReadSourceRoute_Req* request,
                                          BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucRouteId);

  return RCS_N00_Success;
}

uint8_t Parse_C803_ReadSourceRoute_Req ( C803_ReadSourceRoute_Req* request,
                                        BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &request->m_ucRouteId);

  return RCS_N00_Success;
}

uint8_t Compose_C803_ReadSourceRoute_Resp ( C803_ReadSourceRoute_Resp* response,
                                        BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucRouteId);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfHops);

  for(uint8_t i = 0; i < response->m_ucNoOfHops; i++)
  {
    STREAM_WRITE_UINT16(toStream, response->m_aHopNicknames[i]);
  }

  return RCS_N00_Success;
}

uint8_t Parse_C803_ReadSourceRoute_Resp ( C803_ReadSourceRoute_Resp* response,
                                         BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &response->m_ucRouteId);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfHops);

  if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfHops * 2)
    return RCS_E05_TooFewDataBytesReceived;

  for(uint8_t i = 0; i < response->m_ucNoOfHops; i++)
  {
    STREAM_READ_UINT16(fromStream, &response->m_aHopNicknames[i]);
  }

  return RCS_N00_Success;
}
