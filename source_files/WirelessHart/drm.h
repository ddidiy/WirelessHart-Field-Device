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
// Name:         drm.h
// Author:       Nivis, LLC
// Date:         September 2009
// Description:  Header file for Delayed Response Mechanism module 
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRM_H_
#define _DRM_H_

#include "../typedef.h"
#include "config.h"
#include "util/Binarization.h"

#define MAX_DR_SLAVE_REQ_LEN    128
#define MAX_DR_SLAVE_RESP_LEN   128

typedef enum {
  DRSLAVE_INACTIVE = 0,
  DRSLAVE_START,
  DRSLAVE_INITIATE,
  DRSLAVE_RUNNING,
  DRSLAVE_END,
  DRSLAVE_DEAD,
  DRSLAVE_CONFLICT
} DRSLAVE_STATUS;
  
typedef struct {
  uint8       m_ucDRStatus;           // 0 -DR Slave Buffer inactive
  uint8       m_ucRespSize;
  uint16      m_unCmdId;              // 2-byte HART command ID
  SHORT_ADDR  m_unSrcAddr;            // src nickname for wireless network; 0 or 1 (Sec/Pri Master) for wired network
  uint8       m_aResp[MAX_DR_SLAVE_RESP_LEN];  // 4-byte aligned buffer reserved for final cmd-specific response parameters
  uint8       m_aReq[MAX_DR_SLAVE_REQ_LEN];    // buffer for initial, cmd-specific request parameters - must be zeroed before each use!
  #if ( _UAP_TYPE == UAP_TYPE_WHART_API )
    uint32 m_ulTimeout;
  #endif  
} DR_SLAVE_BUFFER_STRUCT;  

//#pragma data_alignment=4

//***//
typedef enum {
  AQE_FREE = 0,
  AQE_USED = 1,
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )  
  AQE_PENDING = 2
#endif    
}AQE_STATUS;
typedef struct{
  AQE_STATUS  m_ucStatus;
  //uint8       m_ucPriority;
  HANDLE      m_hHandle;  
  SHORT_ADDR  m_unSrcAddr;
  uint8       m_ucDirection;
  uint8       m_ucTransportType;
  uint8       m_ucReqSize;
  uint8       m_ucReqCrtPos;  
  uint8       m_ucSequenceNumber;
  uint8       m_ucRspCrtPos;  
  uint32      m_ulTimeout;
  uint8       m_aReqBuffer[MAX_APDU_SIZE]; //TODO, do not leave these magic numbers here!!! use MAX_APDU_SIZE or smth.
  uint8       m_aRspBuffer[MAX_APDU_SIZE];
}APP_QUEUE_ELEMENT;

#if (( _UAP_TYPE == UAP_TYPE_WHART_API ) || (_UAP_TYPE ==UAP_TYPE_SIMPLE_API))
  #define   APP_QUEUE_SIZE  1
#else 
  #define   APP_QUEUE_SIZE  2
#endif

#ifdef DR_USE_MORE_BUFFERS
  #define MAX_DR_SLAVE_BUFFERS    1
  extern DR_SLAVE_BUFFER_STRUCT g_aDRSlaveBuf[MAX_DR_SLAVE_BUFFERS];
#else 
  extern DR_SLAVE_BUFFER_STRUCT g_stDRSlaveBuf;
#endif //DR_USE_MORE_BUFFERS
  
extern APP_QUEUE_ELEMENT g_aAPPQueue[APP_QUEUE_SIZE];


DR_SLAVE_BUFFER_STRUCT* DRM_GetInactiveDRBuf(void);
uint16 APP_ProcessCmdBuf( uint8         p_ucQueueIdx,
                          uint8*        p_pucCmdNo,
                          uint8         p_ucCmdType,
                          uint8         p_ucDirection,
                          SHORT_ADDR    p_unSrcAddr,
                          BinaryStream* p_pInputStream,
                          BinaryStream* p_pOutputStream); 

void APP_QueueTask();

APP_QUEUE_ELEMENT* APP_getQueueElement();
//*~*//
DR_SLAVE_BUFFER_STRUCT* findDRBufByCmdId(uint16 p_unCmdId);
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
void DR_Task();
APP_QUEUE_ELEMENT* APP_getPendingQueueElement();
#endif
#endif // _DRM_H_