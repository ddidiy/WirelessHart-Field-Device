/************************************************************************************
* Public header file for the Timer driver.
*
* Author(s): B04839 
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
Revision history:
   date                        Author                                 Comments
   ----------               ----------------------                    -------
   03.07.2006                  B04839                                 Created
************************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#include "Platform.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#define CHANNEL_OFFSET  0x05
#define CHANNEL_MASK    0x03

/*write value to register macros*/ 
#define SetComp1Val(tmrNumber, val)      ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Comp1 = (val)
#define SetComp2Val(tmrNumber, val)      ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Comp2 = (val)
#define SetCaptureVal(tmrNumber, val)    ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Capt = (val)
#define SetLoadVal(tmrNumber, val)       ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Load = (val)
#define SetHoldVal(tmrNumber, val)       ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Hold = (val)
#define SetCntrVal(tmrNumber, val)       ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Cntr = (val)
#define SetCompLoad1Val(tmrNumber, val)  ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->CmpLd1 = (val)
#define SetCompLoad2Val(tmrNumber, val)  ((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->CmpLd2 = (val)
/*read value from registers macros*/
#define GetComp1Val(tmrNumber)           (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Comp1)
#define GetComp2Val(tmrNumber)           (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Comp2)
#define GetCaptureVal(tmrNumber)         (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Capt)
#define GetLoadVal(tmrNumber)            (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Load)
#define GetHoldVal(tmrNumber)            (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Hold)
#define GetCntrVal(tmrNumber)            (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->Cntr)
#define GetCompLoad1Val(tmrNumber)       (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->CmpLd1)
#define GetCompLoad2Val(tmrNumber)       (((TmrRegs_t *)(MBAR_TMR + (((tmrNumber) & CHANNEL_MASK) << CHANNEL_OFFSET)))->CmpLd2)

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/** This data type enumerates the Timer API calls return values */
typedef enum {
  gTmrErrNoError_c = 0,                 
  gTmrErrTimerIsEn_c,
  gTmrErrTimerIsDis_c,   
  gTmrErrNullPointer_c,    
  gTmrErrInvalidParameter_c,    
  gTmrErrTimerBusy_c,
  gTmrErrMax_c
} TmrErr_t;

/** This data type enumerates the possible values for the timer number */
typedef enum {
  gTmr0_c = 0,
  gTmr1_c,
  gTmr2_c,
  gTmr3_c,
  gTmrMax_c
} TmrNumber_t;

/** This data type enumerates the possible values of the count mode */
typedef enum {
  gTmrNoOperation_c = 0,
  gTmrCntRiseEdgPriSrc_c ,
  gTmrCntRiseEdgFallEdgPriSrc_c,
  gTmrCntRiseEdgWhileSecInputHighActive_c,
  gTmrQuadCnt_c,
  gTmrCntPriSrcRiseEdgSecSrcSpecDir_c,
  gTmrEdgSecSrcTriggerPriCntTillComp_c,
  gTmrSyncCnt_c,
  gTmrModeMax_c
} TmrMode_t;

/** This data type enumerates the possible values for output mode */
typedef enum {
  gTmrAssert_c = 0,                 
  gTmrClearOF_c,                 
  gTmrSetOF_c,
  gTmrToggleOF_c,
  gTmrToggleOFUsingAlternateReg_c,
  gTmrSetOnCompClearOnSecInputEdg_c,
  gTmrSetOnCompClearOnRollover_c,
  gTmrEnGateClock_c,  
  gTmrOutputModeMax_c
} TmrOutputMode_t;

/** This data type enumerates the possible values for primary count source */
typedef enum {
  gTmrPrimaryCnt0Input_c = 0,                 
  gTmrPrimaryCnt1Input_c,                 
  gTmrPrimaryCnt2Input_c,
  gTmrPrimaryCnt3Input_c,
  gTmrPrimaryCnt0Output_c,
  gTmrPrimaryCnt1Output_c,
  gTmrPrimaryCnt2Output_c,
  gTmrPrimaryCnt3Output_c,
  gTmrPrimaryClkDiv1_c,
  gTmrPrimaryClkDiv2_c,
  gTmrPrimaryClkDiv4_c,
  gTmrPrimaryClkDiv8_c,
  gTmrPrimaryClkDiv16_c,
  gTmrPrimaryClkDiv32_c,
  gTmrPrimaryClkDiv64_c,
  gTmrPrimaryClkDiv128_c,
  gTmrPrimaryCntSrcMax_c
} TmrPrimaryCntSrc_t;

/** This data type enumerates the values for the secondary count sorce */
typedef enum {
  gTmrSecondaryCnt0Input_c = 0,                 
  gTmrSecondaryCnt1Input_c,                 
  gTmrSecondaryCnt2Input_c,
  gTmrSecondaryCnt3Input_c,
  gTmrSecondaryCntSrcMax_c
} TmrSecondaryCntSrc_t;

