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
// Name:         uart_link.h
// Author:       Nivis LLC, Ion Ticus
// Date:         January 2008
// Description:  Implements uart link layer
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NIVIS_UART_LINK_H_
#define _NIVIS_UART_LINK_H_

    #define UART_MSG_TYPE_BUFFER_READY    0x00
    #define UART_MSG_TYPE_BUFFER_FULL     0x01
    #define UART_MSG_TYPE_BUFFER_CHECK    0x02
    #define UART_MSG_TYPE_BUFFER_WRONG    0x03
    #define UART_MSG_TYPE_ACK             0x10
    #define UART_MSG_TYPE_ACK_RST         0x20   // this ACK means AP was restarted
    #define UART_MSG_TYPE_RSP_NET_INFO    0x30
    #define UART_MSG_TYPE_NWK             0x80
//    #define UART_MSG_TYPE_APP           0x81
    #define UART_MSG_TYPE_NWK_CONF        0x82
    #define UART_MSG_TYPE_REQ_NET_INFO    0x83
    #define UART_MSG_TYPE_REQ_DISCONNECT  0x84

#ifdef BACKBONE_SUPPORT


    ///////////////////////////////////////////////////////////////////////////////////
    // Name: UART_LINK_Task
    // Author: NIVIS LLC, Ion Ticus
    // Description: read messages from UART and pending messages to UART
    // Parameters: 
    // Return: None
    // Obs:
    //      Access level: user level
    ///////////////////////////////////////////////////////////////////////////////////
    void UART_LINK_Task ( void );

    ///////////////////////////////////////////////////////////////////////////////////
    // Name: UART_LINK_AddMessage
    // Author: NIVIS LLC, Ion Ticus
    // Description: Add a message to be send to UART
    // Parameters: 
    //           p_unMessageType - message type: UART_MSG_TYPE_NWK, UART_MSG_TYPE_APP, UART_MSG_TYPE_NWK_CONF
    //           p_ucPayloadLen - message payload length
    //           p_pPayload - message payload 
    // Return: 
    //        SUCCESS,  OUT_OF_MEMORY, or INVALID_PARAMETER
    // Obs:
    //      Access level: user level and interrupt level
    ///////////////////////////////////////////////////////////////////////////////////
    uint8 UART_LINK_AddMessage ( uint8 p_unMessageType, uint8 p_ucPayloadLen, const void * p_pPayload );


    ///////////////////////////////////////////////////////////////////////////////////
    // Name: UART_LINK_DispatchMessage
    // Author: NIVIS LLC, Ion Ticus
    // Description: dispatch an complete message received on UART link
    // Parameters: 
    //           p_ucPayloadLen - message length
    //           p_pPayload - message payload
    // Return: 1 if success, 0 if not buffer
    // Obs:
    //      Access level: interrupt level
    ///////////////////////////////////////////////////////////////////////////////////
    uint8 UART_LINK_DispatchMessage ( uint8 p_ucPayloadLen, uint8 * p_pPayload );

#else // BACKBONE_SUPPORT
    
  #define UART_LINK_Task()
  #define UART_LINK_AddMessage(...) 0
  #define UART_LINK_DispatchMessage(...) 0

#endif // BACKBONE_SUPPORT   
#endif // _NIVIS_UART_LINK_H_

