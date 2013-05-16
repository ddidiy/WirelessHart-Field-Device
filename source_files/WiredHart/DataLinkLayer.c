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

#include "..\system.h"

#if ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )

#include <string.h>
#include "..\typedef.h"
#include "..\MC1322x.h"
#include "..\uart_hart.h"
#include "..\spi.h"
#include "..\WirelessHart\tmr_util.h"
#include "DataLinkLayer.h"
#include "..\WirelessHart\app_layer.h"

#if ( HART_MODE != NONE )
    #define TransmitOverCurrentLoop(X,Y) BeginTx(X)
#else
    #define TransmitOverCurrentLoop(X,Y)
#endif

T_HART_DllGlobals g_sDllGlobals;
uint8   g_aucRxData[HART_BUFFER_MAX_LEN];
uint8   g_ucRxDataIndex;
uint8   g_aucTxRequest[HART_BUFFER_MAX_LEN];
uint8   g_aucTxResponse[HART_BUFFER_MAX_LEN];
uint16  g_usTxRequestLen;
uint16  g_usTxResponseLen;

const unsigned char ZERO_LONG_ADDR[5] = { 0, 0, 0, 0, 0};


void HART_DLL_Init(T_HART_DEVICE_ROLE p_HartRole)
{
uint8 ucData;

    // DEVICE INIT
    DLLHART_LOCAL_MANAGEMENT_Request(SET_UNIQUE_ADDRESS,g_aJoinDevUID_BE);
    DLLHART_LOCAL_MANAGEMENT_Request(SET_POOLING_ADDRESS,&g_stProv.ucDevicePollingAddr);
    DLLHART_LOCAL_MANAGEMENT_Request(SET_PREAMBLE,&g_stProv.ucMinRespPreamblesNo);
    
    ucData = 3;
    DLLHART_LOCAL_MANAGEMENT_Request(SET_RETRY_LIMIT, &ucData);
    g_sDllGlobals.m_Device.m_Role = p_HartRole;
    switch (p_HartRole)
    {
    case HART_ROLE_MASTER:
        ucData = 1;
        DLLHART_LOCAL_MANAGEMENT_Request(SET_MASTER_ADDRESS, &ucData);
        break;
    
    case HART_ROLE_SECONDARY_MASTER:
        ucData = 0;
        DLLHART_LOCAL_MANAGEMENT_Request(SET_MASTER_ADDRESS, &ucData);
        break;        
      
    default:
        break;
    }

    g_sDllGlobals.m_Token.m_OwnToken = FALSE;
    g_sDllGlobals.m_Token.m_PluckTimeout = 16*1024;

    // DRIVERS INIT
    HART_Init();
//    SPI1_Init();    
}

uint8 WaitAckTimeoutExpired (void)
{
    if ((RTC_COUNT - g_sDllGlobals.m_TxMessageTimeStamp) > TX_RETRY_TIMEOUT) {
        //redundant g_sDllGlobals.m_TxMessageTimeStamp = RTC_COUNT;
        return 1;
    }
    return 0;
}

//returneaza true daca e pt mine
#define DELIMITER_INDEX     0
#define ADDRESS_INDEX       (DELIMITER_INDEX+1)
#define COMMAND_INDEX_5     (ADDRESS_INDEX+5)
#define COMMAND_INDEX_1     (ADDRESS_INDEX+1)
#define ADDR_TYPE_BIT           0x80
#define EXPANSION_BYTES_BITS    0x60
#define MESSAGE_WITH_EXPANSION_BYTES (g_aucRxData[DELIMITER_INDEX] & EXPANSION_BYTES_BITS)
#define ADDR_TYPE_LONG (g_aucRxData[DELIMITER_INDEX] & ADDR_TYPE_BIT)

