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
* Name:         apphart.h
* Author:       Nivis LLC
* Date:         October 2009
* Description:  
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_APPHART_H_
#define _NIVIS_APPHART_H_

#if ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )
/*===========================[ public includes ]=============================*/

#include "../typedef.h"
#include "provision.h"
#include "..\WiredHart\DataLinkLayer.h"
#include "..\WiredHart\options.h"

/*
#include "../global.h"
#include "../spi.h"

#include "network_layer_dm.h"
#include "transport_layer.h"
#include "datalink_layer.h"
#include "datalink_layer_dm.h"
*/
#include "ParseComposeCommand.h"
#include "ParseComposeCommandTables.h"

/*
#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/UniversalCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/CommonPracticeCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/WirelessApplicationCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/DataLinkLayerCommands.h"
#else
  #include "ApplicationLayer/Model/UniversalCommands.h"
  #include "ApplicationLayer/Model/CommonPracticeCommands.h"
  #include "ApplicationLayer/Model/WirelessApplicationCommands.h"
  #include "ApplicationLayer/Model/DataLinkLayerCommands.h"
#endif
*/
#define MAX_NOF_SUBDEV  2

//Sub-device identity summary(belongs to I/O System)
typedef struct
{
/*cmd84*/  
  uint16   m_unSubdeviceIdx;
  uint8    m_ucIOCard;
  uint8    m_ucChannel;
  uint16   m_unManufacturerId;
  uint16   m_unExpandedDeviceTypeCode;
  uint8    m_aDeviceId[3];
  uint8    m_ucRevisionLevel; 
  uint8    m_aLongTag[32]; //if Hart 5, then it contains message from cmd12
/*cmd0*/  
  uint8    m_ucMinPreamblesMasterToSlave;
/*adapter specific*/
  unsigned long long m_ullLastSyncTime;
  uint8    m_ucMinPreamblesSlaveToMaster;
} SUBDEV_ID;

extern SUBDEV_ID g_aSubDev[MAX_NOF_SUBDEV];
extern uint8     g_ucNofSubDev;

typedef enum {
  APPHART_INACTIVE = 0,
  APPHART_SEND,
  APPHART_WAIT,
  APPHART_SUCCESS,
  APPHART_ABORT
}APPHART_STATUS;

typedef struct{
  uint8       m_ucStatus; 
  uint8       m_ucAddrType;
  uint16      m_unCmdId;
  uint8       m_ucRetryCnt;
  uint32      m_ulTimeout;
  uint8       m_ucFromDR;
  uint8       m_aAddr[HART_ADDR_LENGTH];
  uint8       m_aReqData[HART_BUFFER_MAX_LEN]; //parsed data
  uint8       m_aRspData[HART_BUFFER_MAX_LEN]; //parsed data
}APPHART_QUEUE_ELEMENT;

#define APPHART_QUEUE_SIZE 2
extern APPHART_QUEUE_ELEMENT g_aAPPHARTQueue[APPHART_QUEUE_SIZE];
extern uint8_t g_ucSyncSubdevices;

void APPHART_InitQueue();
void APPHART_Task();
APPHART_QUEUE_ELEMENT* APPHART_getQueueEntry();

void APPHART_TRANSMIT_confirm(uint8 p_ucStatus,
                              uint8 p_ucAddrType,
                              const uint8 *p_pSlaveAddr,
                              const uint8 *p_pData);

void APPHART_TRANSMIT_indicate(uint8 p_ucSrcAddr,
                               const uint8 *p_pData);
#else

    #define APPHART_Task()
//    #define

#endif // ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )
#endif /* _NIVIS_APPHART_H_ */