/************************************************************************************
* This header file is provided as an interface to hardware
* This file holds definitions of the peripheral registers  
*
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
* 
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/
#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#include "CRM_Regs.h"
#include "Ssi_Regs.h"

/* platform clock  boundaries */
#define PLATFORM_MAX_CLOCK (26214) // maximum value of platform clock is 26214 kHz
#define PLATFORM_MIN_CLOCK (1000)  // minimum value of platform clock is 1000 kHz

/* RAM jumpvectores located @ Memory start. 0x00400000 */
#define  RAM_BASE    0x00400000
#define  RAM_TOP     0x00417FFF

/*-- Defines for Processor Modes --------------------------*/
#define  Mode_Fvc    0x11   // FIQ
#define  Mode_Ivc    0x12   // IRQ
#define  Mode_Svc    0x13   // supervisor
#define  Mode_Avc    0x17   // abort
#define  Mode_Uvc    0x1b   // undefined
#define  Mode_USER   0x10   // user
#define  Mode_SYS    0x1f   // system mode

/*-- Defines GPIO pin states ------------------------------*/
#define DIR_INPUT    0
#define DIR_OUT      1
#define INPUT_PAD    0
#define INPUT_LATCH  1
#define FN_IO        0
#define FN_ALT       1
#define FN_MASK      3UL

/*Peripheral Memory Map Base addresses*/
#define  MBAR_AIPI   0x80000000  //32 bit
#define  MBAR_GPIO   0x80000000  //32 bit
#define  MBAR_SSI    0x80001000  //32 bit
#define  MBAR_SPI   0x80002000  //32 bit
#define  MBAR_CRM    0x80003000  //32 bit
#define  MBAR_MACA   0x80004000  //32 bit
#define  MBAR_UART1  0x80005000  //32 bit
#define  MBAR_I2C    0x80006000  //32 bit
#define  MBAR_TMR    0x80007000  //16 bit
#define  MBAR_TMR0   0x80007000  //16 bit
#define  MBAR_TMR1   0x80007020  //16 bit
#define  MBAR_TMR2   0x80007040  //16 bit
#define  MBAR_TMR3   0x80007060  //16 bit
#define  MBAR_TMR_EN 0x8000701E  //16 bit 
#define  MBAR_ASM    0x80008000  //32 bit
#define  MBAR_MODEM  0x80009000  //32 bit
#define  MBAR_RF     0x8000A000  //32 bit
#define  MBAR_UART2  0x8000B000  //32 bit
#define  MBAR_FLASH  0x8000C000  //32 bit
#define  MBAR_ADC    0x8000D000  //16 bit
#define  MBAR_AITC   0x80020000  //32 bit
#define  MBAR_ITC    0x80020000  //32 bit
#define  MBAR_NEX    0x80040000  //32 bit

/* Peripheral Structure Pointer definitions */
#define  GPIO_REGS_P   ((GpioRegs_t  *)MBAR_GPIO)
#define  SSI_REGS_P    ((SsiRegs_t   *)MBAR_SSI)
#define  SPI_REGS_P   ((SpiRegs_t   *)MBAR_SPI)
#define  CRM_REGS_P    ((CrmRegs_t   *)MBAR_CRM)
#define  MACA_REGS_P   ((MacaRegs_t  *)MBAR_MACA)
#define  UART1_REGS_P  ((UartRegs_t  *)MBAR_UART1)
#define  I2C_REGS_P    ((I2cRegs_t   *)MBAR_I2C)
#define  TMR_REGS_P    ((TmrRegs_t   *)MBAR_TMR)
#define  TMR0_REGS_P   ((TmrRegs_t   *)MBAR_TMR0)
#define  TMR1_REGS_P   ((TmrRegs_t   *)MBAR_TMR1)
#define  TMR2_REGS_P   ((TmrRegs_t   *)MBAR_TMR2)
#define  TMR3_REGS_P   ((TmrRegs_t   *)MBAR_TMR3)
#define  TMR_EN_REG_P  ((volatile uint16_t   * )MBAR_TMR_EN)
#define  ASM_REGS_P    ((AsmRegs_t  *)MBAR_ASM)
#define  MODEM_REGS_P  ((ModemRegs_t *)MBAR_MODEM)
#define  RF_REGS_P     ((RfRegs_t    *)MBAR_RF)
#define  UART2_REGS_P  ((UartRegs_t  *)MBAR_UART2)
#define  FLASH_REGS_P  ((SpiRegs_t   *)MBAR_FLASH)
#define  ADC_REGS_P    ((AdcRegs_t   *)MBAR_ADC)
#define  AITC_REGS_P   ((AitcRegs_t  *)MBAR_AITC)
#define  ITC_REGS_P    ((ItcRegs_t   *)MBAR_ITC)
#define  NEX_REGS_P    ((NexRegs_t   *)MBAR_NEX)

