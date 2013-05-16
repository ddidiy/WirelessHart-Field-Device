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

#include "C107_WriteBurstDeviceVariables.h"

uint8_t Compose_C107_WriteBurstDeviceVariables_Req(C107_WriteBurstDeviceVariables_Req* request,
		 BinaryStream* toStream)
{
	#ifdef _DEBUG
	if (request->noOfDeviceVariables == 0 || 4 < request->noOfDeviceVariables)
	return RCS_E01_Undefined1;
#endif

	for (uint8_t i = 0; i < request->noOfDeviceVariables; i++)
	{
		STREAM_WRITE_UINT8(toStream, request->deviceVariableCode[i]);
	}
	STREAM_WRITE_UINT8(toStream, request->burstMessage);
	return RCS_N00_Success;
}

uint8_t Parse_C107_WriteBurstDeviceVariables_Req(C107_WriteBurstDeviceVariables_Req* request, 
		BinaryStream* fromStream)
{
	request->noOfDeviceVariables = STREAM_GetRemainingSize(fromStream); // total number of request bytes
        request->backComp = 0;
  if(request->noOfDeviceVariables < 9)
  {
    if(request->noOfDeviceVariables > 4)
      return RCS_E05_TooFewDataBytesReceived;
    else
    {
      request->backComp = 1;  // request contains only 1, 2, 3 or 4 bytes
      request->burstMessage = 0;
    }
  }
  else
  {
    request->noOfDeviceVariables = 8;
  }
  
  uint8_t i;
	for (i = 0; i < request->noOfDeviceVariables; i++)
	{
		STREAM_READ_UINT8(fromStream, &request->deviceVariableCode[i]);
	}
  for( ; i < 8; i++)
  {
    request->deviceVariableCode[i] = DeviceVariableCodes_NotUsed; // 250
  }
  
  if(request->noOfDeviceVariables == 8)
	  STREAM_READ_UINT8(fromStream, &request->burstMessage);
	
  return RCS_N00_Success;
}

uint8_t Compose_C107_WriteBurstDeviceVariables_Resp(C107_WriteBurstDeviceVariables_Resp* response,
		 BinaryStream* toStream)
{
	for (uint8_t i = 0; i < MaxNoOfDeviceVariables; i++)
	{
		STREAM_WRITE_UINT8(toStream, response->deviceVariableCode[i]);
	}
	STREAM_WRITE_UINT8(toStream, response->burstMessage);
	return RCS_N00_Success;
}

uint8_t Parse_C107_WriteBurstDeviceVariables_Resp(C107_WriteBurstDeviceVariables_Resp* response,
		 BinaryStream* fromStream)
{
	for (uint8_t i = 0; i < MaxNoOfDeviceVariables; i++)
	{
		STREAM_READ_UINT8(fromStream, &response->deviceVariableCode[i]);
	}
	STREAM_READ_UINT8(fromStream, &response->burstMessage);
	return RCS_N00_Success;
}
