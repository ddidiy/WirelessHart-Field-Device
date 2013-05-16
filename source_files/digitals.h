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
* Name:         digitals.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_DIGITALS_H_
#define _NIVIS_DIGITALS_H_

#include "global.h"

#include "LibInterface/gpio.h"
#include "LibInterface/Platform.h"

#undef TMR_EN

#include "MC1322x.h"
#include "system.h"
#include "WirelessHART/uap.h"

/////////////////////////////////

// SSI related pins
#define SSI_MOSI    gGpioPin0_c       /* pin #28 */
#define SSI_MISO    gGpioPin1_c       /* pin #29 */
#define RTC_CS      gGpioPin2_c       /* pin #30 */
#define SSI_CLK     gGpioPin3_c       /* pin #27 */
#define SSI_CS      gGpioPin10_c      /* pin #24 */
#define EXT_MEM_PWR gGpioPin11_c

// Pin definitions for ADC I2c
#define I2C_SCL        gGpioPin12_c
#define I2C_SDA        gGpioPin13_c

/////////////////////////////////
// see it on Schematic 
// TEST_LINE_1 = J7_p3 = ADC0     = Pin  1 = GPIO 30
// TEST_LINE_2 = J7_p4 = ADC1     = Pin  2 = GPIO 31
// TEST_LINE_3 = J8_p1 = ADC2     = Pin  3 = GPIO 32
// TEST_LINE_4 = J8_p2 = UART2_RX = Pin 15 = GPIO 19
// TEST_LINE_5 = J8_p3 = UART2_TX = Pin 16 = GPIO 18
    #define TEST_LINE_1   gGpioPin30_c
    #define TEST_LINE_4   gGpioPin24_c    // KBY_2  // former gGpioPin19_c
    #define TEST_LINE_5   gGpioPin9_c     // TMR1   // former gGpioPin18_c

/////////////////////////////////
#define KYB0_LINE     gGpioPin22_c // KB0 
#define KYB1_LINE     gGpioPin23_c // KB1 
#define KYB2_LINE     gGpioPin24_c // KB2 
#define KYB3_LINE     gGpioPin25_c // KB3 
#define KYB4_LINE     gGpioPin26_c // KB4 
#define KYB5_LINE     gGpioPin27_c // KB5 
#define KYB6_LINE     gGpioPin28_c // KB6 -> Led control
#define KYB7_LINE     gGpioPin29_c // KB7 

/////////////////////////////////
#define ADC0_LINE       gGpioPin30_c /* ADC0 */
#define ADC1_LINE       gGpioPin31_c /* ADC1 */
#define ADC2_LINE       gGpioPin32_c /* ADC2 */
#define ADC3_LINE       gGpioPin33_c /* ADC3 */
#define ADC4_LINE       gGpioPin34_c /* ADC4 */
#define ADC5_LINE       gGpioPin35_c /* ADC5 */
#define ADC6_LINE       gGpioPin36_c /* ADC6 */
#define ADC7_RTCK_LINE  gGpioPin37_c /* ADC7_RTCK */
#define ADC2_VREFH_LINE gGpioPin38_c /* ADC2_VREFH */
#define ADC2_VREFL_LINE gGpioPin39_c /* ADC2_VREFL */
#define ADC1_VREFH_LINE gGpioPin40_c /* ADC1_VREFH */
#define ADC1_VREFL_LINE gGpioPin41_c /* ADC1_VREFL */

// Pin definitions for HART Modem communication
#define UART2_TX    gGpioPin18_c
#define UART2_RX    gGpioPin19_c
#define UART2_CTS   gGpioPin20_c 
#define UART2_RTS   gGpioPin21_c  

#define UART1_TX    gGpioPin14_c
#define UART1_RX    gGpioPin15_c
#define UART1_CTS   gGpioPin16_c 
#define UART1_RTS   gGpioPin17_c 

/////////////////////////////////
// Pin definitions for RF Control
#define PA_ENA_PIN1   gGpioPin42_c // ANT_1
#define PA_ENA_PIN2   gGpioPin43_c // ANT_2
#define TX_FCT_PIN    gGpioPin44_c // TX_ON
#define RX_FCT_PIN    gGpioPin45_c // RX_ON

// SPI bit-banging for the AD5621 DigitalToAnalog converter
#define DAC_SYNC    ADC4_LINE
#define DAC_SCLK    ADC5_LINE
#define DAC_SDIN    ADC6_LINE // out for MC13224, in for DAC