/*  extended pointer definition  */
#define GPIO   (*GPIO_REGS_P)
#define SSI    (*SSI_REGS_P)
#define SPI   (*SPI_REGS_P)
#define CRM    (*CRM_REGS_P)
#define MACA   (*MACA_REGS_P)
#define UART1  (*UART1_REGS_P)
#define I2C    (*I2C_REGS_P)
#define TMR0   (*TMR0_REGS_P)
#define TMR1   (*TMR1_REGS_P)
#define TMR2   (*TMR2_REGS_P)
#define TMR3   (*TMR3_REGS_P)
#define TMR_EN (*TMR_EN_REG_P)
#define ASM    (*ASM_REGS_P)
#define MODEM  (*MODEM_REGS_P)
#define RF     (*RF_REGS_P)
#define UART2  (*UART2_REGS_P)
#define FLASH  (*FLASH_REGS_P)
#define ADC    (*ADC_REGS_P)
#define AITC   (*AITC_REGS_P)
#define ITC    (*ITC_REGS_P)
#define NEX    (*NEX_REGS_P)

//define the registers for the GPIO peripheral
typedef struct
{  // Registers
  volatile unsigned long DirLo;             //MBAR_GPIO + 0x00
  volatile unsigned long DirHi;             //MBAR_GPIO + 0x04
  volatile unsigned long DataLo;            //MBAR_GPIO + 0x08
  volatile unsigned long DataHi;            //MBAR_GPIO + 0x0C
  volatile unsigned long PuEnLo;            //MBAR_GPIO + 0x10
  volatile unsigned long PuEnHi;            //MBAR_GPIO + 0x14
  volatile unsigned long FuncSel0;          //MBAR_GPIO + 0x18
  volatile unsigned long FuncSel1;          //MBAR_GPIO + 0x1C
  volatile unsigned long FuncSel2;          //MBAR_GPIO + 0x20
  volatile unsigned long FuncSel3;          //MBAR_GPIO + 0x24
  volatile unsigned long InputDataSelLo;    //MBAR_GPIO + 0x28
  volatile unsigned long InputDataSelHi;    //MBAR_GPIO + 0x2C
  volatile unsigned long PuSelLo;           //MBAR_GPIO + 0x30
  volatile unsigned long PuSelHi;           //MBAR_GPIO + 0x34
  volatile unsigned long HystEnLo;          //MBAR_GPIO + 0x38
  volatile unsigned long HystEnHi;          //MBAR_GPIO + 0x3C
  volatile unsigned long PuKeepLo;          //MBAR_GPIO + 0x40
  volatile unsigned long PuKeepHi;          //MBAR_GPIO + 0x44
  // Virtual registers
  volatile unsigned long DataSetLo;         //MBAR_GPIO + 0x48
  volatile unsigned long DataSetHi;         //MBAR_GPIO + 0x4C
  volatile unsigned long DataResetLo;       //MBAR_GPIO + 0x50
  volatile unsigned long DataResetHi;       //MBAR_GPIO + 0x54
  volatile unsigned long DirSetLo;          //MBAR_GPIO + 0x58
  volatile unsigned long DirSetHi;          //MBAR_GPIO + 0x5C
  volatile unsigned long DirResetLo;        //MBAR_GPIO + 0x60
  volatile unsigned long DirResetHi;        //MBAR_GPIO + 0x64
} GpioRegs_t;

//define the registers for the CRM peripheral

