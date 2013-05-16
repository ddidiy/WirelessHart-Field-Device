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
* Name:         datalink_layer.h
* Author:       NIVIS LLC,
* Date:         July 2008
* Description:  Data Link Layer
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_DLL_H_
#define _NIVIS_DLL_H_

#include "../typedef.h"
#include "provision.h"
#include "phy_layer.h"

#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/CommonTables.h"
#else
  #include "ApplicationLayer/Model/CommonTables.h"
#endif


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
// === DLL CAPACITIES
#define DLL_MAX_DLPDU_SIZE                   125 // cu MIC dar fara FCS check sum ( ... si fara frame len)
#define DLL_MAX_ACK_SIZE                      23 // cu MIC dar fara FCS check sum ( ... si fara frame len)

#define DLL_HIDDEN_SUPERFRAMES                11  // MIN(max num of write/delete Sf req/resp into an APDU)
#define DLL_HIDDEN_LINKS                       6  // MIN(max num of write/delete Link req/resp into an APDU)
#define DLL_HIDDEN_CONNECTIONS                11  // MIN(max num of write/delete Connection req/resp into an APDU)

#ifdef BACKBONE_SUPPORT
  #define DLL_MAX_PACKET_BUFFERS              80 //40
  #define DLL_MAX_SUPERFRAMES                 16
  #define DLL_MAX_NEIGHBORS                   64
  #define DLL_MAX_LINKS                      254  //96  // (255-DLL_HIDDEN_LINKS)
  #define DLL_MAX_GRAPHS                      32
  #define DLL_MAX_CONNECTIONS                254
#else
  #define DLL_MAX_PACKET_BUFFERS              16
  #define DLL_MAX_SUPERFRAMES                 16
  #define DLL_MAX_NEIGHBORS                   32
  #define DLL_MAX_LINKS                       80 //96 // ## DP 24.01 code size limit decrease for SimpleAPI
  #define DLL_MAX_GRAPHS                      32
  #define DLL_MAX_CONNECTIONS                 80 //96
