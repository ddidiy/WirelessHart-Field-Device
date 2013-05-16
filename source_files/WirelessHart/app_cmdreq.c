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
* Name:         app_cmdreq.c
* Author:       Nivis LLC
* Date:         August 2008
* Description:  Application Layer, HART commands: compose the request, parse the response
* Changes:
* Revisions:
****************************************************************************************************/

#include <string.h>
#include "../MC1322x.h"
#include "tmr_util.h"
#include "app_layer.h"
#include "app_cmdreq.h"

#define SRVCREQ_TIMEOUT   (1UL*32768)  // 1s 
#define REQNM_MAX_MSG_AGE    SEC_TO_FRACTION2( 3600 )  

#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
  #define REQNM_SEND_DELAY     SEC_TO_FRACTION2( 1 )  //1 sec
# else
  #define REQNM_SEND_DELAY     SEC_TO_FRACTION2( 10 )  //10 sec
#endif

REQNM_QEUE_T   g_aReqNMQueue[REQNM_QUEUE_SIZE];

void REQNM_Task(void)
{
  static uint8 ucCrtIdx = 0;
  uint8 ucFound = 0; // flag indicating that a sendable request has already been reached in the queue
  
  for(uint8 ucIdx=0; ucIdx<REQNM_QUEUE_SIZE; ucIdx++)
  {
    uint8 ucTmpIdx = (ucCrtIdx + ucIdx) % REQNM_QUEUE_SIZE;
    REQNM_QEUE_T* pQueue = g_aReqNMQueue + ucTmpIdx;
  
    if(REQNM_WAIT == pQueue->m_ucStatus)
    {
      if( (RTC_COUNT - pQueue->m_ulTimestamp) < REQNM_MAX_MSG_AGE )
        return; // still waiting for a response from NM
      // else abort the request
      pQueue->m_ucStatus = REQNM_ABORT;
    }
    if(REQNM_ABORT == pQueue->m_ucStatus)
    { // see wether the cmd was FromDR or not, and send a (DR_DEAD) response to the initiator (GW) if necessary
      DR_SLAVE_BUFFER_STRUCT* pDRBuf = pQueue->m_pDRBuf;
      if(NULL != pDRBuf)
      {
        pDRBuf->m_ucRespSize = 4;
        pDRBuf->m_aResp[0] = pDRBuf->m_unCmdId >> 8;
        pDRBuf->m_aResp[1] = pDRBuf->m_unCmdId;
        pDRBuf->m_aResp[2] = 1;                           // data len
        
        if(DRSLAVE_CONFLICT == pDRBuf->m_ucDRStatus)
          pDRBuf->m_aResp[3] = RCS_E36_DelayedResponseConflict; // RC
        else pDRBuf->m_aResp[3] = RCS_E35_DelayedResponseDead; // RC
      
        if(CMDID_C799_RequestService == pQueue->m_unCmdId)
        {
          if((CMDID_C103_WriteBurstPeriod == pDRBuf->m_unCmdId) ||
             (CMDID_C117_WriteEventNotificationTiming == pDRBuf->m_unCmdId))
          {
            g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_CapacityDenied;
          }
            
        }
      }
      else if(CMDID_C799_RequestService == pQueue->m_unCmdId)
      {
        //srvc request has come from srvc task
        g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_CapacityDenied;
      }
   
      memset( pQueue, 0, sizeof(REQNM_QEUE_T));
      //pQueue->m_ucStatus = REQNM_INACTIVE;  // free the queue element
      continue;
    }
    
    if(ucFound)
      continue;
    
    if(REQNM_SEND ==  pQueue->m_ucStatus)
    {
      ucCrtIdx = ucTmpIdx;
      ucFound = 1; 
      if( (RTC_COUNT - pQueue->m_ulTimestamp) < REQNM_SEND_DELAY )
        return; // delay before sending
    }
  }
  
  if(!ucFound)
  {
    ucCrtIdx = 0; // queue empty => next add will be at the begining of the queue
    return;       // nothing to send 
  }  
  
  REQNM_SendMessageToNM(ucCrtIdx);
}

