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
* Name:         uart_hart.c
* Author:       Dorin Pavel
* Date:         October 2010
* Description:  HART modem handling
*               This file holds the UART definitions for the HART modem
* Changes:
* Revisions:
****************************************************************************************************/
#include ".\WiredHart\options.h"

#if ( HART_MODE != NONE )

#include "uart_hart.h"
#include "typedef.h"
#include "global.h"
#include "itc.h"
#include "WirelessHART/tmr_util.h"
#include "WiredHart/DataLinkLayer.h"




#define HART_RX_INT_EN()    { HART_UCON &=~0x4000; }
#define HART_RX_INT_DI()    { HART_UCON |= 0x4000; }

#define HART_TX_INT_EN()    { HART_UCON &=~0x2000; }
#define HART_TX_INT_DI()    { HART_UCON |= 0x2000; }

#define HART_TX_EN()        { HART_UCON |= UART_TX_ENABLED;}  // TX : 1 = enabled; 0 = disabled
#define HART_TX_DI()        { HART_UCON &=~UART_TX_ENABLED;}  //
#define HART_RX_EN()        { HART_UCON |= UART_RX_ENABLED;}  // RX : 1 = enabled; 0 = disabled
#define HART_RX_DI()        { HART_UCON &=~UART_RX_ENABLED;}  // 

#define AFTER_ADDR_FIELD_END   ( (g_aucRxData[0] & 0x80) ? 6 : 2 )
#define AFTER_EXPB_FIELD_END   ( AFTER_ADDR_FIELD_END + ((g_aucRxData[0] & 0x60) >>5) )
#define AFTER_DATA_FIELD_END   ( (g_aucRxData[0] & 0x80) ? \
                    (8 + 1 + ((g_aucRxData[0] & 0x60) >>5) + g_aucRxData[7 + ((g_aucRxData[0] & 0x60) >>5)]) : \
                    (4 + 1 + ((g_aucRxData[0] & 0x60) >>5) + g_aucRxData[3 + ((g_aucRxData[0] & 0x60) >>5)]) )  

    typedef enum 
    {
        HART_TXRX_SNOOZE = 0, 
        HART_RX_WAIT = 0, 
        HART_RX_PREAMBLE, 
        HART_RX_DELIMITER, 
        HART_RX_ADDRESS, 
        HART_RX_SHORT_ADDRESS, 
        HART_RX_EXPANSION,
        HART_RX_COMMAND, 
        HART_RX_COUNT, 
        HART_RX_DATA, 
        HART_RX_CHECK,
        HART_RX_END,
        HART_TX_PREAMBLE, 
        HART_TX_BUFFER,
        HART_TX_TAIL,
        HART_TX_END
    } T_HART_TXRXSTATE;

#define RX_MASK 1 

T_HART_TXRXSTATE g_TxRxState;
unsigned int g_uiLastBytePosition;
uint8   g_ucTxDataLen;
uint8   g_ucTxDataIndex;
const uint8 * g_pucTxBuffer = NULL; 
uint8   g_ucCheckByte;
uint32  g_uiTimeOfLastByte;
uint8   g_ucLocalMessageStatus;

//volatile uint8 g_ucHartBusy;

#define SetCarrierErrorInStatus()   {g_ucLocalMessageStatus |= 0x80;}
#define SetReceiveErrorInStatus()   {g_ucLocalMessageStatus |= 0x20;}
#define SetCheckByteErrorInStatus() {g_ucLocalMessageStatus |= 0x10;}
#define SetGapErrorInStatus()       {g_ucLocalMessageStatus |= 0x80;}
#define IndicateCarrierInStatus()   {g_sDllGlobals.m_ModemEvent |= 0x04;}
#define IndicateNoCarrierInStatus() {g_sDllGlobals.m_ModemEvent |= 0x01;}
#define IndicateEndTxInStatus()     {g_sDllGlobals.m_ModemEvent |= 0x02;}