#endif
// === DLL LOCAL DEVICE MANAGEMENT COMMANDS
#define DLL_SERV_RESET                         1
#define DLL_SERV_DISCONNECT                    2
#define DLL_SERV_REJOIN                        3
#define DLL_SERV_WRITE_SUPERFRAME              4
#define DLL_SERV_DEL_SUPERFRAME                5
#define DLL_SERV_ADD_LINK                      6
#define DLL_SERV_DEL_LINK                      7
#define DLL_SERV_ADD_CONNECTION                8
#define DLL_SERV_DEL_CONNECTION                9
#define DLL_SERV_READ_NETWORK_ID              10
#define DLL_SERV_WRITE_NETWORK_ID             11
#define DLL_SERV_WRITE_NETWORK_KEY            12
#define DLL_SERV_READ_TIMEOUT_PERIOD          13
#define DLL_SERV_WRITE_TIMEOUT_PERIOD         14
#define DLL_SERV_READ_CAPACITIES              15
#define DLL_SERV_READ_PRIORITY_THRESHOLD      16
#define DLL_SERV_WRITE_PRIORITY_THRESHOLD     17
#define DLL_SERV_READ_JOIN_PRIORITY           18
#define DLL_SERV_WRITE_JOIN_PRIORITY          19
#define DLL_SERV_READ_PROMISCUOUS_MODE        20  // optional
#define DLL_SERV_WRITE_PROMISCUOUS_MODE       21  // optional
#define DLL_SERV_READ_MAX_BACKOFF_EXPONENT    22
#define DLL_SERV_WRITE_MAX_BACKOFF_EXPONENT   23
// === DLL TIMMING
#define DLL_TS_TX_OFFSET                    2120  // +-100  // uS
#define DLL_TS_RX_OFFSET                    1120  // +-100  // uS
#define DLL_TS_RX_WAIT                      2200  // +-100  // uS
#define DLL_TS_MAX_PACKET                   4256            // uS     // 133 bytes incl PHY
#define DLL_TS_TX_ACK_DELAY                 1000  // +-100  // uS
#define DLL_TS_RX_ACK_DELAY                  800  // +-100  // uS
#define DLL_TS_ACK_WAIT                      400  // +-100  // uS
#define DLL_TS_ACK                           832            // uS     // 26 bytes
#define DLL_TS_CCA_OFFSET                   1800  // +-100  // uS
#define DLL_TS_CCA                           128            // uS     // 8 sym
#define DLL_TS_RX_TX                         192            // uS     // 12 sym
// === DLL to PHY proccessing
#define DLL_TO_PHY_TX                         40            // uS
#define DLL_TO_PHY_RX                         38  // 42            // uS

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
// ===
#define DLL_MAX_RETRIES                       4  // 4 is not enough for TML210B (needs about 16)!   /* 4 // 1 + 3 DLL retries      6 // 1 + 5 DLL retries */
// ===
#define DLL_FRAME_CONTROL                     0x41
#define DLL_BROADCAST_ADDR                    0xFFFF
// ===
#define DLL_ACK_SUCCES                        0
#define DLL_ACK_ERR_NO_BUFF_1                61
#define DLL_ACK_ERR_NO_BUFF_2                62
#define DLL_ACK_ERR_PRY                      63
// ===
#define DLL_SRC_ADDR_MASK         0x40
#define DLL_DST_ADDR_MASK         0x04
// ===
#define DLL_TIME_INCREMENT        320       // from (1/32) ms step ... to ... 10 ms step
#define DLL_ACTIVE_SEARCH_TIME    400000UL    // 4000 s  in 10 ms step
#define DLL_CHANNEL_SEARCH_TIME   40UL        // 400 ms  in 10 ms step
#define DLL_PASSIVE_CYCLE_TIME    60000UL     // 600 s   in 10 ms step
#define DLL_PASSIVE_WAKE_TIME     650UL       // 6,5 s   in 10 ms step
// ===
#define DLL_INVALID_SF_ID             0xFF
#define DLL_INVALID_SF_IDX            0xFF
#define DLL_INVALID_LINK_ID           0xFF
#define DLL_INVALID_LINK_IDX          0xFF
#define DLL_INVALID_GRAPH_ID          0xFFFF
#define DLL_INVALID_GRAPH_IDX         0xFF
#define DLL_INVALID_CONN_ID           0xFFFF
#define DLL_INVALID_CONN_IDX          0xFF
#define DLL_INVALID_PACKET_ID         0xFFFF
#define DLL_INVALID_PACKET_IDX        0xFF
#define DLL_INVALID_PACKET_TYPE       0xFF
#define DLL_INVALID_NEIGH_SHORT_ADDR  0xFFFF
#define DLL_INVALID_NEIGH_IDX         0xFF
#define DLL_INVALID_CHANNEL           0xFF
// ===
#define DLL_SUCCESS                     RCS_N00_Success  // 0
#define DLL_ERROR                       1 // Generic error
//#define DLL_OUT_OF_MEMORY               2 // Specific error // already defined in typedef.h
//#define DLL_INVALID_OPERATION           3 // Specific error
//#define DLL_INVALID_INDEX               4 // Specific error
//#define DLL_DISCARD_PACKET	        5 // Specific error
// ===
#define DLL_MAX_JOIN_PRIORITY         15        // lower number indicate a better choice for joining
#define DLL_MAX_RCV_PACKET_PRIORITY   3         // DLPDU has four priority levels
// ===
// spec075r1.1 - valid values for Back-off exponent used in shared slots are: 4, 5, 6, 7
#define DLL_MIN_BACKOFF_EXPONENT    4
#define DLL_MAX_BACKOFF_EXPONENT    7

#define DLL_RSL_DAMP                64


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
  DLL_DISCONNECT_INACTIVE = 0,
  //DLL_DISCONNECT_WAIT_FOR_RSP 
  DLL_DISCONNECT_WAIT_FOR_QUEUE = 1,
  DLL_DISCONNECT_WAIT_FOR_DIS_PACKETS = 2,
  DLL_DISCONNECT_RESET = 3
} DLL_DISCONNECT;

