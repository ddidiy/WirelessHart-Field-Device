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

#ifdef UT_ACTIVED  // unit testing support actived

#include "crm.h"
#include "system.h"
#include "typedef.h"
#include "WirelessHart/ApplicationLayer/Binarization/CommonPracticeCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/DataLinkLayerCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/DeviceSpecificWirelessOnlyCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/GatewayCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/NetworkLayerCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/PhysicalLayerCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/UniversalCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/WirelessApplicationCommands.h"
#include "WirelessHart/ApplicationLayer/Binarization/WirelessNetworkManagerCommands.h"

#if( UT_ACTIVED == UT_SIZEOF_ONLY )
  #include "UT_defines.h"
#else
#error " UT_ACTIVED must be UT_SIZEOF_ONLY (=21)"
#endif
/*!!! guard ParseComposeCommand.c with #ifndef UT_ACTIVED !!!*/

typedef struct
{
	uint16_t cmdId;
        uint16_t unReqSize;
        uint16_t unRespSize;
} ParserComposerEntry;

#undef CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY
#define CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(CommandName)\
  { \
    CMDID_##CommandName, \
    sizeof(CommandName##_Req), \
    sizeof(CommandName##_Resp) \
  }
#undef CREATE_COMPOSER_REQ_PARSER_RESP_ENTR
#define CREATE_COMPOSER_REQ_PARSER_RESP_ENTRY(CommandName)\
  { \
    CMDID_##CommandName, \
    sizeof(CommandName##_Req), \
    sizeof(CommandName##_Resp) \
  }
#undef CREATE_NULLCOMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY
#define CREATE_NULLCOMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(CommandName)\
  { \
    CMDID_##CommandName, \
    sizeof(CommandName##_Req), \
    sizeof(CommandName##_Resp) \
  }
#undef CREATE_NULLCOMPOSER_RESP_NULLPARSER_REQ_NULLEXECUTE_ENTRY
#define CREATE_NULLCOMPOSER_RESP_NULLPARSER_REQ_NULLEXECUTE_ENTRY(CommandName)\
  { \
    CMDID_##CommandName, \
    0, \
    0 \
  }
#undef CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY
#define CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(CommandName)\
  { \
    CMDID_##CommandName, \
    sizeof(CommandName##_Req), \
    sizeof(CommandName##_Resp) \
  }
#include "WirelessHart/DeviceCommandTables.h"

void SIZEOF_Test1()
{
  uint16 unMaxReqSize = 0; uint16 unIdxMaxReq = 0xff;
  uint16 unMaxRspSize = 0; uint16 unIdxMaxRsp = 0xff;
  for(uint8 ucIdx=0; 
      ucIdx < sizeof(g_parseReqComposeResp)/sizeof(*g_parseReqComposeResp); 
      ucIdx++)
  {
    if(g_parseReqComposeResp[ucIdx].unReqSize > unMaxReqSize)
    {
      unMaxReqSize = g_parseReqComposeResp[ucIdx].unReqSize;
      unIdxMaxReq = g_parseReqComposeResp[ucIdx].cmdId;
    }
    if(g_parseReqComposeResp[ucIdx].unRespSize > unMaxRspSize)
    {
      unMaxRspSize = g_parseReqComposeResp[ucIdx].unRespSize; 
      unIdxMaxRsp = g_parseReqComposeResp[ucIdx].cmdId;
    }
  }
  CK(unMaxReqSize > 92);
  CK(unMaxRspSize > 92);
  CK(0xff != unIdxMaxReq);
  CK(0xff != unIdxMaxRsp);
}

TEST_FCT g_SIZEOF_Test[] = {
    SIZEOF_Test1
};


int main()
{  
    GlobalDisableIRQ();
  
    WDT_INIT();  

    Digitals_Init();
  
    
  {  
    uint8 ucIdx;
    for(ucIdx = 0; ucIdx < sizeof(g_SIZEOF_Test)/sizeof(*g_SIZEOF_Test); ucIdx++ )
    {        
        FEED_WDT();
        g_SIZEOF_Test[ucIdx]();
    }
    CK(1);
  }
}

#endif // UT_ACTIVED  // unit testing support end

