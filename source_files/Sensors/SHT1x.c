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
* File: SHT1x.c
* Author: Rares Ivan, Nivis LLC
* SHT1x Humidity and Temperature sensor
*  Driver file
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
*
* After power-up the sensor needs 11ms to get to Sleep State. No commands must
* be sent before that time.
*************************************************************************/



#include "SHT1x.h"

#if ( SHT1X_MODE != 0 )

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../digitals.h"



#define _NOP() asm("nop;")

// Basic pin operations
#define SHT1X_CLK_AS_OUTPUT()       SELECT_GPIO_AS_OUTPUT( SHT_CLK )
#define SHT1X_CLK_AS_INPUT()        SELECT_GPIO_AS_INPUT(  SHT_CLK )
#define SHT1X_CLK_LO()              SET_GPIO_LO(           SHT_CLK )
#define SHT1X_CLK_HI()              SET_GPIO_HI(           SHT_CLK )
#define SHT1X_CLK_ENABLE_PULLUP()   ENABLE_GPIO_PULLUP(    SHT_CLK )
#define SHT1X_CLK_DISABLE_PULLUP()  DISABLE_GPIO_PULLUP(   SHT_CLK )

#define SHT1X_DATA_AS_OUTPUT()      SELECT_GPIO_AS_OUTPUT( SHT_DATA )
#define SHT1X_DATA_AS_INPUT()       SELECT_GPIO_AS_INPUT(  SHT_DATA )
#define SHT1X_DATA_LO()             SELECT_GPIO_AS_OUTPUT( SHT_DATA )
#define SHT1X_DATA_HI()             SELECT_GPIO_AS_INPUT(  SHT_DATA )
#define SHT1X_DATA_ENABLE_PULLUP()  { ENABLE_GPIO_PULLUP(  SHT_DATA ); SET_GPIO_WITH_PULLUP( SHT_DATA ); } 
#define SHT1X_DATA_DISABLE_PULLUP() DISABLE_GPIO_PULLUP(   SHT_DATA )
#define SHT1X_DATA_IN_STATUS()      GET_LO_GPIO_VALUE(     SHT_DATA )

#define SHT1X_POWER_ON()            SET_GPIO_HI( SHT_PWR_PIN )
#define SHT1X_POWER_OFF()           SET_GPIO_LO( SHT_PWR_PIN )

#define TEMPERATURE 1
#define HUMIDITY    2

#define NOACK 0
#define ACK   1

 //
 // SHT 1x Commands
 //                                       adr  cmd  r/w
 #define CMD_RESET               0x1E  // 000  1111 0
 #define CMD_GET_TEMPERATURE     0x03  // 000  0001 1
 #define CMD_GET_HUMIDITY        0x05  // 000  0010 1
 #define CMD_SET_STATUS          0x06  // 000  0011 0
 #define CMD_GET_STATUS          0x07  // 000  0011 1

 #define STATUS_LOW_RESOLUTION   0x01  // Temperature 12 Bit, Humidity 8 bit
 #define STATUS_HIGH_RESOLUTION  0x00  // Temperature 14 Bit, Humidity 12 bit
    
typedef enum {
        SHT_STATE_IDLE             ,
        SHT_STATE_TEMPERATURE_START,  // Tell SHT to measure Temperature
        SHT_STATE_TEMPERATURE_GET  ,  // Read Temperature raw value from SHT
        SHT_STATE_HUMIDITY_GET        // Read Humidity raw value from SHT
} SHT_STATES;

void SHT1x_ConnectionReset(void);
unsigned char SHT1x_WriteByte(unsigned char value);
void SHT1x_SendCommand(unsigned char ucCmd);
unsigned int SHT1x_GetResult(void);
void SHT1x_Calculate(SHT1x_SENSOR_READINGS* p_stSensorReadingsResults, unsigned int unRawTemperature, unsigned int unRawHumidity);

///////////////////////////////////////////////////////////
// Function: SHT1x_CalculateCRC
// Author: Rares Ivan
// Purpose: SHT1x CRC checksum calculation
// Parameters:None
// Returns: None
///////////////////////////////////////////////////////////
void SHT1x_CalculateCRC()
{
  // To BE Implemented
}


