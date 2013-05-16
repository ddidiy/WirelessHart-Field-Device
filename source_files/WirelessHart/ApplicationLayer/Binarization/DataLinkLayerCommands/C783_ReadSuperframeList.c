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

#include "C783_ReadSuperframeList.h"

uint8_t Compose_C783_ReadSuperframeList_Req( C783_ReadSuperframeList_Req* request,
																						 
																						 BinaryStream* toStream )
{
	STREAM_WRITE_UINT8(toStream, request->m_ucSuperframeIndex);
	STREAM_WRITE_UINT8(toStream, request->m_ucNoOfEntriesToRead);

	return RCS_N00_Success; // success
}

uint8_t Parse_C783_ReadSuperframeList_Req( C783_ReadSuperframeList_Req* request,
																					 
																					 BinaryStream* fromStream )
{
	STREAM_READ_UINT8(fromStream, &request->m_ucSuperframeIndex);
	STREAM_READ_UINT8(fromStream, &request->m_ucNoOfEntriesToRead);

	return RCS_N00_Success; // success
}

uint8_t Compose_C783_ReadSuperframeList_Resp( C783_ReadSuperframeList_Resp* response,
																							
																							BinaryStream* toStream )
{
	STREAM_WRITE_UINT8(toStream, response->m_ucSuperframeIndex);
	STREAM_WRITE_UINT8(toStream, response->m_ucNoOfEntriesRead);
	STREAM_WRITE_UINT8(toStream, response->m_ucNoOfActiveSuperframes);

	for(uint8_t i = 0; i < response->m_ucNoOfEntriesRead; i++)
	{
		STREAM_WRITE_UINT8(toStream, response->m_aSuperframes[i].superframeId);
		STREAM_WRITE_UINT16(toStream, response->m_aSuperframes[i].noOfSlotsInSuperframe);
		STREAM_WRITE_UINT8(toStream, response->m_aSuperframes[i].superframeModeFlags);
	}

	return RCS_N00_Success; // success
}

uint8_t Parse_C783_ReadSuperframeList_Resp( C783_ReadSuperframeList_Resp* response,
																						
																						BinaryStream* fromStream )
{
	STREAM_READ_UINT8(fromStream, &response->m_ucSuperframeIndex);
	STREAM_READ_UINT8(fromStream, &response->m_ucNoOfEntriesRead);
	STREAM_READ_UINT8(fromStream, &response->m_ucNoOfActiveSuperframes);

	if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfEntriesRead * 4)
		return RCS_E05_TooFewDataBytesReceived;

	for(uint8_t i = 0; i < response->m_ucNoOfEntriesRead; i++)
	{
		STREAM_READ_UINT8(fromStream, &response->m_aSuperframes[i].superframeId);
		STREAM_READ_UINT16(fromStream, &response->m_aSuperframes[i].noOfSlotsInSuperframe);
		STREAM_READ_UINT8(fromStream, &response->m_aSuperframes[i].superframeModeFlags);
	}

	return RCS_N00_Success; // success
}
