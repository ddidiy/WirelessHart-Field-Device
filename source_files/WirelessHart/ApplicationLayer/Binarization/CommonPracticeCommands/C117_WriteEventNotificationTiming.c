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

#include "C117_WriteEventNotificationTiming.h"

uint8_t Compose_C117_WriteEventNotificationTiming_Req(C117_WriteEventNotificationTiming_Req* request,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->eventNo);
	STREAM_WRITE_TIME(toStream, request->eventNotificationRetryTime);
	STREAM_WRITE_TIME(toStream, request->maximumUpdateTime);
	STREAM_WRITE_TIME(toStream, request->eventDeBounceInterval);
	return RCS_N00_Success;
}

uint8_t Parse_C117_WriteEventNotificationTiming_Req(C117_WriteEventNotificationTiming_Req* request,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->eventNo);
	STREAM_READ_TIME(fromStream, &request->eventNotificationRetryTime);
	STREAM_READ_TIME(fromStream, &request->maximumUpdateTime);
	STREAM_READ_TIME(fromStream, &request->eventDeBounceInterval);
	return RCS_N00_Success;
}

uint8_t Compose_C117_WriteEventNotificationTiming_Resp(C117_WriteEventNotificationTiming_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->eventNo);
	STREAM_WRITE_TIME(toStream, response->eventNotificationRetryTime);
	STREAM_WRITE_TIME(toStream, response->maximumUpdateTime);
	STREAM_WRITE_TIME(toStream, response->eventDeBounceInterval);
	return RCS_N00_Success;
}

uint8_t Parse_C117_WriteEventNotificationTiming_Resp(C117_WriteEventNotificationTiming_Resp* response,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->eventNo);
	STREAM_READ_TIME(fromStream, &response->eventNotificationRetryTime);
	STREAM_READ_TIME(fromStream, &response->maximumUpdateTime);
	STREAM_READ_TIME(fromStream, &response->eventDeBounceInterval);
	return RCS_N00_Success;
}
