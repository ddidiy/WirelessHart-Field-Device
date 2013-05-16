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

#include "C787_ReportNeighborSignalLevels.h"

uint8_t Compose_C787_ReportNeighborSignalLevels_Req ( C787_ReportNeighborSignalLevels_Req* request,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucNeighborTableIndex);
  STREAM_WRITE_UINT8(toStream, request->m_ucNoOfNeighborEntriesToRead);

  return RCS_N00_Success; // success
}

uint8_t Parse_C787_ReportNeighborSignalLevels_Req ( C787_ReportNeighborSignalLevels_Req* request,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C787_ReqSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT8(fromStream, &request->m_ucNeighborTableIndex);
  STREAM_READ_UINT8(fromStream, &request->m_ucNoOfNeighborEntriesToRead);

  return RCS_N00_Success; // success
}

uint8_t Compose_C787_ReportNeighborSignalLevels_Resp ( C787_ReportNeighborSignalLevels_Resp* response,
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucNeighborTableIndex);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfNeighborEntriesRead);
  STREAM_WRITE_UINT8(toStream, response->m_ucTotalNoOfNeighbors);

  for(uint8_t i = 0; i < response->m_ucNoOfNeighborEntriesRead; i++)
  {
    STREAM_WRITE_UINT16(toStream, response->m_aNeighbors[i].nickname);
    STREAM_WRITE_INT8(toStream, response->m_aNeighbors[i].RSLindB);
  }

  return RCS_N00_Success; // success
}

uint8_t Parse_C787_ReportNeighborSignalLevels_Resp ( C787_ReportNeighborSignalLevels_Resp* response,
                                    BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &response->m_ucNeighborTableIndex);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfNeighborEntriesRead);
  STREAM_READ_UINT8(fromStream, &response->m_ucTotalNoOfNeighbors);

  if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfNeighborEntriesRead * 3)
    return RCS_E05_TooFewDataBytesReceived;
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > response->m_ucNoOfNeighborEntriesRead * 3)
    return RCS_E01_Undefined1; // too many bytes
#endif

  for(uint8_t i = 0; i < response->m_ucNoOfNeighborEntriesRead; i++)
  {
    STREAM_READ_UINT16(fromStream, &response->m_aNeighbors[i].nickname);
    STREAM_READ_INT8(fromStream, &response->m_aNeighbors[i].RSLindB);
  }

  return RCS_N00_Success; // success
}
