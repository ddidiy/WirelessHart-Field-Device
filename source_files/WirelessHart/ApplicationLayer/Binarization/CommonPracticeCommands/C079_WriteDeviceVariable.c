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

#include "C079_WriteDeviceVariable.h"

uint8_t Compose_C079_WriteDeviceVariable_Req(C079_WriteDeviceVariable_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->deviceVariableCode);
	STREAM_WRITE_UINT8(toStream, request->writeCommandCode);
	STREAM_WRITE_UINT8(toStream, request->unitsCode);
	STREAM_WRITE_FLOAT(toStream, request->value);
	STREAM_WRITE_UINT8(toStream, request->deviceVariableStatus);
	return RCS_N00_Success;
}

uint8_t Parse_C079_WriteDeviceVariable_Req(C079_WriteDeviceVariable_Req* request, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->deviceVariableCode);
	STREAM_READ_UINT8(fromStream, &request->writeCommandCode);
	STREAM_READ_UINT8(fromStream, &request->unitsCode);
	STREAM_READ_FLOAT(fromStream, &request->value);
	STREAM_READ_UINT8(fromStream, &request->deviceVariableStatus);

	return RCS_N00_Success;
}

uint8_t Compose_C079_WriteDeviceVariable_Resp(C079_WriteDeviceVariable_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->deviceVariableCode);
	STREAM_WRITE_UINT8(toStream, response->writeCommandCode);
	STREAM_WRITE_UINT8(toStream, response->unitsCode);
	STREAM_WRITE_FLOAT(toStream, response->value);
	STREAM_WRITE_UINT8(toStream, response->deviceVariableStatus);

	return RCS_N00_Success;
}

uint8_t Parse_C079_WriteDeviceVariable_Resp(C079_WriteDeviceVariable_Resp* response, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->deviceVariableCode);
	STREAM_READ_UINT8(fromStream, &response->writeCommandCode);
	STREAM_READ_UINT8(fromStream, &response->unitsCode);
	STREAM_READ_FLOAT(fromStream, &response->value);
	STREAM_READ_UINT8(fromStream, &response->deviceVariableStatus);

	return RCS_N00_Success;
}
