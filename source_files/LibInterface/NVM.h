/************************************************************************************
* This is a template header file.
*
* Author(s):
    Thomas O. Jensen
*   Dragos Musoiu
* (c) Copyright 2005, Freescale, Inc.  All rights reserved.
*
* Freescale Confidential Proprietary
* Digianswer Confidential
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/

#ifndef _NVM_DRIVER_H_
#define _NVM_DRIVER_H_

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

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
typedef enum
{
  gNvmType_NoNvm_c,
  gNvmType_SST_c,
  gNvmType_ST_c,
  gNvmType_ATM_c,
  gNvmType_Max_c
} nvmType_t;

typedef enum
{
  gNvmErrNoError_c = 0,
  gNvmErrInvalidInterface_c,
  gNvmErrInvalidNvmType_c,
  gNvmErrInvalidPointer_c,
  gNvmErrWriteProtect_c,
  gNvmErrVerifyError_c,
  gNvmErrAddressSpaceOverflow_c,
  gNvmErrBlankCheckError_c,
  gNvmErrRestrictedArea_c,
  gNvmErrMaxError_c
} nvmErr_t;

typedef enum
{
  gNvmInternalInterface_c,
  gNvmExternalInterface_c, 
  gNvmInterfaceMax_c
} nvmInterface_t;

/***********************************************************************************/



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

/************************************************************************************
* This function executes a search for the known NVM types
*   
* Interface assumptions:
*   None
*   
* Return value:
* nvmErr
*   gNvmErrInvalidInterface_c  : if nvmInterface is not one of the two valid values(gNvmInternalInterface_c or gNvmExternalInterface_c)
*   gNvmErrInvalidPointer_c    : if pNvmType is NULL
*   gNvmErrNoError_c           : if nvmInterface is one of the two valid values
* 
* *pNvmType
*   gNvmType_SST_c             : SST   NVM was found
*   gNvmType_ST_c              : ST    NVM was found
*   gNvmType_ATM_c             : ATMEL NVM was found
*   gNvmType_NoNvm_c           : No    NVM was found
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   030106     TOJ      Created
*  07.11.2007  MD
************************************************************************************/
extern nvmErr_t NVM_Detect(nvmInterface_t nvmInterface,nvmType_t* pNvmType);

/************************************************************************************
* Reads numBytes of data from the flash. The address is used initially, but any
* subsequent reads are done in "continous-read" mode, without setting the address
* again. The read data is copied into the memory location referred to by pDest.
*   
* Interface assumptions:
*   The address uses only 24 bits.
*   
* Return value:
*
* nvmErr_t
*   gNvmErrInvalidInterface_c      : if nvmInterface is not one of the two valid values(gNvmInternalInterface_c or gNvmExternalInterface_c)
*   gNvmErrInvalidNvmType_c        : if nvmType is not one of the three known values(gNvmType_SST_c,gNvmType_ST_c,gNvmType_ATM_c)
*   gNvmErrAddressSpaceOverflow_c  : if (address +numBytes-1 )> gNvmMaxAddress_c
*   gNvmErrInvalidPointer_c        : if  pDest == NULL
*   gNvmErrRestrictedArea_c        : if  nvmInterface = gNvmInternalInterface_c and the address space reachs last sector 
*   gNvmErrNoError_c               : in rest
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   030106     TOJ      Created
* 07.11.2007   MD
************************************************************************************/
extern nvmErr_t NVM_Read(nvmInterface_t nvmInterface , nvmType_t nvmType , void *pDest, uint32_t address, uint32_t numBytes);

