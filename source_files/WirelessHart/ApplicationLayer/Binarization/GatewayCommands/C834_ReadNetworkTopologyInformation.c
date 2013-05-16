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

#include "C834_ReadNetworkTopologyInformation.h"


uint8_t Compose_C834_ReadNetworkTopologyInformation_Req(C834_ReadNetworkTopologyInformation_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, &request->DeviceLongAddress, 5);
	STREAM_WRITE_UINT16(toStream, request->GraphIndexNo);

	return RCS_N00_Success;
}

/**
 * @Debinarize
 */
uint8_t Parse_C834_ReadNetworkTopologyInformation_Req(C834_ReadNetworkTopologyInformation_Req* request, 
		BinaryStream* fromStream)
{
#ifdef _DEBUG
	if (7 > STREAM_GetRemainingSize(fromStream))
		return RCS_E05_TooFewDataBytesReceived;
#endif
	STREAM_READ_BYTES(fromStream, &request->DeviceLongAddress, 5);
	STREAM_READ_UINT16(fromStream, &request->GraphIndexNo);

	return RCS_N00_Success;
}

/**
 *
 */
uint8_t Compose_C834_ReadNetworkTopologyInformation_Resp(C834_ReadNetworkTopologyInformation_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_BYTES(toStream, &response->DeviceLongAddress, 5);
	STREAM_WRITE_UINT16(toStream, response->GraphIndexNo);
	STREAM_WRITE_UINT16(toStream, response->TotalGraphsNo);
	STREAM_WRITE_UINT16(toStream, response->IndexGraphId);
	STREAM_WRITE_UINT16(toStream, response->NeighboursNo);

	for (int i = 0; i < response->NeighboursNo; i++)
	{
		STREAM_WRITE_UINT16(toStream, response->Neighbour[i]);
	}

	return RCS_N00_Success;
}

/**
 *
 */
uint8_t Parse_C834_ReadNetworkTopologyInformation_Resp(C834_ReadNetworkTopologyInformation_Resp* response, 
		BinaryStream* fromStream)
{
//#ifdef _DEBUG
//	if (39 > STREAM_GetRemainingSize(fromStream))
//		return RCS_E05_TooFewDataBytesReceived;
//#endif

	STREAM_READ_BYTES(fromStream, &response->DeviceLongAddress, 5);
	STREAM_READ_UINT16(fromStream, &response->GraphIndexNo);
	STREAM_READ_UINT16(fromStream, &response->TotalGraphsNo);
	STREAM_READ_UINT16(fromStream, &response->IndexGraphId);
	STREAM_READ_UINT16(fromStream, &response->NeighboursNo);
	for (int i = 0; i < response->NeighboursNo; i++)
	{
		STREAM_READ_UINT16(fromStream, &response->Neighbour[i]);
	}

	return RCS_N00_Success;
}
