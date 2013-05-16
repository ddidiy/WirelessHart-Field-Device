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

#include "C973_WriteService.h"

uint8_t Compose_C973_WriteService_Req(C973_WriteService_Req* request, 
                                      BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, request->m_ucServiceID);
  STREAM_WRITE_UINT8(toStream, request->m_ucRequestFlags & 0x07);		// discard bits 7 to 3
  STREAM_WRITE_UINT8(toStream, request->m_eApplicationDomain);
  STREAM_WRITE_UINT16(toStream, request->m_unPeerNickname);
  STREAM_WRITE_TIME(toStream, request->m_tPeriod);
  STREAM_WRITE_UINT8(toStream, request->m_ucRouteID);

  return RCS_N00_Success;
}

/**
* @Debinarize
*/
uint8_t Parse_C973_WriteService_Req(C973_WriteService_Req* request, 
                                    BinaryStream* fromStream)
{
  STREAM_READ_UINT8(fromStream, &request->m_ucServiceID);
  STREAM_READ_UINT8(fromStream, &request->m_ucRequestFlags);
  request->m_ucRequestFlags &= 0x07;		// ignore bits 7 to 3
  STREAM_READ_UINT8(fromStream, &request->m_eApplicationDomain);

// [doinel.alban] moved to command execution function
//  if(request->m_eApplicationDomain > ServiceApplicationDomain_BlockTransfer)
//    return RCM_E68_InvalidApplicationDomain;

  STREAM_READ_UINT16(fromStream, &request->m_unPeerNickname);
  STREAM_READ_TIME(fromStream, &request->m_tPeriod);
  STREAM_READ_UINT8(fromStream, &request->m_ucRouteID);

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Compose_C973_WriteService_Resp(C973_WriteService_Resp* response, 
                                       BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, response->m_ucServiceID);
  STREAM_WRITE_UINT8(toStream, response->m_ucRequestFlags & 0x07);		// discard bits 7 to 3
  STREAM_WRITE_UINT8(toStream, response->m_eApplicationDomain);
  STREAM_WRITE_UINT16(toStream, response->m_unPeerNickname);
  STREAM_WRITE_TIME(toStream, response->m_tPeriod);
  STREAM_WRITE_UINT8(toStream, response->m_ucRouteID);
  STREAM_WRITE_UINT8(toStream, response->m_ucRemainingServicesNo);

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Parse_C973_WriteService_Resp(C973_WriteService_Resp* response, 
                                     BinaryStream* fromStream)
{
  STREAM_READ_UINT8(fromStream, &response->m_ucServiceID);
  STREAM_READ_UINT8(fromStream, &response->m_ucRequestFlags);
  response->m_ucRequestFlags &= 0x07;		// ignore bits 7 to 3
  STREAM_READ_UINT8(fromStream, &response->m_eApplicationDomain);

// [doinel.alban] this check should be moved outside of parsing function
//  if(response->m_eApplicationDomain > ServiceApplicationDomain_BlockTransfer)
//    return RCM_E68_InvalidApplicationDomain;

  STREAM_READ_UINT16(fromStream, &response->m_unPeerNickname);
  STREAM_READ_TIME(fromStream, &response->m_tPeriod);
  STREAM_READ_UINT8(fromStream, &response->m_ucRouteID);
  STREAM_READ_UINT8(fromStream, &response->m_ucRemainingServicesNo);

  return RCS_N00_Success;
}
