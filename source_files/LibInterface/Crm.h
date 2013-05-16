/************************************************************************************
* Header file for Crm Driver
*
* Author(s): Dragos Musoiu
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
* Revision history:
*   date                     Author                       Comments
*   ----------           ------------------------         -------
*   14.11.2006                DM                         Created
*
*
*
************************************************************************************/

#ifndef _CRM_H_
#define _CRM_H_
#include "Platform.h"
/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/


#define RESET_CODE                    0x87651234
#define COP_RST_CODE                  0xc0de5afe   

/************************************************************************************
* This macro  enables the Wakeup Timer interrupt 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_WuTimerInterruptEnable()   CRM_WU_CNTL.timerWuIEn = 1
/************************************************************************************
* This macro  disables  the Wakeup Timer interrupt 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_WuTimerInterruptDisable()  CRM_WU_CNTL.timerWuIEn = 0
/************************************************************************************
* This macro  resets  the COP timer 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_CopReset()                 CRM_REGS_P->CopService = COP_RST_CODE
/************************************************************************************
* This macro  enables the RTC Timer  interrupt
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_RTCInterruptEnable()       CRM_WU_CNTL.rtcWuIEn = 1
/************************************************************************************
* This macro  disables the RTC Timer  interrupt
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_RTCInterruptDisable()      CRM_WU_CNTL.rtcWuIEn = 0
/************************************************************************************
* This macro  write protects CRM_XTAL_CNTL register
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_RefXtalConfigProtection()  CRM_XTAL_CNTL.wp = 1
/************************************************************************************
* This macro  disables the Ring oscillator
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_RingOscillatorDisable()    CRM_RING_OSC_CNTL.rOscEn = 0
/************************************************************************************
* This macro  enables the Ring oscillator
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_RingOscillatorEnable()     CRM_RING_OSC_CNTL.rOscEn = 1
/************************************************************************************
* This macro  sets the Timeout value for the RTC Timer 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_RTCSetTimeout(timeOut)     CRM_REGS_P->RtcTimeout = timeOut
/************************************************************************************
* This macro  
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_Xtal32Enable()             CRM_XTAL32_CNTL.xtal32En = 1; CRM_SYS_CNTL.xtal32Exists = 1
/************************************************************************************
* This macro enables the Jtag interface
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_JtagNexusEnable()          CRM_SYS_CNTL.jtagEn = 1
/************************************************************************************
* This macro disables the Jtag interface
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_JtagNexusDisable()         CRM_SYS_CNTL.jtagEn = 0
/************************************************************************************
* This macro Resets The MCU
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_SoftReset()                CRM_REGS_P->SwRst = RESET_CODE 

#define V_REG_MASK_BUCK               vRegBuckRdy
#define V_REG_MASK_1P5V               vReg1P5VRdy
#define V_REG_MASK_1P8V               vReg1P8VRdy
/************************************************************************************
* This macro can be used in an if statement to test if one of the following three voltage regulators are runing
*    - buck regulator if the macro parameter is V_REG_MASK_BUCK
*    - 1.5V regulator if the macro parameter is V_REG_MASK_1P5V
*    - 1.8V regulator if the macro parameter is V_REG_MASK_1P8V
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  08.11.2007  DM
************************************************************************************/
#define CRM_VRegIsReady(vRegMask)      (CRM_STATUS.vRegMask)


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
  gCrmErrNoError_c,
  gCrmErrCopCntlWP_c,
  gCrmErrXtalCntlWP_c,
  gCrmErrInvalidParameters_c,
  gCrmErrCalInProgress_c,
  gCrmErrNoCalInProgress_c,
  gCrmErrRingOscOff_c,
  gCrmErrIgnoredInActualPowerMode_c,
  gCrmErrInvalidPowerSource_c,
  gCrmErrBuckNotEnabledNorBypassed_c
}crmErr_t;

