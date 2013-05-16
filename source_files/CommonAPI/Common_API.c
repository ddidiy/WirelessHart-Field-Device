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

/***************************************************************************************************
 * Name:         Common_API.c
 * Author:       NIVIS LLC, John Chapman and Kedar Kapoor
 * Date:         July 2008
 * Description:  This file has the Common Radio routines for the RF Modem side of the common API.
 *               This also is the API for the single processor solution.  This routine will call
 *               into the serial port to grab data.
 * Changes:
 * Revisions:
 ***************************************************************************************************/

#include "Common_API.h"
#include "../WirelessHart/util/Binarization.h"
#include "../WirelessHart/uap_data.h"
#include "../WirelessHart/app_layer.h"
#include "../WirelessHart/app_cmdreq.h"
#include "../global.h"
#include "DAQ_Comm.h"
#include <string.h>

#if ( _UAP_TYPE != 0 )

//void API_onRcvUAPAlarm( uint8 * p_pRcvData, uint8 p_ucMsgLen, uint8 * p_pucAlertID );
  
#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
  uint8 API_onRcvDataPassTrough( ApiMessageFrame * p_pMsgFrame );
#endif
  
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
typedef struct{
  uint8 msgId;
  uint8 msgDone;
}LastSPIMsg;
static LastSPIMsg gLastMsg = {0, 0};

  uint8 API_onRcvStackSpecific( ApiMessageFrame * p_pMsgFrame );
#endif

uint8 API_onRcvApiCommands( ApiMessageFrame * p_pMsgFrame );

typedef void (*API_FCT_OnRcv)(ApiMessageFrame * p_pMsgFrame);

/****************
 * SendMessage
 ****************/
enum SEND_ERROR_CODE SendMessage(enum MSG_CLASS p_MsgClass,
                                 uint8 p_MsgType,
                                 uint8 p_ucIsRsp,
                                 uint8 p_ucMSG_ID, 
                                 uint8 p_ucBuffSize,
                           const uint8 *p_pMsgBuff)
{    
    static uint8 s_ucMsgId;
    ApiMessageFrame MessageFrame; 

    if( p_ucBuffSize > (MAX_API_BUFFER_SIZE - sizeof(ApiMessageFrame)) )
    {
        return ER_UNSUPPORTED_DATA_SIZE;
    }
    
    if( !p_ucIsRsp )
    {
        p_ucMSG_ID = (s_ucMsgId++) | 0x80;
    }
    
    MessageFrame.MessageHeader.MessageClass = p_MsgClass;
    MessageFrame.MessageHeader.m_bIsRsp = p_ucIsRsp;    
    MessageFrame.MessageHeader.Reserved = 0;    
    MessageFrame.MessageType = p_MsgType;
    MessageFrame.MessageID = p_ucMSG_ID;        
    MessageFrame.MessageSize = p_ucBuffSize;
    
 
    if (SUCCESS != API_PHY_Write(&MessageFrame, p_pMsgBuff )){
        return ER_QUEUE_FULL;
    }
    
    return NO_ERR;
}

#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
uint8 API_onRcvDataPassTrough( ApiMessageFrame * p_pMsgFrame )
{
    uint8 * pRcvData = (uint8 *)(p_pMsgFrame + 1); // skip the header       
    
    if( p_pMsgFrame->MessageHeader.m_bIsRsp )
    {
        if (p_pMsgFrame->MessageType == READ_DATA_RESP )
        {
            while(p_pMsgFrame->MessageSize >= 5 )
            {
              #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
                if(pRcvData[0] >= DIGITAL_DATA_ATTR_ID_OFFSET)
                {
                    UAPDATA_SetDigitalVal( UAP_DATA_DIGITAL1 + pRcvData[0] - DIGITAL_DATA_ATTR_ID_OFFSET, pRcvData[4] );                    
                }
                else 
              #endif    
                {
                    float fTmp;
                    WriteUint32( &fTmp,  pRcvData + 1, 4 );
                    
                    #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
                      UAPDATA_SetAnalogVal(UAP_DATA_TEMP, UAP_DATA_TEMP + pRcvData[0] - ANALOG_DATA_ATTR_ID_OFFSET, fTmp ); 
                    #elif (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)
                      UAPDATA_SetAnalogVal(UAP_DATA_LOCAL_ANALOG_INPUT, 
                                           UAP_DATA_LOCAL_ANALOG_INPUT + pRcvData[0] - ANALOG_DATA_ATTR_ID_OFFSET - LOCAL_ANALOG_CH_NO, fTmp );
                    #endif
                }
                
                p_pMsgFrame->MessageSize -= 5;
                pRcvData += 5;                
            }
            return 1;
        }
    }
    return 0;
}
#endif  //  _UAP_TYPE == UAP_TYPE_SIMPLE_API  

