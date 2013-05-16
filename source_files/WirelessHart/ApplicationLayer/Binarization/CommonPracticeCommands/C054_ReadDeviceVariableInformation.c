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

#include "C054_ReadDeviceVariableInformation.h"


uint8_t Compose_C054_ReadDeviceVariableInformation_Req(C054_ReadDeviceVariableInformation_Req* request,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->variableCode);

	return RCS_N00_Success;
}

uint8_t Parse_C054_ReadDeviceVariableInformation_Req(C054_ReadDeviceVariableInformation_Req* request, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->variableCode);

	return RCS_N00_Success;
}

uint8_t Compose_C054_ReadDeviceVariableInformation_Resp(C054_ReadDeviceVariableInformation_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->variableCode);
	STREAM_WRITE_UINT24(toStream, response->serialNo);
	STREAM_WRITE_UINT8(toStream, response->spanUnits);
	STREAM_WRITE_FLOAT(toStream, response->upperTransducerLimit);
	STREAM_WRITE_FLOAT(toStream, response->lowerTransducerLimit);
	STREAM_WRITE_FLOAT(toStream, response->dampingValue);
	STREAM_WRITE_FLOAT(toStream, response->minimumSpan);
	STREAM_WRITE_UINT8(toStream, response->classificationCode);
	STREAM_WRITE_UINT8(toStream, response->familyCode);
	STREAM_WRITE_TIME(toStream, response->updateTime);
	return RCS_N00_Success;
}

uint8_t Parse_C054_ReadDeviceVariableInformation_Resp(C054_ReadDeviceVariableInformation_Resp* response, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->variableCode);
	STREAM_READ_UINT24(fromStream, &response->serialNo);
	STREAM_READ_UINT8(fromStream, &response->spanUnits);
	STREAM_READ_FLOAT(fromStream, &response->upperTransducerLimit);
	STREAM_READ_FLOAT(fromStream, &response->lowerTransducerLimit);
	STREAM_READ_FLOAT(fromStream, &response->dampingValue);
	STREAM_READ_FLOAT(fromStream, &response->minimumSpan);
	STREAM_READ_UINT8(fromStream, &response->classificationCode);
	STREAM_READ_UINT8(fromStream, &response->familyCode);
	STREAM_READ_TIME(fromStream, &response->updateTime);
	return RCS_N00_Success;
}