///////////////////////////////////////////////////////////////////////////////////
// Name: UART2_Interrupt
// Author: NIVIS LLC, Marius Vilvoi, Avram Ionut, Ion Ticus, Dorin Pavel
// Description: int handler for reception and transmission
// Parameters:  none
// Return: void
// Obs:
//      the physical medium is half duplex => tx and rx are never simultaneous;
//      rx has priority => once in rx, it will remain there so the tx will be 
//      delayed until getting the token
//      for the gap error time is used the timer from the token since 
//      it has no use during rx
///////////////////////////////////////////////////////////////////////////////////
#define GAP_TICKS   USEC_TO_RTC(2*9200+1000) // = maximum gap is 1 char delay -> 2 chars + 1 ms because our internall possible latency
void HART_Interrupt(void)
{
    unsigned long ulStatus = HART_USTAT; 
    
    if ( (g_TxRxState >= HART_TX_PREAMBLE) && (g_TxRxState < HART_TX_END) ) //tx is on progress
    {
        while( HART_UTXCON )
        {
            switch (g_TxRxState) 
            {
            case HART_TX_PREAMBLE:
                DEBUG5_ON();
                HART_UDATA = 0xFF;
                if(--g_ucTxDataIndex==0) 
                {
                    g_TxRxState = HART_TX_BUFFER;
                    if (g_pucTxBuffer[0] & 0x80) 
                    {
                        g_ucTxDataLen = 8 + g_pucTxBuffer[7];
                    }
                    else               
                    {
                        g_ucTxDataLen = 4 + g_pucTxBuffer[3];
                    }
                    g_ucCheckByte = 0;
                }
                break;
            
            case HART_TX_BUFFER:
                if(g_ucTxDataIndex<g_ucTxDataLen) 
                {
                    g_ucCheckByte ^= g_pucTxBuffer[g_ucTxDataIndex];
                    HART_UDATA = g_pucTxBuffer[g_ucTxDataIndex++];
                }
                else 
                {
                    HART_UDATA = g_ucCheckByte;
                    g_TxRxState = HART_TX_TAIL;
                    g_ucCheckByte = 2; // a delay until all packet bytes are sent  
                }
                break;
            
            case HART_TX_TAIL:
                if( (--g_ucCheckByte) == 0 )
                {
                    g_TxRxState = HART_TX_END;
                    HART_TX_INT_DI(); 
                    
                    return;
                }

                HART_UDATA = 0xFF; 
                break;
            }
        }        
    }
        
    
    while ( HART_URXCON )
    {
        unsigned char ucRxChar = HART_UDATA;
    
        if( ((HART_UCON & 1) == 0) && !g_sDllGlobals.m_RxMessageReady ) // TX is disabled // half duplex and not pending RX message
        {
            if ((RTC_COUNT-g_uiTimeOfLastByte) > GAP_TICKS) // gap error -> fatal error -> reset the status
            {
                g_TxRxState = HART_RX_WAIT;
            }
            g_uiTimeOfLastByte = RTC_COUNT;
            
            switch( g_TxRxState )
            {  
            case HART_RX_WAIT:
                if (ucRxChar==0xFF)  
                {                
                    g_TxRxState = HART_RX_PREAMBLE;  
                } 
                break;
                
            case HART_RX_PREAMBLE: 
                if ( ucRxChar==0xFF ) 
                {
                    g_TxRxState = HART_RX_DELIMITER;
                }
                else 
                {
                    g_TxRxState = HART_RX_WAIT;
                }
                break;
        
            case HART_RX_DELIMITER:    
                if ( ucRxChar!=0xFF )
                {
                    if (((ucRxChar & HART_FRAME_MASK) != HART_BACK) && 
                        ((ucRxChar & HART_FRAME_MASK) != HART_STX) && 
                        ((ucRxChar & HART_FRAME_MASK) != HART_ACK))
                    {
                        g_TxRxState = HART_RX_WAIT; 
                    }
                    else 
                    {
                        g_aucRxData[0] = ucRxChar;
                        g_ucCheckByte = ucRxChar;
                        g_ucRxDataIndex = 1;
                        if (ucRxChar & 0x80) 
                        {
                            g_TxRxState = HART_RX_ADDRESS;
                            g_uiLastBytePosition = AFTER_ADDR_FIELD_END;
                        }
                        else 
                        {
                            g_TxRxState = HART_RX_SHORT_ADDRESS;
                        }
                    }
                }    
                break;
                
            case HART_RX_ADDRESS:
                g_aucRxData[g_ucRxDataIndex++] = ucRxChar;
                g_ucCheckByte ^= ucRxChar;                    
                if (g_ucRxDataIndex==g_uiLastBytePosition) 
                {
                    if ((g_aucRxData[0] & HART_EXTBYTES_MASK) != 0) 
                    {
                        g_TxRxState = HART_RX_EXPANSION;
                        g_uiLastBytePosition = AFTER_EXPB_FIELD_END;
                    }
                    else 
                    {
                        g_TxRxState = HART_RX_COMMAND;     
                    }
                }              
                break;
            
            case HART_RX_SHORT_ADDRESS:
                g_aucRxData[g_ucRxDataIndex++] = ucRxChar;
                g_ucCheckByte ^= ucRxChar;
                    if ((g_aucRxData[0] & HART_EXTBYTES_MASK) != 0) 
                    {
                        g_TxRxState = HART_RX_EXPANSION;
                        g_uiLastBytePosition = AFTER_EXPB_FIELD_END;
                    }
                    else 
                    {
                        g_TxRxState = HART_RX_COMMAND;     
                    }
                break;
                
            case HART_RX_EXPANSION:
                g_aucRxData[g_ucRxDataIndex++] = ucRxChar;
                g_ucCheckByte ^= ucRxChar;
                if (g_ucRxDataIndex==g_uiLastBytePosition) 
                {
                    g_TxRxState = HART_RX_COMMAND;
                }
                break;
                            
            case HART_RX_COUNT:
                g_aucRxData[g_ucRxDataIndex++] = ucRxChar;
                g_ucCheckByte ^= ucRxChar;
                g_TxRxState = HART_RX_DATA;
                g_uiLastBytePosition = AFTER_DATA_FIELD_END;
                break;
                
            case HART_RX_COMMAND:
                g_ucLocalMessageStatus = 0;
                g_TxRxState = HART_RX_COUNT;              
                // not break 
                
            case HART_RX_DATA:
                g_aucRxData[g_ucRxDataIndex++] = ucRxChar;
                g_ucCheckByte ^= ucRxChar;

                // error during of command or data
                if ( ulStatus & 0x07 ) // Start Bit Error | Parity Error | Frame/Stop Bit Error
                {        
                    SetReceiveErrorInStatus();
                }
                                
                if ((g_ucRxDataIndex==g_uiLastBytePosition) && (g_TxRxState == HART_RX_DATA)) 
                {
                    if (g_ucCheckByte) {
                        SetCheckByteErrorInStatus();
                    }
                    g_sDllGlobals.m_ModemEvent |= g_ucLocalMessageStatus;
                    g_sDllGlobals.m_RxMessageReady = 1;
                    g_TxRxState = HART_RX_WAIT; //just avoid altering the buffer                         
                }                              
                continue; // not break; in order to jump at beginig of while()
            }        
            
            // if not HART_RX_COMMAND or HART_RX_DATA, any error is fatal error -> reset the status
            if ( ulStatus & 0x07 ) // Start Bit Error | Parity Error | Frame/Stop Bit Error
            {        
                g_TxRxState = HART_RX_WAIT;
            }
       }
    }
}    

