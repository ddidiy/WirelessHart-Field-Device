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
* Name:         MC1322x.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided as an interface to hardware
*               This file holds definitions of the peripheral registers
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_MC1322x_H_
#define _NIVIS_MC1322x_H_

#include "typedef.h"


typedef union
{
  struct
  {
    uint32  GPIO_00    :1;
    uint32  GPIO_01    :1;
    uint32  GPIO_02    :1;
    uint32  GPIO_03    :1;
    uint32  GPIO_04    :1;
    uint32  GPIO_05    :1;
    uint32  GPIO_06    :1;
    uint32  GPIO_07    :1;
    uint32  GPIO_08    :1;
    uint32  GPIO_09    :1;
    uint32  GPIO_10    :1;
    uint32  GPIO_11    :1;
    uint32  GPIO_12    :1;
    uint32  GPIO_13    :1;
    uint32  GPIO_14    :1;
    uint32  GPIO_15    :1;
    uint32  GPIO_16    :1;
    uint32  GPIO_17    :1;
    uint32  GPIO_18    :1;
    uint32  GPIO_19    :1;
    uint32  GPIO_20    :1;
    uint32  GPIO_21    :1;
    uint32  GPIO_22    :1;
    uint32  GPIO_23    :1;
    uint32  GPIO_24    :1;
    uint32  GPIO_25    :1;
    uint32  GPIO_26    :1;
    uint32  GPIO_27    :1;
    uint32  GPIO_28    :1;
    uint32  GPIO_29    :1;
    uint32  GPIO_30    :1;
    uint32  GPIO_31    :1;
  } Bits;
  uint32 Reg;
} GPIO_PORT0_T;


typedef union
{
  struct
  {
    uint32  GPIO_32    :1;
    uint32  GPIO_33    :1;
    uint32  GPIO_34    :1;
    uint32  GPIO_35    :1;
    uint32  GPIO_36    :1;
    uint32  GPIO_37    :1;
    uint32  GPIO_38    :1;
    uint32  GPIO_39    :1;
    uint32  GPIO_40    :1;
    uint32  GPIO_41    :1;
    uint32  GPIO_42    :1;
    uint32  GPIO_43    :1;
    uint32  GPIO_44    :1;
    uint32  GPIO_45    :1;
    uint32  GPIO_46    :1;
    uint32  GPIO_47    :1;
    uint32  GPIO_48    :1;
    uint32  GPIO_49    :1;
    uint32  GPIO_50    :1;
    uint32  GPIO_51    :1;
    uint32  GPIO_52    :1;
    uint32  GPIO_53    :1;
    uint32  GPIO_54    :1;
    uint32  GPIO_55    :1;
    uint32  GPIO_56    :1;
    uint32  GPIO_57    :1;
    uint32  GPIO_58    :1;
    uint32  GPIO_59    :1;
    uint32  GPIO_60    :1;
    uint32  GPIO_61    :1;
    uint32  GPIO_62    :1;
    uint32  GPIO_63    :1;
  } Bits;
  uint32 Reg;
} GPIO_PORT1_T;


typedef union
{
  struct
  {
    uint16 OUTPUT_MODE            :3;
    uint16 CO_INIT                :1;
    uint16 DIR                    :1;
    uint16 LENGTH                 :1;
    uint16 ONCE                   :1;
    uint16 SECONDARY_SOURCE       :2;
    uint16 PRIMARY_COUNT_SOURCE   :4;
    uint16 COUNT_MODE             :3;
  } Bits;
  uint16 Reg;
} TMR_CTRL_T;


typedef union
{
  struct
  {
    uint16 OEN              :1;
    uint16 OPS              :1;
    uint16 FORCE            :1;
    uint16 VAL              :1;
    uint16 EEOF             :1;
    uint16 MSTR             :1;
    uint16 CAPTURE_MODE     :2;
    uint16 INPUT            :1;
    uint16 IPS              :1;
    uint16 IEFIE            :1;
    uint16 IEF              :1;
    uint16 TOFIE            :1;
    uint16 TOF              :1;
    uint16 TCFIE            :1;
    uint16 TCF              :1;
  } Bits;
  uint16 Reg;
} TMR_SCTRL_T;


typedef union
{
  struct
  {
    uint16 CL1          :2;
    uint16 CL2          :2;
    uint16 TCF1         :1;
    uint16 TCF2         :1;
    uint16 TCF1EN       :1;
    uint16 TCF2EN       :1;
    uint16              :5;
    uint16 FILT_EN      :1;
    uint16 DBG_EN       :2;
  } Bits;
  uint16 Reg;
} TMR_CSCTRL_T;


typedef union
{
  struct
  {
    uint32 ACPL     :1;
    uint32 POLL     :1;
    uint32 DI       :1;
    uint32 WU       :1;
    uint32 RST      :1;
    uint32          :4;
    uint32 LVL      :1;
    uint32 SFT      :1;
    uint32 FLT      :1;
    uint32 CRC      :1;
    uint32 CM       :1;
    uint32 SYNC     :1;
    uint32 STRT     :1;
    uint32          :16;
  } Bits;
  uint32 Reg;
} MACA_IRQ_T;


typedef union
{
  struct
  {
    uint16 VALUE      :12;
    uint16 CHANNEL    :3;
    uint16 GL         :1;
  } Bits;
  uint16 Reg;
} ADC_COMP_T;


typedef union
{
  struct
  {
    uint16 VALUE      :12;
    uint16            :4;
  } Bits;
  uint16 Reg;
} ADC_BAT_COMP_T;


/* platform clock  boundaries */
#define PLATFORM_MAX_CLOCK (26214)          // maximum value of platform clock is 26214 kHz
#define PLATFORM_MIN_CLOCK (1000)           // minimum value of platform clock is 1000 kHz

/* RAM jumpvectores located @ Memory start. 0x00400000 */
#define  RAM_BASE    0x00400000
#define  RAM_TOP     0x00417FFF

/* -- Defines for Processor Modes -------------------------- */
#define  MODE_FVC    0x11                   // FIQ
#define  MODE_IVC    0x12                   // IRQ
#define  MODE_SVC    0x13                   // supervisor
#define  MODE_AVC    0x17                   // abort
#define  MODE_UVC    0x1b                   // undefined
#define  MODE_USER   0x10                   // user
#define  MODE_SYS    0x1f                   // system mode

/* Peripheral Memory Map Base Addresses */
#define  MBAR_AIPI   0x80000000             //32 bit
#define  MBAR_GPIO   0x80000000             //32 bit
#define  MBAR_SSI    0x80001000             //32 bit
#define  MBAR_SPI0   0x80002000             //32 bit
#define  MBAR_CRM    0x80003000             //32 bit
#define  MBAR_MACA   0x80004000             //32 bit
#define  MBAR_UART1  0x80005000             //32 bit
#define  MBAR_I2C    0x80006000             //32 bit
#define  MBAR_TMR0   0x80007000             //16 bit
#define  MBAR_TMR1   0x80007020             //16 bit
#define  MBAR_TMR2   0x80007040             //16 bit
#define  MBAR_TMR3   0x80007060             //16 bit
#define  MBAR_TMR_EN 0x8000701E             //16 bit
#define  MBAR_ASM    0x80008000             //32 bit
#define  MBAR_MODEM  0x80009000             //32 bit
#define  MBAR_RF     0x8000A000             //32 bit
#define  MBAR_UART2  0x8000B000             //32 bit
#define  MBAR_FLASH  0x8000C000             //32 bit
#define  MBAR_ADC    0x8000D000             //32 bit
#define  MBAR_AITC   0x80020000             //32 bit
#define  MBAR_NEX    0x80040000             //32 bit




/***************************************************************************************************/

/* -------------- General Purpose I/O Module (GPIO) ------------------------- */
/*    Base Adrress = MBAR_GPIO = 0x80000000                                   */
__no_init extern volatile GPIO_PORT0_T _GPIO_PAD_DIR0 @(MBAR_GPIO + 0x00);      /* GPIO Pad Direction for GPIO 00-31 R/W 32 */
#define GPIO_PAD_DIR0        _GPIO_PAD_DIR0.Reg
#define GPIO_PAD_DIR0_P00    _GPIO_PAD_DIR0.Bits.GPIO_00                        /* BIT0 */
#define GPIO_PAD_DIR0_P01    _GPIO_PAD_DIR0.Bits.GPIO_01                        /* BIT1 */
#define GPIO_PAD_DIR0_P02    _GPIO_PAD_DIR0.Bits.GPIO_02                        /* BIT2 */
#define GPIO_PAD_DIR0_P03    _GPIO_PAD_DIR0.Bits.GPIO_03                        /* BIT3 */
#define GPIO_PAD_DIR0_P04    _GPIO_PAD_DIR0.Bits.GPIO_04                        /* BIT4 */
#define GPIO_PAD_DIR0_P05    _GPIO_PAD_DIR0.Bits.GPIO_05                        /* BIT5 */
#define GPIO_PAD_DIR0_P06    _GPIO_PAD_DIR0.Bits.GPIO_06                        /* BIT6 */
#define GPIO_PAD_DIR0_P07    _GPIO_PAD_DIR0.Bits.GPIO_07                        /* BIT7 */
#define GPIO_PAD_DIR0_P08    _GPIO_PAD_DIR0.Bits.GPIO_08                        /* BIT8 */
#define GPIO_PAD_DIR0_P09    _GPIO_PAD_DIR0.Bits.GPIO_09                        /* BIT9 */
#define GPIO_PAD_DIR0_P10    _GPIO_PAD_DIR0.Bits.GPIO_10                        /* BIT10 */
#define GPIO_PAD_DIR0_P11    _GPIO_PAD_DIR0.Bits.GPIO_11                        /* BIT11 */
#define GPIO_PAD_DIR0_P12    _GPIO_PAD_DIR0.Bits.GPIO_12                        /* BIT12 */
#define GPIO_PAD_DIR0_P13    _GPIO_PAD_DIR0.Bits.GPIO_13                        /* BIT13 */
#define GPIO_PAD_DIR0_P14    _GPIO_PAD_DIR0.Bits.GPIO_14                        /* BIT14 */
#define GPIO_PAD_DIR0_P15    _GPIO_PAD_DIR0.Bits.GPIO_15                        /* BIT15 */
#define GPIO_PAD_DIR0_P16    _GPIO_PAD_DIR0.Bits.GPIO_16                        /* BIT16 */
#define GPIO_PAD_DIR0_P17    _GPIO_PAD_DIR0.Bits.GPIO_17                        /* BIT17 */
#define GPIO_PAD_DIR0_P18    _GPIO_PAD_DIR0.Bits.GPIO_18                        /* BIT18 */
#define GPIO_PAD_DIR0_P19    _GPIO_PAD_DIR0.Bits.GPIO_19                        /* BIT19 */
#define GPIO_PAD_DIR0_P20    _GPIO_PAD_DIR0.Bits.GPIO_20                        /* BIT20 */
#define GPIO_PAD_DIR0_P21    _GPIO_PAD_DIR0.Bits.GPIO_21                        /* BIT21 */
#define GPIO_PAD_DIR0_P22    _GPIO_PAD_DIR0.Bits.GPIO_22                        /* BIT22 */
#define GPIO_PAD_DIR0_P23    _GPIO_PAD_DIR0.Bits.GPIO_23                        /* BIT23 */
#define GPIO_PAD_DIR0_P24    _GPIO_PAD_DIR0.Bits.GPIO_24                        /* BIT24 */
#define GPIO_PAD_DIR0_P25    _GPIO_PAD_DIR0.Bits.GPIO_25                        /* BIT25 */
#define GPIO_PAD_DIR0_P26    _GPIO_PAD_DIR0.Bits.GPIO_26                        /* BIT26 */
#define GPIO_PAD_DIR0_P27    _GPIO_PAD_DIR0.Bits.GPIO_27                        /* BIT27 */
#define GPIO_PAD_DIR0_P28    _GPIO_PAD_DIR0.Bits.GPIO_28                        /* BIT28 */
#define GPIO_PAD_DIR0_P29    _GPIO_PAD_DIR0.Bits.GPIO_29                        /* BIT29 */
#define GPIO_PAD_DIR0_P30    _GPIO_PAD_DIR0.Bits.GPIO_30                        /* BIT30 */
#define GPIO_PAD_DIR0_P31    _GPIO_PAD_DIR0.Bits.GPIO_31                        /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_PAD_DIR1 @(MBAR_GPIO + 0x04);      /* GPIO Pad Direction for GPIO 32-63 R/W 32 */
#define GPIO_PAD_DIR1        _GPIO_PAD_DIR1.Reg
#define GPIO_PAD_DIR1_P32    _GPIO_PAD_DIR1.Bits.GPIO_32                        /* BIT0 */
#define GPIO_PAD_DIR1_P33    _GPIO_PAD_DIR1.Bits.GPIO_33                        /* BIT1 */
#define GPIO_PAD_DIR1_P34    _GPIO_PAD_DIR1.Bits.GPIO_34                        /* BIT2 */
#define GPIO_PAD_DIR1_P35    _GPIO_PAD_DIR1.Bits.GPIO_35                        /* BIT3 */
#define GPIO_PAD_DIR1_P36    _GPIO_PAD_DIR1.Bits.GPIO_36                        /* BIT4 */
#define GPIO_PAD_DIR1_P37    _GPIO_PAD_DIR1.Bits.GPIO_37                        /* BIT5 */
#define GPIO_PAD_DIR1_P38    _GPIO_PAD_DIR1.Bits.GPIO_38                        /* BIT6 */
#define GPIO_PAD_DIR1_P39    _GPIO_PAD_DIR1.Bits.GPIO_39                        /* BIT7 */
#define GPIO_PAD_DIR1_P40    _GPIO_PAD_DIR1.Bits.GPIO_40                        /* BIT8 */
#define GPIO_PAD_DIR1_P41    _GPIO_PAD_DIR1.Bits.GPIO_41                        /* BIT9 */
#define GPIO_PAD_DIR1_P42    _GPIO_PAD_DIR1.Bits.GPIO_42                        /* BIT10 */
#define GPIO_PAD_DIR1_P43    _GPIO_PAD_DIR1.Bits.GPIO_43                        /* BIT11 */
#define GPIO_PAD_DIR1_P44    _GPIO_PAD_DIR1.Bits.GPIO_44                        /* BIT12 */
#define GPIO_PAD_DIR1_P45    _GPIO_PAD_DIR1.Bits.GPIO_45                        /* BIT13 */
#define GPIO_PAD_DIR1_P46    _GPIO_PAD_DIR1.Bits.GPIO_46                        /* BIT14 */
#define GPIO_PAD_DIR1_P47    _GPIO_PAD_DIR1.Bits.GPIO_47                        /* BIT15 */
#define GPIO_PAD_DIR1_P48    _GPIO_PAD_DIR1.Bits.GPIO_48                        /* BIT16 */
#define GPIO_PAD_DIR1_P49    _GPIO_PAD_DIR1.Bits.GPIO_49                        /* BIT17 */
#define GPIO_PAD_DIR1_P50    _GPIO_PAD_DIR1.Bits.GPIO_50                        /* BIT18 */
#define GPIO_PAD_DIR1_P51    _GPIO_PAD_DIR1.Bits.GPIO_51                        /* BIT19 */
#define GPIO_PAD_DIR1_P52    _GPIO_PAD_DIR1.Bits.GPIO_52                        /* BIT20 */
#define GPIO_PAD_DIR1_P53    _GPIO_PAD_DIR1.Bits.GPIO_53                        /* BIT21 */
#define GPIO_PAD_DIR1_P54    _GPIO_PAD_DIR1.Bits.GPIO_54                        /* BIT22 */
#define GPIO_PAD_DIR1_P55    _GPIO_PAD_DIR1.Bits.GPIO_55                        /* BIT23 */
#define GPIO_PAD_DIR1_P56    _GPIO_PAD_DIR1.Bits.GPIO_56                        /* BIT24 */
#define GPIO_PAD_DIR1_P57    _GPIO_PAD_DIR1.Bits.GPIO_57                        /* BIT25 */
#define GPIO_PAD_DIR1_P58    _GPIO_PAD_DIR1.Bits.GPIO_58                        /* BIT26 */
#define GPIO_PAD_DIR1_P59    _GPIO_PAD_DIR1.Bits.GPIO_59                        /* BIT27 */
#define GPIO_PAD_DIR1_P60    _GPIO_PAD_DIR1.Bits.GPIO_60                        /* BIT28 */
#define GPIO_PAD_DIR1_P61    _GPIO_PAD_DIR1.Bits.GPIO_61                        /* BIT29 */
#define GPIO_PAD_DIR1_P62    _GPIO_PAD_DIR1.Bits.GPIO_62                        /* BIT30 */
#define GPIO_PAD_DIR1_P63    _GPIO_PAD_DIR1.Bits.GPIO_63                        /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_DATA0 @(MBAR_GPIO + 0x08);         /* GPIO Data for GPIO 00-31 R/W 32 */
#define GPIO_DATA0        _GPIO_DATA0.Reg
#define GPIO_DATA0_P00    _GPIO_DATA0.Bits.GPIO_00                              /* BIT0 */
#define GPIO_DATA0_P01    _GPIO_DATA0.Bits.GPIO_01                              /* BIT1 */
#define GPIO_DATA0_P02    _GPIO_DATA0.Bits.GPIO_02                              /* BIT2 */
#define GPIO_DATA0_P03    _GPIO_DATA0.Bits.GPIO_03                              /* BIT3 */
#define GPIO_DATA0_P04    _GPIO_DATA0.Bits.GPIO_04                              /* BIT4 */
#define GPIO_DATA0_P05    _GPIO_DATA0.Bits.GPIO_05                              /* BIT5 */
#define GPIO_DATA0_P06    _GPIO_DATA0.Bits.GPIO_06                              /* BIT6 */
#define GPIO_DATA0_P07    _GPIO_DATA0.Bits.GPIO_07                              /* BIT7 */
#define GPIO_DATA0_P08    _GPIO_DATA0.Bits.GPIO_08                              /* BIT8 */
#define GPIO_DATA0_P09    _GPIO_DATA0.Bits.GPIO_09                              /* BIT9 */
#define GPIO_DATA0_P10    _GPIO_DATA0.Bits.GPIO_10                              /* BIT10 */
#define GPIO_DATA0_P11    _GPIO_DATA0.Bits.GPIO_11                              /* BIT11 */
#define GPIO_DATA0_P12    _GPIO_DATA0.Bits.GPIO_12                              /* BIT12 */
#define GPIO_DATA0_P13    _GPIO_DATA0.Bits.GPIO_13                              /* BIT13 */
#define GPIO_DATA0_P14    _GPIO_DATA0.Bits.GPIO_14                              /* BIT14 */
#define GPIO_DATA0_P15    _GPIO_DATA0.Bits.GPIO_15                              /* BIT15 */
#define GPIO_DATA0_P16    _GPIO_DATA0.Bits.GPIO_16                              /* BIT16 */
#define GPIO_DATA0_P17    _GPIO_DATA0.Bits.GPIO_17                              /* BIT17 */
#define GPIO_DATA0_P18    _GPIO_DATA0.Bits.GPIO_18                              /* BIT18 */
#define GPIO_DATA0_P19    _GPIO_DATA0.Bits.GPIO_19                              /* BIT19 */
#define GPIO_DATA0_P20    _GPIO_DATA0.Bits.GPIO_20                              /* BIT20 */
#define GPIO_DATA0_P21    _GPIO_DATA0.Bits.GPIO_21                              /* BIT21 */
#define GPIO_DATA0_P22    _GPIO_DATA0.Bits.GPIO_22                              /* BIT22 */
#define GPIO_DATA0_P23    _GPIO_DATA0.Bits.GPIO_23                              /* BIT23 */
#define GPIO_DATA0_P24    _GPIO_DATA0.Bits.GPIO_24                              /* BIT24 */
#define GPIO_DATA0_P25    _GPIO_DATA0.Bits.GPIO_25                              /* BIT25 */
#define GPIO_DATA0_P26    _GPIO_DATA0.Bits.GPIO_26                              /* BIT26 */
#define GPIO_DATA0_P27    _GPIO_DATA0.Bits.GPIO_27                              /* BIT27 */
#define GPIO_DATA0_P28    _GPIO_DATA0.Bits.GPIO_28                              /* BIT28 */
#define GPIO_DATA0_P29    _GPIO_DATA0.Bits.GPIO_29                              /* BIT29 */
#define GPIO_DATA0_P30    _GPIO_DATA0.Bits.GPIO_30                              /* BIT30 */
#define GPIO_DATA0_P31    _GPIO_DATA0.Bits.GPIO_31                              /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_DATA1 @(MBAR_GPIO + 0x0C);         /* GPIO Data for GPIO 32-63 R/W 32 */
#define GPIO_DATA1        _GPIO_DATA1.Reg
#define GPIO_DATA1_P32    _GPIO_DATA1.Bits.GPIO_32                              /* BIT0 */
#define GPIO_DATA1_P33    _GPIO_DATA1.Bits.GPIO_33                              /* BIT1 */
#define GPIO_DATA1_P34    _GPIO_DATA1.Bits.GPIO_34                              /* BIT2 */
#define GPIO_DATA1_P35    _GPIO_DATA1.Bits.GPIO_35                              /* BIT3 */
#define GPIO_DATA1_P36    _GPIO_DATA1.Bits.GPIO_36                              /* BIT4 */
#define GPIO_DATA1_P37    _GPIO_DATA1.Bits.GPIO_37                              /* BIT5 */
#define GPIO_DATA1_P38    _GPIO_DATA1.Bits.GPIO_38                              /* BIT6 */
#define GPIO_DATA1_P39    _GPIO_DATA1.Bits.GPIO_39                              /* BIT7 */
#define GPIO_DATA1_P40    _GPIO_DATA1.Bits.GPIO_40                              /* BIT8 */
#define GPIO_DATA1_P41    _GPIO_DATA1.Bits.GPIO_41                              /* BIT9 */
#define GPIO_DATA1_P42    _GPIO_DATA1.Bits.GPIO_42                              /* BIT10 */
#define GPIO_DATA1_P43    _GPIO_DATA1.Bits.GPIO_43                              /* BIT11 */
#define GPIO_DATA1_P44    _GPIO_DATA1.Bits.GPIO_44                              /* BIT12 */
#define GPIO_DATA1_P45    _GPIO_DATA1.Bits.GPIO_45                              /* BIT13 */
#define GPIO_DATA1_P46    _GPIO_DATA1.Bits.GPIO_46                              /* BIT14 */
#define GPIO_DATA1_P47    _GPIO_DATA1.Bits.GPIO_47                              /* BIT15 */
#define GPIO_DATA1_P48    _GPIO_DATA1.Bits.GPIO_48                              /* BIT16 */
#define GPIO_DATA1_P49    _GPIO_DATA1.Bits.GPIO_49                              /* BIT17 */
#define GPIO_DATA1_P50    _GPIO_DATA1.Bits.GPIO_50                              /* BIT18 */
#define GPIO_DATA1_P51    _GPIO_DATA1.Bits.GPIO_51                              /* BIT19 */
#define GPIO_DATA1_P52    _GPIO_DATA1.Bits.GPIO_52                              /* BIT20 */
#define GPIO_DATA1_P53    _GPIO_DATA1.Bits.GPIO_53                              /* BIT21 */
#define GPIO_DATA1_P54    _GPIO_DATA1.Bits.GPIO_54                              /* BIT22 */
#define GPIO_DATA1_P55    _GPIO_DATA1.Bits.GPIO_55                              /* BIT23 */
#define GPIO_DATA1_P56    _GPIO_DATA1.Bits.GPIO_56                              /* BIT24 */
#define GPIO_DATA1_P57    _GPIO_DATA1.Bits.GPIO_57                              /* BIT25 */
#define GPIO_DATA1_P58    _GPIO_DATA1.Bits.GPIO_58                              /* BIT26 */
#define GPIO_DATA1_P59    _GPIO_DATA1.Bits.GPIO_59                              /* BIT27 */
#define GPIO_DATA1_P60    _GPIO_DATA1.Bits.GPIO_60                              /* BIT28 */
#define GPIO_DATA1_P61    _GPIO_DATA1.Bits.GPIO_61                              /* BIT29 */
#define GPIO_DATA1_P62    _GPIO_DATA1.Bits.GPIO_62                              /* BIT30 */
#define GPIO_DATA1_P63    _GPIO_DATA1.Bits.GPIO_63                              /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_PAD_PU_EN0 @(MBAR_GPIO + 0x10);    /* GPIO Pad Pullup Enable for GPIO 00-31 R/W 32 */
#define GPIO_PAD_PU_EN0        _GPIO_PAD_PU_EN0.Reg
#define GPIO_PAD_PU_EN0_P00    _GPIO_PAD_PU_EN0.Bits.GPIO_00                    /* BIT0 */
#define GPIO_PAD_PU_EN0_P01    _GPIO_PAD_PU_EN0.Bits.GPIO_01                    /* BIT1 */
#define GPIO_PAD_PU_EN0_P02    _GPIO_PAD_PU_EN0.Bits.GPIO_02                    /* BIT2 */
#define GPIO_PAD_PU_EN0_P03    _GPIO_PAD_PU_EN0.Bits.GPIO_03                    /* BIT3 */
#define GPIO_PAD_PU_EN0_P04    _GPIO_PAD_PU_EN0.Bits.GPIO_04                    /* BIT4 */
#define GPIO_PAD_PU_EN0_P05    _GPIO_PAD_PU_EN0.Bits.GPIO_05                    /* BIT5 */
#define GPIO_PAD_PU_EN0_P06    _GPIO_PAD_PU_EN0.Bits.GPIO_06                    /* BIT6 */
#define GPIO_PAD_PU_EN0_P07    _GPIO_PAD_PU_EN0.Bits.GPIO_07                    /* BIT7 */
#define GPIO_PAD_PU_EN0_P08    _GPIO_PAD_PU_EN0.Bits.GPIO_08                    /* BIT8 */
#define GPIO_PAD_PU_EN0_P09    _GPIO_PAD_PU_EN0.Bits.GPIO_09                    /* BIT9 */
#define GPIO_PAD_PU_EN0_P10    _GPIO_PAD_PU_EN0.Bits.GPIO_10                    /* BIT10 */
#define GPIO_PAD_PU_EN0_P11    _GPIO_PAD_PU_EN0.Bits.GPIO_11                    /* BIT11 */
#define GPIO_PAD_PU_EN0_P12    _GPIO_PAD_PU_EN0.Bits.GPIO_12                    /* BIT12 */
#define GPIO_PAD_PU_EN0_P13    _GPIO_PAD_PU_EN0.Bits.GPIO_13                    /* BIT13 */
#define GPIO_PAD_PU_EN0_P14    _GPIO_PAD_PU_EN0.Bits.GPIO_14                    /* BIT14 */
#define GPIO_PAD_PU_EN0_P15    _GPIO_PAD_PU_EN0.Bits.GPIO_15                    /* BIT15 */
#define GPIO_PAD_PU_EN0_P16    _GPIO_PAD_PU_EN0.Bits.GPIO_16                    /* BIT16 */
#define GPIO_PAD_PU_EN0_P17    _GPIO_PAD_PU_EN0.Bits.GPIO_17                    /* BIT17 */
#define GPIO_PAD_PU_EN0_P18    _GPIO_PAD_PU_EN0.Bits.GPIO_18                    /* BIT18 */
#define GPIO_PAD_PU_EN0_P19    _GPIO_PAD_PU_EN0.Bits.GPIO_19                    /* BIT19 */
#define GPIO_PAD_PU_EN0_P20    _GPIO_PAD_PU_EN0.Bits.GPIO_20                    /* BIT20 */
#define GPIO_PAD_PU_EN0_P21    _GPIO_PAD_PU_EN0.Bits.GPIO_21                    /* BIT21 */
#define GPIO_PAD_PU_EN0_P22    _GPIO_PAD_PU_EN0.Bits.GPIO_22                    /* BIT22 */
#define GPIO_PAD_PU_EN0_P23    _GPIO_PAD_PU_EN0.Bits.GPIO_23                    /* BIT23 */
#define GPIO_PAD_PU_EN0_P24    _GPIO_PAD_PU_EN0.Bits.GPIO_24                    /* BIT24 */
#define GPIO_PAD_PU_EN0_P25    _GPIO_PAD_PU_EN0.Bits.GPIO_25                    /* BIT25 */
#define GPIO_PAD_PU_EN0_P26    _GPIO_PAD_PU_EN0.Bits.GPIO_26                    /* BIT26 */
#define GPIO_PAD_PU_EN0_P27    _GPIO_PAD_PU_EN0.Bits.GPIO_27                    /* BIT27 */
#define GPIO_PAD_PU_EN0_P28    _GPIO_PAD_PU_EN0.Bits.GPIO_28                    /* BIT28 */
#define GPIO_PAD_PU_EN0_P29    _GPIO_PAD_PU_EN0.Bits.GPIO_29                    /* BIT29 */
#define GPIO_PAD_PU_EN0_P30    _GPIO_PAD_PU_EN0.Bits.GPIO_30                    /* BIT30 */
#define GPIO_PAD_PU_EN0_P31    _GPIO_PAD_PU_EN0.Bits.GPIO_31                    /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_PAD_PU_EN1 @(MBAR_GPIO + 0x14);    /* GPIO Pad Pullup Enable for GPIO 00-31 R/W 32 */
#define GPIO_PAD_PU_EN1        _GPIO_PAD_PU_EN1.Reg
#define GPIO_PAD_PU_EN1_P32    _GPIO_PAD_PU_EN1.Bits.GPIO_32                    /* BIT0 */
#define GPIO_PAD_PU_EN1_P33    _GPIO_PAD_PU_EN1.Bits.GPIO_33                    /* BIT1 */
#define GPIO_PAD_PU_EN1_P34    _GPIO_PAD_PU_EN1.Bits.GPIO_34                    /* BIT2 */
#define GPIO_PAD_PU_EN1_P35    _GPIO_PAD_PU_EN1.Bits.GPIO_35                    /* BIT3 */
#define GPIO_PAD_PU_EN1_P36    _GPIO_PAD_PU_EN1.Bits.GPIO_36                    /* BIT4 */
#define GPIO_PAD_PU_EN1_P37    _GPIO_PAD_PU_EN1.Bits.GPIO_37                    /* BIT5 */
#define GPIO_PAD_PU_EN1_P38    _GPIO_PAD_PU_EN1.Bits.GPIO_38                    /* BIT6 */
#define GPIO_PAD_PU_EN1_P39    _GPIO_PAD_PU_EN1.Bits.GPIO_39                    /* BIT7 */
#define GPIO_PAD_PU_EN1_P40    _GPIO_PAD_PU_EN1.Bits.GPIO_40                    /* BIT8 */
#define GPIO_PAD_PU_EN1_P41    _GPIO_PAD_PU_EN1.Bits.GPIO_41                    /* BIT9 */
#define GPIO_PAD_PU_EN1_P42    _GPIO_PAD_PU_EN1.Bits.GPIO_42                    /* BIT10 */
#define GPIO_PAD_PU_EN1_P43    _GPIO_PAD_PU_EN1.Bits.GPIO_43                    /* BIT11 */
#define GPIO_PAD_PU_EN1_P44    _GPIO_PAD_PU_EN1.Bits.GPIO_44                    /* BIT12 */
#define GPIO_PAD_PU_EN1_P45    _GPIO_PAD_PU_EN1.Bits.GPIO_45                    /* BIT13 */
#define GPIO_PAD_PU_EN1_P46    _GPIO_PAD_PU_EN1.Bits.GPIO_46                    /* BIT14 */
#define GPIO_PAD_PU_EN1_P47    _GPIO_PAD_PU_EN1.Bits.GPIO_47                    /* BIT15 */
#define GPIO_PAD_PU_EN1_P48    _GPIO_PAD_PU_EN1.Bits.GPIO_48                    /* BIT16 */
#define GPIO_PAD_PU_EN1_P49    _GPIO_PAD_PU_EN1.Bits.GPIO_49                    /* BIT17 */
#define GPIO_PAD_PU_EN1_P50    _GPIO_PAD_PU_EN1.Bits.GPIO_50                    /* BIT18 */
#define GPIO_PAD_PU_EN1_P51    _GPIO_PAD_PU_EN1.Bits.GPIO_51                    /* BIT19 */
#define GPIO_PAD_PU_EN1_P52    _GPIO_PAD_PU_EN1.Bits.GPIO_52                    /* BIT20 */
#define GPIO_PAD_PU_EN1_P53    _GPIO_PAD_PU_EN1.Bits.GPIO_53                    /* BIT21 */
#define GPIO_PAD_PU_EN1_P54    _GPIO_PAD_PU_EN1.Bits.GPIO_54                    /* BIT22 */
#define GPIO_PAD_PU_EN1_P55    _GPIO_PAD_PU_EN1.Bits.GPIO_55                    /* BIT23 */
#define GPIO_PAD_PU_EN1_P56    _GPIO_PAD_PU_EN1.Bits.GPIO_56                    /* BIT24 */
#define GPIO_PAD_PU_EN1_P57    _GPIO_PAD_PU_EN1.Bits.GPIO_57                    /* BIT25 */
#define GPIO_PAD_PU_EN1_P58    _GPIO_PAD_PU_EN1.Bits.GPIO_58                    /* BIT26 */
#define GPIO_PAD_PU_EN1_P59    _GPIO_PAD_PU_EN1.Bits.GPIO_59                    /* BIT27 */
#define GPIO_PAD_PU_EN1_P60    _GPIO_PAD_PU_EN1.Bits.GPIO_60                    /* BIT28 */
#define GPIO_PAD_PU_EN1_P61    _GPIO_PAD_PU_EN1.Bits.GPIO_61                    /* BIT29 */
#define GPIO_PAD_PU_EN1_P62    _GPIO_PAD_PU_EN1.Bits.GPIO_62                    /* BIT30 */
#define GPIO_PAD_PU_EN1_P63    _GPIO_PAD_PU_EN1.Bits.GPIO_63                    /* BIT31 */


