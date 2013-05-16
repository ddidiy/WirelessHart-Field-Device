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

#include "ParseComposeCommand.h"
#include "ParseComposeCommandTables.h"
#include "transport_layer.h"
#include "provision.h"

static unsigned  short unExtendedCmdId; /* store the extended cmd in case it is not present in the command table */

uint8_t ParseWiredCommand( BinaryStream * p_pStream, 
                           void* p_pParsedCommand, 
                           uint16_t* p_punCmdId, 
                           ParserComposerEntry** p_ppParsedCmdEntry
                         )
{
  if(31 == *p_punCmdId) // request extended command number
  {
    if(STREAM_GetRemainingSize(p_pStream) < 2)
    {
      *p_ppParsedCmdEntry = NULL; // invalid value
      return RCS_E05_TooFewDataBytesReceived;
    }
    
    STREAM_READ_UINT16(p_pStream, p_punCmdId);
    /* !emerson dev returns cmd not impl at this point!
    
    if(31 == *p_punCmdId)
    { // in order to pass HCF test DLL00024c
      *p_ppParsedCmdEntry = NULL; // invalid value
      return RCS_E05_TooFewDataBytesReceived;
    }
    */
  }
  
  /* store the extended cmd */
  unExtendedCmdId = (*p_punCmdId);
  
  *p_ppParsedCmdEntry = FindParseReqComposeResp(*p_punCmdId);
  if(!(*p_ppParsedCmdEntry))
    return RCS_E64_CommandNotImplemented; 
  
  ParseFunction pParser = (*p_ppParsedCmdEntry)->fnParser;
  if(!pParser)
    return RCS_E64_CommandNotImplemented;  
  
  // generic check; particular cases will be managed by the command's parsing/executing functions
  if(STREAM_GetRemainingSize(p_pStream) < (*p_ppParsedCmdEntry)->ucMinReqSize)
    return RCS_E05_TooFewDataBytesReceived;

  //parse data
  return (*pParser)(p_pParsedCommand, p_pStream);  
}

// SrcAddrIdx: 0 -SecondaryMaster, 1 -PrimaryMaster, >1 -Wireless
void ComposeResponseToCommmand( uint8_t p_ucSrcAddrIdx,
                                      void* p_pRespData, 
                                      uint16_t p_unCmdId, 
                                      const ParserComposerEntry* p_pCmdEntry, 
                                      uint8_t p_ucResponseCode,
                                      BinaryStream * p_pOutStream)
{
// consider caller function has assured enough out buffer space for header + RespData
// wired response header: 4 or 6 bytes for 8-bitCmdId + DataLength + RespCode + DevStatusByte [+ ExtendedCmd]
// wireless response header: 4 bytes for 16-bitCmdID + DataLength + RespCode

  uint8 ucOffset; // (no of bytes included in DataLength before RespData) - 1    

  if(p_ucSrcAddrIdx > 1)
  { // response to be sent through the wireless stack
    ucOffset = (1 - 1);
    STREAM_WRITE_UINT16(p_pOutStream, p_unCmdId);
  }
  else
  {
    STREAM_WRITE_UINT8(p_pOutStream, p_unCmdId);
    if(31 == p_unCmdId)
    { // wired 2 bytes cmd, reserve 2 extra bytes for extended commandID 
      ucOffset = (4 - 1); 
      /* if no entry in the command table and RC = 5 -> return 2 bytes count : response code + field device status */
      if((!p_pCmdEntry) && (RCS_E05_TooFewDataBytesReceived == p_ucResponseCode))
      {
        ucOffset -=2;
      }
    }
    else // could be even 31 in case of a parsing error - too few bytes received (<2, see spec-085 8.1.2)
    { // wired, 8-bitCmdId // CmdID + ByteCnt + RespCode + DevStatusByte
      ucOffset = (2 - 1);      
      
    }
  }

  uint8 * pDataLen = p_pOutStream->nextByte; // retain the position of dataLength byte inside the stream for later use
  
  // compose response header as for an error RC - no RespData will be added
  STREAM_WRITE_UINT8(p_pOutStream, ucOffset + 1); // in case of an error RC, len = RespCode [+ DevStatusByte + [ExtendedCommNumber]]

  STREAM_WRITE_UINT8(p_pOutStream, p_ucResponseCode); 
  
  if(ucOffset)
  { // wired
    STREAM_WRITE_UINT8(p_pOutStream, g_stProv.aDeviceStatus[p_ucSrcAddrIdx]);   
    if(ucOffset == 3)
    {
      STREAM_WRITE_UINT16(p_pOutStream, unExtendedCmdId);    // ExtendedCmd
    }
  }
  
  if (IsResponseCodeError(p_ucResponseCode))
    return; // that was all if error RC   
    
  // get the pointer to response composer function
  ComposeFunction pComposer;
  if(p_pCmdEntry)
    pComposer = p_pCmdEntry->fnComposer;  // binary search was already performed
  else
    pComposer = FindResponseComposer(p_unCmdId);
  
  if (!pComposer)
  { // this shouldn't happen if request parsing function was previously found in CommandTable
    *(pDataLen + 1) = RCS_E64_CommandNotImplemented; // overwite RC byte
    return;    
  }  

  // compose response Data
  uint8_t ucComposingError = (*pComposer)(p_pRespData, p_pOutStream);
  if(ucComposingError)
  { // this could only be RCS_E01_Undefined1 (#ifdef _DEBUG)
    *(pDataLen + 1) = RCS_E06_DeviceSpecificCommandError; // ??? have no better idea
    p_pOutStream->nextByte = pDataLen + ucOffset + 2; // when RC is an error, no response data shall be contained
    return;    
  }  
  
  // put the right data length in header
  *pDataLen = (p_pOutStream->nextByte - pDataLen) - 1; // set data length (-1 because DataLength byte not counted)
}


