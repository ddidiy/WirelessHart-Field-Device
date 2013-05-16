/************************************************************************************
* Public header file for the I2C driver.
*
* Author(s):  B04839 
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
Revision history:
      date                       Author                               Comments
   ----------               ----------------------                    -------
   11.10.2006               B04839                                  Created
************************************************************************************/
#ifndef _I2C_INTERFACE_H_
#define _I2C_INTERFACE_H_

#include "EmbeddedTypes.h"
/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#define DEFAULT_I2C_CLOCK     100    /* set default I2C clock to 100 KHz */
#define DEFAULT_SYSTEM_CLOCK  24000  /* set default System clock to 24000 KHz (24MHZ) */

#ifndef SYSTEM_CLOCK
  #define SYSTEM_CLOCK DEFAULT_SYSTEM_CLOCK
#endif /* SYSTEM_CLOCK */ 
    
#ifndef I2C_CLOCK
  #define I2C_CLOCK DEFAULT_I2C_CLOCK
#else
  #if I2C_CLOCK == 0  
    #error I2C_CLOCK MUST BE GREATER THAN ZERO
  #endif /* I2C_CLOCK == 0*/
#endif /* I2C_CLOCK */ 

#define DIVIDER  (SYSTEM_CLOCK) / (I2C_CLOCK)
#define DFSR_VAL 0x01

#if DIVIDER <= 160
#define FDR_VAL 0x20
#elif DIVIDER <= 192
#define FDR_VAL 0x21
#elif DIVIDER <= 224
#define FDR_VAL 0x22
#elif DIVIDER <= 256
#define FDR_VAL 0x23
#elif DIVIDER <= 288
#define FDR_VAL 0x00
#elif DIVIDER <= 320
#define FDR_VAL 0x01
#elif DIVIDER <= 384
#define FDR_VAL 0x02
#elif DIVIDER <= 448
#define FDR_VAL 0x26
#elif DIVIDER <= 480
#define FDR_VAL 0x03
#elif DIVIDER <= 512
#define FDR_VAL 0x27
#elif DIVIDER <= 576
#define FDR_VAL 0x04
#elif DIVIDER <= 640
#define FDR_VAL 0x05
#elif DIVIDER <= 768
#define FDR_VAL 0x06
#elif DIVIDER <= 896
#define FDR_VAL 0x2A
#elif DIVIDER <= 960
#define FDR_VAL 0x07
#elif DIVIDER <= 1024
#define FDR_VAL 0x2B
#elif DIVIDER <= 1152
#define FDR_VAL 0x08
#elif DIVIDER <= 1280
#define FDR_VAL 0x09
#elif DIVIDER <= 1536
#define FDR_VAL 0x0A
#elif DIVIDER <= 1792
#define FDR_VAL 0x2E
#elif DIVIDER <= 1920
#define FDR_VAL 0x0B
#elif DIVIDER <= 2048
#define FDR_VAL 0x2F
#else
#define FDR_VAL 0x0D
#endif

#undef  DIVIDER

#define I2cGetFDRVal()   FDR_VAL
#define I2cGetDFSRVal()  DFSR_VAL

/*I2C module status parameters definition*/
#define I2C_DEFAULT_STATUS    0x0000
#define I2C_ON                0x0001
#define I2C_TRANSMIT          0x0002
#define I2C_ARB_LOST          0x0004
#define I2C_SLV_NACK_RESP     0x0008
#define I2C_MODULE_BUSY       0x0010
#define I2C_SLV_TRANSMIT_REQ  0x0020
#define I2C_SLV_RECEIVE_REQ   0x0040
#define I2C_BREAK_TRANSF      0x0080
#define I2C_ADDR_AS_SLAVE     0x0100

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

typedef enum {
  gI2cErrNoError_c = 0,
  gI2cErrModuleIsEn_c,
  gI2cErrModuleIsDis_c,
  gI2cErrNullPointer_c,
  gI2cErrInvalidOp_c,
  gI2cErrBusBusy_c,
  gI2cErrNoDevResp_c,
  gI2CErrNoAckResp_c,
  gI2CErrTransferInProgress_c,
  gI2cErrModuleBusy_c,
  gI2cErrArbLost_c,
  gI2cErrMax_c
}I2cErr_t;

