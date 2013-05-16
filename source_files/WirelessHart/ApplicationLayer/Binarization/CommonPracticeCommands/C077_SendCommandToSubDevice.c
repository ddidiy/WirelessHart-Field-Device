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

#include "C077_SendCommandToSubDevice.h"

uint8_t Compose_C077_SendCommandToSubDevice_Req(C077_SendCommandToSubDevice_Req* request, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, request->ioCard);
	STREAM_WRITE_UINT8(toStream, request->channel);
	STREAM_WRITE_UINT8(toStream, request->transmitPreambleCount);
	uint8_t embededCommandDelimiter = ((uint8_t) (request->addressType << 7) | (uint8_t) ((request->noOfExpansionBytes
			& 0x03) << 5) | (uint8_t) ((request->physicalLayerType & 0x03) << 3) | (uint8_t) (request->frameType & 0x07));
	STREAM_WRITE_UINT8(toStream, embededCommandDelimiter);
	if (request->addressType == 0)
		STREAM_WRITE_BYTES(toStream, &request->fieldAddress, ShortFieldAddressSize);
	if (request->addressType == 1)
		STREAM_WRITE_BYTES(toStream, &request->fieldAddress, LongFieldAddressSize);
	STREAM_WRITE_UINT8(toStream, request->commandOfEmbeddedCommand);
	STREAM_WRITE_UINT8(toStream, request->embeddedCommandSize);
	STREAM_WRITE_BYTES(toStream, &request->embeddedCommand, request->embeddedCommandSize);

	return RCS_N00_Success;
}

uint8_t Parse_C077_SendCommandToSubDevice_Req(C077_SendCommandToSubDevice_Req* request, BinaryStream* fromStream)
{
	//if (STREAM_GetRemainingSize(fromStream) < n)
	//	return RCS_E05_TooFewDataBytesReceived;

	STREAM_READ_UINT8(fromStream, &request->ioCard);
	STREAM_READ_UINT8(fromStream, &request->channel);
	STREAM_READ_UINT8(fromStream, &request->transmitPreambleCount);
	uint8_t embededCommandDelimiter = 0;
	STREAM_READ_UINT8(fromStream, &embededCommandDelimiter);

	request->addressType = (uint8_t) ((embededCommandDelimiter & 0x80) >> 7);
	request->noOfExpansionBytes = (uint8_t) ((embededCommandDelimiter & 0x60) >> 5);
	request->physicalLayerType = (uint8_t) ((embededCommandDelimiter & 0x18) >> 3);
	request->frameType = (uint8_t) (embededCommandDelimiter & 0x07);

	if (request->addressType == 0)
		STREAM_READ_BYTES(fromStream, &request->fieldAddress, ShortFieldAddressSize);
	if (request->addressType == 1)
		STREAM_READ_BYTES(fromStream, &request->fieldAddress, LongFieldAddressSize);

	STREAM_READ_UINT8(fromStream, &request->commandOfEmbeddedCommand);
	STREAM_READ_UINT8(fromStream, &request->embeddedCommandSize);
	STREAM_READ_BYTES(fromStream, &request->embeddedCommand, request->embeddedCommandSize);

	return RCS_N00_Success;
}

uint8_t Compose_C077_SendCommandToSubDevice_Resp(C077_SendCommandToSubDevice_Resp* response, 
		BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->ioCard);
	STREAM_WRITE_UINT8(toStream, response->channel);
	uint8_t embededCommandDelimiter = ((uint8_t) (response->addressType << 7) | (uint8_t) ((response->noOfExpansionBytes
			& 0x03) << 5) | (uint8_t) ((response->physicalLayerType & 0x03) << 3) | (uint8_t) (response->frameType & 0x07));
	STREAM_WRITE_UINT8(toStream, embededCommandDelimiter);
	if (response->addressType == 0)
		STREAM_WRITE_BYTES(toStream, &response->fieldAddress, ShortFieldAddressSize);
	if (response->addressType == 1)
		STREAM_WRITE_BYTES(toStream, &response->fieldAddress, LongFieldAddressSize);
	STREAM_WRITE_UINT8(toStream, response->commandOfEmbeddedCommand);
	STREAM_WRITE_UINT8(toStream, response->embeddedCommandSize);
	STREAM_WRITE_BYTES(toStream, &response->embeddedCommand, response->embeddedCommandSize);

	return RCS_N00_Success;
}

uint8_t Parse_C077_SendCommandToSubDevice_Resp(C077_SendCommandToSubDevice_Resp* response, BinaryStream* fromStream)
{
	//if (STREAM_GetRemainingSize(fromStream) < n)
	//	return RCS_E05_TooFewDataBytesReceived;

	STREAM_READ_UINT8(fromStream, &response->ioCard);
	STREAM_READ_UINT8(fromStream, &response->channel);

	uint8_t embededCommandDelimiter = 0;
	STREAM_READ_UINT8(fromStream, &embededCommandDelimiter);
	response->addressType = (uint8_t) ((embededCommandDelimiter & 0x80) >> 7);
	response->noOfExpansionBytes = (uint8_t) ((embededCommandDelimiter & 0x60) >> 5);
	response->physicalLayerType = (uint8_t) ((embededCommandDelimiter & 0x18) >> 3);
	response->frameType = (uint8_t) (embededCommandDelimiter & 0x07);
	if (response->addressType == 0)
		STREAM_READ_BYTES(fromStream, &response->fieldAddress, ShortFieldAddressSize);
	if (response->addressType == 1)
		STREAM_READ_BYTES(fromStream, &response->fieldAddress, LongFieldAddressSize);

	STREAM_READ_UINT8(fromStream, &response->commandOfEmbeddedCommand);
	STREAM_READ_UINT8(fromStream, &response->embeddedCommandSize);
	STREAM_READ_BYTES(fromStream, &response->embeddedCommand, response->embeddedCommandSize);

	return RCS_N00_Success;
}