typedef struct
{
  volatile unsigned long SysCntl;            //MBAR_CRM + 0x00
  volatile unsigned long WuCntl;             //MBAR_CRM + 0x04
  volatile unsigned long SleepCntl;          //MBAR_CRM + 0x08
  volatile unsigned long BsCntl;             //MBAR_CRM + 0x0C
  volatile unsigned long CopCntl;            //MBAR_CRM + 0x10
  volatile unsigned long CopService;         //MBAR_CRM + 0x14
  volatile unsigned long Status;             //MBAR_CRM + 0x18
  volatile unsigned long ModStatus;          //MBAR_CRM + 0x1C
  volatile unsigned long WuCount;            //MBAR_CRM + 0x20
  volatile unsigned long WuTimeout;          //MBAR_CRM + 0x24
  volatile unsigned long RtcCount;           //MBAR_CRM + 0x28
  volatile unsigned long RtcTimeout;         //MBAR_CRM + 0x2C
  volatile unsigned long reserved;           //MBAR_CRM + 0x30
  volatile unsigned long CalCntl;            //MBAR_CRM + 0x34
  volatile unsigned long CalXtalCnt;         //MBAR_CRM + 0x38
  volatile unsigned long RingOsclCntl;       //MBAR_CRM + 0x3C
  volatile unsigned long XtalCntl;           //MBAR_CRM + 0x40
  volatile unsigned long Xtal32Cntl;         //MBAR_CRM + 0x44
  volatile unsigned long VregCntl;           //MBAR_CRM + 0x48
  volatile unsigned long VregTrim;           //MBAR_CRM + 0x4C
  volatile unsigned long SwRst;              //MBAR_CRM + 0x50
} CrmRegs_t;

//define the registers for the ITC peripheral
typedef struct
{
  volatile unsigned long IntCntl;           //MBAR_ITC + 0x00
  volatile unsigned long NiMask;            //MBAR_ITC + 0x04
  volatile unsigned long IntEnNum;          //MBAR_ITC + 0x08
  volatile unsigned long IntDisNum;         //MBAR_ITC + 0x0C
  volatile unsigned long IntEnable;         //MBAR_ITC + 0x10
  volatile unsigned long IntType;           //MBAR_ITC + 0x14
  volatile unsigned long reserved3;         //MBAR_ITC + 0x18
  volatile unsigned long reserved2;         //MBAR_ITC + 0x1C
  volatile unsigned long reserved1;         //MBAR_ITC + 0x20
  volatile unsigned long reserved0;         //MBAR_ITC + 0x24
  volatile unsigned long NiVector;          //MBAR_ITC + 0x28
  volatile unsigned long FiVector;          //MBAR_ITC + 0x2C
  volatile unsigned long IntSrc;            //MBAR_ITC + 0x30
  volatile unsigned long IntFrc;            //MBAR_ITC + 0x34
  volatile unsigned long NiPend;             //MBAR_ITC + 0x38
  volatile unsigned long FiPend;             //MBAR_ITC + 0x3C
} ItcRegs_t, AitcRegs_t;

//define the registers for the UART  peripherals
typedef struct
{
  volatile unsigned long Ucon;              //MBAR_UARTx + 0x00
  volatile unsigned long Ustat;             //MBAR_UARTx + 0x04
  volatile unsigned long Udata;             //MBAR_UARTx + 0x08
  volatile unsigned long Urxcon;            //MBAR_UARTx + 0x0C
  volatile unsigned long Utxcon;            //MBAR_UARTx + 0x10
  volatile unsigned long Ucts;              //MBAR_UARTx + 0x14
  volatile unsigned long Ubr;               //MBAR_UARTx + 0x18
} UartRegs_t;

//define the registers for the SPI  peripherals
typedef struct
{
  volatile unsigned long TxData;            //MBAR_SPIx + 0x00
  volatile unsigned long RxData;            //MBAR_SPIx + 0x04
  volatile unsigned long ClkCtrl;           //MBAR_SPIx + 0x08
  volatile unsigned long Setup;             //MBAR_SPIx + 0x0C
  volatile unsigned long Status;            //MBAR_SPIx + 0x10
} SpiRegs_t;

