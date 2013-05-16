/************************************************************************************
* Public header file for the UART driver.
*
* Author(s): r04471 
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                r04471                                     Created
   25.05.2006                r04471                                    Modified - Review ID 3556
   01.06.2006                r04471                                    Modified - Implementation of flow control mechanism
   22.05.2007                r200036                                  Added UartGetByteFromRxBuffer
                                                                               Added support for Beekit UART componen
                                                                               Added gUartPatchingMode_d define
************************************************************************************/
#ifndef _UART_LOW_LEVEL_H_
#define _UART_LOW_LEVEL_H_

#include "../Interface/EmbeddedTypes.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#define  UART_1                0    // driver internal number associated to the first UART peripheral 
#define  UART_2                1    // driver internal number associated to the second UART peripheral 
#define  UART_NR_INSTANCES     2    // number of UART peripherals on board

#define BIT_TX_EN              BIT0
#define BIT_RX_EN              BIT1
#define BIT_PARITY_EN          BIT2
#define BIT_PARITY_EVEN        BIT3
#define BIT_STOP_BITS2         BIT4
#define BIT_RX_INT             BIT6
#define BIT_TX_INT             BIT7
#define BIT_XTIM               BIT10
#define BIT_MASK_TX_INT        BIT13
#define BIT_MASK_RX_INT        BIT14
#define BIT_FLOW_CNT_EN        BIT12
#define BIT_RTS_ACTIVE_HIGH    BIT11

#define UartOpenReceiver(UartNumber) UartOpenCloseTransceiver((UartNumber), BIT_RX_EN, TRUE)
#define UartCloseReceiver(UartNumber) UartOpenCloseTransceiver((UartNumber), BIT_RX_EN, FALSE)
#define UartOpenTransmitter(UartNumber) UartOpenCloseTransceiver((UartNumber), BIT_TX_EN, TRUE)
#define UartCloseTransmitter(UartNumber) UartOpenCloseTransceiver((UartNumber), BIT_TX_EN, FALSE)

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

/** This data type enumerates the UART API calls return values */
typedef enum {
    gUartErrNoError_c = 0,                 
    gUartErrUartAlreadyOpen_c,                 
    gUartErrUartNotOpen_c,
    gUartErrNoCallbackDefined_c,
    gUartErrReadOngoing_c,
    gUartErrWriteOngoing_c,
    gUartErrInvalidClock_c,
    gUartErrNullPointer_c,
    gUartErrInvalidNrBytes_c,
    gUartErrInvalidBaudrate_c,
    gUartErrInvalidParity_c,
    gUartErrInvalidStop_c,
    gUartErrInvalidCTS_c,
    gUartErrInvalidThreshold_c,
    gUartErrWrongUartNumber_c,
    gUartErrMax_c
} UartErr_t;

/** This data type enumerates the possible read operation status */
typedef enum {
    gUartReadStatusComplete_c = 0,
    gUartReadStatusCanceled_c ,
    gUartReadStatusError_c,
    gUartReadStatusMax_c
} UartReadStatus_t;

/** This data type enumerates the possible write operation status */
typedef enum {
    gUartWriteStatusComplete_c = 0,
    gUartWriteStatusCanceled_c,
    gUartWriteStatusMax_c
} UartWriteStatus_t;

/** This data type enumerates the possible values of UART parity modes */
typedef enum {
    gUartParityNone_c = 0,
    gUartParityEven_c,
    gUartParityOdd_c,
    gUartParityMax_c
} UartParityMode_t;

/** This data type enumerates the possible values of UART stop bits */
typedef enum {
    gUartStopBits1_c = 0,
    gUartStopBits2_c,
    gUartStopBitsMax_c
} UartStopBits_t;

/** This data type describes the configuration of the UART module */
typedef struct {
    uint32_t         UartBaudrate;
    UartParityMode_t UartParity;
    UartStopBits_t   UartStopBits;
    bool_t           UartFlowControlEnabled;
    bool_t           UartRTSActiveHigh;
} UartConfig_t;

/** This data type describes the possible errors passed to the read callback function  */
typedef struct {
    uint32_t   UartReadOverrunError:1;
    uint32_t   UartParityError:1;
    uint32_t   UartFrameError:1;
    uint32_t   UartStartBitError:1;
    uint32_t   Reserved:4;
} UartReadErrorFlags_t;

