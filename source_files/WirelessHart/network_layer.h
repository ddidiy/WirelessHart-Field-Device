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
* Name:         network_layer.h
* Author:       Nivis LLC
* Date:         July 2008
* Description:  Network Layer 
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_NL_H_
#define _NIVIS_NL_H_

#include "../typedef.h"
#include "../global.h"
#include "network_layer_dm.h"
#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/NetworkLayerCommands.h"
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/WirelessApplicationCommands.h"
#else
  #include "ApplicationLayer/Model/NetworkLayerCommands.h"
  #include "ApplicationLayer/Model/WirelessApplicationCommands.h"
#endif


///////////////////////////////////////////////////////////////////////////////////
  // Name: NLDE_Task
  // Author: NIVIS LLC, Ion Ticus
  // Description: Performs network laye messages management
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NLDE_Task(void);


enum {        
        DLPDU_UNICAST,
	DLPDU_BROADCAST        
}; // LocalStatus, this info is delivered from DLL to NET when receiving a packet 

#define BROADCAST_NPDU      BIT0  // 0 -> unicast NPDU, 1 -> broadcast NPDU  
#define BROADCAST_DLPDU     BIT1  // 0 -> unicast DLPDU, 1 -> broadcast DLPDU 
#define IS_GRAPH            BIT2  // 0 -> invalid graph, 1 -> graph is valid 
#define IS_SOURCE_ROUTING   BIT3  // 0 -> not source routing, 1 -> source routing

#define FORWARD_PACKET      BIT7  // 0 -> packet is for me, 1 -> packet is not for me, forward it 


typedef enum {        
	NET_ADDR_SHORT_ADDR,
	NET_ADDR_LONG_ADDR        
} NET_ADDR_TYPE;

typedef enum {
	NET_TT_TRANSFER_REQ,
	NET_TT_TRANSFER_RSP,
	NET_TT_UNICAST_REQ,
	NET_TT_UNICAST_RSP,
	NET_TT_BROADCAST_SEARCH,
	NET_TT_BROADCAST_PUBLISH,
	NET_TT_BROADCAST_REQUEST,
	NET_TT_PUBLISH
} NET_TRANSPORT_TYPE;

typedef enum {
	NET_MNG_RESET,
	NET_MNG_WRITE_SESSION_KEY,
	NET_MNG_DEL_SESSION,
	NET_MNG_ADD_ROUTE,
	NET_MNG_DEL_ROUTE,
	NET_MNG_DEFAULT_ROUTE,
	NET_MNG_READ_PDU_TIMEOUT,
	NET_MNG_WRITE_PDU_TIMEOUT,
	NET_MNG_READ_TTL,
	NET_MNG_WRITE_TTL
} NET_MNG_SERVICE;

typedef enum {
	NET_SUCCESS,
	NET_WARNING,
	NET_ERROR
} NET_STATUS;

enum {
        NO_HEALTH_REPORTS = 0,
        START_HEALTH_REPORTS,
        SEND_HEALTH_REPORTS
}; //HEALTH_REPORTS_STATE 

enum {
        NET_STATE_IDLE,
        NET_STATE_JOINING,
        NET_STATE_QUARANTINED,
        NET_STATE_OPERATIONAL,
        NET_STATE_SUSPENDED
};//NET_STATE

enum {
        NET_JOIN_IDLE,
        NET_JOIN_SEARCHING,
        NET_JOIN_ADV_NEIGHBOR,
        NET_JOIN_REQ_ADMISSION,
        NET_JOIN_LOOSELY_COUPLED
};//NET_JOIN_STATE

enum {
        LED_IDLE,                 // 0b0000
        LED_SEARCHING,            // 0b0001
        LED_ADVERTISE,            // 0b0010
        LED_JOIN_REQ,             // 0b0011
        LED_JOIN_REQ_REPEAT,      // 0b0100
        LED_JOIN_REQ_EXCEEDED,    // 0b0101
        LED_JOIN_LOOSELY_COUPLED, // 0b0110
        LED_JOIN_QUARANTINED,     // 0b0111
        LED_JOIN_OPERATIONAL,     // 0b1000
        LED_JOIN_SUSPENDED
};//LED_JOIN_STATE

