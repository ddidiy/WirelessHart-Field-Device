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
* Name:         network_layer_dm.h
* Author:       Nivis LLC
* Date:         July 2008
* Description:  Network Layer DM (data management)
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_NETWORK_LAYER_DM_H_
#define _NIVIS_NETWORK_LAYER_DM_H_

#include "../typedef.h"
#include "config.h"

#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/WirelessNetworkManagerCommands.h"
#else
  #include "ApplicationLayer/Model/WirelessNetworkManagerCommands.h"
#endif

#define NET_INVALID_IDX               0xFF

#define MAX_SESSIONS_NO               8
#define MAX_CORRESPONDENT_DEVICE_NO   MAX_SESSIONS_NO
#define MAX_TRANSPORT_NO              (2 * MAX_SESSIONS_NO)
#define MAX_ROUTES_NO                 8
#define MAX_SOURCE_ROUTES_NO          2
#define MAX_SERVICES_NO               16

#define JOIN_ROUTE_ID           0x00    // join route id base

#define UNUSED_ENTRY            0x80    // mask for m_ucSessionFlags (session entry), m_ucRetryCount (transport entry)
                                        // and m_ucUsedElem (network queue element)
                                        // 0 -> used entry, 1 -> unused entry

#define FLUSH_PACKET            0x40    // mask for m_ucUsedElem (network queue element) and m_ucRetryCount (transport entry)
                                        // 0 -> nothing tbd, 1 -> clear the packet

#define VALID_OLD_KEY           0x40    // mask for m_ucSessionFlags (session entry)
                                        // 0 -> invalid old session key, 1 -> valid old session key


typedef uint8  SESSION_KEY[16];

enum
{
  NL_SEARCH_BY_SERVICE_ID,
  NL_SEARCH_BY_ROUTE_ID
};

typedef enum
{
  NL_SUCCESS,
  NL_INVALID_PARAMETER,
  NL_OUT_OF_MEMORY,
  NL_APP_DOMAIN_NOT_MATCH,
  NL_NO_ROUTE,
  NL_NO_SOURCE_ROUTE,
  NL_NO_SESSION,
  NL_NO_SERVICE,
  NL_NO_PACKET,
} NL_SC;

typedef enum 
{ 
  NL_RESET, 
  NL_WRITE_SESSION_KEY, 
  NL_DEL_SESSION, 
  NL_ADD_ROUTE, 
  NL_DEL_ROUTE, 
//  NL_DEFAULT_ROUTE,
  NL_ADD_SRC_ROUTE,
  NL_DEL_SRC_ROUTE,
  NL_WRITE_SERVICE,
  NL_READ_PDU_TIMEOUT, 
  NL_WRITE_PDU_TIMEOUT, 
  NL_READ_TTL, 
  NL_WRITE_TTL 
} NL_SV;


// NET_SESSION_ATTRIBUTES
/* field device: first session is the JoinSession !!! */
typedef struct {
  uint32        m_ulPeerNonceCounter;
  uint32        m_ulNonceCounterHistory;
  uint32        m_ulMyNonceCounter;
  uint32        m_ulKeepOldKeyTime;
  SHORT_ADDR    m_unPeerNickname;
  SESSION_KEY   m_aSessionKey;
  SESSION_KEY   m_aOldSessionKey;
  uint8         m_ucSessionType;
  uint8         m_ucSessionFlags; // BIT7 -> session table entry used/unused(0/1); BIT6 -> old session key invalid/valid(0/1)
  uint8         m_ucPacketCounter; // packet counter for un-ACK services, one per session (appears in sequence number from TL Hdr)
  UNIQUE_ID     m_aPeerUniqueID;
} NET_SESSION_ATTRIBUTES;


// NET_TRANSPORT_ATTRIBUTES
/* Every session has two transport entries: even entry for request (m_bMaster = 1) / odd entry for response (m_bMaster = 0) */
typedef struct {   
  uint16          m_unResponseTimer;
  HANDLE          m_hTPDUHandle;
  uint8           m_bActive:1;
  uint8           m_bMaster:1;
  uint8           m_bBroadcast:1;
  uint8           m_ucSequenceNumber:5;
  uint8           m_ucPacketPriority; 
  uint8           m_ucServiceId;
  uint8           m_ucRetryCount;     // if bit 7 is set then this transport table entry is unused
  uint8           m_ucLastPayloadSize;
  uint8           m_aLastPayload[MAX_APDU_SIZE];
} NET_TRANSPORT_ATTRIBUTES;