uint8_t ParseWirelessCommmand(const CommandDirection direction, 
                              BinaryStream * p_pStream, 
                              void* parsedCommand, 
                              uint16_t* parsedCmdId, 
                              ParserComposerEntry** pparsedCmdEntry)
{
  //parse Wireless Application Command Header
  STREAM_READ_UINT16(p_pStream, parsedCmdId);
  _command_size_t dataLength;
  STREAM_READ_UINT8(p_pStream, &dataLength);

  *pparsedCmdEntry = NULL; // invalid value
  
  if (dataLength > STREAM_GetRemainingSize(p_pStream))
  {
    p_pStream->nextByte = p_pStream->endByte; // consume all input stream
    return RCS_E05_TooFewDataBytesReceived;
  }
  
  uint8 ucRC;   // return code
  uint8 * pucNextCmd = p_pStream->nextByte + dataLength;  // save next cmd position to be restored before return
  
  switch (direction)
  {
  case CD_REQUEST:
    {
      // find command's entry in command table
      *pparsedCmdEntry = FindParseReqComposeResp(*parsedCmdId);
      if (!(*pparsedCmdEntry))
      {
        ucRC = RCS_E64_CommandNotImplemented;
        break;
      }        
      ParseFunction parser = (*pparsedCmdEntry)->fnParser;
      if (!parser)
      {
        ucRC = RCS_E64_CommandNotImplemented;
        break;
      }
      
      // generic check; particular cases will be managed by the command's parsing function
      if(dataLength < (*pparsedCmdEntry)->ucMinReqSize)
      {
		    ucRC = RCS_E05_TooFewDataBytesReceived;
      }
      else
      {
        uint8 * pucPacketEnd = p_pStream->endByte;  // save packet end to be restored before return
        p_pStream->endByte = pucNextCmd; // re-init remaining bytes to parse current command only
        // parse request data
        ucRC = (*parser)(parsedCommand, p_pStream);
        p_pStream->endByte = pucPacketEnd;          // restore packet end
      }
    }
    break;

  case CD_RESPONSE:
    { // parse the received un-solicited (published) command response

      // get received response code, which will be returned by this function as is
      STREAM_READ_UINT8(p_pStream, &ucRC);
       
      // find command's entry in g_composeReqParseResp table
      *pparsedCmdEntry = FindComposeReqParseResp(*parsedCmdId);
      if (!(*pparsedCmdEntry))
        ucRC = RCS_E64_CommandNotImplemented; // in this case, overwrite the received RC
      else
      {
        ParseFunction parser = (*pparsedCmdEntry)->fnParser;
        if (!parser)
          ucRC = RCS_E64_CommandNotImplemented; // in this case, overwrite the received RC
        else
        {
          // generic check; particular cases will be managed by the command's parsing function
          if(dataLength < (*pparsedCmdEntry)->ucMinRespSize + 1)  // 1 for RC
                        ucRC = RCS_E05_TooFewDataBytesReceived;

          if(!IsResponseCodeError(ucRC))
          {
            uint8 * pucPacketEnd = p_pStream->endByte;  // save packet end to be restored before return
            p_pStream->endByte = pucNextCmd; // re-init remaining bytes to parse current command only
            // parse response data
            uint8_t parsingError = (*parser)(parsedCommand, p_pStream);
            if(parsingError)
              ucRC = parsingError; // normally, could be RCS_E05_TooFewDataBytesReceived
            p_pStream->endByte = pucPacketEnd;          // restore packet end
          }
      
          // if the return value for CD_RESPONSE is one of: E01, E05 or E64,
          // the caller function should just ignore the current command response
          // and get the next one if exists
        }
      }

    }
    break;

  default:
    ucRC = RCS_E01_Undefined1;  // this should never happen, since was already checked by the calling function  
  } // end switch
 
  p_pStream->nextByte = pucNextCmd; // consume allocated data length for current command
  return ucRC;
}

