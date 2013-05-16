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
* Name:         adc_i2c.c
* Author:       Nivis LLC, Srinivasa Matta
* Date:         March 2008
* Description:  This file holds definitions of external ADC (ADS1110) functions
* Changes:
* Revisions:
****************************************************************************************************/

#include "adc_i2c.h"
#include "system.h"
#include "digitals.h"

#if( DEVICE_TYPE == DEV_TYPE_MC13225 )
#ifdef ADC_I2C

// Define HI and LOW  values for i2c bus
#define LO       0
#define HI       1

// Define OP_READ and OP_WRITE
#define OP_WRITE  0
#define OP_READ   1


// Configure pin SCL for input and output
#define SET_SCL_FOR_INPUT()        SELECT_GPIO_AS_INPUT( I2C_SCL )
#define SET_SCL_FOR_OUTPUT()       SELECT_GPIO_AS_OUTPUT( I2C_SCL )

// Configure pin SDA for input and output
#define SET_SDA_FOR_INPUT()         SELECT_GPIO_AS_INPUT( I2C_SDA )
#define SET_SDA_FOR_OUTPUT()        SELECT_GPIO_AS_OUTPUT( I2C_SDA )

// Defines that receive as a parameter the input or output port and return
// the value of the SDA or SCL bit

#define GET_SCL_BIT()          (GPIO_DATA0_P12)//((PORT_I2C_IN & PIN_SCL)) != 0 )
#define GET_SDA_BIT()          (GPIO_DATA0_P13)//((PORT_I2C_IN & PIN_SDA) != 0 )

//    Defines that receive as parameter the value to be set into the SDA and SCL bit
#define SET_SCL_LO()    { SET_GPIO_LO(I2C_SCL);}
#define SET_SCL_HI()    SET_GPIO_HI(I2C_SCL)

#define SET_SDA_LO()    SET_SDA_FOR_OUTPUT()
#define SET_SDA_HI()    SET_SDA_FOR_INPUT()


//#define SET_SCL_INIT() {} //{SET_SCL_HI(); SET_SCL_FOR_OUTPUT(); } //ion ticus
//#define SET_SDA_INIT() {} //{SET_SDA_FOR_INPUT(); SET_GPIO}//{SET_SDA_FOR_INPUT(); PORT_I2C_OUT &=~PIN_SDA;} //avram

#define SET_SDA_BIT(p_chValue)\
{\
  if ( (p_chValue) == 0 ) SET_SDA_LO();  else SET_SDA_HI(); \
}\

//#define delay_nops(x)         // no delay
//#define delay_nops(x) _NOP()  // just one tics

#define DELAY(x) delay_nops(x)   // the procedure used for delay

#define WRITE_TIMEOUT    500     // delay used to check if a write op

#define PERIOD      1000

#define CTRL(p_chAddr, p_chOp)      ( p_chAddr | p_chOp )     // set the R/W bit of the control byte

#define SELECT_GPIO_AS_INPUT1( GPIO ) \
{ \
  if ( (GPIO) < 64 ) \
  { \
    uint8 reg32 = (uint8)(GPIO) / 32; \
    uint8 bit32 = (uint8)(GPIO) % 32; \
    uint8 reg16 = (uint8)(GPIO) / 16; \
    uint8 bit16 = (uint8)(GPIO) % 16; \
    *( (uint32 *)(&_GPIO_PAD_DIR_RESET0) +  ( reg32 * 0x04 ) ) |= ( ((uint32) 0x00000001) << bit32 ); \
    *( (uint32 *)(&_GPIO_FUNC_SEL0) + ( reg16 * 0x04 ) ) &= ~( ((uint32) 0x00000003) << ( bit16 * 2 ) ); \
  } \
}

