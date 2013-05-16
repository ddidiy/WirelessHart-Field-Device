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

#ifndef WIRELESSNETWORKMANAGERCOMMANDS_H
#define WIRELESSNETWORKMANAGERCOMMANDS_H

#include "CommonTables.h"


enum
{
  CMDID_C960_DisconnectDevice           = 960,
  CMDID_C961_WriteNetworkKey            = 961,
  CMDID_C962_WriteDeviceNicknameAddress = 962,
  CMDID_C963_WriteSession               = 963,
  CMDID_C964_DeleteSession              = 964,
  CMDID_C965_WriteSuperframe            = 965,
  CMDID_C966_DeleteSuperframe           = 966,
  CMDID_C967_WriteLink                  = 967,
  CMDID_C968_DeleteLink                 = 968,
  CMDID_C969_WriteGraphNeighbourPair    = 969,
  CMDID_C970_DeleteGraphConnection      = 970,
  CMDID_C971_WriteNeighbourPropertyFlag = 971,
  CMDID_C972_SuspendDevices             = 972,
  CMDID_C973_WriteService               = 973,
  CMDID_C974_WriteRoute                 = 974,
  CMDID_C975_DeleteRoute                = 975,
  CMDID_C976_WriteSourceRoute           = 976,
  CMDID_C977_DeleteSourceRoute          = 977
};

enum
{
  C960_ReqSize =  1,
  C961_ReqSize = 16,  //21,  // can be truncated to 16 bytes if exec time is missing
  C962_ReqSize =  2,
  C963_ReqSize = 29,  //34,  // can be truncated to 29 bytes if exec time is missing
  C964_ReqSize =  3,
  C965_ReqSize =  5,  //10,  // can be truncated to 5 bytes if exec time is missing
  C966_ReqSize =  1,
  C967_ReqSize =  8,
  C968_ReqSize =  5,
  C969_ReqSize =  4,
  C970_ReqSize =  4,
  C971_ReqSize =  3,
  C972_ReqSize = 10,
  C973_ReqSize = 10,
  C974_ReqSize =  5,
  C975_ReqSize =  1,
  C976_ReqSize =  4,  // or 6, 8, ... (variable size)
  C977_ReqSize =  1
};

enum
{
  C960_RespSize =  1,
  C961_RespSize = 16,  //21,  // can be truncated to 16 bytes if exec time is missing
  C962_RespSize =  2,
  C963_RespSize = 29,  //34,  // can be truncated to 29 bytes if exec time is missing
  C964_RespSize =  4,
  C965_RespSize =  5,  //10,  // can be truncated to 5 bytes if exec time is missing
  C966_RespSize =  2,
  C967_RespSize = 10,
  C968_RespSize =  7,
  C969_RespSize =  5,
  C970_RespSize =  5,
  C971_RespSize =  3,
  C972_RespSize = 10,
  C973_RespSize = 11,
  C974_RespSize =  6,
  C975_RespSize =  2,
  C976_RespSize =  5,  // or 7, 9, ... (variable size)
  C977_RespSize =  1
};

#define C960_DisconnectDevice_ReqSize             C960_ReqSize
#define C961_WriteNetworkKey_ReqSize              C961_ReqSize
#define C962_WriteDeviceNicknameAddress_ReqSize   C962_ReqSize
#define C963_WriteSession_ReqSize                 C963_ReqSize
#define C964_DeleteSession_ReqSize                C964_ReqSize
#define C965_WriteSuperframe_ReqSize              C965_ReqSize
#define C966_DeleteSuperframe_ReqSize             C966_ReqSize
#define C967_WriteLink_ReqSize                    C967_ReqSize
#define C968_DeleteLink_ReqSize                   C968_ReqSize
#define C969_WriteGraphNeighbourPair_ReqSize      C969_ReqSize
#define C970_DeleteGraphConnection_ReqSize        C970_ReqSize
#define C971_WriteNeighbourPropertyFlag_ReqSize   C971_ReqSize
#define C972_SuspendDevices_ReqSize               C972_ReqSize
#define C973_WriteService_ReqSize                 C973_ReqSize
#define C974_WriteRoute_ReqSize                   C974_ReqSize
#define C975_DeleteRoute_ReqSize                  C975_ReqSize
#define C976_WriteSourceRoute_ReqSize             C976_ReqSize
#define C977_DeleteSourceRoute_ReqSize            C977_ReqSize

