/************************************************************************************
* Public header file for the Gpio driver.
*
* Author(s):  - B04839 
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
Revision history:
   Date                     Author                                    Comments
   ----------               ----------------------                    -------
   31.08.2006                B04839                                Created
************************************************************************************/
#ifndef _GPIO_INTERFACE_H_
#define _GPIO_INTERFACE_H_

#include "Platform.h"
#include "../Interface/EmbeddedTypes.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#define PORT_PINS_NO     32
#define FUNCTION_MASK    0x3UL
#define PinMask(gpioPin) (0x01UL << (gpioPin))

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef enum
{
  gGpioPort0_c = 0,
  gGpioPort1_c,
  gGpioPortMax_c
} GpioPort_t;

typedef enum
{
  gGpioDirIn_c = 0,
  gGpioDirOut_c,
  gGpioDirMax_c
} GpioDirection_t;

typedef enum
{
  gGpioPinStateLow_c = 0,
  gGpioPinStateHigh_c,
  gGpioPinStateMax_c,
} GpioPinState_t;

typedef enum
{
  gGpioPinReadPad_c = 0,
  gGpioPinReadReg_c,
  gGpioPinReadMax_c
} GpioPinReadSel_t;

typedef enum
{
  gGpioPinPulldown_c = 0,
  gGpioPinPullup_c,
  gGpioPinPullupMax_c,
} GpioPinPullupSel_t;

typedef enum
{
  gGpioPin0_c,   gGpioPin1_c,  gGpioPin2_c,  gGpioPin3_c,  gGpioPin4_c,  gGpioPin5_c,  gGpioPin6_c, gGpioPin7_c,
  gGpioPin8_c,   gGpioPin9_c, gGpioPin10_c, gGpioPin11_c, gGpioPin12_c, gGpioPin13_c, gGpioPin14_c, gGpioPin15_c,
  gGpioPin16_c, gGpioPin17_c, gGpioPin18_c, gGpioPin19_c, gGpioPin20_c, gGpioPin21_c, gGpioPin22_c, gGpioPin23_c,
  gGpioPin24_c, gGpioPin25_c, gGpioPin26_c, gGpioPin27_c, gGpioPin28_c, gGpioPin29_c, gGpioPin30_c, gGpioPin31_c,
  gGpioPin32_c, gGpioPin33_c, gGpioPin34_c, gGpioPin35_c, gGpioPin36_c, gGpioPin37_c, gGpioPin38_c, gGpioPin39_c,
  gGpioPin40_c, gGpioPin41_c, gGpioPin42_c, gGpioPin43_c, gGpioPin44_c, gGpioPin45_c, gGpioPin46_c, gGpioPin47_c,
  gGpioPin48_c, gGpioPin49_c, gGpioPin50_c, gGpioPin51_c, gGpioPin52_c, gGpioPin53_c, gGpioPin54_c, gGpioPin55_c,
  gGpioPin56_c, gGpioPin57_c, gGpioPin58_c, gGpioPin59_c, gGpioPin60_c, gGpioPin61_c, gGpioPin62_c, gGpioPin63_c,
  gGpioPinMax_c
} GpioPin_t;

typedef enum {
  gGpioNormalMode_c = 0,
  gGpioAlternate1Mode_c,
  gGpioAlternate2Mode_c,
  gGpioAlternate3Mode_c,
  gGpioFunctionModeMax_c
} GpioFunctionMode_t;

typedef enum {
  gGpioErrNoError_c = 0,
  gGpioErrInvalidParameter_c
} GpioErr_t;

typedef enum {
  gGpioDirAttr_c = 0,
  gGpioDataAttr_c ,
  gGpioInputDataSelAttr_c,
  gGpioPullUpEnAttr_c ,
  gGpioPullUpSelAttr_c,
  gGpioHystEnAttr_c,
  gGpioPullUpKeepAttr_c,
  gGpioMaxAttr_c
}GpioPortAttr_t;

typedef struct{
  uint32_t portDir;
  uint32_t portData;
  uint32_t portInputDataSel;
  uint32_t portPuEn;
  uint32_t portPuSel;
  uint32_t portHystEn;
  uint32_t portPuKeepEn;
}GpioPortInit_t;

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

/*GENERAL*/

