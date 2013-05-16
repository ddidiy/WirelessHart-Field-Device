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

#ifndef DLL_LAYER_COMMANDS_H_
#define DLL_LAYER_COMMANDS_H_

#include "CommonTables.h"

// assuming the following: 
// Max. APDU len = 92 (in WirelessHART format)
// Max. Data len, for a non-aggregated command response: 92-4=88

#define C783_MAX_SUPERFRAMES_LIST     ((88 - 3) / 4)
#define C784_MAX_LINKS_LIST           ((88 - 5) / 8)
#define C785_MAX_NEIGHBORS_LIST       ((88 - 5) / 2)  // this will restrict the max number of connections per graph to 41
#define C787_MAX_NEIGHBORS_LIST       ((88 - 3) / 3)


enum
{
  CMDID_C773_WriteNetworkId               = 773,
  CMDID_C774_ReadNetworkId                = 774,
  CMDID_C775_WriteNetworkTag              = 775,
  CMDID_C776_ReadNetworkTag               = 776,  
  CMDID_C781_ReadDeviceNicknameAddress    = 781,
  CMDID_C783_ReadSuperframeList           = 783,
  CMDID_C784_ReadLinkList                 = 784,
  CMDID_C785_ReadGraphList                = 785,
  CMDID_C786_ReadNeighborPropertyFlag     = 786,
  CMDID_C787_ReportNeighborSignalLevels   = 787,
  CMDID_C788_AlarmPathDown                = 788,
  CMDID_C795_WriteTimerInterval           = 795,
  CMDID_C796_ReadTimerInterval            = 796,
  CMDID_C806_ReadHandheldSuperframe       = 806,
  CMDID_C807_RequestHandheldSuperframeMode= 807,
  CMDID_C810_ReadJoinPriority             = 810,
  CMDID_C811_WriteJoinPriority            = 811,
  CMDID_C812_ReadPacketReceivePriority    = 812,
  CMDID_C813_WritePacketReceivePriority   = 813,
  CMDID_C819_ReadBackOffExponent          = 819,
  CMDID_C820_WriteBackOffExponent         = 820
};

enum
{
  C773_ReqSize =  2,
  C774_ReqSize =  0,
  C775_ReqSize = 32,
  C776_ReqSize =  0,  
  C781_ReqSize =  0,
  C783_ReqSize =  2,
  C784_ReqSize =  3,
  C785_ReqSize =  1,
  C786_ReqSize =  2,
  C787_ReqSize =  2,
  C788_ReqSize =  0,
  C795_ReqSize =  5,
  C796_ReqSize =  1,
  C806_ReqSize =  0,
  C807_ReqSize =  2,
  C810_ReqSize =  0,
  C811_ReqSize =  1,
  C812_ReqSize =  0,
  C813_ReqSize =  1,
  C819_ReqSize =  0,
  C820_ReqSize =  1
};

enum
{
  C773_RespSize =  2,
  C774_RespSize =  2,
  C775_RespSize = 32,
  C776_RespSize = 32,  
  C781_RespSize =  2,
  C783_RespSize =  7, // or 11, 15, ... (variable size)
  C784_RespSize = 13, // or 21, 29, ... (variable size)
  C785_RespSize =  7, // or 9, 11, ... (variable size)
  C786_RespSize =  3,
  C787_RespSize =  6, // or 9, 12, ... (variable size)
  C788_RespSize =  2,
  C795_RespSize =  5,
  C796_RespSize =  5,
  C806_RespSize =  4,
  C807_RespSize =  2,
  C810_RespSize =  1,
  C811_RespSize =  1,
  C812_RespSize =  1,
  C813_RespSize =  1,
  C819_RespSize =  1,
  C820_RespSize =  1
};

