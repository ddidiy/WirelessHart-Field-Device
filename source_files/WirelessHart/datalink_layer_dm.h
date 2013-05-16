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
* Name:         datalink_layer_dm.h
* Author:       NIVIS LLC, 
* Date:         July 2008
* Description:  Data Link Layer Management
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_DLL_DM_H_
#define _NIVIS_DLL_DM_H_

#include "../typedef.h"
#include "tmr_util.h"

#define TIME_SOURCE_TIMEOUT   (185UL*32768)    // 185s [RTC units]

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////


extern uint8                  g_aucNetworkKey[AES_KEY_LENGTH];
extern uint16                 g_unNetworkId;

extern uint8 g_ucDllDisconnect;

// TDMA Dinamic Status
extern DLL_STATUS_T           g_ucDllStatus;
extern DLL_XVR_STATUS_T       g_ucDllXVRStatus;
extern DLL_FLAGS_T            g_sDllFlags;
extern DLL_SLOT_PARAMS_T      g_sDllSlot;
extern DLL_SHARED_PARAMS_T    g_sDllSharedParams;
extern volatile int16         g_nDllClkDiffAck;
extern uint8                  g_ucDllAlarmsCounter;  

// ASN 
extern DLL_ASN_T g_sDllASN;
// Channel Map
extern uint16  g_unDllChannelMap;                                                      // in memorie : little endian ! Bit 15 = Channel 15 Bit 0 = Channel 0
extern uint8   g_ucDllActiveChannels;                                                // number of active channels
extern uint8   g_ucDllTotalChannels;                                                 // number of total channels
extern uint8   g_aDllChTable[16];                                    // equivalence table between the index of an active channel and it's position into the channel map array
// Data Buffers for DLL 
extern uint8                   g_aucDllBuffer[DLL_MAX_DLPDU_SIZE];                     // used between DLL and PHY
extern uint8                   g_ucDllBufferLen;
extern uint8                   g_aucDllAckBuffer[DLL_MAX_ACK_SIZE];                    // used between DLL and PHY
extern uint8                   g_ucDllAckBufferLen;
// Tables
extern DLL_NEIGHBOR_T    g_aDllNeighbors[DLL_MAX_NEIGHBORS];
extern uint8             g_ucDllLkdNeighborsSize;
extern uint8             g_ucDllAllNeighborsSize;
extern DLL_SUPERFRAME_T  g_aDllSuperframes[DLL_MAX_SUPERFRAMES + DLL_HIDDEN_SUPERFRAMES];
extern uint8             g_ucDllCrtSuperframesSize;
extern uint8             g_ucDllAllSuperframesSize;
extern uint8             g_ucDllDeletedSuperframes;
extern DLL_LINK_T        g_aDllLinks[DLL_MAX_LINKS + DLL_HIDDEN_LINKS];
extern uint8             g_ucDllCrtLinksSize;
extern uint8             g_ucDllAllLinksSize;
extern uint8             g_ucDllDeletedLinks;
extern DLL_CONNECTION_T  g_aDllConnections[DLL_MAX_CONNECTIONS + DLL_HIDDEN_CONNECTIONS];
extern uint8             g_ucDllCrtConnectionsSize;
extern uint8             g_ucDllAllConnectionsSize;
extern uint8             g_ucDllDeletedConnections;
extern DLL_PACKET_T      g_aDllQueue[DLL_MAX_PACKET_BUFFERS];
extern uint8             g_ucDllCrtQueueSize;
extern uint8             g_ucDllAllQueueSize;
// Hash
extern struct
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