typedef union
{
  struct
  {
    uint32  GPIO_00    :2;
    uint32  GPIO_01    :2;
    uint32  GPIO_02    :2;
    uint32  GPIO_03    :2;
    uint32  GPIO_04    :2;
    uint32  GPIO_05    :2;
    uint32  GPIO_06    :2;
    uint32  GPIO_07    :2;
    uint32  GPIO_08    :2;
    uint32  GPIO_09    :2;
    uint32  GPIO_10    :2;
    uint32  GPIO_11    :2;
    uint32  GPIO_12    :2;
    uint32  GPIO_13    :2;
    uint32  GPIO_14    :2;
    uint32  GPIO_15    :2;
  } Bits;
  uint32 Reg;
} GPIO_FUNC_SEL0_T;
__no_init extern volatile GPIO_FUNC_SEL0_T _GPIO_FUNC_SEL0 @(MBAR_GPIO + 0x18); /* GPIO Function Select for GPIO 00-15 R/W 32 */
#define GPIO_FUNC_SEL0          _GPIO_FUNC_SEL0.Reg
#define GPIO_FUNC_SEL0_P00      _GPIO_FUNC_SEL0.Bits.GPIO_00                    /* BIT0 | BIT1 */
#define GPIO_FUNC_SEL0_P01      _GPIO_FUNC_SEL0.Bits.GPIO_01                    /* BIT2 | BIT3 */
#define GPIO_FUNC_SEL0_P02      _GPIO_FUNC_SEL0.Bits.GPIO_02                    /* BIT4 | BIT5 */
#define GPIO_FUNC_SEL0_P03      _GPIO_FUNC_SEL0.Bits.GPIO_03                    /* BIT6 | BIT7 */
#define GPIO_FUNC_SEL0_P04      _GPIO_FUNC_SEL0.Bits.GPIO_04                    /* BIT8 | BIT9 */
#define GPIO_FUNC_SEL0_P05      _GPIO_FUNC_SEL0.Bits.GPIO_05                    /* BIT10 | BIT11 */
#define GPIO_FUNC_SEL0_P06      _GPIO_FUNC_SEL0.Bits.GPIO_06                    /* BIT12 | BIT13 */
#define GPIO_FUNC_SEL0_P07      _GPIO_FUNC_SEL0.Bits.GPIO_07                    /* BIT14 | BIT15 */
#define GPIO_FUNC_SEL0_P08      _GPIO_FUNC_SEL0.Bits.GPIO_08                    /* BIT16 | BIT17 */
#define GPIO_FUNC_SEL0_P09      _GPIO_FUNC_SEL0.Bits.GPIO_09                    /* BIT18 | BIT19 */
#define GPIO_FUNC_SEL0_P10      _GPIO_FUNC_SEL0.Bits.GPIO_10                    /* BIT20 | BIT21 */
#define GPIO_FUNC_SEL0_P11      _GPIO_FUNC_SEL0.Bits.GPIO_11                    /* BIT22 | BIT23 */
#define GPIO_FUNC_SEL0_P12      _GPIO_FUNC_SEL0.Bits.GPIO_12                    /* BIT24 | BIT25 */
#define GPIO_FUNC_SEL0_P13      _GPIO_FUNC_SEL0.Bits.GPIO_13                    /* BIT26 | BIT27 */
#define GPIO_FUNC_SEL0_P14      _GPIO_FUNC_SEL0.Bits.GPIO_14                    /* BIT28 | BIT29 */
#define GPIO_FUNC_SEL0_P15      _GPIO_FUNC_SEL0.Bits.GPIO_15                    /* BIT30 | BIT31 */


typedef union
{
  struct
  {
    uint32  GPIO_16    :2;
    uint32  GPIO_17    :2;
    uint32  GPIO_18    :2;
    uint32  GPIO_19    :2;
    uint32  GPIO_20    :2;
    uint32  GPIO_21    :2;
    uint32  GPIO_22    :2;
    uint32  GPIO_23    :2;
    uint32  GPIO_24    :2;
    uint32  GPIO_25    :2;
    uint32  GPIO_26    :2;
    uint32  GPIO_27    :2;
    uint32  GPIO_28    :2;
    uint32  GPIO_29    :2;
    uint32  GPIO_30    :2;
    uint32  GPIO_31    :2;
  } Bits;
  uint32 Reg;
} GPIO_FUNC_SEL1_T;
__no_init extern volatile GPIO_FUNC_SEL1_T _GPIO_FUNC_SEL1 @(MBAR_GPIO + 0x1C); /* GPIO Function Select for GPIO 16-31 R/W 32 */
#define GPIO_FUNC_SEL1          _GPIO_FUNC_SEL1.Reg
#define GPIO_FUNC_SEL1_P16      _GPIO_FUNC_SEL1.Bits.GPIO_16                    /* BIT0 | BIT1 */
#define GPIO_FUNC_SEL1_P17      _GPIO_FUNC_SEL1.Bits.GPIO_17                    /* BIT2 | BIT3 */
#define GPIO_FUNC_SEL1_P18      _GPIO_FUNC_SEL1.Bits.GPIO_18                    /* BIT4 | BIT5 */
#define GPIO_FUNC_SEL1_P19      _GPIO_FUNC_SEL1.Bits.GPIO_19                    /* BIT6 | BIT7 */
#define GPIO_FUNC_SEL1_P20      _GPIO_FUNC_SEL1.Bits.GPIO_20                    /* BIT8 | BIT9 */
#define GPIO_FUNC_SEL1_P21      _GPIO_FUNC_SEL1.Bits.GPIO_21                    /* BIT10 | BIT11 */
#define GPIO_FUNC_SEL1_P22      _GPIO_FUNC_SEL1.Bits.GPIO_22                    /* BIT12 | BIT13 */
#define GPIO_FUNC_SEL1_P23      _GPIO_FUNC_SEL1.Bits.GPIO_23                    /* BIT14 | BIT15 */
#define GPIO_FUNC_SEL1_P24      _GPIO_FUNC_SEL1.Bits.GPIO_24                    /* BIT16 | BIT17 */
#define GPIO_FUNC_SEL1_P25      _GPIO_FUNC_SEL1.Bits.GPIO_25                    /* BIT18 | BIT19 */
#define GPIO_FUNC_SEL1_P26      _GPIO_FUNC_SEL1.Bits.GPIO_26                    /* BIT20 | BIT21 */
#define GPIO_FUNC_SEL1_P27      _GPIO_FUNC_SEL1.Bits.GPIO_27                    /* BIT22 | BIT23 */
#define GPIO_FUNC_SEL1_P28      _GPIO_FUNC_SEL1.Bits.GPIO_28                    /* BIT24 | BIT25 */
#define GPIO_FUNC_SEL1_P29      _GPIO_FUNC_SEL1.Bits.GPIO_29                    /* BIT26 | BIT27 */
#define GPIO_FUNC_SEL1_P30      _GPIO_FUNC_SEL1.Bits.GPIO_30                    /* BIT28 | BIT29 */
#define GPIO_FUNC_SEL1_P31      _GPIO_FUNC_SEL1.Bits.GPIO_31                    /* BIT30 | BIT31 */


typedef union
{
  struct
  {
    uint32  GPIO_32    :2;
    uint32  GPIO_33    :2;
    uint32  GPIO_34    :2;
    uint32  GPIO_35    :2;
    uint32  GPIO_36    :2;
    uint32  GPIO_37    :2;
    uint32  GPIO_38    :2;
    uint32  GPIO_39    :2;
    uint32  GPIO_40    :2;
    uint32  GPIO_41    :2;
    uint32  GPIO_42    :2;
    uint32  GPIO_43    :2;
    uint32  GPIO_44    :2;
    uint32  GPIO_45    :2;
    uint32  GPIO_46    :2;
    uint32  GPIO_47    :2;
  } Bits;
  uint32 Reg;
} GPIO_FUNC_SEL2_T;
__no_init extern volatile GPIO_FUNC_SEL2_T _GPIO_FUNC_SEL2 @(MBAR_GPIO + 0x20); /* GPIO Function Select for GPIO 32-47 R/W 32 */
#define GPIO_FUNC_SEL2          _GPIO_FUNC_SEL2.Reg
#define GPIO_FUNC_SEL2_P32      _GPIO_FUNC_SEL2.Bits.GPIO_32                    /* BIT0 | BIT1 */
#define GPIO_FUNC_SEL2_P33      _GPIO_FUNC_SEL2.Bits.GPIO_33                    /* BIT2 | BIT3 */
#define GPIO_FUNC_SEL2_P34      _GPIO_FUNC_SEL2.Bits.GPIO_34                    /* BIT4 | BIT5 */
#define GPIO_FUNC_SEL2_P35      _GPIO_FUNC_SEL2.Bits.GPIO_35                    /* BIT6 | BIT7 */
#define GPIO_FUNC_SEL2_P36      _GPIO_FUNC_SEL2.Bits.GPIO_36                    /* BIT8 | BIT9 */
#define GPIO_FUNC_SEL2_P37      _GPIO_FUNC_SEL2.Bits.GPIO_37                    /* BIT10 | BIT11 */
#define GPIO_FUNC_SEL2_P38      _GPIO_FUNC_SEL2.Bits.GPIO_38                    /* BIT12 | BIT13 */
#define GPIO_FUNC_SEL2_P39      _GPIO_FUNC_SEL2.Bits.GPIO_39                    /* BIT14 | BIT15 */
#define GPIO_FUNC_SEL2_P40      _GPIO_FUNC_SEL2.Bits.GPIO_40                    /* BIT16 | BIT17 */
#define GPIO_FUNC_SEL2_P41      _GPIO_FUNC_SEL2.Bits.GPIO_41                    /* BIT18 | BIT19 */
#define GPIO_FUNC_SEL2_P42      _GPIO_FUNC_SEL2.Bits.GPIO_42                    /* BIT20 | BIT21 */
#define GPIO_FUNC_SEL2_P43      _GPIO_FUNC_SEL2.Bits.GPIO_43                    /* BIT22 | BIT23 */
#define GPIO_FUNC_SEL2_P44      _GPIO_FUNC_SEL2.Bits.GPIO_44                    /* BIT24 | BIT25 */
#define GPIO_FUNC_SEL2_P45      _GPIO_FUNC_SEL2.Bits.GPIO_45                    /* BIT26 | BIT27 */
#define GPIO_FUNC_SEL2_P46      _GPIO_FUNC_SEL2.Bits.GPIO_46                    /* BIT28 | BIT29 */
#define GPIO_FUNC_SEL2_P47      _GPIO_FUNC_SEL2.Bits.GPIO_47                    /* BIT30 | BIT31 */



