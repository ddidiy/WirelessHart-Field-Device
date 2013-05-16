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
* Name:         timers.c
* Author:       Marius Vilvoi
* Date:         July 2008
* Description:  
* Changes:
* Revisions:
****************************************************************************************************/
#include "timers.h"
#include "crm.h"
#include "itc.h"
#include "WirelessHART/tmr_util.h"
#include "WirelessHART/phy_layer.h"
#include "WirelessHART/datalink_layer.h"
#include "WirelessHART/datalink_layer_dm.h"
#include "WirelessHART/network_layer.h"
#include "WirelessHART/network_layer_dm.h"

int16 g_nCrtClkCorrection;

volatile unsigned char g_uc10msFlag = 0;
volatile unsigned char g_uc250msFlag = 0;

void TMR_Init(void)
{
  TmrInit();  

  TmrEnable( gTmr0_c );
  TmrEnable( gTmr1_c );
  TmrEnable( gTmr3_c );  
  
  TMR0_INIT(); // DLL
  TMR1_INIT(); // RTC
  TMR3_INIT(); // DEBUG

  TMR1_START();
  TMR3_START();
}


void TMR_DLL_Function()
{
#ifndef BACKBONE_SUPPORT  
static uint8 s_ucActiveChannel = 0;
#endif

  DLL_IncrementASN(); 
  
  switch ( g_ucDllStatus )
  {  
  case DLL_SYNCHRONIZED:    
    DLL_ComputeSuperframeOffset();
    DLL_Configure();
  case DLL_ACTIVE_SEARCH:     
#ifndef BACKBONE_SUPPORT
    if( ! (g_ulDllRecCntr % g_ulChannelSearchTime) )                            // but listen for 400ms on ChannelMap
    {
      PHY_Disable_Request(PHY_IDLE); 
      // reversed channel sequence
      s_ucActiveChannel = g_ucDllActiveChannels - 1 - ( (g_ulDllRecCntr / g_ulChannelSearchTime) % g_ucDllActiveChannels );
      s_ucActiveChannel = DLL_GetChannel(s_ucActiveChannel);                
    }        
    if ( IS_PHY_IDLE() )
    {
      PHY_RX_Request( s_ucActiveChannel, 0, 0);
    }  
    g_ulDllRecCntr++;
#ifdef HARDCODE_JOINED_DEVICE
//    if (g_ucDllStatus == DLL_ACTIVE_SEARCH && g_ulDllRecCntr >= 2000 )
//    {
//      JSR_START();    // software reset        
//    }
#else
    if ((g_ucDllStatus == DLL_ACTIVE_SEARCH) && 
        (g_ulDllRecCntr >= g_ulDllMaxRecCntr))
    {
      g_ulDllMaxRecCntr = g_ulPassiveCycleTime;
      PHY_Disable_Request(PHY_IDLE);
      PUT_DLL_PASSIVE_SEARCH(); 
      UART1_String("DSpasS");                                                 // DLL Status - passive search
    }      
#endif
#endif
    break;
  case DLL_PASSIVE_SEARCH:
    g_ulDllRecCntr++;
    if(g_ulDllRecCntr >= g_ulDllMaxRecCntr)
    {
      g_ulDllMaxRecCntr = g_ulPassiveWakeTime;
      PUT_DLL_ACTIVE_SEARCH(); 
      UART1_String("DSactS");                                                 // DLL Status - active search
    }
    break;     
  case DLL_JOINED:          
    DLL_ComputeSuperframeOffset();
    DLL_Timeslot_Start();
    break;
  }
}



void TMR_Interrupt(void)
{
  if (ITC.IntFrc & (1 << gTmrInt_c)) // every 250 ms
  {
      DEBUG1_ON(); 
      ITC_TestReset(gTmrInt_c);     // forces deassertion for one interrupt flag in IntFrc register            
      
      RtcTmr_Set250msCorrection( g_nCrtClkCorrection );
      
      DLL_TIMER_STOP();             // recompute TMR0
      
//      DLL_TIMER_SET_CNTR(144);      // 24 uSec prelucrarea pana aici
      
      TMR0_CNTR = RTC_TO_TMR0( RtcTmr_Get250msOffset() ) % 60000 ;    // rtc tmr to dll tmr
      
      TMR0_CSCTRL_TCF1 = 0; 
      
      DLL_TIMER_START();
      
      // Debug pulse
      g_stDllPredSlot.m_uc250msSlotNo = 0;      
      TMR_DLL_Function();                             
      g_uc250msFlag = 1;
      
      while( CRM_STATUS_RTC_WU_EVT )  // wait for RTC clock 2 cycles
      {
          CRM_STATUS_RTC_WU_EVT = 1;  // clear RTC flag (make sense for debug purpose only - step by step debugging)
      }

      ITC.IntEnNum = gCrmInt_c;
      DEBUG1_OFF();
  }
  
  // DLL
  if ( TMR0_CSCTRL_TCF1 )
  {
    TMR0_CSCTRL_TCF1 = 0;      
    if  ( g_stDllPredSlot.m_uc250msSlotNo < MAX_250MS_SLOT_NO - 1 )
    {      
      DEBUG1_ON(); // Signal just for Debug
      // Debug pulse
      g_stDllPredSlot.m_uc250msSlotNo++;      
      TMR_DLL_Function();               
      DEBUG1_OFF(); // Signal just for Debug
    }
  }
  
  // RTC
  if ( TMR1_CSCTRL_TCF1 )
  {
    TMR1_CSCTRL_TCF1 = 0;

    g_uc10msFlag = 1;
  }  
}

