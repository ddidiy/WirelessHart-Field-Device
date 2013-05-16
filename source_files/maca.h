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
* Name:         maca.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_MACA_H_
#define _NIVIS_MACA_H_

#include <string.h>
#include <stdio.h>
 
#include "typedef.h"
#include "global.h"
#include "MC1322x.h"
#include "digitals.h"
#include "WirelessHART/phy_layer.h"
#include "WirelessHART/datalink_layer.h"

//#define MASK_MACA_IRQ       ( 0x0000FC1F )	
//#define MASK_IRQ_ALL        ( 0x0000FE1F )	
//#define MASK_MACA_IRQ       ( MASK_IRQ_STRT | MASK_IRQ_SYNC | MASK_IRQ_CRC | MASK_IRQ_DI | MASK_IRQ_ACPL )
//#define MASK_MACA_IRQ       ( MASK_IRQ_CRC | MASK_IRQ_ACPL )
//#define MASK_MACA_IRQ       ( MASK_IRQ_ACPL )
//#define MASK_MACA_IRQ       ( MASK_IRQ_STRT |  MASK_IRQ_CRC | MASK_IRQ_DI | MASK_IRQ_ACPL )

//#define MASK_MACA_IRQ       ( MASK_IRQ_CRC | MASK_IRQ_DI | MASK_IRQ_ACPL )

#define MASK_IRQ_ACPL	    ( 0x00000001 )
#define MASK_IRQ_POLL	    ( 0x00000002 )
#define MASK_IRQ_DI         ( 0x00000004 )
#define MASK_IRQ_WU	    ( 0x00000008 )
#define MASK_IRQ_RST	    ( 0x00000010 )
#define MASK_IRQ_LVL	    ( 0x00000200 )
#define MASK_IRQ_SFT	    ( 0x00000400 )
#define MASK_IRQ_FLT	    ( 0x00000800 )
#define MASK_IRQ_CRC	    ( 0x00001000 )
#define MASK_IRQ_CM	    ( 0x00002000 )
#define MASK_IRQ_SYNC	    ( 0x00004000 )
#define MASK_IRQ_STRT	    ( 0x00008000 )

#define US_PER_TICK         ( 4 )
#define TX_WARM             ( 144 / US_PER_TICK )
#define RX_WARM             ( 144 / US_PER_TICK )
#define CCA_WARM            ( 144 / US_PER_TICK )
#define CCA_LEN             ( 144 / US_PER_TICK )
#define IFS                 ( 192 / US_PER_TICK )

extern const uint8 gaRFSynVCODivI_c[16];
extern const uint32 gaRFSynVCODivF_c[16];
#define SET_CHANNEL(x)      SetChannel(x,gaRFSynVCODivI_c[x],gaRFSynVCODivF_c[x])

#define SET_STARTCLK(x)       ( MACA_STARTCLK = MACA_CLK + (x) )
#define SET_CPLCLK(x)         ( MACA_CPLCLK = MACA_CLK + (x) )
#define SET_SFTCLK(x)         ( MACA_SFTCLK = MACA_CLK + (x) )

#define MACA_RESET_ON        (1<<0)
#define MACA_RESET_CLK_0N    (1<<1)

#ifdef TRIGGER_RX_LEVEL
  #define MACA_RX_LEVEL_TRIGGER       18
#endif

// MACA STATUS CODES FOR ACTION COMPLETE ( = COMPLETE_CODE )
#define STATUS_CODE_MASK              0x0000000F
#define STATUS_CODE_SUCCESS           0
#define STATUS_CODE_TIMEOUT           1
#define STATUS_CODE_CHANNEL_BUSY      2
#define STATUS_CODE_CRC_FAIL          3
#define STATUS_CODE_ABORTED           4
#define STATUS_CODE_NO_ACK            5
#define STATUS_CODE_NO_DATA           6
#define STATUS_CODE_LATE_START        7
#define STATUS_CODE_EXT_TIMEOUT       8
#define STATUS_CODE_EXT_PND_TIMEOUT   9
#define STATUS_CODE_NU1               10
#define STATUS_CODE_NU2               11
#define STATUS_CODE_NU3               12
#define STATUS_CODE_EXTERNAL_ABORT    13
#define STATUS_CODE_NOT_COMPLETED     14
#define STATUS_CODE_BUS_ERROR         15

