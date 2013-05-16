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
* Name:         MC1322x.c
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This file is provided as an interface to hardware
*               This file holds definitions of the peripheral registers
* Changes:
* Revisions:
****************************************************************************************************/
#include "MC1322x.h"



/* -------------- General Purpose I/O Module (GPIO) ------------------------- */
/*    Base Adrress = MBAR_GPIO = 0x80000000                                   */
volatile GPIO_PORT0_T           _GPIO_PAD_DIR0;                                 /* GPIO Pad Direction for GPIO 00-31 R/W 32 */
volatile GPIO_PORT1_T           _GPIO_PAD_DIR1;                                 /* GPIO Pad Direction for GPIO 32-63 R/W 32 */
volatile GPIO_PORT0_T           _GPIO_DATA0;                                    /* GPIO Data for GPIO 00-31 R/W 32 */
volatile GPIO_PORT1_T           _GPIO_DATA1;                                    /* GPIO Data for GPIO 32-63 R/W 32 */
volatile GPIO_PORT0_T           _GPIO_PAD_PU_EN0;                               /* GPIO Pad Pullup Enable for GPIO 00-31 R/W 32 */
volatile GPIO_PORT1_T           _GPIO_PAD_PU_EN1;                               /* GPIO Pad Pullup Enable for GPIO 00-31 R/W 32 */
volatile GPIO_FUNC_SEL0_T       _GPIO_FUNC_SEL0;                                /* GPIO Function Select for GPIO 00-15 R/W 32 */
volatile GPIO_FUNC_SEL1_T       _GPIO_FUNC_SEL1;                                /* GPIO Function Select for GPIO 16-31 R/W 32 */
volatile GPIO_FUNC_SEL2_T       _GPIO_FUNC_SEL2;                                /* GPIO Function Select for GPIO 32-47 R/W 32 */
volatile GPIO_FUNC_SEL3_T       _GPIO_FUNC_SEL3;                                /* GPIO Function Select for GPIO 48-63 R/W 32 */
volatile GPIO_PORT0_T           _GPIO_DATA_SEL0;                                /* GPIO Data Select for GPIO 00-31 R/W 32 */
volatile GPIO_PORT1_T           _GPIO_DATA_SEL1;                                /* GPIO Data Select for GPIO 32-63 R/W 32 */
volatile GPIO_PORT0_T           _GPIO_PAD_PU_SEL0;                              /* GPIO Pad Pullup Select for GPIO 00-31 R/W 32 */
volatile GPIO_PORT1_T           _GPIO_PAD_PU_SEL1;                              /* GPIO Pad Pullup Select for GPIO 32-63 R/W 32 */
volatile GPIO_PORT0_T           _GPIO_PAD_HYST_EN0;                             /* GPIO Pad Hysteresis Enable for GPIO 00-31 R/W 32 */
volatile GPIO_PORT1_T           _GPIO_PAD_HYST_EN1;                             /* GPIO Pad Hysteresis Enable for GPIO 32-63 R/W 32 */
volatile GPIO_PORT0_T           _GPIO_PAD_KEEP0;                                /* GPIO Pad Keeper Enable for GPIO 00-31 R/W 32 */
volatile GPIO_PORT1_T           _GPIO_PAD_KEEP1;                                /* GPIO Pad Keeper Enable for GPIO 32-63 R/W 32 */
volatile GPIO_PORT0_T           _GPIO_DATA_SET0;                                /* GPIO Data Set for GPIO 00-31 W 32 */
volatile GPIO_PORT1_T           _GPIO_DATA_SET1;                                /* GPIO Data Set for GPIO 32-63 W 32 */
volatile GPIO_PORT0_T           _GPIO_DATA_RESET0;                              /* GPIO Data Reset for GPIO 00-31 W 32 */
volatile GPIO_PORT1_T           _GPIO_DATA_RESET1;                              /* GPIO Data Reset for GPIO 32-63 W 32 */
volatile GPIO_PORT0_T           _GPIO_PAD_DIR_SET0;                             /* GPIO Pad Direction Set for GPIO 00-31 W 32 */
volatile GPIO_PORT1_T           _GPIO_PAD_DIR_SET1;                             /* GPIO Pad Direction Set for GPIO 32-63 W 32 */
volatile GPIO_PORT0_T           _GPIO_PAD_DIR_RESET0;                           /* GPIO Pad Direction Reset for GPIO 00-31 W 32 */
volatile GPIO_PORT1_T           _GPIO_PAD_DIR_RESET1;                           /* GPIO Pad Direction Reset for GPIO 32-63 W 32 */



