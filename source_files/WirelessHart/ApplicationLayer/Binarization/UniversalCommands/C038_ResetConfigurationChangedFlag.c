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

#include "C038_ResetConfigurationChangedFlag.h"

uint8_t Compose_C038_ResetConfigurationChangedFlag_Req(C038_ResetConfigurationChangedFlag_Req* request,
			 BinaryStream* toStream)
{
  if(!request->backComp)
    STREAM_WRITE_UINT16(toStream, request->configurationChangeCounter);
  return RCS_N00_Success;
}

uint8_t Parse_C038_ResetConfigurationChangedFlag_Req(C038_ResetConfigurationChangedFlag_Req* request,
			 BinaryStream* fromStream)
{
  request->backComp = 0;
  if(!STREAM_GetRemainingSize(fromStream))
  {
    request->backComp = 1;
    return RCS_N00_Success;
  }
  if(2 > STREAM_GetRemainingSize(fromStream))
    return RCS_E05_TooFewDataBytesReceived;
  
  STREAM_READ_UINT16(fromStream, &request->configurationChangeCounter);
  return RCS_N00_Success; //success
}

uint8_t Compose_C038_ResetConfigurationChangedFlag_Resp(C038_ResetConfigurationChangedFlag_Resp* response,
		 BinaryStream* toStream)
{
      
	STREAM_WRITE_UINT16(toStream, response->configurationChangeCounter);

	return RCS_N00_Success;
}

uint8_t Parse_C038_ResetConfigurationChangedFlag_Resp(C038_ResetConfigurationChangedFlag_Resp* response,
		 BinaryStream* fromStream)
{

	#if defined(_DEBUG)
		if (STREAM_GetRemainingSize(fromStream) < C038_RespSize)
			return RCS_E05_TooFewDataBytesReceived;
	#endif

         
	STREAM_READ_UINT16(fromStream, &response->configurationChangeCounter);

	return RCS_N00_Success; //success
}

