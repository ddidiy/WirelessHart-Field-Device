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

#include "C081_ReadDeviceVariableTrimGuidelines.h"

uint8_t Compose_C081_ReadDeviceVariableTrimGuidelines_Req(C081_ReadDeviceVariableTrimGuidelines_Req* request,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->deviceVariableCode);

	return RCS_N00_Success;
}

uint8_t Parse_C081_ReadDeviceVariableTrimGuidelines_Req(C081_ReadDeviceVariableTrimGuidelines_Req* request, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->deviceVariableCode);
	return RCS_N00_Success;
}

uint8_t Compose_C081_ReadDeviceVariableTrimGuidelines_Resp(C081_ReadDeviceVariableTrimGuidelines_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->deviceVariableCode);
	STREAM_WRITE_UINT8(toStream, response->trimPointsSupported);
	STREAM_WRITE_UINT8(toStream, response->trimUnitsCode);
	STREAM_WRITE_FLOAT(toStream, response->minLowerTrimPoint);
	STREAM_WRITE_FLOAT(toStream, response->maxLowerTrimPoint);
	STREAM_WRITE_FLOAT(toStream, response->minUpperTrimPoint);
	STREAM_WRITE_FLOAT(toStream, response->maxUpperTrimPoint);
	STREAM_WRITE_FLOAT(toStream, response->minDifferential);

	return RCS_N00_Success;
}

uint8_t Parse_C081_ReadDeviceVariableTrimGuidelines_Resp(C081_ReadDeviceVariableTrimGuidelines_Resp* response, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->deviceVariableCode);
	STREAM_READ_UINT8(fromStream, &response->trimPointsSupported);
	STREAM_READ_UINT8(fromStream, &response->trimUnitsCode);
	STREAM_READ_FLOAT(fromStream, &response->minLowerTrimPoint);
	STREAM_READ_FLOAT(fromStream, &response->maxLowerTrimPoint);
	STREAM_READ_FLOAT(fromStream, &response->minUpperTrimPoint);
	STREAM_READ_FLOAT(fromStream, &response->maxUpperTrimPoint);
	STREAM_READ_FLOAT(fromStream, &response->minDifferential);

	return RCS_N00_Success;
}
