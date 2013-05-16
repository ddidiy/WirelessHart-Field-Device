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
* Name:         crm.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_CRM_H_
#define _NIVIS_CRM_H_

#include "LibInterface/EmbeddedTypes.h"
#include "LibInterface/Crm.h"

#undef TIMER_WU_EN 
#undef RTC_WU_EN

#undef BUCK_EN
#undef BUCK_SYNC_REC_EN
#undef BUCK_BYPASS_EN
#undef VREG_1P5V_EN
#undef VREG_1P8V_EN

#include "mc1322x.h"

// *********************  WATCH DOG TIMER **************************************
// ATENTION ! BE CAREFULLY AT WATCHDOG WHEN IMPLEMENT SLEEP MODE ( if MCU_RET = 0 or 1 ! )

#define WDT_2_SEC_TIMEOUT       (22)  // =>  (THIS_VALUE + 1) * ( (2^21) / 24.000.000 ) // => 2.0097706667 seconds

#define DISABLE_WDT()           { CRM_COP_CNTL_COP_EN = 0; }
#define ENABLE_WDT()            { CRM_COP_CNTL_COP_EN = 1; }
#define FEED_WDT()              { COP_SERVICE = 0xC0DE5AFE; }
#define SET_WDT_TIMEOUT()       { CRM_COP_CNTL_COP_TIMEOUT = WDT_2_SEC_TIMEOUT; }
#define SET_WDT_WPROTECTION()   { CRM_COP_CNTL_COP_WP = 1; }

#define WDT_INIT() { \
                        DISABLE_WDT();     \
                        SET_WDT_TIMEOUT(); \
                        ENABLE_WDT();      \
                        FEED_WDT();        \
                    }
// *********************  WATCH DOG TIMER **************************************


typedef uint8   bool;

extern crmErr_t CRM_VRegCntl(crmVRegCntl_t* pVRegCntl);

enum
{
  CK_LOAD_MODEM_IRQ = 0,
  CK_LOAD_TIMER_IRQ,
  CK_LOAD_TASKS,
  
  CK_LOAD_STRUCT_NO
};

// -----------------------------  CHECK LOAD SUPPORT ---------------------------
#ifdef CK_LOAD_SUPPORT
  #include "timers.h"

  typedef struct 
  {
      uint16 m_unCrtCounter;
      uint16 m_unLastLoad;
      uint16 m_unMaxLoad;
      uint16 m_unAvgCounter;
      uint32 m_ulAvgAcc;      
  } CK_LOAD_STRUCT;
  
  extern CK_LOAD_STRUCT g_aCkLoadTable[CK_LOAD_STRUCT_NO];
  
  #define CK_LOAD_Start(idx) g_aCkLoadTable[idx].m_unCrtCounter = CK_LOAD_TMR_COUNTER
  void CK_LOAD_Stop(uint8 p_unIdx);   
  
#else
  
  #define CK_LOAD_Start(idx) 
  #define CK_LOAD_Stop(idx)   
  
#endif // CK_LOAD_SUPPORT


#define  _SLEEP_TEST_

#define BEGIN_MAIN (void *)0x00400080         /* appl´s start-up code address */

#define JSR(x) (*((void(*)(void))x))()
#define JSR_MAIN() JSR(BEGIN_MAIN)
  
#define CLK_250MS_32KHZ    (32*1024/4-2)
  
extern uint8  gWakedUp;   /* this is a flag for wake-up from DOZE */
extern uint32 gRegister;  /* this variable is used to update the system clock */
extern uint32 g_ul250msStartTmr;  /*this variable is used to memorise the moment of the start of a 250ms interval */
extern uint16 g_unRtc250msDuration;
extern uint32 g_ul250msEndTmr;   // this variable is used to memorise the moment of the end of a 250ms interval 

void CRM_Init (void);
  

#if ( SPI1_MODE != NONE ) && defined( WAKEUP_ON_EXT_PIN ) // wake up for SPI interface
  extern uint32 g_ulLastCrmStatus;
  uint8 CRM_ResetWakeUpFlag(void);
#endif  

#endif /* _NIVIS_CRM_H_ */