/*================================================================================================*/
/*===== Gpio_InitPort =====*/
/**
@brief  Set the initial port settings of the port gpioPort specified as paramer.

@param  GpioPort_t gpioPort                 - input parameter: port which will be affected by the function.

@param  GpioPortInit_t* gpioPortInit        - input parameter: pointer to a structure that hold the 
                                              initial port settings.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the port gpioPort specified as parameter is grater or equal 
                                     than gGpioPortMax_c or gpioPortInit is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_InitPort(GpioPort_t gpioPort, GpioPortInit_t* gpioPortInit);

/*================================================================================================*/
/*===== Gpio_WrPortSetting =====*/
/**
@brief  Modify the attribute portAttr of the port gpioPort specified as parameter.

@param  GpioPort_t gpioPort                 - input parameter: port which will be affected by the function.

@param  GpioPortAttr_t portAttr             - input parameter: port attribute that should be modified.

@param  uint32_t portAttrValue              - input parameter: new attribute value.

@param  uint32_t mask                       - input parameter: specifies which pins of the port gpioPort 
                                              should be affected by the function (the attribute would be 
                                              modified if the corresponding bit is set).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the port gpioPort specified as parameter is grater or equal than 
                                    gGpioPortMax_c or portAttr is grater or equal than gMaxAttr_c .

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_WrPortSetting(GpioPort_t gpioPort, GpioPortAttr_t portAttr, uint32_t portAttrValue, uint32_t mask);

/*================================================================================================*/
/*===== Gpio_RdPortSetting =====*/
/**
@brief  Read the value of attribute portAttr of the gpioPort port specified as parameter.

@param  GpioPort_t gpioPort                 - input parameter: port which will be affected by the function.

@param  GpioPortAttr_t portAttr             - input parameter: port attribute that should be modified.

@param  uint32_t *portAttrValue             - output parameter: pointer to the memory location where 
                                              attribute value will be placed.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the port gpioPort specified as parameter is grater or equal 
                                     than gGpioPortMax_c or portAttrValue is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_RdPortSetting(GpioPort_t gpioPort, GpioPortAttr_t portAttr, uint32_t* portAttrValue);

/*DIRECTION*/

/*================================================================================================*/
/*===== Gpio_SetPortDir =====*/
/**
@brief  Set the direction value of the port gpioPort specified as parameter.

@param  GpioPort_t gpioPort                 - input parameter: port which will be affected by the function.

@param  uint32_t portDir                    - input parameter: port direction value.

@param  uint32_t mask                       - input parameter: specifies which pins of the port gpioPort 
                                              should be affected by the function (the directiorn would be 
                                              modified if the corresponding bit is set).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the port gpioPort specified as parameter is grater or equal 
                                     than gGpioPortMax_c or portAttrValue is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SetPortDir(GpioPort_t gpioPort, uint32_t portDir, uint32_t mask);

/*================================================================================================*/
/*===== Gpio_GetPortDir =====*/
/**
@brief  Read the direction value of the gpioPort port specified as parameter.

@param  GpioPort_t gpioPort                 - input parameter: port to get direction.

@param  uint32_t *portDir                   - output parameter: pointer to memeory location where 
                                              GPIO port direction value will be placed.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the port gpioPort specified as parameter is grater or equal 
                                    than gGpioPortMax_c or portDir is NULL .

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_GetPortDir(GpioPort_t gpioPort, uint32_t* portDir);

/*================================================================================================*/
/*===== Gpio_SetPinDir =====*/
/**
@brief  Set the direction value of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  GpioDirection_t gpioPinDir          - input parameter: pin's direction value.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c or gpioPinDir is grater or equal than gGpioDirMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SetPinDir(GpioPin_t gpioPin, GpioDirection_t gpioPinDir);

/*================================================================================================*/
/*===== Gpio_GetPinDir =====*/
/**
@brief  Get the direction value of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin to get direction.

@param  GpioDirection_t *gpioPinDir         - output parameter: pin's direction value.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c or gpioPinDir is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_GetPinDir(GpioPin_t gpioPin, GpioDirection_t* gpioPinDir);

/*DATA*/