#define   SET_DAC_SYNC_HI()  _GPIO_DATA_SET1.Reg = GET_GPIO_AS_BIT( DAC_SYNC )
#define   SET_DAC_SYNC_LO()  _GPIO_DATA_RESET1.Reg = GET_GPIO_AS_BIT( DAC_SYNC )
#define   SET_DAC_SCLK_HI()  _GPIO_DATA_SET1.Reg = GET_GPIO_AS_BIT( DAC_SCLK )
#define   SET_DAC_SCLK_LO()  _GPIO_DATA_RESET1.Reg = GET_GPIO_AS_BIT( DAC_SCLK )
#define   SET_DAC_SDIN_HI()  _GPIO_DATA_SET1.Reg = GET_GPIO_AS_BIT( DAC_SDIN )
#define   SET_DAC_SDIN_LO()  _GPIO_DATA_RESET1.Reg = GET_GPIO_AS_BIT( DAC_SDIN )

//Pin definitions for UART protocol
#define MT_RTS    gGpioPin20_c        //Modem request to sent
#define SP_CTS    gGpioPin21_c        //Remote side clear to sent

// Pin definitions for SPI on VN210 board
#define SPI_CLK   gGpioPin7_c       /* pin #27 */
#define SPI_MOSI  gGpioPin6_c       /* pin #28 */
#define SPI_MISO  gGpioPin5_c       /* pin #29 */
#define SPI_SS    gGpioPin4_c       /* pin #30 */  


//Pin definitions for external wakeup support
#ifdef WAKEUP_ON_EXT_PIN 
    #define MT_RDY   KYB3_LINE
#endif

#define GET_GPIO_AS_BIT(x) (1L <<((x) % 32))

  #define TX_PA_ON()  _GPIO_DATA_SET1.Reg   = GET_GPIO_AS_BIT( PA_ENA_PIN1 ) | GET_GPIO_AS_BIT( PA_ENA_PIN2 )
  #define TX_PA_OFF() _GPIO_DATA_RESET1.Reg = GET_GPIO_AS_BIT( PA_ENA_PIN1 ) | GET_GPIO_AS_BIT( PA_ENA_PIN2 )

#define SELECT_GPIO_AS_OUTPUT( ucGPIO ) Gpio_SetPinDir( ucGPIO, gGpioDirOut_c )
#define SELECT_GPIO_AS_INPUT( ucGPIO )  Gpio_SetPinDir( ucGPIO, gGpioDirIn_c )


#define SET_GPIO_HI( ucGPIO )  Gpio_SetPinData( ucGPIO, gGpioPinStateHigh_c )
#define SET_GPIO_LO( ucGPIO )  Gpio_SetPinData( ucGPIO, gGpioPinStateLow_c )
#define TOGGLE_GPIO( ucGPIO )  Gpio_TogglePin( ucGPIO )

#define GET_GPIO_VALUE_( ucGPIO ) ( ucGPIO < 32 ? GPIO_REGS_P->DataLo & GET_GPIO_AS_BIT( ucGPIO ) \
                                                : GPIO_REGS_P->DataHi & GET_GPIO_AS_BIT( ucGPIO ) )

  #define ENABLE_GPIO_PULLUP( ucGPIO )      Gpio_EnPinPullup(ucGPIO, 1)
  #define DISABLE_GPIO_PULLUP( ucGPIO )     Gpio_EnPinPullup(ucGPIO, 0)
  #define SET_GPIO_WITH_PULLUP( ucGPIO )    Gpio_SelectPinPullup(ucGPIO, gGpioPinPullup_c)
  #define SET_GPIO_WITH_PULLDOWN( ucGPIO )  Gpio_SelectPinPullup(ucGPIO, gGpioPinPulldown_c)
  #define GET_LO_GPIO_VALUE( ucGPIO )       ((GPIO_REGS_P->DataLo & GET_GPIO_AS_BIT( ucGPIO )))
  #define GET_HI_GPIO_VALUE( ucGPIO )       ((GPIO_REGS_P->DataHi & GET_GPIO_AS_BIT( ucGPIO )))

  // Rares: Some constants defining nr of 250ms loops. Note: due to unsigned char [0..255], max 255*250ms = 63.75 sec is possible
  #define EVERY_250MSEC 1
  #define EVERY_500MSEC 2*EVERY_250MSEC     // 2*1 = 2 (Nr of 250ms cycles)
  #define EVERY_1SEC    2*EVERY_500MSEC     // 2*2 = 4
  #define EVERY_4SEC    4*EVERY_1SEC        // 4*4 = 16
  #define EVERY_5SEC    5*EVERY_1SEC        // 5*4 = 20
  #define EVERY_10SEC   2*EVERY_5SEC        // 2*20 = 40
  #define EVERY_15SEC   3*EVERY_5SEC        // 3*20 = 60
  #define EVERY_30SEC   2*EVERY_15SEC       // 2*60 = 120
  #define EVERY_60SEC   2*EVERY_30SEC       // 2*120 = 240