/* ---------------- Clock/Reset/Power Module (CRM) -------------------------- */
/*    Base Adrress = MBAR_CRM = 0x8000_3000                                   */
volatile CRM_SYS_CNTL_T     _SYS_CNTL;                                          /* CRM System Control R/W 32 */
volatile CRM_WU_CNTL_T      _WU_CNTL;                                           /* CRM Wakeup Control R/W 32 */
volatile CRM_SLEEP_CNTL_T   _SLEEP_CNTL;                                        /* CRM Sleep Control R/W 32 */
volatile CRM_BS_CNTL_T      _BS_CNTL ;                                          /* CRM Bus Stealing Control R/W 32 */
volatile CRM_COP_CNTL_T     _COP_CNTL;                                          /* CRM COP Control R/W 32 */
volatile CRM_COP_SERVICE_T  _COP_SERVICE;                                       /* CRM COP Service R/W 32 */
volatile CRM_STATUS_T       _CRM_STATUS;                                        /* CRM Event Status R/W 32 */ // !!! see it
volatile CRM_MOD_STATUS_T   _MOD_STATUS;                                        /* CRM Module Enable Status R 32 */
volatile CRM_WU_COUNT_T     _WU_COUNT;                                          /* CRM Wakeup Count R 32 */
volatile CRM_WU_TIMEOUT_T   _WU_TIMEOUT;                                        /* CRM Wakeup Timeout R/W 32 */
volatile CRM_RTC_COUNT_T    _RTC_COUNT;                                         /* CRM Real Time Count R/W 32 */
volatile CRM_RTC_TIMEOUT_T  _RTC_TIMEOUT;                                       /* CRM RTC Periodic Wakeup Timeout R/W 32 */
volatile CRM_CAL_CNTL_T     _CAL_CNTL;                                          /* CRM Calibration Control R/W 32 */
volatile CRM_CAL_COUNT_T    _CAL_COUNT;                                         /* CRM Calibration XTAL Count R 32 */
volatile CRM_RINGOSC_CNTL_T _RINGOSC_CNTL;                                      /* CRM 2kHz Ring Oscillator Control R/W 32 */
volatile CRM_XTAL_CNTL_T    _XTAL_CNTL;                                         /* CRM Reference XTAL Control R/W 32 */
volatile CRM_XTAL32_CNTL_T  _XTAL32_CNTL;                                       /* CRM 32kHz XTAL Control R/W 32 */
volatile CRM_VREG_CNTL_T    _VREG_CNTL;                                         /* CRM Voltage Regulator Control R/W 32 */
volatile CRM_VREG_TRIM_T    _VREG_TRIM;                                         /* CRM Voltage Regulator Trim R/W 32 */
volatile CRM_SW_RST_T       _SW_RST;                                            /* CRM Software Reset R/W 32 */



