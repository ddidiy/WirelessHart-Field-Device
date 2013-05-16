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
* Name:         digitals.c
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#include "digitals.h"

#ifdef LED_CONTROL
uint8  g_ucLedStatus;
uint32 g_ulLastRTC;
#endif

#if (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)
#warning "---------> Platform set as: BOARD_TYPE_HART_DEV_KIT"

  #if ( HART_DEV_KIT_REV == HART_DEV_KIT_REV3 )                                    
                                    
    #if DEBUG_RF_LINK
      #ifdef WAKEUP_ON_EXT_PIN                                   
        #define gDirLoValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( RTC_CS )           \
                                      | GET_GPIO_AS_BIT( SSI_CS )           \
                                      | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                      | GET_GPIO_AS_BIT( SSI_CLK )          \
                                      | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                      | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                      | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                      | GET_GPIO_AS_BIT( MODEM_PWR_PIN )    \
                                      | GET_GPIO_AS_BIT( TEST_LINE_1 )      \
                                      | GET_GPIO_AS_BIT( TEST_LINE_2 )      \
                                      | GET_GPIO_AS_BIT( TEST_LINE_3 )      \
                                      | GET_GPIO_AS_BIT( TEST_LINE_4 )      \
                                      | GET_GPIO_AS_BIT( MT_RDY )           \
                                      | GET_GPIO_AS_BIT( TEST_LINE_5 )              //MBAR_GPIO + 0x00
      #else
         #define gDirLoValue_c           0x00000000                         \
                                      | GET_GPIO_AS_BIT( RTC_CS )           \
                                      | GET_GPIO_AS_BIT( SPI_SS )           \
                                      | GET_GPIO_AS_BIT( SSI_CS )           \
                                      | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                      | GET_GPIO_AS_BIT( SSI_CLK )          \
                                      | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                      | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                      | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                      | GET_GPIO_AS_BIT( MODEM_PWR_PIN )    \
                                      | GET_GPIO_AS_BIT( TEST_LINE_1 )      \
                                      | GET_GPIO_AS_BIT( TEST_LINE_2 )      \
                                      | GET_GPIO_AS_BIT( TEST_LINE_3 )      \
                                      | GET_GPIO_AS_BIT( TEST_LINE_4 )      \
                                      | GET_GPIO_AS_BIT( TEST_LINE_5 )              //MBAR_GPIO + 0x00                                       
      #endif //#ifdef WAKEUP_ON_EXT_PIN
      
    #else
      #ifdef WAKEUP_ON_EXT_PIN                                  
        #define gDirLoValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( RTC_CS )           \
                                      | GET_GPIO_AS_BIT( SSI_CS )           \
                                      | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                      | GET_GPIO_AS_BIT( SSI_CLK )          \
                                      | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                      | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                      | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                      | GET_GPIO_AS_BIT( MT_RDY )           \
                                      | GET_GPIO_AS_BIT( MODEM_PWR_PIN )            //MBAR_GPIO + 0x00 
      #else
        #define gDirLoValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( RTC_CS )           \
                                      | GET_GPIO_AS_BIT( SPI_SS )           \
                                      | GET_GPIO_AS_BIT( SSI_CS )           \
                                      | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                      | GET_GPIO_AS_BIT( SSI_CLK )          \
                                      | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                      | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                      | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                      | GET_GPIO_AS_BIT( MODEM_PWR_PIN )            //MBAR_GPIO + 0x00                                        
      #endif
                                      
        #define gDirHiValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN1 )      \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN2 )      \
                                      | GET_GPIO_AS_BIT( SHT_PWR_PIN )              //MBAR_GPIO + 0x04                                                                     
                                        
    #endif  //DEBUG_RF_LINK 
    
        #define gDataLoValue_c          0x00000000                          \
                                      | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                      | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON )           //MBAR_GPIO + 0x08 -> STATUS led off at the beginning
        #define gDataHiValue_c          0x00000000                                    //MBAR_GPIO + 0x0C
    
        // GPIO Pull-up Enable Registers (GPIO_PAD_PU_EN0 = gPuEnLoValue_c and GPIO_PAD_PU_EN1 = gPuEnHiValue_c)
        // 0 = Pull-ups/pull-downs disabled for inputs, 
        // 1 = (Default) Pull-ups/pull-downs enabled for inputs
        // GPIO Pull-up Enable Register for pins   0-31 (Lo) : MBAR_GPIO + 0x10  
        #define gPuEnLoValue_c          0xffffffff                                    //MBAR_GPIO + 0x10
        #define gPuEnHiValue_c          0xffffffff                                    //MBAR_GPIO + 0x14
    
        #define gFuncSel0Value_c        0x50000000    // UART1_TX + UART1_RX          //MBAR_GPIO + 0x18
        
        #define gFuncSel1Value_c        0x00000000                                    //MBAR_GPIO + 0x1C
                                          
        #define gFuncSel2Value_c        0x05000000    // TXon RXon                    //MBAR_GPIO + 0x20     
        #define gFuncSel3Value_c        0x00000000                                    //MBAR_GPIO + 0x24
    
        // GPIO Data Select Registers (GPIO_DATA_SEL1 = gInputDataSelHiValue_c and GPIO_DATA_SEL0 = gInputDataSelLoValue_c)
        // 0 = (Default) Data is read from the pad.
        // 1 = Data is read from the GPIO Data Register
        #define gInputDataSelLoValue_c  0x00000000                                    //MBAR_GPIO + 0x28
        #define gInputDataSelHiValue_c  0x00000000                                    //MBAR_GPIO + 0x2C
    
        // GPIO Pad Pull-up Select (GPIO_PAD_PU_SEL1 = gPuSelHiValue_c and GPIO_PAD_PU_SEL0 = gPuSelLoValue_c)
        // 0 = Weak pull-down (default for all except GPIOs 12-13 (I2C), 63 (EVTI_B), 46 (TMS), 47 (TCK) and 48 (TDI))
        // 1 = Weak pull-up (default for GPIOs 12-13 (I2C), 63 (EVTI_B), 46 (TMS), 47 (TCK) and 48 (TDI))
        #define gPuSelLoValue_c   0x00000000                                \
                                  | GET_GPIO_AS_BIT( I2C_SCL )              \
                                  | GET_GPIO_AS_BIT( I2C_SDA )              \
                                  | GET_GPIO_AS_BIT( PUSHBUTTON_WAKEUP_STATUS )       //MBAR_GPIO + 0x30
                                                                                                                       
        #define gPuSelHiValue_c   0x00000000                                \
                                  | GET_GPIO_AS_BIT( gGpioPin46_c )         \
                                  | GET_GPIO_AS_BIT( gGpioPin47_c )         \
                                  | GET_GPIO_AS_BIT( gGpioPin48_c )         \
                                  | GET_GPIO_AS_BIT( gGpioPin63_c )                   //MBAR_GPIO + 0x34     
                                        
                                        
        //MBAR_GPIO + 0x44                                
        //#define gPuKeepHiValue_c        0x000000df                                    
        #define gPuKeepHiValue_c  0x00000000                                \
                                  | GET_GPIO_AS_BIT( ADC3_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC4_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC5_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC6_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC2_VREFH_LINE )      \
                                  | GET_GPIO_AS_BIT( ADC2_VREFL_LINE )                                        
                                        
                                                                                                                                            
    
        #define gHystEnLoValue_c  0x00000000                                          //MBAR_GPIO + 0x38
        
        #define gPuKeepLoValue_c  0xc0000000                                          //MBAR_GPIO + 0x40
        
                                    
  #elif ( HART_DEV_KIT_REV == HART_DEV_KIT_REV4 )   
       
    #if DEBUG_RF_LINK
       
      #ifdef WAKEUP_ON_EXT_PIN  
                                        
        #if (defined (IS_VN220))
                                        
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )    \
                                        | GET_GPIO_AS_BIT( MT_RDY )           \
                                        | GET_GPIO_AS_BIT( TEST_LINE_1 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_2 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_3 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_4 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_5 )                //MBAR_GPIO + 0x00
                                        
        #else //(defined (IS_VN220) )                        
                                          
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )    \
                                        | GET_GPIO_AS_BIT( TEST_LINE_1 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_2 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_3 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_4 )      \
                                        | GET_GPIO_AS_BIT( MT_RDY )           \
                                        | GET_GPIO_AS_BIT( TEST_LINE_5 )                //MBAR_GPIO + 0x00
                                        
        #endif //#ifdef IS_VN220
                                        
      #else   // WAKEUP_ON_EXT_PIN  
                                        
        #if (defined (IS_VN220))
                                        
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SPI_SS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )    \
                                        | GET_GPIO_AS_BIT( TEST_LINE_1 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_2 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_3 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_4 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_5 )                //MBAR_GPIO + 0x00
                                          
        #else //(defined (IS_VN220) )
                                          
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SPI_SS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )    \
                                        | GET_GPIO_AS_BIT( TEST_LINE_1 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_2 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_3 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_4 )      \
                                        | GET_GPIO_AS_BIT( TEST_LINE_5 )                //MBAR_GPIO + 0x00  
                                        
        #endif //IS_VN220
                                        
      #endif // WAKEUP_ON_EXT_PIN                            
       
      #if defined (IS_VN220)
                                        
        #define gDirHiValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN1 )      \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN2 )                  //MBAR_GPIO + 0x04
                                        
      #else                                
        #define gDirHiValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN1 )      \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN2 )      \
                                      | GET_GPIO_AS_BIT( SHT_PWR_PIN )                //MBAR_GPIO + 0x04 
                                        
      #endif //IS_VN220
      
    #else //DEBUG_RF_LINK
                                        
      #ifdef WAKEUP_ON_EXT_PIN    
                                        
        #if (defined (IS_VN220 ))
                                        
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MT_RDY )           \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )              //MBAR_GPIO + 0x00 
                                          
        #else
                                          
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MT_RDY )           \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )              //MBAR_GPIO + 0x00 
                                        
        #endif //IS_VN220
                                        
      #else //WAKEUP_ON_EXT_PIN
           
      #if (defined (IS_VN220))
                                        
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SPI_SS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )              //MBAR_GPIO + 0x00  
                                          
        #else //(defined (IS_VN220))
                                          
          #define gDirLoValue_c           0x00000000                          \
                                        | GET_GPIO_AS_BIT( RTC_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_CS )           \
                                        | GET_GPIO_AS_BIT( SSI_MOSI )         \
                                        | GET_GPIO_AS_BIT( SSI_CLK )          \
                                        | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                        | GET_GPIO_AS_BIT( JOIN_STATUS_LED )  \
                                        | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON ) \
                                        | GET_GPIO_AS_BIT( USB_CTRL )         \
                                        | GET_GPIO_AS_BIT( MODEM_PWR_PIN )              //MBAR_GPIO + 0x00                                        
                                          
        #endif //(defined (IS_VN220))
                                        
      #endif //WAKEUP_ON_EXT_PIN
        
      #if defined (IS_VN220)
                                        
        #define gDirHiValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN1 )      \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN2 )                //MBAR_GPIO + 0x04 
      #else                               
                                        
        #define gDirHiValue_c           0x00000000                          \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN1 )      \
                                      | GET_GPIO_AS_BIT( PA_ENA_PIN2 )      \
                                      | GET_GPIO_AS_BIT( SHT_PWR_PIN )                //MBAR_GPIO + 0x04  
                                        
      #endif //defined (IS_VN220)
                                        
    #endif  //DEBUG_RF_LINK 
    
      #if (defined (IS_VN220))
                                        
        #define gDataLoValue_c          0x00000000                          \
                                      | GET_GPIO_AS_BIT( EXT_MEM_PWR )                //MBAR_GPIO + 0x08 */
                                        
      #else
                                        
        #define gDataLoValue_c          0x00000000                          \
                                      | GET_GPIO_AS_BIT( EXT_MEM_PWR )      \
                                      | GET_GPIO_AS_BIT( CUR_SENSE_AMP_ON )           //MBAR_GPIO + 0x08 */
      
      #endif
                                        
        #define gDataHiValue_c          0x00000000                                    //MBAR_GPIO + 0x0C
    
        // GPIO Pull-up Enable Registers (GPIO_PAD_PU_EN0 = gPuEnLoValue_c and GPIO_PAD_PU_EN1 = gPuEnHiValue_c)
        // 0 = Pull-ups/pull-downs disabled for inputs, 
        // 1 = (Default) Pull-ups/pull-downs enabled for inputs
        // GPIO Pull-up Enable Register for pins   0-31 (Lo) : MBAR_GPIO + 0x10  
        #define gPuEnLoValue_c          0xffffffff                                    //MBAR_GPIO + 0x10
        #define gPuEnHiValue_c          0xffffffff                                    //MBAR_GPIO + 0x14
    
        #define gFuncSel0Value_c        0x50000000    // UART1_TX + UART1_RX          //MBAR_GPIO + 0x18
        
        #define gFuncSel1Value_c        0x00000000                                    //MBAR_GPIO + 0x1C
                                          
        #define gFuncSel2Value_c        0x05000000    // TXon RXon                    //MBAR_GPIO + 0x20
        

        #define gFuncSel3Value_c      0x00000000                                    //MBAR_GPIO + 0x24

                                        
        // GPIO Data Select Registers (GPIO_DATA_SEL1 = gInputDataSelHiValue_c and GPIO_DATA_SEL0 = gInputDataSelLoValue_c)
        // 0 = (Default) Data is read from the pad.
        // 1 = Data is read from the GPIO Data Register
        #define gInputDataSelLoValue_c  0x00000000                                    //MBAR_GPIO + 0x28
        #define gInputDataSelHiValue_c  0x00000000                                    //MBAR_GPIO + 0x2C
    
        // GPIO Pad Pull-up Select (GPIO_PAD_PU_SEL1 = gPuSelHiValue_c and GPIO_PAD_PU_SEL0 = gPuSelLoValue_c)
        // 0 = Weak pull-down (default for all except GPIOs 12-13 (I2C), 63 (EVTI_B), 46 (TMS), 47 (TCK) and 48 (TDI))
        // 1 = Weak pull-up (default for GPIOs 12-13 (I2C), 63 (EVTI_B), 46 (TMS), 47 (TCK) and 48 (TDI))                                                                                                                    
        #if ( SPI1_MODE != NONE )
          #define gPuSelLoValue_c   0x00000000                                  \
                                  | GET_GPIO_AS_BIT( SP_CTS )                   \
                                  | GET_GPIO_AS_BIT( MT_RTS )                   \
                                  | GET_GPIO_AS_BIT( I2C_SCL )                  \
                                  | GET_GPIO_AS_BIT( I2C_SDA )                  \
                                  | GET_GPIO_AS_BIT( PUSHBUTTON_WAKEUP_STATUS ) \
                                  | GET_GPIO_AS_BIT( SPI_SS )                         //MBAR_GPIO + 0x30
        #elif ( UART2_MODE != NONE )
          #define gPuSelLoValue_c   0x00000000                                  \
                                  | GET_GPIO_AS_BIT( SP_CTS )                   \
                                  | GET_GPIO_AS_BIT( MT_RTS )                   \
                                  | GET_GPIO_AS_BIT( I2C_SCL )                  \
                                  | GET_GPIO_AS_BIT( I2C_SDA )                  \
                                  | GET_GPIO_AS_BIT( PUSHBUTTON_WAKEUP_STATUS ) \
                                  | GET_GPIO_AS_BIT( UART2_TX )                       //MBAR_GPIO + 0x30
        #else
          #define gPuSelLoValue_c   0x00000000                                  \
                                  | GET_GPIO_AS_BIT( I2C_SCL )                  \
                                  | GET_GPIO_AS_BIT( I2C_SDA )                  \
                                  | GET_GPIO_AS_BIT( PUSHBUTTON_WAKEUP_STATUS )       //MBAR_GPIO + 0x30                                                                     
        #endif
                                                                                                                     
        #define gPuSelHiValue_c   0x00000000                                \
                                  | GET_GPIO_AS_BIT( gGpioPin46_c )         \
                                  | GET_GPIO_AS_BIT( gGpioPin47_c )         \
                                  | GET_GPIO_AS_BIT( gGpioPin48_c )         \
                                  | GET_GPIO_AS_BIT( gGpioPin63_c )                   //MBAR_GPIO + 0x34      
    
        #define gHystEnLoValue_c  0x00000000                                          //MBAR_GPIO + 0x38
        #define gHystEnHiValue_c  0x00000000                                          //MBAR_GPIO + 0x3C
        #define gPuKeepLoValue_c  0xc0000000                                          //MBAR_GPIO + 0x40
        
        //MBAR_GPIO + 0x44                                
        //#define gPuKeepHiValue_c        0x000000df  
                                        
    #if (defined (IS_VN220))
                                        
        #define gPuKeepHiValue_c  0x00000000                                \
                                  | GET_GPIO_AS_BIT( ADC2_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC3_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC4_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC5_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC6_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC2_VREFH_LINE )      \
                                  | GET_GPIO_AS_BIT( ADC2_VREFL_LINE )
                                                             
                                        
    #else                                    
        #define gPuKeepHiValue_c  0x00000000                                \
                                  | GET_GPIO_AS_BIT( ADC3_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC4_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC5_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC6_LINE )            \
                                  | GET_GPIO_AS_BIT( ADC2_VREFH_LINE )      \
                                  | GET_GPIO_AS_BIT( ADC2_VREFL_LINE )  
                                        
    #endif //(defined (IS_VN220))
                                                                       
  #endif                            
                     
                                        
