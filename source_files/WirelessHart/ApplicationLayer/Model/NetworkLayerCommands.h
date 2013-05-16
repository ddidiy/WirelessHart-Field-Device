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

#ifndef NETWORK_LAYER_COMMANDS_H_
#define NETWORK_LAYER_COMMANDS_H_

#include "CommonTables.h"

// assuming the following: 
// Max. APDU len = 92 (in WirelessHART format)
// Max. Data len, for a non-aggregated command response: 92-4=88

#define C782_MAX_SESSIONS_LIST        ((88 - 3) / 16)
#define C800_MAX_SERVICES_LIST        ((88 - 3) / 10)
#define C802_MAX_ROUTES_LIST          ((88 - 4) / 6)


#define MAX_DEVICE_LIST_ENTRIES_NO        20      // for C814; may differ for the various device types



enum
{
  CMDID_C768_WriteJoinKey               = 768,
  CMDID_C769_ReadJoinStatus             = 769,
  CMDID_C770_RequestActiveAdvertise     = 770,
  CMDID_C771_ForceJoin                  = 771,
  CMDID_C772_ReadJoinModeConfiguration  = 772,
  CMDID_C782_ReadSessionEntries         = 782,
  CMDID_C789_AlarmSourceRouteFailed     = 789,
  CMDID_C790_AlarmGraphRouteFailed      = 790,
  CMDID_C791_AlarmTransportLayerFailed  = 791,
  CMDID_C799_RequestService             = 799,
  CMDID_C800_ReadServiceList            = 800,
  CMDID_C801_DeleteService              = 801,
  CMDID_C802_ReadRouteList              = 802,
  CMDID_C803_ReadSourceRoute            = 803,
  CMDID_C808_ReadTimeToLive             = 808,
  CMDID_C809_WriteTimeToLive            = 809,
  CMDID_C814_ReadDeviceListEntries      = 814,
  CMDID_C815_AddDeviceListTableEntry    = 815,
  CMDID_C816_DeleteDeviceListTableEntry = 816,
  CMDID_C817_ReadChannelBlacklist       = 817,
  CMDID_C818_WriteChannelBlacklist      = 818,
  CMDID_C821_WriteNetworkAccessMode     = 821,
  CMDID_C822_ReadNetworkAccessMode      = 822,
  CMDID_C823_RequestSession             = 823
};

enum
{
  C768_ReqSize = 16,
  C769_ReqSize =  0,
  C770_ReqSize =  4,
  C771_ReqSize =  5,
  C772_ReqSize =  0,
  C782_ReqSize =  2,
  C789_ReqSize =  0,
  C790_ReqSize =  0,
  C791_ReqSize =  0,
  C799_ReqSize =  9,
  C800_ReqSize =  2,
  C801_ReqSize =  2,
  C802_ReqSize =  2,
  C803_ReqSize =  1,
  C808_ReqSize =  0,
  C809_ReqSize =  1,
  C814_ReqSize =  4,
  C815_ReqSize =  6,
  C816_ReqSize =  6,
  C817_ReqSize =  0,
  C818_ReqSize =  3,
  C821_ReqSize =  1,
  C822_ReqSize =  0,
  C823_ReqSize =  2
};

enum
{
  C768_RespSize = 16,
  C769_RespSize = 14,
  C770_RespSize =  9,
  C771_RespSize =  5,
  C772_RespSize =  5,
  C782_RespSize = 19, // or 35, 51, ... (variable size)
  C789_RespSize =  6,
  C790_RespSize =  2,
  C791_RespSize =  2,
  C799_RespSize = 10,
  C800_RespSize = 13, // or 23, 33, ... (variable size)
  C801_RespSize =  3,
  C802_RespSize = 10, // or 16, 22, ... (variable size)
  C803_RespSize =  4, // or 6, 8, ... (variable size)
  C808_RespSize =  1,
  C809_RespSize =  1,
  C814_RespSize = 12, // or 17, 22, ... (variable size)
  C815_RespSize =  8,
  C816_RespSize =  8,
  C817_RespSize =  5,
  C818_RespSize =  3,
  C821_RespSize =  1,
  C822_RespSize =  1,
  C823_RespSize = 22
};