/* ------------------------- MAC Accelerator (MACA)-------------------------- */
/*    Base Adrress = MBAR_MACA = 0x8000_4000                                  */
volatile MACA_VERSION_T                 _MACA_VERSION;                          /* MACA Version Number R 32 */
volatile MACA_RESET_T                   _MACA_RESET;                            /* MACA Reset R/W 32 */
volatile MACA_RANDOM_T                  _MACA_RANDOM;                           /* MACA Random Number R/W 32 */
volatile MACA_CONTROL_T                 _MACA_CONTROL;                          /* MACA Control W 32 */
volatile MACA_STATUS_T                  _MACA_STATUS;                           /* MACA Status R 32 */
volatile MACA_FRMPND_T                  _MACA_FRMPND;                           /* MACA Frame Pending R/W 32 */
volatile MACA_MC1322x_ID_T              _MACA_MC1322x_ID;                       /* MACA MC1322x ID */
volatile MACA_EDVALUE_T                 _MACA_EDVALUE;                          /* MACA Energy Detect Result R 32 */
volatile MACA_TMREN_T                   _MACA_TMREN;                            /* MACA Enable Timers R/W 32 */
volatile MACA_TMRDIS_T                  _MACA_TMRDIS;                           /* MACA Disable Timers R/W 32 */
volatile MACA_CLK_T                     _MACA_CLK;                              /* MACA Clock R/W 32 */
volatile MACA_STARTCLK_T                _MACA_STARTCLK;                         /* MACA Start Clock R/W 32 */
volatile MACA_CPLCLK_T                  _MACA_CPLCLK;                           /* MACA Complete Clock R/W 32 */
volatile MACA_SFTCLK_T                  _MACA_SFTCLK;                           /* MACA Soft Timeout Clock R/W 32 */
volatile MACA_CLKOFFSET_T               _MACA_CLKOFFSET;                        /* MACA Clock offset R/W 32 */
volatile MACA_RELCLK_T                  _MACA_RELCLK;                           /* MACA Relative clock R/W 32 */
volatile MACA_CPLTIM_T                  _MACA_CPLTIM;                           /* MACA Action Complete Timestamp R 32 */
volatile MACA_SLOTOFFSET_T              _MACA_SLOTOFFSET;                       /* MACA Tx Slot Offset Adjustment R/W 32 */
volatile MACA_TIMESTAMP_T               _MACA_TIMESTAMP;                        /* MACA Receive Time Stamp R 32 */
volatile MACA_DMARX_T                   _MACA_DMARX;                            /* MACA DMA Rx Data Pointer R/W 32 */
volatile MACA_DMATX_T                   _MACA_DMATX;                            /* MACA DMA Tx Data Pointer R/W 32 */
volatile MACA_DMAPOLL_T                 _MACA_DMAPOLL;                          /* MACA DMA Tx Poll Response Pointer R/W 32 */
volatile MACA_TXLEN_T                   _MACA_TXLEN;                            /* MACA Tx Length R/W 32 */
volatile MACA_TXSEQNR_T                 _MACA_TXSEQNR;                          /* MACA Tx Sequence Number R/W 32 */
volatile MACA_SETRXLVL_T                _MACA_SETRXLVL;                         /* MACA Set Rx Level Interrupt R/W 32 */
volatile MACA_GETRXLVL_T                _MACA_GETRXLVL;                         /* MACA Read Number of Received Bytes R 32 */
volatile MACA_IRQ_T                     _MACA_IRQ;                              /* MACA Interrupt R 32 */
volatile MACA_IRQ_T                     _MACA_CLRIRQ;                           /* MACA Interrupt Clear W 32 */
volatile MACA_IRQ_T                     _MACA_SETIRQ;                           /* MACA Interrupt Set W 32 */
volatile MACA_IRQ_T                     _MACA_MASKIRQ;                          /* MACA Interrupt Mask W 32 */
volatile MACA_MACPANID_T                _MACA_MACPANID;                         /* MACA PAN ID R/W 32 */
volatile MACA_MAC16ADDR_T               _MACA_MAC16ADDR;                        /* MACA Short Address R/W 32 */
volatile MACA_MAC64HI_T                 _MACA_MAC64HI;                          /* MACA High Extended Address R/W 32 */
volatile MACA_MAC64LO_T                 _MACA_MAC64LO;                          /* MACA Low Extended Address R/W 32 */
volatile MACA_FLTREJ_T                  _MACA_FLTREJ;                           /* MACA Filter Rejection Mask R/W 32 */
volatile MACA_CLKDIV_T                  _MACA_CLKDIV;                           /* MACA Clock Divider R/W 32 */
volatile MACA_WARMUP_T                  _MACA_WARMUP;                           /* MACA Warmup R/W 32 */
volatile MACA_PREAMBLE_T                _MACA_PREAMBLE;                         /* MACA Preamble R/W 32 */
volatile MACA_WHITESEED_T               _MACA_WHITESEED;                        /* MACA LFSR Whitening Seed R/W 32 */
volatile MACA_FRAMESYNC0_T              _MACA_FRAMESYNC0;                       /* MACA Frame Sync Word 0 R/W 32 */
volatile MACA_FRAMESYNC1_T              _MACA_FRAMESYNC1;                       /* MACA Frame Sync Word 1 R/W 32 */
volatile MACA_TXACKDELAY_T              _MACA_TXACKDELAY;                       /* MACA Tx Acknowledgement Delay R/W 32 */
volatile MACA_RXACKDELAY_T              _MACA_RXACKDELAY;                       /* MACA Rx Acknowledgement Delay R/W 32 */
volatile MACA_EOFDELAY_T                _MACA_EOFDELAY;                         /* MACA End of Frame Delay R/W 32 */
volatile MACA_CCADELAY_T                _MACA_CCADELAY;                         /* MACA CCA Delay R/W 32 */
volatile MACA_RXEND_T                   _MACA_RXEND;                            /* MACA Rx End R/W 32 */
volatile MACA_TXCCADELAY_T              _MACA_TXCCADELAY;                       /* MACA Tx CCA Delay R/W 32 */
volatile MACA_KEY_T                     _MACA_KEY3;                             /* MACA Key3 R 32 */
volatile MACA_KEY_T                     _MACA_KEY2;                             /* MACA Key2 R 32 */
volatile MACA_KEY_T                     _MACA_KEY1;                             /* MACA Key1 R 32 */
volatile MACA_KEY_T                     _MACA_KEY0;                             /* MACA Key0 R 32 */
volatile MACA_OPTIONS_T                 _MACA_OPTIONS;                          /* MACA Options W 32 */