#define C960_DisconnectDevice_RespSize            C960_RespSize
#define C961_WriteNetworkKey_RespSize             C961_RespSize
#define C962_WriteDeviceNicknameAddress_RespSize  C962_RespSize
#define C963_WriteSession_RespSize                C963_RespSize
#define C964_DeleteSession_RespSize               C964_RespSize
#define C965_WriteSuperframe_RespSize             C965_RespSize
#define C966_DeleteSuperframe_RespSize            C966_RespSize
#define C967_WriteLink_RespSize                   C967_RespSize
#define C968_DeleteLink_RespSize                  C968_RespSize
#define C969_WriteGraphNeighbourPair_RespSize     C969_RespSize
#define C970_DeleteGraphConnection_RespSize       C970_RespSize
#define C971_WriteNeighbourPropertyFlag_RespSize  C971_RespSize
#define C972_SuspendDevices_RespSize              C972_RespSize
#define C973_WriteService_RespSize                C973_RespSize
#define C974_WriteRoute_RespSize                  C974_RespSize
#define C975_DeleteRoute_RespSize                 C975_RespSize
#define C976_WriteSourceRoute_RespSize            C976_RespSize
#define C977_DeleteSourceRoute_RespSize           C977_RespSize


/******************** CMD 960 *************************/

typedef struct
{
  DisconnectCauseCodes m_eReason;
}C960_DisconnectDevice_Req;

typedef C960_DisconnectDevice_Req C960_DisconnectDevice_Resp;

typedef enum
{
  C960_N00 = RCS_N00_Success,
  C960_E16 = RCS_E16_AccessRestricted
}C960_DisconnectDevice_RespCodes;

/******************** CMD 961 *************************/

typedef struct
{
  _time40_t m_tExecutionTime;
  uint8_t m_aKeyValue[16];
  uint8_t m_ucTruncated;  //flag set if execution time not sent
}C961_WriteNetworkKey_Req;

typedef struct
{
  _time40_t m_tExecutionTime;
  uint8_t m_aKeyValue[16];
  uint8_t m_ucTruncated;  //flag set if execution time not sent
}C961_WriteNetworkKey_Resp;

typedef enum
{
  C961_N00 = RCS_N00_Success,
  C961_E05 = RCS_E05_TooFewDataBytesReceived,
  C961_E16 = RCS_E16_AccessRestricted,
  C961_E65 = RCM_E65_KeyChangeFailed,
  C961_E66 = RCM_E66_InvalidExecutionTime
}C961_WriteNetworkKey_RespCodes;


/******************** CMD 962 *************************/

typedef struct
{
  uint16_t m_unNickname;
}C962_WriteDeviceNicknameAddress_Req;

typedef struct
{
  uint16_t m_unNickname;
}C962_WriteDeviceNicknameAddress_Resp;

typedef enum
{
  C962_N00 = RCS_N00_Success,
  C962_E05 = RCS_E05_TooFewDataBytesReceived,
  C962_E16 = RCS_E16_AccessRestricted,
  C962_E65 = RCM_E65_InvalidNickname
}C962_WriteDeviceNicknameAddress_RespCodes;


/******************** CMD 963 *************************/

typedef struct
{
  uint32_t m_ulPeerNonceCounterValue;
  uint16_t m_unPeerNickname;
  uint8_t m_aKeyValue[16];
  uint8_t m_eSessionType;     // SessionTypeCodes
  uint8_t m_ucReserved;
  uint8_t m_ucTruncated;  //flag set if execution time not sent
  _device_address_t m_aPeerUniqueID;
  _time40_t m_tExecutionTime;  
}C963_WriteSession_Req;


typedef struct
{
  uint32_t m_ulPeerNonceCounterValue;
  uint32_t m_ulPeerDeviceID;	//uint24
  uint16_t m_unPeerNickname;
  uint16_t m_unPeerExpandedDeviceTypeCode;
  uint8_t m_aKeyValue[16];
  uint8_t m_eSessionType;     // SessionTypeCodes
  uint8_t m_ucRemainingSessionsNo;
  uint8_t m_ucTruncated;  //flag set if execution time not sent
  _time40_t m_tExecutionTime;  
}C963_WriteSession_Resp;

