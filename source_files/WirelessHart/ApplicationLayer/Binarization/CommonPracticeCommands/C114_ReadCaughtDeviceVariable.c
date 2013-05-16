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

#include "C114_ReadCaughtDeviceVariable.h"

uint8_t Compose_C114_ReadCaughtDeviceVariable_Req(C114_ReadCaughtDeviceVariable_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->destinationDeviceVariableCode);
	return RCS_N00_Success;
}

uint8_t Parse_C114_ReadCaughtDeviceVariable_Req(C114_ReadCaughtDeviceVariable_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->destinationDeviceVariableCode);
	return RCS_N00_Success;
}

uint8_t Compose_C114_ReadCaughtDeviceVariable_Resp(C114_ReadCaughtDeviceVariable_Resp* response,
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

uint8_t Parse_C114_ReadCaughtDeviceVariable_Resp(C114_ReadCaughtDeviceVariable_Resp* response, 
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
