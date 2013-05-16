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

#include "C113_CatchDeviceVariable.h"

uint8_t Compose_C113_CatchDeviceVariable_Req(C113_CatchDeviceVariable_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->destinationDeviceVariableCode);
	STREAM_WRITE_UINT8(toStream, request->captureModeCode);
	STREAM_WRITE_UINT16(toStream, request->sourceSlaveExpandedDeviceTypeCode);
	STREAM_WRITE_UINT24(toStream, request->sourceSlaveDeviceID);
	STREAM_WRITE_UINT8(toStream, request->sourceCommandNo);
	STREAM_WRITE_UINT8(toStream, request->sourceSlotNo);
	STREAM_WRITE_FLOAT(toStream, request->shedTime);
	return RCS_N00_Success;
}

uint8_t Parse_C113_CatchDeviceVariable_Req(C113_CatchDeviceVariable_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->destinationDeviceVariableCode);
	STREAM_READ_UINT8(fromStream, &request->captureModeCode);
	STREAM_READ_UINT16(fromStream, &request->sourceSlaveExpandedDeviceTypeCode);
	STREAM_READ_UINT24(fromStream, &request->sourceSlaveDeviceID);
	STREAM_READ_UINT8(fromStream, &request->sourceCommandNo);
	STREAM_READ_UINT8(fromStream, &request->sourceSlotNo);
	STREAM_READ_FLOAT(fromStream, &request->shedTime);
	return RCS_N00_Success;
}

uint8_t Compose_C113_CatchDeviceVariable_Resp(C113_CatchDeviceVariable_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->destinationDeviceVariableCode);
	STREAM_WRITE_UINT8(toStream, response->captureModeCode);
	STREAM_WRITE_BYTES(toStream, &response->sourceSlaveAddress, 5);
	STREAM_WRITE_UINT8(toStream, response->sourceCommandNo);
	STREAM_WRITE_UINT8(toStream, response->sourceSlotNo);
	STREAM_WRITE_FLOAT(toStream, response->shedTime);
	return RCS_N00_Success;
}

uint8_t Parse_C113_CatchDeviceVariable_Resp(C113_CatchDeviceVariable_Resp* response, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->destinationDeviceVariableCode);
	STREAM_READ_UINT8(fromStream, &response->captureModeCode);
	STREAM_READ_BYTES(fromStream, &response->sourceSlaveAddress, 5);
	STREAM_READ_UINT8(fromStream, &response->sourceCommandNo);
	STREAM_READ_UINT8(fromStream, &response->sourceSlotNo);
	STREAM_READ_FLOAT(fromStream, &response->shedTime);
	return RCS_N00_Success;
}