//define the registers for the TIMER  peripherals
typedef struct
{
  volatile unsigned short Comp1;            //MBAR_TMRx + 0x00
  volatile unsigned short Comp2;            //MBAR_TMRx + 0x02
  volatile unsigned short Capt;             //MBAR_TMRx + 0x04
  volatile unsigned short Load;             //MBAR_TMRx + 0x06
  volatile unsigned short Hold;             //MBAR_TMRx + 0x08
  volatile unsigned short Cntr;             //MBAR_TMRx + 0x0A
  volatile unsigned short Ctrl;             //MBAR_TMRx + 0x0C
  volatile unsigned short StatCtrl;         //MBAR_TMRx + 0x0E
  volatile unsigned short CmpLd1;           //MBAR_TMRx + 0x10
  volatile unsigned short CmpLd2;           //MBAR_TMRx + 0x12
  volatile unsigned short CompStatCtrl;     //MBAR_TMRx + 0x14
  volatile unsigned short reserved0;        //MBAR_TMRx + 0x16
  volatile unsigned short reserved1;        //MBAR_TMRx + 0x18
  volatile unsigned short reserved2;        //MBAR_TMRx + 0x1A
  volatile unsigned short reserved3;        //MBAR_TMRx + 0x1C
  volatile unsigned short reserved4;        //MBAR_TMRx + 0x1E
} TmrRegs_t;

//define the registers for the ASM peripheral
typedef struct
{
  volatile unsigned long Key0;             //MBAR_ASM + 0x00
  volatile unsigned long Key1;             //MBAR_ASM + 0x04
  volatile unsigned long Key2;             //MBAR_ASM + 0x08
  volatile unsigned long Key3;             //MBAR_ASM + 0x0C
  volatile unsigned long Data0;            //MBAR_ASM + 0x10
  volatile unsigned long Data1;            //MBAR_ASM + 0x14
  volatile unsigned long Data2;            //MBAR_ASM + 0x18
  volatile unsigned long Data3;            //MBAR_ASM + 0x1C
  volatile unsigned long Ctr0;             //MBAR_ASM + 0x20
  volatile unsigned long Ctr1;             //MBAR_ASM + 0x24
  volatile unsigned long Ctr2;             //MBAR_ASM + 0x28
  volatile unsigned long Ctr3;             //MBAR_ASM + 0x2C
  volatile unsigned long Ctr_result0;      //MBAR_ASM + 0x30
  volatile unsigned long Ctr_result1;      //MBAR_ASM + 0x34
  volatile unsigned long Ctr_result2;      //MBAR_ASM + 0x38
  volatile unsigned long Ctr_result3;      //MBAR_ASM + 0x3C
  volatile unsigned long Cbc_result0;      //MBAR_ASM + 0x40
  volatile unsigned long Cbc_result1;      //MBAR_ASM + 0x44
  volatile unsigned long Cbc_result2;      //MBAR_ASM + 0x48
  volatile unsigned long Cbc_result3;      //MBAR_ASM + 0x4C
  volatile unsigned long Control0;         //MBAR_ASM + 0x50
  volatile unsigned long Control1;         //MBAR_ASM + 0x54
  volatile unsigned long Status;           //MBAR_ASM + 0x58
  volatile unsigned long Undef0;           //MBAR_ASM + 0x5C  
  volatile unsigned long Mac0;             //MBAR_ASM + 0x60
  volatile unsigned long Mac1;             //MBAR_ASM + 0x64
  volatile unsigned long Mac2;             //MBAR_ASM + 0x68
  volatile unsigned long Mac3;             //MBAR_ASM + 0x6C  
} AsmRegs_t;

//define the registers for the I2C  peripheral
typedef struct
{
  volatile unsigned char Address;        //MBAR_I2C + 0x00     address register
  unsigned char dummy0;
  unsigned char dummy1;
  unsigned char dummy2;
  volatile unsigned char FreqDiv;        //MBAR_I2C + 0x04     frequency divider register
  unsigned char dummy3;
  unsigned char dummy4;
  unsigned char dummy5;
  volatile unsigned char Control;        //MBAR_I2C + 0x08     control register
  unsigned char dummy6;
  unsigned char dummy7;
  unsigned char dummy8;
  volatile unsigned char Status;         //MBAR_I2C + 0x0C     status register
  unsigned char dummy9;
  unsigned char dummy10;
  unsigned char dummy11;
  volatile unsigned char Data;           //MBAR_I2C + 0x10     data register
  unsigned char dummy12;
  unsigned char dummy13;
  unsigned char dummy14;
  volatile unsigned char DigitalFilter;  //MBAR_I2C + 0x14     digital filter sampling rate register
  unsigned char dummy15;
  unsigned char dummy16;
  unsigned char dummy17;
  volatile unsigned char ClockEn;        //MBAR_I2C + 0x18     clock enable register
  unsigned char dummy18;
  unsigned char dummy19;
  unsigned char dummy20;
} I2cRegs_t;