#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
uint8 API_onRcvStackSpecific( ApiMessageFrame * p_pMsgFrame )
{
    uint8 * pRcvData = (uint8 *)(p_pMsgFrame + 1); // skip the header
    
    if( !p_pMsgFrame->MessageHeader.m_bIsRsp )
    {
        switch (p_pMsgFrame->MessageType)
        {
            // Generic Read/Write attributes
            case HART_GET_ASN:
                // TBD 9801
                SendMessage(STACK_SPECIFIC, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, 4, pRcvData );
                break;
            
            case HART_JOIN_STATUS:
                // TBD 9801
                SendMessage(STACK_SPECIFIC, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, 1, NULL /* &g_ucJoinStatus */ );
                break;
            
            case HART_SERVICE_REQUEST:
              {
                API_onRcvSrvcReq( p_pMsgFrame );
              }
              break;
                
            case HART_SERVICE_DELETE:
              {
                API_onRcvSrvcDel( p_pMsgFrame );
              }
              break;
              
            default:
                return 0;
        }  
    }
    else
    {
        switch (p_pMsgFrame->MessageType)
        {      
            // Custom/Specialized attributes
            case HART_GET_INITIAL_INFO: // power type, power level, uap ID, routing type
                // TBD 9801
                g_ucUapId = 1; // something != 0, TBD
                API_onRcvGetInitialInfo(p_pMsgFrame);
                g_ucInitAPI = 1;
                break;
                
            case HART_POWER_LEVEL:
                // TBD 9801
                break;
            
            default:
                return 0;
        }  
    }
    
    return 1;
}

uint8 API_onRcvGetInitialInfo(ApiMessageFrame * p_pMsgFrame)
{
  //uint8   ucMessageSize;
  uint8 * pRcvData = (uint8 *)(p_pMsgFrame + 1); // skip the header
  g_stProv.ucMaxNoOfDeviceVars =  *pRcvData;
  //to be continued
  return 1;
}

uint8 API_onRcvSrvcReq(ApiMessageFrame * p_pMsgFrame)
{
  //uint8   ucMessageSize;
  uint8 * pRcvData = (uint8 *)(p_pMsgFrame + 1); // skip the header
   
  if( p_pMsgFrame->MessageHeader.m_bIsRsp )
  {
    return 0;
  }
  else
  { 

    uint16 unCmdId =  (*pRcvData << 8) | *(pRcvData+1);
    uint8 ucSrvcId = *(pRcvData+2) + 1;
    uint32 ulPeriod = (*(pRcvData+3) << 24) | 
                      (*(pRcvData+4) << 16) |
                      (*(pRcvData+5) << 8) |
                      *(pRcvData+6);
    uint16 unRetCode = Process_SrvcReq(unCmdId, ucSrvcId, &ulPeriod);
    if(RCS_N255_MakeNoResponse == unRetCode)
    {
      DR_SLAVE_BUFFER_STRUCT* pDRBuf = findDRBufByCmdId(unCmdId);
      if(pDRBuf)
        memcpy(pDRBuf->m_aReq, p_pMsgFrame, sizeof(ApiMessageFrame)+7);
      return 1;
    }
    uint8 outBuf[30];
    outBuf[0] = unCmdId >> 8;
    outBuf[1] = unCmdId;
    outBuf[2] = unRetCode;
    outBuf[3] = ucSrvcId-1;
    uint8 outBufSize = 4;
    if((unRetCode == RCS_N00_Success) || (unRetCode == RCS_N257_ReturnFromDR))
    {
      outBuf[2] = RCS_N00_Success;
      
      outBuf[4] = ulPeriod >> 24;
      outBuf[5] = ulPeriod >> 16;
      outBuf[6] = ulPeriod >> 8;
      outBuf[7] = ulPeriod;
      outBufSize = 8;
    }
    SendMessage(STACK_SPECIFIC, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, outBufSize, outBuf);
  } 
  
  return 1;  
}

