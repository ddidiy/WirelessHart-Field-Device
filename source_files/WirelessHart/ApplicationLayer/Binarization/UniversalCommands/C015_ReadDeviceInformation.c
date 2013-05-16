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

#include "C015_ReadDeviceInformation.h"
enum {
	C015_Byte16_Reserved = 250
};

uint8_t Compose_C015_ReadDeviceInformation_Req(C015_ReadDeviceInformation_Req* request,
			 BinaryStream* toStream)
{
	return RCS_N00_Success;
}

uint8_t Parse_C015_ReadDeviceInformation_Req(C015_ReadDeviceInformation_Req* request,
			 BinaryStream* fromStream)
{
	return RCS_N00_Success; //success
}

uint8_t Compose_C015_ReadDeviceInformation_Resp(C015_ReadDeviceInformation_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->pvAlarmSelectionCodes);
	STREAM_WRITE_UINT8(toStream, response->pvTransferFunctionCode);
	STREAM_WRITE_UINT8(toStream, response->pvUpperLowerRangeValuesUnitsCode);
	STREAM_WRITE_FLOAT(toStream, response->pvUpperRangeValue);
	STREAM_WRITE_FLOAT(toStream, response->pvLowerRangeValue);
	STREAM_WRITE_FLOAT(toStream, response->pvDampingValue);
	STREAM_WRITE_UINT8(toStream, response->writeProtectCode);
	STREAM_WRITE_UINT8(toStream, C015_Byte16_Reserved);
	STREAM_WRITE_UINT8(toStream, response->pvAnalogChannelFlags);

	return RCS_N00_Success; //success
}

uint8_t Parse_C015_ReadDeviceInformation_Resp(C015_ReadDeviceInformation_Resp* response,
		 BinaryStream* fromStream)
{

	#if defined(_DEBUG)
		if (STREAM_GetRemainingSize(fromStream) < C015_RespSize)
			return RCS_E05_TooFewDataBytesReceived;
	#endif

	STREAM_READ_UINT8(fromStream, &response->pvAlarmSelectionCodes);
	STREAM_READ_UINT8(fromStream, &response->pvTransferFunctionCode);
	STREAM_READ_UINT8(fromStream, &response->pvUpperLowerRangeValuesUnitsCode);
	STREAM_READ_FLOAT(fromStream, &response->pvUpperRangeValue);
	STREAM_READ_FLOAT(fromStream, &response->pvLowerRangeValue);
	STREAM_READ_FLOAT(fromStream, &response->pvDampingValue);
	STREAM_READ_UINT8(fromStream, &response->writeProtectCode);

	uint8_t hardcoded;
	STREAM_READ_UINT8(fromStream, &hardcoded); // read harcoded value 250

	if (hardcoded != C015_Byte16_Reserved)
  {
    #if defined(_DEBUG)
			return 1; //FIXME [george.petrehus] - hardcoded value.
	  #endif
  }

	STREAM_READ_UINT8(fromStream, &response->pvAnalogChannelFlags);

	return RCS_N00_Success; //success
}

