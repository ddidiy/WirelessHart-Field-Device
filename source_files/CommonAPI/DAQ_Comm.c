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

// $Id: DAQ_Comm.c 6912 2009-03-19 08:46:55Z ion.ticus@NIVISATL $
/*************************************************************************
 * File: DAQ_Comm.c
 * Author: John Chapman, Nivis LLC
 * Revised by: $Author: John.Chapman@NIVISATL $
 * Revision: $Revision: 6912 $
 * Description: Methods for communicating/updating data with
 *              the DAQ Processor
 *************************************************************************/
#include "../global.h"
#include "DAQ_Comm.h"
#include "../crm.h"

#if ( _UAP_TYPE != 0 )

#include "../spi.h"

#if ( SPI1_MODE != NONE ) && !defined( WAKEUP_ON_EXT_PIN )
  #define HEARTBEAT_TICS_20ms    0
  #define HEARTBEAT_TICS_50ms    0
  #define HEARTBEAT_TICS_100ms   0
  #define HEARTBEAT_TICS_500ms   2
  #define HEARTBEAT_TICS_1000ms  4
  #define HEARTBEAT_TICS_60000ms (4*60)
  
  static const uint16 HeartBeatFreqTable[NUM_SUPPORTED_HEARTBEAT_FREQ] =
  {
      HEARTBEAT_TICS_500ms,    // Not used
      HEARTBEAT_TICS_20ms,     // HEARTBEAT_20ms,
      HEARTBEAT_TICS_50ms,     // HEARTBEAT_50ms,
      HEARTBEAT_TICS_100ms,    // HEARTBEAT_100ms,
      HEARTBEAT_TICS_500ms,    // HEARTBEAT_500ms,
      HEARTBEAT_TICS_1000ms,  // HEARTBEAT_1000ms,
      HEARTBEAT_TICS_60000ms  // HEARTBEAT_60000ms,
  };
  uint16 g_unHeartBeatTick = 0;
  uint16 g_unHeartBeatLimit = HEARTBEAT_TICS_500ms;
#endif

uint8 g_ucUapId;
uint8 g_ucInitAPI = 0;

///////////////////////////////////////////////////////////////////////////////////
// Name: DAQ_Init
// Description: SPI communication handler
// Parameters: none
// Retunn: none
///////////////////////////////////////////////////////////////////////////////////
void DAQ_Init(void)
{    
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
  /*
    g_ucUapId = 0;

    SendMessage(STACK_SPECIFIC, HART_GET_INITIAL_INFO, API_REQ, 0, 0, NULL);
  */
  
    SendMessage(STACK_SPECIFIC, HART_GET_INITIAL_INFO, API_REQ, 0, 0, NULL);

#else // UAP_TYPE_SIMPLE_API
    g_ucUapId = UAP_APP1_ID;
#endif    
    
}

#if ( SPI1_MODE != NONE ) && !defined( WAKEUP_ON_EXT_PIN )
    ///////////////////////////////////////////////////////////////////////////////////
    // Name: DAQ_UpdateHeartBeatFreq
    // Description: Update the frequency at which the heartbeat is sent to the application
    //              processor
    // Parameters: NewFrequency: the newly requested frequency.
    // Return: none
    ///////////////////////////////////////////////////////////////////////////////////
    uint8 DAQ_UpdateHeartBeatFreq(enum HEARTBEAT_FREQ p_eNewFrequency)
    {
        if (p_eNewFrequency > MIN_HEARTBEAT_FREQ)
            return FAILURE;
        if (p_eNewFrequency < MAX_HEARTBEAT_FREQ)
            return FAILURE;
    
        g_unHeartBeatLimit = HeartBeatFreqTable[ p_eNewFrequency ];
    
        return SUCCESS;
    }
#endif    

///////////////////////////////////////////////////////////////////////////////////
// Name: DAQ_Handler
// Description: SPI communication handler
// Parameters: none
// Retunn: none
///////////////////////////////////////////////////////////////////////////////////
void DAQ_RxHandler(void)
{   
    uint8 aMsgBuff[MAX_API_BUFFER_SIZE];
    ApiMessageFrame * pMsgFrame = (ApiMessageFrame *)aMsgBuff;

    // RX packets
    uint8 ucPacketSize = API_PHY_Read(aMsgBuff, sizeof(aMsgBuff));
    if ( ucPacketSize == (pMsgFrame->MessageSize + sizeof(ApiMessageFrame)) ) 
    {        
        // parse response
        API_OnRcvMsg( pMsgFrame );
    }
}
///////////////////////////////////////////////////////////////////////////////////
// Name: DAQ_Handler
// Description: SPI communication handler
// Parameters: none
// Retunn: none
///////////////////////////////////////////////////////////////////////////////////
void DAQ_TxHandler(void)
{   
    //uint8 aMsgBuff[MAX_API_BUFFER_SIZE];
    
#if ( _UAP_TYPE == UAP_TYPE_WHART_API )    
    //if( !DAQ_IsReady() && !g_unTxQueueSize )
    if(!g_ucInitAPI && !g_unTxQueueSize)
    {
        DAQ_Init();
        return;
    }
    
    // TBD 9801
    
//    // TX packets
//    APDU_IDTF stApduIdtf;
//    uint8 *pApdu = ASLDE_GetMyAPDU( g_ucUapId, &stApduIdtf);
//    if( pApdu )
//    {
//        // Send APDU to DAQ.
//        memcpy(aMsgBuff, stApduIdtf.m_aucAddr, 16 );
//        aMsgBuff[16] = stApduIdtf.m_ucSrcTSAPID;
//        aMsgBuff[17] = stApduIdtf.m_ucDstTSAPID;
//        aMsgBuff[18] = 0; // not implemented yet but will be nice to be added
//        aMsgBuff[19] = stApduIdtf.m_ucPriorityAndFlags;
//        
//        if( sizeof(aMsgBuff) - (ISA100_APDU_INDICATE_PARAM_LEN + sizeof(ApiMessageFrame)) < stApduIdtf.m_unDataLen )
//        {
//            // Message size is larger than can be sent to the application processor.
//            // TODO: Flag this error somehow.
//            ASLDE_MarkForDeleteAPDU( pApdu );
//        }
//        else 
//        {
//            memcpy( aMsgBuff + ISA100_APDU_INDICATE_PARAM_LEN, pApdu, stApduIdtf.m_unDataLen);
//            if( ER_QUEUE_FULL != SendMessage(STACK_SPECIFIC, 
//                            ISA100_TL_APDU_INDICATE, 
//                            API_REQ, 
//                            0, 
//                            ISA100_APDU_INDICATE_PARAM_LEN + stApduIdtf.m_unDataLen, 
//                            aMsgBuff) )
//            {
//                ASLDE_MarkForDeleteAPDU( pApdu );
//            }    
//        }
//
//    }
#endif

#if ( SPI1_MODE != NONE )  
  #ifdef WAKEUP_ON_EXT_PIN    
       if( CRM_ResetWakeUpFlag() || g_unTxQueueSize )         
  #else
        // Normal Heartbeat freqency.
        uint16 unHeartBeatLimit;    
        
        if( EXT_WAKEUP_IS_ON() )        unHeartBeatLimit = 1;
        else if( g_unTxQueueSize )      unHeartBeatLimit = 1;
        else                            unHeartBeatLimit = g_unHeartBeatLimit;
    
        if( (++g_unHeartBeatTick) >= unHeartBeatLimit ) 
  #endif
#endif
    {
        API_PHY_Write( NULL, NULL ); // send pending messsage or API_HEARTBEAT if case
    }
    
}

#endif // _UAP_TYPE != 0
