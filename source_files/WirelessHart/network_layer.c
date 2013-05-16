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
* Name:         network_layer.c
* Author:       Nivis LLC,
* Date:         July 2008
* Description:  Network Layer
* Changes:
* Revisions:
****************************************************************************************************/
#include <string.h>
#include "provision.h"
#include "uart_link.h"
#include "transport_layer.h"
#include "datalink_layer.h"
#include "datalink_layer_dm.h"
#include "ParseComposeCommand.h"
#include "ParseComposeCommandTables.h"
#ifdef INCLUDE_COMMON_STACK_SOURCES
  //#include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/UniversalCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/CommonPracticeCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/DataLinkLayerCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/CommonResponseCode.h"
#else
  //#include "ApplicationLayer/Model/UniversalCommands.h"
  #include "ApplicationLayer/Model/CommonPracticeCommands.h"
  #include "ApplicationLayer/Model/DataLinkLayerCommands.h"
  #include "ApplicationLayer/Model/CommonResponseCode.h"
#endif
#include "app_cmdresp.h"
#include "app_layer.h"

#include "network_layer.h"


const uint8 c_aHCF_OUI_BE[3] = { 0x00, 0x1B, 0x1E };  // HCF_OUI in big endian format
const uint8 c_aHCF_OUI_LE[3] = { 0x1E, 0x1B, 0x00 };  // HCF_OUI in little endian format

const UNIQUE_ID c_aNetManagerUID_BE = { 0xF9, 0x80, 0x00, 0x00, 0x01 };
const UNIQUE_ID c_aNetManagerUID_LE = { 0x01, 0x00, 0x00, 0x80, 0xF9 };
const uint8 c_aNetManagerAddr[2] = { 0xF9, 0x80 };

const UNIQUE_ID c_aGatewayUID_BE = { 0xF9, 0x81, 0x00, 0x00, 0x02 };
const UNIQUE_ID c_aGatewayUID_LE = { 0x02, 0x00, 0x00, 0x81, 0xF9 };
const uint8 c_aGatewayAddr[2] = { 0xF9, 0x81 };

const uint8 c_aBcastAddr[2] = { 0xFF, 0xFF };


/* NET global status */
uint8 g_ucNetState;

/* NET join procedure status */
uint8 g_ucNetJoinState;

/* number of received advertisements */
uint8 g_ucNetAdNumber;

/* advertisement timer */
uint32 g_ulAdWaitTimer;


/* at least one advertisemenet with good RSL heard */
uint8 g_ucAdvOverTresh;

/* flag = wait one more period for adv */
uint8 g_ucAdvOneMorePeriod;

/* join response timer */
uint32 g_ulJoinRspTimer;

/* loosely coupled timer */
uint32 g_ulLooselyCoupledTimer;

/* advertising neighbors table */
T_ADV_NEIGHBOR g_astAdvNeighbors[MAX_ADV_NEIGHBORS];
uint8 g_ucAdvNeighborNmb; // can be maximum MAX_ADV_NEIGHBORS !
uint8 g_ucAdvIdx;         // used by NET_SendJoinReq()

/* Neighbor whom the Join Request is addressed */
uint16 g_unNeighbor;

/* Join Route Id */
//uint8 g_ucJoinRouteId;

/* NET queue for received packets */
T_NET_RX_PACKET g_stRecvNSDU[NET_RX_QUEUE_SIZE];

/* command 769 response (wireless operation mode, join status, etc), see table 51/52 from Common Tables Spec */
C769_ReadJoinStatus_Resp g_stC769;

/* command 779 response, see Wireless Command Spec */
C779_ReportDeviceHealth_Resp g_stC779;

/* command flags (1 -> command executed) for NET join procedure */
uint8 g_ucJoinCmdFlags;

/* see above HEALTH_REPORTS_STATE */
uint8 g_ucHealthReportState;

/* health report timer */
uint32 g_ulHealthReportTimer;

/* incoming packets handle */
uint8 g_ucPacketForMeHandle;

/* forwarded packets handle */
uint16 g_unPacketFwdHandle;

/*join mode code*/
uint8 g_ucJoinMode;

/* loop detection counters  */
#ifndef BACKBONE_SUPPORT 
NET_LOOP_DETECTION g_stLoopDetect;
#endif

/* local functions */
static void  NET_SendJoinReq(void);
static uint8 NET_ValidatePacket( uint8* p_pNsduData );
static uint8 NET_StorePacket( uint8  p_ucPriority, uint8  p_ucNsduDataLen, uint8* p_pNsduData );
#ifndef BACKBONE_SUPPORT 
static uint8 getBestAdvIdx(uint8 ucExcluded);
#endif

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_Init
  // Author: NIVIS LLC, Dorin Pavel
  // Description: resets the network layer variables
  // Parameters: none
  // Return: none
  ///////////////////////////////////////////////////////////////////////////////////
