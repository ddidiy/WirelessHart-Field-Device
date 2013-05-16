/************************************************************************************
* Header file for SPI Driver
*
* Author(s): r04473
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*Revision history:
*   date                     Author                       Comments
*   ----------           ------------------------         -------
*   05.05.2006              r04473                         Created
*   25.05.2006              r04473                         Modified - Review ID 3557
*   29.10.2007              r04473                         Modified
*
************************************************************************************/

#ifndef _SPI_INTERFACE_H_
#define _SPI_INTERFACE_H_

#include "EmbeddedTypes.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
/*********************** Begin of SPI Config Setup Register Modes ******************/
#define  ConfigModeMaster             0
#define  ConfigModeSlave              1
#define  ConfigS3WireInactive         0
#define  ConfigS3WireActive           1

#define  ConfigClockPolPositive       0
#define  ConfigClockPolNegative       1
#define  ConfigClockPhase1stEdge      0
#define  ConfigClockPhase2ndEdge      1
#define  ConfigMisoPhaseOppositeEdge  0
#define  ConfigMisoPhaseSameEdge      1
#define  ConfigSsDelay1Clk            0
#define  ConfigSsDelay2Clk            1
#define  ConfigSsDelay3Clk            2
#define  ConfigSsDelay4Clk            3

#define  ConfigSsSetupMasterAutoH     0
#define  ConfigSsSetupMasterAutoL     1
#define  ConfigSsSetupMasterLow       2
#define  ConfigSsSetupMasterHigh      3
#define  ConfigSsSetupSlaveHigh       0
#define  ConfigSsSetupSlaveLow        1

#define  ConfigSdoInactiveZ           0
#define  ConfigSdoInactiveL           2
#define  ConfigSdoInactiveH           3

#define  ConfigClockFreqDiv2          0
#define  ConfigClockFreqDiv4          1
#define  ConfigClockFreqDiv8          2
#define  ConfigClockFreqDiv16         3
#define  ConfigClockFreqDiv32         4
#define  ConfigClockFreqDiv64         5
#define  ConfigClockFreqDiv128        6
#define  ConfigClockFreqDiv256        7
/************************ End of SPI Config Modes ************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef struct {
  union {
    uint32_t  Word;
    struct {
      uint32_t  DataCount:7;   // max value is 32
      uint32_t  reserved0:1;   // should be zero
      uint32_t  ClockCount:8;  // Master Mode only, max value is 32
      uint32_t  reserved1:16;  // should be zero
    } Bits;
  } ClkCtrl;

  union {
    uint32_t  Word;
    struct {
      uint32_t  SsSetup:2;
      uint32_t  SsDelay:2;     // Master Mode only
      uint32_t  SdoInactive:2;
      uint32_t  reserved0:2;   // should be zero
      uint32_t  ClockPol:1;
      uint32_t  ClockPhase:1;
      uint32_t  MisoPhase:1;   // Master Mode only
      uint32_t  reserved1:1;   // should be zero
      uint32_t  ClockFreq:3;   // Master Mode only
      uint32_t  reserved2:1;   // should be zero
      uint32_t  Mode:1;
      uint32_t  S3Wire:1;
      uint32_t  reserved3:14;  // should be zero
    } Bits;
  } Setup;
} spiConfig_t;

/***********************************************************************************/
typedef enum
{
  gSpiErrNoError_c,
  gSpiErrAlreadyOpen_c,
  gSpiErrPortClosed_c,
  gSpiErrAbortError_c,
  gSpiErrAsyncOperationPending_c,
  gSpiErrWrongConfig_c,
  gSpiErrWrongCallbackFunc_c,
  gSpiErrNullPointer_c,
  gSpiErrMax_c
} spiErr_t;

/***********************************************************************************/
typedef void (*spiCallback_t)(uint32_t Data);

