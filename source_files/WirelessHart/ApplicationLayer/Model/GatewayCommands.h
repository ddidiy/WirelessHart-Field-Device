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

#ifndef GATEWAYCOMMANDS_H
#define GATEWAYCOMMANDS_H

#include "CommonTables.h"


// computed for max packet size = 128
//TODO replace defines
#define MaxNeighbours 7
#define MaxGraphIds 57
#define MaxBurstCommands 20
#define MaxChangeNotifications 61


enum
{
	CMDID_C832_ReadNetworkDeviceIdentity            = 832,
	CMDID_C833_ReadNetworkDeviceNeighbourHealth     = 833,
	CMDID_C834_ReadNetworkTopologyInformation       = 834,
	CMDID_C835_ReadBurstMessageList                 = 835,
	CMDID_C836_FlushCacheResponses                  = 836,
	CMDID_C837_WriteUpdateNotificationBitMask       = 837,
	CMDID_C838_ReadUpdateNotificationBitMask        = 838,
	CMDID_C839_ChangeNotification                   = 839,
	CMDID_C840_ReadDeviceStatistics                 = 840,
	CMDID_C841_ReadNetworkDeviceIdentityByNickname  = 841,
	CMDID_C842_WriteDeviceSchedulingFlags           = 842,
	CMDID_C843_ReadDeviceSchedulingFlags            = 843,
	CMDID_C844_ReadNetworkConstraints               = 844,
	CMDID_C845_WriteNetworkConstraints              = 845
};

enum
{
  C832_ReqSize =  5,
  C833_ReqSize = 10,
  C834_ReqSize =  7,
  C835_ReqSize =  5,
  C836_ReqSize =  5,
  C837_ReqSize =  7,
  C838_ReqSize =  5,
  C839_ReqSize =  5,
  C840_ReqSize =  5,
  C841_ReqSize =  2,
  C842_ReqSize =  6,
  C843_ReqSize =  5,
  C844_ReqSize =  0,
  C845_ReqSize =  2
};

enum
{
  C832_RespSize = 39,
  C833_RespSize = 25, // or 40, 55, ... (variable size)
  C834_RespSize = 15, // or 17, 19, ... (variable size)
  C835_RespSize = 12, // or 18, 24, ... (variable size)
  C836_RespSize =  5,
  C837_RespSize =  7,
  C838_RespSize =  7,
  C839_RespSize =  8, // or 10, 12, ... 26 (variable size)
  C840_RespSize = 57,
  C841_RespSize = 39,
  C842_RespSize =  6,
  C843_RespSize =  6,
  C844_RespSize =  2,
  C845_RespSize =  2
};


#define C832_ReadNetworkDeviceIdentity_ReqSize            C832_ReqSize
#define C833_ReadNetworkDeviceNeighbourHealth_ReqSize     C833_ReqSize
#define C834_ReadNetworkTopologyInformation_ReqSize       C834_ReqSize
#define C835_ReadBurstMessageList_ReqSize                 C835_ReqSize
#define C836_FlushCacheResponses_ReqSize                  C836_ReqSize
#define C837_WriteUpdateNotificationBitMask_ReqSize       C837_ReqSize
#define C838_ReadUpdateNotificationBitMask_ReqSize        C838_ReqSize
#define C839_ChangeNotification_ReqSize                   C839_ReqSize
#define C840_ReadDeviceStatistics_ReqSize                 C840_ReqSize
#define C841_ReadNetworkDeviceIdentityByNickname_ReqSize  C841_ReqSize
#define C842_WriteDeviceSchedulingFlags_ReqSize           C842_ReqSize
#define C843_ReadDeviceSchedulingFlags_ReqSize            C843_ReqSize
#define C844_ReadNetworkConstraints_ReqSize               C844_ReqSize
#define C845_WriteNetworkConstraints_ReqSize              C845_ReqSize


#define C832_ReadNetworkDeviceIdentity_RespSize           C832_RespSize
#define C833_ReadNetworkDeviceNeighbourHealth_RespSize    C833_RespSize
#define C834_ReadNetworkTopologyInformation_RespSize      C834_RespSize
#define C835_ReadBurstMessageList_RespSize                C835_RespSize
#define C836_FlushCacheResponses_RespSize                 C836_RespSize
#define C837_WriteUpdateNotificationBitMask_RespSize      C837_RespSize
#define C838_ReadUpdateNotificationBitMask_RespSize       C838_RespSize
#define C839_ChangeNotification_RespSize                  C839_RespSize
#define C840_ReadDeviceStatistics_RespSize                C840_RespSize
#define C841_ReadNetworkDeviceIdentityByNickname_RespSize C841_RespSize
#define C842_WriteDeviceSchedulingFlags_RespSize          C842_RespSize
#define C843_ReadDeviceSchedulingFlags_RespSize           C843_RespSize
#define C844_ReadNetworkConstraints_RespSize              C844_RespSize
#define C845_WriteNetworkConstraints_RespSize             C845_RespSize


/******************** CMD 832 *************************/

typedef struct
{
	_device_address_t DeviceUniqueID;
}C832_ReadNetworkDeviceIdentity_Req;

typedef struct
{
	_device_address_t DeviceUniqueID;
	uint16_t Nickname;
	char LongTag[32];
}C832_ReadNetworkDeviceIdentity_Resp;

typedef enum
{
    C832_NOO = RCS_N00_Success,
    C832_E02 = RCS_E02_InvalidSelection,
    C832_E05 = RCS_E05_TooFewDataBytesReceived
}C832_ReadNetworkDeviceIdentity_RespCodes;

/******************** CMD 833 *************************/

typedef struct
{
	_device_address_t UniqueID;
	uint8_t NeighbourIndex;
	uint8_t NeighbourEntriesToRead;
}C833_ReadNetworkDeviceNeighbourHealth_Req;


	typedef struct
	{
		uint16_t NeighbourNickname;
		int8_t NeighbourRSL;
		uint32_t TransmittedPacketCount;
		uint32_t TransmittedPacketWithNoACKCount;
		uint32_t ReceivedPacketCount;
	} NeighbourDescription;


typedef struct
{
	_device_address_t UniqueID;
	uint8_t NeighbourIndex;
	uint8_t NeighbourCount;
	NeighbourDescription Neighbours[MaxNeighbours];
}C833_ReadNetworkDeviceNeighbourHealth_Resp;

typedef enum
{
    C833_NOO = RCS_N00_Success,
    C833_E02 = RCS_E02_InvalidSelection,
    C833_E05 = RCS_E05_TooFewDataBytesReceived,
    C833_W08 = RCM_W08_SetToNearestPossibleValue,
    C833_E65 = RCM_E65_InvalidNeighborTableIndex
}C833_ReadNetworkDeviceNeighbourHealth_RespCodes;


/******************** CMD 834 *************************/

typedef struct
{
	_device_address_t DeviceLongAddress;
	uint16_t GraphIndexNo;
}C834_ReadNetworkTopologyInformation_Req;

typedef struct
{
	_device_address_t DeviceLongAddress;
	uint16_t GraphIndexNo;
	uint16_t TotalGraphsNo;
	uint16_t IndexGraphId;
	uint16_t NeighboursNo;
	uint16_t Neighbour[MaxGraphIds];
}C834_ReadNetworkTopologyInformation_Resp;

typedef enum
{
    C834_NOO = RCS_N00_Success,
    C834_E02 = RCS_E02_InvalidSelection,
    C834_E05 = RCS_E05_TooFewDataBytesReceived,
    C834_E65 = RCM_E65_EntryNotFound
}C834_ReadNetworkTopologyInformation_RespCodes;


/******************** CMD 835 *************************/

typedef struct
{
	_device_address_t UniqueID;
}C835_ReadBurstMessageList_Req;

	typedef struct
	{
		uint16_t CommandNoBeingBurst;
		uint32_t ReceivedBurstPacketsNo;
	} BurstCommandDesc;

typedef struct
{
	_device_address_t UniqueID;
	uint8_t DifferentBurstCommandsNo;
	BurstCommandDesc BurstCommands[MaxBurstCommands];

}C835_ReadBurstMessageList_Resp;

typedef enum
{
    C835_NOO = RCS_N00_Success,
    C835_E02 = RCS_E02_InvalidSelection,
    C835_E05 = RCS_E05_TooFewDataBytesReceived
}C835_ReadBurstMessageList_RespCodes;


/******************** CMD 836 *************************/

typedef struct
{
	_device_address_t UniqueID;
}C836_FlushCacheResponses_Req;

typedef struct
{
	_device_address_t UniqueID;
}C836_FlushCacheResponses_Resp;

typedef enum
{
    C836_NOO = RCS_N00_Success,
    C836_E05 = RCS_E05_TooFewDataBytesReceived,
    C836_E16 = RCS_E16_AccessRestricted
}C836_FlushCacheResponses_RespCodes;


/******************** CMD 837 *************************/

typedef struct
{
	_device_address_t UniqueID;
	uint16_t ChangeNotificationFlags;
}C837_WriteUpdateNotificationBitMask_Req;

typedef struct
{
	_device_address_t UniqueID;
	uint16_t ChangeNotificationFlags;
}C837_WriteUpdateNotificationBitMask_Resp;

typedef enum
{
    C837_NOO = RCS_N00_Success,
    C837_E05 = RCS_E05_TooFewDataBytesReceived,
    C837_E16 = RCS_E16_AccessRestricted,
    C837_E65 = RCM_E65_UnknownUID,
    C837_E66 = RCM_E66_UnknownNotificationFlag
}C837_WriteUpdateNotificationBitMask_RespCodes;


/******************** CMD 838 *************************/

typedef struct
{
	_device_address_t UniqueID;
}C838_ReadUpdateNotificationBitMask_Req;

typedef struct
{
	_device_address_t UniqueID;
	uint16_t ChangeNotificationFlags;
}C838_ReadUpdateNotificationBitMask_Resp;

typedef enum
{
    C838_NOO = RCS_N00_Success,
    C838_E05 = RCS_E05_TooFewDataBytesReceived,
    C838_E65 = RCM_E65_UnknownUID
}C838_ReadUpdateNotificationBitMask_RespCodes;


/******************** CMD 839 *************************/

typedef struct
{
	_device_address_t DeviceAddress;
}C839_ChangeNotification_Req;

typedef struct
{
	_device_address_t DeviceAddress;
	uint8_t ChangeNotificationNo;
	uint16_t ChangeNotifications[MaxChangeNotifications];
}C839_ChangeNotification_Resp;

typedef enum
{
    C839_NOO = RCS_N00_Success,
    C839_E05 = RCS_E05_TooFewDataBytesReceived,
    C839_E65 = RCM_E65_UnknownUID
}C839_ChangeNotification_RespCodes;


/******************** CMD 840 *************************/

typedef struct
{
	_device_address_t UniqueID;
}C840_ReadDeviceStatistics_Req;

typedef struct
{
	_device_address_t UniqueID;
 uint16_t ActiveGraphsNo;
 uint16_t ActiveFramesNo;
 uint16_t ActiveLinksNo;
 uint8_t NeighboursNo;
 _time_t AverageLatency;
 uint16_t JoinCount;
 _date_t LastJoinTime;
 uint32_t LastJoinTimeToday;	// last join time in 1/32 ms from 12:00AM today
}C840_ReadDeviceStatistics_Resp;

typedef enum
{
    C840_NOO = RCS_N00_Success,
    C840_E02 = RCS_E02_InvalidSelection,
    C840_E05 = RCS_E05_TooFewDataBytesReceived
}C840_ReadDeviceStatistics_RespCodes;


/******************** CMD 841 *************************/

typedef struct
{
	uint16_t DeviceNickname;
}C841_ReadNetworkDeviceIdentityByNickname_Req;


typedef struct
{
	uint16_t Nickname;
	_device_address_t DeviceUniqueID;
	char LongTag[32];
}C841_ReadNetworkDeviceIdentityByNickname_Resp;

typedef enum
{
    C841_NOO = RCS_N00_Success,
    C841_E02 = RCS_E02_InvalidSelection,
    C841_E05 = RCS_E05_TooFewDataBytesReceived
}C841_ReadNetworkDeviceIdentityByNickname_RespCodes;


/******************** CMD 842 *************************/

typedef struct
{
	_device_address_t UniqueID;
	uint8_t SchedulingFlags;
}C842_WriteDeviceSchedulingFlags_Req;

typedef struct
{
	_device_address_t UniqueID;
	uint8_t SchedulingFlags;
}C842_WriteDeviceSchedulingFlags_Resp;

typedef enum
{
    C842_NOO = RCS_N00_Success,
    C842_E05 = RCS_E05_TooFewDataBytesReceived,
    C842_E07 = RCS_E07_InWriteProtectMode,
    C842_W08 = RCM_W08_UnsupportedPropertyFlag,
    C842_E09 = RCM_E09_InvalidPropertyFlag,
    C842_E65 = RCM_E65_UnknownUID
}C842_WriteDeviceSchedulingFlags_RespCodes;


/******************** CMD 843 *************************/

typedef struct
{
	_device_address_t UniqueID;
}C843_ReadDeviceSchedulingFlags_Req;

typedef struct
{
	_device_address_t UniqueID;
	uint8_t SchedulingFlags;
}C843_ReadDeviceSchedulingFlags_Resp;

typedef enum
{
    C843_NOO = RCS_N00_Success,
    C843_E02 = RCS_E02_InvalidSelection,
    C843_E05 = RCS_E05_TooFewDataBytesReceived
}C843_ReadDeviceSchedulingFlags_RespCodes;


/******************** CMD 844 *************************/

typedef EmptyCommand_Req
 C844_ReadNetworkConstraints_Req;

typedef struct
{
	uint8_t NetworkFlags;
	uint8_t ReqRespPairMessagesPerTenSeconds;
}C844_ReadNetworkConstraints_Resp;

typedef enum
{
    C844_NOO = RCS_N00_Success
}C844_ReadNetworkConstraints_RespCodes;


/******************** CMD 845 *************************/

typedef struct
{
	uint8_t NetworkFlags;
	uint8_t ReqRespPairMessagesPerTenSeconds;
}C845_WriteNetworkConstraints_Req;

typedef struct
{
	uint8_t NetworkFlags;
	uint8_t ReqRespPairMessagesPerTenSeconds;
}C845_WriteNetworkConstraints_Resp;

typedef enum
{
    C845_NOO = RCS_N00_Success,
    C845_E02 = RCS_E02_InvalidSelection,
    C845_E05 = RCS_E05_TooFewDataBytesReceived,
    C845_E07 = RCS_E07_InWriteProtectMode,
    C845_W08 = RCM_W08_SetToNearestPossibleValue,
    C845_E16 = RCS_E16_AccessRestricted
}C845_WriteNetworkConstraints_RespCodes;

#endif /*GATEWAYCOMMANDS_H*/
