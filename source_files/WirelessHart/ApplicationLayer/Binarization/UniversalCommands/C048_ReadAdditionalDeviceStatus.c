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

#include "C048_ReadAdditionalDeviceStatus.h"


uint8_t Compose_C048_ReadAdditionalDeviceStatus_Req(C048_ReadAdditionalDeviceStatus_Req* request,
			 BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, request->deviceSpecificStatus1, sizeof(request->deviceSpecificStatus1))
	STREAM_WRITE_UINT8(toStream, request->extendedDeviceStatus);
	STREAM_WRITE_UINT8(toStream, request->deviceOperatingMode);
	STREAM_WRITE_UINT8(toStream, request->standardizedStatus0);
	STREAM_WRITE_UINT8(toStream, request->standardizedStatus1);
	STREAM_WRITE_UINT8(toStream, request->analogChannelSaturatedCode);

	STREAM_WRITE_UINT8(toStream, request->standardizedStatus2);
	STREAM_WRITE_UINT8(toStream, request->standardizedStatus3);
	STREAM_WRITE_UINT8(toStream, request->analogChannelFixedCode);
	STREAM_WRITE_BYTES(toStream, request->deviceSpecificStatus2, sizeof(request->deviceSpecificStatus2))

	return RCS_N00_Success;
}

uint8_t Parse_C048_ReadAdditionalDeviceStatus_Req(C048_ReadAdditionalDeviceStatus_Req* request,
			 BinaryStream* fromStream)
{
	#if defined(_DEBUG)
	if ((STREAM_GetRemainingSize(fromStream) < C048_CommandSize_25)&&(STREAM_GetRemainingSize(fromStream) > 0))
		return RCS_E05_TooFewDataBytesReceived;
	#endif
  
  request->commandSize =  STREAM_GetRemainingSize(fromStream);
  
  if(request->commandSize == 0)
    return RCS_N00_Success;
  
  if(request->commandSize < C048_SIZE_9)
    return RCS_E05_TooFewDataBytesReceived;
  
  STREAM_READ_BYTES(fromStream, &request->deviceSpecificStatus1, sizeof(request->deviceSpecificStatus1))
  STREAM_READ_UINT8(fromStream, &request->extendedDeviceStatus);
  STREAM_READ_UINT8(fromStream, &request->deviceOperatingMode);
	STREAM_READ_UINT8(fromStream, &request->standardizedStatus0);
  
  if(request->commandSize < C048_SIZE_14)
    return RCS_N00_Success;
  
	STREAM_READ_UINT8(fromStream, &request->standardizedStatus1);
	STREAM_READ_UINT8(fromStream, &request->analogChannelSaturatedCode);
  STREAM_READ_UINT8(fromStream, &request->standardizedStatus2);
	STREAM_READ_UINT8(fromStream, &request->standardizedStatus3);
	STREAM_READ_UINT8(fromStream, &request->analogChannelFixedCode);
        
  if(request->commandSize < C048_SIZE_25)
    return RCS_N00_Success;
  
	STREAM_READ_BYTES(fromStream, &request->deviceSpecificStatus2, sizeof(request->deviceSpecificStatus2))

	return RCS_N00_Success; 
}

uint8_t Compose_C048_ReadAdditionalDeviceStatus_Resp(C048_ReadAdditionalDeviceStatus_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, response->deviceSpecificStatus1, sizeof(response->deviceSpecificStatus1))
	STREAM_WRITE_UINT8(toStream, response->extendedDeviceStatus);
	STREAM_WRITE_UINT8(toStream, response->deviceOperatingMode);
	STREAM_WRITE_UINT8(toStream, response->standardizedStatus0);

	if(C048_SIZE_9 == response->commandSize) 
          return RCS_N00_Success;
        
        STREAM_WRITE_UINT8(toStream, response->standardizedStatus1);
        STREAM_WRITE_UINT8(toStream, response->analogChannelSaturatedCode);
 	STREAM_WRITE_UINT8(toStream, response->standardizedStatus2);
        STREAM_WRITE_UINT8(toStream, response->standardizedStatus3);
        STREAM_WRITE_UINT8(toStream, response->analogChannelFixedCode);
	
	if(C048_SIZE_14 == response->commandSize) 
          return RCS_N00_Success;
        
        //C048_SIZE_25:
        STREAM_WRITE_BYTES(toStream, response->deviceSpecificStatus2, sizeof(response->deviceSpecificStatus2))
	
	return RCS_N00_Success;
}

uint8_t Parse_C048_ReadAdditionalDeviceStatus_Resp(C048_ReadAdditionalDeviceStatus_Resp* response,
		 BinaryStream* fromStream)
{

	#if defined(_DEBUG)
		if (STREAM_GetRemainingSize(fromStream) < C048_CommandSize_9)
			return RCS_E05_TooFewDataBytesReceived;
	#endif

        response->commandSize = STREAM_GetRemainingSize(fromStream);
        
	STREAM_READ_BYTES(fromStream, &response->deviceSpecificStatus1, sizeof(response->deviceSpecificStatus1));
	STREAM_READ_UINT8(fromStream, &response->extendedDeviceStatus);
	STREAM_READ_UINT8(fromStream, &response->deviceOperatingMode);
	STREAM_READ_UINT8(fromStream, &response->standardizedStatus0);

       	if(C048_SIZE_9 == response->commandSize) 
          return RCS_N00_Success;

	STREAM_READ_UINT8(fromStream, &response->standardizedStatus1);
	STREAM_READ_UINT8(fromStream, &response->analogChannelSaturatedCode);
	STREAM_READ_UINT8(fromStream, &response->standardizedStatus2);
        STREAM_READ_UINT8(fromStream, &response->standardizedStatus3);
	STREAM_READ_UINT8(fromStream, &response->analogChannelFixedCode);
	
	if(C048_SIZE_14 == response->commandSize) 
          return RCS_N00_Success;

	STREAM_READ_BYTES(fromStream, &response->deviceSpecificStatus2,	sizeof(response->deviceSpecificStatus2))

	return RCS_N00_Success; //success
}