typedef enum
{
  DLL_PACKET_TYPE_ACK        = 0,
  DLL_PACKET_TYPE_ADVERTISE  = 1,
  DLL_PACKET_TYPE_KEEPALIVE  = 2,
  DLL_PACKET_TYPE_DISCONNECT = 3,
  DLL_PACKET_TYPE_DATA       = 7
} DLL_PACKET_TYPE_T;

typedef enum
{
  DLL_PACKET_PRIORITY_ALARM    = 0, // Lowest
  DLL_PACKET_PRIORITY_NORMAL   = 1,
  DLL_PACKET_PRIORITY_DATA     = 2,
  DLL_PACKET_PRIORITY_COMMAND  = 3  // Highest
} DLL_PACKET_PRIORITY_T;

//typedef enum
//{
//  DLL_LINK_TYPE_NORMAL      = 0,
//  DLL_LINK_TYPE_BROADCAST   = 1,
//  DLL_LINK_TYPE_JOIN        = 2,
//  DLL_LINK_TYPE_DISCOVERY   = 3
//} DLL_LINK_TYPE_T;

  // see CommonTable45
#define  DLL_LINK_TYPE_NORMAL      LinkType_Normal      // = 0
#define  DLL_LINK_TYPE_BROADCAST   LinkType_Broadcast   // = 2
#define  DLL_LINK_TYPE_JOIN        LinkType_Join        // = 3
#define  DLL_LINK_TYPE_DISCOVERY   LinkType_Discovery   // = 1
#define  DLL_LINK_TYPE_T           LinkType

typedef enum
{
  DLL_DEST_TYPE_LONG_ADDR   = 1,
  DLL_DEST_TYPE_SHORT_ADDR  = 2,
  DLL_DEST_TYPE_GRAPH       = 3,
  DLL_DEST_TYPE_SUPERFRAME  = 4,
  DLL_DEST_TYPE_PROXYSHORT_ADDR  = 5
} DLL_DEST_TYPE_T;

typedef enum
{
  DLL_SF_DISABLED = 0,
  DLL_SF_ENABLED  = 1
} DLL_SF_STATUS_T;

typedef enum
{
  DLL_ADDR_SHORT  = 0,
  DLL_ADDR_LONG   = 1
} DLL_ADDR_TYPE_T;

typedef enum
{
  DLL_KEY_WELL_KNOWN  = 0,
  DLL_KEY_NETW        = 1
} DLL_KEY_T;

typedef enum
{
  DLL_IDLE           = 0,
  DLL_ACTIVE_SEARCH  = 1,
  DLL_PASSIVE_SEARCH = 2,
  DLL_SYNCHRONIZED   = 3,
  DLL_JOINED         = 4
} DLL_STATUS_T;

typedef enum
{
  DLL_XVR_IDLE       = 0,
  DLL_XVR_TX         = 1,
  DLL_XVR_RX_ACK     = 2,
  DLL_XVR_RX         = 3,
  DLL_XVR_TX_ACK     = 4
} DLL_XVR_STATUS_T;

typedef enum {
  NET_DLL_JOIN_SUCCESS = 0,
  NET_DLL_JOIN_ABORT   = 1
} NET_DLL_JOIN_T;


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
typedef union {
  unsigned long long m_ullASN;
  struct
  {
    uint32 m_ulLsb;
    uint32 m_ulMsb;
  };
  uint8 m_aucBytes[8];                                                          // LSB = 0 ... MSB = 4  ( Little Endian )
} DLL_ASN_T;

typedef struct
{
  uint8           m_ucSuperframeID;
  uint8           m_ucChannelOffset;
  uint16          m_unNeighborShortAddr;                                        // little endian
  uint16          m_unSlot;                                                     // little endian
  uint8           m_bTx;
  uint8           m_bRx;
  uint8           m_bShared;
  uint8           m_bMarkDeleted;
  DLL_LINK_TYPE_T m_eLinkType;
} DLL_LINK_T;                                                                   // size = 12 bytes