typedef union
{
  struct
  {
    uint32  GPIO_48    :2;
    uint32  GPIO_49    :2;
    uint32  GPIO_50    :2;
    uint32  GPIO_51    :2;
    uint32  GPIO_52    :2;
    uint32  GPIO_53    :2;
    uint32  GPIO_54    :2;
    uint32  GPIO_55    :2;
    uint32  GPIO_56    :2;
    uint32  GPIO_57    :2;
    uint32  GPIO_58    :2;
    uint32  GPIO_59    :2;
    uint32  GPIO_60    :2;
    uint32  GPIO_61    :2;
    uint32  GPIO_62    :2;
    uint32  GPIO_63    :2;
  } Bits;
  uint32 Reg;
} GPIO_FUNC_SEL3_T;
__no_init extern volatile GPIO_FUNC_SEL3_T _GPIO_FUNC_SEL3 @(MBAR_GPIO + 0x24); /* GPIO Function Select for GPIO 48-63 R/W 32 */
#define GPIO_FUNC_SEL3          _GPIO_FUNC_SEL3.Reg
#define GPIO_FUNC_SEL3_P48      _GPIO_FUNC_SEL3.Bits.GPIO_48                    /* BIT0 | BIT1 */
#define GPIO_FUNC_SEL3_P49      _GPIO_FUNC_SEL3.Bits.GPIO_49                    /* BIT2 | BIT3 */
#define GPIO_FUNC_SEL3_P50      _GPIO_FUNC_SEL3.Bits.GPIO_50                    /* BIT4 | BIT5 */
#define GPIO_FUNC_SEL3_P51      _GPIO_FUNC_SEL3.Bits.GPIO_51                    /* BIT6 | BIT7 */
#define GPIO_FUNC_SEL3_P52      _GPIO_FUNC_SEL3.Bits.GPIO_52                    /* BIT8 | BIT9 */
#define GPIO_FUNC_SEL3_P53      _GPIO_FUNC_SEL3.Bits.GPIO_53                    /* BIT10 | BIT11 */
#define GPIO_FUNC_SEL3_P54      _GPIO_FUNC_SEL3.Bits.GPIO_54                    /* BIT12 | BIT13 */
#define GPIO_FUNC_SEL3_P55      _GPIO_FUNC_SEL3.Bits.GPIO_55                    /* BIT14 | BIT15 */
#define GPIO_FUNC_SEL3_P56      _GPIO_FUNC_SEL3.Bits.GPIO_56                    /* BIT16 | BIT17 */
#define GPIO_FUNC_SEL3_P57      _GPIO_FUNC_SEL3.Bits.GPIO_57                    /* BIT18 | BIT19 */
#define GPIO_FUNC_SEL3_P58      _GPIO_FUNC_SEL3.Bits.GPIO_58                    /* BIT20 | BIT21 */
#define GPIO_FUNC_SEL3_P59      _GPIO_FUNC_SEL3.Bits.GPIO_59                    /* BIT22 | BIT23 */
#define GPIO_FUNC_SEL3_P60      _GPIO_FUNC_SEL3.Bits.GPIO_60                    /* BIT24 | BIT25 */
#define GPIO_FUNC_SEL3_P61      _GPIO_FUNC_SEL3.Bits.GPIO_61                    /* BIT26 | BIT27 */
#define GPIO_FUNC_SEL3_P62      _GPIO_FUNC_SEL3.Bits.GPIO_62                    /* BIT28 | BIT29 */
#define GPIO_FUNC_SEL3_P63      _GPIO_FUNC_SEL3.Bits.GPIO_63                    /* BIT30 | BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_DATA_SEL0 @(MBAR_GPIO + 0x28);     /* GPIO Data Select for GPIO 00-31 R/W 32 */
#define GPIO_DATA_SEL0        _GPIO_DATA_SEL0.Reg
#define GPIO_DATA_SEL0_P00    _GPIO_DATA_SEL0.Bits.GPIO_00                      /* BIT0 */
#define GPIO_DATA_SEL0_P01    _GPIO_DATA_SEL0.Bits.GPIO_01                      /* BIT1 */
#define GPIO_DATA_SEL0_P02    _GPIO_DATA_SEL0.Bits.GPIO_02                      /* BIT2 */
#define GPIO_DATA_SEL0_P03    _GPIO_DATA_SEL0.Bits.GPIO_03                      /* BIT3 */
#define GPIO_DATA_SEL0_P04    _GPIO_DATA_SEL0.Bits.GPIO_04                      /* BIT4 */
#define GPIO_DATA_SEL0_P05    _GPIO_DATA_SEL0.Bits.GPIO_05                      /* BIT5 */
#define GPIO_DATA_SEL0_P06    _GPIO_DATA_SEL0.Bits.GPIO_06                      /* BIT6 */
#define GPIO_DATA_SEL0_P07    _GPIO_DATA_SEL0.Bits.GPIO_07                      /* BIT7 */
#define GPIO_DATA_SEL0_P08    _GPIO_DATA_SEL0.Bits.GPIO_08                      /* BIT8 */
#define GPIO_DATA_SEL0_P09    _GPIO_DATA_SEL0.Bits.GPIO_09                      /* BIT9 */
#define GPIO_DATA_SEL0_P10    _GPIO_DATA_SEL0.Bits.GPIO_10                      /* BIT10 */
#define GPIO_DATA_SEL0_P11    _GPIO_DATA_SEL0.Bits.GPIO_11                      /* BIT11 */
#define GPIO_DATA_SEL0_P12    _GPIO_DATA_SEL0.Bits.GPIO_12                      /* BIT12 */
#define GPIO_DATA_SEL0_P13    _GPIO_DATA_SEL0.Bits.GPIO_13                      /* BIT13 */
#define GPIO_DATA_SEL0_P14    _GPIO_DATA_SEL0.Bits.GPIO_14                      /* BIT14 */
#define GPIO_DATA_SEL0_P15    _GPIO_DATA_SEL0.Bits.GPIO_15                      /* BIT15 */
#define GPIO_DATA_SEL0_P16    _GPIO_DATA_SEL0.Bits.GPIO_16                      /* BIT16 */
#define GPIO_DATA_SEL0_P17    _GPIO_DATA_SEL0.Bits.GPIO_17                      /* BIT17 */
#define GPIO_DATA_SEL0_P18    _GPIO_DATA_SEL0.Bits.GPIO_18                      /* BIT18 */
#define GPIO_DATA_SEL0_P19    _GPIO_DATA_SEL0.Bits.GPIO_19                      /* BIT19 */
#define GPIO_DATA_SEL0_P20    _GPIO_DATA_SEL0.Bits.GPIO_20                      /* BIT20 */
#define GPIO_DATA_SEL0_P21    _GPIO_DATA_SEL0.Bits.GPIO_21                      /* BIT21 */
#define GPIO_DATA_SEL0_P22    _GPIO_DATA_SEL0.Bits.GPIO_22                      /* BIT22 */
#define GPIO_DATA_SEL0_P23    _GPIO_DATA_SEL0.Bits.GPIO_23                      /* BIT23 */
#define GPIO_DATA_SEL0_P24    _GPIO_DATA_SEL0.Bits.GPIO_24                      /* BIT24 */
#define GPIO_DATA_SEL0_P25    _GPIO_DATA_SEL0.Bits.GPIO_25                      /* BIT25 */
#define GPIO_DATA_SEL0_P26    _GPIO_DATA_SEL0.Bits.GPIO_26                      /* BIT26 */
#define GPIO_DATA_SEL0_P27    _GPIO_DATA_SEL0.Bits.GPIO_27                      /* BIT27 */
#define GPIO_DATA_SEL0_P28    _GPIO_DATA_SEL0.Bits.GPIO_28                      /* BIT28 */
#define GPIO_DATA_SEL0_P29    _GPIO_DATA_SEL0.Bits.GPIO_29                      /* BIT29 */
#define GPIO_DATA_SEL0_P30    _GPIO_DATA_SEL0.Bits.GPIO_30                      /* BIT30 */
#define GPIO_DATA_SEL0_P31    _GPIO_DATA_SEL0.Bits.GPIO_31                      /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_DATA_SEL1 @(MBAR_GPIO + 0x2C);     /* GPIO Data Select for GPIO 32-63 R/W 32 */
#define GPIO_DATA_SEL1        _GPIO_DATA_SEL1.Reg
#define GPIO_DATA_SEL1_P32    _GPIO_DATA_SEL1.Bits.GPIO_32                      /* BIT0 */
#define GPIO_DATA_SEL1_P33    _GPIO_DATA_SEL1.Bits.GPIO_33                      /* BIT1 */
#define GPIO_DATA_SEL1_P34    _GPIO_DATA_SEL1.Bits.GPIO_34                      /* BIT2 */
#define GPIO_DATA_SEL1_P35    _GPIO_DATA_SEL1.Bits.GPIO_35                      /* BIT3 */
#define GPIO_DATA_SEL1_P36    _GPIO_DATA_SEL1.Bits.GPIO_36                      /* BIT4 */
#define GPIO_DATA_SEL1_P37    _GPIO_DATA_SEL1.Bits.GPIO_37                      /* BIT5 */
#define GPIO_DATA_SEL1_P38    _GPIO_DATA_SEL1.Bits.GPIO_38                      /* BIT6 */
#define GPIO_DATA_SEL1_P39    _GPIO_DATA_SEL1.Bits.GPIO_39                      /* BIT7 */
#define GPIO_DATA_SEL1_P40    _GPIO_DATA_SEL1.Bits.GPIO_40                      /* BIT8 */
#define GPIO_DATA_SEL1_P41    _GPIO_DATA_SEL1.Bits.GPIO_41                      /* BIT9 */
#define GPIO_DATA_SEL1_P42    _GPIO_DATA_SEL1.Bits.GPIO_42                      /* BIT10 */
#define GPIO_DATA_SEL1_P43    _GPIO_DATA_SEL1.Bits.GPIO_43                      /* BIT11 */
#define GPIO_DATA_SEL1_P44    _GPIO_DATA_SEL1.Bits.GPIO_44                      /* BIT12 */
#define GPIO_DATA_SEL1_P45    _GPIO_DATA_SEL1.Bits.GPIO_45                      /* BIT13 */
#define GPIO_DATA_SEL1_P46    _GPIO_DATA_SEL1.Bits.GPIO_46                      /* BIT14 */
#define GPIO_DATA_SEL1_P47    _GPIO_DATA_SEL1.Bits.GPIO_47                      /* BIT15 */
#define GPIO_DATA_SEL1_P48    _GPIO_DATA_SEL1.Bits.GPIO_48                      /* BIT16 */
#define GPIO_DATA_SEL1_P49    _GPIO_DATA_SEL1.Bits.GPIO_49                      /* BIT17 */
#define GPIO_DATA_SEL1_P50    _GPIO_DATA_SEL1.Bits.GPIO_50                      /* BIT18 */
#define GPIO_DATA_SEL1_P51    _GPIO_DATA_SEL1.Bits.GPIO_51                      /* BIT19 */
#define GPIO_DATA_SEL1_P52    _GPIO_DATA_SEL1.Bits.GPIO_52                      /* BIT20 */
#define GPIO_DATA_SEL1_P53    _GPIO_DATA_SEL1.Bits.GPIO_53                      /* BIT21 */
#define GPIO_DATA_SEL1_P54    _GPIO_DATA_SEL1.Bits.GPIO_54                      /* BIT22 */
#define GPIO_DATA_SEL1_P55    _GPIO_DATA_SEL1.Bits.GPIO_55                      /* BIT23 */
#define GPIO_DATA_SEL1_P56    _GPIO_DATA_SEL1.Bits.GPIO_56                      /* BIT24 */
#define GPIO_DATA_SEL1_P57    _GPIO_DATA_SEL1.Bits.GPIO_57                      /* BIT25 */
#define GPIO_DATA_SEL1_P58    _GPIO_DATA_SEL1.Bits.GPIO_58                      /* BIT26 */
#define GPIO_DATA_SEL1_P59    _GPIO_DATA_SEL1.Bits.GPIO_59                      /* BIT27 */
#define GPIO_DATA_SEL1_P60    _GPIO_DATA_SEL1.Bits.GPIO_60                      /* BIT28 */
#define GPIO_DATA_SEL1_P61    _GPIO_DATA_SEL1.Bits.GPIO_61                      /* BIT29 */
#define GPIO_DATA_SEL1_P62    _GPIO_DATA_SEL1.Bits.GPIO_62                      /* BIT30 */
#define GPIO_DATA_SEL1_P63    _GPIO_DATA_SEL1.Bits.GPIO_63                      /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_PAD_PU_SEL0 @(MBAR_GPIO + 0x30);   /* GPIO Pad Pullup Select for GPIO 00-31 R/W 32 */
#define GPIO_PAD_PU_SEL0        _GPIO_PAD_PU_SEL0.Reg
#define GPIO_PAD_PU_SEL0_P00    _GPIO_PAD_PU_SEL0.Bits.GPIO_00                  /* BIT0 */
#define GPIO_PAD_PU_SEL0_P01    _GPIO_PAD_PU_SEL0.Bits.GPIO_01                  /* BIT1 */
#define GPIO_PAD_PU_SEL0_P02    _GPIO_PAD_PU_SEL0.Bits.GPIO_02                  /* BIT2 */
#define GPIO_PAD_PU_SEL0_P03    _GPIO_PAD_PU_SEL0.Bits.GPIO_03                  /* BIT3 */
#define GPIO_PAD_PU_SEL0_P04    _GPIO_PAD_PU_SEL0.Bits.GPIO_04                  /* BIT4 */
#define GPIO_PAD_PU_SEL0_P05    _GPIO_PAD_PU_SEL0.Bits.GPIO_05                  /* BIT5 */
#define GPIO_PAD_PU_SEL0_P06    _GPIO_PAD_PU_SEL0.Bits.GPIO_06                  /* BIT6 */
#define GPIO_PAD_PU_SEL0_P07    _GPIO_PAD_PU_SEL0.Bits.GPIO_07                  /* BIT7 */
#define GPIO_PAD_PU_SEL0_P08    _GPIO_PAD_PU_SEL0.Bits.GPIO_08                  /* BIT8 */
#define GPIO_PAD_PU_SEL0_P09    _GPIO_PAD_PU_SEL0.Bits.GPIO_09                  /* BIT9 */
#define GPIO_PAD_PU_SEL0_P10    _GPIO_PAD_PU_SEL0.Bits.GPIO_10                  /* BIT10 */
#define GPIO_PAD_PU_SEL0_P11    _GPIO_PAD_PU_SEL0.Bits.GPIO_11                  /* BIT11 */
#define GPIO_PAD_PU_SEL0_P12    _GPIO_PAD_PU_SEL0.Bits.GPIO_12                  /* BIT12 */
#define GPIO_PAD_PU_SEL0_P13    _GPIO_PAD_PU_SEL0.Bits.GPIO_13                  /* BIT13 */
#define GPIO_PAD_PU_SEL0_P14    _GPIO_PAD_PU_SEL0.Bits.GPIO_14                  /* BIT14 */
#define GPIO_PAD_PU_SEL0_P15    _GPIO_PAD_PU_SEL0.Bits.GPIO_15                  /* BIT15 */
#define GPIO_PAD_PU_SEL0_P16    _GPIO_PAD_PU_SEL0.Bits.GPIO_16                  /* BIT16 */
#define GPIO_PAD_PU_SEL0_P17    _GPIO_PAD_PU_SEL0.Bits.GPIO_17                  /* BIT17 */
#define GPIO_PAD_PU_SEL0_P18    _GPIO_PAD_PU_SEL0.Bits.GPIO_18                  /* BIT18 */
#define GPIO_PAD_PU_SEL0_P19    _GPIO_PAD_PU_SEL0.Bits.GPIO_19                  /* BIT19 */
#define GPIO_PAD_PU_SEL0_P20    _GPIO_PAD_PU_SEL0.Bits.GPIO_20                  /* BIT20 */
#define GPIO_PAD_PU_SEL0_P21    _GPIO_PAD_PU_SEL0.Bits.GPIO_21                  /* BIT21 */
#define GPIO_PAD_PU_SEL0_P22    _GPIO_PAD_PU_SEL0.Bits.GPIO_22                  /* BIT22 */
#define GPIO_PAD_PU_SEL0_P23    _GPIO_PAD_PU_SEL0.Bits.GPIO_23                  /* BIT23 */
#define GPIO_PAD_PU_SEL0_P24    _GPIO_PAD_PU_SEL0.Bits.GPIO_24                  /* BIT24 */
#define GPIO_PAD_PU_SEL0_P25    _GPIO_PAD_PU_SEL0.Bits.GPIO_25                  /* BIT25 */
#define GPIO_PAD_PU_SEL0_P26    _GPIO_PAD_PU_SEL0.Bits.GPIO_26                  /* BIT26 */
#define GPIO_PAD_PU_SEL0_P27    _GPIO_PAD_PU_SEL0.Bits.GPIO_27                  /* BIT27 */
#define GPIO_PAD_PU_SEL0_P28    _GPIO_PAD_PU_SEL0.Bits.GPIO_28                  /* BIT28 */
#define GPIO_PAD_PU_SEL0_P29    _GPIO_PAD_PU_SEL0.Bits.GPIO_29                  /* BIT29 */
#define GPIO_PAD_PU_SEL0_P30    _GPIO_PAD_PU_SEL0.Bits.GPIO_30                  /* BIT30 */
#define GPIO_PAD_PU_SEL0_P31    _GPIO_PAD_PU_SEL0.Bits.GPIO_31                  /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_PAD_PU_SEL1 @(MBAR_GPIO + 0x34);   /* GPIO Pad Pullup Select for GPIO 32-63 R/W 32 */
#define GPIO_PAD_PU_SEL1        _GPIO_PAD_PU_SEL1.Reg
#define GPIO_PAD_PU_SEL1_P32    _GPIO_PAD_PU_SEL1.Bits.GPIO_32                  /* BIT0 */
#define GPIO_PAD_PU_SEL1_P33    _GPIO_PAD_PU_SEL1.Bits.GPIO_33                  /* BIT1 */
#define GPIO_PAD_PU_SEL1_P34    _GPIO_PAD_PU_SEL1.Bits.GPIO_34                  /* BIT2 */
#define GPIO_PAD_PU_SEL1_P35    _GPIO_PAD_PU_SEL1.Bits.GPIO_35                  /* BIT3 */
#define GPIO_PAD_PU_SEL1_P36    _GPIO_PAD_PU_SEL1.Bits.GPIO_36                  /* BIT4 */
#define GPIO_PAD_PU_SEL1_P37    _GPIO_PAD_PU_SEL1.Bits.GPIO_37                  /* BIT5 */
#define GPIO_PAD_PU_SEL1_P38    _GPIO_PAD_PU_SEL1.Bits.GPIO_38                  /* BIT6 */
#define GPIO_PAD_PU_SEL1_P39    _GPIO_PAD_PU_SEL1.Bits.GPIO_39                  /* BIT7 */
#define GPIO_PAD_PU_SEL1_P40    _GPIO_PAD_PU_SEL1.Bits.GPIO_40                  /* BIT8 */
#define GPIO_PAD_PU_SEL1_P41    _GPIO_PAD_PU_SEL1.Bits.GPIO_41                  /* BIT9 */
#define GPIO_PAD_PU_SEL1_P42    _GPIO_PAD_PU_SEL1.Bits.GPIO_42                  /* BIT10 */
#define GPIO_PAD_PU_SEL1_P43    _GPIO_PAD_PU_SEL1.Bits.GPIO_43                  /* BIT11 */
#define GPIO_PAD_PU_SEL1_P44    _GPIO_PAD_PU_SEL1.Bits.GPIO_44                  /* BIT12 */
#define GPIO_PAD_PU_SEL1_P45    _GPIO_PAD_PU_SEL1.Bits.GPIO_45                  /* BIT13 */
#define GPIO_PAD_PU_SEL1_P46    _GPIO_PAD_PU_SEL1.Bits.GPIO_46                  /* BIT14 */
#define GPIO_PAD_PU_SEL1_P47    _GPIO_PAD_PU_SEL1.Bits.GPIO_47                  /* BIT15 */
#define GPIO_PAD_PU_SEL1_P48    _GPIO_PAD_PU_SEL1.Bits.GPIO_48                  /* BIT16 */
#define GPIO_PAD_PU_SEL1_P49    _GPIO_PAD_PU_SEL1.Bits.GPIO_49                  /* BIT17 */
#define GPIO_PAD_PU_SEL1_P50    _GPIO_PAD_PU_SEL1.Bits.GPIO_50                  /* BIT18 */
#define GPIO_PAD_PU_SEL1_P51    _GPIO_PAD_PU_SEL1.Bits.GPIO_51                  /* BIT19 */
#define GPIO_PAD_PU_SEL1_P52    _GPIO_PAD_PU_SEL1.Bits.GPIO_52                  /* BIT20 */
#define GPIO_PAD_PU_SEL1_P53    _GPIO_PAD_PU_SEL1.Bits.GPIO_53                  /* BIT21 */
#define GPIO_PAD_PU_SEL1_P54    _GPIO_PAD_PU_SEL1.Bits.GPIO_54                  /* BIT22 */
#define GPIO_PAD_PU_SEL1_P55    _GPIO_PAD_PU_SEL1.Bits.GPIO_55                  /* BIT23 */
#define GPIO_PAD_PU_SEL1_P56    _GPIO_PAD_PU_SEL1.Bits.GPIO_56                  /* BIT24 */
#define GPIO_PAD_PU_SEL1_P57    _GPIO_PAD_PU_SEL1.Bits.GPIO_57                  /* BIT25 */
#define GPIO_PAD_PU_SEL1_P58    _GPIO_PAD_PU_SEL1.Bits.GPIO_58                  /* BIT26 */
#define GPIO_PAD_PU_SEL1_P59    _GPIO_PAD_PU_SEL1.Bits.GPIO_59                  /* BIT27 */
#define GPIO_PAD_PU_SEL1_P60    _GPIO_PAD_PU_SEL1.Bits.GPIO_60                  /* BIT28 */
#define GPIO_PAD_PU_SEL1_P61    _GPIO_PAD_PU_SEL1.Bits.GPIO_61                  /* BIT29 */
#define GPIO_PAD_PU_SEL1_P62    _GPIO_PAD_PU_SEL1.Bits.GPIO_62                  /* BIT30 */
#define GPIO_PAD_PU_SEL1_P63    _GPIO_PAD_PU_SEL1.Bits.GPIO_63                  /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_PAD_HYST_EN0 @(MBAR_GPIO + 0x38);  /* GPIO Pad Hysteresis Enable for GPIO 00-31 R/W 32 */
#define GPIO_PAD_HYST_EN0        _GPIO_PAD_HYST_EN0.Reg
#define GPIO_PAD_HYST_EN0_P00    _GPIO_PAD_HYST_EN0.Bits.GPIO_00                /* BIT0 */
#define GPIO_PAD_HYST_EN0_P01    _GPIO_PAD_HYST_EN0.Bits.GPIO_01                /* BIT1 */
#define GPIO_PAD_HYST_EN0_P02    _GPIO_PAD_HYST_EN0.Bits.GPIO_02                /* BIT2 */
#define GPIO_PAD_HYST_EN0_P03    _GPIO_PAD_HYST_EN0.Bits.GPIO_03                /* BIT3 */
#define GPIO_PAD_HYST_EN0_P04    _GPIO_PAD_HYST_EN0.Bits.GPIO_04                /* BIT4 */
#define GPIO_PAD_HYST_EN0_P05    _GPIO_PAD_HYST_EN0.Bits.GPIO_05                /* BIT5 */
#define GPIO_PAD_HYST_EN0_P06    _GPIO_PAD_HYST_EN0.Bits.GPIO_06                /* BIT6 */
#define GPIO_PAD_HYST_EN0_P07    _GPIO_PAD_HYST_EN0.Bits.GPIO_07                /* BIT7 */
#define GPIO_PAD_HYST_EN0_P08    _GPIO_PAD_HYST_EN0.Bits.GPIO_08                /* BIT8 */
#define GPIO_PAD_HYST_EN0_P09    _GPIO_PAD_HYST_EN0.Bits.GPIO_09                /* BIT9 */
#define GPIO_PAD_HYST_EN0_P10    _GPIO_PAD_HYST_EN0.Bits.GPIO_10                /* BIT10 */
#define GPIO_PAD_HYST_EN0_P11    _GPIO_PAD_HYST_EN0.Bits.GPIO_11                /* BIT11 */
#define GPIO_PAD_HYST_EN0_P12    _GPIO_PAD_HYST_EN0.Bits.GPIO_12                /* BIT12 */
#define GPIO_PAD_HYST_EN0_P13    _GPIO_PAD_HYST_EN0.Bits.GPIO_13                /* BIT13 */
#define GPIO_PAD_HYST_EN0_P14    _GPIO_PAD_HYST_EN0.Bits.GPIO_14                /* BIT14 */
#define GPIO_PAD_HYST_EN0_P15    _GPIO_PAD_HYST_EN0.Bits.GPIO_15                /* BIT15 */
#define GPIO_PAD_HYST_EN0_P16    _GPIO_PAD_HYST_EN0.Bits.GPIO_16                /* BIT16 */
#define GPIO_PAD_HYST_EN0_P17    _GPIO_PAD_HYST_EN0.Bits.GPIO_17                /* BIT17 */
#define GPIO_PAD_HYST_EN0_P18    _GPIO_PAD_HYST_EN0.Bits.GPIO_18                /* BIT18 */
#define GPIO_PAD_HYST_EN0_P19    _GPIO_PAD_HYST_EN0.Bits.GPIO_19                /* BIT19 */
#define GPIO_PAD_HYST_EN0_P20    _GPIO_PAD_HYST_EN0.Bits.GPIO_20                /* BIT20 */
#define GPIO_PAD_HYST_EN0_P21    _GPIO_PAD_HYST_EN0.Bits.GPIO_21                /* BIT21 */
#define GPIO_PAD_HYST_EN0_P22    _GPIO_PAD_HYST_EN0.Bits.GPIO_22                /* BIT22 */
#define GPIO_PAD_HYST_EN0_P23    _GPIO_PAD_HYST_EN0.Bits.GPIO_23                /* BIT23 */
#define GPIO_PAD_HYST_EN0_P24    _GPIO_PAD_HYST_EN0.Bits.GPIO_24                /* BIT24 */
#define GPIO_PAD_HYST_EN0_P25    _GPIO_PAD_HYST_EN0.Bits.GPIO_25                /* BIT25 */
#define GPIO_PAD_HYST_EN0_P26    _GPIO_PAD_HYST_EN0.Bits.GPIO_26                /* BIT26 */
#define GPIO_PAD_HYST_EN0_P27    _GPIO_PAD_HYST_EN0.Bits.GPIO_27                /* BIT27 */
#define GPIO_PAD_HYST_EN0_P28    _GPIO_PAD_HYST_EN0.Bits.GPIO_28                /* BIT28 */
#define GPIO_PAD_HYST_EN0_P29    _GPIO_PAD_HYST_EN0.Bits.GPIO_29                /* BIT29 */
#define GPIO_PAD_HYST_EN0_P30    _GPIO_PAD_HYST_EN0.Bits.GPIO_30                /* BIT30 */
#define GPIO_PAD_HYST_EN0_P31    _GPIO_PAD_HYST_EN0.Bits.GPIO_31                /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_PAD_HYST_EN1 @(MBAR_GPIO + 0x3C);  /* GPIO Pad Hysteresis Enable for GPIO 32-63 R/W 32 */
#define GPIO_PAD_HYST_EN1        _GPIO_PAD_HYST_EN1.Reg
#define GPIO_PAD_HYST_EN1_P32    _GPIO_PAD_HYST_EN1.Bits.GPIO_32                /* BIT0 */
#define GPIO_PAD_HYST_EN1_P33    _GPIO_PAD_HYST_EN1.Bits.GPIO_33                /* BIT1 */
#define GPIO_PAD_HYST_EN1_P34    _GPIO_PAD_HYST_EN1.Bits.GPIO_34                /* BIT2 */
#define GPIO_PAD_HYST_EN1_P35    _GPIO_PAD_HYST_EN1.Bits.GPIO_35                /* BIT3 */
#define GPIO_PAD_HYST_EN1_P36    _GPIO_PAD_HYST_EN1.Bits.GPIO_36                /* BIT4 */
#define GPIO_PAD_HYST_EN1_P37    _GPIO_PAD_HYST_EN1.Bits.GPIO_37                /* BIT5 */
#define GPIO_PAD_HYST_EN1_P38    _GPIO_PAD_HYST_EN1.Bits.GPIO_38                /* BIT6 */
#define GPIO_PAD_HYST_EN1_P39    _GPIO_PAD_HYST_EN1.Bits.GPIO_39                /* BIT7 */
#define GPIO_PAD_HYST_EN1_P40    _GPIO_PAD_HYST_EN1.Bits.GPIO_40                /* BIT8 */
#define GPIO_PAD_HYST_EN1_P41    _GPIO_PAD_HYST_EN1.Bits.GPIO_41                /* BIT9 */
#define GPIO_PAD_HYST_EN1_P42    _GPIO_PAD_HYST_EN1.Bits.GPIO_42                /* BIT10 */
#define GPIO_PAD_HYST_EN1_P43    _GPIO_PAD_HYST_EN1.Bits.GPIO_43                /* BIT11 */
#define GPIO_PAD_HYST_EN1_P44    _GPIO_PAD_HYST_EN1.Bits.GPIO_44                /* BIT12 */
#define GPIO_PAD_HYST_EN1_P45    _GPIO_PAD_HYST_EN1.Bits.GPIO_45                /* BIT13 */
#define GPIO_PAD_HYST_EN1_P46    _GPIO_PAD_HYST_EN1.Bits.GPIO_46                /* BIT14 */
#define GPIO_PAD_HYST_EN1_P47    _GPIO_PAD_HYST_EN1.Bits.GPIO_47                /* BIT15 */
#define GPIO_PAD_HYST_EN1_P48    _GPIO_PAD_HYST_EN1.Bits.GPIO_48                /* BIT16 */
#define GPIO_PAD_HYST_EN1_P49    _GPIO_PAD_HYST_EN1.Bits.GPIO_49                /* BIT17 */
#define GPIO_PAD_HYST_EN1_P50    _GPIO_PAD_HYST_EN1.Bits.GPIO_50                /* BIT18 */
#define GPIO_PAD_HYST_EN1_P51    _GPIO_PAD_HYST_EN1.Bits.GPIO_51                /* BIT19 */
#define GPIO_PAD_HYST_EN1_P52    _GPIO_PAD_HYST_EN1.Bits.GPIO_52                /* BIT20 */
#define GPIO_PAD_HYST_EN1_P53    _GPIO_PAD_HYST_EN1.Bits.GPIO_53                /* BIT21 */
#define GPIO_PAD_HYST_EN1_P54    _GPIO_PAD_HYST_EN1.Bits.GPIO_54                /* BIT22 */
#define GPIO_PAD_HYST_EN1_P55    _GPIO_PAD_HYST_EN1.Bits.GPIO_55                /* BIT23 */
#define GPIO_PAD_HYST_EN1_P56    _GPIO_PAD_HYST_EN1.Bits.GPIO_56                /* BIT24 */
#define GPIO_PAD_HYST_EN1_P57    _GPIO_PAD_HYST_EN1.Bits.GPIO_57                /* BIT25 */
#define GPIO_PAD_HYST_EN1_P58    _GPIO_PAD_HYST_EN1.Bits.GPIO_58                /* BIT26 */
#define GPIO_PAD_HYST_EN1_P59    _GPIO_PAD_HYST_EN1.Bits.GPIO_59                /* BIT27 */
#define GPIO_PAD_HYST_EN1_P60    _GPIO_PAD_HYST_EN1.Bits.GPIO_60                /* BIT28 */
#define GPIO_PAD_HYST_EN1_P61    _GPIO_PAD_HYST_EN1.Bits.GPIO_61                /* BIT29 */
#define GPIO_PAD_HYST_EN1_P62    _GPIO_PAD_HYST_EN1.Bits.GPIO_62                /* BIT30 */
#define GPIO_PAD_HYST_EN1_P63    _GPIO_PAD_HYST_EN1.Bits.GPIO_63                /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_PAD_KEEP0 @(MBAR_GPIO + 0x40);     /* GPIO Pad Keeper Enable for GPIO 00-31 R/W 32 */
#define GPIO_PAD_KEEP0        _GPIO_PAD_KEEP0.Reg
#define GPIO_PAD_KEEP0_P00    _GPIO_PAD_KEEP0.Bits.GPIO_00                      /* BIT0 */
#define GPIO_PAD_KEEP0_P01    _GPIO_PAD_KEEP0.Bits.GPIO_01                      /* BIT1 */
#define GPIO_PAD_KEEP0_P02    _GPIO_PAD_KEEP0.Bits.GPIO_02                      /* BIT2 */
#define GPIO_PAD_KEEP0_P03    _GPIO_PAD_KEEP0.Bits.GPIO_03                      /* BIT3 */
#define GPIO_PAD_KEEP0_P04    _GPIO_PAD_KEEP0.Bits.GPIO_04                      /* BIT4 */
#define GPIO_PAD_KEEP0_P05    _GPIO_PAD_KEEP0.Bits.GPIO_05                      /* BIT5 */
#define GPIO_PAD_KEEP0_P06    _GPIO_PAD_KEEP0.Bits.GPIO_06                      /* BIT6 */
#define GPIO_PAD_KEEP0_P07    _GPIO_PAD_KEEP0.Bits.GPIO_07                      /* BIT7 */
#define GPIO_PAD_KEEP0_P08    _GPIO_PAD_KEEP0.Bits.GPIO_08                      /* BIT8 */
#define GPIO_PAD_KEEP0_P09    _GPIO_PAD_KEEP0.Bits.GPIO_09                      /* BIT9 */
#define GPIO_PAD_KEEP0_P10    _GPIO_PAD_KEEP0.Bits.GPIO_10                      /* BIT10 */
#define GPIO_PAD_KEEP0_P11    _GPIO_PAD_KEEP0.Bits.GPIO_11                      /* BIT11 */
#define GPIO_PAD_KEEP0_P12    _GPIO_PAD_KEEP0.Bits.GPIO_12                      /* BIT12 */
#define GPIO_PAD_KEEP0_P13    _GPIO_PAD_KEEP0.Bits.GPIO_13                      /* BIT13 */
#define GPIO_PAD_KEEP0_P14    _GPIO_PAD_KEEP0.Bits.GPIO_14                      /* BIT14 */
#define GPIO_PAD_KEEP0_P15    _GPIO_PAD_KEEP0.Bits.GPIO_15                      /* BIT15 */
#define GPIO_PAD_KEEP0_P16    _GPIO_PAD_KEEP0.Bits.GPIO_16                      /* BIT16 */
#define GPIO_PAD_KEEP0_P17    _GPIO_PAD_KEEP0.Bits.GPIO_17                      /* BIT17 */
#define GPIO_PAD_KEEP0_P18    _GPIO_PAD_KEEP0.Bits.GPIO_18                      /* BIT18 */
#define GPIO_PAD_KEEP0_P19    _GPIO_PAD_KEEP0.Bits.GPIO_19                      /* BIT19 */
#define GPIO_PAD_KEEP0_P20    _GPIO_PAD_KEEP0.Bits.GPIO_20                      /* BIT20 */
#define GPIO_PAD_KEEP0_P21    _GPIO_PAD_KEEP0.Bits.GPIO_21                      /* BIT21 */
#define GPIO_PAD_KEEP0_P22    _GPIO_PAD_KEEP0.Bits.GPIO_22                      /* BIT22 */
#define GPIO_PAD_KEEP0_P23    _GPIO_PAD_KEEP0.Bits.GPIO_23                      /* BIT23 */
#define GPIO_PAD_KEEP0_P24    _GPIO_PAD_KEEP0.Bits.GPIO_24                      /* BIT24 */
#define GPIO_PAD_KEEP0_P25    _GPIO_PAD_KEEP0.Bits.GPIO_25                      /* BIT25 */
#define GPIO_PAD_KEEP0_P26    _GPIO_PAD_KEEP0.Bits.GPIO_26                      /* BIT26 */
#define GPIO_PAD_KEEP0_P27    _GPIO_PAD_KEEP0.Bits.GPIO_27                      /* BIT27 */
#define GPIO_PAD_KEEP0_P28    _GPIO_PAD_KEEP0.Bits.GPIO_28                      /* BIT28 */
#define GPIO_PAD_KEEP0_P29    _GPIO_PAD_KEEP0.Bits.GPIO_29                      /* BIT29 */
#define GPIO_PAD_KEEP0_P30    _GPIO_PAD_KEEP0.Bits.GPIO_30                      /* BIT30 */
#define GPIO_PAD_KEEP0_P31    _GPIO_PAD_KEEP0.Bits.GPIO_31                      /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_PAD_KEEP1 @(MBAR_GPIO + 0x44);     /* GPIO Pad Keeper Enable for GPIO 32-63 R/W 32 */
#define GPIO_PAD_KEEP1        _GPIO_PAD_KEEP1.Reg
#define GPIO_PAD_KEEP1_P32    _GPIO_PAD_KEEP1.Bits.GPIO_32                      /* BIT0 */
#define GPIO_PAD_KEEP1_P33    _GPIO_PAD_KEEP1.Bits.GPIO_33                      /* BIT1 */
#define GPIO_PAD_KEEP1_P34    _GPIO_PAD_KEEP1.Bits.GPIO_34                      /* BIT2 */
#define GPIO_PAD_KEEP1_P35    _GPIO_PAD_KEEP1.Bits.GPIO_35                      /* BIT3 */
#define GPIO_PAD_KEEP1_P36    _GPIO_PAD_KEEP1.Bits.GPIO_36                      /* BIT4 */
#define GPIO_PAD_KEEP1_P37    _GPIO_PAD_KEEP1.Bits.GPIO_37                      /* BIT5 */
#define GPIO_PAD_KEEP1_P38    _GPIO_PAD_KEEP1.Bits.GPIO_38                      /* BIT6 */
#define GPIO_PAD_KEEP1_P39    _GPIO_PAD_KEEP1.Bits.GPIO_39                      /* BIT7 */
#define GPIO_PAD_KEEP1_P40    _GPIO_PAD_KEEP1.Bits.GPIO_40                      /* BIT8 */
#define GPIO_PAD_KEEP1_P41    _GPIO_PAD_KEEP1.Bits.GPIO_41                      /* BIT9 */
#define GPIO_PAD_KEEP1_P42    _GPIO_PAD_KEEP1.Bits.GPIO_42                      /* BIT10 */
#define GPIO_PAD_KEEP1_P43    _GPIO_PAD_KEEP1.Bits.GPIO_43                      /* BIT11 */
#define GPIO_PAD_KEEP1_P44    _GPIO_PAD_KEEP1.Bits.GPIO_44                      /* BIT12 */
#define GPIO_PAD_KEEP1_P45    _GPIO_PAD_KEEP1.Bits.GPIO_45                      /* BIT13 */
#define GPIO_PAD_KEEP1_P46    _GPIO_PAD_KEEP1.Bits.GPIO_46                      /* BIT14 */
#define GPIO_PAD_KEEP1_P47    _GPIO_PAD_KEEP1.Bits.GPIO_47                      /* BIT15 */
#define GPIO_PAD_KEEP1_P48    _GPIO_PAD_KEEP1.Bits.GPIO_48                      /* BIT16 */
#define GPIO_PAD_KEEP1_P49    _GPIO_PAD_KEEP1.Bits.GPIO_49                      /* BIT17 */
#define GPIO_PAD_KEEP1_P50    _GPIO_PAD_KEEP1.Bits.GPIO_50                      /* BIT18 */
#define GPIO_PAD_KEEP1_P51    _GPIO_PAD_KEEP1.Bits.GPIO_51                      /* BIT19 */
#define GPIO_PAD_KEEP1_P52    _GPIO_PAD_KEEP1.Bits.GPIO_52                      /* BIT20 */
#define GPIO_PAD_KEEP1_P53    _GPIO_PAD_KEEP1.Bits.GPIO_53                      /* BIT21 */
#define GPIO_PAD_KEEP1_P54    _GPIO_PAD_KEEP1.Bits.GPIO_54                      /* BIT22 */
#define GPIO_PAD_KEEP1_P55    _GPIO_PAD_KEEP1.Bits.GPIO_55                      /* BIT23 */
#define GPIO_PAD_KEEP1_P56    _GPIO_PAD_KEEP1.Bits.GPIO_56                      /* BIT24 */
#define GPIO_PAD_KEEP1_P57    _GPIO_PAD_KEEP1.Bits.GPIO_57                      /* BIT25 */
#define GPIO_PAD_KEEP1_P58    _GPIO_PAD_KEEP1.Bits.GPIO_58                      /* BIT26 */
#define GPIO_PAD_KEEP1_P59    _GPIO_PAD_KEEP1.Bits.GPIO_59                      /* BIT27 */
#define GPIO_PAD_KEEP1_P60    _GPIO_PAD_KEEP1.Bits.GPIO_60                      /* BIT28 */
#define GPIO_PAD_KEEP1_P61    _GPIO_PAD_KEEP1.Bits.GPIO_61                      /* BIT29 */
#define GPIO_PAD_KEEP1_P62    _GPIO_PAD_KEEP1.Bits.GPIO_62                      /* BIT30 */
#define GPIO_PAD_KEEP1_P63    _GPIO_PAD_KEEP1.Bits.GPIO_63                      /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_DATA_SET0 @(MBAR_GPIO + 0x48);     /* GPIO Data Set for GPIO 00-31 W 32 */
#define GPIO_DATA_SET0        _GPIO_DATA_SET0.Reg
#define GPIO_DATA_SET0_P00    _GPIO_DATA_SET0.Bits.GPIO_00                      /* BIT0 */
#define GPIO_DATA_SET0_P01    _GPIO_DATA_SET0.Bits.GPIO_01                      /* BIT1 */
#define GPIO_DATA_SET0_P02    _GPIO_DATA_SET0.Bits.GPIO_02                      /* BIT2 */
#define GPIO_DATA_SET0_P03    _GPIO_DATA_SET0.Bits.GPIO_03                      /* BIT3 */
#define GPIO_DATA_SET0_P04    _GPIO_DATA_SET0.Bits.GPIO_04                      /* BIT4 */
#define GPIO_DATA_SET0_P05    _GPIO_DATA_SET0.Bits.GPIO_05                      /* BIT5 */
#define GPIO_DATA_SET0_P06    _GPIO_DATA_SET0.Bits.GPIO_06                      /* BIT6 */
#define GPIO_DATA_SET0_P07    _GPIO_DATA_SET0.Bits.GPIO_07                      /* BIT7 */
#define GPIO_DATA_SET0_P08    _GPIO_DATA_SET0.Bits.GPIO_08                      /* BIT8 */
#define GPIO_DATA_SET0_P09    _GPIO_DATA_SET0.Bits.GPIO_09                      /* BIT9 */
#define GPIO_DATA_SET0_P10    _GPIO_DATA_SET0.Bits.GPIO_10                      /* BIT10 */
#define GPIO_DATA_SET0_P11    _GPIO_DATA_SET0.Bits.GPIO_11                      /* BIT11 */
#define GPIO_DATA_SET0_P12    _GPIO_DATA_SET0.Bits.GPIO_12                      /* BIT12 */
#define GPIO_DATA_SET0_P13    _GPIO_DATA_SET0.Bits.GPIO_13                      /* BIT13 */
#define GPIO_DATA_SET0_P14    _GPIO_DATA_SET0.Bits.GPIO_14                      /* BIT14 */
#define GPIO_DATA_SET0_P15    _GPIO_DATA_SET0.Bits.GPIO_15                      /* BIT15 */
#define GPIO_DATA_SET0_P16    _GPIO_DATA_SET0.Bits.GPIO_16                      /* BIT16 */
#define GPIO_DATA_SET0_P17    _GPIO_DATA_SET0.Bits.GPIO_17                      /* BIT17 */
#define GPIO_DATA_SET0_P18    _GPIO_DATA_SET0.Bits.GPIO_18                      /* BIT18 */
#define GPIO_DATA_SET0_P19    _GPIO_DATA_SET0.Bits.GPIO_19                      /* BIT19 */
#define GPIO_DATA_SET0_P20    _GPIO_DATA_SET0.Bits.GPIO_20                      /* BIT20 */
#define GPIO_DATA_SET0_P21    _GPIO_DATA_SET0.Bits.GPIO_21                      /* BIT21 */
#define GPIO_DATA_SET0_P22    _GPIO_DATA_SET0.Bits.GPIO_22                      /* BIT22 */
#define GPIO_DATA_SET0_P23    _GPIO_DATA_SET0.Bits.GPIO_23                      /* BIT23 */
#define GPIO_DATA_SET0_P24    _GPIO_DATA_SET0.Bits.GPIO_24                      /* BIT24 */
#define GPIO_DATA_SET0_P25    _GPIO_DATA_SET0.Bits.GPIO_25                      /* BIT25 */
#define GPIO_DATA_SET0_P26    _GPIO_DATA_SET0.Bits.GPIO_26                      /* BIT26 */
#define GPIO_DATA_SET0_P27    _GPIO_DATA_SET0.Bits.GPIO_27                      /* BIT27 */
#define GPIO_DATA_SET0_P28    _GPIO_DATA_SET0.Bits.GPIO_28                      /* BIT28 */
#define GPIO_DATA_SET0_P29    _GPIO_DATA_SET0.Bits.GPIO_29                      /* BIT29 */
#define GPIO_DATA_SET0_P30    _GPIO_DATA_SET0.Bits.GPIO_30                      /* BIT30 */
#define GPIO_DATA_SET0_P31    _GPIO_DATA_SET0.Bits.GPIO_31                      /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_DATA_SET1 @(MBAR_GPIO + 0x4C);     /* GPIO Data Set for GPIO 32-63 W 32 */
#define GPIO_DATA_SET1        _GPIO_DATA_SET1.Reg
#define GPIO_DATA_SET1_P32    _GPIO_DATA_SET1.Bits.GPIO_32                      /* BIT0 */
#define GPIO_DATA_SET1_P33    _GPIO_DATA_SET1.Bits.GPIO_33                      /* BIT1 */
#define GPIO_DATA_SET1_P34    _GPIO_DATA_SET1.Bits.GPIO_34                      /* BIT2 */
#define GPIO_DATA_SET1_P35    _GPIO_DATA_SET1.Bits.GPIO_35                      /* BIT3 */
#define GPIO_DATA_SET1_P36    _GPIO_DATA_SET1.Bits.GPIO_36                      /* BIT4 */
#define GPIO_DATA_SET1_P37    _GPIO_DATA_SET1.Bits.GPIO_37                      /* BIT5 */
#define GPIO_DATA_SET1_P38    _GPIO_DATA_SET1.Bits.GPIO_38                      /* BIT6 */
#define GPIO_DATA_SET1_P39    _GPIO_DATA_SET1.Bits.GPIO_39                      /* BIT7 */
#define GPIO_DATA_SET1_P40    _GPIO_DATA_SET1.Bits.GPIO_40                      /* BIT8 */
#define GPIO_DATA_SET1_P41    _GPIO_DATA_SET1.Bits.GPIO_41                      /* BIT9 */
#define GPIO_DATA_SET1_P42    _GPIO_DATA_SET1.Bits.GPIO_42                      /* BIT10 */
#define GPIO_DATA_SET1_P43    _GPIO_DATA_SET1.Bits.GPIO_43                      /* BIT11 */
#define GPIO_DATA_SET1_P44    _GPIO_DATA_SET1.Bits.GPIO_44                      /* BIT12 */
#define GPIO_DATA_SET1_P45    _GPIO_DATA_SET1.Bits.GPIO_45                      /* BIT13 */
#define GPIO_DATA_SET1_P46    _GPIO_DATA_SET1.Bits.GPIO_46                      /* BIT14 */
#define GPIO_DATA_SET1_P47    _GPIO_DATA_SET1.Bits.GPIO_47                      /* BIT15 */
#define GPIO_DATA_SET1_P48    _GPIO_DATA_SET1.Bits.GPIO_48                      /* BIT16 */
#define GPIO_DATA_SET1_P49    _GPIO_DATA_SET1.Bits.GPIO_49                      /* BIT17 */
#define GPIO_DATA_SET1_P50    _GPIO_DATA_SET1.Bits.GPIO_50                      /* BIT18 */
#define GPIO_DATA_SET1_P51    _GPIO_DATA_SET1.Bits.GPIO_51                      /* BIT19 */
#define GPIO_DATA_SET1_P52    _GPIO_DATA_SET1.Bits.GPIO_52                      /* BIT20 */
#define GPIO_DATA_SET1_P53    _GPIO_DATA_SET1.Bits.GPIO_53                      /* BIT21 */
#define GPIO_DATA_SET1_P54    _GPIO_DATA_SET1.Bits.GPIO_54                      /* BIT22 */
#define GPIO_DATA_SET1_P55    _GPIO_DATA_SET1.Bits.GPIO_55                      /* BIT23 */
#define GPIO_DATA_SET1_P56    _GPIO_DATA_SET1.Bits.GPIO_56                      /* BIT24 */
#define GPIO_DATA_SET1_P57    _GPIO_DATA_SET1.Bits.GPIO_57                      /* BIT25 */
#define GPIO_DATA_SET1_P58    _GPIO_DATA_SET1.Bits.GPIO_58                      /* BIT26 */
#define GPIO_DATA_SET1_P59    _GPIO_DATA_SET1.Bits.GPIO_59                      /* BIT27 */
#define GPIO_DATA_SET1_P60    _GPIO_DATA_SET1.Bits.GPIO_60                      /* BIT28 */
#define GPIO_DATA_SET1_P61    _GPIO_DATA_SET1.Bits.GPIO_61                      /* BIT29 */
#define GPIO_DATA_SET1_P62    _GPIO_DATA_SET1.Bits.GPIO_62                      /* BIT30 */
#define GPIO_DATA_SET1_P63    _GPIO_DATA_SET1.Bits.GPIO_63                      /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_DATA_RESET0 @(MBAR_GPIO + 0x50);   /* GPIO Data Reset for GPIO 00-31 W 32 */
#define GPIO_DATA_RESET0        _GPIO_DATA_RESET0.Reg
#define GPIO_DATA_RESET0_P00    _GPIO_DATA_RESET0.Bits.GPIO_00                  /* BIT0 */
#define GPIO_DATA_RESET0_P01    _GPIO_DATA_RESET0.Bits.GPIO_01                  /* BIT1 */
#define GPIO_DATA_RESET0_P02    _GPIO_DATA_RESET0.Bits.GPIO_02                  /* BIT2 */
#define GPIO_DATA_RESET0_P03    _GPIO_DATA_RESET0.Bits.GPIO_03                  /* BIT3 */
#define GPIO_DATA_RESET0_P04    _GPIO_DATA_RESET0.Bits.GPIO_04                  /* BIT4 */
#define GPIO_DATA_RESET0_P05    _GPIO_DATA_RESET0.Bits.GPIO_05                  /* BIT5 */
#define GPIO_DATA_RESET0_P06    _GPIO_DATA_RESET0.Bits.GPIO_06                  /* BIT6 */
#define GPIO_DATA_RESET0_P07    _GPIO_DATA_RESET0.Bits.GPIO_07                  /* BIT7 */
#define GPIO_DATA_RESET0_P08    _GPIO_DATA_RESET0.Bits.GPIO_08                  /* BIT8 */
#define GPIO_DATA_RESET0_P09    _GPIO_DATA_RESET0.Bits.GPIO_09                  /* BIT9 */
#define GPIO_DATA_RESET0_P10    _GPIO_DATA_RESET0.Bits.GPIO_10                  /* BIT10 */
#define GPIO_DATA_RESET0_P11    _GPIO_DATA_RESET0.Bits.GPIO_11                  /* BIT11 */
#define GPIO_DATA_RESET0_P12    _GPIO_DATA_RESET0.Bits.GPIO_12                  /* BIT12 */
#define GPIO_DATA_RESET0_P13    _GPIO_DATA_RESET0.Bits.GPIO_13                  /* BIT13 */
#define GPIO_DATA_RESET0_P14    _GPIO_DATA_RESET0.Bits.GPIO_14                  /* BIT14 */
#define GPIO_DATA_RESET0_P15    _GPIO_DATA_RESET0.Bits.GPIO_15                  /* BIT15 */
#define GPIO_DATA_RESET0_P16    _GPIO_DATA_RESET0.Bits.GPIO_16                  /* BIT16 */
#define GPIO_DATA_RESET0_P17    _GPIO_DATA_RESET0.Bits.GPIO_17                  /* BIT17 */
#define GPIO_DATA_RESET0_P18    _GPIO_DATA_RESET0.Bits.GPIO_18                  /* BIT18 */
#define GPIO_DATA_RESET0_P19    _GPIO_DATA_RESET0.Bits.GPIO_19                  /* BIT19 */
#define GPIO_DATA_RESET0_P20    _GPIO_DATA_RESET0.Bits.GPIO_20                  /* BIT20 */
#define GPIO_DATA_RESET0_P21    _GPIO_DATA_RESET0.Bits.GPIO_21                  /* BIT21 */
#define GPIO_DATA_RESET0_P22    _GPIO_DATA_RESET0.Bits.GPIO_22                  /* BIT22 */
#define GPIO_DATA_RESET0_P23    _GPIO_DATA_RESET0.Bits.GPIO_23                  /* BIT23 */
#define GPIO_DATA_RESET0_P24    _GPIO_DATA_RESET0.Bits.GPIO_24                  /* BIT24 */
#define GPIO_DATA_RESET0_P25    _GPIO_DATA_RESET0.Bits.GPIO_25                  /* BIT25 */
#define GPIO_DATA_RESET0_P26    _GPIO_DATA_RESET0.Bits.GPIO_26                  /* BIT26 */
#define GPIO_DATA_RESET0_P27    _GPIO_DATA_RESET0.Bits.GPIO_27                  /* BIT27 */
#define GPIO_DATA_RESET0_P28    _GPIO_DATA_RESET0.Bits.GPIO_28                  /* BIT28 */
#define GPIO_DATA_RESET0_P29    _GPIO_DATA_RESET0.Bits.GPIO_29                  /* BIT29 */
#define GPIO_DATA_RESET0_P30    _GPIO_DATA_RESET0.Bits.GPIO_30                  /* BIT30 */
#define GPIO_DATA_RESET0_P31    _GPIO_DATA_RESET0.Bits.GPIO_31                  /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_DATA_RESET1 @(MBAR_GPIO + 0x54);   /* GPIO Data Reset for GPIO 32-63 W 32 */
#define GPIO_DATA_RESET1        _GPIO_DATA_RESET1.Reg
#define GPIO_DATA_RESET1_P32    _GPIO_DATA_RESET1.Bits.GPIO_32                  /* BIT0 */
#define GPIO_DATA_RESET1_P33    _GPIO_DATA_RESET1.Bits.GPIO_33                  /* BIT1 */
#define GPIO_DATA_RESET1_P34    _GPIO_DATA_RESET1.Bits.GPIO_34                  /* BIT2 */
#define GPIO_DATA_RESET1_P35    _GPIO_DATA_RESET1.Bits.GPIO_35                  /* BIT3 */
#define GPIO_DATA_RESET1_P36    _GPIO_DATA_RESET1.Bits.GPIO_36                  /* BIT4 */
#define GPIO_DATA_RESET1_P37    _GPIO_DATA_RESET1.Bits.GPIO_37                  /* BIT5 */
#define GPIO_DATA_RESET1_P38    _GPIO_DATA_RESET1.Bits.GPIO_38                  /* BIT6 */
#define GPIO_DATA_RESET1_P39    _GPIO_DATA_RESET1.Bits.GPIO_39                  /* BIT7 */
#define GPIO_DATA_RESET1_P40    _GPIO_DATA_RESET1.Bits.GPIO_40                  /* BIT8 */
#define GPIO_DATA_RESET1_P41    _GPIO_DATA_RESET1.Bits.GPIO_41                  /* BIT9 */
#define GPIO_DATA_RESET1_P42    _GPIO_DATA_RESET1.Bits.GPIO_42                  /* BIT10 */
#define GPIO_DATA_RESET1_P43    _GPIO_DATA_RESET1.Bits.GPIO_43                  /* BIT11 */
#define GPIO_DATA_RESET1_P44    _GPIO_DATA_RESET1.Bits.GPIO_44                  /* BIT12 */
#define GPIO_DATA_RESET1_P45    _GPIO_DATA_RESET1.Bits.GPIO_45                  /* BIT13 */
#define GPIO_DATA_RESET1_P46    _GPIO_DATA_RESET1.Bits.GPIO_46                  /* BIT14 */
#define GPIO_DATA_RESET1_P47    _GPIO_DATA_RESET1.Bits.GPIO_47                  /* BIT15 */
#define GPIO_DATA_RESET1_P48    _GPIO_DATA_RESET1.Bits.GPIO_48                  /* BIT16 */
#define GPIO_DATA_RESET1_P49    _GPIO_DATA_RESET1.Bits.GPIO_49                  /* BIT17 */
#define GPIO_DATA_RESET1_P50    _GPIO_DATA_RESET1.Bits.GPIO_50                  /* BIT18 */
#define GPIO_DATA_RESET1_P51    _GPIO_DATA_RESET1.Bits.GPIO_51                  /* BIT19 */
#define GPIO_DATA_RESET1_P52    _GPIO_DATA_RESET1.Bits.GPIO_52                  /* BIT20 */
#define GPIO_DATA_RESET1_P53    _GPIO_DATA_RESET1.Bits.GPIO_53                  /* BIT21 */
#define GPIO_DATA_RESET1_P54    _GPIO_DATA_RESET1.Bits.GPIO_54                  /* BIT22 */
#define GPIO_DATA_RESET1_P55    _GPIO_DATA_RESET1.Bits.GPIO_55                  /* BIT23 */
#define GPIO_DATA_RESET1_P56    _GPIO_DATA_RESET1.Bits.GPIO_56                  /* BIT24 */
#define GPIO_DATA_RESET1_P57    _GPIO_DATA_RESET1.Bits.GPIO_57                  /* BIT25 */
#define GPIO_DATA_RESET1_P58    _GPIO_DATA_RESET1.Bits.GPIO_58                  /* BIT26 */
#define GPIO_DATA_RESET1_P59    _GPIO_DATA_RESET1.Bits.GPIO_59                  /* BIT27 */
#define GPIO_DATA_RESET1_P60    _GPIO_DATA_RESET1.Bits.GPIO_60                  /* BIT28 */
#define GPIO_DATA_RESET1_P61    _GPIO_DATA_RESET1.Bits.GPIO_61                  /* BIT29 */
#define GPIO_DATA_RESET1_P62    _GPIO_DATA_RESET1.Bits.GPIO_62                  /* BIT30 */
#define GPIO_DATA_RESET1_P63    _GPIO_DATA_RESET1.Bits.GPIO_63                  /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_PAD_DIR_SET0 @(MBAR_GPIO + 0x58);  /* GPIO Pad Direction Set for GPIO 00-31 W 32 */
#define GPIO_PAD_DIR_SET0        _GPIO_PAD_DIR_SET0.Reg
#define GPIO_PAD_DIR_SET0_P00    _GPIO_PAD_DIR_SET0.Bits.GPIO_00                /* BIT0 */
#define GPIO_PAD_DIR_SET0_P01    _GPIO_PAD_DIR_SET0.Bits.GPIO_01                /* BIT1 */
#define GPIO_PAD_DIR_SET0_P02    _GPIO_PAD_DIR_SET0.Bits.GPIO_02                /* BIT2 */
#define GPIO_PAD_DIR_SET0_P03    _GPIO_PAD_DIR_SET0.Bits.GPIO_03                /* BIT3 */
#define GPIO_PAD_DIR_SET0_P04    _GPIO_PAD_DIR_SET0.Bits.GPIO_04                /* BIT4 */
#define GPIO_PAD_DIR_SET0_P05    _GPIO_PAD_DIR_SET0.Bits.GPIO_05                /* BIT5 */
#define GPIO_PAD_DIR_SET0_P06    _GPIO_PAD_DIR_SET0.Bits.GPIO_06                /* BIT6 */
#define GPIO_PAD_DIR_SET0_P07    _GPIO_PAD_DIR_SET0.Bits.GPIO_07                /* BIT7 */
#define GPIO_PAD_DIR_SET0_P08    _GPIO_PAD_DIR_SET0.Bits.GPIO_08                /* BIT8 */
#define GPIO_PAD_DIR_SET0_P09    _GPIO_PAD_DIR_SET0.Bits.GPIO_09                /* BIT9 */
#define GPIO_PAD_DIR_SET0_P10    _GPIO_PAD_DIR_SET0.Bits.GPIO_10                /* BIT10 */
#define GPIO_PAD_DIR_SET0_P11    _GPIO_PAD_DIR_SET0.Bits.GPIO_11                /* BIT11 */
#define GPIO_PAD_DIR_SET0_P12    _GPIO_PAD_DIR_SET0.Bits.GPIO_12                /* BIT12 */
#define GPIO_PAD_DIR_SET0_P13    _GPIO_PAD_DIR_SET0.Bits.GPIO_13                /* BIT13 */
#define GPIO_PAD_DIR_SET0_P14    _GPIO_PAD_DIR_SET0.Bits.GPIO_14                /* BIT14 */
#define GPIO_PAD_DIR_SET0_P15    _GPIO_PAD_DIR_SET0.Bits.GPIO_15                /* BIT15 */
#define GPIO_PAD_DIR_SET0_P16    _GPIO_PAD_DIR_SET0.Bits.GPIO_16                /* BIT16 */
#define GPIO_PAD_DIR_SET0_P17    _GPIO_PAD_DIR_SET0.Bits.GPIO_17                /* BIT17 */
#define GPIO_PAD_DIR_SET0_P18    _GPIO_PAD_DIR_SET0.Bits.GPIO_18                /* BIT18 */
#define GPIO_PAD_DIR_SET0_P19    _GPIO_PAD_DIR_SET0.Bits.GPIO_19                /* BIT19 */
#define GPIO_PAD_DIR_SET0_P20    _GPIO_PAD_DIR_SET0.Bits.GPIO_20                /* BIT20 */
#define GPIO_PAD_DIR_SET0_P21    _GPIO_PAD_DIR_SET0.Bits.GPIO_21                /* BIT21 */
#define GPIO_PAD_DIR_SET0_P22    _GPIO_PAD_DIR_SET0.Bits.GPIO_22                /* BIT22 */
#define GPIO_PAD_DIR_SET0_P23    _GPIO_PAD_DIR_SET0.Bits.GPIO_23                /* BIT23 */
#define GPIO_PAD_DIR_SET0_P24    _GPIO_PAD_DIR_SET0.Bits.GPIO_24                /* BIT24 */
#define GPIO_PAD_DIR_SET0_P25    _GPIO_PAD_DIR_SET0.Bits.GPIO_25                /* BIT25 */
#define GPIO_PAD_DIR_SET0_P26    _GPIO_PAD_DIR_SET0.Bits.GPIO_26                /* BIT26 */
#define GPIO_PAD_DIR_SET0_P27    _GPIO_PAD_DIR_SET0.Bits.GPIO_27                /* BIT27 */
#define GPIO_PAD_DIR_SET0_P28    _GPIO_PAD_DIR_SET0.Bits.GPIO_28                /* BIT28 */
#define GPIO_PAD_DIR_SET0_P29    _GPIO_PAD_DIR_SET0.Bits.GPIO_29                /* BIT29 */
#define GPIO_PAD_DIR_SET0_P30    _GPIO_PAD_DIR_SET0.Bits.GPIO_30                /* BIT30 */
#define GPIO_PAD_DIR_SET0_P31    _GPIO_PAD_DIR_SET0.Bits.GPIO_31                /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_PAD_DIR_SET1 @(MBAR_GPIO + 0x5C);  /* GPIO Pad Direction Set for GPIO 32-63 W 32 */
#define GPIO_PAD_DIR_SET1        _GPIO_PAD_DIR_SET1.Reg
#define GPIO_PAD_DIR_SET1_P32    _GPIO_PAD_DIR_SET1.Bits.GPIO_32                /* BIT0 */
#define GPIO_PAD_DIR_SET1_P33    _GPIO_PAD_DIR_SET1.Bits.GPIO_33                /* BIT1 */
#define GPIO_PAD_DIR_SET1_P34    _GPIO_PAD_DIR_SET1.Bits.GPIO_34                /* BIT2 */
#define GPIO_PAD_DIR_SET1_P35    _GPIO_PAD_DIR_SET1.Bits.GPIO_35                /* BIT3 */
#define GPIO_PAD_DIR_SET1_P36    _GPIO_PAD_DIR_SET1.Bits.GPIO_36                /* BIT4 */
#define GPIO_PAD_DIR_SET1_P37    _GPIO_PAD_DIR_SET1.Bits.GPIO_37                /* BIT5 */
#define GPIO_PAD_DIR_SET1_P38    _GPIO_PAD_DIR_SET1.Bits.GPIO_38                /* BIT6 */
#define GPIO_PAD_DIR_SET1_P39    _GPIO_PAD_DIR_SET1.Bits.GPIO_39                /* BIT7 */
#define GPIO_PAD_DIR_SET1_P40    _GPIO_PAD_DIR_SET1.Bits.GPIO_40                /* BIT8 */
#define GPIO_PAD_DIR_SET1_P41    _GPIO_PAD_DIR_SET1.Bits.GPIO_41                /* BIT9 */
#define GPIO_PAD_DIR_SET1_P42    _GPIO_PAD_DIR_SET1.Bits.GPIO_42                /* BIT10 */
#define GPIO_PAD_DIR_SET1_P43    _GPIO_PAD_DIR_SET1.Bits.GPIO_43                /* BIT11 */
#define GPIO_PAD_DIR_SET1_P44    _GPIO_PAD_DIR_SET1.Bits.GPIO_44                /* BIT12 */
#define GPIO_PAD_DIR_SET1_P45    _GPIO_PAD_DIR_SET1.Bits.GPIO_45                /* BIT13 */
#define GPIO_PAD_DIR_SET1_P46    _GPIO_PAD_DIR_SET1.Bits.GPIO_46                /* BIT14 */
#define GPIO_PAD_DIR_SET1_P47    _GPIO_PAD_DIR_SET1.Bits.GPIO_47                /* BIT15 */
#define GPIO_PAD_DIR_SET1_P48    _GPIO_PAD_DIR_SET1.Bits.GPIO_48                /* BIT16 */
#define GPIO_PAD_DIR_SET1_P49    _GPIO_PAD_DIR_SET1.Bits.GPIO_49                /* BIT17 */
#define GPIO_PAD_DIR_SET1_P50    _GPIO_PAD_DIR_SET1.Bits.GPIO_50                /* BIT18 */
#define GPIO_PAD_DIR_SET1_P51    _GPIO_PAD_DIR_SET1.Bits.GPIO_51                /* BIT19 */
#define GPIO_PAD_DIR_SET1_P52    _GPIO_PAD_DIR_SET1.Bits.GPIO_52                /* BIT20 */
#define GPIO_PAD_DIR_SET1_P53    _GPIO_PAD_DIR_SET1.Bits.GPIO_53                /* BIT21 */
#define GPIO_PAD_DIR_SET1_P54    _GPIO_PAD_DIR_SET1.Bits.GPIO_54                /* BIT22 */
#define GPIO_PAD_DIR_SET1_P55    _GPIO_PAD_DIR_SET1.Bits.GPIO_55                /* BIT23 */
#define GPIO_PAD_DIR_SET1_P56    _GPIO_PAD_DIR_SET1.Bits.GPIO_56                /* BIT24 */
#define GPIO_PAD_DIR_SET1_P57    _GPIO_PAD_DIR_SET1.Bits.GPIO_57                /* BIT25 */
#define GPIO_PAD_DIR_SET1_P58    _GPIO_PAD_DIR_SET1.Bits.GPIO_58                /* BIT26 */
#define GPIO_PAD_DIR_SET1_P59    _GPIO_PAD_DIR_SET1.Bits.GPIO_59                /* BIT27 */
#define GPIO_PAD_DIR_SET1_P60    _GPIO_PAD_DIR_SET1.Bits.GPIO_60                /* BIT28 */
#define GPIO_PAD_DIR_SET1_P61    _GPIO_PAD_DIR_SET1.Bits.GPIO_61                /* BIT29 */
#define GPIO_PAD_DIR_SET1_P62    _GPIO_PAD_DIR_SET1.Bits.GPIO_62                /* BIT30 */
#define GPIO_PAD_DIR_SET1_P63    _GPIO_PAD_DIR_SET1.Bits.GPIO_63                /* BIT31 */


