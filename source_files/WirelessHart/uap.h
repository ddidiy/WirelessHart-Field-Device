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
#ifndef _NIVIS_UAP_H_
#define _NIVIS_UAP_H_

#include "../typedef.h"
#include "../digitals.h"
#include "uap_data.h"


#if (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)

#define CRM_SET_SHT()  0 

#endif 



// UAP read interval
#define UAP_READ_INTERVAL     EVERY_4SEC




#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API || (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )
    
      extern uint16 g_unAttributesMap;
      extern uint32 g_ulDAQReadInterval;
      extern uint8  g_ucShtReadRequest;
      
      extern uint8  g_ucRequestForLedsON;    
      
#endif
     
      #define UAP_NeedPowerOn() DAQ_NeedPowerOn()
      
      void UAP_MainTask(void); 



#endif // _NIVIS_UAP_H_