/************************************************************************************
* Erase a set of sectors, identified by the sector_bitfield.
*   
* for  nvmType = gNvmType_SST_c NVM 
*         Sector size = 4096. Number of sectors = 32 .
*         Each bit of the sectorBitfield represents a sector.Bit 0 represents sector 0... Bit 31 represents sector 31
* 
* for  nvmType = gNvmType_ST_c or nvmType = gNvmType_ATM_c
*         Sector size = 32768. Number of sectors = 4 
*         Only the least 4  significant bits represents a sector.Bit 0 represents sector 0... Bit 3 represents sector 3
*   
* Return value:
*   nvmErr
*     gNvmErrInvalidInterface_c      : if nvmInterface is not one of the two valid values(gNvmInternalInterface_c or gNvmExternalInterface_c)
*     gNvmErrInvalidNvmType_c        : if nvmType is not one of the three known values(gNvmType_SST_c,gNvmType_ST_c,gNvmType_ATM_c) 
*     gNvmErrRestrictedArea_c        : if the last sector is set to be erased  
*     gNvmErrNoError_c               : in rest
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
*   030106     TOJ      Created
* 07.11.2007   MD
************************************************************************************/
extern nvmErr_t NVM_Erase(nvmInterface_t nvmInterface, nvmType_t nvmType ,uint32_t sectorBitfield);
/************************************************************************************
* Writes numBytes of data from the memory location referred to by pSrc to the address location in flash.
*
* Interface assumptions:
*   The address uses only 24 bits.
*   
* Return value:
*   nvmErr
*     gNvmErrInvalidInterface_c      : if nvmInterface is not one of the two valid values(gNvmInternalInterface_c or gNvmExternalInterface_c)
*     gNvmErrInvalidNvmType_c        : if nvmType is not one of the three known values(gNvmType_SST_c,gNvmType_ST_c,gNvmType_ATM_c)
*     gNvmErrAddressSpaceOverflow_c  : if (address +numBytes-1 )> gNvmMaxAddress_c
*     gNvmErrRestrictedArea_c        : if  nvmInterface = gNvmInternalInterface_c and the address space reachs last sector 
*     gNvmErrBlankCheckError_c       : if at lest one of the location to write is not blank(0xff)
*     gNvmErrWriteProtect_c          : if the NVM cannot be enabled to be write   
*     gNvmErrVerifyError_c           : if after the write operation at least one location in NVM is different from it's intended value
*     gNvmErrNoError_c               : in rest
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
* 07.11.2007   MD
* 
************************************************************************************/
extern nvmErr_t NVM_Write(nvmInterface_t nvmInterface, nvmType_t nvmType ,void *pSrc, uint32_t address, uint32_t numBytes);

/************************************************************************************
* Verifies  numBytes of data from the address address in flash to be equal with the data referred to by pSrc in RAM .
*   
* Interface assumptions:
*   The address uses only 24 bits.
*   
* Return value:
*   nvmErr
*     gNvmErrInvalidInterface_c      : if nvmInterface is not one of the two valid values(gNvmInternalInterface_c or gNvmExternalInterface_c)
*     gNvmErrInvalidNvmType_c        : if nvmType is not one of the three known values(gNvmType_SST_c,gNvmType_ST_c,gNvmType_ATM_c)
*     gNvmErrAddressSpaceOverflow_c  : if (address +numBytes-1 )> gNvmMaxAddress_c
*     gNvmErrInvalidPointer_c        : if pSrc is NULL
*     gNvmErrVerifyError_c           : if at least one location in NVM is different from it's corespondind location in RAM 
*     gNvmErrNoError_c               : in rest
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
* 07.11.2007   MD
* 
************************************************************************************/
extern nvmErr_t NVM_Verify(nvmInterface_t nvmInterface, nvmType_t nvmType ,void *pSrc, uint32_t address, uint32_t numBytes);

/************************************************************************************
* Verifies  numBytes of data from the address address in NVM to be blank .
*   
* Interface assumptions:
*   The address uses only 24 bits.
*   
* Return value:
*   nvmErr
*     gNvmErrInvalidInterface_c      : if nvmInterface is not one of the two valid values(gNvmInternalInterface_c or gNvmExternalInterface_c)
*     gNvmErrInvalidNvmType_c        : if nvmType is not one of the three known values(gNvmType_SST_c,gNvmType_ST_c,gNvmType_ATM_c)
*     gNvmErrAddressSpaceOverflow_c  : if (address +numBytes-1 )> gNvmMaxAddress_c
*     gNvmErrInvalidPointer_c        : if pSrc is NULL
*     gNvmErrBlankCheckError_c       : if at least one addressed location in NVM is not blank(0xff)
*     gNvmErrNoError_c               : in rest
* Revision history:
*   date      Author    Comments
*   ------    ------    --------
* 07.11.2007   MD
* 
************************************************************************************/
extern nvmErr_t NVM_BlankCheck(nvmInterface_t nvmInterface, nvmType_t nvmType , uint32_t address, uint32_t numBytes);




#endif /* _NVM_DRIVER_H_ */
