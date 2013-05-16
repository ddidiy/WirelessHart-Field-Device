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

#ifndef _NIVIS_UAP_DATA_H_
#define _NIVIS_UAP_DATA_H_



#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )

#include "../typedef.h"


#define ANALOG_DATA_ATTR_ID_OFFSET      1


#if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)

  #define LOCAL_ANALOG_CH_NO     4
  #define ANALOG_CH_NO           4
  #define DIGITAL_CH_NO          4

  #define DIGITAL_DATA_ATTR_ID_OFFSET     16

enum
{
    UAP_DATA_RESERVED,
    UAP_DATA_TEMP,
    UAP_DATA_HUMIDITY,
    UAP_DATA_DEWPOINT,
    UAP_DATA_ANALOG1,
    UAP_DATA_DIGITAL1,
    UAP_DATA_DIGITAL2,
    UAP_DATA_DIGITAL3,
    UAP_DATA_DIGITAL4,

    UAP_DATA_ATTR_NO                  
}; // UAPDATA_ATTRIBUTES

#elif ((BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE))

  #define LOCAL_ANALOG_CH_NO     6
  #define ANALOG_CH_NO           8
  #define DIGITAL_CH_NO          0

enum
{
    UAP_DATA_RESERVED = 0,
    
    // LOCAL_ANALOG_CH_NO = 6
    UAP_DATA_LOCAL_ANALOG_INPUT,
    UAP_DATA_LOCAL_TEMP,
    UAP_DATA_LOCAL_HUMIDITY,
    UAP_DATA_LOCAL_DEWPOINT,
    UAP_DATA_LOCAL_PERCENT_OF_RANGE,
    UAP_DATA_LOCAL_LOOP_CURRENT,
    
    // ANALOG_CH_NO = 8
    UAP_DATA_ANALOG1,
    UAP_DATA_ANALOG2,
    UAP_DATA_ANALOG3,
    UAP_DATA_ANALOG4,
    UAP_DATA_ANALOG5,
    UAP_DATA_ANALOG6,
    UAP_DATA_ANALOG7,
    UAP_DATA_ANALOG8,
    
    UAP_DATA_ATTR_NO                  
}; // UAPDATA_ATTRIBUTES

#endif
 
    extern fp32    g_afAnalogData[ANALOG_CH_NO];
    
    #pragma inline 
    void UAPDATA_SetAnalogVal( uint8 p_ucStartIdx, uint8 p_ucAttrNo, fp32 p_fValue )
    {
        if( (p_ucAttrNo - p_ucStartIdx)  <= sizeof(g_afAnalogData) / sizeof(g_afAnalogData[0]) ) 
        {
           g_afAnalogData[p_ucAttrNo - p_ucStartIdx] = p_fValue;
        }  
    }

    #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
      extern uint8   g_aucDigitalData[DIGITAL_CH_NO];
      
      #pragma inline 
      void UAPDATA_SetDigitalVal( uint8 p_ucAttrNo, uint8 p_ucValue )
      {
        if( (p_ucAttrNo-UAP_DATA_DIGITAL1) <= sizeof(g_aucDigitalData) / sizeof(g_aucDigitalData[0]) ) 
        {
           g_aucDigitalData[p_ucAttrNo-UAP_DATA_DIGITAL1] = p_ucValue;
        }
      }
    #endif
     
#endif  // #if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )

#endif // _NIVIS_UAP_DATA_H_