void REQNM_SendMessageToNM(uint8 p_ucIdx)
{
  uint8 aAppData[C799_ReqSize + 2 + 1]; // the longest cmd req + cmdID + dataLen   
  BinaryStream stStream;     
  STREAM_INIT(&stStream, (uint8_t*)aAppData , sizeof(aAppData) );

  ComposeWirelessRequestCommmand((void*)(&g_aReqNMQueue[p_ucIdx].m_stC799Req),  // use any union member's start address
                                         g_aReqNMQueue[p_ucIdx].m_unCmdId, 
                                         &stStream );

  uint8 ucTLCode = TL_TransmitRequest(REQNM_HANDLE_BASE + p_ucIdx,      //p_appHandle
                                      NET_ADDR_SHORT_ADDR,              //p_ucDstType
                                      c_aNetManagerAddr,                //p_pDstAddr
                                      DLL_PACKET_PRIORITY_COMMAND,      //p_ucPriority
                                      0,                                //p_ucServiceID
                                      REQ_UNICAST,                      //p_ucTransportType
                                      stStream.nextByte - aAppData,     //p_ucAppDataLen
                                      aAppData
                                     );
  if(SUCCESS == ucTLCode)
  {
    g_aReqNMQueue[p_ucIdx].m_ucStatus = REQNM_WAIT;
    g_aReqNMQueue[p_ucIdx].m_ulTimestamp = RTC_COUNT;
  }
  //else it will be sent again next time
  
//  if(TL_MAX_RETRIES_EXCEEDED == ucTLCode) // this should be checked in TransmitConfirm()
//  {
//    g_aReqNMQueue[p_ucIdx].m_ucRetryCnt--;
//  }
}

REQNM_QEUE_T* REQNM_GetQueueEntry(uint8* pucIdx)
{
  REQNM_QEUE_T* p = g_aReqNMQueue;
  for(uint8 i = 0; i < REQNM_QUEUE_SIZE; i++, p++)
  {
    if(*(uint8*)p == REQNM_INACTIVE)     // m_ucStatus
    { // m_unCmdId, m_ucDRBufId and m_stCxxxReq will be added by the calling function
      p->m_ucStatus = REQNM_SEND;
      p->m_ucRetryCnt = MAX_REQNM_RETRY_CNT;
      p->m_ulTimestamp = RTC_COUNT - REQNM_SEND_DELAY; // no send delay in this case
      *pucIdx = i;
      return p;
    }
  }
  return NULL;              // queue full
}

REQNM_QEUE_T* REQNM_FindSrvcCmdPending(uint8 ucSrvcId)
{
  REQNM_QEUE_T* p = g_aReqNMQueue;
  for(uint8 i = 0; i < REQNM_QUEUE_SIZE; i++, p++)
  {
    if((REQNM_SEND == p->m_ucStatus) || (REQNM_WAIT == p->m_ucStatus))
      if( ((CMDID_C799_RequestService == p->m_unCmdId) && (ucSrvcId == p->m_stC799Req.m_ucServiceId)) ||
          ((CMDID_C801_DeleteService == p->m_unCmdId) && (ucSrvcId == p->m_stC801Req.m_ucServiceId))
        )
      {
        return p;
      }
  }
  return NULL;
}