typedef enum
{
  gCrmPwS3V3Battery_c,
  gCrmPwSBuckRegulation_c,
  gCrmPwS1V8Battery_c,
  gCrmPwSNotValid_c
}crmPowerSource_t;

typedef enum
{
  gPadStandardDrive_c,
  gPadHighDrive_c
}crmPadsDriveStrength_t;

typedef enum 
{
  gARegDisable_c,
  gRxTxRegEnable_c,
  gARegNotValid_c,
  gRxTxandPLLRegEnable_c = 3
}crmAnalogVRegEnable_t;

typedef enum 
{
  gARegCurent4mA_c,
  gARegCurent20mA_c,
  gARegCurentNotValid_c,
  gARegCurent40mA_c = 3
}crmAnalogVRegCurrentSel_t;

typedef enum 
{
  gBuckVReg_c,
  g1P5VReg_c,
  g1P8VReg_c,
  gMaxVReg_c
}crmVReg_t;

typedef enum 
{
  gBuckClkDiv_16_c,
  gBuckClkDiv_8_c = 8,
  gBuckClkDiv_9_c,
  gBuckClkDiv_10_c,
  gBuckClkDiv_11_c,
  gBuckClkDiv_12_c,
  gBuckClkDiv_13_c,
  gBuckClkDiv_14_c,
  gBuckClkDiv_15_c,
}crmBuckClkDiv_t;

typedef   struct
{
  uint32_t buckEn:1; 
  uint32_t buckSyncRecEn:1; 
  uint32_t buckBypassEn:1; 
  uint32_t buckClkDiv:4; 
}crmBuckCntl_t;

typedef   struct
{
  uint32_t vReg1P5VEn:2; 
  uint32_t vReg1P5VISel:2; 
}crmVReg1P5VCntl_t;                

typedef   struct
{
  crmVReg_t vReg;
  union
  {
    crmBuckCntl_t buckCntl;
    crmVReg1P5VCntl_t vReg1P5VCntl;
    bool_t vReg1P8VEn;
  }cntl;
}crmVRegCntl_t;               

#define  BUCK_EN                    cntl.buckCntl.buckEn
#define  BUCK_SYNC_REC_EN           cntl.buckCntl.buckSyncRecEn
#define  BUCK_BYPASS_EN             cntl.buckCntl.buckBypassEn
#define  BUCK_CLK_DIV               cntl.buckCntl.buckClkDiv

#define  VREG_1P5V_EN               cntl.vReg1P5VCntl.vReg1P5VEn
#define  VREG_1P5V_I_SEL            cntl.vReg1P5VCntl.vReg1P5VISel

#define  VREG_1P8V_EN               cntl.vReg1P8VEn

typedef enum
{
  gCrmTimerWuEvent_c,
  gCrmRTCWuEvent_c,
  gCrmKB4WuEvent_c,
  gCrmKB5WuEvent_c,
  gCrmKB6WuEvent_c,
  gCrmKB7WuEvent_c,
  gCrmCalDoneEvent_c,
  gCrmXtal32ReadyEvent_c,
  gCrmCOPTimeoutEvent_c
}crmInterruptSource_t;

typedef enum
{
  gBgapBatt_c,
  gBgap1P8V_c,
  gVReg0P9VTrim_c,
  gVReg0P9ITrim_c,
  gVRegXtalTrim_c,
  gVRegOscTrim_c,
  gTrimMax_c
}crmTrimmedDevice_t;

typedef void (*pfCallback_t)(void);

typedef enum
{
  gXSD512_c,
  gXSD1024_c, 
  gXSD2048_c,
  gXSD4096_c,
  gXSD8192_c,
  gXSD16384_c,
  gXSD32768_c,
  gXSD65536_c
}crmXtalStartupDelay_t;

typedef union
{
  uint32_t word;
  struct
  {
    uint32_t wp:1;
    uint32_t smallSig:1;
    uint32_t startupDelayBypass:1;
    uint32_t startupDelaySel:3;
    uint32_t refXtalDiv:6;
  }bit; 
}crmRefXtalCntl_t;              

