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

#include "C003_ReadDynamicVariablesAndLoopCurrent.h"

uint8_t Compose_C003_ReadDynamicVariablesAndLoopCurrent_Req(C003_ReadDynamicVariablesAndLoopCurrent_Req* request,
                                                             BinaryStream* toStream)
{
  return RCS_N00_Success;
}

uint8_t Parse_C003_ReadDynamicVariablesAndLoopCurrent_Req(C003_ReadDynamicVariablesAndLoopCurrent_Req* request,
                                                           BinaryStream* fromStream)
{
  return RCS_N00_Success;
}

uint8_t Compose_C003_ReadDynamicVariablesAndLoopCurrent_Resp(C003_ReadDynamicVariablesAndLoopCurrent_Resp* response,
                                                              BinaryStream* toStream)
{
#ifdef _DEBUG
  if(STREAM_GetRemainingSize(toStream) < C003_RespSize)
    return RCS_E01_Undefined1;
#endif

  STREAM_WRITE_FLOAT(toStream, response->primaryVariableLoopCurrent);
  for(uint8_t i = 0; i < response->noOfVariables; i++)
  {
    STREAM_WRITE_UINT8(toStream, response->variables[i].unitsCode);
    STREAM_WRITE_FLOAT(toStream, response->variables[i].variable);
  }

  return RCS_N00_Success;
}

uint8_t Parse_C003_ReadDynamicVariablesAndLoopCurrent_Resp(C003_ReadDynamicVariablesAndLoopCurrent_Resp* response,
                                                            BinaryStream* fromStream)
{
  // response length can be 9, 14, 19, 24 for 1, 2, 3 or 4 variables, respectively
  response->noOfVariables = (STREAM_GetRemainingSize(fromStream) - 4) / 5;   // 4 bytes for PV loop current, 5 bytes for each variable

  STREAM_READ_FLOAT(fromStream, &response->primaryVariableLoopCurrent) ;

  for(uint8_t i = 0; i < response->noOfVariables; i++)
  {
    STREAM_READ_UINT8(fromStream, &response->variables[i].unitsCode);
    STREAM_READ_FLOAT(fromStream, &response->variables[i].variable);
  }

  return RCS_N00_Success;
}
