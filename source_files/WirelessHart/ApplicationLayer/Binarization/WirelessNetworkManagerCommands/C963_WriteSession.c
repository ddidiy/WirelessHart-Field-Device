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

#include "C963_WriteSession.h"

uint8_t Compose_C963_WriteSession_Req(C963_WriteSession_Req* request, 
                                      BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, request->m_eSessionType);
  STREAM_WRITE_UINT16(toStream, request->m_unPeerNickname);
  STREAM_WRITE_BYTES(toStream, &request->m_aPeerUniqueID, 5);
  STREAM_WRITE_UINT32(toStream, request->m_ulPeerNonceCounterValue);
  STREAM_WRITE_BYTES(toStream, request->m_aKeyValue, 16);
  STREAM_WRITE_UINT8(toStream, request->m_ucReserved);
  // the execution time can be truncated if this is a new session;
  // if execution time is zero, we can omit sending it
  for(uint8_t i = 0; i < 5; i++)
  {
    if(request->m_tExecutionTime[i])
    {
      STREAM_WRITE_BYTES(toStream, request->m_tExecutionTime, 5);
      break;
    }
  }

  return RCS_N00_Success;
}

/**
* @Debinarize
*/
uint8_t Parse_C963_WriteSession_Req(C963_WriteSession_Req* request, 
                                    BinaryStream* fromStream)
{

  STREAM_READ_UINT8(fromStream, &request->m_eSessionType);

// [doinel.alban] moved to command execution function
//  if( request->m_eSessionType > SessionTypeCode_Join )
//    return RCM_E67_SessionTypeInvalid;

  STREAM_READ_UINT16(fromStream, &request->m_unPeerNickname);
  STREAM_READ_BYTES(fromStream, request->m_aPeerUniqueID, 5);
  STREAM_READ_UINT32(fromStream, &request->m_ulPeerNonceCounterValue);
  STREAM_READ_BYTES(fromStream, request->m_aKeyValue, 16);
  STREAM_READ_UINT8(fromStream, &request->m_ucReserved);

  uint8_t n = STREAM_GetRemainingSize(fromStream);
  if(n >= 5)
  { // take only the first 5 bytes, ignore the rest
    request->m_ucTruncated = 0;
    STREAM_READ_BYTES(fromStream, &request->m_tExecutionTime, 5);
  }
  else if(n == 0) 
  { // command is truncated; execution time is set to 0
    // -truncation is permitted only if this is a new session
    // -the response must respect the same style: truncated/not truncated (for all commands with Execution Time)
    request->m_ucTruncated = 1;
    memset(request->m_tExecutionTime, 0, 5);
  }
  else
  { // m_tExecutionTime must have 5 bytes
    return RCS_E05_TooFewDataBytesReceived;
  }

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Compose_C963_WriteSession_Resp(C963_WriteSession_Resp* response, 
                                       BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, response->m_eSessionType);
  STREAM_WRITE_UINT16(toStream, response->m_unPeerNickname);
  STREAM_WRITE_UINT16(toStream, response->m_unPeerExpandedDeviceTypeCode);
  STREAM_WRITE_UINT24(toStream, response->m_ulPeerDeviceID);    // uint24
  STREAM_WRITE_UINT32(toStream, response->m_ulPeerNonceCounterValue);
  STREAM_WRITE_BYTES(toStream, response->m_aKeyValue, 16);
  STREAM_WRITE_UINT8(toStream, response->m_ucRemainingSessionsNo);
  
  if(!response->m_ucTruncated)
  {
    STREAM_WRITE_BYTES(toStream, response->m_tExecutionTime, 5);
  }

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Parse_C963_WriteSession_Resp(C963_WriteSession_Resp* response, 
                                     BinaryStream* fromStream)
{

  STREAM_READ_UINT8(fromStream, &response->m_eSessionType);

// [doinel.alban] this check should be moved outside of parsing function
//  if( response->m_eSessionType > SessionTypeCode_Join )
//    return RCM_E67_SessionTypeInvalid;

  STREAM_READ_UINT16(fromStream, &response->m_unPeerNickname);
  STREAM_READ_UINT16(fromStream, &response->m_unPeerExpandedDeviceTypeCode);
  STREAM_READ_UINT24(fromStream, &response->m_ulPeerDeviceID);		// uint24
  STREAM_READ_UINT32(fromStream, &response->m_ulPeerNonceCounterValue);
  STREAM_READ_BYTES(fromStream, response->m_aKeyValue, 16);
  STREAM_READ_UINT8(fromStream, &response->m_ucRemainingSessionsNo);
  if(STREAM_GetRemainingSize(fromStream) >= 5)
  {
    STREAM_READ_BYTES(fromStream, response->m_tExecutionTime, 5);
  }
  else // command is truncated; execution time is set to 0
  {
    memset(response->m_tExecutionTime, 0, 5);
  }

  return RCS_N00_Success;
}
