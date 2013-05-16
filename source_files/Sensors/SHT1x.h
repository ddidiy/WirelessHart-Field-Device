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

/*************************************************************************
* File: SHT1x.h
* Author: Rares Ivan, Nivis LLC
* SHT1x Humidity and Temperature sensor
*************************************************************************/

/*************************************************************************
* High Precision Humidity Sensor SHT15
*
* The SHT15 digital humidity and temperature sensor is fully calibrated and 
* offers high precision and excellent long-term stability at low cost. 
* The digital CMOSens® Technology integrates two sensors and readout circuitry
* on one single chip.

* Features
* - 2 sensors for relative humidity & temperature 
* - Precise dewpoint calculation possible 
* - Measurement range: 0-100% RH 
* - Absol. RH accuracy: +/- 2% RH (10...90% RH) 
* - Repeatability RH: +/- 0.1% RH 
* - Temp. accuracy: +/- 0.3°C @ 25°C 
* - Calibrated & digital output (2-wire interface) 
* - Fast response time < 8 seconds  
* - Low power consumption (typ. 30 µW) 
* - Low cost 
* - High precision sensor at low cost 
* - Leading CMOSens® Technology for superior long-term stability 
* - Very easy-to-use due to calibration & digital 2-wire interface 
*************************************************************************/


#ifndef _NIVIS_SENSORS_SHT1X_H_
#define _NIVIS_SENSORS_SHT1X_H_

#include "../system.h"

#ifndef SHT1X_MODE
  #if ( BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT )
      #define SHT1X_MODE 1
  #else
      #define SHT1X_MODE 0
  #endif
#endif

#if ( SHT1X_MODE != 0 )

    #include "../global.h"

    // SHT State Machine steps (this is a state machine that performs readings from STH sensor. It has nothing to do with SHT sensor internals)
    #define SHT_STATE_BUSY             0
    #define SHT_STATE_DONE             1

    // Sensor readings structure
    typedef struct
    {
       unsigned char ucStatus;
       float fTemperature;
       float fHumidity;
       float fDewPoint;
    } SHT1x_SENSOR_READINGS;


    void SHT1x_PowerOn(void);
    void SHT1x_PowerOff(void);
    
        
    unsigned char SHT1x_ReadData( SHT1x_SENSOR_READINGS* p_pSHT1xSensorReadings);
    
#else
    #define SHT1x_PowerOff()
#endif // SHT1X_MODE

#define SHT1x_INIT() SHT1x_PowerOff()    

#endif // _NIVIS_SENSORS_SHT1X_H_