enum {
        JOIN_ACTIVE,
        NMB_OF_JOIN_REQ_EXCEEDED,
        DEVICE_JOINED,
};//JOIN STATUS (from NET_JOIN_STATE)

// ADVERTISE NEIGHBOR TYPE
#define MIN_ADS_NEEDED     20
#define MAX_ADV_NEIGHBORS   3
//#define MIN_AVERAGE_RSL -100  // dummy value, TBD
typedef struct {
  SHORT_ADDR    m_unNickname;
  int8          m_cRSL;
  uint8         m_ucJoinPrio;
  uint8         m_aucPayload[DLL_MAX_DLPDU_SIZE-9];       
} T_ADV_NEIGHBOR; 

// NET queue for received packets
#define NET_RX_QUEUE_SIZE 2


// hardcoded GraphId to be used by AccessPoint to communicate to NM during join process
#define TR_NM_GRAPHID 0x0101   //0xFFFE   


typedef struct {
  HANDLE   m_hPacketHandler;
  uint8    m_ucUsedElem;
  uint8    m_ucPrio;
  uint8    m_ucPayloadLength;
  uint8    m_aucPayload[MAX_NSDU_SIZE];
} T_NET_RX_PACKET; 

#ifndef BACKBONE_SUPPORT 
typedef struct {
  
  uint16 m_unMyselfLoopCtr;
  uint16 m_unHopNoLoopCtr;  
  uint16 m_unMyTxMsgCtr;    // counter for my own tx messages
  uint16 m_unToNextResetTmr;
  uint8  m_ucDetected ;
  
} NET_LOOP_DETECTION;
#endif

extern const uint8 c_aHCF_OUI_BE[3];
extern const uint8 c_aHCF_OUI_LE[3];

extern const uint8 c_aNetManagerAddr[2];
extern const UNIQUE_ID c_aNetManagerUID_BE;
extern const UNIQUE_ID c_aNetManagerUID_LE;

extern const uint8 c_aGatewayAddr[2];
extern const UNIQUE_ID c_aGatewayUID_BE;
extern const UNIQUE_ID c_aGatewayUID_LE;

extern const uint8 c_aBcastAddr[2];

/* NET global status */
extern uint8 g_ucNetState;

/* NET join procedure status */
extern uint8 g_ucNetJoinState;

/* number of received advertisements */
extern uint8 g_ucNetAdNumber;

/* join response timer */
extern uint32 g_ulJoinRspTimer;

/* advertising neighbors table */
extern T_ADV_NEIGHBOR g_astAdvNeighbors[MAX_ADV_NEIGHBORS];

/* command 769 response (wireless operation mode, join status, etc), see table 51/52 from Common Tables Spec */
extern C769_ReadJoinStatus_Resp g_stC769;

/* command 779 response, see Wireless Command Spec */
extern C779_ReportDeviceHealth_Resp g_stC779;

/* COMMAND FLAGS, BIT DEFINITIONS */ 
#define NETWORK_KEY_WRITTEN                 BIT0
#define NICKNAME_WRITTEN                    BIT1
#define UNICAST_SESSION_WRITTEN             BIT2
#define RECEIVED_NORMAL_FRAME               BIT3

/* command flags (1 -> command executed) for NET join procedure */
extern uint8 g_ucJoinCmdFlags;

/* see above HEALTH_REPORTS_STATE */
extern uint8 g_ucHealthReportState;

/* health report timer */
extern uint32 g_ulHealthReportTimer;

/*join mode code*/
extern uint8 g_ucJoinMode;


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_Init
  // Author: NIVIS LLC, Dorin Pavel
  // Description: resets the network layer variables
  // Parameters: none
  // Return: none
  ///////////////////////////////////////////////////////////////////////////////////