#else   //BOARD_TYPE_DEVELOPMENT
#warning "---------> Platform set as: DEVELOPMENT_BOARD"                                      

#if DEBUG_RF_LINK
    #define gDirLoValue_c           0x00000000                              \
                                  | GET_GPIO_AS_BIT( RTC_CS )               \
                                  | GET_GPIO_AS_BIT( SSI_CS )               \
                                  | GET_GPIO_AS_BIT( SSI_MOSI )             \
                                  | GET_GPIO_AS_BIT( SSI_CLK )              \
                                  | GET_GPIO_AS_BIT( EXT_MEM_PWR )          \
                                  | GET_GPIO_AS_BIT( KYB0_LINE )            \
                                  | GET_GPIO_AS_BIT( JOIN_STATUS_LED )      \
                                  | GET_GPIO_AS_BIT( TEST_LINE_1 )          \
                                  | GET_GPIO_AS_BIT( TEST_LINE_2 )          \
                                  | GET_GPIO_AS_BIT( TEST_LINE_4 )          \
                                  | GET_GPIO_AS_BIT( TEST_LINE_5 )              //MBAR_GPIO + 0x00

    #define gDirHiValue_c           0x00000000                              \
                                  | GET_GPIO_AS_BIT( PA_ENA_PIN1 )          \
                                  | GET_GPIO_AS_BIT( PA_ENA_PIN2 )          \
                                  | GET_GPIO_AS_BIT( TEST_LINE_3 )              //MBAR_GPIO + 0x04
  
