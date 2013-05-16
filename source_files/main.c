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


#ifndef UT_ACTIVED  // unit testing support must be off


#include <string.h>
#include <stdio.h>

#include "typedef.h"
#include "MC1322x.h"
#include "global.h"
#include "itc.h"
#include "timers.h"
#include "digitals.h"
#include "asm.h"
#include "maca.h"
#include "uart1.h"
#include "uart2.h"
#include "spi.h"
#include "uart_hart.h"
#include "spif_interface.h"
#include "crm.h"
 
#include "WirelessHart/provision.h"
#include "WirelessHart/app_layer.h"
#include "WirelessHart/transport_layer.h"
#include "WirelessHart/network_layer.h"
#include "WirelessHart/datalink_layer.h"
#include "WirelessHart/datalink_layer_dm.h"
#include "WirelessHart/uart_link.h"
#include "WirelessHart/app_cmdreq.h"
#include "WirelessHart/app_cmdresp.h"
#include "WirelessHart/uap.h"

#include "Sensors/SHT1x.h"

#include "CommonAPI/DAQ_Comm.h"

#include "WiredHart/DataLinkLayer.h"
 

#if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )
  #if (!defined (IS_VN220))
    #include "adc_loopboard.h"
  #endif
#endif

uint8 g_ucCanSleep;

void InitDevice(void)
{
    IntDisableAll();
    WDT_INIT();
    Digitals_Init();  
    ItcInit();
    NVM_FlashInit();
    CRM_Init();  
    TMR_Init();
    ASM_Init();
    UART1_Init();
    UART2_Init();
    SPI_Init();
    MACA_Init(); 
    
///    DMAP_ResetStack(0);
  // WirelessHART Stack Initialisation
    NVM_ReadRecords(); // persistent data reading
    APP_Init();
    DLL_Init();   // reset the modem inside
    TL_Init();
    NET_Init();
    HART_DLL_Init(HART_ROLE_DECIDED);
  
  #if ( SHT1X_MODE != 0 )
    SHT1x_INIT();
  #endif
 
  #if ( (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT) )
    #if (!defined (IS_VN220))
      ADC_Extern_Init();
    #endif
  #endif
    
    IntEnableAll(); 
}    

#ifdef TEST_HART_MODEM_TX
extern uint16_t Execute_C000_ReadUniqueIdentifier( C000_ReadUniqueIdentifier_Req* request,
                                            C000_ReadUniqueIdentifier_Resp* response,
                                            SHORT_ADDR srcAddr,
                                            uint8_t control, uint8_t remainingBytes);
void TEST_SendCmd0ToWiredInterface(void)
{
  uint8 aOutputBuffer[30];  
  BinaryStream stOutputStream;
  STREAM_INIT(&stOutputStream, aOutputBuffer, sizeof(aOutputBuffer));
  
  uint8 aComposedCommand[sizeof(C000_ReadUniqueIdentifier_Resp)]; 
  uint8 ucRespCode;
  
  // Cmd0 response
  ucRespCode = Execute_C000_ReadUniqueIdentifier(NULL,
                                                 (C000_ReadUniqueIdentifier_Resp*)aComposedCommand,
                                                 MASTER_WIRED_PRIMARY_IDX,
                                                 WIRED_CMD,
                                                 30); 
  
  ComposeResponseToCommmand(MASTER_WIRED_PRIMARY_IDX, (void*)aComposedCommand, CMDID_C000_ReadUniqueIdentifier, NULL, ucRespCode, &stOutputStream);
  DLLHART_TRANSMIT_Response(stOutputStream.nextByte - aOutputBuffer, aOutputBuffer);
  g_sDllGlobals.m_Token.m_OwnToken = 1;
} 
#endif      


#ifdef LED_CONTROL