///////////////////////////////////////////////////////////
// Function: SHT1x_PowerOn
// Author: Rares Ivan
// Purpose: SHT1x power on
// Parameters:None
// Returns: None
///////////////////////////////////////////////////////////
void SHT1x_PowerOn(void)
{
  SHT1X_DATA_LO();
  SHT1X_DATA_AS_OUTPUT();
  SHT1X_DATA_ENABLE_PULLUP();
  
  SHT1X_CLK_LO();
  SHT1X_CLK_AS_OUTPUT();  

  SHT1X_POWER_ON();
  
  // SHT1x : Start up Sensor
  //  As a first step the sensor is powered up to chosen supply voltage VDD. 
  //  The slew rate during power up shall not fall below 1V/ms. 
  //  After power-up the sensor needs !!!  11 ms  !!! to get to Sleep State. 
  //  No commands must be sent before that time.
}

///////////////////////////////////////////////////////////////////////////////////
// Name: SHT1x_PowerOff
// Description: SHT1x Power OFF to minimize power consumption. Module is disabled.
//              All pins used by this module are set to a low power mode
// Parameters: none 
// Return: none
//
// Note: SHT1x's power supply is controlled by other module
//
///////////////////////////////////////////////////////////////////////////////////
void SHT1x_PowerOff(void)
{
  // SHT1x does not have a command for sleep mode. SHT1x goes automatically into sleep mode if not used for ~11 ms
  // or after conversion results are read out.
  // Note that SHT1x may be powered off by other module.  
  SHT1X_CLK_LO();
  SHT1X_CLK_AS_INPUT();
    
  SHT1X_DATA_AS_INPUT();      // release the bus
  SHT1X_DATA_DISABLE_PULLUP();

  SHT1X_POWER_OFF();
}

///////////////////////////////////////////////////////////
// Function: SHT1x_WriteByte
// Author: Rares Ivan
// Purpose: writes a byte on the Sensibus and checks the acknowledge
// Parameters: byte to write
// Returns: error
///////////////////////////////////////////////////////////
unsigned char SHT1x_WriteByte(unsigned char value)
{
  unsigned char ucError;
  unsigned int  i;

  SHT1X_DATA_AS_OUTPUT();

  for (i = 0x80; i > 0; i /= 2)   //shift bit for masking
  {
    if (i & value)
      SHT1X_DATA_HI();            // masking value with i , write to SENSI-BUS
    else
      SHT1X_DATA_LO();

    SHT1X_CLK_HI();                         // clk for SENSI-BUS
    _NOP(); _NOP(); _NOP();  _NOP();       // pulswith approx. 5 us  	
    SHT1X_CLK_LO();
    _NOP(); _NOP(); _NOP();  _NOP();
  }

  //SHT1X_DATA_HI();
  SHT1X_DATA_AS_INPUT();          // release DATA-line

  SHT1X_CLK_HI();                 // clk #9 for ack
  _NOP(); _NOP(); _NOP();  _NOP();
  ucError = SHT1X_DATA_IN_STATUS(); // check ack (DATA will be pulled down by SHT1x or remain HI because of pull-up resistor)
  SHT1X_CLK_LO();
  _NOP(); _NOP(); _NOP();  _NOP();

//  SHT1X_DATA_HI();
//  SHT1X_DATA_AS_OUTPUT();          // release DATA-line

  return ucError;                   // error = 1 in case of no acknowledge
}

