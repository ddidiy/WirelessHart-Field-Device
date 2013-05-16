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
#include "../Sensors/SHT1x.h"
#include "util/Binarization.h"
#include "../COmmonAPI/Common_API.h"
#include "app_layer.h"



// only for simple API OR IK/Loop/VS220 boards
#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API || (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )  
    
    uint16 g_unAttributesMap;
    uint32 g_ulDAQReadInterval = 0;
    uint8  g_ucShtReadRequest;
 
  
    // The states signaled by the EXTERNAL_COMM led
    //#define LED_EXTERNAL_COMM_NOT_CONNECTED   0
    //#define LED_EXTERNAL_COMM_CONNECTED       1               // Solid ON
    #define LED_EXTERNAL_COMM_COMMUNICATING   EVERY_500MSEC     // Blink
    #define EXTERNAL_COMM_NOTOK               0
    #define EXTERNAL_COMM_OK                  1
    #define EXTERNAL_COMM_DISABLED            2    
  
    uint8 g_ucExternalCommStatus; // Holds the status of communication with external device
            
    uint8 g_ucRequestForLedsON;       // Activate/Deactivate LEDs Signaling

#endif  // ( _UAP_TYPE == UAP_TYPE_SIMPLE_API || (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) ) 

    
  static void UAP_readDAQValues( void )
  {
   #if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
   
          if( g_ulDAQReadInterval )
          {
              g_ulDAQReadInterval--;
          }
          else
          {
              //read the analog/digitals channels from the DAQ
              uint8 aucMsgBuff[ANALOG_CH_NO + DIGITAL_CH_NO];
              uint8 ucIdx = 0;
              
              #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
              
                for( ; ucIdx < ANALOG_CH_NO; ucIdx++)
                {
                    aucMsgBuff[ucIdx] = ucIdx + ANALOG_DATA_ATTR_ID_OFFSET;
                }
                
                for( ; ucIdx < ANALOG_CH_NO + DIGITAL_CH_NO; ucIdx++)
                {
                    aucMsgBuff[ucIdx] = (ucIdx - ANALOG_CH_NO) + DIGITAL_DATA_ATTR_ID_OFFSET;
                }
                
              #elif (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)
              
                for( ; ucIdx < ANALOG_CH_NO; ucIdx++)
                {
                    aucMsgBuff[ucIdx] = ucIdx + ANALOG_DATA_ATTR_ID_OFFSET + LOCAL_ANALOG_CH_NO;
                }
              
              #endif  
                
              //send the API Read Data command
              if( NO_ERR == SendMessage(DATA_PASSTHROUGH, 
                                        READ_DATA_REQ, 
                                        API_REQ,   //response waiting
                                        0,      //Todo: Message ID is for later use
                                        sizeof(aucMsgBuff),      // Attribute ID(1 byte) * AttrNo    
                                        aucMsgBuff) )
              {
                  //update the sensor data read interval
                  g_ulDAQReadInterval = UAP_READ_INTERVAL;
                  
                
                  // LED_EXTERNAL_COMM status signaling
                  g_ucExternalCommStatus = EXTERNAL_COMM_OK;
              }
              else
              {
                  // LED_EXTERNAL_COMM status signaling
                  g_ucExternalCommStatus = EXTERNAL_COMM_NOTOK;
              }
          }  
    #endif   //  ( _UAP_TYPE == UAP_TYPE_SIMPLE_API ) 
  } 



static void UAP_readLocalValues(void)
{
#if ( SHT1X_MODE != 0 || _UAP_TYPE == UAP_TYPE_SIMPLE_API )  // only for IK or SIMPLE_API

  static uint8 s_ucLocalValuePeriod = 0;  
  
  if ( (s_ucLocalValuePeriod++) >= (UAP_READ_INTERVAL) ) // rollover every UAP_READ_INTERVAL seconds
  {      
    s_ucLocalValuePeriod = 0;
          
    // Raise flag to read the SHT. SHT is read every DATA_READ_INTERVAL second(s).
    g_ucShtReadRequest = 1;
  }
  
  /* data is read regardless of the join status, in order to deliver accurate values if a read request was received on the maintenance port */
  if( g_ucShtReadRequest )
  {
    
    #if ( SHT1X_MODE != 0 )
    
      SHT1x_SENSOR_READINGS stSHT1xValues;
                      
      if ( SHT1x_ReadData( &stSHT1xValues) == SHT_STATE_DONE )
      {    
        #if ((BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT)) // VS220 -> PV = 4-20mA analog input
        g_stDeviceVariables[1].m_fValue = stSHT1xValues.fTemperature;
        g_stDeviceVariables[2].m_fValue = stSHT1xValues.fHumidity;
        g_stDeviceVariables[3].m_fValue = stSHT1xValues.fDewPoint;
        #else
        g_stDeviceVariables[0].m_fValue = stSHT1xValues.fTemperature;
        g_stDeviceVariables[1].m_fValue = stSHT1xValues.fHumidity;
        g_stDeviceVariables[2].m_fValue = stSHT1xValues.fDewPoint;
        #endif
        
        // SHT reading complete. Set flag to 0 to prevent another SHT reading until next cycle begins
        g_ucShtReadRequest = 0;        
      }  
      
    #elif ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
      
        uint8 ucIdx=0;
        #if (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)
          for(; ucIdx < ANALOG_CH_NO; ucIdx ++)
          {
            g_stDeviceVariables[ucIdx].m_fValue     = g_afAnalogData[ucIdx];
            g_stDeviceVariables[ucIdx + 4].m_fValue = g_aucDigitalData[ucIdx];
          }
        #elif (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)
          for(; ucIdx < ANALOG_CH_NO; ucIdx ++)
          {
            g_stDeviceVariables[ucIdx + LOCAL_ANALOG_CH_NO].m_fValue = g_afAnalogData[ucIdx];
          }
        #endif  
          
        // SHT reading complete. Set flag to 0 to prevent another SHT reading until next cycle begins
        g_ucShtReadRequest = 0;  
    #endif
     
  }
  
#else
  
  APP_CopyChannel();
  
#endif  // ( SHT1X_MODE != 0 || _UAP_TYPE == UAP_TYPE_SIMPLE_API ) 
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @author NIVIS LLC,
/// @brief  UAP periodic task
/// @param  -
/// @return -
/// @remarks
///      Access level: user level
///      Obs:
////////////////////////////////////////////////////////////////////////////////////////////////////
void UAP_MainTask()
{
    // implement here any required periodic functionalities for this UAP
    // called every second  from the main loop
  
    UAP_readDAQValues();      
    UAP_readLocalValues();       
}