__no_init extern volatile GPIO_PORT0_T _GPIO_PAD_DIR_RESET0 @(MBAR_GPIO + 0x60);/* GPIO Pad Direction Reset for GPIO 00-31 W 32 */
#define GPIO_PAD_DIR_RESET0        _GPIO_PAD_DIR_RESET0.Reg
#define GPIO_PAD_DIR_RESET0_P00    _GPIO_PAD_DIR_RESET0.Bits.GPIO_00            /* BIT0 */
#define GPIO_PAD_DIR_RESET0_P01    _GPIO_PAD_DIR_RESET0.Bits.GPIO_01            /* BIT1 */
#define GPIO_PAD_DIR_RESET0_P02    _GPIO_PAD_DIR_RESET0.Bits.GPIO_02            /* BIT2 */
#define GPIO_PAD_DIR_RESET0_P03    _GPIO_PAD_DIR_RESET0.Bits.GPIO_03            /* BIT3 */
#define GPIO_PAD_DIR_RESET0_P04    _GPIO_PAD_DIR_RESET0.Bits.GPIO_04            /* BIT4 */
#define GPIO_PAD_DIR_RESET0_P05    _GPIO_PAD_DIR_RESET0.Bits.GPIO_05            /* BIT5 */
#define GPIO_PAD_DIR_RESET0_P06    _GPIO_PAD_DIR_RESET0.Bits.GPIO_06            /* BIT6 */
#define GPIO_PAD_DIR_RESET0_P07    _GPIO_PAD_DIR_RESET0.Bits.GPIO_07            /* BIT7 */
#define GPIO_PAD_DIR_RESET0_P08    _GPIO_PAD_DIR_RESET0.Bits.GPIO_08            /* BIT8 */
#define GPIO_PAD_DIR_RESET0_P09    _GPIO_PAD_DIR_RESET0.Bits.GPIO_09            /* BIT9 */
#define GPIO_PAD_DIR_RESET0_P10    _GPIO_PAD_DIR_RESET0.Bits.GPIO_10            /* BIT10 */
#define GPIO_PAD_DIR_RESET0_P11    _GPIO_PAD_DIR_RESET0.Bits.GPIO_11            /* BIT11 */
#define GPIO_PAD_DIR_RESET0_P12    _GPIO_PAD_DIR_RESET0.Bits.GPIO_12            /* BIT12 */
#define GPIO_PAD_DIR_RESET0_P13    _GPIO_PAD_DIR_RESET0.Bits.GPIO_13            /* BIT13 */
#define GPIO_PAD_DIR_RESET0_P14    _GPIO_PAD_DIR_RESET0.Bits.GPIO_14            /* BIT14 */
#define GPIO_PAD_DIR_RESET0_P15    _GPIO_PAD_DIR_RESET0.Bits.GPIO_15            /* BIT15 */
#define GPIO_PAD_DIR_RESET0_P16    _GPIO_PAD_DIR_RESET0.Bits.GPIO_16            /* BIT16 */
#define GPIO_PAD_DIR_RESET0_P17    _GPIO_PAD_DIR_RESET0.Bits.GPIO_17            /* BIT17 */
#define GPIO_PAD_DIR_RESET0_P18    _GPIO_PAD_DIR_RESET0.Bits.GPIO_18            /* BIT18 */
#define GPIO_PAD_DIR_RESET0_P19    _GPIO_PAD_DIR_RESET0.Bits.GPIO_19            /* BIT19 */
#define GPIO_PAD_DIR_RESET0_P20    _GPIO_PAD_DIR_RESET0.Bits.GPIO_20            /* BIT20 */
#define GPIO_PAD_DIR_RESET0_P21    _GPIO_PAD_DIR_RESET0.Bits.GPIO_21            /* BIT21 */
#define GPIO_PAD_DIR_RESET0_P22    _GPIO_PAD_DIR_RESET0.Bits.GPIO_22            /* BIT22 */
#define GPIO_PAD_DIR_RESET0_P23    _GPIO_PAD_DIR_RESET0.Bits.GPIO_23            /* BIT23 */
#define GPIO_PAD_DIR_RESET0_P24    _GPIO_PAD_DIR_RESET0.Bits.GPIO_24            /* BIT24 */
#define GPIO_PAD_DIR_RESET0_P25    _GPIO_PAD_DIR_RESET0.Bits.GPIO_25            /* BIT25 */
#define GPIO_PAD_DIR_RESET0_P26    _GPIO_PAD_DIR_RESET0.Bits.GPIO_26            /* BIT26 */
#define GPIO_PAD_DIR_RESET0_P27    _GPIO_PAD_DIR_RESET0.Bits.GPIO_27            /* BIT27 */
#define GPIO_PAD_DIR_RESET0_P28    _GPIO_PAD_DIR_RESET0.Bits.GPIO_28            /* BIT28 */
#define GPIO_PAD_DIR_RESET0_P29    _GPIO_PAD_DIR_RESET0.Bits.GPIO_29            /* BIT29 */
#define GPIO_PAD_DIR_RESET0_P30    _GPIO_PAD_DIR_RESET0.Bits.GPIO_30            /* BIT30 */
#define GPIO_PAD_DIR_RESET0_P31    _GPIO_PAD_DIR_RESET0.Bits.GPIO_31            /* BIT31 */


__no_init extern volatile GPIO_PORT1_T _GPIO_PAD_DIR_RESET1 @(MBAR_GPIO + 0x64);/* GPIO Pad Direction Reset for GPIO 32-63 W 32 */
#define GPIO_PAD_DIR_RESET1        _GPIO_PAD_DIR_RESET1.Reg
#define GPIO_PAD_DIR_RESET1_P32    _GPIO_PAD_DIR_RESET1.Bits.GPIO_32            /* BIT0 */
#define GPIO_PAD_DIR_RESET1_P33    _GPIO_PAD_DIR_RESET1.Bits.GPIO_33            /* BIT1 */
#define GPIO_PAD_DIR_RESET1_P34    _GPIO_PAD_DIR_RESET1.Bits.GPIO_34            /* BIT2 */
#define GPIO_PAD_DIR_RESET1_P35    _GPIO_PAD_DIR_RESET1.Bits.GPIO_35            /* BIT3 */
#define GPIO_PAD_DIR_RESET1_P36    _GPIO_PAD_DIR_RESET1.Bits.GPIO_36            /* BIT4 */
#define GPIO_PAD_DIR_RESET1_P37    _GPIO_PAD_DIR_RESET1.Bits.GPIO_37            /* BIT5 */
#define GPIO_PAD_DIR_RESET1_P38    _GPIO_PAD_DIR_RESET1.Bits.GPIO_38            /* BIT6 */
#define GPIO_PAD_DIR_RESET1_P39    _GPIO_PAD_DIR_RESET1.Bits.GPIO_39            /* BIT7 */
#define GPIO_PAD_DIR_RESET1_P40    _GPIO_PAD_DIR_RESET1.Bits.GPIO_40            /* BIT8 */
#define GPIO_PAD_DIR_RESET1_P41    _GPIO_PAD_DIR_RESET1.Bits.GPIO_41            /* BIT9 */
#define GPIO_PAD_DIR_RESET1_P42    _GPIO_PAD_DIR_RESET1.Bits.GPIO_42            /* BIT10 */
#define GPIO_PAD_DIR_RESET1_P43    _GPIO_PAD_DIR_RESET1.Bits.GPIO_43            /* BIT11 */
#define GPIO_PAD_DIR_RESET1_P44    _GPIO_PAD_DIR_RESET1.Bits.GPIO_44            /* BIT12 */
#define GPIO_PAD_DIR_RESET1_P45    _GPIO_PAD_DIR_RESET1.Bits.GPIO_45            /* BIT13 */
#define GPIO_PAD_DIR_RESET1_P46    _GPIO_PAD_DIR_RESET1.Bits.GPIO_46            /* BIT14 */
#define GPIO_PAD_DIR_RESET1_P47    _GPIO_PAD_DIR_RESET1.Bits.GPIO_47            /* BIT15 */
#define GPIO_PAD_DIR_RESET1_P48    _GPIO_PAD_DIR_RESET1.Bits.GPIO_48            /* BIT16 */
#define GPIO_PAD_DIR_RESET1_P49    _GPIO_PAD_DIR_RESET1.Bits.GPIO_49            /* BIT17 */
#define GPIO_PAD_DIR_RESET1_P50    _GPIO_PAD_DIR_RESET1.Bits.GPIO_50            /* BIT18 */
#define GPIO_PAD_DIR_RESET1_P51    _GPIO_PAD_DIR_RESET1.Bits.GPIO_51            /* BIT19 */
#define GPIO_PAD_DIR_RESET1_P52    _GPIO_PAD_DIR_RESET1.Bits.GPIO_52            /* BIT20 */
#define GPIO_PAD_DIR_RESET1_P53    _GPIO_PAD_DIR_RESET1.Bits.GPIO_53            /* BIT21 */
#define GPIO_PAD_DIR_RESET1_P54    _GPIO_PAD_DIR_RESET1.Bits.GPIO_54            /* BIT22 */
#define GPIO_PAD_DIR_RESET1_P55    _GPIO_PAD_DIR_RESET1.Bits.GPIO_55            /* BIT23 */
#define GPIO_PAD_DIR_RESET1_P56    _GPIO_PAD_DIR_RESET1.Bits.GPIO_56            /* BIT24 */
#define GPIO_PAD_DIR_RESET1_P57    _GPIO_PAD_DIR_RESET1.Bits.GPIO_57            /* BIT25 */
#define GPIO_PAD_DIR_RESET1_P58    _GPIO_PAD_DIR_RESET1.Bits.GPIO_58            /* BIT26 */
#define GPIO_PAD_DIR_RESET1_P59    _GPIO_PAD_DIR_RESET1.Bits.GPIO_59            /* BIT27 */
#define GPIO_PAD_DIR_RESET1_P60    _GPIO_PAD_DIR_RESET1.Bits.GPIO_60            /* BIT28 */
#define GPIO_PAD_DIR_RESET1_P61    _GPIO_PAD_DIR_RESET1.Bits.GPIO_61            /* BIT29 */
#define GPIO_PAD_DIR_RESET1_P62    _GPIO_PAD_DIR_RESET1.Bits.GPIO_62            /* BIT30 */
#define GPIO_PAD_DIR_RESET1_P63    _GPIO_PAD_DIR_RESET1.Bits.GPIO_63            /* BIT31 */


/*
0x8000_1000 SSI
+ 0x00 SSI_STX0             SSI Transmit Data register 0 R/W 32
+ 0x04 SSI_STX1             SSI Transmit Data register 1 R/W 32
+ 0x08 SSI_SRX0             SSI Receive Data register 0 R 32
+ 0x0C SSI_SRX1             SSI Receive Data register 1 R 32
+ 0x10 SSI_SCR              SSI Control register R/W 32
+ 0x14 SSI_SISR             SSI Interrupt Status register R 32
+ 0x18 SSI_SIER             SSI Interrupt Enable register R/W 32
+ 0x1C SSI_STCR             SSI Transmit Configuration register R/W 32
+ 0x20 SSI_SRCR             SSI Receive Configuration register R/W 32
+ 0x24 SSI_STCCR            SSI Transmit Clock Control register R/W 32
+ 0x28 SSI_SRCCR            SSI Receive Clock Control register R/W 32
+ 0x2C SSI_SFCSR            SSI FIFO Control/Status register R/W 32
+ 0x30 Reserved
+ 0x34 SSI_SOR              SSI Option register R/W 32
+ 0x38 SSI_SACNT            SSI AC97 Control register R/W 32
+ 0x3C SSI_SACADD           SSI AC97 Command Address register R/W 32
+ 0x40 SSI_SACDAT           SSI AC97 Command Data register R/W 32
+ 0x44 SSI_SATAG            SSI AC97 Tag register R/W 32
+ 0x48 SSI_STMSK            SSI Transmit Time Slot Mask register R/W 32
+ 0x4C SSI_SRMSK            SSI Receive Time Slot Mask register R/W 32
+ 0x50 SSI_SACCST           SSI AC97 Channel Status register R 32
+ 0x54 SSI_SACCEN           SSI AC97 Channel Enable register W 32
+ 0x58 SSI_SACCDIS          SSI AC97 Channel Disable register W 32
*/

/*
0x8000_2000 SPI
+ 0x00 SPI_TX_DATA          SPI Transmit Data register R/W 32
+ 0x04 SPI_RX_DATA          SPI Received Data register R/W 32
+ 0x08 SPI_CLK_CTRL         SPI Status and Clock Control register R/W 32
+ 0x0C SPI_SETUP            SPI Setup register R/W 32
+ 0x10 SPI_STATUS           SPI Status register R/W 32
*/


/* ---------------- Clock/Reset/Power Module (CRM) -------------------------- */
/*    Base Adrress = MBAR_CRM = 0x8000_3000                                   */
typedef union
{
  struct
  {
    uint32  PWR_SOURCE    :2;
    uint32  PADS_1P8V_SEL :1;
    uint32  SPIF_1P8V_SEL :1;
    uint32  JTAG_SECU_OFF :1;
    uint32  XTAL32_EXISTS :1;
    uint32                :2;
    uint32  XTAL_CLKDIV   :6;
    uint32                :18;
  } Bits;
  uint32 Reg;
} CRM_SYS_CNTL_T;
__no_init extern volatile CRM_SYS_CNTL_T _SYS_CNTL @(MBAR_CRM + 0x00);          /* CRM System Control R/W 32 */
#define SYS_CNTL                  _SYS_CNTL.Reg
#define CRM_SYS_PWR_SOURCE        _SYS_CNTL.Bits.PWR_SOURCE                     /* BIT0 | BIT1 */
#define CRM_SYS_PADS_1P8V_SEL     _SYS_CNTL.Bits.PADS_1P8V_SEL                  /* BIT2 */
#define CRM_SYS_SPIF_1P8V_SEL     _SYS_CNTL.Bits.SPIF_1P8V_SEL                  /* BIT3 */
#define CRM_SYS_JTAG_SECU_OFF     _SYS_CNTL.Bits.JTAG_SECU_OFF                  /* BIT4 */
#define CRM_SYS_XTAL32_EXISTS     _SYS_CNTL.Bits.XTAL32_EXISTS                  /* BIT5 */
#define CRM_SYS_XTAL_CLKDIV       _SYS_CNTL.Bits.XTAL_CLKDIV                    /* BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 */


typedef union
{
  struct
  {
    uint32  TIMER_WU_EN   :1;
    uint32  RTC_WU_EN     :1;
    uint32  HOST_WAKE     :1;
    uint32  AUTO_ADC      :1;
    uint32  EXT_WU_EN     :4;
    uint32  EXT_WU_EDGE   :4;
    uint32  EXT_WU_POL    :4;
    uint32  TIMER_WU_IEN  :1;
    uint32  RTC_WU_IEN    :1;
    uint32                :2;
    uint32  EXT_WU_IEN    :4;
    uint32                :4;
    uint32  EXT_OUT_POL   :4;
  } Bits;
  uint32 Reg;
} CRM_WU_CNTL_T;
__no_init extern volatile CRM_WU_CNTL_T _WU_CNTL @(MBAR_CRM + 0x04);            /* CRM Wakeup Control R/W 32 */
#define WU_CNTL                     _WU_CNTL.Reg
#define CRM_WU_CNTL_TIMER_WU_EN     _WU_CNTL.Bits.TIMER_WU_EN                   /* BIT0 */
#define CRM_WU_CNTL_RTC_WU_EN       _WU_CNTL.Bits.RTC_WU_EN                     /* BIT1 */
#define CRM_WU_CNTL_HOST_WAKE       _WU_CNTL.Bits.HOST_WAKE                     /* BIT2 */
#define CRM_WU_CNTL_AUTO_ADC        _WU_CNTL.Bits.AUTO_ADC                      /* BIT3 */
#define CRM_WU_CNTL_EXT_WU_EN       _WU_CNTL.Bits.EXT_WU_EN                     /* BIT4 | BIT5 | BIT6 | BIT7 */
#define CRM_WU_CNTL_EXT_WU_EDGE     _WU_CNTL.Bits.EXT_WU_EDGE                   /* BIT8 | BIT9 | BIT10 | BIT11 */
#define CRM_WU_CNTL_EXT_WU_POL      _WU_CNTL.Bits.EXT_WU_POL                    /* BIT12 | BIT13 | BIT14 | BIT15 */
#define CRM_WU_CNTL_TIMER_WU_IEN    _WU_CNTL.Bits.TIMER_WU_IEN                  /* BIT16 */
#define CRM_WU_CNTL_RTC_WU_IEN      _WU_CNTL.Bits.RTC_WU_IEN                    /* BIT17 */
#define CRM_WU_CNTL_EXT_WU_IEN      _WU_CNTL.Bits.EXT_WU_IEN                    /* BIT20 | BIT21 | BIT22 | BIT23 */
#define CRM_WU_CNTL_EXT_OUT_POL     _WU_CNTL.Bits.EXT_OUT_POL                   /* BIT28 | BIT29 | BIT30 | BIT31 */


typedef union
{
  struct
  {
    uint32 HIB            :1;
    uint32 DOZE           :1;
    uint32                :2;
    uint32 RAM_RET        :2;
    uint32 MCU_RET        :1;
    uint32 DIG_PAD_EN     :1;
    uint32                :24;
  } Bits;
  uint32 Reg;
}CRM_SLEEP_CNTL_T;
__no_init extern volatile CRM_SLEEP_CNTL_T _SLEEP_CNTL @(MBAR_CRM + 0x08);      /* CRM Sleep Control R/W 32 */
#define SLEEP_CNTL                  _SLEEP_CNTL.Reg
#define CRM_SLEEP_CNTL_HIB          _SLEEP_CNTL.Bits.HIB                        /* BIT0 */
#define CRM_SLEEP_CNTL_DOZE         _SLEEP_CNTL.Bits.DOZE                       /* BIT1 */
#define CRM_SLEEP_CNTL_RAM_RET      _SLEEP_CNTL.Bits.RAM_RET                    /* BIT4 | BIT5 */
#define CRM_SLEEP_CNTL_MCU_RET      _SLEEP_CNTL.Bits.MCU_RET                    /* BIT6 */
#define CRM_SLEEP_CNTL_DIG_PAD_EN   _SLEEP_CNTL.Bits.DIG_PAD_EN                 /* BIT7 */


typedef union
{
  struct
  {
    uint32 BS_EN            :1;
    uint32 WAIT4IRQ         :1;
    uint32 BS_MAN_EN        :1;
    uint32 ARM_CLKG_EN      :1;
    uint32                  :4;
    uint32 ARM_OFF_TIME     :6;
    uint32                  :18;
  } Bits;
  uint32 Reg;
}CRM_BS_CNTL_T;
__no_init extern volatile CRM_BS_CNTL_T _BS_CNTL @(MBAR_CRM + 0x0C);            /* CRM Bus Stealing Control R/W 32 */
#define BS_CNTL                   _BS_CNTL.Reg
#define CRM_BS_CNTL_BS_EN         _BS_CNTL.Bits.BS_EN                           /* BIT0 */
#define CRM_BS_CNTL_WAIT4IRQ      _BS_CNTL.Bits.WAIT4IRQ                        /* BIT1 */
#define CRM_BS_CNTL_BS_MAN_EN     _BS_CNTL.Bits.BS_MAN_EN                       /* BIT2 */
#define CRM_BS_CNTL_ARM_CLKG_EN   _BS_CNTL.Bits.ARM_CLKG_EN                     /* BIT3 */
#define CRM_BS_CNTL_ARM_OFF_TIME  _BS_CNTL.Bits.ARM_OFF_TIME                    /*  */


typedef union
{
  struct
  {
    uint32 COP_EN           :1;
    uint32 COP_OUT          :1;
    uint32 COP_WP           :1;
    uint32                  :5;
    uint32 COP_TIMEOUT      :7;
    uint32                  :1;
    uint32 COP_COUNT        :7;
    uint32                  :9;
  } Bits;
  uint32 Reg;
}CRM_COP_CNTL_T;
__no_init extern volatile CRM_COP_CNTL_T _COP_CNTL @(MBAR_CRM + 0x10);          /* CRM COP Control R/W 32 */
#define COP_CNTL                    _COP_CNTL.Reg
#define CRM_COP_CNTL_COP_EN         _COP_CNTL.Bits.COP_EN                       /* BIT0 */
#define CRM_COP_CNTL_COP_OUT        _COP_CNTL.Bits.COP_OUT                      /* BIT1 */
#define CRM_COP_CNTL_COP_WP         _COP_CNTL.Bits.COP_WP                       /* BIT2 */
#define CRM_COP_CNTL_COP_TIMEOUT    _COP_CNTL.Bits.COP_TIMEOUT                  /*  */
#define CRM_COP_CNTL_COP_COUNT      _COP_CNTL.Bits.COP_COUNT                    /*  */


typedef uint32 CRM_COP_SERVICE_T;
__no_init extern volatile CRM_COP_SERVICE_T _COP_SERVICE @(MBAR_CRM + 0x14);    /* CRM COP Service R/W 32 */
#define COP_SERVICE                 _COP_SERVICE


