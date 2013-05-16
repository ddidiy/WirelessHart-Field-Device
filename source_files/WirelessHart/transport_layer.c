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
// Name:         transport_layer.c
// Author:       Nivis, LLC
// Date:         July 2008
// Description:  Transport Layer 
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "provision.h"
#include "app_layer.h"
#include "network_layer.h"
#include "transport_layer_dm.h"

#include "transport_layer.h"
#include "uart_link.h"

#define TL_MAX_REPLY_TIME     (30*4)  // 30s [250ms units]

// TT: 1 = 8 !!! (BLOCK_DATA_TRANSFER_RESP = PUBLISH_NOTIFY) 
// we can assume that while Block Data Transfer isn't implemented
const uint8 g_aTBtoTT[8]={0, 4, 1, 5, 2, 6, 3, 7};  
const uint8 g_aTTtoTB[8]={0, 2, 4, 6, 1, 3, 5, 7};

#ifdef TRANSPORT_EXTRA_BUFFER

// extra buffer for ack-services
NET_TRANSPORT_ATTRIBUTES g_stXBuff;
uint8 g_ucXBuffProperties; // MSB -> buffer empty or not; low nibble -> transport entry index

#endif

// local functions
static void TL_RebuildAndForwardPacket( uint8 p_ucTransportIndex, uint8 p_ucServiceID );


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_Init
  // Author: NIVIS LLC, Dorin Pavel
  // Description: resets the transport layer variables, must be called BEFORE NET_Init !
  // Parameters: none
  // Return: none
  ///////////////////////////////////////////////////////////////////////////////////