/** This data type describes the parameter passed to the read callback function */
/** Do not modify the order of the members in the structure,  because it is optimized for lower space consumption*/
typedef struct {
    UartReadStatus_t            UartStatus;
    uint16_t                    UartNumberBytesReceived;    
    UartReadErrorFlags_t        UartReadError;
} UartReadCallbackArgs_t;

/** This data type describes the parameter passed to the write callback function */
typedef struct {
    UartWriteStatus_t       UartStatus;
    uint16_t                UartNumberBytesSent;
} UartWriteCallbackArgs_t;

/** This data type lists the callback functions for the UART driver */
typedef struct {
    void (*pfUartReadCallback)(UartReadCallbackArgs_t* args);
    void (*pfUartWriteCallback)(UartWriteCallbackArgs_t* args);
} UartCallbackFunctions_t;

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
/*===== UartOpen =====*/
/**
@brief  This function will be called to open an UART instance.

@param  uint8_t    UartNumber    - input parameter: number of the UART instance to be opened
@param  uint32_t PlatformClock -  input parameter: platform clock (in Khz). Will be used for baudrate calculation

@return parameter of type UartErr_t
        possible values: they are self-explaining
              gUartErrNoError_c
              gUartErrWrongUartNumber_c
              gUartErrInvalidClock_c
              gUartErrUartAlreadyOpen_c

 Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                          r04471                           Created
   25.05.2006                          r04471                           Modified - Review ID 3556

===============================================================================================*/
extern UartErr_t UartOpen(uint8_t UartNumber, uint32_t PlatformClock);


/*================================================================================================*/
/*===== UartSetConfig =====*/
/**
@brief  This function will be called to configure the UART.

@param  uint8_t       UartNumber   - input parameter: number of the UART to configure
@param    UartConfig_t* pConfig      - input parameter: a pointer to the stucture containing the configuration settings for UART.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c
                  gUartErrNullPointer_c
                  gUartErrUartNotOpen_c
                  gUartErrReadOngoing_c
                  gUartErrWriteOngoing_c
                  gUartErrInvalidBaudrate_c
                  gUartErrInvalidParity_c
                  gUartErrInvalidStop_c
       
 Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
   01.06.2006                     r04471                               Flow control feature added
*/
/*================================================================================================*/
extern UartErr_t UartSetConfig(uint8_t UartNumber, UartConfig_t* pConfig);


/*================================================================================================*/
/*===== UartSetReceiverThreshold =====*/
/*
@brief  This function is called to set the level of the Rx FIFO threshold.

@param  uint8_t UartNumber - input parameter: number of the UART instance to set receiver threshold.
@param  uint8_t Threshold  - input parameter: specifies the value of the threshold.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                gUartErrNoError_c
                gUartErrWrongUartNumber_c
                gUartErrInvalidThreshold_c
                gUartErrUartNotOpen_c
                gUartErrReadOngoing_c

    
Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
   01.06.2006                     r04471                               Flow control feature added       
*/
/*================================================================================================*/
extern UartErr_t UartSetReceiverThreshold(uint8_t UartNumber, uint8_t Threshold);


/*================================================================================================*/
/*===== UartSetTransmitterThreshold =====*/
/*
@brief  This function is called to set the level of the Tx FIFO threshold.

@param  uint8_t UartNumber - input parameter: number of the UART instance to set transmitter threshold.
@param  uint8_t Threshold  - input parameter: specifies the value of the threshold.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                gUartErrNoError_c
                gUartErrWrongUartNumber_c
                gUartErrInvalidThreshold_c
                gUartErrUartNotOpen_c
                gUartErrReadOngoing_c

    
Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
       
*/
/*================================================================================================*/
extern UartErr_t UartSetTransmitterThreshold(uint8_t UartNumber, uint8_t Threshold);

/*================================================================================================*/
/*===== UartSetCTSThreshold =====*/
/*
@brief  This function is called to set the level of the CTS FIFO threshold.

@param  uint8_t UartNumber - input parameter: number of the UART instance to set CTS threshold.
@param  uint8_t Threshold  - input parameter: specifies the value of the threshold.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                gUartErrNoError_c
                gUartErrWrongUartNumber_c
                gUartErrInvalidThreshold_c
                gUartErrUartNotOpen_c
                gUartErrReadOngoing_c

Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   01.06.2006                     r04471                               Created      
   
*/
/*================================================================================================*/
extern UartErr_t UartSetCTSThreshold(uint8_t UartNumber, uint8_t Threshold);