typedef enum {
  gI2cAckResponse_c = 0,
  gI2cNoAckResponse_c
}I2cResponse_t;

typedef enum {
  gI2cWrite_c = 0,
  gI2cRead_c
}I2cOperation_t;

typedef enum {
  gI2cBusIdle_c = 0,
  gI2cBusBusy_c
}I2cBusStatus_t;

typedef enum {
  gI2cSlvTransfer_c = 0,
  gI2cMstrReleaseBus_c,
  gI2cMstrHoldBus_c,
  gI2cTransferModeMax_c
}I2cTransferMode_t;

typedef enum {
  gI2cTransmitData_c = 0,
  gI2cReceiveData_c
}I2cTransferType_t;

typedef struct I2cConfig_tag{
  uint8_t  slaveAddress;
  uint8_t  freqDivider;
  uint8_t  saplingRate;
  bool_t   i2cInterruptEn;
  bool_t   i2cBroadcastEn;
}I2cConfig_t;

typedef  void (*I2cCallbackFunction_t)(uint16_t transfBytesNo, uint16_t status);

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
/*===== I2c_Init =====*/
/**
@brief  The function is called to initialize the I2C driver. Must be called before any further access.

@param  None

@return None

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern void I2c_Init(void);

/*================================================================================================*/
/*===== I2c_Enable =====*/
/**
@brief  The function is called to enable I2C module.

@param  None

@return parameter of type I2cErr_t with following possible values:

    gI2cErrModuleIsEn_c      - if the I2C module is already enabled
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_Enable(void);

/*================================================================================================*/
/*===== I2c_Disable =====*/
/**
@brief  The function is called to disable I2C module.

@param  None

@return parameter of type I2cErr_t with following possible values:

    gI2cErrModuleBusy_c      - if I2C module is busy
    gI2cErrModuleIsDis_c     - if the I2C module is already disabled
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_Disable(void);

/*================================================================================================*/
/*===== I2c_RecoverBus =====*/
/**
@brief  The function is called to recover I2C bus.
        Force the I2C module to become the I2C bus master and drive the SCL signal (even though SDA may
        already be driven, which indicates that the bus is busy)

@param  None

@return parameter of type I2cErr_t with following possible values:

    gI2cErrModuleIsDis_c     - if the I2C module is previously disabled; the function must be called 
                               when the module is enabled.
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_RecoverBus(void);

/*================================================================================================*/
/*===== I2c_SetCallbackFunction =====*/
/**
@brief  Set pointer to a data containing the address of the callback function for the I2C events.

@param  I2cCallbackFunction_t pI2cCallback  - input parameter: pointer to a callback function.

@return parameter of type I2cErr_t with following possible values:

    gI2cErrModuleIsDis_c     - if the I2C module is previously disabled. The function must be called 
                               when the module is enabled.
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_SetCallbackFunction(I2cCallbackFunction_t pI2cCallback);

/*================================================================================================*/
/*===== I2c_SetConfig =====*/
/**
@brief  Set the parameters for the I2C peripheral (enable/disable interrupts, enable/disable 
        broadcast capability, slave address, set I2C frequency divider and I2C sampling rate).

@param  I2cConfig_t *pI2cConfig             - input parameter: pointer to a structure which contain the 
                                              configuration parameters.

@return parameter of type I2cErr_t with following possible values:

    gI2cErrModuleIsDis_c     - if the I2C module is previously disabled; the function must be called 
                               when the module is enabled.
    gI2cErrNullPointer_c     - if pI2cConfig parameter is NULL
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_SetConfig(I2cConfig_t* pI2cConfig);

/*================================================================================================*/
/*===== I2c_GetStatus =====*/
/**
@brief  Get the status of the I2C module.

@param  uint8_t* status                     - output parameter: pointer to data where the status of the 
                                              I2C bus will be written.

@return parameter of type I2cErr_t with following possible values:

    gI2cErrNullPointer_c     - if status parameter is NULL
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_GetStatus(uint8_t* status);

/*================================================================================================*/
/*===== I2c_CancelTransfer =====*/
/**
@brief  Cancel transmitting/receiving data.

@param  I2cTransferType_t transferType      - input parameter: type of the trasfer that will be canceled.

@return parameter of type I2cErr_t with following possible values:
    gI2cErrModuleIsDis_c     - if the I2C module is previously disabled. The function must be called when
                               the module is enabled.
    gI2cErrInvalidOp_c       - if the function is called when the module is master and interrupts are not 
                               enabled.
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_CancelTransfer(I2cTransferType_t transferType);

/*================================================================================================*/
/*===== I2c_SendData =====*/
/**
@brief  Send a sequence of bytes to the device specified by slaveDestAddr parameter (if master mode 
        is used) or to the device which will address the module to read data from (if slave mode is 
        used). The function attempts to send dataLength bytes which are read from i2cBuffData buffer.

@param  uint8_t slaveDestAddr               - input parameter: destination  address  of slave device
                                              (only for module working in master mode).
@param  uint8_t* i2cBuffData                - input parameter: pointer to a memory location wherefrom 
                                              data will be sent.
@param  uint16_t dataLength                 - input parameter: length of data.
@param  I2cTransferMode_t transferMode      - input parameter: type of data transfer (master/slave).

@return parameter of type I2cErr_t with following possible values:

    gI2cErrModuleIsDis_c     - if the I2C module is previously disabled. The function must be called when
                               the module is enabled.
    gI2CErrNullPointer_c
    gI2cErrInvalidOp_c       - if the function is called when the module is slave and interrupts are not 
                               enabled.
    gI2cErrModuleBusy_c      - if the previous transaction is not completed.
    gI2cErrBusBusy_c         - if the bus is busy (there is another device that is transmitting data).
    gI2CErrNoAckResp_c       - if the receiving device doesn't acknowledge at least one byte sent during transmission.
    gI2cErrNoDevResp_c       - if there is no slave device with address slaveDestAddr.
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_SendData(uint8_t slaveDestAddr, uint8_t* i2cBuffData, uint16_t dataLength, I2cTransferMode_t transferMode);


/*================================================================================================*/
/*===== I2c_ReceiveData =====*/
/**
@brief  Receive a sequence of bytes from the device specified by slaveDestAddr parameter (if master mode is used) 
        or from the device which will address the module (if slave mode is used).

@param  uint8_t slaveDestAddr               - input parameter: destination  address  of slave device
                                              (only for module working in master mode).
@param  uint8_t* i2cBuffData                - input parameter: pointer to a memory location wherefrom 
                                              data will be sent.
@param  uint16_t dataLength                 - input parameter: length of data.
@param  I2cTransferMode_t transferMode      - input parameter: type of data transfer.

@return parameter of type I2cErr_t with following possible values:

    gI2cErrModuleIsDis_c     - if the I2C module is previously disabled. The function must be called when
                               the module is enabled.
    gI2CErrNullPointer_c
    gI2cErrInvalidOp_c       - if the function is called when the module is slave and interrupts are not 
                               enabled.
    gI2cErrModuleBusy_c      - if the previous transaction is not completed.
    gI2cErrBusBusy_c         - if the bus is busy (there is another device that is transmitting data).
    gI2CErrNoAckResp_c       - if the receiving device doesn't acknowledge at least one byte sent during transmission.
    gI2cErrNoDevResp_c       - if there is no slave device with address slaveDestAddr.
    gI2CErrNoError_c         - in rest

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern I2cErr_t I2c_ReceiveData(uint8_t slaveDestAddr, uint8_t* i2cBuffData, uint16_t dataLength, I2cTransferMode_t transferMode);

/*================================================================================================*/
/*===== I2c_Isr =====*/
/**
@brief  I2C interrupt service routine.

@param  None

@return None

 Revision history:
      Date                          Author                            Comments
   ----------               ----------------------                    -------
   16.07.2007                       B04839                             Created
*/
/*===============================================================================================*/
extern void I2c_Isr(void);

#endif /* _I2C_INTERFACE_H_ */