#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
uint16 Process_SrvcReq(uint16 unCmdId, uint8 ucServiceId, uint32* ulPeriod)
{
  //if(REQNM_FindSrvcCmdPending(ucServiceId))
    //return RCS_E32_Busy;
 
        
  DR_SLAVE_BUFFER_STRUCT* pDRBuf = findDRBufByCmdId(unCmdId);
  if(!pDRBuf)
  {
    uint8 ucIdxSrvc = FindServiceIndex(ucServiceId, NL_SEARCH_BY_SERVICE_ID);
    if(NET_INVALID_IDX != ucIdxSrvc)
    { 
      if(g_aServicesTable[ucIdxSrvc].m_tPeriod.u32 == *ulPeriod)
        return RCS_N00_Success; //already got srvc
        
    }     
    if(0x1F != unCmdId)
    {
      pDRBuf = DRM_GetInactiveDRBuf();
      if(!pDRBuf)
        return RCS_E32_Busy;
    }
    if(REQNM_FindSrvcCmdPending(ucServiceId))
      return RCS_E32_Busy;
    // issue a RequestService(Cmd799) to the NM for this burst/DR buffer
    uint8 ucQIdx;
    REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
    if(!p) // no entry available, cannot send the request to NM
      return RCS_E32_Busy; 
    if(pDRBuf)
    {
      pDRBuf->m_unCmdId = unCmdId;
      //pDRBuf->m_ucDRStatus = DRSLAVE_START;
      pDRBuf->m_ucDRStatus = DRSLAVE_RUNNING; 
      //pDRBuf->m_ulTimeout = RTC_COUNT;    
    }
    //g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_BandwidthAllocationPending;
    //APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
    p->m_unCmdId = CMDID_C799_RequestService;
    p->m_pDRBuf = pDRBuf;
    p->m_stC799Req.m_ucServiceRequestFlags = ServiceRequestFlagsMask_Source; 
    p->m_stC799Req.m_ucServiceApplicationDomain = ServiceApplicationDomain_Publish;
    p->m_stC799Req.m_unNicknameOfPeer = GW_NICKNAME;   
    p->m_stC799Req.m_tPeriod.u32 = *ulPeriod;
    p->m_stC799Req.m_ucServiceId = ucServiceId;  
    if(pDRBuf)    
      //return RCS_N255_MakeNoResponse;
      return RCS_E33_DelayedResponseInitiated;    
    return RCS_N2598_ReturnFromSrvcReq;
      //or return RCS_E33_DelayedResponseInitiated;    
  }
  else
  {
    switch(pDRBuf->m_ucDRStatus)
    {
      case DRSLAVE_START:
        {
          if((RTC_COUNT - pDRBuf->m_ulTimeout) < SRVCREQ_TIMEOUT)
          {
            return RCS_N255_MakeNoResponse;
          }
        }
    case DRSLAVE_INITIATE:
         {
           pDRBuf->m_ucDRStatus = DRSLAVE_RUNNING;
           return RCS_E33_DelayedResponseInitiated;
         }                   
       case DRSLAVE_RUNNING:
         {
           return RCS_E34_DelayedResponseRunning;
         }
       case DRSLAVE_END:
         {
           pDRBuf->m_ucDRStatus = DRSLAVE_INACTIVE;
           memcpy((uint8*)ulPeriod, pDRBuf->m_aResp, sizeof(uint32));
           return RCS_N257_ReturnFromDR;
         }
        case DRSLAVE_DEAD:
         {
           return RCS_E35_DelayedResponseDead;
         }
        case DRSLAVE_CONFLICT:
          {
            memset(pDRBuf, 0, sizeof(DR_SLAVE_BUFFER_STRUCT));
            return RCS_E36_DelayedResponseConflict;
          }
      
        default:
          break;
    }
  }

  
  return RCS_N00_Success;
}
uint8 Process_SrvcDel(uint8 ucSrvcId)
{
  uint8 ucQIdx;
  REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
  if(!p) // no entry available, cannot send the request to NM
    return RCS_E32_Busy;   
  
  p->m_unCmdId = CMDID_C801_DeleteService;
  p->m_pDRBuf = NULL;      // no DR buffer used
  p->m_stC801Req.m_ucServiceId = ucSrvcId;
  p->m_stC801Req.m_ucReason = ServiceDeletionReasonCodes_RequestedByPeer; // CommonTable 49  
  
  return RCS_N00_Success;
}
#endif//( _UAP_TYPE == UAP_TYPE_WHART_API )