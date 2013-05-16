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
* Name:         adc.c
* Author:       Dorin Muica
* Date:         September 2009
* Description:  Read battery voltage using internal ADC
*           
* Changes:
* Revisions:
****************************************************************************************************/
#include "adc.h"

#define RESISTIVE_DIVIDER   ( 3 )
#define REFERENCE_VOLTAGE   ( 3.3f )
#define INTERNAL_REFERENCE   0
#define EXTERNAL_REFERENCE   1
#define ADC_VOLTAGE_REFERENCE  INTERNAL_REFERENCE

//#if ( ADC_USED != NONE )
const AdcConfig_t c_st_ADC_Config = {
  PRESCALER_24MHZ_TO_1MHZ, // adcPrescaler
  DIVIDER_24MHZ_TO_300KHZ, // adcDivider
  ANALOG_WARMUP_TIME + 2,  // adcOnPeriod
  2*CONVERT_TIME,          // adcConvPeriod, see chapter 17.7.17 from 1322x RM
  0,                       // adcCompIrqEn
  0,                       // adcFifoIrqEn
  gAdcManualControl_c      // adcMode
};

///////////////////////////////////////////////////////////////////////////////////
// Name:          Read_Battery_Voltage
// Author:        Dorin Muica
// Description:   read external battery voltage using secondary ADC (ADC_2)
// Parameters:    none
// Return:        battery voltage value
// Note:
///////////////////////////////////////////////////////////////////////////////////
float Read_Battery_Voltage(void)
{
  uint16 unBattery_Voltage;
  
#if (ADC_VOLTAGE_REFERENCE == EXTERNAL_REFERENCE)
  //Gpio_SetPinFunction(gGpioPin32_c,gGpioNormalMode_c); // Pin name: ADC2, pin # 3, GPIO32
  //Gpio_SetPinDir(gGpioPin32_c, gGpioDirOut_c);
  //Gpio_SetPinData(gGpioPin32_c, gGpioPinStateHigh_c);  // enable external reference voltage IF there is such reference  
  Gpio_SetPinFunction(gGpioPin38_c, gGpioAlternate1Mode_c);      // set pin #64 (GPIO_38) as ADC2_VrefH
  Gpio_SetPinFunction(gGpioPin39_c, gGpioAlternate1Mode_c);      // set pin #61 (GPIO_39) as ADC2_VrefL
#endif
  
  Gpio_SetPinFunction(gGpioPin33_c, gGpioAlternate1Mode_c);      // set pin #04 (GPIO_33) as ADC Analog Input (Channel. 3)
  
  // When these ports (ADC7-ADC0, ADCx_VREFH, and ADCxVREFL) are used as analog signals the bus keepers must be disabled (page 304)
#if (ADC_VOLTAGE_REFERENCE == EXTERNAL_REFERENCE)
   Gpio_EnPinPuKeeper(gGpioPin38_c,0);  // GPIO_PAD_KEEP1_P38 = 0;
   Gpio_EnPinPuKeeper(gGpioPin39_c,0);  // GPIO_PAD_KEEP1_P39 = 0;
#endif
   Gpio_EnPinPuKeeper(gGpioPin33_c,0);  // GPIO_PAD_KEEP1_P33 = 0;
  
  Adc_Init();
  Adc_SetConfig (&c_st_ADC_Config);
  Adc_Reset ();
  
#if (ADC_VOLTAGE_REFERENCE == EXTERNAL_REFERENCE)
    ADC_CONTROL |= ADC_ON ;
#elif (ADC_VOLTAGE_REFERENCE == INTERNAL_REFERENCE)
    ADC_CONTROL &= ~ ADC_AD2_VREFHL_EN; // Internal connection to VBATT and VSS (GND)
    ADC_CONTROL |= ADC_ON ;
#else 
    #error "Unsupported reference type"
#endif // Reference type
  
  
  DelayLoop( 1000 );
  
  if (Adc_StartManualConv(gAdcSecondary_c, gAdcChan3_c) != gAdcErrNoError_c )
  {
    unBattery_Voltage = 0;
  }
  else
  {
    DelayLoop( 1000 );
    
    if ( Adc_ManualRead(gAdcSecondary_c, &unBattery_Voltage) != gAdcErrNoError_c )
    {
      unBattery_Voltage = 0;
    }
    
  }
  
  ADC_CONTROL = 0; // ADC disabled
  
#if (ADC_VOLTAGE_REFERENCE == EXTERNAL_REFERENCE)
    //Gpio_SetPinData(gGpioPin32_c, gGpioPinStateLow_c); // disable external reference voltage
#endif
  
  // ADC * 3.3 / 4095 = ADC / 1241
  // There is a divider onboard that divides battery reading by RESISTIVE_DIVIDER = 3 
  return ( (float)(unBattery_Voltage * RESISTIVE_DIVIDER)) / (4095.0f / REFERENCE_VOLTAGE) ;
}



//#endif
