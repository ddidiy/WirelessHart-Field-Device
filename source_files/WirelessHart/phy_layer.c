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
* Name:         phy.c
* Author:       Marius Vilvoi
* Date:         November 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#include "phy_layer.h"
#include "../itc.h"
#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/PhysicalLayerCommands.h"
#else
  #include "ApplicationLayer/Model/PhysicalLayerCommands.h"
#endif



#define ONE_BYTE_LEN  ( 8 )  //  1 byte = 32 uSec = 2 symbols => 2 Symbols / 0.25 Symbols clock step = 8
#define PHY_LATENT_DELAY  50

#define DISABLE_SFD_SEARCH() *(uint32*)0x80009418 &= ~0x00020000
#define ENABLE_SFD_SEARCH()  *(uint32*)0x80009418 |=  0x00020000

#define LOW_TX_POWER   (-30)
#define MED_TX_POWER   (-10)
#define HIGH_TX_POWER  (6)

volatile uint8 g_ucXVRStatus = PHY_DISABLED;                                    // state machine
volatile uint8 g_aucPhyRx[PHY_BUFFERS_LENGTH + 1];                              // plus 1 FRAME LEN !


        // BAD DOCUMENTATION => NOT FULL SPECIFIED LQI
        // The first transferred data will be the LQI and packet length. The receive
        // buffer must be large enough to hold these bytes as well as the data.

// la receptie este receptionat in primul byte lungimea mesajului primit inclusiv CRC(FCS)
// nu apare nici un LQI = iar CRC(FCS)-ul nu este copiat in memorie
// deci buferul trebuie sa aiba lungimea mesajului maxim plus 1
// diferenta intre disable si idle este faptul ca la idle MACA CLOCK in on iar la disable MACA CLOCK is off



///////////////////////////////////////////////////////////////////////////////////
// Name:          Delay
// Author:        Marius Vilvoi
// Description:   Local function to be used for delay in init proccess (in specs it is required 700 us)
// Parameters:
//                p_ulCount = number of loop to wait
// Return:        none
// Obs:
///////////////////////////////////////////////////////////////////////////////////
void Delay(uint32 p_ulCount)
{
volatile uint32 i = 0;

  for ( ; i < p_ulCount; i++ )
  {
    ;
  }
}




///////////////////////////////////////////////////////////////////////////////////
/// @author NIVIS LLC, Ion Ticus, Marius Vilvoi
/// @brief  Set MACA_CONTROL register
/// @param  p_ulMacaCtrlValue - new value to be set
/// @return none
/// @remarks 
///      Access level: Interrupt level
///      Context: 
///////////////////////////////////////////////////////////////////////////////////
__arm void PHY_setMacaCtrl( uint32 p_ulMacaCtrlValue )
{
    volatile uint8 i = 24;
    MACA_CONTROL = p_ulMacaCtrlValue;
    
    while( --i )
      ;
}



