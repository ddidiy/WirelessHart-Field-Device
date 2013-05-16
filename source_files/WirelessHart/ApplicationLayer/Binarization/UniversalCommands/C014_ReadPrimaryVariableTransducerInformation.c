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

#include "C014_ReadPrimaryVariableTransducerInformation.h"

uint8_t Compose_C014_ReadPrimaryVariableTransducerInformation_Req(C014_ReadPrimaryVariableTransducerInformation_Req* request,
			 BinaryStream* toStream)
{
	return RCS_N00_Success;
}

uint8_t Parse_C014_ReadPrimaryVariableTransducerInformation_Req(C014_ReadPrimaryVariableTransducerInformation_Req* request,
			 BinaryStream* fromStream)
{
	return RCS_N00_Success; //success
}

uint8_t Compose_C014_ReadPrimaryVariableTransducerInformation_Resp(C014_ReadPrimaryVariableTransducerInformation_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT24(toStream, response->transducerSerialNumber);
	STREAM_WRITE_UINT8(toStream, response->transducerLimitsMinimumSpanUnitsCode);
	STREAM_WRITE_FLOAT(toStream, response->upperTransducerLimit);
	STREAM_WRITE_FLOAT(toStream, response->lowerTransducerLimit);
	STREAM_WRITE_FLOAT(toStream, response->minimumSpan);

	return RCS_N00_Success; //success
}

uint8_t Parse_C014_ReadPrimaryVariableTransducerInformation_Resp(C014_ReadPrimaryVariableTransducerInformation_Resp* response,
		 BinaryStream* fromStream)
{

	#if defined(_DEBUG)
		if (STREAM_GetRemainingSize(fromStream) < C014_RespSize)
			return RCS_E05_TooFewDataBytesReceived;
	#endif

	STREAM_READ_UINT24(fromStream, &response->transducerSerialNumber);
	STREAM_READ_UINT8(fromStream, &response->transducerLimitsMinimumSpanUnitsCode);
	STREAM_READ_FLOAT(fromStream, &response->upperTransducerLimit);
	STREAM_READ_FLOAT(fromStream, &response->lowerTransducerLimit);
	STREAM_READ_FLOAT(fromStream, &response->minimumSpan);

	return RCS_N00_Success; //success
}

