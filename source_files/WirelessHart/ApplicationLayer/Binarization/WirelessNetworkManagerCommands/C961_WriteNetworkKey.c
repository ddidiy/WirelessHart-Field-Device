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

#include "C961_WriteNetworkKey.h"

uint8_t Compose_C961_WriteNetworkKey_Req(C961_WriteNetworkKey_Req* request, 
                                         BinaryStream* toStream)
{
  STREAM_WRITE_BYTES(toStream, &request->m_aKeyValue[0], 16);
  // this command can be truncated after the key value if the network key hasn't been set before;
  // if execution time is zero, we can omit sending it
  for(uint8_t i = 0; i < 5; i++)
  {
    if(request->m_tExecutionTime[i])
    {
      STREAM_WRITE_BYTES(toStream, &request->m_tExecutionTime, 5);
      break;
    }
  }

  return RCS_N00_Success;
}

/**
* @Debinarize
*/
uint8_t Parse_C961_WriteNetworkKey_Req(C961_WriteNetworkKey_Req* request, 
                                       BinaryStream* fromStream)
{
  STREAM_READ_BYTES(fromStream, &request->m_aKeyValue[0], 16);

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
uint8_t Compose_C961_WriteNetworkKey_Resp(C961_WriteNetworkKey_Resp* response, 
                                          BinaryStream* toStream)
{
  STREAM_WRITE_BYTES(toStream, &response->m_aKeyValue[0], 16);
  if(!response->m_ucTruncated)
  {
    STREAM_WRITE_BYTES(toStream, response->m_tExecutionTime, 5);
  }

  return RCS_N00_Success;
}

/**
*
*/
uint8_t Parse_C961_WriteNetworkKey_Resp(C961_WriteNetworkKey_Resp* response, 
                                        BinaryStream* fromStream)
{
  STREAM_READ_BYTES(fromStream, &response->m_aKeyValue[0], 16);
  if(STREAM_GetRemainingSize(fromStream) >= 5)
  {
    STREAM_READ_BYTES(fromStream, &response->m_tExecutionTime, 5);
  }
  else // command is truncated; execution time is set to 0
  {
    memset(response->m_tExecutionTime, 0, 5);
  }

  return RCS_N00_Success;
}
