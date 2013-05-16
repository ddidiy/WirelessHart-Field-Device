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

#include "C008_ReadDynamicVariableClassifications.h"

uint8_t Compose_C008_ReadDynamicVariableClassifications_Req(C008_ReadDynamicVariableClassifications_Req* request,
		 BinaryStream* toStream)
{
	return RCS_N00_Success;
}

uint8_t Parse_C008_ReadDynamicVariableClassifications_Req(C008_ReadDynamicVariableClassifications_Req* request,
		 BinaryStream* fromStream)
{
	return RCS_N00_Success;
}

uint8_t Compose_C008_ReadDynamicVariableClassifications_Resp(C008_ReadDynamicVariableClassifications_Resp* response,
		 BinaryStream* toStream)
{
	STREAM_WRITE_UINT8(toStream, response->primaryVariableClassification);

	STREAM_WRITE_UINT8(toStream, response->secondaryVariableClassification);

	STREAM_WRITE_UINT8(toStream, response->tertiaryVariableClassification);

	STREAM_WRITE_UINT8(toStream, response->quaternaryVariableClassification);

	return RCS_N00_Success;
}

uint8_t Parse_C008_ReadDynamicVariableClassifications_Resp(C008_ReadDynamicVariableClassifications_Resp* response,
		 BinaryStream* fromStream)
{
	STREAM_READ_UINT8(fromStream, &response->primaryVariableClassification);

	STREAM_READ_UINT8(fromStream, &response->secondaryVariableClassification);

	STREAM_READ_UINT8(fromStream, &response->tertiaryVariableClassification);

	STREAM_READ_UINT8(fromStream, &response->quaternaryVariableClassification);

	return RCS_N00_Success;
}
