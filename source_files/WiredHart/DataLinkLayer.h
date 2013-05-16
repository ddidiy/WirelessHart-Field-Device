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

#ifndef __NIVIS_HART_DLL_H_
#define __NIVIS_HART_DLL_H_

#if ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )

    #include "..\typedef.h"
    
    typedef enum 
    {
        HART_ROLE_MASTER, 
        HART_ROLE_SECONDARY_MASTER,
        HART_ROLE_SLAVE
        //HART_BURST_SLAVE //it will never be
    } T_HART_DEVICE_ROLE;
    
        #if (WIHARTDEV_TYPE == WIHARTDEV_TYPE_FIELDDEV)
            #define HART_ROLE_DECIDED   HART_ROLE_SLAVE 
        #else
            #define HART_ROLE_DECIDED   HART_ROLE_MASTER
        #endif
    
    typedef enum 
    {
        SET_RETRY_LIMIT, 
        SET_MASTER_ADDRESS, 
        CAPTURE_BURST_FRAMES, 
        IGNORE_BURST_FRAMES, 
        CAPTURE_OTHER_MASTER_FRAMES, 
        IGNORE_OTHER_MASTER_FRAMES,
        SET_UNIQUE_ADDRESS, 
        SET_POOLING_ADDRESS, 
        SET_PREAMBLE, 
        SET_CAPTURE_ADDRESS, 
        DISABLE_CAPTURE_ADDRESS, 
        ENABLE_BURST_MODE,
        DISABLE_BURST_MODE    
    } T_HART_LM_SERVICE;

    typedef struct 
    {
        T_HART_DEVICE_ROLE m_Role;  
        
        uint8 m_LongAddr[5];
        uint8 m_ShortAddr;
        uint8 m_PreambleLength;
        uint8 m_RetryLimit;       
        uint8 m_CaptureAddress;    // ???
        
        uint8 m_MasterAddress :1;    
        uint8 m_Burst :1;        
        uint8 m_CaptureBurstFrames :1;  // ???
        uint8 m_CaptureOMasterFrames :1;    // ???
    } T_HART_DEVICE;

    typedef struct 
    {
        union {
            uint32 m_AnyTimeout;
            struct {
                int16 m_PluckTimeout;
                int16 m_LooseTimeout;
            };
        };
        uint8 m_OwnToken;
    } T_TOKEN_MDL; 

    typedef struct 
    {
        uint8 m_FrameType:3;
        uint8 m_PhyLayerType:2;
        uint8 m_ExpBytes:2;
        uint8 m_AddrType:1;
    } T_DELIMITER;
    
    typedef struct 
    {
      T_HART_DEVICE m_Device;
      T_TOKEN_MDL   m_Token;
      
      uint8 m_TxRetrysLeft;
      uint32 m_TxMessageTimeStamp;
      
      union {
          uint8 m_TxMessageReady:2;  
          struct {
              uint8 m_StxMessageReady:1;
              uint8 m_AckMessageReady:1;
          };
      };  
      uint8 m_RxMessageReady:1;  
      uint8 m_BurstNetworkMode :1; // la ce foloseste
      uint8 m_OMasterFrame:1;   // la ce foloseste
      
      union {
          uint8 m_ModemEvent;
          struct {
              uint8 m_ModemCarrierLost:1;
              uint8 m_MyTxIsDone:1;
              uint8 m_ModemCarrierPresent:1;
              uint8 unused_1:1;
              uint8 m_CheckByteError:1;
              uint8 m_ReceiveError:1;
              uint8 unused_2:1;
              uint8 m_GapOrCarrierError:1;
          };
      };
              
      uint8 m_BurstDeviceLongAddr[5];   // ?? la ce foloseste
      uint8 m_BurstDeviceShortAddr;     // ?? la ce foloseste
      union {
        T_DELIMITER m_RcvDelimiter;
        uint8       m_RcvDelimiterByte;
      };
      union {
          struct {
          uint8 m_DumbBits          :6;
          uint8 m_RcvNetworkMode    :1; 
          uint8 m_RcvMasterAddress  :1;     
          };
          uint8 m_RcvMasterNetwork;
      };
      uint8 * m_pRcvData;
    } T_HART_DllGlobals;

    #define HART_DELIMITER_1      0x00      //1 byte  Address + No Expansion Bytes + Async Medium + No Frame Defined
    #define HART_DELIMITER_5      0x80      //5 bytes Address + No Expansion Bytes + Async Medium + No Frame Defined
    #define HART_STX              (0x02)
    #define HART_ACK              (0x06)
    #define HART_BACK             (0x01)
    #define HART_ADDR_LENGTH      5
    #define HART_FRAME_MASK       7
    #define HART_EXTBYTES_MASK    0x60

    // HART TIMEOUTS
    #define HART_CHARACTER_TIME   327                                   // 9.166(6) miliSeconds   
    #define HART_STO              (28 * HART_CHARACTER_TIME)
    #define HART_HOLD             (2 * HART_CHARACTER_TIME)
    #define HART_RT1_PRIMARY      (33 * HART_CHARACTER_TIME)
    #define HART_RT1_SECONDARY    (41 * HART_CHARACTER_TIME)
    #define HART_RT2              (8 * HART_CHARACTER_TIME)
    #define HART_BURST_TIMEOUT    (HART_RT1_PRIMARY)
    #define TX_RETRY_TIMEOUT        (200*HART_CHARACTER_TIME)

    extern T_HART_DllGlobals g_sDllGlobals;
    extern uint8   g_aucRxData[];
    extern uint8   g_ucRxDataIndex;
    extern uint8   g_aucTxRequest[];
    extern uint8   g_aucTxResponse[];
    extern uint16  g_usTxRequestLen;
    extern uint16  g_usTxResponseLen;

    void HART_DLL_Init (T_HART_DEVICE_ROLE p_HartRole);
    void HART_DLL_Task (void);
    #if (1)
        void DLLHART_LOCAL_MANAGEMENT_Request (T_HART_LM_SERVICE p_eService, uint8 * p_pucData);
    #else
        #define DLLHART_LOCAL_MANAGEMENT_SET_RETRY_LIMIT(X)             {g_sDllGlobals.m_Device.m_RetryLimit = *(uint8 *)X;}
        #define DLLHART_LOCAL_MANAGEMENT_SET_MASTER_ADDRESS(X)          {g_sDllGlobals.m_Device.m_MasterAddress = ( (*(uint8 *)X) !=0 );}
        #define DLLHART_LOCAL_MANAGEMENT_CAPTURE_BURST_FRAMES(X)        {g_sDllGlobals.m_Device.m_CaptureBurstFrames = 1;}
        #define DLLHART_LOCAL_MANAGEMENT_IGNORE_BURST_FRAMES(X)         {g_sDllGlobals.m_Device.m_CaptureBurstFrames = 0;}
        #define DLLHART_LOCAL_MANAGEMENT_CAPTURE_OTHER_MASTER_FRAMES(X) {g_sDllGlobals.m_Device.m_CaptureOMasterFrames = 1;}
        #define DLLHART_LOCAL_MANAGEMENT_IGNORE_OTHER_MASTER_FRAMES(X)  {g_sDllGlobals.m_Device.m_CaptureOMasterFrames = 0;}
        #define DLLHART_LOCAL_MANAGEMENT_SET_UNIQUE_ADDRESS(X)          {\
                                                                  uint8 i;  \
                                                                  for (i=0;i<HART_ADDR_LENGTH;i++)  \
                                                                  { \
                                                                    g_sDllGlobals.m_Device.m_LongAddr[i] = ((uint8 *)X)[i];    \
                                                                  } \
                                                                  g_sDllGlobals.m_Device.m_LongAddr[0] &= 0x3F; \
                                                                }
        #define DLLHART_LOCAL_MANAGEMENT_SET_POOLING_ADDRESS(X)         {g_sDllGlobals.m_Device.m_ShortAddr = *(uint8 *)X;}
        #define DLLHART_LOCAL_MANAGEMENT_SET_PREAMBLE(X)                {g_sDllGlobals.m_Device.m_PreambleLength = *(uint8 *)X;}
        #define DLLHART_LOCAL_MANAGEMENT_SET_CAPTURE_ADDRESS(X)         {g_sDllGlobals.m_Device.m_CaptureAddress = 1;}
        #define DLLHART_LOCAL_MANAGEMENT_DISABLE_CAPTURE_ADDRESS(X)     {g_sDllGlobals.m_Device.m_CaptureAddress = 0;}
        #define DLLHART_LOCAL_MANAGEMENT_ENABLE_BURST_MODE(X)           {g_sDllGlobals.m_Device.m_Burst = 1;}
        #define DLLHART_LOCAL_MANAGEMENT_DISABLE_BURST_MODE(X)          {g_sDllGlobals.m_Device.m_Burst = 0; }
    #endif

    void DLLHART_TRANSMIT_Request ( 
                            uint8   p_ucAddrType, 
                            uint8 * p_pucAddress, 
                            uint8 * p_pucData
                          );

    void DLLHART_TRANSMIT_Response ( 
                            uint8 p_ucByteCount, 
                            uint8 * p_pucData
                           );
#else

    #define HART_ROLE_DECIDED   0
    #define HART_DLL_Init(X)
    #define HART_DLL_Task()

#endif //( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )
#endif // __NIVIS_HART_DLL_H_