#define C768_WriteJoinKey_ReqSize                 C768_ReqSize
#define C769_ReadJoinStatus_ReqSize               C769_ReqSize
#define C770_RequestActiveAdvertise_ReqSize       C770_ReqSize
#define C771_ForceJoin_ReqSize                    C771_ReqSize
#define C772_ReadJoinModeConfiguration_ReqSize    C772_ReqSize
#define C782_ReadSessionEntries_ReqSize           C782_ReqSize
#define C789_AlarmSourceRouteFailed_ReqSize       C789_ReqSize
#define C790_AlarmGraphRouteFailed_ReqSize        C790_ReqSize
#define C791_AlarmTransportLayerFailed_ReqSize    C791_ReqSize
#define C799_RequestService_ReqSize               C799_ReqSize
#define C800_ReadServiceList_ReqSize              C800_ReqSize
#define C801_DeleteService_ReqSize                C801_ReqSize
#define C802_ReadRouteList_ReqSize                C802_ReqSize
#define C803_ReadSourceRoute_ReqSize              C803_ReqSize
#define C808_ReadTimeToLive_ReqSize               C808_ReqSize
#define C809_WriteTimeToLive_ReqSize              C809_ReqSize
#define C814_ReadDeviceListEntries_ReqSize        C814_ReqSize
#define C815_AddDeviceListTableEntry_ReqSize      C815_ReqSize
#define C816_DeleteDeviceListTableEntry_ReqSize   C816_ReqSize
#define C817_ReadChannelBlacklist_ReqSize         C817_ReqSize
#define C818_WriteChannelBlacklist_ReqSize        C818_ReqSize
#define C821_WriteNetworkAccessMode_ReqSize       C821_ReqSize
#define C822_ReadNetworkAccessMode_ReqSize        C822_ReqSize
#define C823_RequestSession_ReqSize               C823_ReqSize

#define C768_WriteJoinKey_RespSize                C768_RespSize
#define C769_ReadJoinStatus_RespSize              C769_RespSize
#define C770_RequestActiveAdvertise_RespSize      C770_RespSize
#define C771_ForceJoin_RespSize                   C771_RespSize
#define C772_ReadJoinModeConfiguration_RespSize   C772_RespSize
#define C782_ReadSessionEntries_RespSize          C782_RespSize
#define C789_AlarmSourceRouteFailed_RespSize      C789_RespSize
#define C790_AlarmGraphRouteFailed_RespSize       C790_RespSize
#define C791_AlarmTransportLayerFailed_RespSize   C791_RespSize
#define C799_RequestService_RespSize              C799_RespSize
#define C800_ReadServiceList_RespSize             C800_RespSize
#define C801_DeleteService_RespSize               C801_RespSize
#define C802_ReadRouteList_RespSize               C802_RespSize
#define C803_ReadSourceRoute_RespSize             C803_RespSize
#define C808_ReadTimeToLive_RespSize              C808_RespSize
#define C809_WriteTimeToLive_RespSize             C809_RespSize
#define C814_ReadDeviceListEntries_RespSize       C814_RespSize
#define C815_AddDeviceListTableEntry_RespSize     C815_RespSize
#define C816_DeleteDeviceListTableEntry_RespSize  C816_RespSize
#define C817_ReadChannelBlacklist_RespSize        C817_RespSize
#define C818_WriteChannelBlacklist_RespSize       C818_RespSize
#define C821_WriteNetworkAccessMode_RespSize      C821_RespSize
#define C822_ReadNetworkAccessMode_RespSize       C822_RespSize
#define C823_RequestSession_RespSize              C823_RespSize


/******************** CMD 768 *************************/

typedef struct
{
    uint8_t key[16];
}C768_WriteJoinKey_Req;

typedef struct
{
    uint8_t key[16];
}C768_WriteJoinKey_Resp;

typedef enum
{
    C768_NOO = RCS_N00_Success,
    C768_E05 = RCS_E05_TooFewDataBytesReceived,
    C768_E06 = RCS_E06_DeviceSpecificCommandError,
    C768_E07 = RCS_E07_InWriteProtectMode,
    C768_E16 = RCS_E16_AccessRestricted,
    C768_E32 = RCS_E32_Busy,
    C768_E33 = RCS_E33_DelayedResponseInitiated,
    C768_E34 = RCS_E34_DelayedResponseRunning,
    C768_E35 = RCS_E35_DelayedResponseDead,
    C768_E36 = RCS_E36_DelayedResponseConflict,
    C768_E65 = RCM_E65_KeyChangeFailed
}C768_WriteJoinKey_RespCodes;