#define C773_WriteNetworkId_ReqSize                 C773_ReqSize
#define C774_ReadNetworkId_ReqSize                  C774_ReqSize
#define C775_WriteNetworkTag_ReqSize                C775_ReqSize
#define C776_ReadNetworkTag_ReqSize                 C776_ReqSize
#define C781_ReadDeviceNicknameAddress_ReqSize      C781_ReqSize
#define C783_ReadSuperframeList_ReqSize             C783_ReqSize
#define C784_ReadLinkList_ReqSize                   C784_ReqSize
#define C785_ReadGraphList_ReqSize                  C785_ReqSize
#define C786_ReadNeighborPropertyFlag_ReqSize       C786_ReqSize
#define C787_ReportNeighborSignalLevels_ReqSize     C787_ReqSize
#define C788_AlarmPathDown_ReqSize                  C788_ReqSize
#define C795_WriteTimerInterval_ReqSize             C795_ReqSize
#define C796_ReadTimerInterval_ReqSize              C796_ReqSize
#define C806_ReadHandheldSuperframe_ReqSize         C806_ReqSize
#define C807_RequestHandheldSuperframeMode_ReqSize  C807_ReqSize
#define C810_ReadJoinPriority_ReqSize               C810_ReqSize
#define C811_WriteJoinPriority_ReqSize              C811_ReqSize
#define C812_ReadPacketReceivePriority_ReqSize      C812_ReqSize
#define C813_WritePacketReceivePriority_ReqSize     C813_ReqSize
#define C819_ReadBackOffExponent_ReqSize            C819_ReqSize
#define C820_WriteBackOffExponent_ReqSize           C820_ReqSize

#define C773_WriteNetworkId_RespSize                C773_RespSize
#define C774_ReadNetworkId_RespSize                 C774_RespSize
#define C775_WriteNetworkTag_RespSize               C775_RespSize
#define C776_ReadNetworkTag_RespSize                C776_RespSize
#define C781_ReadDeviceNicknameAddress_RespSize     C781_RespSize
#define C783_ReadSuperframeList_RespSize            C783_RespSize
#define C784_ReadLinkList_RespSize                  C784_RespSize
#define C785_ReadGraphList_RespSize                 C785_RespSize
#define C786_ReadNeighborPropertyFlag_RespSize      C786_RespSize
#define C787_ReportNeighborSignalLevels_RespSize    C787_RespSize
#define C788_AlarmPathDown_RespSize                 C788_RespSize
#define C795_WriteTimerInterval_RespSize            C795_RespSize
#define C796_ReadTimerInterval_RespSize             C796_RespSize
#define C806_ReadHandheldSuperframe_RespSize        C806_RespSize
#define C807_RequestHandheldSuperframeMode_RespSize C807_RespSize
#define C810_ReadJoinPriority_RespSize              C810_RespSize
#define C811_WriteJoinPriority_RespSize             C811_RespSize
#define C812_ReadPacketReceivePriority_RespSize     C812_RespSize
#define C813_WritePacketReceivePriority_RespSize    C813_RespSize
#define C819_ReadBackOffExponent_RespSize           C819_RespSize
#define C820_WriteBackOffExponent_RespSize          C820_RespSize



/******************** CMD 773 *************************/

typedef struct
{
    uint16_t m_unNetworkId;
}C773_WriteNetworkId_Req;

typedef struct
{
    uint16_t m_unNetworkId;
}C773_WriteNetworkId_Resp;

typedef enum
{
    C773_NOO = RCS_N00_Success,
    C773_E05 = RCS_E05_TooFewDataBytesReceived,
    C773_E06 = RCS_E06_DeviceSpecificCommandError,
    C773_E07 = RCS_E07_InWriteProtectMode,    
    C773_W08 = RCM_W08_NetworkIDChangePending,
    C773_E16 = RCS_E16_AccessRestricted,
    C773_E32 = RCS_E32_Busy,
    C773_E33 = RCS_E33_DelayedResponseInitiated,
    C773_E34 = RCS_E34_DelayedResponseRunning,
    C773_E35 = RCS_E35_DelayedResponseDead,
    C773_E36 = RCS_E36_DelayedResponseConflict,    
    C773_E65 = RCM_E65_InvalidNetworkID
}C773_WriteNetworkId_RespCodes;


/******************** CMD 774 *************************/

typedef EmptyCommand_Req  C774_ReadNetworkId_Req;   // empty req

typedef struct
{
    uint16_t m_unNetworkId;
}C774_ReadNetworkId_Resp;

typedef enum
{
    C774_NOO = RCS_N00_Success,
    C774_E06 = RCS_E06_DeviceSpecificCommandError,
    C774_E32 = RCS_E32_Busy
}C774_ReadNetworkId_RespCodes;


/******************** CMD 775 *************************/

typedef struct
{
    uint8_t m_aNetworkTag[32];
}C775_WriteNetworkTag_Req;

typedef struct
{
    uint8_t m_aNetworkTag[32];
}C775_WriteNetworkTag_Resp;

