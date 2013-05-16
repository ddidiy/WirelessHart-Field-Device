/************************************************************************************
* Header file for AITC Driver
*
* Author(s): R04473
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/

#ifndef _ITC_INTERFACE_H_
#define _ITC_INTERFACE_H_

#include "platform.h"

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef void (*IntHandlerFunc_t)(void);

// Interrupt priorities (Normal / Fast)
typedef enum {
    gItcNormalPriority_c,
    gItcFastPriority_c
} ItcPriority_t;


//  Error codes
typedef enum {
    gItcErr_OK_c,
    gItcErr_InvalidNumber_c,
    gItcErr_InvalidPriority_c
} ItcErr_t;

////////////////////////////// Interrupt numbers //////////////////////////////////
typedef enum {
    gAsmInt_c,
    gUart1Int_c,
    gUart2Int_c,
    gCrmInt_c,
    gI2cInt_c,
    gTmrInt_c,
    gFlashInt_c,
    gMacaInt_c,
    gSsiInt_c,
    gAdcInt_c,
    gSpiInt_c,
    gMaxInt_c,
    gUndefinedException_c,
    gSupervisorException_c,
    gPrefetchAbortException_c,
    gDataAbortException_c,
    gMaxException_c
} ItcNumber_t;


#define gNoneInt_c    gMaxInt_c


/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
/************************************************************************************
* Macro:        ITC_TestSet
*
* Description:  This macro forces assertion for one interrupt flag in IntFrc register
*
* Parameters:    IntNumber_t  Number  - interrupt number corresponding to a peripheral device
*   
* Return value:  ItcErr_t
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
#define ITC_TestSet(Number)          ITC.IntFrc |= (1 << (Number))

/************************************************************************************
* Macro:        ITC_TestReset
*
* Description:  This macro forces deassertion for one interrupt flag in IntFrc register
*
* Parameters:    IntNumber_t  Number  - interrupt number corresponding to a peripheral device
*   
* Return value:  ItcErr_t
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
#define ITC_TestReset(Number)        ITC.IntFrc &= ~(1 << (Number))

/************************************************************************************
* Macro:        ITC_GetIntEnable
*
* Description:  This macro returns the content of INTENABLE register
*
* Parameters:    IntNumber_t  Number  - interrupt number corresponding to a peripheral device
*   
* Return value:  unsigned int  - LSB is set accordingly with the designed bit by Number parameter
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
#define ITC_GetIntEnable(Number)     ((ITC.IntEnable >> (Number)) & 1)

/************************************************************************************
* Macro:         ITC_GetFastPending
*
* Description:  This macro returns the content of FIPEND register
*
* Parameters:    IntNumber_t  Number - interrupt number corresponding to a peripheral device
*   
* Return value:  unsigned int - LSB is set accordingly with the designed bit by Number parameter
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
#define ITC_GetFastPending(Number)   ((ITC.FiPend >> (Number)) & 1)

/************************************************************************************
* Macro:         ITC_GetNormalPending
*
* Description:  This macro returns the content of NIPEND register
*
* Parameters:    IntNumber_t  Number  - interrupt number corresponding to a peripheral device
*   
* Return value:  unsigned int  - LSB is set accordingly with the designed bit by Number parameter
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
#define ITC_GetNormalPending(Number) ((ITC.NiPend >> (Number)) & 1)

/************************************************************************************
* Macro:        ITC_GetIntSrc
*
* Description:  This macro returns the content of INTSRC register
*
* Parameters:    IntNumber_t Number  - interrupt number corresponding to a peripheral device
*   
* Return value:  unsigned int  - LSB is set accordingly with the designed bit by Number parameter
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
#define ITC_GetIntSrc(Number)        ((ITC.IntSrc >> (Number)) & 1)

/************************************************************************************
* Macro:        IntRemoveHandler
*
* Description:  This macro removes an IRQ handler
*
* Parameters:    ItcNumber_t      Number        - interrupt number, max. value is gMaxException_c - 1
*   
* Return value:  error code
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
#define IntRemoveHandler(x)  IntAssignHandler((x), NULL)



/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Function:    ITC_Init
*
* Description:  This function initilizes the ARM Interrupt Control module hardware.
*
* Parameters:  None
*   
* Return value:  None
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
extern void ITC_Init(void);

/************************************************************************************
* Function:    IntAssignHandler
*
* Description:  This function assigns a function to an IRQ number
*
* Parameters:  IntHandlerFunc_t pfIrqHandler  - pointer to interrupt handler
*             ItcNumber_t      Number        - interrupt number, max. value is gMaxException_c - 1
*   
* Return value:  error code
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
extern ItcErr_t IntAssignHandler(ItcNumber_t Number, IntHandlerFunc_t pfIrqHandler);

/************************************************************************************
* Function:    IntGetHandler
*
* Description:  This function returns the handler for an IRQ number. If there is none
*               assigned, it will return NULL.
*
* Parameters:   ItcNumber_t Number    - interrupt number, max. value is gMaxException_c - 1
*   
* Return value:  IntHandlerFunc_t       - pointer to interrupt handler or NULL if any error
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   151007     GB      Created
* 
************************************************************************************/
extern IntHandlerFunc_t IntGetHandler(ItcNumber_t Number);

/************************************************************************************
* Function:    ITC_SetPriority
*
* Description:  This function sets the priority to an IRQ number
*
* Parameters:  ItcNumber_t      Number        - interrupt number, max. value is gDataAbortException_c (gMaxInt_c - 1)
*             ItcPriority_t    Priority      - NormalInt or FastInt for routing to 
*                                             IRQ or FIQ into ARM7TDMI core
*   
* Return value:  error code
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
extern ItcErr_t ITC_SetPriority(ItcNumber_t Number, ItcPriority_t Priority);

/************************************************************************************
* Function:    ITC_EnableInterrupt
*
* Description:  This function enables the interrupt corresponding to IrqNum
*
* Parameters:  ItcNumber_t Number  - interrupt number corresponding to a peripheral device
*   
* Return value:  ItcErr_t
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
extern ItcErr_t ITC_EnableInterrupt(ItcNumber_t Number);

/************************************************************************************
* Function:    ITC_DisableInterrupt
*
* Description:  This function disables the interrupt corresponding to IrqNum
*
* Parameters:  IntNumber_t  Number  - interrupt number corresponding to a peripheral device
*   
* Return value:  ItcErr_t
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
extern ItcErr_t ITC_DisableInterrupt(ItcNumber_t Number);

/************************************************************************************
* Function:    ITC_SetIrqMinimumLevel
*
* Description:  This function sets the normal interrupt level mask register - NIMASK
*
* Parameters:  IntNumber_t  Number  - interrupt number corresponding to a peripheral device
*                                    set to gNoneInt_c to disable this feature
*   
* Return value:  ItcErr_t
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   110406     GB      Created
* 
************************************************************************************/
extern ItcErr_t ITC_SetIrqMinimumLevel(ItcNumber_t Number);


#endif /* _ITC_INTERFACE_H_ */


