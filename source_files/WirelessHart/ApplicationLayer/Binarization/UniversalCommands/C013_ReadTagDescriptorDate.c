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

#include "C013_ReadTagDescriptorDate.h"

uint8_t Compose_C013_ReadTagDescriptorDate_Req(C013_ReadTagDescriptorDate_Req* request,
			 BinaryStream* toStream)
{
	return RCS_N00_Success;
}

uint8_t Parse_C013_ReadTagDescriptorDate_Req(C013_ReadTagDescriptorDate_Req* request,
			 BinaryStream* fromStream)
{
	return RCS_N00_Success; //success
}

uint8_t Compose_C013_ReadTagDescriptorDate_Resp(C013_ReadTagDescriptorDate_Resp* response,
		 BinaryStream* toStream)
{
//	STREAM_WRITE_PACKED(toStream, response->tag, sizeof(response->tag));
//	STREAM_WRITE_PACKED(toStream, response->descriptor, sizeof(response->descriptor));
	STREAM_WRITE_BYTES(toStream, response->tag, sizeof(response->tag));
	STREAM_WRITE_BYTES(toStream, response->descriptor, sizeof(response->descriptor));
	STREAM_WRITE_DATE(toStream, response->dateCode);

	return RCS_N00_Success; //success
}

uint8_t Parse_C013_ReadTagDescriptorDate_Resp(C013_ReadTagDescriptorDate_Resp* response,
		 BinaryStream* fromStream)
{

	#if defined(_DEBUG)
		if (STREAM_GetRemainingSize(fromStream) < C013_RespSize)
			return RCS_E05_TooFewDataBytesReceived;
	#endif

//	STREAM_READ_PACKED(fromStream, response->tag, sizeof(response->tag));
//	STREAM_READ_PACKED(fromStream, response->descriptor, sizeof(response->descriptor));
	STREAM_READ_BYTES(fromStream, response->tag, sizeof(response->tag));
	STREAM_READ_BYTES(fromStream, response->descriptor, sizeof(response->descriptor));
	STREAM_READ_DATE(fromStream, &response->dateCode);

	return RCS_N00_Success; //success
}