/*
Led handling task:
- passive search -> toggle LED every 2 seconds(TOGGLE_FREQ_1)
- active search -> toggle LED every 1 second(TOGGLE_FREQ_2)
- ADV received -> toggle LED every 500ms(TOGGLE_FREQ_3)
- device is operational -> LED static ON
*/
void doLedTask( void )
{
    if( g_ucNetState == NET_STATE_OPERATIONAL )
    {
      JOIN_STATUS_LED_ON();
      if(!g_ucLedStatus) g_ucLedStatus = 1;
    }
    else if( (RTC_COUNT - g_ulLastRTC) > 
            ( (g_ucDllStatus != DLL_PASSIVE_SEARCH) ? 
             ((g_ucNetJoinState >= NET_JOIN_ADV_NEIGHBOR) ? TOGGLE_FREQ_3 : TOGGLE_FREQ_2) : 
               TOGGLE_FREQ_1 ) )
    {
      TOGGLE_GPIO(JOIN_STATUS_LED);
      g_ucLedStatus = g_ucLedStatus ? 0 : 1;
      g_ulLastRTC = RTC_COUNT;
    }  
}
#endif  // LED_CONTROL

void disconnectTask(void)
{
#ifndef HCF_TEST_DEBUG // do not perform a reset upon Cmd42/Cmd771 (used while running HCF tests, to allow JTAG debugging)
  if( g_ucDoDeviceInitFromCmd )
  { // the device reset flag was set through command 42 or 771
    // check the delay that allow the cmd response to be transmitted   
    if( 
#if ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )             
        ((g_ucDoDeviceInitFromCmd & APP_DEVRESET_MASK_WIRED) && g_sDllGlobals.m_MyTxIsDone) ||  
#endif      
        ((int32)(RTC_COUNT - g_ulDelayInitFromCmd) >= 0) )
    { // save the RestartReason and wait for a watchdog reset
      if(g_ucDoDeviceInitFromCmd & APP_DEVRESET_MASK_C771)
      {
        //          if(g_ucNetJoinState < NET_JOIN_LOOSELY_COUPLED)
        //            g_ucNetState = NET_STATE_IDLE; // just trying to avoid a watchdog reset -doesn't work!
        //          else
        PrepareForReset(RREASON_CMD771);  
      }
      else
        PrepareForReset(RREASON_CMD42);
    }
  }
#endif // HCF_TEST_DEBUG
  
  
  if( g_ucResetReason )
  {
    if(!g_ucDllDisconnect)
    {
      switch (g_ucResetReason)
      {
      case RREASON_CMD42:
      case RREASON_CMD771:
        if(g_ucNetJoinState < NET_JOIN_LOOSELY_COUPLED)
          g_ucDllDisconnect = DLL_DISCONNECT_RESET; // no need to tx packets in queue or send disconnect packets
        //          else
        //          { // DLL_DISCONNECT_INACTIVE == g_ucDllDisconnect
        //            // send packets from queue then disconnect packets
        //          }
        break;
        
      // for the following: just send disconnect packets without waiting for DLL queue empty
      case RREASON_TOUT_TIMESRC:
      case RREASON_TOUT_LOOSELY:
      case RREASON_TOUT_BBR:
      case RREASON_RESET_BBR:
      case RREASON_NO_ACK:
      case RREASON_LOOP_DETECTED:
        g_ulDiscStartRTC = RTC_COUNT;
        g_ucDllDisconnect = DLL_DISCONNECT_WAIT_FOR_DIS_PACKETS;
        g_ucDllAllQueueSize = 0;
        g_ucDllCrtQueueSize = 0;
        break;
        
//    for the following: no need to wait for DLL queue nor to send disconnect packets  
//    case RREASON_TOUT_RECEPTION:
//    case RREASON_NO_SUPERFRAME:
//    case RREASON_OLD_ADVERTISE:
//    case RREASON_JOIN_REQ_EXCEEDED:
//    case RREASON_STACK_CORRUPTION:
//    case RREASON_CMD960:  // all actions have been finished already
//    case RREASON_CMD972:  // all actions have been finished already
      default: 
        g_ucDllDisconnect = DLL_DISCONNECT_RESET; 
        break;
        
      }
    }
    if(!g_ucDllDisconnect) 
    {
      // for reset reasons which imply sending packets from queue then disconnect packets
      DLL_LM_Request(DLL_SERV_DISCONNECT, NULL);  // makes g_ucDllDisconnect = DLL_DISCONNECT_WAIT_FOR_QUEUE;
    }
    else if(DLL_DISCONNECT_RESET == g_ucDllDisconnect)  
    {
      MONITOR_ENTER();
      
      PHY_Disable_Request();        
      NVM_SaveBeforeRestart(g_ucResetReason);
      
      // reset equivalent     
      //SW_RST = 0x87651234;
      
      // wait for watchdog reset -just in case
      while(1)
        ;
    }
  }
  
  
  
  
  if(DLL_DISCONNECT_INACTIVE == g_ucDllDisconnect)
    return; // this is the normal case, so return to not spend more time here
  
  if(DLL_DISCONNECT_WAIT_FOR_QUEUE == g_ucDllDisconnect) 
  {
    if(g_ucDllAllQueueSize && ((RTC_COUNT - g_ulDiscStartRTC) < g_ulDllQueueDiscCntr))
      return;       
    g_ulDiscStartRTC = RTC_COUNT;
    g_ucDllDisconnect = DLL_DISCONNECT_WAIT_FOR_DIS_PACKETS;
    g_ucDllAllQueueSize = 0; //time for emptying the dll queue has expired!
    g_ucDllCrtQueueSize = 0; //time for emptying the dll queue has expired!
  }
  else if(DLL_DISCONNECT_WAIT_FOR_DIS_PACKETS == g_ucDllDisconnect)
  {
    if((RTC_COUNT - g_ulDiscStartRTC) < DISCONNECT_INTERVAL)
      return;       
    g_ucDllDisconnect = DLL_DISCONNECT_RESET;
  }
}


