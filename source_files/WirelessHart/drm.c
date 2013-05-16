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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name:         drm.c
// Author:       Nivis, LLC
// Date:         September 2009
// Description:  Delayed Response Mechanism module 
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "drm.h"
#include "ParseComposeCommandTables.h"
#include "ParseComposeCommand.h"
#include "transport_layer.h"
#include "app_layer.h" //for g_ucIsEventAck
#include "tmr_util.h"
#include "..\CommonAPI\Common_API.h"

#define APPQUEUE_TIMEOUT   (18UL*32768) // 18s -see TML208B    // ((30UL*32768*75)/100)  // 30s * 75%

#ifdef DR_USE_MORE_BUFFERS
  DR_SLAVE_BUFFER_STRUCT g_aDRSlaveBuf[MAX_DR_SLAVE_BUFFERS];
#else 
  DR_SLAVE_BUFFER_STRUCT g_stDRSlaveBuf;
#endif //DR_USE_MORE_BUFFERS
  
APP_QUEUE_ELEMENT g_aAPPQueue[APP_QUEUE_SIZE];
    
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
void DR_Task()
{
  if(DRSLAVE_START == g_stDRSlaveBuf.m_ucDRStatus)
  {
    if((g_stDRSlaveBuf.m_unCmdId == 109) || (g_stDRSlaveBuf.m_unCmdId == 103))
       API_onRcvSrvcReq((ApiMessageFrame*)g_stDRSlaveBuf.m_aReq);
  }
}
#endif
/*
void DR_Task()
{
#ifdef DR_USE_MORE_BUFFERS
  for(uint8 ucIdx=0; ucIdx<MAX_DR_SLAVE_BUFFERS; ucIdx++)
  {
    if(DRSLAVE_START == g_aDRSlaveBuf[ucIdx].m_ucDRStatus)
    {
      if(g_aDRSlaveBuf[ucIdx].m_unResponseTimer) 
        g_aDRSlaveBuf[ucIdx].m_unResponseTimer--; //TODO: check what to do with this timer maybe -- is not quite ok
      else g_aDRSlaveBuf[ucIdx].m_ucDRStatus = DRSLAVE_INITIATE;
    }    
  }
#else
  if(DRSLAVE_START == g_stDRSlaveBuf.m_ucDRStatus)
  {
    if(g_stDRSlaveBuf.m_unResponseTimer) 
      g_stDRSlaveBuf.m_unResponseTimer--; //TODO: check what to do with this timer maybe -- is not quite ok
    else g_stDRSlaveBuf.m_ucDRStatus = DRSLAVE_INITIATE;
  }
#endif //DR_USE_MORE_BUFFERS
}
*/

DR_SLAVE_BUFFER_STRUCT* findDRBufByCmdId(uint16 p_unCmdId)
{
  #ifdef DR_USE_MORE_BUFFERS
  for(uint8 ucIdx=0; ucIdx<MAX_DR_SLAVE_BUFFERS; ucIdx++)
  {
    if((g_aDRSlaveBuf[ucIdx].m_ucDRStatus) && (p_unCmdId == g_aDRSlaveBuf[ucIdx].m_unCmdId))
      return (g_aDRSlaveBuf + ucIdx);
  }
  #else 
  if((g_stDRSlaveBuf.m_ucDRStatus) && (p_unCmdId == g_stDRSlaveBuf.m_unCmdId))
    return &g_stDRSlaveBuf;
  #endif //DR_USE_MORE_BUFFERS    
  return NULL;
}

DR_SLAVE_BUFFER_STRUCT* DRM_GetInactiveDRBuf(void)
{
  #ifdef DR_USE_MORE_BUFFERS
  for(uint8 ucIdx=0; ucIdx<MAX_DR_SLAVE_BUFFERS; ucIdx++)
  {
    if(!g_aDRSlaveBuf[ucIdx].m_ucDRStatus)
      return (g_aDRSlaveBuf + ucIdx);
  }
  #else 
  if(!g_stDRSlaveBuf.m_ucDRStatus)
    return &g_stDRSlaveBuf;
  #endif //DR_USE_MORE_BUFFERS    
  return NULL;  
}

