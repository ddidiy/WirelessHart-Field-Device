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

#include "C006_WritePollingAddress.h"

uint8_t Compose_C006_WritePollingAddress_Req(C006_WritePollingAddress_Req* request, 
		BinaryStream* toStream)
{
  uint8_t ucLoopMode = request->loopCurrentModeCode;
	STREAM_WRITE_UINT8(toStream, request->pollingAddressDevice);
  if(request->ucBackwardCompatMode)
    ucLoopMode = request->pollingAddressDevice ? LoopCurrentModeCodes_Disabled : LoopCurrentModeCodes_Enabled;

	STREAM_WRITE_UINT8(toStream, ucLoopMode);

	return RCS_N00_Success;
}

uint8_t Parse_C006_WritePollingAddress_Req(C006_WritePollingAddress_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &request->pollingAddressDevice);
  if(STREAM_GetRemainingSize(fromStream) < 1)
  { // backward compatibility mode
    request->loopCurrentModeCode = request->pollingAddressDevice ? LoopCurrentModeCodes_Disabled : LoopCurrentModeCodes_Enabled;
    request->ucBackwardCompatMode = 1;
  }
  else
  {
	  STREAM_READ_UINT8(fromStream, &request->loopCurrentModeCode);
    request->ucBackwardCompatMode = 0;
  }

	return RCS_N00_Success;
}

uint8_t Compose_C006_WritePollingAddress_Resp(C006_WritePollingAddress_Resp* response, 
		BinaryStream* toStream)
{ // backward compatibility mode doesn't matter
	STREAM_WRITE_UINT8(toStream, response->pollingAddressDevice);

	STREAM_WRITE_UINT8(toStream, response->loopCurrentModeCode);

	return RCS_N00_Success;
}

uint8_t Parse_C006_WritePollingAddress_Resp(C006_WritePollingAddress_Resp* response, 
		BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->pollingAddressDevice);
  if(STREAM_GetRemainingSize(fromStream) < 1)
  { // backward compatibility mode
    response->loopCurrentModeCode = response->pollingAddressDevice ? LoopCurrentModeCodes_Disabled : LoopCurrentModeCodes_Enabled;
    response->ucBackwardCompatMode = 1;
  }
  else
  {
	  STREAM_READ_UINT8(fromStream, &response->loopCurrentModeCode);
    response->ucBackwardCompatMode = 0;
  }

	return RCS_N00_Success;
}
