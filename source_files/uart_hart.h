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
* Name:         uart_hart.h
* Author:       Dorin Pavel
* Date:         October 2010
* Description:  HART modem handling
*               This file holds the UART definitions for the HART modem
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_UART_HART_H_
#define _NIVIS_UART_HART_H_

#include ".\WiredHart\options.h"


    #if (HART_MODE == NONE)

        #define HART_Init()
        #define HART_CarrierDetectWatch()

    #else //(HART_MODE == ACTIVED)
        
        // BAUD RATE, fix, 1200Bd
        #define UART_BAUD_1200        7L
      
      #if ( (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE ))
        
        #define HART_UCON             UART1_UCON 
        #define HART_UTXCON           UART1_UTXCON
        #define HART_URXCON           UART1_URXCON
        #define HART_UBRCNT           UART1_UBRCNT
        #define HART_USTAT            UART1_USTAT
        #define HART_UDATA            UART1_UDATA
      
        #define gHartInt_c            gUart1Int_c
 
      #endif

        void HART_Init (void);
        void HART_CarrierDetectWatch (void);
        void BeginTx (const uint8* p_pucTxBuffer);   
        
        #define UART_TX_ENABLED     0x0001
        #define UART_RX_ENABLED     0x0002
        
        //extern volatile uint8 g_ucHartBusy;
        
        #define  HART_IsBusy() /*g_ucHartBusy*/ (HART_UCON & (UART_TX_ENABLED | UART_RX_ENABLED) ) // if RX or TX interrupt on progress
     
    #endif

  
#endif /* _NIVIS_UART_HART_H_ */