/** This data type enumerates the values for timer events*/
typedef  enum {
  gTmrComp1Event_c = 0,
  gTmrComp2Event_c,
  gTmrCompEvent_c,
  gTmrOverEvent_c,
  gTmrEdgeEvent_c,
  gTmrMaxEvent
} TmrEvent_t;
  
/** This data type describes the configuration of the Timer module .*/
typedef struct TmrConfig_tag{
  TmrOutputMode_t       tmrOutputMode;
  bool_t                tmrCoInit;
  bool_t                tmrCntDir;
  bool_t                tmrCntLen;
  bool_t                tmrCntOnce;
  TmrSecondaryCntSrc_t  tmrSecondaryCntSrc;
  TmrPrimaryCntSrc_t    tmrPrimaryCntSrc;
} TmrConfig_t;

/** This data type describes the status and control of the Timer module.*/
typedef union TmrStatusCtrl_tag{
  struct{
    uint16_t OEN:1;
    uint16_t OPS:1;
    uint16_t FORCE:1;
    uint16_t VAL:1;
    uint16_t EEOF:1;
    uint16_t MSTR:1;
    uint16_t CAPMODE:2;
    uint16_t INPUT:1;
    uint16_t IPS:1;
    uint16_t IEFIE:1;
    uint16_t IEF:1;
    uint16_t TOFIE:1;
    uint16_t TOF:1;
    uint16_t TCFIE:1;
    uint16_t TCF:1;
  } bitFields;
  uint16_t uintValue;
} TmrStatusCtrl_t;

/** This data type describes the comparator status and control of the Timer module.*/
typedef union TmrComparatorStatusCtrl_tag{
  struct{
    uint16_t CL1:2;
    uint16_t CL2:2;
    uint16_t TCF1:1;
    uint16_t TCF2:1;
    uint16_t TCF1EN:1;
    uint16_t TCF2EN:1;
    uint16_t RESERVED:5;
    uint16_t FILT_EN:1;
    uint16_t DBG_EN:2;
  } bitFields;
  uint16_t uintValue;
} TmrComparatorStatusCtrl_t;

/** This data type lists the callback functions for the Timer module driver*/
typedef void (*TmrCallbackFunction_t)(TmrNumber_t tmrNumber);

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*================================================================================================*/
/*===== TmrInit =====*/
/**
@brief The function is called to init timer module.

@param  

@return 
 
 Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/  
/*===============================================================================================*/
extern void TmrInit(void);

/*================================================================================================*/
/*===== TmrEnable =====*/
/**
@brief The function is called to enable corresponding  timer module.

@param  TmrNumber_t tmrNumber - number of the timer instance to enable

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrTimerAlreadyEnabled_c
 
 Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/  
/*===============================================================================================*/
extern TmrErr_t TmrEnable(TmrNumber_t tmrNumber);

/*================================================================================================*/
/*===== TmrDisable =====*/
/**
@brief The function is called to disable corresponding  timer module.

@param  TmrNumber_t tmrNumber               - number of the timer instance to enable

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrTimerAlreadyDisabled_c
 
 Revision history:
   date                              Author                           Comments
   ----------               ----------------------                    -------
   03.07.2006                        B04839                           Created
*/
/*===============================================================================================*/
extern TmrErr_t TmrDisable(TmrNumber_t tmrNumber);

/*================================================================================================*/
/*===== TmrSetMode =====*/
/**
@brief The function is called to set the working mode for the Timer peripheral specified as parameter.

@param  TmrNumber_t tmrNumber               - number of the timer instance to set working mode
@param   TmrMode_t tmrMode                  - timer working mode

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrTimerNotEnabled_c    
          gTmrErrTimerBusy_c
 
 Revision history:
   date                              Author                           Comments
   ----------               ----------------------                    -------
   03.07.2006                        B04839                           Created
*/
/*===============================================================================================*/
extern TmrErr_t TmrSetMode(TmrNumber_t tmrNumber, TmrMode_t tmrMode);

/*================================================================================================*/
/*===== TmrSetConfig =====*/
/**
@brief  The function is called to set the parameters for the Timer peripheral.

@param  TmrNumber_t tmrNumber               - number of the timer instance to enable
@param    TmrConfig_t* pConfig              - input parameter: pointer to a structure containing the configuration 
                                              parameters.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabled_c
          gTmrErrNoCallbackDefined

 Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/
/*================================================================================================*/
extern TmrErr_t TmrSetConfig(TmrNumber_t tmrNumber, TmrConfig_t* pconfig);