///////////////////////////////////////////////////////////////////////////////////
// Name:          PHY_DisableTR
// Author:        Marius Vilvoi
// Description:   this function will be used for disable transceiver at all (switch to PHY_DISABLE)
// Parameters:
// Return:        none
// Obs:
//      Access level: Interrupt level
//      Context:
///////////////////////////////////////////////////////////////////////////////////
//.void PHY_Disable_Request ( uint8 p_ucNewStatus )
void PHY_DisableTR ( void ) // as ISA
{
  if( g_ucXVRStatus != PHY_DISABLED )
  {     
      //uint32_t clk;
      volatile uint32_t i;
      uint32 unMacaStatusCode = MACA_STATUS & 0x0F;
      
      TX_PA_OFF();
      
      MACA_TMRDIS = (CLK_START | CLK_CPL | CLK_SOFT | CLK_START_ABORT | CLK_CPL_ABORT | CLK_SOFT_ABORT); // disable all timers      
      MACA_CLRIRQ =  MACA_CLRIRQ_MASK;
      
      if( unMacaStatusCode == 12 ) // it is pll unlock error
      {
          g_unPllLockWachDog ++;
      } 
      
// ----------- Freescale patch --------------
      {
          uint32_t clk, TsmRxSteps, LastWarmupStep, LastWarmupData, LastWarmdownStep, LastWarmdownData;
          
//          ITC_DisableInterrupt(gMacaInt_c);  
        // Manual TSM modem shutdown 
        
          // read TSM_RX_STEPS 
          TsmRxSteps = (*((volatile uint32_t *)(0x80009204)));
         
          // isolate the RX_WU_STEPS 
          // shift left to align with 32-bit addressing 
          LastWarmupStep = (TsmRxSteps & 0x1f) << 2;
          // Read "current" TSM step and save this value for later 
          LastWarmupData = (*((volatile uint32_t *)(0x80009300 + LastWarmupStep)));
        
          // isolate the RX_WD_STEPS 
          // right-shift bits down to bit 0 position 
          // left-shift to align with 32-bit addressing 
          LastWarmdownStep = ((TsmRxSteps & 0x1f00) >> 8) << 2;
          // write "last warmdown data" to current TSM step to shutdown rx 
          LastWarmdownData = (*((volatile uint32_t *)(0x80009300 + LastWarmdownStep)));
          (*((volatile uint32_t *)(0x80009300 + LastWarmupStep))) = LastWarmdownData;
        
          // Abort 
//          MACA_CONTROL = CTRL_SEQ_ABORT; // Freescale original implementation
          MACA_CONTROL = CTRL_SEQ_ABORT | CTRL_PRM | CTRL_ASAP;
          
          // Wait ~8us 
          for (clk = MACA_CLK, i = 0; MACA_CLK - clk < 3 && i < 300; i++)
            ;
         
          // NOP 
//          MACA_CONTROL = CTRL_SEQ_NOP; // Freescale original implementation
          MACA_CONTROL = CTRL_SEQ_NOP | CTRL_PRM;
          
          
          // Wait ~8us 
          for (clk = MACA_CLK, i = 0; MACA_CLK - clk < 3 && i < 300; i++)
            ;
                   
          // restore original "last warmup step" data to TSM (VERY IMPORTANT!!!) 
          (*((volatile uint32_t *)(0x80009300 + LastWarmupStep))) = LastWarmupData;
        
          // Clear all MACA interrupts - we should have gotten the ABORT IRQ 
          MACA_CLRIRQ =  MACA_CLRIRQ_MASK;
          
//          ITC_EnableInterrupt(gMacaInt_c);          
      }
      
// ----------- Freescale patch end --------------
                  
      g_ucXVRStatus = PHY_DISABLED;
      
      DEBUG3_OFF();
      DEBUG4_OFF();
  }
}



