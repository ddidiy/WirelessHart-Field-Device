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
* Name:         network_layer_dm.c
* Author:       Nivis LLC
* Date:         July 2008
* Description:  Network Layer  Management
* Changes:
* Revisions:
****************************************************************************************************/
#include <string.h>
#include "network_layer_dm.h"

#include "app_layer.h"
#include "app_cmdresp.h"
#include "network_layer.h"
#include "datalink_layer.h"
#include "datalink_layer_dm.h"

// NET_ATTRIBUTES
TIME_250MS      g_ulJoinRspTimeout;
TIME_10MS       g_ulChannelSearchTime;

SHORT_ADDR      g_unNickname;
uint8           g_ucMaxRetries;
uint8           g_ucMaxJoinRetries;

NET_SESSION_ATTRIBUTES    g_aSessionsTable[MAX_SESSIONS_NO];
NET_TRANSPORT_ATTRIBUTES  g_aTransportTable[MAX_TRANSPORT_NO];
NET_ROUTE_ATTRIBUTES      g_aRoutesTable[MAX_ROUTES_NO];
NET_SRC_ROUTE_ATTRIBUTES  g_aSrcRoutesTable[MAX_SOURCE_ROUTES_NO];
NET_SERVICE_ATTRIBUTES    g_aServicesTable[MAX_SERVICES_NO];

uint8 g_ucSessionsNo;
uint8 g_ucTransportTableNo;
uint8 g_ucRoutesNo;
uint8 g_ucSrcRoutesNo;
uint8 g_ucServicesNo;

uint8 g_ucOldSessionKeys; // no of still valid old keys