typedef enum
{
    C775_N00 = RCS_N00_Success,
    C775_E05 = RCS_E05_TooFewDataBytesReceived,
    C775_E06 = RCS_E06_DeviceSpecificCommandError,
    C775_E07 = RCS_E07_InWriteProtectMode,
    C775_E16 = RCS_E16_AccessRestricted,
    C775_E32 = RCS_E32_Busy,
    C775_E33 = RCS_E33_DelayedResponseInitiated,
    C775_E34 = RCS_E34_DelayedResponseRunning,
    C775_E35 = RCS_E35_DelayedResponseDead,
    C775_E36 = RCS_E36_DelayedResponseConflict
}C775_WriteNetworkTag_RespCodes;


/******************** CMD 776 *************************/

typedef EmptyCommand_Req  C776_ReadNetworkTag_Req;   // empty req

typedef struct
{
    uint8_t m_aNetworkTag[32];
}C776_ReadNetworkTag_Resp;

typedef enum
{
    C776_N00 = RCS_N00_Success,
    C776_E06 = RCS_E06_DeviceSpecificCommandError,
    C776_E32 = RCS_E32_Busy
}C776_ReadNetworkTag_RespCodes;


/******************** CMD 781 *************************/

typedef EmptyCommand_Req  C781_ReadDeviceNicknameAddress_Req;

typedef struct
{
    uint16_t Nickname;
}C781_ReadDeviceNicknameAddress_Resp;

typedef enum
{
    C781_N00 = RCS_N00_Success
}C781_ReadDeviceNicknameAddress_RespCodes;


/******************** CMD 783 *************************/
typedef struct
{
    uint8_t m_ucSuperframeIndex;
    uint8_t m_ucNoOfEntriesToRead;
}C783_ReadSuperframeList_Req;

typedef struct
{
    struct
    {
        uint8_t superframeId;
        SuperframeModeFlagsMasks superframeModeFlags;
        uint16_t noOfSlotsInSuperframe;
    } m_aSuperframes[C783_MAX_SUPERFRAMES_LIST];
    uint8_t m_ucSuperframeIndex;
    uint8_t m_ucNoOfEntriesRead;
    uint8_t m_ucNoOfActiveSuperframes;
}C783_ReadSuperframeList_Resp;

typedef enum
{
    C783_N00 = RCS_N00_Success,
    C783_E02 = RCS_E02_InvalidSelection,
    C783_E05 = RCS_E05_TooFewDataBytesReceived,
    C783_W08 = RCM_W08_SetToNearestPossibleValue
}C783_ReadSuperframeList_RespCodes;


/******************** CMD 784 *************************/

typedef struct
{
    uint16_t m_unLinkIndex;
    uint8_t m_ucNoOfLinksToRead;
}C784_ReadLinkList_Req;

typedef struct
{
    struct
    {
        uint8_t superframeId;
        uint8_t channelOffsetForThisLink;
        uint16_t slotNoForThisLink;
        uint16_t nicknameOfNeighborForThisLink;
        // LinkOptionFlagCodesMasks linkOptions; //doinel.alban: flag combinations may be set, too
        uint8_t linkOptions;
        LinkType linkType;
    } m_aLinks[C784_MAX_LINKS_LIST];
    uint16_t m_unLinkIndex;
    uint16_t m_unNoOfActiveLinks;
    uint8_t m_ucNoOfLinksRead;
}C784_ReadLinkList_Resp;

typedef enum
{
    C784_N00 = RCS_N00_Success,
    C784_E02 = RCS_E02_InvalidSelection,
    C784_E05 = RCS_E05_TooFewDataBytesReceived,
    C784_W08 = RCM_W08_SetToNearestPossibleValue
}C784_ReadLinkList_RespCodes;


/******************** CMD 785 *************************/

typedef struct
{
    uint8_t m_ucGraphListIndex;
}C785_ReadGraphList_Req;

typedef struct
{
    uint16_t m_aNicknameOfNeighbor[C785_MAX_NEIGHBORS_LIST];
    uint8_t m_ucGraphListIndex;
    uint8_t m_ucTotalNoOfGraphs;
    uint16_t m_unGraphId;
    uint8_t m_ucNoOfNeighbors;
}C785_ReadGraphList_Resp;

typedef enum
{
    C785_N00 = RCS_N00_Success,
    C785_E02 = RCS_E02_InvalidSelection,
    C785_E05 = RCS_E05_TooFewDataBytesReceived,  
    C785_W08 = RCM_W08_SetToNearestPossibleValue
}C785_ReadGraphList_RespCodes;


/******************** CMD 786 *************************/