/* ------------------------- UART Module (UART1)   -------------------------- */
/*    Base Adrress = MBAR_UART1 = 0x8000_5000                                 */
//volatile UART1_UCON_T           _UART1_UCON;                                    /* UART Control R/W 8 */
//volatile UART1_USTAT_T          _UART1_USTAT;                                   /* UART Status R 8 */
//volatile UART1_UDATA_T          _UART1_UDATA;                                   /* UART Data R/W 8 */
//volatile UART1_URXCON_T         _UART1_URXCON;                                  /* UART RxBuffer Control R/W 8 */
//volatile UART1_UTXCON_T         _UART1_UTXCON;                                  /* UART TxBuffer Control R/W 8 */
//volatile UART1_UCTS_T           _UART1_UCTS;                                    /* UART CTS Level Control R/W 8 */
//volatile UART1_UBRCNT_T         _UART1_UBRCNT;                                  /* UBRINC — Fractional Divider/UBRMOD — Fractional Divider R/W 8 */



/* --------------------------- Timer Module (TMR) --------------------------- */
/*    Base Adrress = MBAR_TMR0 = 0x80007000                                   */
volatile TMR0_COMP1_T       _TMR0_COMP1;                                        /* TMR Channel 0 Compare Register #1 R/W 16 */
volatile TMR0_COMP2_T       _TMR0_COMP2;                                        /* TMR Channel 0 Compare Register #2 R/W 16 */
volatile TMR0_CAPT_T        _TMR0_CAPT;                                         /* TMR Channel 0 Capture Register R/W 16 */
volatile TMR0_LOAD_T        _TMR0_LOAD;                                         /* TMR Channel 0 Load Register R/W 16 */
volatile TMR0_HOLD_T        _TMR0_HOLD;                                         /* TMR Channel 0 Hold Register R/W 16 */
volatile TMR0_CNTR_T        _TMR0_CNTR;                                         /* TMR Channel 0 Counter Register R/W 16 */
volatile TMR_CTRL_T         _TMR0_CTRL;                                         /* TMR Channel 0 Control Register R/W 16 */
volatile TMR_SCTRL_T        _TMR0_SCTRL;                                        /* TMR Channel 0 Status and Control Register R/W 16 */
volatile TMR0_CMPLD1_T      _TMR0_CMPLD1;                                       /* TMR Channel 0 Comparator Load Register 1 R/W 16 */
volatile TMR0_CMPLD2_T      _TMR0_CMPLD2;                                       /* TMR Channel 0 Comparator Load Register 2 R/W 16 */
volatile TMR_CSCTRL_T       _TMR0_CSCTRL;                                       /* TMR Channel 0 Comparator Status and Control Register R/W 16 */

