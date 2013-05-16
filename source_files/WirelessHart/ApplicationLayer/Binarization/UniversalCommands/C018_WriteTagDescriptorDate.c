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

#include "C018_WriteTagDescriptorDate.h"

uint8_t Compose_C018_WriteTagDescriptorDate_Req(C018_WriteTagDescriptorDate_Req* request,
			 BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, request->tag, sizeof(request->tag));
	STREAM_WRITE_BYTES(toStream, request->descriptorUsedByTheMasterForRecordKeeping,
			sizeof(request->descriptorUsedByTheMasterForRecordKeeping));
	STREAM_WRITE_DATE(toStream, request->dateCodeUsedByTheMasterForRecordKeeping);

	return RCS_N00_Success;
}

uint8_t Parse_C018_WriteTagDescriptorDate_Req(C018_WriteTagDescriptorDate_Req* request,
			 BinaryStream* fromStream)
{
	#if defined(_DEBUG)
		if (STREAM_GetRemainingSize(fromStream) < C018_ReqSize)
			return RCS_E05_TooFewDataBytesReceived;
	#endif

	STREAM_READ_BYTES(fromStream, request->tag, sizeof(request->tag));
	STREAM_READ_BYTES(fromStream, request->descriptorUsedByTheMasterForRecordKeeping,
			sizeof(request->descriptorUsedByTheMasterForRecordKeeping));
	STREAM_READ_DATE(fromStream, &request->dateCodeUsedByTheMasterForRecordKeeping);

	return RCS_N00_Success; //success
}

uint8_t Compose_C018_WriteTagDescriptorDate_Resp(C018_WriteTagDescriptorDate_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, response->tag, sizeof(response->tag));
	STREAM_WRITE_BYTES(toStream, response->descriptorUsedByTheMasterForRecordKeeping,
			sizeof(response->descriptorUsedByTheMasterForRecordKeeping));
	STREAM_WRITE_DATE(toStream, response->dateCodeUsedByTheMasterForRecordKeeping);

	return RCS_N00_Success;
}

uint8_t Parse_C018_WriteTagDescriptorDate_Resp(C018_WriteTagDescriptorDate_Resp* response,
		 BinaryStream* fromStream)
{

	#if defined(_DEBUG)
		if (STREAM_GetRemainingSize(fromStream) < C018_RespSize)
			return RCS_E05_TooFewDataBytesReceived;
	#endif

	STREAM_READ_BYTES(fromStream, response->tag, sizeof(response->tag));
	STREAM_READ_BYTES(fromStream, response->descriptorUsedByTheMasterForRecordKeeping,
			sizeof(response->descriptorUsedByTheMasterForRecordKeeping));
	STREAM_READ_DATE(fromStream, &response->dateCodeUsedByTheMasterForRecordKeeping);

	return RCS_N00_Success; //success
}

