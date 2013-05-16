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

/*******************************************************************************
* Name:         spif_interface.c
* Author:       Dorin Pavel
* Date:         Nov 2007
* Description:  serial flash interface, read/erase/write access  
* Changes:
* Revisions:
*******************************************************************************/

#include "typedef.h"
#include "spif_interface.h"
#include "digitals.h"

/***************************[ public includes ]*******************************/
/***************************[ public defines ]********************************/

/***************************[ public datatypes ]******************************/
/***************************[ public variables ]******************************/
/***************************[ public functions ]******************************/


void NVM_FlashInit( void )
{
  // NVM_powerUp
  crmVRegCntl_t VRegCntl;
  
  VRegCntl.vReg = g1P8VReg_c;
  VRegCntl.cntl.vReg1P8VEn =1;
  
  CRM_VRegCntl(&VRegCntl);
  
  DelayLoop(8*1000); // min 1ms
  
  CRM_VREG_CNTL.vReg1P8VEn = 1; // NVM_StartNvmRegulator();
  while( !CRM_STATUS.vReg1P8VRdy ) // Wait for regulator to become active
      ;  
  
  DelayLoop( 1000 );
}
