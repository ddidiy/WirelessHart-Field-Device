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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file       tmr_util.h
/// @verbatim   
/// Author:       Nivis LLC, Ion Ticus
/// Date:         December 2008
/// Description:  Timer translations
/// Changes:      Created 
/// Revisions:    1.0
/// @endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _NIVIS_TMR_UTIL_H_
#define _NIVIS_TMR_UTIL_H_

  #include "../system.h"

  // fraction  = 1 / 2^^20 sec
  // fraction2 = 1 / 2^^15 sec
  // usec      = 1 / 10^^6 sec

  // 2 ^^ 15 / 10^^6 =  2147 / 2^^16
  #define USEC_TO_FRACTION2(x) ((((uint32)(x)) * 2147) >> 16)

  // 2 ^^ 15 
  #define SEC_TO_FRACTION2(x) (((uint32)(x)) * 32768)
  
  //  10 ^^ 6 / 2^^15  =  5 ^^ 6 / 2 ^^ 9
  #define FRACTION2_TO_USEC(x) ((((uint32)(x)) * (125*125)) >> 9)

  // 10 ^^ 6 / 2 ^^ 20 = 4 * 5 ^^ 6 / 2 ^^ 16 =  62500 / 2 ^^ 16
  #define FRACTION_TO_USEC(x) ((((uint32)(x)) * 62500) >> 16)

  // 2 ^^ 20 / 10 ^^ 6 = 2^^16 / (4 * 5^^6) =  2 ^^ 16 / 62500
  #define USEC_TO_FRACTION(x) ((((uint32)(x)) << 16) / 62500)


  // 2^^16 / 2 ^^ 5 / (3 * 5^^6) = 2 ^^ 11 / (3 * 5^^6)
//  #define TMR2_TO_TMR0(x)      ((((uint32)(x)) << 11) / (15625 * 3))
//  #define TMR0_TO_TMR2(x)      ((((uint32)(x)) * 15625 * 3) >> 11)

  // 2^^15 / 6 x 10^^6 = 2 ^^ 8 / (3 * 5^^6)
  #define TMR0_TO_RTC(x)      ((((uint32)(x)) << 8) / (15625 * 3))
  #define RTC_TO_TMR0(x)      ((((uint32)(x)) * 15625 * 3) >> 8)

  // 6 x 10^^6 / 10^^6 = 6
  #define USEC_TO_TMR0(x) (((uint32)(x))*6)
  #define TMR0_TO_USEC(x) (((uint32)(x))/6)

  #define USEC_TO_RTC(x) USEC_TO_FRACTION2(x)
  #define RTC_TO_USEC(x) FRACTION2_TO_USEC(x)  

#endif // _NIVIS_TMR_UTIL_H_