#ifdef LED_CONTROL
  // lungimea palierului
  #define TOGGLE_FREQ_1  2*1000*32 // pasive search(2s palier)
  #define TOGGLE_FREQ_2  1*1000*32 // active search(1s palier)
  #define TOGGLE_FREQ_3  500*32 // adv. received(0.5s palier)

  // time period on which the LEDs signaling is active 
  #define LEDS_SIGNALING_PERIOD         10*EVERY_1SEC // 10s activity period for status LED
  #define LEDS_SIGNALING_ALWAYS_ON      255 
  #define LEDS_SIGNALING_OFF            0

  // debounce for pushbutton(SW4)
  #define PUSHBUTTON_WAKEUP_DEBOUNCE    EVERY_1SEC // 1s push button delay

  #define CRM_SET_LED() (g_ucRequestForLedsON && g_ucLedStatus ? 0x01 : 0x00) // KYB0_LINE
#else
  #define CRM_SET_LED()  0x00
#endif


#if ( BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE )

  #if ( HART_DEV_KIT_REV == HART_DEV_KIT_REV3 ) 

    //----------------------------------------------------------------------------
    // Sensors: SHT1x Temperature & Humidity 
    //----------------------------------------------------------------------------
    // SHT1x Power ON/OFF controll pin on GPIO25 gGpioPin25_c / KBI_3
    #define SHT_PWR_PIN                   (ADC2_LINE) //ADC_Chan2
    #define SHT1X_POWER_ON()              SET_GPIO_HI( SHT_PWR_PIN )
    #define SHT1X_POWER_OFF()             SET_GPIO_LO( SHT_PWR_PIN )
    
    // SHT1x Clock pin
    //  The specific functions for this pin are defined in the SHT1x.c driver file
    #define SHT_CLK                       KYB2_LINE
    
    // SHT1x Data pin
    //  The specific functions for this pin are defined in the SHT1x.c driver file
    #define SHT_DATA                      KYB7_LINE

    // Wake-up circuit on GPIO26 gGpioPin26_c / KBI_4
    #define EXT_WAKEUP                    KYB4_LINE
    #define EXT_WAKEUP_IS_ON()            ( GET_LO_GPIO_VALUE( EXT_WAKEUP ) )

    // KBI6: Push-Button on GPIO28 gGpioPin28_c / KBI_6 input
    #define PUSHBUTTON_WAKEUP_STATUS      KYB6_LINE
    #define WAKEUP_STATUS_IS_PRESSED()    ( !GET_LO_GPIO_VALUE( PUSHBUTTON_WAKEUP_STATUS ) )  

    #define JOIN_STATUS_LED               KYB0_LINE
    #define JOIN_STATUS_LED_ON()          SET_GPIO_HI(JOIN_STATUS_LED)
    #define JOIN_STATUS_LED_OFF()         SET_GPIO_LO(JOIN_STATUS_LED)      

    #define TMR0_LINE                     gGpioPin8_c  // -> CUR_SENSE_AMP_ON
    #define TMR1_LINE                     gGpioPin9_c  // -> pornirea modemului (HARTM_ON)

    #define CUR_SENSE_AMP_ON              (TMR0_LINE) //TMR0 
 
    #define MODEM_PWR_PIN                 (TMR1_LINE)
    #define MODEM_POWER_ON()              SET_GPIO_LO( MODEM_PWR_PIN )
    #define MODEM_POWER_OFF()             SET_GPIO_HI( MODEM_PWR_PIN )

    #define MODEM_OUT_CD_SIGNAL           (ADC3_LINE) //ADC_Chan3 
    #define MODEM_IN_RTS                  UART1_RTS  

    #define TEST_LINE_2                   KYB1_LINE // ???
    #define TEST_LINE_3                   KYB3_LINE // ??? 

  #elif ( HART_DEV_KIT_REV == HART_DEV_KIT_REV4 )

    //----------------------------------------------------------------------------
    // Sensors: SHT1x Temperature & Humidity 
    //----------------------------------------------------------------------------
   // SHT1x Power ON/OFF controll pin on GPIO25 gGpioPin25_c / KBI_3
    #define SHT_PWR_PIN                   (ADC2_LINE) //ADC_Chan2
    #define SHT1X_POWER_ON()              SET_GPIO_HI( SHT_PWR_PIN )
    #define SHT1X_POWER_OFF()             SET_GPIO_LO( SHT_PWR_PIN )
    
    // SHT1x Clock pin
    //  The specific functions for this pin are defined in the SHT1x.c driver file
    #define SHT_CLK                       UART1_CTS
    
    // SHT1x Data pin
    //  The specific functions for this pin are defined in the SHT1x.c driver file
    #define SHT_DATA                      (ADC0_LINE) //ADC_Chan0

    // Wake-up circuit on GPIO26 gGpioPin26_c / KBI_4
    #define EXT_WAKEUP                    KYB4_LINE
    #define EXT_WAKEUP_IS_ON()            ( GET_LO_GPIO_VALUE( EXT_WAKEUP ) )

    #define MODEM_CD_WAKEUP               KYB7_LINE
    #define MODEM_CD_WAKEUP_IS_ON()       ( GET_LO_GPIO_VALUE( MODEM_CD_WAKEUP ) )

    #define USB_CTRL                      KYB2_LINE
    #define HART_UART_ENABLE()            ( SET_GPIO_LO(USB_CTRL) )

    // KBI6: Push-Button on GPIO28 gGpioPin28_c / KBI_6 input
    #define PUSHBUTTON_WAKEUP_STATUS      KYB6_LINE
    #define WAKEUP_STATUS_IS_PRESSED()    ( !GET_LO_GPIO_VALUE( PUSHBUTTON_WAKEUP_STATUS ) )  

    #define JOIN_STATUS_LED               KYB0_LINE
    #define JOIN_STATUS_LED_ON()          SET_GPIO_HI(JOIN_STATUS_LED)
    #define JOIN_STATUS_LED_OFF()         SET_GPIO_LO(JOIN_STATUS_LED)      

    #define TMR0_LINE                     gGpioPin8_c  // -> CUR_SENSE_AMP_ON
    #define TMR1_LINE                     gGpioPin9_c  // -> pornirea modemului (HARTM_ON)

    #define CUR_SENSE_AMP_ON              (TMR0_LINE) //TMR0 
    
    #define MODEM_PWR_PIN                 (TMR1_LINE)
    #define MODEM_POWER_ON()              SET_GPIO_LO( MODEM_PWR_PIN )
    #define MODEM_POWER_OFF()             SET_GPIO_HI( MODEM_PWR_PIN )

    #define MODEM_OUT_CD_SIGNAL           (ADC3_LINE) //ADC_Chan3 
    #define MODEM_IN_RTS                  UART1_RTS  

    #define TEST_LINE_2                   KYB1_LINE // ???
    #define TEST_LINE_3                   KYB3_LINE // ??? 

  #endif //( HART_DEV_KIT_REV == HART_DEV_KIT_REV3 )