// NET_ROUTE_ATTRIBUTES
typedef struct {
//  TIME         m_tLastTransmitTime;
  SHORT_ADDR   m_unDestNickname;
  uint16       m_unGraphId;
//  uint16       m_unNumPktsTransitted;
//  uint16       m_unNumPktsReceived;
  uint8        m_ucRouteId;
//  uint8        m_bIsDefault:1;
  uint8        m_bSourceRouteAttached:1; 
//  uint8        m_ucRouteType;
//  UNIQUE_ID    m_aDestUniqueID;
} NET_ROUTE_ATTRIBUTES;

// NET_SOURCE_ROUTE_ATTRIBUTES
typedef struct {
  uint8         m_ucRouteId;
  uint8         m_ucNoOfHops;
  SHORT_ADDR    m_aSrcRouteHops[8];
} NET_SRC_ROUTE_ATTRIBUTES;  

// NET_SERVICE_ATTRIBUTES
typedef struct {   
  _time_t         m_tPeriod;        // 1/32ms
  uint16          m_unPeerNickname;
  uint8           m_ucServiceID;
  uint8           m_ucServiceFlags;
  uint8           m_ucApplicationDomain;
  uint8           m_ucRouteID;
}NET_SERVICE_ATTRIBUTES;


/*
// NET_MNG_WRITE_SESSION_KEY_PARAM
typedef struct {
  //uint8        m_ucSessionId;
  uint8        m_ucSessionType;
  UNIQUE_ID    m_aPeerUniqueID;
  SHORT_ADDR   m_unPeerNickname;
  //  SHORT_ADDR   m_unMyNickName;
  SESSION_KEY  m_aSessionKey;
  uint32       m_ulNonceCounterHistory;
} NET_MNG_WRITE_SESSION_KEY_PARAM;

// NET_MNG_ADD_ROUTE_PARAM
typedef struct {
  uint8        m_ucRouteId;
  uint16       m_unGraphId;
  uint8        m_ucRouteType;
  uint8        m_bIsDefault:1;
  uint8        m_bIsGraphRoute:1; 
  UNIQUE_ID    m_pDestUniqueID;
  SHORT_ADDR   m_unDestNickname;
  SHORT_ADDR   m_aSrcRouteHops[8]; 
} NET_MNG_ADD_ROUTE_PARAM;
*/


// NET_ATTRIBUTES
extern TIME_250MS      g_ulJoinRspTimeout;
extern TIME_10MS       g_ulChannelSearchTime;
extern SHORT_ADDR      g_unNickname;
//extern uint8           g_ucDefaultTTL;
extern uint8           g_ucMaxRetries;
extern uint8           g_ucMaxJoinRetries;

#define         g_ulPassiveCycleTime            DLL_PASSIVE_CYCLE_TIME 
#define         g_ulPassiveWakeTime             DLL_PASSIVE_WAKE_TIME  

extern NET_SESSION_ATTRIBUTES    g_aSessionsTable[MAX_SESSIONS_NO];     // not compacted after deleting an entry
extern NET_TRANSPORT_ATTRIBUTES  g_aTransportTable[MAX_TRANSPORT_NO];   // not compacted after deleting an entry
extern NET_ROUTE_ATTRIBUTES      g_aRoutesTable[MAX_ROUTES_NO];
extern NET_SRC_ROUTE_ATTRIBUTES  g_aSrcRoutesTable[MAX_SOURCE_ROUTES_NO];
extern NET_SERVICE_ATTRIBUTES    g_aServicesTable[MAX_SERVICES_NO];

extern uint8 g_ucSessionsNo;
extern uint8 g_ucTransportTableNo;
extern uint8 g_ucRoutesNo;
extern uint8 g_ucSrcRoutesNo;
extern uint8 g_ucServicesNo;

extern uint8 g_ucOldSessionKeys;