// Rares: Startup Message with useful flags and settings for debug purposes (please do not remove unless you have a good reason)
void DisplayStartMsg(void)
{
  Log("\r\n\r\nWiHart 0xx ver %02d.%02d build %02d%c\r\n", MAJOR_VERSION, MINOR_VERSION, BUILD, BUILD_LETTER);
  Log("Compile Date: "); Log(__DATE__); Log(" "); Log(__TIME__); Log("\r\nSettings:\r\n");
  
  #if (BOARD_TYPE == BOARD_TYPE_HART_DEV_KIT)
    #if ( HART_DEV_KIT_REV == HART_DEV_KIT_REV3 )
      Log("  - Platform: Hart Dvlp Kit Rev. 3\r\n");
    #elif ( HART_DEV_KIT_REV == HART_DEV_KIT_REV4 )
      Log("  - Platform: Hart Dvlp Kit Rev. 4\r\n");
    #endif
  #else
    Log("  - Platform: Development Board\r\n");
  #endif
    
  #ifdef NET_ID
    Log("  - NET_ID = %04X\r\n", NET_ID);
  #endif    
    
  #ifdef UART1_DEBUG
    Log("  - UART1_DEBUG\r\n");
  #endif     
  #ifdef USE_DISCONNECT
    Log("  - USE_DISCONNECT\r\n");
  #endif  
  #ifdef HCF_LOOP_CURRENT_WA
    Log("  - HCF_LOOP_CURRENT_WA\r\n");
  #endif 
  #ifdef GRAPH_EXCEPTION
    Log("  - GRAPH_EXCEPTION\r\n");
  #endif 
  #ifdef DEBUG_RF_LINK
    Log("  - DEBUG_RF_LINK\r\n");
  #endif
    
  #ifdef WIRELESS_HART_DEVICE
    Log("  - WIRELESS_HART_DEVICE\r\n");
  #endif 
  #ifdef WIHARTDEV_TYPE
    Log("  - WIHARTDEV_TYPE=%d\r\n", WIHARTDEV_TYPE);
  #endif 
  #ifdef BOARD_TYPE
    Log("  - BOARD_TYPE=%d\r\n", BOARD_TYPE);
  #endif 
  #ifdef DEVICE_TYPE
    Log("  - DEVICE_TYPE=%d\r\n", DEVICE_TYPE);
  #endif
  #ifdef _UAP_TYPE
    Log("  - _UAP_TYPE=%d\r\n", _UAP_TYPE);
  #endif
  Log("\r\n");
}

