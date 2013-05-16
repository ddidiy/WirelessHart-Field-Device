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

#include "C840_ReadDeviceStatistics.h"


uint8_t Compose_C840_ReadDeviceStatistics_Req(C840_ReadDeviceStatistics_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, &request->UniqueID, 5);

	return RCS_N00_Success;
}

/**
 * @Debinarize
 */
uint8_t Parse_C840_ReadDeviceStatistics_Req(C840_ReadDeviceStatistics_Req* request, 
		BinaryStream* fromStream)
{
#ifdef _DEBUG
	if (5 > STREAM_GetRemainingSize(fromStream))
		return RCS_E05_TooFewDataBytesReceived;
#endif
	STREAM_READ_BYTES(fromStream, &request->UniqueID, 5);

	return RCS_N00_Success;
}

/**
 *
 */
uint8_t Compose_C840_ReadDeviceStatistics_Resp(C840_ReadDeviceStatistics_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, &response->UniqueID, 5);

	STREAM_WRITE_UINT16(toStream, response->ActiveGraphsNo);
	STREAM_WRITE_UINT16(toStream, response->ActiveFramesNo);
	STREAM_WRITE_UINT16(toStream, response->ActiveLinksNo);
	STREAM_WRITE_UINT8(toStream, response->NeighboursNo);
	STREAM_WRITE_TIME(toStream, response->AverageLatency);

	STREAM_WRITE_UINT16(toStream, response->JoinCount);
	STREAM_WRITE_DATE(toStream, response->LastJoinTime);
	STREAM_WRITE_UINT32(toStream, response->LastJoinTimeToday);

	return RCS_N00_Success;
}

/**
 *
 */
uint8_t Parse_C840_ReadDeviceStatistics_Resp(C840_ReadDeviceStatistics_Resp* response, 
		BinaryStream* fromStream)
{
#ifdef _DEBUG
	if (25 > STREAM_GetRemainingSize(fromStream))
		return RCS_E05_TooFewDataBytesReceived;
#endif

	STREAM_READ_BYTES(fromStream, &response->UniqueID, 5);

	STREAM_READ_UINT16(fromStream, &response->ActiveGraphsNo);
	STREAM_READ_UINT16(fromStream, &response->ActiveFramesNo);
	STREAM_READ_UINT16(fromStream, &response->ActiveLinksNo);
	STREAM_READ_UINT8(fromStream, &response->NeighboursNo);
	STREAM_READ_TIME(fromStream, &response->AverageLatency);

	STREAM_READ_UINT16(fromStream, &response->JoinCount);
	STREAM_READ_DATE(fromStream, &response->LastJoinTime);
	STREAM_READ_UINT32(fromStream, &response->LastJoinTimeToday);

	return RCS_N00_Success;
}
