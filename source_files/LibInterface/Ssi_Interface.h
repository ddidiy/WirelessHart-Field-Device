/************************************************************************************
* Public header file for the SSI driver.
*
* Author(s):  B06485
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
Revision history:
   date (DD.MM.YYYY)               Author                         Comments
   ----------               ----------------------                -------
   25.06.2007                      B06485                         Created
************************************************************************************/
#ifndef _SSI_INTERFACE_H_
#define _SSI_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
/* SSI Word Length Configuration (STCCR.WL) */
#define SSI_WL_2_BITS     0x00
#define SSI_WL_4_BITS     0x01
#define SSI_WL_6_BITS     0x02
#define SSI_WL_8_BITS     0x03
#define SSI_WL_10_BITS    0x04
#define SSI_WL_12_BITS    0x05
#define SSI_WL_14_BITS    0x06
#define SSI_WL_16_BITS    0x07
#define SSI_WL_18_BITS    0x08
#define SSI_WL_20_BITS    0x09
#define SSI_WL_22_BITS    0x0A
#define SSI_WL_24_BITS    0x0B
#define SSI_WL_26_BITS    0x0C
#define SSI_WL_28_BITS    0x0D
#define SSI_WL_30_BITS    0x0E
#define SSI_WL_32_BITS    0x0F

/* SSI Clock configuration */
#define SSI_CLK_DIV2      0x00040000
#define SSI_CLK_PSR8      0x00020000

/* SSI BIT CLOCK Frequency Configuration */
#define SSI_SET_BIT_CLOCK_FREQ(sys_freq, bit_clk_freq)\
     (uint32_t)(bit_clk_freq) <= ((uint32_t)(sys_freq) / 16)?\
       (((uint32_t)(((uint32_t)(sys_freq) / ((uint32_t)(bit_clk_freq) * 2)) - 1) & 0xFF ) | SSI_CLK_DIV2 | SSI_CLK_PSR8):\
      (uint32_t)(bit_clk_freq) <= ((uint32_t)(sys_freq) / 8)?\
         (((uint32_t)(((uint32_t)(sys_freq) / ((uint32_t)(bit_clk_freq) * 2)) -1) & 0xFF ) | SSI_CLK_PSR8):\
          (uint32_t)(bit_clk_freq) <= ((uint32_t)(sys_freq) / 2)?\
                  (((uint32_t)(((uint32_t)(sys_freq) / ((uint32_t)(bit_clk_freq) *2)) -1) & 0xFF ) | SSI_CLK_DIV2):\
                      0/*(((uint32_t)(((uint32_t)(sys_freq) / ((uint32_t)(bit_clk_freq) *2))) & 0xFF ))*/

/* Default configuration for the SSI Clock structure      DIV2   |    PSR    |     WL    |    DC    |     PM     */
/* For default clock configuration, assign this macro to the SsiClockConfig_t.ssiClockConfigWord instance */
/* Then call the Ssi_SetClockConfig function */
#define SSI_DEFAULT_CLOCK_CONFIG           (uint32_t)( (1 << 18) | (1 << 17) | (7 << 13) | (1 << 8) |  (1 << 0))

/* Default configuration for the SSI TX Structure        TXBIT0  |   TFEN   |  TFDIR   |  TXDIR   |  TSHFD   |   TSCKP  |   TFSI   |   TFSL   |  TEFS   */ 
/* For default TX configuration, assign this macro to the SsiTxRxConfig_t.ssiTxRxConfigWord instance */
/* Then call the Ssi_SetTxConfig function */
#define SSI_DEFAULT_TX_CONFIG               (uint16_t)( (1 << 9) | (1 << 7) | (1 << 6) | (1 << 5) | (0 << 4) | (1 << 3) | (1 << 2) | (0 << 1) | (0 << 0))               

/* Default configuration for the SSI RX Structure       RXEXT    | RXBIT0   |   RFEN   |  RFDIR   |  RXDIR   |  RSHFD   |   RSCKP  |   RFSI   |   RFSL   |  REFS   */ 
/* For default TX configuration, assign this macro to the SsiTxRxConfig_t.ssiTxRxConfigWord instance */
/* Then call the Ssi_SetRxConfig function */
#define SSI_DEFAULT_RX_CONFIG              (uint16_t)( (0 << 10) | (1 << 9) | (1 << 7) | (0 << 6) | (0 << 5) | (0 << 4) | (1 << 3) | (1 << 2) | (0 << 1) | (0 << 0))               

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

