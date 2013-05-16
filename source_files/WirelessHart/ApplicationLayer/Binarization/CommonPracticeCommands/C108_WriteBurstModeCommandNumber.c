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

#include "C108_WriteBurstModeCommandNumber.h"

#define C108_NO_BACK_REQ_SIZE 3

uint8_t Compose_C108_WriteBurstModeCommandNumber_Req(C108_WriteBurstModeCommandNumber_Req* request,
		 BinaryStream* toStream)
{
	
	if(request->backComp)
	{
          STREAM_WRITE_UINT8(toStream, request->commandNo);
        }
        else
        {
          STREAM_WRITE_UINT16(toStream, request->commandNo);
          STREAM_WRITE_UINT8(toStream, request->burstMessage);
        }
	return RCS_N00_Success;
}

uint8_t Parse_C108_WriteBurstModeCommandNumber_Req(C108_WriteBurstModeCommandNumber_Req* request,
		 BinaryStream* fromStream)
{
	if(STREAM_GetRemainingSize(fromStream) >= C108_NO_BACK_REQ_SIZE)
	{
          request->backComp = 0;
          STREAM_READ_UINT16(fromStream, &request->commandNo);
          STREAM_READ_UINT8(fromStream, &request->burstMessage);
	}
	else
        {
          request->backComp = 1;
          uint8_t ucCommandNo;
          STREAM_READ_UINT8(fromStream, &ucCommandNo);
          request->commandNo = ucCommandNo;
          request->burstMessage = 0;
          
        }
	return RCS_N00_Success;
}

uint8_t Compose_C108_WriteBurstModeCommandNumber_Resp(C108_WriteBurstModeCommandNumber_Resp* response,
		 BinaryStream* toStream)
{
        if(response->backComp)
        {
          STREAM_WRITE_UINT8(toStream, response->commandNo); 
        }
        else
        {
          STREAM_WRITE_UINT16(toStream, response->commandNo);
          STREAM_WRITE_UINT8(toStream, response->burstMessage);          
        }

	return RCS_N00_Success;
}

uint8_t Parse_C108_WriteBurstModeCommandNumber_Resp(C108_WriteBurstModeCommandNumber_Resp* response,
		 BinaryStream* fromStream)
{
	if(STREAM_GetRemainingSize(fromStream) >= C108_NO_BACK_REQ_SIZE)
	{
          response->backComp = 0;
          STREAM_READ_UINT16(fromStream, &response->commandNo);
          STREAM_READ_UINT8(fromStream, &response->burstMessage);          
        }
        else
        {
          response->backComp = 1;
          uint8_t ucCommandNo;
          STREAM_READ_UINT8(fromStream, &ucCommandNo);
          response->commandNo = ucCommandNo;
          response->burstMessage = 0;
        }
	return RCS_N00_Success;
}