volatile TMR0_ENBL_T        _TMR0_ENBL;                                         /* TMR Channel Enable Register R/W 16 */

/*    Base Adrress = MBAR_TMR1 = 0x80007020                                   */
volatile TMR1_COMP1_T       _TMR1_COMP1;                                        /* TMR Channel 1 Compare Register #1 R/W 16 */
volatile TMR1_COMP2_T       _TMR1_COMP2;                                        /* TMR Channel 1 Compare Register #2 R/W 16 */
volatile TMR1_CAPT_T        _TMR1_CAPT;                                         /* TMR Channel 1 Capture Register R/W 16 */
volatile TMR1_LOAD_T        _TMR1_LOAD;                                         /* TMR Channel 1 Load Register R/W 16 */
volatile TMR1_HOLD_T        _TMR1_HOLD;                                         /* TMR Channel 1 Hold Register R/W 16 */
volatile TMR1_CNTR_T        _TMR1_CNTR;                                         /* TMR Channel 1 Counter Register R/W 16 */
volatile TMR_CTRL_T         _TMR1_CTRL;                                         /* TMR Channel 1 Control Register R/W 16 */
volatile TMR_SCTRL_T        _TMR1_SCTRL;                                        /* TMR Channel 1 Status and Control Register R/W 16 */
volatile TMR1_CMPLD1_T      _TMR1_CMPLD1;                                       /* TMR Channel 1 Comparator Load Register 1 R/W 16 */
volatile TMR1_CMPLD2_T      _TMR1_CMPLD2;                                       /* TMR Channel 1 Comparator Load Register 2 R/W 16 */
volatile TMR_CSCTRL_T       _TMR1_CSCTRL;                                       /* TMR Channel 1 Comparator Status and Control Register R/W 16 */

/*    Base Adrress = MBAR_TMR2 = 0x80007040                                   */
volatile TMR2_COMP1_T       _TMR2_COMP1;                                        /* TMR Channel 2 Compare Register #1 R/W 16 */
volatile TMR2_COMP2_T       _TMR2_COMP2;                                        /* TMR Channel 2 Compare Register #2 R/W 16 */
volatile TMR2_CAPT_T        _TMR2_CAPT;                                         /* TMR Channel 2 Capture Register R/W 16 */
volatile TMR2_LOAD_T        _TMR2_LOAD;                                         /* TMR Channel 2 Load Register R/W 16 */
volatile TMR2_HOLD_T        _TMR2_HOLD;                                         /* TMR Channel 2 Hold Register R/W 16 */
volatile TMR2_CNTR_T        _TMR2_CNTR;                                         /* TMR Channel 2 Counter Register R/W 16 */
volatile TMR_CTRL_T         _TMR2_CTRL;                                         /* TMR Channel 2 Control Register R/W 16 */
volatile TMR_SCTRL_T        _TMR2_SCTRL;                                        /* TMR Channel 2 Status and Control Register R/W 16 */
volatile TMR2_CMPLD1_T      _TMR2_CMPLD1;                                       /* TMR Channel 2 Comparator Load Register 1 R/W 16 */
volatile TMR2_CMPLD2_T      _TMR2_CMPLD2;                                       /* TMR Channel 2 Comparator Load Register 2 R/W 16 */
volatile TMR_CSCTRL_T       _TMR2_CSCTRL;                                       /* TMR Channel 2 Comparator Status and Control Register R/W 16 */