void NET_Init (void)
{
  uint8 ucIdx;

  g_unNickname = 0xFFFF; // field device: invalid nickname at the beginning    
  g_unNetworkId = g_stProv.unNetworkId; // the actual value used by the DLL 
  
  // create the join session
  g_aSessionsTable[0].m_ucSessionFlags &= ~(UNUSED_ENTRY | VALID_OLD_KEY); // used entry, invalid old session key
  g_aSessionsTable[0].m_ulPeerNonceCounter=0;
  g_aSessionsTable[0].m_ulNonceCounterHistory=0;
  g_aSessionsTable[0].m_ulMyNonceCounter=0;   // TODO: must be non-volatile!
//  InverseMemcpy(&g_aSessionsTable[0].m_unPeerNickname, (void*)c_aNetManagerAddr, 2);
  g_aSessionsTable[0].m_unPeerNickname = NM_NICKNAME;
  memcpy(g_aSessionsTable[0].m_aSessionKey, g_stProv.aJoinSessionKey, sizeof(SESSION_KEY));   // TODO: must be non-volatile!
  g_aSessionsTable[0].m_ucSessionType = SessionTypeCode_Join;
  g_aSessionsTable[0].m_ucPacketCounter = 0x1F;
  memcpy(g_aSessionsTable[0].m_aPeerUniqueID, c_aNetManagerUID_BE, 5);
  // only join session exists now
  g_ucSessionsNo = 1;
  g_ucOldSessionKeys = 0; // no of still valid old keys

  for(ucIdx=g_ucSessionsNo; ucIdx<MAX_SESSIONS_NO; ucIdx++)
  {  
    g_aSessionsTable[ucIdx].m_ucSessionFlags = UNUSED_ENTRY;
    g_aSessionsTable[ucIdx].m_ucPacketCounter = 0x1F;
  }  
    
  // init the transport table
  for(ucIdx=0; ucIdx<MAX_TRANSPORT_NO; ucIdx++)
  {
    g_aTransportTable[ucIdx].m_ucRetryCount = UNUSED_ENTRY;
    g_aTransportTable[ucIdx].m_hTPDUHandle = TRANSPORT_TABLE_INVALID_HANDLER;
  }
  // init the NET rx queue
  for(ucIdx=0; ucIdx<NET_RX_QUEUE_SIZE; ucIdx++)
    g_stRecvNSDU[ucIdx].m_ucUsedElem = UNUSED_ENTRY;

  // init NET JoinStateMachine
  g_ucNetState = NET_STATE_IDLE;
  g_ucNetJoinState = NET_JOIN_SEARCHING;

  // Join Request Initialisations
#ifdef BACKBONE_SUPPORT
  g_ulJoinRspTimeout = 30*4;  // 30s [250ms units]
#else
  g_ulJoinRspTimeout = 30*4;  //[doinel.alban] -for HCF g_stProv.ulDllKeepAlive;
#endif
//[doinel.alban]  g_ulChannelSearchTime = DLL_CHANNEL_SEARCH_TIME; // 40*10ms = 400ms
//[doinel.alban]  g_stProv.ulActiveSearchShedTime = DLL_ACTIVE_SEARCH_TIME; // 4000s  // 
  g_ucMaxRetries = 5;
  g_ucMaxJoinRetries = 5;

  memset(&g_stC769, 0x00, sizeof(g_stC769)); // init Cmd769
  memset(&g_stC779, 0x00, sizeof(g_stC779)); // init Cmd779
  
  // other Inits
  g_ucJoinCmdFlags=0;
  g_ulAdWaitTimer=0;
  g_ulJoinRspTimer=0;
  g_ucNetAdNumber=0;
  g_ucAdvNeighborNmb=0;
  g_ucAdvIdx = 0xFF;
  g_ucHealthReportState = NO_HEALTH_REPORTS;
  g_ulHealthReportTimer=0;
  g_ucPacketForMeHandle=0;
  g_unPacketFwdHandle=0x0100;
  
  g_ucAdvOverTresh = 0;
  g_ucAdvOneMorePeriod = 0;  
  
#ifndef BACKBONE_SUPPORT 
  memset( &g_stLoopDetect, 0, sizeof(g_stLoopDetect) );
#endif
  g_ucServicesNo = 0;
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_TransmitRequest
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is used by the Transport Layer to send a request to a specific destination.
  // Parameters:
  //                p_hHandle  - transport level handler
  //                p_ucDstType  - destination type
  //                p_pDstAddr - destination address
  //                p_ucTransportType  - message transport type
  //                p_ucPriority  - message priority
  //                p_ucServiceID  - service ID
  //                p_pSession  - active session
  //                p_ucNsduDataLen - NSDU data length
  //                p_pNsduData - NSDU data buffer
  // Return: success / error code
  // Obs:
  //      Access level: User level
  //      Context: After calling of that function a NET_TransmitConfirm has to be received.
  ///////////////////////////////////////////////////////////////////////////////////
uint8 NET_TransmitRequest( HANDLE        p_hHandle,
                           NET_ADDR_TYPE p_ucDstType,
                           const uint8*  p_pDstAddr,
                           uint8         p_ucTransportType,
                           uint8         p_ucPriority,
                           uint8         p_ucServiceID,
                           NET_SESSION_ATTRIBUTES* p_pSession,
                           uint8         p_ucNsduDataLen,
                           const uint8*  p_pNsduData )
{
  uint8 ucCtrlByte=0,
        aTxBufer[105],
        ucTxLen=1, // TTL index
        ucIdx,
        ucRouteType=0xFF,

#ifndef BACKBONE_SUPPORT

        ucDllDstType,
        aucGenericDstAddr[8],

#endif

        aNonce[13],
        ucRC = DLL_OUT_OF_MEMORY; // return code

  NET_ROUTE_ATTRIBUTES * pRoute=NULL;


  // compose the NET header

  // add the control byte later

  // TTL = 0 before encryption
  aTxBufer[ucTxLen++] = 0;

  {
    // ASN snippet
    MONITOR_ENTER();
    aTxBufer[ucTxLen++] = g_sDllASN.m_aucBytes[1];
    aTxBufer[ucTxLen++] = g_sDllASN.m_aucBytes[0];
    MONITOR_EXIT();
  }
  
  { // FieldDevice: we assume that the DstAddr is always short
    uint16 unDstAddr;

    unDstAddr = *p_pDstAddr;
    unDstAddr <<= 8;
    unDstAddr += *(p_pDstAddr + 1);

    if( 0xFFFF == g_unNickname )
    {
      // JoinRequest

      ucCtrlByte |= 0x40;

//      ucIdx = FindRouteIndex(g_ucJoinRouteId);
      ucIdx = FindRouteIndex(JOIN_ROUTE_ID);
      if(ucIdx != NET_INVALID_IDX)
      {
        pRoute = &g_aRoutesTable[ucIdx];
      }
      else
      {
        pRoute = FindFirstRoute(unDstAddr);

        if(NULL == pRoute)
        {
          return NO_ROUTE;
        }
      }
    }
    else
    {
      ucIdx=0xFF; // invalid index

      if( 0xF980 != unDstAddr )
      { // DstAddr != NM
        
        ucIdx = FindServiceIndex( p_ucServiceID, NL_SEARCH_BY_SERVICE_ID );
        
        if( ucIdx >= g_ucServicesNo )
        { // passed service id not found -> look for the first management service with the peer Nickname
          for( ucIdx = 0; ucIdx < g_ucServicesNo; ucIdx++ )
          {
            if (  (g_aServicesTable[ucIdx].m_unPeerNickname == unDstAddr) &&
                  (g_aServicesTable[ucIdx].m_ucApplicationDomain == ServiceApplicationDomain_Maintenance) )
            {
              break;
            }         
          }
        }
        
      }

      if( ucIdx < g_ucServicesNo )
      { // ServiceId found
        
        switch( p_ucPriority )
        {
          case DLL_PACKET_PRIORITY_COMMAND: ucRouteType = ServiceApplicationDomain_Maintenance;
                                            break;

          case DLL_PACKET_PRIORITY_DATA:    ucRouteType = ServiceApplicationDomain_Publish;
                                            break;

          case DLL_PACKET_PRIORITY_NORMAL:  if( p_ucTransportType <= BLOCK_DATA_TRANSFER_RESP )
                                              ucRouteType = ServiceApplicationDomain_BlockTransfer;
                                            else
                                              ucRouteType = ServiceApplicationDomain_Maintenance;
                                            break;

          case DLL_PACKET_PRIORITY_ALARM:   ucRouteType = ServiceApplicationDomain_Event;
                                            break;
        }; // END SWITCH

        // the matching of the route type is no longer performed, due to incompatibility with the Emerson Network.
        // However, an official response shall be received in the future from Emerson, to check if they fit the HCF requirements.
        // in our view, Table 18 within the HCF_SPEC_085 shall be commented together with Emerson.
        
        //if (ucRouteType == g_aServicesTable[ucIdx].m_ucApplicationDomain)
        //{
          ucIdx = FindRouteIndex(g_aServicesTable[ucIdx].m_ucRouteID);
          
          if(ucIdx != NET_INVALID_IDX)
          {
            pRoute = &g_aRoutesTable[ucIdx];
          }
          else
          {
            pRoute = FindFirstRoute(unDstAddr);

            if(NULL == pRoute)
            {
              return NO_ROUTE;
            }
          }
        //}
        //else
        //{
          //APP_TransmitConfirm( p_hHandle, NL_APP_DOMAIN_NOT_MATCH, 0, NULL );
          //return APP_DOMAIN_NOT_MATCH;
       // }
      }
      else
      { // ServiceId not found OR DstAddr = NM
        
        pRoute = FindFirstRoute(unDstAddr);

        if(NULL == pRoute)
        {
          return NO_ROUTE;
        }
      }
    }

    // Graph Id
    aTxBufer[ucTxLen++] = pRoute->m_unGraphId >> 8;
    aTxBufer[ucTxLen++] = pRoute->m_unGraphId;

    // DstAddr
    aTxBufer[ucTxLen++] = *p_pDstAddr;
    aTxBufer[ucTxLen++] = *(p_pDstAddr + 1);
  }

  // SrcAddr
  if( ucCtrlByte & 0x40 )
  {
    memcpy(aTxBufer + ucTxLen, c_aHCF_OUI_BE, 3);
    ucTxLen += 3;

    memcpy(aTxBufer + ucTxLen, g_aJoinDevUID_BE, 5);
    ucTxLen += 5;
  }
  else
  {
    aTxBufer[ucTxLen++] = g_unNickname >> 8;
    aTxBufer[ucTxLen++] = g_unNickname;
  }

#ifndef BACKBONE_SUPPORT
  if( pRoute )
  {
    // add source routing
    if( pRoute->m_bSourceRouteAttached )
    {
      ucIdx = FindSrcRouteIndex( pRoute->m_ucRouteId );

      if( ucIdx >= g_ucSrcRoutesNo )
      {
        //APP_TransmitConfirm( p_hHandle, NL_NO_SOURCE_ROUTE, 0, NULL );
        return NO_SOURCE_ROUTE;
      }
      else
      { // source route found, copy it into the NET header

        #define SrcTableLength g_aSrcRoutesTable[ucIdx].m_ucNoOfHops

        uint8 ucSrcIdx = 0;
        for(; ucSrcIdx < SrcTableLength; ucSrcIdx++ )
        {
          aTxBufer[ucTxLen++] = g_aSrcRoutesTable[ucIdx].m_aSrcRouteHops[ucSrcIdx] >> 8; // MSB
          aTxBufer[ucTxLen++] = g_aSrcRoutesTable[ucIdx].m_aSrcRouteHops[ucSrcIdx]; // LSB
        }

        // assuming that at least one hop is present (m_ucNoOfHops >= 1)
        ucCtrlByte |= 0x01; // 1st Source Route

        if( SrcTableLength > 4 )
        {
          ucCtrlByte |= 0x02; // 2nd Source Route

          if( SrcTableLength < 8 )
          { // fill the 2nd Source Route with 0xFF
            memset( aTxBufer + ucTxLen, 0xFF, 2*(8 - SrcTableLength) );
            ucTxLen += 2*(8 - SrcTableLength);
          }
        }
        else
        {
          if( SrcTableLength < 4 )
          { // fill the 1st Source Route with 0xFF
            memset( aTxBufer + ucTxLen, 0xFF, 2*(4 - SrcTableLength) );
            ucTxLen += 2*(4 - SrcTableLength);
          }
        }

        // forward the packet to the first address in the source route
        ucDllDstType = DLL_DEST_TYPE_SHORT_ADDR;
        aucGenericDstAddr[0] = g_aSrcRoutesTable[ucIdx].m_aSrcRouteHops[0]; // LSB
        aucGenericDstAddr[1] = g_aSrcRoutesTable[ucIdx].m_aSrcRouteHops[0] >> 8; //MSB

        #undef SrcTableLength
      }
    }
    else
    {
      if( 0xFFFF == g_unNickname )
      {
        ucDllDstType = DLL_DEST_TYPE_SHORT_ADDR;
        aucGenericDstAddr[0] = g_unNeighbor; // LSB
        aucGenericDstAddr[1] = g_unNeighbor >> 8; //MSB
      }
      else
      {
        ucDllDstType = DLL_DEST_TYPE_GRAPH;
        aucGenericDstAddr[0] = pRoute->m_unGraphId; // LSB
        aucGenericDstAddr[1] = pRoute->m_unGraphId >> 8; //MSB
      }
    }
  }
#endif

  // add the control byte
  aTxBufer[0] = ucCtrlByte;


  // now build the security header

  // Counter = 0, MIC = 0 before encryption
  if (ucCtrlByte & 0xC0)
  { // long DstAddr or SrcAddr
    aTxBufer[ucTxLen++] = SecurityTypeCodes_JoinKeyed;
    memset(aTxBufer + ucTxLen, 0x00, 8);
    ucTxLen += 8;
  }
  else
  {
    aTxBufer[ucTxLen++] = SecurityTypeCodes_SessionKeyed;
    memset(aTxBufer + ucTxLen, 0x00, 5);
    ucTxLen += 5;
  }

////////////////////////// TEST -doinel.alban
#ifdef TEST_HCFRD
const uint8 acHCFAppData[59] = {
  0x8F, 0x00, 0x00, 0x03, 0xC1, 0x10, 0x4B, 0xAD, 0xAA, 0x14, 0xAE, 0x16, 0x3C, 0x6D, 0xF9, 0x18,
  0x9F, 0x56, 0x5D, 0x25, 0x1A, 0xFE, 0x03, 0xC2, 0x02, 0x00, 0x02, 0x03, 0xC3, 0x1D, 0x00, 0xF9,
  0x80, 0xF9, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xA1, 0x6D, 0xF6, 0x7B, 0x8A, 0xE8,
  0x8A, 0xBE, 0x42, 0xDE, 0xC4, 0x5B, 0x62, 0x67, 0x38, 0x65, 0x00
};
const uint8 ucHCFAppLen = 59;

const uint8 acHCFNetHdr[27] = {
//0x84, 0x7E, 0x0A, 0xB0, 0x00, 0x01, 0x00, 0x1B, 0x1E, 0xF8, 0x70, 0x00, 0x00, 0x01, 0xF9, 0x80,
//0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xDD, 0xC7, 0x6B, 0x56
  0x84, 0x00, 0x0A, 0xB0, 0x00, 0x01, 0x00, 0x1B, 0x1E, 0xF8, 0x70, 0x00, 0x00, 0x01, 0xF9, 0x80,
  0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const uint8 ucHCFNetHdrLen = 27;

  memcpy((void*)p_pNsduData, acHCFAppData, ucHCFAppLen);
  p_ucNsduDataLen = ucHCFAppLen;
  memcpy(aTxBufer, acHCFNetHdr, ucHCFNetHdrLen);
  ucTxLen = ucHCFNetHdrLen;
#endif
//////////////////////////


  // packet encryption

  // copy the payload (protect the transport entry in case of retries)
  memcpy(aTxBufer + ucTxLen, p_pNsduData, p_ucNsduDataLen);

#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER
  uint8 uaPayload[TL_MAX_PAYLOAD+16];
#else // same buffer support ..
  #define uaPayload (aTxBufer + ucTxLen)
#endif

  #define p (p_pSession)

  // Nonce construction
  if( ucCtrlByte & 0x80 )
  {
    aNonce[0] = 1; // N[0]=1 when join response
    p_pSession->m_ulMyNonceCounter = p_pSession->m_ulPeerNonceCounter;
  }
  else
  {
    aNonce[0] = 0; // N[0] is set to zero
//    p_pSession->m_ulMyNonceCounter++;
    p->m_ulMyNonceCounter++;
  }

  InverseMemcpy(aNonce+1, &p->m_ulMyNonceCounter, 4);

  if( 0xFFFF == g_unNickname )
  {
    memcpy(aNonce + 5, c_aHCF_OUI_BE, 3);
    memcpy(aNonce + 8, g_aJoinDevUID_BE, 5);
  }
  else
  {
    memset(aNonce+5, 0x00, 6);
    aNonce[11] = g_unNickname >> 8;
    aNonce[12] = g_unNickname;
  }

  AES_Crypt_User( p_pSession->m_aSessionKey,
                  aNonce,
                  aTxBufer,
                  ucTxLen,
                  aTxBufer + ucTxLen,
                  p_ucNsduDataLen,
                  (uint8 *)uaPayload );

#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER
  memcpy(aTxBufer + ucTxLen, uaPayload, p_ucNsduDataLen + 4);
#else
  #undef uaPayload
#endif

  // write TTL byte into the NET header
  aTxBufer[1] = g_stProv.ucDefaultTTL;

  // security sub-header -> write COUNTER and MIC fields
  if( ucCtrlByte & 0xC0 )
  { // NL_JOIN_KEYED
    ucTxLen -= 8;
    InverseMemcpy(aTxBufer + ucTxLen, &p_pSession->m_ulMyNonceCounter, 4); // write counter
    ucTxLen += 4;
    memcpy(aTxBufer + ucTxLen, aTxBufer + ucTxLen + 4 + p_ucNsduDataLen, 4); // write MIC
    ucTxLen += 4;
  }
  else
  { // NL_SESSION_KEYED
    ucTxLen -= 5;
    aTxBufer[ucTxLen++] = (uint8)p->m_ulMyNonceCounter; // write counter
    memcpy(aTxBufer + ucTxLen, aTxBufer + ucTxLen + 4 + p_ucNsduDataLen, 4); // write MIC
    ucTxLen += 4;
  }

  #undef p

#ifdef BACKBONE_SUPPORT

  // add the network message header, see: Access Point - Serial Protocol
  if( 0xFFFF == g_unNickname )
  {
    memmove(aTxBufer + 9, aTxBufer, ucTxLen + p_ucNsduDataLen);

    // add control(1 byte): BIT7 .. BIT4  = 0; BIT3 = unicast/broadcast DstAddr; BIT2 = short/long SrcAddr; BIT1,BIT0 = priority
    aTxBufer[0]  = p_ucPriority & 0x03;
    aTxBufer[0] |= BIT2;

    // add SrcAddr(8 bytes)
    memcpy(aTxBufer + 1, c_aHCF_OUI_BE, 3);
    memcpy(aTxBufer + 4, g_aJoinDevUID_BE, 5);

    // update length
    ucTxLen += 9;
  }
  else
  {
    memmove(aTxBufer + 3, aTxBufer, ucTxLen + p_ucNsduDataLen);

    // add control(1 byte): BIT7 .. BIT4  = 0; BIT3 = unicast/broadcast DstAddr; BIT2 = short/long SrcAddr; BIT1,BIT0 = priority
    aTxBufer[0]  = p_ucPriority & 0x03;

    // add SrcAddr(2 bytes)
    aTxBufer[1] = g_unNickname >> 8; // MSB
    aTxBufer[2] = g_unNickname; // LSB

    // update length
    ucTxLen += 3;
  }

  // add serial message and generate confirm
  ucRC = UART_LINK_AddMessage( UART_MSG_TYPE_NWK, ucTxLen + p_ucNsduDataLen, aTxBufer );

//  if( ucRC ) ucRC = UNSPECIFIED_ERROR;

  return ucRC;

#else
  {
    uint8 ucBufLimit;
    if(p_ucPriority == DLL_PACKET_PRIORITY_ALARM)
    {
       ucBufLimit = DLL_MAX_PACKET_BUFFERS/4;  // to limit the self-generated Alarm packets on queue to DLL_MAX_PACKET_BUFFERS/4
    }
    else if(p_ucPriority < DLL_PACKET_PRIORITY_COMMAND)
    {
       ucBufLimit = DLL_MAX_PACKET_BUFFERS-1;  // at least one buffer should be reserved for Cmd packets
    }
    else
    {
       ucBufLimit = DLL_MAX_PACKET_BUFFERS;  // no restriction for Cmd packets
    }
      
    MONITOR_ENTER();
    if( g_ucDllAllQueueSize < ucBufLimit )
    {
      ucRC = DLL_TX_Request( p_hHandle,
                            ((uint16)aTxBufer[2]<<8) + aTxBufer[3],
                            ucTxLen + p_ucNsduDataLen,
                            aTxBufer,
                            p_ucPriority,
                            ucDllDstType,
                            DLL_PACKET_TYPE_DATA,
                            (void*)aucGenericDstAddr
                              );
      
    }
    else
    { // DLL queue full -> decrement the nonce counter
      p_pSession->m_ulMyNonceCounter --; 
    }  
      
    MONITOR_EXIT();
  }

#ifndef BACKBONE_SUPPORT   
  if( SUCCESS == ucRC )
  {
      g_stLoopDetect.m_unMyTxMsgCtr++; 
  }
#endif
  
  return ucRC;

#endif
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_TransmitIndicate
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by DLL Layer (or by serial interface on TR) when a packet is received
  // Parameters:
  //                p_ucLocalStatus - unicast/broadcast DLPDU
  //                p_ucPriority  - message priority
  //                p_ucNsduDataLen - NSDU data length
  //                p_pNsduData - NSDU data buffer
  // Return: success / error code (see CommonResponseCode.h)
  // Obs:
  //      Access level: interrupt level
  ///////////////////////////////////////////////////////////////////////////////////
uint8 NET_TransmitIndicate( uint8  p_ucLocalStatus,
                            uint8  p_ucPriority,
                            uint8  p_ucNsduDataLen, // including 4-byte DLL MIC
                            uint8* p_pNsduData )
{

#define me  p_pNsduData

  uint16 unValue;
//  uint8 ucOldPkt;
  uint8 ucCtrlByte = *me,
        ucDllDstType = 0xFF,
        aucDstAddr[8],
        ucNetHdrLen = 6, // fixed-length fields in NET header
        ucRoutePacket = 0;



  if( ucCtrlByte & 0x80 )
  { // long DstAddr

    if( memcmp(me + 9, g_aJoinDevUID_BE, 5) )
    { // forward the packet, first check TTL and ASN

      if( (g_ucNetState == NET_STATE_OPERATIONAL ) || (g_ucNetState == NET_STATE_QUARANTINED ) )// condition to forward a packet
      {
        ucRoutePacket |= FORWARD_PACKET;

        ucNetHdrLen += 8;

        if( /*ucOldPkt =*/ NET_ValidatePacket(me) )
        {
          return RCS_N00_Success; // discard packet due to TTL or maxPacket Age, but send ACK
//          return ucOldPkt; // discard packet due to TTL or maxPacket Age, but send ACK with RC=1 -invalid!!!
        }
        else
        {
          if( ucCtrlByte & 0x04 )
          { // proxy routing, extract proxy

            ucNetHdrLen += 2;

            unValue = *(me + 16);
            unValue <<= 8;
            unValue += *(me + 17);

            if( unValue == g_unNickname )
            {
              ucDllDstType = DLL_DEST_TYPE_LONG_ADDR;
              InverseMemcpy(aucDstAddr, me + 6, 8);
            }
          }
        }
      }
      else
      {
        return 0xFF; // discard packet
      }
    }
    else
    { // moved the flash writing out of the interrupt! (see NET_InterpretPacket() )
//      // Join Response, clear restart reason + state
//      if( g_stProv.aRestartReason[0] | g_stProv.aRestartReason[1] | g_stProv.aRestartReason[2] )
//      {
//        memset(g_stProv.aRestartReason, 0x00, 3); 
//        
//        // clear restart reason & state in NVM 
//        NVM_WriteIndividualRecord(23, (uint8*)&g_stProv + c_aProvNvmTable[22].m_unOffset, 
//                                                             c_aProvNvmTable[22].m_ucSize);
//      } 
    } 
  }
  else
  { // short DstAddr, extract it
    
    unValue = *(me + 6);
    unValue <<= 8;
    unValue += *(me + 7);
 
#ifndef BACKBONE_SUPPORT 
    if( !(ucCtrlByte & 0x40) ) // short SrcAddr 
    { 
        // extract originator address
        uint16 unOriginator = ((uint16)(*(me + 8)) << 8) + *(me + 9);
        
        // check for loop
        if( unOriginator == g_unNickname )
        { 
            g_stLoopDetect.m_unMyselfLoopCtr++; 
        }
    }
#endif    

    if( unValue != g_unNickname )
    { // forward the packet, first check TTL and ASN

      if( (g_ucNetState == NET_STATE_OPERATIONAL ) || (g_ucNetState == NET_STATE_QUARANTINED ) )// condition to forward a packet
      {
#ifdef BACKBONE_SUPPORT
        if( (0xF980 == unValue) || (0xF981 == unValue) )
        { // add the network message header, see: Access Point - Serial Protocol
          // short SrcAddr: the packet is always sent from TR to BBR, and TR is OPERATIONAL (has a nickname)

//          if( NET_ValidatePacket(me) )
//          {
//            return RCS_N00_Success; // discard packet due to TTL or maxPacket Age, but send ACK
//          }

          uint8 aNwkMsgHdr[3];  // tmp buffer

          memcpy(aNwkMsgHdr, p_pNsduData - 3, 3); // save the DLL buffer

          // add control(1 byte): BIT7 .. BIT4  = 0; BIT3 = unicast/broadcast DstAddr; BIT2 = short/long SrcAddr; BIT1,BIT0 = priority
          *(p_pNsduData - 3)  = p_ucPriority & 0x03;

          // add SrcAddr(2 bytes)
          *(p_pNsduData - 2) = g_unNickname >> 8; // MSB
          *(p_pNsduData - 1) = g_unNickname; // LSB

          // add serial message
          uint8 ucRC = UART_LINK_AddMessage( UART_MSG_TYPE_NWK, p_ucNsduDataLen - 4 + 3, p_pNsduData - 3 ); // 4 bytes DLL MIC

          // re-build the DLL buffer before return
          memcpy(p_pNsduData - 3, aNwkMsgHdr, 3);

          return (ucRC ? RCS_E61_NoBuffersAvailable : RCS_N00_Success);
        }
#endif
        if( unValue == 0xFFFF )
        { // DstAddr = broadcast address

          ucRoutePacket |= BROADCAST_NPDU;

          // store the packet, do nothing if the NET queue is full ( ??? TBD )
          NET_StorePacket(p_ucPriority, p_ucNsduDataLen, me);
        }

        ucRoutePacket |= FORWARD_PACKET;

        ucNetHdrLen += 2;

        if( /*ucOldPkt =*/ NET_ValidatePacket(me) )
        {
          #ifndef BACKBONE_SUPPORT 
            g_stLoopDetect.m_unHopNoLoopCtr++; // increment loop detection counter
          #endif
            
          return RCS_N00_Success; // discard packet due to TTL or maxPacket Age, but send ACK
//          return ucOldPkt; // discard packet due to TTL or maxPacket Age, but send ACK with RC=1 -invalid!!!
        }
        else if( ucCtrlByte & 0x04 )
        { // proxy routing with short dest address, extract proxy
          ucNetHdrLen += 2;
          
          uint16 unProxy;
          unProxy = *(me + 10);
          unProxy <<= 8;
          unProxy += *(me + 11);
          
          if( unProxy == g_unNickname )
          {
            ucDllDstType = DLL_DEST_TYPE_PROXYSHORT_ADDR;
            aucDstAddr[0] = unValue;      // LSB
            aucDstAddr[1] = unValue >> 8; // MSB
          }
        }
        else 
        {
          uint8 ucNeighIdx = DLL_FindShortNeighbor(unValue);
          if( (g_ucDllLkdNeighborsSize > ucNeighIdx) && (g_aDllNeighbors[ucNeighIdx].m_bLinked & LinkOptionFlagCodesMask_Transmit) )
          { // unicast NPDU destination matches a linked neighbor and has a (Normal) Tx link to that neighbor, then route the packet directly to it
            ucDllDstType = DLL_DEST_TYPE_SHORT_ADDR;
            aucDstAddr[0] = unValue;      // LSB
            aucDstAddr[1] = unValue >> 8; // MSB
          }
        }
      }
      else
      {
        return 0xFF; // device not operational, discard the packet with no ACK
      }
    }
  }

  if( ucRoutePacket & FORWARD_PACKET ) // packet must be forwarded
  {

    // DLPDU priority and Flow Control
    if( DLL_PACKET_PRIORITY_ALARM == p_ucPriority )
    { // "Alarm" priority

      //the single buffer reserved for that class is busy -> discard
      if( g_ucDllAlarmsCounter )
        return RCS_E62_NoAlarmEventBuffersAvailable;  // no more than one Alarm packet shall be buffered (spec-075 8.3 DLPDU Priority and Flow Control)
      else if(g_ucDllAllQueueSize >= (DLL_MAX_PACKET_BUFFERS - 1))
        return RCS_E61_NoBuffersAvailable;            // at least one buffer should be reserved for Command packets
    }
    else if( p_ucPriority < DLL_PACKET_PRIORITY_COMMAND )
    { // "Process-Data" or "Normal" priority

      if( p_ucPriority < g_ucDllPriorityThreshold )
      {
        return RCS_E63_PriorityTooLow;
      }
      else if(  ((DLL_PACKET_PRIORITY_DATA == p_ucPriority) && (g_ucDllAllQueueSize >= (DLL_MAX_PACKET_BUFFERS*(uint16)3)/4))
              ||((DLL_PACKET_PRIORITY_NORMAL == p_ucPriority) && (g_ucDllAllQueueSize >= DLL_MAX_PACKET_BUFFERS/2))  )
      {
        return RCS_E61_NoBuffersAvailable;
      }
    }
    else if(g_ucDllAllQueueSize >= DLL_MAX_PACKET_BUFFERS)
    { // "Command" priority and buffer full
      return RCS_E61_NoBuffersAvailable;
    }

    if( 0xFF == ucDllDstType )
    { // Table 19 implementation

      uint8 idx, len; // used for source routing

      // first collect the routing info in ucRoutePacket
      if( DLPDU_BROADCAST == p_ucLocalStatus )
      {
        ucRoutePacket |= BROADCAST_DLPDU;
      }

      // extract the graph id
      unValue = *(me + 4);
      unValue <<= 8;
      unValue += *(me + 5);

      if( 0xFFFF != unValue )
      { // valid graph id
        ucRoutePacket |= IS_GRAPH;
      }

      // long/short SrcAddr
      if( ucCtrlByte & 0x40 )
      {
        ucNetHdrLen += 8;
      }
      else
      {
        ucNetHdrLen += 2;
      }

      if( ucCtrlByte & 0x01 )
      { // source routing

        if( 0xFFFF != g_unNickname ) // test if valid nickname
        {
          ucRoutePacket |= IS_SOURCE_ROUTING;

          len = (ucCtrlByte & 0x02) ? 16 : 8; // 1/2 source route segment

          for(idx = 0; idx < len; idx += 2)
          { // find the number of hops
            if(  (*(me + ucNetHdrLen + idx) == 0xFF)
               &&(*(me + ucNetHdrLen + idx + 1) == 0xFF) )
              break;
          }

          if(idx < len)
            len = idx;

          for(idx = 0; idx < len; idx += 2)
          { // find me among the hops
            if(  (*(me + ucNetHdrLen + idx) == (uint8)(g_unNickname >> 8))
               &&(*(me + ucNetHdrLen + idx + 1) == (uint8)g_unNickname) )
              break;
          }
        }
        else
        {
          return 0xFF; // discard packet
        }
      }

      // routing info collected, now forward the packet based on Table 19
      switch( ucRoutePacket & 0x0F )
      {
        case 0x0C:
                   // Source = Yes, Graph = Yes, DLPDU = Unicast, NPDU = Unicast

                   if( idx < (len - 2) )
                   { // I am not the last hop

                     idx += 2; // next hop index

                     // save graphId for future use: if the next hop doesn't match any of my neighbors -> graph routing
                     ucDllDstType = DLL_DEST_TYPE_GRAPH;
                     aucDstAddr[0] = unValue; // graph's LSB
                     aucDstAddr[1] = unValue >> 8; // graph's MSB

                     // store the next hop
                     unValue  = *(me + ucNetHdrLen + idx) << 8;
                     unValue += *(me + ucNetHdrLen + idx + 1);

                     uint8 ucNeighIdx = DLL_FindShortNeighbor(unValue);
                     if( (g_ucDllLkdNeighborsSize > ucNeighIdx) && (g_aDllNeighbors[ucNeighIdx].m_bLinked & LinkOptionFlagCodesMask_Transmit) )
                     { // source routing: forward the packet to the next hop if have a (Normal) Tx link to it (overwrite the graph routing)
                       ucDllDstType = DLL_DEST_TYPE_SHORT_ADDR;
                       aucDstAddr[0] = unValue; // hop's LSB
                       aucDstAddr[1] = unValue >> 8; // hop's MSB
                     }
//TODO:              // else if( graphId not found in hash )
                     // { send APP_ALARM_C789: SrcRoute error;
                     //   return RCS_N00_Success; }

                   }
                   else
                   { // source route exhausted (no hop matches my address OR I am the last hop)

                     // graph routing
                     ucDllDstType = DLL_DEST_TYPE_GRAPH;
                     aucDstAddr[0] = unValue; // graph's LSB
                     aucDstAddr[1] = unValue >> 8; // graph's MSB
                   }

                   break;

        case 0x0D:
                   // Source = Yes, Graph = Yes, DLPDU = Unicast, NPDU = Broadcast

                   if( idx < (len - 2) )
                   { // I am not the last hop -> forward the packet to the next hop

                     idx += 2; // next hop index

                     // store the next hop
                     unValue  = *(me + ucNetHdrLen + idx) << 8;
                     unValue += *(me + ucNetHdrLen + idx + 1);

                     // source routing
                     ucDllDstType = DLL_DEST_TYPE_SHORT_ADDR;
                     aucDstAddr[0] = unValue; // hop's LSB
                     aucDstAddr[1] = unValue >> 8; // hop's MSB
                   }
                   else
                   { // source route exhausted (no hop matches my address OR I am the last hop)

                     // graph routing
                     ucDllDstType = DLL_DEST_TYPE_GRAPH;
                     aucDstAddr[0] = unValue; // graph's LSB
                     aucDstAddr[1] = unValue >> 8; // graph's MSB
                   }

                   break;

        case 0x0E:
        case 0x0F:
                   // Source = Yes, Graph = Yes, DLPDU = Broadcast, NPDU = X
                   ucDllDstType = DLL_DEST_TYPE_GRAPH;
                   aucDstAddr[0] = unValue; // graph's LSB
                   aucDstAddr[1] = unValue >> 8; // graph's MSB
                   break;

        case 0x08:
        case 0x0A:
                   // Source = Yes, Graph = No, DLPDU = X, NPDU = Unicast

                   if( idx >= (len - 2) )
                   { // source route exhausted (no hop matches my address OR I am the last hop)

                     // find out the MIC position inside the NPDU
                     ucNetHdrLen += (ucCtrlByte & 0x02) ? 16 : 8; // 1/2 source route segment
                     ucNetHdrLen += (*(me + ucNetHdrLen) & 0x0F) ? 1+4 : 1+1; // Security Control Byte + Counter Length

                     // signal a source route error
                     APP_AddToAlarmBuffer(APP_ALARM_C789,
                                          ((uint16)(*(me + 6))<<8)+(*(me + 7)), // final dest address ???always short???
                                          me + ucNetHdrLen);                    // pointer to NL MIC in NPDU
                     // avoid a retry since this source route is failing
                     return RCS_N00_Success;
                   }
                   else
                   { // I am not the last hop -> forward the packet to the next hop

                     idx += 2; // next hop index

                     // store the next hop
                     unValue  = *(me + ucNetHdrLen + idx) << 8;
                     unValue += *(me + ucNetHdrLen + idx + 1);

                     // source routing
                     ucDllDstType = DLL_DEST_TYPE_SHORT_ADDR;
                     aucDstAddr[0] = unValue; // hop's LSB
                     aucDstAddr[1] = unValue >> 8; // hop's MSB
                   }
                   break;

        case 0x09:
        case 0x0B:
                   // Source = Yes, Graph = No, DLPDU = X, NPDU = Broadcast

                   if( idx < (len - 2) )
                   { // I am not the last hop -> forward the packet to the next hop

                     idx += 2; // next hop index

                     // store the next hop
                     unValue  = *(me + ucNetHdrLen + idx) << 8;
                     unValue += *(me + ucNetHdrLen + idx + 1);

                     // source routing
                     ucDllDstType = DLL_DEST_TYPE_SHORT_ADDR;
                     aucDstAddr[0] = unValue; // hop's LSB
                     aucDstAddr[1] = unValue >> 8; // hop's MSB
                   }
                   else
                   { // source route exhausted (no hop matches my address OR I am the last hop)
                     return 0xFF; // discard packet
                   }

                   break;

        case 0x06:
        case 0x07:
                   // Source = No, Graph = Yes, DLPDU = Broadcast, NPDU = X

                   if( unValue < 0x100 ) // GraphId = SuperframeId
                   {
                     ucDllDstType = DLL_DEST_TYPE_SUPERFRAME;
                     aucDstAddr[0] = unValue; // graph's LSB
                     aucDstAddr[1] = unValue >> 8; // graph's MSB
                   }
                   else
                   {
                     return 0xFF; // discard packet
                   }
                   break;

        case 0x04:
                   // Source = No, Graph = Yes, DLPDU = Unicast, NPDU = Unicast

                   // check the graph/connection presence
                   if(DLL_FindFirstGraphIdx(unValue) == DLL_INVALID_CONN_IDX)
                   { // signal a graph route error
                     APP_AddToAlarmBuffer(APP_ALARM_C790, unValue, NULL);
                     // the precedent hop should redirect this packet to other device
                     return RCS_E61_NoBuffersAvailable;  // to avoid a retry;
                   }

                   // graph routing
                   ucDllDstType = DLL_DEST_TYPE_GRAPH;
                   aucDstAddr[0] = unValue; // graph's LSB
                   aucDstAddr[1] = unValue >> 8; // graph's MSB
                   break;

        case 0x05:
                   // Source = No, Graph = Yes, DLPDU = Unicast, NPDU = Broadcast

                   // forward the packet to all neighbors in the graph; NPDU discarded at end of graph

                   break;

        // any other combination is illegal and result in the NPDU being discarded !!

        default:   return 0xFF; // discard packet

      }; // END SWITCH
    }
      
    // update handle
    g_unPacketFwdHandle = (g_unPacketFwdHandle == 0x6FFF) ? 0x0100 : (g_unPacketFwdHandle + 1);

    unValue = *(p_pNsduData + 2); // ASN Snippet
    unValue <<= 8;
    unValue += *(p_pNsduData + 3);

    DLL_TX_Request( g_unPacketFwdHandle,
                    unValue,
                    p_ucNsduDataLen - 4,  // remove trailing DLL MIC
                    p_pNsduData,
                    p_ucPriority,
                    ucDllDstType,
                    DLL_PACKET_TYPE_DATA,
                    (void*)aucDstAddr
                  );

    return RCS_N00_Success;
  }

  // packet for me, store it
  if( NET_StorePacket(p_ucPriority, p_ucNsduDataLen, p_pNsduData) )
  {
    return RCS_E61_NoBuffersAvailable; // discard packet ( ??? TBD )
  }

#undef me

  return RCS_N00_Success;
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_FlushReq
  // Author: NIVIS LLC, Ion Ticus
  // Description: It is invoked by Application Layer to delete an indicated packet
  //              from transport table and DLL queue
  // Parameters:
  //                p_hHandle  - network level handler
  // Return: None
  // Obs:
  //      Access level: User level
  //      Context: Must be be followed by a flush confirmation
  ///////////////////////////////////////////////////////////////////////////////////
void NET_FlushReq( HANDLE p_oHandle )
{
  uint8 ucIdx,
        ucPktFound=0xFF;

  // look into the transport table
  ucIdx = FindTransportEntryByHandle( p_oHandle );

  if( ucIdx < MAX_TRANSPORT_NO )
  { // packet found -> flush it
    g_aTransportTable[ucIdx].m_bActive = FALSE; // flush the packet
    ucPktFound = 1;
  }

#ifdef TRANSPORT_EXTRA_BUFFER
  
  // inspect the NET extra buffer
  if( g_ucXBuffProperties & BUFFER_BUSY_MASK )
  {
    if( p_oHandle == g_stXBuff.m_hTPDUHandle )
    {
      g_ucXBuffProperties &= ~(BUFFER_BUSY_MASK | TRANSP_IDX_MASK); // clear extra buffer
      ucPktFound = 1;
    }
  }

#endif  
  
  // remove the packet from DLL queue (if exist)
  ucPktFound = DLL_FlushRequest( p_oHandle );

  if( 0xFF != ucPktFound )
  {
    NET_FlushConfirm(p_oHandle, NL_SUCCESS);
  }
  else
  {
    NET_FlushConfirm(p_oHandle, NL_NO_PACKET);
  }
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_FlushConfirm
  // Author: NIVIS LLC, Ion Ticus
  // Description: It is invoked by Network Layer to confirm if the packet was deleted or not.
  // Parameters:
  //                p_hHandle  - network level handler
  //                p_ucLocalStatus - flush result code
  // Return: None
  // Obs:
  //      Access level: User level
  //      Context: Must be preceded by a flush request
  ///////////////////////////////////////////////////////////////////////////////////
void NET_FlushConfirm( HANDLE p_oHandle, unsigned char p_ucLocalStatus )
{
  // TBD
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_SendJoinReq
  // Author: NIVIS LLC, Dorin Pavel
  // Description: send a join request
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NET_SendJoinReq (void)
{
#ifndef BACKBONE_SUPPORT
  static uint8 aSortedAdv[MAX_ADV_NEIGHBORS];
  uint8  ucIdx;
#endif
  
  uint8 aOutputBuffer[MAX_JOIN_APDU_SIZE];  
  BinaryStream stOutputStream;
  STREAM_INIT(&stOutputStream, aOutputBuffer, sizeof(aOutputBuffer));

    // build JoinReq payload
    uint8 aComposedCommand[sizeof(C787_ReportNeighborSignalLevels_Resp)];  // largest of 0, 20, 787
//    _command_size_t ucWrittenBufferBytes;
//    _command_size_t ucOutputBufferSize = MAX_JOIN_APDU_SIZE;
    uint8 ucRespCode;

    // Cmd0 response
    ucRespCode = Execute_C000_ReadUniqueIdentifier(NULL,
                                                  (C000_ReadUniqueIdentifier_Resp*)aComposedCommand,
                                                  NM_NICKNAME, // NM
                                                  WIRELESS_CMD,
                                                  MAX_JOIN_APDU_SIZE - 4);   // 2 for cmdID, 1 for dataLen, 1 for respCode

    ComposeResponseToCommmand(0xFF, (void*)aComposedCommand, CMDID_C000_ReadUniqueIdentifier, NULL, ucRespCode, &stOutputStream);

    // Cmd20 response
    ucRespCode = Execute_C020_ReadLongTag(NULL,
                                                  (C020_ReadLongTag_Resp*)aComposedCommand,
                                                  NM_NICKNAME, // NM
                                                  WIRELESS_CMD,
                                                  STREAM_GetRemainingSize(&stOutputStream) - 4);   // 2 for cmdID, 1 for dataLen, 1 for respCode

    ComposeResponseToCommmand(0xFF, (void*)aComposedCommand, CMDID_C020_ReadLongTag, NULL, ucRespCode, &stOutputStream);

#ifdef BACKBONE_SUPPORT
// Send Join Request from Transceiver, through BBR, to NM

  // Cmd787 response
  // for now, we will put the GW as the only neighbor in Cmd787 response
  // TODO[enhancement]: after listening for a while, TR send the Join Request containing two heard neighbors having the strongest RSL
#define p       ((C787_ReportNeighborSignalLevels_Resp*)aComposedCommand)
  p->m_ucNeighborTableIndex = 0;
  p->m_ucNoOfNeighborEntriesRead = 1;
  p->m_ucTotalNoOfNeighbors = 1;
  p->m_aNeighbors[0].RSLindB  = 0xDD;   // dummy value
  p->m_aNeighbors[0].nickname = GW_NICKNAME;
#undef p

  ComposeResponseToCommmand(0xFF, (void*)aComposedCommand, CMDID_C787_ReportNeighborSignalLevels, NULL, RCS_N00_Success, &stOutputStream);

  // Gateway is always the proxy device for TR Join
  // A hardcoded GraphId will be used to communicate to NM during join process

  // create the joining route to NM
  g_aRoutesTable[0].m_ucRouteId = 0x00;
  g_aRoutesTable[0].m_unDestNickname = NM_NICKNAME;
  g_aRoutesTable[0].m_unGraphId = TR_NM_GRAPHID;
  g_aRoutesTable[0].m_bSourceRouteAttached = 0;
  // create the pre-defined route to GW (Nivis exception - on TR only)
  g_aRoutesTable[1].m_ucRouteId = 0x01;
  g_aRoutesTable[1].m_unDestNickname = GW_NICKNAME;
  g_aRoutesTable[1].m_unGraphId = TR_NM_GRAPHID;
  g_aRoutesTable[1].m_bSourceRouteAttached = 0;
  g_ucRoutesNo = 2;

  // send join request
  TL_TransmitRequest ( (0xB000 | (++g_ucAdvIdx)),
                       NET_ADDR_SHORT_ADDR,
                       c_aNetManagerAddr,
                       DLL_PACKET_PRIORITY_COMMAND,
                       0,
                       PUBLISH_NOTIFY,
                       (stOutputStream.nextByte - aOutputBuffer), // ucJoinReqPayloadLength,
                       aOutputBuffer    // aucJoinReqPayload
                     );

#else    
// Send Join Request from a Field Device, through a Proxy Device, to NM
  // choose the JoinRequest proxy from the advertising neighbors
  if (0xFF == g_ucAdvIdx)
  { // JoinRequest sent first time
//    for (ucIdx=0; ucIdx<g_ucAdvNeighborNmb; ucIdx++)
//    { //TODO: the actual RSL values should be taken again from NeighborTable, and JoinPriority must be considered, too
//      if ( g_astAdvNeighbors[ucIdx].m_cRSL > g_stProv.cRSLThresholdGoodConnection ) // 
//        break;
//    }
//    // no neighbor with good RSL -> choose the first neighbor
//    if (ucIdx == g_ucAdvNeighborNmb)
//      ucIdx = 0;

    aSortedAdv[0] = getBestAdvIdx(0xFF);
    if(g_ucAdvNeighborNmb > 1)
    {
      aSortedAdv[1] = getBestAdvIdx(aSortedAdv[0]);
      aSortedAdv[2] = 3 - aSortedAdv[1] - aSortedAdv[0];  // MAX_ADV_NEIGHBORS = 3
    }
    ucIdx = 0;
    g_ucAdvIdx = 0;
  }
  else
  { // JoinRequest retries: choose the next neighbor
    if (++g_ucAdvIdx >= g_ucAdvNeighborNmb)
    {
      g_ucAdvIdx = 0;
    }
    ucIdx = g_ucAdvIdx;
  }

  // Cmd787 response
#define p       ((C787_ReportNeighborSignalLevels_Resp*)aComposedCommand)
  p->m_ucNeighborTableIndex = 0;
  p->m_ucNoOfNeighborEntriesRead = (g_ucAdvNeighborNmb < 2) ? 1 : 2;
  p->m_ucTotalNoOfNeighbors = g_ucDllAllNeighborsSize;
    
    
  for(uint8 ucIdxRsp = 0; ucIdxRsp < p->m_ucNoOfNeighborEntriesRead; ucIdxRsp++, ucIdx++)
  {
    if(ucIdx >= g_ucAdvNeighborNmb)
      ucIdx = 0;   // roll-over
    p->m_aNeighbors[ucIdxRsp].RSLindB  = g_astAdvNeighbors[aSortedAdv[ucIdx]].m_cRSL;
    p->m_aNeighbors[ucIdxRsp].nickname = g_astAdvNeighbors[aSortedAdv[ucIdx]].m_unNickname;
  }
  ucIdx = aSortedAdv[g_ucAdvIdx];
  
#undef p

  ComposeResponseToCommmand(0xFF, (void*)aComposedCommand, 787, NULL, RCS_N00_Success, &stOutputStream);
    
  // proxy found
  g_unNeighbor = g_astAdvNeighbors[ucIdx].m_unNickname;    // store the proxy

  MONITOR_ENTER(); // EXCLUSIVE Code Begin
  NET_ParseProxyInfo(g_unNeighbor, g_astAdvNeighbors[ucIdx].m_aucPayload);
  MONITOR_EXIT();  // EXCLUSIVE Code End
  
//  g_ucJoinRouteId = JOIN_ROUTE_ID ;              // compute the join route id

  g_aRoutesTable[0].m_ucRouteId = JOIN_ROUTE_ID;
  g_aRoutesTable[0].m_unDestNickname = NM_NICKNAME;
  g_aRoutesTable[0].m_unGraphId = g_sDllSharedParams.m_unJoinGraphID;
  g_aRoutesTable[0].m_bSourceRouteAttached = 0;
  g_ucRoutesNo = 1;
  
  
  // send join request
  TL_TransmitRequest ( (0xB000 | g_ucAdvIdx),
                       NET_ADDR_SHORT_ADDR,
                       c_aNetManagerAddr,
                       DLL_PACKET_PRIORITY_COMMAND,
                       0,
                       PUBLISH_NOTIFY,
                       (stOutputStream.nextByte - aOutputBuffer), // ucJoinReqPayloadLength,
                       aOutputBuffer    // aucJoinReqPayload
                     );
#endif  // BACKBONE_SUPPORT
  
}

#ifndef BACKBONE_SUPPORT 
uint8 getBestAdvIdx(uint8 ucExcluded)
{
  uint8 i;
  uint8 ucPrio = 15;
  int8  cRSL = -128;
  uint8 j = (ucExcluded == 0) ? 1 : 0;
  
  for(i = j; i < g_ucAdvNeighborNmb; i++)
  { // find out the advertising neighbor having the smallest JoinPriority and the greatest RSL
    if(i == ucExcluded)
      continue;
    if(g_astAdvNeighbors[i].m_ucJoinPrio <= ucPrio)
    {
      if(g_astAdvNeighbors[i].m_ucJoinPrio == ucPrio && g_astAdvNeighbors[i].m_cRSL <= cRSL)
        continue;
      
      // keep this neighbor
      j = i;
      ucPrio = g_astAdvNeighbors[i].m_ucJoinPrio;
      cRSL = g_astAdvNeighbors[i].m_cRSL;
    }
  }
  return j;
}
#endif

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_CheckTimers
  // Author: NIVIS LLC, Dorin Pavel
  // Description: decrement join timers each 250ms
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NET_CheckTimers (void)
{
  if (g_ulAdWaitTimer) // NET join procedure: advertisement timeout
    g_ulAdWaitTimer--;

  if (g_ulJoinRspTimer) // NET join procedure: join response timeout
    g_ulJoinRspTimer--;

  if (g_ulLooselyCoupledTimer) // NET join procedure: loosely coupled timeout
    g_ulLooselyCoupledTimer--;
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_doIdle
  // Author: NIVIS LLC, Dorin Pavel
  // Description: network idle mechanismus
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
static void NET_doIdle (void)
{
  // at this point the device has to be provisioned with network id and join key

  switch( g_ucJoinMode ) 
  {
    case JoinModeCode_DontAttemptToJoin:
      if(g_ucDllStatus != DLL_IDLE)
      {
        DLL_Init();
        NET_Init();
      }
      break;
    case JoinModeCode_JoinNow:
    case JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset:
      NET_Init(); // before DLL_Join() because of g_stC769 init
      DLL_Join(); // start the DLL networt search procedure
      g_ucNetState = NET_STATE_JOINING;
      break;

    default: break;
  }; // END SWITCH
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: DLL_AdvertiseIndicate
  // Author: NIVIS LLC, Dorin Pavel
  // Description: called when an advertisement is received while in DLL_ACTIVE_SEARCH
  //                or DLL_SYNCHRONIZED states
  // Parameters: neighbor
  // Return: None
  // Obs:
  //      Access level: Interrupt level
  ///////////////////////////////////////////////////////////////////////////////////
void DLL_AdvertiseIndicate (uint8 p_ucNeighborIdx)
{
////////test
#ifdef TEST_TR_BANNED
  if(g_aDllNeighbors[p_ucNeighborIdx].m_unShortAddr == 0x0701)
    return; // avoid joining through the TR
#endif
////////

  if( NET_JOIN_SEARCHING == g_ucNetJoinState )
  {
    g_stC769.networkSearchTimer.u32 = g_ulDllRecCntr * (10UL * 32); // the final value for NetworkSearchTimer param (time listening for first advertisement)

    if(g_ulDllMaxRecCntr == g_ulPassiveCycleTime)
    { // device is in PassiveWakeTime, switch to Active Search
      g_ulDllMaxRecCntr = g_stProv.ulActiveSearchShedTime;
      PUT_DLL_ACTIVE_SEARCH();
    }
    // init for the next state
    g_ucNetJoinState = NET_JOIN_ADV_NEIGHBOR;
    g_ucNetAdNumber  = g_ucAdvNeighborNmb = 0;
    g_ulAdWaitTimer  = g_stProv.ulAdWaitTimeout;     // 250ms / 250ms = 1
    g_ucAdvOverTresh = 0;
    g_ucAdvOneMorePeriod = 0;    
  }
  
  g_stC769.joinStatus |= (JoinProcessStatusMask_NetworkPacketsHeard | 
                          JoinProcessStatusMask_ASNAcquired | 
                          JoinProcessStatusMask_SynchronizedToSlotTime | 
                          JoinProcessStatusMask_AdvertisementHeard);

  // this will prevent the DLL to go to Passive Search after an advertise was heard
  g_ulDllRecCntr = 0;
  g_ulDllMaxRecCntr = g_stProv.ulActiveSearchShedTime;  

  if( NET_JOIN_ADV_NEIGHBOR == g_ucNetJoinState )
  {
    g_ucNetAdNumber ++; // new advertise received
    
    DLL_NEIGHBOR_T* pNeigh = g_aDllNeighbors + p_ucNeighborIdx;
    T_ADV_NEIGHBOR* pAdvNeigh = g_astAdvNeighbors;
 
    //check if this advertiser has RSL over threshold
    uint8 ucTresh = (pNeigh->m_cAvgRSL > g_stProv.cRSLThresholdGoodConnection);

    //parse advertisers table
    while(pAdvNeigh != (g_astAdvNeighbors + g_ucAdvNeighborNmb))
    {
      if( pNeigh->m_unShortAddr == pAdvNeigh->m_unNickname )
      {
        //check if first time over threshold, increase counter
        if(ucTresh && (pAdvNeigh->m_cRSL < g_stProv.cRSLThresholdGoodConnection)) 
          g_ucAdvOverTresh++;        
        
        //refresh the RSL and Join Priority (could be changed mean time)
        pAdvNeigh->m_cRSL = pNeigh->m_cAvgRSL;
        pAdvNeigh->m_ucJoinPrio = pNeigh->m_bJoinPriority;
        return;
      }
      pAdvNeigh++;
    }
    
    if((g_ucAdvNeighborNmb >= MAX_ADV_NEIGHBORS) || (g_ucNetAdNumber >= MIN_ADS_NEEDED))
    { // normally there's no more room for a new advertising neighbor
      // but if current advertiser has RSL over threshold, must add him!
      if(!ucTresh)
        return;
      //for now won't waste time with looking for the lowest rsl adv neighbour, we'll just replace the first under tresh found in table
      pAdvNeigh = g_astAdvNeighbors + g_ucAdvOverTresh;
      g_ucAdvNeighborNmb--; //(**)
    }
      
    // Add new neighbor
    pAdvNeigh->m_unNickname = pNeigh->m_unShortAddr;
    pAdvNeigh->m_cRSL       = pNeigh->m_cAvgRSL;
    pAdvNeigh->m_ucJoinPrio = pNeigh->m_bJoinPriority;
    // we are always using 2-byte Channel Map
    memcpy(pAdvNeigh->m_aucPayload, 
           g_sDllBufferParsed.m_pucDLPDUPayload + 9, 
           g_sDllBufferParsed.m_ucPayloadLen - 9);
    
    g_ucAdvNeighborNmb++; //(**)
    if(ucTresh)    
      g_ucAdvOverTresh++;    
  }
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_JoinProcedure
  // Author: NIVIS LLC, Dorin Pavel
  // Description: network layer join procedure
  // Parameters: None
  // Return: join status
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
static uint8 NET_JoinProcedure (void)
{
  switch (g_ucNetJoinState)
  {
    case NET_JOIN_SEARCHING: // do nothing
                             break;

    case NET_JOIN_ADV_NEIGHBOR:
                             if(!g_ucAdvNeighborNmb)
                               break;
                             if(!g_ucAdvOverTresh && !g_ucAdvOneMorePeriod)
                             {
                                //not at least one adv. over threshold heard, let's wait for one more period
                                g_ucAdvOneMorePeriod = 1;
                                g_ulAdWaitTimer  = g_stProv.ulAdWaitTimeout;
                                break;
                             }                             
                             if (
                                  (g_ucNetAdNumber >= MIN_ADS_NEEDED) || 
                                  (g_ucAdvNeighborNmb >= MAX_ADV_NEIGHBORS) || 
                                  !g_ulAdWaitTimer
                                  
                                )
                             {
                               g_ucNetJoinState = NET_JOIN_REQ_ADMISSION;
                               PUT_DLL_SYNCHRONIZED();
                             }
                             break;

    case NET_JOIN_REQ_ADMISSION:
                            // if network key, networt manager session and device's nickname are received
                            // -> go to "Loosely Coupled" state
                            if ((g_ucJoinCmdFlags & (BIT0|BIT1|BIT2)) == (BIT0|BIT1|BIT2))
                            {
                              g_ucNetJoinState = NET_JOIN_LOOSELY_COUPLED;
                              g_ulLooselyCoupledTimer = 500*4;  // 200 sec [250ms interval]
                              g_stC769.joinStatus |= JoinProcessStatusMask_Authenticated;
                              g_stC769.joinStatus &= ~(uint16)(JoinProcessStatusMask_JoinRetrying | JoinProcessStatusMask_JoinFailed);
                              
                              g_ucJoinMode = JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset;
                              if(JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset != g_stProv.ucJoinModeCode)
                              {  
                                g_stProv.ucJoinModeCode = JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset;
                                
                                // NVM -> store the individual record into FLASH 
                                // write ucJoinModeCode to FLASH
                                NVM_WriteIndividualRecord( 7, 
                                                          (uint8*)&g_stProv + c_aProvNvmTable[6].m_unOffset, 
                                                          c_aProvNvmTable[6].m_ucSize);
                              }
                            }
                            else if ( !g_ulJoinRspTimer )
                            {
                              if ( g_stC769.noOfJoinAttempts <= g_ucMaxJoinRetries )
                              {
                                PUT_DLL_JOINED();
                                UART1_String("DSjoin"); // DLL Status: joined
                                // send a join request
                                NET_SendJoinReq();

                                g_stC769.wirelessMode = WirelessOperationMode_Negotiating;

                                g_stC769.joinStatus |= JoinProcessStatusMask_JoinRequested;

                                if( g_stC769.noOfJoinAttempts )
                                  g_stC769.joinStatus |= JoinProcessStatusMask_JoinRetrying;

                                g_ulJoinRspTimer = g_ulJoinRspTimeout;     // 250ms / 250ms = 1
                              #ifdef BACKBONE_SUPPORT
                                g_stC769.noOfJoinAttempts = 1; // unlimited retry number
                              #else
                                g_stC769.noOfJoinAttempts++;
                              #endif
                              }
                              else
                              {
                                g_stC769.joinStatus |= JoinProcessStatusMask_JoinFailed;
                                g_stC769.joinRetryTimer.u32 = (g_ulJoinRspTimeout - g_ulJoinRspTimer) * (250UL * 32); // save the time since the last JReq was sent for future C769 requests
                                return NMB_OF_JOIN_REQ_EXCEEDED;
                              }
                            }
                            break;

    case NET_JOIN_LOOSELY_COUPLED:

#ifdef BACKBONE_SUPPORT
                            if (g_ucJoinCmdFlags & (BIT3))
#else
                            if ((g_ucJoinCmdFlags & (BIT3|BIT4|BIT5)) == (BIT3|BIT4|BIT5))
#endif
                            {
                              return DEVICE_JOINED;
                            }
                            else 
                              if(!g_ulLooselyCoupledTimer)
                            { // if Superframe and Links not received for 500s, consider join failed
                              g_stC769.joinStatus |= JoinProcessStatusMask_JoinFailed;  // ??? or other value???
                              return NMB_OF_JOIN_REQ_EXCEEDED;  // this will force Idle (and then Active search if Join Mode=2)
                            }
                               
                            break;

  };

  return JOIN_ACTIVE;
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_doQuarantine
  // Author: NIVIS LLC, Dorin Pavel
  // Description: what to do when devive is embraced
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
static void NET_doQuarantine (void)
{
  if( NO_HEALTH_REPORTS == g_ucHealthReportState )
  { // first pass, entering QUARANTINED state
    g_stC769.wirelessMode = WirelessOperationMode_Quarantined;
    g_stC769.joinStatus |= JoinProcessStatusMask_NetworkJoined;
    
    // change addr of the self-created Join links into 0xFFFF
    COMMIT_MODIFY_DLL_JLINKS();
    
    g_ulHealthReportTimer = g_stProv.ulHealthReportTime; // initialise the health report timer
    g_ucHealthReportState = START_HEALTH_REPORTS; // start generation of HealthReports
  }
  if(g_ucJoinCmdFlags & BIT6)
  {
    g_ucNetState = NET_STATE_OPERATIONAL;
    
    g_stC769.wirelessMode = WirelessOperationMode_Operational;
    g_stC769.joinStatus |= JoinProcessStatusMask_NegotiatingNetworkProperties;
    
    // device is operational -> clear restart reason + state
    if( g_stProv.aRestartReason[0] | g_stProv.aRestartReason[1] | g_stProv.aRestartReason[2] )
    {
      memset(g_stProv.aRestartReason, 0x00, 3); 
                    
      // clear restart reason & state in NVM 
      NVM_WriteIndividualRecord(23, (uint8*)&g_stProv + c_aProvNvmTable[22].m_unOffset, 
                                              c_aProvNvmTable[22].m_ucSize);
    }
  }  
}


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_doOperation
  // Author: NIVIS LLC, Dorin Pavel
  // Description: device functionality when joined
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
//static void NET_doOperation (void)
//{
//  g_stC769.joinStatus = OPERATIONAL;
//
//  g_stC769.wirelessMode |= (NEGOTIATE_NETWORK | NORMAL_OPERATION);
//}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_doSuspend
  // Author: NIVIS LLC, Dorin Pavel
  // Description: sends the device in idle state
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
static void NET_doSuspend (void)
{
  g_stC769.wirelessMode = WirelessOperationMode_Suspended;
  //NET_doIdle();
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_StateMachine
  // Author: NIVIS LLC, Dorin Pavel
  // Description: This implements the network layer state machine
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NET_StateMachine (void)
{
  switch (g_ucNetState)
  {
    case NET_STATE_IDLE:        NET_doIdle();
                                break;

    case NET_STATE_JOINING:     switch ( NET_JoinProcedure() )
                                {
                                  case JOIN_ACTIVE:  // nothing tbd
                                                     break;

                                  case NMB_OF_JOIN_REQ_EXCEEDED:
                                                     /*
                                                     if(JoinModeCode_JoinNow == g_ucJoinMode)
                                                       g_ucJoinMode = JoinModeCode_DontAttemptToJoin;
                                                     NET_Init();  //TODO: perform necessary initializations for other layers, too!
                                                     g_ucNetState = NET_STATE_IDLE;
                                                     break;
                                                     */
                                    
                                                     // ##DP, 22/11/2010 force a WDG reset here // this will clear 
                                                     PrepareForReset( RREASON_JOIN_REQ_EXCEEDED ); 
                                                     break;

                                  case DEVICE_JOINED:
                                                     g_ucNetState = NET_STATE_QUARANTINED;
                                                     break;

                                };
                                break;

    case NET_STATE_QUARANTINED: 
                                NET_doQuarantine();
                                break;

    case NET_STATE_OPERATIONAL: // tbd
//                                NET_doOperation();
                                break;

    case NET_STATE_SUSPENDED:   // tbd
                                NET_doSuspend();
                                break;


    default: break;
  }

}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_ForwardPacket
  // Author: NIVIS LLC, Dorin Pavel
  // Description: validate an incoming packet (TTL + ASN)
  // Parameters: ptr. to the rx packet
  // Return: 0 = success / 1 = error
  // Obs:
  //      Access level: Interrupt level
  ///////////////////////////////////////////////////////////////////////////////////
static uint8 NET_ValidatePacket( uint8* p_pNsduData )
{
  uint8  ucTTLByte = *(p_pNsduData + 1);

  uint16 unCrtASN, unPktASN;

  if( ucTTLByte )
  {
    if( 0xFF != ucTTLByte )
      ucTTLByte--; // decrement TTL

    if( !ucTTLByte )
      return 1; // discard

    *(p_pNsduData + 1) = ucTTLByte; // update the TTL
  }
  else
  { // this should never happen: TTL=0 -> discard
    return 1;
  }

  // TTL is valid, now test the packet ASN

  // get actual ASN
//  MONITOR_ENTER();                 // not needed from inside of an interrupt
//  unCrtASN = g_sDllASN.m_aucBytes[1];
//  unCrtASN <<= 8;
//  unCrtASN += g_sDllASN.m_aucBytes[0];
//  MONITOR_EXIT();
  unCrtASN = *(uint16*)&g_sDllASN.m_ulLsb;

  // packet ASN
  unPktASN = *(p_pNsduData + 2);
  unPktASN <<= 8;
  unPktASN += *(p_pNsduData + 3);

  if( unCrtASN >= unPktASN )
  {
    unCrtASN -= unPktASN;
  }
  else
  {
    unCrtASN = 0xFFFF - unPktASN + unCrtASN;
  }

  if( unCrtASN > (uint16)g_stProv.ulMaxPacketAge )
  {
    return 1; // discard
  }

  return 0;
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_StorePacket
  // Author: NIVIS LLC, Dorin Pavel
  // Description: stores a received packet (for me)
  // Parameters:
  //                p_ucPriority  - message priority
  //                p_ucNsduDataLen - NSDU data length
  //                p_pNsduData - NSDU data buffer
  // Return: None
  // Obs:
  //      Access level: interrupt level
  ///////////////////////////////////////////////////////////////////////////////////
static uint8 NET_StorePacket( uint8  p_ucPriority, uint8  p_ucNsduDataLen, uint8* p_pNsduData )
{
  uint8 ucRC = 0;
  // new packet for me, store it
  T_NET_RX_PACKET *pCrtRxPacket = g_stRecvNSDU;
  for(; pCrtRxPacket < g_stRecvNSDU + NET_RX_QUEUE_SIZE; pCrtRxPacket++)
  {
    if( pCrtRxPacket->m_ucUsedElem & UNUSED_ENTRY )
    {
      pCrtRxPacket->m_ucUsedElem &= ~UNUSED_ENTRY;
      pCrtRxPacket->m_ucPrio = p_ucPriority;
      pCrtRxPacket->m_ucPayloadLength = p_ucNsduDataLen;
      memcpy(pCrtRxPacket->m_aucPayload, p_pNsduData, p_ucNsduDataLen);
      pCrtRxPacket->m_hPacketHandler = g_ucPacketForMeHandle;
      break;
    }
  }

  // update handle
  g_ucPacketForMeHandle = (g_ucPacketForMeHandle == 0xFF) ? 0x00 : (g_ucPacketForMeHandle + 1);
  
  g_ucCanSleep = 0; // don't sleep next time, in order to process the packet from main loop 
  
  if( pCrtRxPacket >= (g_stRecvNSDU + NET_RX_QUEUE_SIZE) )
  {
    // NET rx queue is full, discard packet ??? (TBD)
    ucRC = 1;
  }
    return ucRC;
}

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_InterpretPacket
  // Author: NIVIS LLC, Dorin Pavel
  // Description: NET rx packet interpretor
  // Parameters: ptr. to the rx packet
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
static void NET_InterpretPacket(T_NET_RX_PACKET* p_RxPacket)
{
#define me    (p_RxPacket->m_aucPayload)
#define size  (p_RxPacket->m_ucPayloadLength)

  uint8 ucCtrlByte = *me,
        aNonce[13],
        ucNetHdrLen;  // = 6; // fixed-length fields in NET header

  uint16 unPeerAddr, unDestAddr;

  uint32 ulCounter = 0;

  NET_SESSION_ATTRIBUTES* pSession = NULL;


  // find the session
  switch( ucCtrlByte & 0xC0 )
  {
    case 0xC0:  // wrong control byte, discard the packet
                return;

    case 0x80:  // packet for me, long DstAddr -> JoinResponse or other proxy routed packet
                unPeerAddr = ((uint16)me[14] << 8) | me[15];
                unDestAddr = 0;

//                pSession = (NET_SESSION_ATTRIBUTES*)g_aSessionsTable; // first session in session table is JoinSession
                ucNetHdrLen = 16; // 16 = 6 + 8 + 2, 8->long DstAddr, 2->short SrcAddr
                
                /*
                { // Join Response, clear restart reason + state
                  if( g_stProv.aRestartReason[0] | g_stProv.aRestartReason[1] | g_stProv.aRestartReason[2] )
                  {
                    memset(g_stProv.aRestartReason, 0x00, 3); 
                    
                    // clear restart reason & state in NVM 
                    NVM_WriteIndividualRecord(23, (uint8*)&g_stProv + c_aProvNvmTable[22].m_unOffset, 
                                              c_aProvNvmTable[22].m_ucSize);
                  } 
                } 
                */
                break;

    default:    // device joined, normal packet

                unPeerAddr = ((uint16)me[8] << 8) | me[9];
                unDestAddr = ((uint16)me[6] << 8) | me[7];
//                pSession = FindSessionByType( NL_SESSION_TYPE_UNICAST, unPeerAddr );
                ucNetHdrLen = 10; // 10 = 6 + 2 + 2, both DstAddr and SrcAddr are short
                break;


  }; // end SWITCH

//  if( NULL == pSession )
//    return;

  if( ucCtrlByte & 0x04 )
  {
    ucNetHdrLen += 2; // proxy address
  }

  if( ucCtrlByte & 0x01 )
  {
    ucNetHdrLen += 8; // 1st source route

    if(  ucCtrlByte & 0x02 )
      ucNetHdrLen += 8; // 2nd source route
  }
  uint8 ucSecurityType = me[ucNetHdrLen] & 0x0F;  // Security Control Byte
  if(ucSecurityType == SecurityTypeCodes_SessionKeyed)
  {
    pSession = FindSessionByType( (unDestAddr == 0xFFFF) ? SessionTypeCode_Broadcast : SessionTypeCode_Unicast, unPeerAddr );
    ucNetHdrLen += 6;
  }
  else if(ucSecurityType > SecurityTypeCodes_HandheldKeyed)
    return;  // unknown security type
  else
  { // join or handheld keyed
    ucNetHdrLen += 9;
    if(ucSecurityType == SecurityTypeCodes_JoinKeyed)
      pSession = (NET_SESSION_ATTRIBUTES*)g_aSessionsTable; // first session in session table is JoinSession
    else
    { // handheld keyed
      // TBD
      return; // just until an action is defined
    }
  }

  if( NULL == pSession )
    return;

// Decryption of packet

#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER
  uint8 uaPayload[MAX_TPDU_SIZE];
#else // same buffer support ..
  #define uaPayload  (me + ucNetHdrLen)
#endif

  // payload too long -> discard the packet
  if( size > ucNetHdrLen + MAX_TPDU_SIZE + 4 )
    return;

  // copy the MIC at the end of crypted payload, then clear it in NSDU header
  memcpy(me + size - 4, me + ucNetHdrLen - 4, 4);

  // clear TLL byte in NET header
  *(me + 1) = 0;

  // first nonce byte is always 0 except when the packet is JoinResponse
  aNonce[0] = 0;

  // Nonce construction
  if( ucSecurityType == SecurityTypeCodes_JoinKeyed )
  {
    InverseMemcpy(&ulCounter, me + ucNetHdrLen - 4 - 4, 4); // save the packet counter

    memset(me + ucNetHdrLen - 4 - 4, 0x00, 4 + 4); // clear counter + MIC

    if( ucCtrlByte & 0x80 )
    { // long DstAddr -> JoinResponse
      aNonce[0] = 1;

      // MyNonceCounter != packet counter -> discard the packet
      if( pSession->m_ulMyNonceCounter != ulCounter )
        return;

      memcpy(aNonce+5, c_aHCF_OUI_BE, 3);
      memcpy(aNonce+8, g_aJoinDevUID_BE, 5);
    }
    else
    { // long SrcAddr -> JoinRequest
      pSession->m_ulMyNonceCounter = ulCounter;

      memcpy(aNonce+5, c_aHCF_OUI_BE, 3);
      memcpy(aNonce+8, pSession->m_aPeerUniqueID, 5);
    }

    InverseMemcpy(aNonce+1, &pSession->m_ulMyNonceCounter, 4);
  }
  else if( ucSecurityType == 0 )
  { // session keyed
    ulCounter = *(me + ucNetHdrLen - 4 - 1); // save the packet counter - it's one byte only!

    memset(me + ucNetHdrLen - 4 - 1, 0x00, 1 + 4); //clear counter + MIC

    // packet counter is one byte long -> nonce counter must be reconstructed !
    uint32 ulHiPeerCnt = pSession->m_ulPeerNonceCounter >> 8; // get the most significant 24 bits

    // sliding window algorithm for replay protection
    if( (uint8)pSession->m_ulPeerNonceCounter < 31 )
    {
      if(ulCounter >= ((uint8)pSession->m_ulPeerNonceCounter - 31))
        ulHiPeerCnt--;  // decrement the 24-bit value
    }
    else if(ulCounter < ((uint8)pSession->m_ulPeerNonceCounter - 31))
    { 
      ulHiPeerCnt++;  // increment the 24-bit value
    }
    ulCounter = (ulHiPeerCnt << 8) + (uint8)ulCounter;  // compose the 32-bit NPDU counter value
    
    // look into the NonceCounterHistory if a packet with this counter was already received
    if((pSession->m_ulPeerNonceCounter - ulCounter) < 32) 
    {
      if(pSession->m_ulNonceCounterHistory & (0x80000000 >> (pSession->m_ulPeerNonceCounter - ulCounter)))
      {
        return; // duplicate packet -> discard the packet
      }
    }

    // complete the nonce
    InverseMemcpy(aNonce+1, &ulCounter, 4);

    memset(aNonce+5, 0x00, 6);
    aNonce[11] = pSession->m_unPeerNickname >> 8;
    aNonce[12] = pSession->m_unPeerNickname;
  }
  else
  { // handheld keyed
    // TBD
    return;
  }

  if( AES_SUCCES != AES_Decrypt_User( pSession->m_aSessionKey,
                                      aNonce,
                                      me,
                                      ucNetHdrLen,
                                      me + ucNetHdrLen,
                                      size - ucNetHdrLen,
                                      uaPayload ) )
  {
//    if((pSession->m_ucSessionFlags & VALID_OLD_KEY) == 0)
//       )
    g_stC779.m_ucNoOfNetworkLayerMICFailuresDetected++; // inc Cmd779 counter
    return; // invalid MIC -> discard the packet
  }

  // update the PeerNonceCounter and NonceCounterHistory
  if( pSession->m_ulPeerNonceCounter - ulCounter < 32  )
  { // it's an older packet, just set the correspondent bit in nonce counter history
    pSession->m_ulNonceCounterHistory |= 1L << (31 - (pSession->m_ulPeerNonceCounter - ulCounter));
  }
  else
  { // ulCounter > pSession->m_ulPeerNonceCounter, both history and peer nonce counter must be updated
    for( ucCtrlByte = ulCounter - pSession->m_ulPeerNonceCounter; ucCtrlByte > 0; ucCtrlByte-- )
    {
      if( !(1L & pSession->m_ulNonceCounterHistory) )
      {
        g_stC779.m_ucNoOfNonceCounterValuesNotReceived++; // inc Cmd779 counter
      }
      
      pSession->m_ulNonceCounterHistory >>= 1;
    }
    pSession->m_ulNonceCounterHistory |= 0x80000000;
    pSession->m_ulPeerNonceCounter = ulCounter;
  }


#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER
  memcpy(me + ucNetHdrLen, uaPayload, size - ucNetHdrLen - 4);
#else
  #undef uaPayload
#endif

  // decryption was successful
  TL_TransmitIndicate( p_RxPacket->m_hPacketHandler,
                       p_RxPacket->m_ucPrio,
                       pSession,
                       size - ucNetHdrLen - 4,
                       me + ucNetHdrLen );

  #undef me
  #undef size
}

///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_RecvQueueManager
  // Author: NIVIS LLC, Dorin Pavel
  // Description: NET rx queue handling
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NET_RecvQueueManager (void)
{
  T_NET_RX_PACKET *pCrtRxPacket = g_stRecvNSDU;
  for(; pCrtRxPacket < g_stRecvNSDU + NET_RX_QUEUE_SIZE; pCrtRxPacket++)
  {
    if ( !(pCrtRxPacket->m_ucUsedElem & UNUSED_ENTRY) )
    {
      NET_InterpretPacket(pCrtRxPacket);
      pCrtRxPacket->m_ucUsedElem |= UNUSED_ENTRY;
      g_ucCanSleep = 0; // don't sleep 
    }
  }
}


///////////////////////////////////////////////////////////////////////////////////
// Name: NET_LoopCheck
// Author: NIVIS LLC, Erdei Eduard
// Description: Handles the loop detection mechanism
// Parameters: None
// Return: None
// Obs: Called once per second
//      Access level: User level
///////////////////////////////////////////////////////////////////////////////////
#ifndef BACKBONE_SUPPORT 
void NET_LoopCheck( void )
{   
  MONITOR_ENTER();
  
  if( g_stLoopDetect.m_unToNextResetTmr )
  {
      g_stLoopDetect.m_unToNextResetTmr--;    
  }
  else
  {      
      // reset only if two consecutive windows contained loop messages
      if( g_stLoopDetect.m_unMyTxMsgCtr && (g_stLoopDetect.m_unMyselfLoopCtr || g_stLoopDetect.m_unHopNoLoopCtr)) // at least a transmission
      {              
          if( g_stLoopDetect.m_ucDetected )
          {            
              PrepareForReset( RREASON_LOOP_DETECTED );  
          }
          else g_stLoopDetect.m_ucDetected  = 1;
      }
      else
      {
          g_stLoopDetect.m_ucDetected  = 0;
      }
      
      g_stLoopDetect.m_unToNextResetTmr = (uint16)g_stProv.ulMaxPacketAge >> 6; // ASN : 64 -> almost twice the packet age in seconds
      g_stLoopDetect.m_unMyselfLoopCtr  = 0;
      g_stLoopDetect.m_unHopNoLoopCtr   = 0;
      g_stLoopDetect.m_unMyTxMsgCtr     = 0;
  }
  
  MONITOR_EXIT();
}
#endif