// MACA CONTROL
// CONTROL CODES BITS FOR AUTO SEQUENCE
#define CTRL_SEQ_NOP                  0
#define CTRL_SEQ_ABORT                1
#define CTRL_SEQ_WAIT                 2
#define CTRL_SEQ_TX                   3
#define CTRL_SEQ_RX                   4
#define CTRL_SEQ_TXPOLL               5
#define CTRL_SEQ_CCA                  6
#define CTRL_SEQ_ED                   7
// CONTROL CODES BITS FOR MODE
#define CTRL_MODE_NO_CCA              ( 0x00000000 )
#define CTRL_MODE_NO_SLOTTED          ( 0x00000008 )  // 1 <<  3
#define CTRL_MODE_SLOTTED             ( 0x00000010 )  // 1 <<  4
// OTHERS
#define CTRL_AUTO                     ( 0x00000080 )  // 1 <<  7  // 1 continuous rx, rx only once
#define CTRL_BCN                      ( 0x00000100 )  // 1 <<  8  // 1 beacon only, 0 for a
#define CTRL_ASAP                     ( 0x00000200 )  // 1 <<  9  // 1 start now, 0 timer start
#define CTRL_RELATIVE                 ( 0x00000400 )  // 1 << 10  // 1 for relative, 0 for absolute
#define CTRL_PRM                      ( 0x00000800 )  // 1 << 11  // set for promiscuous mode
#define CTRL_NOFC                     ( 0x00001000 )  // 1 << 12  // set to disable FCS
#define CTRL_ROLE                     ( 0x00002000 )  // 1 << 13  // set if PAN coordinator
#define CTRL_RST_SLOT                 ( 0x00008000 )  // 1 << 15  // reset slot counter
#define CTRL_ISM                      ( 0x00100000 )  // 1 << 20  // 0 = 802 , 1 = turbo link

// MACA ACTION TRIGGERS ( TMREN, TMRDIS )
#define CLK_START                     ( 0x01 )        // TMREN & TMRDIS enable/disable start clock
#define CLK_CPL                       ( 0x02 )        // TMREN & TMRDIS enable/disable complete clock
#define CLK_SOFT                      ( 0x04 )        // TMREN & TMRDIS enable/disable soft complete clock
#define CLK_START_ABORT               ( 0x08 )        // TMRDIS abort start clock
#define CLK_CPL_ABORT                 ( 0x10 )        // TMRDIS abort complete clock
#define CLK_SOFT_ABORT                ( 0x20 )        // TMRDIS abort soft complete clock

// functions
void MACA_Init(void);
void MACA_Interrupt(void);
void MACA_Reset(void);

extern uint16 g_unWatchDogRTC;
extern uint8  g_unPllLockWachDog;

void MACA_WatchDog(void);

inline void MACA_ResetWatchDog(void) {g_unWatchDogRTC = 0;}


// extern functions and definitions from Freescale = from library
typedef enum FuncReturn_tag
{
  gSuccess_c = 0,
  gFailOutOfRange_c,
  gFailNoResourcesAvailable_c,
  gFailNoValidCondition_c,
  gFailBusy_c,
  gFailCorrupted,
  gFailTest_c,
  gAuthenticationFailed_c,
  gAuthenticationPassed_c,
  gFail_c
} FuncReturn_t;

typedef enum DemTypes_tag
{
  NCD = 0,
  DCD
}DemTypes_t;

#define pa_switch_bit       (  1 << 13 )
#define pa_lowpwr_en_bit    (  1 << 12 )
#define pa_highpwr_en_bit   (  1 << 11 )
#define pa_trim_default     ( 32 <<  4 )


//#define MACA_CLRIRQ_MASK 0xFFFF
//#define MACA_CLRIRQ_MASK ( MASK_IRQ_STRT | MASK_IRQ_SYNC | MASK_IRQ_CM | MASK_IRQ_CRC \
//                              | MASK_IRQ_FLT | MASK_IRQ_SFT | MASK_IRQ_LVL | MASK_IRQ_DI | MASK_IRQ_POLL | MASK_IRQ_ACPL )
#define MACA_CLRIRQ_MASK ( MASK_IRQ_STRT | MASK_IRQ_SYNC | MASK_IRQ_CM | MASK_IRQ_CRC \
                              | MASK_IRQ_FLT | MASK_IRQ_SFT | MASK_IRQ_LVL | MASK_IRQ_DI | MASK_IRQ_ACPL )

extern uint8_t SetPower(uint8_t);
extern void SetComplementaryPAState(bool_t);
extern uint8_t SetPowerLevelLockMode(bool_t);
extern void SetDemulatorMode(DemTypes_t demodulator);
//extern void disable_test_mode_continuos_tx_nomod(void);
//extern void set_test_mode_continuos_tx_nomod(void);

extern void SetEdCcaThreshold(uint8_t u8CcaThreshold);
extern uint8 PhyPlmeGetLQI(void);

extern void RadioInit(uint32 u32ClockFreq, uint8 u8PN, uint32 u32LoopDiv); //extern void RadioInit(void);
extern void SetChannel(uint8 channel,  uint8 RFSynVCODivI, uint32 RFSynVCODivF);

#define GET_LAST_RSSI() (int16)(((((uint16)g_ucLastLQI) * 85) >> 8) - 100 )
extern uint8 g_ucLastLQI;

#endif /* _NIVIS_MACA_H_ */

