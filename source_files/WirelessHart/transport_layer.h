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
// Name:         transport_layer.h
// Author:       Nivis, LLC
// Date:         July 2008
// Description:  Transport Layer 
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NIVIS_TL_H_
#define _NIVIS_TL_H_

#include "../typedef.h"
#include "../global.h"
#include "../asm.h"

#include "network_layer.h"
#include "network_layer_dm.h"

#include "transport_layer_dm.h"

/* TRANSPORT TYPE CODES */
#define BLOCK_DATA_TRANSFER_REQ             0x00  // block data transfer, master side
#define BLOCK_DATA_TRANSFER_RESP            0x01  // block data transfer, slave side
#define REQ_UNICAST                         0x02  // req./resp. transactions, master side
#define RESP_UNICAST                        0x03  // req./resp. transactions, slave side
#define SEARCH_BROADCAST                    0x04  // send a broadcast msg. to identify a specific device
#define PUBLISH_BROADCAST                   0x05  // broadcast info
#define REQ_BROADCAST                       0x06  // broadcast req.
#define RESP_BROADCAST                      0x07  // unicast resp. to the corresponding req. broadcast 
#define PUBLISH_NOTIFY                      0x08  // process data


/* BIT MASK DEFINITIONS FOR DEVICE_STATUS */ 
#define DEVICE_MALFUNCTION                  FieldDeviceStatusesMask_DeviceMalfunction
#define CONFIG_CHANGED                      FieldDeviceStatusesMask_ConfigurationChanged
#define COLD_START                          FieldDeviceStatusesMask_ColdStart
#define MORE_STATUS_AVAILABLE               FieldDeviceStatusesMask_MoreStatusAvailable
#define LOOP_CURRENT_FIXED                  FieldDeviceStatusesMask_LoopCurrentFixed
#define LOOP_CURRENT_SATURATED              FieldDeviceStatusesMask_LoopCurrentSaturated
#define NON_PRIMARY_VARIABLE_OUT_OF_LIMITS  FieldDeviceStatusesMask_NonPrimaryVariableOutOfLimits
#define PRIMARY_VARIABLE_OUT_OF_LIMITS      FieldDeviceStatusesMask_PrimaryVariableOutOfLimits


/* EXTENDED DEVICE_STATUS CODES */ 
#define MAINTENANCE_REQUIRED                ExtendedDeviceStatusCodesMask_MaintenanceRequired
#define DEVICE_VARIABLE_ALERT               ExtendedDeviceStatusCodesMask_DeviceVariableAlert
#define CRITICAL_POWER_FAILURE              ExtendedDeviceStatusCodesMask_CriticalPowerFailure


/* TRANSPORT TYPE */ 
#define IsAckTT(arg)               (((2==(arg))||(3==arg)||(6==(arg))||(7==arg))?1:0) 
#define IsRespTT(arg)              (((1==(arg))||(3==arg)||(5==(arg))||(7==arg)||(8==arg))?1:0) 
#define IsBroadcastTT(arg)         ((((arg)>=4) && ((arg)<=7))?1:0) 


/* TRANSPORT BYTE */ 
#define IsAckTB(arg)               (((arg) & 0x80) ? 1 : 0) 
#define IsRespTB(arg)              (((arg) & 0x40) ? 1 : 0) 
#define IsBroadcastTB(arg)         (((arg) & 0x20) ? 1 : 0) 
//doinel.alban #define IsValidSequenceNmbTB(arg)  (((arg)<0x1F)?1:0) 
#define IsValidSequenceNmbTB(arg)  (((arg) < 0x1F) ? 1 : 0) 

#define TRANSPORT_TABLE_INVALID_HANDLER            0xFFFF

enum
{
  TL_REQUEST = 0,
  TL_RESPONSE,
};  

