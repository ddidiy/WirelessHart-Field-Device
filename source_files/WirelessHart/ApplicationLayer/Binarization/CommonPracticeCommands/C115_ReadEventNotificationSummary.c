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

#include "C115_ReadEventNotificationSummary.h"

uint8_t Compose_C115_ReadEventNotificationSummary_Req(C115_ReadEventNotificationSummary_Req* request,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->eventNo);
	return RCS_N00_Success;
}

uint8_t Parse_C115_ReadEventNotificationSummary_Req(C115_ReadEventNotificationSummary_Req* request,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->eventNo);
	return RCS_N00_Success;
}

uint8_t Compose_C115_ReadEventNotificationSummary_Resp(C115_ReadEventNotificationSummary_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->eventNo);
	STREAM_WRITE_UINT8(toStream, response->noOfEventsSupported);
	uint8_t eventData = 0;
	eventData = eventData | response->eventStatus;

	eventData = eventData << 4;
	eventData = eventData | response->eventNotificationControlCode;
	//TODO:[Ovidiu]-add range validations for eventStatus and eventNotificationControlCode values
	STREAM_WRITE_UINT8(toStream, eventData);
	STREAM_WRITE_TIME(toStream, response->firstUnacknowledged);
	STREAM_WRITE_TIME(toStream, response->eventNotificationRetryTime);
	STREAM_WRITE_TIME(toStream, response->maximumUpdateTime);
	STREAM_WRITE_TIME(toStream, response->eventDeBounceInterval);
	//TODO:[Ovidiu] - add validation for no of events.Response is tuncable
	for (uint8_t i = 0; i < response->noOfReadEvents; i++)
	{
		STREAM_WRITE_UINT8(toStream, response->eventMask[i]);
	}
	return RCS_N00_Success;
}

uint8_t Parse_C115_ReadEventNotificationSummary_Resp(C115_ReadEventNotificationSummary_Resp* response,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->eventNo);
	STREAM_READ_UINT8(fromStream, &response->noOfEventsSupported);
	uint8_t eventData = 0;
	STREAM_READ_UINT8(fromStream, &eventData);
	response->eventStatus = eventData&0xF0;
	response->eventNotificationControlCode = eventData & 0x0F;
	STREAM_READ_TIME(fromStream, &response->firstUnacknowledged);
	STREAM_READ_TIME(fromStream, &response->eventNotificationRetryTime);
	STREAM_READ_TIME(fromStream, &response->maximumUpdateTime);
	STREAM_READ_TIME(fromStream, &response->eventDeBounceInterval);
	//TODO:[Ovidiu] - add validation for no of events
	response->noOfReadEvents= STREAM_GetRemainingSize(fromStream);
	for (uint8_t i = 0; i < response->noOfReadEvents; i++)
	{
		STREAM_READ_UINT8(fromStream, &response->eventMask[i]);
	}
	return RCS_N00_Success;
}