/*================================================================================================*/
/*===== UartSetCallbackFunctions =====*/
/*
@brief  This function will be called to set the callback functions for the UART driver.

@param  uint8_t                     UartNumber - input parameter: number of the UART instance to set the callback functions to
@param  UartCallbackFunctions_t* pConfig    - input parameter: a pointer to the stucture containing the callback functions for UART driver.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c 
                  gUartErrNullPointer_c
                  gUartErrUartNotOpen_c

Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
   
*/
/*================================================================================================*/
extern UartErr_t UartSetCallbackFunctions(uint8_t UartNumber, UartCallbackFunctions_t* pConfig);


/*================================================================================================*/
/*===== UartReadData =====*/
/*
@brief  This function is called to make a read request to a UART driver instance.

@param  uint8_t  UartNumber          - input parameter: number of the UART instance to read from
@param  uint8_t* pBuf                - input parameter: pointer to a location in memory where the data shall be copied to.
@param  uint16_t BufferSize          - input parameter: maximum number of bytes that can be copied to the buffer. 
@param  bool_t UartDirectFifoMode    - input parameter: Enable the UART direct FIFO mode.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c
                  gUartErrNullPointer_c
                  gUartErrInvalidNrBytes_c
                  gUartErrUartNotOpen_c
                  gUartErrReadOngoing_c
                  gUartErrNoCallbackDefined_c

Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
       
*/
/*================================================================================================*/
extern UartErr_t UartReadData(uint8_t UartNumber,  uint8_t* pBuf, uint16_t BufferSize, bool_t UartDirectFifoMode);


/*================================================================================================*/
/*===== UartGetByteFromRxBuffer =====*/
/*
@brief  This function gets the trailing byte from the receiving queue

@param  uint8_t  UartNumber   - input parameter: number of the UART instance to read from
@param  uint8_t* pDst         - output parameter: pointer to a location in memory where the byte shall be copied to.

@return parameter of type bool_t
        possible values: 
    TRUE - if there is at least one byte in the queue
    FALSE - otherwise

Revision history:
   date                     Author                                          Comments
   ----------               ----------------------                         -------------
   22.05.2007                         r200036                                  Created         
*/
/*================================================================================================*/
extern bool_t UartGetByteFromRxBuffer(uint8_t UartNumber, uint8_t *pDst);


/*================================================================================================*/
/*===== UartOpenCloseTransceiver =====*/
/*
@brief  This function enables or disables the receiver or transmitter of the specified UART port

@param  uint8_t  UartNumber   - input parameter: number of the UART instance to read from
@param  uint8_t Pin         - input parameter: BIT_RX_EN or BIT_TX_EN
@param  bool_t Open         - input parameter: TRUE if the specified pin will be enabled, FALSE otherwise

@return parameter or type void      

Revision history:
   date                     Author                                          Comments
   ----------               ----------------------                         -------------
   22.05.2007                        r200036                                 Created         
*/
/*================================================================================================*/
extern void UartOpenCloseTransceiver(uint8_t UartNumber, uint8_t Pin, bool_t Open);


/*================================================================================================*/
/*===== UartClearErrors =====*/
/*
@brief  This function clears the Status register of the specified UART port

@param  uint8_t  UartNumber   - input parameter: number of the UART instance to read from

@return parameter or type void      

Revision history:
   date                     Author                                          Comments
   ----------               ----------------------                         -------------
   22.05.2007                      r200036                                  Created         
*/
/*================================================================================================*/
extern void UartClearErrors(uint8_t UartNumber);


/*================================================================================================*/
/*===== UartCancelReadData =====*/
/*
@brief  This function is called to make a cancel read request to a UART driver instance.

@param  uint8_t  UartNumber   - input parameter: number of the UART instance to cancel read operation.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c
                  gUartErrUartNotOpen_c

           
Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
       
*/
/*================================================================================================*/
extern UartErr_t UartCancelReadData(uint8_t UartNumber);