/******************** CMD 769 *************************/

typedef EmptyCommand_Req C769_ReadJoinStatus_Req;

typedef struct
{
    uint8_t wirelessMode;
    uint8_t noOfAvailableNeighbors;
    uint8_t noOfAdvertisingPacketsReceived;
    uint8_t noOfJoinAttempts;
    _time_t joinRetryTimer;
    _time_t networkSearchTimer;
    uint16_t joinStatus;
}C769_ReadJoinStatus_Resp;

typedef enum
{
    C769_NOO = RCS_N00_Success,
    C769_E06 = RCS_E06_DeviceSpecificCommandError,
    C769_E32 = RCS_E32_Busy
}C769_ReadJoinStatus_RespCodes;


/******************** CMD 770 *************************/

typedef struct
{
    _time_t m_tShedTime;
}C770_RequestActiveAdvertise_Req;

typedef struct
{
    _time_t m_tShedTime;
    _time_t m_tAdvertisingPeriod;
    uint8_t m_ucNoOfNeighborsAdvertising;
}C770_RequestActiveAdvertise_Resp;

typedef enum
{
    C770_NOO = RCS_N00_Success,
    C770_E03 = RCS_E03_PassedParameterTooLarge,
    C770_E04 = RCS_E04_PassedParameterTooSmall,
    C770_E05 = RCS_E05_TooFewDataBytesReceived,
    C770_E06 = RCS_E06_DeviceSpecificCommandError,
    C770_E07 = RCS_E07_InWriteProtectMode,  
    C770_W08 = RCM_W08_SetToNearestPossibleValue,
    C770_E16 = RCS_E16_AccessRestricted,
    C770_E32 = RCS_E32_Busy,
    C770_E33 = RCS_E33_DelayedResponseInitiated,
    C770_E34 = RCS_E34_DelayedResponseRunning,
    C770_E35 = RCS_E35_DelayedResponseDead,
    C770_E36 = RCS_E36_DelayedResponseConflict,    
    C770_E65 = RCM_E65_Declined
}C770_RequestActiveAdvertise_RespCodes;


/******************** CMD 771 *************************/

typedef struct
{
    _time_t m_tJoinShedTime;
    JoinModeCode m_ucJoinMode;
}C771_ForceJoin_Req;

typedef struct
{
    _time_t m_tJoinShedTime;
    JoinModeCode m_ucJoinMode;
}C771_ForceJoin_Resp;

typedef enum
{
    C771_NOO = RCS_N00_Success,
    C771_E03 = RCS_E03_PassedParameterTooLarge,
    C771_E04 = RCS_E04_PassedParameterTooSmall,
    C771_E05 = RCS_E05_TooFewDataBytesReceived,
    C771_E06 = RCS_E06_DeviceSpecificCommandError,
    C771_W08 = RCM_W08_SetToNearestPossibleValue,
    C771_E16 = RCS_E16_AccessRestricted,
    C771_E32 = RCS_E32_Busy,
    C771_E33 = RCS_E33_DelayedResponseInitiated,
    C771_E34 = RCS_E34_DelayedResponseRunning,
    C771_E35 = RCS_E35_DelayedResponseDead,
    C771_E36 = RCS_E36_DelayedResponseConflict,    
    C771_E65 = RCM_E65_ForceJoinDeclined
}C771_ForceJoin_RespCodes;


/******************** CMD 772 *************************/

typedef EmptyCommand_Req C772_ReadJoinModeConfiguration_Req;

typedef struct
{
    _time_t m_tJoinShedTime;
    JoinModeCode m_ucJoinMode;
}C772_ReadJoinModeConfiguration_Resp;

typedef enum
{
    C772_NOO = RCS_N00_Success,
    C772_E06 = RCS_E06_DeviceSpecificCommandError,
    C772_E32 = RCS_E32_Busy
}C772_ReadJoinModeConfiguration_RespCodes;


