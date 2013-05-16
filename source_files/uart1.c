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
* Name:         uart1.c
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#include <stdarg.h>
#include "uart1.h"
#include "typedef.h"
#include "global.h"
#include "digitals.h"
#include "itc.h"
#include "timers.h"
#include "asm.h"
#include "WirelessHART/uart_link.h"

  // BAUD RATES
  #define UART_BAUD_115200      767L
  #define UART_BAUD_57600       383L
  #define UART_BAUD_38400       256L
  #define UART_BAUD_19200       128L
  #define UART_BAUD_9600         64L

#if defined(BACKBONE_SUPPORT)

  #define STX 0xF0
  #define ETX 0xF1
  #define CHX 0xF2

  unsigned char    g_auc_Uart1_Tx[UART1_PACKET_LENGTH+2+2];
  unsigned char    g_uc_Uart1_TxIdx = 0;
  unsigned char    g_uc_Uart1_TxLen = 0;

  unsigned char    g_auc_Uart1_Rx[UART1_PACKET_LENGTH+2];
  unsigned char    g_uc_Uart1_RxIdx = 0;

  unsigned char    g_ucBufferReadyReq = 0;
  unsigned int     g_unLastSTXTime;
  uint16           g_uc_Uart1_CRC;

  void UART1_Interrupt(void);


  void UART1_Init(void)
  {

    IntAssignHandler(gUart1Int_c, (IntHandlerFunc_t)UART1_Interrupt);
    ITC_SetPriority(gUart1Int_c, gItcNormalPriority_c);
    ITC_EnableInterrupt(gUart1Int_c);

    UART1_UCON    = 0x0000;                                                       // disable UART 1 for setting baud rate
    GPIO_FUNC_SEL0_P14 = 1;                                                       // set pin P20 (gpio 14) for function select tx
    GPIO_FUNC_SEL0_P15 = 1;                                                       // set pin P19 (gpio 15) for function select rx
    UART1_UBRCNT  = ( (UART_BAUD_115200 ) << 16 ) + 9999;                           // set desired baud rate
    UART1_URXCON  = 1;                                                            // set buffer level
    UART1_UTXCON  = 31;                                                           // 31 = ok // set buffer level
    UART1_TX_INT_DI();
    UART1_RXTX_EN();
    g_ucBufferReadyReq = 1+UART_MSG_TYPE_BUFFER_READY;
  }


  void UART1_Interrupt(void)
  {
    static unsigned char s_ucRxLastChar = 0;
    volatile unsigned long ulDEBStatus = UART1_USTAT;  //Dummy read to clear flags

    // RX
    while ( UART1_URXCON )
    {
        volatile unsigned char ucRxChar = UART1_UDATA;
        if( ucRxChar == STX )
        {
            g_uc_Uart1_RxIdx = 0;
            g_uc_Uart1_CRC = 0xFFFF;
            g_unLastSTXTime = TMR3_CNTR;
        }
        else if( ucRxChar == ETX )
        {
             if( g_uc_Uart1_RxIdx == 1 && g_auc_Uart1_Rx[0] == UART_MSG_TYPE_BUFFER_CHECK )
             {
                   g_ucBufferReadyReq = 1 + UART_MSG_TYPE_BUFFER_READY;
             }
             else if( g_uc_Uart1_CRC || g_uc_Uart1_RxIdx >= sizeof( g_auc_Uart1_Rx ) )
             {
                   g_ucBufferReadyReq = 1 + UART_MSG_TYPE_BUFFER_WRONG;
             }
             else if( UART_LINK_DispatchMessage( g_uc_Uart1_RxIdx-2, g_auc_Uart1_Rx ) )
             {
                   g_ucBufferReadyReq = 1 + UART_MSG_TYPE_BUFFER_READY;
             }
             g_uc_Uart1_RxIdx = 0xFF;
        }
        else if( g_uc_Uart1_RxIdx < sizeof( g_auc_Uart1_Rx ) )
        {
            if( ucRxChar != CHX )
            {
                unsigned char ucBufCh = ( s_ucRxLastChar != CHX ? ucRxChar : ~ucRxChar );
                g_auc_Uart1_Rx[g_uc_Uart1_RxIdx++] = ucBufCh;
                g_uc_Uart1_CRC = COMPUTE_CRC( g_uc_Uart1_CRC, ucBufCh );
            }
        }
        s_ucRxLastChar = ucRxChar;
    }
    
    // TX
    while( UART1_UTXCON )
    {
        if ( g_uc_Uart1_TxIdx < g_uc_Uart1_TxLen )
        { // Tx buffer not empty, fill out TxFIFO
            unsigned char ucTxChar = g_auc_Uart1_Tx[g_uc_Uart1_TxIdx];
            if( g_uc_Uart1_TxIdx && ucTxChar >= STX && ucTxChar <= CHX  )
            { // escape sequence for STX, ETX, CHX
                g_auc_Uart1_Tx[g_uc_Uart1_TxIdx] = ~ucTxChar;
                ucTxChar = CHX;
            }
            else
            {
                g_uc_Uart1_TxIdx++;
            }

            UART1_UDATA = ucTxChar;
        }
        else if ( g_uc_Uart1_TxLen && g_uc_Uart1_TxIdx == g_uc_Uart1_TxLen )
        { // last message byte 
             g_uc_Uart1_TxIdx ++;
             UART1_UDATA = ETX;
        }
        else if( g_ucBufferReadyReq ) // add a buffer ready message
        { // overwriting the initial message
              g_auc_Uart1_Tx[0] = g_ucBufferReadyReq-1;
              g_uc_Uart1_TxIdx = 0;
              g_uc_Uart1_TxLen = 1;
              UART1_UDATA = STX;

              g_ucBufferReadyReq = 0;
        }
        else
        { // end packet transmission
            UART1_TX_INT_DI();
            g_uc_Uart1_TxIdx = 0;
            g_uc_Uart1_TxLen = 0;
            break;
        }
    }

  }

  ///////////////////////////////////////////////////////////////////////////////////
  // Name: UART1_SendMsg
  // Author: NIVIS LLC, Ion Ticus
  // Description: write message to UART Tx buffer if no other pending message
  // Parameters:  message, length
  // Return: 0 -other pending message, 1 -message sent to UART buffer
  // Obs:
  //      Access level: user level
  ///////////////////////////////////////////////////////////////////////////////////
  unsigned char UART1_SendMsg(const unsigned char * p_pucBuff, unsigned char p_ucLen)
  {
    unsigned char ucStatus = 0;
    uint16 unCRC;
    
    if( p_ucLen )
    {
        unCRC = ComputeCCITTCRC( p_pucBuff, p_ucLen );
    }

    UART1_RXTX_INT_DI();

    if ( !g_uc_Uart1_TxLen ) // not message on progress
    {
        if( g_ucBufferReadyReq ) // a buffer ready request
        {
            // simulate an end of one byte TX buffer
            g_uc_Uart1_TxLen = 1;
            g_uc_Uart1_TxIdx = 2;
        }
        else 
        if( p_ucLen )
        {
            
            ucStatus = 1;
            
            g_auc_Uart1_Tx[0] = STX;
            memcpy( g_auc_Uart1_Tx+1, p_pucBuff, p_ucLen );
            g_auc_Uart1_Tx[ ++p_ucLen ] = (unCRC >> 8); // CRC -> HI byte
            g_auc_Uart1_Tx[ ++p_ucLen ] = unCRC & 0xFF; // CRC -> LO byte

            g_uc_Uart1_TxLen = p_ucLen+1; // STX 
        }
    }

    UART1_RXTX_INT_EN();
    return ucStatus;
  }

