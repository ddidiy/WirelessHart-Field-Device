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

#include "C104_WriteBurstTrigger.h"

uint8_t Compose_C104_WriteBurstTrigger_Req(C104_WriteBurstTrigger_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->burstMessage);
	STREAM_WRITE_UINT8(toStream, request->selectionCode);
	STREAM_WRITE_UINT8(toStream, request->classificationCode);
	STREAM_WRITE_UINT8(toStream, request->unitsCode);
	STREAM_WRITE_FLOAT(toStream, request->triggerLevel);
	return RCS_N00_Success;
}

uint8_t Parse_C104_WriteBurstTrigger_Req(C104_WriteBurstTrigger_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->burstMessage);
	STREAM_READ_UINT8(fromStream, &request->selectionCode);
	STREAM_READ_UINT8(fromStream, &request->classificationCode);
	STREAM_READ_UINT8(fromStream, &request->unitsCode);
	STREAM_READ_FLOAT(fromStream, &request->triggerLevel);

	return RCS_N00_Success;
}

uint8_t Compose_C104_WriteBurstTrigger_Resp(C104_WriteBurstTrigger_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->burstMessage);
	STREAM_WRITE_UINT8(toStream, response->selectionCode);
	STREAM_WRITE_UINT8(toStream, response->classificationCode);
	STREAM_WRITE_UINT8(toStream, response->unitsCode);
	STREAM_WRITE_FLOAT(toStream, response->triggerLevel);
	return RCS_N00_Success;
}

uint8_t Parse_C104_WriteBurstTrigger_Resp(C104_WriteBurstTrigger_Resp* response, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->burstMessage);
	STREAM_READ_UINT8(fromStream, &response->selectionCode);
	STREAM_READ_UINT8(fromStream, &response->classificationCode);
	STREAM_READ_UINT8(fromStream, &response->unitsCode);
	STREAM_READ_FLOAT(fromStream, &response->triggerLevel);

	return RCS_N00_Success;
}