typedef union
{
  struct
  {
    uint32 SLEEP_SYNC         :1;
    uint32 HIB_WU_EVT         :1;
    uint32 DOZE_WU_EVT        :1;
    uint32 RTC_WU_EVT         :1;
    uint32 EXT_WU_EVT         :4;
    uint32 XTAL32_RDY         :1;
    uint32 CAL_DONE           :1;
    uint32 COP_EVT            :1;
    uint32                    :5;
    uint32 XTAL32_EXISTS      :1;
    uint32 VREG_BUCK_RDY      :1;
    uint32 VREG_1P8V_RDY      :1;
    uint32 VREG_1P5V_RDY      :1;
  } Bits;
  uint32 Reg;
}CRM_STATUS_T;
__no_init extern volatile CRM_STATUS_T _CRM_STATUS @(MBAR_CRM + 0x18);          /* CRM Event Status R/W 32 */
//#define CRM_STATUS                _CRM_STATUS.Reg
#define CRM_STATUS_SLEEP_SYNC     _CRM_STATUS.Bits.SLEEP_SYNC                   /* BIT0 */
#define CRM_STATUS_HIB_WU_EVT     _CRM_STATUS.Bits.HIB_WU_EVT                   /* BIT1 */
#define CRM_STATUS_DOZE_WU_EVT    _CRM_STATUS.Bits.DOZE_WU_EVT                  /* BIT2 */
#define CRM_STATUS_RTC_WU_EVT     _CRM_STATUS.Bits.RTC_WU_EVT                   /* BIT3 */
#define CRM_STATUS_EXT_WU_EVT     _CRM_STATUS.Bits.EXT_WU_EVT                   /* BIT4 | BIT5 | BIT6 | BIT7 */
#define CRM_STATUS_CAL_DONE       _CRM_STATUS.Bits.CAL_DONE                     /* BIT9 */
#define CRM_STATUS_COP_EVT        _CRM_STATUS.Bits.COP_EVT                      /* BIT10 */
#define CRM_STATUS_VREG_BUCK_RDY  _CRM_STATUS.Bits.VREG_BUCK_RDY                /* BIT17 */
#define CRM_STATUS_VREG_1P8V_RDY  _CRM_STATUS.Bits.VREG_1P8V_RDY                /* BIT18 */
#define CRM_STATUS_VREG_1P5V_RDY  _CRM_STATUS.Bits.VREG_1P5V_RDY                /* BIT19 */


typedef union
{
  struct
  {
    uint32 ARM_EN             :1;
    uint32 MACA_EN            :1;
    uint32 ASM_EN             :1;
    uint32 SPI_EN             :1;
    uint32 GPIO_EN            :1;
    uint32 UART1_EN           :1;
    uint32 UART2_EN           :1;
    uint32 TMR_EN             :1;
    uint32 RIF_EN             :1;
    uint32 I2C_EN             :1;
    uint32 SSI_EN             :1;
    uint32 SPIF_EN            :1;
    uint32 ADC_EN             :1;
    uint32 AHB_EN             :1;
    uint32 JTA_EN             :1;
    uint32 NEX_EN             :1;
    uint32 TMX_EN             :1;
    uint32 AIM_EN             :1;
    uint32                    :14;
  } Bits;
  uint32 Reg;
}CRM_MOD_STATUS_T;
__no_init extern volatile CRM_MOD_STATUS_T _MOD_STATUS @(MBAR_CRM + 0x1C);      /* CRM Module Enable Status R 32 */
#define MOD_STATUS                _MOD_STATUS.Reg


typedef uint32 CRM_WU_COUNT_T;
__no_init extern volatile CRM_WU_COUNT_T _WU_COUNT @(MBAR_CRM + 0x20);          /* CRM Wakeup Count R 32 */
#define WU_COUNT                  _WU_COUNT


typedef uint32 CRM_WU_TIMEOUT_T;
__no_init extern volatile CRM_WU_TIMEOUT_T _WU_TIMEOUT @(MBAR_CRM + 0x24);      /* CRM Wakeup Timeout R/W 32 */
#define WU_TIMEOUT                _WU_TIMEOUT


typedef uint32 CRM_RTC_COUNT_T;
__no_init extern volatile CRM_RTC_COUNT_T _RTC_COUNT @(MBAR_CRM + 0x28);        /* CRM Real Time Count R/W 32 */
#define RTC_COUNT                 _RTC_COUNT


typedef uint32 CRM_RTC_TIMEOUT_T;
__no_init extern volatile CRM_RTC_TIMEOUT_T _RTC_TIMEOUT @(MBAR_CRM + 0x2C);    /* CRM RTC Periodic Wakeup Timeout R/W 32 */
#define RTC_TIMEOUT               _RTC_TIMEOUT


typedef union
{
  struct
  {
    uint32 CAL_TIMEOUT      :16;
    uint32 CAL_EN           :1;
    uint32 CAL_IEN          :1;
    uint32                  :14;
  } Bits;
  uint32 Reg;
}CRM_CAL_CNTL_T;
__no_init extern volatile CRM_CAL_CNTL_T _CAL_CNTL @(MBAR_CRM + 0x34);          /* CRM Calibration Control R/W 32 */
#define CAL_CNTL                  _CAL_CNTL.Reg


typedef uint32 CRM_CAL_COUNT_T;
__no_init extern volatile CRM_CAL_COUNT_T _CAL_COUNT @(MBAR_CRM + 0x38);        /* CRM Calibration XTAL Count R 32 */
#define CAL_COUNT                 _CAL_COUNT


typedef union
{
  struct
  {
    uint32 ROSC_EN          :1;
    uint32 ROSC_BIAS_SEL    :3;
    uint32 ROSC_FTUNE       :5;
    uint32 ROSC_CTUNE       :4;
    uint32                  :19;
  } Bits;
  uint32 Reg;
} CRM_RINGOSC_CNTL_T;
__no_init extern volatile CRM_RINGOSC_CNTL_T _RINGOSC_CNTL @(MBAR_CRM + 0x3C);  /* CRM 2kHz Ring Oscillator Control R/W 32 */
#define RINGOSC_CNTL                 _RINGOSC_CNTL.Reg


typedef union
{
  struct
  {
    uint32 XTAL_WP            :1;
    uint32 XTAL_SMALL_SIG     :1;
    uint32 XTAL_BYPASS        :1;
    uint32                    :1;
    uint32 XTAL_TIME_SEL      :3;
    uint32                    :1;
    uint32 XTAL_IBIAS_SEL     :4;
    uint32                    :4;
    uint32 XTAL_FTUNE         :5;
    uint32 XTAL_CTUNE         :5;
    uint32                    :6;
  } Bits;
  uint32 Reg;
} CRM_XTAL_CNTL_T;
__no_init extern volatile CRM_XTAL_CNTL_T _XTAL_CNTL @(MBAR_CRM + 0x40);        /* CRM Reference XTAL Control R/W 32 */
#define XTAL_CNTL                    _XTAL_CNTL.Reg


typedef union
{
  struct
  {
    uint32 XTAL32_EN    :1;
    uint32              :31;
  } Bits;
  uint32 Reg;
} CRM_XTAL32_CNTL_T;
__no_init extern volatile CRM_XTAL32_CNTL_T _XTAL32_CNTL @(MBAR_CRM + 0x44);    /* CRM 32kHz XTAL Control R/W 32 */
#define XTAL32_CNTL                 _XTAL32_CNTL.Reg


typedef union
{
  struct
  {
    uint32 BUCK_EN            :1;
    uint32 BUCK_SYNC_REC_EN   :1;
    uint32 BUCK_BYPASS_EN     :1;
    uint32 VREG_1P5V_EN       :2;
    uint32 VREG_1P5V_SEL      :2;
    uint32 VREG_1P8V_EN       :1;
    uint32 BUCK_CLKDIV        :4;
    uint32 VREG_BUCK_PASS_N   :1;
    uint32                    :19;
  } Bits;
  uint32 Reg;
} CRM_VREG_CNTL_T;
__no_init extern volatile CRM_VREG_CNTL_T _VREG_CNTL @(MBAR_CRM + 0x48);        /* CRM Voltage Regulator Control R/W 32 */
#define VREG_CNTL                   _VREG_CNTL.Reg


typedef union
{
  struct
  {
    uint32 BGAP_BATT_TRIM     :4;
    uint32 VREG_1P8V_TRIM     :4;
    uint32 VREG_0P9V_TRIM     :4;
    uint32 VREG_0P9I_TRIM     :4;
    uint32 VREG_XTAL_TRIM     :4;
    uint32 VREG_ROSC_TRIM     :4;
    uint32                    :8;
  } Bits;
  uint32 Reg;
} CRM_VREG_TRIM_T;
__no_init extern volatile CRM_VREG_TRIM_T _VREG_TRIM @(MBAR_CRM + 0x4C);        /* CRM Voltage Regulator Trim R/W 32 */
#define VREG_TRIM                   _VREG_TRIM.Reg


typedef uint32 CRM_SW_RST_T;
__no_init extern volatile CRM_SW_RST_T _SW_RST @(MBAR_CRM + 0x50);              /* CRM Software Reset R/W 32 */
#define SW_RST                 _SW_RST



/* ------------------------- MAC Accelerator (MACA)-------------------------- */
/*    Base Adrress = MBAR_MACA = 0x8000_4000                                  */
typedef union
{
  struct
  {
    uint32 MINOR  :8;
    uint32        :8;
    uint32 MAJOR  :8;
    uint32        :8;
  } Bits;
  uint32 Reg;
} MACA_VERSION_T;
__no_init extern volatile MACA_VERSION_T _MACA_VERSION @(MBAR_MACA + 0x00);     /* MACA Version Number R 32 */
#define MACA_VERSION                   _MACA_VERSION.Reg
#define MACA_VERSION_MINOR             _MACA_VERSION.Bits.MINOR
#define MACA_VERSION_MAJOR             _MACA_VERSION.Bits.MAJOR


typedef union
{
  struct
  {
    uint32 RST      :1;
    uint32 CLK_ON   :1;
    uint32          :30;
  } Bits;
  uint32 Reg;
} MACA_RESET_T;
__no_init extern volatile MACA_RESET_T _MACA_RESET @(MBAR_MACA + 0x04);         /* MACA Reset R/W 32 */
#define MACA_RESET                    _MACA_RESET.Reg
#define MACA_RESET_RST                _MACA_RESET.Bits.RST
#define MACA_RESET_CLK_ON             _MACA_RESET.Bits.CLK_ON


typedef uint32 MACA_RANDOM_T;
__no_init extern volatile MACA_RANDOM_T _MACA_RANDOM @(MBAR_MACA + 0x08);       /* MACA Random Number R/W 32 */
#define MACA_RANDOM                   _MACA_RANDOM


typedef union
{
  struct
  {
    uint32 SEQUENCE   :3;
    uint32 MODE       :2;
    uint32 TM         :1;
    uint32 LFSR       :1;
    uint32 AUTO       :1;
    uint32 BCN        :1;
    uint32 ASAP       :1;
    uint32 REL        :1;
    uint32 PRM        :1;
    uint32 NOFC       :1;
    uint32 ROLE       :1;
    uint32 RSV        :1;
    uint32 RSTO       :1;
    uint32 PRE_COUNT  :4;
    uint32 ISM        :1;
    uint32            :11;
  } Bits;
  uint32 Reg;
} MACA_CONTROL_T;
__no_init extern volatile MACA_CONTROL_T _MACA_CONTROL @(MBAR_MACA + 0x0C);     /* MACA Control W 32 */
#define MACA_CONTROL		          _MACA_CONTROL.Reg
#define MACA_CONTROL_SEQUENCE	          _MACA_CONTROL.Bits.SEQUENCE
#define MACA_CONTROL_MODE	          _MACA_CONTROL.Bits.MODE
#define MACA_CONTROL_TM		          _MACA_CONTROL.Bits.TM
#define MACA_CONTROL_LFSR	          _MACA_CONTROL.Bits.LFSR
#define MACA_CONTROL_AUTO	          _MACA_CONTROL.Bits.AUTO
#define MACA_CONTROL_BCN	          _MACA_CONTROL.Bits.BCN
#define MACA_CONTROL_ASAP	          _MACA_CONTROL.Bits.ASAP
#define MACA_CONTROL_REL	          _MACA_CONTROL.Bits.REL
#define MACA_CONTROL_PRM	          _MACA_CONTROL.Bits.PRM
#define MACA_CONTROL_NOFC	          _MACA_CONTROL.Bits.NOFC
#define MACA_CONTROL_ROLE	          _MACA_CONTROL.Bits.ROLE
#define MACA_CONTROL_RSV	          _MACA_CONTROL.Bits.RSV
#define MACA_CONTROL_RSTO	          _MACA_CONTROL.Bits.RSTO
#define MACA_CONTROL_PRE_COUNT	          _MACA_CONTROL.Bits.PRE_COUNT
#define MACA_CONTROL_ISM	          _MACA_CONTROL.Bits.ISM


typedef union
{
  struct
  {
    uint32 COMPLETE_CODE    :4;
    uint32                  :7;
    uint32 ZIGBEE           :1;
    uint32 OVR              :1;
    uint32 BUSY             :1;
    uint32 CRC              :1;
    uint32 TO               :1;
    uint32                  :16;
  } Bits;
  uint32 Reg;
} MACA_STATUS_T;
__no_init extern volatile MACA_STATUS_T _MACA_STATUS @(MBAR_MACA + 0x10);       /* MACA Status R 32 */
#define MACA_STATUS		          _MACA_STATUS.Reg
#define MACA_STATUS_COMPLETE_CODE	  _MACA_STATUS.Bits.COMPLETE_CODE
#define MACA_STATUS_ZIGBEE	          _MACA_STATUS.Bits.ZIGBEE
#define MACA_STATUS_OVR		          _MACA_STATUS.Bits.OVR
#define MACA_STATUS_BUSY	          _MACA_STATUS.Bits.BUSY
#define MACA_STATUS_CRC	                  _MACA_STATUS.Bits.CRC
#define MACA_STATUS_TO	                  _MACA_STATUS.Bits.TO


typedef union
{
  struct
  {
    uint32 PND    :1;
    uint32        :31;
  } Bits;
  uint32 Reg;
} MACA_FRMPND_T;
__no_init extern volatile MACA_FRMPND_T _MACA_FRMPND @(MBAR_MACA + 0x14);       /* MACA Frame Pending R/W 32 */
#define MACA_FRMPND                 _MACA_FRMPND.Reg
#define MACA_FRMPND_PND	            _MACA_FRMPND.Bits.PND

/*
typedef union
{
  struct
  {
    uint32 CHANNEL    :4;
    uint32            :28;
  } Bits;
  uint32 Reg;
} MACA_FREQ_T;
__no_init extern volatile MACA_FREQ_T _MACA_FREQ @(MBAR_MACA + 0x18);           // MACA Frequency Channel R/W 32 
#define MACA_FREQ                 _MACA_FREQ.Reg
#define MACA_FREQ_CHANNEL         _MACA_FREQ.Bits.CHANNEL
*/

typedef uint32 MACA_MC1322x_ID_T;
__no_init extern volatile MACA_MC1322x_ID_T _MACA_MC1322x_ID @(MBAR_MACA + 0x18);           // MACA MC1322x ID 
#define MACA_MC1322x_ID               _MACA_MC1322x_ID


typedef uint32 MACA_EDVALUE_T;
__no_init extern volatile MACA_EDVALUE_T _MACA_EDVALUE @(MBAR_MACA + 0x1C);     /* MACA Energy Detect Result R 32 */
#define MACA_EDVALUE                   _MACA_EDVALUE


typedef union
{
  struct
  {
    uint32 STRT       :1;
    uint32 CPL        :1;
    uint32 SFT        :1;
    uint32            :29;
  } Bits;
  uint32 Reg;
} MACA_TMREN_T;
__no_init extern volatile MACA_TMREN_T _MACA_TMREN @(MBAR_MACA + 0x40);         /* MACA Enable Timers R/W 32 */
#define MACA_TMREN		        _MACA_TMREN.Reg
#define MACA_TMREN_STRT	                _MACA_TMREN.Bits.STRT
#define MACA_TMREN_CPL	                _MACA_TMREN.Bits.CPL
#define MACA_TMREN_SFT		        _MACA_TMREN.Bits.SFT


typedef union
{
  struct
  {
    uint32 STRT       :1;
    uint32 CPL        :1;
    uint32 SFT        :1;
    uint32 STRT_OFF   :1;
    uint32 CPL_OFF    :1;
    uint32 SFT_OFF    :1;
    uint32            :26;
  } Bits;
  uint32 Reg;
} MACA_TMRDIS_T;
__no_init extern volatile MACA_TMRDIS_T _MACA_TMRDIS @(MBAR_MACA + 0x44);       /* MACA Disable Timers R/W 32 */
#define MACA_TMRDIS		      _MACA_TMRDIS.Reg
#define MACA_TMRDIS_STRT	      _MACA_TMRDIS.Bits.STRT
#define MACA_TMRDIS_CPL	              _MACA_TMRDIS.Bits.CPL
#define MACA_TMRDIS_SFT		      _MACA_TMRDIS.Bits.SFT
#define MACA_TMRDIS_STRT_OFF	      _MACA_TMRDIS.Bits.STRT_OFF
#define MACA_TMRDIS_CPL_OFF           _MACA_TMRDIS.Bits.CPL_OFF
#define MACA_TMRDIS_SFT_OFF           _MACA_TMRDIS.Bits.SFT_OFF


typedef uint32 MACA_CLK_T;
__no_init extern volatile MACA_CLK_T _MACA_CLK @(MBAR_MACA + 0x48);             /* MACA Clock R/W 32 */
#define MACA_CLK                   _MACA_CLK


typedef uint32 MACA_STARTCLK_T;
__no_init extern volatile MACA_STARTCLK_T _MACA_STARTCLK @(MBAR_MACA + 0x4C);   /* MACA Start Clock R/W 32 */
#define MACA_STARTCLK                   _MACA_STARTCLK


typedef uint32 MACA_CPLCLK_T;
__no_init extern volatile MACA_CPLCLK_T _MACA_CPLCLK @(MBAR_MACA + 0x50);       /* MACA Complete Clock R/W 32 */
#define MACA_CPLCLK                   _MACA_CPLCLK


typedef uint32 MACA_SFTCLK_T;
__no_init extern volatile MACA_SFTCLK_T _MACA_SFTCLK @(MBAR_MACA + 0x54);       /* MACA Soft Timeout Clock R/W 32 */
#define MACA_SFTCLK                   _MACA_SFTCLK


typedef uint32 MACA_CLKOFFSET_T;
__no_init extern volatile MACA_CLKOFFSET_T _MACA_CLKOFFSET @(MBAR_MACA + 0x58); /* MACA Clock offset R/W 32 */
#define MACA_CLKOFFSET                   _MACA_CLKOFFSET


typedef uint32 MACA_RELCLK_T;
__no_init extern volatile MACA_RELCLK_T _MACA_RELCLK @(MBAR_MACA + 0x5C);       /* MACA Relative clock R/W 32 */
#define MACA_RELCLK                   _MACA_RELCLK


typedef uint32 MACA_CPLTIM_T;
__no_init extern volatile MACA_CPLTIM_T _MACA_CPLTIM @(MBAR_MACA + 0x60);       /* MACA Action Complete Timestamp R 32 */
#define MACA_CPLTIM                   _MACA_CPLTIM


typedef union
{
  struct
  {
    uint32 TX_SLOT_OFFSET   :12;
    uint32                  :4;
    uint32 RX_SLOT_OFFSET   :12;
    uint32                  :4;
  } Bits;
  uint32 Reg;
} MACA_SLOTOFFSET_T;
__no_init extern volatile MACA_SLOTOFFSET_T _MACA_SLOTOFFSET @(MBAR_MACA + 0x64); /* MACA Tx Slot Offset Adjustment R/W 32 */
#define MACA_SLOTOFFSET                     _MACA_SLOTOFFSET.Reg
#define MACA_SLOTOFFSET_TX_SLOT_OFFSET      _MACA_SLOTOFFSET.Bits.TX_SLOT_OFFSET
#define MACA_SLOTOFFSET_RX_SLOT_OFFSET      _MACA_SLOTOFFSET.Bits.RX_SLOT_OFFSET


typedef uint32 MACA_TIMESTAMP_T;
__no_init extern volatile MACA_TIMESTAMP_T _MACA_TIMESTAMP @(MBAR_MACA + 0x68); /* MACA Receive Time Stamp R 32 */
#define MACA_TIMESTAMP                   _MACA_TIMESTAMP


typedef uint32 MACA_DMARX_T;
__no_init extern volatile MACA_DMARX_T _MACA_DMARX @(MBAR_MACA + 0x80);         /* MACA DMA Rx Data Pointer R/W 32 */
#define MACA_DMARX                   _MACA_DMARX


typedef uint32 MACA_DMATX_T;
__no_init extern volatile MACA_DMATX_T _MACA_DMATX @(MBAR_MACA + 0x84);         /* MACA DMA Tx Data Pointer R/W 32 */
#define MACA_DMATX                   _MACA_DMATX


typedef uint32 MACA_DMAPOLL_T;
__no_init extern volatile MACA_DMAPOLL_T _MACA_DMAPOLL @(MBAR_MACA + 0x88);     /* MACA DMA Tx Poll Response Pointer R/W 32 */
#define MACA_DMAPOLL                   _MACA_DMAPOLL


typedef union
{
  struct
  {
    uint32 TX_LEN   :15;
    uint32          :1;
    uint32 RX_LEN   :15;
    uint32          :1;
  } Bits;
  uint32 Reg;
} MACA_TXLEN_T;
__no_init extern volatile MACA_TXLEN_T _MACA_TXLEN @(MBAR_MACA + 0x8C);         /* MACA Tx Length R/W 32 */
#define MACA_TXLEN              _MACA_TXLEN.Reg
#define MACA_TXLEN_TX_LEN       _MACA_TXLEN.Bits.TX_LEN
#define MACA_TXLEN_RX_LEN       _MACA_TXLEN.Bits.RX_LEN


typedef union
{
  struct
  {
    uint32 TXSEQN   :8;
    uint32          :24;
  } Bits;
  uint32 Reg;
} MACA_TXSEQNR_T;
__no_init extern volatile MACA_TXSEQNR_T _MACA_TXSEQNR @(MBAR_MACA + 0x90);     /* MACA Tx Sequence Number R/W 32 */
#define MACA_TXSEQNR              _MACA_TXSEQNR.Reg
#define MACA_TXSEQNR_TXSEQN       _MACA_TXSEQNR.Bits.TXSEQN


typedef union
{
  struct
  {
    uint32 FIFO_LVL   :16;
    uint32            :16;
  } Bits;
  uint32 Reg;
} MACA_SETRXLVL_T;
__no_init extern volatile MACA_SETRXLVL_T _MACA_SETRXLVL @(MBAR_MACA + 0x94);   /* MACA Set Rx Level Interrupt R/W 32 */
#define MACA_SETRXLVL               _MACA_SETRXLVL.Reg
#define MACA_SETRXLVL_FIFO_LVL      _MACA_SETRXLVL.Bits.FIFO_LVL


typedef union
{
  struct
  {
    uint32 RECVBYTES    :16;
    uint32              :16;
  } Bits;
  uint32 Reg;
} MACA_GETRXLVL_T;
__no_init extern volatile MACA_GETRXLVL_T _MACA_GETRXLVL @(MBAR_MACA + 0x98);   /* MACA Read Number of Received Bytes R 32 */
#define MACA_GETRXLVL               _MACA_GETRXLVL.Reg
#define MACA_GETRXLVL_RECVBYTES     _MACA_GETRXLVL.Bits.RECVBYTES


__no_init extern volatile MACA_IRQ_T _MACA_IRQ @(MBAR_MACA + 0xC0);             /* MACA Interrupt R 32 */
#define MACA_IRQ		  _MACA_IRQ.Reg
#define MACA_IRQ_ACPL	          _MACA_IRQ.Bits.ACPL
#define MACA_IRQ_POLL	          _MACA_IRQ.Bits.POLL
#define MACA_IRQ_DI		  _MACA_IRQ.Bits.DI
#define MACA_IRQ_WU	          _MACA_IRQ.Bits.WU
#define MACA_IRQ_RST	          _MACA_IRQ.Bits.RST
#define MACA_IRQ_LVL	          _MACA_IRQ.Bits.LVL
#define MACA_IRQ_SFT	          _MACA_IRQ.Bits.SFT
#define MACA_IRQ_FLT	          _MACA_IRQ.Bits.FLT
#define MACA_IRQ_CRC	          _MACA_IRQ.Bits.CRC
#define MACA_IRQ_CM	          _MACA_IRQ.Bits.CM
#define MACA_IRQ_SYNC	          _MACA_IRQ.Bits.SYNC
#define MACA_IRQ_STRT	          _MACA_IRQ.Bits.STRT


__no_init extern volatile MACA_IRQ_T _MACA_CLRIRQ @(MBAR_MACA + 0xC4);          /* MACA Interrupt Clear W 32 */
#define MACA_CLRIRQ		  _MACA_CLRIRQ.Reg
#define MACA_CLRIRQ_ACPL          _MACA_CLRIRQ.Bits.ACPL
#define MACA_CLRIRQ_POLL          _MACA_CLRIRQ.Bits.POLL
#define MACA_CLRIRQ_DI		  _MACA_CLRIRQ.Bits.DI
#define MACA_CLRIRQ_WU	          _MACA_CLRIRQ.Bits.WU
#define MACA_CLRIRQ_RST	          _MACA_CLRIRQ.Bits.RST
#define MACA_CLRIRQ_LVL	          _MACA_CLRIRQ.Bits.LVL
#define MACA_CLRIRQ_SFT	          _MACA_CLRIRQ.Bits.SFT
#define MACA_CLRIRQ_FLT	          _MACA_CLRIRQ.Bits.FLT
#define MACA_CLRIRQ_CRC	          _MACA_CLRIRQ.Bits.CRC
#define MACA_CLRIRQ_CM	          _MACA_CLRIRQ.Bits.CM
#define MACA_CLRIRQ_SYNC          _MACA_CLRIRQ.Bits.SYNC
#define MACA_CLRIRQ_STRT          _MACA_CLRIRQ.Bits.STRT


__no_init extern volatile MACA_IRQ_T _MACA_SETIRQ @(MBAR_MACA + 0xC8);          /* MACA Interrupt Set W 32 */
#define MACA_SETIRQ		  _MACA_SETIRQ.Reg
#define MACA_SETIRQ_ACPL          _MACA_SETIRQ.Bits.ACPL
#define MACA_SETIRQ_POLL          _MACA_SETIRQ.Bits.POLL
#define MACA_SETIRQ_DI		  _MACA_SETIRQ.Bits.DI
#define MACA_SETIRQ_WU	          _MACA_SETIRQ.Bits.WU
#define MACA_SETIRQ_RST	          _MACA_SETIRQ.Bits.RST
#define MACA_SETIRQ_LVL	          _MACA_SETIRQ.Bits.LVL
#define MACA_SETIRQ_SFT	          _MACA_SETIRQ.Bits.SFT
#define MACA_SETIRQ_FLT	          _MACA_SETIRQ.Bits.FLT
#define MACA_SETIRQ_CRC	          _MACA_SETIRQ.Bits.CRC
#define MACA_SETIRQ_CM	          _MACA_SETIRQ.Bits.CM
#define MACA_SETIRQ_SYNC          _MACA_SETIRQ.Bits.SYNC
#define MACA_SETIRQ_STRT          _MACA_SETIRQ.Bits.STRT


__no_init extern volatile MACA_IRQ_T _MACA_MASKIRQ @(MBAR_MACA + 0xCC);         /* MACA Interrupt Mask W 32 */
#define MACA_MASKIRQ		  _MACA_MASKIRQ.Reg
#define MACA_MASKIRQ_ACPL         _MACA_MASKIRQ.Bits.ACPL
#define MACA_MASKIRQ_POLL         _MACA_MASKIRQ.Bits.POLL
#define MACA_MASKIRQ_DI		  _MACA_MASKIRQ.Bits.DI
#define MACA_MASKIRQ_WU	          _MACA_MASKIRQ.Bits.WU
#define MACA_MASKIRQ_RST          _MACA_MASKIRQ.Bits.RST
#define MACA_MASKIRQ_LVL          _MACA_MASKIRQ.Bits.LVL
#define MACA_MASKIRQ_SFT          _MACA_MASKIRQ.Bits.SFT
#define MACA_MASKIRQ_FLT          _MACA_MASKIRQ.Bits.FLT
#define MACA_MASKIRQ_CRC          _MACA_MASKIRQ.Bits.CRC
#define MACA_MASKIRQ_CM	          _MACA_MASKIRQ.Bits.CM
#define MACA_MASKIRQ_SYNC         _MACA_MASKIRQ.Bits.SYNC
#define MACA_MASKIRQ_STRT         _MACA_MASKIRQ.Bits.STRT


typedef union
{
  struct
  {
    uint32 PANID    :16;
    uint32          :16;
  } Bits;
  uint32 Reg;
} MACA_MACPANID_T;
__no_init extern volatile MACA_MACPANID_T _MACA_MACPANID @(MBAR_MACA + 0x100);  /* MACA PAN ID R/W 32 */
#define MACA_MACPANID                 _MACA_MACPANID.Reg
#define MACA_MACPANID_PANID           _MACA_MACPANID.Bits.PANID


typedef union
{
  struct
  {
    uint32 ADDR     :16;
    uint32          :16;
  } Bits;
  uint32 Reg;
} MACA_MAC16ADDR_T;
__no_init extern volatile MACA_MAC16ADDR_T _MACA_MAC16ADDR @(MBAR_MACA + 0x104);/* MACA Short Address R/W 32 */
#define MACA_MAC16ADDR                  _MACA_MAC16ADDR.Reg
#define MACA_MAC16ADDR_ADDR             _MACA_MAC16ADDR.Bits.ADDR


typedef uint32 MACA_MAC64HI_T;
__no_init extern volatile MACA_MAC64HI_T _MACA_MAC64HI @(MBAR_MACA + 0x108);    /* MACA High Extended Address R/W 32 */
#define MACA_MAC64HI                   _MACA_MAC64HI


typedef uint32 MACA_MAC64LO_T;
__no_init extern volatile MACA_MAC64LO_T _MACA_MAC64LO @(MBAR_MACA + 0x10C);    /* MACA Low Extended Address R/W 32 */
#define MACA_MAC64LO                   _MACA_MAC64LO


typedef union
{
  struct
  {
    uint32 BCN      :1;
    uint32 DATA     :1;
    uint32 ACK      :1;
    uint32 CMD      :1;
    uint32          :4;
    uint32 POLL     :1;
    uint32          :7;
    uint32 FC_MASK  :16;
  } Bits;
  uint32 Reg;
} MACA_FLTREJ_T;
__no_init extern volatile MACA_FLTREJ_T _MACA_FLTREJ @(MBAR_MACA + 0x110);      /* MACA Filter Rejection Mask R/W 32 */
#define MACA_FLTREJ		    _MACA_FLTREJ.Reg
#define MACA_FLTREJ_BCN	            _MACA_FLTREJ.Bits.BCN
#define MACA_FLTREJ_DATA	    _MACA_FLTREJ.Bits.DATA
#define MACA_FLTREJ_ACK		    _MACA_FLTREJ.Bits.ACK
#define MACA_FLTREJ_CMD	            _MACA_FLTREJ.Bits.CMD
#define MACA_FLTREJ_POLL            _MACA_FLTREJ.Bits.POLL
#define MACA_FLTREJ_FC_MASK	    _MACA_FLTREJ.Bits.FC_MASK


typedef union
{
  struct
  {
    uint32 DIVIDER    :16;
    uint32            :16;
  } Bits;
  uint32 Reg;
} MACA_CLKDIV_T;
__no_init extern volatile MACA_CLKDIV_T _MACA_CLKDIV @(MBAR_MACA + 0x114);      /* MACA Clock Divider R/W 32 */
#define MACA_CLKDIV                   _MACA_CLKDIV.Reg
#define MACA_CLKDIV_DIVIDER           _MACA_CLKDIV.Bits.DIVIDER


typedef union
{
  struct
  {
    uint32 RX_WARMUP    :12;
    uint32              :4;
    uint32 TX_WARMUP    :12;
    uint32              :4;
  } Bits;
  uint32 Reg;
} MACA_WARMUP_T;
__no_init extern volatile MACA_WARMUP_T _MACA_WARMUP @(MBAR_MACA + 0x118);      /* MACA Warmup R/W 32 */
#define MACA_WARMUP                 _MACA_WARMUP.Reg
#define MACA_WARMUP_RX_WARMUP       _MACA_WARMUP.Bits.RX_WARMUP
#define MACA_WARMUP_TX_WARMUP       _MACA_WARMUP.Bits.TX_WARMUP


typedef uint32 MACA_PREAMBLE_T;
__no_init extern volatile MACA_PREAMBLE_T _MACA_PREAMBLE @(MBAR_MACA + 0x11C);  /* MACA Preamble R/W 32 */
#define MACA_PREAMBLE               _MACA_PREAMBLE


typedef union
{
  struct
  {
    uint32 SEED1      :7;
    uint32            :1;
    uint32 SEED2      :7;
    uint32            :17;
  } Bits;
  uint32 Reg;
} MACA_WHITESEED_T;
__no_init extern volatile MACA_WHITESEED_T _MACA_WHITESEED @(MBAR_MACA + 0x120);/* MACA LFSR Whitening Seed R/W 32 */
#define MACA_WHITESEED             _MACA_WHITESEED.Reg
#define MACA_WHITESEED_SEED1       _MACA_WHITESEED.Bits.SEED1
#define MACA_WHITESEED_SEED2       _MACA_WHITESEED.Bits.SEED2


typedef uint32 MACA_FRAMESYNC0_T;
__no_init extern volatile MACA_FRAMESYNC0_T _MACA_FRAMESYNC0 @(MBAR_MACA + 0x124);  /* MACA Frame Sync Word 0 R/W 32 */
#define MACA_FRAMESYNC0               _MACA_FRAMESYNC0


typedef uint32 MACA_FRAMESYNC1_T;
__no_init extern volatile MACA_FRAMESYNC1_T _MACA_FRAMESYNC1 @(MBAR_MACA + 0x128);  /* MACA Frame Sync Word 1 R/W 32 */
#define MACA_FRAMESYNC1               _MACA_FRAMESYNC1


typedef union
{
  struct
  {
    uint32 TXACKDELAY     :12;
    uint32                :4;
    uint32 TXPOLLDELAY    :12;
    uint32                :4;
  } Bits;
  uint32 Reg;
} MACA_TXACKDELAY_T;
__no_init extern volatile MACA_TXACKDELAY_T _MACA_TXACKDELAY @(MBAR_MACA + 0x140);  /* MACA Tx Acknowledgement Delay R/W 32 */
#define MACA_TXACKDELAY                   _MACA_TXACKDELAY.Reg
#define MACA_TXACKDELAY_TXACKDELAY        _MACA_TXACKDELAY.Bits.TXACKDELAY
#define MACA_TXACKDELAY_TXPOLLDELAY       _MACA_TXACKDELAY.Bits.TXPOLLDELAY