typedef enum
{
  C963_N00 = RCS_N00_Success,
  C963_E05 = RCS_E05_TooFewDataBytesReceived,
  C963_E16 = RCS_E16_AccessRestricted,
  C963_E65 = RCM_E65_NoMoreEntriesAvailable,
  C963_E66 = RCM_E66_InvalidExecutionTime,
  C963_E67 = RCM_E67_SessionTypeInvalid
}C963_WriteSession_RespCodes;


/******************** CMD 964 *************************/

typedef struct
{
  uint16_t m_unPeerNickname;
  uint8_t m_eSessionType;     // SessionTypeCodes
}C964_DeleteSession_Req;

typedef struct
{
  uint16_t m_unPeerNickname;
  uint8_t m_eSessionType;     // SessionTypeCodes
  uint8_t m_ucRemainingSessionsNo;
}C964_DeleteSession_Resp;

typedef enum
{
  C964_NOO = RCS_N00_Success,
  C964_E05 = RCS_E05_TooFewDataBytesReceived,
  C964_E16 = RCS_E16_AccessRestricted,
  C964_E65 = RCM_E65_SessionWithGivenPeerDoesntExist
}C964_DeleteSession_RespCodes;


/******************** CMD 965 *************************/

typedef struct
{
  uint16_t m_unSuperframeSlotsNo;
  uint8_t m_ucSuperframeID;
  uint8_t m_ucSuperframeMode;
  uint8_t m_ucReserved;
  uint8_t m_ucTruncated;  //flag set if execution time not sent
  _time40_t m_tExecutionTime;
}C965_WriteSuperframe_Req;


typedef struct
{
  uint16_t m_unSuperframeSlotsNo;
  uint8_t m_ucSuperframeID;
  uint8_t m_ucSuperframeMode;
  uint8_t m_ucRemainingSuperframesNo;
  uint8_t m_ucTruncated;  //flag set if execution time not sent
  _time40_t m_tExecutionTime;
}C965_WriteSuperframe_Resp;

typedef enum
{
  C965_NOO = RCS_N00_Success,
  C965_E05 = RCS_E05_TooFewDataBytesReceived,
  C965_E16 = RCS_E16_AccessRestricted,  
  C965_E65 = RCM_E65_NoMoreEntriesAvailable,
  C965_E66 = RCM_E66_InvalidExecutionTime,
  C965_E67 = RCM_E67_InvalidNoOfSlots,
  C965_E68 = RCM_E68_InvalidSuperframeMode
}C965_WriteSuperframe_RespCodes;


/******************** CMD 966 *************************/

typedef struct
{
  uint8_t m_ucSuperframeID;
}C966_DeleteSuperframe_Req;

typedef struct
{
  uint8_t m_ucSuperframeID;
  uint8_t m_ucRemainingSuperframeEntriesNo;
}C966_DeleteSuperframe_Resp;

typedef enum
{
  C966_NOO = RCS_N00_Success,
  C966_E05 = RCS_E05_TooFewDataBytesReceived,
  C966_E16 = RCS_E16_AccessRestricted,   
  C966_E65 = RCM_E65_EntryNotFound
}C966_DeleteSuperframe_RespCodes;


/******************** CMD 967 *************************/

typedef struct
{
  uint16_t m_unSlotNumber;
  uint16_t m_unNeighborNickname;
  uint8_t m_ucSuperframeID;
  uint8_t m_ucChannelOffset;
  uint8_t m_ucLinkOptions;
  uint8_t m_eLinkType;      // LinkType
}C967_WriteLink_Req;

typedef struct
{
  uint16_t m_unSlotNumber;
  uint16_t m_unNeighborNickname;
  uint8_t m_ucSuperframeID;
  uint8_t m_ucChannelOffset;
  uint8_t m_ucLinkOptions;
  uint8_t m_eLinkType;      // LinkType
  uint16_t m_unRemainingLinksNo;
}C967_WriteLink_Resp;

typedef enum
{
  C967_NOO = RCS_N00_Success,
  C967_E05 = RCS_E05_TooFewDataBytesReceived,
  C967_E16 = RCS_E16_AccessRestricted,   
  C967_E65 = RCM_E65_NoMoreEntriesAvailable,
  C967_E66 = RCM_E66_LinkAlreadyExists,
  C967_E67 = RCM_E67_UnknownSuperframeID,
  C967_E68 = RCM_E68_InvalidSlotNumber,
  C967_E69 = RCM_E69_InvalidLinkOptions,
  C967_E70 = RCM_E70_InvalidChannelOffset,
  C967_E71 = RCM_E71_InvalidLinkType,
  C967_E72 = RCM_E72_NoMoreNeighborsAvailable
}C967_WriteLink_RespCodes;