typedef struct
{
    uint16_t Nickname;
}C786_ReadNeighborPropertyFlag_Req;

typedef struct
{
    uint16_t Nickname;
    uint8_t NeighbourFlag;   // 1 byte, bit0 (Common Table 59)
}C786_ReadNeighborPropertyFlag_Resp;

typedef enum
{
    C786_N00 = RCS_N00_Success,
    C786_E05 = RCS_E05_TooFewDataBytesReceived,    
    C786_E65 = RCM_E65_UnknownNickname
}C786_ReadNeighborPropertyFlag_RespCodes;


/******************** CMD 787 *************************/

typedef struct
{
    uint8_t m_ucNeighborTableIndex;
    uint8_t m_ucNoOfNeighborEntriesToRead;
}C787_ReportNeighborSignalLevels_Req;

typedef struct
{
    uint8_t m_ucNeighborTableIndex;
    uint8_t m_ucNoOfNeighborEntriesRead;
    uint8_t m_ucTotalNoOfNeighbors;
    struct
    {
        int8_t RSLindB;
        uint16_t nickname;
    } m_aNeighbors[C787_MAX_NEIGHBORS_LIST];

}C787_ReportNeighborSignalLevels_Resp;

typedef enum
{
    C787_N00 = RCS_N00_Success,
    C787_E05 = RCS_E05_TooFewDataBytesReceived,   
    C787_W08 = RCM_W08_SetToNearestPossibleValue
}C787_ReportNeighborSignalLevels_RespCodes;


/******************** CMD 788 *************************/


typedef EmptyCommand_Req  C788_AlarmPathDown_Req;

typedef struct
{
    uint16_t Nickname;
}C788_AlarmPathDown_Resp;

typedef enum
{
    C788_N00 = RCS_N00_Success,
    C788_E16 = RCS_E16_AccessRestricted
}C788_AlarmPathDown_RespCodes;

/******************** CMD 795 *************************/

typedef struct
{
    uint32_t m_ulTimerInterval;
    uint8_t m_ucTimerType;    // WirelessTimerCode
}C795_WriteTimerInterval_Req;

typedef struct
{
    uint32_t m_ulTimerInterval;
    uint8_t m_ucTimerType;    // WirelessTimerCode
}C795_WriteTimerInterval_Resp;

typedef enum
{
    C795_NOO = RCS_N00_Success,
    C795_EO3 = RCS_E03_PassedParameterTooLarge,
    C795_EO4 = RCS_E04_PassedParameterTooSmall,
    C795_E05 = RCS_E05_TooFewDataBytesReceived,
    C795_E06 = RCS_E06_DeviceSpecificCommandError,
    C795_E07 = RCS_E07_InWriteProtectMode,
    C795_W08 = RCM_W08_SetToNearestPossibleValue,
    C795_E16 = RCS_E16_AccessRestricted,
    C795_E32 = RCS_E32_Busy,
    C795_E33 = RCS_E33_DelayedResponseInitiated,
    C795_E34 = RCS_E34_DelayedResponseRunning,
    C795_E35 = RCS_E35_DelayedResponseDead,
    C795_E36 = RCS_E36_DelayedResponseConflict, 
    C795_E65 = RCM_E65_InvalidTimerType
}C795_WriteTimerInterval_RespCodes;


/******************** CMD 796 *************************/

typedef struct
{
    uint8_t m_ucTimerType;    // WirelessTimerCode
}C796_ReadTimerInterval_Req;

typedef struct
{
    uint32_t m_ulTimerInterval;
    uint8_t m_ucTimerType;    // WirelessTimerCode
}C796_ReadTimerInterval_Resp;

typedef enum
{
    C796_NOO = RCS_N00_Success,
    C796_E05 = RCS_E05_TooFewDataBytesReceived,
    C796_E06 = RCS_E06_DeviceSpecificCommandError,
    C796_E32 = RCS_E32_Busy,
    C796_E65 = RCM_E65_InvalidTimerType
}C796_ReadTimerInterval_RespCodes;


/******************** CMD 806 *************************/

typedef EmptyCommand_Req  C806_ReadHandheldSuperframe_Req;

typedef struct
{
    uint8_t m_ucSuperframeId;
    SuperframeModeFlagsMasks m_ucSuperframeModeFlags;
    uint16_t m_unNoOfSlotsInSuperframe;
}C806_ReadHandheldSuperframe_Resp;


typedef enum
{
    C806_NOO = RCS_N00_Success,
    C806_E09 = RCM_E09_NoHandheldSuperframe
}C806_ReadHandheldSuperframe_RespCodes;


