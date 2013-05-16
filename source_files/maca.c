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
* Name:         maca.c
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#include "maca.h"
#include "crm.h"
#include "WirelessHART/datalink_layer_dm.h"

const uint8 gaRFSynVCODivI_c[16] ={
          0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,0x2f, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30
  };

const uint32 gaRFSynVCODivF_c[16] = { 
      0x00355555, 0x006aaaaa, 0x00a00000, 0x00d55555, 0x010aaaaa, 0x01400000, 0x01755555, 0x01aaaaaa,\
      0x01e00000, 0x00155555, 0x004aaaaa, 0x00800000, 0x00b55555, 0x00eaaaaa, 0x01200000, 0x01555555
  };


typedef union
{
  struct
  {
    uint32 bAcplCode      :4;
    uint32                :7;
    uint32 b802           :1;
    uint32 bOvr           :1;
    uint32 bBusy          :1;
    uint32 bCrc           :1;
    uint32 bTo            :1;
    uint32                :16;
  } Bits;
  uint32 Reg;
} MACA_STATUS_CODE_T;

uint8  g_ucLastLQI;

#define RESET_POWER_OUTAGE 0
#define RESET_UNKWNOWN     1
#define RESET_DIRTY_MEM    2
#define RESET_NO_ACK       3

uint16 g_unStackSignature = 0x3A72;
uint16 g_unWatchDogRTC;
uint8  g_unPllLockWachDog;

#define IS_STRT_FLAG(x)        ( MASK_IRQ_STRT & (x) )
#define IS_SYNC_FLAG(x)        ( MASK_IRQ_SYNC & (x) )
#define IS_LVL_FLAG(x)         ( MASK_IRQ_LVL & (x) )
#define IS_DI_FLAG(x)          ( MASK_IRQ_DI & (x) )
#define IS_CRC_FLAG(x)         ( MASK_IRQ_CRC & (x) )
#define IS_ACPL_FLAG(x)        ( MASK_IRQ_ACPL & (x) )
#define IS_SFT_FLAG(x)         ( MASK_IRQ_SFT & (x) )


///////////////////////////////////////////////////////////////////////////////////
// Name:
// Author:        Marius Vilvoi
// Description:
//                this function will be used for
//
//
// Parameters:
//
// Return:        none
// Obs:
//      Access level:
//      Context:
///////////////////////////////////////////////////////////////////////////////////
#define PLATFORM_CLOCK         (24000000)
#define gDigitalClock_PN_c     24
#define gDigitalClock_RN_c     ((uint32)21)
#define gDigitalClock_RAFC_c_   0

void MACA_startClk(void)
{
  MACA_RESET = MACA_RESET_CLK_0N;    // set clock on and clear the reset flag      
  DelayLoop( 100 );
  
  MACA_CONTROL = CTRL_SEQ_NOP | CTRL_PRM;  
  while ( (MACA_STATUS & STATUS_CODE_MASK) == STATUS_CODE_NOT_COMPLETED )
    ;    
  
  MACA_CLRIRQ = 0xFFFF; // Clear all interrupts.     
}

void MACA_Init()
{      
//  MACA_startClk();
  
  //Freescale lib call
//  RadioInit(PLATFORM_CLOCK, gDigitalClock_PN_c, (gDigitalClock_RN_c<<25) + gDigitalClock_RAFC_c_ );
    
//  PHY_Reset();     
}

