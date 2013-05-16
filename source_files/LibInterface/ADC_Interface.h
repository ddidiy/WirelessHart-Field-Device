/************************************************************************************
* Public header file for the Adc driver.
*
* Author(s): B04839
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
Revision history:
   date                             Author                            Comments
   ----------               ----------------------                    -------
   12.12.2006                    B04839                            Created
************************************************************************************/
#ifndef _ADC_INTERFACE_H_
#define _ADC_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#define FIFO_DEEP             7
#define FIFO_STAT_LEVEL_MASK  0x0007
#define CTRL_ON               0x0001

#define DEFAULT_ON_TIME       8    
#define DEFAULT_CONV_TIME     40

#define Adc_DefaultConfig(adcConfig, oscVal) \
  adcConfig.adcPrescaler = (((uint16_t)((oscVal) / 1000)) - 1), adcConfig.adcDivider = (((uint16_t) ((oscVal) / 300)) - 1), \
  adcConfig.adcOnPeriod = DEFAULT_ON_TIME, adcConfig.adcConvPeriod = DEFAULT_CONV_TIME, \
  adcConfig.adcCompIrqEn = FALSE, adcConfig.adcFifoIrqEn = FALSE, adcConfig.adcMode = gAdcAutoControl_c


#define Adc_TurnOn()        ADC.Control |= CTRL_ON
#define Adc_TurnOff()       ADC.Control &= ~CTRL_ON
#define Adc_FifoData()      (ADC.FifoRead)
#define Adc_FifoLevel()     (ADC.FifoStatus & FIFO_STAT_LEVEL_MASK)
#define Adc_ChnTriggered()  (ADC.Triggers)
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
/*ADC module status parameters definition*/
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum {
  gAdcErrNoError_c = 0,
  gAdcErrWrongParameter,
  gAdcErrNullPointer_c,
  gAdcErrModuleBusy_c,
  gAdcInvalidOp_c,
  gAdcErrMax_c
}AdcErr_t;

typedef enum {
  gAdcChan0_c = 0,
  gAdcChan1_c,
  gAdcChan2_c,
  gAdcChan3_c,
  gAdcChan4_c,
  gAdcChan5_c,
  gAdcChan6_c,
  gAdcChan7_c,
  gAdcBatt_c,
  gAdcMaxChan_c
}AdcChannel_t;

typedef enum {
  gAdcPrimary_c = 0,
  gAdcSecondary_c,
  gAdcMaxModule_c
}AdcModule_t;

typedef enum {
  gAdcModuleOff_c = 0,
  gAdcModuleOn_c
}AdcModuleStatus_t;

typedef enum {
  gAdcAutoControl_c = 0,
  gAdcManualControl_c
}AdcMode_t;

typedef enum {
  gAdcCompGrater_c = 0,
  gAdcCompLess_c
}AdcCompType_t;

typedef enum {
  gAdcSeqOnConvTime_c = 0,
  gAdcSeqOnTmrEv_c
}AdcSeqMode_t;

typedef enum {
  gAdcInactiveChan_c = 0,
  gAdcActiveChan_c
}AdcChanStatus_t;

typedef enum {
  gAdcBatteryRefVoltage_c = 0,
  gAdcExtRefVoltage_c
}AdcRefVoltage_t;

typedef enum {
  gAdcFifoEmpty_c = 0,
  gAdcFifoFull_c,
  gAdcFifoFilled_c
}AdcFifoStatus_t;

typedef enum {
  gAdcCompEvent_c = 0,
  gAdcSeq1event_c,
  gAdcSeq2event_c,
  gAdcFifoEvent_c,
  gAdcMaxEvent_c
}AdcEvent_t;

typedef struct{
  uint8_t          adcPrescaler;
  uint8_t          adcDivider;
  uint16_t         adcOnPeriod;
  uint16_t         adcConvPeriod;
  bool_t           adcCompIrqEn;
  bool_t           adcFifoIrqEn;
  AdcMode_t        adcMode;
}AdcConfig_t;

typedef struct{
  bool_t           adcTmrOn;
  bool_t           adcSeqIrqEn;
  uint16_t         adcChannels;
  uint32_t         adcTmBtwSamples;
  AdcSeqMode_t     adcSeqMode;
  AdcRefVoltage_t  adcRefVoltage;
}AdcConvCtrl_t;

typedef struct{
  AdcChannel_t     adcChannel;
  AdcCompType_t    adcCompType;
  uint16_t         adcCompVal;
}AdcCompCtrl_t;

typedef struct{
  AdcChannel_t     adcChannel;
  uint16_t         adcValue;
}AdcFifoData_t;

