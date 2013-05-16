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
* Name:         timers.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_TIMERS_H_
#define _NIVIS_TIMERS_H_

#include "typedef.h"
#include "global.h"
#include "MC1322x.h"
#include "digitals.h"
#include "uart1.h"
#include "crm.h"
 
#include "LibInterface/Timer.h"

extern volatile unsigned char g_uc10msFlag;
extern volatile unsigned char g_uc250msFlag;

//.#define DISABLE_DLL_TMRIRQ()  {ITC_DISABLE_INTERRUPT(TMR_INT); ITC_DISABLE_INTERRUPT(MACA_INT);}
//.#define ENABLE_DLL_TMRIRQ()   {ITC_ENABLE_INTERRUPT(MACA_INT); ITC_ENABLE_INTERRUPT(TMR_INT);}
//..#define DISABLE_DLL_TMRIRQ()  MONITOR_ENTER()
//..#define ENABLE_DLL_TMRIRQ()   MONITOR_EXIT()

#define RTC_32KHZ_250MS_CLK   (8192)                               // =32*1024/4
#define RTC_32KHZ_250MS_TICS  (RTC_32KHZ_250MS_CLK-2)              // =32*1024/4-2; 2 clk for crm
#define MAX_BBR_CORRECTION    1                                    // 1 step = (1/32768)s at 250 ms !!! ( 120 uS at S )


#define DllTmr_GetSlotOffset()      TMR0_CNTR

extern int16 g_nCrtClkCorrection;

inline void RtcTmr_Set250msCorrection(int16 p_nCorrection)
{      
    if( p_nCorrection <= -1 )  
    {      
        g_nCrtClkCorrection = p_nCorrection+1;
        g_unRtc250msDuration = RTC_32KHZ_250MS_TICS + 1; 
    }        
    else if( p_nCorrection >= 1 ) 
    {
        g_nCrtClkCorrection = p_nCorrection-1;
        g_unRtc250msDuration = RTC_32KHZ_250MS_TICS - 1; 
    }
    else                         
    {
        g_nCrtClkCorrection = 0;
        g_unRtc250msDuration = RTC_32KHZ_250MS_TICS; 
    }
}

__arm inline void RtcTmr_SetCounter(uint16 p_unCntr)
{
  if( p_unCntr < (RTC_32KHZ_250MS_TICS + 1) )
  {
    p_unCntr = RTC_32KHZ_250MS_TICS + 1 - p_unCntr;
    RTC_TIMEOUT = p_unCntr;
    g_ul250msEndTmr = RTC_COUNT + p_unCntr;              
  }
}  

__arm inline uint32 RtcTmr_Get250msOffset(void) 
{
    return (RTC_COUNT - g_ul250msStartTmr);
}


// TMR0 ==> TIMER for DLL = one pulse at 10 ms 
// 1 sec = 24.000.000 => 10 ms = 240.000
// 24.000.000 / 4 = 6.000.000 => 6.000.000 / 60.000 = 100 pulses per second => 10 ms 

#define TMR0_INIT() {  \
  TMR0_CTRL   = 0x1420; \
  TMR0_SCTRL  = 0x0000; \
  TMR0_LOAD   = 0x0000; \
  TMR0_COMP1  =  60000-1; \
  TMR0_CSCTRL = 0x0040; \
}

// Start timer 
#define DLL_TIMER_START()           { TMR0_CTRL_COUNT_MODE = 1; }
#define DLL_TIMER_CNTR                TMR0_CNTR 
#define DLL_TIMER_SET_CNTR(x)       { TMR0_CNTR = (x); }
#define DLL_TIMER_STOP()            { TMR0_CTRL_COUNT_MODE = 0; }
#define DLL_TIMER_START_FROM_ZERO() { DLL_TIMER_SET_CNTR(0); DLL_TIMER_START(); }



// TMR1 ==> TIMER for RTC = one pulse at 10 ms
// 1 sec = 24.000.000
// 10 ms = 240.000
// ( 240.000 / 4 = 60.000 ) or ( 24.000.000 / 4 = 6.000.000 => 6.000.000 / 60.000 = 100 pulses per second => 10 ms )
#define TMR1_INIT() {   \
  TMR1_CTRL   = 0x1420; \
  TMR1_SCTRL  = 0x0000; \
  TMR1_LOAD   = 0x0000; \
  TMR1_COMP1  =  60000-1; \
  TMR1_CSCTRL = 0x0040; \
}


// Start timer for RTC
#define TMR1_START() { TMR1_CNTR = 0x00; TMR1_CTRL_COUNT_MODE = 1; }


// TMR2 ==> TIMER for HART
//copy of TMR1 but 10 times faster
#define TMR2_INIT() {  \
  TMR2_CTRL   = 0x1420; \
  TMR2_SCTRL  = 0x0000; \
  TMR2_LOAD   = 0x0000; \
  TMR2_COMP1  =   6000; \
  TMR2_CSCTRL = 0x0040; \
}
// Start timer for HART
#define TMR2_START() { TMR2_CNTR = 0x00; TMR2_CTRL_COUNT_MODE = 1; }

// TMR3 ==> TIMER for debug and measureaments = rollover at 349,519(999) miliS and not raise interrupts
// 1 sec = 24.000.000
// 24.000.000 / 128 = 187.500 => 5,(333) uS
// 0xFFFF = 65535 * 5,(333) uS = 349,519(999) miliS
#define TMR3_INIT() {   \
  TMR3_CTRL   = 0x1E00; \
  TMR3_SCTRL  = 0x0000; \
  TMR3_LOAD   = 0x0000; \
  TMR3_COMP1  = 0xFFFF; \
  TMR3_CSCTRL = 0x0000; \
}
// Start timer for debug and measureaments
#define TMR3_START() { TMR3_CNTR = 0x00; TMR3_CTRL_COUNT_MODE = 1; }
#define CK_LOAD_TMR_COUNTER   ( TMR3_CNTR )

#define STARTUP_SEG (void *)0x400000  
#define JSR(x) (*((void(*)(void))x))()
#define JSR_START() JSR(STARTUP_SEG)    // call JSR_START() to make a software reset

void TMR_Init(void);
void TMR_Interrupt(void);


#endif /* _NIVIS_TIMERS_H_ */