/*    Base Adrress = MBAR_TMR3 = 0x80007060                                   */
volatile TMR3_COMP1_T       _TMR3_COMP1;                                        /* TMR Channel 3 Compare Register #1 R/W 16 */
volatile TMR3_COMP2_T       _TMR3_COMP2;                                        /* TMR Channel 3 Compare Register #2 R/W 16 */
volatile TMR3_CAPT_T        _TMR3_CAPT;                                         /* TMR Channel 3 Capture Register R/W 16 */
volatile TMR3_LOAD_T        _TMR3_LOAD;                                         /* TMR Channel 3 Load Register R/W 16 */
volatile TMR3_HOLD_T        _TMR3_HOLD;                                         /* TMR Channel 3 Hold Register R/W 16 */
volatile TMR3_CNTR_T        _TMR3_CNTR;                                         /* TMR Channel 3 Counter Register R/W 16 */
volatile TMR_CTRL_T         _TMR3_CTRL;                                         /* TMR Channel 3 Control Register R/W 16 */
volatile TMR_SCTRL_T        _TMR3_SCTRL;                                        /* TMR Channel 3 Status and Control Register R/W 16 */
volatile TMR3_CMPLD1_T      _TMR3_CMPLD1;                                       /* TMR Channel 3 Comparator Load Register 1 R/W 16 */
volatile TMR3_CMPLD2_T      _TMR3_CMPLD2;                                       /* TMR Channel 3 Comparator Load Register 2 R/W 16 */
volatile TMR_CSCTRL_T       _TMR3_CSCTRL;                                       /* TMR Channel 3 Comparator Status and Control Register R/W 16 */



/* -------------------- Advanced Security Module (ASM) ---------------------- */
/*    Base Adrress = MBAR_ASM = 0x8000_8000                                   */
volatile ASM_KEY_T            _ASM_KEY0;                                        /* 128-BIT ENCRYPTION KEY (1 of 4) R/W 32 */
volatile ASM_KEY_T            _ASM_KEY1;                                        /* 128-BIT ENCRYPTION KEY (2 of 4) R/W 32 */
volatile ASM_KEY_T            _ASM_KEY2;                                        /* 128-BIT ENCRYPTION KEY (3 of 4) R/W 32 */
volatile ASM_KEY_T            _ASM_KEY3;                                        /* 128-BIT ENCRYPTION KEY (4 of 4) R/W 32 */
volatile ASM_DATA_T           _ASM_DATA0;                                       /* 128-BIT DATA Register (1 of 4) R/W 32 */
volatile ASM_DATA_T           _ASM_DATA1;                                       /* 128-BIT DATA Register (2 of 4) R/W 32 */
volatile ASM_DATA_T           _ASM_DATA2;                                       /* 128-BIT DATA Register (3 of 4) R/W 32 */
volatile ASM_DATA_T           _ASM_DATA3;                                       /* 128-BIT DATA Register (4 of 4) R/W 32 */
volatile ASM_CTR_T            _ASM_CTR0;                                        /* 128-BIT COUNTER Register (1 of 4) R/W 32 */
volatile ASM_CTR_T            _ASM_CTR1;                                        /* 128-BIT COUNTER Register (2 of 4) R/W 32 */
volatile ASM_CTR_T            _ASM_CTR2;                                        /* 128-BIT COUNTER Register (3 of 4) R/W 32 */
volatile ASM_CTR_T            _ASM_CTR3;                                        /* 128-BIT COUNTER Register (4 of 4) R/W 32 */
volatile ASM_CTR_RESULT_T     _ASM_CTR_RESULT0;                                 /* 128-BIT COUNTER RESULT Register (1 of 4) R 32 */
volatile ASM_CTR_RESULT_T     _ASM_CTR_RESULT1;                                 /* 128-BIT COUNTER RESULT Register (2 of 4) R 32 */
volatile ASM_CTR_RESULT_T     _ASM_CTR_RESULT2;                                 /* 128-BIT COUNTER RESULT Register (3 of 4) R 32 */
volatile ASM_CTR_RESULT_T     _ASM_CTR_RESULT3;                                 /* 128-BIT COUNTER RESULT Register (4 of 4) R 32 */
volatile ASM_CBC_RESULT_T     _ASM_CBC_RESULT0;                                 /* 128-BIT CBC MAC RESULT Register (1 of 4) R 32 */
volatile ASM_CBC_RESULT_T     _ASM_CBC_RESULT1;                                 /* 128-BIT CBC MAC RESULT Register (2 of 4) R 32 */
volatile ASM_CBC_RESULT_T     _ASM_CBC_RESULT2;                                 /* 128-BIT CBC MAC RESULT Register (3 of 4) R 32 */
volatile ASM_CBC_RESULT_T     _ASM_CBC_RESULT3;                                 /* 128-BIT CBC MAC RESULT Register (4 of 4) R 32 */
volatile ASM_CONTROL0_T       _ASM_CONTROL0;                                    /* CONTROL 0 Register (Self Clearing) W 32 */
volatile ASM_CONTROL1_T       _ASM_CONTROL1;                                    /* CONTROL 1 Register R/W 32 */
volatile ASM_STATUS_T         _ASM_STATUS;                                      /* STATUS Register R 32 */
volatile ASM_MAC_T            _ASM_MAC0;                                        /* 128-BIT CBC MAC Register (1 of 4) R/W 32 */
volatile ASM_MAC_T            _ASM_MAC1;                                        /* 128-BIT CBC MAC Register (2 of 4) R/W 32 */
volatile ASM_MAC_T            _ASM_MAC2;                                        /* 128-BIT CBC MAC Register (3 of 4) R/W 32 */
volatile ASM_MAC_T            _ASM_MAC3;                                        /* 128-BIT CBC MAC Register (4 of 4) R/W 32 */



