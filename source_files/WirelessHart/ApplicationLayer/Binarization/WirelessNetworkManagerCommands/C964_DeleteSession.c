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

#include "C964_DeleteSession.h"

uint8_t Compose_C964_DeleteSession_Req(C964_DeleteSession_Req* request, 
                                       BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, request->m_eSessionType);
  STREAM_WRITE_UINT16(toStream, request->m_unPeerNickname);

  return RCS_N00_Success;
}

/**
* @Debinarize
*/
uint8_t Parse_C964_DeleteSession_Req(C964_DeleteSession_Req* request, 
                                     BinaryStream* fromStream)
{
  STREAM_READ_UINT8(fromStream, &request->m_eSessionType);
  STREAM_READ_UINT16(fromStream, &request->m_unPeerNickname);

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Compose_C964_DeleteSession_Resp(C964_DeleteSession_Resp* response, 
                                        BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, response->m_eSessionType);
  STREAM_WRITE_UINT16(toStream, response->m_unPeerNickname);
  STREAM_WRITE_UINT8(toStream, response->m_ucRemainingSessionsNo);

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Parse_C964_DeleteSession_Resp(C964_DeleteSession_Resp* response, 
                                      BinaryStream* fromStream)
{
  STREAM_READ_UINT8(fromStream, &response->m_eSessionType);
  STREAM_READ_UINT16(fromStream, &response->m_unPeerNickname);
  STREAM_READ_UINT8(fromStream, &response->m_ucRemainingSessionsNo);

  return RCS_N00_Success;
}
