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

#include "C775_WriteNetworkTag.h"

uint8_t Compose_C775_WriteNetworkTag_Req( C775_WriteNetworkTag_Req* request,
																				 
																				 BinaryStream* toStream )
{
	STREAM_WRITE_LATIN(toStream, request->m_aNetworkTag, sizeof(request->m_aNetworkTag));

	return RCS_N00_Success; // success
}

uint8_t Parse_C775_WriteNetworkTag_Req( C775_WriteNetworkTag_Req* request,
																			 
																			 BinaryStream* fromStream )
{
	STREAM_READ_LATIN(fromStream, request->m_aNetworkTag, sizeof(request->m_aNetworkTag));

	return RCS_N00_Success; // success
}

uint8_t Compose_C775_WriteNetworkTag_Resp( C775_WriteNetworkTag_Resp* response,
																					
																					BinaryStream* toStream )
{
	STREAM_WRITE_LATIN(toStream, response->m_aNetworkTag, sizeof(response->m_aNetworkTag));

	return RCS_N00_Success; // success
}

uint8_t Parse_C775_WriteNetworkTag_Resp( C775_WriteNetworkTag_Resp* response,
																				
																				BinaryStream* fromStream )
{
	STREAM_READ_LATIN(fromStream, response->m_aNetworkTag, sizeof(response->m_aNetworkTag));

	return RCS_N00_Success; // success
}
