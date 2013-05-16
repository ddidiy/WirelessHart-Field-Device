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

#include "C075_PollSubDevice.h"
#include "../UniversalCommands/C000_ReadUniqueIdentifier.h"

uint8_t Compose_C075_PollSubDevice_Req(C075_PollSubDevice_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->ioCard);
	STREAM_WRITE_UINT8(toStream, request->channel);
	STREAM_WRITE_UINT8(toStream, request->pollingAddress);

	return RCS_N00_Success;
}

uint8_t Parse_C075_PollSubDevice_Req(C075_PollSubDevice_Req* request, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->ioCard);
	STREAM_READ_UINT8(fromStream, &request->channel);
	STREAM_READ_UINT8(fromStream, &request->pollingAddress);
	return RCS_N00_Success;
}

uint8_t Compose_C075_PollSubDevice_Resp(C075_PollSubDevice_Resp* response, 
		BinaryStream* toStream)
{
	uint8_t retValue = Compose_C000_ReadUniqueIdentifier_Resp((C000_ReadUniqueIdentifier_Resp*) response, toStream);
	return retValue;
}

uint8_t Parse_C075_PollSubDevice_Resp(C075_PollSubDevice_Resp* response, BinaryStream* fromStream)
{
	return Parse_C000_ReadUniqueIdentifier_Resp((C000_ReadUniqueIdentifier_Resp*) response, fromStream);
}