typedef enum
{
  gAutomaticBs_c,
  gManualBs_c,
  gDisableBs_c
}crmBSCntl_t;

typedef union
{
  uint32_t word;
  struct
  {
    uint32_t armEn:1;
    uint32_t macaEn:1;
    uint32_t asmEn:1;
    uint32_t spiEn:1;
    uint32_t gpioEn:1;
    uint32_t uart1En:1;
    uint32_t uart2En:1;
    uint32_t tmrEn:1;
    uint32_t rifEn:1;
    uint32_t i2cEn:1;
    uint32_t ssiEn:1;
    uint32_t spiFEn:1;
    uint32_t adcEn:1;
    uint32_t ahbEn:1;
    uint32_t jtagEn:1;
    uint32_t nexEn:1;
    uint32_t tmxEn:1;
    uint32_t aimEn:1;
  }bit;
}crmModuleEnableStatus_t;

typedef  union
{
  uint32_t word;
  struct
  {
    uint32_t copEn:1;
    uint32_t copOut:1;
    uint32_t copWP:1;
    uint32_t copTimeOut:7;
  } bit;
}crmCopCntl_t;

typedef enum
{
  gExtWu_c,
  gTimerWu_c,
  gRtcWu_c
} crmWuSource_t;

typedef enum
{
  gExtWuEventLevel_c,
  gExtWuEventEdge_c
}crmExtWuEvent_t;

typedef enum
{
  gExtWuPolLLevel_or_NEdge_c,
  gExtWuPolHLevel_or_PEdge_c
}crmExtWuPol_t;

typedef union
{
  uint32_t word;
  struct 
  {
    uint32_t  kbi4WuEn:1;
    uint32_t  kbi5WuEn:1;
    uint32_t  kbi6WuEn:1;
    uint32_t  kbi7WuEn:1;
    uint32_t  kbi4WuEvent:1;
    uint32_t  kbi5WuEvent:1;
    uint32_t  kbi6WuEvent:1;
    uint32_t  kbi7WuEvent:1;
    uint32_t  kbi4WuPol:1;
    uint32_t  kbi5WuPol:1;
    uint32_t  kbi6WuPol:1;
    uint32_t  kbi7WuPol:1;
    uint32_t  kbi4IntEn:1;
    uint32_t  kbi5IntEn:1;
    uint32_t  kbi6IntEn:1;
    uint32_t  kbi7IntEn:1;
  }bit;
  struct 
  {
    uint32_t  kbiWuEn:4;
    uint32_t  kbiWuEvent:4;
    uint32_t  kbiWuPol:4;
    uint32_t  kbiIntEn:4;
  }nibble;
}crmExtWuCtrl_t;

 #define KBI4_WU_EN                    ctrl.ext.bit.kbi4WuEn
 #define KBI5_WU_EN                    ctrl.ext.bit.kbi5WuEn
 #define KBI6_WU_EN                    ctrl.ext.bit.kbi6WuEn
 #define KBI7_WU_EN                    ctrl.ext.bit.kbi7WuEn
 
 #define KBI4_WU_EVENT                 ctrl.ext.bit.kbi4WuEvent
 #define KBI5_WU_EVENT                 ctrl.ext.bit.kbi5WuEvent
 #define KBI6_WU_EVENT                 ctrl.ext.bit.kbi6WuEvent
 #define KBI7_WU_EVENT                 ctrl.ext.bit.kbi7WuEvent
 
 #define KBI4_WU_POL                   ctrl.ext.bit.kbi4WuPol
 #define KBI5_WU_POL                   ctrl.ext.bit.kbi5WuPol
 #define KBI6_WU_POL                   ctrl.ext.bit.kbi6WuPol
 #define KBI7_WU_POL                   ctrl.ext.bit.kbi7WuPol
 
 #define KBI4_INT_EN                   ctrl.ext.bit.kbi4IntEn
 #define KBI5_INT_EN                   ctrl.ext.bit.kbi5IntEn
 #define KBI6_INT_EN                   ctrl.ext.bit.kbi6IntEn
 #define KBI7_INT_EN                   ctrl.ext.bit.kbi7IntEn
 
 #define KBI_WU_EN                     ctrl.ext.nibble.kbiWuEn
 #define KBI_WU_EVENT                  ctrl.ext.nibble.kbiWuEvent
 #define KBI_WU_POL                    ctrl.ext.nibble.kbiWuPol
 #define KBI_INT_EN                    ctrl.ext.nibble.kbiIntEn
               