///////////////////////////////////////////////////////////
// Function: SHT1x_ReadByte
// Author: Rares Ivan
// Purpose: reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
// Parameters: ack or not ack
// Returns: read byte value
///////////////////////////////////////////////////////////
unsigned char SHT1x_ReadByte(unsigned char ack)
{
  unsigned char i, val = 0;

  SHT1X_DATA_AS_INPUT();             // release DATA-line

  for (i = 0x80; i > 0; i /= 2)   // shift bit for masking
  {
    SHT1X_CLK_HI();               // clk for SENSI-BUS
    _NOP(); _NOP(); _NOP();  _NOP();
    if ( SHT1X_DATA_IN_STATUS() )
      val = (val | i);            // read bit
    SHT1X_CLK_LO();
    _NOP(); _NOP(); _NOP();  _NOP();
  }

  if (ack)
  {
    SHT1X_DATA_LO();      // in case of "ack==1" pull down DATA-Line    
    SHT1X_DATA_AS_OUTPUT();
  }  
  
  SHT1X_CLK_HI();         // clk for SENSI-BUS
  _NOP(); _NOP(); _NOP();  _NOP();// pulswith approx. 5 us  	
  SHT1X_CLK_LO();

  _NOP(); _NOP(); _NOP();  _NOP();

  SHT1X_DATA_HI();
  SHT1X_DATA_AS_INPUT();          // release DATA-line

  return val;
}

///////////////////////////////////////////////////////////
// Function: SHT1x_WaitForResponse
// Author: Rares Ivan
// Purpose: wait for SHT1x sensor to finish its measurement job
// Parameters:
// Returns: 0  = timeout error
//          >0 = data ready
///////////////////////////////////////////////////////////
unsigned long SHT1x_WaitForResponse(void)
{
  unsigned long i = 10000; // 10 ms wait ...

  SHT1X_DATA_AS_INPUT(); // release DATA-line

  // wait until sensor has finished the measurement or timeout
  while ( SHT1X_DATA_IN_STATUS() && (i--) )
      ;
  
  return i;
}


///////////////////////////////////////////////////////////
// Function: SHT1x_ConnectionReset
// Author: Rares Ivan
// Purpose: communication reset: DATA-line=1 for at least 9 CLK cycles, followed by transaction start
// Parameters: None
// Returns: None
// Note:
//             _    _    _    _    _    _    _    _    _        ___     ___
//    SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
//          _____________________________________________________         ________
//    DATA:                                                      |_______|
//
//          |--------------- 9 CLK cycles ----------------|-- start transaction --|
//
///////////////////////////////////////////////////////////
void SHT1x_ConnectionReset(void)
{
  unsigned char i;

  // Initial state
  //SHT1X_CLK_LO();
  //SHT1X_CLK_AS_OUTPUT();
  SHT1X_DATA_HI();
  //SHT1X_DATA_AS_OUTPUT();

  // 9 CLK cycles
  for(i = 0; i < 9; i++)
  {
    SHT1X_CLK_HI();
    _NOP(); _NOP(); _NOP();  _NOP();
    SHT1X_CLK_LO();
  }
  
  // start transaction
  _NOP();
  _NOP();
  SHT1X_CLK_HI();
  _NOP();
  _NOP();
  SHT1X_DATA_LO();
  _NOP();
  _NOP(); 
  SHT1X_CLK_LO();
  _NOP(); _NOP(); _NOP(); _NOP();
  SHT1X_CLK_HI();
  _NOP(); _NOP();
  SHT1X_DATA_HI();
  _NOP(); _NOP();
  SHT1X_CLK_LO();
}

///////////////////////////////////////////////////////////
// Function: SHT1x_SendCommand()
// Author: Rares Ivan
// Purpose: Send a command to the SHT sensor
// Parameters: ucCmd = SHT Command: see SHT Commands in SHT1x.h 
// Returns:
//
// The duration of the Temperature conversion depends on the resolution used:
//   12 bit - 80 milliSeconds
//   14 bit - 320 milliSeconds
//
// The duration of the Humidity conversion depends on the resolution used:
//    8 bit - 20 milliSeconds
//   12 bit - 80 milliSeconds
///////////////////////////////////////////////////////////
void SHT1x_SendCommand(unsigned char ucCmd)
{
  //#warning "SHT1x_SendCommand: why SHT1x_connectionReset() here? Make sure Start Transaction is not included.."  
  SHT1x_ConnectionReset();

  //error = SHT1x_WriteByte(ucCmd);
  (void)SHT1x_WriteByte(ucCmd);
}

