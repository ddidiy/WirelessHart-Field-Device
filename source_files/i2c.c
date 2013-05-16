/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/***************************************************************************************************
* Name:         i2c.c
* Author:       Dorin Muica
* Date:         September 2009
* Description:  
*           
* Changes:
* Revisions:
****************************************************************************************************/

#include "i2c.h"

#define EEPROM_SLAVE_ADDRESS    ( 0xA0 )
#define CONTROL_BYTE_WRITE      ( 0xA0 )
#define CONTROL_BYTE_READ       ( 0xA1 )
#define GPIO_EEPROM_WP           gGpioPin6_c // !!! must change this gpio when HW is ok
#define EEPROM_WRITE_ENABLE()    Gpio_SetPinData(GPIO_EEPROM_WP, gGpioPinStateLow_c)
#define EEPROM_WRITE_DISABLE()   Gpio_SetPinData(GPIO_EEPROM_WP, gGpioPinStateHigh_c)
#define EEPROM_PAGE_SIZE        ( 128 )
#define FAILURE_RETRIES         ( 500 )

I2cConfig_t stI2cConfig = {
  0xEE,  // slave address of MC1322x
  0x20,  // 150 Kbs maximum supported by MC1322x
  0x10,  // 
  0,     // Interrupt enable - no
  0      // Broadcast enable - no
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// Name     I2C_EEPROM_Init
// Author   Dorin Muica
// Brief    Init I2C Module
// Param    none
//          none
// Return   none 
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_EEPROM_Init (void)
{
  Gpio_SetPinFunction(GPIO_EEPROM_WP , gGpioNormalMode_c);  
  Gpio_SetPinDir( GPIO_EEPROM_WP, gGpioDirOut_c );
  EEPROM_WRITE_DISABLE();
  
  Gpio_SetPinFunction( gGpioPin12_c, gGpioAlternate1Mode_c ); // Peripheral Control Mode of pin #22 SCL
  Gpio_SetPinFunction( gGpioPin13_c, gGpioAlternate1Mode_c ); // Peripheral Control Mode of pin #21 SDA
  
  I2c_Init();
  I2c_Enable();
  I2c_SetConfig(&stI2cConfig);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Name     I2C_WriteMem
// Author   Nivis LLC, Dorin Muica
// Brief    write data to EEPROM memory
// Param    p_pcAddrAndData -  is formated as Addr HI byte, Addr LO byte, Data bytes
//          p_nAddrAndDataLen -  length of p_pcAddrAndData 2bytes+Effective Data bytes
// Return   0 if fail, !=0 if success
// Remarks  p_pcAddrAndData is formated as Addr HI, Addr LO, Data
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 I2C_WriteMem(uint8 *p_pcAddrAndData, uint16 p_nAddrAndDataLen ) // NOT TESTED
{
  if(p_nAddrAndDataLen > 2)
  {
    uint16 ulAddress;
    uint16 unCnt;
    
    ulAddress = ((uint16)(p_pcAddrAndData[0]) << 8) | p_pcAddrAndData[1];
    
    p_nAddrAndDataLen -= 2;
    
    EEPROM_WRITE_ENABLE();
    do
    {
      uint8  acMem[2];
      uint16 unWriteSize = EEPROM_PAGE_SIZE - (ulAddress & (EEPROM_PAGE_SIZE-1));
      
      if( unWriteSize > p_nAddrAndDataLen )
      {
        unWriteSize = p_nAddrAndDataLen;
      }
      
      acMem[0] = p_pcAddrAndData[0]; // save previous data
      acMem[1] = p_pcAddrAndData[1];
      
      p_pcAddrAndData[0] = (uint8)(ulAddress >> 8);
      p_pcAddrAndData[1] = (uint8)(ulAddress);
          
      unCnt =  FAILURE_RETRIES ;
      do
      {
        if( I2c_SendData(CONTROL_BYTE_WRITE, p_pcAddrAndData, unWriteSize+2, gI2cMstrReleaseBus_c ) == gI2cErrNoError_c )
        {
          p_nAddrAndDataLen -= unWriteSize;
          break;
        }
      }while(--unCnt);
      
      p_pcAddrAndData[0] = acMem[0]; //restore previous data
      p_pcAddrAndData[1] = acMem[1];
      p_pcAddrAndData += unWriteSize;
      ulAddress += unWriteSize;
      
    }while( p_nAddrAndDataLen && unCnt );
    
    
    while (I2CSR_MBB)  // wait until bus is idle
      ;
    EEPROM_WRITE_DISABLE();
  }
  return (p_nAddrAndDataLen == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Name      I2C_ReadMem  
// Author    NIVIS LLC,  Dorin Muica
// Brief     Read data from EEPROM memory using library functions
// Param     p_unAddress -  address of EEPROM  location from where data will be read
///          p_pcDest    -  pointer to a memory location where data will be sent
///          p_nDestLen  -  length of data
/// Return   0 if fail, !=0 if success
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 I2C_ReadMem(uint16 p_unAddress, uint8 *p_pcDest, uint16 p_nDestLen)  // NOT TESTED
{
  uint16  unCnt;
  uint8 aMemAddr[2];
  uint8 ucResult = 0;
  
  aMemAddr[0] = (uint8)(p_unAddress >> 8);
  aMemAddr[1] = (uint8)(p_unAddress);
  
  unCnt = FAILURE_RETRIES;
  while ( --unCnt )
  {
      if( I2c_SendData(CONTROL_BYTE_WRITE, aMemAddr, 2, gI2cMstrHoldBus_c ) == gI2cErrNoError_c )
          break;
  }
  
  if( I2c_ReceiveData(CONTROL_BYTE_READ, p_pcDest, p_nDestLen, gI2cMstrReleaseBus_c ) == gI2cErrNoError_c )
  {
      ucResult = 1;
  }
  
  return (ucResult);
}

// [dorin.muica] ---Write persistent data to 4 sectors from internal flash NVM
#include "../NVM.h"
#include "../spif_interface.h"


NvmErr_t BLANK_CHECK(uint32 addr, uint32 numBytes);

NvmErr_t BLANK_CHECK(uint32 addr, uint32 numBytes)
{
  return NvmBlankCheck(gNvmInternalInterface_c, gNvmType_SST_c, addr, numBytes );
}

#define SECTOR_SIZE (4*1024)

#define PERSISTENT_SECTOR_IDX  (5)  // must ask this value !!!!
#define NVM_PERSISTENT_SECTORS (4)

#define PERSISTENT_SECTOR_NO   (1UL << PERSISTENT_SECTOR_IDX) 
#define PERSISTENT_START_ADDR  (SECTOR_SIZE * PERSISTENT_SECTOR_IDX)

#define PERSISTENT_BLOCK_SIZE (256)
#define PERSISTENT_BLOCKS_SECTOR (SECTOR_SIZE/PERSISTENT_BLOCK_SIZE)

#define DELETION_MARK_ERASE_BIT (1UL << 31)

typedef struct
{
  uint8 m_ucGenericData[PERSISTENT_BLOCK_SIZE];
}PERSISTENT_DATA_T;

void SavePersistentData( uint8 *p_pucSourceData, uint16 p_unLength );
uint8 ReadLastPersistentData(void );

///////////////////////////////////////////////////////////////////////////////////
// Name:          SavePersistentData
// Author:        Dorin Muica
// Description:   Save persistent data structures to 4 sectors from internal flash memory (NVM)
// Parameters:    uint8 *p_pucSourceData = pointer to data structure ; 
//                uint16 p_unLength = length of data
// Return:        none
// Note:          
///////////////////////////////////////////////////////////////////////////////////
PERSISTENT_DATA_T g_stPersistentData;
uint8 g_ucPersistDataIdx = 0;
uint32 g_ulDeletionMark = 0;
void SavePersistentData( uint8 *p_pucSourceData, uint16 p_unLength )
{
  switch(g_ucPersistDataIdx)
  {
  case (0*PERSISTENT_BLOCKS_SECTOR):
    g_ulDeletionMark |= (PERSISTENT_SECTOR_NO << 1);
    break;
  case (1*PERSISTENT_BLOCKS_SECTOR):
    g_ulDeletionMark |= (PERSISTENT_SECTOR_NO << 2);
    break;
  case (2*PERSISTENT_BLOCKS_SECTOR):
    g_ulDeletionMark |= (PERSISTENT_SECTOR_NO << 3);
    break;
  case (3*PERSISTENT_BLOCKS_SECTOR):
    g_ulDeletionMark |= (PERSISTENT_SECTOR_NO << 0);
    break;
  default:
    ;     // nothing to do
  }
  //DeleteNextSector();
  WRITE_FLASH((uint8 *)&g_stPersistentData, PERSISTENT_START_ADDR + g_ucPersistDataIdx*PERSISTENT_BLOCK_SIZE, p_unLength );
  g_ucPersistDataIdx++;
  if(g_ucPersistDataIdx >= NVM_PERSISTENT_SECTORS*PERSISTENT_BLOCKS_SECTOR )
  {
    g_ucPersistDataIdx = 0;
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Name:          ReadLastPersistentData
// Author:        Dorin Muica
// Description:   Read last data block written to NVM
// Parameters:    none
// Return:        0 = at least one entry was found
//                1 = the interest zone of NVM (4 sectors) was not been written yet  
// Note:          
///////////////////////////////////////////////////////////////////////////////////

uint8 ReadLastPersistentData(void )
{ 
  if(gNvmErrNoError_c == BLANK_CHECK(PERSISTENT_START_ADDR , SECTOR_SIZE))// first sector blank
  {
    if(gNvmErrNoError_c == BLANK_CHECK(PERSISTENT_START_ADDR + (NVM_PERSISTENT_SECTORS - 1) * SECTOR_SIZE, SECTOR_SIZE)) // 4th sector blank
    {
      return 1;
    }
    else // 4th sector not blank
    {
      g_ucPersistDataIdx = NVM_PERSISTENT_SECTORS * PERSISTENT_BLOCKS_SECTOR - 1 ; 
    }    
  }
  else // first sector not blank
  {
    if(gNvmErrNoError_c == BLANK_CHECK(PERSISTENT_START_ADDR + (NVM_PERSISTENT_SECTORS - 3) * SECTOR_SIZE, SECTOR_SIZE)) // 2nd sector blank
    {
      g_ucPersistDataIdx = ( NVM_PERSISTENT_SECTORS - 3 )* PERSISTENT_BLOCKS_SECTOR - 1;
    }
    else // 2nd sector not blank
    {
      g_ucPersistDataIdx = ( NVM_PERSISTENT_SECTORS - 1 )* PERSISTENT_BLOCKS_SECTOR - 1;   
    }
  } 
  for( ; ; )
  {
    FEED_WDT() ;
    if(gNvmErrBlankCheckError_c == BLANK_CHECK(PERSISTENT_START_ADDR + g_ucPersistDataIdx * PERSISTENT_BLOCK_SIZE, PERSISTENT_BLOCK_SIZE ) )
    {
      READ_FLASH((uint8 *)&g_stPersistentData, PERSISTENT_START_ADDR + g_ucPersistDataIdx * PERSISTENT_BLOCK_SIZE, sizeof(g_stPersistentData));
      g_ucPersistDataIdx++;
      if(g_ucPersistDataIdx >= NVM_PERSISTENT_SECTORS*PERSISTENT_BLOCKS_SECTOR )
      {
        g_ucPersistDataIdx = 0;
      }
      break;
    } 
    --g_ucPersistDataIdx;
  }
  return 0;
}


void DeleteNextSector(void)
{
  if(g_ulDeletionMark)
  {
    ERASE_FLASH(g_ulDeletionMark);
    g_ulDeletionMark = 0;
  }
}