uint8 CheckReadCmd(uint16 p_unCmdID)
{
  switch (p_unCmdID)
  {
    //TBC
    //common practice read cmds:
    case CMDID_C033_ReadDeviceVariables: 
    case CMDID_C050_ReadDynamicVariableAssignments:
    case CMDID_C054_ReadDeviceVariableInformation:
    case CMDID_C057_ReadUnitTagDescriptorDate:
    case CMDID_C060_ReadAnalogChannelAndPercentOfRange:
    case CMDID_C061_ReadDynamicVariablesAndPrimaryVariableAnalogChannel:
    case CMDID_C062_ReadAnalogChannels:
    case CMDID_C063_ReadAnalogChannelInformation:
    case CMDID_C070_ReadAnalogChannelEndpointValues:
    case CMDID_C074_ReadIOSystemCapabilities:
    case CMDID_C076_ReadLockDeviceState:
    case CMDID_C080_ReadDeviceVariableTrimPoints:
    case CMDID_C081_ReadDeviceVariableTrimGuidelines:
    case CMDID_C084_ReadSubDeviceIdentitySummary:
    case CMDID_C085_ReadIOChannelStatistics:
    case CMDID_C086_ReadSubDeviceStatistics:
    case CMDID_C090_ReadRealTimeClock:
    case CMDID_C091_ReadTrendConfiguration:
    case CMDID_C093_ReadTrend:
    case CMDID_C094_ReadIOSystemClientSideCommunicationStatistics:
    case CMDID_C095_ReadDeviceCommunicationsStatistics:
    case CMDID_C096_ReadSynchronousAction:
    case CMDID_C098_ReadCommandAction:
    case CMDID_C101_ReadSubDeviceToBurstMessageMap:
    case CMDID_C105_ReadBurstModeConfiguration:
    case CMDID_C110_ReadAllDynamicVariables:
    case CMDID_C114_ReadCaughtDeviceVariable:
    case CMDID_C115_ReadEventNotificationSummary:
    case CMDID_C512_ReadCountryCode:
    //data link layer cmds:
    case CMDID_C774_ReadNetworkId:
    case CMDID_C776_ReadNetworkTag:
    case CMDID_C781_ReadDeviceNicknameAddress:
    case CMDID_C783_ReadSuperframeList:
    case CMDID_C784_ReadLinkList:
    case CMDID_C785_ReadGraphList:
    case CMDID_C786_ReadNeighborPropertyFlag:
    case CMDID_C796_ReadTimerInterval:
    case CMDID_C806_ReadHandheldSuperframe:
    case CMDID_C810_ReadJoinPriority:
    case CMDID_C812_ReadPacketReceivePriority:
    case CMDID_C819_ReadBackOffExponent:
    //dev specific wireless only cmds:
    //case CMDID_C64512_ReadWirelessModuleRevision:
    //network layer cmds
    case CMDID_C769_ReadJoinStatus:
    case CMDID_C772_ReadJoinModeConfiguration:
    case CMDID_C782_ReadSessionEntries:
    case CMDID_C800_ReadServiceList:
    case CMDID_C802_ReadRouteList:
    case CMDID_C803_ReadSourceRoute:
    case CMDID_C808_ReadTimeToLive:
    case CMDID_C814_ReadDeviceListEntries:
    case CMDID_C817_ReadChannelBlacklist:
    case CMDID_C822_ReadNetworkAccessMode:
      //physical layer cmds:
    //case CMDID_C798_ReadRadioPower:
    //case CMDID_C804_ReadRadioCCAMode:
    //universal cmds:
    case CMDID_C000_ReadUniqueIdentifier:
    case CMDID_C001_ReadPrimaryVariable:
    case CMDID_C002_ReadLoopCurrentAndPercentOfRange:
    case CMDID_C003_ReadDynamicVariablesAndLoopCurrent:
    case CMDID_C007_ReadLoopConfiguration:
    case CMDID_C008_ReadDynamicVariableClassifications:
    case CMDID_C009_ReadDeviceVariablesWithStatus:
    case CMDID_C011_ReadUniqueIdentifierAssociatedWithTag:
    case CMDID_C012_ReadMessage:
    case CMDID_C013_ReadTagDescriptorDate:
    case CMDID_C014_ReadPrimaryVariableTransducerInformation:
    case CMDID_C015_ReadDeviceInformation:
    case CMDID_C016_ReadFinalAssemblyNumber:
    case CMDID_C020_ReadLongTag:
    case CMDID_C021_ReadUniqueIdentifierAssociatedWithLongTag:
    case CMDID_C048_ReadAdditionalDeviceStatus:
      //wireless app cmds:
    case CMDID_C777_ReadWirelessDeviceInformation:
    case CMDID_C778_ReadBatteryLife:
    case CMDID_C794_ReadUTCTime:
      return 1;
    default:
      return 0;
    
  }
}