/******************** CMD 782 *************************/

typedef struct
{
    uint8_t m_ucSessionIndex;
    uint8_t m_ucNoOfEntriesToRead;
}C782_ReadSessionEntries_Req;

typedef struct
{
    struct
    {
        uint32_t peerDeviceNonceCounterVal;
        uint32_t theDeviceNonceCounterVal;
        uint16_t peerDeviceNickname;
        _device_address_t peerDeviceUniqueId;
        SessionTypeCodes sessionType;
    } m_aSessions[C782_MAX_SESSIONS_LIST];
    uint8_t m_ucSessionIndex;
    uint8_t m_ucNoOfEntriesRead;
    uint8_t m_unNoOfActiveSessions;
}C782_ReadSessionEntries_Resp;

typedef enum
{
    C782_NOO = RCS_N00_Success,
    C782_E02 = RCS_E02_InvalidSelection,
    C782_E05 = RCS_E05_TooFewDataBytesReceived,
    C782_W08 = RCM_W08_SetToNearestPossibleValue
}C782_ReadSessionEntries_RespCodes;


/******************** CMD 789 *************************/

typedef EmptyCommand_Req C789_AlarmSourceRouteFailed_Req;

typedef struct
{
    uint32_t m_ulNetworkLayerMICfromNPDUthatFailedRouting;
    uint16_t m_unNicknameOfUnreachableNeighbor;
}C789_AlarmSourceRouteFailed_Resp;

typedef enum
{
    C789_NOO = RCS_N00_Success,
    C789_E16 = RCS_E16_AccessRestricted
}C789_AlarmSourceRouteFailed_RespCodes;


/******************** CMD 790 *************************/

typedef EmptyCommand_Req C790_AlarmGraphRouteFailed_Req;

typedef struct
{
    uint16_t m_unGraphIdOfFailedRoute;
}C790_AlarmGraphRouteFailed_Resp;

typedef enum
{
    C790_NOO = RCS_N00_Success,
    C790_E16 = RCS_E16_AccessRestricted
}C790_AlarmGraphRouteFailed_RespCodes;


/******************** CMD 791 *************************/

typedef EmptyCommand_Req C791_AlarmTransportLayerFailed_Req;

typedef struct
{
    uint16_t m_unNicknameOfUnreachablePeer;
}C791_AlarmTransportLayerFailed_Resp;

typedef enum
{
    C791_NOO = RCS_N00_Success,
    C791_E16 = RCS_E16_AccessRestricted
}C791_AlarmTransportLayerFailed_RespCodes;


/******************** CMD 799 *************************/

typedef struct
{
    ServiceRequestFlagsMasks m_ucServiceRequestFlags;
    ServiceApplicationDomain m_ucServiceApplicationDomain;
    uint16_t m_unNicknameOfPeer;
    _time_t m_tPeriod;
    uint8_t m_ucServiceId;
}C799_RequestService_Req;

typedef struct
{
    ServiceRequestFlagsMasks m_ucServiceRequestFlags;
    ServiceApplicationDomain m_ucServiceApplicationDomain;
    uint16_t m_unNicknameOfPeer;
    _time_t m_tPeriod;
    uint8_t m_ucServiceId;
    uint8_t m_ucRouteId;
}C799_RequestService_Resp;

typedef enum
{
    C799_NOO = RCS_N00_Success,
    C799_E04 = RCS_E04_PassedParameterTooSmall,
    C799_E05 = RCS_E05_TooFewDataBytesReceived,
    C799_E06 = RCS_E06_DeviceSpecificCommandError,
    C799_W08 = RCM_W08_SetToNearestPossibleValue,    
    C799_W14 = RCM_W14_CommBandwidthLessThanRequested,
    C799_E16 = RCS_E16_AccessRestricted,
    C799_E32 = RCS_E32_Busy,
    C799_E33 = RCS_E33_DelayedResponseInitiated,
    C799_E34 = RCS_E34_DelayedResponseRunning,
    C799_E35 = RCS_E35_DelayedResponseDead,
    C799_E36 = RCS_E36_DelayedResponseConflict,
    C799_E65 = RCM_E65_ServiceRequestDenied,
    C799_E66 = RCM_E66_UnknownServiceFlag,
    C799_E67 = RCM_E67_UnknownApplicationDomain,
    C799_E68 = RCM_E68_UnknownNickname,
    C799_E69 = RCM_E69_InvalidServiceId
}C799_RequestService_RespCodes;


