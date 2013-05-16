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

#include "C084_ReadSubDeviceIdentitySummary.h"

uint8_t Compose_C084_ReadSubDeviceIdentitySummary_Req(C084_ReadSubDeviceIdentitySummary_Req* request,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT16(toStream, request->subDeviceIndex);

	return RCS_N00_Success;
}

uint8_t Parse_C084_ReadSubDeviceIdentitySummary_Req(C084_ReadSubDeviceIdentitySummary_Req* request,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT16(fromStream, &request->subDeviceIndex);

	return RCS_N00_Success;
}

uint8_t Compose_C084_ReadSubDeviceIdentitySummary_Resp(C084_ReadSubDeviceIdentitySummary_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT16(toStream, response->subDeviceIndex);
	STREAM_WRITE_UINT8(toStream, response->ioCard);
	STREAM_WRITE_UINT8(toStream, response->channel);
	STREAM_WRITE_UINT16(toStream, response->manufacturerID);
	STREAM_WRITE_UINT16(toStream, response->expandedDeviceType);
	STREAM_WRITE_UINT24(toStream, response->deviceID);
	STREAM_WRITE_UINT8(toStream, response->revisionLevel);
	STREAM_WRITE_LATIN(toStream, response->tag, 31);

	return RCS_N00_Success;
}

uint8_t Parse_C084_ReadSubDeviceIdentitySummary_Resp(C084_ReadSubDeviceIdentitySummary_Resp* response,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT16(fromStream, &response->subDeviceIndex);
	STREAM_READ_UINT8(fromStream, &response->ioCard);
	STREAM_READ_UINT8(fromStream, &response->channel);
	STREAM_READ_UINT16(fromStream, &response->manufacturerID);
	STREAM_READ_UINT16(fromStream, &response->expandedDeviceType);
	STREAM_READ_UINT24(fromStream, &response->deviceID);
	STREAM_READ_UINT8(fromStream, &response->revisionLevel);
	STREAM_READ_LATIN(fromStream, &response->tag, 31);

	return RCS_N00_Success;
}