void MACA_Reset()
{
// reset the modem
    MACA_RESET = MACA_RESET_ON;                                               // set clock off and reset the modem        
    DelayLoop( 100 );
   
//wake up the modem
    MACA_startClk();
    
    RadioInit(PLATFORM_CLOCK, gDigitalClock_PN_c, (gDigitalClock_RN_c<<25) + gDigitalClock_RAFC_c_ );

// Freescale forum patch start // SMAC_InitFlybackSettings
    #define RF_BASE 0x80009a00 
    
    uint32_t val8, or; 
    val8 = *(volatile uint32_t *)(RF_BASE+8); 
    or = val8 | 0x0000f7df; 
    *(volatile uint32_t *)(RF_BASE+8) = or; 
    *(volatile uint32_t *)(RF_BASE+12) = 0x00ffffff; 
    *(volatile uint32_t *)(RF_BASE+16) = (((uint32_t)0x00ffffff)>>12); 
    *(volatile uint32_t *)(RF_BASE) = 16;       
// Freescale forum patch end
      
    
    MACA_TMRDIS       = 0x0000003F;                                           // Disable all action triggers
    MACA_CLKDIV       = 0x0000005F;                                           // DIVIDER_24_MHZ_TO_250_KHZ      
    MACA_WARMUP       = 0x00180012;      
    MACA_EOFDELAY     = 0x00000004;  // 16 us for complete action delay
    MACA_CCADELAY     = 0x00200020;  // <- Freescale recomandation //0x001a0022;
//    MACA_CCADELAY     = 0x001A0022; 
    MACA_TXCCADELAY   = 0x00000025;  

    MACA_PREAMBLE     = 0x00000000;
    MACA_FRAMESYNC0   = 0x000000A7;
    MACA_FRAMESYNC1   = 0;
    MACA_FLTREJ       = 0;
    
#ifdef TRIGGER_RX_LEVEL      
    MACA_SETRXLVL     = MACA_RX_LEVEL_TRIGGER + 1;                            //  de corelat cu IRQs de LVL poate nu se va folosi deloc
#else
    MACA_SETRXLVL     = 0x00FF; 
#endif
    
    MACA_MASKIRQ      = 0;

    
#ifdef SINGLE_ENDED_RF
    SetComplementaryPAState( 0 );
#else    
    SetComplementaryPAState( 1 ); // dual port
#endif        
    
    SetPowerLevelLockMode( 0 ); // no power lock ?? TO be checked    
    SetEdCcaThreshold( 30 );
//    SetEdCcaThreshold( 60 ); // very big -> no CCA
    
#ifdef BACKBONE_SUPPORT      
//  NCD Mode Provides an increased ~3.5 dB of sensitivity. However, the addition of the AFC
//            increases the demodulator current drain about 3 mA.
// DCD Mode Default receive mode at lower current.    
//    SetDemulatorMode( NCD ); // higher sensitivity
#endif
    
    CRM_REGS_P->VregCntl = 0x00000FF8; //Enables the radio voltage source  
    while( !CRM_STATUS.vReg1P5VRdy ) // Wait for regulator to become active
      ;  
        
    g_ucXVRStatus = PHY_DISABLED;
    
    MACA_CLRIRQ = 0xFFFF; // Clear all interrupts.     
    
    SetPower( g_ucPAValue ); 
}

