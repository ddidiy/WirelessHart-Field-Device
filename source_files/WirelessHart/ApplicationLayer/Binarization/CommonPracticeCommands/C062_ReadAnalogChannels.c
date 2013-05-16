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

#include "C062_ReadAnalogChannels.h"


uint8_t Compose_C062_ReadAnalogChannels_Req(C062_ReadAnalogChannels_Req* request, 
		BinaryStream* toStream)
{
       #ifdef _DEBUG
	if (request->noOfChannelsNumberCodes == 0 || 4 < request->noOfChannelsNumberCodes)
	return RCS_E01_Undefined1;
#endif
	for (uint8_t i = 0; i < request->noOfChannelsNumberCodes; i++)
	{
		STREAM_WRITE_UINT8(toStream, request->channelsNumberCodes[i]);
	}
	return RCS_N00_Success;
}

uint8_t Parse_C062_ReadAnalogChannels_Req(C062_ReadAnalogChannels_Req* request, BinaryStream* fromStream)
{
	request->noOfChannelsNumberCodes = STREAM_GetRemainingSize(fromStream);
	for (uint8_t i = 0; i < request->noOfChannelsNumberCodes; i++)
	{
		STREAM_READ_UINT8(fromStream, &request->channelsNumberCodes[i]);
	}
	return RCS_N00_Success;
}

uint8_t Compose_C062_ReadAnalogChannels_Resp(C062_ReadAnalogChannels_Resp* response, 
		BinaryStream* toStream)
{
	response->noOfChannelsNumberCodes = C062_RespSize/6;

#ifdef _DEBUG
	if (response->noOfChannelsNumberCodes != 4 )
	return RCS_E01_Undefined1;
#endif
	for (uint8_t i = 0; i < response->noOfChannelsNumberCodes; i++)
	{
		STREAM_WRITE_UINT8(toStream, response->channels[i].channelNumberCode);
		STREAM_WRITE_UINT8(toStream, response->channels[i].unitsCode);
		STREAM_WRITE_FLOAT(toStream, response->channels[i].channelLevel);
	}
	return RCS_N00_Success;
}

uint8_t Parse_C062_ReadAnalogChannels_Resp(C062_ReadAnalogChannels_Resp* response, BinaryStream* fromStream)
{
	response->noOfChannelsNumberCodes = 4; //always
	for (uint8_t i = 0; i < response->noOfChannelsNumberCodes; i++)
	{
		STREAM_READ_UINT8(fromStream, &response->channels[i].channelNumberCode);
		STREAM_READ_UINT8(fromStream, &response->channels[i].unitsCode);
		STREAM_READ_FLOAT(fromStream, &response->channels[i].channelLevel);
	}
	return RCS_N00_Success;
}