///////////////////////////////////////////////////////////////////////////////////
// Name: NET_Request
// Author: NIVIS LLC
// Description: Handle management requests
// Parameters: 
//         Service =     NL_RESET,
//                       NL_WRITE_SESSION_KEY,
//                       NL_DEL_SESSION,
//                       NL_ADD_ROUTE,
//                       NL_DEL_ROUTE,
//                       NL_DEFAULT_ROUTE,
//                       NL_ADD_SRC_ROUTE,
//                       NL_DEL_SRC_ROUTE,
//                       NL_WRITE_SERVICE,
//                       NL_READ_PDU_TIMEOUT,
//                       NL_WRITE_PDU_TIMEOUT,
//                       NL_READ_TTL,
//                       NL_WRITE_TTL
//         p_pData - pointer to C963_WriteSession_Resp structure,
//                   pointer to C964_DeleteSession_Resp structure,
//                   pointer to C974_WriteRoute_Resp structure,
//                   pointer to C975_DeleteRoute_Resp structure,
//                   pointer to uint8 (RouteID for default route),
//                   pointer to C976_WriteSourceRoute_Resp structure,
//                   pointer to C977_DeleteSourceRoute_Resp structure,
//                   pointer to C973_WriteService_Resp structure,
// Return: None
// Obs:
//      Access level: User level
///////////////////////////////////////////////////////////////////////////////////
NL_SC NET_Request( NL_SV Service, void* p_pData );



void NET_CheckOldSessionKeysTimes();



///////////////////////////////////////////////////////////////////////////////////
// Name: Net_Confirm
///////////////////////////////////////////////////////////////////////////////////
void NET_Confirm( NL_SV Service, NL_SC Status, const void* p_pData );

///////////////////////////////////////////////////////////////////////////////////
// Name: Net_Indication
///////////////////////////////////////////////////////////////////////////////////
void NET_Indication( NL_SV Service, NL_SC Status, const void* p_pData );

///////////////////////////////////////////////////////////////////////////////////
// Name: FindSession
// Author: NIVIS LLC, Adrian Simionescu
// Description: find a session index
// Parameters: 
//              p_pSession - session entry
// Return: session index
///////////////////////////////////////////////////////////////////////////////////
//uint8 FindSessionIndex( NET_SESSION_ATTRIBUTES *p_pSession );

///////////////////////////////////////////////////////////////////////////////////
// Name: FindSessionByType
// Author: NIVIS LLC, Adrian Simionescu
// Description: find a session on table
// Parameters: 
//              p_unSessionType - NL_SESSION_TYPE_UNICAST,
//                                NL_SESSION_TYPE_BROADCAST,
//                                NL_SESSION_TYPE_JOIN
//              p_unDstAddr - the peer nickname 
// Return: the address of a Session on Sessions table
// Obs:
//      if returned NULL means no session found
///////////////////////////////////////////////////////////////////////////////////
NET_SESSION_ATTRIBUTES* FindSessionByType( SessionTypeCodes p_unSessionType, const SHORT_ADDR p_unDstAddr );

///////////////////////////////////////////////////////////////////////////////////
// Name: FindRouteIndex
// Author: NIVIS LLC
// Description: find a route by its Id
// Parameters: 
//              p_ucRouteId - route Id
// Return: index in route table
///////////////////////////////////////////////////////////////////////////////////
uint8 FindRouteIndex( const uint8 p_ucRouteId );

///////////////////////////////////////////////////////////////////////////////////
// Name: FindRouteIndex
// Author: NIVIS LLC
// Description: find the first route to the passed destination (if exist)
// Parameters: p_unDst - destination
// Return: route entry or NULL
///////////////////////////////////////////////////////////////////////////////////
NET_ROUTE_ATTRIBUTES* FindFirstRoute( const uint16 p_unDst );

///////////////////////////////////////////////////////////////////////////////////
// Name: FindSrcRouteIndex
// Author: NIVIS LLC
// Description: finds a source route in the source route table
// Parameters: 
//              p_ucRouteId - RouteId
// Return: index of source route within source route table
// Obs:
//      if returned index == g_ucRoutesNo no route was found
///////////////////////////////////////////////////////////////////////////////////
uint8 FindSrcRouteIndex( const uint8 p_ucRouteId );

///////////////////////////////////////////////////////////////////////////////////
// Name: FindServiceIndex
// Author: NIVIS LLC
// Description: finds a service id in the corresponding table
// Parameters: 
//              p_ucID - search dupa Id
//              p_ucType - ID type: service/route id  
// Return: table entry
// Obs:
//      if returned index == g_ucServicesNo no route was found
///////////////////////////////////////////////////////////////////////////////////
uint8 FindServiceIndex( const uint8 p_ucID, uint8 p_ucType );

#endif // _NIVIS_NETWORK_LAYER_DM_H_ 