///////////////////////////////////////////////////////////
// Function: SHT1x_GetResult()
// Author: Rares Ivan
// Purpose: Get the raw result of a Conversion
// Parameters:
// Returns: raw conversion result
///////////////////////////////////////////////////////////
unsigned int SHT1x_GetResult(void)
{
  //unsigned char error    = 0;
  //unsigned char checksum = 0;
  unsigned int unResult = 0xFFFF;

  if ( SHT1x_WaitForResponse() )
  {
    unResult = SHT1x_ReadByte(ACK);
    unResult = (unResult << 8) | SHT1x_ReadByte(NOACK);

    //checksum = (unsigned char)SHT1x_ReadByte(NOACK);
//    (void)SHT1x_ReadByte(NOACK);
  }
  return unResult;
}

///////////////////////////////////////////////////////////
// Function: SHT1x_SetStatusReg()
// Author: Rares Ivan
// Purpose: Write the Status Register
// Parameters: ucStatusReg
// Returns:
///////////////////////////////////////////////////////////
#pragma inline
void SHT1x_SetStatusReg(unsigned char ucStatusReg)
{
  SHT1x_ConnectionReset();

  // Set status register
  //error = SHT1x_WriteByte(CMD_SET_STATUS);
  (void)SHT1x_WriteByte(CMD_SET_STATUS);
  (void)SHT1x_WriteByte(ucStatusReg);
}


///////////////////////////////////////////////////////////
// Function: SHT1x_Calculate
// Author: Rares Ivan
// Purpose: calculates temperature [°C] and humidity [%RH]
//          input :  humidity
//                   temperature
//          output:  humidity [%RH]
//                   temperature [°C]
//                   dew point [°C]
// Parameters:
// Returns:
///////////////////////////////////////////////////////////
void SHT1x_Calculate(SHT1x_SENSOR_READINGS* p_stSensorReadings, unsigned int unRawTemperature, unsigned int unRawHumidity)
{
  // Temperature Conversion Formula
  //  T = d1 + d2 * SOt
  //
  //  VDD | d1 [°C] | d1 [°F] |     | SOt   | d2 [°C] | d2 [°F] |
  // -----|---------|---------|     |-------|---------|---------|
  //   5V | -40.00  | -40.00  |     | 14bit | 0.01    | 0.018   |
  //   4V | -39.75  | -39.55  |     | 12bit | 0.04    | 0.072   |
  // 3.5V | -39.66  | -39.39  |
  //   3V | -39.60  | -39.28  |
  // 2.5V | -39.55  | -39.19  |
  //
  // Note: SOt is temp param (the raw reading)
  //
  const float d1 = -39.60; // [°C]
  const float d2 = 0.04;   // [°C]

  // Humidity Linear Conversion Formula
  // RHlinear = c1 + c2 * SOrh + c3 * (SOrh * SOrh)
  //
  // SOrh  | c1   | c2     | c3              |
  // ------|------|--------|-----------------|
  // 12bit | -4.0 | 0.0405 | (-2.8)/1000,000 |
  // 8 bit | -4.0 | 0.648  | (-7.2)/10,000   |
  //
  // Note: SOrh is humid param (the raw reading)
  //
  // Humidity Temperature Compensated Conversion Formula
  // RHtrue = (T[°C] - 25) * (T1 + T2 * SOrh) + RHlinear
  //
  // SOrh  | t1   | t2      |
  // ------|------|---------|
  // 12bit | 0.01 | 0.00008 |
  // 8 bit | 0.01 | 0.00128 |
  const float c1 = -4.0;
  const float c2 = +0.648;
  const float c3 = (-7.2)/10000;
  const float t1 = +0.01;
  const float t2 = +0.00128;

  float rh_lin;  // rh_lin  =  Humidity linear
  float rh_true; // rh_true = Temperature compensated humidity
  float t_C;     // t_C     =  Temperature [°C]
  float logEx;

  float rh = (float)unRawHumidity;
  float t  = (float)unRawTemperature;

  t_C     = d1 + d2 * t;                          // calculate temperature from ticks to [°C]
  rh_lin  = c1 + c2 * rh + c3 * (rh * rh);        // calculate humidity from ticks to [%RH]
  rh_true = (t_C - 25) * (t1 + t2 * rh) + rh_lin; // calculate temperature compensated humidity [%RH]

  // cut if the value is outside of the physical possible range
  if (rh_true > 100)
    rh_true=100;
  if (rh_true < 0.1)
    rh_true=0.1;

  p_stSensorReadings->fTemperature = t_C;     //return temperature [°C]
  p_stSensorReadings->fHumidity    = rh_true; //return humidity[%RH]

  // calculate dew point [°C]
  logEx = ( log10(rh_true) - 2 ) / 0.4343 + ( 17.62 * t_C ) / ( 243.12 + t_C );
  p_stSensorReadings->fDewPoint = ( 243.12 * logEx ) / ( 17.62 - logEx );
}

