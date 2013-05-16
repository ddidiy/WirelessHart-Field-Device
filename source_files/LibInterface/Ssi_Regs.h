/************************************************************************************
* This header file is provided as an interface to hardware
* This file holds definitions of the peripheral registers  
*
*
* Author(s): B06485
*
* (c) Copyright 2006, Freescale, Inc.  All rights reserved.
* 
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
* Revision history:
*   date (DD.MM.YYYY)       Author                       Comments
*   ----------           ------------------------         -------
*   25.06.2007                               B06485       Created
*
************************************************************************************/
#ifndef _SSI_REGS_H_
#define _SSI_REGS_H_

#include "EmbeddedTypes.h"


#define SSI_STX            (SSI_REGS_P->STX)
#define SSI_SRX            (SSI_REGS_P->SRX)
#define SSI_SCR_BIT        ((volatile SsiCntlReg_t*)&SSI_REGS_P->SCR)->bit
#define SSI_SCR_WORD       ((volatile SsiCntlReg_t*)&SSI_REGS_P->SCR)->word
#define SSI_SISR_BIT       ((volatile SsiISReg_t*)&SSI_REGS_P->SISR)->bit
#define SSI_SISR_WORD      ((volatile SsiISReg_t*)&SSI_REGS_P->SISR)->word
#define SSI_SIER_BIT       ((volatile SsiIEReg_t*)&SSI_REGS_P->SIER)->bit
#define SSI_SIER_WORD      ((volatile SsiIEReg_t*)&SSI_REGS_P->SIER)->word
#define SSI_STCR_BIT       ((volatile SsiTCReg_t*)&SSI_REGS_P->STCR)->bit
#define SSI_STCR_WORD      ((volatile SsiTCReg_t*)&SSI_REGS_P->STCR)->word
#define SSI_SRCR_BIT       ((volatile SsiRCReg_t*)&SSI_REGS_P->SRCR)->bit
#define SSI_SRCR_WORD      ((volatile SsiRCReg_t*)&SSI_REGS_P->SRCR)->word
#define SSI_STCCR_BIT      ((volatile SsiTCCReg_t*)&SSI_REGS_P->STCCR)->bit
#define SSI_STCCR_WORD     ((volatile SsiTCCReg_t*)&SSI_REGS_P->STCCR)->word
#define SSI_SFCSR_BIT      ((volatile SsiFCSReg_t*)&SSI_REGS_P->SFCSR)->bit
#define SSI_SFCSR_WORD     ((volatile SsiFCSReg_t*)&SSI_REGS_P->SFCSR)->word
#define SSI_SOR_BIT        ((volatile SsiOptReg_t*)&SSI_REGS_P->SOR)->bit
#define SSI_SOR_WORD       ((volatile SsiOptReg_t*)&SSI_REGS_P->SOR)->word
#define SSI_STMSK          (SSI_REGS_P->STMSK)
#define SSI_SRMSK          (SSI_REGS_P->SRMSK)

//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
   uint32_t SSIEN        :1;
   uint32_t TE           :1;
   uint32_t RE           :1;
   uint32_t NET          :1;
   uint32_t              :1;
   uint32_t I2S_MODE     :2;
   uint32_t              :2;
   uint32_t CLK_IST      :1;
   uint32_t TFR_CLK_DIS  :1;
   uint32_t RFR_CLK_DIS  :1;
 }bit;
}SsiCntlReg_t;
//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
  uint32_t TFE            :1;
  uint32_t                :1;
  uint32_t RFF            :1;
  uint32_t                :1;
  uint32_t RLS            :1;
  uint32_t TLS            :1;
  uint32_t RFS            :1;
  uint32_t TFS            :1;
  uint32_t TUE            :1;
  uint32_t                :1;
  uint32_t ROE            :1;
  uint32_t                :1;
  uint32_t TDE            :1;
  uint32_t                :1;
  uint32_t RDR            :1;
  uint32_t                :8;
  uint32_t TFRC           :1;
  uint32_t RFRC           :1;
 }bit;
}SsiISReg_t;
//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
  uint32_t TFE_EN         :1;
  uint32_t                :1;
  uint32_t RFF_EN         :1;
  uint32_t                :1;
  uint32_t RLS_EN         :1;
  uint32_t TLS_EN         :1;
  uint32_t RFS_EN         :1;
  uint32_t TFS_EN         :1;
  uint32_t TUE_EN         :1;
  uint32_t                :1;
  uint32_t ROE_EN         :1;
  uint32_t                :1;
  uint32_t TDE_EN         :1;
  uint32_t                :1;
  uint32_t RDR_EN         :1;
  uint32_t                :4;
  uint32_t TIE            :1;
  uint32_t                :1;
  uint32_t RIE            :1;
  uint32_t                :8;
  uint32_t TFRC_EN        :1;
  uint32_t RFRC_EN        :1;
 }bit;
}SsiIEReg_t;
//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
  uint32_t TEFS           :1;
  uint32_t TFSL           :1;
  uint32_t TFSI           :1;
  uint32_t TSCKP          :1;
  uint32_t TSHFD          :1;
  uint32_t TXDIR          :1;
  uint32_t TFDIR          :1;
  uint32_t TFEN           :1;
  uint32_t                :1;
  uint32_t TXBIT0         :1; 
 }bit;
}SsiTCReg_t;
//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
  uint32_t REFS           :1;
  uint32_t RFSL           :1;
  uint32_t RFSI           :1;
  uint32_t RSCKP          :1;
  uint32_t RSHFD          :1;
  uint32_t RXDIR          :1;
  uint32_t RFDIR          :1;
  uint32_t RFEN           :1;
  uint32_t                :1;
  uint32_t RXBIT0         :1; 
  uint32_t RXEXT          :1;
 }bit;
}SsiRCReg_t;
//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
  uint32_t PM             :8;
  uint32_t DC             :5;
  uint32_t WL             :4;
  uint32_t PSR            :1;
  uint32_t DIV2           :1;
 }bit;
}SsiTCCReg_t;
//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
  uint32_t TFWM0          :4;
  uint32_t RFWM0          :4;
  uint32_t TFCNT0         :4;
  uint32_t RFCNT0         :4;
 }bit;
}SsiFCSReg_t;
//------------------------------------------------------------------------
typedef union
{
 uint32_t word;
 struct
 {
  uint32_t SYNRST         :1;
  uint32_t                :2;
  uint32_t INIT           :1;
  uint32_t CLKOFF         :1;  
 }bit;
}SsiOptReg_t;
//------------------------------------------------------------------------
#endif /*_SSI_REGS_H_ */
