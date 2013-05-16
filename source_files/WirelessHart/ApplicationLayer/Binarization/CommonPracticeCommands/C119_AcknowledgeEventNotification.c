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

#include "C119_AcknowledgeEventNotification.h"
#include "../../../provision.h"

uint8_t Compose_C119_AcknowledgeEventNotification_Req(C119_AcknowledgeEventNotification_Req* request,
		 BinaryStream* toStream)
{
	//TODO:[Ovidiu] - TRUNCABLE !!! This command may be issued with only the event number in the request data.
	STREAM_WRITE_UINT8(toStream, request->eventNo);
        STREAM_WRITE_TIME(toStream, request->firstUnacknowledged);
        STREAM_WRITE_UINT16(toStream, request->configurationChangedCounter);
        STREAM_WRITE_UINT8(toStream, request->deviceStatus);
	for (uint8_t i = 0; i < g_stEventsTableNvData[request->eventNo].m_ucMaskSize - 1; i++)
	{
		STREAM_WRITE_UINT8(toStream, request->command48Data[i]);
	}
	return RCS_N00_Success;
}

uint8_t Parse_C119_AcknowledgeEventNotification_Req(C119_AcknowledgeEventNotification_Req* request,
		 BinaryStream* fromStream)
{
	//	if (STREAM_GetRemainingSize(fromStream) < C117_CommandSize)
	//	{
	//		return RCS_E05_TooFewDataBytesReceived;
	//	}
	STREAM_READ_UINT8(fromStream, &request->eventNo);
        STREAM_READ_TIME(fromStream, &request->firstUnacknowledged);
        STREAM_READ_UINT16(fromStream, &request->configurationChangedCounter);
        STREAM_READ_UINT8(fromStream, &request->deviceStatus);
	for (uint8_t i = 0; i < g_stEventsTableNvData[request->eventNo].m_ucMaskSize - 1; i++)
	{
		STREAM_READ_UINT8(fromStream, &request->command48Data[i]);
	}
	return RCS_N00_Success;
}

uint8_t Compose_C119_AcknowledgeEventNotification_Resp(C119_AcknowledgeEventNotification_Resp* response,
		 BinaryStream* toStream)
{
	//TODO:[Ovidiu] - add validation for no of events.Response is tuncable

	STREAM_WRITE_UINT8(toStream, response->eventNo);
        STREAM_WRITE_TIME(toStream, response->firstUnacknowledged);
        STREAM_WRITE_UINT16(toStream, response->configurationChangedCounter);
        STREAM_WRITE_UINT8(toStream, response->deviceStatus);
	for (uint8_t i = 0; i < MaxLengthOf48Command; i++)
	{
		STREAM_WRITE_UINT8(toStream, response->command48Data[i]);
	}
	return RCS_N00_Success;
}

uint8_t Parse_C119_AcknowledgeEventNotification_Resp(C119_AcknowledgeEventNotification_Resp* response,
		 BinaryStream* fromStream)
{
	//	if (STREAM_GetRemainingSize(fromStream) < C117_CommandSize)
	//	{
	//		return RCS_E05_TooFewDataBytesReceived;
	//	}
	STREAM_READ_UINT8(fromStream, &response->eventNo);
        STREAM_READ_TIME(fromStream, &response->firstUnacknowledged);
        STREAM_READ_UINT16(fromStream, &response->configurationChangedCounter);
        STREAM_READ_UINT8(fromStream, &response->deviceStatus);
	for (uint8_t i = 0; i < MaxLengthOf48Command; i++)
	{
		STREAM_READ_UINT8(fromStream, &response->command48Data[i]);
	}
	return RCS_N00_Success;
}
