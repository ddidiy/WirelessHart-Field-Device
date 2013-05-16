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

#include "C089_SetRealTimeClock.h"

uint8_t Compose_C089_SetRealTimeClock_Req(C089_SetRealTimeClock_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->timeSetCode);
	STREAM_WRITE_DATE(toStream, request->date);
	STREAM_WRITE_TIME(toStream, request->time);
	STREAM_WRITE_UINT16(toStream, request->nothingSpecial);
	return RCS_N00_Success;
}

uint8_t Parse_C089_SetRealTimeClock_Req(C089_SetRealTimeClock_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->timeSetCode);
	STREAM_READ_DATE(fromStream, &request->date);
	STREAM_READ_TIME(fromStream, &request->time);
	STREAM_READ_UINT16(fromStream, &request->nothingSpecial);

	return RCS_N00_Success;
}

uint8_t Compose_C089_SetRealTimeClock_Resp(C089_SetRealTimeClock_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->timeSetCode);
	STREAM_WRITE_DATE(toStream, response->date);
	STREAM_WRITE_TIME(toStream, response->time);
	return RCS_N00_Success;
}

uint8_t Parse_C089_SetRealTimeClock_Resp(C089_SetRealTimeClock_Resp* response, 
		BinaryStream* fromStream)
{
		STREAM_READ_UINT8(fromStream, &response->timeSetCode);
		STREAM_READ_DATE(fromStream, &response->date);
		STREAM_READ_TIME(fromStream, &response->time);

		return RCS_N00_Success;
}