/*================================================================================================*/
/*===== UartWriteData =====*/
/*
@brief  This function is called to make a write request to the UART.


@param  uint8_t  UartNumber     - input parameter: number of the UART instance to write to.
@param  uint8_t* pBuf        - input parameter: pointer to a location in memory where from the data shall be send from.
@param  uint16_t NumberBytes - input parameter: number of bytes to send.


@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c
                  gUartErrNullPointer_c
                  gUartErrInvalidNrBytes_c
                  gUartErrUartNotOpen_c
                  gUartErrWriteOngoing_c
                  gUartErrNoCallbackDefined_c


Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
       
*/
/*================================================================================================*/
extern UartErr_t UartWriteData( uint8_t UartNumber, uint8_t* pBuf, uint16_t NumberBytes);


/*================================================================================================*/
/*===== UartCancelWriteData =====*/
/*
@brief  This function is called to make a cancel write request to a UART driver instance.

@param  uint8_t  UartNumber   - input parameter: number of the UART instance to cancel write operation.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c
                  gUartErrUartNotOpen_c

Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
       
*/
/*================================================================================================*/
extern UartErr_t UartCancelWriteData(uint8_t UartNumber);


/*================================================================================================*/
/*===== UartGetStatus =====*/
/*
@brief  This function returns the status of the UART instance.

@param  uint8_t UartNumber - input parameter: number of the UART instance to get status from

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c 
                  gUartErrUartNotOpen_c
                  gUartErrReadOngoing_c
                  gUartErrWriteOngoing_c
                  gUartErrNoCallbacksDefined_c

Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
       
*/
/*================================================================================================*/
extern UartErr_t UartGetStatus(uint8_t UartNumber);


/*================================================================================================*/
/*===== UartGetConfig =====*/
/*
@brief  This function will be called to get the configuration of the UART.

@param  uint8_t       UartNumber - input parameter: number of the UART instance to get configuration from
@param  UartConfig_t* pConfig    - input parameter: a pointer to the stucture where the configuration settings for UART will be placed.

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c
                  gUartErrWrongUartNumber_c
                  gUartErrNullPointer_c
                  gUartErrUartNotOpen_c

Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
   01.06.2006                     r04471                               Flow control feature added       
*/
/*================================================================================================*/
extern UartErr_t UartGetConfig(uint8_t UartNumber, UartConfig_t* pConfig);



extern uint32_t UartGetUnreadBytesNumber(uint8_t UartNumber);


/*================================================================================================*/
/*===== UartClose =====*/
/*
@brief  This function will be called to close an UART instance.

@param  uint8_t UartNumber - input parameter: number of the UART instance to close

@return parameter of type UartErr_t
        possible values: they are self-explaining
                  gUartErrNoError_c 
                  gUartErrWrongUartNumber_c
                  gUartErrReadOngoing_c
                  gUartErrWriteOngoing_c
    
Revision history:
   date                     Author                                     Comments
   ----------               ----------------------                    -------
   05.05.2006                     r04471                               Created
   25.05.2006                     r04471                               Modified - Review ID 3556
             
*/
/*================================================================================================*/
extern UartErr_t UartClose(uint8_t UartNumber);

/*================================================================================================*/
/*===== UartSetHalfFlowControl =====*/
/*
@brief  This function sets the half flow control mode.

@param  uint8_t uartNumber - input parameter: number of the UART instance to set HFC

@return parameter of type void

Revision history:
      date                        Author                             Comments
   ----------               ----------------------                    -------
   06.06.2008                     B04839                              Created
       
*/
/*================================================================================================*/
extern void UartSetHalfFlowControl(uint8_t uartNumber, bool_t bHalfFlowControl);


/*================================================================================================*/
/*===== UartIsr1 =====*/
/*
@brief  This function will be called by the AITC driver when interrupt for UART 1 instance is triggered.

@param  none

@return none
    
 Revision history:
   date                Author                        Comments
   ----------        ----------------------        -------
   05.05.2006                     r04471               Created
       
*/
/*================================================================================================*/
extern void UartIsr1(void);


/*================================================================================================*/
/*===== UartIsr2 =====*/
/*
@brief  This function will be called by the AITC driver when interrupt for UART 2 instance is triggered.

@param  none

@return none
    
 Revision history:
   date                Author                        Comments
   ----------        ----------------------        -------
   05.05.2006                     r04471               Created
       
*/
/*================================================================================================*/
extern void UartIsr2(void);


#endif /* _UART_LOW_LEVEL_H_ */