///////////////////////////////////////////////////////////////////////////////////
// Name:          PHY_RX_Request
// Author:        Marius Vilvoi
// Description:   this function will be used for
//                    enable transceiver in receive mode on desired frequency channel (p_ucChannel)
//                    the receive sequence will begin after minimum 152uS
//                    so as an example :
//                      p_unDelay = 0 - 152 => 152 uS
//                      p_unDelay = 153,154,155,156 => 152 + 4 uS = 156 uS
//                      p_unDelay = 157,158,159,160 => 152 + 8 uS = 160 uS ... and so on
// Parameters:
//                p_ucChannel = Channel index
//                p_unDelay =  Delay [us]
//                p_unAbortTimeout = Timeout for RX in 4us steps (not TMR2 steps!), from start RX to end RX; 0 = NO TIMEOUT
// Return:        none
// Obs:
//      Access level: Interrupt level
//      Context:
///////////////////////////////////////////////////////////////////////////////////
void PHY_RX_Request ( uint8 p_ucChannel, uint16 p_unDelay, uint16 p_unAbortTimeout )
{  
    uint32 ulMacaCtrl;
    uint32 ulMacaIrq =  MASK_IRQ_CRC | MASK_IRQ_DI;
        
#ifdef DEBUG_RF_LINK    
   ulMacaIrq |= MASK_IRQ_STRT;
#endif    
#ifdef TRIGGER_RX_LEVEL    
   ulMacaIrq |= MASK_IRQ_LVL; 
#endif    
    
    p_unDelay = p_unDelay / 4;

    MACA_TMRDIS = (CLK_START | CLK_CPL | CLK_SOFT | CLK_START_ABORT | CLK_CPL_ABORT | CLK_SOFT_ABORT); // disable all timers
    MACA_CLK = 0;
    
    ulMacaCtrl = (CTRL_PRM | CTRL_SEQ_RX);
    
    PHY_Disable_Request();
    
    ENABLE_SFD_SEARCH();
    
    SET_CHANNEL(p_ucChannel);
    
    MACA_TXLEN = 0x007F0000;    // RXLen
    MACA_DMATX = (uint32) g_aucPhyRx;
    MACA_DMARX = (uint32) g_aucPhyRx;
    g_ucXVRStatus = PHY_RX_START_DO;

    MACA_CLKOFFSET = 0;
    if( p_unDelay > (MACA_CLK + WARM_UP_DELAY) ) // minimum delay
    {
        MACA_STARTCLK = p_unDelay;
        if ( p_unAbortTimeout )
        {
          MACA_SFTCLK = p_unDelay + p_unAbortTimeout;
          MACA_TMREN = CLK_START | CLK_SOFT;   // enable Soft Timeout for RX Clock for timer triggered action          
          ulMacaIrq |= MASK_IRQ_ACPL;
        }
        else
        {
            MACA_TMREN = CLK_START;    // enable Start Clock for timer triggered action
        }
    }
    else // go to on RX now
    {
        if ( p_unAbortTimeout > MACA_CLK + WARM_UP_DELAY )
        {
          MACA_SFTCLK = p_unAbortTimeout;
          MACA_TMREN = CLK_SOFT;    // enable Soft Timeout for RX Clock for timer triggered action    
          ulMacaIrq |= MASK_IRQ_ACPL;
        }
        else
        {
          MACA_TMREN = 0x00;          // disable Start Clock and Soft Timeout Clock for timer triggered action
        }
        ulMacaCtrl |= CTRL_ASAP;
    }
    
    MACA_MASKIRQ = ulMacaIrq; 
    MACA_CONTROL = ulMacaCtrl;
          
    while(p_ucChannel--)
    {
        DEBUG3_ON();
        DEBUG3_OFF();
    }
    
    DEBUG3_ON();
}