uint8 CheckRxMessageIsForMe (void)
{  
    g_sDllGlobals.m_RcvDelimiterByte = g_aucRxData[DELIMITER_INDEX];
    g_sDllGlobals.m_RcvMasterNetwork = g_aucRxData[ADDRESS_INDEX] & 0x80;
    
    if (MESSAGE_WITH_EXPANSION_BYTES)   return 0;
    g_aucRxData[ADDRESS_INDEX] &=~ 0xC0;
    if (ADDR_TYPE_LONG) 
    { // Long Addr
        if ( memcmp(g_sDllGlobals.m_Device.m_LongAddr, &g_aucRxData[ADDRESS_INDEX], 5) == 0 )
        { // is for me as slave
            g_sDllGlobals.m_pRcvData = &g_aucRxData[COMMAND_INDEX_5];
            return 1;
        }
        else if ((memcmp(ZERO_LONG_ADDR, &g_aucRxData[ADDRESS_INDEX], 5) == 0) && /*test for proper command*/
                (( g_aucRxData[COMMAND_INDEX_5] == 0 ) || ( g_aucRxData[COMMAND_INDEX_5] == 11 ) || 
                 ( g_aucRxData[COMMAND_INDEX_5] == 21 ) || ( g_aucRxData[COMMAND_INDEX_5] == 73 )))  // astea sunt sau e doar 2 ?
        { //broadcast
            g_sDllGlobals.m_pRcvData = &g_aucRxData[COMMAND_INDEX_5];
            return 1;
        }            
    }
    else
    { // Short Addr
        if (( g_sDllGlobals.m_Device.m_ShortAddr == g_aucRxData[ADDRESS_INDEX] ) && /*test for proper command*/
            ( g_aucRxData[COMMAND_INDEX_1] == 0 ))
        { // is for me as slave and only cmd 0 accepted
            g_sDllGlobals.m_pRcvData = &g_aucRxData[COMMAND_INDEX_1];
            return 1; 
        }   
    }
    if ((g_sDllGlobals.m_Device.m_Role != HART_ROLE_SLAVE) && (g_sDllGlobals.m_RcvDelimiter.m_FrameType!=HART_STX))
    {
        if (g_sDllGlobals.m_RcvMasterAddress==g_sDllGlobals.m_Device.m_MasterAddress) {
            g_sDllGlobals.m_pRcvData = &g_aucRxData[COMMAND_INDEX_1];
            return 1;
        }
    }
    return 0;
}
     
#define LONGITUDINAL_PARITY_ERROR   0x08
#define RCV_COMMAND_INDEX   ( ( (g_aucRxData[0] & 0x80) ? 6 : 2 ) + ((g_aucRxData[0] & 0x60) >>5) )
void DigestRxMessage (void)
{
    if( g_sDllGlobals.m_ModemEvent )   //(g_sDllGlobals.m_ModemEvent & 0xF0) 
    { //message with errors
        if (g_sDllGlobals.m_RcvDelimiter.m_FrameType!=HART_STX) {
            return; //nothing else matters
        }
        else {  //mama said
            {
                uint8 aucErrResponse[4];
                aucErrResponse[0] = g_aucRxData[RCV_COMMAND_INDEX];
                aucErrResponse[1] = 2;//ByteCount;
                aucErrResponse[2] = 0x80 + LONGITUDINAL_PARITY_ERROR;  //de facut un if pe ModemEvent?
                aucErrResponse[3] = 0;
                DLLHART_TRANSMIT_Response (4, aucErrResponse); 
            }
        }
    }
    else 
    { //no errors
        switch (g_sDllGlobals.m_RcvDelimiter.m_FrameType)
        {
        case HART_STX:
            {
                APPHART_TRANSMIT_indicate(  g_sDllGlobals.m_RcvMasterAddress, 
                                            g_sDllGlobals.m_pRcvData); // get the party started
            }
            break;
        case HART_ACK:
            {
                //APPHART_TRANSMIT_confirm( // Mercy - Duffy
                g_sDllGlobals.m_TxRetrysLeft = 0;
            }
            break;
        case HART_BACK:
            {
                // APPHART_CYCLIC_indicate(  
            }
            break;
        }
    }
}