///////////////////////////////////////////////////////////////////////////////////
// Name: UART2_Init
// Author: NIVIS LLC, Marius Vilvoi, Avram Ionut, Ion Ticus, Dorin Pavel
// Description: init the UART2 to 1200-8NOdd1
// Parameters:  none
// Return: void
// Obs:
//      Tx interrupt is disabled until Msg ready to send
//      There is a false int generated by the peripheral, do not remove UART2_RX_EN() - UART2_RX_DI() statements
//      The false int remains locked if triggered so it is a must to avoid it
///////////////////////////////////////////////////////////////////////////////////
void HART_Init (void)
{ 
    HART_UCON    = 0x6000;                                   // disable HART-UART for setting baud rate

#if ( (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE) )
   
    GPIO_FUNC_SEL0_P14 = 1;                                   // UART1: set pin 20 (gpio 14) for function select tx
    GPIO_FUNC_SEL0_P15 = 1;                                   // UART1: set pin 19 (gpio 15) for function select rx
    
#endif
    

#if ( (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE) )
        
        //use UART1-RTS=gpio17 as Tx enable for modem
        //use ADC3=gpio33 as CD input from the modem
        GPIO_FUNC_SEL1_P17 = 0;
        GPIO_DATA_SET0_P17 = 1; //high is receiving
        GPIO_PAD_DIR_SET0_P17 = 1;  //output 
        
        GPIO_FUNC_SEL2_P33 = 0;
        GPIO_PAD_DIR_RESET1_P33 = 1;    //input     
        GPIO_PAD_PU_SEL1_P33 = 0;       //weak down
        GPIO_PAD_PU_EN1_P33 = 1;

        //temporar aici pornirea modemului (HARTM_ON)
        //use TMR1=gpio9 
        MODEM_POWER_ON(); // start the modem
        
        /*
        GPIO_FUNC_SEL0_P09 = 0;
        GPIO_DATA_RESET0_P09 = 1;   //0 in on, 1 is off    
        GPIO_PAD_DIR_SET0_P09 = 1;  //output
        */

#endif        
            
        
    HART_UCON    = 0x0004 + 0x08;                            // parity enable, odd 
    HART_UBRCNT  = ( (UART_BAUD_1200 ) << 16 ) + 9999;       // set desired baud rate
    HART_URXCON  = 1;                                        // set rx buffer level int
    HART_UTXCON  = 16; // 31                                 // 31 = ok // set buffer level     
 
    HART_TX_INT_DI();
    HART_RX_INT_DI();
    HART_RX_EN();
    //g_ucHartBusy = 0;

    IntAssignHandler(gHartInt_c, (IntHandlerFunc_t)HART_Interrupt);
    ITC_SetPriority(gHartInt_c, gItcNormalPriority_c);
    ITC_EnableInterrupt(gHartInt_c);       
    
    HART_RX_DI();
}