#else // BACKBONE_SUPPORT
  
#ifdef UART1_DEBUG
  
  void UART1_Init(void)
  {
    //itcAssignHandler(UART1_Interrupt,UART1_INT,NORMAL_INT);
    //ITC_ENABLE_INTERRUPT(UART1_INT);

    UART1_UCON    = 0x0000;                                                       // disable UART 1 for setting baud rate
    GPIO_FUNC_SEL0_P14 = 1;                                                       // set pin P20 (gpio 14) for function select tx
    GPIO_FUNC_SEL0_P15 = 1;                                                       // set pin P19 (gpio 15) for function select rx
    UART1_UBRCNT  = ( (UART_BAUD_115200 ) << 16 ) + 9999;                           // set desired baud rate
    UART1_URXCON  = 1;                                                            // set buffer level
    UART1_UTXCON  = 31;                                                           // 31 = ok // set buffer level
    //UART1_TX_INT_DI();
    UART1_RXTX_EN();
    
    UART1_SendChar(0x0D);
    UART1_SendChar(0x0A); // CR, LF after reset
  }

  void UART1_SendChar(unsigned char p_ucData)
  {
    while( ! UART1_UTXCON )
      ;        
    UART1_UDATA = p_ucData;
  }
  
  void UART1_String(const unsigned char *p_pszData)
  { // for null-terminated strings only!
    unsigned char c, i = 0;
    while( ! UART1_UTXCON )
      ;
    UART1_UDATA = 0x0D;
    while( ! UART1_UTXCON ) //[doinel.alban] this isn't necessary  
      ;                     // if HyperTerminal is configured        
    UART1_UDATA = 0x0A;     // to append line feeds to incoming line ends; would be more efficient      
    while(c = p_pszData[i++])
    {
      while( ! UART1_UTXCON )
        ;        
      UART1_UDATA = c;
    }  
  }
  
 

#endif  // UART1_DEBUG

  
#endif // BACKBONE_SUPPORT

  void Log(unsigned char* fmt, ...)
  {
    #ifdef UART1_DEBUG
    
    va_list ap;
    unsigned char szTempBuffer[80], i = 0;
    
    // Note: vsprintf returns the number of characters generated (not including the null character), but user must assure that we dont go over the size of szTempBuffer.
    memset(szTempBuffer, 0x00, sizeof(szTempBuffer));
    va_start(ap, fmt);
    (void)vsprintf( (char*)szTempBuffer, (char const*)fmt, ap );  // vsprintf adds automatically the null char to the string
    va_end(ap);

    while( szTempBuffer[i] )
    {
      while( ! UART1_UTXCON )
        ;        
      UART1_UDATA = szTempBuffer[i++];
    }
    
    #endif
    
  }