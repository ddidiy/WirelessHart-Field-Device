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

#include "C971_WriteNeighbourPropertyFlag.h"

uint8_t Compose_C971_WriteNeighbourPropertyFlag_Req(C971_WriteNeighbourPropertyFlag_Req* request, 
                                                    BinaryStream* toStream)
{
  // [doinel.alban] this check should be moved outside of composing function
#ifdef _DEBUG
  if(request->m_ucNeighborFlags & 0xFE)	// at least one of bits 7 to 1 is set
    return RCM_E66_InvalidNeighborProperty;
#endif

  STREAM_WRITE_UINT16(toStream, request->m_unNeighborNickname);
  STREAM_WRITE_UINT8(toStream, request->m_ucNeighborFlags);

  return RCS_N00_Success;
}

/**
* @Debinarize
*/
uint8_t Parse_C971_WriteNeighbourPropertyFlag_Req(C971_WriteNeighbourPropertyFlag_Req* request, 
                                                  BinaryStream* fromStream)
{
  STREAM_READ_UINT16(fromStream, &request->m_unNeighborNickname);
  STREAM_READ_UINT8(fromStream, &request->m_ucNeighborFlags);

// [doinel.alban] moved to command execution function
//  if(request->m_ucNeighborFlags & 0xFE)	// at least one of bits 7 to 1 is set
//    return RCM_E66_InvalidNeighborProperty;

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Compose_C971_WriteNeighbourPropertyFlag_Resp(C971_WriteNeighbourPropertyFlag_Resp* response, 
                                                     BinaryStream* toStream)
{
  // [doinel.alban] this check should be moved outside of composing function
//#ifdef _DEBUG
//  if(response->m_ucNeighborFlags & 0xFE)	// at least one of bits 7 to 1 is set
//    return RCM_E66_InvalidNeighborProperty;
//#endif

  STREAM_WRITE_UINT16(toStream, response->m_unNeighborNickname);
  STREAM_WRITE_UINT8(toStream, response->m_ucNeighborFlags);

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Parse_C971_WriteNeighbourPropertyFlag_Resp(C971_WriteNeighbourPropertyFlag_Resp* response, 
                                                   BinaryStream* fromStream)
{
  STREAM_READ_UINT16(fromStream, &response->m_unNeighborNickname);
  STREAM_READ_UINT8(fromStream, &response->m_ucNeighborFlags);

// [doinel.alban] this check should be moved outside of parsing function
//  if(response->m_ucNeighborFlags & 0xFE)	// at least one of bits 7 to 1 is set
//    return RCM_E66_InvalidNeighborProperty;

  return RCS_N00_Success;
}