void NET_Init (void);


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_TransmitReqShortAddr
  // Author: NIVIS LLC, Ion Ticus
  // Description: It is used by the Transport Layer to send a request to a specific destination.
  // Parameters: 
  //                p_hHandle  - transport level handler
  //                p_ucDstType  - destination type
  //                p_pDstAddr - destination address 
  //                p_ucTransportType - message transport type
  //                p_ucPriority  - message priority
  //                p_ucServiceID  - service ID
  //                p_pSession  - active session
  //                p_ucNsduDataLen - NSDU data length
  //                p_pNsduData - NSDU data buffer
  // Return: success / error code
  // Obs:
  //      Access level: User level
  //      Context: After calling of that function a NET_TransmitConfirm has to be received.
  ///////////////////////////////////////////////////////////////////////////////////
uint8 NET_TransmitRequest( HANDLE        p_hHandle,
                          NET_ADDR_TYPE p_ucDstType, 
                          const uint8*  p_pDstAddr,
                          uint8         p_ucTransportType,
                          uint8         p_ucPriority,
                          uint8         p_ucServiceID,
                          NET_SESSION_ATTRIBUTES* p_pSession,  
                          uint8         p_ucNsduDataLen,
                          const uint8*  p_pNsduData );				


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_TransmitIndicate
  // Author: NIVIS LLC, Dorin Pavel
  // Description: It is invoked by DLL Layer when a packet is received
  // Parameters: 
  //                p_ucLocalStatus - unicast/broadcast DLPDU
  //                p_ucPriority  - message priority
  //                p_ucNsduDataLen - NSDU data length
  //                p_pNsduData - NSDU data buffer
  // Return: success / error code (see CommonResponseCode.h)
  // Obs:
  //      Access level: interrupt level
  ///////////////////////////////////////////////////////////////////////////////////
uint8 NET_TransmitIndicate( uint8  p_ucLocalStatus,
                            uint8  p_ucPriority,
                            uint8  p_ucNsduDataLen,
                            uint8* p_pNsduData );


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_FlushReq
  // Author: NIVIS LLC, Ion Ticus
  // Description: It is invoked by Application Layer to delete an indicated packet.
  // Parameters: 
  //                p_hHandle  - network level handler
  // Return: None
  // Obs:
  //      Access level: User level
  //      Context: Must be be followed by a flush confirmation
  ///////////////////////////////////////////////////////////////////////////////////
void NET_FlushReq( HANDLE p_oHandle );


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_FlushConfirm
  // Author: NIVIS LLC, Ion Ticus
  // Description: It is invoked by Network Layer to confirm if the packet was deleted or not.
  // Parameters: 
  //                p_hHandle  - network level handler
  //                p_ucLocalStatus - flush result code
  // Return: None
  // Obs:
  //      Access level: User level
  //      Context: Must be preceded by a flush request
  ///////////////////////////////////////////////////////////////////////////////////
void NET_FlushConfirm( HANDLE p_oHandle, unsigned char p_ucLocalStatus );   


  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_StateMachine
  // Author: NIVIS LLC, Dorin Pavel
  // Description: This implements the network layer state machine
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NET_StateMachine (void);

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_CheckTimers
  // Author: NIVIS LLC, Dorin Pavel
  // Description: decrement join timers
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NET_CheckTimers (void);

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: NET_RecvQueueManager
  // Author: NIVIS LLC, Dorin Pavel
  // Description: NET rx queue handling
  // Parameters: None
  // Return: None
  // Obs:
  //      Access level: User level
  ///////////////////////////////////////////////////////////////////////////////////
void NET_RecvQueueManager (void);

///////////////////////////////////////////////////////////////////////////////////
// Name: NET_LoopCheck
// Author: NIVIS LLC, Erdei Eduard
// Description: Handles the loop detection mechanism
// Parameters: None
// Return: None
// Obs: Called once per second
//      Access level: User level
///////////////////////////////////////////////////////////////////////////////////
void NET_LoopCheck( void );

#endif // _NIVIS_NL_H_ 

