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
* Name:         datalink_layer_dm.c
* Author:       NIVIS LLC, 
* Date:         July 2008
* Description:  Data Link Layer Management
* Changes:
* Revisions:
****************************************************************************************************/
#include <string.h>
#include "../timers.h"
#include "../asm.h"
#include "datalink_layer.h"
#include "datalink_layer_dm.h"
#include "network_layer.h"
#include "network_layer_dm.h"
#include "app_layer.h"
#include "provision.h"

#define MAX_PATH_FAIL_ALARMS_NO  3 
extern void InitDevice(void);
////////////////////////////////////////////////////////////////////////////////
// Note : 
//       1. All time variables in this file are kept with ...
//              ... the least significant bit representing 10 ms !!!
//           => TIME_10MS instead of TIME
// 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
const uint8   c_aucWKKey[AES_KEY_LENGTH]      = { 0x77, 0x77, 0x77, 0x2E, 0x68, 0x61, 0x72, 0x74, 0x63, 0x6F, 0x6D, 0x6D, 0x2E, 0x6F, 0x72, 0x67 };
uint8         g_aucNetworkKey[AES_KEY_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint16        g_unNetworkId;  // the actual value used by the DLL -to be initialized from g_stProv.unNetworkId before each join process start!

uint8 g_ucDllDisconnect;

// TDMA Dinamic Status
DLL_STATUS_T          g_ucDllStatus;
DLL_XVR_STATUS_T      g_ucDllXVRStatus;
DLL_FLAGS_T           g_sDllFlags;
DLL_SLOT_PARAMS_T     g_sDllSlot;
DLL_SHARED_PARAMS_T   g_sDllSharedParams;
volatile int16        g_nDllClkDiffAck;
uint8                 g_ucDllAlarmsCounter;                   // number of Alarm packets in DLL Queue

// ASN 
DLL_ASN_T             g_sDllASN; 
// Channel Map
uint16  g_unDllChannelMap;                                    // in memory: little endian! Bit 15 = Ch15, Bit 0 = Ch0
uint8   g_ucDllActiveChannels;                                // number of active channels in ChannelMap
uint8   g_ucDllTotalChannels;                                 // number of total channels in ChannelMap (9 to 16, but Ch15 not allowed -see C818 description)
uint8   g_aDllChTable[16];                                    // equivalence table between the index of an active channel and it's position into the channel map array
// Data Buffers for DLL 
uint8                   g_aucDllBuffer[DLL_MAX_DLPDU_SIZE];   // used between DLL and PHY
uint8                   g_ucDllBufferLen;
uint8                   g_aucDllAckBuffer[DLL_MAX_ACK_SIZE];  // used between DLL and PHY
uint8                   g_ucDllAckBufferLen;
// Tables
DLL_NEIGHBOR_T    g_aDllNeighbors[DLL_MAX_NEIGHBORS];
uint8             g_ucDllLkdNeighborsSize;
uint8             g_ucDllAllNeighborsSize;
DLL_SUPERFRAME_T  g_aDllSuperframes[DLL_MAX_SUPERFRAMES + DLL_HIDDEN_SUPERFRAMES];
uint8             g_ucDllCrtSuperframesSize;
uint8             g_ucDllAllSuperframesSize;
uint8             g_ucDllDeletedSuperframes;
DLL_LINK_T        g_aDllLinks[DLL_MAX_LINKS + DLL_HIDDEN_LINKS];
uint8             g_ucDllCrtLinksSize;
uint8             g_ucDllAllLinksSize;
uint8             g_ucDllDeletedLinks;
DLL_CONNECTION_T  g_aDllConnections[DLL_MAX_CONNECTIONS + DLL_HIDDEN_CONNECTIONS];
uint8             g_ucDllCrtConnectionsSize;
uint8             g_ucDllAllConnectionsSize;
uint8             g_ucDllDeletedConnections;
DLL_PACKET_T      g_aDllQueue[DLL_MAX_PACKET_BUFFERS];
uint8             g_ucDllCrtQueueSize;
uint8             g_ucDllAllQueueSize;
// Hash
uint8                     g_ucDllHashLinksSize;
uint8                     g_ucDllHashGraphsSize;
// Properties 
uint8   g_ucDllPriorityThreshold;
uint8   g_ucDllJoinPriority;
uint8   g_ucDllMaxBackoffExponent;
// Active Search / Syncronized Counter 
uint32  g_ulDllRecCntr;
uint32  g_ulDllMaxRecCntr;

//// DLL logical watchdog reset counter 
//#ifndef BACKBONE_SUPPORT 
//uint8   g_ucConsecutiveNoAckCtr;
//#endif

// unnamed struct in order to keep alignment
// Timeouts
struct
{
    uint32  g_ulDllAdvCntr;
    uint32  g_ulDllDiscoveryCntr;
    uint32 g_ulDiscStartRTC;
    uint32  g_ulDllQueueDiscCntr;
    
    unsigned long long  g_ullDllSuspendTimeout;
    unsigned long long  g_ullDllResumeTimeout;
    uint32  g_ulTimeSourceTimer;
    DLL_HASH_LINKS_STRUCT_T   g_aDllHashLinks[DLL_MAX_LINKS]; // keep on this position
//  DLL_HASH_GRAPHS_STRUCT_T  g_aDllHashGraphs[DLL_MAX_GRAPHS]; 
    DLL_HASH_GRAPHS_STRUCT_T  g_aDllHashGraphs[DLL_MAX_CONNECTIONS]; // keep on this position 
};
//
DLL_BUFFER_PARSED_T     g_sDllBufferParsed;
uint8                   g_ucDllAdvertiseTX;
uint8                   g_ucDllDiscoveryTX;

//
union 
{
  uint32 m_ulAlignForced;
  uint8 m_aucNonce[MAX_NONCE_LEN];            
} stAligned; 
//
PRED_SLOT g_stDllPredSlot;
PRED_LNK g_stDllPredLink;

void DLL_Predictive_Schedule_Link(void);
uint16 GetSlotDiff( uint16 p_unLinkSlot, uint16 p_unCrtSfOffset, uint16 p_unNumSfSlots);

//#define DLL_CHARGE_TEST  // -> test de incarcare pt DLL, mai ales pt rebuild-ul de hash links
#ifdef DLL_CHARGE_TEST  

#define FUNCTION1 0  // rebuild paket_queue/superframes
#define FUNCTION2 1  // rebuild neighbors
#define FUNCTION3 2  // rebuild hash links !!!
#define FUNCTION4 3  // rebuild connections
#define FUNCTION5 4  // rebuild hash graphs

#define SIZE_OF_TIMINGS 10

uint32 g_aucTimimgs[SIZE_OF_TIMINGS];

#endif

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_IncrementASN()
{ 
  g_sDllASN.m_ullASN++;
  if(g_sDllASN.m_aucBytes[5])
  { // ASN is a 5-byte variable
    g_sDllASN.m_aucBytes[5] = g_sDllASN.m_aucBytes[6] = g_sDllASN.m_aucBytes[7] = 0;
  }
  
  if(g_ucDllStatus == DLL_JOINED)
  {
    // Check Advertise Timeout
    if( g_ulDllAdvCntr )
    {
      g_ulDllAdvCntr--;
    }
    else
    {
      g_ulDllAdvCntr = g_stProv.ulDllAdvertise; 
      g_ucDllAdvertiseTX = 1; //DLL_MAX_RETRIES;
    }
    
    // Check Discovery Timeout
    if(g_stProv.ulDllDiscovery != 0xFFFFFFFF)
    { // must send Keep-Alive packets on Discovery links
      if ( g_ulDllDiscoveryCntr ) 
      {
        g_ulDllDiscoveryCntr--;
      }
      else
      {
        g_ulDllDiscoveryCntr = MACA_RANDOM % g_stProv.ulDllDiscovery; 
        g_ucDllDiscoveryTX = 1; //DLL_MAX_RETRIES;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_Init(void)
{  // reset the modem inside
  // Status 
  PHY_Disable_Request(PHY_IDLE);
  PUT_DLL_IDLE();    
  g_ucDllXVRStatus = DLL_XVR_IDLE;
  g_ucDllAlarmsCounter = 0;
  // Join info
   g_sDllSharedParams.m_unJoinGraphID = TR_NM_GRAPHID; // NIVIS hardcoded value for TR, Gw, NM
  // RESET ASN & Channel Map
  memset( &g_sDllASN, 0, sizeof(g_sDllASN) );
  g_unDllChannelMap     = 0;
  g_ucDllActiveChannels = 0;
  g_ucDllTotalChannels  = 0x0F;
  // RESET Buffers  
  g_ucDllBufferLen = 0;
  g_ucDllAckBufferLen = 0;
  // RESET Tables
  g_ucDllLkdNeighborsSize = 0; 
  g_ucDllAllNeighborsSize = 0;   
  g_ucDllCrtSuperframesSize = 0;
  g_ucDllAllSuperframesSize = 0;
  g_ucDllDeletedSuperframes = 0;
  g_ucDllCrtLinksSize = 0;
  g_ucDllAllLinksSize = 0;
  g_ucDllDeletedLinks = 0;
  g_ucDllCrtConnectionsSize = 0;
  g_ucDllAllConnectionsSize = 0;  
  g_ucDllDeletedConnections = 0;  
  g_ucDllCrtQueueSize = 0;
  g_ucDllAllQueueSize = 0;
  g_unNickname = 0xFFFF;
  // RESET Hash
  g_ucDllHashLinksSize = 0;
  g_ucDllHashGraphsSize = 0;  
  // RESET Properties
  g_ucDllPriorityThreshold  = 0;
#ifdef BACKBONE_SUPPORT
  g_ucDllJoinPriority       = 4;
#else
  g_ucDllJoinPriority       = 0;
#endif
  g_ucDllMaxBackoffExponent = 4;                                                // default to 4, valid values : [4,5,6,7]
  
  g_stDllPredSlot.m_uc250msSlotNo = 0;
  g_stDllPredLink.m_uc250msComputeTimeSlot = 0xFF;  
  g_stDllPredLink.m_unSlotsUntilWakeUp = 0xFFFF;
  
  g_ucDllDisconnect = DLL_DISCONNECT_INACTIVE;
  g_ullDllSuspendTimeout = 0;
  g_ullDllResumeTimeout = 0;  

  DelayLoop( 1000 ); // dummy wait in order to be sure PHY is idle ?!
  PHY_Reset();
  
  memset(&g_stC769, 0, sizeof(C769_ReadJoinStatus_Resp));
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_Join()
{
  DLL_Init();               // sterge absolut tot ... toate tabelele ... sterge toate linkurile create automat !!!   
  DLL_CheckAndSetChannelMap(0, 0x0F, 0x7FFF);                                                    // la inceput vor fi utilizate toate canalele permise pentru cautare, canalul 15 always reseted !
  g_ulChannelSearchTime = DLL_CHANNEL_SEARCH_TIME;                        
          
  g_ulDllMaxRecCntr      = g_stProv.ulActiveSearchShedTime;
  g_ulDllAdvCntr         = 0;  
  g_ulDllDiscoveryCntr   = 0;
  
  g_ulDllQueueDiscCntr = 0;
  
  PUT_DLL_ACTIVE_SEARCH();     
  DLL_TIMER_START_FROM_ZERO();  
  UART1_String("DSactS");                                                       // DLL Status: active search
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_Timeslot_Start(void)
{  
  if ( g_ucDllXVRStatus == DLL_XVR_RX_ACK )                                     // nu am primit ACK desi ar fi trebuit 
  { 
//#ifndef BACKBONE_SUPPORT     
//    if ( g_sDllSlot.m_pQueue )
//    {
//        g_ucConsecutiveNoAckCtr++;
//    }
//
//    if( g_ucConsecutiveNoAckCtr >= MAX_CONSECUTIVE_NO_ACKS )
//    {
//      PrepareForReset( RREASON_NO_ACK );
//    }
//#endif    
    
    if ( g_sDllSlot.m_pNeighbor )                                               // necessary precaution
    {                
      // 1. Missed Ack for this neighbor
      g_sDllSlot.m_pNeighbor->m_unPacketsMissedACK++;
      
      // 2. BACKOFF mecanism
      if ( g_sDllSlot.m_pLink )
      {
        if ( ! g_sDllSlot.m_pLink->m_bShared )
        { // not collision but channel degradation => reset backoffs :
          g_sDllSlot.m_pNeighbor->m_bBOExp = 0;
          g_sDllSlot.m_pNeighbor->m_ucBOCntr = 0;
        }
        else if(g_sDllSlot.m_pNeighbor->m_unShortAddr == g_sDllSlot.m_pLink->m_unNeighborShortAddr)
        { // collision : start back off mechanism
          if (g_sDllSlot.m_pNeighbor->m_bBOExp < g_ucDllMaxBackoffExponent )
          {
            g_sDllSlot.m_pNeighbor->m_bBOExp++;
          }
          uint8 ucMask = (0x01 << (g_sDllSlot.m_pNeighbor->m_bBOExp)) - 1;
          g_sDllSlot.m_pNeighbor->m_ucBOCntr = MACA_RANDOM & ucMask;
        }
      }
      // ... BACKOFF
      
    }
  }
    
  PHY_Disable_Request(PHY_IDLE);
  g_ucDllXVRStatus = DLL_XVR_IDLE;

  if ( g_sDllFlags.m_ucDisconnect )
  { // a Disconnect packet was received in the previous slot, now that neighbor and its links/connections will be deleted before other actions upon slot beginning
    g_sDllFlags.m_ucDisconnect = 0;
    for ( DLL_NEIGHBOR_T * pNeighbor = g_aDllNeighbors; pNeighbor < g_aDllNeighbors + g_ucDllAllNeighborsSize ; pNeighbor++ )
    {
      if ( pNeighbor->m_bDisconnected ) 
      {
        // sterg toate packetele din coada pentru el ... ar trebui sa notific si NL de aceasta treaba
        for ( DLL_PACKET_T * pQueue = g_aDllQueue; pQueue < g_aDllQueue + g_ucDllAllQueueSize ; pQueue++  )
        {
          if ( 
              ( IntFromLE( pQueue->m_aDestAddr ) == pNeighbor->m_unShortAddr ) &&
              ( pQueue->m_ePacketDestType != DLL_DEST_TYPE_GRAPH ) &&
              ( pQueue->m_ePacketDestType != DLL_DEST_TYPE_SUPERFRAME ) &&
              ( pQueue->m_stAddrSpec.m_bDestLongAddr == 0 )  
             )
          { // packet for that linked or non-linked neighbor, on short dest. address (see DLL_TX_Request() )
            pQueue->m_bMarkDeleted = 1; 
            COMMIT_MODIFY_DLL_QUEUE();
          }
        }
        
        if(pNeighbor->m_bLinked || (pNeighbor < (g_aDllNeighbors + g_ucDllLkdNeighborsSize)))
        { // delete all links with the disconnecting neighbor
          for ( DLL_LINK_T * pLink = g_aDllLinks; pLink < g_aDllLinks + g_ucDllAllLinksSize ; pLink++  )
          {
            if ( pLink->m_unNeighborShortAddr == pNeighbor->m_unShortAddr )
            {
              pLink->m_bMarkDeleted = 1;            
            }
          }
          // delete all connections with the disconnecting neighbor
          for ( DLL_CONNECTION_T * pConn = g_aDllConnections; pConn < g_aDllConnections + g_ucDllAllConnectionsSize ; pConn++  )
          {
            if ( pConn->m_unNeighborShortAddr == pNeighbor->m_unShortAddr )
            {
              pConn->m_bMarkDeleted = 1;            
            }
          }
        }        
        //
        pNeighbor->m_bLinked = 0;
        pNeighbor->m_bMarkDeleted = 1;
        COMMIT_MODIFY_DLL_STRUCTS();
      }
    }  
  }
  
  if(DLL_DISCONNECT_RESET == g_ucDllDisconnect)
  {
    if(RREASON_NONE == g_ucResetReason)
    { // can only happen for C960/C972
      if(g_ullDllResumeTimeout && 
         g_ullDllSuspendTimeout) //check suspend activated and disconnect mechanism ready
      {
        if(g_sDllASN.m_ullASN >= g_ullDllSuspendTimeout)
        {
          if(g_sDllASN.m_ullASN >= g_ullDllResumeTimeout) 
          { 
            //PrepareForReset(RREASON_CMD972);
            g_ucResetReason = RREASON_CMD972;
          }
        }
      }
      else 
      {
        //PrepareForReset( RREASON_CMD960 ); 
        g_ucResetReason = RREASON_CMD960;
      }
    }
    return;        
  }
    
//  
  
  DLL_Configure();

  g_stDllPredLink.m_uc250msComputeTimeSlot = g_stDllPredSlot.m_uc250msSlotNo;
    
  if (g_stDllPredLink.m_unSlotsUntilWakeUp)
  {
    g_stDllPredLink.m_unSlotsUntilWakeUp--;
  }       
    
  if( g_stDllPredLink.m_unSlotsUntilWakeUp > 1 )
  {
      PHY_Disable_Request(PHY_IDLE);
      g_ucDllXVRStatus = DLL_XVR_IDLE;
 
      return; // keep modem on sleep mode
  }

  DLL_Schedule_Link();  
  if ( g_sDllSlot.m_pLink ) 
  { // have an active link        
    uint16 unDelay;
    
    g_sDllSlot.m_ucChannel = DLL_GetChannel((( g_sDllASN.m_ullASN + g_sDllSlot.m_pLink->m_ucChannelOffset ) % g_ucDllActiveChannels));    
    g_stDllPredLink.m_unSlotsUntilWakeUp = 0;
    
//    DEBUG2_ON(); DEBUG2_OFF();          
    if ( g_sDllSlot.m_ucPacketType == DLL_INVALID_PACKET_TYPE )                 // RX Link              
    { // do Rx ( first or second )

      g_ucDllXVRStatus = DLL_XVR_RX;
      unDelay = (DLL_TS_RX_OFFSET - DLL_TO_PHY_RX) - (DLL_TIMER_CNTR / 6);        // us
      DEBUG2_ON(); DEBUG2_OFF();          
      if((int16)unDelay < 0)
      {        
        g_stProv.unDllRxTimeouts++;        
        unDelay = 0;
      }   
#ifdef RX_TEST
      PHY_RX_Request( g_sDllSlot.m_ucChannel, 0, RX_ABORT_TIMEOUT );
#else
      PHY_RX_Request( g_sDllSlot.m_ucChannel, unDelay, RX_ABORT_TIMEOUT );
#endif      
    }
    else if( g_ucDllBufferLen )                 // TX Link with something to transmit
    { // do Tx, Tx Shared 
      if ( ! g_sDllSharedParams.m_ucUseCCA )
      { // ... Tx without CCA
        unDelay = (DLL_TS_TX_OFFSET - DLL_TO_PHY_TX);
      }
      else
      { // ... Tx with CCA
        unDelay = (DLL_TS_CCA_OFFSET - DLL_TO_PHY_TX);
      }
      unDelay -= DLL_TIMER_CNTR / 6; // subtract no of us from beginning of slot
      DEBUG2_ON(); DEBUG2_OFF();

      // transmit the packet only when the required timing can be respected, to assure an accurate clock correction for the peer device
      if((int16)unDelay >= (WARM_UP_DELAY*4)) // modem's warm-up time in us
      {
        PHY_TX_Request(g_sDllSlot.m_ucChannel, unDelay, g_sDllSharedParams.m_ucUseCCA, g_aucDllBuffer, g_ucDllBufferLen);
        g_ucDllXVRStatus = DLL_XVR_TX;
        if ( g_sDllSlot.m_pQueue )
        { // was TX Data packet, not DLL generated packet      
          g_sDllSlot.m_pQueue->m_bNoRetries--;
        }
        if ( g_sDllSlot.m_pNeighbor )
        {                
          g_sDllSlot.m_pNeighbor->m_unPacketsTransmitted++;
        }
      }
      else
      {
        g_stProv.unDllTxTimeouts++;
        if ( g_sDllSlot.m_pQueue && (g_sDllSlot.m_pQueue->m_ucTxTimeouts < 0xFF))
        { // was TX Data packet, not DLL generated packet      
          g_sDllSlot.m_pQueue->m_ucTxTimeouts++;
        }
        DEBUG4_ON(); DEBUG4_OFF();  // two pulses on TXdebug line if was too late for start transmission 
        DEBUG4_ON(); DEBUG4_OFF();
      }
    }         
  }
  else
  { // g_sDllSlot.m_pLink == NULL 
    DLL_Predictive_Schedule_Link();
  }
  DLL_CheckOtherTimeouts();
}
 
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_Configure(void)
{  
 
#ifdef DLL_DO_HASH_LINKS  
  // WORK AROUND: Always Rebuild Hash for Links !
  DLL_RebuildHashLinks(); 
#endif  
  
  
#ifdef DLL_CHARGE_TEST  
  uint32 aucLocalTimings[SIZE_OF_TIMINGS];
  
  aucLocalTimings[FUNCTION1] = RTC_COUNT;
#endif  
  
  
  if ( g_sDllFlags.m_ucQueue )
  {
    DLL_PACKET_T * pDllQueue = g_aDllQueue;    
    // Delete marked Packets
    while ( pDllQueue < ( g_aDllQueue + g_ucDllAllQueueSize ) )
    {
      if ( pDllQueue->m_bMarkDeleted )
      {
        g_ucDllAllQueueSize--;
        memmove( pDllQueue, pDllQueue + 1, (uint8*)(g_aDllQueue+g_ucDllAllQueueSize) - (uint8*)pDllQueue );
      }
      else
      {
        pDllQueue++;
      }
    }    
    // Add Packets
    g_ucDllCrtQueueSize = g_ucDllAllQueueSize;
    // OK Done
    g_sDllFlags.m_ucQueue = 0;
  }
  
  if ( g_sDllFlags.m_ucStructs )
  {           
    DLL_SUPERFRAME_T * pSf;
    DLL_LINK_T * pLink;    
    
    g_stDllPredLink.m_unSlotsUntilWakeUp = 0;
    
    // Delete SF
    pSf = g_aDllSuperframes;
    while ( pSf < ( g_aDllSuperframes + g_ucDllAllSuperframesSize ) )
    {
      if ( pSf->m_bMarkDeleted )
      { 
        g_ucDllAllSuperframesSize--;    
        memmove( pSf, pSf + 1, (uint8*)(g_aDllSuperframes+g_ucDllAllSuperframesSize) - (uint8*)pSf );
      }
      else
      {
        pSf++;
      }
    }            
    // Add SF
    g_ucDllCrtSuperframesSize = g_ucDllAllSuperframesSize;     
    // Modify SF
    pSf = g_aDllSuperframes;    
    while ( pSf < ( g_aDllSuperframes + g_ucDllCrtSuperframesSize ) )
    {
      if ( pSf->m_bSetDisabled )
      { // Set SF Disabled
        pSf->m_eActive = DLL_SF_DISABLED;
        pSf->m_bSetDisabled = 0;
      }
      if ( pSf->m_bModifySlots )                          // nu am voie sa fie setat decat daca SF este disabled sau set disabled
      { // Modify Slots
        pSf->m_unNumSlots = pSf->m_unNewSlots;
        pSf->m_bModifySlots = 0;       
      } 
      if ( pSf->m_bSetEnabled )
      { // Set SF Enabled   
        pSf->m_eActive = DLL_SF_ENABLED;
        pSf->m_bSetEnabled = 0;
        pSf->m_unCrtOffset = g_sDllASN.m_ullASN % pSf->m_unNumSlots;
      }
      pSf++;
    }        
    // Delete Links
    pLink = g_aDllLinks;
    while ( pLink < ( g_aDllLinks + g_ucDllAllLinksSize ) )
    {
      if ( pLink->m_bMarkDeleted )
      { 
        g_ucDllAllLinksSize--;    
        memmove( pLink, pLink + 1, (uint8*)(g_aDllLinks+g_ucDllAllLinksSize) - (uint8*)pLink );
      }
      else
      {
        pLink++;
      }
    }                  
    // Add Links
    g_ucDllCrtLinksSize = g_ucDllAllLinksSize;

    // change addr of the self-created Join links into 0xFFFF
    if( g_sDllFlags.m_ucStructs == 3 )
    {
      pLink = g_aDllLinks;
      while ( pLink < ( g_aDllLinks + g_ucDllAllLinksSize ) )
      {
        if ( (pLink->m_eLinkType == LinkType_Join) )
          pLink->m_unNeighborShortAddr = 0xFFFF;
        pLink++;
      }
    }
    
#ifdef DLL_CHARGE_TEST   
    
    aucLocalTimings[FUNCTION1] = RTC_COUNT - aucLocalTimings[FUNCTION1];
    
    if( aucLocalTimings[FUNCTION1] > g_aucTimimgs[FUNCTION1] )
      g_aucTimimgs[FUNCTION1] = aucLocalTimings[FUNCTION1];
    
    
    
    
    // Rebuild Neighbors
    aucLocalTimings[FUNCTION2] = RTC_COUNT;
    
    DLL_RebuildNeighbors(); 
    
    aucLocalTimings[FUNCTION2] = RTC_COUNT - aucLocalTimings[FUNCTION2];
    
    if( aucLocalTimings[FUNCTION2] > g_aucTimimgs[FUNCTION2] )
      g_aucTimimgs[FUNCTION2] = aucLocalTimings[FUNCTION2];
    
    
    
    
    // Rebuild Hash for Links !
    aucLocalTimings[FUNCTION3] = RTC_COUNT;
    DLL_RebuildHashLinks(); 
    
    aucLocalTimings[FUNCTION3] = RTC_COUNT - aucLocalTimings[FUNCTION3];
    
    if( aucLocalTimings[FUNCTION3] > g_aucTimimgs[FUNCTION3] )
      g_aucTimimgs[FUNCTION3] = aucLocalTimings[FUNCTION3];
    
    
    
    
    // Rebuild Connections for Hash Graph
    aucLocalTimings[FUNCTION4] = RTC_COUNT;
    
    DLL_RebuildConnections(); 

    aucLocalTimings[FUNCTION4] = RTC_COUNT - aucLocalTimings[FUNCTION4];
    
    if( aucLocalTimings[FUNCTION4] > g_aucTimimgs[FUNCTION4] )
      g_aucTimimgs[FUNCTION4] = aucLocalTimings[FUNCTION4];
        
    
    
    
    // Rebuild Hash for GRAPHS !
    aucLocalTimings[FUNCTION5] = RTC_COUNT;
    
    DLL_RebuildHashGraphs();
    
    aucLocalTimings[FUNCTION5] = RTC_COUNT - aucLocalTimings[FUNCTION5];
    
    if( aucLocalTimings[FUNCTION5] > g_aucTimimgs[FUNCTION5] )
      g_aucTimimgs[FUNCTION5] = aucLocalTimings[FUNCTION5];
    
#else
    
    // Rebuild Neighbors
    DLL_RebuildNeighbors();    
    // Rebuild Hash for Links !
    DLL_RebuildHashLinks();    
    // Rebuild Connections for Hash Graph
    DLL_RebuildConnections();     
    // Rebuild Hash for GRAPHS !
    DLL_RebuildHashGraphs();
    //
    
#endif
    
    g_sDllFlags.m_ucStructs = 0;
  }
}

void DLL_HoldThisLink( DLL_LINK_T * p_pLink, DLL_NEIGHBOR_T * p_pNeighbor)  //, uint8 p_ucPacketType )
{
  g_sDllSlot.m_pLink        = p_pLink;
  g_sDllSlot.m_pNeighbor    = p_pNeighbor;
//  g_sDllSlot.m_ucPacketType = p_ucPacketType;
//  g_sDllSlot.m_ucChannel    = ( g_sDllASN.m_ullASN + p_pLink->m_ucChannelOffset ) % g_ucDllActiveChannels;
//  g_sDllSlot.m_ucChannel    = DLL_GetChannel(g_sDllSlot.m_ucChannel);                               
  g_sDllSlot.m_ucChannel    = DLL_GetChannel((( g_sDllASN.m_ullASN + p_pLink->m_ucChannelOffset ) % g_ucDllActiveChannels));                               
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_Schedule_Link(void)
{       
DLL_HASH_LINKS_STRUCT_T   * pHashLink = g_aDllHashLinks;
DLL_HASH_LINKS_STRUCT_T   * pEndHash = g_aDllHashLinks+g_ucDllHashLinksSize;
DLL_LINK_T                * pLink;
DLL_SUPERFRAME_T          * pSf;
DLL_NEIGHBOR_T            * pNeighbor;

  memset( &g_sDllSlot, 0, sizeof(g_sDllSlot) );
  g_sDllSlot.m_ucPacketType  = DLL_INVALID_PACKET_TYPE;
  
  g_ucDllBufferLen = 0;
  
  for( ; pHashLink < pEndHash; pHashLink++ )
  {  
    pLink     = &g_aDllLinks[pHashLink->m_ucLinkIdx];
    pSf       = &g_aDllSuperframes[pHashLink->m_ucSuperframeIdx];
      
    if ( pLink->m_unSlot == pSf->m_unCrtOffset )                                // found an active link ... to be served
    {                        
      if ( pLink->m_bRx )
      { // Only Rx
        if ( ! g_sDllSlot.m_pLink )
        { // retin doar primul link de RX ce trebuie servit ... dar ma uit in continuare daca exista Tx cu packet pending !!!
//          DLL_HoldThisLink( pLink, NULL, DLL_INVALID_PACKET_TYPE );

            g_sDllSlot.m_pLink = pLink;
            g_sDllSlot.m_pNeighbor = NULL;
        }
      }
      
      if ( pLink->m_bTx )
      { // Tx or TxShared            
        pNeighbor = NULL;
        
        // Keep Neighbor
        if ( pHashLink->m_ucNeighborIdx != DLL_INVALID_NEIGH_IDX )
        {
          pNeighbor = &g_aDllNeighbors[pHashLink->m_ucNeighborIdx]; 
          
          // BACKOFF mecanism 
          if ( pLink->m_bShared && pNeighbor->m_ucBOCntr && (DLL_DISCONNECT_INACTIVE == g_ucDllDisconnect) )
          {
            pNeighbor->m_ucBOCntr--;
            continue;
          }
          // ... BACKOFF
        }            
        
        g_sDllSlot.m_pQueue = DLL_ReqCrtPktFromQueue(pHashLink);
        if ( g_sDllSlot.m_pQueue )
        { // found packet for this Tx or Tx Shared link ...   
//          DLL_HoldThisLink( pLink, pNeighbor, DLL_PACKET_TYPE_DATA );          
//          return;
          g_sDllSlot.m_ucPacketType = g_sDllSlot.m_pQueue->m_stDLDPUSpec.m_bPacketType;
          break;
        }
        

        
        if(DLL_DISCONNECT_WAIT_FOR_DIS_PACKETS == g_ucDllDisconnect)
        {
          g_sDllSlot.m_ucPacketType = DLL_PACKET_TYPE_DISCONNECT;
          pNeighbor = NULL;
            //
          break; //do not generate discovery, keep alive and adv packets while disconnecting!
        }
        
        if(DLL_DISCONNECT_INACTIVE != g_ucDllDisconnect)
          break; //do not generate discovery, keep alive and adv packets while disconnecting! nor disconnect packet while waiting for queue or resetting       
        
        // Check Discovery Timeout
        if(g_ucDllDiscoveryTX && (pLink->m_eLinkType == DLL_LINK_TYPE_DISCOVERY) && 
           ( g_ucNetState == NET_STATE_OPERATIONAL || g_ucNetState == NET_STATE_QUARANTINED) )
        {
//            g_ucDllDiscoveryTX--;
          g_ucDllDiscoveryTX = 0;   // no retry on DLL-generated packets
          
#ifdef SEND_DISCOVERY_TO_OLDEST
          // search for the linked neighbor with the oldest m_tmLastComm attribute value
          pNeighbor = g_aDllNeighbors;
          for(DLL_NEIGHBOR_T * p = g_aDllNeighbors + 1; p < ( g_aDllNeighbors + g_ucDllLkdNeighborsSize); p++ ) 
          { 
            if ( (int32)(p->m_tmLastComm - pNeighbor->m_tmLastComm) < 0 )
            { // p is older
              pNeighbor = p;
            }       
          }
#else
          pNeighbor = NULL; // send packet to FFFF
#endif
          g_sDllSlot.m_ucPacketType = DLL_PACKET_TYPE_KEEPALIVE;
          break;
        }
          
        // Check KeepAlive Timeout
        if ( pNeighbor && //pNeighbor->m_ucKeepAliveTX &&
             ( pLink->m_eLinkType == DLL_LINK_TYPE_NORMAL || 
               ( g_ucNetState == NET_STATE_JOINING && pLink->m_eLinkType == DLL_LINK_TYPE_JOIN ) ) )
        {
          //uint32 ulDiff =  g_sDllASN.m_ulLsb - g_stProv.ulDllKeepAlive;   
          //if ( (int32)(pNeighbor->m_tmLastComm - ulDiff) <= 0 )
          if ( (int32)(pNeighbor->m_tmLastComm - g_sDllASN.m_ulLsb + g_stProv.ulDllKeepAlive) <= 0 )
          {
            g_sDllSlot.m_ucPacketType = DLL_PACKET_TYPE_KEEPALIVE;
            // do not break here; check if an AdvertiseTx needed, too, and if this link is eligible
          }       
        }
          
        // Check Advertise Timeout       
        if ( g_ucDllAdvertiseTX && ( ! pLink->m_bShared ) && ( g_ucNetState == NET_STATE_OPERATIONAL ) )
        { // send ADV packets only in NET_STATE_OPERATIONAL
//            g_ucDllAdvertiseTX--;
          g_ucDllAdvertiseTX = 0;   // no retry on DLL-generated packets
          
          // if KA (clock sync) not needed, send the advertise to 0xFFFF in order to save the energy consumed with the ACK
          if(g_sDllSlot.m_ucPacketType != DLL_PACKET_TYPE_KEEPALIVE)
            pNeighbor = NULL;
          // even if KA needed, I will send an Advertise packet instead
          g_sDllSlot.m_ucPacketType = DLL_PACKET_TYPE_ADVERTISE;
          break;
        }
        if(g_sDllSlot.m_ucPacketType == DLL_PACKET_TYPE_KEEPALIVE)
          break;
      }          
    }
  }
  if (g_sDllSlot.m_ucPacketType != DLL_INVALID_PACKET_TYPE)
  {
    g_sDllSlot.m_pLink        = pLink;
    g_sDllSlot.m_pNeighbor    = pNeighbor;
    
    if( g_sDllSlot.m_pQueue )
    {
        DEBUG4_ON(); DEBUG4_OFF();  // one pulse on TXdebug line for non-DLL generated packets 
        DLL_PrepareOutgoingPkt();
    }
    else if(g_sDllSlot.m_ucPacketType == DLL_PACKET_TYPE_ADVERTISE)
    {
        DLL_ComputeAdvertisePkt( pNeighbor );
    }
    else if(g_sDllSlot.m_ucPacketType == DLL_PACKET_TYPE_KEEPALIVE)
    {
        DLL_ComputeKeepAlivePkt( pNeighbor );
    }
    else if(g_sDllSlot.m_ucPacketType == DLL_PACKET_TYPE_DISCONNECT)
    {
        DLL_ComputeDisconnectPkt( );
    }
  }  
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_CheckOtherTimeouts(void)
{
  if(DLL_DISCONNECT_INACTIVE == g_ucDllDisconnect)
  {
    // Check for Maintaining Neighbors
    for(DLL_NEIGHBOR_T * pNeighbor = g_aDllNeighbors; pNeighbor < ( g_aDllNeighbors + g_ucDllLkdNeighborsSize); pNeighbor++ ) 
    {         
      if (  ( pNeighbor->m_tmPathFailCounter-- ) == 0 ) 
      { 
        //      DLL_PathFailIndicate();
        // generates a C788_AlarmPathDown packet in the aggregated alarm buffer
        APP_AddToAlarmBuffer(APP_ALARM_C788, pNeighbor->m_unShortAddr, NULL); 
        UART1_String("DNFi"); // DLL Network Event: PathFail indicate  
        pNeighbor->m_tmPathFailCounter = g_stProv.ulDllPathFail;
      } 
      //#ifndef BACKBONE_SUPPORT    
      //    if((pNeighbor->m_bTimeSource & NeighbourFlagsMask_TimeSource) &&
      //       //((int32)(pNeighbor->m_tmLastComm - g_sDllASN.m_ulLsb + MAX_PATH_FAIL_ALARMS_NO * g_stProv.ulDllPathFail) >= 0))
      //       ((int32)(pNeighbor->m_tmLastComm - g_sDllASN.m_ulLsb + ((TIME_SOURCE_TIMEOUT/32768)*100)) >= 0))
      //    { // at least one clock source has not exceeded path fail max number of alarms
      //      g_ulTimeSourceTimer = RTC_COUNT + TIME_SOURCE_TIMEOUT; // this is bad, it will double the delay (TIME_SOURCE_TIMEOUT) before de-registering
      //    }
      //#endif //#ifndef BACKBONE_SUPPORT   
      
    } 
    
#ifndef BACKBONE_SUPPORT  
    if( (int32)(RTC_COUNT - g_ulTimeSourceTimer) >= 0 )
    { //abandon hope all ye who have no clock source!
      
      PrepareForReset( RREASON_TOUT_TIMESRC ); 
    }  
#endif //#ifndef BACKBONE_SUPPORT
  } // if(!g_ucDllDisconnect)
  
  // Check for Deleting Packets Queue
  uint16 unDiff =  *(uint16*)&g_sDllASN.m_ulLsb - (uint16)g_stProv.ulMaxPacketAge;   
  for( DLL_PACKET_T * pQueue = g_aDllQueue; pQueue < ( g_aDllQueue + g_ucDllCrtQueueSize ); pQueue++ ) 
  { 
    if( ( ! pQueue->m_bNoRetries ) || ( (int16)(pQueue->m_unPacketBirthASN - unDiff) <= 0) ) 
    { 
      pQueue->m_bMarkDeleted = 1;       
      if ( g_ucDllAlarmsCounter && ( pQueue->m_stDLDPUSpec.m_bPriority == DLL_PACKET_PRIORITY_ALARM ) )
      {
        g_ucDllAlarmsCounter--;
      }
      
      // Transmit error indicate for NL
//      DLL_TX_Confirm( pQueue->m_unPacketID, DLL_ERROR );      
      COMMIT_MODIFY_DLL_QUEUE();
    }
  }

}


////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_RebuildNeighbors()
{
  g_ucDllLkdNeighborsSize = 0;  
  for (DLL_NEIGHBOR_T * pNeighbor = g_aDllNeighbors; pNeighbor < ( g_aDllNeighbors + g_ucDllAllNeighborsSize ); pNeighbor++)
  { // not optimized (as execution speed) for a neighbor that isn't linked anymore! 
    if ( pNeighbor->m_bLinked )    
    {
      if ( pNeighbor > ( g_aDllNeighbors + g_ucDllLkdNeighborsSize ) )
      {
        DLL_NEIGHBOR_T sTemp = g_aDllNeighbors[g_ucDllLkdNeighborsSize];
        g_aDllNeighbors[g_ucDllLkdNeighborsSize] = *pNeighbor;
        *pNeighbor = sTemp;
      }
      g_ucDllLkdNeighborsSize++;
    }
    else
    {
      if ( pNeighbor->m_bMarkDeleted )    
      {
        g_ucDllAllNeighborsSize--;
        if ( pNeighbor < ( g_aDllNeighbors + g_ucDllLkdNeighborsSize ) )
        {
          g_ucDllLkdNeighborsSize--; // when a Disconnect packet was received from that neighbor 
        }
        memmove( pNeighbor, pNeighbor + 1, (uint8*)(g_aDllNeighbors+g_ucDllAllNeighborsSize) - (uint8*)pNeighbor );   
      }
    }    
  }
}

////////////////////////////////////////////////////////////////////////////////
// keep sorted by Graph, Neighbor ... check outside before add if conn ID already exists !!!
////////////////////////////////////////////////////////////////////////////////
void DLL_RebuildConnections()
{  
DLL_CONNECTION_T  * pConn;  
DLL_CONNECTION_T    sConnToAdd;  

  // first of all ... deleting all marked connections
  pConn = g_aDllConnections;
  while ( pConn < ( g_aDllConnections + g_ucDllAllConnectionsSize ) )
  {
    if ( pConn->m_bMarkDeleted )
    { // Delete connection         
      g_ucDllAllConnectionsSize--;
      if ( pConn < ( g_aDllConnections + g_ucDllCrtConnectionsSize ) )
      {
        g_ucDllCrtConnectionsSize--;  
      }                
      memmove( pConn, pConn + 1, (uint8*)(g_aDllConnections+g_ucDllAllConnectionsSize) - (uint8*)pConn );
    }
    else
    {
      pConn++;
    }
  }
  
  // adding
  while ( g_ucDllCrtConnectionsSize < g_ucDllAllConnectionsSize )
  {
    sConnToAdd = g_aDllConnections[g_ucDllCrtConnectionsSize];            
    
    pConn = g_aDllConnections;
    while ( pConn < ( g_aDllConnections + g_ucDllCrtConnectionsSize ) )
    {
      if ( sConnToAdd.m_unGraphID < pConn->m_unGraphID)
      {
        memmove( pConn + 1, pConn, (uint8*)(g_aDllConnections+g_ucDllCrtConnectionsSize) - (uint8*)pConn );
        break; // exit for
      }
      else if ( sConnToAdd.m_unGraphID == pConn->m_unGraphID) 
      {
        if ( sConnToAdd.m_unNeighborShortAddr < pConn->m_unNeighborShortAddr )
        {
          memmove( pConn + 1, pConn, (uint8*)(g_aDllConnections+g_ucDllCrtConnectionsSize) - (uint8*)pConn );
          break; // exit for
        }
      }
      pConn++;
    }      
    *pConn = sConnToAdd;
    g_ucDllCrtConnectionsSize++;
  }  
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_RebuildHashGraphs()
{
  g_ucDllHashGraphsSize = 0;  
  if ( g_ucDllCrtConnectionsSize )
  {
    DLL_HASH_GRAPHS_STRUCT_T * pHashGraph = g_aDllHashGraphs;
    pHashGraph->m_unGraphID =  g_aDllConnections[0].m_unGraphID;
    pHashGraph->m_ucIdx = 0;
    pHashGraph->m_ucLen = 1;   
    pHashGraph++;
    g_ucDllHashGraphsSize++;
    for( uint8 ucIndex = 1; ucIndex < g_ucDllCrtConnectionsSize; ucIndex++ )
    {
      if ( (pHashGraph-1)->m_unGraphID == g_aDllConnections[ucIndex].m_unGraphID )
      {
        (pHashGraph-1)->m_ucLen++; 
      }
      else
      {
        pHashGraph->m_unGraphID =  g_aDllConnections[ucIndex].m_unGraphID;
        pHashGraph->m_ucIdx = ucIndex;
        pHashGraph->m_ucLen = 1;
        pHashGraph++;
        g_ucDllHashGraphsSize++;        
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
// verificare inaintea apelarii 
////////////////////////////////////////////////////////////////////////////////
void DLL_RebuildHashLinks(void)
{   
  unsigned int unCrtSuperframeId = 0xFFFFFFFF;
  unsigned int unSuperframeIdx;
  
  g_ucDllHashLinksSize = 0;
  DLL_HASH_LINKS_STRUCT_T * pEndHash = g_aDllHashLinks;  
  DLL_LINK_T * pLink = g_aDllLinks;

  for( uint8 ucIndex = 0; ucIndex < g_ucDllCrtLinksSize; ucIndex++, pLink++ )
  {
    if( unCrtSuperframeId != pLink->m_ucSuperframeID )
    {
        unCrtSuperframeId = pLink->m_ucSuperframeID;
        unSuperframeIdx = DLL_FindSuperframe( unCrtSuperframeId );
    }
    
    if ( 
        ( unSuperframeIdx == DLL_INVALID_SF_IDX ) ||
        ( g_aDllSuperframes[unSuperframeIdx].m_eActive == DLL_SF_DISABLED )
       )
    {
      continue; // go to evaluation of next link 
    }  
        
    DLL_HASH_LINKS_STRUCT_T* pCrtHash = pEndHash;
    while( pCrtHash > g_aDllHashLinks )
    {
        pCrtHash --;
        if ( unCrtSuperframeId >= pCrtHash->m_ucSuperframeID )
        {
            pCrtHash ++;
            break; // exit while
        }
                
        if( sizeof(*pCrtHash) == 4 )
        {
          *(uint32*)(pCrtHash+1) = *(uint32*)(pCrtHash);
        }
        else
        {
          memcpy( pCrtHash+1, pCrtHash, sizeof(*pCrtHash) );
        }
    }
    pCrtHash->m_ucLinkIdx = ucIndex;
    pCrtHash->m_ucSuperframeIdx = unSuperframeIdx;
    pCrtHash->m_ucSuperframeID = unCrtSuperframeId;
    
    if ( pLink->m_eLinkType == DLL_LINK_TYPE_NORMAL || pLink->m_eLinkType == DLL_LINK_TYPE_JOIN )
    {    
        pCrtHash->m_ucNeighborIdx  = DLL_FindShortNeighbor( pLink->m_unNeighborShortAddr );
    }
    else
    {
        pCrtHash->m_ucNeighborIdx  = DLL_INVALID_NEIGH_IDX;
    }    
    
    pEndHash ++;    
    g_ucDllHashLinksSize++;
    
    //pLink++;
  }
}












////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_AddPktToQueue(const DLL_PACKET_T * p_pMsg)
{
uint8 ucReturnValue = DLL_OUT_OF_MEMORY;

  g_stDllPredLink.m_unSlotsUntilWakeUp = 0;
    
//[doinel.alban]called only from uninterruptable sequences!!!  MONITOR_ENTER(); // exclusive access to Packet Queue
  if( g_ucDllAllQueueSize < DLL_MAX_PACKET_BUFFERS )
  {
    memcpy( &g_aDllQueue[g_ucDllAllQueueSize++], p_pMsg, sizeof(DLL_PACKET_T));
    if ( p_pMsg->m_stDLDPUSpec.m_bPriority == DLL_PACKET_PRIORITY_ALARM )
    {
      g_ucDllAlarmsCounter++;
    }
    COMMIT_MODIFY_DLL_QUEUE();    
    ucReturnValue = SUCCESS;
  }
//[doinel.alban]called only from uninterruptable sequences!!!    MONITOR_EXIT();  
  return ucReturnValue;
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_RemCrtPktFromQueue(void)
{
//  if ( g_sDllSlot.m_pQueue )
  {
    g_sDllSlot.m_pQueue->m_bMarkDeleted = 1;
    if ( ( g_sDllSlot.m_pQueue->m_stDLDPUSpec.m_bPriority == DLL_PACKET_PRIORITY_ALARM ) && ( g_ucDllAlarmsCounter ) )
    {
      g_ucDllAlarmsCounter--;
    }
    COMMIT_MODIFY_DLL_QUEUE();
    g_sDllSlot.m_pQueue = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
DLL_PACKET_T * DLL_HoldThisPacket( DLL_PACKET_T * p_pDllQueue, DLL_PACKET_T * p_pQPkt, uint8 * p_pucPktPry, uint16 * p_punPktASN)
{    
  if ( p_pDllQueue->m_bNoRetries ) 
  { 
    if ( ! p_pQPkt )
    { 
      p_pQPkt = p_pDllQueue;
      *p_pucPktPry = p_pDllQueue->m_stDLDPUSpec.m_bPriority;
      *p_punPktASN = p_pDllQueue->m_unPacketBirthASN;
    } 
    else
    { 
      if ( p_pDllQueue->m_stDLDPUSpec.m_bPriority > (*p_pucPktPry) )
      { 
        p_pQPkt = p_pDllQueue; 
        *p_pucPktPry = p_pDllQueue->m_stDLDPUSpec.m_bPriority;
        *p_punPktASN = p_pDllQueue->m_unPacketBirthASN; 
      } 
      else if ( p_pDllQueue->m_stDLDPUSpec.m_bPriority == (*p_pucPktPry) )
      { 
//        if(((int16)p_pDllQueue->m_unPacketBirthASN - (int16)(*p_punPktASN)) < 0)    // assuming that maxPacketAge is lower than 327.68s (default 300s, --> 30000)  
        if((p_pDllQueue->m_unPacketBirthASN - (*p_punPktASN)) >= 0x8000)    // assuming that maxPacketAge is lower than 327.68s (default 300s, --> 30000)  
        { 
          p_pQPkt = p_pDllQueue;
          *p_punPktASN = p_pDllQueue->m_unPacketBirthASN; 
        }
      }
    }
  }
  return p_pQPkt;
}

////////////////////////////////////////////////////////////////////////////////
// Search in entire queue for a packet to be sent ... if exists ... request packet from queue
////////////////////////////////////////////////////////////////////////////////
DLL_PACKET_T * DLL_ReqCrtPktFromQueue(DLL_HASH_LINKS_STRUCT_T * p_pHashLink)
{
DLL_PACKET_T * pDllQueue = g_aDllQueue;  
DLL_PACKET_T * pQPkt     = NULL;  
uint8          ucQPktPry = 0xFF;                   
uint16         unQPktASN = 0xFFFF;

  while( pDllQueue < ( g_aDllQueue + g_ucDllCrtQueueSize ) )
  { 
    switch ( pDllQueue->m_ePacketDestType )
    {     
    case DLL_DEST_TYPE_LONG_ADDR:  
      if ( g_aDllLinks[p_pHashLink->m_ucLinkIdx].m_eLinkType == DLL_LINK_TYPE_JOIN )
      {        
        pQPkt = DLL_HoldThisPacket( pDllQueue, pQPkt, &ucQPktPry, &unQPktASN);
      }
      break; // switch
    case DLL_DEST_TYPE_SUPERFRAME:
      if ( g_aDllLinks[p_pHashLink->m_ucLinkIdx].m_eLinkType == DLL_LINK_TYPE_BROADCAST )
      {
        if ( pDllQueue->m_ucSuperframeID == g_aDllSuperframes[p_pHashLink->m_ucSuperframeIdx].m_ucSuperframeID )
        {         
          pQPkt = DLL_HoldThisPacket( pDllQueue, pQPkt, &ucQPktPry, &unQPktASN);
        }
      }  
      break; // switch      
    case DLL_DEST_TYPE_SHORT_ADDR:
      if ( p_pHashLink->m_ucNeighborIdx != DLL_INVALID_NEIGH_IDX && ////
          g_aDllLinks[p_pHashLink->m_ucLinkIdx].m_eLinkType == DLL_LINK_TYPE_NORMAL )
      {
        if ( IntFromLE(pDllQueue->m_aDestAddr ) == g_aDllNeighbors[p_pHashLink->m_ucNeighborIdx].m_unShortAddr )
        {         
          pQPkt = DLL_HoldThisPacket( pDllQueue, pQPkt, &ucQPktPry, &unQPktASN);
        }
      }
      break; // switch
    case DLL_DEST_TYPE_GRAPH:          
      if ( p_pHashLink->m_ucNeighborIdx != DLL_INVALID_NEIGH_IDX ) ////
      {
        for( DLL_HASH_GRAPHS_STRUCT_T * pHashGraph = g_aDllHashGraphs; pHashGraph < (g_aDllHashGraphs + g_ucDllHashGraphsSize); pHashGraph++ )     
        { // Search Graph
          if ( pDllQueue->m_unGraphID == pHashGraph->m_unGraphID ) 
          { // Found Graph
            DLL_CONNECTION_T * pConn = &g_aDllConnections[pHashGraph->m_ucIdx];
            while ( pConn < ( g_aDllConnections + pHashGraph->m_ucIdx + pHashGraph->m_ucLen ) )
            { // Search Neighbors in this graph
              if (
                  ////            ( g_aDllLinks[p_pHashLink->m_ucLinkIdx].m_eLinkType == DLL_LINK_TYPE_JOIN ) 
                  ////              ||
                  ( g_aDllLinks[p_pHashLink->m_ucLinkIdx].m_unNeighborShortAddr == pConn->m_unNeighborShortAddr )
                    )
              { // Neighbor founded ... actualizeaza short addr in packet
                memcpy( pDllQueue->m_aDestAddr, &pConn->m_unNeighborShortAddr, 2); // LE
                pQPkt = DLL_HoldThisPacket( pDllQueue, pQPkt, &ucQPktPry, &unQPktASN);
                break; // from while
              }
              pConn++;
            }
            break; // from for
          }
        }
      }
      break; // switch  
    }
    pDllQueue++;
  }
  return pQPkt;
}

////////////////////////////////////////////////////////////////////////////////        
// ok
////////////////////////////////////////////////////////////////////////////////        
uint8 DLL_FindShortNeighbor( uint16 p_unAddr )
{
  for( unsigned int unIndex = 0; unIndex < g_ucDllAllNeighborsSize; unIndex++ )
  {
    if ( g_aDllNeighbors[unIndex].m_unShortAddr == p_unAddr )
    {
      return unIndex;
    }      
  }  
  return DLL_INVALID_NEIGH_IDX; // neighbor not found, invalid index returned.
}


////////////////////////////////////////////////////////////////////////////////        
// search in the specified neighbor table part for the neighbor with the oldest m_tmLastComm attribute value
////////////////////////////////////////////////////////////////////////////////        
DLL_NEIGHBOR_T * DLL_FindOldCommNeighbor(DLL_NEIGHBOR_T * p_pStart, uint8 p_ucEndIdx)
{
  for(DLL_NEIGHBOR_T * p = p_pStart + 1; p < ( g_aDllNeighbors + p_ucEndIdx); p++ ) 
  { 
    if ( (int32)(p->m_tmLastComm - p_pStart->m_tmLastComm) < 0 )
    { // p is older
      p_pStart = p;
    }       
  }
  return p_pStart;
}

////////////////////////////////////////////////////////////////////////////////        
// ok
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_AddShortNeighbor( uint16 p_unAddr )
{ 
DLL_NEIGHBOR_T * pNeighbor;  
  if ( g_ucDllAllNeighborsSize < DLL_MAX_NEIGHBORS ) 
  { // add to end one non linked neighbor
    pNeighbor = &g_aDllNeighbors[g_ucDllAllNeighborsSize++];
  }
  else
  { // replace the oldest non linked neighbor
    if ( g_ucDllLkdNeighborsSize >= DLL_MAX_NEIGHBORS ) 
    {
     return DLL_OUT_OF_MEMORY; 
    }    
    pNeighbor = &g_aDllNeighbors[g_ucDllLkdNeighborsSize];                    
    for( DLL_NEIGHBOR_T * pLocalNeighbor = pNeighbor + 1; pLocalNeighbor < ( g_aDllNeighbors + DLL_MAX_NEIGHBORS ); pLocalNeighbor++ )
    { // search between non linked neighbors to replace the oldest one  
//      if ( pLocalNeighbor->m_tmLastComm < pNeighbor->m_tmLastComm )
      if ( (int32)(pLocalNeighbor->m_tmLastComm - pNeighbor->m_tmLastComm) < 0 )
      { // pLocalNeighbor is older
         pNeighbor = pLocalNeighbor;
      }
    }
  }
  // Clean 
  memset( pNeighbor, 0x00, sizeof( DLL_NEIGHBOR_T ) );
  
  // Add
  pNeighbor->m_unShortAddr = p_unAddr;    
  pNeighbor->m_tmLastComm = g_sDllASN.m_ulLsb;
  pNeighbor->m_tmPathFailCounter = g_stProv.ulDllPathFail;
  pNeighbor->m_cAvgRSL = -128; // init with min value
  return DLL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// ok
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_FindSuperframe( uint8 p_ucSuperframeID )
{
  for( unsigned int unIndex = 0; unIndex < g_ucDllAllSuperframesSize; unIndex++ )   
  {
    if ( g_aDllSuperframes[unIndex].m_ucSuperframeID == p_ucSuperframeID )      
    {      
      return unIndex;
    }  
  }    
  return DLL_INVALID_SF_IDX; // superframe not found, invalid index returned.
}

////////////////////////////////////////////////////////////////////////////////
// ok
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_AddSuperframe( uint8 p_ucSuperframeID, uint8 p_ucSuperframeMode, uint16 p_unNumSlots )
{
  if ( g_ucDllAllSuperframesSize >= DLL_MAX_SUPERFRAMES ) 
    return DLL_OUT_OF_MEMORY; 
  
  DLL_SUPERFRAME_T * pSF = &g_aDllSuperframes[g_ucDllAllSuperframesSize++];
  
  memset( pSF, 0x00, sizeof(DLL_SUPERFRAME_T) );
  pSF->m_ucSuperframeID = p_ucSuperframeID;
  pSF->m_unNumSlots = p_unNumSlots;
  if(p_ucSuperframeMode & 0x80)   // handheld superframe flag is set
    pSF->m_bHandheldSF = 1;       // shoud this be delayed to next slot?
  if(p_ucSuperframeMode & DLL_SF_ENABLED)   // superframe active flag is set
    pSF->m_bSetEnabled = 1;       // will be activated next slot
   
  return DLL_SUCCESS; 
}

////////////////////////////////////////////////////////////////////////////////
// .a
////////////////////////////////////////////////////////////////////////////////
void DLL_ModifySuperframe( uint8 p_ucIndex, uint16 p_unNumSlots)
{   
  g_aDllSuperframes[p_ucIndex].m_unNewSlots = p_unNumSlots;
  g_aDllSuperframes[p_ucIndex].m_bModifySlots = 1;    
  for (uint8 ucLinkIdx = 0; ucLinkIdx < g_ucDllAllLinksSize; ucLinkIdx++ )           // delete links from the removed slots
  {
    if ( 
         ( g_aDllLinks[ucLinkIdx].m_ucSuperframeID == g_aDllSuperframes[p_ucIndex].m_ucSuperframeID ) && 
         ( g_aDllLinks[ucLinkIdx].m_unSlot >= p_unNumSlots )
       )
    {
      DLL_DeleteLink( ucLinkIdx );
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// .a
////////////////////////////////////////////////////////////////////////////////
void DLL_SetSuperframe( uint8 p_ucIndex, DLL_SF_STATUS_T p_eActive)
{   
  if ( p_eActive == DLL_SF_ENABLED )
  {
    g_aDllSuperframes[p_ucIndex].m_bSetEnabled = 1;
  }
  else
  {
    g_aDllSuperframes[p_ucIndex].m_bSetDisabled = 1;
  }
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_DeleteSuperframe( uint8 p_ucIndex )
{
  // Delete all links for this superframe :
  for(uint8 ucLinkIdx = 0; ucLinkIdx < g_ucDllAllLinksSize; ucLinkIdx ++ )
  {
    if ( g_aDllLinks[ucLinkIdx].m_ucSuperframeID == g_aDllSuperframes[p_ucIndex].m_ucSuperframeID )
    {
      DLL_DeleteLink(ucLinkIdx);
    }
  }      
  g_aDllSuperframes[p_ucIndex].m_bMarkDeleted = 1; 
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_FindLink( uint8 p_ucSuperframeID, uint16 p_unNeighborShortAddr, uint16 p_unSlot )
{
  for( uint8 ucIndex = 0; ucIndex < g_ucDllAllLinksSize; ucIndex++ )
  {
    if  ( 
          ( g_aDllLinks[ucIndex].m_ucSuperframeID == p_ucSuperframeID ) && 
          ( g_aDllLinks[ucIndex].m_unNeighborShortAddr == p_unNeighborShortAddr ) && 
          ( g_aDllLinks[ucIndex].m_unSlot == p_unSlot ) 
        )
    {      
      return ucIndex;
    }      
  }  
  return DLL_INVALID_LINK_IDX; // link not found, invalid index returned.
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_AddLink( uint8 p_ucSuperframeID, 
                   uint16 p_unNeighborShortAddr, 
                   uint16 p_unSlot, 
                   uint8 p_ucChOffset, 
                   uint8 p_ucTx,
                   uint8 p_ucRx,    
                   uint8 p_ucShared,
                   DLL_LINK_TYPE_T p_eLinkType )
{  
//  if ( ((g_ucDllAllLinksSize - g_ucDllDeletedLinks) >= DLL_MAX_LINKS) ||
//       (g_ucDllAllLinksSize >= (DLL_MAX_LINKS + DLL_HIDDEN_LINKS)) )
  if ( g_ucDllAllLinksSize >= DLL_MAX_LINKS )
    return DLL_OUT_OF_MEMORY; 
  
//  if ( p_eLinkType != DLL_LINK_TYPE_BROADCAST )
  if ( p_unNeighborShortAddr != 0xFFFF )
  {
    uint8 ucIdx = DLL_FindShortNeighbor( p_unNeighborShortAddr );
    if(ucIdx != DLL_INVALID_NEIGH_IDX)
    {
      if(p_ucTx)
        g_aDllNeighbors[ucIdx].m_bLinked |= LinkOptionFlagCodesMask_Transmit;   // 0x01 // CommonTable46
      if(p_ucRx)
        g_aDllNeighbors[ucIdx].m_bLinked |= LinkOptionFlagCodesMask_Receive;    // 0x02
      if(p_ucShared)
        g_aDllNeighbors[ucIdx].m_bLinked |= LinkOptionFlagCodesMask_Shared;     // 0x04
      if(ucIdx >= g_ucDllLkdNeighborsSize)
        g_aDllNeighbors[ucIdx].m_tmPathFailCounter = g_stProv.ulDllPathFail;    // to avoid an immediate PathFail alarm, and give a chance to send KA first
    }
  }
  
  DLL_LINK_T * pLink = &g_aDllLinks[g_ucDllAllLinksSize++];
  
  pLink->m_ucSuperframeID       = p_ucSuperframeID; 
  pLink->m_unNeighborShortAddr  = p_unNeighborShortAddr;
  pLink->m_unSlot               = p_unSlot; 
  pLink->m_ucChannelOffset      = p_ucChOffset;  
  pLink->m_bTx                  = p_ucTx;   
  pLink->m_bRx                  = p_ucRx;
  pLink->m_bShared              = p_ucShared;    
  pLink->m_eLinkType            = p_eLinkType; 
  pLink->m_bMarkDeleted         = 0;   
  return DLL_SUCCESS; 
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_DeleteLink( uint8 p_ucIndex )
{  
  if ( p_ucIndex < g_ucDllAllLinksSize )
  {           
    g_aDllLinks[p_ucIndex].m_bMarkDeleted = 1;    
    //if ( g_aDllLinks[p_ucIndex].m_eLinkType == DLL_LINK_TYPE_NORMAL )
    //if ( g_aDllLinks[p_ucIndex].m_eLinkType != DLL_LINK_TYPE_BROADCAST )
    if ( g_aDllLinks[p_ucIndex].m_unNeighborShortAddr != 0xFFFF )
    {
      uint8 ucLocalIdx, ucLinked;        
      for ( ucLocalIdx=0, ucLinked=0; ucLocalIdx < g_ucDllAllLinksSize; ucLocalIdx++ )      // search for other links of that neighbor
      {
        if ( ( g_aDllLinks[ucLocalIdx].m_unNeighborShortAddr == g_aDllLinks[p_ucIndex].m_unNeighborShortAddr ) &&
            ( g_aDllLinks[ucLocalIdx].m_bMarkDeleted == 0 ) )
        {
          if(g_aDllLinks[ucLocalIdx].m_bTx)
            ucLinked |= LinkOptionFlagCodesMask_Transmit;   // 0x01 // CommonTable46
          if(g_aDllLinks[ucLocalIdx].m_bRx)
            ucLinked |= LinkOptionFlagCodesMask_Receive;    // 0x02
          if(g_aDllLinks[ucLocalIdx].m_bShared)
            ucLinked |= LinkOptionFlagCodesMask_Shared;     // 0x04
        }
      } 
      ucLocalIdx = DLL_FindShortNeighbor( g_aDllLinks[p_ucIndex].m_unNeighborShortAddr );        
      if ( ucLocalIdx < g_ucDllAllNeighborsSize ) // ??                               
      { // update the neighbor table       
        g_aDllNeighbors[ucLocalIdx].m_bLinked = ucLinked;
        
#ifdef BACKBONE_SUPPORT   
        // this is connected with the CMD971
        if(g_aDllNeighbors[ucLocalIdx].m_bTimeSource & 0x80)
          g_aDllNeighbors[ucLocalIdx].m_bLinked |= LinkOptionFlagCodesMask_Receive;  
#endif
        
//        if((ucLinked & (LinkOptionFlagCodesMask_Transmit | LinkOptionFlagCodesMask_Receive)) == 0)
//        { // means that no other valid link to that neighbor was found
//          g_aDllNeighbors[ucLocalIdx].m_bTimeSource = 0;    //  
//        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint16 DLL_FindConnectionGraph( uint16 p_unNeighborShortAddr )
{  
  for( uint8 ucIndex=0; ucIndex < g_ucDllAllConnectionsSize; ucIndex++)
  {
    if ( g_aDllConnections[ucIndex].m_unNeighborShortAddr == p_unNeighborShortAddr )
      return g_aDllConnections[ucIndex].m_unGraphID;      
  }  
  return DLL_INVALID_GRAPH_ID; // connection not found, invalid graph id index returned.   
}  
  
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_FindConnection( uint16 p_unGraphID, uint16 p_unNeighborShortAddr)
{
  for( uint8 ucIndex = 0; ucIndex < g_ucDllAllConnectionsSize; ucIndex++ )
  {
    if ( 
          ( g_aDllConnections[ucIndex].m_unGraphID == p_unGraphID ) &&
          ( g_aDllConnections[ucIndex].m_unNeighborShortAddr == p_unNeighborShortAddr )
        )
    {
      return ucIndex;
    }      
  }  
  return DLL_INVALID_CONN_IDX; // connection not found, invalid index returned.    
}

///////////////////////////////////////////////////////////////////////////////////
// Name:          DLL_FindFirstGraphIdx
// Author:        NIVIS LLC, Doinel Alban
// Description:   Search the ConnectionTable for the first occurence of the specified GraphId 
// Parameters:    p_unGraphID
// Return:        connection index OR invalid index
// Obs:           Access level: interrupt and, possibly, user level
///////////////////////////////////////////////////////////////////////////////////
uint8 DLL_FindFirstGraphIdx(uint16 p_unGraphID)
{
  for( uint8 ucIndex = 0; ucIndex < g_ucDllAllConnectionsSize; ucIndex++ )
  {
    if (g_aDllConnections[ucIndex].m_unGraphID == p_unGraphID)
    {
      return ucIndex;
    }      
  }  
  return DLL_INVALID_CONN_IDX; // connection not found, invalid index returned.    
} // End - DLL_FindFirstGraphIdx()
  
////////////////////////////////////////////////////////////////////////////////
// Connections table must be kept sorted by Graph, Neighbor ... 
// ... first of all check outside all premises !!!
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_AddConnection( uint16 p_unGraphID, uint16 p_unNeighborShortAddr)
{
  if ( g_ucDllAllConnectionsSize >= DLL_MAX_CONNECTIONS )
    return DLL_OUT_OF_MEMORY; // out of memory
	
  DLL_CONNECTION_T * pConn = &g_aDllConnections[g_ucDllAllConnectionsSize++];
  
  pConn->m_bMarkDeleted = 0;
  pConn->m_unGraphID = p_unGraphID;
  pConn->m_unNeighborShortAddr = p_unNeighborShortAddr;
  return DLL_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_AuthIncomingPkt()
{  
  uint8 * pBuffer = g_aucDllBuffer;
  uint8 ucTmp;
  
  // Check : Frame Control
  if ( (*pBuffer) != DLL_FRAME_CONTROL )
  { 
    return DLL_DISCARD_PACKET; 
  }
  pBuffer++;
  
  // Check : Address Specifier ... processing Src and Dest addresses, DLPDU Specifier and DLL Payload
  ucTmp = *(pBuffer++); 
  if( ((ucTmp & 0xBB) != 0x88) /*|| (ucTmp == 0xCC)*/ ) // Address Spec 0xCC is also valid 
  { 
    return DLL_DISCARD_PACKET; // wrong Address Specifier or both Destionation and Source Address are Long
  }
  g_sDllBufferParsed.m_bSrcAddrLong = ( ucTmp & DLL_SRC_ADDR_MASK ) ? DLL_ADDR_LONG : DLL_ADDR_SHORT;
  g_sDllBufferParsed.m_bDstAddrLong = ( ucTmp & DLL_DST_ADDR_MASK ) ? DLL_ADDR_LONG : DLL_ADDR_SHORT;  
  
  // Check : SQN 
  if ( ( g_ucDllStatus == DLL_JOINED ) && ( (*pBuffer) != g_sDllASN.m_aucBytes[0] ) )
  {                                                                             // ... to do here : ... poate ar fi util sa verific daca eu sunt cel nesincronizat (din numarul de pachete cu aceasta caracteristica)
    return DLL_DISCARD_PACKET;                                                  // ... si daca eu sunt ala desincronizat poate trebuie sa semnalez mai sus si sa inceapa procedura de reinregistrare    
  }
  pBuffer++;
  
  // Check : NetworkID
#ifdef TEST_NET_ID
  if( (*pBuffer == (TEST_NET_ID & 0xFF)) && (*(pBuffer+1) == (TEST_NET_ID >> 8)))
    g_unNetworkId = TEST_NET_ID;
#endif
  if ( memcmp( pBuffer, &g_unNetworkId, 2) )
  {
    return DLL_DISCARD_PACKET;  
  }
  pBuffer += 2;

  // Get : Destination address
  g_sDllBufferParsed.m_pucDstAddr = pBuffer;
  pBuffer += ( g_sDllBufferParsed.m_bDstAddrLong ? 8 : 2 );
  
  // Get : Source address
  g_sDllBufferParsed.m_pucSrcAddr = pBuffer; 
  pBuffer += ( g_sDllBufferParsed.m_bSrcAddrLong ? 8 : 2 );
 
  // Get : DLPDU Specifier
  ucTmp = *(pBuffer++); 
  g_sDllBufferParsed.m_bPriority    = ( ( ucTmp & 0x30 ) >> 4 ); 
  g_sDllBufferParsed.m_bEncryptKey  = ( ucTmp & 0x08 ) ? 1 : 0; 
  ucTmp &= 0x07;  // mask for packet type
  if((ucTmp > DLL_PACKET_TYPE_DISCONNECT) && (ucTmp < DLL_PACKET_TYPE_DATA)) 
  {
    return DLL_DISCARD_PACKET;  // discard packet with invalid type
  }
  g_sDllBufferParsed.m_bPacketType  = ucTmp;
  
  // Get : DLL Payload 
  g_sDllBufferParsed.m_pucDLPDUPayload = pBuffer; 
  g_sDllBufferParsed.m_ucPayloadLen = g_ucDllBufferLen - ( pBuffer - g_aucDllBuffer );  
    
  // Authentication : COMPUTE NONCE
  if ( ( g_ucDllStatus == DLL_ACTIVE_SEARCH ) && ( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_ADVERTISE ) )
  { // use the ASN value from Advertise packet
    memcpy( stAligned.m_aucNonce , g_sDllBufferParsed.m_pucDLPDUPayload, 5 );   // ordinea este corecta MSB first la ambele
  }
  else
  {
    DLL_GetASN_Intr( stAligned.m_aucNonce );
  }
  if ( g_sDllBufferParsed.m_bSrcAddrLong )                                      // ATENTIE :     
  {                                                                             // SrcAddr este LE ... in nounce trebuie BE ... deci trebuie copiata invers    
    stAligned.m_aucNonce[5]  = g_sDllBufferParsed.m_pucSrcAddr[7]; // MSB
    stAligned.m_aucNonce[6]  = g_sDllBufferParsed.m_pucSrcAddr[6]; // to 
    stAligned.m_aucNonce[7]  = g_sDllBufferParsed.m_pucSrcAddr[5]; // ...
    stAligned.m_aucNonce[8]  = g_sDllBufferParsed.m_pucSrcAddr[4];
    stAligned.m_aucNonce[9]  = g_sDllBufferParsed.m_pucSrcAddr[3];
    stAligned.m_aucNonce[10] = g_sDllBufferParsed.m_pucSrcAddr[2];    
  }
  else      
  { 
    memset( &stAligned.m_aucNonce[5], 0, 6);
  }
  stAligned.m_aucNonce[11] = g_sDllBufferParsed.m_pucSrcAddr[1]; // ...
  stAligned.m_aucNonce[12] = g_sDllBufferParsed.m_pucSrcAddr[0]; // LSB
  
  // Authentication : AUTH and DECRYPT
  g_ucDllBufferLen -= 4; // DLL MIC
  if ( 
      AES_SUCCES != AES_Decrypt_Intr( 
                                      ( g_sDllBufferParsed.m_bEncryptKey == DLL_KEY_WELL_KNOWN ? c_aucWKKey : g_aucNetworkKey ), 
                                      stAligned.m_aucNonce, 
                                      g_aucDllBuffer, 
                                      g_ucDllBufferLen, 
                                      g_aucDllBuffer + g_ucDllBufferLen, 
                                      4, 
                                      NULL) 
      )
  {
    g_stC779.m_ucNoOfDataLinkLayerMICFailuresDetected++; // inc Cmd779 counter
#ifndef TEST_HCFJD    // TEST -[doinel.alban] 
    return DLL_DISCARD_PACKET;
#endif // TEST_HCFJD 
  }

  if ( ( g_ucDllStatus == DLL_ACTIVE_SEARCH ) && ( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_ADVERTISE ) )
  { // compare the ASN in the advertise with the current ASN and keep the newest
    if( g_ucNetAdNumber ) // at least one ADV received
    { // this is not the first advertise heard
      DLL_GetASN_Intr( stAligned.m_aucNonce );  // get current ASN in Big Endian (5 bytes)
      signed int n = memcmp(stAligned.m_aucNonce, g_sDllBufferParsed.m_pucDLPDUPayload, 5);
      if(n < 0)
        return DLL_DISCARD_PACKET;  // current ASN is lower (i.e. newer) than the advertise's
      if(n > 0)
      { // current ASN is higher (i.e. older) than the advertise's
        // erase the previously received advertises, neighbors, etc., and take this advertise as the first
//        g_ucDllAllNeighborsSize = 0;
//        g_ucDllLkdNeighborsSize = 0;
//        g_ucNetJoinState = NET_JOIN_SEARCHING; 
        
        PrepareForReset(RREASON_OLD_ADVERTISE ); //
      }
    }
  }
  
  return DLL_SUCCESS;
}


////
////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_ParseIncomingPkt()
{  
  // find out if this message needs ACK     
  if ( g_sDllBufferParsed.m_bDstAddrLong ) 
  { // long destination address
    if (
        memcmp( g_sDllBufferParsed.m_pucDstAddr, g_aJoinDevUID_LE, 5 ) || 
        memcmp( g_sDllBufferParsed.m_pucDstAddr + 5, c_aHCF_OUI_LE, 3 )            
        ) 
    { // not for me 
      return DLL_DISCARD_PACKET; 
    }   
    // message for me ... I need to ACK it
    g_sDllBufferParsed.m_ucNeedAck = 1;   
    g_sDllBufferParsed.m_ucBroadcast = 0;
    UART1_String("DJRL");              // DLL Joining: Rx packet with long source/dest address (Join Request, Join response)
  }
  else 
  { // short destination address
    if ( memcmp( g_sDllBufferParsed.m_pucDstAddr, &c_aBcastAddr, 2 ) )
    { // not broadcast         
      g_sDllBufferParsed.m_ucBroadcast = 0;
      if ( g_ucDllStatus == DLL_JOINED )
      { 
        if ( memcmp( g_sDllBufferParsed.m_pucDstAddr, &g_unNickname, 2 ) )
        { // not for me
          return DLL_DISCARD_PACKET; 
        }        
        // message for me ... I need to ACK it
        g_sDllBufferParsed.m_ucNeedAck = 1;        
      }
      else
      { // not for me but process it                                            // in prejoining permit some packets in searching for ADV packets 
        g_sDllBufferParsed.m_ucNeedAck = 0;                                     // you need to process this packets but not to ACK it
      }                
    } 
    else 
    { // broadcast message       
      g_sDllBufferParsed.m_ucBroadcast = 1;
      g_sDllBufferParsed.m_ucNeedAck = 0;                                       // not need ACK
    }        
  }        
  return DLL_SUCCESS;
}
////

//!! must be called with parametes g_astAdvNeighbors[ucNeighborIndex].m_unNickname and g_astAdvNeighbors[ucNeighborIndex].m_aucPayload
// DLL interrupts must be disabled when calling this function!! 
uint8 NET_ParseProxyInfo(uint16 unNeighShortAddr, uint8* pMsg)
{
  //unNeighShortAddr = g_astAdvNeighbors[ucNeighborIndex].m_unNickname
  //uint8 *pMsg = g_astAdvNeighbors[ucNeighborIndex].m_aucPayload;
  uint8   ucLinks;
  uint8   ucID;
  uint8   ucSfCnt;
  
  // GraphID ... this will be used to route JoinRequest, and after join, to compose Advertise packets 
  g_sDllSharedParams.m_unJoinGraphID = IntFromBE( pMsg );
  pMsg += 2;  
    
  // Get the number of Superframes containing join links
  ucSfCnt = *(pMsg++);

  for(uint8 ucIdxConn=0; ucIdxConn < g_ucDllAllConnectionsSize; ucIdxConn++)
  {
    g_aDllConnections[ucIdxConn].m_bMarkDeleted = 1;
  }
  for(uint8 ucIdxSf=0; ucIdxSf < g_ucDllAllSuperframesSize; ucIdxSf++)
  {
    DLL_DeleteSuperframe(ucIdxSf);
  }
  
  DLL_AddConnection( g_sDllSharedParams.m_unJoinGraphID, unNeighShortAddr );
  
  // Process All these Superframes one by one until the end of message
  while ( ucSfCnt-- )
  {
    ucID = *(pMsg++);
    if ( DLL_AddSuperframe( ucID, DLL_SF_ENABLED, IntFromBE( pMsg ) ) )
    {
      return DLL_ERROR; // ... out of memory for superframes
    }
    pMsg += 2;
    ucLinks = *(pMsg++);    
    while( ucLinks-- )
    {                  
      // Process All Links in this Superframe       
//      if ( DLL_FindLink( ucID, DLL_BROADCAST_ADDR, IntFromBE( pMsg )) == DLL_INVALID_LINK_IDX  )

        uint8 ucTxBit = (0x40 & *(pMsg + 2) ? 1 : 0); // Tx, Shared    
        
        if ( 
             DLL_AddLink( 
                         ucID,                                // p_ucSuperframeID
                         unNeighShortAddr,  // DLL_BROADCAST_ADDR, // p_unNeighborShortAddr
                         IntFromBE( pMsg ),                   // p_unSlot
                         ( 0x3F & (*(pMsg + 2)) ),            // p_ucChOffset                   
                         ucTxBit,    //( aZero[0] ? 1 : 0),   // p_ucTx
                         1 - ucTxBit, //( aZero[0] ? 0 : 1),  // p_ucRx
                         ucTxBit,    //( aZero[0] ? 1 : 0),   // p_ucShared
                         DLL_LINK_TYPE_JOIN                   // p_eLinkType
                        )
            )
        {
          return DLL_ERROR; // ... out of memory for links
        }        
    
      pMsg+=3;
    }                                                              
  }    
    
  for( ucID = 0; ucID < g_ucDllAllNeighborsSize; ucID++ )
  {
    if ( g_aDllNeighbors[ucID].m_unShortAddr == unNeighShortAddr )
    {
      g_aDllNeighbors[ucID].m_bTimeSource = 1;
    }
    else
    {
      g_aDllNeighbors[ucID].m_bTimeSource = 0;
    }
  }    

  COMMIT_MODIFY_DLL_STRUCTS();
  
  return DLL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_ParseAdvertisePkt(uint8 ucNeighborIndex)
{  
uint8   * pMsg = g_sDllBufferParsed.m_pucDLPDUPayload;  
uint16  unBuffer;
uint8   ucMode;
  // Join Control - Security level supported (see common table 53) ... should be 0 for HART7 ???
// HCF test tool gives 0x11 in byte 5!!!
//  if( 0xF0 & pMsg[5] )
//  {
//    return DLL_ERROR; // error security level not supported
//  }

  // Channel Map Check (and set if active search)  
  unBuffer = IntFromLE( pMsg + 7 );
  ucMode = ( g_ucDllStatus != DLL_ACTIVE_SEARCH );  // ? 1 : 0;
  if ( DLL_CheckAndSetChannelMap( ucMode, pMsg[6], unBuffer ) )
  {
    return DLL_ERROR; // channel map invalid or doesn't match with the actual used one
  }
  
  if ( !ucMode )
  { // DLL_ACTIVE_SEARCH
    
    // Set ASN and slot timer each time, to stay synchronized during active search
    g_sDllASN.m_aucBytes[4] = pMsg[0];
    g_sDllASN.m_aucBytes[3] = pMsg[1];
    g_sDllASN.m_aucBytes[2] = pMsg[2];
    g_sDllASN.m_aucBytes[1] = pMsg[3];
    g_sDllASN.m_aucBytes[0] = pMsg[4];
    g_stDllPredSlot.m_uc250msSlotNo = g_sDllASN.m_ullASN % MAX_250MS_SLOT_NO;
    g_aDllNeighbors[ucNeighborIndex].m_tmLastComm = g_sDllASN.m_ulLsb; // ASN was 0 when neighbor was created
    g_aDllNeighbors[ucNeighborIndex].m_bTimeSource = 1;  // ??? 
  }  
  
  g_aDllNeighbors[ucNeighborIndex].m_bJoinPriority = 0x0F & pMsg[5];            // Update Neighbor : join priority

  return DLL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_ComputeSpecialNonce()
{
  // COMPUTE NONCE only for Advertise and KA (Dll generated packets)
//  memcpy( stAligned.m_aucNonce, g_aDllASN, 5);
  DLL_GetASN_Intr( stAligned.m_aucNonce );
  if(g_unNickname == 0xFFFF)
  {
    memcpy( &stAligned.m_aucNonce[5], c_aHCF_OUI_BE, 3 );    
    memcpy( &stAligned.m_aucNonce[8], g_aJoinDevUID_BE, 5 ); 
  }
  else
  {
    memset(&stAligned.m_aucNonce[5], 0, 6);         // ATENTIE : SrcAddr este LE ... in nonce trebuie BE ... deci trebuie copiata invers        
    stAligned.m_aucNonce[11] = (g_unNickname >> 8); // MSB   
    stAligned.m_aucNonce[12] = g_unNickname;        // LSB  
  }
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_ComputeAdvertisePkt(DLL_NEIGHBOR_T * p_pNeighbor)
{
  uint8 * pBuffer = g_aucDllBuffer;
      
  // frame control
  *(pBuffer++) = DLL_FRAME_CONTROL;
  // address specifier : Source and Dest Address = only short both of them ... that means 0x88
  *(pBuffer++) = 0x88; 
  // SQN
  *(pBuffer++) = g_sDllASN.m_aucBytes[0];
  // NetID
  memcpy( pBuffer, &g_unNetworkId, 2);                                          // little endian     
  pBuffer+=2;  
  // Dest ADDR
  if ( p_pNeighbor )
  {
    memcpy( pBuffer, (void const *) &(p_pNeighbor->m_unShortAddr), 2);          // little endian
    g_sDllSlot.m_ucNeedACK = 1;
  }
  else
  {
    memset( pBuffer, 0xFF, 2);                                                  // broadcast !!!
    g_sDllSlot.m_ucNeedACK = 0;
  }
  pBuffer+=2;    
  // Source ADDR
  memcpy( pBuffer, &g_unNickname, 2);                                           // little endian         
  pBuffer += 2;
  // DLPDU Spec
//#ifdef TEST_09FB
  *(pBuffer++) = (DLL_PACKET_PRIORITY_COMMAND << 4) | (DLL_KEY_WELL_KNOWN << 3) | (DLL_PACKET_TYPE_ADVERTISE);
//#else
//  *(pBuffer++) = (DLL_PACKET_PRIORITY_NORMAL << 4) | (DLL_KEY_WELL_KNOWN << 3) | (DLL_PACKET_TYPE_ADVERTISE);
//#endif
  // DLPDU Payload ... compute the payload with join information
//  memcpy( pBuffer, g_aDllASN, 5 );                                              // ASN
  DLL_GetASN_Intr( pBuffer );
  pBuffer+=5;
#ifdef TEST_09FB
  *(pBuffer++) = (SecurityTypeCodes_JoinKeyed << 4) | ( 0x0F & g_ucDllJoinPriority );     // Join Control = Security + Priority
#else
  *(pBuffer++) = (SecurityTypeCodes_SessionKeyed << 4) | ( 0x0F & g_ucDllJoinPriority );  // Join Control = Security + Priority
#endif
  *(pBuffer++) = g_ucDllTotalChannels;                                          // No of bits in Channel Map  
  *(pBuffer++) = g_unDllChannelMap;                                             // Channel Map LSB    
  if ( g_ucDllTotalChannels > 8 ) // ? always using 2-byte Channel Map ?
  {
    *(pBuffer++) = (g_unDllChannelMap >> 8);                                    // Channel Map MSB
  }
#ifdef TEST_09FB
  *(pBuffer++) = 0;                                        // Graph ID MSB  
  *(pBuffer++) = 0;                                               // Graph ID LSB  
#else
  *(pBuffer++) = (g_sDllSharedParams.m_unJoinGraphID >> 8);                                        // Graph ID MSB  
  *(pBuffer++) = g_sDllSharedParams.m_unJoinGraphID;                                               // Graph ID LSB  
#endif
  
  uint8 ucIdxJoinSf = pBuffer - g_aucDllBuffer;                                       // Len ( DLL Payload ) = MaxLen(DLPDU) - 14; (header + mic) = 14
  uint8 ucNumJoinSf = 0;
  uint8 ucIdxJoinLnk;
  uint8 ucNumJoinLnk;
  pBuffer++;
  for ( uint8 i = 0; i < g_ucDllCrtSuperframesSize; i++ )  
  {
    if ( g_aDllSuperframes[i].m_eActive == DLL_SF_ENABLED )
    {
      ucIdxJoinLnk = 0;
      ucNumJoinLnk = 0;
      for ( uint8 j = 0; j < g_ucDllCrtLinksSize; j++ )
      {
        if ( ( g_aDllLinks[j].m_eLinkType == DLL_LINK_TYPE_JOIN ) && ( g_aDllSuperframes[i].m_ucSuperframeID == g_aDllLinks[j].m_ucSuperframeID ) )
        {
          // Add superframe
          if ( ucIdxJoinLnk == 0 )
          {         
            if ( (pBuffer - g_aucDllBuffer) < 7 )                               // 7 = 4 (un superframe) + 3 (un link)
              return;                                                           // daca linkurile sunt prea multe din cauza ca nu mi-au fost configurate bine atunci nu mai trimit nimic
            
            *(pBuffer++) = g_aDllSuperframes[i].m_ucSuperframeID;               // Join Superframe ID 
            *(pBuffer++) = (g_aDllSuperframes[i].m_unNumSlots >> 8);            // Superframe NumSlots MSB  
            *(pBuffer++) = g_aDllSuperframes[i].m_unNumSlots;                   // Superframe NumSlots LSB
            ucIdxJoinLnk = pBuffer - g_aucDllBuffer;                            // Num Links Idx
            pBuffer++;
            ucNumJoinSf++;
          }
          
          // Add Link 
          if ( (pBuffer - g_aucDllBuffer) < 3 )                                 // daca linkurile sunt prea multe din cauza ca nu mi-au fost configurate bine atunci nu mai trimit nimic
            return;                                                             // este treaba NM ca eu sa fiu configurat corect
          
          *(pBuffer++) = ( g_aDllLinks[j].m_unSlot >> 8 );                      // MSB
          *(pBuffer++) = g_aDllLinks[j].m_unSlot;                               // LSB
          *(pBuffer++) = ( g_aDllLinks[j].m_bRx ? 0x40 : 0x00 ) | ( 0x3F & g_aDllLinks[j].m_ucChannelOffset) ; 
          ucNumJoinLnk++;
        }
      }
      if ( ucIdxJoinLnk )
      {
        g_aucDllBuffer[ucIdxJoinLnk] = ucNumJoinLnk;
      }            
    }
  }
  g_aucDllBuffer[ucIdxJoinSf] = ucNumJoinSf;
  
  if ( ucNumJoinSf == 0 ) 
    return;                                                                     // nu are sens sa trimit Advertise daca nu am join links 
  
  // COMPUTE NONCE 
  DLL_ComputeSpecialNonce();
  // ENCRYPTION
  AES_Crypt_Intr( 
                  c_aucWKKey ,                        // pKey
                  stAligned.m_aucNonce,               // pNonce
                  g_aucDllBuffer,                     // pToAuthOnly
                  pBuffer - g_aucDllBuffer,           // nToAuthOnlyLen
                  pBuffer,                            // pToEncrypt
                  0,                                  // nToEncryptLen
                  pBuffer                             // pEncryptedBuff
                 );
  g_ucDllBufferLen = pBuffer - g_aucDllBuffer + 4;    // Add MIC  
  
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_ComputeKeepAlivePkt(DLL_NEIGHBOR_T * p_pNeighbor)
{  
  uint8 * pBuffer = g_aucDllBuffer;
  uint8 ucLongSrcAddr = (g_unNickname == 0xFFFF) ? 0x40 : 0x00;
      
  // frame control
  *(pBuffer++) = DLL_FRAME_CONTROL;
  // address specifier : Source Address could be long
  *(pBuffer++) = 0x88 | ucLongSrcAddr; 
  // SQN
  *(pBuffer++) = g_sDllASN.m_aucBytes[0];          
  // NetID
  memcpy( pBuffer, &g_unNetworkId, 2);                                          // little endian     
  pBuffer+=2;  
  // Dest ADDR
//  memcpy( pBuffer, (void const *) &(p_pNeighbor->m_unShortAddr), 2);            // little endian
//  g_sDllSlot.m_ucNeedACK = 1;
//  pBuffer+=2;    
  if ( p_pNeighbor )
  {
    memcpy( pBuffer, (void const *) &(p_pNeighbor->m_unShortAddr), 2);          // little endian
    g_sDllSlot.m_ucNeedACK = 1;
  }
  else
  {
    memset( pBuffer, 0xFF, 2);                                                  // send KA to FFFF on Discovery links
    g_sDllSlot.m_ucNeedACK = 0;
  }
  pBuffer+=2;    
  // Source ADDR
  if(ucLongSrcAddr)
  {
    memcpy( pBuffer, g_aJoinDevUID_LE, 5 ); 
    memcpy( pBuffer + 5, c_aHCF_OUI_LE, 3 );    
    pBuffer += 8;
  }
  else
  {
    memcpy( pBuffer, &g_unNickname, 2);                                           // little endian         
    pBuffer += 2;
  }
  // DLPDU Spec
//  *(pBuffer++) = (DLL_PACKET_PRIORITY_NORMAL << 4) | (ucLongSrcAddr ? 0 : (DLL_KEY_NETW << 3)) | (DLL_PACKET_TYPE_KEEPALIVE);  
  *(pBuffer++) = (DLL_PACKET_PRIORITY_COMMAND << 4) | (ucLongSrcAddr ? 0 : (DLL_KEY_NETW << 3)) | (DLL_PACKET_TYPE_KEEPALIVE);  
  // DLPDU Payload = field Empty
  ;
      
  // COMPUTE NONCE 
  DLL_ComputeSpecialNonce();  
  // ENCRYPTION
  AES_Crypt_Intr( 
                  ucLongSrcAddr ? c_aucWKKey : g_aucNetworkKey,   // pKey
                  stAligned.m_aucNonce,               // pNonce
                  g_aucDllBuffer,                     // pToAuthOnly
                  pBuffer - g_aucDllBuffer,           // nToAuthOnlyLen
                  pBuffer,                            // pToEncrypt
                  0,                                  // nToEncryptLen
                  pBuffer                             // pEncryptedBuff
                 );
  g_ucDllBufferLen = pBuffer - g_aucDllBuffer + 4;    // Add MIC  
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_ComputeDisconnectPkt()
{  
  uint8 * pBuffer = g_aucDllBuffer;
  uint8 ucLongSrcAddr = (g_unNickname == 0xFFFF) ? 0x40 : 0x00;
      
  // frame control
  *(pBuffer++) = DLL_FRAME_CONTROL;
  // address specifier : Source Address could be long
  *(pBuffer++) = 0x88 | ucLongSrcAddr; 
  // SQN
  *(pBuffer++) = g_sDllASN.m_aucBytes[0];          
  // NetID
  memcpy( pBuffer, &g_unNetworkId, 2);                                          // little endian     
  pBuffer+=2;  
  // Dest ADDR
  memset( pBuffer, 0xFF, 2);
  
  pBuffer+=2;    
  // Source ADDR
  if(ucLongSrcAddr)
  {
    memcpy( pBuffer, g_aJoinDevUID_LE, 5 ); 
    memcpy( pBuffer + 5, c_aHCF_OUI_LE, 3 );    
    pBuffer += 8;
  }
  else
  {
    memcpy( pBuffer, &g_unNickname, 2);                                           // little endian         
    pBuffer += 2;
  }
  // DLPDU Spec
//  *(pBuffer++) = (DLL_PACKET_PRIORITY_NORMAL << 4) | (ucLongSrcAddr ? 0 : (DLL_KEY_NETW << 3)) | (DLL_PACKET_TYPE_DISCONNECT);  
  *(pBuffer++) = (DLL_PACKET_PRIORITY_COMMAND << 4) | (ucLongSrcAddr ? 0 : (DLL_KEY_NETW << 3)) | (DLL_PACKET_TYPE_DISCONNECT);  
  // DLPDU Payload = field Empty
  ;
      
  // COMPUTE NONCE 
  DLL_ComputeSpecialNonce();  
  // ENCRYPTION
  AES_Crypt_Intr( 
                  ucLongSrcAddr ? c_aucWKKey : g_aucNetworkKey,   // pKey
                  stAligned.m_aucNonce,               // pNonce
                  g_aucDllBuffer,                     // pToAuthOnly
                  pBuffer - g_aucDllBuffer,           // nToAuthOnlyLen
                  pBuffer,                            // pToEncrypt
                  0,                                  // nToEncryptLen
                  pBuffer                             // pEncryptedBuff
                 );
  g_ucDllBufferLen = pBuffer - g_aucDllBuffer + 4;    // Add MIC  
  //
  g_sDllSlot.m_ucNeedACK = 0;
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_ComposeAckPkt(uint8 p_ucRespCode)
{
uint8 ucAddrSpec = g_aucDllBuffer[1];
uint8 ucNetworkKey;
uint8 * pAck = g_aucDllAckBuffer;

  *(pAck++) = 0x41;
  *(pAck++) = 0x88 ;
  *(pAck++) = g_aucDllBuffer[2];   // Sequence Number
  *(pAck++) = g_aucDllBuffer[3];   // NetworkId
  *(pAck++) = g_aucDllBuffer[4];   // 
  
  //  invert DestADDR & SourceADDR, and save DLPDU Specifier into ucNetworkKey

  if( (ucAddrSpec & 0x04) == 0) // was Short DestAddr in the received packet
  {
    if ( (ucAddrSpec & 0x40) == 0 ) 
    { // both addresses are short
      *(pAck++) = g_aucDllBuffer[7];
      *(pAck++) = g_aucDllBuffer[8];
      
      ucNetworkKey = g_aucDllBuffer[9]; 
    }
    else // (ucAddrSpec & 0x40) != 0 
    { // SrcAddr=long, DstAddr=short
      g_aucDllAckBuffer[1] = (0x88 | 0x04);
      memcpy( pAck, g_aucDllBuffer+7, 8);
      pAck += 8;
          
      ucNetworkKey = g_aucDllBuffer[15]; 
    }
    
    *(pAck++) = g_aucDllBuffer[5];
    *(pAck++) = g_aucDllBuffer[6];
      
    // build NONCE
    memset( stAligned.m_aucNonce + 5, 0, 6 ); 
    stAligned.m_aucNonce[11] = g_aucDllBuffer[6]; // 
    stAligned.m_aucNonce[12] = g_aucDllBuffer[5]; // LSB
  }
  else  // (ucAddrSpec & 0x04) != 0  // was Long DestADDR
  {    
    if( ucAddrSpec & 0x40 ) 
    { // both addresses are long
      g_aucDllAckBuffer[1] = (0x88 | 0x04);
      memcpy( pAck, g_aucDllBuffer+13, 8);
      pAck += 8;
      
      ucNetworkKey = g_aucDllBuffer[21];
    }
    else
    { // SrcAddr=short, DstAddr=long 
      *(pAck++) = g_aucDllBuffer[13];
      *(pAck++) = g_aucDllBuffer[14]; 
      
      ucNetworkKey = g_aucDllBuffer[15];
    }
    
    if( g_unNickname != 0xFFFF )
    {
      *(pAck++) = g_unNickname;
      *(pAck++) = g_unNickname >> 8;
//      pAck += 2;
      
      // build NONCE
      memset( stAligned.m_aucNonce + 5, 0, 6 ); 
      stAligned.m_aucNonce[11] = g_unNickname >> 8; // 
      stAligned.m_aucNonce[12] = g_unNickname; // LSB
    }
    else
    {  
      g_aucDllAckBuffer[1] |= 0x40;
      memcpy( pAck, g_aJoinDevUID_LE, 5 ); 
      memcpy( pAck + 5, c_aHCF_OUI_LE, 3 );       
      pAck += 8; 
      
      // build NONCE
      memcpy( stAligned.m_aucNonce + 5, c_aHCF_OUI_BE, 3 ); 
      memcpy( stAligned.m_aucNonce + 8, g_aJoinDevUID_BE, 5 ); 
    } 
  }

  // DLPDU Specifier
  // change Packet Type to ACK
  *(pAck++) = ucNetworkKey & 0x38; // DLL_PACKET_TYPE_ACK = 0  last 3 bits
  
  // DLPDU Payload 
  *(pAck++) = p_ucRespCode;
  
  // Time Drift
  *(pAck++) = (uint8) (g_nDllClkDiffAck >> 8 ); 
  *(pAck++) = (uint8) (g_nDllClkDiffAck); 

  // COMPUTE NONCE 
  DLL_GetASN_Intr( stAligned.m_aucNonce );
  
  g_ucDllAckBufferLen = pAck - g_aucDllAckBuffer;
  
  // ENCRYPTION
  AES_Crypt_Intr( 
  
              ( ucNetworkKey & 0x08 ? g_aucNetworkKey : c_aucWKKey ), // pKey // Mask for Encryption Key ... in DLPDU Specifier  
                stAligned.m_aucNonce,                                 // pNonce
                g_aucDllAckBuffer,                                    // pToAuthOnly
                g_ucDllAckBufferLen,                                  // nToAuthOnlyLen
                g_aucDllAckBuffer + g_ucDllAckBufferLen,              // pToEncrypt
                0,                                                    // nToEncryptLen
                g_aucDllAckBuffer + g_ucDllAckBufferLen               // pEncryptedBuff
                );
  
  g_ucDllAckBufferLen += 4;  
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_PrepareOutgoingPkt(void)
{
DLL_PACKET_T * pQueue = g_sDllSlot.m_pQueue;
uint8 * pBuffer = g_aucDllBuffer;    
      
  // frame control
  *(pBuffer++) = DLL_FRAME_CONTROL;
  // address specifier 
  *(pBuffer++) = ( 
                  0x88 | 
                  ( pQueue->m_stAddrSpec.m_bSourceLongAddr ? 0x40 : 0x00 ) |
                  ( pQueue->m_stAddrSpec.m_bDestLongAddr ? 0x04 : 0x00 )
                );
  // SQN
  *(pBuffer++) = g_sDllASN.m_aucBytes[0];          
  // NetID
  memcpy( pBuffer, &g_unNetworkId, 2);                                        // little endian     
  pBuffer+=2;
  
  // Dest ADDR
  if ( pQueue->m_stAddrSpec.m_bDestLongAddr )
  {    
    memcpy( pBuffer, pQueue->m_aDestAddr, 8);
    g_sDllSlot.m_ucNeedACK = 1; // 
    pBuffer+=8;    
  }
  else
  {
    memcpy( pBuffer, pQueue->m_aDestAddr, 2);            
    g_sDllSlot.m_ucNeedACK = ( (((*pBuffer) & (*(pBuffer+1))) == 0xFF) ? 0 : 1 );
    pBuffer+=2;
  }    
  // Source ADDR
  if ( pQueue->m_stAddrSpec.m_bSourceLongAddr )
  { 
    memcpy( pBuffer, pQueue->m_aSourceAddr, 8);    
    pBuffer += 8;
  }
  else
  {
    memcpy( pBuffer, pQueue->m_aSourceAddr, 2);        
    pBuffer += 2;
  }    
  // DLPDU Spec
  *(pBuffer++) = ( 
                  ((0x03 & pQueue->m_stDLDPUSpec.m_bPriority) << 4) | 
                  ((0x01 & pQueue->m_stDLDPUSpec.m_bEncryptKey) << 3) |
                  ( 0x07 & pQueue->m_stDLDPUSpec.m_bPacketType) 
                );        
  // DLPDU Payload     
  memcpy( pBuffer, pQueue->m_aucDllPayload, pQueue->m_ucDllPayloadLen);
  pBuffer += pQueue->m_ucDllPayloadLen;      
  
  // COMPUTE NOUNCE 
  //memcpy( stAligned.m_aucNonce, g_aDllASN, 5);
  DLL_GetASN_Intr( stAligned.m_aucNonce );  
  if ( pQueue->m_stAddrSpec.m_bSourceLongAddr )
  { // ATENTIE : !!! SrcAddr este LE ... in nounce trebuie BE ... deci trebuie copiata invers                 
    stAligned.m_aucNonce[5] = pQueue->m_aSourceAddr[7]; // MSB
    stAligned.m_aucNonce[6] = pQueue->m_aSourceAddr[6];
    stAligned.m_aucNonce[7] = pQueue->m_aSourceAddr[5];
    stAligned.m_aucNonce[8] = pQueue->m_aSourceAddr[4];
    stAligned.m_aucNonce[9] = pQueue->m_aSourceAddr[3];
    stAligned.m_aucNonce[10] = pQueue->m_aSourceAddr[2];
  }
  else
  { // ATENTIE : !!! SrcAddr este LE ... in nounce trebuie BE ... deci trebuie copiata invers        
    memset(&stAligned.m_aucNonce[5], 0, 6);    
  }
  stAligned.m_aucNonce[11] = pQueue->m_aSourceAddr[1]; 
  stAligned.m_aucNonce[12] = pQueue->m_aSourceAddr[0]; // LSB   
  // ENCRYPTION
  AES_Crypt_Intr( 
                  (pQueue->m_stDLDPUSpec.m_bEncryptKey == DLL_KEY_WELL_KNOWN ? c_aucWKKey : g_aucNetworkKey ), // pKey
                  stAligned.m_aucNonce,               // pNonce
                  g_aucDllBuffer,                     // pToAuthOnly
                  pBuffer - g_aucDllBuffer,           // nToAuthOnlyLen
                  pBuffer,                            // pToEncrypt
                  0,                                  // nToEncryptLen
                  pBuffer                             // pEncryptedBuff
                 );
  
  g_ucDllBufferLen = pBuffer - g_aucDllBuffer + 4;
//  pQueue->m_bNoRetries--; // will be done by DLL_Timeslot_Start(), only if the packet is actually transmitted
} 


////////////////////////////////////////////////////////////////////////////////
//////                    AUX SECTION                                    ///////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_CheckAndSetChannelMap(uint8 p_ucMode, uint8 p_ucTotalChannels, uint16 p_unChannelMap)
{
  if(p_ucMode)
  { // just check the matching between the received values and the actual values
    if ( p_ucTotalChannels != g_ucDllTotalChannels || p_unChannelMap != g_unDllChannelMap )
    {
      return RCM_E65_IllegalFrequencyChannelBits; // error channel map doesn't match
    }
  }
  else
  { // check the error conditions and set ChMap variables to be used by DLL
    if ( /*( p_ucTotalChannels <= 8 ) ||*/ ( p_ucTotalChannels > 16 ) )
    {
      return RCS_E03_PassedParameterTooLarge; // channel map length not supported
    }
//    if ( !p_ucTotalChannels || !p_unChannelMap || p_unChannelMap & 0x8000 )
    if ( p_ucTotalChannels <= 8 || !p_unChannelMap || p_unChannelMap & 0x8000 )
    {
      return RCM_E65_IllegalFrequencyChannelBits; // ch16 not supported by 802.15.4 PHY -see Cmd818 description
    }
    g_ucDllTotalChannels = p_ucTotalChannels;
    g_unDllChannelMap = p_unChannelMap; 
    g_ucDllActiveChannels = 0;
    
    uint16 unMask = 0x0001;
    uint8 i;
    for(i = 0; i < p_ucTotalChannels; i++, unMask <<= 1)
    {
      if ( unMask & p_unChannelMap )
      {
        g_aDllChTable[g_ucDllActiveChannels++] = i;
      }
    }
//    memset(g_aDllChTable + g_ucDllActiveChannels, 0x00, sizeof(g_aDllChTable) - g_ucDllActiveChannels); // fill in the rest of the table with a harmless value
    memset(g_aDllChTable + g_ucDllActiveChannels, 0x0F, sizeof(g_aDllChTable) - g_ucDllActiveChannels); // fill in the rest of the table with a harmless value
  }
  return RCS_N00_Success;  
}

////////////////////////////////////////////////////////////////////////////////
// Note : p_ucActiveChannel must be = [0 , g_ucDllActiveChannels) 
////////////////////////////////////////////////////////////////////////////////
//uint8 DLL_GetChannel(uint8 p_ucActiveChannel)
//{
//uint16 i = 0x0001;
//uint8  ucChannel = 0xFF;
//  
//  p_ucActiveChannel %= g_ucDllActiveChannels; // ??? ok
//  while ( i )
//  {
//    ucChannel++;
//    if ( i & g_unDllChannelMap )
//    {
//      if ( ! p_ucActiveChannel ) 
//      {
//        break;
//      }
//      p_ucActiveChannel--;
//    }
//    i <<= 1;
//  }
//  return ucChannel;
//}

////////////////////////////////////////////////////////////////////////////////
// p_pucAddr is an unaligned address
////////////////////////////////////////////////////////////////////////////////
uint16 IntFromBE( uint8 * p_pucAddr )
{
  return ( 0xFF00 & ( *(p_pucAddr) << 8 ) | ( 0x00FF & *(p_pucAddr+1) ) );  
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
uint16 IntFromLE( uint8 * p_pucAddr )
{
  return ( 0xFF00 & ( *(p_pucAddr+1) << 8 ) | ( 0x00FF & *(p_pucAddr) ) );  
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
//void DLL_GetASN(uint8 * p_pucBuff)
//{
//  MONITOR_ENTER();
//  *(p_pucBuff++) = g_sDllASN.m_aucBytes[4]; // MSB
//  *(p_pucBuff++) = g_sDllASN.m_aucBytes[3];
//  *(p_pucBuff++) = g_sDllASN.m_aucBytes[2];
//  *(p_pucBuff++) = g_sDllASN.m_aucBytes[1];
//  *(p_pucBuff)   = g_sDllASN.m_aucBytes[0]; // LSB
//  MONITOR_EXIT();
//}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void DLL_Get_5Bytes_BE_ASN(uint8 * p_pucBuff, uint8 p_ucUserFlag)
{
//.uint8 bInterruptEnabled =  IS_ITC_INT_ENABLE(TMR_INT);  
uint8 * pTemp = &g_sDllASN.m_aucBytes[4];
unsigned int ulIrqStatus;

//.  if ( ( p_ucUserFlag ) && ( bInterruptEnabled ) )
//.  {    
//.    DISABLE_DLL_TMRIRQ();
//.  }
  if ( p_ucUserFlag )
  {    
    ulIrqStatus = IntDisableIRQ();  // MONITOR_ENTER()
  }
  *(p_pucBuff++) = *(pTemp--); // g_sDllASN.m_aucBytes[4]; // MSB
  *(p_pucBuff++) = *(pTemp--); // g_sDllASN.m_aucBytes[3];
  *(p_pucBuff++) = *(pTemp--); // g_sDllASN.m_aucBytes[2];
  *(p_pucBuff++) = *(pTemp--); // g_sDllASN.m_aucBytes[1];
  *(p_pucBuff)   = *(pTemp);   // g_sDllASN.m_aucBytes[0]; // LSB
//.  if ( ( p_ucUserFlag ) && ( bInterruptEnabled ) )
//.  {
//.    ENABLE_DLL_TMRIRQ();  
//.  }
  if ( p_ucUserFlag )
  {    
    IntRestoreIRQ(ulIrqStatus); // MONITOR_EXIT()
  }
}

#ifdef BACKBONE_SUPPORT
////////////////////////////////////////////////////////////////////////////////
// p_ulSec sec * 100 slots/sec = ( p_ulSec * 100 ) slots
// 100 = 0b 0110 0100
////////////////////////////////////////////////////////////////////////////////
void DLL_SetTAItoASN( uint32 p_ulSec, uint32 p_ulUSec )
{
static uint8 s_ucPrevCorrection = 0;

  switch ( g_ucDllStatus )                                                      // ??? cum va lucra DLL pe transceiver 
  {  
  case DLL_ACTIVE_SEARCH:
    {
      uint16 unClkCorr = (10000 - (p_ulUSec % 10000)) * 6;
      g_sDllASN.m_ullASN = (unsigned long long) p_ulSec*100 + p_ulUSec/10000;      
      g_stDllPredSlot.m_uc250msSlotNo = g_sDllASN.m_ullASN % MAX_250MS_SLOT_NO;
      DLL_TIMER_STOP();
      DLL_TIMER_SET_CNTR( unClkCorr );                                          // clock correction
      DLL_TIMER_START();  
      s_ucPrevCorrection = 1;
      PUT_DLL_SYNCHRONIZED();
    }
    break;
  case DLL_PASSIVE_SEARCH:  
  case DLL_SYNCHRONIZED: 
  case DLL_JOINED: 
    {
      unsigned long long ullASN = (unsigned long long) p_ulSec*100 + p_ulUSec/10000;  
      uint16 unTimeSteps = (10000 - (p_ulUSec % 10000)) * 6;
      s_ucPrevCorrection <<= 1;
      if( g_sDllASN.m_ullASN > ullASN ) 
      { 
        s_ucPrevCorrection |= 1;                                                // negative correction needed
      }
      else if( g_sDllASN.m_ullASN == ullASN ) 
      {
        if( TMR0_CNTR > unTimeSteps ) 
        {
          s_ucPrevCorrection |= 1;                                              // negative correction needed  
        }
      }      
      // correction performed when 8 consecutive in one direction
      if( s_ucPrevCorrection == 0 ) // CORECT                                   // ... // if( (0x0F & s_ucPrevCorrection) == 0 )
      { // positive correction needed
        //RtcTmr_Set250msCorrection(+MAX_BBR_CORRECTION);
        // Debug pulse
      }
      else if( s_ucPrevCorrection == 0xFF ) // CORECT                           // ... // else if( (0x0F & s_ucPrevCorrection) == 0x0F ) // !!! 
      { // negative correction needed  
        //RtcTmr_Set250msCorrection(-MAX_BBR_CORRECTION);
        // Debug pulse
      }
    }
    break;
  } 
  DEBUG4_ON();  // debug signal 
  DEBUG4_OFF();
}
#endif // BACKBONE_SUPPORT

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


uint16 GetSlotDiff( uint16 p_unLinkSlot, uint16 p_unCrtSfOffset, uint16 p_unNumSfSlots)
{
uint16 unDiff;

  if ( p_unCrtSfOffset < p_unLinkSlot )
  {
    unDiff = p_unLinkSlot - p_unCrtSfOffset;

  }
  else
  {
    unDiff = p_unLinkSlot - p_unCrtSfOffset +  p_unNumSfSlots;            
  }
  return unDiff;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_Predictive_Schedule_Link(void)
{       
DLL_HASH_LINKS_STRUCT_T   * pHashLink = g_aDllHashLinks;
DLL_HASH_LINKS_STRUCT_T   * pEndHash = g_aDllHashLinks+g_ucDllHashLinksSize;
DLL_LINK_T                * pLink;
DLL_SUPERFRAME_T          * pSf;

  g_stDllPredLink.m_unSlotsUntilWakeUp = 0xFFFF;
  
  for( ; pHashLink < pEndHash; pHashLink++ )
  {  
    pLink     = &g_aDllLinks[pHashLink->m_ucLinkIdx];
    pSf       = &g_aDllSuperframes[pHashLink->m_ucSuperframeIdx];
              
    uint16 unDiff = GetSlotDiff(pLink->m_unSlot,pSf->m_unCrtOffset,pSf->m_unNumSlots);
    if( unDiff > g_stDllPredLink.m_unSlotsUntilWakeUp )
        continue;
    
    if ( pLink->m_bRx ) // Rx 
    { 
      g_stDllPredLink.m_unSlotsUntilWakeUp = unDiff;
    }        
    else if ( pLink->m_bTx )
    { // Tx or TxShared                      
      if ( g_ucDllCrtQueueSize ) // daca am cel putin un packet in coada de mesaje        
      {
        g_stDllPredLink.m_unSlotsUntilWakeUp = unDiff;
      }
      else
      {
        // Check Discovery Timeout
        if ( (pLink->m_eLinkType == DLL_LINK_TYPE_DISCOVERY) 
              && (g_ucNetState == NET_STATE_OPERATIONAL || g_ucNetState == NET_STATE_QUARANTINED)
            )
        {
            if ( g_ucDllDiscoveryTX || (g_ulDllDiscoveryCntr <= unDiff) )
            {
              g_stDllPredLink.m_unSlotsUntilWakeUp = unDiff;
              continue;
            } 
            
             if( g_ulDllDiscoveryCntr < g_stDllPredLink.m_unSlotsUntilWakeUp ) // false wakeup but at that momment will recompute correct diff
             {
                g_stDllPredLink.m_unSlotsUntilWakeUp = g_ulDllDiscoveryCntr;
             }            
        }
          
        // Check Advertise Timeout       
        if ( ( ! pLink->m_bShared ) && ( g_ucNetState == NET_STATE_OPERATIONAL ) )
        { // send ADV packets only in NET_STATE_OPERATIONAL
          if ( g_ucDllAdvertiseTX || (g_ulDllAdvCntr <= unDiff) )
          {
            g_stDllPredLink.m_unSlotsUntilWakeUp = unDiff;
            continue;
          }          
           if( g_ulDllAdvCntr < g_stDllPredLink.m_unSlotsUntilWakeUp ) // false wakeup but at that momment will recompute correct diff
           {
              g_stDllPredLink.m_unSlotsUntilWakeUp = g_ulDllAdvCntr;
           }            
        }
          
        // Check KeepAlive Timeout - here to the end, to give Advertise a chance to be transmitted when has one single non-shared link
        if (  ( pHashLink->m_ucNeighborIdx != DLL_INVALID_NEIGH_IDX ) 
            &&(  pLink->m_eLinkType == DLL_LINK_TYPE_NORMAL 
               || ( (g_ucNetState == NET_STATE_JOINING) && (pLink->m_eLinkType == DLL_LINK_TYPE_JOIN) )  ) 
           )
        {                      
            if( (int32)(  g_aDllNeighbors[pHashLink->m_ucNeighborIdx].m_tmLastComm 
                         - (g_sDllASN.m_ulLsb + unDiff)  // ASN in the future
                         + g_stProv.ulDllKeepAlive) <= 0 )
            {
              if ( unDiff < g_stDllPredLink.m_unSlotsUntilWakeUp )
              {
                g_stDllPredLink.m_unSlotsUntilWakeUp = unDiff;
              }
            }  
        }          
      }
    }
  }
  
  //if ( g_stDllPredLink.m_unSlotsUntilWakeUp >= 25 )
  //{
  //  g_stDllPredLink.m_unSlotsUntilWakeUp = 25;
  //}
}

void DLL_ComputeSuperframeOffset()
{
  DLL_SUPERFRAME_T * pSf = g_aDllSuperframes;    
  while ( pSf < ( g_aDllSuperframes + g_ucDllCrtSuperframesSize ) )
  {
    if ( pSf->m_eActive == DLL_SF_ENABLED )
    { 
      pSf->m_unCrtOffset = g_sDllASN.m_ullASN % pSf->m_unNumSlots;
    }
    pSf++;
  }  
}

#ifdef SEND_FICTIVE_ACK
///////////////////////////////////////////////////////////////////////////////////
// Name:          DLL_SendFictiveACK
// Author:        Marius Vilvoi
// Description:   send an imaginary ACK when RX slot and soft timer exceeded  
// Parameters:    none
// Return:        none
// Obs:
//      Access level: Interrupt level
//      Context:
///////////////////////////////////////////////////////////////////////////////////
void DLL_SendFictiveACK(void)
{
  if( (g_sDllSlot.m_pLink->m_eLinkType == LinkType_Join) || (g_sDllSlot.m_pLink->m_eLinkType == LinkType_Normal) )
  { // only for normal/join link
       
    uint8 * pAck = g_aucDllAckBuffer; // all header in little endian
  
    // compose ACK  
    *(pAck++) = 0x41;
    
    // address specifier
    if( 0xFFFF == g_unNickname )
      *(pAck++) = 0xC8;  // long src addr, short dst addr
    else
      *(pAck++) = 0x88;  // short src addr, short dst addr 
    
    // sequence number
    *(pAck++) = g_sDllASN.m_aucBytes[0];  
    
    // NetID
    memcpy(pAck, &g_unNetworkId, 2);    
    pAck+=2; 
    
    // Dst ADDR
    *(pAck++) = g_sDllSlot.m_pLink->m_unNeighborShortAddr;     
    *(pAck++) = g_sDllSlot.m_pLink->m_unNeighborShortAddr >> 8; 
    
    // Src ADDR  
    if( 0xFFFF == g_unNickname )
    { 
      memcpy( pAck, g_aJoinDevUID_LE, 5 ); 
      memcpy( pAck + 5, c_aHCF_OUI_LE, 3 );       
      pAck += 8; 
    }
    else
    {  
      memcpy( pAck, &g_unNickname, 2);       
      pAck += 2;
    }
    
    // DLPDU specifier
    if( 0xFFFF == g_unNickname )  
      *(pAck++) = 0x00;  // well known key
    else
      *(pAck++) = 0x08;  // network key 
    
    
    // fictive DLPDU Payload 
    
    // fictive RC = Superframe Id | Link Type
    *(pAck++) = (g_sDllSlot.m_pLink->m_ucSuperframeID << 4) | g_sDllSlot.m_pLink->m_eLinkType;
    
    // fictive Time Drift = 0xABCD
    *(pAck++) = 0xAB;  
    *(pAck++) = 0xCD;   
    
    // added for Andy: slot number + channel offset
    *(pAck++) = g_sDllSlot.m_pLink->m_unSlot >> 8;     
    *(pAck++) = g_sDllSlot.m_pLink->m_unSlot;
    *(pAck++) = g_sDllSlot.m_pLink->m_ucChannelOffset;
      
    DLL_ComputeSpecialNonce();
    
    g_ucDllAckBufferLen = pAck - g_aucDllAckBuffer;
    // ENCRYPTION
    AES_Crypt_Intr( 
                    ( (0xFFFF != g_unNickname) ? g_aucNetworkKey : c_aucWKKey ),    // pKey
                    stAligned.m_aucNonce,                                           // pNonce
                    g_aucDllAckBuffer,                                              // pToAuthOnly
                    g_ucDllAckBufferLen,                                            // nToAuthOnlyLen
                    g_aucDllAckBuffer + g_ucDllAckBufferLen,                        // pToEncrypt
                    0,                                                              // nToEncryptLen
                    g_aucDllAckBuffer + g_ucDllAckBufferLen                         // pEncryptedBuff
                 );
    
    g_ucDllAckBufferLen += 4;  
       
    
    // put the DLL state machine in idle
    g_ucDllXVRStatus = DLL_XVR_IDLE;
    
    // send fictive ACK(always at 8ms from the beginning of the Rx slot)
    uint16 unDelay = 8000 - (DLL_TIMER_CNTR / 6); // subtract no of us from beginning of slot 
    PHY_TX_Request(g_sDllSlot.m_ucChannel, unDelay, 0, g_aucDllAckBuffer, g_ucDllAckBufferLen); // + 2 pentru CRC
  }  
}  
#endif