///////////////////////////////////////////////////////////////////////////////////
// Name:          PHY_TX_Request
// Author:        Marius Vilvoi
// Description:   this function will be used for
//                    enable transceiver in transmit mode on desired frequency channel
//                    the transmit sequence will begin after minimum 152uS
//                      p_unDelay = 0 - 152 => 152 uS
//                      p_unDelay = 153,154,155,156 => 152 + 4 uS = 156 uS
//                      p_unDelay = 157,158,159,160 => 152 + 8 uS = 160 uS ... and so on
//                    See NOTE 1 below for timing
// Parameters:
//                p_ucChannel - Channel
//                p_unDelay   - us Delay
//                p_ucCCAFlag - 0 => without CCA ; else => with CCA
//                p_pucData   - pointer to Data buffer
//                p_ucLen     - max 125 bytes
// Return:        none
// Obs:
//      Access level: Interrupt level
//      Context:
//      Note 1:   For any TX sequence the RF transimted bytes (symbols) are :
//                            Preamble                        = 4 Bytes = 8 symbols = 128 uS
//                            Start of Frame Delimiter (SFD)  = 1 bytes = 2 symbols = 32 uS
//                            PHR (frame len)                 = 1 bytes = 2 symbols = 32 uS
//                            PHY message ... ()              = max 125 bytes
//                            FCS (CRC)                       = 2 bytes = 4 symbols = 64 uS
//                FOR TX with CCA delays :      = 152us minimum = delay
//                                              = 144uS = CCA len
//                                              = 192uS = from CCA back to TX front (TX seq as described above)
//                FOR TX without CCA delays :   = 152us = delay TX front (TX seq as described above)
//      Note 2:   MACA_TXLEN register specifies how many bytes of data is to be transmitted
//                during a Tx auto-sequence (including checksum, but excluding PHY header).
///////////////////////////////////////////////////////////////////////////////////
void PHY_TX_Request ( uint8 p_ucChannel, uint16 p_unDelay, uint8 p_ucCCAFlag, uint8 * p_pucData, uint8 p_ucLen )
{
  uint32 ulMacaCtrl;
    
  p_unDelay = p_unDelay / 4;

  MACA_TMRDIS = (CLK_START | CLK_CPL | CLK_SOFT | CLK_START_ABORT | CLK_CPL_ABORT | CLK_SOFT_ABORT); // disable all timers
  MACA_CLK = 0;
  
  ulMacaCtrl = ( p_ucCCAFlag ?  (CTRL_PRM | CTRL_MODE_NO_SLOTTED | CTRL_SEQ_TX) : (CTRL_PRM | CTRL_MODE_NO_CCA | CTRL_SEQ_TX) );
  
  PHY_Disable_Request();
  
  TX_PA_ON();
  
  
  SET_CHANNEL(p_ucChannel);
    
  MACA_TXLEN = (uint32) (p_ucLen + 2);                                        // p_ucLen Data buffer plus + 2 crc => see Obs. Note.
  MACA_DMATX = (uint32) p_pucData;
  MACA_DMARX = (uint32) g_aucPhyRx;                                           // here need for temp ack buffer !
  g_ucXVRStatus = PHY_TX_START_DO;

  if( p_unDelay >= (MACA_CLK + WARM_UP_DELAY) ) // minimum delay
  {
      MACA_CLKOFFSET = 0;
      MACA_STARTCLK = p_unDelay;
      MACA_TMREN = CLK_START;   // enable Start Clock for timer triggered action        
  }
  else // go to on TX now
  {
      MACA_TMREN = 0x00; // disable Start Clock for timer triggered action
      ulMacaCtrl |= CTRL_ASAP;
  }
  
#ifdef DEBUG_RF_LINK
  MACA_MASKIRQ = (MASK_IRQ_STRT | MASK_IRQ_ACPL);
#else
  MACA_MASKIRQ = (MASK_IRQ_ACPL);
#endif
  
  MACA_CONTROL = ulMacaCtrl;
  
#ifdef DEBUG_RF_LINK
    while(p_ucChannel--)
    {
          DEBUG4_ON();
          DEBUG4_OFF();
    }
#endif

  DEBUG4_ON();
}


 
///////////////////////////////////////////////////////////////////////////////////
// Name:          PHY_MNG_Request
// Author:        Marius Vilvoi
// Description:   this function implements the WirelessHART primitive :
//
//
// Parameters:
//                unsigned char p_ucService =
//                unsigned char * p_pucData =
// Return:        none
// Obs:
//      Access level:
//      Context:
///////////////////////////////////////////////////////////////////////////////////
unsigned char PHY_MNG_Request ( unsigned char p_ucService, void * p_pData)
{

  switch ( p_ucService )
  {
//.    case PHY_MNG_RESET : //. replaced by MACA_Reset() as on ISA100
//
//      MACA_RESET = MACA_RESET_ON;                                               // set clock off and reset the modem          
//      Delay( 200 );                                                             // 118 us      
//      MACA_RESET = MACA_RESET_CLK_0N;                                           // set clock on and clear the reset flag
//      MACA_CONTROL = CTRL_SEQ_NOP;            
//      while ( (MACA_STATUS & STATUS_CODE_MASK) == STATUS_CODE_NOT_COMPLETED )
//        ;
//      
//      MACA_TMRDIS       = 0x0000003F;                                           // Disable all action triggers
//      MACA_CLKDIV       = 0x0000005F;                                           // DIVIDER_24_MHZ_TO_250_KHZ      
//      MACA_WARMUP       = 0x00180012;      
//      MACA_EOFDELAY     = 0x00000004;  // 16 us for complete action delay
//      MACA_CCADELAY     = 0x00200020;  // <- Freescale recomandation //0x001a0022;
//      MACA_TXCCADELAY   = 0x00000025;  // <-
//      MACA_PREAMBLE     = 0x00000000;
//      MACA_FRAMESYNC0   = 0x000000A7;
//      MACA_FRAMESYNC1   = 0;
//      MACA_FLTREJ       = 0;
//      MACA_SETRXLVL     = 0x00FF; //0xFFFF;                                     //  de corelat cu IRQs de LVL poate nu se va folosi deloc      
//      MACA_MASKIRQ      = 0;
//
//      Delay( 1000 );                                                            // 585 us
//      g_ucXVRStatus = PHY_DISABLED;                                             // init but without working clock
//.      return PHY_MNG_SUCCES;

    case PHY_MNG_READ_TX_PWR_LVL :
#define p       ((C798_ReadRadioPower_Resp*)p_pData)
//      p->outputPower = 2 * (int8) PA_ADJ_TARGET_PWR + LOW_TX_POWER;
//      if(p->outputPower > MED_TX_POWER)
//        p->outputPower -= 2;
      p->outputPower = 10;  // only +10dBm for now
      return PHY_MNG_SUCCES;  
#undef p
      
    case PHY_MNG_WRITE_TX_PWR_LVL :
    { 
#define p       ((C797_WriteRadioPower_Resp*)p_pData)
      int8 Data = p->outputPower;
      
      if(Data != 10)
      {
        p->outputPower = 10;  // only accept  +10dBm for now
        return PHY_MNG_WARN_SET_TO_NEAREST_VAL;
      }
      else
      {
        p->outputPower = 10;  // only accept  +10dBm for now
        return PHY_MNG_SUCCES;  
      }
#undef p
     }
     
  case PHY_MNG_WRITE_RX_OVERFLOW_LVL :
      // TBD
      return PHY_MNG_SUCCES;  // or one of the defined error/warning codes - see phy_layer.h

  default:
      return PHY_MNG_ERR_DEVICE_COMMAND_ERR;
  }
}