extern uint8                     g_ucDllHashLinksSize;
extern uint8                     g_ucDllHashGraphsSize;
// Properties 
extern uint8   g_ucDllPriorityThreshold;
extern uint8   g_ucDllJoinPriority;
extern uint8   g_ucDllMaxBackoffExponent;
// Active Search / Syncronized counter 
extern uint32  g_ulDllRecCntr;
extern uint32  g_ulDllMaxRecCntr;
//
extern DLL_BUFFER_PARSED_T     g_sDllBufferParsed;
extern uint8                   g_ucDllAdvertiseTX;
extern uint8                   g_ucDllDiscoveryTX;
//
extern PRED_SLOT g_stDllPredSlot;
extern PRED_LNK g_stDllPredLink;
//
//#ifndef BACKBONE_SUPPORT 
//extern uint8 g_ucConsecutiveNoAckCtr;
//#endif

//#define CAN_MODIFY_DLL_STRUCTS()      ( g_sDllFlags.m_ucStructs == 0 )
//#define REQUEST_MODIFY_DLL_STRUCTS()  { \
//                                      while(g_sDllFlags.m_ucStructs) \
//                                                  ; \
//                                      }
#define COMMIT_MODIFY_DLL_STRUCTS()   { g_sDllFlags.m_ucStructs = 1; }

#define COMMIT_MODIFY_DLL_QUEUE()     { g_sDllFlags.m_ucQueue = 1; }

#define COMMIT_MODIFY_DLL_DISCONNECT()     { g_sDllFlags.m_ucDisconnect = 1; }

#define COMMIT_MODIFY_DLL_JLINKS()    { g_sDllFlags.m_ucStructs = 3; }  // change addr of the self-created Join links into 0xFFFF

#define PUT_DLL_IDLE()                { g_ucDllStatus = DLL_IDLE; g_unNetworkId = g_stProv.unNetworkId; }
#define PUT_DLL_ACTIVE_SEARCH()       { g_ucDllStatus = DLL_ACTIVE_SEARCH; g_ulDllRecCntr = 0; \
                                        g_stC769.wirelessMode = WirelessOperationMode_ActiveSearch; \
                                        g_stC769.joinStatus = 0; }
#define PUT_DLL_PASSIVE_SEARCH()      { g_ucDllStatus = DLL_PASSIVE_SEARCH; g_ulDllRecCntr = 0; \
                                        g_stC769.wirelessMode = WirelessOperationMode_DeepSleep_UltraLowPower_PassiveSearch; \
                                        g_stC769.joinStatus = JoinProcessStatusMask_JoinFailed; }
#define PUT_DLL_SYNCHRONIZED()        { g_ucDllStatus = DLL_SYNCHRONIZED; g_ulDllRecCntr = 0; }
#define PUT_DLL_JOINED()              { g_ucDllStatus = DLL_JOINED; g_ulTimeSourceTimer = RTC_COUNT + TIME_SOURCE_TIMEOUT; }

//WRONG: #define DISCONNECT_INTERVAL USEC_TO_FRACTION2(2000UL*10000) //20s 
#define DISCONNECT_INTERVAL     SEC_TO_FRACTION2(20) //20s 

#define MAX_CONSECUTIVE_NO_ACKS   20    // max number of consecutive tx packets that have not received an ack

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_IncrementASN(void);
void DLL_Init(void);
void DLL_Join(void);
void DLL_Timeslot_Start(void);
void DLL_Configure(void);
void DLL_Schedule_Link(void);
void DLL_CheckOtherTimeouts(void);
void DLL_SetTAItoASN( uint32 p_ulSec, uint32 p_ulUSec );
void DLL_Get_5Bytes_BE_ASN(uint8 * p_pucBuff, uint8 p_ucFlag);
#define DLL_GetASN(p_pucBuff)         DLL_Get_5Bytes_BE_ASN(p_pucBuff, 1)
#define DLL_GetASN_Intr(p_pucBuff)    DLL_Get_5Bytes_BE_ASN(p_pucBuff, 0)

