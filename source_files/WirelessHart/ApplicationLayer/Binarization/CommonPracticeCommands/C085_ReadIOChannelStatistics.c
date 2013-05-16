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

#include "C085_ReadIOChannelStatistics.h"

uint8_t Compose_C085_ReadIOChannelStatistics_Req(C085_ReadIOChannelStatistics_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->ioCard);
	STREAM_WRITE_UINT8(toStream, request->channel);
	return RCS_N00_Success;
}

uint8_t Parse_C085_ReadIOChannelStatistics_Req(C085_ReadIOChannelStatistics_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->ioCard);
	STREAM_READ_UINT8(fromStream, &request->channel);

	return RCS_N00_Success;
}

uint8_t Compose_C085_ReadIOChannelStatistics_Resp(C085_ReadIOChannelStatistics_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->ioCard);
	STREAM_WRITE_UINT8(toStream, response->channel);
	STREAM_WRITE_UINT16(toStream, response->stxMessagesSentCount);
	STREAM_WRITE_UINT16(toStream, response->ackMessagesReceivedCount);
	STREAM_WRITE_UINT16(toStream, response->ostxMessagesReceivedCount);
	STREAM_WRITE_UINT16(toStream, response->oackMessagesReceivedCount);
	STREAM_WRITE_UINT16(toStream, response->backMessagesReceivedCount);

	return RCS_N00_Success;
}

uint8_t Parse_C085_ReadIOChannelStatistics_Resp(C085_ReadIOChannelStatistics_Resp* response, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->ioCard);
	STREAM_READ_UINT8(fromStream, &response->channel);
	STREAM_READ_UINT16(fromStream, &response->stxMessagesSentCount);
	STREAM_READ_UINT16(fromStream, &response->ackMessagesReceivedCount);
	STREAM_READ_UINT16(fromStream, &response->ostxMessagesReceivedCount);
	STREAM_READ_UINT16(fromStream, &response->oackMessagesReceivedCount);
	STREAM_READ_UINT16(fromStream, &response->backMessagesReceivedCount);

	return RCS_N00_Success;
}