//define the registers for the SSI peripheral
typedef struct
{
  volatile unsigned long  STX;        //MBAR_SSI + 0x00     STX   (Transmit Data register)
           unsigned long  dummy1;     //MBAR_SSI + 0x04
  volatile unsigned long  SRX;        //MBAR_SSI + 0x08     SRX   (Receive Data Register)
           unsigned long  dummy2;     //MBAR_SSI + 0x0C
  volatile unsigned long  SCR;        //MBAR_SSI + 0x10     SCR   (Control register)
  volatile unsigned long  SISR;       //MBAR_SSI + 0x14     SISR  (Interrupt status register)
  volatile unsigned long  SIER;       //MBAR_SSI + 0x18     SIER  (Interrupt enable register)
  volatile unsigned long  STCR;       //MBAR_SSI + 0x1C     STCR  (Transmit configuration register)
  volatile unsigned long  SRCR;       //MBAR_SSI + 0x20     SRCR  (Receive configuration register)
  volatile unsigned long  STCCR;      //MBAR_SSI + 0x24     STCCR (Transmit and Receive Clock configuration register)
           unsigned long  dummy3;     //MBAR_SSI + 0x28
  volatile unsigned long  SFCSR;      //MBAR_SSI + 0x2C     SFCSR (FIFO control / status register)
  volatile unsigned long  STR;        //MBAR_SSI + 0x30     STR   (Test register)
  volatile unsigned long  SOR;        //MBAR_SSI + 0x34     SOR   (Option register)
           unsigned long  dummy4;     //MBAR_SSI + 0x38
           unsigned long  dummy5;     //MBAR_SSI + 0x3C
           unsigned long  dummy6;     //MBAR_SSI + 0x40
           unsigned long  dummy7;     //MBAR_SSI + 0x44
  volatile unsigned long  STMSK;      //MBAR_SSI + 0x48     STMSK (Transmit Time Slot mask register)
  volatile unsigned long  SRMSK;      //MBAR_SSI + 0x4C     SRMSK (Receive Time Slot mask register)
}SsiRegs_t;

//define the registers for the ADC  peripheral
typedef struct
{
  volatile unsigned short Comp0;            //MBAR_ADC + 0x00     Compare0 register
  volatile unsigned short Comp1;            //MBAR_ADC + 0x02     Compare1 register
  volatile unsigned short Comp2;            //MBAR_ADC + 0x04     Compare2 register
  volatile unsigned short Comp3;            //MBAR_ADC + 0x06     Compare3 register
  volatile unsigned short Comp4;            //MBAR_ADC + 0x08     Compare4 register
  volatile unsigned short Comp5;            //MBAR_ADC + 0x0A     Compare5 register
  volatile unsigned short Comp6;            //MBAR_ADC + 0x0C     Compare6 register
  volatile unsigned short Comp7;            //MBAR_ADC + 0x0E     Compare7 register
  volatile unsigned short BattCompOver;     //MBAR_ADC + 0x10     Battery Voltage upper trip point
  volatile unsigned short BattCompUnder;    //MBAR_ADC + 0x12     Battery Voltage lower trip point
  volatile unsigned short Seq1;             //MBAR_ADC + 0x14     Sequencer1 register
  volatile unsigned short Seq2;             //MBAR_ADC + 0x16     Sequencer2 register
  volatile unsigned short Control;          //MBAR_ADC + 0x18     Control register
  volatile unsigned short Triggers;         //MBAR_ADC + 0x1A     Triggers register
  volatile unsigned short Prescale;         //MBAR_ADC + 0x1C     Prescale register
  volatile unsigned short reserved1;        //MBAR_ADC + 0x1E     reserved
  volatile unsigned short FifoRead;         //MBAR_ADC + 0x20     FIFO Read register
  volatile unsigned short FifoCtrl;         //MBAR_ADC + 0x22     FIFO Control register
  volatile unsigned short FifoStatus;       //MBAR_ADC + 0x24     FIFO Status register
  volatile unsigned short reserved2;        //MBAR_ADC + 0x26     register
  volatile unsigned short reserved3;        //MBAR_ADC + 0x28     register
  volatile unsigned short reserved4;        //MBAR_ADC + 0x2A     register
  volatile unsigned short reserved5;        //MBAR_ADC + 0x2C     register
  volatile unsigned short reserved6;        //MBAR_ADC + 0x2E     register
  volatile unsigned short Sr1High;          //MBAR_ADC + 0x30     Timer1 Sample Rate High Value
  volatile unsigned short Sr1Low;           //MBAR_ADC + 0x32     Timer1 Sample Rate Low Value
  volatile unsigned short Sr2High;          //MBAR_ADC + 0x34     Timer2 Sample Rate High Value
  volatile unsigned short Sr2Low;           //MBAR_ADC + 0x36     Timer2 Sample Rate Low Value
  volatile unsigned short OnTime;           //MBAR_ADC + 0x38     On Time register
  volatile unsigned short ConvTime;         //MBAR_ADC + 0x3A     Convert Time register
  volatile unsigned short ClkDiv;           //MBAR_ADC + 0x3C     Clock divider register
  volatile unsigned short reserved7;        //MBAR_ADC + 0x3E     reserved
  volatile unsigned short Override;         //MBAR_ADC + 0x40     Override register
  volatile unsigned short Irq;              //MBAR_ADC + 0x42     Interrupt register
  volatile unsigned short Mode;             //MBAR_ADC + 0x44     ADC Mode register
  volatile unsigned short Adc1Result;       //MBAR_ADC + 0x46     ADC1 Result register
  volatile unsigned short Adc2Result;       //MBAR_ADC + 0x48     ADC2 Result register
} AdcRegs_t;

