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
/// Author:       Nivis LLC, Dorin Pavel
/// Date:         Sep 2010
/// Description:  This file implements UAP functionalities
/// Changes:      Created
/// Revisions:    1.0
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uap.h"
#include "../typedef.h"
#include "../CommonAPI/RadioApi.h"
#include "../CommonAPI/Common_API.h"

#if (_UAP_TYPE == UAP_TYPE_SIMPLE_API)


#if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
fp32    g_afAnalogData[ANALOG_CH_NO]   = {0, 0, 0, 0};
uint8   g_aucDigitalData[DIGITAL_CH_NO] = {FALSE, FALSE, FALSE, FALSE};
#elif (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)
fp32    g_afAnalogData[ANALOG_CH_NO]   = {0, 0, 0, 0, 0, 0, 0, 0};
#endif

#endif // #if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )