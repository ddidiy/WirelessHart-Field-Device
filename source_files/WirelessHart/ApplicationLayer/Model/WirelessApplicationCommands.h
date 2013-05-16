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

#ifndef WIRELESS_APPLICATION_COMMANDS_H_
#define WIRELESS_APPLICATION_COMMANDS_H_

#include "CommonTables.h"

// assuming the following: 
// Max. APDU len = 92 (in WirelessHART format)
// Max. Data len, for a non-aggregated command response: 92-4=88

#define C780_MAX_NEIGHBORS_LIST       ((88 - 3) / 10)


enum
{
  CMDID_C777_ReadWirelessDeviceInformation = 777,
  CMDID_C778_ReadBatteryLife               = 778,
  CMDID_C779_ReportDeviceHealth            = 779,
  CMDID_C780_ReportNeighborHealthList      = 780,
  CMDID_C793_WriteUTCTime                  = 793,
  CMDID_C794_ReadUTCTime                   = 794,
};

enum
{
  C777_ReqSize =  0,
  C778_ReqSize =  0,
  C779_ReqSize =  0,
  C780_ReqSize =  2,
  C793_ReqSize =  7,
  C794_ReqSize =  0
};

enum
{
  C777_RespSize = 22,
  C778_RespSize =  2,
  C779_RespSize =  9,
  C780_RespSize = 13, // or 23, 33, ... (variable size)
  C793_RespSize =  7,
  C794_RespSize =  7
};

#define C777_ReadWirelessDeviceInformation_ReqSize  C777_ReqSize
#define C778_ReadBatteryLife_ReqSize                C778_ReqSize
#define C779_ReportDeviceHealth_ReqSize             C779_ReqSize
#define C780_ReportNeighborHealthList_ReqSize       C780_ReqSize
#define C793_WriteUTCTime_ReqSize                   C793_ReqSize
#define C794_ReadUTCTime_ReqSize                    C794_ReqSize

#define C777_ReadWirelessDeviceInformation_RespSize C777_RespSize
#define C778_ReadBatteryLife_RespSize               C778_RespSize
#define C779_ReportDeviceHealth_RespSize            C779_RespSize
#define C780_ReportNeighborHealthList_RespSize      C780_RespSize
#define C793_WriteUTCTime_RespSize                  C793_RespSize
#define C794_ReadUTCTime_RespSize                   C794_RespSize


/******************** CMD 777 *************************/

typedef EmptyCommand_Req C777_ReadWirelessDeviceInformation_Req;

typedef struct
{
    float m_fPeakPacketsPerSecond;
    _time_t m_tDurationAtPeakPacketLoadBeforePowerDrained;
    _time_t m_tTimeToRecoverFromPowerDrain;
    _time_t m_tMinRequiredKeepAliveTime;
    uint16_t m_unMaxNoOfNeighbors;
    uint16_t m_unMaxNoOfPacketBuffers;
    uint8_t m_ucPowerSource;     // DevicePowerSource - CommonTable44; bits 7-4 must be set to 0
    int8_t m_cRSLThresholdGoodConnection;
}C777_ReadWirelessDeviceInformation_Resp;

typedef enum
{
    C777_N00 = RCS_N00_Success
}C777_ReadWirelessDeviceInformation_RespCodes;


/******************** CMD 778 *************************/

typedef EmptyCommand_Req C778_ReadBatteryLife_Req;

typedef struct
{
    uint16_t m_unBatteryLifeRemaining;
}C778_ReadBatteryLife_Resp;

typedef enum
{
    C778_N00 = RCS_N00_Success,
    C778_E06 = RCS_E06_DeviceSpecificCommandError,
    C778_E32 = RCS_E32_Busy
}C778_ReadBatteryLife_RespCodes;


/******************** CMD 779 *************************/

typedef EmptyCommand_Req C779_ReportDeviceHealth_Req;

typedef struct
{
    uint16_t m_unNoOfPacketsGeneratedByDevice;
    uint16_t m_unNoOfPacketsTerminatedByDevice;
    uint8_t m_ucNoOfDataLinkLayerMICFailuresDetected;
    uint8_t m_ucNoOfNetworkLayerMICFailuresDetected;
    uint8_t m_ucPowerStatus;    // DevicePowerStatus - CommonTable58
    uint8_t m_ucNoOfCRCErrorsDetected;
    uint8_t m_ucNoOfNonceCounterValuesNotReceived;

}C779_ReportDeviceHealth_Resp;

typedef enum
{
    C779_N00 = RCS_N00_Success
}C779_ReportDeviceHealth_RespCodes;


/******************** CMD 780 *************************/

typedef struct
{
    uint8_t m_ucNeighborTableIndex;
    uint8_t m_ucNoOfNeighborEntriesToRead;
}C780_ReportNeighborHealthList_Req;

typedef struct
{
    struct
    {
        uint16_t nicknameOfNeighbor;
        NeighbourFlagsMasks neighborFlags;
        int8_t meanRSLSinceLastReport;
        uint16_t noOfPacketsTransmittedToThisNeighbor;
        uint16_t noOfFailedTransmits;
        uint16_t noOfPacketsReceivedFromThisNeighbor;
    } m_aNeighbors[C780_MAX_NEIGHBORS_LIST];
    uint8_t m_ucNeighborTableIndex;
    uint8_t m_ucNoOfNeighborEntriesRead;
    uint8_t m_ucTotalNoOfNeighbors;
}C780_ReportNeighborHealthList_Resp;

typedef enum
{
    C780_N00 = RCS_N00_Success,
    C780_E02 = RCS_E02_InvalidSelection,
    C780_E05 = RCS_E05_TooFewDataBytesReceived,
    C780_W08 = RCM_W08_SetToNearestPossibleValue
}C780_ReportNeighborHealthList_RespCodes;


/******************** CMD 793 *************************/

typedef struct
{
    _date_t m_dDate;
    _time_t m_tTime;
}C793_WriteUTCTime_Req;

typedef struct
{
    _date_t m_dDate;
    _time_t m_tTime;
}C793_WriteUTCTime_Resp;

typedef enum
{
    C793_N00 = RCS_N00_Success,
    C793_E05 = RCS_E05_TooFewDataBytesReceived,
    C793_E16 = RCS_E16_AccessRestricted
}C793_WriteUTCTime_RespCodes;

/******************** CMD 794 *************************/

typedef EmptyCommand_Req C794_ReadUTCTime_Req;

typedef struct
{
    _date_t m_dDate;
    _time_t m_tTime;
}C794_ReadUTCTime_Resp;

typedef enum
{
    C794_N00 = RCS_N00_Success
}C794_ReadUTCTime_RespCodes;




#endif /* WIRELESS_APPLICATION_COMMANDS_H_ */