void removeOldConnections(uint8 ucRouteIdx)
{
  uint8 ucIdx;
  for(ucIdx = 0; ucIdx < g_ucRoutesNo; ucIdx++)
  {
    if((g_aRoutesTable[ucIdx].m_unGraphId == g_aRoutesTable[ucRouteIdx].m_unGraphId) && (ucIdx !=ucRouteIdx))
      break;  // old superframe id is still used by another route
  }
  if(ucIdx == g_ucRoutesNo)
  { // old superframe id isn't used by another route, so the related connections shall be deleted
    for(ucIdx = 0; ucIdx < g_ucDllAllConnectionsSize; ucIdx++)
    {
      if(g_aDllConnections[ucIdx].m_unGraphID == g_aRoutesTable[ucRouteIdx].m_unGraphId)
      {
        g_aDllConnections[ucIdx].m_bMarkDeleted = 1;
        COMMIT_MODIFY_DLL_STRUCTS();
      }
    }
  }
}

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
NL_SC NET_Request( NL_SV Service, void* p_pData )
{
  switch(Service)
  {
    uint8 ucFoundIdx;
    NET_SESSION_ATTRIBUTES* pSession;

  case NL_RESET:
    g_ucSessionsNo = 0;
    g_ucRoutesNo = 0;
    g_ucSrcRoutesNo = 0;
    return NL_SUCCESS;

  case NL_WRITE_SESSION_KEY:
  {
    pSession = FindSessionByType(((C963_WriteSession_Resp*)p_pData)->m_eSessionType,
                                ((C963_WriteSession_Resp*)p_pData)->m_unPeerNickname);
    if(pSession == NULL)
    {	// new session

      // RCM_E65_NoMoreEntriesAvailable aldready handled

      // find first empty place in session table
      for(ucFoundIdx = 0; ucFoundIdx < MAX_SESSIONS_NO; ucFoundIdx++)
      {
        if(g_aSessionsTable[ucFoundIdx].m_ucSessionFlags & UNUSED_ENTRY)
          break;
      }

      pSession = &g_aSessionsTable[ucFoundIdx];

      g_ucSessionsNo++;

      // reset all fields in the session structure
      memset(pSession, 0, sizeof(NET_SESSION_ATTRIBUTES));  // the unused entry bit is implicitly reset

      pSession->m_ucSessionType = ((C963_WriteSession_Resp*)p_pData)->m_eSessionType;
      pSession->m_unPeerNickname = ((C963_WriteSession_Resp*)p_pData)->m_unPeerNickname;

      uint8* p = (uint8*)&((C963_WriteSession_Resp*)p_pData)->m_unPeerExpandedDeviceTypeCode;
#if defined(IS_MACHINE_LITTLE_ENDIAN)
      pSession->m_aPeerUniqueID[0] = *(p + 1);
      pSession->m_aPeerUniqueID[1] = *p;

      p = (uint8*)&((C963_WriteSession_Resp*)p_pData)->m_ulPeerDeviceID;
      pSession->m_aPeerUniqueID[2] = *(p + 2);
      pSession->m_aPeerUniqueID[3] = *(p + 1);
      pSession->m_aPeerUniqueID[4] = *p;
#elif defined(IS_MACHINE_BIG_ENDIAN)
      pSession->m_aPeerUniqueID[0] = *p;
      pSession->m_aPeerUniqueID[1] = *(p + 1);

      p = (uint8*)&((C963_WriteSession_Resp*)p_pData)->m_ulPeerDeviceID;
      pSession->m_aPeerUniqueID[2] = *(p + 1);
      pSession->m_aPeerUniqueID[3] = *(p + 2);
      pSession->m_aPeerUniqueID[4] = *(p + 3);
#endif

      memcpy(pSession->m_aSessionKey, ((C963_WriteSession_Resp*)p_pData)->m_aKeyValue, 16);

      // peer nonce counter is initialized only when creating a new session
      pSession->m_ulPeerNonceCounter = ((C963_WriteSession_Resp*)p_pData)->m_ulPeerNonceCounterValue;

      uint8 ucIdx = pSession - g_aSessionsTable;
      // initialize corresponding transport table entries to mark them as unused
      g_aTransportTable[2*ucIdx].m_ucRetryCount = UNUSED_ENTRY;
      g_aTransportTable[2*ucIdx + 1].m_ucRetryCount = UNUSED_ENTRY;
    }
    else
    { // existing session

      // old key has to be retained for (2 * MaxPacketAge) slots
      memcpy(pSession->m_aOldSessionKey, pSession->m_aSessionKey, 16);
      memcpy(pSession->m_aSessionKey, ((C963_WriteSession_Resp*)p_pData)->m_aKeyValue, 16);

      pSession->m_ulKeepOldKeyTime = (2 * g_stProv.ulMaxPacketAge) / 25;  // will be decremented once every 250ms
      pSession->m_ucSessionFlags |= VALID_OLD_KEY;  // use both old and new key until timer reaches zero
      g_ucOldSessionKeys++;
    }

    if ((NET_JOIN_REQ_ADMISSION == g_ucNetJoinState) &&
        (pSession->m_unPeerNickname == 0xF980) &&
        (pSession->m_ucSessionType == SessionTypeCode_Unicast)) // NM unicast session
    {
      g_ucJoinCmdFlags |= BIT2; // will switch to NET_JOIN_LOOSELY_COUPLED
    }
    else if((NET_STATE_QUARANTINED == g_ucNetState) &&
            (pSession->m_unPeerNickname == 0xF981) &&
            (pSession->m_ucSessionType == SessionTypeCode_Unicast)) // GW unicast session
      g_ucJoinCmdFlags |= BIT6; // will switch to NET_STATE_OPERATIONAL

    return RCS_N00_Success;
  }

  case NL_DEL_SESSION:
  {
    pSession = FindSessionByType(((C964_DeleteSession_Resp*)p_pData)->m_eSessionType,
                                 ((C964_DeleteSession_Resp*)p_pData)->m_unPeerNickname);
    if(pSession == NULL)
    { // session not found
      return RCM_E65_SessionWithGivenPeerDoesntExist;
    }

    g_ucSessionsNo--;

    // mark session entry as "unused"
    pSession->m_ucSessionFlags = UNUSED_ENTRY;

    ((C964_DeleteSession_Resp*)p_pData)->m_ucRemainingSessionsNo = MAX_SESSIONS_NO - g_ucSessionsNo;

    return RCS_N00_Success;
  }

  case NL_ADD_ROUTE:
  {
    // check if there is room into the route table
    ucFoundIdx = FindRouteIndex(((C974_WriteRoute_Resp*)p_pData)->m_ucRouteID);
    if( NET_INVALID_IDX == ucFoundIdx )
    { // new route
      if( g_ucRoutesNo >= MAX_ROUTES_NO )
      { // route table is full
        return RCM_E65_NoMoreEntriesAvailable;
      }
      else
      {
        ucFoundIdx = g_ucRoutesNo; // new route index
      }
    }

    // check nickname
    pSession = FindSessionByType( SessionTypeCode_Unicast, ((C974_WriteRoute_Resp*)p_pData)->m_unPeerNickname );
    if( pSession == NULL )
    { // no session with given correspondent
      return RCM_E66_InvalidNickname;
    }

    uint8 ucRC = 0;
    MONITOR_ENTER();
    do
    {
      // check graph
      // always allow GraphID=FFFF to can have a route with source routing only
      if( ((C974_WriteRoute_Resp*)p_pData)->m_unGraphID != 0xFFFF )
      { 
        if( ((C974_WriteRoute_Resp*)p_pData)->m_unGraphID >= 0x100)
        { // GraphId >= 0x100 identifies a graph
          uint8 ucIdx;
          for(ucIdx = 0; ucIdx < g_ucDllAllConnectionsSize; ucIdx++)
          {
            if( (g_aDllConnections[ucIdx].m_unGraphID == ((C974_WriteRoute_Resp*)p_pData)->m_unGraphID) && !g_aDllConnections[ucIdx].m_bMarkDeleted )
              break;
          }
          if(ucIdx >= g_ucDllAllConnectionsSize)
          { // graph not found
            ucRC = RCM_E67_InvalidGraphID;
            break;
          }
        }
        else
        { // GraphId < 0x100 actually identifies a superframe used for superframe routing
          uint8 ucSfIdx = DLL_FindSuperframe((uint8)((C974_WriteRoute_Resp*)p_pData)->m_unGraphID );
          if( (ucSfIdx == DLL_INVALID_SF_IDX) || (g_aDllSuperframes[ucSfIdx].m_bMarkDeleted) )
          { // superframe not found
            ucRC = RCM_E67_InvalidGraphID;
            break;
          }
          else
          { // add connections to all neighbors having Tx links to in that superframe
            uint8 ucCFlag = 0;  // set if at least one connection on that superframe
            for( uint8 ucIdx = 0; ucIdx < g_ucDllAllLinksSize; ucIdx++ )
            {
              if( ( g_aDllLinks[ucIdx].m_ucSuperframeID == (uint8)((C974_WriteRoute_Resp*)p_pData)->m_unGraphID ) && 
                    g_aDllLinks[ucIdx].m_bTx && !g_aDllLinks[ucIdx].m_bMarkDeleted &&
                    ((g_aDllLinks[ucIdx].m_eLinkType == LinkType_Normal) || ((g_aDllLinks[ucIdx].m_eLinkType == LinkType_Join) && (g_ucNetState < NET_STATE_QUARANTINED)))   
                 )
              { // found an eligible neighbor
                // check if such a connection already exists
                uint8 ucConnIdx = DLL_FindConnection( (uint8)((C974_WriteRoute_Resp*)p_pData)->m_unGraphID, g_aDllLinks[ucIdx].m_unNeighborShortAddr );
                if( DLL_INVALID_CONN_IDX == ucConnIdx)
                { // add new connection
                  if( ! DLL_AddConnection( (uint8)((C974_WriteRoute_Resp*)p_pData)->m_unGraphID, g_aDllLinks[ucIdx].m_unNeighborShortAddr )  )
                  { // added to the end, but the table still has to be re-arranged at the beginning of the next slot
                    COMMIT_MODIFY_DLL_STRUCTS();
                    ucCFlag = 1;
                  }
//                  else
//                  { // TODO: what if no more room in the connection table???
//                  }
                }
                else
                { // connection already exists, but could be marked for delete
                  g_aDllConnections[ucConnIdx].m_bMarkDeleted = 0;
                  ucCFlag = 1;
                }
              }      
            }  
            if(ucCFlag == 0)
            { // no connection could be created on specified superframe (no link, or no more room)
              ucRC = RCM_E67_InvalidGraphID;
              break;
            }
          }
        }
      }
      
      // all checks already made, the route can now be created/modified with no error
      if((ucFoundIdx != g_ucRoutesNo) && (((C974_WriteRoute_Resp*)p_pData)->m_unGraphID != g_aRoutesTable[ucFoundIdx].m_unGraphId))
      { // the route already exists, but the GraphID has to be changed
        if(g_aRoutesTable[ucFoundIdx].m_unGraphId < 0x100)
        { // the old route used superframe routing
          removeOldConnections(ucFoundIdx);
        }
      }
    } while(0);
    MONITOR_EXIT();
    if(ucRC)
      return ucRC; // an error condition was encountered inside do-while block
    
    if(ucFoundIdx == g_ucRoutesNo)
    { // add new route
      memset( &g_aRoutesTable[ucFoundIdx], 0, sizeof(NET_ROUTE_ATTRIBUTES) );

      g_aRoutesTable[ucFoundIdx].m_ucRouteId = ((C974_WriteRoute_Resp*)p_pData)->m_ucRouteID;
//      g_aRoutesTable[ucFoundIdx].m_bIsDefault = 0;
//      g_aRoutesTable[ucFoundIdx].m_bSourceRouteAttached = 0;

      g_ucRoutesNo++;
    }

    // set route params
    g_aRoutesTable[ucFoundIdx].m_unDestNickname = ((C974_WriteRoute_Resp*)p_pData)->m_unPeerNickname;
    g_aRoutesTable[ucFoundIdx].m_unGraphId = ((C974_WriteRoute_Resp*)p_pData)->m_unGraphID;
    if(((C974_WriteRoute_Resp*)p_pData)->m_unPeerNickname == NM_NICKNAME)
    { // if a new route to NM was set, use it when send Advertisements
      MONITOR_ENTER();
      g_sDllSharedParams.m_unJoinGraphID = ((C974_WriteRoute_Resp*)p_pData)->m_unGraphID;
      MONITOR_EXIT();
    }
    
    ((C974_WriteRoute_Resp*)p_pData)->m_ucRemainingRoutesNo = MAX_ROUTES_NO - g_ucRoutesNo;

    return RCS_N00_Success;
  }

  case NL_DEL_ROUTE:
  {
    ucFoundIdx = FindRouteIndex( ((C975_DeleteRoute_Resp*)p_pData)->m_ucRouteID );
    if (ucFoundIdx >= g_ucRoutesNo)
    { // route not found
      return RCM_E65_InvalidRouteID;
    }
    
    if(g_aRoutesTable[ucFoundIdx].m_unGraphId < 0x100)
    { // check if that superframe is not still used by another route, and if not - delete those connections
      MONITOR_ENTER();
      removeOldConnections(ucFoundIdx);
      MONITOR_EXIT();
    }
    g_ucRoutesNo--;

    memcpy( &g_aRoutesTable[ucFoundIdx],
            &g_aRoutesTable[ucFoundIdx + 1],
            (g_ucRoutesNo - ucFoundIdx) * sizeof(NET_ROUTE_ATTRIBUTES) );

    ((C975_DeleteRoute_Resp*)p_pData)->m_ucRemainingRoutesNo = MAX_ROUTES_NO - g_ucRoutesNo;
    return RCS_N00_Success;
  }

//  case NL_DEFAULT_ROUTE:    // ??? see Issue45
//  {
//    ucFoundIdx = FindRouteIndex( *(uint8*)p_pData );
//    if (ucFoundIdx >= g_ucRoutesNo)
//    {
//      return NL_NO_ROUTE;
//    }
//    for( uint8 ucIdx = 0; ucIdx < g_ucRoutesNo; ucIdx++ )
//    {
//      g_aRoutesTable[ucIdx].m_bIsDefault = 0;
//    }
//    g_aRoutesTable[ucFoundIdx].m_bIsDefault = 1;
//    return NL_SUCCESS;
//  }

  case NL_ADD_SRC_ROUTE:
    {
      uint8 ucRouteIdx = FindRouteIndex( ((C976_WriteSourceRoute_Resp*)p_pData)->m_ucRouteID );
      if( ucRouteIdx >= g_ucRoutesNo )
      { // route not found
        return RCM_E66_InvalidRouteID;
      }

      // check for invalid nicknames
      uint8 i, j;
      for( i = 0; i < ((C976_WriteSourceRoute_Resp*)p_pData)->m_ucHopsNo; i++ )
      {
        if( ((C976_WriteSourceRoute_Resp*)p_pData)->m_aNicknameHopEntries[i] == 0x0000)
        { // invalid nickname
          return RCM_E67_InvalidNickname;
        }
      }

      ucFoundIdx = FindSrcRouteIndex( ((C976_WriteSourceRoute_Resp*)p_pData)->m_ucRouteID );
      if( ucFoundIdx >= g_ucSrcRoutesNo )
      { // new source route
        if( g_ucSrcRoutesNo >= MAX_SOURCE_ROUTES_NO)
        {
          return RCM_E65_NoMoreEntriesAvailable;
        }
        ucFoundIdx = g_ucSrcRoutesNo;
        g_aSrcRoutesTable[ucFoundIdx].m_ucRouteId = ((C976_WriteSourceRoute_Resp*)p_pData)->m_ucRouteID;
      }

      uint8 ucRespCode = NL_SUCCESS;

      // copy valid hop addresses
      for( i = 0, j = 0; i < ((C976_WriteSourceRoute_Resp*)p_pData)->m_ucHopsNo; i++ )
      {
        if( ((C976_WriteSourceRoute_Resp*)p_pData)->m_aNicknameHopEntries[i] == 0xFFFF)
        { // broadcast address - not valid in a source route
          ucRespCode = RCM_W08_BroadcastAddrDeletedInResp;
        }
        else
        {
          g_aSrcRoutesTable[ucFoundIdx].m_aSrcRouteHops[j] = ((C976_WriteSourceRoute_Resp*)p_pData)->m_aNicknameHopEntries[i];
          j++;
        }
      }

      if( j == 0 )
      { // all hop addresses were FFFF; no valid address left to set the source route
        return RCM_E68_InvalidNoOfHops;
      }

      if( ucFoundIdx == g_ucSrcRoutesNo )
        g_ucSrcRoutesNo++;

      // save number of hops in source route
      g_aSrcRoutesTable[ucFoundIdx].m_ucNoOfHops = j;

      // copy destination address to unused hop entries
      for( ; j < 8; j++ )
      {
        g_aSrcRoutesTable[ucFoundIdx].m_aSrcRouteHops[j] = g_aSrcRoutesTable[ucFoundIdx].m_aSrcRouteHops[j - 1];
      }

      g_aRoutesTable[ucRouteIdx].m_bSourceRouteAttached = 1;
      
      ((C976_WriteSourceRoute_Resp*)p_pData)->m_ucRemainingSourceRoutesNo = MAX_SOURCE_ROUTES_NO - g_ucSrcRoutesNo;

      return ucRespCode;
    }

  case NL_DEL_SRC_ROUTE:
    {
      ucFoundIdx = FindSrcRouteIndex( ((C977_DeleteSourceRoute_Resp*)p_pData)->m_ucRouteID );
      if( ucFoundIdx >= g_ucSrcRoutesNo )
      { // source route not found
        return RCM_E66_InvalidRouteID;
      }

      g_ucSrcRoutesNo--;

      memcpy( &g_aSrcRoutesTable[ucFoundIdx],
              &g_aSrcRoutesTable[ucFoundIdx + 1],
             (g_ucSrcRoutesNo - ucFoundIdx) * sizeof(NET_SRC_ROUTE_ATTRIBUTES) );

      ucFoundIdx = FindRouteIndex( ((C977_DeleteSourceRoute_Resp*)p_pData)->m_ucRouteID );
      if( ucFoundIdx < g_ucRoutesNo )
      {
        g_aRoutesTable[ucFoundIdx].m_bSourceRouteAttached = 0;
      }
      //else route not found (probably deleted earlier)

      return RCS_N00_Success;
    }

  case NL_WRITE_SERVICE:
    {
#define p ((C973_WriteService_Resp*)p_pData)
      pSession = FindSessionByType( SessionTypeCode_Unicast, p->m_unPeerNickname );
      if( pSession == NULL )
      { // no session with given correspondent
        return RCM_E69_UnknownCorrespNickname;
      }

      uint8 ucRouteIdx = FindRouteIndex( p->m_ucRouteID );
      if( ucRouteIdx >= g_ucRoutesNo )
      { // route not found
        return RCM_E70_InvalidRouteID;
      }

      if( pSession->m_unPeerNickname != g_aRoutesTable[ucRouteIdx].m_unDestNickname )
      {
        return RCM_E71_CorrespNicknameAndRouteCorrespMismatch;
      }

      ucFoundIdx = FindServiceIndex( p->m_ucServiceID, NL_SEARCH_BY_SERVICE_ID );

      if(ucFoundIdx == NET_INVALID_IDX )
      { // new service must be added
        if( g_ucServicesNo >= MAX_SERVICES_NO )
        {
          return RCM_E65_NoMoreEntriesAvailable;
        }
        
        ucFoundIdx = g_ucServicesNo;
        g_aServicesTable[ucFoundIdx].m_ucServiceID = p->m_ucServiceID;

        g_ucServicesNo++;
      }
      else if(g_aServicesTable[ucFoundIdx].m_tPeriod.u32 > p->m_tPeriod.u32)   // 1/32ms
      { // when reducing the existing bandwidth - set "Capacity denied", "More status available"
        // TODO: should modify the period of the affected burst/event, too?????
        g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_CapacityDenied;
        APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
      }

      g_aServicesTable[ucFoundIdx].m_unPeerNickname = p->m_unPeerNickname;
      g_aServicesTable[ucFoundIdx].m_ucServiceFlags = p->m_ucRequestFlags;
      g_aServicesTable[ucFoundIdx].m_ucApplicationDomain = p->m_eApplicationDomain;
      g_aServicesTable[ucFoundIdx].m_ucRouteID = p->m_ucRouteID;
      g_aServicesTable[ucFoundIdx].m_tPeriod.u32 = p->m_tPeriod.u32;   // 1/32ms

      p->m_ucRemainingServicesNo = MAX_SERVICES_NO - g_ucServicesNo;
#undef p
      return RCS_N00_Success;
    }

  case NL_READ_TTL:
    *(uint8*)p_pData = g_stProv.ucDefaultTTL;
    return RCS_N00_Success;

  case NL_WRITE_TTL:
    //TODO: TTL must be at least 8; if requested TTL is less, TTL must be set to 8 and RCM_W08_SetToNearestPossibleValue returned
    g_stProv.ucDefaultTTL = *(const uint8*) p_pData;

// TTL must get the default value of 32 after reset (see TML211A), no need to save it in NVM   
//    // NVM -> store the individual record into FLASH 
//    // write ucDefaultTTL to FLASH
//    NVM_WriteIndividualRecord( 6, 
//                               (uint8*)&g_stProv + c_aProvNvmTable[5].m_unOffset, 
//                               c_aProvNvmTable[5].m_ucSize);
    
    return RCS_N00_Success;

  default:
    return NL_INVALID_PARAMETER;
  }
}