/***********************************************************************************/
typedef enum
{
  gSpiStatusClosed_c,
  gSpiStatusIdle_c,
  gSpiStatusSyncOperationPending_c,
  gSpiStatusAsyncOperationPending_c,
  gSpiStatusError_c,
  gSpiStatusMax_c
} spiStatus_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Function:      SPI_Open
*
* Description:   open SPI port
*
*  Default config:
*      SsSetup     = ConfigSsSetupSlaveLow / ConfigSsSetupMasterAutoL
*      SsDelay     = ConfigSsDelay1Clk
*      SdoInactive = ConfigSdoInactiveZ
*      ClockFreq   = ConfigClockFreqDiv256
*      Mode        = ConfigModeSlave
*      S3Wire      = ConfigS3WireInactive
*      DataCount   = 8
*      ClockCount  = 8
*
* Parameters:    none
*
* Return value:  gSpiErrAlreadyOpen_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_Open(void);

/************************************************************************************
* Function:      SPI_Close
*
* Description:   closes SPI port
*
* Parameters:    none
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_Close(void);

/************************************************************************************
* Function:      SPI_SetConfig
*
* Description:   configure a Spi port with info contained in the spiConfig_t structure
*
* Parameters:    spiConfig_t *pConfig   - pointer to configuration structure 
*                                           (is defined in this file)
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrWrongConfig_c,
*                gSpiErrNullPointer_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_SetConfig(spiConfig_t *pConfig);

/************************************************************************************
* Function:      SPI_GetConfig
*
* Description:   put the configuration of Spi into spiConfig_t structure
*
* Parameters:    spiConfig_t *pConfig   - pointer to configuration structure 
*                                           (is defined in this file)
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrNullPointer_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_GetConfig(spiConfig_t *pConfig);

/************************************************************************************
* Function:      SPI_WriteSync
*
* Description:   transmit a word through Spi,
*                remains in function until the end of transmission
*
* Parameters:    uint32_t    Data       - 32 bit data to be transmitted
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrAsyncOperationPending_c,
*                gSpiErrAbortError_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_WriteSync(uint32_t Data);

/************************************************************************************
* Function:      SPI_ReadSync
*
* Description:   receive a word through Spi,
*                remains in function until the end of reception
*
* Parameters:    uint32_t *  pData      - pointer to 32 bit word,
*                                         here will be placed the received data
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrAsyncOperationPending_c,
*                gSpiErrAbortError_c,
*                gSpiErrNullPointer_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_ReadSync(uint32_t *pData);

/************************************************************************************
* Function:      SPI_SetTxAsync
*
* Description:   put data to be transmitted into Tx Spi register
*
* Parameters:    uint32_t    Data       - data to be transmitted
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrAsyncOperationPending_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_SetTxAsync(uint32_t Data);

/************************************************************************************
* Function:      SPI_GetRxAsync
*
* Description:   get last received data from Rx Spi register
*
* Parameters:    uint32_t *  pData      - pointer to received data
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrAsyncOperationPending_c,
*                gSpiErrNullPointer_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_GetRxAsync(uint32_t *pData);

/************************************************************************************
* Function:      SPI_StartAsync
*
* Description:   start tx/rx operation
*
* Parameters:    none
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrAsyncOperationPending_c,
*                gSpiErrWrongCallbackFunc_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_StartAsync(void);

/************************************************************************************
* Function:      SPI_Abort
*
* Description:   abort current operation
*
* Parameters:    none
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrAbortError_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_Abort(void);

/************************************************************************************
* Function:      SPI_SetCallback
*
* Description:   sets callback function which will be called
*                from interrupt service routine
*
* Parameters:    spiCallback_t CallbackFunc  - pointer to function to be called
*
* Return value:  gSpiErrPortClosed_c,
*                gSpiErrWrongCallbackFunc_c,
*                gSpiErrNoError_c
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiErr_t  SPI_SetCallback(spiCallback_t CallbackFunc);

/************************************************************************************
* Function:      SPI_GetStatus
*
* Description:   returns status of a Spi port
*
* Parameters:    none
*
* Return value:  gSpiStatusClosed_c,
*                gSpiStatusIdle_c,
*                gSpiStatusAsyncOperationPending_c,
*                gSpiStatusSyncOperationPending_c,
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
spiStatus_t  SPI_GetStatus(void);

/************************************************************************************
* Function:      SPI_ISR
*
* Description:   interrupt service routine for SPI
*
* Parameters:    none
*
* Return value:  none
* 
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   070406     GB      Created
* 
************************************************************************************/
void    SPI_ISR(void);

#endif /* _SPI_INTERFACE_H_ */