typedef struct
{
  uint16          m_unNumSlots;                                                 // little endian
  uint16          m_unCrtOffset;                                                // ...
  uint16          m_unNewSlots;                                                 // little endian
  uint8           m_ucSuperframeID;
  DLL_SF_STATUS_T m_eActive;
  uint8           m_bSetDisabled;
  uint8           m_bModifySlots;
  uint8           m_bMarkDeleted;
  uint8           m_bSetEnabled;
  uint8           m_bHandheldSF;
} DLL_SUPERFRAME_T;                                                             // size = 14 bytes ( +2 )

typedef struct
{
  TIME_10MS m_tmLastComm;                                                       // 10 ms Step like ASN
  TIME_10MS m_tmPathFailCounter;                                                // 10 ms Step
  uint16    m_unShortAddr;                                                      // little endian
  uint16    m_unPacketsTransmitted;
  uint16    m_unPacketsMissedACK;
  uint16    m_unPacketsReceived;
//  uint16    m_unBroadcastsReceived;
  int8      m_cAvgRSL;
  uint8     m_ucBOCntr;
  uint8     m_bBOExp;
  uint8     m_bJoinPriority; // could be eliminated... with small changes in DLL_AdvertiseIndicate()
  uint8     m_bTimeSource;              // 0x01 -TimeSource, 0x80 -ActiveNeighbor (Nivis custom flag - on TR only)
  uint8     m_bLinked;
  uint8     m_bDisconnected;
  uint8     m_bMarkDeleted;
} DLL_NEIGHBOR_T;                                                               // size = 24 bytes

typedef struct
{
  uint16        m_unGraphID;                                                    // little endian
  uint16        m_unNeighborShortAddr;                                          // little endian
  uint8         m_bMarkDeleted;
} DLL_CONNECTION_T;                                                             // size = 6 bytes ( +2 )

typedef struct
{
  uint16        m_unGraphID;                                                    // little endian
  uint8         m_ucIdx;     // !
  uint8         m_ucLen;
} DLL_HASH_GRAPHS_STRUCT_T;                                                     // size = 4 bytes
//typedef struct
//{
//  uint16              m_unGraphID;                                              // little endian
//  DLL_CONNECTION_T  * m_pConn;
//  uint8               m_ucLen;
//} DLL_HASH_GRAPHS_STRUCT_T;

typedef struct
{
    uint8 m_ucLinkIdx;       // !
    uint8 m_ucSuperframeIdx; // !
    uint8 m_ucNeighborIdx;   // !
    uint8 m_ucSuperframeID;
} DLL_HASH_LINKS_STRUCT_T;                                                      // size = 3 bytes ( +1 )
//typedef struct
//{
//  DLL_LINK_T * m_pLink;
//  DLL_SUPERFRAME_T * m_pSuperframe;
//  DLL_NEIGHBOR_T * m_pNeighbor;
//} DLL_HASH_LINKS_STRUCT_T;                                                      // size = x bytes

typedef struct
{
  DLL_ADDR_TYPE_T m_bSourceLongAddr;
  DLL_ADDR_TYPE_T m_bDestLongAddr;
} DLL_ADDR_SPECIFIER_T;                                                         // size = 2 bytes ( +2 )

typedef struct
{
  DLL_PACKET_PRIORITY_T   m_bPriority;
  DLL_KEY_T               m_bEncryptKey;
  DLL_PACKET_TYPE_T       m_bPacketType;
} DLL_DLPDU_SPECIFIER_T;                                                        // size = 3 bytes ( +1 )