void TokenTimeoutsProgress (void)
{   
    static uint32 uiLastTimeMark;
    uint16 usTimeElapsed = RTC_COUNT - uiLastTimeMark;
    uiLastTimeMark = RTC_COUNT;
        
    if (g_sDllGlobals.m_Token.m_PluckTimeout)
    {
        g_sDllGlobals.m_Token.m_PluckTimeout -= usTimeElapsed;
        if (g_sDllGlobals.m_Token.m_PluckTimeout<0) {
            g_sDllGlobals.m_Token.m_PluckTimeout = 0;
            g_sDllGlobals.m_Token.m_OwnToken = TRUE;
        }
    }
    if (g_sDllGlobals.m_Token.m_LooseTimeout)
    {
        g_sDllGlobals.m_Token.m_LooseTimeout -= usTimeElapsed;
        if (g_sDllGlobals.m_Token.m_LooseTimeout<0) {
            g_sDllGlobals.m_Token.m_LooseTimeout = 0;
            g_sDllGlobals.m_Token.m_OwnToken = FALSE;
        }
    }
}

void HART_DLL_Task (void)
{
    
  //1. manage hart msgs
    if (g_sDllGlobals.m_RxMessageReady)
    {
        if (CheckRxMessageIsForMe()) { 
            DigestRxMessage();
        }
        g_sDllGlobals.m_RxMessageReady = 0;
    }
    
  //2. manage app msgs
    if (g_sDllGlobals.m_TxRetrysLeft && (!g_sDllGlobals.m_StxMessageReady)) {
        g_sDllGlobals.m_StxMessageReady = WaitAckTimeoutExpired();
    }
    
  //3. manage token
    if (g_sDllGlobals.m_ModemEvent) {
        if (g_sDllGlobals.m_ModemCarrierPresent) {
            g_sDllGlobals.m_Token.m_OwnToken = FALSE;
            g_sDllGlobals.m_Token.m_AnyTimeout=0;
        }
        if (g_sDllGlobals.m_ModemCarrierLost) {
            if (g_sDllGlobals.m_AckMessageReady) {
                g_sDllGlobals.m_Token.m_OwnToken = TRUE;
            }
            else {
                g_sDllGlobals.m_Token.m_OwnToken = FALSE;
//                if (g_sDllGlobals.m_Token.m_AnyTimeout==0) {
                    g_sDllGlobals.m_Token.m_PluckTimeout = HART_HOLD;//x msecs;
//                }
            }
        }
        if (g_sDllGlobals.m_MyTxIsDone) {
            g_sDllGlobals.m_Token.m_OwnToken = FALSE;
            g_sDllGlobals.m_Token.m_PluckTimeout = HART_HOLD;//x msecs;
        }
        g_sDllGlobals.m_ModemEvent = 0;
    }
    TokenTimeoutsProgress();
    
  //4. commit
    if (g_sDllGlobals.m_TxMessageReady) {
        if (g_sDllGlobals.m_Token.m_OwnToken) {
            if (g_sDllGlobals.m_AckMessageReady) {
                TransmitOverCurrentLoop(g_aucTxResponse, g_usTxResponseLen);
                g_sDllGlobals.m_AckMessageReady = 0;
            }
            else { //assumed g_sDllGlobals.m_StxMessageReady
                g_sDllGlobals.m_StxMessageReady = 0;
                if (g_sDllGlobals.m_TxRetrysLeft--) {
                    TransmitOverCurrentLoop(g_aucTxRequest, g_usTxRequestLen);
                    g_sDllGlobals.m_TxMessageTimeStamp = RTC_COUNT;
                }
                else {//retry depleated
                    //APPHART_TRANSMIT_confirm ( mercy
                }
            }
        }
    }
}

// used by master devices to request slaves
void DLLHART_TRANSMIT_Request ( 
                        uint8   p_ucAddrType, 
                        uint8 * p_pucAddress, 
                        uint8 * p_pucData
                      ) 
{           
    uint8 i = 0;
    uint8 j;

    if ( !p_ucAddrType ) 
    { // ==0 => 1 byte 
        g_aucTxRequest[i++] = HART_DELIMITER_1 + HART_STX;
        g_aucTxRequest[i++] = p_pucAddress[0];
    }
    else 
    { // ==1 => 5 bytes
        g_aucTxRequest[i++] = HART_DELIMITER_5 + HART_STX;
        for (j=0;j<HART_ADDR_LENGTH;)
        {
            g_aucTxRequest[i++] = p_pucAddress[j++];
        }
    }
    
    // expansion bytes would be here ...
    
    for (j = p_pucData[1];j--;)
    {
        g_aucTxRequest[i++] = *p_pucData++;
    }
    g_usTxRequestLen = i;
    g_sDllGlobals.m_StxMessageReady = 1;
    g_sDllGlobals.m_TxRetrysLeft = 3;
}


