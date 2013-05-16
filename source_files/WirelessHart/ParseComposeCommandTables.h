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

#ifndef PARSE_COMPOSE_COMMAND_TABLES_H_
#define PARSE_COMPOSE_COMMAND_TABLES_H_

#include "ApplicationLayer/Model/CommonTables.h"
#include "util/Binarization.h"


typedef uint8_t (*ParseFunction)(void* /*command*/,  BinaryStream* /*fromStream*/);
typedef uint8_t (*ComposeFunction)(void* /*command*/, BinaryStream* /*toStream*/);
typedef uint16_t (*ExecuteFunction)(void* /*request*/, void* /*response*/, 
                                   uint16_t/*srcAddress*/, uint8_t/*control*/, uint8_t/*remainingBytes*/);

/**
 * Holds a pair of parser & composer functions for cmdId
 * 	request parser + response composer
 *  request composer + response parser
 */
typedef struct
{
	uint16_t cmdId;
  uint8_t  ucMinReqSize;
  uint8_t  ucMinRespSize;
	ComposeFunction fnComposer;
	ParseFunction   fnParser;
  void*           fnExecCmd;
} ParserComposerEntry;



#ifdef __cplusplus
extern "C" {
#endif

/**
 * @Finds a ParserEntry by command id (as binary search).
 */
const ParserComposerEntry*  FindEntry(const ParserComposerEntry* sortedList, int listSize, const uint16_t cmdId);

ParserComposerEntry*        FindParseReqComposeResp(const uint16_t cmdId);

ParseFunction               FindRequestParser(const uint16_t cmdId);

ExecuteFunction             FindRequestExecuter(const uint16_t cmdId);

ComposeFunction             FindResponseComposer(const uint16_t cmdId);

ParserComposerEntry*        FindComposeReqParseResp(const uint16_t cmdId);

ComposeFunction             FindRequestComposer(const uint16_t cmdId);

ParseFunction               FindResponseParser(const uint16_t cmdId);

ExecuteFunction             FindResponseAnalyzer(const uint16_t cmdId);



#ifdef __cplusplus
}
#endif


#endif /* PARSE_COMPOSE_COMMAND_TABLES_H_ */
