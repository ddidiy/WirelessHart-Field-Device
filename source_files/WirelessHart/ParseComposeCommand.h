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

#ifndef PARSE_COMPOSE_COMMAND_H_
#define PARSE_COMPOSE_COMMAND_H_

#include "ParseComposeCommandTables.h"
#include "util/WirelessStackTypes.h"
#include "ApplicationLayer/Model/CommonResponseCode.h"

typedef enum
{
	CD_REQUEST = 1,
	CD_RESPONSE = 2
} CommandDirection;

enum
{
	WIRELESS_CMD = 0,
	WIRED_CMD = 1
};

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ParseWiredCommand( BinaryStream * p_pStream,
                           void* p_pParsedCommand, 
                           uint16_t* p_punCmdId, 
                           ParserComposerEntry** p_ppParsedCmdEntry
                         );
    
void ComposeResponseToCommmand( uint8_t p_ucSrcAddrIdx,
                                      void* p_pRespData, 
                                      uint16_t p_unCmdId, 
                                      const ParserComposerEntry* p_pCmdEntry, 
                                      uint8_t p_ucResponseCode,
                                      BinaryStream * p_pOutStream);

/**
 * Parses the first Application Command from the Stream, and put the result into parsedCommand.
 *
 * @direction - tell if received packet type is request or un-solicited (published) response 
 * @Stream - holds input binary command data
 * @Stream.nextByte - position of the first byte in input binary command data
 * @Stream.endByte - end of received packet in input binary command data (could contain aggregated commands)
 *                 - will be restored before return
 * @parsedCommand - preallocated memory that will contain parsed Cxxx_..._Req/Resp structure
 * @parsedCmdId - returns parsed command ID
 * @parsedCmdEntry - returns the entry into the parse/compose/execute/analyze command table for the parsed command
 *
 * @returns 0 success, or an error code (CommonResponseCode); for a received response will return 
 *      the received RC or a parsing error code (E01, E05, E64) -in this case no further analyze should be performed
 */
uint8_t ParseWirelessCommmand(const CommandDirection direction, 
                              BinaryStream * p_pStream, 
                              void* parsedCommand, 
                              uint16_t* parsedCmdId, 
                              ParserComposerEntry** pparsedCmdEntry);

/**
 *
 */
uint8_t ComposeWirelessRequestCommmand(void* requestParam, uint16_t cmdId, BinaryStream * pStream);
uint8_t ComposeWiredRequestCommmand(void* requestParam, uint16_t cmdId, BinaryStream* pStream);

uint8_t IsResponseCodeError(uint8_t rc);

#ifdef __cplusplus
}
#endif

#endif /*PARSE_COMPOSE_COMMAND_H_*/
