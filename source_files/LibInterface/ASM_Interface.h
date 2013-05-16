/************************************************************************************
* Public header file for the ASM driver.
*
* Author(s): Adrian Rendiuc - R11091
*
* (c) Copyright 2007, Freescale, Inc.  All rights reserved.
*
* Freescale Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
Revision history:
   Date             Author                          Comments
   ----------       -----------------------         --------
   26.04.2007       Adrian Rendiuc - R11091         Created
************************************************************************************/

#ifndef __ASM_INTERFACE_H
#define __ASM_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

#define Asm_TurnModuleClockOn()  (((Asm_Control1_t*)&ASM_REGS_P->Control1)->b.On = 1)
#define Asm_TurnModuleClockOff() (((Asm_Control1_t*)&ASM_REGS_P->Control1)->b.On = 0)

#define Asm_GoToNormalMode()     (((Asm_Control1_t*)&ASM_REGS_P->Control1)->b.NormalMode = 1;\
                                  ((Asm_Control1_t*)&ASM_REGS_P->Control1)->b.Bypass = 0)

#define Asm_GoToBypassMode()     (((Asm_Control1_t*)&ASM_REGS_P->Control1)->b.NormalMode = 1;\
                                  ((Asm_Control1_t*)&ASM_REGS_P->Control1)->b.Bypass = 1)     
                                
#define Asm_GetTestPassStatus()  (((Asm_Status_t*)&ASM_REGS_P->Status)->b.TestPass)


/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

typedef volatile union
{
  uint32_t w;
  struct{
           uint32_t :24;
           uint32_t Start:1;
           uint32_t Clear:1;
           uint32_t LoadMac:1;           
           uint32_t :4;
           uint32_t ClearIrq:1;
        }b;
}Asm_Control0_t;

typedef volatile union
{
  uint32_t w;
  struct{
           uint32_t On:1;
           uint32_t NormalMode:1;
           uint32_t Bypass:1;           
           uint32_t :21;
           uint32_t Cbc:1;
           uint32_t Ctr:1;
           uint32_t SelfTest:1;           
           uint32_t :4;
           uint32_t MaskIrq:1;           
        }b;
}Asm_Control1_t;

typedef volatile union
{
  uint32_t w;
  struct{
           uint32_t :24;
           uint32_t Done:1;
           uint32_t TestPass:1;
        }b;
}Asm_Status_t;

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* This data type enumerates the ASM API calls return values */
typedef enum {
    gAsmErrNoError_c = 0,                 
    gAsmSelfTestFail_c,
    gAsmInternalError_c,
    gAsmNullPointerErr_c,
    gAsmErrMax_c
} Asm_Err_t;

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

/*=================================================================================*/
/*===== Asm_Init =====*/
/*
@brief  This function will be called to initialize ASM.

@param  none

@return parameter of type Asm_Err_t
        possible values: 
              gAsmErrNoError_c   - the hardware passed the selt test; 
              gAsmSelfTestFail_c - the hardware did not pass the self test;

Revision history:
   Date             Author                        Comments
   ----------       -----------------------       --------
   02.05.2007       Adrian Rendiuc - R11091       Created
*/
/*=================================================================================*/
Asm_Err_t Asm_Init(void);