typedef union
{
  struct
  {
    uint32 RXACKDELAY     :12;
    uint32                :4;
    uint32 RXAUTODELAY    :12;
    uint32                :4;
  } Bits;
  uint32 Reg;
} MACA_RXACKDELAY_T;
__no_init extern volatile MACA_RXACKDELAY_T _MACA_RXACKDELAY @(MBAR_MACA + 0x144);  /* MACA Rx Acknowledgement Delay R/W 32 */
#define MACA_RXACKDELAY                   _MACA_RXACKDELAY.Reg
#define MACA_RXACKDELAY_RXACKDELAY        _MACA_RXACKDELAY.Bits.RXACKDELAY
#define MACA_RXACKDELAY_RXAUTODELAY       _MACA_RXACKDELAY.Bits.RXAUTODELAY


typedef union
{
  struct
  {
    uint32 EOF_DELAY  :12;
    uint32            :20;
  } Bits;
  uint32 Reg;
} MACA_EOFDELAY_T;
__no_init extern volatile MACA_EOFDELAY_T _MACA_EOFDELAY @(MBAR_MACA + 0x148);  /* MACA End of Frame Delay R/W 32 */
#define MACA_EOFDELAY                   _MACA_EOFDELAY.Reg
#define MACA_EOFDELAY_EOF_DELAY         _MACA_EOFDELAY.Bits.EOF_DELAY


typedef union
{
  struct
  {
    uint32 CCADELAY       :12;
    uint32                :4;
    uint32 CCALENGTH      :12;
    uint32                :4;
  } Bits;
  uint32 Reg;
} MACA_CCADELAY_T;
__no_init extern volatile MACA_CCADELAY_T _MACA_CCADELAY @(MBAR_MACA + 0x14C);  /* MACA CCA Delay R/W 32 */
#define MACA_CCADELAY                   _MACA_CCADELAY.Reg
#define MACA_CCADELAY_CCADELAY          _MACA_CCADELAY.Bits.CCADELAY
#define MACA_CCADELAY_CCALENGTH         _MACA_CCADELAY.Bits.CCALENGTH


typedef union
{
  struct
  {
    uint32 RXACK_END      :12;
    uint32                :4;
    uint32 RXSLOTTED_END  :12;
    uint32                :4;
  } Bits;
  uint32 Reg;
} MACA_RXEND_T;
__no_init extern volatile MACA_RXEND_T _MACA_RXEND @(MBAR_MACA + 0x150);        /* MACA Rx End R/W 32 */
#define MACA_RXEND                    _MACA_RXEND.Reg
#define MACA_RXEND_RXACK_END          _MACA_RXEND.Bits.RXACK_END
#define MACA_RXEND_RXSLOTTED_END      _MACA_RXEND.Bits.RXSLOTTED_END


typedef union
{
  struct
  {
    uint32 TXCCADELAY   :12;
    uint32              :20;
  } Bits;
  uint32 Reg;
} MACA_TXCCADELAY_T;
__no_init extern volatile MACA_TXCCADELAY_T _MACA_TXCCADELAY @(MBAR_MACA + 0x154);  /* MACA Tx CCA Delay R/W 32 */
#define MACA_TXCCADELAY                     _MACA_TXCCADELAY.Reg
#define MACA_TXCCADELAY_TXCCADELAY          _MACA_TXCCADELAY.Bits.TXCCADELAY


typedef uint32 MACA_KEY_T;
__no_init extern volatile MACA_KEY_T _MACA_KEY3 @(MBAR_MACA + 0x158);           /* MACA Key3 R 32 */
#define MACA_KEY3               _MACA_KEY3

__no_init extern volatile MACA_KEY_T _MACA_KEY2 @(MBAR_MACA + 0x15C);           /* MACA Key2 R 32 */
#define MACA_KEY2               _MACA_KEY2

__no_init extern volatile MACA_KEY_T _MACA_KEY1 @(MBAR_MACA + 0x160);           /* MACA Key1 R 32 */
#define MACA_KEY1               _MACA_KEY1

__no_init extern volatile MACA_KEY_T _MACA_KEY0 @(MBAR_MACA + 0x164);           /* MACA Key0 R 32 */
#define MACA_KEY0               _MACA_KEY0


typedef union
{
  struct
  {
    uint32 POLL         :1;
    uint32 PLL_TX       :1;
    uint32 PLL_IGNORE   :1;
    uint32 SEED_KEY     :1;
    uint32              :28;
  } Bits;
  uint32 Reg;
} MACA_OPTIONS_T;
__no_init extern volatile MACA_OPTIONS_T _MACA_OPTIONS @(MBAR_MACA + 0x180);    /* MACA Options W 32 */
#define MACA_OPTIONS		          _MACA_OPTIONS.Reg
#define MACA_OPTIONS_POLL                 _MACA_OPTIONS.Bits.POLL
#define MACA_OPTIONS_PLL_TX	          _MACA_OPTIONS.Bits.PLL_TX
#define MACA_OPTIONS_PLL_IGNORE		  _MACA_OPTIONS.Bits.PLL_IGNORE
#define MACA_OPTIONS_SEED_KEY		  _MACA_OPTIONS.Bits.SEED_KEY




/* ------------------------- UART Module (UART1)   -------------------------- */
/*    Base Adrress = MBAR_UART1 = 0x8000_5000                                 */
typedef union
{
  struct
  {
    uint32 TXE        :1;
    uint32 RXE        :1;
    uint32 PEN        :1;
    uint32 EP         :1;
    uint32 ST2        :1;
    uint32 SB         :1;
    uint32 CONTX      :1;
    uint32 TX_OEN_B   :1;
    uint32            :1;
    uint32 SEL        :1;
    uint32 XTIM       :1;
    uint32 FCP        :1;
    uint32 FCE        :1;
    uint32 MTXR       :1;
    uint32 MRXR       :1;
    uint32 TST        :1;
    uint32            :16;
  } Bits;
  uint32 Reg;
} UARTx_UCON_T;
__no_init extern volatile UARTx_UCON_T _UART1_UCON @(MBAR_UART1 + 0x00);        /* UART Control R/W 8 */
#define UART1_UCON		          _UART1_UCON.Reg
#define UART1_UCON_TXE		          _UART1_UCON.Bits.TXE
#define UART1_UCON_RXE		          _UART1_UCON.Bits.RXE
#define UART1_UCON_PEN		          _UART1_UCON.Bits.PEN
#define UART1_UCON_EP		          _UART1_UCON.Bits.EP
#define UART1_UCON_ST2		          _UART1_UCON.Bits.ST2
#define UART1_UCON_SB		          _UART1_UCON.Bits.SB
#define UART1_UCON_CONTX	          _UART1_UCON.Bits.CONTX
#define UART1_UCON_TX_OEN_B	          _UART1_UCON.Bits.TX_OEN_B
#define UART1_UCON_SEL		          _UART1_UCON.Bits.SEL
#define UART1_UCON_XTIM		          _UART1_UCON.Bits.XTIM
#define UART1_UCON_FCP		          _UART1_UCON.Bits.FCP
#define UART1_UCON_FCE		          _UART1_UCON.Bits.FCE
#define UART1_UCON_MTXR		          _UART1_UCON.Bits.MTXR
#define UART1_UCON_MRXR		          _UART1_UCON.Bits.MRXR
#define UART1_UCON_TST		          _UART1_UCON.Bits.TST


typedef union
{
  struct
  {
    uint32 SE         :1;
    uint32 PE         :1;
    uint32 FE         :1;
    uint32 TOE        :1;
    uint32 ROE        :1;
    uint32 RUE        :1;
    uint32 RXRDY      :1;
    uint32 TXRDY      :1;
    uint32            :24;
  } Bits;
  uint32 Reg;
} UARTx_USTAT_T;
__no_init extern volatile UARTx_USTAT_T _UART1_USTAT @(MBAR_UART1 + 0x04);      /* UART Status R 8 */
#define UART1_USTAT		          _UART1_USTAT.Reg
#define UART1_USTAT_SE		          _UART1_USTAT.Bits.SE
#define UART1_USTAT_PE		          _UART1_USTAT.Bits.PE
#define UART1_USTAT_FE		          _UART1_USTAT.Bits.FE
#define UART1_USTAT_TOE		          _UART1_USTAT.Bits.TOE
#define UART1_USTAT_ROE		          _UART1_USTAT.Bits.ROE
#define UART1_USTAT_RUE		          _UART1_USTAT.Bits.RUE
#define UART1_USTAT_RXRDY	          _UART1_USTAT.Bits.RXRDY
#define UART1_USTAT_TXRDY	          _UART1_USTAT.Bits.TXRDY


typedef uint32 UARTx_UDATA_T;
__no_init extern volatile UARTx_UDATA_T _UART1_UDATA @(MBAR_UART1 + 0x08);      /* UART Data R/W 8 */
#define UART1_UDATA                     _UART1_UDATA


typedef uint32 UARTx_URXCON_T;
__no_init extern volatile UARTx_URXCON_T _UART1_URXCON @(MBAR_UART1 + 0x0C);    /* UART RxBuffer Control R/W 8 */
#define UART1_URXCON                   _UART1_URXCON


typedef uint32 UARTx_UTXCON_T;
__no_init extern volatile UARTx_UTXCON_T _UART1_UTXCON @(MBAR_UART1 + 0x10);    /* UART TxBuffer Control R/W 8 */
#define UART1_UTXCON                   _UART1_UTXCON


typedef uint32 UARTx_UCTS_T;
__no_init extern volatile UARTx_UCTS_T _UART1_UCTS @(MBAR_UART1 + 0x14);        /* UART CTS Level Control R/W 8 */
#define UART1_UCTS                     _UART1_UCTS


typedef union
{
  struct
  {
    uint32 UBRMOD         :16;
    uint32 UBRINC         :16;
  } Bits;
  uint32 Reg;
} UARTx_UBRCNT_T;
__no_init extern volatile UARTx_UBRCNT_T _UART1_UBRCNT @(MBAR_UART1 + 0x18);    /* UBRINC — Fractional Divider/UBRMOD — Fractional Divider R/W 8 */
#define UART1_UBRCNT		         _UART1_UBRCNT.Reg
#define UART1_UBRCNT_UBRMOD		 _UART1_UBRCNT.Bits.UBRMOD
#define UART1_UBRCNT_UBRINC 	         _UART1_UBRCNT.Bits.UBRINC



/*
0x8000_6000 I2C
+ 0x00 I2C_ADR              I2C Address R/W 8
+ 0x04 I2C_FDR              I2C Frequency Divider R/W 8
+ 0x08 I2C_CR               I2C Control R/W 8
+ 0x0C I2C_SR               I2C Status R/W 8
+ 0x10 I2C_DR               I2C Data R/W 8
+ 0x14 I2C_DFSRR            I2C Digital Filter Sampling Rate R/W 8
+ 0x18 I2C_CKER             I2C Clock Enable Register R/W 8
*/

/* --------------------------- Timer Module (TMR) --------------------------- */
/*    Base Adrress = MBAR_TMR0 = 0x80007000                                   */
typedef uint16 TMR0_COMP1_T;
__no_init extern volatile TMR0_COMP1_T _TMR0_COMP1 @(MBAR_TMR0 + 0x00);         /* TMR Channel 0 Compare Register #1 R/W 16 */
#define TMR0_COMP1                    _TMR0_COMP1
#define TMR0_COMP1_COMPARISON_VALUE   _TMR0_COMP1


typedef uint16 TMR0_COMP2_T;
__no_init extern volatile TMR0_COMP2_T _TMR0_COMP2 @(MBAR_TMR0 + 0x02);         /* TMR Channel 0 Compare Register #2 R/W 16 */
#define TMR0_COMP2                    _TMR0_COMP2
#define TMR0_COMP2_COMPARISON_VALUE   _TMR0_COMP2


typedef uint16 TMR0_CAPT_T;
__no_init extern volatile TMR0_CAPT_T _TMR0_CAPT @(MBAR_TMR0 + 0x04);           /* TMR Channel 0 Capture Register R/W 16 */
#define TMR0_CAPT                     _TMR0_CAPT
#define TMR0_CAPT_CAPTURE_VALUE       _TMR0_CAPT


typedef uint16 TMR0_LOAD_T;
__no_init extern volatile TMR0_LOAD_T _TMR0_LOAD @(MBAR_TMR0 + 0x06);           /* TMR Channel 0 Load Register R/W 16 */
#define TMR0_LOAD                     _TMR0_LOAD
#define TMR0_LOAD_LOAD_VALUE          _TMR0_LOAD


typedef uint16 TMR0_HOLD_T;
__no_init extern volatile TMR0_HOLD_T _TMR0_HOLD @(MBAR_TMR0 + 0x08);           /* TMR Channel 0 Hold Register R/W 16 */
#define TMR0_HOLD                     _TMR0_HOLD
#define TMR0_HOLD_HOLD_VALUE          _TMR0_HOLD


typedef uint16 TMR0_CNTR_T;
__no_init extern volatile TMR0_CNTR_T _TMR0_CNTR @(MBAR_TMR0 + 0x0A);           /* TMR Channel 0 Counter Register R/W 16 */
#define TMR0_CNTR                     _TMR0_CNTR
#define TMR0_CNTR_COUNTER_VALUE       _TMR0_CNTR


__no_init extern volatile TMR_CTRL_T _TMR0_CTRL @(MBAR_TMR0 + 0x0C);           /* TMR Channel 0 Control Register R/W 16 */
#define TMR0_CTRL                       _TMR0_CTRL.Reg
#define TMR0_CTRL_OUTPUT_MODE           _TMR0_CTRL.Bits.OUTPUT_MODE             /* BIT0  | BIT1  | BIT2 */
#define TMR0_CTRL_CO_INIT               _TMR0_CTRL.Bits.CO_INIT                 /* BIT3  */
#define TMR0_CTRL_DIR                   _TMR0_CTRL.Bits.DIR                     /* BIT4  */
#define TMR0_CTRL_LENGTH                _TMR0_CTRL.Bits.LENGTH                  /* BIT5  */
#define TMR0_CTRL_ONCE                  _TMR0_CTRL.Bits.ONCE                    /* BIT6  */
#define TMR0_CTRL_SECONDARY_SOURCE      _TMR0_CTRL.Bits.SECONDARY_SOURCE        /* BIT7  | BIT8 */
#define TMR0_CTRL_PRIMARY_COUNT_SOURCE  _TMR0_CTRL.Bits.PRIMARY_COUNT_SOURCE    /* BIT9  | BIT10 | BIT11 | BIT12 */
#define TMR0_CTRL_COUNT_MODE            _TMR0_CTRL.Bits.COUNT_MODE              /* BIT13 | BIT14 | BIT15 */


__no_init extern volatile TMR_SCTRL_T _TMR0_SCTRL @(MBAR_TMR0 + 0x0E);          /* TMR Channel 0 Status and Control Register R/W 16 */
#define TMR0_SCTRL                      _TMR0_SCTRL.Reg
#define TMR0_SCTRL_OEN                  _TMR0_SCTRL.Bits.OEN                    /* BIT0 */
#define TMR0_SCTRL_OPS                  _TMR0_SCTRL.Bits.OPS                    /* BIT1 */
#define TMR0_SCTRL_FORCE                _TMR0_SCTRL.Bits.FORCE                  /* BIT2 */
#define TMR0_SCTRL_VAL                  _TMR0_SCTRL.Bits.VAL                    /* BIT3 */
#define TMR0_SCTRL_EEOF                 _TMR0_SCTRL.Bits.EEOF                   /* BIT4 */
#define TMR0_SCTRL_MSTR                 _TMR0_SCTRL.Bits.MSTR                   /* BIT5 */
#define TMR0_SCTRL_CAPTURE_MODE         _TMR0_SCTRL.Bits.CAPTURE_MODE           /* BIT6 | BIT7 */
#define TMR0_SCTRL_INPUT                _TMR0_SCTRL.Bits.INPUT                  /* BIT8 */
#define TMR0_SCTRL_IPS                  _TMR0_SCTRL.Bits.IPS                    /* BIT9 */
#define TMR0_SCTRL_IEFIE                _TMR0_SCTRL.Bits.IEFIE                  /* BIT10 */
#define TMR0_SCTRL_IEF                  _TMR0_SCTRL.Bits.IEF                    /* BIT11 */
#define TMR0_SCTRL_TOFIE                _TMR0_SCTRL.Bits.TOFIE                  /* BIT12 */
#define TMR0_SCTRL_TOF                  _TMR0_SCTRL.Bits.TOF                    /* BIT13 */
#define TMR0_SCTRL_TCFIE                _TMR0_SCTRL.Bits.TCFIE                  /* BIT14 */
#define TMR0_SCTRL_TCF                  _TMR0_SCTRL.Bits.TCF                    /* BIT15 */


typedef uint16 TMR0_CMPLD1_T;
__no_init extern volatile TMR0_CMPLD1_T _TMR0_CMPLD1 @(MBAR_TMR0 + 0x10);       /* TMR Channel 0 Comparator Load Register 1 R/W 16 */
#define TMR0_CMPLD1                     _TMR0_CMPLD1
#define TMR0_CMPLD1_COMPARATOR_LOAD_1   _TMR0_CMPLD1


typedef uint16 TMR0_CMPLD2_T;
__no_init extern volatile TMR0_CMPLD2_T _TMR0_CMPLD2 @(MBAR_TMR0 + 0x12);       /* TMR Channel 0 Comparator Load Register 2 R/W 16 */
#define TMR0_CMPLD2                     _TMR0_CMPLD2
#define TMR0_CMPLD2_COMPARATOR_LOAD_2   _TMR0_CMPLD2


__no_init extern volatile TMR_CSCTRL_T _TMR0_CSCTRL @(MBAR_TMR0 + 0x14);        /* TMR Channel 0 Comparator Status and Control Register R/W 16 */
#define TMR0_CSCTRL                   _TMR0_CSCTRL.Reg
#define TMR0_CSCTRL_CL1               _TMR0_CSCTRL.Bits.CL1                     /* BIT0 | BIT1 */
#define TMR0_CSCTRL_CL2               _TMR0_CSCTRL.Bits.CL2                     /* BIT2 | BIT3 */
#define TMR0_CSCTRL_TCF1              _TMR0_CSCTRL.Bits.TCF1                    /* BIT4 */
#define TMR0_CSCTRL_TCF2              _TMR0_CSCTRL.Bits.TCF2                    /* BIT5 */
#define TMR0_CSCTRL_TCF1EN            _TMR0_CSCTRL.Bits.TCF1EN                  /* BIT6 */
#define TMR0_CSCTRL_TCF2EN            _TMR0_CSCTRL.Bits.TCF2EN                  /* BIT7 */
#define TMR0_CSCTRL_FILT_EN           _TMR0_CSCTRL.Bits.FILT_EN                 /* BIT13 */
#define TMR0_CSCTRL_DBG_EN            _TMR0_CSCTRL.Bits.DBG_EN                  /* BIT14 | BIT15 */


typedef union
{
  struct
  {
    uint16 ENBL0            :1;
    uint16 ENBL1            :1;
    uint16 ENBL2            :1;
    uint16 ENBL3            :1;
    uint16                  :12;
  } Bits;
  struct
  {
    uint16 ENBL             :4;
    uint16                  :12;
  } GrpBits;
  uint16 Reg;
} TMR0_ENBL_T;
__no_init extern volatile TMR0_ENBL_T _TMR0_ENBL @(MBAR_TMR0 + 0x1E);           /* TMR Channel Enable Register R/W 16 */
#define TMR0_ENBL                 _TMR0_ENBL.Reg
#define TMR0_ENBL_0               _TMR0_ENBL.Bits.ENBL0                         /* BIT0 */
#define TMR0_ENBL_1               _TMR0_ENBL.Bits.ENBL1                         /* BIT1 */
#define TMR0_ENBL_2               _TMR0_ENBL.Bits.ENBL2                         /* BIT2 */
#define TMR0_ENBL_3               _TMR0_ENBL.Bits.ENBL3                         /* BIT3 */
#define TMR0_ENBL_GRP             _TMR0_ENBL.GrpBits.ENBL                       /* BIT0 | BIT1 | BIT2 | BIT3 */


/*    Base Adrress = MBAR_TMR1 = 0x80007020                                   */
typedef uint16 TMR1_COMP1_T;
__no_init extern volatile TMR1_COMP1_T _TMR1_COMP1 @(MBAR_TMR1 + 0x00);         /* TMR Channel 1 Compare Register #1 R/W 16 */
#define TMR1_COMP1                    _TMR1_COMP1
#define TMR1_COMP1_COMPARISON_VALUE   _TMR1_COMP1


typedef uint16 TMR1_COMP2_T;
__no_init extern volatile TMR1_COMP2_T _TMR1_COMP2 @(MBAR_TMR1 + 0x02);         /* TMR Channel 1 Compare Register #2 R/W 16 */
#define TMR1_COMP2                    _TMR1_COMP2
#define TMR1_COMP2_COMPARISON_VALUE   _TMR1_COMP2


typedef uint16 TMR1_CAPT_T;
__no_init extern volatile TMR1_CAPT_T _TMR1_CAPT @(MBAR_TMR1 + 0x04);           /* TMR Channel 1 Capture Register R/W 16 */
#define TMR1_CAPT                     _TMR1_CAPT
#define TMR1_CAPT_CAPTURE_VALUE       _TMR1_CAPT


typedef uint16 TMR1_LOAD_T;
__no_init extern volatile TMR1_LOAD_T _TMR1_LOAD @(MBAR_TMR1 + 0x06);           /* TMR Channel 1 Load Register R/W 16 */
#define TMR1_LOAD                     _TMR1_LOAD
#define TMR1_LOAD_LOAD_VALUE          _TMR1_LOAD


typedef uint16 TMR1_HOLD_T;
__no_init extern volatile TMR1_HOLD_T _TMR1_HOLD @(MBAR_TMR1 + 0x08);           /* TMR Channel 1 Hold Register R/W 16 */
#define TMR1_HOLD                     _TMR1_HOLD
#define TMR1_HOLD_HOLD_VALUE          _TMR1_HOLD


typedef uint16 TMR1_CNTR_T;
__no_init extern volatile TMR1_CNTR_T _TMR1_CNTR @(MBAR_TMR1 + 0x0A);           /* TMR Channel 1 Counter Register R/W 16 */
#define TMR1_CNTR                     _TMR1_CNTR
#define TMR1_CNTR_COUNTER_VALUE       _TMR1_CNTR


__no_init extern volatile TMR_CTRL_T _TMR1_CTRL @(MBAR_TMR1 + 0x0C);           /* TMR Channel 1 Control Register R/W 16 */
#define TMR1_CTRL                       _TMR1_CTRL.Reg
#define TMR1_CTRL_OUTPUT_MODE           _TMR1_CTRL.Bits.OUTPUT_MODE             /* BIT0  | BIT1  | BIT2 */
#define TMR1_CTRL_CO_INIT               _TMR1_CTRL.Bits.CO_INIT                 /* BIT3  */
#define TMR1_CTRL_DIR                   _TMR1_CTRL.Bits.DIR                     /* BIT4  */
#define TMR1_CTRL_LENGTH                _TMR1_CTRL.Bits.LENGTH                  /* BIT5  */
#define TMR1_CTRL_ONCE                  _TMR1_CTRL.Bits.ONCE                    /* BIT6  */
#define TMR1_CTRL_SECONDARY_SOURCE      _TMR1_CTRL.Bits.SECONDARY_SOURCE        /* BIT7  | BIT8 */
#define TMR1_CTRL_PRIMARY_COUNT_SOURCE  _TMR1_CTRL.Bits.PRIMARY_COUNT_SOURCE    /* BIT9  | BIT10 | BIT11 | BIT12 */
#define TMR1_CTRL_COUNT_MODE            _TMR1_CTRL.Bits.COUNT_MODE              /* BIT13 | BIT14 | BIT15 */


__no_init extern volatile TMR_SCTRL_T _TMR1_SCTRL @(MBAR_TMR1 + 0x0E);          /* TMR Channel 1 Status and Control Register R/W 16 */
#define TMR1_SCTRL                      _TMR1_SCTRL.Reg
#define TMR1_SCTRL_OEN                  _TMR1_SCTRL.Bits.OEN                    /* BIT0 */
#define TMR1_SCTRL_OPS                  _TMR1_SCTRL.Bits.OPS                    /* BIT1 */
#define TMR1_SCTRL_FORCE                _TMR1_SCTRL.Bits.FORCE                  /* BIT2 */
#define TMR1_SCTRL_VAL                  _TMR1_SCTRL.Bits.VAL                    /* BIT3 */
#define TMR1_SCTRL_EEOF                 _TMR1_SCTRL.Bits.EEOF                   /* BIT4 */
#define TMR1_SCTRL_MSTR                 _TMR1_SCTRL.Bits.MSTR                   /* BIT5 */
#define TMR1_SCTRL_CAPTURE_MODE         _TMR1_SCTRL.Bits.CAPTURE_MODE           /* BIT6 | BIT7 */
#define TMR1_SCTRL_INPUT                _TMR1_SCTRL.Bits.INPUT                  /* BIT8 */
#define TMR1_SCTRL_IPS                  _TMR1_SCTRL.Bits.IPS                    /* BIT9 */
#define TMR1_SCTRL_IEFIE                _TMR1_SCTRL.Bits.IEFIE                  /* BIT10 */
#define TMR1_SCTRL_IEF                  _TMR1_SCTRL.Bits.IEF                    /* BIT11 */
#define TMR1_SCTRL_TOFIE                _TMR1_SCTRL.Bits.TOFIE                  /* BIT12 */
#define TMR1_SCTRL_TOF                  _TMR1_SCTRL.Bits.TOF                    /* BIT13 */
#define TMR1_SCTRL_TCFIE                _TMR1_SCTRL.Bits.TCFIE                  /* BIT14 */
#define TMR1_SCTRL_TCF                  _TMR1_SCTRL.Bits.TCF                    /* BIT15 */


typedef uint16 TMR1_CMPLD1_T;
__no_init extern volatile TMR1_CMPLD1_T _TMR1_CMPLD1 @(MBAR_TMR1 + 0x10);       /* TMR Channel 1 Comparator Load Register 1 R/W 16 */
#define TMR1_CMPLD1                     _TMR1_CMPLD1
#define TMR1_CMPLD1_COMPARATOR_LOAD_1   _TMR1_CMPLD1


typedef uint16 TMR1_CMPLD2_T;
__no_init extern volatile TMR1_CMPLD2_T _TMR1_CMPLD2 @(MBAR_TMR1 + 0x12);       /* TMR Channel 1 Comparator Load Register 2 R/W 16 */
#define TMR1_CMPLD2                     _TMR1_CMPLD2
#define TMR1_CMPLD2_COMPARATOR_LOAD_2   _TMR1_CMPLD2


__no_init extern volatile TMR_CSCTRL_T _TMR1_CSCTRL @(MBAR_TMR1 + 0x14);        /* TMR Channel 1 Comparator Status and Control Register R/W 16 */
#define TMR1_CSCTRL                   _TMR1_CSCTRL.Reg
#define TMR1_CSCTRL_CL1               _TMR1_CSCTRL.Bits.CL1                     /* BIT0 | BIT1 */
#define TMR1_CSCTRL_CL2               _TMR1_CSCTRL.Bits.CL2                     /* BIT2 | BIT3 */
#define TMR1_CSCTRL_TCF1              _TMR1_CSCTRL.Bits.TCF1                    /* BIT4 */
#define TMR1_CSCTRL_TCF2              _TMR1_CSCTRL.Bits.TCF2                    /* BIT5 */
#define TMR1_CSCTRL_TCF1EN            _TMR1_CSCTRL.Bits.TCF1EN                  /* BIT6 */
#define TMR1_CSCTRL_TCF2EN            _TMR1_CSCTRL.Bits.TCF2EN                  /* BIT7 */
#define TMR1_CSCTRL_FILT_EN           _TMR1_CSCTRL.Bits.FILT_EN                 /* BIT13 */
#define TMR1_CSCTRL_DBG_EN            _TMR1_CSCTRL.Bits.DBG_EN                  /* BIT14 | BIT15 */


/*    Base Adrress = MBAR_TMR2 = 0x80007040                                   */
typedef uint16 TMR2_COMP1_T;
__no_init extern volatile TMR2_COMP1_T _TMR2_COMP1 @(MBAR_TMR2 + 0x00);         /* TMR Channel 2 Compare Register #1 R/W 16 */
#define TMR2_COMP1                    _TMR2_COMP1
#define TMR2_COMP1_COMPARISON_VALUE   _TMR2_COMP1


typedef uint16 TMR2_COMP2_T;
__no_init extern volatile TMR2_COMP2_T _TMR2_COMP2 @(MBAR_TMR2 + 0x02);         /* TMR Channel 2 Compare Register #2 R/W 16 */
#define TMR2_COMP2                    _TMR2_COMP2
#define TMR2_COMP2_COMPARISON_VALUE   _TMR2_COMP2


typedef uint16 TMR2_CAPT_T;
__no_init extern volatile TMR2_CAPT_T _TMR2_CAPT @(MBAR_TMR2 + 0x04);           /* TMR Channel 2 Capture Register R/W 16 */
#define TMR2_CAPT                     _TMR2_CAPT
#define TMR2_CAPT_CAPTURE_VALUE       _TMR2_CAPT


typedef uint16 TMR2_LOAD_T;
__no_init extern volatile TMR2_LOAD_T _TMR2_LOAD @(MBAR_TMR2 + 0x06);           /* TMR Channel 2 Load Register R/W 16 */
#define TMR2_LOAD                     _TMR2_LOAD
#define TMR2_LOAD_LOAD_VALUE          _TMR2_LOAD


typedef uint16 TMR2_HOLD_T;
__no_init extern volatile TMR2_HOLD_T _TMR2_HOLD @(MBAR_TMR2 + 0x08);           /* TMR Channel 2 Hold Register R/W 16 */
#define TMR2_HOLD                     _TMR2_HOLD
#define TMR2_HOLD_HOLD_VALUE          _TMR2_HOLD


typedef uint16 TMR2_CNTR_T;
__no_init extern volatile TMR2_CNTR_T _TMR2_CNTR @(MBAR_TMR2 + 0x0A);           /* TMR Channel 2 Counter Register R/W 16 */
#define TMR2_CNTR                     _TMR2_CNTR
#define TMR2_CNTR_COUNTER_VALUE       _TMR2_CNTR


__no_init extern volatile TMR_CTRL_T _TMR2_CTRL @(MBAR_TMR2 + 0x0C);           /* TMR Channel 2 Control Register R/W 16 */
#define TMR2_CTRL                       _TMR2_CTRL.Reg
#define TMR2_CTRL_OUTPUT_MODE           _TMR2_CTRL.Bits.OUTPUT_MODE             /* BIT0  | BIT1  | BIT2 */
#define TMR2_CTRL_CO_INIT               _TMR2_CTRL.Bits.CO_INIT                 /* BIT3  */
#define TMR2_CTRL_DIR                   _TMR2_CTRL.Bits.DIR                     /* BIT4  */
#define TMR2_CTRL_LENGTH                _TMR2_CTRL.Bits.LENGTH                  /* BIT5  */
#define TMR2_CTRL_ONCE                  _TMR2_CTRL.Bits.ONCE                    /* BIT6  */
#define TMR2_CTRL_SECONDARY_SOURCE      _TMR2_CTRL.Bits.SECONDARY_SOURCE        /* BIT7  | BIT8 */
#define TMR2_CTRL_PRIMARY_COUNT_SOURCE  _TMR2_CTRL.Bits.PRIMARY_COUNT_SOURCE    /* BIT9  | BIT10 | BIT11 | BIT12 */
#define TMR2_CTRL_COUNT_MODE            _TMR2_CTRL.Bits.COUNT_MODE              /* BIT13 | BIT14 | BIT15 */


__no_init extern volatile TMR_SCTRL_T _TMR2_SCTRL @(MBAR_TMR2 + 0x0E);          /* TMR Channel 2 Status and Control Register R/W 16 */
#define TMR2_SCTRL                      _TMR2_SCTRL.Reg
#define TMR2_SCTRL_OEN                  _TMR2_SCTRL.Bits.OEN                    /* BIT0 */
#define TMR2_SCTRL_OPS                  _TMR2_SCTRL.Bits.OPS                    /* BIT1 */
#define TMR2_SCTRL_FORCE                _TMR2_SCTRL.Bits.FORCE                  /* BIT2 */
#define TMR2_SCTRL_VAL                  _TMR2_SCTRL.Bits.VAL                    /* BIT3 */
#define TMR2_SCTRL_EEOF                 _TMR2_SCTRL.Bits.EEOF                   /* BIT4 */
#define TMR2_SCTRL_MSTR                 _TMR2_SCTRL.Bits.MSTR                   /* BIT5 */
#define TMR2_SCTRL_CAPTURE_MODE         _TMR2_SCTRL.Bits.CAPTURE_MODE           /* BIT6 | BIT7 */
#define TMR2_SCTRL_INPUT                _TMR2_SCTRL.Bits.INPUT                  /* BIT8 */
#define TMR2_SCTRL_IPS                  _TMR2_SCTRL.Bits.IPS                    /* BIT9 */
#define TMR2_SCTRL_IEFIE                _TMR2_SCTRL.Bits.IEFIE                  /* BIT10 */
#define TMR2_SCTRL_IEF                  _TMR2_SCTRL.Bits.IEF                    /* BIT11 */
#define TMR2_SCTRL_TOFIE                _TMR2_SCTRL.Bits.TOFIE                  /* BIT12 */
#define TMR2_SCTRL_TOF                  _TMR2_SCTRL.Bits.TOF                    /* BIT13 */
#define TMR2_SCTRL_TCFIE                _TMR2_SCTRL.Bits.TCFIE                  /* BIT14 */
#define TMR2_SCTRL_TCF                  _TMR2_SCTRL.Bits.TCF                    /* BIT15 */