#else
    #define gDirLoValue_c           0x00000000                              \
                                  | GET_GPIO_AS_BIT( RTC_CS )               \
                                  | GET_GPIO_AS_BIT( SSI_CS )               \
                                  | GET_GPIO_AS_BIT( SSI_MOSI )             \
                                  | GET_GPIO_AS_BIT( SSI_CLK )              \
                                  | GET_GPIO_AS_BIT( EXT_MEM_PWR )          \
                                  | GET_GPIO_AS_BIT( KYB0_LINE )            \
                                  | GET_GPIO_AS_BIT( JOIN_STATUS_LED )              //MBAR_GPIO + 0x00 

    #define gDirHiValue_c           0x00000000                              \
                                  | GET_GPIO_AS_BIT( PA_ENA_PIN1 )          \
                                  | GET_GPIO_AS_BIT( PA_ENA_PIN2 )              //MBAR_GPIO + 0x04                                  
#endif  //DEBUG_RF_LINK 

    #define gDataLoValue_c          0x10000000                                    //MBAR_GPIO + 0x08 -> Join Status led off at the beginning
    #define gDataHiValue_c          0x00000000                                    //MBAR_GPIO + 0x0C

    // GPIO Pull-up Enable Registers (GPIO_PAD_PU_EN0 = gPuEnLoValue_c and GPIO_PAD_PU_EN1 = gPuEnHiValue_c)
    // 0 = Pull-ups/pull-downs disabled for inputs, 
    // 1 = (Default) Pull-ups/pull-downs enabled for inputs
    // GPIO Pull-up Enable Register for pins   0-31 (Lo) : MBAR_GPIO + 0x10  
    #define gPuEnLoValue_c          0xffffffff                                    //MBAR_GPIO + 0x10
    #define gPuEnHiValue_c          0xffffffff                                    //MBAR_GPIO + 0x14

    #define gFuncSel0Value_c        0x00000000                                    //MBAR_GPIO + 0x18
    #define gFuncSel1Value_c        0x00000000                                    //MBAR_GPIO + 0x1C
    #define gFuncSel2Value_c        0x05000000    // TXon RXon                    //MBAR_GPIO + 0x20 
    #define gFuncSel3Value_c        0x00000000                                    //MBAR_GPIO + 0x24
    #define gInputDataSelLoValue_c  0x00000000                                    //MBAR_GPIO + 0x28
    #define gInputDataSelHiValue_c  0x00000000                                    //MBAR_GPIO + 0x2C

    // GPIO Pad Pull-up Select (GPIO_PAD_PU_SEL1 = gPuSelHiValue_c and GPIO_PAD_PU_SEL0 = gPuSelLoValue_c)
    // 0 = Weak pull-down (default for all except GPIOs 12-13 (I2C), 63 (EVTI_B), 46 (TMS), 47 (TCK) and 48 (TDI))
    // 1 = Weak pull-up (default for GPIOs 12-13 (I2C), 63 (EVTI_B), 46 (TMS), 47 (TCK) and 48 (TDI))
    #define gPuSelLoValue_c   0x00000000                                    \
                              | GET_GPIO_AS_BIT( I2C_SCL )                  \
                              | GET_GPIO_AS_BIT( I2C_SDA )                        //MBAR_GPIO + 0x30
                               
    #define gPuSelHiValue_c   0x00000000                                    \
                              | GET_GPIO_AS_BIT( gGpioPin46_c )             \
                              | GET_GPIO_AS_BIT( gGpioPin47_c )             \
                              | GET_GPIO_AS_BIT( gGpioPin48_c )             \
                              | GET_GPIO_AS_BIT( gGpioPin63_c )                   //MBAR_GPIO + 0x34      

    #define gHystEnLoValue_c  0x00000000                                    //MBAR_GPIO + 0x38
    #define gHystEnHiValue_c  0x00000000                                    //MBAR_GPIO + 0x3C
    #define gPuKeepLoValue_c  0xc0000000                                    //MBAR_GPIO + 0x40

    //MBAR_GPIO + 0x44                                
    //#define gPuKeepHiValue_c        0x000000df                                    
    #define gPuKeepHiValue_c  0x00000000                                    \
                              | GET_GPIO_AS_BIT( ADC2_LINE )                \
                              | GET_GPIO_AS_BIT( ADC3_LINE )                \
                              | GET_GPIO_AS_BIT( ADC4_LINE )                \
                              | GET_GPIO_AS_BIT( ADC5_LINE )                \
                              | GET_GPIO_AS_BIT( ADC6_LINE )                \
                              | GET_GPIO_AS_BIT( ADC2_VREFH_LINE )          \
                              | GET_GPIO_AS_BIT( ADC2_VREFL_LINE )    
                                    