typedef union
{  
  struct 
  {   
    uint8  m_ucSequenceNumber :5;
    uint8  m_bBroadcast       :1;
    uint8  m_bResponse        :1;
    uint8  m_bAcknowledged    :1;  // MSB
  }flags;
  uint8 byte;
} TL_TransportByte;  

typedef struct
{
  TL_TransportByte  m_ucTransportByte;
  uint8             m_ucDeviceStatusByte;
  uint8             m_ucXDeviceStatusByte;
} TL_Hdr;   
 
#ifdef TRANSPORT_EXTRA_BUFFER

// extra buffer for ack-services
#define BUFFER_BUSY_MASK 0x80
#define BUFFER_BEING_WRITTEN_MASK 0x40
#define TRANSP_IDX_MASK  0x0F

extern NET_TRANSPORT_ATTRIBUTES g_stXBuff;
extern uint8 g_ucXBuffProperties; // MSB -> buffer empty or not; low nibble -> transport entry index
 
#endif


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_Init
  // Author: NIVIS LLC, Dorin Pavel
  // Description: resets the transport layer variables
  // Parameters: none
  // Return: none
  ///////////////////////////////////////////////////////////////////////////////////
void TL_Init (void);
  
    

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_TransmitRequest
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by the Application Layer to send down a message
  // Parameters:
  //                p_appHandle - app level handler  
  //                p_ucDstType - type of destination 
  //                p_pDstAddr - destination address
  //                p_ucPriority  - message priority
  //                p_ucServiceID  - service ID
  //                p_ucTransportType - message transport type
  //                p_ucAppDataLen - APP data length
  //                p_pAppData - APP data buffer
  // Return: success / error code
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
uint8 TL_TransmitRequest( HANDLE           p_appHandle,
                         NET_ADDR_TYPE    p_ucDstType,
                         const uint8*     p_pDstAddr,
                         uint8            p_ucPriority,
                         uint8            p_ucServiceID,
                         uint8            p_ucTransportType,
                         uint8            p_ucAppDataLen,
                         const void*      p_pAppData
                       ); 


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_TransmitIndicate
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by Network Layer when a packet is received and authentificated
  // Parameters: 
  //                p_hHandle  - transport level handler
  //                p_ucPriority  - message priority
  //                p_pSession  - active session
  //                p_ucTpduDataLen - TPDU data length
  //                p_pTpduData - TPDU data buffer
  // Return: None
  // Obs:
  //      Access level: user level
  ///////////////////////////////////////////////////////////////////////////////////
void TL_TransmitIndicate  ( HANDLE                  p_hHandle,                      
                            uint8                   p_ucPriority,
                            NET_SESSION_ATTRIBUTES* p_pSession,
                            uint8                   p_ucTpduDataLen,
                            uint8*                  p_pTpduData );	


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_TransmitRsp
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by client as a response to an incoming packet that require a response.
  // Parameters: 
  //                p_hHandle  - packet handler
  //                p_ucAppDataLen - payload length
  //                p_pAppData - payload buffer
  //                p_ucSequenceNumber - packet sequence number  
  //                p_ucServiceID  - service ID
  // Return: success / error code
  // Obs:
  //      Access level: User level
  //      Context: p_hHandle must be same as on transmit indicate callback
  ///////////////////////////////////////////////////////////////////////////////////
uint8 TL_TransmitRsp ( HANDLE                  p_hHandle, 
                      uint8                   p_unAppDataLen, 
                      const void*             p_pAppData,
                      uint8                   p_ucSequenceNumber,
                      uint8                   p_ucServiceID
                    );
 

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: TL_doRetry
  // Author: NIVIS LLC, Dorin Pavel
  // Description: TL retry mechanismus
  // Parameters: None
  // Return: None
  // Access level: this function must be called cyclically from main
  ///////////////////////////////////////////////////////////////////////////////////
void TL_doRetry ( void );




#ifdef TRANSPORT_EXTRA_BUFFER
void TL_CheckExtraBuffer ( void );
#endif


#endif // _NIVIS_TL_H_ 