typedef struct
{
  uint16                m_unPacketID;                                           // little endian
  uint16                m_unGraphID;                                            // little endian
  uint16                m_unPacketBirthASN;                                     // little endian
  uint8                 m_ucSuperframeID;                                       //
  uint8                 m_bMarkDeleted;                                         //
  uint8                 m_bNoRetries;                                           // ... de cate ori pot sa incerc sa trimit mesajul
  uint8                 m_ucTxTimeouts;                                         // no of abandoned transmissions (calculated Delay < warm-up time)
  DLL_DEST_TYPE_T       m_ePacketDestType;
  DLL_ADDR_SPECIFIER_T  m_stAddrSpec;
  DLL_DLPDU_SPECIFIER_T m_stDLDPUSpec;
  uint8                 m_ucDllPayloadLen;
  uint8                 m_aSourceAddr[8];
  uint8                 m_aDestAddr[8];
  uint8                 m_aucDllPayload[DLL_MAX_DLPDU_SIZE];
} DLL_PACKET_T;                                                                 // size = 158 (+2)

typedef struct
{
  uint8 * m_pucDstAddr;
  uint8 * m_pucSrcAddr;
  uint8 * m_pucDLPDUPayload;
  uint8 m_bDstAddrLong;
  uint8 m_bSrcAddrLong;
  uint8 m_bPriority;
  uint8 m_bEncryptKey;
  uint8 m_bPacketType;
  uint8 m_ucPayloadLen;
  uint8 m_ucNeedAck;
  uint8 m_ucBroadcast;                                                          // ! it is different from NeedAck
} DLL_BUFFER_PARSED_T;                                                          // size = 20

typedef struct
{
  DLL_NEIGHBOR_T  * m_pNeighbor;
  DLL_LINK_T      * m_pLink;
  DLL_PACKET_T    * m_pQueue;
  DLL_PACKET_TYPE_T m_ucPacketType;
  uint8             m_ucChannel;
  uint8             m_ucNeedACK;
} DLL_SLOT_PARAMS_T;

typedef struct
{
  uint16            m_unJoinGraphID;
  uint8             m_ucUseCCA;                                                 //  == 0 : not using CCA, != 0 : using CCA
} DLL_SHARED_PARAMS_T;

typedef struct
{
  uint8    m_ucStructs;
  uint8    m_ucQueue;
  uint8    m_ucDisconnect;
} DLL_FLAGS_T;

typedef struct
{
  uint8  m_uc250msComputeTimeSlot;  
  uint16 m_unSlotsUntilWakeUp;
} PRED_LNK;

typedef struct
{
  uint8  m_uc250msSlotNo;
} PRED_SLOT;

#define MAX_250MS_SLOT_NO   25

typedef struct
{
  uint16  unFactor;
  uint32  ulMinLimit;
  uint32  ulMaxLimit;
  uint32  ulDefault;
  uint32* pStore;
} TIME_MIN_MAX_T;
extern const TIME_MIN_MAX_T c_tTMinMax[7];

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
// Local Management
uint8 DLL_LM_Request ( uint8 p_ucService, void * p_pData );

// Transmit Primitive
uint8 DLL_TX_Request(
                      uint16    p_unHandle,
                      uint16    p_unPacketBirthASN,
                      uint8     p_ucPayloadLen,
                      uint8   * p_pucPayload,
                      DLL_PACKET_PRIORITY_T p_ucPacketPriority,
                      DLL_DEST_TYPE_T       p_ucDestType,
                      DLL_PACKET_TYPE_T     p_ucPacketType,
                      void    * p_pDestAddr
                   );
void DLL_TX_Confirm( uint16 p_unHandle, uint8  p_ucStatus );

// Flush Primitive
uint8 DLL_FlushRequest( uint16 p_unHandler );
void DLL_FlushConfirm( uint16 p_unHandler, uint8 p_ucStatus );

// Events for Network Layer
void DLL_DisconnectIndicate();
void DLL_AdvertiseIndicate (uint8 p_ucNeighborIdx);
//void DLL_NeighborIndicate();
void DLL_ReceiveIndicate();

// PHY Callbacks
void PHY_DataConfirm   ( unsigned char p_ucStatus);
void PHY_DataIndicate  ( uint8 p_ucRSL, uint8 *p_pucData, uint8 p_ucLen);
void PHY_ErrorIndicate ( unsigned char p_ucStatus, unsigned char * p_pucData);

#endif  // _NIVIS_DLL_H_