/*================================================================================================*/
/*===== Gpio_SetPortData =====*/
/**
@brief  Set the data value of the port gpioPin specified as parameter.

@param  GpioPort_t gpioPort                 - input parameter: port which will be affected by the function.

@param  uint32_t portData                   - input parameter: port data value.

@param  uint32_t mask                       - input parameter: specifies which pins of the port gpioPort 
                                              should be affected by the function (the data would be 
                                              modified if the corresponding bit is set).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the port gpioPort specified as parameter is grater or equal than 
                                    gGpioPortMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SetPortData(GpioPort_t gpioPort, uint32_t portData, uint32_t mask);

/*================================================================================================*/
/*===== Gpio_GetPortData =====*/
/**
@brief  Read data value of the gpioPort port specified as parameter.

@param  GpioPort_t gpioPort                 - input parameter: port to get direction.

@param  uint32_t *portData                  - output parameter: pointer to memeory location where port
                                              data value will be placed.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the port gpioPort specified as parameter is grater or equal than 
                                    gGpioPortMax_c or portData is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_GetPortData(GpioPort_t gpioPort, uint32_t* portData);

/*================================================================================================*/
/*===== Gpio_SetPinData =====*/
/**
@brief  Set the data value of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  GpioPinState_t gpioPinState         - input parameter: pin's data value.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than
                                    gGpioPinMax_c or gpioPinState is grater or equal than gGpioPinStateMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SetPinData(GpioPin_t gpioPin, GpioPinState_t gpioPinState);


/*================================================================================================*/
/*===== Gpio_GetPinData =====*/
/**
@brief  Get the data value of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin to get data.

@param  GpioPinState_t *gpioPinState        - output parameter: pin's data value.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than
                                    gGpioPinMax_c or gpioPinState is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_GetPinData(GpioPin_t gpioPin, GpioPinState_t* gpioPinState);

/*================================================================================================*/
/*===== Gpio_TogglePin =====*/
/**
@brief  Toggle the state of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than gGpioPinMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_TogglePin(GpioPin_t gpioPin);

/*READ CONTROL*/

/*================================================================================================*/
/*===== Gpio_SetPinReadSource =====*/
/**
@brief  Set the read source of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  GpioPinReadSel_t gpioReadSource     - input parameter: pin read source (gGpioPinReadPad_c for 
                                              reading data from pin, gGpioPinReadReg_c for reading data from latch).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c or gpioReadSource is grater or equal than gGpioPinReadMax_c 
                                    the function exits with gGpioErrInvalidParamater_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SetPinReadSource(GpioPin_t gpioPin, GpioPinReadSel_t gpioReadSource);

/*================================================================================================*/
/*===== Gpio_GetPinReadSource =====*/
/**
@brief  Get the source of reading (pad or latch) for the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin to get read source.

@param  GpioPinReadSel_t *gpioReadSource    - output parameter: pin's read source value.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c or gpioReadSource is NULL the function exits with 
                                    gGpioErrInvalidParamater_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_GetPinReadSource(GpioPin_t gpioPin, GpioPinReadSel_t* gpioReadSource);

/*PULLUP ENABLE*/

/*================================================================================================*/
/*===== Gpio_EnPinPullup =====*/
/**
@brief  Enable pull-up of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  bool_t gpioEnPinPu                  - input parameter: pull-up enable (TRUE for enabling pull-up, 
                                              FALSE for disabling pull-up).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_EnPinPullup(GpioPin_t gpioPin, bool_t gpioEnPinPu);

/*================================================================================================*/
/*===== Gpio_IsPinPullupEn =====*/
/**
@brief  Get the pull-up enable state of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: GPIO pin to get pull-up enable state 
                                              (enabled/disabled).

@param  bool_t *gpioEnPinPu                 - output parameter: pin's pull-up enable state (TRUE for pull-up 
                                              enabled, FALSE for pull-up disabled).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than
                                    gGpioPinMax_c or gpioEnPinPu is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_IsPinPullupEn(GpioPin_t gpioPin, bool_t* gpioEnPinPu);

/*PULLUP SELECT*/

/*================================================================================================*/
/*===== Gpio_SelectPinPullup =====*/
/**
@brief  Select pull-up/pull-down of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  GpioPinPullupSel_t gpioPinPuSel     - input parameter: pin pull-up selection (gGpioPinPulldown_c 
                                              for pull-down selection, gGpioPinPullup_c for pull-up selection).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c or gpioPinPuSel is grater or equal than gGpioPinPullupMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SelectPinPullup(GpioPin_t gpioPin, GpioPinPullupSel_t gpioPinPuSel);

/*================================================================================================*/
/*===== Gpio_GetPinPullupSel =====*/
/**
@brief  Get the pull-up selection state of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: GPIO pin to get pull-up selection state
                                              (pull-up/pull-down).

@param  GpioPinPullupSel_t *gpioPinPuSel    - output parameter: pin pull-up selection state (gGpioPinPulldown_c
                                              for pull-down selection, gGpioPinPullup_c for pull-up selection).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than gGpioPinMax_c
                                    or gpioPinPuSel is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_GetPinPullupSel(GpioPin_t gpioPin, GpioPinPullupSel_t* gpioPinPuSel);

/*PULLUP KEEPER*/