typedef uint16 TMR2_CMPLD1_T;
__no_init extern volatile TMR2_CMPLD1_T _TMR2_CMPLD1 @(MBAR_TMR2 + 0x10);       /* TMR Channel 2 Comparator Load Register 1 R/W 16 */
#define TMR2_CMPLD1                     _TMR2_CMPLD1
#define TMR2_CMPLD1_COMPARATOR_LOAD_1   _TMR2_CMPLD1


typedef uint16 TMR2_CMPLD2_T;
__no_init extern volatile TMR2_CMPLD2_T _TMR2_CMPLD2 @(MBAR_TMR2 + 0x12);       /* TMR Channel 2 Comparator Load Register 2 R/W 16 */
#define TMR2_CMPLD2                     _TMR2_CMPLD2
#define TMR2_CMPLD2_COMPARATOR_LOAD_2   _TMR2_CMPLD2


__no_init extern volatile TMR_CSCTRL_T _TMR2_CSCTRL @(MBAR_TMR2 + 0x14);        /* TMR Channel 2 Comparator Status and Control Register R/W 16 */
#define TMR2_CSCTRL                   _TMR2_CSCTRL.Reg
#define TMR2_CSCTRL_CL1               _TMR2_CSCTRL.Bits.CL1                     /* BIT0 | BIT1 */
#define TMR2_CSCTRL_CL2               _TMR2_CSCTRL.Bits.CL2                     /* BIT2 | BIT3 */
#define TMR2_CSCTRL_TCF1              _TMR2_CSCTRL.Bits.TCF1                    /* BIT4 */
#define TMR2_CSCTRL_TCF2              _TMR2_CSCTRL.Bits.TCF2                    /* BIT5 */
#define TMR2_CSCTRL_TCF1EN            _TMR2_CSCTRL.Bits.TCF1EN                  /* BIT6 */
#define TMR2_CSCTRL_TCF2EN            _TMR2_CSCTRL.Bits.TCF2EN                  /* BIT7 */
#define TMR2_CSCTRL_FILT_EN           _TMR2_CSCTRL.Bits.FILT_EN                 /* BIT13 */
#define TMR2_CSCTRL_DBG_EN            _TMR2_CSCTRL.Bits.DBG_EN                  /* BIT14 | BIT15 */


/*    Base Adrress = MBAR_TMR3 = 0x80007060                                   */
typedef uint16 TMR3_COMP1_T;
__no_init extern volatile TMR3_COMP1_T _TMR3_COMP1 @(MBAR_TMR3 + 0x00);         /* TMR Channel 3 Compare Register #1 R/W 16 */
#define TMR3_COMP1                    _TMR3_COMP1
#define TMR3_COMP1_COMPARISON_VALUE   _TMR3_COMP1


typedef uint16 TMR3_COMP2_T;
__no_init extern volatile TMR3_COMP2_T _TMR3_COMP2 @(MBAR_TMR3 + 0x02);         /* TMR Channel 3 Compare Register #2 R/W 16 */
#define TMR3_COMP2                    _TMR3_COMP2
#define TMR3_COMP2_COMPARISON_VALUE   _TMR3_COMP2


typedef uint16 TMR3_CAPT_T;
__no_init extern volatile TMR3_CAPT_T _TMR3_CAPT @(MBAR_TMR3 + 0x04);           /* TMR Channel 3 Capture Register R/W 16 */
#define TMR3_CAPT                     _TMR3_CAPT
#define TMR3_CAPT_CAPTURE_VALUE       _TMR3_CAPT


typedef uint16 TMR3_LOAD_T;
__no_init extern volatile TMR3_LOAD_T _TMR3_LOAD @(MBAR_TMR3 + 0x06);           /* TMR Channel 3 Load Register R/W 16 */
#define TMR3_LOAD                     _TMR3_LOAD
#define TMR3_LOAD_LOAD_VALUE          _TMR3_LOAD


typedef uint16 TMR3_HOLD_T;
__no_init extern volatile TMR3_HOLD_T _TMR3_HOLD @(MBAR_TMR3 + 0x08);           /* TMR Channel 3 Hold Register R/W 16 */
#define TMR3_HOLD                     _TMR3_HOLD
#define TMR3_HOLD_HOLD_VALUE          _TMR3_HOLD


typedef uint16 TMR3_CNTR_T;
__no_init extern volatile TMR3_CNTR_T _TMR3_CNTR @(MBAR_TMR3 + 0x0A);           /* TMR Channel 3 Counter Register R/W 16 */
#define TMR3_CNTR                     _TMR3_CNTR
#define TMR3_CNTR_COUNTER_VALUE       _TMR3_CNTR


__no_init extern volatile TMR_CTRL_T _TMR3_CTRL @(MBAR_TMR3 + 0x0C);           /* TMR Channel 3 Control Register R/W 16 */
#define TMR3_CTRL                       _TMR3_CTRL.Reg
#define TMR3_CTRL_OUTPUT_MODE           _TMR3_CTRL.Bits.OUTPUT_MODE             /* BIT0  | BIT1  | BIT2 */
#define TMR3_CTRL_CO_INIT               _TMR3_CTRL.Bits.CO_INIT                 /* BIT3  */
#define TMR3_CTRL_DIR                   _TMR3_CTRL.Bits.DIR                     /* BIT4  */
#define TMR3_CTRL_LENGTH                _TMR3_CTRL.Bits.LENGTH                  /* BIT5  */
#define TMR3_CTRL_ONCE                  _TMR3_CTRL.Bits.ONCE                    /* BIT6  */
#define TMR3_CTRL_SECONDARY_SOURCE      _TMR3_CTRL.Bits.SECONDARY_SOURCE        /* BIT7  | BIT8 */
#define TMR3_CTRL_PRIMARY_COUNT_SOURCE  _TMR3_CTRL.Bits.PRIMARY_COUNT_SOURCE    /* BIT9  | BIT10 | BIT11 | BIT12 */
#define TMR3_CTRL_COUNT_MODE            _TMR3_CTRL.Bits.COUNT_MODE              /* BIT13 | BIT14 | BIT15 */


__no_init extern volatile TMR_SCTRL_T _TMR3_SCTRL @(MBAR_TMR3 + 0x0E);          /* TMR Channel 3 Status and Control Register R/W 16 */
#define TMR3_SCTRL                      _TMR3_SCTRL.Reg
#define TMR3_SCTRL_OEN                  _TMR3_SCTRL.Bits.OEN                    /* BIT0 */
#define TMR3_SCTRL_OPS                  _TMR3_SCTRL.Bits.OPS                    /* BIT1 */
#define TMR3_SCTRL_FORCE                _TMR3_SCTRL.Bits.FORCE                  /* BIT2 */
#define TMR3_SCTRL_VAL                  _TMR3_SCTRL.Bits.VAL                    /* BIT3 */
#define TMR3_SCTRL_EEOF                 _TMR3_SCTRL.Bits.EEOF                   /* BIT4 */
#define TMR3_SCTRL_MSTR                 _TMR3_SCTRL.Bits.MSTR                   /* BIT5 */
#define TMR3_SCTRL_CAPTURE_MODE         _TMR3_SCTRL.Bits.CAPTURE_MODE           /* BIT6 | BIT7 */
#define TMR3_SCTRL_INPUT                _TMR3_SCTRL.Bits.INPUT                  /* BIT8 */
#define TMR3_SCTRL_IPS                  _TMR3_SCTRL.Bits.IPS                    /* BIT9 */
#define TMR3_SCTRL_IEFIE                _TMR3_SCTRL.Bits.IEFIE                  /* BIT10 */
#define TMR3_SCTRL_IEF                  _TMR3_SCTRL.Bits.IEF                    /* BIT11 */
#define TMR3_SCTRL_TOFIE                _TMR3_SCTRL.Bits.TOFIE                  /* BIT12 */
#define TMR3_SCTRL_TOF                  _TMR3_SCTRL.Bits.TOF                    /* BIT13 */
#define TMR3_SCTRL_TCFIE                _TMR3_SCTRL.Bits.TCFIE                  /* BIT14 */
#define TMR3_SCTRL_TCF                  _TMR3_SCTRL.Bits.TCF                    /* BIT15 */


typedef uint16 TMR3_CMPLD1_T;
__no_init extern volatile TMR3_CMPLD1_T _TMR3_CMPLD1 @(MBAR_TMR3 + 0x10);       /* TMR Channel 3 Comparator Load Register 1 R/W 16 */
#define TMR3_CMPLD1                     _TMR3_CMPLD1
#define TMR3_CMPLD1_COMPARATOR_LOAD_1   _TMR3_CMPLD1


typedef uint16 TMR3_CMPLD2_T;
__no_init extern volatile TMR3_CMPLD2_T _TMR3_CMPLD2 @(MBAR_TMR3 + 0x12);       /* TMR Channel 3 Comparator Load Register 2 R/W 16 */
#define TMR3_CMPLD2                     _TMR3_CMPLD2
#define TMR3_CMPLD2_COMPARATOR_LOAD_2   _TMR3_CMPLD2


__no_init extern volatile TMR_CSCTRL_T _TMR3_CSCTRL @(MBAR_TMR3 + 0x14);        /* TMR Channel 3 Comparator Status and Control Register R/W 16 */
#define TMR3_CSCTRL                   _TMR3_CSCTRL.Reg
#define TMR3_CSCTRL_CL1               _TMR3_CSCTRL.Bits.CL1                     /* BIT0 | BIT1 */
#define TMR3_CSCTRL_CL2               _TMR3_CSCTRL.Bits.CL2                     /* BIT2 | BIT3 */
#define TMR3_CSCTRL_TCF1              _TMR3_CSCTRL.Bits.TCF1                    /* BIT4 */
#define TMR3_CSCTRL_TCF2              _TMR3_CSCTRL.Bits.TCF2                    /* BIT5 */
#define TMR3_CSCTRL_TCF1EN            _TMR3_CSCTRL.Bits.TCF1EN                  /* BIT6 */
#define TMR3_CSCTRL_TCF2EN            _TMR3_CSCTRL.Bits.TCF2EN                  /* BIT7 */
#define TMR3_CSCTRL_FILT_EN           _TMR3_CSCTRL.Bits.FILT_EN                 /* BIT13 */
#define TMR3_CSCTRL_DBG_EN            _TMR3_CSCTRL.Bits.DBG_EN                  /* BIT14 | BIT15 */


/* -------------------- Advanced Security Module (ASM) ---------------------- */
/*    Base Adrress = MBAR_ASM = 0x8000_8000                                   */
typedef uint32 ASM_KEY_T;
__no_init extern volatile ASM_KEY_T _ASM_KEY0 @(MBAR_ASM + 0x00);               /* 128-BIT ENCRYPTION KEY (1 of 4) R/W 32 */
#define ASM_KEY0                    _ASM_KEY0

__no_init extern volatile ASM_KEY_T _ASM_KEY1 @(MBAR_ASM + 0x04);               /* 128-BIT ENCRYPTION KEY (2 of 4) R/W 32 */
#define ASM_KEY1                    _ASM_KEY1

__no_init extern volatile ASM_KEY_T _ASM_KEY2 @(MBAR_ASM + 0x08);               /* 128-BIT ENCRYPTION KEY (3 of 4) R/W 32 */
#define ASM_KEY2                    _ASM_KEY2

__no_init extern volatile ASM_KEY_T _ASM_KEY3 @(MBAR_ASM + 0x0C);               /* 128-BIT ENCRYPTION KEY (4 of 4) R/W 32 */
#define ASM_KEY3                    _ASM_KEY3


typedef uint32 ASM_DATA_T;
__no_init extern volatile ASM_DATA_T _ASM_DATA0 @(MBAR_ASM + 0x10);             /* 128-BIT DATA Register (1 of 4) R/W 32 */
#define ASM_DATA0                    _ASM_DATA0

__no_init extern volatile ASM_DATA_T _ASM_DATA1 @(MBAR_ASM + 0x14);             /* 128-BIT DATA Register (2 of 4) R/W 32 */
#define ASM_DATA1                    _ASM_DATA1

__no_init extern volatile ASM_DATA_T _ASM_DATA2 @(MBAR_ASM + 0x18);             /* 128-BIT DATA Register (3 of 4) R/W 32 */
#define ASM_DATA2                    _ASM_DATA2

__no_init extern volatile ASM_DATA_T _ASM_DATA3 @(MBAR_ASM + 0x1C);             /* 128-BIT DATA Register (4 of 4) R/W 32 */
#define ASM_DATA3                    _ASM_DATA3


typedef uint32 ASM_CTR_T;
__no_init extern volatile ASM_CTR_T _ASM_CTR0 @(MBAR_ASM + 0x20);               /* 128-BIT COUNTER Register (1 of 4) R/W 32 */
#define ASM_CTR0                    _ASM_CTR0

__no_init extern volatile ASM_CTR_T _ASM_CTR1 @(MBAR_ASM + 0x24);               /* 128-BIT COUNTER Register (2 of 4) R/W 32 */
#define ASM_CTR1                    _ASM_CTR1

__no_init extern volatile ASM_CTR_T _ASM_CTR2 @(MBAR_ASM + 0x28);               /* 128-BIT COUNTER Register (3 of 4) R/W 32 */
#define ASM_CTR2                    _ASM_CTR2

__no_init extern volatile ASM_CTR_T _ASM_CTR3 @(MBAR_ASM + 0x2C);               /* 128-BIT COUNTER Register (4 of 4) R/W 32 */
#define ASM_CTR3                    _ASM_CTR3


typedef uint32 ASM_CTR_RESULT_T;
__no_init extern volatile ASM_CTR_RESULT_T _ASM_CTR_RESULT0 @(MBAR_ASM + 0x30); /* 128-BIT COUNTER RESULT Register (1 of 4) R 32 */
#define ASM_CTR_RESULT0                    _ASM_CTR_RESULT0

__no_init extern volatile ASM_CTR_RESULT_T _ASM_CTR_RESULT1 @(MBAR_ASM + 0x34); /* 128-BIT COUNTER RESULT Register (2 of 4) R 32 */
#define ASM_CTR_RESULT1                    _ASM_CTR_RESULT1

__no_init extern volatile ASM_CTR_RESULT_T _ASM_CTR_RESULT2 @(MBAR_ASM + 0x38); /* 128-BIT COUNTER RESULT Register (3 of 4) R 32 */
#define ASM_CTR_RESULT2                    _ASM_CTR_RESULT2

__no_init extern volatile ASM_CTR_RESULT_T _ASM_CTR_RESULT3 @(MBAR_ASM + 0x3C); /* 128-BIT COUNTER RESULT Register (4 of 4) R 32 */
#define ASM_CTR_RESULT3                    _ASM_CTR_RESULT3


typedef uint32 ASM_CBC_RESULT_T;
__no_init extern volatile ASM_CBC_RESULT_T _ASM_CBC_RESULT0 @(MBAR_ASM + 0x40); /* 128-BIT CBC MAC RESULT Register (1 of 4) R 32 */
#define ASM_CBC_RESULT0                    _ASM_CBC_RESULT0

__no_init extern volatile ASM_CBC_RESULT_T _ASM_CBC_RESULT1 @(MBAR_ASM + 0x44); /* 128-BIT CBC MAC RESULT Register (2 of 4) R 32 */
#define ASM_CBC_RESULT1                    _ASM_CBC_RESULT1

__no_init extern volatile ASM_CBC_RESULT_T _ASM_CBC_RESULT2 @(MBAR_ASM + 0x48); /* 128-BIT CBC MAC RESULT Register (3 of 4) R 32 */
#define ASM_CBC_RESULT2                    _ASM_CBC_RESULT2

__no_init extern volatile ASM_CBC_RESULT_T _ASM_CBC_RESULT3 @(MBAR_ASM + 0x4C); /* 128-BIT CBC MAC RESULT Register (4 of 4) R 32 */
#define ASM_CBC_RESULT3                    _ASM_CBC_RESULT3


typedef union
{
  struct
  {
    uint32            : 24;
    uint32 START      : 1;
    uint32 CLEAR      : 1;
    uint32 LOAD_MAC   : 1;
    uint32            : 4;
    uint32 CLEAR_IRQ  : 1;
  } Bits;
  uint32 Reg;
} ASM_CONTROL0_T;
__no_init extern volatile ASM_CONTROL0_T _ASM_CONTROL0 @(MBAR_ASM + 0x50);      /* CONTROL 0 Register (Self Clearing) W 32 */
#define ASM_CONTROL0                  _ASM_CONTROL0.Reg
#define ASM_CONTROL0_START            _ASM_CONTROL0.Bits.START                  /* BIT24 */
#define ASM_CONTROL0_CLEAR            _ASM_CONTROL0.Bits.CLEAR                  /* BIT25 */
#define ASM_CONTROL0_LOAD_MAC         _ASM_CONTROL0.Bits.LOAD_MAC               /* BIT26 */
#define ASM_CONTROL0_CLEAR_IRQ        _ASM_CONTROL0.Bits.CLEAR_IRQ              /* BIT31 */


typedef union
{
  struct
  {
    uint32 ON         : 1;
    uint32 MODE       : 1;
    uint32 BYPASS     : 1;
    uint32            : 21;
    uint32 CBC        : 1;
    uint32 CTR        : 1;
    uint32 SELFTEST   : 1;
    uint32            : 4;
    uint32 MASK_IRQ   : 1;
  } Bits;
  struct
  {
    uint32            : 24;
    uint32 CRYPTMODE  : 2;
    uint32            : 6;
  } Crypt;
  uint32 Reg;
} ASM_CONTROL1_T;
__no_init extern volatile ASM_CONTROL1_T _ASM_CONTROL1 @(MBAR_ASM + 0x54);      /* CONTROL 1 Register R/W 32 */
#define ASM_CONTROL1                  _ASM_CONTROL1.Reg
#define ASM_CONTROL1_ON               _ASM_CONTROL1.Bits.ON                     /* BIT0 */
#define ASM_CONTROL1_MODE             _ASM_CONTROL1.Bits.MODE                   /* BIT1 */
#define ASM_CONTROL1_BYPASS           _ASM_CONTROL1.Bits.BYPASS                 /* BIT2 */
#define ASM_CONTROL1_CBC              _ASM_CONTROL1.Bits.CBC                    /* BIT24 */
#define ASM_CONTROL1_CTR              _ASM_CONTROL1.Bits.CTR                    /* BIT25 */
#define ASM_CONTROL1_SELFTEST         _ASM_CONTROL1.Bits.SELFTEST               /* BIT26 */
#define ASM_CONTROL1_MASK_IRQ         _ASM_CONTROL1.Bits.MASK_IRQ               /* BIT31 */
#define ASM_CONTROL1_CRYPTMODE        _ASM_CONTROL1.Crypt.CRYPTMODE             /* BIT24 | BIT25 */

typedef union
{
  struct
  {
    uint32            : 24;
    uint32 DONE       : 1;
    uint32 TESTPASS   : 1;
    uint32            : 6;
  } Bits;
  uint32 Reg;
} ASM_STATUS_T;
__no_init extern volatile ASM_STATUS_T _ASM_STATUS @(MBAR_ASM + 0x58);          /* STATUS Register R 32 */
#define ASM_STATUS                    _ASM_STATUS.Reg
#define ASM_STATUS_DONE               _ASM_STATUS.Bits.DONE                     /* BIT24 */
#define ASM_STATUS_TESTPASS           _ASM_STATUS.Bits.TESTPASS                 /* BIT25 */


typedef uint32 ASM_MAC_T;
__no_init extern volatile ASM_MAC_T _ASM_MAC0 @(MBAR_ASM + 0x60);               /* 128-BIT CBC MAC Register (1 of 4) R/W 32 */
#define ASM_MAC0                    _ASM_MAC0

__no_init extern volatile ASM_MAC_T _ASM_MAC1 @(MBAR_ASM + 0x64);               /* 128-BIT CBC MAC Register (2 of 4) R/W 32 */
#define ASM_MAC1                    _ASM_MAC1

__no_init extern volatile ASM_MAC_T _ASM_MAC2 @(MBAR_ASM + 0x68);               /* 128-BIT CBC MAC Register (3 of 4) R/W 32 */
#define ASM_MAC2                    _ASM_MAC2

__no_init extern volatile ASM_MAC_T _ASM_MAC3 @(MBAR_ASM + 0x6C);               /* 128-BIT CBC MAC Register (4 of 4) R/W 32 */
#define ASM_MAC3                    _ASM_MAC3



/*
0x8000_9000 Modem Radio Functions

0x8000_9000 Modem Clock Synthesizer (See .pdf Note )
+ 0x00 SYN_ENABLE           Enable and Override Register R/W 32
+ 0x04 Reserved
+ 0x08 SYN_REFDIV           Reference Loop Divider Register R/W 32
+ 0x0C SYN_VCODIV           VCO Loop Divider Register R/W 32
+ 0x10 SYN_VCOLOCK          VCO Lock Register R/W 32
+ 0x14 to + 0x28 Reserved

0x8000_9200 Modem Transceiver Sequence Manager
+ 0x00 to + 0x1FC Reserved

0x8000_9400 Modem Radio Receiver Functions
+ 0x00 to + 0x14 Reserved
+ 0x18 RX_SFD_CTRL          RX SFD Control Register R/W 32
+ 0x1C to + 0x5C Reserved
+ 0x60 RX_AGC_CCA_ED        RX AGC CCA and ED Control Register R/W 32
+ 0x64 RX_AGC_RSSI          RX AGC RSSI Parameters Register R/W 32
+ 0x68 to +0xFC Reserved

0x8000_9600 Modem Radio Transmitter Functions
+ 0x00 to +0x0C Reserved

0x8000_9800 Modem RF Synthesizer
+ 0x00 RFSYN_ENABLE         Enable and Override Register (Reserved) R/W 32
+ 0x04 to +0x08 Reserved
+ 0x0C RFSYN_VCODIV_INTG    VCO Loop Divider Integer R/W 32
+ 0x10 RFSYN_VCODIV_FRAC    VCO Loop Divider Fractional R/W 32
+ 0x14 RFSYN_VCOLOCK        VCO Lock R 32
+ 0x18 to +0x38 Reserved

0x8000_9A00 Modem Tracking Oscillator Controller (TOC)
+ 0x00 to 0x10 Reserved

*/


/* -------------------- Radio Analog Functions ----------------------------- */
/*    Base Adrress = MBAR_RF = 0x8000_A000                                   */
typedef union
{
  struct
  {
    uint32              :13;
    uint32 PA_SWITCH    :1;
    uint32              :18;
  } Bits;
  uint32 Reg;
}PA_CTRL_T;
__no_init extern volatile PA_CTRL_T _PA_CTRL @(MBAR_RF + 0x20);                 /* Transmit Power Amp Control W 32 */
#define PA_CTRL                   _PA_CTRL.Reg
#define PA_CTRL_PA_SWITCH         _PA_CTRL.Bits.PA_SWITCH


typedef union
{
  struct
  {
    uint32              :14;
    uint32 TARGET_PWR   :5;
    uint32              :13;
  } Bits;
  uint32 Reg;
}PA_ADJ_T;
__no_init extern volatile PA_ADJ_T _PA_ADJ @(MBAR_RF + 0x54);                   /* Transmit Power Adjust W 32 */
#define PA_ADJ                   _PA_ADJ.Reg
#define PA_ADJ_TARGET_PWR        _PA_ADJ.Bits.TARGET_PWR


/* ------------------------- UART Module (UART2)   -------------------------- */
/*    Base Adrress = MBAR_UART2 = 0x8000_B000                                 */

__no_init extern volatile UARTx_UCON_T _UART2_UCON @(MBAR_UART2 + 0x00);        /* UART Control R/W 8 */
#define UART2_UCON		          _UART2_UCON.Reg
#define UART2_UCON_TXE		          _UART2_UCON.Bits.TXE
#define UART2_UCON_RXE		          _UART2_UCON.Bits.RXE
#define UART2_UCON_PEN		          _UART2_UCON.Bits.PEN
#define UART2_UCON_EP		          _UART2_UCON.Bits.EP
#define UART2_UCON_ST2		          _UART2_UCON.Bits.ST2
#define UART2_UCON_SB		          _UART2_UCON.Bits.SB
#define UART2_UCON_CONTX	          _UART2_UCON.Bits.CONTX
#define UART2_UCON_TX_OEN_B	          _UART2_UCON.Bits.TX_OEN_B
#define UART2_UCON_SEL		          _UART2_UCON.Bits.SEL
#define UART2_UCON_XTIM		          _UART2_UCON.Bits.XTIM
#define UART2_UCON_FCP		          _UART2_UCON.Bits.FCP
#define UART2_UCON_FCE		          _UART2_UCON.Bits.FCE
#define UART2_UCON_MTXR		          _UART2_UCON.Bits.MTXR
#define UART2_UCON_MRXR		          _UART2_UCON.Bits.MRXR
#define UART2_UCON_TST		          _UART2_UCON.Bits.TST

__no_init extern volatile UARTx_USTAT_T _UART2_USTAT @(MBAR_UART2 + 0x04);      /* UART Status R 8 */
#define UART2_USTAT		          _UART2_USTAT.Reg
#define UART2_USTAT_SE		          _UART2_USTAT.Bits.SE
#define UART2_USTAT_PE		          _UART2_USTAT.Bits.PE
#define UART2_USTAT_FE		          _UART2_USTAT.Bits.FE
#define UART2_USTAT_TOE		          _UART2_USTAT.Bits.TOE
#define UART2_USTAT_ROE		          _UART2_USTAT.Bits.ROE
#define UART2_USTAT_RUE		          _UART2_USTAT.Bits.RUE
#define UART2_USTAT_RXRDY	          _UART2_USTAT.Bits.RXRDY
#define UART2_USTAT_TXRDY	          _UART2_USTAT.Bits.TXRDY

__no_init extern volatile UARTx_UDATA_T _UART2_UDATA @(MBAR_UART2 + 0x08);      /* UART Data R/W 8 */
#define UART2_UDATA                     _UART2_UDATA

__no_init extern volatile UARTx_URXCON_T _UART2_URXCON @(MBAR_UART2 + 0x0C);    /* UART RxBuffer Control R/W 8 */
#define UART2_URXCON                   _UART2_URXCON

__no_init extern volatile UARTx_UTXCON_T _UART2_UTXCON @(MBAR_UART2 + 0x10);    /* UART TxBuffer Control R/W 8 */
#define UART2_UTXCON                   _UART2_UTXCON

__no_init extern volatile UARTx_UCTS_T _UART2_UCTS @(MBAR_UART2 + 0x14);        /* UART CTS Level Control R/W 8 */
#define UART2_UCTS                     _UART2_UCTS

__no_init extern volatile UARTx_UBRCNT_T _UART2_UBRCNT @(MBAR_UART2 + 0x18);    /* UBRINC — Fractional Divider/UBRMOD — Fractional Divider R/W 8 */
#define UART2_UBRCNT		        _UART2_UBRCNT.Reg
#define UART2_UBRCNT_UBRMOD		    _UART2_UBRCNT.Bits.UBRMOD
#define UART2_UBRCNT_UBRINC 	    _UART2_UBRCNT.Bits.UBRINC




/* -------------------- SPI FLASH Module (SPIF) ----------------------------- */
/*    Base Adrress = MBAR_SPIF = 0x8000_C000                                  */
typedef union
{
  struct
  {
    uint32  :32;
  } Bits;
  uint32 Reg;
} SPIF_TX_DATA_T;
__no_init extern volatile SPIF_TX_DATA_T _SPIF_TX_DATA @(MBAR_FLASH + 0x00);    /* SPIF Tx Data R/W 32 */

typedef union
{
  struct
  {
    uint32  :32;
  } Bits;
  uint32 Reg;
} SPIF_RX_DATA_T;
__no_init extern volatile SPIF_RX_DATA_T _SPIF_RX_DATA @(MBAR_FLASH + 0x04);    /* SPIF Rx Data R 32 */

typedef union
{
  struct
  {
    uint32 SPI_DATA_LENGTH  :7;
    uint32 SPI_START        :1;
    uint32 SPI_SCK_COUNT    :8;
    uint32                  :16;
  } Bits;
  uint32 Reg;
}SPIF_CLK_CTRL_T;
__no_init extern volatile SPIF_CLK_CTRL_T _SPIF_CLK_CTRL @(MBAR_FLASH + 0x08);  /* SPIF Clock Control Register R/W 32 */
#define SPIF_CLK_CTRL                  _SPIF_CLK_CTRL.Reg
#define SPIF_CLK_CTRL_SPI_DATA_LENGTH  _SPIF_CLK_CTRL.Bits.SPI_DATA_LENGTH      /* BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 */
#define SPIF_CLK_CTRL_SPI_START        _SPIF_CLK_CTRL.Bits.SPI_START            /* BIT7 */
#define SPIF_CLK_CTRL_SPI_SCK_COUNT    _SPIF_CLK_CTRL.Bits.SPI_SCK_COUNT        /* BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15 */

typedef union
{
  struct
  {
    uint32 SPI_SS_SETUP         :2;
    uint32 SPI_SS_DELAY         :2;
    uint32 SPI_SDO_INACTIVE_ST  :2;
    uint32                      :2;
    uint32 SPI_SCK_POL          :1;
    uint32 SPI_SCK_PHASE        :1;
    uint32 SPI_MISO_PHASE       :1;
    uint32                      :1;
    uint32 SPI_SCK_FREQ         :3;
    uint32                      :1;
    uint32 SPI_MODE             :1;
    uint32 SPI_3WIRE            :1;
  uint32                        :14;
  } Bits;
  uint32 Reg;
}SPIF_SETUP_T;
__no_init extern volatile SPIF_SETUP_T _SPIF_SETUP @(MBAR_FLASH + 0x0C);        /* SPIF Setup Register R/W 32 */
#define SPIF_SETUP                      _SPIF_SETUP.Reg
#define SPIF_SETUP_SPI_SS_SETUP         _SPIF_SETUP.Bits.SPI_SS_SETUP           /* BIT0 | BIT1 */
#define SPIF_SETUP_SPI_SS_DELAY         _SPIF_SETUP.Bits.SPI_SS_DELAY           /* BIT2 | BIT3 */
#define SPIF_SETUP_SPI_SDO_INACTIVE_ST  _SPIF_SETUP.Bits.SPI_SDO_INACTIVE_ST    /* BIT4 | BIT5 */
#define SPIF_SETUP_SPI_SCK_POL          _SPIF_SETUP.Bits.SPI_SCK_POL            /* BIT8 */
#define SPIF_SETUP_SPI_SCK_PHASE        _SPIF_SETUP.Bits.SPI_SCK_PHASE          /* BIT9 */
#define SPIF_SETUP_SPI_MISO_PHASE       _SPIF_SETUP.Bits.SPI_MISO_PHASE         /* BIT10 */
#define SPIF_SETUP_SPI_SCK_FREQ         _SPIF_SETUP.Bits.SPI_SCK_FREQ           /* BIT12 | BIT13 | BIT14 */
#define SPIF_SETUP_SPI_MODE             _SPIF_SETUP.Bits.SPI_MODE               /* BIT16 */
#define SPIF_SETUP_SPI_3WIRE            _SPIF_SETUP.Bits.SPI_3WIRE              /* BIT17 */

typedef union
{
  struct
  {
    uint32 SPI_INT         :1;
    uint32                 :3;
    uint32 SPI_OVERFLOW    :1;
    uint32                 :3;
    uint32 SPI_FIRST_DATA  :1;
    uint32                 :23;
  } Bits;
  uint32 Reg;
}SPIF_STATUS_T;
__no_init extern volatile SPIF_STATUS_T _SPIF_STATUS @(MBAR_FLASH + 0x10);      /* SPIF Status Register R/W 32 */
#define SPIF_STATUS                 _SPIF_STATUS.Reg
#define SPIF_STATUS_SPI_INT         _SPIF_STATUS.Bits.SPI_INT                   /* BIT0 */
#define SPIF_STATUS_SPI_OVERFLOW    _SPIF_STATUS.Bits.SPI_OVERFLOW              /* BIT4 */
#define SPIF_STATUS_SPI_FIRST_DATA  _SPIF_STATUS.Bits.SPI_FIRST_DATA            /* BIT8 */




/* ------------- Analog to Digital Converter Module (ADC)-------------------- */
/*    Base Adrress = MBAR_ADC = 0x8000D000                                    */
__no_init extern volatile ADC_COMP_T  _ADC_COMP_0 @(MBAR_ADC + 0x00);           /* Monitor GP-ADC Pins Threshold detection w/o MCU intervention to generate IRQ R/W 16 */
#define ADC_COMP_0		          _ADC_COMP_0.Reg
#define ADC_COMP_0_VALUE	          _ADC_COMP_0.Bits.VALUE
#define ADC_COMP_0_CHANNEL	          _ADC_COMP_0.Bits.CHANNEL
#define ADC_COMP_0_GL		          _ADC_COMP_0.Bits.GL

__no_init extern volatile ADC_COMP_T  _ADC_COMP_1 @(MBAR_ADC + 0x02);           /* Monitor GP-ADC Pins Threshold R/W 16 */
#define ADC_COMP_1		          _ADC_COMP_1.Reg
#define ADC_COMP_1_VALUE	          _ADC_COMP_1.Bits.VALUE
#define ADC_COMP_1_CHANNEL	          _ADC_COMP_1.Bits.CHANNEL
#define ADC_COMP_1_GL		          _ADC_COMP_1.Bits.GL