////////////////////////////////// UART ////////////////////////////////////////
// GPIO settings
// 1 bit masks
// 'Lo' group
#define  GPIO_UART1_TX_bit    BIT14
#define  GPIO_UART1_RX_bit    BIT15
#define  GPIO_UART1_CTS_bit   BIT16
#define  GPIO_UART1_RTS_bit   BIT17

// 'Lo' group
#define  GPIO_UART2_TX_bit    BIT18
#define  GPIO_UART2_RX_bit    BIT19
#define  GPIO_UART2_CTS_bit   BIT20
#define  GPIO_UART2_RTS_bit   BIT21

// 2 bit groups positions
// FNSEL0 group
#define  GPIO_UART1_TX_fnpos  (14*2)
#define  GPIO_UART1_RX_fnpos  (15*2)

// FNSEL1 group
#define  GPIO_UART1_CTS_fnpos (16*2-32)
#define  GPIO_UART1_RTS_fnpos (17*2-32)

#define  GPIO_UART2_TX_fnpos  (18*2-32)
#define  GPIO_UART2_RX_fnpos  (19*2-32)
#define  GPIO_UART2_CTS_fnpos (20*2-32)
#define  GPIO_UART2_RTS_fnpos (21*2-32)

////////////////////////////////// SPI  ////////////////////////////////////////
// GPIO settings
// 1 bit masks
// 'Lo' group
#define  GPIO_SPI_SS_bit      BIT4
#define  GPIO_SPI_MISO_bit    BIT5
#define  GPIO_SPI_MOSI_bit    BIT6
#define  GPIO_SPI_SCK_bit     BIT7

// 2 bit groups positions
// FNSEL0 group
#define  GPIO_SPI_SS_fnpos    (4*2)
#define  GPIO_SPI_MISO_fnpos  (5*2)
#define  GPIO_SPI_MOSI_fnpos  (6*2)
#define  GPIO_SPI_SCK_fnpos   (7*2)

////////////////////////////////// Timer////////////////////////////////////////
// GPIO settings
// 1 bit masks
// 'Lo' group
#define  GPIO_TIMER0_INOUT_bit    BIT8
#define  GPIO_TIMER1_INOUT_bit    BIT9
#define  GPIO_TIMER2_INOUT_bit    BIT10
#define  GPIO_TIMER3_INOUT_bit    BIT11

// 2 bit groups positions
// FNSEL0 group
#define  GPIO_TIMER0_INOUT_fnpos  (8*2)
#define  GPIO_TIMER1_INOUT_fnpos  (9*2)
#define  GPIO_TIMER2_INOUT_fnpos  (10*2)
#define  GPIO_TIMER3_INOUT_fnpos  (11*2)

////////////////////////////////////////////////////////////////////////////////


#endif /* _PLATFORM_H_ */
