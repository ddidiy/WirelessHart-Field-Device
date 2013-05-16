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
* Name:         itc.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_ITC_H_
#define _NIVIS_ITC_H_

#include "typedef.h"
#include "global.h"
#include "MC1322x.h"
#include "maca.h"
#include "timers.h"
#include "LibInterface/ITC_Interface.h"

extern void InterruptInit(void);
extern void IntRestoreFIQ(unsigned int);  // 1 - int disabled, 0 - int enabled
extern void IntRestoreIRQ(unsigned int);  // 1 - int disabled, 0 - int enabled
extern void IntRestoreAll(unsigned int);  // 0 - all enabled, 1 - FIQ disabled/ IRQ enabled, 2 - FIQ enabled/ IRQ disabled, 3 - all disabled
extern unsigned int IntDisableIRQ(void);  // 1 - int disabled, 0 - int enabled
extern unsigned int IntDisableFIQ(void);  // 1 - int disabled, 0 - int enabled
extern unsigned int IntDisableAll(void);  // 0 - all enabled, 1 - FIQ disabled/ IRQ enabled, 2 - FIQ enabled/ IRQ disabled, 3 - all disabled

#define IntEnableFIQ()         IntRestoreFIQ ( 0 ) 
#define IntEnableIRQ()         IntRestoreIRQ ( 0 )
#define IntEnableAll()         IntRestoreAll ( 0 ) 

#define IRQ_STATE_ENABLED  0
#define IRQ_STATE_DISABLED 1

////////////////////////////// NOTES //////////////////////////////////////
//
// Exceptions are always handled in ARM state.
// When the processor is in Thumb state and an exception occurs,
// the switch to ARM state takes place automatically when the exception vector address is loaded into the PC.
//
// See it on
// book : MC13224/225 Reference Manual, Rev. 0.0
// file : MC1322x_Reference_Manual.pdf
// capt : 7.9.2 Entering an Exception
// pag  : 7-13; 175 in pdf file
//
// My observation :
// we are in Thumb mode
// so we will use the __arm directive to tell compiler
// to compile Exceptions Handlers in Arm Mode
//
///////////////////////////////////////////////////////////////////////////


////////////////////////////// Interrupts //////////////////////////////////////
// These defines reflects the interrupts priority in MC 1322x chip
// to full understanding of this priority system in this microprocessor see it on
// book : MC13224/225 Reference Manual, Rev. 0.0
// file : MC1322x_Reference_Manual.pdf
// capt : 8.7 ITC Prioritization of Interrupt Sources
// pag  : 8-17; 197 in pdf file

// Interrupt source priorities
enum
{
  ASM_INT,
  UART1_INT,
  UART2_INT,
  CRM_INT,
  I2C_INT,
  TMR_INT,
  SPIF_INT,
  MACA_INT,
  SSI_INT,
  ADC_INT,
  SPI_INT,
  TOTAL_NUMBER_INT
};

// Interrupt type priorities
enum
{
  NORMAL_INT,
  FAST_INT
};


// these function
__arm void ItcInit(void);


#define ITC_ENABLE_INTERRUPT(Irq)   { ITC_INTENNUM = (Irq);   }
#define ITC_DISABLE_INTERRUPT(Irq)  { ITC_INTDISNUM = (Irq);  }
#define IS_ITC_INT_ENABLE(Irq)      ( ( ITC_INTENABLE  >> (Irq) ) & 1 )
#define IS_ITC_INT_FLAG(Irq)        ( ( ITC_INTSRC     >> (Irq) ) & 1 )

#endif /* _NIVIS_ITC_H_ */