///////////////////////////////////////////////////////////////////////////////////
// Name:
// Author:        Marius Vilvoi
// Description:
//                this function will be used for
//
//
// Parameters:
//
// Return:        none
// Obs:
//      Access level:
//      Context:
///////////////////////////////////////////////////////////////////////////////////
void MACA_Interrupt(void)
{
MACA_STATUS_CODE_T ulMacaStatus;

//#ifdef DEBUG_RF_LINK
//extern uint8 g_ucDllStatus;
//volatile uint16 unTmr0Cnt = TMR0_CNTR;   // hold the timestamp
//#endif

      DEBUG2_ON();
      DEBUG2_OFF();
uint32 ulGetIrqs = MACA_IRQ;

  ulMacaStatus.Reg = MACA_STATUS;
  MACA_CLRIRQ = ulGetIrqs;

  CK_LOAD_Start( CK_LOAD_MODEM_IRQ ); // active if CK_LOAD_SUPPORT only

//  if ( IS_STRT_FLAG(ulGetIrqs) )
//  {
//      DEBUG2_ON();
//      DEBUG2_OFF();
//  }

  if( g_ucXVRStatus == PHY_RX_START_DO ) // was on correct status
  {
//#ifdef DEBUG_RF_LINK
//      if ( IS_SYNC_FLAG(ulGetIrqs) )
//      {
//        unTmr0Cnt /= 6;
//        if(g_ucDllStatus == DLL_SYNCRONISED)
//          DEBUG2_OFF(); // just to support a breakpoint on SYNC case
//      }
//#endif
      if ( IS_CRC_FLAG(ulGetIrqs) )
      {
        PHY_ErrorIndicate(RX_CRC_ERROR, NULL);        
      }
#ifdef TRIGGER_RX_LEVEL      
      else if ( IS_LVL_FLAG(ulGetIrqs)  )
      {        
        MACA_MASKIRQ &= ~MASK_IRQ_LVL ;
        MACA_CLRIRQ = ulGetIrqs;        
        // TO DO at RX LEVEL IRQ
        // ...
      }      
#endif      
      else if ( IS_DI_FLAG(ulGetIrqs) ) // valid RX packet
      {        
        DEBUG3_OFF();
                            
        PHY_Disable_Request();
	    g_unPllLockWachDog = 0;
        
        uint8 ucRxLen = g_aucPhyRx[0] - 2; // remove the CRC from length
        if( ucRxLen < (PHY_BUFFERS_LENGTH-2) )
        {      
//          g_unWatchDogRTC = 0;
          
          g_ucLastLQI = PhyPlmeGetLQI(); 
          DEBUG3_ON();
//          if( !PHY_HeaderIndicate( (uint8*)g_aucPhyRx ) )
//          {
//              PHY_ErrorIndicate(OTH_ERROR, NULL);        
//          }
//          else 
          {
          	//TODO: could be optimized: first buffer byte contains the packet length
          	PHY_DataIndicate( g_ucLastLQI, (unsigned char *) g_aucPhyRx+1, ucRxLen ); // LQI + LEN = 2  ... so - 2 ?! ATENTIE !!! (g_aucPhyRx[0] si buff len la receptie)
          }
        }
        else
        {
          PHY_ErrorIndicate(RX_BUFFER_OVERFLOW, NULL);
        }
        DEBUG3_OFF();
      }
      else if( IS_ACPL_FLAG(ulGetIrqs) ) // action complete
      {
        DEBUG3_OFF();
#ifdef SEND_FICTIVE_ACK         
        if( MACA_STATUS_COMPLETE_CODE == 8 ) // soft timeout
        {
          // generate a fictive ACK
          DLL_SendFictiveACK(); 
        }
        /*
        else if( MACA_STATUS_COMPLETE_CODE == 9 ) // soft pending timeout
        { // nothing tbd here 
        }
        */
        else
#endif          
        {  
          PHY_ErrorIndicate(PACKET_INCOMPLETE, NULL);   
        }  
      }
  }
  else if ( g_ucXVRStatus == PHY_TX_START_DO )
  {
      DEBUG4_OFF();
      if( IS_ACPL_FLAG(ulGetIrqs) ) // action complete
      {
        PHY_Disable_Request();
        DEBUG4_ON();

        if ( ulMacaStatus.Bits.bAcplCode )
        {
          PHY_DataConfirm(PHY_DATA_OTH_ERROR );
        }
        else if( ulMacaStatus.Bits.bBusy )
        {
          PHY_DataConfirm( TRANCEIVER_BUSY  );
        }
        else
        {
          PHY_DataConfirm(PHY_DATA_SUCCES );
        }
        DEBUG4_OFF();
      }
      else // was start only case
      {
          DEBUG4_ON();
      }
  }
  CK_LOAD_Stop( CK_LOAD_MODEM_IRQ ); // active if CK_LOAD_SUPPORT only  
}


  // !!! de apelat in main la 1 secunda
  void MACA_WatchDog(void)
  {  

    if( g_unStackSignature != 0x3A72 )
    {
        PrepareForReset(RREASON_STACK_CORRUPTION); 
    }

    if ((g_ucDllStatus != DLL_IDLE) && (g_ucDllStatus != DLL_PASSIVE_SEARCH)) // TODO: check if this is OK for active search, too
    {          
          if( g_unPllLockWachDog >= 2 )
          {
              MONITOR_ENTER();
              PHY_Disable_Request();
              
              g_unPllLockWachDog = 0;
              *(unsigned int *)(0x80009380) = 0xFFFFFFFF;
//              *(unsigned int *)(0x80009380) |= 0x800;
              
              DelayLoop(1000);
              
              MACA_Reset();
              
              MONITOR_EXIT();
          }

    #if !defined(BACKBONE_SUPPORT)
        if( ++g_unWatchDogRTC > 900 ) // nothig received 15 minutes ... something very wrong ... .. reset it
        {
            PrepareForReset(RREASON_TOUT_RECEPTION);
        }
    #endif
    }
  }

///////////////////////////////////////////////////////////////////////////////////
// Name:
// Author:        Received from Freescale
// Description:
//                this function will be used for computing ED 
// Parameters:
// Return:        ED
// Obs:
//      Access level:
//      Context:
///////////////////////////////////////////////////////////////////////////////////
//uint8 GetED()
//{
//  uint32 mEdVal = (*((volatile uint32 *)(0x80009488)));   //0x80009488 = undocumented register in Modem Radio Receiver Functions
//  mEdVal = mEdVal & 0x000000FF;  
//  if(mEdVal<0x0F) return 0x00;
//  if(mEdVal>0x64) return 0xFF;   
//  return ((mEdVal<<1)+mEdVal)-0x2D;
//}