/******************** CMD 968 *************************/

typedef struct
{
  uint16_t m_unSlotNumber;
  uint16_t m_unNeighborNickname;
  uint8_t m_ucSuperframeID;
}C968_DeleteLink_Req;

typedef struct
{
  uint16_t m_unSlotNumber;
  uint16_t m_unNeighborNickname;
  uint16_t m_unRemainingLinksNo;
  uint8_t m_ucSuperframeID;
}C968_DeleteLink_Resp;

typedef enum
{
  C968_NOO = RCS_N00_Success,
  C968_E05 = RCS_E05_TooFewDataBytesReceived,
  C968_E16 = RCS_E16_AccessRestricted,  
  C968_E65 = RCM_E65_LinkNotFound
}C968_DeleteLink_RespCodes;


/******************** CMD 969 *************************/

typedef struct
{
  uint16_t m_unGraphID;
  uint16_t m_unNeighborNickname;
}C969_WriteGraphNeighbourPair_Req;

typedef struct
{
  uint16_t m_unGraphID;
  uint16_t m_unNeighborNickname;
  uint8_t m_ucRemainingConnectionsNo;
}C969_WriteGraphNeighbourPair_Resp;

typedef enum
{
  C969_NOO = RCS_N00_Success,
  C969_E05 = RCS_E05_TooFewDataBytesReceived,
  C969_E16 = RCS_E16_AccessRestricted,    
  C969_E65 = RCM_E65_NoMoreEntriesAvailable,
  C969_E66 = RCM_E66_UnknownNickname,
  C969_E67 = RCM_E67_CantAddEdgeWhenSuperframeUsedAsGraphEquiv
}C969_WriteGraphNeighbourPair_RespCodes;


/******************** CMD 970 *************************/

typedef struct
{
  uint16_t m_unGraphID;
  uint16_t m_unNeighborNickname;
}C970_DeleteGraphConnection_Req;

typedef struct
{
  uint16_t m_unGraphID;
  uint16_t m_unNeighborNickname;
  uint8_t m_ucRemainingConnectionsNo;
}C970_DeleteGraphConnection_Resp;

typedef enum
{
  C970_NOO = RCS_N00_Success,
  C970_E05 = RCS_E05_TooFewDataBytesReceived,
  C970_E16 = RCS_E16_AccessRestricted,    
  C970_E65 = RCM_E65_EntryNotFound
}C970_DeleteGraphConnection_RespCodes;


/******************** CMD 971 *************************/

typedef struct
{
  uint16_t m_unNeighborNickname;
  uint8_t m_ucNeighborFlags;
}C971_WriteNeighbourPropertyFlag_Req;

typedef struct
{
  uint16_t m_unNeighborNickname;
  uint8_t m_ucNeighborFlags;
}C971_WriteNeighbourPropertyFlag_Resp;

typedef enum
{
  C971_NOO = RCS_N00_Success,
  C971_E05 = RCS_E05_TooFewDataBytesReceived,
  C971_E16 = RCS_E16_AccessRestricted,    
  C971_E65 = RCM_E65_UnknownNickname,
  C971_E66 = RCM_E66_InvalidNeighborProperty
}C971_WriteNeighbourPropertyFlag_RespCodes;


/******************** CMD 972 *************************/

typedef struct
{
  _time40_t m_tTimeToSuspend;
  _time40_t m_tTimeToResume;
}C972_SuspendDevices_Req;

typedef C972_SuspendDevices_Req C972_SuspendDevices_Resp;

typedef enum
{
  C972_NOO = RCS_N00_Success,
  C972_E05 = RCS_E05_TooFewDataBytesReceived,
  C972_E16 = RCS_E16_AccessRestricted,    
  C972_E65 = RCM_E65_InvalidSuspendTime,
  C972_E66 = RCM_E66_InvalidResumeTime
}C972_SuspendDevices_RespCodes;


/******************** CMD 973 *************************/

typedef struct
{
  uint8_t m_ucRequestFlags;
  uint8_t m_eApplicationDomain;   // ServiceApplicationDomain
  uint16_t m_unPeerNickname;
  _time_t m_tPeriod;
  uint8_t m_ucServiceID;
  uint8_t m_ucRouteID;
}C973_WriteService_Req;

