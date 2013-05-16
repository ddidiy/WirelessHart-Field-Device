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
* Name:         adc_i2c.h
* Author:       Nivis LLC, Srinivasa Matta
* Date:         March 2008
* Description:  This file holds definitions of external ADC (ADS1110) functions
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_ADC_I2C_H_
#define _NIVIS_ADC_I2C_H_

#include "system.h"

#if( DEVICE_TYPE == DEV_TYPE_MC13225 )
// ADS1110 VREF is 2048 milivolts. Read the specs 
#define VREF_ADC_ADS1110  2048


///////////////////////////////////////////////////////////////////////////////////
// Name: InitI2c
// Author: Avram Ionut, Marius Chile
// Description: Initializes the i2c communication by setting SDA and SCL for output
// Parameters: 
// Return: 
// Note: !!! MUST BE CALLED before any i2c function call !!! 
/////////////////////////////////////////////////////////////////////////////////// 
#define ADC_I2C_INIT()  ADCI2cInit()
void ADCI2cInit();


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
uint8 ReadADC( byte *p_pchBuffer, byte p_uchLen );


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
uint8 WriteADC( byte p_chBuffer );

#else
#define ADC_I2C_INIT()
#endif

#endif  //_NIVIS_ADC_I2C_H_
