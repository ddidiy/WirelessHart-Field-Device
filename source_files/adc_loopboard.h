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
* Name:         adc_loopboard.h
* Author:       Dorin Muica
* Date:         July 2010
* Description:               
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_ADC_LOOP_BOARD_H_
#define _NIVIS_ADC_LOOP_BOARD_H_

#include "system.h"
#include "digitals.h"

#if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )

  #define I2C_SDA    gGpioPin13_c     
  #define I2C_SCL    gGpioPin12_c  

  #define SET_SCL_NORMAL_MODE()     Gpio_SetPinFunction(I2C_SCL,gGpioNormalMode_c)
  #define SET_SCL_AS_OUTPUT()       SELECT_GPIO_AS_OUTPUT(I2C_SCL)
  #define SET_SCL_HI()              SET_GPIO_HI(I2C_SCL)
  #define SET_SCL_LO()              SET_GPIO_LO(I2C_SCL)
  #define SCL_ENABLE_PULLUP()       { ENABLE_GPIO_PULLUP(I2C_SCL); SET_GPIO_WITH_PULLUP(I2C_SCL); }
  
  
  #define SET_SDA_NORMAL_MODE()     Gpio_SetPinFunction(I2C_SDA,gGpioNormalMode_c)
  #define SET_SDA_AS_OUTPUT()       SELECT_GPIO_AS_OUTPUT(I2C_SDA)
  #define SET_SDA_AS_INPUT()        SELECT_GPIO_AS_INPUT( I2C_SDA )
  #define SET_SDA_HI()              SET_GPIO_HI(I2C_SDA)
  #define SET_SDA_LO()              SET_GPIO_LO(I2C_SDA)
  #define SDA_ENABLE_PULLUP()       { ENABLE_GPIO_PULLUP(I2C_SDA); SET_GPIO_WITH_PULLUP(I2C_SDA); }

  void DelayLoop(uint32);
  #define PERIOD 1
  #define DELAY(x) DelayLoop(x)  // the procedure used for delay

  #define GET_GPIO_VALUE( ucGPIO, state )  Gpio_GetPinData(ucGPIO, state )

  #define ADC_READ          0x01
  #define ADC_ID            (0x90) // 1001000xb
  #define EXT_ADC_VREF      2500U        //2.5 V
#ifdef LOOP_BOARD_VERSION_1
  #define PRE_AMP_FACTOR    25     
#else   
  #define PRE_AMP_FACTOR    20    
#endif
  #define RETRIES_NB 100

  extern uint16 g_aunADC_FILO[4];

  unsigned char i2c_select_device();
  void i2c_start_transaction();
  void i2c_stop_transaction();
  void i2c_write_bit(unsigned char p_ucBit);
  unsigned char i2c_write_byte(unsigned char p_ucByte);
  unsigned char i2c_read_bit();
  unsigned char i2c_read_byte( unsigned char p_chLastByteFlag );
  void i2c_stop_transaction();
  void ADC_Extern_Init(void);
  unsigned char ADC_ReadFILO(unsigned short * p_punBuffer);


#endif /* (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) */

#endif /* _NIVIS_ADC_LOOP_BOARD_H_ */