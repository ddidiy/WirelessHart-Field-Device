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

#include "C093_ReadTrend.h"

uint8_t Compose_C093_ReadTrend_Req(C093_ReadTrend_Req* request,  BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->trendNo);
	return RCS_N00_Success;
}

uint8_t Parse_C093_ReadTrend_Req(C093_ReadTrend_Req* request,  BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->trendNo);

	return RCS_N00_Success;
}

uint8_t Compose_C093_ReadTrend_Resp(C093_ReadTrend_Resp* response,  BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->trendNo);
	STREAM_WRITE_UINT8(toStream, response->deviceVariableCode);
	STREAM_WRITE_UINT8(toStream, response->deviceVariableClassificationCode);
	STREAM_WRITE_UINT8(toStream, response->deviceVariableUnitCode);
	STREAM_WRITE_DATE(toStream, response->dateOfTrendValue0);
	STREAM_WRITE_TIME(toStream, response->timeOfTrendValue0);
	STREAM_WRITE_TIME(toStream, response->sampleIterval);
	for (uint8_t i = 0; i < C093_NoOfTrendValues; i++)
	{
		STREAM_WRITE_FLOAT(toStream, response->trendValues[i].trendValue);
		STREAM_WRITE_UINT8(toStream, response->trendValues[i].trendValueStatus);
	}
	return RCS_N00_Success;
}

uint8_t Parse_C093_ReadTrend_Resp(C093_ReadTrend_Resp* response,  BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->trendNo);
	STREAM_READ_UINT8(fromStream, &response->deviceVariableCode);
	STREAM_READ_UINT8(fromStream, &response->deviceVariableClassificationCode);
	STREAM_READ_UINT8(fromStream, &response->deviceVariableUnitCode);
	STREAM_READ_DATE(fromStream, &response->dateOfTrendValue0);
	STREAM_READ_TIME(fromStream, &response->timeOfTrendValue0);
	STREAM_READ_TIME(fromStream, &response->sampleIterval);
	for (uint8_t i = 0; i < C093_NoOfTrendValues; i++)
	{
		STREAM_READ_FLOAT(fromStream, &response->trendValues[i].trendValue);
		STREAM_READ_UINT8(fromStream, &response->trendValues[i].trendValueStatus);
	}
	return RCS_N00_Success;
}
