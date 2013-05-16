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

#include "C769_ReadJoinStatus.h"

uint8_t Compose_C769_ReadJoinStatus_Req(C769_ReadJoinStatus_Req* request, 
                                        BinaryStream* toStream)
{
  return RCS_N00_Success;
}

uint8_t Parse_C769_ReadJoinStatus_Req(C769_ReadJoinStatus_Req* request, 
                                      BinaryStream* fromStream)
{
  return RCS_N00_Success;
}

uint8_t Compose_C769_ReadJoinStatus_Resp(C769_ReadJoinStatus_Resp* response, 
                                         BinaryStream* toStream)
{
  STREAM_WRITE_UINT8(toStream, response->wirelessMode & 0x0F);  // bits 7 to 4 must be set to 0
  STREAM_WRITE_UINT16(toStream, response->joinStatus);
  STREAM_WRITE_UINT8(toStream, response->noOfAvailableNeighbors);
  STREAM_WRITE_UINT8(toStream, response->noOfAdvertisingPacketsReceived);
  STREAM_WRITE_UINT8(toStream, response->noOfJoinAttempts);
  STREAM_WRITE_TIME(toStream, response->joinRetryTimer);
  STREAM_WRITE_TIME(toStream, response->networkSearchTimer);

  return RCS_N00_Success;
}

uint8_t Parse_C769_ReadJoinStatus_Resp(C769_ReadJoinStatus_Resp* response, 
                                       BinaryStream* fromStream)
{
  STREAM_READ_UINT8(fromStream, &response->wirelessMode);
  response->wirelessMode &= 0x0F;   // bits 7 to 4 must be set to 0
  STREAM_READ_UINT16(fromStream, &response->joinStatus);
  STREAM_READ_UINT8(fromStream, &response->noOfAvailableNeighbors);
  STREAM_READ_UINT8(fromStream, &response->noOfAdvertisingPacketsReceived);
  STREAM_READ_UINT8(fromStream, &response->noOfJoinAttempts);
  STREAM_READ_TIME(fromStream, &response->joinRetryTimer);
  STREAM_READ_TIME(fromStream, &response->networkSearchTimer);

  return RCS_N00_Success;
}