#endif

void Digitals_Init(void)
{
  GPIO_REGS_P->DataLo = gDataLoValue_c;                                         //MBAR_GPIO + 0x08
  GPIO_REGS_P->DataHi = gDataHiValue_c;                                         //MBAR_GPIO + 0x0C
  GPIO_REGS_P->DirLo =  gDirLoValue_c;                                          //MBAR_GPIO + 0x00
  GPIO_REGS_P->DirHi =  gDirHiValue_c;                                          //MBAR_GPIO + 0x04
  GPIO_REGS_P->PuEnLo = gPuEnLoValue_c;                                         //MBAR_GPIO + 0x10
  GPIO_REGS_P->PuEnHi = gPuEnHiValue_c;                                         //MBAR_GPIO + 0x14
  GPIO_REGS_P->FuncSel0 = gFuncSel0Value_c;                                     //MBAR_GPIO + 0x18
  GPIO_REGS_P->FuncSel1 = gFuncSel1Value_c;                                     //MBAR_GPIO + 0x1C
  GPIO_REGS_P->FuncSel2 = gFuncSel2Value_c;                                     //MBAR_GPIO + 0x20
  GPIO_REGS_P->FuncSel3 = gFuncSel3Value_c;                                     //MBAR_GPIO + 0x24
  GPIO_REGS_P->InputDataSelLo = gInputDataSelLoValue_c;                         //MBAR_GPIO + 0x28
  GPIO_REGS_P->InputDataSelHi = gInputDataSelHiValue_c;                         //MBAR_GPIO + 0x2C
  GPIO_REGS_P->PuSelLo = gPuSelLoValue_c;                                       //MBAR_GPIO + 0x30
  GPIO_REGS_P->PuSelHi = gPuSelHiValue_c;                                       //MBAR_GPIO + 0x34
  GPIO_REGS_P->HystEnLo = gHystEnLoValue_c;                                     //MBAR_GPIO + 0x38
  GPIO_REGS_P->HystEnHi = gHystEnHiValue_c;                                     //MBAR_GPIO + 0x3C
  GPIO_REGS_P->PuKeepLo = gPuKeepLoValue_c;                                     //MBAR_GPIO + 0x40
  GPIO_REGS_P->PuKeepHi = gPuKeepHiValue_c;                                     //MBAR_GPIO + 0x44  

#ifdef LED_CONTROL
  g_ulLastRTC = RTC_COUNT;
#endif   
    
#ifdef TEST_MORE_STATUS 
  SELECT_GPIO_AS_INPUT(gGpioPin23_c); // KYB1_LINE - initialized as input (STACK line on Adapter board)
#endif //TEST_MORE_STATUS
}
