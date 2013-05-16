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

#include "C078_ReadAggregatedCommands.h"

uint8_t Compose_C078_ReadAggregatedCommands_Req(C078_ReadAggregatedCommands_Req* request, 
		BinaryStream* toStream)
{
	#ifdef _DEBUG
	if (request->noOfCommands == 0 )
	return RCS_E01_Undefined1;
#endif

	STREAM_WRITE_UINT8(toStream, request->noOfCommands);
        STREAM_WRITE_BYTES(toStream, request->cmdData, request->cmdSize);
        /*
	for (uint8_t i = 0; i < request->noOfCommands; i++)
	{
		STREAM_WRITE_UINT16(toStream, request->commands[i].CmdNo);
		STREAM_WRITE_UINT8(toStream, request->commands[i].CmdSize);
		STREAM_WRITE_BYTES(toStream, &request->commands[i].CmdData[0], request->commands[i].CmdSize);
	}
        */
	return RCS_N00_Success;
}

uint8_t Parse_C078_ReadAggregatedCommands_Req(C078_ReadAggregatedCommands_Req* request, BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->noOfCommands);
        request->cmdSize = STREAM_GetRemainingSize(fromStream);
        if(request->cmdSize)
          STREAM_READ_BYTES(fromStream, request->cmdData, request->cmdSize);
        /*
	for (uint8_t i = 0; i < request->noOfCommands; i++)
	{
                STREAM_READ_UINT16(fromStream, &request->commands[i].CmdNo);
		STREAM_READ_UINT8(fromStream, &request->commands[i].CmdSize);
		STREAM_READ_BYTES(fromStream, &request->commands[i].CmdData[0], request->commands[i].CmdSize);
	}
        */
	return RCS_N00_Success;
}

uint8_t Compose_C078_ReadAggregatedCommands_Resp(C078_ReadAggregatedCommands_Resp* response, 
		BinaryStream* toStream)
{
	#ifdef _DEBUG
	if (response->noOfCommands == 0 )
	return RCS_E01_Undefined1;
#endif
        STREAM_WRITE_UINT8(toStream, response->extendedDeviceStatus);
	STREAM_WRITE_UINT8(toStream, response->noOfCommands);
        if(response->cmdSize)
          STREAM_WRITE_BYTES(toStream, &response->cmdData, response->cmdSize);
        /*
	for (uint8_t i = 0; i < response->noOfCommands; i++)
	{
                STREAM_WRITE_UINT16(toStream, response->commands[i].CmdNo);
		STREAM_WRITE_UINT8(toStream, response->commands[i].CmdSize);
		STREAM_WRITE_BYTES(toStream, &response->commands[i].CmdData[0], response->commands[i].CmdSize);
	}
        */
	return RCS_N00_Success;
}

uint8_t Parse_C078_ReadAggregatedCommands_Resp(C078_ReadAggregatedCommands_Resp* response, BinaryStream* fromStream)
{

        STREAM_READ_UINT8(fromStream, &response->extendedDeviceStatus);
	STREAM_READ_UINT8(fromStream, &response->noOfCommands);
        response->cmdSize = STREAM_GetRemainingSize(fromStream);
        STREAM_READ_BYTES(fromStream, &response->cmdData, response->cmdSize);
        /*
	for (uint8_t i = 0; i < response->noOfCommands; i++)
	{
                STREAM_READ_UINT16(fromStream, &response->commands[i].CmdNo);
		STREAM_READ_UINT8(fromStream, &response->commands[i].CmdSize);
		STREAM_READ_BYTES(fromStream, &response->commands[i].CmdData[0], response->commands[i].CmdSize);
	}
        */
	return RCS_N00_Success;
}
