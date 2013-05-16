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

#include "../digitals.h"
#include "SHT1x.h"


// How often to read ADC, SHT
#define DATA_READ_INTERVAL_FAST  EVERY_5SEC
#define DATA_READ_INTERVAL_SLOW  EVERY_15SEC
#define DATA_READ_INTERVAL       DATA_READ_INTERVAL_SLOW


#define CRM_SET_SHT() (g_ucShtReadRequest ? 0x08 : 0x00) // KYB3_LINE

extern uint8 g_ucShtReadRequest;

void SensorTask();