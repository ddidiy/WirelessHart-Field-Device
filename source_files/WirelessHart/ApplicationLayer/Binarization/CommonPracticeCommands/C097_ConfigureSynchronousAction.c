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

#include "C097_ConfigureSynchronousAction.h"

uint8_t Compose_C097_ConfigureSynchronousAction_Req(C097_ConfigureSynchronousAction_Req* request,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->actionNo);
	STREAM_WRITE_UINT8(toStream, request->actionControl);
	STREAM_WRITE_UINT8(toStream, request->deviceVariableCode);
	STREAM_WRITE_UINT16(toStream, request->commandNo);
	STREAM_WRITE_DATE(toStream, request->triggerDate);
	STREAM_WRITE_TIME(toStream, request->triggerTime);
	return RCS_N00_Success;
}

uint8_t Parse_C097_ConfigureSynchronousAction_Req(C097_ConfigureSynchronousAction_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->actionNo);
	STREAM_READ_UINT8(fromStream, &request->actionControl);
	STREAM_READ_UINT8(fromStream, &request->deviceVariableCode);
	STREAM_READ_UINT16(fromStream, &request->commandNo);
	STREAM_READ_DATE(fromStream, &request->triggerDate);
	STREAM_READ_TIME(fromStream, &request->triggerTime);
	return RCS_N00_Success;
}

uint8_t Compose_C097_ConfigureSynchronousAction_Resp(C097_ConfigureSynchronousAction_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->actionNo);
	STREAM_WRITE_UINT8(toStream, response->actionControl);
	STREAM_WRITE_UINT8(toStream, response->deviceVariableCode);
	STREAM_WRITE_UINT16(toStream, response->commandNo);
	STREAM_WRITE_DATE(toStream, response->triggerDate);
	STREAM_WRITE_TIME(toStream, response->triggerTime);
	return RCS_N00_Success;
}

uint8_t Parse_C097_ConfigureSynchronousAction_Resp(C097_ConfigureSynchronousAction_Resp* response,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->actionNo);
	STREAM_READ_UINT8(fromStream, &response->actionControl);
	STREAM_READ_UINT8(fromStream, &response->deviceVariableCode);
	STREAM_READ_UINT16(fromStream, &response->commandNo);
	STREAM_READ_DATE(fromStream, &response->triggerDate);
	STREAM_READ_TIME(fromStream, &response->triggerTime);
	return RCS_N00_Success;
}
