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

#include "C798_ReadRadioPower.h"

uint8_t Compose_C798_ReadRadioPower_Req(C798_ReadRadioPower_Req* request,  BinaryStream* toStream)
{
	return RCS_N00_Success;
}

uint8_t Parse_C798_ReadRadioPower_Req(C798_ReadRadioPower_Req* request,  BinaryStream* fromStream)
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > 0)
    return RCS_E01_Undefined1; // too many bytes
#endif
	return RCS_N00_Success;
}

uint8_t Compose_C798_ReadRadioPower_Resp(C798_ReadRadioPower_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_INT8 (toStream, response->outputPower);

	return RCS_N00_Success;
}

uint8_t Parse_C798_ReadRadioPower_Resp(C798_ReadRadioPower_Resp* response, 
		BinaryStream* fromStream)
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C798_RespSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

	STREAM_READ_INT8(fromStream, &response->outputPower);

	return RCS_N00_Success;
}
