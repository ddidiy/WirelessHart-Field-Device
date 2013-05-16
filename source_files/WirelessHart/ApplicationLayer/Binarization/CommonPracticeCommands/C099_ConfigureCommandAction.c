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

#include "C099_ConfigureCommandAction.h"

uint8_t Compose_C099_ConfigureCommandAction_Req(C099_ConfigureCommandAction_Req* request, 
		BinaryStream* toStream)
{
#ifdef _DEBUG
	if (request->commandSize == 0 || MaxEmbeddedCommandsSize < request->commandSize)
	return RCS_E01_Undefined1;
#endif
	STREAM_WRITE_UINT8(toStream, request->triggerAction);
	STREAM_WRITE_UINT16(toStream, request->commandNo);
	STREAM_WRITE_UINT8(toStream, request->commandSize);
	STREAM_WRITE_BYTES(toStream, &request->commandData, request->commandSize);

	return RCS_N00_Success;
}

uint8_t Parse_C099_ConfigureCommandAction_Req(C099_ConfigureCommandAction_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->triggerAction);
	STREAM_READ_UINT16(fromStream, &request->commandNo);
	STREAM_READ_UINT8(fromStream, &request->commandSize);
	STREAM_READ_BYTES(fromStream, &request->commandData, request->commandSize);

	return RCS_N00_Success;
}

uint8_t Compose_C099_ConfigureCommandAction_Resp(C099_ConfigureCommandAction_Resp* response, 
		BinaryStream* toStream)
{
	#ifdef _DEBUG
	if (response->commandSize == 0 || MaxEmbeddedCommandsSize < response->commandSize)
	return RCS_E01_Undefined1;
#endif
	STREAM_WRITE_UINT8(toStream, response->triggerAction);
	STREAM_WRITE_UINT16(toStream, response->commandNo);
	STREAM_WRITE_UINT8(toStream, response->commandSize);
	STREAM_WRITE_BYTES(toStream, &response->commandData, response->commandSize);

	return RCS_N00_Success;
}

uint8_t Parse_C099_ConfigureCommandAction_Resp(C099_ConfigureCommandAction_Resp* response, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->triggerAction);
	STREAM_READ_UINT16(fromStream, &response->commandNo);
	STREAM_READ_UINT8(fromStream, &response->commandSize);
	STREAM_READ_BYTES(fromStream, &response->commandData, response->commandSize);

	return RCS_N00_Success;
}
