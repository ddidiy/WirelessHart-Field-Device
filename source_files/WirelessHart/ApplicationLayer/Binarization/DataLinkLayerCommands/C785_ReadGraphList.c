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



#include "C785_ReadGraphList.h"

uint8_t Compose_C785_ReadGraphList_Req ( C785_ReadGraphList_Req* request,
                                    
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucGraphListIndex);

  return RCS_N00_Success; // success
}

uint8_t Parse_C785_ReadGraphList_Req ( C785_ReadGraphList_Req* request,
                                    
		                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C785_ReqSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &request->m_ucGraphListIndex);

  return RCS_N00_Success; // success
}

uint8_t Compose_C785_ReadGraphList_Resp ( C785_ReadGraphList_Resp* response,
                                    
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucGraphListIndex);
  STREAM_WRITE_UINT8(toStream, response->m_ucTotalNoOfGraphs);
  STREAM_WRITE_UINT16(toStream, response->m_unGraphId);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfNeighbors);

  for(uint8_t i = 0; i < response->m_ucNoOfNeighbors; i++)
  {
    STREAM_WRITE_UINT16(toStream, response->m_aNicknameOfNeighbor[i]);
  }

  return RCS_N00_Success; // success
}

uint8_t Parse_C785_ReadGraphList_Resp ( C785_ReadGraphList_Resp* response,
                                    
		                    BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &response->m_ucGraphListIndex);
  STREAM_READ_UINT8(fromStream, &response->m_ucTotalNoOfGraphs);
  STREAM_READ_UINT16(fromStream, &response->m_unGraphId);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfNeighbors);

  if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfNeighbors * 2)
    return RCS_E05_TooFewDataBytesReceived;
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > response->m_ucNoOfNeighbors * 2)
    return RCS_E01_Undefined1; // too many bytes
#endif

  for(uint8_t i = 0; i < response->m_ucNoOfNeighbors; i++)
  {
    STREAM_READ_UINT16(fromStream, &response->m_aNicknameOfNeighbor[i]);
  }

  return RCS_N00_Success; // success
}

