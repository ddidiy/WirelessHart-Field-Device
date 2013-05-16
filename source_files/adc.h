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
* Name:         adc.h
* Author:       Dorin Muica
* Date:         September 2009
* Description:               
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_ADC_H_
#define _NIVIS_ADC_H_

#include "MC1322x.h"
#include "digitals.h"
#include "LibInterface/ADC_Interface.h"

#define PRESCALER_24MHZ_TO_1MHZ    ( 23 ) // 24.000.000 / 1.000.000 = 24 // (-1)        // 0 => 1, ..., 23 => 24
#define DIVIDER_24MHZ_TO_300KHZ    ( 80 ) // 24.000.000 /   300.000 = 80 // (NOT -1)    // 0 => adc_clock = off
#define ANALOG_WARMUP_TIME         ( 8 )  // at 1 Mhz => 8 means 8 uS;
#define CONVERT_TIME               ( 20 ) // at 1 Mhz => 20 means 20 uS;

#define ADC_ON                      0x0001
#define ADC_AD2_VREFHL_EN	    0x0020

float Read_Battery_Voltage(void);

#endif /* _NIVIS_ADC_H_ */

