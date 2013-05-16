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
* Name:         datalink_layer.c
* Author:       NIVIS LLC,
* Date:         July 2008
* Description:  Data Link Layer
* Changes:
* Revisions:
****************************************************************************************************/
#include <string.h>
#include "../timers.h"
#include "../asm.h"
#include "tmr_util.h"
#include "datalink_layer.h"
#include "datalink_layer_dm.h"
#include "network_layer.h"
#include "network_layer_dm.h"
#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/DataLinkLayerCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/WirelessNetworkManagerCommands.h"
#else
  #include "ApplicationLayer/Model/DataLinkLayerCommands.h"
  #include "ApplicationLayer/Model/WirelessNetworkManagerCommands.h"
#endif
#include "provision.h"


const TIME_MIN_MAX_T c_tTMinMax[7] =
{ //Fact//Min  //Max     //Default    //Variable - Common Table 43
  {  10,  2*100, 1000*100,   10*100,     &g_stProv.ulDllDiscovery}, // WirelessTimerCode_Discovery
  {  10,      0,  100*100,       90,     &g_stProv.ulDllAdvertise}, //0.9*100 // WirelessTimerCode_Advertisment // when AdvertiseInterval is set to 0 then an Advertise packet shall be generated whenever a non-shared transmit link is available
  {  10,  5*100, 5000*100,   10*100,     &g_stProv.ulDllKeepAlive}, // WirelessTimerCode_Keep_Alive
  {  10, 15*100, 5000*100,  100*100,      &g_stProv.ulDllPathFail}, // WirelessTimerCode_PathFailure
  { 250,   30*4,   5000*4,  15*60*4, &g_stProv.ulHealthReportTime}, // WirelessTimerCode_HealthReport
  {  10,  5*100,  300*100,   60*100,   &g_stProv.ulBcastReplyTime}, // WirelessTimerCode_BroadcastReply
  {  10,  1*100,    32767,  300*100,     &g_stProv.ulMaxPacketAge}  // WirelessTimerCode_MaximumPDUAge // max. limit is 32767 to cover no more than half of ASN snippet which is a 2-byte unsigned
};

static DLL_PACKET_T g_sDllPacket;

uint16 g_unDiscPktHandle;

