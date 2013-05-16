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

#include "C105_ReadBurstModeConfiguration.h"


uint8_t Compose_C105_ReadBurstModeConfiguration_Req(C105_ReadBurstModeConfiguration_Req* request,
		 BinaryStream* toStream)
{
	if(!request->requestIsEmpty)
	{
		STREAM_WRITE_UINT8(toStream, request->burstMessage);
	}
	return RCS_N00_Success;
}

uint8_t Parse_C105_ReadBurstModeConfiguration_Req(C105_ReadBurstModeConfiguration_Req* request, 
		BinaryStream* fromStream)
{
	if(!STREAM_GetRemainingSize(fromStream))
	{
	    request->burstMessage = 0;
            request->requestIsEmpty = 1;
	}
	else
	{
	    STREAM_READ_UINT8(fromStream, &request->burstMessage);
            request->requestIsEmpty = 0;
	}

	return RCS_N00_Success;
}

uint8_t Compose_C105_ReadBurstModeConfiguration_Resp(C105_ReadBurstModeConfiguration_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->burstMode);
        STREAM_WRITE_UINT8(toStream, response->commandNo);
        
	for (uint8_t i = 0; i < MaxNoOfDeviceVariables; i++)
	{
		STREAM_WRITE_UINT8(toStream, response->deviceVariableCode[i]);
	}
	STREAM_WRITE_UINT8(toStream, response->burstMessage);
	STREAM_WRITE_UINT8(toStream, response->maxBurstMessagesSupported);
        STREAM_WRITE_UINT16(toStream, response->extendedCommandNumber);
	STREAM_WRITE_TIME(toStream, response->updateTime);
	STREAM_WRITE_TIME(toStream, response->maxUpdateTime);
	STREAM_WRITE_UINT8(toStream, response->burstTriggerMode);
	STREAM_WRITE_UINT8(toStream, response->classificationCode);
	STREAM_WRITE_UINT8(toStream, response->unitsCode);
	STREAM_WRITE_FLOAT(toStream, response->triggerValue);

	return RCS_N00_Success;
}

uint8_t Parse_C105_ReadBurstModeConfiguration_Resp(C105_ReadBurstModeConfiguration_Resp* response,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->burstMode);
	STREAM_READ_UINT8(fromStream, &response->commandNo); //might be 0x1F cmd no expansion flag
	for (uint8_t i = 0; i < MaxNoOfDeviceVariables; i++)
	{
		STREAM_READ_UINT8(fromStream, &response->deviceVariableCode[i]);
	}
	STREAM_READ_UINT8(fromStream, &response->burstMessage);
	STREAM_READ_UINT8(fromStream, &response->maxBurstMessagesSupported);
        STREAM_READ_UINT16(fromStream, &response->extendedCommandNumber);
	STREAM_READ_TIME(fromStream, &response->updateTime);
	STREAM_READ_TIME(fromStream, &response->maxUpdateTime);
	STREAM_READ_UINT8(fromStream, &response->burstTriggerMode);
	STREAM_READ_UINT8(fromStream, &response->classificationCode);
	STREAM_READ_UINT8(fromStream, &response->unitsCode);
	STREAM_READ_FLOAT(fromStream, &response->triggerValue);

	return RCS_N00_Success;
}