// Neighbors     
uint8 DLL_FindShortNeighbor( uint16 p_unAddr );
uint8 DLL_AddShortNeighbor( uint16 p_unAddr );       
void DLL_RebuildNeighbors();
// Superframes
uint8 DLL_FindSuperframe( uint8 p_ucSuperframeID );
//DLL_SUPERFRAME_T * DLL_FindSuperframe( uint8 p_ucSuperframeID );
uint8 DLL_AddSuperframe( uint8 p_ucSuperframeID, uint8 p_ucSuperframeMode, uint16 p_unNumSlots );
void  DLL_ModifySuperframe( uint8 p_ucIndex, uint16 p_unNumSlots);
void  DLL_SetSuperframe( uint8 p_ucIndex, DLL_SF_STATUS_T p_eActive);
void  DLL_DeleteSuperframe( uint8 p_ucIndex );
// Links
uint8 DLL_FindLink( uint8 p_ucSuperframeID, uint16 p_unNeighborShortAddr, uint16 p_unSlot );
uint8 DLL_AddLink( uint8 p_ucSuperframeID, 
                   uint16 p_unNeighborShortAddr, 
                   uint16 p_unSlot, 
                   uint8 p_ucChOffset, 
                   uint8 p_ucTx,
                   uint8 p_ucRx,    
                   uint8 p_ucShared,
                   DLL_LINK_TYPE_T p_eLinkType );
void DLL_DeleteLink( uint8 p_ucIndex );
// Connections
uint16 DLL_FindConnectionGraph( uint16 p_unNeighborShortAddr );
uint8 DLL_FindConnection( uint16 p_unGraphID, uint16 p_unNeighborShortAddr);
uint8 DLL_FindFirstGraphIdx( uint16 p_unGraphID);
uint8 DLL_AddConnection( uint16 p_unGraphID, uint16 p_unNeighborShortAddr);
//uint8 DLL_DeleteConnection( uint8 p_ucIndex );
void  DLL_RebuildConnections(void);
// Graphs
void DLL_RebuildHashGraphs(void);
// Hash
void DLL_RebuildHashLinks(void);
// Packets
uint8 DLL_AddPktToQueue(const DLL_PACKET_T * p_pMsg);
void DLL_RemCrtPktFromQueue(void);
//uint8 DLL_ReqCrtPktFromQueue(uint8 p_ucIndex);
DLL_PACKET_T * DLL_ReqCrtPktFromQueue(DLL_HASH_LINKS_STRUCT_T * p_pHashLink);
// Parsing Incomings
uint8 DLL_AuthIncomingPkt();
uint8 DLL_ParseIncomingPkt();
uint8 DLL_ParseAdvertisePkt(uint8 ucNeighborIndex);
// Computing
//void DLL_ComputeAdvertisePkt(uint8 p_ucIndex);
//void DLL_ComputeKeepAlivePkt(uint8 p_ucIndex);
void DLL_ComputeAdvertisePkt(DLL_NEIGHBOR_T * p_pNeighbor);
void DLL_ComputeKeepAlivePkt(DLL_NEIGHBOR_T * p_pNeighbor);
void DLL_ComputeDisconnectPkt();
// Outgoings
void DLL_ComposeAckPkt(uint8 p_ucRespCode);
void DLL_PrepareOutgoingPkt(void);

uint8 NET_ParseProxyInfo(uint16 unNeighShortAddr, uint8* pMsg);

void DLL_ComputeSuperframeOffset();

////////////////////////////////////////////////////////////////////////////////
//////                    AUX SECTION                                    ///////
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_CheckAndSetChannelMap(uint8 p_ucMode, uint8 p_ucTotalChannels, uint16 p_unChannelMap);
//uint8 DLL_GetChannel(uint8 p_ucActiveChannel);
#define DLL_GetChannel(ch) ( g_aDllChTable[((uint8)(ch)) % g_ucDllActiveChannels] )
uint16 IntFromBE( uint8 * p_pucAddr );
uint16 IntFromLE( uint8 * p_pucAddr );

void DLL_RD_Init(); // doar pentru integrare

#endif  // _NIVIS_DLL_DM_H_