/*================================================================================================*/
/*===== Gpio_EnPinPuKeeper =====*/
/**
@brief  Enable pull-up keeper of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  bool_t gpioEnPinPuKeep              - input parameter: pin pull-up keeper enable (TRUE for enabling 
                                              pull-up keeper, FALSE for disabling pull-up keeper).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than gGpioPinMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_EnPinPuKeeper(GpioPin_t gpioPin, bool_t gpioEnPinPuKeep);

/*================================================================================================*/
/*===== Gpio_IsPinPuKeeperEn =====*/
/**
@brief  Get the pull-up keeper enable state of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: GPIO pin to get pull-up selection state
                                              (pull-up/pull-down).

@param  bool_t *gpioEnPinPuKeep            - output parameter: pin pull-up keeper enable state (TRUE
                                             for pull-up keeper enabled, FALSE for pull-up keeper disabled).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than gGpioPinMax_c
                                    or gpioPinPuSel is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_IsPinPuKeeperEn(GpioPin_t gpioPin, bool_t* gpioEnPinPuKeep);

/*HYSTERESIS*/

/*================================================================================================*/
/*===== Gpio_EnPinHyst =====*/
/**
@brief  Enable hysteresis of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  bool_t gpioEnPinPuKeep              - input parameter: enable hysteresis state for the pin gpioPin
                                              specified as parameter.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than gGpioPinMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_EnPinHyst(GpioPin_t gpioPin, bool_t gpioEnPinHyst);

/*================================================================================================*/
/*===== Gpio_IsPinHystEn =====*/
/**
@brief  Get the hysteresis enable state of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: GPIO pin to get pull-up selection state
                                              (pull-up/pull-down).

@param  bool_t *gpioEnPinHyst               - output parameter: pin's hysteresis enable state (TRUE for 
                                              hysteresis enabled, FALSE for hysteresis  disabled).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c or gpioEnPinHyst is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_IsPinHystEn(GpioPin_t gpioPin, bool_t* gpioEnPinHyst);

/*FUCTION MODE*/

/*================================================================================================*/
/*===== Gpio_SetPortFunction =====*/
/**
@brief  Set the functionality (GPIO or alternate function) for the gpioPort port specified as parameter.

@param  GpioPort_t gpioPort                 - input parameter: port which will be affected by the function.

@param  GpioFunctionMode_t gpioPortFunction - input parameter: port function mode.

@param  uint32_t mask                       - input parameter: specifies which pins of the port gpioPort 
                                              should be affected by the function (the function mode would
                                              be modified if the corresponding bit is set).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPort port specified as parameter is grater or equal than
                                    gGpioPortMax_c or gpioPortFunction is grater or equal than gGpioFunctionModeMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SetPortFunction(GpioPort_t gpioPort, GpioFunctionMode_t gpioPortFunction, uint32_t mask);

/*================================================================================================*/
/*===== Gpio_SetPinFunction =====*/
/**
@brief  Set function mode for the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin which will be affected by the function.

@param  GpioFunctionMode_t gpioPinFunction  - input parameter: pin function mode.

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than 
                                    gGpioPinMax_c or gpioPinFunction is grater or equal than gGpioFunctionModeMax_c.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_SetPinFunction(GpioPin_t gpioPin, GpioFunctionMode_t gpioPinFunction);

/*================================================================================================*/
/*===== Gpio_GetPinFunction =====*/
/**
@brief  Get function mode of the pin gpioPin specified as parameter.

@param  GpioPin_t gpioPin                   - input parameter: pin to get function mode.

@param  GpioFunctionMode_t* gpioPinFunction - output parameter: pin's hysteresis enable state (TRUE for 
                                              hysteresis enabled, FALSE for hysteresis  disabled).

@return parameter of type GpioErr_t with following possible values:

    gGpioErrInvalidParamater_c    - if the gpioPin pin specified as parameter is grater or equal than
                                    gGpioPinMax_c or gpioPinFunction is NULL.

    gGpioErrNoError_c             - in rest.

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   26.06.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern GpioErr_t Gpio_GetPinFunction(GpioPin_t gpioPin, GpioFunctionMode_t* gpioPinFunction);

#endif /* _GPIO_INTERFACE_H_ */