#else //BOARD_TYPE_DEVELOPMENT
    #define JOIN_STATUS_LED         KYB6_LINE
    #define JOIN_STATUS_LED_ON()    SET_GPIO_HI(JOIN_STATUS_LED)
    #define JOIN_STATUS_LED_OFF()   SET_GPIO_LO(JOIN_STATUS_LED)

    #define EXT_WAKEUP              KYB4_LINE
    #define EXT_WAKEUP_IS_ON()      ( GET_LO_GPIO_VALUE( EXT_WAKEUP ) )

    #define TEST_LINE_2             (ADC1_LINE)  //ADC_Chan1
    #define TEST_LINE_3             (ADC2_LINE)   

#endif //BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE


  #define DEBUG1_OFF() 
  #define DEBUG1_ON()  
  #define DEBUG2_OFF() 
  #define DEBUG2_ON()  
  #define DEBUG3_OFF() 
  #define DEBUG3_ON()  
  #define DEBUG4_OFF() 
  #define DEBUG4_ON()  
  #define DEBUG5_OFF() 
  #define DEBUG5_ON()  


#ifdef LED_CONTROL

extern uint8  g_ucLedStatus;
extern uint32 g_ulLastRTC;

#endif

void Digitals_Init(void);


#endif /* _NIVIS_DIGITALS_H_ */