/*=================================================================================*/
/*===== Asm_CtrEncryptDecrypt =====*/
/*
@brief  This function will be called for CTR encryption/decryption

@param  uint32_t* maKey     - input parameter: pointer to key used for encryption/decryption
                                               or NULL if no key should be loaded
        uint32_t* maCounter - input parameter: pointer to counter used for encryption/decryption
                                               or NULL if no counter should be loaded  
        uint32_t* maTextIn  - input parameter: pointer to data to encrypt/decrypt
        uint32_t* maTextOut - input parameter: pointer to location where to store encrypted/
                                               decrypted data

@return parameter of type Asm_Err_t
        possible values: 
              gAsmErrNoError_c   - the operation finished without errors;
              gAsmSelfTestFail_c - the hardware did not pass the self test;
              gAsmNullPointerErr - function call with a null pointer for data location
              gAsmInternalError  - ASM did not finish operations after expected number of cycles 
                                    
Revision history:
   Date             Author                        Comments
   ----------       -----------------------       --------
   02.05.2007       Adrian Rendiuc - R11091       Created
*/
/*=================================================================================*/
Asm_Err_t Asm_CtrEncryptDecrypt( uint32_t* maKey, 
                                 uint32_t* maCounter, 
                                 uint32_t* maTextIn,
                                 uint32_t* maTextOut); 

/*=================================================================================*/
/*===== Asm_CbcMacEncryptDecrypt =====*/
/*
@brief  This function will be called for CBC-MAC generation

@param  uint32_t* maKey             - input parameter: pointer to key used for encryption/decryption    
                                                       or NULL if no key should be loaded
        uint32_t* maMacAccPreloadVal- input parameter: pointer to previously calculated MAC value that 
                                                       can be loaded at user request or NULL if no MAC
                                                       value should be loaded
        uint32_t* maText            - input parameter: pointer to data to encrypt/decrypt
        uint32_t* maCbcMacVal       - input parameter: pointer to location where to store computed 
                                                       CBC-MAC value 

@return parameter of type Asm_Err_t
        possible values: 
              gAsmErrNoError_c   - the operation finished without errors;
              gAsmSelfTestFail_c - the hardware did not pass the self test;
              gAsmNullPointerErr - function call with null pointer parameters for maText or maCbcMacVal
              gAsmInternalError  - ASM did not finish operations after expected number of cycles 
                                   
Revision history:
   Date             Author                        Comments
   ----------       -----------------------       --------
   14.05.2007       Adrian Rendiuc - R11091       Created
*/
/*=================================================================================*/
Asm_Err_t Asm_CbcMacEncryptDecrypt( uint32_t* maKey, 
                                    uint32_t* maMacAccPreloadVal, 
                                    uint32_t* maText, 
                                    uint32_t* maCbcMacVal);

/*=================================================================================*/
/*===== Asm_CcmEncryptDecrypt =====*/
/*
@brief  This function will be called for CCM encryption/decryption

@param  uint32_t* maKey             - input parameter: pointer to key used for encryption/decryption
                                                       or NULL if no key should be loaded   
        uint32_t* maCounter         - input parameter: pointer to counter value used for encryption/decryption 
                                                       or NULL if no counter should be loaded  
        uint32_t* maMacAccPreloadVal- input parameter: previously calculated MAC value that can be loaded
                                                       at user request 
        uint32_t* maTextIn          - input parameter: pointer to data to encrypt/decrypt
        uint32_t* maCbcMacVal       - input parameter: pointer to location where to store computed 
                                                       CBC-MAC value 
        uint32_t* maTextOut         - input parameter: pointer to location where to store encrypted/
                                                       decrypted data                                                      

@return parameter of type Asm_Err_t
        possible values: 
              gAsmErrNoError_c   - the operation finished without errors;
              gAsmSelfTestFail_c - the hardware did not pass the self test;
              gAsmNullPointerErr - function call with null pointer parameters for maText or maCbcMacVal
              gAsmInternalError  - ASM did not finish operations after expected number of cycles 
                                   
Revision history:
   Date             Author                        Comments
   ----------       -----------------------       --------
   15.05.2007       Adrian Rendiuc - R11091       Created
*/
/*=================================================================================*/
Asm_Err_t Asm_CcmEncryptDecrypt( uint32_t* maKey,
                                 uint32_t* maCounter,
                                 uint32_t* maMacAccPreloadVal, 
                                 uint32_t* maTextIn, 
                                 uint32_t* maCbcMacVal,
                                 uint32_t* maTextOut);

#endif