uint16 APP_ProcessCmdBuf( uint8         p_ucQueueIdx, //0xFF if called from app transmit indicate, queue element index if called from app queue task
                          uint8*        p_pucCmdNo,
                          uint8         p_ucCmdType,
                          uint8         p_ucDirection,
                          SHORT_ADDR    p_unSrcAddr,
                          BinaryStream* p_pInputStream,
                          BinaryStream* p_pOutputStream) 
{
  
  ParserComposerEntry* pCmdEntry;
  uint16 unCmdId;    // ID of the processed HART command
  uint16 unRespCode;
  uint8 ucCmdNo = 0;
  
  // TODO: adjust allocated space for req/resp structures (use Unit Test to check max sizes)
  // aParsedMsg   [ MAX( sizeof(Cxxx_..._Req), sizeof(Cxxx_..._Resp) )]  --> 100 bytes for Cmd78 !!!to be reworked for memory optimization!!!
  uint8 aParsedMsg[128];  // RAM allocated for Cxxx_..._Req/Resp structure to be used by parser & executer/analyzer
  
  // aRespData [ MAX( sizeof(Cxxx_..._Resp) )]  --> 116 bytes for Cmd787
  uint8 aRespData[128];  // RAM allocated for Cxxx_..._Resp structure to be used by req executer & resp composer

  uint8 full = 0; // =1 means no more commands have to be parsed in this packet
  uint8 ucMinRecApduSize;
  if( WIRED_CMD == p_ucCmdType)
    ucMinRecApduSize = 2;
  else
    ucMinRecApduSize = (p_ucDirection == CD_REQUEST) ? 3 : 4;
 
  
  // TL has already checked that the received APDU has the min. requested size
  while(!full && (STREAM_GetRemainingSize(p_pInputStream) >= ucMinRecApduSize))
  {
    uint8 ucWiredFlag = 0;
    DR_SLAVE_BUFFER_STRUCT* pDRBuf = NULL; 
      
    //remember where input started in case it is needed to copy payld to dr.
    //p_pInputStream will be modified by ParseWirelessCommmand call
    uint8_t* pStart = p_pInputStream->nextByte; 
    if( WIRELESS_CMD == p_ucCmdType)
      unRespCode = ParseWirelessCommmand(p_ucDirection, 
                                         p_pInputStream, 
                                         (void*)aParsedMsg, 
                                         &unCmdId, 
                                         &pCmdEntry);
    else
    {
      STREAM_READ_UINT8(p_pInputStream, &unCmdId);
      if(31==unCmdId)
        ucWiredFlag = 1;
      STREAM_SKIP(p_pInputStream, 1); //byte count not needed in parser
      unRespCode = ParseWiredCommand( p_pInputStream,
                                      (void*)aParsedMsg,
                                      &unCmdId,
                                      &pCmdEntry
                                    );
      if(APP_QUEUE_SIZE != p_ucQueueIdx) //call does not come from a 78 context
        full = 1; //wired apdu has only one command
    }   
    
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )  
  
    // full API: Cmds tunneling  
    
    if( !APP_IsCmdForMe(unCmdId) )
    { // forward the command to the user board 
      // Cmd not for me, send it to the user board via API interface
        
        if(WIRED_CMD == p_ucCmdType)
        {
          uint8 aTunnelBuffer[MAX_APDU_SIZE];  
          uint8 aTunnelBufferSize = 3 + *(pStart+1); // 1(dev status) + 1(cmdId) + bc + [2(ext cmd id))
          if(ucWiredFlag) 
            aTunnelBufferSize += 2;
          aTunnelBuffer[0] = g_stProv.aDeviceStatus[p_unSrcAddr];
          memcpy(aTunnelBuffer+1, pStart, aTunnelBufferSize - 1);
          if( SendMessage( USER_BOARD_COMMANDS, 
                           DAQ_TUNNEL_WIRED,   // msg type ???
                           API_REQ, 
                           0,   // msg id ???
                           aTunnelBufferSize, // 1(dev status) + 1(cmdId) + bc + [2(ext cmd id))
                           aTunnelBuffer ) )
          {
            unRespCode = RCS_E61_NoBuffersAvailable;
          }
          
        }
        else 
        {
          if( SendMessage( USER_BOARD_COMMANDS, 
                           DAQ_TUNNEL_WIRELESS,   // msg type ???
                           API_REQ, 
                           0,   // msg id ???
                           3 + *(pStart+2), // 2(cmdId) + 1(bc) + bc
                           pStart ) )
          { // SPI queue is full, discard the message
            unRespCode = RCS_E61_NoBuffersAvailable;
          }
        }
    
        if(RCS_E61_NoBuffersAvailable != unRespCode) //cmd succesfuly sent through spi
        {
          if(0xff == p_ucQueueIdx)
          {
            if(NULL !=  APP_getQueueElement())
              return RCS_N258_ReturnToAppQueue;  
            unRespCode = RCS_E61_NoBuffersAvailable;
          }
          else 
            return RCS_N258_ReturnToAppQueue;  
        }
             
    }
  
    else //cmd is for me
#endif
    {  
      if( (APP_QUEUE_SIZE == p_ucQueueIdx)  && (RCS_E64_CommandNotImplemented != unRespCode))//call comes from cmd 78 executer
      {
        if(!CheckReadCmd(unCmdId))
          unRespCode =  RCM_E09_InvalidCommandRequested;
        if(++ucCmdNo == *p_pucCmdNo)
          full = 1; // ignore further commands in this packet
      }
    } // end if(command for me) 
    
    switch (p_ucDirection)
    {
      case CD_REQUEST:
        {
          if(!unRespCode)
          { // request was parsed successfully
            uint8 ucDoExecute = 1;
            
            // exception for Cmd119
            if( unCmdId == 119 )
              g_ucIsEventAck = 1;
           
            //check DR buf same cmd id 
            pDRBuf = findDRBufByCmdId(unCmdId);
  //check dr
            if(NULL != pDRBuf)
            {  
              if(p_unSrcAddr != pDRBuf->m_unSrcAddr)
              {
                unRespCode = RCS_E36_DelayedResponseConflict;
                ucDoExecute = 0;
              }
              else
              {
                if(!memcmp( pDRBuf->m_aReq, 
                            pStart, 
                            p_pInputStream->nextByte-pStart )
                  )               
                {
                  switch(pDRBuf->m_ucDRStatus)
                  {        
                    case DRSLAVE_START:
                      {
                        if((WIRELESS_CMD == p_ucCmdType) && (0xff != p_ucQueueIdx)) //must come from appqueue in a wireless context
                        {
                          if((RTC_COUNT - g_aAPPQueue[p_ucQueueIdx].m_ulTimeout) < APPQUEUE_TIMEOUT)
                          {
                            p_pInputStream->nextByte = pStart; 
                            return RCS_N256_ReturnWithDR;
                          }
                        }
                        //else pDRBuf->m_ucDRStatus = DRSLAVE_INITIATE;
                      }
                    case DRSLAVE_INITIATE:
                      unRespCode = RCS_E33_DelayedResponseInitiated;
                      pDRBuf->m_ucDRStatus = DRSLAVE_RUNNING;
                      ucDoExecute = 0;
                      break;
                    case DRSLAVE_RUNNING:
                      unRespCode = RCS_E34_DelayedResponseRunning;
                      ucDoExecute = 0;
                      break;
                    case DRSLAVE_END:
                      unRespCode = RCS_N257_ReturnFromDR;
                      pDRBuf->m_ucDRStatus = DRSLAVE_INACTIVE;
                      ucDoExecute = 0;
                      break;
                    case DRSLAVE_DEAD:
                      unRespCode = RCS_E35_DelayedResponseDead;
                      memset(pDRBuf, 0, sizeof(DR_SLAVE_BUFFER_STRUCT));
                      //pDRBuf->m_ucDRStatus = DRSLAVE_INACTIVE;
                      ucDoExecute = 0;
                      break;
                    case DRSLAVE_CONFLICT:
                      unRespCode = RCS_E36_DelayedResponseConflict;
                      memset(pDRBuf, 0, sizeof(DR_SLAVE_BUFFER_STRUCT));
                      //pDRBuf->m_ucDRStatus = DRSLAVE_INACTIVE;
                      ucDoExecute = 0;                  
                    default:
                      break;
                  }
                }
                else
                {
#ifdef DRM_REDESIGN                   
                  if((CMDID_C109_BurstModeControl != unCmdId) &&
                     (CMDID_C118_EventNotificationControl != unCmdId))
#endif                    
                  {
                    unRespCode = RCS_E36_DelayedResponseConflict;
                    ucDoExecute = 0;             
                  }
                }
  
              }
            }
  //~check dr    
          
            
            if(ucDoExecute)
            {
              ExecuteFunction exec = (ExecuteFunction)pCmdEntry->fnExecCmd;
              if(!exec)
              {
                unRespCode = RCS_E64_CommandNotImplemented;
              }
              else
              {
                // if this isn't the last command, make sure there is enough space to return
                //  a "payload too long" error further on (if necessary)
                uint8 ucReservedOutBytes = (STREAM_GetRemainingSize(p_pInputStream) >= ucMinRecApduSize) ? (4 + 4) : 4;   // 2 for cmdID, 1 for dataLen, 1 for respCode
                if(STREAM_GetRemainingSize(p_pOutputStream) < (ucReservedOutBytes + pCmdEntry->ucMinRespSize))
                {
                  unRespCode = RCS_E60_PayloadTooLong;
                  full = 1; // ignore further commands in this packet
                }
                else
                {
                  unRespCode = (*exec)((void*)aParsedMsg, (void*)aRespData,
                                       p_unSrcAddr, p_ucCmdType,
                                       (STREAM_GetRemainingSize(p_pOutputStream) - ucReservedOutBytes) );
      // CHECKME - maybe TL needs to know somehow that no response shall be transmitted
                  if(unRespCode == RCS_N255_MakeNoResponse)
                    continue;   // ??? this request will return no response!!! (e.g. C011, C021)
                  
                  if((RCS_N256_ReturnWithDR == unRespCode))
                  {
                    if((WIRELESS_CMD == p_ucCmdType) && (NULL == APP_getQueueElement()))
                      unRespCode = RCS_E32_Busy;
                    else  
                    {
                      pDRBuf = DRM_GetInactiveDRBuf();
                      if(NULL != pDRBuf)
                      {
                        //add req  & size to g_stDRSlaveBuf
                        memcpy( pDRBuf->m_aReq, 
                                pStart, 
                                p_pInputStream->nextByte-pStart );             
                        pDRBuf->m_unSrcAddr = p_unSrcAddr;
                        pDRBuf->m_unCmdId = unCmdId;
                        
                        
                        if(CMDID_C077_SendCommandToSubDevice == unCmdId)
                        {
                          pDRBuf->m_ucRespSize = p_pInputStream->nextByte-pStart;
                          memcpy(pDRBuf->m_aResp, pDRBuf->m_aReq, pDRBuf->m_ucRespSize);
                          //the rest of the response will be added from apphart
                        }
                        
                        if(WIRELESS_CMD == p_ucCmdType)
                        {
                          pDRBuf->m_ucDRStatus = DRSLAVE_START;
                          p_pInputStream->nextByte = pStart; 
                          return RCS_N256_ReturnWithDR;
                        //TODO when exited with this code from app transmit indicate, must add to queue
                        }
                        else
                        {
                          pDRBuf->m_ucDRStatus = DRSLAVE_RUNNING;
                          unRespCode = RCS_E33_DelayedResponseInitiated;
                        }
                        
                      }
                      else unRespCode = RCS_E32_Busy;
                    }
                  //if(NULL == pDRBuf)
                    //unRespCode = RCS_E32_Busy;
                  } //endif(RCS_N256_ReturnWithDR == unRespCode)             
                }
              }
            }//if ucDoExecute
          }
          else
          { // request parse error
            if(unRespCode == RCS_N255_MakeNoResponse)
              continue;   // ??? this request will return no response!!! (e.g. C011, C021)            
  #ifdef _DEBUG
            while(unRespCode == RCS_E01_Undefined1)
              FEED_WDT();   // infinite loop, for testing only!!!
  #endif
            if(STREAM_GetRemainingSize(p_pInputStream) >= ucMinRecApduSize)
            {
              // if this isn't the last command and there's no more space to return a "payload too long" error
              //  further on, return payload too long instead of parsing error
              if(STREAM_GetRemainingSize(p_pOutputStream) < (4 + 4))
              {
                unRespCode = RCS_E60_PayloadTooLong;
                full = 1;
              }
            }
            else
            {
              full = 1;
            }
          }
          
          if((RCS_N257_ReturnFromDR == unRespCode) && (NULL != pDRBuf))         
          {
            //unRespCode = pDRBuf->m_ucRC;
            memcpy(p_pOutputStream->nextByte, pDRBuf->m_aResp, pDRBuf->m_ucRespSize);
            p_pOutputStream->nextByte += pDRBuf->m_ucRespSize;
            //clean up!!!
            memset(pDRBuf, 0, sizeof(DR_SLAVE_BUFFER_STRUCT));
          }
          else
          {
          //TODO: check if  must add p_unSrcAddr to  p_unSrcAddrIdx transformation
            if(ucWiredFlag)
              unCmdId = 31;
            ComposeResponseToCommmand( p_unSrcAddr,                              
                                      (void*)aRespData, 
                                      unCmdId, 
                                      pCmdEntry, 
                                      unRespCode,
                                      p_pOutputStream
                                      );
          }
        }
        break;
  
  
      case CD_RESPONSE:
        { // analyze the received un-solicited (published) command response
          if(unRespCode == RCS_E01_Undefined1 ||
             unRespCode == RCS_E05_TooFewDataBytesReceived ||
             unRespCode == RCS_E64_CommandNotImplemented)
          { // parsing error, 
            // nothing to do with this received response message
          }
          else
          {
          ////////TODO: CD_RESPONSE    
            ExecuteFunction analyze = (ExecuteFunction)pCmdEntry->fnExecCmd;
            if(analyze)
            {
              //TODO: define response analyze functions for the requested commands -maybe with a changed prototype 
              unRespCode = (*analyze)((void*)aParsedMsg,  // received response -parsed data 
                                      (void*)aRespData,   // empty buffer
                                      p_unSrcAddr, 
                                      WIRELESS_CMD,
                                      unRespCode          // received RC
                                        );
              
              if(unRespCode)
              {
                //TODO: manage the analyzer's returned code 
              }
            }
          }
        }
        break;
      } // end switch(direction)   
     
  } // end while()
  
  if(unRespCode == RCS_N255_MakeNoResponse)
    return RCS_N255_MakeNoResponse;
  
  if( (APP_QUEUE_SIZE == p_ucQueueIdx)  && (RCS_E64_CommandNotImplemented != unRespCode))//call comes from cmd 78 executer
    *p_pucCmdNo = ucCmdNo;
  return RCS_N00_Success; //when exit from app transmit send request, //when exit from app task, send request and free queue element
}