void BeginTx (const uint8* p_pucTxBuffer)
{
    SET_GPIO_LO( MODEM_IN_RTS );  // modem in Tx mode

    g_pucTxBuffer = p_pucTxBuffer;
    g_ucTxDataIndex = g_sDllGlobals.m_Device.m_PreambleLength;
    g_TxRxState = HART_TX_PREAMBLE;    
    //g_ucHartBusy = 1;

    // enable TX
    HART_TX_EN(); 
    
    // should wait (5 bit time) before start transmission
//    DelayLoop(100) ;
    
    // enable TX int
    HART_TX_INT_EN();
}

void HART_CarrierDetectWatch (void)
{
    static unsigned int lastState = 0;
    
    if( HART_UCON & UART_TX_ENABLED ) // TX on progress
    {
        if( HART_UTXCON != 32 ) // buffer not empty
        {
            return;
        }
        
        // nothing more to send, stop TX 
        DEBUG5_OFF();
        
        //disable tx int, cut the modem tx
        g_TxRxState = HART_TXRX_SNOOZE;
        HART_TX_DI(); 
        SET_GPIO_HI( MODEM_IN_RTS );

        g_sDllGlobals.m_MyTxIsDone = 1;
                    
        // reset HART busy-flag
        //g_ucHartBusy = 0;
    }

    unsigned int newState = GET_GPIO_VALUE_( MODEM_OUT_CD_SIGNAL );    
    if( !g_sDllGlobals.m_RxMessageReady && (lastState!=newState) ) 
    {
        if( newState ) // carrier ON
        {
            lastState = newState;

            // set HART busy-flag
            //g_ucHartBusy = 1;
            g_TxRxState = HART_RX_WAIT;
            IndicateCarrierInStatus();
            
            // enable RX 
            (void) HART_USTAT;
            
            HART_RX_EN(); 
            HART_RX_INT_EN();
            
            
        }// carrier OFF , if not RX command on progress or a timeout happend
        else if( (g_TxRxState < HART_RX_COMMAND) || ((RTC_COUNT-g_uiTimeOfLastByte) > GAP_TICKS) ) 
        {
            lastState = newState;
            
            IndicateNoCarrierInStatus();
            
            // disable RX 
            HART_RX_INT_DI(); 
            HART_RX_DI(); 
            
            g_TxRxState = HART_TXRX_SNOOZE;
        }
    }
}

#endif // HART_MODE




















