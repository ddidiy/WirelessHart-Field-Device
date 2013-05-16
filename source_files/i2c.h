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
* Name:         i2c.h
* Author:       Dorin Muica
* Date:         2009
* Description:  
*           
* Changes:
* Revisions:
****************************************************************************************************/
#include "digitals.h"
#include "LibInterface/I2C_Interface.h"

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



//////////////////////////////////////////////put in MC1322x.c !!!!!!!!!///////////////////////////////////////
/* ---------------------- I2C Module (I2C) ------------------------- */
/*            Base Adrress = MBAR_I2C = 0x80006000                   */
volatile I2CADDR_T    _I2CADDR;                                                 /* I2C address register R/W 8 bit only */
volatile I2CFDR_T     _I2CFDR;                                                  /* I2C frequency divider register R/W 8 bit only */
volatile I2CCR_T      _I2CCR;                                                   /* I2C control register R/W 8 bit only */
volatile I2CSR_T      _I2CSR;                                                   /* I2C status register R/W 8 bit only */
volatile I2CDR_T      _I2CDR;                                                   /* I2C data register R/W 8 bit only */
volatile I2CDFSRR_T   _I2CDFSRR;                                                /* I2C digital filter sampling rate register R/W 8 bit only */
volatile I2CCKER_T    _I2CCKER;                                                 /* I2C clock enable register R/W 8 bit only */
