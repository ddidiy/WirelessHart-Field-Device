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

/*************************************************************************
 * File: DAQ_Comm.h
 * Author: John Chapman, Nivis LLC
 * Control functions for handling comms with the DAQ card.
 *************************************************************************/

#ifndef DAQ_COMM_H_
#define DAQ_COMM_H_

#include "RadioApi.h"
#include "Common_API.h"
#include "../system.h"

#if ( SPI1_MODE != NONE ) && !defined ( WAKEUP_ON_EXT_PIN )
  extern uint16 g_unHeartBeatTick;
#endif
extern uint8 g_ucUapId;
extern uint8 g_ucInitAPI;
#ifndef _UAP_TYPE
  #define _UAP_TYPE 0
#endif

#if ( _UAP_TYPE != 0 )
    void DAQ_Init(void);
    void DAQ_RxHandler(void);
    void DAQ_TxHandler(void);
    
    uint8 DAQ_UpdateHeartBeatFreq(enum HEARTBEAT_FREQ);

    #if ( _UAP_TYPE == UAP_TYPE_WHART_API )    
        #define DAQ_IsReady() (g_ucUapId)
    #else
        #define DAQ_IsReady() 1
    #endif        
    
    #if ( SPI1_MODE != NONE )
    
      #if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )
         #define DAQ_NeedPowerOn()  SPI_IsBusy() || HART_IsBusy() || g_ucShtReadRequest //|| EXT_WAKEUP_IS_ON()
      #else
         #define DAQ_NeedPowerOn()  SPI_IsBusy() //|| EXT_WAKEUP_IS_ON())
      #endif   
    
    #elif ( UART2_MODE != NONE )
    
        #define DAQ_NeedPowerOn() (UART2_IsBusy() || EXT_WAKEUP_IS_ON())    
    
    #endif   
        
#else   
    #define DAQ_Init()
    #define DAQ_RxHandler(...)
    #define DAQ_TxHandler(...)
    
  #if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) ) 
    #define DAQ_NeedPowerOn()  (HART_IsBusy() || g_ucShtReadRequest)
  #else
    #define DAQ_NeedPowerOn()  0    
  #endif    
    
    #define DAQ_IsReady()      1    
#endif //


#endif //DAQ_COMM_H_
