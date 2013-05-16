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

#include "C513_WriteCountryCode.h"

uint8_t Compose_C513_WriteCountryCode_Req(C513_WriteCountryCode_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_LATIN(toStream, request->countryCode, 2);
	STREAM_WRITE_UINT8(toStream, request->siUnitsOnly);
	return RCS_N00_Success;
}

uint8_t Parse_C513_WriteCountryCode_Req(C513_WriteCountryCode_Req* request, 
		BinaryStream* fromStream)
{
	STREAM_READ_LATIN(fromStream, &request->countryCode, 2);
	STREAM_READ_UINT8(fromStream, &request->siUnitsOnly);
	return RCS_N00_Success;
}

uint8_t Compose_C513_WriteCountryCode_Resp(C513_WriteCountryCode_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_LATIN(toStream, response->countryCode, 2);
	STREAM_WRITE_UINT8(toStream, response->siUnitsOnly);
	return RCS_N00_Success;
}

uint8_t Parse_C513_WriteCountryCode_Resp(C513_WriteCountryCode_Resp* response, 
		BinaryStream* fromStream)
{
	STREAM_READ_LATIN(fromStream, &response->countryCode, 2);
	STREAM_READ_UINT8(fromStream, &response->siUnitsOnly);
	return RCS_N00_Success;
}