///////////////////////////////////////////////////////////////////////////////////
// Name:          getLastRXuSecAge
// Author:        Marius Vilvoi
// Description:   uSec de la inceput ultimului RX astfel :
//                inainte de preambul = MACA_CLK - MACA_TIMESTAMP + 6 * ONE_BYTE_LEN
//                inainte de SFD      = MACA_CLK - MACA_TIMESTAMP + ONE_BYTE_LEN + ONE_BYTE_LEN
//                dupa SFD            = MACA_CLK - MACA_TIMESTAMP + ONE_BYTE_LEN
//                dupa F Len          = MACA_CLK - MACA_TIMESTAMP
//                The MACA_TIMESTAMP register contains the latched clock for the last received packet. 
//                The clock is latched immediately after receiving the length field
// Parameters:   
// Return:
// Obs:
//      Access level: Interrupt level
//      Context:
///////////////////////////////////////////////////////////////////////////////////
__arm uint32 PHY_GetLastRXuSecAge(void)
{
// values are volatile but we are OK with any order
#pragma diag_suppress=Pa082
  return (uint32)( MACA_CLK - MACA_TIMESTAMP + 6 * ONE_BYTE_LEN ) * 4 + PHY_LATENT_DELAY; // !! ISA hasn't 6 * ONE_BYTE_LEN
#pragma diag_default=Pa082
}



///////////////////////////////////////////////////////////////////////////////////
// Name:          getLastTXuSecAge
// Author:        Marius Vilvoi
// Description:   uSec de la sfarsitul ultimului TX (Action Complete). 
//                  Este diferit de ISA100. 
//                  Folosit pentru alinierea la receptia ACK-ului  
// Parameters:
// Return:
// Obs:
//      Access level: Interrupt level
//      Context:
///////////////////////////////////////////////////////////////////////////////////
__arm uint32 PHY_GetLastTXuSecAge(void)
{
// values are volatile but we are OK with any order
#pragma diag_suppress=Pa082
  return (uint32)( MACA_CLK - MACA_CPLTIM ) * 4;
#pragma diag_default=Pa082
}



