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
* Name:         crm.c
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#include "typedef.h"
#include "maca.h"
#include "itc.h"
#include "crm.h"
#include "WirelessHART/datalink_layer.h"
#include "WirelessHART/datalink_layer_dm.h"
#include "WirelessHART/tmr_util.h"
#include "WirelessHART/uap.h"
#include "WiredHart/options.h"
#include "system.h"

void RTC_ExtRtcInit (void);

#if ( SPI1_MODE != NONE ) && defined( WAKEUP_ON_EXT_PIN ) // wake up for SPI interface
  uint32 g_ulLastCrmStatus;
#endif 

#ifdef CK_LOAD_SUPPORT
  CK_LOAD_STRUCT g_aCkLoadTable[CK_LOAD_STRUCT_NO];

  void CK_LOAD_Stop(uint8 p_unIdx)
  {
    uint16 unLoad = CK_LOAD_TMR_COUNTER - g_aCkLoadTable[ p_unIdx ].m_unCrtCounter;
    g_aCkLoadTable[ p_unIdx ].m_unLastLoad = unLoad; 
    if( g_aCkLoadTable[ p_unIdx ].m_unMaxLoad < unLoad )
    {
      g_aCkLoadTable[ p_unIdx ].m_unMaxLoad = unLoad;
    }    
    if( 0 == (++ (g_aCkLoadTable[ p_unIdx ].m_unAvgCounter) ) )
    {
      g_aCkLoadTable[ p_unIdx ].m_ulAvgAcc = 0;
    }
    g_aCkLoadTable[ p_unIdx ].m_ulAvgAcc += unLoad;
  }
#endif
  
uint32 g_aSaveGPIO[4];  
uint32 g_ul250msStartTmr; // this variable is used to memorise the moment of the start of a 250ms interval 
uint32 g_ul250msEndTmr;   // this variable is used to memorise the moment of the end of a 250ms interval 
uint16 g_unRtc250msDuration = RTC_32KHZ_250MS_TICS;


__arm void RtcClock_Interrupt(void)
{    
  ITC.IntDisNum = gCrmInt_c;             // disable CRM interrupt until timer interrupt
#if ( SPI1_MODE != NONE ) && defined( WAKEUP_ON_EXT_PIN ) // wake up for SPI interface
    g_ulLastCrmStatus |= _CRM_STATUS.Reg; // save wake up status before clear the register
#endif    
  _CRM_STATUS.Reg = _CRM_STATUS.Reg; 
  
  g_ul250msStartTmr = RTC_COUNT; 
  RTC_TIMEOUT = g_unRtc250msDuration;
  
  // Debug pulse on
//  DEBUG5_ON();
  
  ITC_TestSet(gTmrInt_c);                                                     // force timer interrupt        
  
  g_ul250msEndTmr = g_ul250msStartTmr + g_unRtc250msDuration + 2;             // duration is 2 tics more that g_unRtc250msDuration
  g_unRtc250msDuration = RTC_32KHZ_250MS_TICS;
 
  // Debug pulse off
//  DEBUG5_OFF();  
}

