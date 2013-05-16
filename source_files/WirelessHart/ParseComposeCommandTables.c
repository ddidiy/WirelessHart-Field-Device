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

#include "ParseComposeCommandTables.h"

#include "ApplicationLayer/Binarization/UniversalCommands.h"
#include "ApplicationLayer/Binarization/CommonPracticeCommands.h"

#include "ApplicationLayer/Binarization/PhysicalLayerCommands.h"
#include "ApplicationLayer/Binarization/DataLinkLayerCommands.h"
#include "ApplicationLayer/Binarization/NetworkLayerCommands.h"

#include "ApplicationLayer/Binarization/GatewayCommands.h"
#include "ApplicationLayer/Binarization/WirelessApplicationCommands.h"
#include "ApplicationLayer/Binarization/WirelessNetworkManagerCommands.h"

#include "ApplicationLayer/Binarization/DeviceSpecificWirelessOnlyCommands.h"
#include "ApplicationLayer/Binarization/DeviceSpecificCommands.h"

// Macros used to construct the function pointer table for the Master functionality
//TODO [doinel.alban]: the functions to take over the received command response are not yet defined
#define CREATE_COMPOSER_REQ_PARSER_RESP_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    (ComposeFunction)&Compose_##CommandName##_Req, \
    (ParseFunction)&Parse_##CommandName##_Resp, \
    NULL \
  }
#define CREATE_NULLCOMPOSER_REQ_PARSER_RESP_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    NULL, \
    (ParseFunction)&Parse_##CommandName##_Resp, \
    NULL \
  }
#define CREATE_COMPOSER_REQ_NULLPARSER_RESP_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    (ComposeFunction)&Compose_##CommandName##_Req, \
    NULL, \
    NULL \
  }
#define CREATE_NULLCOMPOSER_REQ_NULLPARSER_RESP_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    NULL, \
    NULL, \
    NULL \
  }


// Macros used to construct the function pointer table for the Slave functionality
#define CREATE_COMPOSER_RESP_PARSER_REQ_EXECUTE_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    (ComposeFunction)&Compose_##CommandName##_Resp, \
    (ParseFunction)&Parse_##CommandName##_Req, \
    (void*)&Execute_##CommandName \
  }
#define CREATE_COMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    (ComposeFunction)&Compose_##CommandName##_Resp, \
    (ParseFunction)&Parse_##CommandName##_Req, \
    NULL \
  }
#define CREATE_NULLCOMPOSER_RESP_PARSER_REQ_NULLEXECUTE_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    NULL, \
    (ParseFunction)&Parse_##CommandName##_Req, \
    NULL \
  }
#define CREATE_NULLCOMPOSER_RESP_NULLPARSER_REQ_NULLEXECUTE_ENTRY(CommandName)\
  { CMDID_##CommandName, CommandName##_ReqSize, CommandName##_RespSize, \
    NULL, \
    NULL, \
    NULL \
  }


//#define CREATE_EXECUTE_ENTRY(CommandName)\
//  {CMDID_##CommandName, (ExecuteFunction)&Execute_##CommandName##}
//#define CREATE_NULLEXECUTE_ENTRY(CommandName)\
//  {CMDID_##CommandName, NULL}

// include specified command tables

#include "../system.h"
#	include "app_cmdresp.h"

#if( WIHARTDEV_TYPE == WIHARTDEV_TYPE_FIELDDEV ) 
  #	include "CommandTables_FieldDevice.h"
#elif( WIHARTDEV_TYPE == WIHARTDEV_TYPE_ACCPOINT )
  #	include "CommandTables_AccessPoint.h"
#else
  #error "Unsupported WirelessHART device type"
////just for testing purpose
//#	include "CommandTables_All.h"
#endif

/**
* Binary search algorithm.
*/
const ParserComposerEntry* FindEntry(const ParserComposerEntry* sortedList, int listSize, const uint16_t cmdId)
{
uint16 unLow = 0;
uint16 unHigh = listSize-1;
  
  while(unLow < unHigh)
  {
    uint16 unMid = (unLow + unHigh) / 2;  // cannot be more than 32768 entries in the command table
    if((sortedList + unMid)->cmdId < cmdId)
      unLow = unMid + 1;
    else
      unHigh = unMid;
  }
  
  if((unLow < listSize) && (sortedList + unLow)->cmdId == cmdId)
    return sortedList + unLow;  // found
  else
    return 0;                   // not found

}



// The following functions will be called from APP layer


// device as a slave, receiving acknowledged requests from a master or publishing un-solicited response messages
ParserComposerEntry*  FindParseReqComposeResp(const uint16_t cmdId)
{
  return (ParserComposerEntry*)FindEntry(g_parseReqComposeResp, sizeof(g_parseReqComposeResp) / sizeof(g_parseReqComposeResp[0]), cmdId);
}

ParseFunction FindRequestParser(const uint16_t cmdId)
{
  const ParserComposerEntry* entry = FindEntry(g_parseReqComposeResp, sizeof(g_parseReqComposeResp)
                                               / sizeof(g_parseReqComposeResp[0]), cmdId);
  return entry
    ? entry->fnParser
      : 0;
}

ExecuteFunction FindRequestExecuter(const uint16_t cmdId)
{
  const ParserComposerEntry* entry = FindEntry(g_parseReqComposeResp, sizeof(g_parseReqComposeResp)
                                            / sizeof(g_parseReqComposeResp[0]), cmdId);
  return entry
    ? (ExecuteFunction)entry->fnExecCmd
      : 0;
}

ComposeFunction FindResponseComposer(const uint16_t cmdId)
{
  const ParserComposerEntry* entry = FindEntry(g_parseReqComposeResp, sizeof(g_parseReqComposeResp)
                                               / sizeof(g_parseReqComposeResp[0]), cmdId);
  return entry
    ? entry->fnComposer
      : 0;
}


// device as a master, sending acknowledged requests or receiving un-solicited response messages
ParserComposerEntry*  FindComposeReqParseResp(const uint16_t cmdId)
{
  return (ParserComposerEntry*)FindEntry(g_composeReqParseResp, sizeof(g_composeReqParseResp) / sizeof(g_composeReqParseResp[0]), cmdId);
}

ComposeFunction FindRequestComposer(const uint16_t cmdId)
{
  const ParserComposerEntry* entry = FindEntry(g_composeReqParseResp, sizeof(g_composeReqParseResp)
                                               / sizeof(g_composeReqParseResp[0]), cmdId);
  return entry
    ? entry->fnComposer
      : 0;
}

ParseFunction FindResponseParser(const uint16_t cmdId)
{
  const ParserComposerEntry* entry = FindEntry(g_composeReqParseResp, sizeof(g_composeReqParseResp)
                                               / sizeof(g_composeReqParseResp[0]), cmdId);
  return entry
    ? entry->fnParser
      : 0;
}

ExecuteFunction FindResponseAnalyzer(const uint16_t cmdId)
{
  const ParserComposerEntry* entry = FindEntry(g_composeReqParseResp, sizeof(g_composeReqParseResp)
                                            / sizeof(g_composeReqParseResp[0]), cmdId);
  return entry
    ? (ExecuteFunction)entry->fnExecCmd
      : 0;
}