/*================================================================================================*/
/*===== TmrSetStatusControl =====*/
/**
@brief  This function is called to set the level of the Rx FIFO threshold.

@param  TmrNumber_t tmrNumber               - input parameter: number of the timer instance to configurate.
@param  TmrStatusCtrl_t *pStatusCtrl        - input parameter: pointer to a structure containing the control parameters.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabled_c
    
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrSetStatusControl(TmrNumber_t tmrNumber, TmrStatusCtrl_t* pStatusCtrl);

/*================================================================================================*/
/*===== TmrSetComparatorStatusControl =====*/
/**
@brief  The function is called to set the comparator configuration of the timer peripheral instance.

@param  uint8_t  tmrNumber                         - input parameter: number of the timer instance to configurate comparator.
@param  TmrComparatorStatusCtrl_t* pCompStatusCtrl - input parameter: pointer to a structure where the comparator configuration 
                                                     parameters shall be placed.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c 
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabled_c
  
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrSetCompStatusControl(TmrNumber_t tmrNumber, TmrComparatorStatusCtrl_t* pCompStatusCtrl);

/*================================================================================================*/
/*===== TmrSetCallbackFunction =====*/
/**
@brief  This function will be called to set the callback functions for the timer driver.

@param  TmrNumber_t tmrNumber               - input parameter: number of the Timer instance to set the callback function.
@param  TmrCallbackFunction_t *pCallback    - input parameter: pointer to a structure containing the pointer to the callback 
                                              function.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c 
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabed_c
   
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrSetCallbackFunction(TmrNumber_t tmrNumber, TmrEvent_t tmrEvent, TmrCallbackFunction_t pCallbackFunc);

/*================================================================================================*/
/*===== TmrWriteValue =====*/
/**
@brief  The function is called to write a new value to the timer register to be used as the base for its next count.

@param  uint8_t  tmrNumber                  - input parameter: number of the Timer instance to write to.
@param  uint16_t value                      - input parameter: value to be used as the base for the next count.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrTimerNotEnabled_c
   
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrWriteValue( TmrNumber_t tmrNumber, uint16_t value);

/*================================================================================================*/
/*===== TmrGetMode =====*/
/**
@brief  The function is called to get the working mode of the timer peripheral specified as parameter. 

@param  uint8_t  tmrNumber                  - input parameter: number of the timer instance to get configuration from.
@param   TmrMode_t *tmrMode                 - input parameter: pointer to a structure where the timer working mode shall 
                                              be placed.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrTimerNotEnabled_c
          gTmrErrNullPointer_c
    
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrGetMode(TmrNumber_t tmrNumber, TmrMode_t *pTmrMode);

/*================================================================================================*/
/*===== TmrGetConfig =====*/
/**
@brief  The function is called to get the configuration of the timer peripheral specified as parameter. 

@param  uint8_t  tmrNumber                  - input parameter: number of the timer instance to get working mode from.
@param  TmrConfig_t *pConfig                - input parameter: pointer to a structure where the configuration parameters 
                                              shall be placed.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabled_c
    
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrGetConfig(TmrNumber_t tmrNumber, TmrConfig_t* pconfig);

/*================================================================================================*/
/*===== TmrGetStatusControl =====*/
/**
@brief  The function is called to get the status of the timer peripheral instance.

@param  uint8_t  tmrNumber                  - input parameter: number of the timer instance to get status from.
@param  TmrStatusCtrl_t *pStatusCtrl        - input parameter: pointer to a structure where the status parameters shall 
                                              be placed.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c 
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabled_c
    
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrGetStatusControl(TmrNumber_t tmrNumber, TmrStatusCtrl_t* pStatusCtrl);

/*================================================================================================*/
/*===== TmrGetComparatorStatusControl =====*/
/**
@brief  The function is called to get the comparator status of the timer peripheral instance.

@param  uint8_t  tmrNumber                         - input parameter: number of the timer instance to get comparator status.
@param  TmrComparatorStatusCtrl_t* pCompStatusCtrl - input parameter: pointer to a structure where the comparator status 
                                                     parameters shall be placed.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c 
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabled_c
  
Revision history:
   date                         Author                                Comments
   ----------               ----------------------                    -------
   03.07.2006                   B04839                                Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrGetCompStatusControl(TmrNumber_t tmrNumber, TmrComparatorStatusCtrl_t* pCompStatusCtrl);

/*================================================================================================*/
/*===== TmrReadValue =====*/
/**
@brief  The function is called to read a new value from the timer register.

@param  uint8_t  tmrNumber                  - input parameter: number of the Timer instance to read from
@param  uint16_t *value                     - input parameter: memory place where the new value from timer register 
                                              will be placed.

@return parameter of type TmrErr_t with following possible values (they are self-explaining):

          gTmrErrNoError_c
          gTmrErrInvalidParameter_c
          gTmrErrNullPointer_c
          gTmrErrTimerNotEnabled_c

Revision history:
   date                         Author                                  Comments
   ----------               ----------------------                      -------
   03.07.2006                   B04839                                  Created
*/   
/*================================================================================================*/
extern TmrErr_t TmrReadValue(TmrNumber_t tmrNumber,  uint16_t* pvalue);    

/*================================================================================================*/
/*===== TmrIsr =====*/
/**
@brief  Timer interrupt service routine.

@param  None

@return None

Revision history:
      date                          Author                            Comments
   ----------              ----------------------                     -------
   03.07.2006                       B04839                            Created
*/   
/*================================================================================================*/
extern void TmrIsr(void);

#endif  /* _TIMER_H_ */