/******************** CMD 800 *************************/

typedef struct
{
    uint8_t m_ucServiceIndex;
    uint8_t m_ucNoOfEntriesToRead;
}C800_ReadServiceList_Req;

typedef struct
{
    struct
    {
        ServiceRequestFlagsMasks serviceRequestFlags;
        ServiceApplicationDomain serviceApplicationDomain;
        uint16_t nicknameOfPeer;
        _time_t period;
        uint8_t serviceId;
        uint8_t routeId;
    } m_aServices[C800_MAX_SERVICES_LIST];

    uint8_t m_ucServiceIndex;
    uint8_t m_ucNoOfEntriesRead;
    uint8_t m_ucNoOfActiveServices;
}C800_ReadServiceList_Resp;

typedef enum
{
    C800_NOO = RCS_N00_Success,
    C800_E02 = RCS_E02_InvalidSelection,
    C800_E05 = RCS_E05_TooFewDataBytesReceived,
    C800_E06 = RCS_E06_DeviceSpecificCommandError,
    C800_E08 = RCM_W08_SetToNearestPossibleValue 
}C800_ReadServiceList_RespCodes;


/******************** CMD 801 *************************/

typedef struct
{
    uint8_t m_ucServiceId;
    uint8_t m_ucReason;   // ServiceDeletionReasonCodes
}C801_DeleteService_Req;

typedef struct
{
    uint8_t m_ucServiceId;
    uint8_t m_ucReason;   // ServiceDeletionReasonCodes
    uint8_t m_ucNoOfServiceEntriesRemaining;
}C801_DeleteService_Resp;

typedef enum
{
    C801_NOO = RCS_N00_Success,
    C801_E05 = RCS_E05_TooFewDataBytesReceived,
    C801_E16 = RCS_E16_AccessRestricted,
    C801_E65 = RCM_E65_EntryNotFound,
    C801_E66 = RCM_E66_InvalidReasonCode,
    C801_E67 = RCM_E67_ReasonCodeRejected,  // Service not deleted
    C801_E68 = RCM_E68_DeleteNotAllowed
}C801_DeleteService_RespCodes;


/******************** CMD 802 *************************/

typedef struct
{
    uint8_t m_ucRouteIndex;
    uint8_t m_ucNoOfEntriesToRead;
}C802_ReadRouteList_Req;

typedef struct
{
    uint8_t m_ucRouteIndex;
    uint8_t m_ucNoOfEntriesRead;
    uint8_t m_ucNoOfActiveRoutes;
    uint8_t m_ucNoOfRoutesRemaining;
    struct
    {
        uint8_t routeId;
        uint8_t sourceRouteAttached;
        uint16_t destinationNickname;
        uint16_t graphId;
    } m_aRoutes[C802_MAX_ROUTES_LIST];
}C802_ReadRouteList_Resp;

typedef enum
{
    C802_NOO = RCS_N00_Success,
    C802_E02 = RCS_E02_InvalidSelection,
    C802_E05 = RCS_E05_TooFewDataBytesReceived,
    C802_W08 = RCM_W08_SetToNearestPossibleValue
}C802_ReadRouteList_RespCodes;


/******************** CMD 803 *************************/

typedef struct
{
    uint8_t m_ucRouteId;
}C803_ReadSourceRoute_Req;

typedef struct
{
    uint8_t m_ucRouteId;
    uint8_t m_ucNoOfHops;
    uint16_t m_aHopNicknames[8];
}C803_ReadSourceRoute_Resp;

typedef enum
{
    C803_NOO = RCS_N00_Success,
    C803_E05 = RCS_E05_TooFewDataBytesReceived,  
    C803_E65 = RCM_E65_EntryNotFound
}C803_ReadSourceRoute_RespCodes;


/******************** CMD 808 *************************/

typedef EmptyCommand_Req C808_ReadTimeToLive_Req;

