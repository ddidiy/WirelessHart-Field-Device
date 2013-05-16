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
 * Name:         RadioApi.h
 * Author:       NIVIS LLC, John Chapman and Kedar Kapoor
 * Date:         July 2008
 * Description:  This file contains definitions important to the API.
 * Changes:
 * Revisions:
 ****************************************************************************************************/

#ifndef _NIVIS_RADIOAPI_H_
#define _NIVIS_RADIOAPI_H_

/*===========================[ public includes ]=============================*/
#include "../typedef.h"
/*==========================[ public datatypes ]=============================*/

#define API_VERSION 0x0001      // Version 0.1

#define API_MSG_CRC_SIZE                   2
#define MAX_API_BUFFER_SIZE  (sizeof(ApiMessageFrame) + MAX_APDU_SIZE)

/*  Defines either single/multiple processor based system.
 *   If the Radio Modem Stack and the Application code reside on the same processor, then single setup
 *   else, use the multi processor setup.
 */
enum PLATFORM_HOSTS {
    HOST_SINGLE_PROCESSOR,
    HOST_MULTI_PROCESSOR,

    NUM_TYPES_PLATFORM_HOSTS
};

#define API_PLATFORM_HOSTS HOST_MULTI_PROCESSOR

/*  Defines the user processor used in this platform.
 *  In the multi-processor setup, each side specifies the processor type used.  */
enum PLATFORM_CPU {
    PLATFORM_MSP430,
    PLATFORM_HCS08,
    PLATFORM_ARM7,
    PLATFORM_ARM9,

    NUM_ITEMS_PLATFORM_CPU
};

#define API_PLATFORM PLATFORM_ARM7

enum SPI_SPEED {
    SPI_SPEED_25k = 1,
    SPI_SPEED_50k,
    SPI_SPEED_75k,
    SPI_SPEED_100k,
    SPI_SPEED_200k,
    SPI_SPEED_250k,
    SPI_SPEED_500k,
    SPI_SPEED_1000k,
    SPI_SPEED_2000k,
    NUM_SUPPORTED_SPI_SPEEDS
};

#define MIN_SPI_SPEED       SPI_SPEED_100k
#define MAX_SPI_SPEED       SPI_SPEED_500k
#define DEFAULT_SPI_SPEED   SPI_SPEED_100k

enum UART_SPEED {
    UART_SPEED_9_6K = 1,
    UART_SPEED_19k,
    UART_SPEED_38k,
    UART_SPEED_115k,
    NUM_SUPPORTED_UART_SPEEDS
};

#define MAX_UART_SPEED  UART_SPEED_115k
#define DEFAULT_UART_SPEED UART_SPEED_9_6K,

enum HEARTBEAT_FREQ {
    HEARTBEAT_20ms = 1,
    HEARTBEAT_50ms,
    HEARTBEAT_100ms,
    HEARTBEAT_500ms,
    HEARTBEAT_1000ms,
    HEARTBEAT_60000ms,
    NUM_SUPPORTED_HEARTBEAT_FREQ
};

#define MIN_HEARTBEAT_FREQ       HEARTBEAT_60000ms
#define MAX_HEARTBEAT_FREQ       HEARTBEAT_20ms
#define DEFAULT_HEARTBEAT_FREQ   HEARTBEAT_500ms

enum ACK_MESSAGES {
    DATA_OK = 0,
    MESSAGE_SENT_VIA_RF,
    MESSAGE_SENT_VIA_MODEM,
    API_CHANGE_ACCEPTED,

    NUM_ACK_MESSAGES
};

enum API_STATE {
    STATE_NOT_INITIALIZED,
    STATE_INIT,
    STATE_ACTIVE,
    STATE_BUSY,
    STATE_FAIL,

    NUM_API_STATES
};

enum NACK_MESSAGES {
    NACK_CRC_FAIL = 1,
    NACK_DATA_OVERRUN,
    NACK_PACKET_INCOMPLETE,
    NACK_PARITY_ERROR,
    NACK_API_NOT_INITIALIZED,
    NACK_API_COMMAND_ERROR,
    NACK_API_BUSY,
    NACK_API_ERROR,
    NACK_STACK_ERROR, // Recommend getting Stack status from Stack Specific commands.
    NACK_UNSUPPORTED_FEATURE, // Feature not yet supported in this release

    NUM_NACK_MESSAGES
};


enum PASSTHROUGH_MESSAGES
{
    WRITE_DATA_REQ = 1,
    READ_DATA_REQ = 2,
    READ_DATA_RESP = 3,
    PASSTHROUGH_MESSAGES_NO
};

enum MSG_CLASS {
    DATA_PASSTHROUGH = 1, // Applies to stack specific objects (i.e. ISA100.11a UploadDownloadObject)
    POWER_MANAGEMENT, // Messages directing radio on power management (i.e. Sleep Unit)
    STACK_SPECIFIC, // Messages to gain access to stack specific data (i.e. ISA100.11a “Join status”)
    API_COMMANDS, // Messages to control/query API related data (i.e. FW/HW Version, platform)
    API_ACK,
    API_NACK,
    USER_BOARD_COMMANDS, // Messages(commands) forwarded by the radio to the user board
    NUM_ITEMS_MSG_CLASS
};

typedef struct {
    struct {
        uint8 Reserved : 3;        
        uint8 m_bIsRsp : 1;
        enum MSG_CLASS MessageClass : 4;
    } MessageHeader;
    uint8 MessageType;
    uint8 MessageID;
    uint8 MessageSize;
} ApiMessageFrame;

/*==========================[ public functions ]==============================*/

#endif // _NIVIS_RADIOAPI_H_
