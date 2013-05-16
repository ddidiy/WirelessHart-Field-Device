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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name:         config.h
// Author:       Nivis, LLC
// Date:         March 2008
// Description:  Define main WIRELESS_HART config parameters for different layers
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NIVIS_WIRELESS_HART_CONFIG_H_
#define _NIVIS_WIRELESS_HART_CONFIG_H_

#include <string.h>
#include <stdio.h>
#include "../system.h"



// Application configuration

#define MIN_APDU_SIZE  76  // 133 - MAHL, MAHL=57 maximal all headers length for normal request
#define MAX_APDU_SIZE  92  // 133 - mAHL, mAHL=41 minimal all headers length for normal request

#define MAX_JOIN_APDU_SIZE  75  // 133 - mAHL, mAHL=58 minimal all headers length for join request

// Transport layer configuration
#define MAX_TPDU_SIZE (MAX_APDU_SIZE + 3 + 4) //!!! +4 is for MIC generation!!! 
    
// Network layer configuration

#define MIN_NSDU_SIZE (MAX_TPDU_SIZE + 10)
//#define MAX_NSDU_SIZE (MAX_TPDU_SIZE + 43)  //32)  //!!! not enough for Source-Route!!!
#define MAX_NSDU_SIZE         (133-8-10)      // maxPHY - PHYheader - minDLLheader // (DLL MIC still included in MAX_NSDU_SIZE)
    
// Data Link Layer configuration
    
#endif // _NIVIS_WIRELESS_HART_CONFIG_H_ 