typedef struct
{
    uint8_t m_ucTimeToLive;
}C808_ReadTimeToLive_Resp;

typedef enum
{
    C808_NOO = RCS_N00_Success,
    C808_E06 = RCS_E06_DeviceSpecificCommandError,  
    C808_E32 = RCS_E32_Busy
}C808_ReadTimeToLive_RespCodes;


/******************** CMD 809 *************************/

typedef struct
{
    uint8_t m_ucTimeToLive;
}C809_WriteTimeToLive_Req;

typedef struct
{
    uint8_t m_ucTimeToLiveSet;
}C809_WriteTimeToLive_Resp;

typedef enum
{
    C809_NOO = RCS_N00_Success,
    C809_E05 = RCS_E05_TooFewDataBytesReceived,
    C809_E06 = RCS_E06_DeviceSpecificCommandError,
    C809_E07 = RCS_E07_InWriteProtectMode,
    C809_W08 = RCM_W08_SetToNearestPossibleValue,
    C809_E16 = RCS_E16_AccessRestricted,
    C809_E32 = RCS_E32_Busy,
    C809_E33 = RCS_E33_DelayedResponseInitiated,
    C809_E34 = RCS_E34_DelayedResponseRunning,
    C809_E35 = RCS_E35_DelayedResponseDead,
    C809_E36 = RCS_E36_DelayedResponseConflict    
}C809_WriteTimeToLive_RespCodes;


/******************** CMD 814 *************************/

typedef struct
{
    uint8_t m_ucDeviceListCode;         // DeviceListCode
    uint8_t m_ucNoOfListEntriesToRead;
    uint16_t m_unStartingListIndex;
}C814_ReadDeviceListEntries_Req;

typedef struct
{
    uint8_t m_ucDeviceListCode;         // DeviceListCode
    uint8_t m_ucNoOfListEntriesRead;
    uint16_t m_unStartingListIndex;
    uint16_t m_unTotalNoOfEntriesInList;

    // if noOfListEntriesToRead > MAX_DEVICE_LIST_ENTRIES_NO
    // a SetToNearestPossibleValue warning should be issued
    _device_address_t m_aDeviceUniqueIds[MAX_DEVICE_LIST_ENTRIES_NO];

}C814_ReadDeviceListEntries_Resp;

typedef enum
{
    C814_NOO = RCS_N00_Success,
    C814_E02 = RCS_E02_InvalidSelection,
    C814_E05 = RCS_E05_TooFewDataBytesReceived,
    C814_W08 = RCM_W08_SetToNearestPossibleValue
}C814_ReadDeviceListEntries_RespCodes;


/******************** CMD 815 *************************/

typedef struct
{
    _device_address_t m_aDeviceUniqueId;
    uint8_t m_ucDeviceListCode;         // DeviceListCode
}C815_AddDeviceListTableEntry_Req;

typedef struct
{
    uint16_t m_unNoOfListEntriesRemaining;
    _device_address_t m_aDeviceUniqueId;
    uint8_t m_ucDeviceListCode;         // DeviceListCode
}C815_AddDeviceListTableEntry_Resp;

typedef enum
{
    C815_NOO = RCS_N00_Success,
    C815_E02 = RCS_E02_InvalidSelection,
    C815_E05 = RCS_E05_TooFewDataBytesReceived,
    C815_W08 = RCM_W08_DeviceAlreadyInList,
    C815_E16 = RCS_E16_AccessRestricted,
    C815_E65 = RCM_E65_NoMoreEntriesAvailable,
    C815_E66 = RCM_E66_DeviceListConflict
}C815_AddDeviceListTableEntry_RespCodes;


/******************** CMD 816 *************************/

typedef struct
{
    _device_address_t m_aDeviceUniqueId;
    uint8_t m_ucDeviceListCode;         // DeviceListCode
}C816_DeleteDeviceListTableEntry_Req;

typedef struct
{
    uint16_t m_unNoOfListEntriesRemaining;
    _device_address_t m_aDeviceUniqueId;
    uint8_t m_ucDeviceListCode;         // DeviceListCode
}C816_DeleteDeviceListTableEntry_Resp;

