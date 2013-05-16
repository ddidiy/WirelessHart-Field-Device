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

#include "C098_ReadCommandAction.h"

uint8_t Compose_C098_ReadCommandAction_Req(C098_ReadCommandAction_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->triggerAction);

	return RCS_N00_Success;
}

uint8_t Parse_C098_ReadCommandAction_Req(C098_ReadCommandAction_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->triggerAction);

	return RCS_N00_Success;
}

uint8_t Compose_C098_ReadCommandAction_Resp(C098_ReadCommandAction_Resp* response, 
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

uint8_t Parse_C098_ReadCommandAction_Resp(C098_ReadCommandAction_Resp* response, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->triggerAction);
	STREAM_READ_UINT16(fromStream, &response->commandNo);
	STREAM_READ_UINT8(fromStream, &response->commandSize);
	STREAM_READ_BYTES(fromStream, &response->commandData, response->commandSize);

	return RCS_N00_Success;
}
