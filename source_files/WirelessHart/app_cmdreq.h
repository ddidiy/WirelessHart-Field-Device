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
* Name:         app_cmdreq.h
* Author:       Nivis LLC
* Date:         August 2008
* Description:  Application Layer, HART commands: compose the request, parse the response
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_APP_CMDREQ_H_
#define _NIVIS_APP_CMDREQ_H_

#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Binarization/NetworkLayerCommands.h"
#else
  #include "ApplicationLayer/Binarization/NetworkLayerCommands.h"
#endif

#include "drm.h"

#define REQNM_QUEUE_SIZE        4
#define MAX_REQNM_RETRY_CNT     4
#define REQNM_HANDLE_BASE 0x7000

typedef enum {
  REQNM_INACTIVE = 0,
  REQNM_SEND,
  REQNM_WAIT,
  REQNM_SUCCESS,
  REQNM_ABORT
} REQNM_STATUS;
  

// wireless master thread: Request to NM Queue structure
typedef struct
{ // Request packet handle will be: 0x7000 + queue index
  uint8  m_ucStatus;
  uint8  m_ucRetryCnt;
  uint16 m_unCmdId;
  uint32 m_ulTimestamp;
  DR_SLAVE_BUFFER_STRUCT* m_pDRBuf; // null if request wasn't initiated from a DR context

  union
  { // 4-byte align
    C799_RequestService_Req         m_stC799Req;  // 3 bytes pad at the end
    C770_RequestActiveAdvertise_Req m_stC770Req;
    C801_DeleteService_Req          m_stC801Req;
  }; 
} REQNM_QEUE_T;  

extern REQNM_QEUE_T   g_aReqNMQueue[REQNM_QUEUE_SIZE];

REQNM_QEUE_T* REQNM_GetQueueEntry(uint8* pucIdx);
void REQNM_Task(void);
void REQNM_SendMessageToNM(uint8 p_ucIdx);
REQNM_QEUE_T* REQNM_FindSrvcCmdPending(uint8 ucSrvcId);

#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
uint16 Process_SrvcReq(uint16 unCmdId, uint8 ucServiceId, uint32* ulPeriod);
uint8 Process_SrvcDel(uint8 ucServiceId);
#endif  //#if ( _UAP_TYPE == UAP_TYPE_WHART_API )


#endif /* _NIVIS_APP_CMDREQ_H_ */