typedef struct
{
  uint8_t m_ucRequestFlags;
  uint8_t m_eApplicationDomain;   // ServiceApplicationDomain
  uint16_t m_unPeerNickname;
  _time_t m_tPeriod;
  uint8_t m_ucServiceID;
  uint8_t m_ucRouteID;
  uint8_t m_ucRemainingServicesNo;
}C973_WriteService_Resp;

typedef enum
{
  C973_NOO = RCS_N00_Success,
  C973_E05 = RCS_E05_TooFewDataBytesReceived,
  C973_E16 = RCS_E16_AccessRestricted,      
  C973_E65 = RCM_E65_NoMoreEntriesAvailable,
  C973_E66 = RCM_E66_InvalidServiceID,
  C973_E67 = RCM_E67_OpenTransactionPending,
  C973_E68 = RCM_E68_InvalidApplicationDomain,
  C973_E69 = RCM_E69_UnknownCorrespNickname,
  C973_E70 = RCM_E70_InvalidRouteID,
  C973_E71 = RCM_E71_CorrespNicknameAndRouteCorrespMismatch
}C973_WriteService_RespCodes;


/******************** CMD 974 *************************/

typedef struct
{
  uint16_t m_unPeerNickname;
  uint16_t m_unGraphID;
  uint8_t m_ucRouteID;
}C974_WriteRoute_Req;

typedef struct
{
  uint16_t m_unPeerNickname;
  uint16_t m_unGraphID;
  uint8_t m_ucRouteID;
  uint8_t m_ucRemainingRoutesNo;
}C974_WriteRoute_Resp;

typedef enum
{
  C974_NOO = RCS_N00_Success,
  C974_E05 = RCS_E05_TooFewDataBytesReceived,
  C974_E16 = RCS_E16_AccessRestricted,      
  C974_E65 = RCM_E65_NoMoreEntriesAvailable,
  C974_E66 = RCM_E66_InvalidNickname,
  C974_E67 = RCM_E67_InvalidGraphID
}C974_WriteRoute_RespCodes;


/******************** CMD 975 *************************/

typedef struct
{
  uint8_t m_ucRouteID;
}C975_DeleteRoute_Req;

typedef struct
{
  uint8_t m_ucRouteID;
  uint8_t m_ucRemainingRoutesNo;
}C975_DeleteRoute_Resp;

typedef enum
{
  C975_NOO = RCS_N00_Success,
  C975_E05 = RCS_E05_TooFewDataBytesReceived,
  C975_E16 = RCS_E16_AccessRestricted,      
  C975_E65 = RCM_E65_InvalidRouteID
}C975_DeleteRoute_RespCodes;


/******************** CMD 976 *************************/

typedef struct
{
  uint8_t m_ucRouteID;
  uint8_t m_ucHopsNo;
  uint16_t m_aNicknameHopEntries[8];
}C976_WriteSourceRoute_Req;

typedef struct
{
  uint8_t m_ucRouteID;
  uint8_t m_ucHopsNo;
  uint16_t m_aNicknameHopEntries[8];
  uint8_t m_ucRemainingSourceRoutesNo;
}C976_WriteSourceRoute_Resp;

typedef enum
{
  C976_NOO = RCS_N00_Success,
  C976_E05 = RCS_E05_TooFewDataBytesReceived,
  C976_W08 = RCM_W08_BroadcastAddrDeletedInResp,
  C976_E16 = RCS_E16_AccessRestricted,   
  C976_E65 = RCM_E65_NoMoreEntriesAvailable,
  C976_E66 = RCM_E66_InvalidRouteID,
  C976_E67 = RCM_E67_InvalidNickname,
  C976_E68 = RCM_E68_InvalidNoOfHops
}C976_WriteSourceRoute_RespCodes;


/******************** CMD 977 *************************/

typedef struct
{
  uint8_t m_ucRouteID;
}C977_DeleteSourceRoute_Req;

typedef struct
{
  uint8_t m_ucRouteID;
}C977_DeleteSourceRoute_Resp;

typedef enum
{
  C977_NOO = RCS_N00_Success,
  C977_E05 = RCS_E05_TooFewDataBytesReceived,
  C977_E16 = RCS_E16_AccessRestricted,  
  C977_E66 = RCM_E66_InvalidRouteID
}C977_DeleteSourceRoute_RespCodes;



#endif /*WIRELESSNETWORKMANAGERCOMMANDS_H*/