/* SSI Error codes */
typedef enum {
  gSsiErrNoError_c = 0,                 
  gSsiAlreadyEnabled_c,
  gSsiAlreadyDisabled_c,    
  gSsiErrNullPointer_c, 
  gSsiErrDataLength_c,
  gSsiErrWordSize_c,
  gSsiErrInvalidConfiguration_c,    
  gSsiErrNoCallback_c,
  gSsiErrInvalidOpType_c,
  gSsiErrBusy_c,
  gSsiErrNotBusy_c,    
  gSsiErrMax_c
} SsiErr_t;

/* SSI mode */
typedef enum {
  gSsiNormalMode_c = 0,
  gSsiI2SMasterMode_c,
  gSsiI2SSlaveMode_c,
  gSsiModeMax_c
} SsiMode_t;

/* Operation type for the Abort function */
typedef enum {
  gSsiOpTypeTx_c = 0,
  gSsiOpTypeRx_c,
  gSsiOpTypeMax_c
}SsiOpType_t;

/* SSI General Config */
typedef struct SSIConfig_tag 
{
  bool_t      ssiInterruptEn;
  SsiMode_t   ssiMode;
  bool_t      ssiNetworkMode;
  bool_t      ssiGatedTxClockMode;
  bool_t      ssiGatedRxClockMode;    
} SsiConfig_t;

/* SSI Clock Config */
typedef union SSIClockConfig_tag
{
  uint32_t ssiClockConfigWord;
  struct{
    uint32_t      ssiPM: 8;
    uint32_t      ssiDC : 5 ;     
    uint32_t      ssiWL : 4 ;     
    uint32_t      ssiPSR : 1;     
    uint32_t      ssiDIV2 : 1;     
  } bit;    
} SsiClockConfig_t;

/* SSI Tx/RX Config */
typedef union SSITxRxConfig_tag
{
  uint16_t      ssiTxRxConfigWord;
  struct{
    uint16_t      ssiEFS : 1;
    uint16_t      ssiFSL : 1;
    uint16_t      ssiFSI : 1;     
    uint16_t      ssiSCKP : 1;     
    uint16_t      ssiSHFD: 1;     
    uint16_t      ssiCLKDIR : 1 ;     
    uint16_t      ssiFDIR : 1 ;     
    uint16_t      ssiFEN : 1;     
    uint16_t      ssiReserved2: 1;     
    uint16_t      ssiBIT0 : 1;
    uint16_t      ssiRxEXT : 1;     
  } bit;    
} SsiTxRxConfig_t;

/* Data word Size for TX and RX functions */
typedef enum
{
  gSsiWordSize8bit_c = 0,
  gSsiWordSize16bit_c = 1,
  gSsiWordSizeInvalid_c = 2,
  gSsiWordSize32bit_c = 3,
  gSsiWordSizeMax_c
} SsiWordSize_t;

/* Callback Functions types */
typedef   void    (*SSITxCallback_t)(void);
typedef   void    (*SSIRxCallback_t)(void);
typedef   void*   (*SSIContinuousRxCallback_t)(uint8_t *length);

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
extern void      SSI_Init(void);
extern SsiErr_t  SSI_Enable(bool_t enable);
extern SsiErr_t  SSI_SetConfig(SsiConfig_t* pSsiConfig);
extern SsiErr_t  SSI_SetClockConfig(SsiClockConfig_t* pSsiClockConfig);
extern SsiErr_t  SSI_SetTxRxConfig(SsiTxRxConfig_t* pSsiTxRxConfig, SsiOpType_t opType);
extern SsiErr_t  SSI_SetTxCallback(SSITxCallback_t ssiTxCallback);
extern SsiErr_t  SSI_SetRxCallback(SSIRxCallback_t ssiRxCallback);
extern SsiErr_t  SSI_SetContinuousRxCallback(SSIContinuousRxCallback_t ssiContinuousRxCallback);
extern SsiErr_t  SSI_Abort(SsiOpType_t opType);
extern SsiErr_t  SSI_TxData(void* pData, uint8_t length, SsiWordSize_t wordSize, uint32_t timeSlot);
extern SsiErr_t  SSI_RxData(void* pData, uint8_t length, SsiWordSize_t wordSize, uint32_t timeSlot);
extern SsiErr_t  SSI_StartContinuousRx(void * pData, uint8_t length, SsiWordSize_t wordSize, uint32_t timeSlot);

/* SSI Interrupt Service Routine. Covers both TX and RX paths. Use the ITC driver to register this ISR. DO NOT CALL IT MANUALLY. */
extern void      SSI_ISR(void);

#endif /* _SSI_INTERFACE_H_ */