int main()
{
  volatile unsigned char uc1SecFlag = 0;
  
  InitDevice();

#if ( _UAP_TYPE == UAP_TYPE_WHART_API )
  //SendMessage(STACK_SPECIFIC, HART_RADIO_RESET, API_REQ, 0, 0, NULL);
#endif  

  
  // Rares: Startup Message with useful flags and settings for debug purposes (please do not remove unless you have a good reason)
  DisplayStartMsg();  
  
  for (;;)
  {   
    
    g_ucCanSleep = 0; // don't sleep 
        
  #ifdef TEST_MORE_STATUS 
    // force MORE_STATUS_AVAILABLE condition (to pass UAL048 HCF test) when KBI input is forced to 1
    if(GPIO_DATA0_P23) // gGpioPin23_c is KYB1_LINE - initialized as input (STACK line on Adapter board)
    {
      APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
      g_stCmd48Data.data.standardizedStatus0 = StandardizedStatus0Mask_EnvironmentalConditionsOutOfRange; 
    }
  #endif //TEST_MORE_STATUS
    
    NET_RecvQueueManager(); // rx NET queue handler 
      
    NET_StateMachine(); // NET join state machine // should be called no more than once at 10ms while using UART1_String();

  #ifdef TRANSPORT_EXTRA_BUFFER
    TL_CheckExtraBuffer();
  #endif

    UART2_CommControl();
    
    DAQ_RxHandler();
    
    HART_CarrierDetectWatch();
    
////---- begin 10ms tasks ------------------------------------------------------

    if( g_uc10msFlag )    // 10ms Tasks
    {
      g_uc10msFlag = 0;
      
      // only for AP
      UART_LINK_Task();
      
      // wired HART
      HART_DLL_Task();

      APP_BurstModeAndEventNotificationTask();     // burst mode task
    }
////---- end 10ms tasks --------------------------------------------------------

    
////++++ begin 250ms tasks +++++++++++++++++++++++++++++++++++++++++++++++++++++
    if( g_uc250msFlag ) // 250ms Tasks
    {
      static uint8 s_ucCounter250;
      g_uc250msFlag = 0;
      if (++uc1SecFlag == 4)
      {
        Log(".");
        uc1SecFlag = 0;
      }

      FEED_WDT();
      
  #ifdef LED_CONTROL
      doLedTask();       // led control task
  #endif      
    
      APP_TriggerEvents();  // event triggering
      APP_PublishAlarmBuffer();  
      APP_SendHealthReports(); // HealthReports generation
      APP_SrvcTask(); 
      APP_QueueTask();
      REQNM_Task();
    #if ( _UAP_TYPE == UAP_TYPE_WHART_API )
      DR_Task();
    #endif  
      NET_CheckTimers();
      TL_doRetry();     // TL retry mechanism
      NET_CheckOldSessionKeysTimes();
      DAQ_TxHandler();   
      UAP_MainTask();

      
  ////==== begin 1s tasks ======================================================
      if ( 0 == ((++s_ucCounter250) & 0x03) ) // 1sec Tasks
      {        
      #ifdef TEST_HART_MODEM_TX
        TEST_SendCmd0ToWiredInterface();
      #endif      
        
        /*
        if( g_ucDllStatus >= DLL_SYNCHRONIZED )
        { // Time & Date calculation(synchronous with ASN, max. error = 1s) 
          APP_SetTimeDate();
        }
        */
        
      #ifndef BACKBONE_SUPPORT 
        NET_LoopCheck();
      #endif
        
        MACA_WatchDog();  
      }
  ////==== end 1s tasks ========================================================
  // still 250ms zone
      
      disconnectTask();

    }
////++++ end 250ms tasks +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
  } // end - for (;;)
} // end - main()

#endif // UT_ACTIVED