// used by slave devices to respond masters
void DLLHART_TRANSMIT_Response (uint8 p_ucByteCount, uint8 * p_pucData)
{    
    uint8 i = 0;
    uint8 j = 0;
    
    if ( !g_sDllGlobals.m_RcvDelimiter.m_AddrType ) 
    { // ==0 => 1 byte 
        g_aucTxResponse[i++] = HART_DELIMITER_1 + HART_ACK;
        g_aucTxResponse[i++] = g_sDllGlobals.m_Device.m_ShortAddr + g_sDllGlobals.m_RcvMasterNetwork;
    }
    else 
    { // ==1 => 5 bytes
        g_aucTxResponse[i++] = HART_DELIMITER_5 + HART_ACK;
        j=0;
        g_aucTxResponse[i++] = g_sDllGlobals.m_Device.m_LongAddr[j++] + g_sDllGlobals.m_RcvMasterNetwork;
        for (;j<HART_ADDR_LENGTH;)
        {
            g_aucTxResponse[i++] = g_sDllGlobals.m_Device.m_LongAddr[j++];
        }
    }
  
    // expansion bytes would be here ...
  
    for (j=0;j<p_ucByteCount;)
    {
        g_aucTxResponse[i++] = p_pucData[j++];
    }
    g_sDllGlobals.m_AckMessageReady = 1;
    g_usTxResponseLen = i;
}

void DLLHART_LOCAL_MANAGEMENT_Request (T_HART_LM_SERVICE p_eService, uint8 * p_pucData)
{
  switch (p_eService)
  {
  case SET_RETRY_LIMIT:
    {
      g_sDllGlobals.m_Device.m_RetryLimit = *p_pucData;
    }
    break;
  case SET_MASTER_ADDRESS:
    {
      g_sDllGlobals.m_Device.m_MasterAddress = ( (*p_pucData) !=0 );           
    }
    break;
  case CAPTURE_BURST_FRAMES:
    {
      g_sDllGlobals.m_Device.m_CaptureBurstFrames = 1;
    }
    break;
  case IGNORE_BURST_FRAMES:
    {
      g_sDllGlobals.m_Device.m_CaptureBurstFrames = 0;
    }
    break;
  case CAPTURE_OTHER_MASTER_FRAMES:
    {
      g_sDllGlobals.m_Device.m_CaptureOMasterFrames = 1;
    }
    break;
  case IGNORE_OTHER_MASTER_FRAMES:
    {
      g_sDllGlobals.m_Device.m_CaptureOMasterFrames = 0;
    }
    break;
  case SET_UNIQUE_ADDRESS: 
    {
      uint8 i;
      for (i=0;i<HART_ADDR_LENGTH;i++) 
      {
        g_sDllGlobals.m_Device.m_LongAddr[i] = p_pucData[i];            
      }
      g_sDllGlobals.m_Device.m_LongAddr[0] &= 0x3F;
    }
    break;
  case SET_POOLING_ADDRESS:
    {
      g_sDllGlobals.m_Device.m_ShortAddr = *p_pucData;
    }
    break;
  case SET_PREAMBLE:
    {
      g_sDllGlobals.m_Device.m_PreambleLength = *p_pucData;
    }    
    break;
  case SET_CAPTURE_ADDRESS:
    {
      g_sDllGlobals.m_Device.m_CaptureAddress = 1; // ???
    }    
    break;
  case DISABLE_CAPTURE_ADDRESS: 
    {
      g_sDllGlobals.m_Device.m_CaptureAddress = 0; // ???
    }    
    break;
  case ENABLE_BURST_MODE:
    {
      g_sDllGlobals.m_Device.m_Burst = 1; 
    }   
    break;
  case DISABLE_BURST_MODE:
    {
      g_sDllGlobals.m_Device.m_Burst = 0; 
    }    
    break;
  } 
}

#endif //( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )
