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
* Name:         itc.c
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This file is provided ... Interrupt controller
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#include "itc.h"

void ItcInit(void)
{
  ITC_Init();
  IntDisableAll();
  
  IntAssignHandler(gMacaInt_c, (IntHandlerFunc_t)MACA_Interrupt);
  ITC_SetPriority(gMacaInt_c, gItcNormalPriority_c);
  ITC_EnableInterrupt(gMacaInt_c);
  IntAssignHandler(gTmrInt_c, (IntHandlerFunc_t)/*TmrIsr*/TMR_Interrupt);
  ITC_SetPriority(gTmrInt_c, gItcNormalPriority_c);
  ITC_EnableInterrupt(gTmrInt_c);
}