uint8_t ComposeWirelessRequestCommmand(void* requestParam, uint16_t cmdId, BinaryStream * pStream )
{
 
// consider pStream has at least 3 bytes size for CmdId + dataLength

  STREAM_WRITE_UINT16(pStream, cmdId);
  
  uint8 * pDataLen = pStream->nextByte; // CmdID + dataLength + data
  
  STREAM_WRITE_UINT8(pStream, 0);       // set data length 0 if cmd not implemented
  
  // compose Data
  ComposeFunction composer = FindRequestComposer(cmdId);
  if (!composer)
    return RCS_E64_CommandNotImplemented;

  uint8_t composingError = (*composer)(requestParam,  pStream);
  *pDataLen = (pStream->nextByte - pDataLen) - 1; // set data length (-1 because data len not include itself byte)

  return composingError;
}

//uint8_t ComposeWirelessResponseCommmand(void* p_pRespData, 
//                                        uint16_t p_unCmdId, 
//                                        const ParserComposerEntry* p_pCmdEntry, 
//                                        uint8_t p_ucResponseCode,
//                                        BinaryStream * p_pOutStream)
////                                        const void* outputBuffer, 
////                                        const _command_size_t outputBufferSize, 
////                                        _command_size_t* writenBufferBytes)
//{
//// consider caller function has assured enough out buffer space for header + RespData
//
//  // compose response header as for an error RC
//  STREAM_WRITE_UINT16(p_pOutStream, p_unCmdId);
//  uint8 * pDataLen = p_pOutStream->nextByte;  // retain the position of dataLength byte inside the stream for later use
//  STREAM_WRITE_UINT8(p_pOutStream, 1);        // in case of an error RC, only the RespCode byte will be included
//  STREAM_WRITE_UINT8(p_pOutStream, p_ucResponseCode);
//  if(IsResponseCodeError(p_ucResponseCode))
//    return;
//
//  // get the pointer to response composer function
//  ComposeFunction pComposer;
//  if(p_pCmdEntry)
//    pComposer = p_pCmdEntry->fnComposer;  // binary search was already performed
//  else
//    pComposer = FindResponseComposer(p_unCmdId);
//  if (!pComposer)
//  { // this shouldn't happen if request parsing function was previously found in CommandTable
//    *(pDataLen + 1) = RCS_E64_CommandNotImplemented; // overwite RC byte
//    return;    
//  }  
//  
//  // compose response Data
//  uint8_t ucComposingError = (*pComposer)(p_pRespData, p_pOutStream);
//  if(ucComposingError)
//  { // this could only be RCS_E01_Undefined1 (#ifdef _DEBUG)
//    *(pDataLen + 1) = RCS_E06_DeviceSpecificCommandError; // ??? have no better idea
//    p_pOutStream->nextByte = pDataLen + 2; // when RC is an error, no response data will be added
//    return;    
//  }  
//  
//  // put the right data length in header
//  *pDataLen = (p_pOutStream->nextByte - pDataLen) - 1; // set data length (-1 because data len not include itself byte)
//}

uint8_t ComposeWiredRequestCommmand(void* requestParam, uint16_t cmdId, BinaryStream* pStream)
{
// consider pStream has at least 3 bytes size for CmdId + dataLength
  if(255 >= cmdId)
  {
    STREAM_WRITE_UINT8(pStream, cmdId);
  }
  else STREAM_WRITE_UINT8(pStream, 31);
  
  uint8 * pDataLen = pStream->nextByte; // CmdID + dataLength + data
  
  STREAM_WRITE_UINT8(pStream, 0);       // set data length 0 if cmd not implemented
 
  if(255 < cmdId)
  {
    STREAM_WRITE_UINT16(pStream, cmdId);
  }
  // compose Data
  ComposeFunction composer = FindRequestComposer(cmdId);
  if (!composer)
    return RCS_E64_CommandNotImplemented;

  uint8_t composingError = (*composer)(requestParam,  pStream);
  *pDataLen = (pStream->nextByte - pDataLen) - 1; 
  
  return RCS_N00_Success;
}

uint8_t IsResponseCodeError(uint8_t rc)
{
  return ((1 <= rc && rc <= 7) || (9 <= rc && rc <= 13) || (15 <= rc && rc <= 23)
         || (rc == 28) || (rc == 29) || (32 <= rc && rc <= 95));
}