__no_init extern volatile ADC_COMP_T  _ADC_COMP_2 @(MBAR_ADC + 0x04);           /* Monitor GP-ADC Pins Threshold R/W 16 */
#define ADC_COMP_2		          _ADC_COMP_2.Reg
#define ADC_COMP_2_VALUE	          _ADC_COMP_2.Bits.VALUE
#define ADC_COMP_2_CHANNEL	          _ADC_COMP_2.Bits.CHANNEL
#define ADC_COMP_2_GL		          _ADC_COMP_2.Bits.GL

__no_init extern volatile ADC_COMP_T  _ADC_COMP_3 @(MBAR_ADC + 0x06);           /* Monitor GP-ADC Pins Threshold R/W 16 */
#define ADC_COMP_3		          _ADC_COMP_3.Reg
#define ADC_COMP_3_VALUE	          _ADC_COMP_3.Bits.VALUE
#define ADC_COMP_3_CHANNEL	          _ADC_COMP_3.Bits.CHANNEL
#define ADC_COMP_3_GL		          _ADC_COMP_3.Bits.GL

__no_init extern volatile ADC_COMP_T  _ADC_COMP_4 @(MBAR_ADC + 0x08);           /* Monitor GP-ADC Pins Threshold R/W 16 */
#define ADC_COMP_4		          _ADC_COMP_4.Reg
#define ADC_COMP_4_VALUE	          _ADC_COMP_4.Bits.VALUE
#define ADC_COMP_4_CHANNEL	          _ADC_COMP_4.Bits.CHANNEL
#define ADC_COMP_4_GL		          _ADC_COMP_4.Bits.GL

__no_init extern volatile ADC_COMP_T  _ADC_COMP_5 @(MBAR_ADC + 0x0A);           /* Monitor GP-ADC Pins Threshold R/W 16 */
#define ADC_COMP_5		          _ADC_COMP_5.Reg
#define ADC_COMP_5_VALUE	          _ADC_COMP_5.Bits.VALUE
#define ADC_COMP_5_CHANNEL	          _ADC_COMP_5.Bits.CHANNEL
#define ADC_COMP_5_GL		          _ADC_COMP_5.Bits.GL

__no_init extern volatile ADC_COMP_T  _ADC_COMP_6 @(MBAR_ADC + 0x0C);           /* Monitor GP-ADC Pins Threshold R/W 16 */
#define ADC_COMP_6		          _ADC_COMP_6.Reg
#define ADC_COMP_6_VALUE	          _ADC_COMP_6.Bits.VALUE
#define ADC_COMP_6_CHANNEL	          _ADC_COMP_6.Bits.CHANNEL
#define ADC_COMP_6_GL		          _ADC_COMP_6.Bits.GL

__no_init extern volatile ADC_COMP_T  _ADC_COMP_7 @(MBAR_ADC + 0x0E);           /* Monitor GP-ADC Pins Threshold R/W 16 */
#define ADC_COMP_7		          _ADC_COMP_7.Reg
#define ADC_COMP_7_VALUE	          _ADC_COMP_7.Bits.VALUE
#define ADC_COMP_7_CHANNEL	          _ADC_COMP_7.Bits.CHANNEL
#define ADC_COMP_7_GL		          _ADC_COMP_7.Bits.GL


__no_init extern volatile ADC_BAT_COMP_T  _ADC_BAT_COMP_OVER @(MBAR_ADC + 0x10);/* Battery Voltage Upper Trip Point (ADC1 only) R/W 16 */
#define ADC_BAT_COMP_OVER		  _ADC_BAT_COMP_OVER.Reg
#define ADC_BAT_COMP_OVER_VALUE	          _ADC_BAT_COMP_OVER.Bits.VALUE


__no_init extern volatile ADC_BAT_COMP_T  _ADC_BAT_COMP_UNDER @(MBAR_ADC + 0x12);/* Battery Voltage Lower Trip Point (ADC1 only) R/W 16 */
#define ADC_BAT_COMP_UNDER		  _ADC_BAT_COMP_UNDER.Reg
#define ADC_BAT_COMP_UNDER_VALUE	  _ADC_BAT_COMP_UNDER.Bits.VALUE


typedef union
{
  struct
  {
    uint16 CH0        :1;
    uint16 CH1        :1;
    uint16 CH2        :1;
    uint16 CH3        :1;
    uint16 CH4        :1;
    uint16 CH5        :1;
    uint16 CH6        :1;
    uint16 CH7        :1;
    uint16 BATT       :1;
    uint16            :6;
    uint16 SEQ_MODE   :1;
  } Bits;
  uint16 Reg;
} ADC_SEQ_1_T;
__no_init extern volatile ADC_SEQ_1_T   _ADC_SEQ_1 @(MBAR_ADC + 0x14);          /* Monitor GP-ADC Pins for ADC1 R/W 16 */
#define ADC_SEQ_1		        _ADC_SEQ_1.Reg
#define ADC_SEQ_1_CH0	                _ADC_SEQ_1.Bits.CH0
#define ADC_SEQ_1_CH1	                _ADC_SEQ_1.Bits.CH1
#define ADC_SEQ_1_CH2	                _ADC_SEQ_1.Bits.CH2
#define ADC_SEQ_1_CH3	                _ADC_SEQ_1.Bits.CH3
#define ADC_SEQ_1_CH4	                _ADC_SEQ_1.Bits.CH4
#define ADC_SEQ_1_CH5	                _ADC_SEQ_1.Bits.CH5
#define ADC_SEQ_1_CH6	                _ADC_SEQ_1.Bits.CH6
#define ADC_SEQ_1_CH7	                _ADC_SEQ_1.Bits.CH7
#define ADC_SEQ_1_BATT	                _ADC_SEQ_1.Bits.BATT
#define ADC_SEQ_1_SEQ_MODE              _ADC_SEQ_1.Bits.SEQ_MODE


typedef union
{
  struct
  {
    uint16 CH0        :1;
    uint16 CH1        :1;
    uint16 CH2        :1;
    uint16 CH3        :1;
    uint16 CH4        :1;
    uint16 CH5        :1;
    uint16 CH6        :1;
    uint16 CH7        :1;
    uint16            :7;
    uint16 SEQ_MODE   :1;
  } Bits;
  uint16 Reg;
} ADC_SEQ_2_T;
__no_init extern volatile ADC_SEQ_2_T   _ADC_SEQ_2 @(MBAR_ADC + 0x16);          /* Monitor GP-ADC Pins for ADC2 R/W 16 */
#define ADC_SEQ_2		        _ADC_SEQ_2.Reg
#define ADC_SEQ_2_CH0	                _ADC_SEQ_2.Bits.CH0
#define ADC_SEQ_2_CH1	                _ADC_SEQ_2.Bits.CH1
#define ADC_SEQ_2_CH2	                _ADC_SEQ_2.Bits.CH2
#define ADC_SEQ_2_CH3	                _ADC_SEQ_2.Bits.CH3
#define ADC_SEQ_2_CH4	                _ADC_SEQ_2.Bits.CH4
#define ADC_SEQ_2_CH5	                _ADC_SEQ_2.Bits.CH5
#define ADC_SEQ_2_CH6	                _ADC_SEQ_2.Bits.CH6
#define ADC_SEQ_2_CH7	                _ADC_SEQ_2.Bits.CH7
#define ADC_SEQ_2_SEQ_MODE              _ADC_SEQ_2.Bits.SEQ_MODE


typedef union
{
  struct
  {
    uint16 ON               :1;
    uint16 TIMER1_ON        :1;
    uint16 TIMER2_ON        :1;
    uint16 SOFT_RESET       :1;
    uint16 AD1_VREFHL_EN    :1;
    uint16 AD2_VREFHL_EN    :1;
    uint16                  :6;
    uint16 COMPARE_IRQ_MASK :1;
    uint16 SEQ1_IRQ_MASK    :1;
    uint16 SEQ2_IRQ_MASK    :1;
    uint16 FIFO_IRQ_MASK    :1;
  } Bits;
  uint16 Reg;
} ADC_CONTROL_T;
__no_init extern volatile ADC_CONTROL_T   _ADC_CONTROL @(MBAR_ADC + 0x18);      /* Primary Module Control Register R/W 16 */
#define ADC_CONTROL		          _ADC_CONTROL.Reg
#define ADC_CONTROL_ON	                  _ADC_CONTROL.Bits.ON
#define ADC_CONTROL_TIMER1_ON	          _ADC_CONTROL.Bits.TIMER1_ON
#define ADC_CONTROL_TIMER2_ON	          _ADC_CONTROL.Bits.TIMER2_ON
#define ADC_CONTROL_SOFT_RESET	          _ADC_CONTROL.Bits.SOFT_RESET
#define ADC_CONTROL_AD1_VREFHL_EN	  _ADC_CONTROL.Bits.AD1_VREFHL_EN
#define ADC_CONTROL_AD2_VREFHL_EN	  _ADC_CONTROL.Bits.AD2_VREFHL_EN
#define ADC_CONTROL_COMPARE_IRQ_MASK	  _ADC_CONTROL.Bits.COMPARE_IRQ_MASK
#define ADC_CONTROL_SEQ1_IRQ_MASK	  _ADC_CONTROL.Bits.SEQ1_IRQ_MASK
#define ADC_CONTROL_SEQ2_IRQ_MASK	  _ADC_CONTROL.Bits.SEQ2_IRQ_MASK
#define ADC_CONTROL_FIFO_IRQ_MASK	  _ADC_CONTROL.Bits.FIFO_IRQ_MASK


typedef union
{
  struct
  {
    uint16 CH0        :1;
    uint16 CH1        :1;
    uint16 CH2        :1;
    uint16 CH3        :1;
    uint16 CH4        :1;
    uint16 CH5        :1;
    uint16 CH6        :1;
    uint16 CH7        :1;
    uint16 BAT_UNDER  :1;
    uint16 BAT_OVER   :1;
    uint16            :6;
  } Bits;
  uint16 Reg;
} ADC_TRIGGERS_T;
__no_init extern volatile ADC_TRIGGERS_T   _ADC_TRIGGERS @(MBAR_ADC + 0x1A);    /* Triggered Channels Register R 16 */
#define ADC_TRIGGERS		          _ADC_TRIGGERS.Reg
#define ADC_TRIGGERS_CH0	          _ADC_TRIGGERS.Bits.CH0
#define ADC_TRIGGERS_CH1	          _ADC_TRIGGERS.Bits.CH1
#define ADC_TRIGGERS_CH2	          _ADC_TRIGGERS.Bits.CH2
#define ADC_TRIGGERS_CH3	          _ADC_TRIGGERS.Bits.CH3
#define ADC_TRIGGERS_CH4	          _ADC_TRIGGERS.Bits.CH4
#define ADC_TRIGGERS_CH5	          _ADC_TRIGGERS.Bits.CH5
#define ADC_TRIGGERS_CH6	          _ADC_TRIGGERS.Bits.CH6
#define ADC_TRIGGERS_CH7	          _ADC_TRIGGERS.Bits.CH7
#define ADC_TRIGGERS_BAT_UNDER	          _ADC_TRIGGERS.Bits.BAT_UNDER
#define ADC_TRIGGERS_BAT_OVER	          _ADC_TRIGGERS.Bits.BAT_OVER


typedef union
{
  struct
  {
    uint16 PRESCALE   :8;
    uint16            :8;
  } Bits;
  uint16 Reg;
} ADC_PRESCALE_T;
__no_init extern volatile ADC_PRESCALE_T  _ADC_PRESCALE @(MBAR_ADC + 0x1C);     /* Bus Clock Divide Register (8-bit prescaler for use with 26MHz) R/W 16 ??? */
#define ADC_PRESCALE		          _ADC_PRESCALE.Reg
#define ADC_PRESCALE_PRESCALE	          _ADC_PRESCALE.Bits.PRESCALE


typedef union
{
  struct
  {
    uint16 VALUE      :12;
    uint16 CHANNEL    :4;
  } Bits;
  uint16 Reg;
} ADC_FIFO_READ_T;
__no_init extern volatile ADC_FIFO_READ_T  _ADC_FIFO_READ @(MBAR_ADC + 0x20);   /* ADC FIFO Read (8 deep FIFO) R 16 */
#define ADC_FIFO_READ		          _ADC_FIFO_READ.Reg
#define ADC_FIFO_READ_VALUE	          _ADC_FIFO_READ.Bits.VALUE
#define ADC_FIFO_READ_CHANNEL	          _ADC_FIFO_READ.Bits.CHANNEL


typedef union
{
  struct
  {
    uint16 LEVEL    :4;
    uint16          :12;
  } Bits;
  uint16 Reg;
} ADC_FIFO_CONTROL_T;
__no_init extern volatile ADC_FIFO_CONTROL_T _ADC_FIFO_CONTROL @(MBAR_ADC + 0x22); /* ADC Interrupt Level Control R/W 16 */
#define ADC_FIFO_CONTROL		    _ADC_FIFO_CONTROL.Reg
#define ADC_FIFO_CONTROL_LEVEL	            _ADC_FIFO_CONTROL.Bits.LEVEL


typedef union
{
  struct
  {
    uint16 LEVEL    :4;
    uint16 FULL     :1;
    uint16 EMPTY    :1;
    uint16          :10;
  } Bits;
  uint16 Reg;
} ADC_FIFO_STATUS_T;
__no_init extern volatile ADC_FIFO_STATUS_T _ADC_FIFO_STATUS @(MBAR_ADC + 0x24);/* ADC FIFO Status R 16 */
#define ADC_FIFO_STATUS		        _ADC_FIFO_STATUS.Reg
#define ADC_FIFO_STATUS_LEVEL	        _ADC_FIFO_STATUS.Bits.LEVEL
#define ADC_FIFO_STATUS_FULL	        _ADC_FIFO_STATUS.Bits.FULL
#define ADC_FIFO_STATUS_EMPTY	        _ADC_FIFO_STATUS.Bits.EMPTY


typedef uint16 ADC_1_SR_HIGH_T;
__no_init extern volatile ADC_1_SR_HIGH_T _ADC_1_SR_HIGH @(MBAR_ADC + 0x30);    /* ADC 1 Sample Rate High R/W 16 */
#define ADC_1_SR_HIGH                    _ADC_1_SR_HIGH


typedef uint16 ADC_1_SR_LOW_T;
__no_init extern volatile ADC_1_SR_LOW_T _ADC_1_SR_LOW @(MBAR_ADC + 0x32);      /* ADC 1 Sample Rate Low R/W 16 */
#define ADC_1_SR_LOW                    _ADC_1_SR_LOW

typedef uint16 ADC_2_SR_HIGH_T;
__no_init extern volatile ADC_2_SR_HIGH_T _ADC_2_SR_HIGH @(MBAR_ADC + 0x34);    /* ADC 2 Sample Rate High R/W 16 */
#define ADC_2_SR_HIGH                    _ADC_2_SR_HIGH


typedef uint16 ADC_2_SR_LOW_T;
__no_init extern volatile ADC_2_SR_LOW_T _ADC_2_SR_LOW @(MBAR_ADC + 0x36);      /* ADC 2 Sample Rate Low R/W 16 */
#define ADC_2_SR_LOW                    _ADC_2_SR_LOW


typedef uint16 ADC_ON_TIME_T;
__no_init extern volatile ADC_ON_TIME_T _ADC_ON_TIME @(MBAR_ADC + 0x38);        /* ADC TurnOn Time R/W 16 */
#define ADC_ON_TIME                    _ADC_ON_TIME


typedef uint16 ADC_CONVERT_TIME_T;
__no_init extern volatile ADC_CONVERT_TIME_T _ADC_CONVERT_TIME @(MBAR_ADC + 0x3A); /* ADC Convert Time R/W 16 */
#define ADC_CONVERT_TIME                    _ADC_CONVERT_TIME


typedef union
{
  struct
  {
    uint16 CLOCK_DIVIDER  :8;
    uint16                :8;
  } Bits;
  uint16 Reg;
} ADC_CLOCK_DIV_T;
__no_init extern volatile ADC_CLOCK_DIV_T _ADC_CLOCK_DIV @(MBAR_ADC + 0x3C);    /* ADC Clock Divider R/W 16 */
#define ADC_CLOCK_DIV		        _ADC_CLOCK_DIV.Reg
#define ADC_CLOCK_DIV_CLOCK_DIVIDER	_ADC_CLOCK_DIV.Bits.CLOCK_DIVIDER


typedef union
{
  struct
  {
    uint16 MUX1     :4;
    uint16 MUX2     :4;
    uint16 AD1_ON   :1;
    uint16 AD2_ON   :1;
    uint16          :6;
  } Bits;
  uint16 Reg;
} ADC_OVERRIDE_T;
__no_init extern volatile ADC_OVERRIDE_T _ADC_OVERRIDE @(MBAR_ADC + 0x40);      /* ADC Manual Control R/W 16 */
#define ADC_OVERRIDE		        _ADC_OVERRIDE.Reg
#define ADC_OVERRIDE_MUX1	        _ADC_OVERRIDE.Bits.MUX1
#define ADC_OVERRIDE_MUX2	        _ADC_OVERRIDE.Bits.MUX2
#define ADC_OVERRIDE_AD1_ON	        _ADC_OVERRIDE.Bits.AD1_ON
#define ADC_OVERRIDE_AD2_ON	        _ADC_OVERRIDE.Bits.AD2_ON


typedef union
{
  struct
  {
    uint16          :12;
    uint16 COMPARE  :1;
    uint16 SEQ1     :1;
    uint16 SEQ2     :1;
    uint16 FIFO     :1;
  } Bits;
  uint16 Reg;
} ADC_IRQ_T;
__no_init extern volatile ADC_IRQ_T _ADC_IRQ @(MBAR_ADC + 0x42);                /* ADC Read/Clear Active Interrupts R/W 16 */
#define ADC_IRQ		        _ADC_IRQ.Reg
#define ADC_IRQ_COMPARE	        _ADC_IRQ.Bits.COMPARE
#define ADC_IRQ_SEQ1	        _ADC_IRQ.Bits.SEQ1
#define ADC_IRQ_SEQ2	        _ADC_IRQ.Bits.SEQ2
#define ADC_IRQ_FIFO	        _ADC_IRQ.Bits.FIFO


typedef union
{
  struct
  {
    uint16 OVERRIDE   :1;
    uint16            :15;
  } Bits;
  uint16 Reg;
} ADC_MODE_T;
__no_init extern volatile ADC_MODE_T _ADC_MODE @(MBAR_ADC + 0x44);              /* ADC Mode R/W 16 */
#define ADC_MODE		  _ADC_MODE.Reg
#define ADC_MODE_OVERRIDE	  _ADC_MODE.Bits.OVERRIDE


typedef union
{
  struct
  {
    uint16 AD1_RESULT   :12;
    uint16              :4;
  } Bits;
  uint16 Reg;
} ADC_1_RESULT_T;
__no_init extern volatile ADC_1_RESULT_T _ADC_1_RESULT @(MBAR_ADC + 0x46);      /* ADC 1 Result R 16 */
#define ADC_1_RESULT		  _ADC_1_RESULT.Reg
#define ADC_1_RESULT_AD1_RESULT	  _ADC_1_RESULT.Bits.AD1_RESULT


typedef union
{
  struct
  {
    uint16 AD2_RESULT   :12;
    uint16              :4;
  } Bits;
  uint16 Reg;
} ADC_2_RESULT_T;
__no_init extern volatile ADC_2_RESULT_T _ADC_2_RESULT @(MBAR_ADC + 0x48);      /* ADC 2 Result R 16 */
#define ADC_2_RESULT		  _ADC_2_RESULT.Reg
#define ADC_2_RESULT_AD2_RESULT	  _ADC_2_RESULT.Bits.AD2_RESULT




/* ---------------------- Interrupt Controller (ITC) ------------------------ */
/*    Base Adrress = MBAR_AITC = 0x80020000                                   */
typedef union
{
  struct
  {
    uint32              :19;
    uint32 FIAD         :1;
    uint32 NIAD         :1;
    uint32              :11;
  } Bits;
  uint32 Reg;
} ITC_INTCNTL_T;
__no_init extern volatile ITC_INTCNTL_T _ITC_INTCNTL @(MBAR_AITC + 0x00);       /* ITC Interrupt Controller R/W 32 */
#define ITC_INTCNTL               _ITC_INTCNTL.Reg
#define ITC_INTCNTL_FIAD          _ITC_INTCNTL.Bits.FIAD                        /* BIT19 */
#define ITC_INTCNTL_NIAD          _ITC_INTCNTL.Bits.NIAD                        /* BIT20 */


typedef union
{
  struct
  {
    uint32 NIMASK0            :1;
    uint32 NIMASK1            :1;
    uint32 NIMASK2            :1;
    uint32 NIMASK3            :1;
    uint32 NIMASK4            :1;
    uint32                    :27;
  } Bits;
  struct
  {
    uint32 NIMASK             :5;
    uint32                    :27;
  } GrpBits;
  uint32 Reg;
} ITC_NIMASK_T;
__no_init extern volatile ITC_NIMASK_T _ITC_NIMASK @(MBAR_AITC + 0x04);         /* ITC Normal Interrupt Mask R/W 32 */
#define ITC_NIMASK                 _ITC_NIMASK.Reg
#define ITC_NIMASK_0               _ITC_NIMASK.Bits.NIMASK0                     /* BIT0 */
#define ITC_NIMASK_1               _ITC_NIMASK.Bits.NIMASK1                     /* BIT1 */
#define ITC_NIMASK_2               _ITC_NIMASK.Bits.NIMASK2                     /* BIT2 */
#define ITC_NIMASK_3               _ITC_NIMASK.Bits.NIMASK3                     /* BIT3 */
#define ITC_NIMASK_4               _ITC_NIMASK.Bits.NIMASK4                     /* BIT4 */
#define ITC_NIMASK_GRP             _ITC_NIMASK.GrpBits.NIMASK                   /* BIT0 | BIT1 | BIT2 | BIT3 | BIT4 */


typedef union
{
  struct
  {
    uint32 ENNUM0             :1;
    uint32 ENNUM1             :1;
    uint32 ENNUM2             :1;
    uint32 ENNUM3             :1;
    uint32                    :28;
  } Bits;
  struct
  {
    uint32 ENNUM              :4;
    uint32                    :28;
  } GrpBits;
  uint32 Reg;
} ITC_INTENNUM_T;
__no_init extern volatile ITC_INTENNUM_T _ITC_INTENNUM @(MBAR_AITC + 0x08);     /* ITC Interrupt Enable Number W 32 */
#define ITC_INTENNUM                _ITC_INTENNUM.Reg
#define ITC_INTENNUM_0              _ITC_INTENNUM.Bits.ENNUM0                   /* BIT0 */
#define ITC_INTENNUM_1              _ITC_INTENNUM.Bits.ENNUM1                   /* BIT1 */
#define ITC_INTENNUM_2              _ITC_INTENNUM.Bits.ENNUM2                   /* BIT2 */
#define ITC_INTENNUM_3              _ITC_INTENNUM.Bits.ENNUM3                   /* BIT3 */
#define ITC_INTENNUM_GRP            _ITC_INTENNUM.GrpBits.ENNUM                 /* BIT0 | BIT1 | BIT2 | BIT3 */


typedef union
{
  struct
  {
    uint32 DISNUM0              :1;
    uint32 DISNUM1              :1;
    uint32 DISNUM2              :1;
    uint32 DISNUM3              :1;
    uint32                      :28;
  } Bits;
  struct
  {
    uint32 DISNUM               :4;
    uint32                      :28;
  } GrpBits;
  uint32 Reg;
} ITC_INTDISNUM_T;
__no_init extern volatile ITC_INTDISNUM_T _ITC_INTDISNUM @(MBAR_AITC + 0x0c);   /* ITC Interrupt Disable Number W 32 */
#define ITC_INTDISNUM                _ITC_INTDISNUM.Reg
#define ITC_INTDISNUM_0              _ITC_INTDISNUM.Bits.DISNUM0                /* BIT0 */
#define ITC_INTDISNUM_1              _ITC_INTDISNUM.Bits.DISNUM1                /* BIT1 */
#define ITC_INTDISNUM_2              _ITC_INTDISNUM.Bits.DISNUM2                /* BIT2 */
#define ITC_INTDISNUM_3              _ITC_INTDISNUM.Bits.DISNUM3                /* BIT3 */
#define ITC_INTDISNUM_GRP            _ITC_INTDISNUM.GrpBits.DISNUM              /* BIT0 | BIT1 | BIT2 | BIT3 */


typedef union
{
  struct
  {
    uint32 INTENABLE            :16;
    uint32                      :16;
  } GrpBits;
  uint32 Reg;
} ITC_INTENABLE_T;
__no_init extern volatile ITC_INTENABLE_T _ITC_INTENABLE @(MBAR_AITC + 0x10);   /* ITC Interrupt Enable R/W 32 */
#define ITC_INTENABLE                _ITC_INTENABLE.Reg
#define ITC_INTENABLE_GRP            _ITC_INTENABLE.GrpBits.INTENABLE           /* BIT0 - BIT15 */


typedef union
{
  struct
  {
    uint32 INTTYPE              :16;
    uint32                      :16;
  } GrpBits;
  uint32 Reg;
} ITC_INTTYPE_T;
__no_init extern volatile ITC_INTTYPE_T _ITC_INTTYPE @(MBAR_AITC + 0x14);       /* ITC Interrupt Type R/W 32 */
#define ITC_INTTYPE                _ITC_INTTYPE.Reg
#define ITC_INTTYPE_GRP            _ITC_INTTYPE.GrpBits.INTTYPE                 /* BIT0 - BIT15 */


typedef union
{
  struct
  {
    uint32 NIVECTOR;
  } GrpBits;
  uint32 Reg;
} ITC_NIVECTOR_T;
__no_init extern volatile ITC_NIVECTOR_T _ITC_NIVECTOR @(MBAR_AITC + 0x28);     /* ITC Normal Interrupt R 32 */
#define ITC_NIVECTOR                _ITC_NIVECTOR.Reg
#define ITC_NIVECTOR_GRP            _ITC_NIVECTOR.GrpBits.NIVECTOR              /* BIT0 - BIT31 */


typedef union
{
  struct
  {
    uint32 FIVECTOR;
  } GrpBits;
  uint32 Reg;
} ITC_FIVECTOR_T;
__no_init extern volatile ITC_FIVECTOR_T _ITC_FIVECTOR @(MBAR_AITC + 0x2C);     /* ITC Fast Interrupt R 32 */
#define ITC_FIVECTOR                _ITC_FIVECTOR.Reg
#define ITC_FIVECTOR_GRP            _ITC_FIVECTOR.GrpBits.FIVECTOR              /* BIT0 - BIT31 */


typedef union
{
  struct
  {
    uint32 INTSRC               :16;
    uint32                      :16;
  } GrpBits;
  uint32 Reg;
} ITC_INTSRC_T;
__no_init extern volatile ITC_INTSRC_T _ITC_INTSRC @(MBAR_AITC + 0x30);         /* ITC Interrupt Source R 32 */
#define ITC_INTSRC                _ITC_INTSRC.Reg
#define ITC_INTSRC_GRP            _ITC_INTSRC.GrpBits.INTSRC                    /* BIT0 - BIT15 */


typedef union
{
  struct
  {
    uint32 INTFRC               :16;
    uint32                      :16;
  } GrpBits;
  uint32 Reg;
} ITC_INTFRC_T;
__no_init extern volatile ITC_INTFRC_T _ITC_INTFRC @(MBAR_AITC + 0x34);         /* ITC Interrupt Force R/W 32 */
#define ITC_INTFRC                _ITC_INTFRC.Reg
#define ITC_INTFRC_GRP            _ITC_INTFRC.GrpBits.INTFRC                    /* BIT0 - BIT15 */


typedef union
{
  struct
  {
    uint32 NIPEND               :16;
    uint32                      :16;
  } GrpBits;
  uint32 Reg;
} ITC_NIPEND_T;
__no_init extern volatile ITC_NIPEND_T _ITC_NIPEND @(MBAR_AITC + 0x38);         /* ITC Normal Interrupt Pending R 32 */
#define ITC_NIPEND                _ITC_NIPEND.Reg
#define ITC_NIPEND_GRP            _ITC_NIPEND.GrpBits.NIPEND                    /* BIT0 - BIT15 */


typedef union
{
  struct
  {
    uint32 FIPEND               :16;
    uint32                      :16;
  } GrpBits;
  uint32 Reg;
} ITC_FIPEND_T;
__no_init extern volatile ITC_FIPEND_T _ITC_FIPEND @(MBAR_AITC + 0x3C);         /* ITC Fast Interrupt Pending R 32 */
#define ITC_FIPEND                _ITC_FIPEND.Reg
#define ITC_FIPEND_GRP            _ITC_FIPEND.GrpBits.FIPEND                    /* BIT0 - BIT15 */


/***************************************************************************************************/



/* ---------------------- I2C Module (I2C) ------------------------- */
/*            Base Adrress = MBAR_I2C = 0x80006000                   */


/* ---- I2C Address Register ---- */
typedef union
{
  struct
  {
    byte          :1;
    byte  ADDR    :7;
  } Bits;
  byte Reg;
} I2CADDR_T;

__no_init extern volatile I2CADDR_T _I2CADDR @(MBAR_I2C + 0x000);  /* I2C Address Register */

#define I2CADDR          _I2C_ADDR.Reg
#define I2CADDR_ADDR     _I2C_ADDR.Bits.ADDR   /* Contain the specific slave address that is used by the I2C interface */
                                               /* BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 */

/* ---- I2C Frequency Divider Register ---- */
typedef union
{
  struct
  {
    byte FDR    :6;
    byte        :1;
    byte        :1;
  } Bits;
  byte Reg;
} I2CFDR_T;

__no_init extern volatile I2CFDR_T _I2CFDR @(MBAR_I2C + 0x004);  /* I2C Frequency Divider Register */

#define I2CFDR          _I2CFDR.Reg
#define I2CFDR_FDR      _I2CFDR.Bits.FDR   /* Frequency divider ratio.Used to prescale the clock for bit rate selection */
                                           /* BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5  */



/* ---- I2C Control Register ---- */
typedef union
{
  struct
  {
    byte BCST   :1;
    byte        :1;
    byte RSTA   :1;
    byte TXAK   :1;
    byte MTX    :1;
    byte MSTA   :1;
    byte MIEN   :1;
    byte MEN    :1;
  }Bits;
  byte Reg;
}I2CCR_T;

__no_init extern volatile I2CCR_T _I2CCR @(MBAR_I2C + 0x008); /* I2C Control Register */

#define I2CCR        _I2CCR.Reg
#define I2CCR_BCST   _I2CCR.Bits.BCST    /* BIT0 */  
                                         /* BIT1    RESERVED */ 
#define I2CCR_RSTA   _I2CCR.Bits.RSTA    /* BIT2 */
#define I2CCR_TXAK   _I2CCR.Bits.TXAK    /* BIT3 */
#define I2CCR_MTX    _I2CCR.Bits.MTX     /* BIT4 */
#define I2CCR_MSTA   _I2CCR.Bits.MSTA    /* BIT5 */
#define I2CCR_MIEN   _I2CCR.Bits.MIEN    /* BIT6 */
#define I2CCR_MEN    _I2CCR.Bits.MEN     /* BIT7 */



/* ---- I2C Status Register ---- */
typedef union
{
  struct
  {
    byte RXAK   :1;
    byte MIF    :1;
    byte SRW    :1;
    byte BCSTM  :1;
    byte MAL    :1;
    byte MBB    :1;
    byte MAAS   :1;
    byte MCF    :1;
  }Bits;
  byte Reg;
}I2CSR_T;

__no_init extern volatile I2CSR_T _I2CSR @(MBAR_I2C + 0x00C); /* I2C Status Register */

#define I2CSR         _I2CSR.Reg
#define I2CSR_RXAK    _I2CSR.Bits.RXAK     /* BIT0 */  
#define I2CSR_MIF     _I2CSR.Bits.MIF      /* BIT1 */ 
#define I2CSR_SRW     _I2CSR.Bits.SRW      /* BIT2 */
#define I2CSR_BCSTM   _I2CSR.Bits.BCSTM    /* BIT3 */
#define I2CSR_MAL     _I2CSR.Bits.MAL      /* BIT4 */
#define I2CSR_MBB     _I2CSR.Bits.MBB      /* BIT5 */
#define I2CSR_MAAS    _I2CSR.Bits.MAAS     /* BIT6 */
#define I2CSR_MCF     _I2CSR.Bits.MCF      /* BIT7 */

/* ---- I2C Data Register ---- */
typedef byte I2CDR_T;
__no_init extern volatile I2CDR_T _I2CDR @(MBAR_I2C + 0x010); /* I2C Data Register */
#define I2CDR  _I2CDR     /* BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 */


/* ---- I2C Digital Filter Sampling Rate Register ---- */
typedef union
{
  struct
  {
    byte DFSR  :6;
    byte       :1;
    byte       :1;
  }Bits;
  byte Reg;
}I2CDFSRR_T;

__no_init extern volatile I2CDFSRR_T _I2CDFSRR @(MBAR_I2C + 0x14); /* I2C Digital Filter Sampling Rate Register */
#define I2CDFSRR         _I2CDFSRR.Reg
#define I2CDFSRR_DFSR    _I2CDFSRR.Bits.DFSR  /* BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 */


/* ---- I2C Clock Enable Register ---- */
typedef union
{
  struct
  {
    byte CKEN   :1;
    byte        :1;
    byte        :1;
    byte        :1;
    byte        :1;
    byte        :1;
    byte        :1;
    byte        :1;
  }Bits;
  byte Reg;
}I2CCKER_T;

__no_init extern volatile I2CCKER_T _I2CCKER @(MBAR_I2C + 0x018); /* I2C Clock Enable Register */
#define I2CCKER          _I2CCKER.Reg
#define I2CCKER_CKEN     _I2CCKER.Bits.CKEN  /* BIT0 */

#endif /* _NIVIS_MC1322x_H_ */
