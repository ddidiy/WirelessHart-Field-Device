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
// Name:         uart_link.c
// Author:       Nivis LLC, Ion Ticus
// Date:         January 2008
// Description:  Implements uart link layer
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef BACKBONE_SUPPORT

#include <string.h>

#include "../typedef.h"
#include "../uart1.h"
#include "../itc.h"

#include "uart_link.h"
#include "provision.h"
#include "network_layer_dm.h"
#include "network_layer.h"
#include "datalink_layer_dm.h"
#include "tmr_util.h"

  #define TX_MAX_RETRY_CNT      3
  #define TX_ACK_WAIT_TIME      50  //25
  #define TX_MAX_TO_BBR_MSG_NO  8

  #define TO_BBR_MSG_MAX_SIZE  (UART1_PACKET_LENGTH-3)

  #define UART_TASK_FREQ USEC_TO_FRACTION2( 10000 ) // each 10ms

  volatile uint8 g_aTxMessagesLen[ TX_MAX_TO_BBR_MSG_NO ];
  uint8 g_aTxMessagesTimer[ TX_MAX_TO_BBR_MSG_NO ];
  uint8 g_aTxMessagesRetry[ TX_MAX_TO_BBR_MSG_NO ];
  uint8 g_aToBBRPayloads[ TX_MAX_TO_BBR_MSG_NO ][TO_BBR_MSG_MAX_SIZE];

  uint8  g_ucWaitAck = 0;
  uint8  g_ucBBRFailedResp = 0;
  uint8  g_ucNetInfoReceived = 0;
  uint16 g_unNextIdlePk = 0;

  uint32 ulInitSec;
  uint32 ulInitSecFraction;
  
  uint8 UART_LINK_sendMsgToBackbone ( const uint8 * p_pMsg,  uint8 p_ucMsgLen );  

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: UART_LINK_Task
  // Author: NIVIS LLC, Ion Ticus
  // Description: read messages from UART and pending messages to UART
  // Parameters:
  // Return: None
  // Obs:
  //      Access level: user level
  ///////////////////////////////////////////////////////////////////////////////////
  void UART_LINK_Task ( void )
  {
    
    for(uint8 ucMsgIdx = 0; ucMsgIdx < TX_MAX_TO_BBR_MSG_NO; ucMsgIdx++ )
    {
        uint8 ucMsgLen = g_aTxMessagesLen[ucMsgIdx]; 
        if( ucMsgLen )
        {
            if( (ucMsgIdx == 0) || (g_aTxMessagesTimer[ucMsgIdx] >= TX_ACK_WAIT_TIME) )
            { // new message to transmit to BBR, or ACK timeout for an already transmitted message
                UART1_RX_INT_DI(); // stop receiving ACK to have consistent data

                if( ucMsgIdx == 0 ) // confirmation message 
                {
                     if( UART1_SendMsg( g_aToBBRPayloads[0], ucMsgLen ) )
                     {
//.                          ITC_DISABLE_INTERRUPT( UART1_INT );
//.                          ITC_DISABLE_INTERRUPT( TMR_INT );
//.                          ITC_DISABLE_INTERRUPT( MACA_INT );
                          MONITOR_ENTER();
                          ucMsgLen = g_aTxMessagesLen[0] - ucMsgLen;
                          
                          if( !ucMsgLen || ucMsgLen > (sizeof(g_aToBBRPayloads[0])-3) )
                          {
                            g_aTxMessagesLen[0] = 0;
                          }
                          else // an interrupt added new info
                          {
//[doinel.alban] removed RadioHandler:  memmove( g_aToBBRPayloads[0] + 3, g_aToBBRPayloads[0] + g_aTxMessagesLen[0] - ucMsgLen, ucMsgLen ); 
//                                      g_aTxMessagesLen[0] = 3+ucMsgLen;
                            memmove( g_aToBBRPayloads[0] + 1, g_aToBBRPayloads[0] + g_aTxMessagesLen[0] - ucMsgLen, ucMsgLen ); 
                            g_aTxMessagesLen[0] = 1+ucMsgLen;
                          }
                                              
//.                          ITC_ENABLE_INTERRUPT( MACA_INT );
//.                          ITC_ENABLE_INTERRUPT( TMR_INT );
//.                          ITC_ENABLE_INTERRUPT( UART1_INT );
                          MONITOR_EXIT();
                     }
                }
                else if(g_aTxMessagesRetry[ucMsgIdx])
                { // network message, try to make one of the scheduled retries
                    if( UART_LINK_sendMsgToBackbone( g_aToBBRPayloads[ucMsgIdx], ucMsgLen ) )
                    {                        
                        g_aTxMessagesTimer[ucMsgIdx] = 0;
                        g_aTxMessagesRetry[ucMsgIdx]--;
                    }
                }
                else
                { // retry number exhausted, no ACK received
                  g_aTxMessagesLen[ucMsgIdx] = 0;  // release Tx buffer
                  g_ucBBRFailedResp++;
                }
                
                UART1_RX_INT_EN();
                return;
            }
            g_aTxMessagesTimer[ucMsgIdx] ++;
        }
    }

    if( g_ucBufferReadyReq )
    {
      UART1_SendMsg( NULL, 0 ); // null message will force buffer ready message
    }

    if( g_unNextIdlePk )
    {
      g_unNextIdlePk --;
    }
    else if(g_ucBBRFailedResp >= 4)
    { // no response from BBR for at least 20 sec
//        if( (g_stProv.aRestartReason[0] & 0xF0) != (RREASON_TOUT_BBR << 4) )
        PrepareForReset( RREASON_TOUT_BBR );  
    }
    else
    {
      static const uint8 c_aIdlePk[] = {UART_MSG_TYPE_NWK,0xFF,0xFF};
      static const uint8 c_aReqNetInfoPk[] = {UART_MSG_TYPE_REQ_NET_INFO,0xFF,0xFF};
      
      if(!g_ucNetInfoReceived)
        UART_LINK_sendMsgToBackbone( c_aReqNetInfoPk, sizeof(c_aIdlePk) );
      else
        UART_LINK_sendMsgToBackbone( c_aIdlePk, sizeof(c_aIdlePk) );
      
      g_ucBBRFailedResp++;
    }

  }

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: UART_LINK_AddMessage
  // Author: NIVIS LLC, Ion Ticus
  // Description: Add a message to be sent to UART
  // Parameters:
  //           p_unMessageType - message type: UART_MSG_TYPE_NWK, UART_MSG_TYPE_NWK_CONF
  //           p_ucPayloadLen - message payload length
  //           p_pPayload - message payload
  // Return:
  //        SUCCESS,  OUT_OF_MEMORY, or PAYLOAD_TOO_LONG
  // Obs:
  //      Access level: user level and interrupt level
  ///////////////////////////////////////////////////////////////////////////////////
  uint8 UART_LINK_AddMessage ( uint8 p_unMessageType, uint8 p_ucPayloadLen, const void * p_pPayload )
  {
     uint8 nIdx;
     if( p_unMessageType == UART_MSG_TYPE_NWK_CONF ) 
     {
          // NWK confirm is from interrupt so not need interrupt protection
          nIdx = g_aTxMessagesLen[0];
          if( !nIdx )
          {
//             nIdx = 3;  //[doinel.alban] removed RadioHandler
             nIdx = 1;    // message type size
          }
                    
          if( nIdx < sizeof(g_aToBBRPayloads[0])-3 )  // confirmation payload size = 3
          {               
               g_aToBBRPayloads[0][0] = UART_MSG_TYPE_NWK_CONF;
//[doinel.alban] removed RadioHandler, because TR will expect no ACK after confirmation packet               
//               g_aToBBRPayloads[0][1] = 0;    // RadioHandler
//               g_aToBBRPayloads[0][2] ++;     // RadioHandler
                              
               memcpy( (uint8*)(g_aToBBRPayloads[0]) + nIdx, p_pPayload, 3 );
               g_aTxMessagesLen[0] = nIdx+3;
               g_aTxMessagesTimer[0] = TX_ACK_WAIT_TIME; //[doinel.alban] to be sent quickly by UART_LINK_Task()
               
               return SUCCESS;
          } // else  return OUT_OF_MEMORY;
     }
     else
     { // UART_MSG_TYPE_NWK
          uint8 * pTxMsg;
          
          if( p_ucPayloadLen > sizeof(g_aToBBRPayloads[0])-3 ) // 3 bytes reserved for message type and handle
              return PAYLOAD_TOO_LONG;
  
            // search for the first free buffer
            for( nIdx = 1; nIdx < TX_MAX_TO_BBR_MSG_NO-1 && g_aTxMessagesLen[nIdx]; nIdx ++ )
                ;
            pTxMsg = g_aToBBRPayloads[ nIdx ];
      
            // begin exclusive section
//.            ITC_DISABLE_INTERRUPT( UART1_INT );
//.            ITC_DISABLE_INTERRUPT( TMR_INT );
//.            ITC_DISABLE_INTERRUPT( MACA_INT );
            MONITOR_ENTER();
      
            if( g_aTxMessagesLen[nIdx] == 0 ) // buffer empty
            {
                pTxMsg[2] ++;     // Handle[1]
                g_aTxMessagesTimer[nIdx] = TX_ACK_WAIT_TIME; 
                g_aTxMessagesLen[ nIdx ] = p_ucPayloadLen+3;
            }
            else // the selected buffer was taken by interrupt, or no free buffer 
            {
                nIdx = TX_MAX_TO_BBR_MSG_NO;
            }
      
//.            ITC_ENABLE_INTERRUPT( MACA_INT );
//.            ITC_ENABLE_INTERRUPT( TMR_INT );
//.            ITC_ENABLE_INTERRUPT( UART1_INT );
            MONITOR_EXIT();
            // end exclusive section
      
            if( nIdx < TX_MAX_TO_BBR_MSG_NO )
            {
                pTxMsg[0] = p_unMessageType;
                pTxMsg[1] = nIdx; // Handle[0]
                g_aTxMessagesRetry[nIdx] = TX_MAX_RETRY_CNT;  // no of retries for network messages 
                memcpy( pTxMsg+3, p_pPayload, p_ucPayloadLen );
                return SUCCESS;
            }
      }
      return ULINK_OUT_OF_MEMORY;
  }


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: UART_LINK_DispatchMessage
  // Author: NIVIS LLC, Ion Ticus
  // Description: dispatch an complete message received on UART link
  // Parameters:
  //           p_ucPayloadLen - message length
  //           p_pPayload - message payload
  // Return: 1 if success, 0 if not buffer
  // Obs:
  //      Access level: interrupt level
  ///////////////////////////////////////////////////////////////////////////////////
  uint8 UART_LINK_DispatchMessage ( uint8 p_ucPayloadLen, uint8 * p_pPayload )
  {
      if( p_ucPayloadLen >= 3 ) // min packet
      {
          p_ucPayloadLen -= 3;        // MessageType + Handle size
          switch( p_pPayload[0] )     // MessageType
          {
          case UART_MSG_TYPE_ACK_RST:
            if(g_ucNetJoinState != NET_JOIN_SEARCHING)
            {
              PrepareForReset( RREASON_RESET_BBR );

            }
          case UART_MSG_TYPE_ACK:
                      if( p_ucPayloadLen >= 7 ) // 3+7 = 10
                      {
                          uint8  ucMsgIdx = p_pPayload[1];  // Handle[0] - index in Tx buffer

                          if( ucMsgIdx && ucMsgIdx < TX_MAX_TO_BBR_MSG_NO )
                          {
                              if( g_aToBBRPayloads[ucMsgIdx][2] == p_pPayload[2] ) // same counter (Handle[1])
                              {
                                  g_aTxMessagesLen[ucMsgIdx] = 0;                  // ACK ok, release Tx buffer
                              }
                          }

                          if( g_ucWaitAck )
                          { // if no other message was received since the correspondent NWK message was sent to BBR
                              uint32 ulSec;
                              uint32 ulSecFraction;
                              uint16 unRXDelay;

                              // Little endian
                              ulSec =   (((uint32)(p_pPayload[3])))
                                      | (((uint32)(p_pPayload[4])) << 8)
                                      | (((uint32)(p_pPayload[5])) << 16)
                                      | (((uint32)(p_pPayload[6])) << 24);

                              ulSecFraction =
                                        (((uint32)(p_pPayload[7])))
                                      | (((uint32)(p_pPayload[8])) << 8)
                                      | (((uint32)(p_pPayload[9])) << 16);

                              unRXDelay = (TMR3_CNTR - g_unLastSTXTime) * 16 / 3; // transform into us

                              ulSecFraction += unRXDelay;
                              if( ulSecFraction > 1000000 )
                              {
                                  ulSecFraction -= 1000000;
                                  ulSec ++;
                              }

                              if(g_ucNetJoinState == NET_JOIN_SEARCHING)
                              { // it is the first TAI achieved from AP
                                g_ucNetJoinState = NET_JOIN_REQ_ADMISSION;
                                ulInitSec = ulSec;
                                ulInitSecFraction = ulSecFraction;
                                ulSec = 0;
                                ulSecFraction = 0;
                              }
                              else
                              {
                                ulSec -= ulInitSec;
                                if(ulSecFraction >= ulInitSecFraction)
                                {
                                  ulSecFraction -= ulInitSecFraction;
                                }
                                else
                                {
                                  ulSecFraction += 1000000 - ulInitSecFraction;
                                  ulSec--;
                                }
                              }
                              DLL_SetTAItoASN( ulSec, ulSecFraction );
                              
                              g_ulTimeSourceTimer = RTC_COUNT + TIME_SOURCE_TIMEOUT;    // used to find out when the device has lost its time source
                              
                          }

                      }
                      break;

          case UART_MSG_TYPE_RSP_NET_INFO:            
                       
                        // NetId
                        g_unNetworkId = p_pPayload[3];
                        g_unNetworkId <<= 8;
                        g_unNetworkId += p_pPayload[4]; 
                        g_stProv.unNetworkId = g_unNetworkId;
                        
                        // join session key
                        memcpy(g_stProv.aJoinSessionKey, p_pPayload + 5, 16);  
                        memcpy(g_aSessionsTable[0].m_aSessionKey, g_stProv.aJoinSessionKey, sizeof(SESSION_KEY));
                      
                        // MAC(EUI64) 
                        memcpy(g_aJoinDevUID_BE + 2, p_pPayload + 26, 3); 
                        InverseMemcpy( g_aJoinDevUID_LE, p_pPayload + 26, 3);
                        
                        // LongTag 
                        memcpy(g_stProv.aLongTag, p_pPayload + 29, 32); 
                        
                        // set flag for receiving the network information
                        g_ucNetInfoReceived = 1;
                        
                      break;
                      
          case UART_MSG_TYPE_REQ_DISCONNECT:

                       PrepareForReset(RREASON_RESET_BBR);
            
                      break;
            
          case UART_MSG_TYPE_NWK:

// 0x80 | Handle[0] | Handle[1] | Control | SrcAddr[0] | SrcAddr[1] | NPDU...       // long SrcAddr not applicable here
              if( !(p_pPayload[3] & 0x04)     // not long SrcAddr
                 && p_ucPayloadLen >= (3+16)) // 16 = min. NET + Security headers in NPDU
              {
                // hold the response code in p_pPayload[3] for confirmation packet
                p_pPayload[3] = NET_TransmitIndicate( (p_pPayload[3] & 0x08)? DLPDU_BROADCAST : DLPDU_UNICAST, // p_ucLocalStatus
                                      p_pPayload[3] & 0x03,   // p_ucPriority
                                      p_ucPayloadLen - 3 + 4,     // p_ucNsduDataLen // 4 extra bytes for MIC at authentication
                                      p_pPayload + 6);        // p_pNsduData 

                // conf = handle[0] | handle[1] | ResponseCode
//                p_pPayload[3] = SUCCESS;
                UART_LINK_AddMessage( UART_MSG_TYPE_NWK_CONF, 3, p_pPayload + 1 );
              }
          }
        g_ucBBRFailedResp = 0;
      }
      g_ucWaitAck = 0;
      return 1;
  }

  
  ///////////////////////////////////////////////////////////////////////////////////
  // Name: UART_LINK_sendMsgToBackbone
  // Author: NIVIS LLC, Ion Ticus
  // Description: send a message to UART and prepare to wait for ACK
  // Parameters:
  //           p_pMsg - message data
  //           p_ucMsgLen - message length
  // Return: none
  // Obs:
  //      Access level: user level
  ///////////////////////////////////////////////////////////////////////////////////
  uint8 UART_LINK_sendMsgToBackbone ( const uint8 * p_pMsg,  uint8 p_ucMsgLen )
  {
    uint8 ucStatus = UART1_SendMsg( p_pMsg, p_ucMsgLen );
    if( ucStatus )
    {
        g_unNextIdlePk = 500; // -> 5 sec for resync
        g_ucWaitAck = 1;
    }
    return ucStatus;
  }

    
#endif // BACKBONE_SUPPORT