///////////////////////////////////////////////////////////
// Function: SHT1x_ReadData
// Author: Rares Ivan
// Purpose: Executes a given step in the SHT State Machine or cycles between steps starting with first
// Parameters:
// Returns: pointer to structure of temperature [°C], humidity [%RH], dew point [°C]
//
// Note: For a complete reading cycle and calculation of temperature, humidity and dew point, call this 
//       function in a loop until the return code is SHT_STATE_DONE
//       See function SHT1x_ReadDataNow()
///////////////////////////////////////////////////////////
unsigned char SHT1x_ReadData( SHT1x_SENSOR_READINGS* p_pSHT1xSensorReadings )
{
  static unsigned char s_ucState= SHT_STATE_IDLE;
  static unsigned int  s_unRawTemperature;
    
  switch(s_ucState)
  {
    case SHT_STATE_IDLE:
            SHT1x_PowerOn();
            s_ucState = SHT_STATE_TEMPERATURE_START;  // Prepare for next step

            // There is a power on delay needed and that's why we perform next step only at next cycle
            //    After power-up the sensor needs !!!  11 ms  !!! to get to Sleep State. 
            //    No commands must be sent before that time.

            break;
            
    case SHT_STATE_TEMPERATURE_START:

            // Set sensor resolution. Same resolution will be used for both Temperature and Humidity
            SHT1x_SetStatusReg(STATUS_LOW_RESOLUTION);

            SHT1x_SendCommand( CMD_GET_TEMPERATURE ); // Tell SHT to measure Temperature
            s_ucState = SHT_STATE_TEMPERATURE_GET;
            break;
            
    case SHT_STATE_TEMPERATURE_GET:
            s_unRawTemperature = SHT1x_GetResult(); // save the SHT Temperature
            
            if( s_unRawTemperature == 0xFFFF )
            {
               s_ucState = SHT_STATE_TEMPERATURE_START;
            }
            else
            {  
                SHT1x_SendCommand( CMD_GET_HUMIDITY ); // Tell SHT to measure Humidity
                s_ucState = SHT_STATE_HUMIDITY_GET;
            }
            break;
            
    case SHT_STATE_HUMIDITY_GET:
      {
            unsigned int unRawHumidity = SHT1x_GetResult();
             
             if( unRawHumidity == 0xFFFF )
             {
                  SHT1x_SendCommand( CMD_GET_HUMIDITY ); // Tell SHT to measure Humidity                
                  break;
             }
             
             SHT1x_PowerOff();
            
            // Calculate Temperature, Humidity and Dew Point
            SHT1x_Calculate(p_pSHT1xSensorReadings, s_unRawTemperature, unRawHumidity);
            
            s_ucState = SHT_STATE_IDLE;
            return SHT_STATE_DONE;
      }            
  }
  
  return SHT_STATE_BUSY;
}

/*
///////////////////////////////////////////////////////////
// Function: SHT1x_ReadDataNow
// Author: Rares Ivan
// Purpose: Read and calculate temperature, humidy and dew point
// Parameters: pointer to a structure which will contain the final readings
// Returns: 
///////////////////////////////////////////////////////////
void SHT1x_ReadDataNow( SHT1x_SENSOR_READINGS* p_pSHT1xSensorReadings )
{
    while ( SHT1x_ReadData( &stSHT1xValues) == SHT_STATE_DONE )
      ;
    
    // Note: the readings are available in the given param structure:
    //   stSHT1xValues.fTemperature
    //   stSHT1xValues.fHumidity
    //   stSHT1xValues.fDewPoint
}
*/

#endif // SHT1X_MODE



