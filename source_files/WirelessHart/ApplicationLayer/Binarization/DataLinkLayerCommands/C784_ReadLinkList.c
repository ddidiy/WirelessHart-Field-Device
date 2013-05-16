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

#include "C784_ReadLinkList.h"

uint8_t Compose_C784_ReadLinkList_Req ( C784_ReadLinkList_Req* request,
                                    
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT16(toStream, request->m_unLinkIndex);
  STREAM_WRITE_UINT8(toStream, request->m_ucNoOfLinksToRead);

  return RCS_N00_Success; // success
}

uint8_t Parse_C784_ReadLinkList_Req ( C784_ReadLinkList_Req* request,
                                    
		                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C784_ReqSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  STREAM_READ_UINT16(fromStream, &request->m_unLinkIndex);
  STREAM_READ_UINT8(fromStream, &request->m_ucNoOfLinksToRead);

  return RCS_N00_Success; // success
}

uint8_t Compose_C784_ReadLinkList_Resp ( C784_ReadLinkList_Resp* response,
                                    
                                    BinaryStream* toStream )
{
  STREAM_WRITE_UINT16(toStream, response->m_unLinkIndex);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfLinksRead);
  STREAM_WRITE_UINT16(toStream, response->m_unNoOfActiveLinks);

  for(uint8_t i = 0; i < response->m_ucNoOfLinksRead; i++)
  {
    STREAM_WRITE_UINT8(toStream, response->m_aLinks[i].superframeId);
    STREAM_WRITE_UINT16(toStream, response->m_aLinks[i].slotNoForThisLink);
    STREAM_WRITE_UINT8(toStream, response->m_aLinks[i].channelOffsetForThisLink);
    STREAM_WRITE_UINT16(toStream, response->m_aLinks[i].nicknameOfNeighborForThisLink);
    STREAM_WRITE_UINT8(toStream, response->m_aLinks[i].linkOptions);
    STREAM_WRITE_UINT8(toStream, response->m_aLinks[i].linkType);
  }

  return RCS_N00_Success; // success
}

uint8_t Parse_C784_ReadLinkList_Resp ( C784_ReadLinkList_Resp* response,
                                    
		                    BinaryStream* fromStream )
{
  STREAM_READ_UINT16(fromStream, &response->m_unLinkIndex);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfLinksRead);
  STREAM_READ_UINT16(fromStream, &response->m_unNoOfActiveLinks);

  if (STREAM_GetRemainingSize(fromStream) < response->m_ucNoOfLinksRead * 8)
    return RCS_E05_TooFewDataBytesReceived;
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > response->m_ucNoOfLinksRead * 8)
    return RCS_E01_Undefined1; // too many bytes
#endif

  for(uint8_t i = 0; i < response->m_ucNoOfLinksRead; i++)
  {
    STREAM_READ_UINT8(fromStream, &response->m_aLinks[i].superframeId);
    STREAM_READ_UINT16(fromStream, &response->m_aLinks[i].slotNoForThisLink);
    STREAM_READ_UINT8(fromStream, &response->m_aLinks[i].channelOffsetForThisLink);
    STREAM_READ_UINT16(fromStream, &response->m_aLinks[i].nicknameOfNeighborForThisLink);
    STREAM_READ_UINT8(fromStream, &response->m_aLinks[i].linkOptions);
    STREAM_READ_UINT8(fromStream, &response->m_aLinks[i].linkType);
  }

  return RCS_N00_Success; // success
}