void NET_CheckOldSessionKeysTimes()
{
  if(g_ucOldSessionKeys)
  {
    uint8 n = 0;
    for(uint8 i = 0; i < MAX_SESSIONS_NO; i++)
    {
      if((g_aSessionsTable[i].m_ucSessionFlags & (UNUSED_ENTRY | VALID_OLD_KEY)) == VALID_OLD_KEY)
      { // this session entry is in use and the old key is still valid
        n++;
        if(--g_aSessionsTable[i].m_ulKeepOldKeyTime == 0)
        {
          n--;
          g_aSessionsTable[i].m_ucSessionFlags &= (uint8)(~VALID_OLD_KEY);   // reset bit -> don't use old key anymore
        }
      }
    }
    g_ucOldSessionKeys = n; // no of still valid old keys
  }
}


///////////////////////////////////////////////////////////////////////////////////
// Name: Net_Confirm
///////////////////////////////////////////////////////////////////////////////////
void NET_Confirm( NL_SV Service, NL_SC Status, const void* p_pData )
{

}

///////////////////////////////////////////////////////////////////////////////////
// Name: Net_Indication
///////////////////////////////////////////////////////////////////////////////////
void NET_Indication( NL_SV Service, NL_SC Status, const void* p_pData )
{

}

///////////////////////////////////////////////////////////////////////////////////
// Name: FindSession
// Author: NIVIS LLC, Adrian Simionescu
// Description: find a session index
// Parameters:
//              p_pSession - session entry
// Return: session index
///////////////////////////////////////////////////////////////////////////////////
/*
uint8 FindSessionIndex( NET_SESSION_ATTRIBUTES *p_pSession )
{
  uint8 ucIdx;
  for( ucIdx = 0; ucIdx < MAX_SESSIONS_NO; ucIdx++ )
  {
    if (  ( 0 == (g_aSessionsTable[ucIdx].m_ucSessionType & UNUSED_ENTRY) )
        &&(&g_aSessionsTable[ucIdx] == p_pSession) )
    return ucIdx;
  }

  return ucIdx;
}
*/

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
NET_SESSION_ATTRIBUTES* FindSessionByType( SessionTypeCodes p_unSessionType, const SHORT_ADDR p_unDstAddr )
{
  // search entire session table, skipping unused entries
  for( uint8 ucIdx = 0; ucIdx < MAX_SESSIONS_NO; ucIdx++ )
  {
    if( 0 == (g_aSessionsTable[ucIdx].m_ucSessionFlags & UNUSED_ENTRY) )
    { // this entry is in use
      if( (g_aSessionsTable[ucIdx].m_ucSessionType == p_unSessionType) &&
          (g_aSessionsTable[ucIdx].m_unPeerNickname == p_unDstAddr) )
        return &g_aSessionsTable[ucIdx];
    }
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: FindRouteIndex
// Author: NIVIS LLC
// Description: find a route by its Id
// Parameters:
//              p_ucRouteId - route Id
// Return: index in route table
///////////////////////////////////////////////////////////////////////////////////
uint8 FindRouteIndex( const uint8 p_ucRouteId )
{
  uint8 ucIdx;
  for (ucIdx = 0; ucIdx < g_ucRoutesNo; ucIdx++)
  {
    if (g_aRoutesTable[ucIdx].m_ucRouteId == p_ucRouteId)
      return ucIdx;
  }
  return NET_INVALID_IDX;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: FindRouteIndex
// Author: NIVIS LLC
// Description: find the first route to the passed destination (if exist)
// Parameters: p_unDst - destination
// Return: route entry or NULL
///////////////////////////////////////////////////////////////////////////////////
NET_ROUTE_ATTRIBUTES* FindFirstRoute( const uint16 p_unDst )
{
  uint8 ucIdx=0;
  for( ; ucIdx<g_ucRoutesNo; ucIdx++ )
  {
     if( p_unDst == g_aRoutesTable[ucIdx].m_unDestNickname )
       break;
  }

  if( ucIdx >= g_ucRoutesNo )
  {
   return NULL;
  }
  else
  {
    return &g_aRoutesTable[ucIdx];
  }
}

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
uint8 FindSrcRouteIndex( const uint8 p_ucRouteId )
{
  uint8 ucIdx;
  for( ucIdx = 0; ucIdx < g_ucSrcRoutesNo; ucIdx++ )
  {
    if ( g_aSrcRoutesTable[ucIdx].m_ucRouteId == p_ucRouteId)
      return ucIdx;
  }
  return NET_INVALID_IDX;
}


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
uint8 FindServiceIndex( const uint8 p_ucID, uint8 p_ucType )
{
  for( uint8 ucIdx = 0; ucIdx < g_ucServicesNo; ucIdx++ )
  {
    if (NL_SEARCH_BY_SERVICE_ID == p_ucType)
    {
      if ( g_aServicesTable[ucIdx].m_ucServiceID == p_ucID )
        return ucIdx;
    }
    else if (NL_SEARCH_BY_ROUTE_ID == p_ucType)
    {
      if ( g_aServicesTable[ucIdx].m_ucRouteID == p_ucID )
        return ucIdx;
    }
  }
  return NET_INVALID_IDX;
}
