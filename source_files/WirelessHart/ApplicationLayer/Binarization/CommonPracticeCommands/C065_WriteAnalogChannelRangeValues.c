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

#include "C065_WriteAnalogChannelRangeValues.h"

uint8_t Compose_C065_WriteAnalogChannelRangeValues_Req(C065_WriteAnalogChannelRangeValues_Req* request,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->channelNumberCode);
	STREAM_WRITE_UINT8(toStream, request->upperLowerRangeUnitsCode);
	STREAM_WRITE_FLOAT(toStream, request->upperRangeValue);
	STREAM_WRITE_FLOAT(toStream, request->lowerRangeValue);

	return RCS_N00_Success;
}

uint8_t Parse_C065_WriteAnalogChannelRangeValues_Req(C065_WriteAnalogChannelRangeValues_Req* request, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->channelNumberCode);
	STREAM_READ_UINT8(fromStream, &request->upperLowerRangeUnitsCode);
	STREAM_READ_FLOAT(fromStream, &request->upperRangeValue);
	STREAM_READ_FLOAT(fromStream, &request->lowerRangeValue);

	return RCS_N00_Success;
}

uint8_t Compose_C065_WriteAnalogChannelRangeValues_Resp(C065_WriteAnalogChannelRangeValues_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->channelNumberCode);
	STREAM_WRITE_UINT8(toStream, response->upperLowerRangeUnitsCode);
	STREAM_WRITE_FLOAT(toStream, response->upperRangeValue);
	STREAM_WRITE_FLOAT(toStream, response->lowerRangeValue);
	return RCS_N00_Success;
}

uint8_t Parse_C065_WriteAnalogChannelRangeValues_Resp(C065_WriteAnalogChannelRangeValues_Resp* response, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->channelNumberCode);
	STREAM_READ_UINT8(fromStream, &response->upperLowerRangeUnitsCode);
	STREAM_READ_FLOAT(fromStream, &response->upperRangeValue);
	STREAM_READ_FLOAT(fromStream, &response->lowerRangeValue);
	return RCS_N00_Success;
}
