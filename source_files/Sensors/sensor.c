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

#include "sensor.h"
#include "SHT1x.h"
#include "../WirelessHart/app_layer.h"

uint8 g_ucShtReadRequest;

void SensorTask()
{
#if ( SHT1X_MODE != 0 )  // only for IK

  static uint8 s_ucLocalValuePeriod = 0;  
  
  if ( (s_ucLocalValuePeriod++) >= (DATA_READ_INTERVAL) ) // rollover every DATA_READ_INTERVAL seconds
  {      
    s_ucLocalValuePeriod = 0;
          
    // Raise flag to read the SHT. SHT is read every DATA_READ_INTERVAL second(s).
    g_ucShtReadRequest = 1;
  }
  
  if(  ( NET_STATE_OPERATIONAL == g_ucNetState ) 
     &&(g_stC769.joinStatus & JoinProcessStatusMask_NormalOperationCommencing)
     && g_ucShtReadRequest )
  {
    SHT1x_SENSOR_READINGS stSHT1xValues;
                    
    if ( SHT1x_ReadData( &stSHT1xValues) == SHT_STATE_DONE )
    {    
      g_stDeviceVariables[0].m_fValue = stSHT1xValues.fTemperature;
      g_stDeviceVariables[1].m_fValue = stSHT1xValues.fHumidity;
      g_stDeviceVariables[2].m_fValue = stSHT1xValues.fDewPoint;
  
      
      g_stDeviceVariables[5].m_fValue = stSHT1xValues.fTemperature;
      g_stDeviceVariables[6].m_fValue = stSHT1xValues.fHumidity;
      g_stDeviceVariables[7].m_fValue = stSHT1xValues.fDewPoint;
      
      // SHT reading complete. Set flag to 0 to prevent another SHT reading until next cycle begins
      g_ucShtReadRequest = 0; 
    }  
  }
#else
  
  APP_CopyChannel();
  
#endif  
}