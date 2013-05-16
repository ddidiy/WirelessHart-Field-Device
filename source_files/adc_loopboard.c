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

#include "adc_loopboard.h"

#if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )

uint16 g_aunADC_FILO[4];

void ADC_Extern_Init(void)
{
  // set SCL and SDA pins controlled by GPI0 module, high level, output direction
  SET_SCL_NORMAL_MODE(); 
  SET_SCL_HI();
  SET_SCL_AS_OUTPUT();
  
  SET_SDA_NORMAL_MODE(); 
  SET_SDA_HI();
  SDA_ENABLE_PULLUP(); // for input only (read ack)
  SET_SDA_AS_OUTPUT();
}

///////////////////////////////////////////////////////////////////////////////////////////
// Name: i2c_start_transaction
// Author: Nivis LLC, Dorin Muica
// Description: Generates the start condition necessary before every read or write command.
// Parameters: none
// Return: none
// Note: The start condition is determined by a HI to LO transition of SDA while SCL is HI
///////////////////////////////////////////////////////////////////////////////////////////
void i2c_start_transaction()
{
    SET_SCL_LO();    DELAY(PERIOD);
    SET_SDA_HI();    DELAY(PERIOD);
    SET_SCL_HI();    DELAY(PERIOD); // start sck
    SET_SDA_LO();    DELAY(PERIOD); 
    SET_SCL_LO();    DELAY(PERIOD);
}

///////////////////////////////////////////////////////////////////////////////////
// Name: i2c_write_bit
// Author: Nivis LLC, Dorin Muica
// Description: Writes a bit on the bus (sets the SDA line to that bit)
// Parameters:  p_ucBit - unsigned char - the bit to be written
// Return: 
// Note: The value of the SDA line must be changed during the LO period of the SCL line
/////////////////////////////////////////////////////////////////////////////////// 
void i2c_write_bit(unsigned char p_ucBit)
{
  if(p_ucBit) // set data bit
  {
    SET_SDA_HI();
  }
  else 
  {
    SET_SDA_LO();
  }
  DELAY(PERIOD);    
  SET_SCL_LO();            // delay
  SET_SCL_LO();            // delay
  SET_SCL_HI();            DELAY(PERIOD);  
  SET_SCL_LO();            DELAY(PERIOD);  
}


///////////////////////////////////////////////////////////////////////////////////
// Name: read_bit
// Author: Nivis LLC, Dorin Muica
// Description: Reads a bit from the bus (reads the value of the SDA line)
// Parameters: 
// Return: the value of the read bit
// Note:   the value of the SDA line is valid (can be read) for the HI duration of SCL
/////////////////////////////////////////////////////////////////////////////////// 
unsigned char i2c_read_bit()
{
    GpioPinState_t i;
    SET_SCL_HI();   DELAY(PERIOD); // start scl
    GET_GPIO_VALUE(I2C_SDA,&i);   
    SET_SCL_LO();   DELAY(PERIOD);
    return (unsigned char)i;
}
///////////////////////////////////////////////////////////////////////////////////
// Name: write_byte
// Author: Nivis LLC, Dorin Muica
// Description: Writes a byte on the bus and checks for acknowledge
// Parameters: p_ucByte - unsigned char - the byte to be written
// Return:  1 - byte written and acknowledged
//          0 - written byte not acknowledged
// Note: After the byte is written, the receiving device should post an acknowledge
//       signal represented by a LO period of SDA during the HI period of SCL
/////////////////////////////////////////////////////////////////////////////////// 
unsigned char i2c_write_byte(unsigned char p_ucByte)
{
    unsigned int i;
        
    for( i = 0; i < 8; i++ )
    {
         i2c_write_bit( p_ucByte & 0x80 );
         p_ucByte <<= 1;
    }
         
    SET_SDA_AS_INPUT();
    return (i2c_read_bit()==0);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Name: i2c_select_device
// Author: Nivis LLC, Dorin Muica
// Description: Select a device and wait for ACK
// Parameters: None
// Return:    1 - the device is sellected
//            0 - the device doesn't responds
////////////////////////////////////////////////////////////////////////////////////////////
unsigned char i2c_select_device(p_ucAddr)
{
    unsigned int i = (RETRIES_NB + 1);
      
    i2c_stop_transaction(); 
    while( -- i )
    {
      i2c_start_transaction();
      if( i2c_write_byte(p_ucAddr) )
          return 1;
      DELAY(PERIOD);    
    }
      
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Name: i2c_read_byte
// Author: Nivis LLC, Dorin Muica
// Description: Reads a byte from the bus
// Parameters:  
// Return: The read byte
/////////////////////////////////////////////////////////////////////////////////// 
unsigned char i2c_read_byte( unsigned char p_ucLastByteFlag )
{
    unsigned int i;
    unsigned char ucRes;
    
    SET_SDA_AS_INPUT();
    
    for( i = 0; i < 8; i++ )
    {               
        ucRes = ( ucRes << 1 ) | i2c_read_bit();
    }
    
    SET_SDA_AS_OUTPUT();
    i2c_write_bit( p_ucLastByteFlag ); //send NACK if it is the last byte
    return ucRes;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Name: i2c_stop_transaction
// Author: Nivis LLC, Dorin Muica
// Description: Generates the stop condition necessary after every read or write command.
// Parameters:
// Return:
// Note: The stop condition is determined by a LO to HI transition of SDA while SCL is HI
///////////////////////////////////////////////////////////////////////////////////////////
void i2c_stop_transaction()
{
    SET_SCL_LO();    DELAY(PERIOD); 
    SET_SDA_LO();    DELAY(PERIOD); // transition must be from LO to HI
    
    SET_SCL_HI();    DELAY(PERIOD); // start scl
    SET_SDA_HI();    DELAY(PERIOD); // go to HI from LO
 }


///////////////////////////////////////////////////////////////////////////////////////////
// Name: DAC_ReadFILO()
// Author: Dorin Muica
// Description: Read internal 4-word first-in last-out buffer (FILO) that stores the results of up to 4 conversions
//              while they are waiting to be read out over the I2C bus.
// Parameters: p_punBuffer - pointer to memory location where data will be stored
// Return:     1 - success
//             0 - fail
// Note:       for standard mode (100kHz clock rate) and fast mode (400kHz clock rate) only
////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ADC_ReadFILO(unsigned short * p_punBuffer)
{
  unsigned char ucLength;
  if(!i2c_select_device(ADC_ID))
    return 0;
  
  SET_SDA_AS_OUTPUT();
  if( !i2c_write_byte(0) ) return 0;
  
  SET_SDA_AS_OUTPUT();
  i2c_start_transaction(); // repeated START Condition  -must be tested !?!?
  if( !i2c_write_byte(ADC_ID | ADC_READ) ) return 0;

  ucLength = sizeof(g_aunADC_FILO);
  do
  {
    *(((unsigned char *)p_punBuffer) + 1 ) = i2c_read_byte(!(--ucLength));
    *(((unsigned char *)p_punBuffer)     ) = i2c_read_byte(!(--ucLength));
    ++p_punBuffer;
  }while(ucLength);
  
  i2c_stop_transaction();
  return 1;
}

#endif /*  (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) */
