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

#include "C053_WriteDeviceVariableUnits.h"

uint8_t Compose_C053_WriteDeviceVariableUnits_Req(C053_WriteDeviceVariableUnits_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->variable);
	STREAM_WRITE_UINT8(toStream, request->unitsCode);

	return RCS_N00_Success;
}

uint8_t Parse_C053_WriteDeviceVariableUnits_Req(C053_WriteDeviceVariableUnits_Req* request, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->variable);
	STREAM_READ_UINT8(fromStream, &request->unitsCode);

	return RCS_N00_Success;
}

uint8_t Compose_C053_WriteDeviceVariableUnits_Resp(C053_WriteDeviceVariableUnits_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->variable);
	STREAM_WRITE_UINT8(toStream, response->unitsCode);

	return RCS_N00_Success;
}

uint8_t Parse_C053_WriteDeviceVariableUnits_Resp(C053_WriteDeviceVariableUnits_Resp* response, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->variable);
	STREAM_READ_UINT8(fromStream, &response->unitsCode);

	return RCS_N00_Success;
}