/******************** CMD 807 *************************/

typedef struct
{
    uint8_t m_ucSuperframeId;
    SuperframeModeFlagsMasks m_ucSuperframeModeFlags;
}C807_RequestHandheldSuperframeMode_Req;

typedef struct
{ 
    uint8_t m_ucSuperframeId;
    SuperframeModeFlagsMasks m_ucSuperframeModeFlags;
}C807_RequestHandheldSuperframeMode_Resp;

typedef enum
{
    C807_NOO = RCS_N00_Success,
    C807_E02 = RCS_E02_InvalidSelection,
    C807_E05 = RCS_E05_TooFewDataBytesReceived,
    C807_E16 = RCS_E16_AccessRestricted
}C807_RequestHandheldSuperframeMode_RespCodes;


/******************** CMD 810 *************************/

typedef EmptyCommand_Req  C810_ReadJoinPriority_Req;

typedef struct
{
    uint8_t JoinPriority;
}C810_ReadJoinPriority_Resp;

typedef enum
{
    C810_NOO = RCS_N00_Success
}C810_ReadJoinPriority_RespCodes;

/******************** CMD 811 *************************/

typedef struct
{
    uint8_t JoinPriority;
}C811_WriteJoinPriority_Req;

typedef struct
{
    uint8_t JoinPriority;
}C811_WriteJoinPriority_Resp;

typedef enum
{
    C811_NOO = RCS_N00_Success,
    C811_E05 = RCS_E05_TooFewDataBytesReceived,
    C811_W08 = RCM_W08_SetToNearestPossibleValue,
    C811_E16 = RCS_E16_AccessRestricted
}C811_WriteJoinPriority_RespCodes;


/******************** CMD 812 *************************/

typedef EmptyCommand_Req  C812_ReadPacketReceivePriority_Req;

typedef struct
{
    uint8_t PacketRecPriority;
}C812_ReadPacketReceivePriority_Resp;

typedef enum
{
    C812_NOO = RCS_N00_Success
}C812_ReadPacketReceivePriority_RespCodes;


/******************** CMD 813 *************************/

typedef struct
{
    uint8_t PacketRecPriority;
}C813_WritePacketReceivePriority_Req;

typedef struct
{
    uint8_t PacketRecPriority;
}C813_WritePacketReceivePriority_Resp;

typedef enum
{
    C813_NOO = RCS_N00_Success,
    C813_E05 = RCS_E05_TooFewDataBytesReceived,
    C813_W08 = RCM_W08_SetToNearestPossibleValue,
    C813_E16 = RCS_E16_AccessRestricted
}C813_WritePacketReceivePriority_RespCodes;


/******************** CMD 819 *************************/

typedef EmptyCommand_Req  C819_ReadBackOffExponent_Req;

typedef struct
{
    uint8_t MaxBackOffExp;
}C819_ReadBackOffExponent_Resp;

typedef enum
{
    C819_NOO = RCS_N00_Success,
    C819_E06 = RCS_E06_DeviceSpecificCommandError,
    C819_E32 = RCS_E32_Busy
}C819_ReadBackOffExponent_RespCodes;


/******************** CMD 820 *************************/

typedef struct
{
    uint8_t MaxBackOffExp;
}C820_WriteBackOffExponent_Req;

typedef struct
{
    uint8_t MaxBackOffExp;
}C820_WriteBackOffExponent_Resp;

typedef enum
{
    C820_NOO = RCS_N00_Success,
    C820_E03 = RCS_E03_PassedParameterTooLarge,
    C820_E04 = RCS_E04_PassedParameterTooSmall,
    C820_E05 = RCS_E05_TooFewDataBytesReceived,
    C820_E06 = RCS_E06_DeviceSpecificCommandError,
    C820_E07 = RCS_E07_InWriteProtectMode,
    C820_W08 = RCM_W08_SetToNearestPossibleValue,
    C820_E16 = RCS_E16_AccessRestricted,
    C820_E32 = RCS_E32_Busy,
    C820_E33 = RCS_E33_DelayedResponseInitiated,
    C820_E34 = RCS_E34_DelayedResponseRunning,
    C820_E35 = RCS_E35_DelayedResponseDead,
    C820_E36 = RCS_E36_DelayedResponseConflict     
}C820_WriteBackOffExponent_RespCodes;


#endif /* DLL_LAYER_COMMANDS_H_ */