void APP_QueueTask()
{
  uint8 ucIdx = 0;
  for(; ucIdx < APP_QUEUE_SIZE; ucIdx++)
  {
    if(AQE_USED == g_aAPPQueue[ucIdx].m_ucStatus)
      break;
    
    if((APP_QUEUE_SIZE-1) == ucIdx) 
     return;
  }
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )      
  if(g_aAPPQueue[ucIdx].m_unSrcAddr <= 1) 
  {
    DLLHART_TRANSMIT_Response(g_aAPPQueue[ucIdx].m_ucRspCrtPos, g_aAPPQueue[ucIdx].m_aRspBuffer);
    memset(g_aAPPQueue+ucIdx, 0, sizeof(APP_QUEUE_ELEMENT));
    return;    
  }
#endif  
  
  BinaryStream stInputStream;
  STREAM_INIT(&stInputStream, g_aAPPQueue[ucIdx].m_aReqBuffer + g_aAPPQueue[ucIdx].m_ucReqCrtPos, g_aAPPQueue[ucIdx].m_ucReqSize - g_aAPPQueue[ucIdx].m_ucReqCrtPos);
  
  BinaryStream stOutputStream;
  STREAM_INIT(&stOutputStream, g_aAPPQueue[ucIdx].m_aRspBuffer + g_aAPPQueue[ucIdx].m_ucRspCrtPos, sizeof(g_aAPPQueue[ucIdx].m_aRspBuffer) - g_aAPPQueue[ucIdx].m_ucRspCrtPos);
  uint8 ucCmdType = g_aAPPQueue[ucIdx].m_unSrcAddr > 1 ? WIRELESS_CMD : WIRED_CMD;
  
  uint16 unRspCode = APP_ProcessCmdBuf(ucIdx,
                                       0,
                                       ucCmdType,
                                       g_aAPPQueue[ucIdx].m_ucDirection,
                                       g_aAPPQueue[ucIdx].m_unSrcAddr,
                                       &stInputStream,
                                       &stOutputStream
                                      );
  

  if((RCS_N256_ReturnWithDR != unRspCode)
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
     &&(RCS_N258_ReturnToAppQueue!=unRspCode)
#endif   
    )
  {
    //g_stC779.m_unNoOfPacketsTerminatedByDevice++; //TODO check if this shit must be incremented here
    if(RCS_N255_MakeNoResponse != unRspCode)
      //TODO check if should use switch(g_aAPPQueue[ucIdx].m_ucTransportType) just as in app transmit indicate.
      TL_TransmitRsp( g_aAPPQueue[ucIdx].m_hHandle,
                      stOutputStream.nextByte - g_aAPPQueue[ucIdx].m_aRspBuffer,
                      g_aAPPQueue[ucIdx].m_aRspBuffer,
                      g_aAPPQueue[ucIdx].m_ucSequenceNumber,
                      0/*dummy service ID*/ ); 
    else
      TL_TransmitRsp(g_aAPPQueue[ucIdx].m_hHandle, 0, g_aAPPQueue[ucIdx].m_aRspBuffer, g_aAPPQueue[ucIdx].m_ucSequenceNumber, 0/*dummy service ID*/);
  

    memset(g_aAPPQueue+ucIdx, 0, sizeof(APP_QUEUE_ELEMENT));
    return;
  }
  g_aAPPQueue[ucIdx].m_ucReqCrtPos = stInputStream.nextByte - g_aAPPQueue[ucIdx].m_aReqBuffer;
  g_aAPPQueue[ucIdx].m_ucRspCrtPos = stOutputStream.nextByte - g_aAPPQueue[ucIdx].m_aRspBuffer;
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )  
  if(RCS_N258_ReturnToAppQueue == unRspCode)
     g_aAPPQueue[ucIdx].m_ucStatus = AQE_PENDING; 
#endif  
}


APP_QUEUE_ELEMENT* APP_getQueueElement()
{
  uint8 ucIdx = 0;
  for(; ucIdx < APP_QUEUE_SIZE; ucIdx++)
  {
     if(AQE_FREE == g_aAPPQueue[ucIdx].m_ucStatus)
       return (g_aAPPQueue+ucIdx);
  }
 
  return NULL;
}

#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
APP_QUEUE_ELEMENT* APP_getPendingQueueElement()
{
  uint8 ucIdx = 0;
  for(; ucIdx < APP_QUEUE_SIZE; ucIdx++)
  {
     if(AQE_PENDING == g_aAPPQueue[ucIdx].m_ucStatus)
       return (g_aAPPQueue+ucIdx);
  }
 
  return NULL; 
}
#endif