void CRM_Init (void)
{
  //XtalAdjust
  //CRM_XTAL_CNTL.CTune = 0x15;//0x18;
  //CRM_XTAL_CNTL.FTune = 0x10;//0x0F;
  RTC_ExtRtcInit();
  
#if (HART_MODE != NONE)
  
  #if( (HART_DEV_KIT_REV == HART_DEV_KIT_REV3) ) 
    CRM_WU_CNTL_EXT_WU_EN   = 1;    // 1 = 0x0001 -> wake up on KBI_4
  #elif ( (HART_DEV_KIT_REV == HART_DEV_KIT_REV4) )   
    CRM_WU_CNTL_EXT_WU_EN   = 9;    // 9 = 0x1001 -> wake up on KBI_4 or KBI_7
  #endif 

#else
  
    CRM_WU_CNTL_EXT_WU_EN   = 0;    // no wake up    

#endif
    
  CRM_WU_CNTL_EXT_WU_POL &= ~4;   // internal Pullup for KBI_6(Wakeup and Status button)
    
  CRM_WU_CNTL_EXT_WU_IEN  = 0;    // without generating an interrupt
    
  CRM_WU_CNTL_TIMER_WU_EN   = 1;  // wake up from counter
  CRM_SLEEP_CNTL_RAM_RET    = 3;  // 3=96k
  CRM_SLEEP_CNTL_MCU_RET    = 1;  // e un must pt ticus
  CRM_SLEEP_CNTL_DIG_PAD_EN = 1;
    
  //  Set 32KHz pin in 1 to keep osc on ... only for old versions
  SET_GPIO_HI(KYB0_LINE);         // only for old versions of VN210 (C, D)
  CRM_WU_CNTL_EXT_OUT_POL = 0x01; // only for old versions of VN210 (C, D)

  //start the 32KHz osc
  _RINGOSC_CNTL.Bits.ROSC_EN=0;
  _XTAL32_CNTL.Bits.XTAL32_EN=1; //start the osc
  //XTAL32_CNTL = 1;  // !!!!!! wrong
  
  DelayLoop( 100 * 1000 );
  
  //source RTC_counter from 32KHz
  CRM_SYS_XTAL32_EXISTS = 1;  
  //reduce pwr consumption
  XTAL32_CNTL = 1;
          
  // set RTC interrupt    
  IntAssignHandler(gCrmInt_c, (IntHandlerFunc_t)RtcClock_Interrupt);
  ITC_SetPriority(gCrmInt_c, gItcFastPriority_c); //fiq
  ITC_EnableInterrupt(gCrmInt_c);
  g_ul250msStartTmr = RTC_COUNT; 
  RTC_TIMEOUT = RTC_32KHZ_250MS_TICS;
  CRM_WU_CNTL_RTC_WU_EN = 1;
  CRM_WU_CNTL_RTC_WU_IEN = 1;
}

void SSI_sendByte( uint8 p_ucByte )
{
    unsigned int i = 0x80;
    for(;i; i >>= 1)
    {
        //bit out
        Gpio_SetPinData(SSI_MOSI, p_ucByte & i ? gGpioPinStateHigh_c : gGpioPinStateLow_c);
        
        //clk 0
        Gpio_SetPinData(SSI_CLK, gGpioPinStateLow_c);        
        
        //clk 1
        Gpio_SetPinData(SSI_CLK, gGpioPinStateHigh_c);        
    }
}

void RTC_ExtRtcInit (void)
{
    Gpio_SetPinData( SSI_CLK, gGpioPinStateHigh_c);
    Gpio_SetPinData( RTC_CS, gGpioPinStateHigh_c );
   
    // write on register 0x0F
    SSI_sendByte( 0x1F ); 
    SSI_sendByte( 0x40 ); // disable all interrupt events
    
    Gpio_SetPinData( RTC_CS, gGpioPinStateLow_c );      
    
    Gpio_SetPinData( SSI_CLK, gGpioPinStateHigh_c);
    Gpio_SetPinData( RTC_CS, gGpioPinStateHigh_c );
    
    // write on register 0x0D
    SSI_sendByte( 0x1D ); 
    SSI_sendByte( 0x00 ); // 32KHz output
    
    Gpio_SetPinData( RTC_CS, gGpioPinStateLow_c );      
    
    Gpio_SetPinData( SSI_MOSI, gGpioPinStateLow_c); // lower consumption
    Gpio_SetPinData( SSI_CLK, gGpioPinStateLow_c); // lower consumption
}

  
#if ( SPI1_MODE != NONE ) && defined( WAKEUP_ON_EXT_PIN ) // wake up for SPI interface
  uint8 CRM_ResetWakeUpFlag(void)
  {
      uint8 ucResult = (g_ulLastCrmStatus | _CRM_STATUS.Reg) & 0x10; // KBI4
      
      _CRM_STATUS.Reg = ucResult; // clear the EXT status flag
      g_ulLastCrmStatus = 0;      // clear the last saved EXT status 
      
      return ucResult;
  }
#endif