uint8 API_onRcvSrvcDel(ApiMessageFrame * p_pMsgFrame)
{
  //uint8   ucMessageSize;
  uint8 * pRcvData = (uint8 *)(p_pMsgFrame + 1); // skip the header
   
  if( p_pMsgFrame->MessageHeader.m_bIsRsp )
  {
    return 0;
  }
  else
  { 
    //uint16 unCmdId =  (*pRcvData << 8) | *(pRcvData+1);
    uint8 ucSrvcId = *(pRcvData+2) + 1;
    uint8 ucRetCode = Process_SrvcDel(ucSrvcId);

    /*uint8 outBuf[30];
    outBuf[0] = unCmdId >> 8;
    outBuf[1] = unCmdId;
    outBuf[2] = ucRetCode;
    uint8 outBufSize = 3;    
    SendMessage(STACK_SPECIFIC, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, outBufSize, outBuf);
    */
    SendMessage(API_ACK, DATA_OK, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
  }
  return 1;
}

uint8 API_onRcvHartCommands( ApiMessageFrame * p_pMsgFrame )
{
  //uint8   ucMessageSize;
  uint8 * pRcvData = (uint8 *)(p_pMsgFrame + 1); // skip the header
  static uint8 cntFails=0;
  
  switch (p_pMsgFrame->MessageType)
  {
    case DAQ_BURST_PIPE_0: 
    case DAQ_BURST_PIPE_1:
    case DAQ_BURST_PIPE_2:
   // case DAQ_EVENT_PIPE_0:  
      if( !p_pMsgFrame->MessageHeader.m_bIsRsp )
      {
           
        if((p_pMsgFrame->MessageID != gLastMsg.msgId) || !gLastMsg.msgDone)
        {
          gLastMsg.msgId = p_pMsgFrame->MessageID;
          gLastMsg.msgDone = 0;
           if(SUCCESS == TL_TransmitRequest( g_unBurstAndEventsHandle,
                               NET_ADDR_SHORT_ADDR,
                               c_aGatewayAddr,
                               (p_pMsgFrame->MessageType <= MAX_BURST_NO) ? DLL_PACKET_PRIORITY_DATA : DLL_PACKET_PRIORITY_ALARM,
                               p_pMsgFrame->MessageType,
                               PUBLISH_NOTIFY,
                               p_pMsgFrame->MessageSize,
                               pRcvData
                             ))
           { 
              SET_BURST_EVENT_HANDLER(g_unBurstAndEventsHandle); 
              // send ACK(pt achitare)
              //SendMessage(API_ACK, DATA_OK, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
              gLastMsg.msgDone = 1; 
           }
           else
           {
             
             cntFails++;
           }
          
        }
         SendMessage(API_ACK, DATA_OK, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
         return 1;
      }
      break;
    case  DAQ_TUNNEL_WIRELESS:
      case  DAQ_TUNNEL_WIRED:
      {
        if((p_pMsgFrame->MessageID != gLastMsg.msgId) || !gLastMsg.msgDone)
        {
          gLastMsg.msgId = p_pMsgFrame->MessageID;
          gLastMsg.msgDone = 0;
          APP_QUEUE_ELEMENT* pQueue = APP_getPendingQueueElement();
          if(NULL != pQueue)
          {
            memcpy(pQueue->m_aRspBuffer + pQueue->m_ucRspCrtPos, 
                   pRcvData, 
                   p_pMsgFrame->MessageSize);
            pQueue->m_ucRspCrtPos += p_pMsgFrame->MessageSize;
            pQueue->m_ucStatus = AQE_USED;
            gLastMsg.msgDone = 1;
            
          }   
        }
        if(!p_pMsgFrame->MessageHeader.m_bIsRsp)
          SendMessage(API_ACK, DATA_OK, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
      }
      break;
      // must never happen
      default: return 0;
        
  }
  return 1;
}

#endif

uint8 API_onRcvApiCommands( ApiMessageFrame * p_pMsgFrame )
{
    uint8 * pRcvData = (uint8*)(p_pMsgFrame+1);
    if( !p_pMsgFrame->MessageHeader.m_bIsRsp ) // is request
    {
        switch (p_pMsgFrame->MessageType)
        {
        case API_HW_PLATFORM:
            pRcvData[0] = API_PLATFORM >> 8;
            pRcvData[1] = API_PLATFORM;
            SendMessage(API_COMMANDS, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, 2, pRcvData );
            break;        
          
        case API_FW_VERSION:
            pRcvData[0] = API_VERSION >> 8;
            pRcvData[1] = API_VERSION;
            SendMessage(API_COMMANDS, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, 2, pRcvData );
            break;        
          
        case API_MAX_BUFFER_SIZE: 
            pRcvData[0] = 0;
            pRcvData[1] = MAX_API_BUFFER_SIZE;
            SendMessage(API_COMMANDS, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, 2, pRcvData );
            break;        
    
    #if (SPI1_MODE != NONE)
            
        case API_MAX_SPI_SPEED:
            pRcvData[0] = MAX_SPI_SPEED;
            SendMessage(API_COMMANDS, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, 1, pRcvData );
            break;        
          
        case API_UPDATE_SPI_SPEED:
            if (API_PHY_SetSpeed((enum SPI_SPEED)pRcvData[0]) == SUCCESS){
                SendMessage(API_ACK, API_CHANGE_ACCEPTED, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
            }
            else {
                SendMessage(API_NACK, NACK_API_COMMAND_ERROR, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
            }
            break;
    
      #ifndef WAKEUP_ON_EXT_PIN          
        case API_HEARTBEAT_FREQ:
            if (DAQ_UpdateHeartBeatFreq((enum HEARTBEAT_FREQ)pRcvData[0]) == SUCCESS){
                SendMessage(API_ACK, API_CHANGE_ACCEPTED, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
            }
            else {
                SendMessage(API_NACK, NACK_API_COMMAND_ERROR, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
            }
            break;            
      #endif
            
    #endif
    
    #if ( UART2_MODE != NONE )
        case API_UPDATE_UART_SPEED:
            if( UART2_UpdateSpeed( pRcvData[0] ) == SUCCESS )
            {
                SendMessage(API_ACK, API_CHANGE_ACCEPTED, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
            }
            else {
                SendMessage(API_NACK, NACK_API_COMMAND_ERROR, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
            }
            break;
            
        case API_MAX_UART_SPEED:
            pRcvData[0] = MAX_UART_SPEED;
            SendMessage(API_COMMANDS, p_pMsgFrame->MessageType, API_RSP, p_pMsgFrame->MessageID, 1, pRcvData );
            break;        
    #endif
            
        default:
            return 0;
        }
    }
    else // is response
    {
        switch (p_pMsgFrame->MessageType)
        {        
               
          default:
               return 0;
        }
    }
    
    return 1;
}

/****************
 * GetMessage
 ****************/
void API_OnRcvMsg( ApiMessageFrame * p_pMsgFrame )
{
    uint8 ucParseFlag = 0;
    switch( p_pMsgFrame->MessageHeader.MessageClass )
    {
#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
    case DATA_PASSTHROUGH: ucParseFlag = API_onRcvDataPassTrough( p_pMsgFrame );  break;
#endif    
    
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
    case STACK_SPECIFIC:   ucParseFlag = API_onRcvStackSpecific( p_pMsgFrame ); break;
    
    case USER_BOARD_COMMANDS: ucParseFlag = API_onRcvHartCommands( p_pMsgFrame ); break;  
#endif
    
    case API_COMMANDS:     ucParseFlag = API_onRcvApiCommands( p_pMsgFrame ); break;
    }
    
    if( !ucParseFlag && !p_pMsgFrame->MessageHeader.m_bIsRsp )
    {
        SendMessage(API_NACK, NACK_UNSUPPORTED_FEATURE, API_RSP, p_pMsgFrame->MessageID, 0, NULL);
    }
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @author NIVIS LLC, Eduard Erdei
/// @brief  Parses a recieved alarm through ISA API interface, and adds the alarm to the ARMO queue
/// @param  p_pRcvData - pointer to a buffer that containss concatenated APDUs
/// @param  p_ucMsgLen - lenght of the p_pRcvData buffer
/// @param  p_pucAlertID - pointer to a buffer where to put API response
/// @return none
/// @remarks
///      Access level: user level
////////////////////////////////////////////////////////////////////////////////////////////////////
void API_onRcvUAPAlarm( uint8 * p_pRcvData, uint8 p_ucMsgLen, uint8 * p_pucAlertID )
{
    // TBD 9801
}
*/

#endif // _UAP_TYPE