typedef  void (*AdcEvCallback_t)(void);

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
/*===== Adc_Init =====*/
/**
@brief  The function is called to initialize the ADC driver. Must be called before any further access.

@param  None

@return None

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern void Adc_Init(void);

/*================================================================================================*/
/*===== Adc_Reset =====*/
/**
@brief  The function is called to reset the ADC hardware.

@param  None

@return None

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern void Adc_Reset(void);

/*================================================================================================*/
/*===== Adc_SetConfig =====*/
/**
@brief  Set the parameters for the ADC peripheral.

@param  const AdcConfig_t *adcConfig        - input parameter: pointer to a structure containing the 
                                              configuration parameters.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrModuleBusy_c      - if the module is busy (the module was programmed to sample data from 
                               ADC pins and was turned ON).
    gAdcErrNullPointer_c     - if adcConfig parameter is NULL.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_SetConfig(const AdcConfig_t *adcConfig);

/*================================================================================================*/
/*===== Adc_SetConvCtrl =====*/
/**
@brief  Set the conversion control of the ADC module specified as parameter.

@param  AdcModule_t adcModule               - input parameter: instance of the ADC module (primary/secondary
                                              ADC module) that will be configured.

@param  const AdcConvCtrl_t *adcConvCtrl    - input parameter: pointer to a structure containing the 
                                              configuration parameters.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrWrongParameter    - if the adcModule value is grater or equal than gAdcMaxModule_c.
    gAdcErrNullPointer_c     - if adcConfig parameter is NULL.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_SetConvCtrl(AdcModule_t adcModule, const AdcConvCtrl_t *adcConvCtrl);

/*================================================================================================*/
/*===== Adc_SetCompCtrl =====*/
/**
@brief  Set the conversion control of the ADC module specified as parameter.

@param  const AdcCompCtrl_t* adcCompCtrl    - input parameter: pointer to a structure containing the 
                                              comparison configuration parameters.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrNullPointer_c     - if adcConfig parameter is NULL.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_SetCompCtrl(const AdcCompCtrl_t *adcCompCtrl);

/*================================================================================================*/
/*===== Adc_SetFifoCtrl =====*/
/**
@brief  Set the threshold of the ADC FIFO.

@param  uint8_t adcFifoThreshold            - input parameter: value of the FIFO treshold.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrWrongParameter_c  - if the value of adcFifoThreshold exceeds maximum value of the threshold (7).
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_SetFifoCtrl(uint8_t adcFifoThreshold);

/*================================================================================================*/
/*===== Adc_GetFifoStatus =====*/
/**
@brief  Get the FIFO status (empty/full/filled) and the fill level.

@param  AdcFifoStatus_t *fifoStatus         - output parameter: pointer to a memory location where the 
                                              current FIFO status value will be placed.
@param  uint8_t *adcFifoLevel               - output parameter: pointer to a memory location where the 
                                              current FIFO level value will be placed.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrNullPointer_c     - if fifoStatus or adcFifoLevel parameter is NULL.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_GetFifoStatus(AdcFifoStatus_t *fifoStatus, uint8_t *adcFifoLevel);

/*================================================================================================*/
/*===== Adc_ReadFifoData =====*/
/**
@brief  Read data from the ADC FIFO.

@param  AdcFifoData_t *adcFifoData          - output parameter: pointer to a memory location where FIFO 
                                              data will be placed.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrNullPointer_c     - if adcFifoData parameter is NULL.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_ReadFifoData(AdcFifoData_t *adcFifoData);

/*================================================================================================*/
/*===== Adc_StartManualConv =====*/
/**
@brief  Configure the manual conversion mode of the ADC.

@param  AdcModule_t adcModule               - input parameter: instance of the ADC module (primary/secondary
                                              ADC module) that will be configured for manual conversion.

@param  AdcChannel_t adcChannel             - input parameter: channel that will be sampled in manual mode.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrWrongParameter_c  - if the adcModule value is grater or equal than gAdcMaxModule_c.
    gAdcInvalidOp_c          - if the current ADC mode is not manual conversion mode.
    gAdcErrModuleBusy_c      - if the function is called before reading the result of the previous started
                               conversion.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_StartManualConv(AdcModule_t adcModule, AdcChannel_t adcChannel);

/*================================================================================================*/
/*===== Adc_ManualRead =====*/
/**
@brief  Get the conversion result value after manual mode was set and configured.

@param  AdcModule_t adcModule               - input parameter: instance of the ADC module (primary/secondary
                                              ADC module) that will be configured for manual conversion.

@param  uint16_t* adcConvResult             - output parameter: pointer to a memory location where the 
                                              result of the manual conversion is placed.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrWrongParameter_c  - if the adcModule value is grater or equal than gAdcMaxModule_c.
    gAdcInvalidOp_c          - if the current ADC mode is not manual conversion mode.
    gAdcErrModuleBusy_c      - if the function is called before reading the result of the previous started
                               conversion.
    gAdcErrNullPointer_c     - if adcConvResult parameter is NULL.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_ManualRead(AdcModule_t adcModule, uint16_t *adcConvResult);

/*================================================================================================*/
/*===== Adc_SetCallback =====*/
/**
@brief  Set the callbacks for ADC events.

@param  AdcEvent_t adcEvent                 - input parameter: the ADC event wherefore the callback is set.

@param  AdcEvCallback_t pCallbackFunction   - input parameter: pointer to a memory address of the event
                                              callback.

@return parameter of type AdcErr_t with following possible values:

    gAdcErrWrongParameter_c  - if the adcEvent value is grater or equal than gAdcMaxEvent_c.
    gAdcErrNullPointer_c     - if pCallbackFunction parameter is NULL.
    gAdcErrNoError_c         - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/
extern AdcErr_t Adc_SetCallback(AdcEvent_t adcEvent, AdcEvCallback_t pCallbackFunction);

/*================================================================================================*/
/*===== Adc_Isr =====*/
/**
@brief  AdC interrupt service routine.

@param  None

@return None

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   31.07.2007                       B04839                             Created
*/   
/*================================================================================================*/
extern void Adc_Isr(void);

#endif /* _ADC_INTERFACE_H_  */
