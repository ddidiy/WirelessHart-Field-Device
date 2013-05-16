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

#include "C976_WriteSourceRoute.h"

uint8_t Compose_C976_WriteSourceRoute_Req(C976_WriteSourceRoute_Req* request, 
                                          BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, request->m_ucRouteID);
  STREAM_WRITE_UINT8(toStream, request->m_ucHopsNo);

// [doinel.alban] this check should be moved outside of composing function
  if( request->m_ucHopsNo < 1 || request->m_ucHopsNo > 8 )
    return RCM_E68_InvalidNoOfHops;

  for( int i = 0; i < request->m_ucHopsNo; i++ )
    STREAM_WRITE_UINT16(toStream, request->m_aNicknameHopEntries[i]);

  return RCS_N00_Success;
}

/**
* @Debinarize
*/
uint8_t Parse_C976_WriteSourceRoute_Req(C976_WriteSourceRoute_Req* request, 
                                        BinaryStream* fromStream)
{
  STREAM_READ_UINT8(fromStream, &request->m_ucRouteID);
  STREAM_READ_UINT8(fromStream, &request->m_ucHopsNo);

// [doinel.alban] moved to command execution function
//  if( request->m_ucHopsNo < 1 || request->m_ucHopsNo > 8 )
//    return RCM_E68_InvalidNoOfHops;

  uint8_t ucHopsToCopy = (request->m_ucHopsNo > 8) ? 8 : request->m_ucHopsNo; // m_aNicknameHopEntries[8]
  
  if (STREAM_GetRemainingSize(fromStream) < ucHopsToCopy * 2)
    return RCS_E05_TooFewDataBytesReceived;

  for (int i = 0; i < ucHopsToCopy; i++)
    STREAM_READ_UINT16(fromStream, &request->m_aNicknameHopEntries[i]);

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Compose_C976_WriteSourceRoute_Resp(C976_WriteSourceRoute_Resp* response, 
                                           BinaryStream* toStream)
{
  // [doinel.alban] this check should be moved outside of composing function
#ifdef _DEBUG
  if( response->m_ucHopsNo < 1 || response->m_ucHopsNo > 8 )
    return RCS_E01_Undefined1;   // return RCM_E68_InvalidNoOfHops;
#endif

  STREAM_WRITE_UINT8(toStream, response->m_ucRouteID);
  STREAM_WRITE_UINT8(toStream, response->m_ucHopsNo);

  uint8_t ucHopsToCopy = (response->m_ucHopsNo > 8) ? 8 : response->m_ucHopsNo; // m_aNicknameHopEntries[8]
  for (int i = 0; i < ucHopsToCopy; i++)
    STREAM_WRITE_UINT16(toStream, response->m_aNicknameHopEntries[i]);

  STREAM_WRITE_UINT8(toStream, response->m_ucRemainingSourceRoutesNo);


  return RCS_N00_Success;
}

/**
*
*/
uint8_t Parse_C976_WriteSourceRoute_Resp(C976_WriteSourceRoute_Resp* response, 
                                         BinaryStream* fromStream)
{
  STREAM_READ_UINT8(fromStream, &response->m_ucRouteID);
  STREAM_READ_UINT8(fromStream, &response->m_ucHopsNo);

// [doinel.alban] this check should be moved outside of parsing function
#ifdef _DEBUG
  if( response->m_ucHopsNo < 1 || response->m_ucHopsNo > 8 )
    return RCM_E68_InvalidNoOfHops;
#endif

  if( STREAM_GetRemainingSize(fromStream) < response->m_ucHopsNo * 2 + 1)	// hops + no. of source routes left
    return RCS_E05_TooFewDataBytesReceived;

  uint8_t ucHopsToCopy = (response->m_ucHopsNo > 8) ? 8 : response->m_ucHopsNo; // m_aNicknameHopEntries[8]
  for( int i = 0; i < ucHopsToCopy; i++ )
    STREAM_READ_UINT16(fromStream, &response->m_aNicknameHopEntries[i]);

  STREAM_READ_UINT8(fromStream, &response->m_ucRemainingSourceRoutesNo);

  return RCS_N00_Success;
}