void TL_Init (void)
{ 
  // clean the transport table
  memset(g_aTransportTable, 0x00, sizeof(g_aTransportTable));
  
#ifdef TRANSPORT_EXTRA_BUFFER
  g_ucXBuffProperties = 0;
  memset(&g_stXBuff, 0x00, sizeof(g_stXBuff));
#endif  
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_TransmitRequest
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by the Application Layer to send down a message
  // Parameters:
  //                p_appHandle - app level handler  
  //                p_ucDstType - type of destination 
  //                p_pDstAddr - destination address
  //                p_ucPriority  - message priority
  //                p_ucServiceID  - service ID
  //                p_ucTransportType - message transport type
  //                p_ucAppDataLen - APP data length
  //                p_pAppData - APP data buffer
  // Return: success / error code
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
uint8 TL_TransmitRequest( HANDLE           p_appHandle,
                          NET_ADDR_TYPE    p_ucDstType,
                          const uint8*     p_pDstAddr,
                          uint8            p_ucPriority,
                          uint8            p_ucServiceID,
                          uint8            p_ucTransportType,
                          uint8            p_ucAppDataLen,
                          const void*      p_pAppData
                        )
{ 
  uint8 ucRC; // return code from NET/DLL
  uint8 ucIdx; // session/transport entry index
  uint16 unDstAddr; // short destination address 
  TL_Hdr stHdr; // transport header
  NET_SESSION_ATTRIBUTES* pSession=NULL; // actual session
  uint8 aucPayload[MAX_TPDU_SIZE]; // TPDU buffer (can be optimised)
  
  // init the transport header
  memset( &stHdr, 0, sizeof(stHdr) );
  
  // discard if APDU too long
  if( p_ucAppDataLen > MAX_APDU_SIZE )
  {
    //APP_TransmitConfirm( p_appHandle, TL_PAYLOAD_TOO_LONG, 0, NULL );
    return PAYLOAD_TOO_LONG;
  }    
  
  // find the session
  if( NET_ADDR_SHORT_ADDR == p_ucDstType )
  {   
    unDstAddr = *p_pDstAddr;
    unDstAddr <<= 8;
    unDstAddr += *(p_pDstAddr+1); 
    
    if( IsBroadcastTT(p_ucTransportType) || (0xFFFF == unDstAddr) )
    {
      pSession = FindSessionByType( SessionTypeCode_Broadcast, unDstAddr ); 
    }  
    else
    {   
      if( 0xFFFF == g_unNickname )     
      { // JoinRequest
        pSession = FindSessionByType( SessionTypeCode_Join, unDstAddr );
      }
      else
      {  
        pSession = FindSessionByType( SessionTypeCode_Unicast, unDstAddr ); 
      }
    }
  }
  
  if( NULL == pSession )
  {
    //APP_TransmitConfirm( p_appHandle, TL_NO_SESSION, 0, NULL );
    return NO_SESSION;
  }
  
  if( IsAckTT(p_ucTransportType) )
  { // find the transport entry (if it doesn't exist -> create it)
    ucIdx = pSession - g_aSessionsTable;
    ucIdx *= 2;
    
    if( g_aTransportTable[ucIdx].m_ucRetryCount & UNUSED_ENTRY )
    { // create a new transport entry   
      g_aTransportTable[ucIdx].m_ucRetryCount &= ~UNUSED_ENTRY; // used entry
      g_aTransportTable[ucIdx].m_ucSequenceNumber = 0x1F;
    } 
    else
    { // tbd: one extra buffer, currently the packet is discarded if the pipe is full
      if( TRUE == g_aTransportTable[ucIdx].m_bActive )
      { 
#ifdef TRANSPORT_EXTRA_BUFFER        
        if( g_ucXBuffProperties & BUFFER_BUSY_MASK )
        { // discard the packet 
          //APP_TransmitConfirm( p_appHandle, TL_PIPE_IS_FULL, 0, NULL );
          return TL_OUT_OF_MEMORY; 
        }
        else
        { // extra buffer empty       
          g_ucXBuffProperties = (BUFFER_BUSY_MASK | BUFFER_BEING_WRITTEN_MASK | ucIdx); 
        }
#else
        return TL_OUT_OF_MEMORY;      
#endif        
      }
    }  
    
    // request with ACK => increment the sequence number
    if( IsValidSequenceNmbTB(g_aTransportTable[ucIdx].m_ucSequenceNumber) )
    {  
      stHdr.m_ucTransportByte.flags.m_ucSequenceNumber = ++g_aTransportTable[ucIdx].m_ucSequenceNumber;
    }
    else
    {  
      g_aTransportTable[ucIdx].m_ucSequenceNumber=0;
      stHdr.m_ucTransportByte.flags.m_ucSequenceNumber=0;
    }
   
#ifdef TRANSPORT_EXTRA_BUFFER
    
    if( g_ucXBuffProperties & BUFFER_BEING_WRITTEN_MASK )
    { // write extra buffer 
      g_stXBuff.m_bActive = TRUE;
      g_stXBuff.m_bMaster = TRUE;
      g_stXBuff.m_bBroadcast = IsBroadcastTT(p_ucTransportType) ? TRUE : FALSE;
      g_stXBuff.m_ucSequenceNumber = g_aTransportTable[ucIdx].m_ucSequenceNumber; 
      g_stXBuff.m_ucRetryCount = 0;
      g_stXBuff.m_unResponseTimer = 0; 
      g_stXBuff.m_hTPDUHandle = p_appHandle;
      g_stXBuff.m_ucPacketPriority = p_ucPriority;
      g_stXBuff.m_ucServiceId = p_ucServiceID; 
    }
    else
#endif      
    { // initialise the transport entry
      g_aTransportTable[ucIdx].m_bActive = TRUE;
      g_aTransportTable[ucIdx].m_bMaster = TRUE;
      g_aTransportTable[ucIdx].m_bBroadcast = IsBroadcastTT(p_ucTransportType) ? TRUE : FALSE;
      g_aTransportTable[ucIdx].m_ucRetryCount = 0;
      g_aTransportTable[ucIdx].m_unResponseTimer = TL_MAX_REPLY_TIME; // start the response timer [250ms]
      g_aTransportTable[ucIdx].m_hTPDUHandle = p_appHandle;
      g_aTransportTable[ucIdx].m_ucPacketPriority = p_ucPriority;
      g_aTransportTable[ucIdx].m_ucServiceId = p_ucServiceID; 
    }  
  }
  else
  {    
    // according to addendum (request without ACK) => sequence number = 5-LSB of the PacketCounter
    if( IsValidSequenceNmbTB(pSession->m_ucPacketCounter) )
      stHdr.m_ucTransportByte.flags.m_ucSequenceNumber = ++pSession->m_ucPacketCounter;
    else
      stHdr.m_ucTransportByte.flags.m_ucSequenceNumber = pSession->m_ucPacketCounter = 0;
  } 
  
  // inc Cmd779 counter 
  g_stC779.m_unNoOfPacketsGeneratedByDevice++;
  
  // build the transport header
  // exception for join request, this works while Block Data Transfer is not implemented !!   
  stHdr.m_ucTransportByte.byte += (PUBLISH_NOTIFY == p_ucTransportType) ? (g_aTTtoTB[BLOCK_DATA_TRANSFER_RESP] << 5) : (g_aTTtoTB[p_ucTransportType] << 5);   
  if(NM_NICKNAME == unDstAddr)
  {
    stHdr.m_ucDeviceStatusByte = g_stProv.aDeviceStatus[MASTER_NM_IDX]; 
  }
  else 
  {
    stHdr.m_ucDeviceStatusByte = g_stProv.aDeviceStatus[MASTER_GW_IDX]; 
  }
  stHdr.m_ucXDeviceStatusByte = g_stCmd48Data.data.extendedDeviceStatus; 
  
  // build the network payload
  memmove( aucPayload + sizeof(stHdr), (uint8*)p_pAppData, p_ucAppDataLen );
  memcpy( aucPayload, &stHdr, sizeof(stHdr) );
  p_ucAppDataLen += sizeof(stHdr);
  
  // buffer the packet if necessary
  if( IsAckTB(stHdr.m_ucTransportByte.byte) )
  { 
#ifdef TRANSPORT_EXTRA_BUFFER    
    if( g_ucXBuffProperties & BUFFER_BEING_WRITTEN_MASK )
    { // write extra buffer
      g_stXBuff.m_ucLastPayloadSize = p_ucAppDataLen;
      memcpy(g_stXBuff.m_aLastPayload, aucPayload, p_ucAppDataLen); 
      
      g_ucXBuffProperties &= ~BUFFER_BEING_WRITTEN_MASK;
      return PACKET_WRITTEN_TO_TRANSPORT_EXTRA_BUFFER;
    }
    else
#endif      
    { // TPDU -> transport entry 
      g_aTransportTable[ucIdx].m_ucLastPayloadSize = p_ucAppDataLen;
      memcpy(g_aTransportTable[ucIdx].m_aLastPayload, aucPayload, p_ucAppDataLen);
    }  
  }
  
  ucRC = NET_TransmitRequest( p_appHandle,
                               p_ucDstType, 
                               p_pDstAddr,
                               p_ucTransportType,
                               p_ucPriority,
                               p_ucServiceID,
                               pSession,
                               p_ucAppDataLen,
                               aucPayload
                             );
  
  if( ucRC && IsAckTT(p_ucTransportType) )
  { // if DLL or NET returns an error code for an acknowledged service -> free the transport entry and adjust the sequence number
    g_aTransportTable[ucIdx].m_bActive = FALSE;
    g_aTransportTable[ucIdx].m_ucSequenceNumber = (g_aTransportTable[ucIdx].m_ucSequenceNumber - 1) & 0x1F;
  }  
  
  return ucRC;
}


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_TransmitIndicate
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by Network Layer when a packet is received and authentificated
  // Parameters: 
  //                p_hHandle  - transport level handler
  //                p_pSrcAddr  - source address
  //                p_ucPriority  - message priority
  //                p_ucTransportType  - message transport type
  //                p_ucTpduDataLen - TPDU data length
  //                p_pTpduData - TPDU data buffer
  // Return: None
  // Obs:
  //      Access level: user level
  ///////////////////////////////////////////////////////////////////////////////////
void TL_TransmitIndicate( HANDLE                  p_hHandle,                      
                          uint8                   p_ucPriority,
                          NET_SESSION_ATTRIBUTES* p_pSession,
                          uint8                   p_ucTpduDataLen,
                          uint8*                  p_pTpduData )
{ 
  uint8  ucIdx,
         ucTransportByte = ((TL_Hdr*)p_pTpduData)->m_ucTransportByte.byte,
         ucTransportType = g_aTBtoTT[ucTransportByte>>5], // TB -> TT
         ucSequenceNmb = ucTransportByte & 0x1F,
         aucDstAddr[2];
  
  const uint8* pPayload = p_pTpduData + sizeof(TL_Hdr);
  
  // is it necessary ?? packet too small => discard 
  if( p_ucTpduDataLen < (IsRespTB(ucTransportByte) ? (sizeof(TL_Hdr) + 4) : (sizeof(TL_Hdr) + 3)) )
    return; // min. 3 bytes request APDU size, min. 4 bytes response APDU size 
  
  // no session => discard
  if( NULL == p_pSession )
    return;
  
  // correct the TT (exception for join request, this works while Block Data Transfer is not implemented !!) 
  ucTransportType = (BLOCK_DATA_TRANSFER_RESP == ucTransportType) ? PUBLISH_NOTIFY : ucTransportType;     
  
  if( !IsRespTB(ucTransportByte) ) 
  { 
    //-------------------------------------------------------//
    //-------- Slave device receives a request NPDU ---------//
    //-------------------------------------------------------//
    
    if( IsAckTB(ucTransportByte) || (SEARCH_BROADCAST == ucTransportType) ) // exception for SEARCH_BROADCAST !
    {
      // find the transport entry     
      ucIdx = p_pSession - g_aSessionsTable;
      ucIdx = 2*ucIdx+1;
      
      if( g_aTransportTable[ucIdx].m_ucRetryCount & UNUSED_ENTRY )
      { // create a new transport entry if it doesn't exist   
        
        g_aTransportTable[ucIdx].m_ucRetryCount &= ~UNUSED_ENTRY; // used entry
        
        g_aTransportTable[ucIdx].m_bActive = TRUE;
        g_aTransportTable[ucIdx].m_bMaster = FALSE;
        g_aTransportTable[ucIdx].m_ucSequenceNumber = (ucSequenceNmb - 1) & 0x1F;
      } 
      
      // sequence number decision
      if( ucSequenceNmb == g_aTransportTable[ucIdx].m_ucSequenceNumber )
      { // packet is a retry => buffered response must be re-sent          
        
        if( p_pSession == g_aSessionsTable )
        { // exception for join session: packet (ACK for JoinResponse) must be send on unicast session with NM
          p_pSession ++; // packet must be sent on this session
          ucIdx += 2; // corresponding transport entry
        }
        
        if( SEARCH_BROADCAST == ucTransportType )
        { // exception for SEARCH_BROADCAST 
          p_pSession --;
          ucIdx -= 2; 
        }
        
        // discard this retry because there is no response for it !!  
        if( !g_aTransportTable[ucIdx].m_ucLastPayloadSize )
          return;
        
        // resend the response
        InverseMemcpy(aucDstAddr, &p_pSession->m_unPeerNickname, 2); 
        
        NET_TransmitRequest( g_aTransportTable[ucIdx].m_hTPDUHandle,
                             NET_ADDR_SHORT_ADDR,
                             aucDstAddr,
                             ucTransportType,
                             g_aTransportTable[ucIdx].m_ucPacketPriority,
                             0, // dummy service id
                             p_pSession,
                             g_aTransportTable[ucIdx].m_ucLastPayloadSize,
                             g_aTransportTable[ucIdx].m_aLastPayload );
        
      }
      else
      {    
        if( g_aTransportTable[ucIdx].m_ucSequenceNumber == ((ucSequenceNmb-1) & 0x1F) )
        { 
          // new request, store it (the packet's sequence number is stored later when the response is being generated, see the spec)
          g_aTransportTable[ucIdx].m_bBroadcast = IsBroadcastTB(ucTransportByte) ? TRUE : FALSE;
          g_aTransportTable[ucIdx].m_hTPDUHandle = p_hHandle;
          g_aTransportTable[ucIdx].m_ucPacketPriority = p_ucPriority;          
          g_aTransportTable[ucIdx].m_ucLastPayloadSize = 0; // m_ucLastPayloadSize=0 until the response is saved !
          memcpy(g_aTransportTable[ucIdx].m_aLastPayload, p_pTpduData, p_ucTpduDataLen);
          g_aTransportTable[ucIdx].m_ucRetryCount = 0;
          g_aTransportTable[ucIdx].m_unResponseTimer = 0; 
          
          // notify the client
          APP_TransmitIndicate( p_hHandle,                      
                                p_ucPriority,
                                p_pSession->m_unPeerNickname,
                                ucSequenceNmb,
                                ucTransportType,
                                p_ucTpduDataLen - sizeof(TL_Hdr),
                                pPayload ); 
        }  
        else
        {  
          // discard the packet  
          return;  
        }
      }        
    }
    else if( !IsAckTB(ucTransportByte) )
    {            
      // notify the client
      APP_TransmitIndicate( p_hHandle,                      
                            p_ucPriority,
                            p_pSession->m_unPeerNickname,
                            ucSequenceNmb,
                            ucTransportType,
                            p_ucTpduDataLen - sizeof(TL_Hdr),
                            pPayload );         
    }
  }
  else
  { 
    //---------------------------------------------------------//
    //-------- Master device receives a response NPDU ---------// 
    //---------------------------------------------------------//    
    
    // compute transport entry index
    ucIdx = p_pSession - g_aSessionsTable;
    ucIdx = 2*ucIdx; 
      
    // if the transport table doesn't exist => discard the packet
    if( g_aTransportTable[ucIdx].m_ucRetryCount & UNUSED_ENTRY )
      return; 
      
    // broadcast bit from transport byte and founded transport table must be the same
    if( ((TL_Hdr*)p_pTpduData)->m_ucTransportByte.flags.m_bBroadcast != g_aTransportTable[ucIdx].m_bBroadcast )
      return;
      
    // sequence number check
    if( ucSequenceNmb == g_aTransportTable[ucIdx].m_ucSequenceNumber )
    { 
      APP_TransmitConfirm( g_aTransportTable[ucIdx].m_hTPDUHandle, 
                           TL_SUCCESS, 
                           p_ucTpduDataLen - sizeof(TL_Hdr),
                           (uint8*)p_pTpduData + sizeof(TL_Hdr) );
       
      // de-activate the transpot entry
      g_aTransportTable[ucIdx].m_bActive = FALSE;  
    }  
    else
    {
      APP_TransmitConfirm( p_hHandle, TL_INVALID_SEQUENCE_NUMBER, 0, NULL );
    }    
  }     
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_TransmitRsp
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by client as a response to an incoming packet that require a response.
  // Parameters: 
  //                p_hHandle  - packet handler
  //                p_ucAppDataLen - payload length
  //                p_pAppData - payload buffer
  //                p_ucSequenceNumber - packet sequence number  
  //                p_ucServiceID  - service ID
  // Return: success / error code
  // Obs:
  //      Access level: User level
  //      Context: p_hHandle must be same as on transmit indicate callback
  ///////////////////////////////////////////////////////////////////////////////////
uint8 TL_TransmitRsp( HANDLE                  p_hHandle, 
                      uint8                   p_ucAppDataLen, 
                      const void*             p_pAppData,
                      uint8                   p_ucSequenceNumber,
                      uint8                   p_ucServiceID
                    )
{ 
  uint8  ucRC,
         ucTransportIdx,
         ucSessionIdx,
         ucTransportByte,
         aucDstAddr[2];
  
  TL_Hdr stHdr; 
  
  ucTransportIdx = FindTransportEntryByHandle(p_hHandle); 
  ucTransportByte = g_aTransportTable[ucTransportIdx].m_aLastPayload[0]; // peer transport byte
  
  if( ucTransportIdx >= MAX_TRANSPORT_NO )
  {  
    //APP_TransmitConfirm( p_hHandle, TL_INVALID_HANDLER, 0, NULL );
    return INVALID_HANDLER;  
  }    
  
  if( g_aTransportTable[ucTransportIdx].m_ucSequenceNumber == ((p_ucSequenceNumber-1) & 0x1F) )
  {
    // store the new sequence number
    g_aTransportTable[ucTransportIdx].m_ucSequenceNumber = p_ucSequenceNumber;
    
    ucSessionIdx = ucTransportIdx/2; // find the session
    
    if( !ucSessionIdx )
    { // exception for join session: packet (ACK for JoinResponse) must be send on unicast session with NM
      
      // invalidate the transport entry handler
      g_aTransportTable[ucTransportIdx].m_hTPDUHandle = TRANSPORT_TABLE_INVALID_HANDLER; 
      
      ucSessionIdx += 1; // packet must be sent on this session
      ucTransportIdx += 2; // corresponding transport entry
      
      // copy the packet priority into the right transport entry
      g_aTransportTable[ucTransportIdx].m_ucPacketPriority = g_aTransportTable[ucTransportIdx - 2].m_ucPacketPriority;
    }
       
    // find the TT
    ucTransportByte = g_aTBtoTT[ucTransportByte>>5]; // peer TB -> peer TT
      
    // TT can't be 1, so there is no need for the conversion BLOCK_DATA_TRANSFER_RESP -> PUBLISH_NOTIFY
    switch( ucTransportByte )
    {
      case REQ_UNICAST:      
         
           ucTransportByte = RESP_UNICAST; 
           break;
                             
      case SEARCH_BROADCAST: 
      case PUBLISH_BROADCAST:
      case REQ_BROADCAST: 
        
           // invalidate the transport entry handler
           g_aTransportTable[ucTransportIdx].m_hTPDUHandle = TRANSPORT_TABLE_INVALID_HANDLER;
           
           // copy the packet priority into the right transport entry
           g_aTransportTable[ucTransportIdx-2].m_ucPacketPriority = g_aTransportTable[ucTransportIdx].m_ucPacketPriority;
           
           // change the transport entry(broadcast -> unicast)
           ucSessionIdx -= 1; // packet must be sent on unicast session
           ucTransportIdx -= 2; // corresponding transport entry    
        
           ucTransportByte = RESP_BROADCAST;  
           break;
    };
    
    // invalidate the transport entry handler
    g_aTransportTable[ucTransportIdx].m_hTPDUHandle = TRANSPORT_TABLE_INVALID_HANDLER; 
    
    // search broadcast is not for me -> no response
    if( /*(ucTransportByte == SEARCH_BROADCAST) &&*/ !p_ucAppDataLen )
    {  
      g_aTransportTable[ucTransportIdx].m_ucLastPayloadSize = 0;
      return SUCCESS;   
    }
    
    // TL Hdr. construction
    stHdr.m_ucTransportByte.byte = g_aTTtoTB[ucTransportByte] << 5; // TT -> TB
    stHdr.m_ucTransportByte.flags.m_ucSequenceNumber = p_ucSequenceNumber;
    
    
    if(NM_NICKNAME == g_aSessionsTable[ucSessionIdx].m_unPeerNickname)
    {
      stHdr.m_ucDeviceStatusByte = g_stProv.aDeviceStatus[MASTER_NM_IDX]; 
    }
    else 
    {
      stHdr.m_ucDeviceStatusByte = g_stProv.aDeviceStatus[MASTER_GW_IDX]; 
    }
                 
    stHdr.m_ucXDeviceStatusByte = g_stCmd48Data.data.extendedDeviceStatus; 
     
    // buffer the response
    memcpy (g_aTransportTable[ucTransportIdx].m_aLastPayload, &stHdr, sizeof(TL_Hdr));
    memcpy(g_aTransportTable[ucTransportIdx].m_aLastPayload + sizeof(TL_Hdr), p_pAppData, p_ucAppDataLen);
    p_ucAppDataLen += sizeof(TL_Hdr); 
    
    g_aTransportTable[ucTransportIdx].m_ucLastPayloadSize = p_ucAppDataLen;

    aucDstAddr[0] = (uint8)(g_aSessionsTable[ucSessionIdx].m_unPeerNickname >> 8); 
    aucDstAddr[1] = (uint8)g_aSessionsTable[ucSessionIdx].m_unPeerNickname; 

  
    // inc Cmd779 counter //??? see Issue#63 
    g_stC779.m_unNoOfPacketsGeneratedByDevice++;
      
    ucRC = NET_TransmitRequest( p_hHandle,
                                NET_ADDR_SHORT_ADDR, 
                                aucDstAddr,
                                ucTransportByte,
                                g_aTransportTable[ucTransportIdx].m_ucPacketPriority,
                                p_ucServiceID,
                                &g_aSessionsTable[ucSessionIdx],
                                p_ucAppDataLen,
                                g_aTransportTable[ucTransportIdx].m_aLastPayload ); 
    
    if( ucRC == DLL_OUT_OF_MEMORY )
    {
      // TBD(do retries from TL, APP receives SUCCESS)
      
      g_aTransportTable[ucTransportIdx].m_unResponseTimer = 0xFFFF; // mark this response for TL retries
      return SUCCESS;
    }
    else
    {  
      return ucRC;
    } 
  }
  else
  {
    //APP_TransmitConfirm( p_hHandle, TL_INVALID_SEQUENCE_NUMBER, 0, NULL );
    return INVALID_SEQUENCE_NUMBER;   
  }  
} 

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_BuildAndForwardPacket
  // Author: NIVIS LLC, Dorin Pavel
  // Description: the function builds a packet based on the passed transport table entry
  //              and send it to the NET layer 
  // Parameters:  p_pSession - ptr. to active session, assuming it isn't NULL
  //              p_ucServiceID - service id
  // Return: None
  ///////////////////////////////////////////////////////////////////////////////////
void TL_RebuildAndForwardPacket( uint8 p_ucTransportIndex, uint8 p_ucServiceID )
{
  uint8  ucRC,
         ucDstAddrType,
         ucTransportType,
         aucDstAddr[8];
  
  NET_SESSION_ATTRIBUTES* pSession = &g_aSessionsTable[p_ucTransportIndex/2];
  
  // find the transport type
  ucTransportType = g_aTransportTable[p_ucTransportIndex].m_aLastPayload[0]; // TB
  ucTransportType = g_aTBtoTT[ucTransportType>>5]; // TB -> TT
  
  // resend the request
  if( SessionTypeCode_Join == pSession->m_ucSessionType )
  {
    memcpy(aucDstAddr, c_aHCF_OUI_BE, 3);
    memcpy(aucDstAddr+3, pSession->m_aPeerUniqueID, 5);
    ucDstAddrType = NET_ADDR_LONG_ADDR;
  }   
  else
  {  
    InverseMemcpy(aucDstAddr, &pSession->m_unPeerNickname, 2);
    ucDstAddrType = NET_ADDR_SHORT_ADDR;
  }
  
  ucRC = NET_TransmitRequest( g_aTransportTable[p_ucTransportIndex].m_hTPDUHandle,
                              ucDstAddrType,
                              aucDstAddr,
                              ucTransportType,
                              g_aTransportTable[p_ucTransportIndex].m_ucPacketPriority,
                              p_ucServiceID,
                              pSession,
                              g_aTransportTable[p_ucTransportIndex].m_ucLastPayloadSize,
                              g_aTransportTable[p_ucTransportIndex].m_aLastPayload ); 
  
  if( !ucRC && p_ucTransportIndex%2 )
  { // when a response retry was successfuly added on DLL queue
    
    // stop sending response retries
    g_aTransportTable[p_ucTransportIndex].m_unResponseTimer = 0; 
  }  
}  

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_doRetry
  // Author: NIVIS LLC, Dorin Pavel
  // Description: TL retry mechanismus
  // Parameters: None
  // Return: None
  // Access level: this function must be called cyclically from main, each 250ms
  ///////////////////////////////////////////////////////////////////////////////////
void TL_doRetry(void)
{
  uint8 ucIdx=0;
  for(; ucIdx < MAX_TRANSPORT_NO; ucIdx ++) // for all even entries
  {
    if( 0 == (g_aTransportTable[ucIdx].m_ucRetryCount & UNUSED_ENTRY) )
    { // if transport entry exists 
      if( TRUE == g_aTransportTable[ucIdx].m_bActive )  
      { 
        if( TRUE == g_aTransportTable[ucIdx].m_bMaster )
        {  
          // request didn't receive ACK
          if( g_aTransportTable[ucIdx].m_unResponseTimer )
          { // decrement timer 
            g_aTransportTable[ucIdx].m_unResponseTimer --; 
          }
          else
          { // timeout, resend request 
            if( g_aTransportTable[ucIdx].m_ucRetryCount < g_ucMaxRetries )
            {  
              g_aTransportTable[ucIdx].m_unResponseTimer = TL_MAX_REPLY_TIME; // restart the response timer [250ms] 
              g_aTransportTable[ucIdx].m_ucRetryCount ++;
             
              TL_RebuildAndForwardPacket( ucIdx, g_aTransportTable[ucIdx].m_ucServiceId );
            }  
            else
            { // number of retries exceeded
              g_aTransportTable[ucIdx].m_bActive = FALSE; // transport entry released
              APP_TransmitConfirm( g_aTransportTable[ucIdx].m_hTPDUHandle, TL_MAX_RETRIES_EXCEEDED, 0, NULL );
              
              // trigger an alarm message to NM: Transport Layer Failed
              APP_AddToAlarmBuffer(APP_ALARM_C791, g_aSessionsTable[ucIdx/2].m_unPeerNickname, NULL);
            }
          }
        }
        else
        {
          if( g_aTransportTable[ucIdx].m_unResponseTimer == 0xFFFF )
          { 
            // slave side(if the response for an incoming request fails): do retries from TL 
            TL_RebuildAndForwardPacket( ucIdx, g_aTransportTable[ucIdx].m_ucServiceId );
          }  
        }  
      }  
    }
  }
}  


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_CheckExtraBuffer
  // Author: NIVIS LLC, Dorin Pavel
  // Description: Check the TL extra buffer and move to Transport Table if free
  // Parameters: None
  // Return: None
  // Access level: this function must be called cyclically from main loop
  ///////////////////////////////////////////////////////////////////////////////////
#ifdef TRANSPORT_EXTRA_BUFFER
void TL_CheckExtraBuffer(void)
{
  // extra buffer for acknowledged services
  if( g_ucXBuffProperties & BUFFER_BUSY_MASK )
  { // buffer not empty, treat it
    if( FALSE == g_aTransportTable[g_ucXBuffProperties & TRANSP_IDX_MASK].m_bActive )    
    {
      // if the corresponding transport entry is free: extra buffer -> transport table
      memcpy( &g_aTransportTable[g_ucXBuffProperties & TRANSP_IDX_MASK], &g_stXBuff, sizeof(NET_TRANSPORT_ATTRIBUTES) ); 
      g_ucXBuffProperties &= ~(BUFFER_BUSY_MASK | TRANSP_IDX_MASK); // clear extra buffer
    }  
  }  
}
#endif

