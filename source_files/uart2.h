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
* Name:         uart2.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_UART2_H_
#define _NIVIS_UART2_H_

#include "typedef.h"
#include <string.h>
#include "MC1322x.h"
#include "CommonAPI/RadioApi.h"

#if ( UART2_MODE != NONE )

    #define CHECK_STATUS_METHOD_ID  0x04

    #define SPI_UPGRADE_BUF_LEN     70

      // BAUD RATES
      #define UART_BAUD_115200      767L
      #define UART_BAUD_57600       383L
      #define UART_BAUD_38400       256L
      #define UART_BAUD_19200       128L
      #define UART_BAUD_9600         64L
    

      #define UART2_RX_INT_EN()    { UART2_UCON &=~ 0x4000; }
      #define UART2_RX_INT_DI()    { UART2_UCON |= 0x4000; }
      
      #define UART2_TX_INT_EN()    { UART2_UCON &=~ 0x2000; }
      #define UART2_TX_INT_DI()    { UART2_UCON |= 0x2000; }
    
      #define UART2_RXTX_INT_EN()    { UART2_UCON &= ~0x6000; }
      #define UART2_RXTX_INT_DI()    { UART2_UCON |=  0x6000; }
    
      #define UART2_TX_EN()    { UART2_UCON |= 0x0001; }  // TX : 1 = enabled; 0 = disabled
      #define UART2_TX_DI()    { UART2_UCON &=~0x0001; }  //
      #define UART2_RX_EN()    { UART2_UCON |= 0x0002; }  // RX : 1 = enabled; 0 = disabled
      #define UART2_RX_DI()    { UART2_UCON &=~0x0002; }  //
    
      #define UART2_RXTX_DI()    { UART2_UCON &=~0x0003; }  //
      #define UART2_RXTX_EN()    { UART2_UCON |= 0x0003; }  //
      
    void UART2_Init( void );
 
    uint8 UART2_ReadBuff(unsigned char* p_pSPI1Msg,
                       unsigned char p_ucMsgLen);

    SC UART2_WriteBuff( const ApiMessageFrame * p_pMsgHeader,
                      const uint8*  p_pMsgPayload);

    Boolean UART2_IsBusy(void);
    uint8 UART2_UpdateSpeed(uint8 p_NewSpeed);

    extern uint16 g_unTxQueueSize;

    // 4 bytes API Msg header + 20 bytes API Msg params + APDU
    #define UART2_BUFFER_SIZE   MAX_API_BUFFER_SIZE 

    #define MSEC_1      (33)
    #define MSEC_5      (164)
    #define MSEC_25     (819)
    #define MSEC_100    (3276)
    
        void UART2_Timeouts (void);
        #define UART2_CommControl() UART2_Timeouts()
            

#elif defined ( UART2_DEBUG_MODE )        
    void UART2_Init(void);
    void UART2_Timeouts (void);
    
    #define UART2_IsBusy()                    FALSE
    #define UART2_CommControl()               UART2_Timeouts()
        
#else //UART2_MODE == NONE
    #define UART2_Init()
    #define UART2_IsBusy()                      FALSE
    #define UART2_UpdateSpeed(X)                TRUE
    #define UART2_CommControl() 

#endif // UART2_MODE


#endif /* _NIVIS_UART2_H_ */