typedef enum
{
    C816_NOO = RCS_N00_Success,
    C816_E02 = RCS_E02_InvalidSelection,
    C816_E05 = RCS_E05_TooFewDataBytesReceived,
    C816_E16 = RCS_E16_AccessRestricted,
    C816_E66 = RCM_E66_DeviceListConflict
}C816_DeleteDeviceListTableEntry_RespCodes;


/******************** CMD 817 *************************/

typedef EmptyCommand_Req C817_ReadChannelBlacklist_Req;

typedef struct
{
    uint16_t m_unCurrentChannelMapArray;
    uint16_t m_unPendingChannelMapArray;
    uint8_t m_ucNoOfBitsInCurrentChannelMapArray;
}C817_ReadChannelBlacklist_Resp;

typedef enum
{
    C817_NOO = RCS_N00_Success
}C817_ReadChannelBlacklist_RespCodes;

/******************** CMD 818 *************************/

typedef struct
{
    uint16_t m_unPendingChannelMapArray;
    uint8_t m_ucNoOfBitsInNewChannelMapArray;
}C818_WriteChannelBlacklist_Req;

typedef struct
{
    uint16_t m_unPendingChannelMapArray;
    uint8_t m_ucNoOfBitsInNewChannelMapArray;
}C818_WriteChannelBlacklist_Resp;

typedef enum
{
    C818_NOO = RCS_N00_Success,
    C818_E03 = RCS_E03_PassedParameterTooLarge,
    C818_E05 = RCS_E05_TooFewDataBytesReceived,
    C818_E06 = RCS_E06_DeviceSpecificCommandError,
    C818_E16 = RCS_E16_AccessRestricted,
    C818_E65 = RCM_E65_IllegalFrequencyChannelBits
}C818_WriteChannelBlacklist_RespCodes;


/******************** CMD 821 *************************/

typedef struct
{
    uint8_t m_ucNetworkAccessModeCode;    // NetworkAccessModeCode
}C821_WriteNetworkAccessMode_Req;

typedef struct
{
    uint8_t m_ucNetworkAccessModeCode;    // NetworkAccessModeCode
}C821_WriteNetworkAccessMode_Resp;

typedef enum
{
    C821_NOO = RCS_N00_Success,
    C821_E02 = RCS_E02_InvalidSelection,
    C821_E05 = RCS_E05_TooFewDataBytesReceived,
    C821_E06 = RCS_E06_DeviceSpecificCommandError,
    C821_E07 = RCS_E07_InWriteProtectMode,
    C821_E16 = RCS_E16_AccessRestricted,
    C821_E32 = RCS_E32_Busy,
    C821_E33 = RCS_E33_DelayedResponseInitiated,
    C821_E34 = RCS_E34_DelayedResponseRunning,
    C821_E35 = RCS_E35_DelayedResponseDead,
    C821_E36 = RCS_E36_DelayedResponseConflict
}C821_WriteNetworkAccessMod_RespCodes;


/******************** CMD 822 *************************/

typedef EmptyCommand_Req C822_ReadNetworkAccessMode_Req;

typedef struct
{
    uint8_t m_ucNetworkAccessModeCode;    // NetworkAccessModeCode
}C822_ReadNetworkAccessMode_Resp;

typedef enum
{
    C822_NOO = RCS_N00_Success,
    C822_E06 = RCS_E06_DeviceSpecificCommandError,
    C822_E32 = RCS_E32_Busy
}C822_ReadNetworkAccessMode_RespCodes;

/******************** CMD 823 *************************/

typedef struct
{
    uint16_t m_unPeerDeviceNickname;
}C823_RequestSession_Req;

typedef struct
{
    uint8_t m_aKeyVal[16];
    uint32_t m_ulPeerDeviceNonceCounterVal;
    uint16_t m_unPeerDeviceNickname;
}C823_RequestSession_Resp;

typedef enum
{
    C823_NOO = RCS_N00_Success,
    C823_E05 = RCS_E05_TooFewDataBytesReceived,
    C823_E16 = RCS_E16_AccessRestricted,
    C823_E65 = RCM_E65_UnknownNickname,
    C823_E66 = RCM_E66_PeerHasInsufficientCapacityForAnotherSession
}C823_RequestSession_RespCodes;





#endif /* NETWORK_LAYER_COMMANDS_H_ */
