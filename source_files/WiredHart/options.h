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

#ifndef _NIVIS_OPTIONS_H_
#define _NIVIS_OPTIONS_H_

    #include "..\system.h"

    #define HART_BUFFER_MAX_LEN   350

    #if( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )
        #define HART_MODE	  ACTIVED
    #else
        #define HART_MODE	  NONE
    #endif

#endif // _NIVIS_OPTIONS_H_
