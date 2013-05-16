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
 * Name:         Common_API.h
 * Author:       NIVIS LLC, John Chapman and Kedar Kapoor
 * Date:         July 2008
 * Description:  This file has the SendMessage routine for the RF Modem side of the common API.
 * 				 This also is the API for the single processor solution.
 * Changes:
 * Revisions:
 ****************************************************************************************************/

#ifndef _NIVIS_COMMON_API_H_
#define _NIVIS_COMMON_API_H_

/*===========================[ public includes ]=============================*/
#include "../typedef.h"
#include "RadioApi.h"
#include "../system.h"

#if ( _UAP_TYPE != 0 )
/*==========================[ public datatypes ]=============================*/
enum SEND_ERROR_CODE {
    NO_ERR = 0,
    DATA_FOR_PROCESSING,

    ER_QUEUE_FULL,
    ER_LINK_UNAVAILABLE, // Recommend querying STACK_SPECIFIC attributes for link outage cause
    ER_UNSUPPORTED_DATA_SIZE, // i.e. max string size is 150 and 160 byte string or an empty string is sent
    ER_UNKNOWN_DATA_TYPE, // Datagram type is not supported
    ER_PARAM, // There was an error with one of the parameters
    ER_API_FAILED,
    ER_API_NOT_INITIALIZED,

    NUM_ITEMS_SEND_ERROR_CODE
};

#define API_REQ FALSE
#define API_RSP TRUE

#define UAP_APP1_ID   2


enum API_COMMAND_TYPES { 
    
    API_HW_PLATFORM,
    API_FW_VERSION,
    API_MAX_BUFFER_SIZE, // This is platform specific and may vary depending on platform resources.
    API_MAX_SPI_SPEED,
    API_UPDATE_SPI_SPEED,
    API_MAX_UART_SPEED,
    API_UPDATE_UART_SPEED,
    API_HEARTBEAT_FREQ, // 500 ms default value
    API_HEARTBEAT,
    API_ENERGY_LEFT, // tbd
    API_ENERGY_MODEM_REQ, // tbd

    NUM_ITEMS_API_COMMAND_TYPES
};


enum STACK_SPECIFIC_TYPES {   
      
    HART_JOIN_STATUS = 0,  // tbd
    HART_GET_ASN,          // tbd     
    HART_POWER_LEVEL,      // tbd

    // WHART Service
    HART_SERVICE_REQUEST,
    HART_SERVICE_DELETE,

    // WHART Notifications
    HART_NOTIFY_JOIN,      // tbd
    HART_GET_INITIAL_INFO, 
    HART_RADIO_RESET,
    NUM_ITEMS_STACK_SPECIFIC_TYPES
};


enum USER_BOARD_TYPES {    
  
    DAQ_TUNNEL_WIRELESS = 0,  
  
    DAQ_BURST_PIPE_0,
    DAQ_BURST_PIPE_1,
    DAQ_BURST_PIPE_2,
    DAQ_TUNNEL_WIRED,

    NUM_ITEMS_USER_BOARD_TYPES
};

typedef enum {
    API_STATUS_INIT,
    API_STATUS_RUN
} API_STATUS_TYPE; 


/*==========================[ public functions ]==============================*/

////////////////////////////////////////////////////////////////////////////////
// Name: SendMessage
// Author: NIVIS LLC, John Chapman and Kedar Kapoor
// Description: method used to send message via common stack API.  This method will add the message
//              to a queue for transmission and assign a MSG_ID.  This will copy the data into
//              a stack buffer before sending it to the next method for processing.
// Parameters:
//        MSG_CLASS             - the type of message.  This directs SendMessage on how to handle the message buffer.
//        MSG_TYPE              - sub-type of the MSG_CLASS.  This may not be used for all MSG_CLASSes.
//        uint8                 - RequestResponse 0 = FALSE, 1 = TRUE
//        MSG_ID          *     - Empty if sending unsolicited message, MSG_ID if responding to a message
//                                  If sending message, the next indication a message is available to process will
//                                  contain the ID for this message.  This is for future use and not presently processed.
//        uint8 bufSize         - the size of the buffer
//        uint8 *buf            - the message to be sent
//
// Return: SEND_ERR_NO   - Error when handling the message.  Will return NO_ERR if message successfully handled.
////////////////////////////////////////////////////////////////////////////////
enum SEND_ERROR_CODE SendMessage(enum MSG_CLASS p_MsgClass,
                                 uint8 p_MsgType,
                                 uint8 p_ucAckRequest,
                                 uint8 p_ucMSG_ID,
                                 uint8 p_BuffSize,
                           const uint8 *p_pMsgBuff);

void API_OnRcvMsg( ApiMessageFrame * p_pMessageFrame );

#endif // _UAP_TYPE

#if         (DEVICE_TYPE == DEV_TYPE_MC13225)
    #if ( SPI1_MODE != NONE )
          #include "../spi.h"
          #define DAQ_BUFFER_SIZE   SPI_BUFFER_SIZE
          #define API_PHY_Write(p_pMsgHeader,p_pMsgPayload) SPI_WriteBuff(p_pMsgHeader,p_pMsgPayload)
          #define API_PHY_Read(p_pBuff,p_unBuffSize) SPI_ReadBuff(p_pBuff,p_unBuffSize)
          #define API_PHY_SetSpeed(p_Speed)          SPI_UpdateSpiSpeed(p_Speed)
    #elif ( UART2_MODE != NONE )
          #include "../uart2.h"
          #define DAQ_BUFFER_SIZE   UART2_BUFFER_SIZE
          #define API_PHY_Write(p_pMsgHeader,p_pMsgPayload) UART2_WriteBuff(p_pMsgHeader,p_pMsgPayload)
          #define API_PHY_Read(p_pBuff,p_unBuffSize) UART2_ReadBuff(p_pBuff,p_unBuffSize)
          #define API_PHY_SetSpeed(p_Speed)          UART2_UpdateSpeed(p_Speed)
    #else
        //let them rise errs
    #endif

#elif       (DEVICE_TYPE == DEV_TYPE_MSP430F2618) // raptor modem
      #include "../spi1.h"

      #define API_PHY_Write(p_pBuff,p_unBuffLen) SPI1_WriteBuff(p_pBuff,p_unBuffLen)
      #define API_PHY_Read(p_pBuff,p_unBuffSize) SPI1_ReadBuff(p_pBuff,p_unBuffSize)
      #define API_PHY_SetSpeed(p_Speed)          SPI1_UpdateSpiSpeed(p_Speed)

#elif       (DEVICE_TYPE == DEV_TYPE_AP91SAM7X512)

      #define API_PHY_Write(p_pBuff,p_unBuffLen)
      #define API_PHY_Read(p_pBuff,p_unBuffSize) 0
      #define API_PHY_SetSpeed(p_Speed)          SFC_FAILURE
#else
      #error  "Unknown device, please select apropiate API PHY interface"
#endif

uint8 API_onRcvGetInitialInfo(ApiMessageFrame * p_pMsgFrame);
uint8 API_onRcvSrvcReq(ApiMessageFrame * p_pMsgFrame);
uint8 API_onRcvSrvcDel(ApiMessageFrame * p_pMsgFrame);
#endif // _NIVIS_COMMON_API_H_