#ifndef BACKBONE_SUPPORT 
extern NET_LOOP_DETECTION g_stLoopDetect;
#endif

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint8 getRemainingSuperframes(void)
{
  uint8 ucMarkedForDel = 0;
  for(uint8 ucIdx = 0; ucIdx < g_ucDllAllSuperframesSize; ucIdx++)
  {
    if(g_aDllSuperframes[ucIdx].m_bMarkDeleted)
      ucMarkedForDel++;
  }
  return (DLL_MAX_SUPERFRAMES - g_ucDllAllSuperframesSize + ucMarkedForDel);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint8 getRemainingLinks(void)
{
  uint8 ucMarkedForDel = 0;
  for(uint8 ucIdx = 0; ucIdx < g_ucDllAllLinksSize; ucIdx++)
  {
    if(g_aDllLinks[ucIdx].m_bMarkDeleted)
      ucMarkedForDel++;
  }
  return (DLL_MAX_LINKS - g_ucDllAllLinksSize + ucMarkedForDel);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint8 getRemainingConnections(void)
{
  uint8 ucMarkedForDel = 0;
  for(uint8 ucIdx = 0; ucIdx < g_ucDllAllConnectionsSize; ucIdx++)
  {
    if(g_aDllConnections[ucIdx].m_bMarkDeleted)
      ucMarkedForDel++;
  }
  return (DLL_MAX_CONNECTIONS - g_ucDllAllConnectionsSize + ucMarkedForDel);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_LM_Request ( uint8 p_ucService, void * p_pData )
{
  uint8 ucStatus = RCS_N00_Success;
//.  uint8 bInterruptEnabled =  IS_ITC_INT_ENABLE(TMR_INT);

  switch ( p_ucService )
  {
  case DLL_SERV_RESET :
    DLL_Init();
    break;

  case DLL_SERV_DISCONNECT :
    {
      MONITOR_ENTER();
      // ??? aici sa sterg packete din anumite intervale de handler din coada ??? 
      // ??? aici sa refac timeouturi cu asn
      g_ucDllDisconnect = DLL_DISCONNECT_WAIT_FOR_QUEUE;   
      g_ulDiscStartRTC = RTC_COUNT;
      if(NULL != p_pData)    
      { // Suspend & Resume 
        uint8 * pData = ( uint8 * ) p_pData;
        
        ((uint8*)(&g_ullDllSuspendTimeout))[7] = 0;
        ((uint8*)(&g_ullDllSuspendTimeout))[6] = 0;
        ((uint8*)(&g_ullDllSuspendTimeout))[5] = 0;
        ((uint8*)(&g_ullDllSuspendTimeout))[4] = pData[0]; // MSB
        ((uint8*)(&g_ullDllSuspendTimeout))[3] = pData[1]; 
        ((uint8*)(&g_ullDllSuspendTimeout))[2] = pData[2]; 
        ((uint8*)(&g_ullDllSuspendTimeout))[1] = pData[3]; 
        ((uint8*)(&g_ullDllSuspendTimeout))[0] = pData[4]; // LSB
        
        ((uint8*)(&g_ullDllResumeTimeout))[7] = 0;
        ((uint8*)(&g_ullDllResumeTimeout))[6] = 0;
        ((uint8*)(&g_ullDllResumeTimeout))[5] = 0;
        ((uint8*)(&g_ullDllResumeTimeout))[4] = pData[5]; // MSB      
        ((uint8*)(&g_ullDllResumeTimeout))[3] = pData[6]; 
        ((uint8*)(&g_ullDllResumeTimeout))[2] = pData[7]; 
        ((uint8*)(&g_ullDllResumeTimeout))[1] = pData[8]; 
        ((uint8*)(&g_ullDllResumeTimeout))[0] = pData[9]; // LSB
      }
      
      g_ulDllQueueDiscCntr = 0;
      // find out the max superframe length to figure out how much time has to wait to get the DLL queue empty 
      for(DLL_SUPERFRAME_T* pSf = g_aDllSuperframes; pSf<(g_aDllSuperframes + g_ucDllCrtSuperframesSize); pSf++)
      {
        if(!pSf->m_bMarkDeleted && (DLL_SF_ENABLED == pSf->m_eActive) && (pSf->m_unNumSlots > g_ulDllQueueDiscCntr))
          g_ulDllQueueDiscCntr = pSf->m_unNumSlots;
      }
      g_ulDllQueueDiscCntr *= DLL_MAX_RETRIES; // [slots]
      
      // before this point it would be nice to clean up the DLL queue: only keep the forwarded packets (handle 0x100 to 0x6FFF)
      //  and the response to command that caused the disconnecting (C42, C771, C960, C972: handle == g_unDiscPktHandle)
      if(g_ucDllCrtQueueSize > 1)  
        g_ulDllQueueDiscCntr *= g_ucDllCrtQueueSize;
      if(g_ulDllQueueDiscCntr > g_stProv.ulMaxPacketAge)
        g_ulDllQueueDiscCntr = g_stProv.ulMaxPacketAge; // max limit for ulMaxPacketAge is 32767 slots

      g_ulDllQueueDiscCntr = (g_ulDllQueueDiscCntr << 15)/100; // [RTC counts]
      MONITOR_EXIT();
    }    
    break;

  case DLL_SERV_REJOIN :
    DLL_Join();
    break;

  case DLL_SERV_WRITE_SUPERFRAME :
    {
      C965_WriteSuperframe_Resp* p_pSF = (C965_WriteSuperframe_Resp*) p_pData;
//.      if( bInterruptEnabled )
//.      {
//.        DISABLE_DLL_TMRIRQ(); // EXCLUSIVE Code Begin
//.      }
      MONITOR_ENTER();
      uint8 ucSFIdx = DLL_FindSuperframe( p_pSF->m_ucSuperframeID );
      if( ucSFIdx == DLL_INVALID_SF_IDX )
      { // SF not exists : Adaugare
        DLL_AddSuperframe( p_pSF->m_ucSuperframeID, p_pSF->m_ucSuperframeMode, p_pSF->m_unSuperframeSlotsNo ); // error: no more entries available (already handled)
      }
      else
      { // SF exists : Modificare
        g_aDllSuperframes[ucSFIdx].m_bMarkDeleted = 0;  // in case it was marked for delete
        if ( ! ( p_pSF->m_ucSuperframeMode & DLL_SF_ENABLED ) )
        {
          g_aDllSuperframes[ucSFIdx].m_bSetDisabled = 1;
        }
        if( g_aDllSuperframes[ucSFIdx].m_eActive == DLL_SF_DISABLED || g_aDllSuperframes[ucSFIdx].m_bSetDisabled == 1 )
        {
          if( g_aDllSuperframes[ucSFIdx].m_unNumSlots != p_pSF->m_unSuperframeSlotsNo )
          {
            DLL_ModifySuperframe( ucSFIdx, p_pSF->m_unSuperframeSlotsNo);
          }
        }
        if ( p_pSF->m_ucSuperframeMode & DLL_SF_ENABLED )
        {
          g_aDllSuperframes[ucSFIdx].m_bSetEnabled = 1;
        }
        if(p_pSF->m_ucSuperframeMode & 0x80)                  // handheld superframe flag is set
          g_aDllSuperframes[ucSFIdx].m_bHandheldSF = 1;       // shoud this be delayed to next slot?
        else
          g_aDllSuperframes[ucSFIdx].m_bHandheldSF = 0;       // shoud this be delayed to next slot?
          
      }
      p_pSF->m_ucRemainingSuperframesNo = getRemainingSuperframes();
      uint16 unIndex = 0;
      for( ; unIndex < g_ucDllAllSuperframesSize; unIndex++ )   
      {
        if( (g_aDllSuperframes[unIndex].m_eActive | g_aDllSuperframes[unIndex].m_bSetEnabled) && 
           !(g_aDllSuperframes[unIndex].m_bMarkDeleted | g_aDllSuperframes[unIndex].m_bSetDisabled))
        {
          break;
        }
      }
      if(unIndex == g_ucDllAllSuperframesSize)
      { //if no active superframe, restart device
        // save the RestartReason and wait for a watchdog reset
        PrepareForReset(RREASON_NO_SUPERFRAME); //
      }        
      COMMIT_MODIFY_DLL_STRUCTS();
//.      if( bInterruptEnabled )
//.      {
//.        ENABLE_DLL_TMRIRQ(); // EXCLUSIVE Code End
//.      }
      MONITOR_EXIT();
      if (NET_JOIN_LOOSELY_COUPLED == g_ucNetJoinState)
        g_ucJoinCmdFlags |= BIT3;   // CHECK ? must be active or enabled?
    }
    break;

  case DLL_SERV_DEL_SUPERFRAME :
    {
      C966_DeleteSuperframe_Resp* p_pSF = (C966_DeleteSuperframe_Resp*) p_pData;

//.      if( bInterruptEnabled )
//.      {
//.        DISABLE_DLL_TMRIRQ(); // EXCLUSIVE Code Begin
//.      }
      MONITOR_ENTER();
      uint8 ucSFIdx = DLL_FindSuperframe( p_pSF->m_ucSuperframeID );
      if( ucSFIdx != DLL_INVALID_SF_IDX )
      {
        DLL_DeleteSuperframe( ucSFIdx );
        p_pSF->m_ucRemainingSuperframeEntriesNo = getRemainingSuperframes();
        uint16 unIndex = 0;
        for( ; unIndex < g_ucDllAllSuperframesSize; unIndex++ )   
        {
          if( (g_aDllSuperframes[unIndex].m_eActive || g_aDllSuperframes[unIndex].m_bSetEnabled) && 
             !(g_aDllSuperframes[unIndex].m_bMarkDeleted | g_aDllSuperframes[unIndex].m_bSetDisabled))
          {
            break;
          }
        }
        if(unIndex == g_ucDllAllSuperframesSize)
        { //if last active superframe deleted, restart device
          // save the RestartReason and wait for a watchdog reset
          PrepareForReset(RREASON_NO_SUPERFRAME); //
        }        
        COMMIT_MODIFY_DLL_STRUCTS();
      }
//.      if( bInterruptEnabled )
//.      {
//.        ENABLE_DLL_TMRIRQ(); // EXCLUSIVE Code End
//.      }
      MONITOR_EXIT();
      if( ucSFIdx == DLL_INVALID_SF_IDX )
      { // error: superframe not found
        return RCM_E65_EntryNotFound;
      }
    }
    break;

  case DLL_SERV_ADD_LINK :
    {
      if ( g_ucDllAllLinksSize >= DLL_MAX_LINKS )
      { // error: no more links available
        return RCM_E65_NoMoreEntriesAvailable;
      }

      C967_WriteLink_Resp* p_pLink = (C967_WriteLink_Resp*) p_pData;

      // LinkOptions: bit2=shared, bit1=receive, bit0=transmit
      if (
          ( (p_pLink->m_ucLinkOptions & 0x03) == 0 ) // neither transmit, nor receive
            ||
          ( (p_pLink->m_ucLinkOptions & 0x07) == 6 ) // both receive and shared, but not transmit
         )
      { // error: invalid link options
        return RCM_E69_InvalidLinkOptions;
      }

      if( p_pLink->m_ucChannelOffset >= g_ucDllActiveChannels )    // TODO: recheck condition
      { // error: invalid channel offset
        return RCM_E70_InvalidChannelOffset;
      }

      uint8 ucRetErr = 0;
//.      if( bInterruptEnabled )
//.      {
//.        DISABLE_DLL_TMRIRQ(); // EXCLUSIVE Code Begin
//.      }
      MONITOR_ENTER();
      
      do
      {
        if( NET_STATE_OPERATIONAL == g_ucNetState) 
        {
          uint8 ucLnkIdx = DLL_FindLink( p_pLink->m_ucSuperframeID, p_pLink->m_unNeighborNickname, p_pLink->m_unSlotNumber );
          if( (DLL_INVALID_LINK_IDX != ucLnkIdx) && !g_aDllLinks[ucLnkIdx].m_bMarkDeleted )
          { // error: link already exists
            ucRetErr = RCM_E66_LinkAlreadyExists;
            break;
          }
        }
        
        uint8 ucSFIdx = DLL_FindSuperframe( p_pLink->m_ucSuperframeID );
        if( ucSFIdx == DLL_INVALID_SF_IDX )
        {	// error: superframe doesn't exist
          ucRetErr = RCM_E67_UnknownSuperframeID;
          break;
        }
        
        if ( p_pLink->m_unSlotNumber >= g_aDllSuperframes[ucSFIdx].m_unNumSlots )
        { // error: invalid slot number
          ucRetErr = RCM_E68_InvalidSlotNumber;
          break;
        }
        
        if ( ( p_pLink->m_eLinkType == DLL_LINK_TYPE_NORMAL ) &&
            ( p_pLink->m_unNeighborNickname != 0xF980) &&
              ( p_pLink->m_unNeighborNickname != 0xF981) &&
                ( p_pLink->m_unNeighborNickname != 0xFFFF) )
        {
          if( DLL_INVALID_NEIGH_IDX == DLL_FindShortNeighbor( p_pLink->m_unNeighborNickname ) )   // nickname not found in neighbor table
          {
            if( DLL_AddShortNeighbor( p_pLink->m_unNeighborNickname ) )
            { // error: no more neighbors available
              ucRetErr = RCM_E72_NoMoreNeighborsAvailable;
              break;
            }
            //          else
            //          { // if a new neighbor was added from Cmd967_WriteLink
            //            DLL_NEIGHBOR_T * pNeighbor = g_aDllNeighbors + DLL_FindShortNeighbor( p_pLink->m_unNeighborNickname );
            //            pNeighbor->m_tmLastComm = g_sDllASN.m_ulLsb;  // g_sDllASN.m_ulLsb - g_stProv.ulDllKeepAlive; // in order to start Keep-Alives immediately
            //            pNeighbor->m_tmPathFailCounter = g_stProv.ulDllPathFail;
            //            pNeighbor->m_cAvgRSL = -128; // init with min value, to distinguish from a self-discovered linked neighbor
            //          }
          }
        }
        
        DLL_AddLink( p_pLink->m_ucSuperframeID , p_pLink->m_unNeighborNickname,
                      p_pLink->m_unSlotNumber, p_pLink->m_ucChannelOffset,
                      (p_pLink->m_ucLinkOptions & 0x01) >> 0,    // transmit
                      (p_pLink->m_ucLinkOptions & 0x02) >> 1,    // receive
                      (p_pLink->m_ucLinkOptions & 0x04) >> 2,    // shared
                      p_pLink->m_eLinkType );
          
        p_pLink->m_unRemainingLinksNo = getRemainingLinks();
        COMMIT_MODIFY_DLL_STRUCTS();
        
      } while (0);  // fake do-while loop, only used to make a break in case of an error condition (to avoid goto)
      
//.      if( bInterruptEnabled )
//.      {
//.        ENABLE_DLL_TMRIRQ(); // EXCLUSIVE Code End
//.      }
      MONITOR_EXIT();
      if ( ucRetErr )
      { // error in exclusive zone above
        return ucRetErr;
      }
      
      // the Emerson Network Manager writes a broadcast Rx link during the join process; the field device must accept it in order to successfully finish the join process;
      // since there is no restriction in the HCF standard, we are going to accept a broadcast Rx/Tx link when the NET layer is in loosely coupled.
      if( (NET_JOIN_LOOSELY_COUPLED == g_ucNetJoinState) && 
         (p_pLink->m_unNeighborNickname != 0xFFFF) && ((p_pLink->m_eLinkType == DLL_LINK_TYPE_NORMAL) || (p_pLink->m_eLinkType == DLL_LINK_TYPE_BROADCAST)) )
      { // one single TR link not accepted to switch to Quarantined state
        if(p_pLink->m_ucLinkOptions & 0x01)
          g_ucJoinCmdFlags |= BIT4;     // transmit link
        else if(p_pLink->m_ucLinkOptions & 0x02)
          g_ucJoinCmdFlags |= BIT5;     // receive link
      }
    }
    break;

  case DLL_SERV_DEL_LINK :
    {
      C968_DeleteLink_Resp* p_pLink = (C968_DeleteLink_Resp*) p_pData;

//.      if( bInterruptEnabled )
//.      {
//.        DISABLE_DLL_TMRIRQ(); // EXCLUSIVE Code Begin
//.      }
      MONITOR_ENTER();
      uint8 ucLinkIdx = DLL_FindLink( p_pLink->m_ucSuperframeID, p_pLink->m_unNeighborNickname, p_pLink->m_unSlotNumber);
      if( ucLinkIdx != DLL_INVALID_LINK_IDX )
      {
        DLL_DeleteLink( ucLinkIdx );
        p_pLink->m_unRemainingLinksNo = getRemainingLinks();
        COMMIT_MODIFY_DLL_STRUCTS();
      }
//.      if( bInterruptEnabled )
//.      {
//.        ENABLE_DLL_TMRIRQ(); // EXCLUSIVE Code End
//.      }
      MONITOR_EXIT();
      if( ucLinkIdx == DLL_INVALID_LINK_IDX )
      { // error: link not found
        return RCM_E65_LinkNotFound;
      }
    }
    break;

  case DLL_SERV_ADD_CONNECTION :
    {
      C969_WriteGraphNeighbourPair_Resp* p_pConn = (C969_WriteGraphNeighbourPair_Resp*) p_pData;
      if( p_pConn->m_unGraphID < 0x100 )    // a graphID < 256 actually represents a superframe
      { // error: can't add edge to superframe used as graph equivalent
        return RCM_E67_CantAddEdgeWhenSuperframeUsedAsGraphEquiv;
      }
      
      MONITOR_ENTER();
      do
      {
#ifdef BACKBONE_SUPPORT      
        if(p_pConn->m_unNeighborNickname != GW_NICKNAME)  // exception for AccessPoint
#endif
        {
          uint8 ucNeighIdx = DLL_FindShortNeighbor( p_pConn->m_unNeighborNickname );
          if( DLL_INVALID_NEIGH_IDX == ucNeighIdx )
          { // error: neighbor doesn't exist
            ucStatus = RCM_E66_UnknownNickname;
            break;
          }
          if((g_aDllNeighbors[ucNeighIdx].m_bLinked & LinkOptionFlagCodesMask_Transmit) == 0)
          { // error: that neighbor hasn't a Tx link
            ucStatus = RCS_E02_InvalidSelection; // should also restrict to a Normal link???
            break;
          }
        }
        uint8 ucConnIdx = DLL_FindConnection( p_pConn->m_unGraphID, p_pConn->m_unNeighborNickname );
        if( DLL_INVALID_CONN_IDX == ucConnIdx)
        {
          ucStatus = DLL_AddConnection( p_pConn->m_unGraphID, p_pConn->m_unNeighborNickname );
          if( ! ucStatus )
          {
            p_pConn->m_ucRemainingConnectionsNo = getRemainingConnections();
            COMMIT_MODIFY_DLL_STRUCTS();
          }
          else
          {
            ucStatus = RCM_E65_NoMoreEntriesAvailable;
            break;
          }
        }
        else
        { // connection already exists, but could be marked for delete
          g_aDllConnections[ucConnIdx].m_bMarkDeleted = 0;
          p_pConn->m_ucRemainingConnectionsNo = getRemainingConnections();
        }
      } while(0);
      MONITOR_EXIT();
    }

    break;

  case DLL_SERV_DEL_CONNECTION :
    {
      C970_DeleteGraphConnection_Resp* p_pConn = (C970_DeleteGraphConnection_Resp*) p_pData;
      MONITOR_ENTER();
      uint8 ucConnIdx = DLL_FindConnection( p_pConn->m_unGraphID, p_pConn->m_unNeighborNickname );
      if( ucConnIdx != DLL_INVALID_CONN_IDX )
      {
        g_aDllConnections[ucConnIdx].m_bMarkDeleted = 1;
        p_pConn->m_ucRemainingConnectionsNo = getRemainingConnections();
        COMMIT_MODIFY_DLL_STRUCTS();
      }
      MONITOR_EXIT();
      if( ucConnIdx == DLL_INVALID_CONN_IDX )
      { // error: connection not found
        return RCM_E65_EntryNotFound;
      }
    }
    break;

  case DLL_SERV_READ_NETWORK_ID :
    // in Flash Direct sau la Provision
    break;
  case DLL_SERV_WRITE_NETWORK_ID :
    // in Flash Direct sau la Provision
    break;
  case DLL_SERV_WRITE_NETWORK_KEY :
    // in Flash Direct sau la Provision
    memcpy(g_aucNetworkKey, ((C961_WriteNetworkKey_Resp*)p_pData)->m_aKeyValue, 16);
    break;

  case DLL_SERV_READ_TIMEOUT_PERIOD :
    {  
#define p       ((C796_ReadTimerInterval_Resp*)p_pData)

      if(p->m_ucTimerType >= WirelessTimerCode_NoOfEntries)  // table 43
        return RCM_E65_InvalidTimerType;
      
      const TIME_MIN_MAX_T *pTab = &c_tTMinMax[p->m_ucTimerType];
  
      if(p->m_ucTimerType == WirelessTimerCode_Discovery && *pTab->pStore == 0xFFFFFFFF)
      { // discovery links are not serviced, discovery transmissions are not generated
        p->m_ulTimerInterval = 0xFFFFFFFF;     
      }
      else
      { // m_ulTimerInterval in [ms] units -see command description
        p->m_ulTimerInterval = *pTab->pStore * pTab->unFactor;    // convert [TIME_10MS] or [TIME_250MS] to [ms]
      }
#undef p
    }
    break;

  case DLL_SERV_WRITE_TIMEOUT_PERIOD :
  {
#define p       ((C795_WriteTimerInterval_Resp*)p_pData)
    
    if(p->m_ucTimerType >= WirelessTimerCode_NoOfEntries)  // table 43
      return RCM_E65_InvalidTimerType;
    
    // m_ulTimerInterval in [ms] units -see command description
    uint32_t ulTimerInterval = p->m_ulTimerInterval;
    const TIME_MIN_MAX_T *pTab = &c_tTMinMax[p->m_ucTimerType];
    uint8 ucIdx=0;

    if(p->m_ucTimerType == WirelessTimerCode_Discovery && ulTimerInterval == 0xFFFFFFFF)
    { // discovery links are not serviced, discovery transmissions are not generated
      MONITOR_ENTER();
      *pTab->pStore = ulTimerInterval;     
      g_ucDllDiscoveryTX = 0;
      MONITOR_EXIT();
      break;
    }
    // rounded integer division
    if(ulTimerInterval < ((uint32)(-pTab->unFactor)) )
      ulTimerInterval += pTab->unFactor >> 2; // unFactor is a multiple of 2 (10 or 250)
    ulTimerInterval /= pTab->unFactor;                  // convert [ms] to [TIME_10MS] or [TIME_250MS]
    
    if(ulTimerInterval < pTab->ulMinLimit)
      ulTimerInterval = pTab->ulMinLimit;               // limit to min. interval
    else if(ulTimerInterval > pTab->ulMaxLimit)
      ulTimerInterval = pTab->ulMaxLimit;               // limit to max. interval
    MONITOR_ENTER();
    *pTab->pStore = ulTimerInterval;                    // store the new (adjusted) value
    switch(p->m_ucTimerType)                            // make the change effective, replacing the old timer schedule
    {
    case WirelessTimerCode_Discovery:
      g_ulDllDiscoveryCntr = MACA_RANDOM % ulTimerInterval;
      break;
    case WirelessTimerCode_Advertisment:
      g_ulDllAdvCntr = ulTimerInterval;
      break;
//    case WirelessTimerCode_Keep_Alive:    // nothing to do here
//      break;
    case WirelessTimerCode_PathFailure:
      for(DLL_NEIGHBOR_T * pNeighbor = g_aDllNeighbors; pNeighbor < ( g_aDllNeighbors + g_ucDllLkdNeighborsSize); pNeighbor++ )
      {
        pNeighbor->m_tmPathFailCounter = ulTimerInterval;
      }
      break;
    case WirelessTimerCode_HealthReport:
      g_ulHealthReportTimer = ulTimerInterval;
      break;
    case WirelessTimerCode_BroadcastReply:
      {
        uint32 ulDividedInt = ulTimerInterval/25;
        for(ucIdx=1; ucIdx < MAX_TRANSPORT_NO; ucIdx += 2) // for all odd entries
        {
          if( (TRUE == g_aTransportTable[ucIdx].m_bBroadcast) && (g_aTransportTable[ucIdx].m_unResponseTimer > ulDividedInt) )
          {
            g_aTransportTable[ucIdx].m_unResponseTimer = (uint16)(MACA_RANDOM % ulDividedInt); //[250ms]
          } 
        }
      }
      break;
#ifndef BACKBONE_SUPPORT       
    case WirelessTimerCode_MaximumPDUAge:
      g_stLoopDetect.m_unToNextResetTmr = (uint16)ulTimerInterval >> 6;
      // what about pSession->m_ulKeepOldKeyTime???
      break;
#endif      
    }
    MONITOR_EXIT();
    ulTimerInterval *= pTab->unFactor;                  // convert [TIME_10MS] or [TIME_250MS] back to [ms] to compose the response
    
    if(ulTimerInterval != p->m_ulTimerInterval)
    {
      p->m_ulTimerInterval = ulTimerInterval;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
#undef p
  }
    break;

  case DLL_SERV_READ_CAPACITIES :
    * ((uint16 *) p_pData)     = DLL_MAX_SUPERFRAMES;
    * ((uint16 *) p_pData + 1) = DLL_MAX_LINKS;
    * ((uint16 *) p_pData + 2) = DLL_MAX_NEIGHBORS;
    * ((uint16 *) p_pData + 3) = DLL_MAX_PACKET_BUFFERS;
    break;

  case DLL_SERV_READ_PRIORITY_THRESHOLD :
#define p       ((C812_ReadPacketReceivePriority_Resp*)p_pData)
    p->PacketRecPriority = g_ucDllPriorityThreshold;
#undef p
    break;

  case DLL_SERV_WRITE_PRIORITY_THRESHOLD :
#define p       ((C813_WritePacketReceivePriority_Resp*)p_pData)
    if(p->PacketRecPriority > DLL_MAX_RCV_PACKET_PRIORITY) // spec155r1.1
    {
      p->PacketRecPriority = DLL_MAX_RCV_PACKET_PRIORITY;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    g_ucDllPriorityThreshold = p->PacketRecPriority;
#undef p
    break;

  case DLL_SERV_READ_JOIN_PRIORITY :
#define p       ((C810_ReadJoinPriority_Resp*)p_pData)
    p->JoinPriority = g_ucDllJoinPriority;
#undef p
    break;

  case DLL_SERV_WRITE_JOIN_PRIORITY :
#define p       ((C811_WriteJoinPriority_Resp*)p_pData)
    if(p->JoinPriority > DLL_MAX_JOIN_PRIORITY)  // spec155r1.1
    {
      p->JoinPriority = DLL_MAX_JOIN_PRIORITY;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    // lower number indicates a better choice for joining
    g_ucDllJoinPriority    = p->JoinPriority;
#undef p
    break;

  case DLL_SERV_READ_PROMISCUOUS_MODE :
    // TBD is optional
    break;
  case DLL_SERV_WRITE_PROMISCUOUS_MODE :
    // TBD is optional
    break;
  case DLL_SERV_READ_MAX_BACKOFF_EXPONENT :
#define p       ((C819_ReadBackOffExponent_Resp*)p_pData)
    p->MaxBackOffExp = g_ucDllMaxBackoffExponent;
#undef p
    break;

  case DLL_SERV_WRITE_MAX_BACKOFF_EXPONENT :
#define p       ((C820_WriteBackOffExponent_Resp*)p_pData)
    // spec075r1.1 - valid values for Back-off exponent used in shared slots are: 4, 5, 6, 7
    if(p->MaxBackOffExp < DLL_MIN_BACKOFF_EXPONENT)
    {
      p->MaxBackOffExp   = DLL_MIN_BACKOFF_EXPONENT;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    else if(p->MaxBackOffExp > DLL_MAX_BACKOFF_EXPONENT)
    {
      p->MaxBackOffExp   = DLL_MAX_BACKOFF_EXPONENT;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    g_ucDllMaxBackoffExponent = p->MaxBackOffExp;
#undef p
    break;

  default:
    break;
  }
  return ucStatus;
}


////////////////////////////////////////////////////////////////////////////////
// numarul de retries ? fix pentru toate mesajele la fel ?
//   initializat automat la o valoare predefinita
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_TX_Request(
                      uint16    p_unHandle,           // Packet ID
                      uint16    p_unPacketBirthASN,   // based on ASN Snippet in NL
                      uint8     p_ucPayloadLen,
                      uint8   * p_pucPayload,
                      DLL_PACKET_PRIORITY_T p_ucPacketPriority,
                      DLL_DEST_TYPE_T       p_ucDestType,
                      DLL_PACKET_TYPE_T     p_ucPacketType,
                      void    * p_pDestAddr
                    )
{
  if(g_ucDllDisconnect && (g_unDiscPktHandle != p_unHandle)) // while disconnecting, refuse any packet from app, except "initiating" command response
      return DLL_OUT_OF_MEMORY; // !!! Error status returns
  
  g_sDllPacket.m_unPacketID = p_unHandle;
  g_sDllPacket.m_unGraphID  = DLL_INVALID_GRAPH_ID;
  g_sDllPacket.m_unPacketBirthASN = p_unPacketBirthASN;
  g_sDllPacket.m_ucSuperframeID = DLL_INVALID_SF_ID;
  g_sDllPacket.m_bMarkDeleted = 0;
  g_sDllPacket.m_bNoRetries = DLL_MAX_RETRIES;
  g_sDllPacket.m_ucTxTimeouts = 0;
  // m_ePacketDestType
  g_sDllPacket.m_stAddrSpec.m_bSourceLongAddr = 0;
  g_sDllPacket.m_stAddrSpec.m_bDestLongAddr = 0;
  g_sDllPacket.m_stDLDPUSpec.m_bPriority = p_ucPacketPriority;
  g_sDllPacket.m_stDLDPUSpec.m_bEncryptKey = DLL_KEY_NETW;
  g_sDllPacket.m_stDLDPUSpec.m_bPacketType = p_ucPacketType;
  g_sDllPacket.m_ucDllPayloadLen = p_ucPayloadLen;
  memcpy( g_sDllPacket.m_aSourceAddr, &g_unNickname, 2);
  memcpy( g_sDllPacket.m_aDestAddr, &c_aBcastAddr, 2);
  memcpy( g_sDllPacket.m_aucDllPayload, p_pucPayload, p_ucPayloadLen);

  switch ( p_ucDestType )
  {
  case DLL_DEST_TYPE_PROXYSHORT_ADDR :                                    
    g_sDllPacket.m_ePacketDestType              = DLL_DEST_TYPE_LONG_ADDR;      // this type of packet will only be sent on join links (first packet after JoinResp)
    memcpy( g_sDllPacket.m_aDestAddr, (uint8 *) p_pDestAddr, 2);                // dest nickname
    break;
  case DLL_DEST_TYPE_LONG_ADDR :                                                
    g_sDllPacket.m_ePacketDestType              = DLL_DEST_TYPE_LONG_ADDR;      // this type of packet will only be sent on join links
    g_sDllPacket.m_stDLDPUSpec.m_bEncryptKey    = DLL_KEY_WELL_KNOWN;           // destinatia nu poate decat cu well known key
    g_sDllPacket.m_stAddrSpec.m_bDestLongAddr = 1;
    memcpy( g_sDllPacket.m_aDestAddr, (uint8 *) p_pDestAddr, 8);
    break;
  case DLL_DEST_TYPE_SHORT_ADDR :
    if ( g_unNickname == 0xFFFF )
    {                                                                           // in accest caz nu pot comunica decat pe join links
      g_sDllPacket.m_ePacketDestType              = DLL_DEST_TYPE_LONG_ADDR;    // Caz PARTICULAR (Join Request) !
      g_sDllPacket.m_stDLDPUSpec.m_bEncryptKey    = DLL_KEY_WELL_KNOWN;
      g_sDllPacket.m_stAddrSpec.m_bSourceLongAddr = 1;
      memcpy( g_sDllPacket.m_aSourceAddr, g_aJoinDevUID_LE, 5 );
      memcpy( g_sDllPacket.m_aSourceAddr + 5, c_aHCF_OUI_LE, 3 );
      memcpy( g_sDllPacket.m_aDestAddr, (uint8 *) p_pDestAddr, 2);
    }
    else
    {
      g_sDllPacket.m_ePacketDestType              = DLL_DEST_TYPE_SHORT_ADDR;
      memcpy( g_sDllPacket.m_aDestAddr, (uint8 *) p_pDestAddr, 2);
    }
    break;
  case DLL_DEST_TYPE_GRAPH :
    g_sDllPacket.m_ePacketDestType              = DLL_DEST_TYPE_GRAPH;
    g_sDllPacket.m_unGraphID                    = IntFromLE(p_pDestAddr);
    break;
  case DLL_DEST_TYPE_SUPERFRAME :
    g_sDllPacket.m_ePacketDestType              = DLL_DEST_TYPE_SUPERFRAME;
    g_sDllPacket.m_ucSuperframeID               = * (uint8 *) p_pDestAddr;
    g_sDllPacket.m_bNoRetries                   = 1;
    break;
  }

  return DLL_AddPktToQueue( &g_sDllPacket );
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//void DLL_TX_Confirm( uint16 p_unHandle, uint8  p_ucStatus )
//{
//
//  return; //
//}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
uint8 DLL_FlushRequest( uint16 p_unHandler )
{
  uint8 ucPktFound=0xFF; // 0xFF -> packet doesn't exist on DLL queue; 0x01 -> packet exist, flush it

  DLL_PACKET_T *pQueue = g_aDllQueue;
  for(; pQueue < (g_aDllQueue + g_ucDllAllQueueSize); pQueue++)
  {
    if( p_unHandler == pQueue->m_unPacketID )
    {
      pQueue->m_bMarkDeleted = 1;
      ucPktFound = 1;
    }
  }

  return ucPktFound;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_FlushConfirm( uint16 p_unHandler, uint8 p_ucStatus)
{ // in principiu nu o sa fie nevoie de aceasta functie, va fi un singur FlushConfirm in NET

  return; //
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_DisconnectIndicate()
{

  return; //
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//void DLL_NeighborIndicate()
//{
//  UART1_String("DNNi"); // DLL Net event: Neighbor Indicate
//  if(NET_STATE_QUARANTINED == g_ucNetState || NET_STATE_OPERATIONAL == g_ucNetState)
//  {
//    ;  // TBD
//  }
//  return; //
//}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DLL_ReceiveIndicate()
{

  return; //
}


///////////////////////////////////////////////////////////////////////////////////
// Name:          PHY_DataConfirm
// Author:        NIVIS LLC,
// Description:   Callback function. It is called either by PHY_TX_Request or by MACA_Interrupt
//                after a transmission was requested.
// Parameters:    unsigned char p_ucStatus  = PHY_DATA_SUCCES, TRANCEIVER_BUSY, PHY_DATA_OTH_ERROR (CCA)
// Return:        none
// Obs:           Access level: interrupt
///////////////////////////////////////////////////////////////////////////////////
void PHY_DataConfirm (uint8 p_ucStatus)
{
  if( p_ucStatus == PHY_DATA_SUCCES ) // on succes
  {
    if ( g_ucDllXVRStatus == DLL_XVR_TX )
    {
      if ( g_sDllSlot.m_ucNeedACK  )
      {
        g_ucDllXVRStatus = DLL_XVR_RX_ACK;
        uint16 unDelay = DLL_TS_RX_ACK_DELAY - (uint16)PHY_GetLastTXuSecAge() - DLL_TO_PHY_RX; // nu este luata in calcul latenta la PHY_GetLastTXuSecAge ... probabil va incepe RxAck cu vreo 20~30 microsecunde mai devreme dar cred ca e ok asa
        if((int16)unDelay < 0)
          unDelay = 0;
        PHY_RX_Request( g_sDllSlot.m_ucChannel, unDelay, RX_ACK_ABORT_TIMEOUT );
        return;
      }
      else if ( g_sDllSlot.m_pQueue )
      {
        DLL_RemCrtPktFromQueue();
      }
    }
  }
  
  //just on DLL_JOINED state the hibernation is accepted  
  g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP
  
  PHY_Disable_Request(PHY_IDLE);
  g_ucDllXVRStatus = DLL_XVR_IDLE;
}


#ifdef TEST_HCFJD                                                               // TEST - doinel.alban
  const uint8 acHCFData[106] = {
  //  0x6C,
    0x41, 0x8C, 0xCB, 0x01, 0xE0, 0x01, 0x00, 0x00, 0x70, 0xF8, 0x1E, 0x1B, 0x00, 0x01, 0x00, 0x37,
    0x84, 0x7E, 0x0A, 0xB0, 0x00, 0x01, 0x00, 0x1B, 0x1E, 0xF8, 0x70, 0x00, 0x00, 0x01, 0xF9, 0x80,
    0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xDD, 0xC7, 0x6B, 0x56, 0xE9, 0x88, 0xCE, 0xF0, 0x6D,
    0xC0, 0x2A, 0x45, 0xB1, 0x38, 0x13, 0xE4, 0x49, 0xBF, 0x98, 0x5C, 0x08, 0x0C, 0x28, 0xA1, 0x56,
    0xD0, 0xA4, 0xDF, 0x53, 0x32, 0x10, 0x56, 0x94, 0x57, 0xB5, 0xD8, 0x35, 0x69, 0x1E, 0x2F, 0x9E,
    0xA2, 0xE4, 0xDD, 0x78, 0xE8, 0xD8, 0xBC, 0x9C, 0x96, 0x6D, 0x11, 0x50, 0x40, 0x36, 0xD8, 0x31,
    0x4A, 0x81, 0xF4, 0x8A, 0x9B, 0x04, 0x22, 0x35, 0x7A, 0x61  //, 0xD9, 0xE6  - PHY CRC
  };
  const uint8 ucHCFLen = 106;
#endif // TEST_HCFJD


///////////////////////////////////////////////////////////////////////////////////
// Name:          PHY_DataIndicate
// Author:        NIVIS LLC,
// Description:   Callback function. It is called by MACA_Interrupt after a data frame
//                or ACK packet was successfully received.
// Parameters:    uint8 p_ucRSL     = received signal level
//                uint8 * p_pucData = PHY reception buffer
//                uint8 p_ucLen     = length of received packet, without CRC
// Return:        none
// Obs:           Access level: MACA interrupt
//                Context: transceiver is in IDLE state
///////////////////////////////////////////////////////////////////////////////////
void PHY_DataIndicate( uint8 p_ucRSL, uint8 *p_pucData, uint8 p_ucLen )
{
  volatile uint32 ulClock;
  uint8 ucCodeForAck;
  uint8 ucNeighborIndex = DLL_INVALID_NEIGH_IDX;
  
  // For Freescale MC1322x only: 
  // p_ucRSL represents LQI (0 for values <= -100dBm, 255 for values >= -15 dBm)
//  int8  cRSLdBm = (((int16)p_ucRSL * 85) / 255) - 100; // [-100dBm to -15dBm] 
  int8  cRSLdBm = GET_LAST_RSSI(); // [-100dBm to -15dBm] 

  memcpy(g_aucDllBuffer, p_pucData, p_ucLen);                                   // copy msg to DLL buffer
  g_ucDllBufferLen = p_ucLen;

  if ( DLL_AuthIncomingPkt() )  // check out DLL header and authenticate the packet
  {
    if(g_ucDllStatus == DLL_ACTIVE_SEARCH)
      MACA_ResetWatchDog();
    
    //just on DLL_JOINED state the hibernation is accepted  
    g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP
    
    return; // ... discard packet
  }
  MACA_ResetWatchDog();

#ifdef TEST_HCFJD                                                               // TEST - doinel.alban
  if ( g_sDllBufferParsed.m_bDstAddrLong && ( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_DATA ) )
  { // LONG DestAddress - Join Response
    memcpy(g_aucDllBuffer, acHCFData, ucHCFLen);
    g_ucDllBufferLen = ucHCFLen;

    if ( DLL_AuthIncomingPkt() )
    {
        //just on DLL_JOINED state the hibernation is accepted  
        g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP

        return; // ... discard packet
    }
  }
#endif // TEST_HCFJD

  // Parse message ... variable ucCodeForAck forced temporary for other purpose ... keeping here if parsing was ok !
  ucCodeForAck = DLL_ParseIncomingPkt();

  // Process Neighbor
  if ( ! g_sDllBufferParsed.m_bSrcAddrLong )
  { // SHORT Address Source
    uint16 unSrcAddr = IntFromLE(g_sDllBufferParsed.m_pucSrcAddr);
    
    // !! ucNeighborIndex and pNeighbor must be both invalid values, or designate the same neighbor!!
    ucNeighborIndex = DLL_FindShortNeighbor( unSrcAddr );
    if ( ucNeighborIndex >= DLL_MAX_NEIGHBORS )
    {
      // new neighbor ... isn't in g_aDllNeighbors table
      if ( DLL_AddShortNeighbor( unSrcAddr ) )
      { // there is no non-linked neighbor in the table
        if ( g_ucDllStatus != DLL_JOINED )  //when joined, the received packet must be processed even if no more room in Neighbor Table
        {
            //just on DLL_JOINED state the hibernation is accepted  
            g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP

            return; // ... discard packet
        }
      }
      //DLL_NeighborIndicate();  // ? cu ce parametru se va apela
      ucNeighborIndex = DLL_FindShortNeighbor( unSrcAddr );
    }

    // update neighbor statistics ...
    if ( ucNeighborIndex < g_ucDllAllNeighborsSize )
    { // update neighbor statistics ... even the message is not for me ( this is an authenticated message )
//      if(!g_aDllNeighbors[ucNeighborIndex].m_bLinked)
      if(( ucNeighborIndex >= g_ucDllLkdNeighborsSize))
      { // Last Time Communicated for non-linked neighbors - updated upon each reception
//        g_aDllNeighbors[ucNeighborIndex].m_tmPathFailCounter = g_stProv.ulDllPathFail; // not need to be updated each time until the neighbor it's not linked
        g_aDllNeighbors[ucNeighborIndex].m_tmLastComm = g_sDllASN.m_ulLsb;
        if ( g_aDllNeighbors[ucNeighborIndex].m_cAvgRSL < cRSLdBm )
        {
          g_aDllNeighbors[ucNeighborIndex].m_cAvgRSL = cRSLdBm; // keep max RSL
        }
      }
      else if ( g_aDllNeighbors[ucNeighborIndex].m_cAvgRSL != -128)
      {
        g_aDllNeighbors[ucNeighborIndex].m_cAvgRSL = g_aDllNeighbors[ucNeighborIndex].m_cAvgRSL + ( ((int16)cRSLdBm - g_aDllNeighbors[ucNeighborIndex].m_cAvgRSL) / DLL_RSL_DAMP );
      }
      else
      { // a linked neighbor just added by Cmd967_WriteLink has initial RSL = -128
        g_aDllNeighbors[ucNeighborIndex].m_cAvgRSL = cRSLdBm;
      }
    }
  }

  // Parse message   ... variable ucCodeForAck released for base purpose
  if ( ucCodeForAck )
  {               // ... parsed message was not for me

      //just on DLL_JOINED state the hibernation is accepted  
      g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP

      return;       // ... discard packet
  }

  // packet for me, update linked neighbor statistics ...
  if ( ucNeighborIndex < g_ucDllLkdNeighborsSize)
  {
//    if( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_ACK )
    if( (g_sDllSlot.m_pLink->m_eLinkType == DLL_LINK_TYPE_NORMAL) 
       ||
       ( g_ucNetState == NET_STATE_JOINING && g_sDllSlot.m_pLink->m_eLinkType == DLL_LINK_TYPE_JOIN ) )
    { // update these only if I'm sure that this is a scheduled communication, not a hazardous received packet
//      g_aDllNeighbors[ucNeighborIndex].m_tmPathFailCounter = g_stProv.ulDllPathFail;  // not update it upon bcast packets, this will force the device to send KA
      
      if( !g_sDllBufferParsed.m_ucBroadcast ) 
      { // only if not broadcast 
        g_aDllNeighbors[ucNeighborIndex].m_tmPathFailCounter = g_stProv.ulDllPathFail;
        g_aDllNeighbors[ucNeighborIndex].m_tmLastComm = g_sDllASN.m_ulLsb;
        if(g_aDllNeighbors[ucNeighborIndex].m_bTimeSource & NeighbourFlagsMask_TimeSource)
          g_ulTimeSourceTimer = RTC_COUNT + TIME_SOURCE_TIMEOUT;    // used to find out when the device has lost its time sources
      }
    }

//    if ( g_sDllBufferParsed.m_ucBroadcast )
//    {
//      g_aDllNeighbors[ucNeighborIndex].m_unBroadcastsReceived++;
//    }
//    else 
    if( g_sDllBufferParsed.m_bPacketType != DLL_PACKET_TYPE_ACK )
    { // Packets Received - for me or bcast, non-ACK
      g_aDllNeighbors[ucNeighborIndex].m_unPacketsReceived++;
    }
  }

  switch ( g_ucDllStatus )
  {
  case DLL_ACTIVE_SEARCH :                                                      // packet arrived while I am in Active Search

    if ( ( DLL_DISCONNECT_INACTIVE == g_ucDllDisconnect ) && 
         ( ucNeighborIndex < g_ucDllAllNeighborsSize ) && 
         ( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_ADVERTISE ) )
    { // Received FIRST ADV msg
      DEBUG2_ON(); DEBUG2_OFF(); DEBUG2_ON(); DEBUG2_OFF();
      if ( DLL_ParseAdvertisePkt(ucNeighborIndex) )
      {
        return; // ... discard packet
      }
      DEBUG2_ON(); DEBUG2_OFF(); DEBUG2_ON(); DEBUG2_OFF();
      // prepare clock correction to sycronize with network
      ulClock = DLL_TS_TX_OFFSET; 
      ulClock += PHY_GetLastRXuSecAge(); // ulClock in uSec for now
      DLL_TIMER_STOP();            
      DLL_TIMER_SET_CNTR( ulClock * 6 );
      DLL_TIMER_START();
      ulClock = USEC_TO_RTC( ulClock );
      ulClock += (uint32)g_stDllPredSlot.m_uc250msSlotNo*983/3;                 // 983 = 327 + 2*328
      RtcTmr_SetCounter( ulClock );   
      DEBUG4_ON(); DEBUG4_OFF();  // pulse on DEBUG(Tx) line at clock sync moment
      //PUT_DLL_SYNCHRONIZED();   // not so fast; wait to hear several advertisements and get synchronized with the last of them                                              
      
      DLL_AdvertiseIndicate(ucNeighborIndex);                                   // Generate ADVERTISE_INDICATE for the Network Layer
    }
    break;

  case DLL_PASSIVE_SEARCH:
    // TBD
    break;

  case DLL_SYNCHRONIZED:

    if ( ( DLL_DISCONNECT_INACTIVE == g_ucDllDisconnect ) && 
         ( ucNeighborIndex < g_ucDllAllNeighborsSize ) )
    {
      switch (  g_sDllBufferParsed.m_bPacketType )
      {
      case DLL_PACKET_TYPE_ADVERTISE:
        DEBUG2_ON(); DEBUG2_OFF(); DEBUG2_ON(); DEBUG2_OFF();
        if ( DLL_ParseAdvertisePkt(ucNeighborIndex) )
        {
            //just on DLL_JOINED state the hibernation is accepted  
            g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP

            return; // ... discard packet
        }
        DEBUG2_ON(); DEBUG2_OFF(); DEBUG2_ON(); DEBUG2_OFF();
        UART1_String("DJAi");
        DLL_AdvertiseIndicate(ucNeighborIndex);
      case DLL_PACKET_TYPE_KEEPALIVE:
      case DLL_PACKET_TYPE_DATA:
        if ( g_aDllNeighbors[ucNeighborIndex].m_bTimeSource & NeighbourFlagsMask_TimeSource )
        {
          int16  nClkDiff  = - ( (int16) ( ( (uint32) ( TMR0_CNTR / 6 ) -  PHY_GetLastRXuSecAge() ) - DLL_TS_TX_OFFSET ) ); // in microseconds
          DEBUG4_ON(); DEBUG4_OFF();  // pulse on DEBUG(Tx) line at clock sync determination
          RtcTmr_Set250msCorrection( ( USEC_TO_RTC(nClkDiff) ) );          
        }
      }
    }
    break;

  case DLL_JOINED:

    switch( g_ucDllXVRStatus )
    {
    case DLL_XVR_RX_ACK :                                                       // am primit un ACK
      if( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_ACK 
         && (g_sDllBufferParsed.m_ucPayloadLen == (3+4)) )    // ACK payload + DLL MIC len
      {  
        // Clock corection ... if needed
        if ( ucNeighborIndex < g_ucDllLkdNeighborsSize )
        {
          if ( g_aDllNeighbors[ucNeighborIndex].m_bTimeSource & NeighbourFlagsMask_TimeSource )
          {
            int16 nTics = (int16) ( ((uint16)g_sDllBufferParsed.m_pucDLPDUPayload[1] << 8) | g_sDllBufferParsed.m_pucDLPDUPayload[2] );
            DEBUG4_ON(); DEBUG4_OFF();  // pulse on DEBUG(Tx) line at clock sync determination
            RtcTmr_Set250msCorrection( ( - USEC_TO_RTC(nTics) ) );
          }
        }
        // Ack Interpret
        if ( g_sDllBufferParsed.m_pucDLPDUPayload[0] == DLL_ACK_SUCCES )
        {
          if ( g_sDllSlot.m_pQueue )
          {
              DLL_RemCrtPktFromQueue();
            
//            #ifndef BACKBONE_SUPPORT 
//              g_ucConsecutiveNoAckCtr = 0;
//            #endif
          }
          // not needed since no retry on DLL-generated packets
//        else
//        { // DLL packet
//          if ( g_sDllSlot.m_ucPacketType == DLL_PACKET_TYPE_KEEPALIVE )
//          { // was KA or Discovery
//            if ( g_sDllSlot.m_pLink->m_eLinkType == DLL_LINK_TYPE_DISCOVERY )
//              g_ucDllDiscoveryTX = 0;
//            //else
//              //g_aDllNeighbors[ucNeighborIndex].m_ucKeepAliveTX = 0;
//          }
//          else if ( g_sDllSlot.m_ucPacketType == DLL_PACKET_TYPE_ADVERTISE )
//          { // was ADV
//            g_ucDllAdvertiseTX = 0;
//          }
//        }
        }
      }
      g_ucDllXVRStatus = DLL_XVR_IDLE;
      break;
    case DLL_XVR_RX :                                                           // am primit un mesaj
      g_nDllClkDiffAck = - ( (int16) ( ( (uint32) ( TMR0_CNTR / 6 ) -  PHY_GetLastRXuSecAge() ) - DLL_TS_TX_OFFSET ) ); // in microseconds
      DEBUG4_ON(); DEBUG4_OFF();  // pulse on DEBUG(Tx) line at clock sync determination
      if ( ( ucNeighborIndex < g_ucDllLkdNeighborsSize ) && ( g_aDllNeighbors[ucNeighborIndex].m_bTimeSource & NeighbourFlagsMask_TimeSource ) )
      {
        RtcTmr_Set250msCorrection( ( USEC_TO_RTC(g_nDllClkDiffAck) ) );
      }
      if ( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_DATA )
      {
        if ( DLL_DISCONNECT_INACTIVE == g_ucDllDisconnect )
        {
          //call the NET indicate function
          ucCodeForAck = NET_TransmitIndicate( g_sDllBufferParsed.m_ucBroadcast,
                                               g_sDllBufferParsed.m_bPriority,
                                               g_sDllBufferParsed.m_ucPayloadLen,
                                               g_sDllBufferParsed.m_pucDLPDUPayload );
        }
        else
        { // sunt in disconnecting :
          ucCodeForAck = 61; // ??? code !!! nu mai primesc nici un fel de packet de la vecini cand fac disconnect
        }
      }
      else if ( g_sDllBufferParsed.m_bPacketType == DLL_PACKET_TYPE_DISCONNECT )
      { // ... indiferent daca eu sunt sau nu in disconnecting :
        if ( ucNeighborIndex < g_ucDllAllNeighborsSize )
        {          
          g_aDllNeighbors[ucNeighborIndex].m_bDisconnected = 1; // sterge-i toate linkurile
          DLL_DisconnectIndicate();                             // notifica NL
          COMMIT_MODIFY_DLL_DISCONNECT();
          ucCodeForAck = DLL_ACK_SUCCES;                        // dai ack succes.          
        }
      }  
      else
      {
        ucCodeForAck = DLL_ACK_SUCCES;
      }
      
//#ifdef NO_ACK
      #ifdef BACKBONE_SUPPORT       
      // TR gives no ACK for a message from an unlinked neighbor, excepting JoinRequest and the other packets received on Join links       
//      if( (ucNeighborIndex >= g_ucDllLkdNeighborsSize) && (g_sDllSlot.m_pLink->m_eLinkType != DLL_LINK_TYPE_JOIN) && !g_sDllBufferParsed.m_bSrcAddrLong ) 
      if( !(g_aDllNeighbors[ucNeighborIndex].m_bTimeSource & 0x80 ) && (g_sDllSlot.m_pLink->m_eLinkType != DLL_LINK_TYPE_JOIN) && !g_sDllBufferParsed.m_bSrcAddrLong )
      { // not from an active (linked) neighbor, nor from a joining device, so don't send back ACK
        ucCodeForAck = 0xFF;  
      }
      #endif     
//#endif     
            
      if ( ( g_sDllBufferParsed.m_ucNeedAck ) && ( ucCodeForAck != 0xFF ) )     // 0xFF = invalid error code from net_trasmit_indicate
      { // ... trebuie sa dau ACK
        DLL_ComposeAckPkt(ucCodeForAck);
        g_ucDllXVRStatus = DLL_XVR_TX_ACK;
        uint16 unDelay = DLL_TS_TX_ACK_DELAY - (uint16)PHY_GetLastRXuSecAge() + (uint16)(p_ucLen + 8)*32; // DLL_TO_PHY_TX compensat cu latenta de la PHY
        if((int16)unDelay < 0)
          unDelay = 0;
        PHY_TX_Request(g_sDllSlot.m_ucChannel, unDelay , 0, g_aucDllAckBuffer, g_ucDllAckBufferLen); // + 2 pentru CRC
        return;
      }
      else
      {
        g_ucDllXVRStatus = DLL_XVR_IDLE;
      }
      break; // poate fi scos
    }
    break; // poate fi scos
  }

    //just on DLL_JOINED state the hibernation is accepted  
    g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP
}



///////////////////////////////////////////////////////////////////////////////////
// Name:          PHY_ErrorIndicate
// Author:        NIVIS LLC,
// Description:   Callback function
// Parameters:    unsigned char p_ucStatus  =
//                unsigned char * p_pucData =
// Return:        none
// Obs:
///////////////////////////////////////////////////////////////////////////////////
void PHY_ErrorIndicate(uint8 p_ucStatus, uint8 *p_pucData)
{
  PHY_Disable_Request(PHY_IDLE);
  if(p_ucStatus == RX_CRC_ERROR)
  {
    g_stC779.m_ucNoOfCRCErrorsDetected++; // inc Cmd779 counter
  }
  
  //just on DLL_JOINED state the hibernation is accepted  
  g_stDllPredLink.m_unSlotsUntilWakeUp = 1;  //go to sleep mode ASAP
  
  g_ucDllXVRStatus = DLL_XVR_IDLE;
}