/* -------------------- Radio Analog Functions ----------------------------- */
/*    Base Adrress = MBAR_RF = 0x8000_A000                                   */
volatile PA_CTRL_T            _PA_CTRL;                                         /* Transmit Power Amp Control W 32 */
volatile PA_ADJ_T             _PA_ADJ;                                          /* Transmit Power Adjust W 32 */



/* ------------- Analog to Digital Converter Module (ADC)-------------------- */
/*    Base Adrress = MBAR_ADC = 0x8000D000                                    */
volatile ADC_COMP_T           _ADC_COMP_0;                                      /* Monitor GP-ADC Pins Threshold detection w/o MCU intervention to generate IRQ R/W 16 */
volatile ADC_COMP_T           _ADC_COMP_1;                                      /* Monitor GP-ADC Pins Threshold R/W 16 */
volatile ADC_COMP_T           _ADC_COMP_2;                                      /* Monitor GP-ADC Pins Threshold R/W 16 */
volatile ADC_COMP_T           _ADC_COMP_3;                                      /* Monitor GP-ADC Pins Threshold R/W 16 */
volatile ADC_COMP_T           _ADC_COMP_4;                                      /* Monitor GP-ADC Pins Threshold R/W 16 */
volatile ADC_COMP_T           _ADC_COMP_5;                                      /* Monitor GP-ADC Pins Threshold R/W 16 */
volatile ADC_COMP_T           _ADC_COMP_6;                                      /* Monitor GP-ADC Pins Threshold R/W 16 */
volatile ADC_COMP_T           _ADC_COMP_7;                                      /* Monitor GP-ADC Pins Threshold R/W 16 */
volatile ADC_BAT_COMP_T       _ADC_BAT_COMP_OVER;                               /* Battery Voltage Upper Trip Point (ADC1 only) R/W 16 */
volatile ADC_BAT_COMP_T       _ADC_BAT_COMP_UNDER;                              /* Battery Voltage Lower Trip Point (ADC1 only) R/W 16 */
volatile ADC_SEQ_1_T          _ADC_SEQ_1;                                       /* Monitor GP-ADC Pins for ADC1 R/W 16 */
volatile ADC_SEQ_2_T          _ADC_SEQ_2;                                       /* Monitor GP-ADC Pins for ADC2 R/W 16 */
volatile ADC_CONTROL_T        _ADC_CONTROL;                                     /* Primary Module Control Register R/W 16 */
volatile ADC_TRIGGERS_T       _ADC_TRIGGERS;                                    /* Triggered Channels Register R 16 */
volatile ADC_PRESCALE_T       _ADC_PRESCALE;                                    /* Bus Clock Divide Register (8-bit prescaler for use with 26MHz) R/W 16 ??? */
volatile ADC_FIFO_READ_T      _ADC_FIFO_READ;                                   /* ADC FIFO Read (8 deep FIFO) R 16 */
volatile ADC_FIFO_CONTROL_T   _ADC_FIFO_CONTROL;                                /* ADC Interrupt Level Control R/W 16 */
volatile ADC_FIFO_STATUS_T    _ADC_FIFO_STATUS;                                 /* ADC FIFO Status R 16 */
volatile ADC_1_SR_HIGH_T      _ADC_1_SR_HIGH;                                   /* ADC 1 Sample Rate High R/W 16 */
volatile ADC_1_SR_LOW_T       _ADC_1_SR_LOW;                                    /* ADC 1 Sample Rate Low R/W 16 */
volatile ADC_2_SR_HIGH_T      _ADC_2_SR_HIGH;                                   /* ADC 2 Sample Rate High R/W 16 */
volatile ADC_2_SR_LOW_T       _ADC_2_SR_LOW;                                    /* ADC 2 Sample Rate Low R/W 16 */
volatile ADC_ON_TIME_T        _ADC_ON_TIME;                                     /* ADC TurnOn Time R/W 16 */
volatile ADC_CONVERT_TIME_T   _ADC_CONVERT_TIME;                                /* ADC Convert Time R/W 16 */
volatile ADC_CLOCK_DIV_T      _ADC_CLOCK_DIV;                                   /* ADC Clock Divider R/W 16 */
volatile ADC_OVERRIDE_T       _ADC_OVERRIDE;                                    /* ADC Manual Control R/W 16 */
volatile ADC_IRQ_T            _ADC_IRQ;                                         /* ADC Read/Clear Active Interrupts R/W 16 */
volatile ADC_MODE_T           _ADC_MODE;                                        /* ADC Mode R/W 16 */
volatile ADC_1_RESULT_T       _ADC_1_RESULT;                                    /* ADC 1 Result R 16 */
volatile ADC_2_RESULT_T       _ADC_2_RESULT;                                    /* ADC 2 Result R 16 */



