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

#include "C811_WriteJoinPriority.h"

uint8_t Compose_C811_WriteJoinPriority_Req ( C811_WriteJoinPriority_Req* request,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->JoinPriority);

  return RCS_N00_Success; // success
}

uint8_t Parse_C811_WriteJoinPriority_Req ( C811_WriteJoinPriority_Req* request,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C811_ReqSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &request->JoinPriority);

  return RCS_N00_Success; // success
}

uint8_t Compose_C811_WriteJoinPriority_Resp ( C811_WriteJoinPriority_Resp* response,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->JoinPriority);

  return RCS_N00_Success; // success
}

uint8_t Parse_C811_WriteJoinPriority_Resp ( C811_WriteJoinPriority_Resp* response,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C811_RespSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &response->JoinPriority);

  return RCS_N00_Success; // success
}