typedef struct
{
  uint32_t timerWuEn:1; 
  uint32_t timerWuIntEn:1; 
  uint32_t timeOut;      
}crmTimerWuCtrl_t; 
              
#define TIMER_WU_EN                    ctrl.timer.timerWuEn
#define TIMER_WU_INT_EN                ctrl.timer.timerWuIntEn
#define TIMER_WU_TIMEOUT               ctrl.timer.timeOut
              
typedef struct
{
  uint32_t rtcWuEn:1;
  uint32_t rtcWuIntEn:1;
  uint32_t timeOut;      
}crmRtcWuCtrl_t;    

#define RTC_WU_EN                     ctrl.rtc.rtcWuEn
#define RTC_WU_INT_EN                 ctrl.rtc.rtcWuIntEn
#define RTC_WU_TIMEOUT                ctrl.rtc.timeOut              
              
typedef struct{
  crmWuSource_t wuSource;
  union
  {
    crmExtWuCtrl_t   ext;
    crmTimerWuCtrl_t timer;
    crmRtcWuCtrl_t   rtc;              
  }ctrl;
}crmWuCtrl_t;

typedef enum
{
  gRamRet8k_c,
  gRamRet32k_c,
  gRamRet64k_c,
  gRamRet96k_c
} crmRamRet_t;

typedef enum
{
  gNoMcuRet_c,
  gMcuRet_c,
  gMcuAndDigPadRet_c=3
} crmMcuRet_t;

typedef enum
{
  gHibernate_c,
  gDoze_c
} crmSleep_t;

typedef struct
{
  uint8_t sleepType:1;// 0 hibernate / 1 doze
  uint8_t ramRet:2;
  uint8_t mcuRet:1;
  uint8_t digPadRet:1;
  pfCallback_t  pfToDoBeforeSleep;
}crmSleepCtrl_t;


/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern void CRM_GoToSleep(crmSleepCtrl_t* pSleepCtrl);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_WuCntl(crmWuCtrl_t* pWuCtrl);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmModuleEnableStatus_t CRM_ModuleEnStatus(void);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_CopCntl(crmCopCntl_t copCntl);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_ForceCopTimeout(void);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern void CRM_Wait4Irq(void);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_BusStealingCntl(crmBSCntl_t bsCntl,bool_t armClkGate,uint8_t cyclesToSteal);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_RefXtalControl(crmRefXtalCntl_t xtalCntl);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_2kToXtal32Switch(bool_t armHalted, pfCallback_t pfToCallback);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_RingOscCal(uint16_t calLength,bool_t haltMcu, void * pTR);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_RingOscAbortCal(void);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_SetPowerSource(crmPowerSource_t pwSource);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_VRegCntl(crmVRegCntl_t* pVRegCntl);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_VRegTrimm(crmTrimmedDevice_t trimmedDevice,uint8_t trimmValue);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_SetDigOutDriveStrength(crmPadsDriveStrength_t padsDriveStrength);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_SetSPIFDriveStrength(crmPadsDriveStrength_t spiFDriveStrength);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern crmErr_t CRM_RegisterISR(crmInterruptSource_t crmIS, pfCallback_t pfISR);

/************************************************************************************
* Interface assumptions:
*   None
*   
* Return value:
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*  07.11.2007  MD
************************************************************************************/
extern void CRM_Isr(void);
#endif /* _CRM_H_ */
