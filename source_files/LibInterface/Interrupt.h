/************************************************************************************
*
* (c) Copyright 2007, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "ITC_Interface.h"

#define IntEnableFIQ()         IntRestoreFIQ(0)
#define IntEnableIRQ()         IntRestoreIRQ(0)
#define IntEnableAll()         IntRestoreAll(0)

extern void InterruptInit(void);
extern void IntRestoreFIQ(unsigned int);  // 1 - int disabled, 0 - int enabled
extern void IntRestoreIRQ(unsigned int);  // 1 - int disabled, 0 - int enabled
extern void IntRestoreAll(unsigned int);  // 0 - all enabled, 1 - FIQ disabled/ IRQ enabled, 2 - FIQ enabled/ IRQ disabled, 3 - all disabled
extern unsigned int IntDisableIRQ(void);  // 1 - int disabled, 0 - int enabled
extern unsigned int IntDisableFIQ(void);  // 1 - int disabled, 0 - int enabled
extern unsigned int IntDisableAll(void);  // 0 - all enabled, 1 - FIQ disabled/ IRQ enabled, 2 - FIQ enabled/ IRQ disabled, 3 - all disabled

#endif /* _INTERRUPT_H_ */

