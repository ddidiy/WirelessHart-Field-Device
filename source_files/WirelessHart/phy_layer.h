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

/***************************************************************************************************
* Name:         phy.h
* Author:       Marius Vilvoi
* Date:         November 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_PHY_H_
#define _NIVIS_PHY_H_

#include "../typedef.h"
#include "../global.h"
#include "../MC1322x.h"
#include "../digitals.h"
#include "../itc.h"
#include "../maca.h"
#ifdef INCLUDE_COMMON_STACK_SOURCES
  #include "../../../../../../../WirelessHART/trunk/Stack/src/ApplicationLayer/Model/CommonResponseCode.h"
#else
  #include "ApplicationLayer/Model/CommonResponseCode.h"
#endif

#define WARM_UP_DELAY       (50)                                                // 50 * 4 = 144 uS

#define PHY_BUFFERS_LENGTH          ( 128 )


#ifndef BACKBONE_SUPPORT
  //#define SEND_FICTIVE_ACK
#endif
#define USE_RX_ABORT_TIMEOUT 
#ifdef USE_RX_ABORT_TIMEOUT
  #define RX_ABORT_TIMEOUT        ((DLL_TS_RX_WAIT + 400) / 4)    // [4us] units    
  #define RX_ACK_ABORT_TIMEOUT    0 // 0 until validate the solution   //((DLL_TS_ACK_WAIT + 400) / 4)   // [4us] units 
#else
  #define RX_ABORT_TIMEOUT        0    // [4us] units 
  #define RX_ACK_ABORT_TIMEOUT    0    // [4us] units 
#endif

extern volatile uint8 g_ucXVRStatus;
extern volatile uint8 g_aucPhyRx[PHY_BUFFERS_LENGTH + 1];




// ----------- PHY CHANNELS ------------
#define PHY_CHAN_11               (unsigned char)  0
#define PHY_CHAN_12               (unsigned char)  1
#define PHY_CHAN_13               (unsigned char)  2
#define PHY_CHAN_14               (unsigned char)  3
#define PHY_CHAN_15               (unsigned char)  4
#define PHY_CHAN_16               (unsigned char)  5
#define PHY_CHAN_17               (unsigned char)  6
#define PHY_CHAN_18               (unsigned char)  7
#define PHY_CHAN_19               (unsigned char)  8
#define PHY_CHAN_20               (unsigned char)  9
#define PHY_CHAN_21               (unsigned char) 10
#define PHY_CHAN_22               (unsigned char) 11
#define PHY_CHAN_23               (unsigned char) 12
#define PHY_CHAN_24               (unsigned char) 13
#define PHY_CHAN_25               (unsigned char) 14
#define PHY_CHAN_26               (unsigned char) 15
#define PHY_NO_CHANNELS           (unsigned char) 16

// ------- PHY ENABLE REQUEST PARAMETERS -----------
#define PHY_DISABLE               (unsigned char) 0
#define PHY_ENABLE_TX             (unsigned char) 1
#define PHY_ENABLE_RX             (unsigned char) 2
// ------- ENABLE CONFIRM CODES
#define PHY_DISABLED_SUCCES       (unsigned char) 0
#define PHY_TX_ENABLED_SUCCES     (unsigned char) 1
#define PHY_TX_ENABLED_ERROR      (unsigned char) 2
#define PHY_RX_ENABLED_SUCCES     (unsigned char) 3
#define PHY_RX_ENABLED_ERROR      (unsigned char) 4


// ------- CCA REQUEST VOID   -----------
// ------- CCA CONFIRM CODES
#define PHY_CCA_CHAN_IDLE         (unsigned char) 0
#define PHY_CCA_CHAN_BUSY         (unsigned char) 1
#define PHY_CCA_TRANCEIVER_OFF    (unsigned char) 2
#define PHY_CCA_OTH_ERROR         (unsigned char) 3


// ------- DATA REQUEST VOID   -----------
// ------- DATA CONFIRM TX CODES
#define PHY_DATA_SUCCES           (unsigned char) 0
#define PHY_DATA_TRANCEIVER_OFF   (unsigned char) 1
#define PHY_DATA_TRANCEIVER_BUSY  (unsigned char) 2
#define PHY_DATA_RECEIVER_ON      (unsigned char) 3
#define PHY_DATA_OTH_ERROR        (unsigned char) 4


// ------- LOCAL MANAGEMENT REQUEST SERVICES PARAMETERS
#define PHY_MNG_RESET                 (unsigned char) 0
#define PHY_MNG_READ_TX_PWR_LVL       (unsigned char) 1
#define PHY_MNG_WRITE_TX_PWR_LVL      (unsigned char) 2
#define PHY_MNG_WRITE_RX_OVERFLOW_LVL (unsigned char) 3
// ------- LOCAL MANAGEMENT CONFIRM SERVICES CODES
#define PHY_MNG_SUCCES                  RCS_N00_Success                      // 0
#define PHY_MNG_ERR_PARAM_TOO_LARGE     RCS_E03_PassedParameterTooLarge      // 3
#define PHY_MNG_ERR_PARAM_TOO_SMALL     RCS_E04_PassedParameterTooSmall      // 4
#define PHY_MNG_ERR_DEVICE_COMMAND_ERR  RCS_E06_DeviceSpecificCommandError    // 6
#define PHY_MNG_WARN_SET_TO_NEAREST_VAL RCM_W08_SetToNearestPossibleValue    // 8


// ------- ERROR CODES (RX INDICATE)
#define RX_BUFFER_OVERFLOW        (unsigned char) 1
#define PACKET_INCOMPLETE         (unsigned char) 2
#define TRANCEIVER_BUSY           (unsigned char) 3
#define OTH_ERROR                 (unsigned char) 4
#define RX_GENERAL_ERROR          (unsigned char) 5
#define RX_CRC_ERROR              (unsigned char) 6


// ------- STATE MACHINE CODE
#define   PHY_DISABLED            (unsigned char)  0 // maca clock off
#define   PHY_IDLE                (unsigned char)  1 // maca clock on but do nothing
#define   PHY_CCA_START_DO        (unsigned char)  2 // start action
#define   PHY_TX_START_DO         (unsigned char)  3 // start action
#define   PHY_RX_START_DO         (unsigned char)  4 // start action


//.void PHY_Disable_Request ( void );
#define PHY_Disable_Request( ... ) PHY_DisableTR()
void PHY_DisableTR ( void );
void PHY_RX_Request ( uint8 p_ucChannel, uint16 p_unDelay, uint16 p_unAbortTimeout );
void PHY_TX_Request ( uint8 p_ucChannel, uint16 p_unDelay, uint8 p_ucCCAFlag, uint8 * p_pucData, uint8 p_ucLen );
unsigned char PHY_MNG_Request ( unsigned char p_ucService, void * p_pData);

#ifdef SEND_FICTIVE_ACK
void DLL_SendFictiveACK(void);
#endif


__arm uint32 PHY_GetLastRXuSecAge(void);
__arm uint32 PHY_GetLastTXuSecAge(void);

//#define PHY_RESET() PHY_MNG_Request( PHY_MNG_RESET, NULL )
//.#define PHY_INIT()  PHY_MNG_Request( PHY_MNG_RESET, NULL )

#define PHY_Reset() MACA_Reset()

#define IS_PHY_IDLE() ( ( g_ucXVRStatus == PHY_IDLE ) || ( g_ucXVRStatus == PHY_DISABLED ) ? 1 : 0 )

//void PHY_CkWachDog(void);

#endif /* _NIVIS_PHY_H_ */