// Rares
// ADS1110 i2c Address --> please check the part number on your board before changing the i2c address
// (the part number format you should check, has the format: EDn where n = [0..7])
#define ADS1110_ED0   0x90  // 1001 000	= 0x48  = ED0.   0x90 = ED0 shifted left 1
#define ADS1110_ED1   0x92  // 1001 001	= 0x49  = ED1.   0x92 = ED1 shifted left 1
#define ADS1110_ED2   0x94  // 1001 010	= 0x4A  = ED2.   0x94 = ED2 shifted left 1
#define ADS1110_ED3   0x96  // 1001 011	= 0x4B  = ED3.   0x96 = ED3 shifted left 1
#define ADS1110_ED4   0x98  // 1001 100	= 0x4C  = ED4.   0x98 = ED4 shifted left 1
#define ADS1110_ED5   0x9A  // 1001 101	= 0x4D  = ED5.   0x9A = ED5 shifted left 1
#define ADS1110_ED6   0x9C  // 1001 110	= 0x4E  = ED6.   0x9C = ED6 shifted left 1
#define ADS1110_ED7   0x9E  // 1001 111	= 0x4F  = ED7.   0x9E = ED7 shifted left 1
static uint16  g_nAddress = ADS1110_ED5;

void delay_nops(int x)
{
  while (x)
  {
    x--;
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Name: InitI2c
// Author: Avram Ionut, Marius Chile
// Description: Initializes the i2c communication by setting SDA and SCL for output
// Parameters:
// Return:
// Note: !!! MUST BE CALLED before any i2c function call !!!
///////////////////////////////////////////////////////////////////////////////////
void ADCI2cInit()
{
  SET_SDA_LO();
  SET_SCL_LO();
  
  SET_SCL_FOR_OUTPUT();
  SET_SDA_FOR_INPUT();

  DELAY(PERIOD);
  SET_SCL_HI();
  SET_SDA_HI();
}

///////////////////////////////////////////////////////////////////////////////////////////
// Name: start_transaction
// Author: Avram Ionut, Marius Chile
// Description: Generates the start condition necessary before every read or write command.
// Parameters:
// Return:
// Note: The start condition is determined by a HI to LO transition of SDA while SCL is HI
///////////////////////////////////////////////////////////////////////////////////////////
////#pragma optimize=s 9
void i2c_start_transaction()
{
    SET_SCL_FOR_OUTPUT(); // work in push pull

    SET_SCL_LO();    // close scl first - no need delay after //? discutabil
    SET_SDA_HI();    DELAY(PERIOD);
    SET_SCL_HI();    DELAY(PERIOD); // start sck
    SET_SDA_LO();    DELAY(PERIOD);
    SET_SCL_LO();    DELAY(PERIOD);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Name: stop_transaction
// Author: Marius Chile
// Description: Generates the stop condition necessary after every read or write command.
// Parameters:
// Return:
// Note: The stop condition is determined by a LO to HI transition of SDA while SCL is HI
///////////////////////////////////////////////////////////////////////////////////////////
//#pragma optimize=s 9
void i2c_stop_transaction()
{
    SET_SCL_LO();    // close scl first - no need delay after
    SET_SDA_LO();    DELAY(PERIOD); // transition must be from LO to HI

    SET_SCL_HI();    DELAY(PERIOD); // start scl
    SET_SDA_HI();    DELAY(PERIOD); // go to HI from LO

    SET_SCL_FOR_INPUT(); // set in pull up to reduce the consumption
}


///////////////////////////////////////////////////////////////////////////////////
// Name: write_bit
// Author: Avram Ionut, Marius Chile
// Description: Writes a bit on the bus (sets the SDA line to that bit)
// Parameters:  chBit - byte - the bit to be written
// Return:
// Note: The value of the SDA line must be changed during the LO period of the SCL line
///////////////////////////////////////////////////////////////////////////////////
//#pragma optimize=s 9
void i2c_write_bit(byte chBit)
{
    SET_SDA_BIT( chBit );    DELAY(PERIOD);    // set data bit
    SET_SCL_LO();            // delay
    SET_SCL_LO();            // delay
    SET_SCL_HI();            DELAY(PERIOD);
    SET_SCL_LO();            DELAY(PERIOD);
}

///////////////////////////////////////////////////////////////////////////////////
// Name: read_bit
// Author: Avram Ionut, Marius Chile
// Description: Reads a bit from the bus (reads the value of the SDA line)
// Parameters:
// Return: The value of the read bit
// Note: The value of the SDA line is valid (can be read) for the HI duration of SCL
///////////////////////////////////////////////////////////////////////////////////
//#pragma optimize=s 9
byte i2c_read_bit()
{
    byte i;
    SET_SCL_HI();   DELAY(PERIOD); // start scl
    i = GET_SDA_BIT();
    SET_SCL_LO();   DELAY(PERIOD);
    return i;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: write_byte
// Author: Avram Ionut, Marius Chile
// Description: Writes a byte on the bus and checks for acknowledge
// Parameters: ch - byte - the byte to be written
// Return:  1 - byte written and acknowledged
//          0 - written byte not acknowledged
// Note: After the byte is written, the receiving device should post an acknowledge
//       signal represented by a LO period of SDA during the HI period of SCL
///////////////////////////////////////////////////////////////////////////////////
byte i2c_write_byte(byte p_ch)
{
    unsigned int i;
    for( i = 0; i < 8; i++ )
    {
         i2c_write_bit( p_ch & 0x80 );
         p_ch <<= 1;
    }

    SET_SDA_FOR_INPUT();
    return (i2c_read_bit()==0);
}

///////////////////////////////////////////////////////////////////////////////////
// Name: read_byte
// Author: Avram Ionut, Marius Chile
// Description: Reads a byte from the bus
// Parameters:
// Return: The read byte
///////////////////////////////////////////////////////////////////////////////////
byte i2c_read_byte( byte p_chLastByteFlag )
{
    unsigned int i;
    byte chRes;

    SET_SDA_FOR_INPUT();

    for( i = 0; i < 8; i++ )
    {
        chRes = ( chRes << 1 ) | i2c_read_bit();
    }
    i2c_write_bit( p_chLastByteFlag ); //send NACK if it is the last byte
    return chRes;
}


///////////////////////////////////////////////////////////////////////////////////
// Name: WriteADC
// Author: Popeanga Catalin
// Description: Writes a sequence of  bytes
// Parameters: p_pchBuffer - byte* - the buffer from which to write
//
// Return:  The read byte or
//          >=0 operation is OK
//          0xFF - if write operation failed.
///////////////////////////////////////////////////////////////////////////////////
byte WriteADC( byte p_chBuffer )
{
   byte uchRez = 1;

   i2c_start_transaction();

   if( i2c_write_byte(CTRL(g_nAddress, OP_WRITE)) ) i2c_write_byte(  p_chBuffer ) ;
      else uchRez = 0;

   i2c_stop_transaction();

   return uchRez;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: ReadADC
// Author: Popeanga Catalin
// Description: Reads a sequence of bytes
// Parameters:p_pchBuffer  - byte* - the buffer from in which to write
//             p_uchLen     - byte  - the number of characters to be read from memory
// Return: 0 - Fail
//         1 - OK
//
///////////////////////////////////////////////////////////////////////////////////
byte ReadADC( byte *p_pchBuffer, byte p_uchLen )
{
    byte uchRez = 1;

    i2c_start_transaction();

    if(! i2c_write_byte(CTRL(g_nAddress, OP_READ)) )
         {
            uchRez = 0;
         }
    else
        {
           for ( ; p_uchLen; p_pchBuffer ++)
             {
                 // Decrypt data after read
                 *p_pchBuffer = i2c_read_byte( ! (-- p_uchLen) );
             }
        }

    i2c_stop_transaction();

    return uchRez;
}


#endif // ADC_I2C
#endif
