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


//
//////////////////////////////////////////////////////////////////////////////////////////////////
// Name:         UT_defines.h
// Author:       Nivis LLC
// Date:         January, 2008
// Description:  Unit Test header support
// Changes:
// Revisions:
//////////////////////////////////////////////////////////////////////////////////////////////////

// to activate a unit test select UTest project workspace and set:
// UNIT_TEST_ACTIVED = UT_... on Compiler -> Preprocessor -> Defined simbols

#ifndef UT_DEFINE_H
#define UT_DEFINE_H

#define UT_NONE      0
#define UT_ASM_ONLY  1
#define UT_PHY_ONLY  2
#define UT_DL_ONLY   3
#define UT_NL_ONLY   4
#define UT_TL_ONLY   5
#define UT_NL_TL     6
#define UT_MLDE_ONLY 7
#define UT_ASL_ONLY  8
#define UT_DMAP_ONLY 9
#define UT_DLDE1_ONLY 10
#define UT_DMAP_DLMO_ONLY 11
#define UT_BBRME_ONLY 12
#define UT_SCHEDULE_LINK_ONLY 13
#define UT_JRSP_ONLY 14
#define UT_HYBRIDHOP_ONLY 15
#define UT_SLOWCLKCORR_ONLY 16
#define UT_CONTRACT_ONLY 17
#define UT_NEWASL_ONLY 18
#define UT_DECRYPT_ONLY 19
#define UT_DAPLCO_ONLY  20

#define UT_SIZEOF_ONLY 21

#define CK(x) if(!(x)) while(1)
typedef void (*TEST_FCT)();


#endif // UT_DEFINE_H