/* ---------------------- Interrupt Controller (ITC) ------------------------ */
/*    Base Adrress = MBAR_AITC = 0x80020000                                   */
volatile ITC_INTCNTL_T      _ITC_INTCNTL;                                       /* ITC Interrupt Controller R/W 32 */
volatile ITC_NIMASK_T       _ITC_NIMASK;                                        /* ITC Normal Interrupt Mask R/W 32 */
volatile ITC_INTENNUM_T     _ITC_INTENNUM;                                      /* ITC Interrupt Enable Number W 32 */
volatile ITC_INTDISNUM_T    _ITC_INTDISNUM;                                     /* ITC Interrupt Disable Number W 32 */
volatile ITC_INTENABLE_T    _ITC_INTENABLE;                                     /* ITC Interrupt Enable R/W 32 */
volatile ITC_INTTYPE_T      _ITC_INTTYPE;                                       /* ITC Interrupt Type R/W 32 */
volatile ITC_NIVECTOR_T     _ITC_NIVECTOR;                                      /* ITC Normal Interrupt R 32 */
volatile ITC_FIVECTOR_T     _ITC_FIVECTOR;                                      /* ITC Fast Interrupt R 32 */
volatile ITC_INTSRC_T       _ITC_INTSRC;                                        /* ITC Interrupt Source R 32 */
volatile ITC_INTFRC_T       _ITC_INTFRC;                                        /* ITC Interrupt Force R/W 32 */
volatile ITC_NIPEND_T       _ITC_NIPEND;                                        /* ITC Normal Interrupt Pending R 32 */
volatile ITC_FIPEND_T       _ITC_FIPEND;                                        /* ITC Fast Interrupt Pending R 32 */



/* ---------------------- I2C Module (I2C) ------------------------- */
/*            Base Adrress = MBAR_I2C = 0x80006000                   */
volatile I2CADDR_T    _I2CADDR;                                                 /* I2C address register R/W 8 bit only */
volatile I2CFDR_T     _I2CFDR;                                                  /* I2C frequency divider register R/W 8 bit only */
volatile I2CCR_T      _I2CCR;                                                   /* I2C control register R/W 8 bit only */
volatile I2CSR_T      _I2CSR;                                                   /* I2C status register R/W 8 bit only */
volatile I2CDR_T      _I2CDR;                                                   /* I2C data register R/W 8 bit only */
volatile I2CDFSRR_T   _I2CDFSRR;                                                /* I2C digital filter sampling rate register R/W 8 bit only */
volatile I2CCKER_T    _I2CCKER;                                                 /* I2C clock enable register R/W 8 bit only */

