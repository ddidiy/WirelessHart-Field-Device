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
* Name:         spi.c
* Author:       Nivis LLC, Dorin Muica
* Date:         February 2009
* Description:  This source file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
***************************************************************************************************/
#include "spi.h"
#include "typedef.h"
//#include "ISA100/tmr_util.h"
#include "CommonAPI/DAQ_Comm.h"
#include "LibInterface/SPI_Interface.h"
#include "itc.h"
#include "global.h"
#include "string.h"
#include "digitals.h"
#include "WirelessHart/tmr_util.h"


#define SPI_MAX_NO       500
#define DATA_LENGTH  8
#define SCK_COUNT    DATA_LENGTH

////////////////////////////////////////////////////////////////////////////////////////////////////////


#if ( SPI1_MODE != NONE )

#define STX 0xF1
#define CHX 0xF2

// General data
struct SpiFlags
{
    uint16 m_bRxComplete:1;
};

static struct SpiFlags Status = {0};

#if ( _UAP_TYPE == UAP_TYPE_SIMPLE_API )
  static uint8  g_aucTxQueue[2*SPI_BUFFER_SIZE];
#elif ( _UAP_TYPE == UAP_TYPE_WHART_API )
  static uint8  g_aucTxQueue[8*SPI_BUFFER_SIZE];  
#endif
  
uint16 g_unTxQueueSize;


static uint8 auc_Tx[SPI_BUFFER_SIZE + API_MSG_CRC_SIZE]; // add CRC
static uint8 uc_TxIdx = 0;
static uint8 uc_TxLen = 0;

volatile uint8 g_ucSpiBusy;

//
//  12MHz/SpiSPeedTable[x] = Speed
//
const static uint8 SpiSpeedTable[NUM_SUPPORTED_SPI_SPEEDS] =
{
    0,
    7, // SPI_SPEED_25k
    7, // SPI_SPEED_50k
    7, // SPI_SPEED_75k
    7, // SPI_SPEED_100k (93.75KHz)
    6, // SPI_SPEED_200k (187.5KHz)
    6, // SPI_SPEED_250k (187.5KHz)
    5, // SPI_SPEED_500k (375KHz)
    4, // SPI_SPEED_1000k (750KHz)
    3, // SPI_SPEED_2000k (1.5MHz)
};
static enum SPI_SPEED DaqSpiSpeed = DEFAULT_SPI_SPEED;

static uint8 auc_Rx[SPI_BUFFER_SIZE + API_MSG_CRC_SIZE];
static uint8 uc_RxIdx = 0;

uint8 g_ucWaitResponseFlag;

static void SPI_Interrupt(uint32 Data);

void SPI_sendPendingBuff( void );

#ifdef WAKEUP_ON_EXT_PIN
  uint8 WKUP_ckExtSignal( void );
#else
  #define WKUP_ckExtSignal() 1
#endif  

////////////////////////////////////////////////////////////////////////////////////
// Name: SPI_Init
// Description: SPI Initialisation
// Parameters: ucInitMode - initialization mode
// Return: none
///////////////////////////////////////////////////////////////////////////////////

void SPI_Init(void)
{
  spiConfig_t stSpiConfig;
  spiErr_t eErrorCodes;

  memset(&stSpiConfig, 0,sizeof(stSpiConfig));

  // Assign interrupt to the Freescale driver interrupt
  IntAssignHandler(gSpiInt_c, (IntHandlerFunc_t)SPI_ISR);
  ITC_SetPriority(gSpiInt_c, gItcNormalPriority_c);

  // assign coresponding pins to SPI peripheral function
  Gpio_SetPinFunction(SPI_CLK , gGpioAlternate1Mode_c);
  Gpio_SetPinFunction(SPI_MOSI, gGpioAlternate1Mode_c);
  Gpio_SetPinFunction(SPI_MISO, gGpioAlternate1Mode_c);
  
#if defined ( WAKEUP_ON_EXT_PIN )
  Gpio_SetPinFunction(SPI_SS  , gGpioAlternate1Mode_c);
#else
   Gpio_SetPinData( SPI_SS, gGpioPinStateHigh_c ); // manual control
#endif            

  stSpiConfig.ClkCtrl.Bits.DataCount = DATA_LENGTH;   // max value is 32
  stSpiConfig.ClkCtrl.Bits.ClockCount = SCK_COUNT;  // Master Mode only, max value is 32

  stSpiConfig.Setup.Bits.SsSetup = ConfigSsSetupMasterAutoL;
  stSpiConfig.Setup.Bits.SsDelay = ConfigSsDelay1Clk;        // Master Mode only
  stSpiConfig.Setup.Bits.SdoInactive = ConfigSdoInactiveZ;
  stSpiConfig.Setup.Bits.ClockPol = ConfigClockPolPositive;
  stSpiConfig.Setup.Bits.ClockPhase = ConfigClockPhase1stEdge;   // ??? in accordance with page 289 MSP430x1xx guide and previous RAPTOR config
  stSpiConfig.Setup.Bits.MisoPhase = ConfigMisoPhaseSameEdge;       // Master Mode only
  stSpiConfig.Setup.Bits.ClockFreq = SpiSpeedTable[DaqSpiSpeed];
  stSpiConfig.Setup.Bits.Mode = ConfigModeMaster;
  stSpiConfig.Setup.Bits.S3Wire = ConfigS3WireInactive;

  eErrorCodes = SPI_Open();
  if (gSpiErrNoError_c == eErrorCodes){
      eErrorCodes = SPI_SetCallback((spiCallback_t)SPI_Interrupt);
  }

  if (gSpiErrNoError_c == eErrorCodes){
      eErrorCodes = SPI_SetConfig(&stSpiConfig);
  }

  ITC_EnableInterrupt(gSpiInt_c);

  uc_TxIdx = 0;
  uc_TxLen = 0;

  Status.m_bRxComplete = FALSE;
  
  g_ucSpiBusy = 0;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: SPI_UpdateSpiSpeed
// Description: send a byte via SPI
// Parameters: p_NewSpeed: Requested speed.
// Retunn: none
///////////////////////////////////////////////////////////////////////////////////
uint8 SPI_UpdateSpiSpeed(enum SPI_SPEED p_NewSpeed)
{
    if (p_NewSpeed > MAX_SPI_SPEED)
        return FAILURE;

    if (p_NewSpeed < MIN_SPI_SPEED)
        return FAILURE;

    DaqSpiSpeed = p_NewSpeed;

    spiConfig_t stSpiConfig;
    spiErr_t ErrorCodes = SPI_GetConfig(&stSpiConfig);
    if (gSpiErrNoError_c != ErrorCodes){
        return UNSPECIFIED_ERROR;
    }

    stSpiConfig.Setup.Bits.ClockFreq = SpiSpeedTable[DaqSpiSpeed];

    ErrorCodes = SPI_SetConfig(&stSpiConfig);
    if (gSpiErrNoError_c != ErrorCodes){
        return UNSPECIFIED_ERROR;
    }

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: SPI_Write
// Description:   transmit a byte through Spi,
//                remains in library function until the end of transmission
//                and returns the data read
// Parameters: ucValue - data to send
//             p_pData - data received
// Return: spi operation result
///////////////////////////////////////////////////////////////////////////////////
void SPI_Write( uint8 ucValue )
{
    spiErr_t ErrorCodes;
    ErrorCodes = SPI_SetTxAsync((uint32)((ucValue << 24)));
    if (gSpiErrNoError_c == ErrorCodes){
        ErrorCodes = SPI_StartAsync();
    }
}

///////////////////////////////////////////////////////////////////////////////////
// Name: SPI_Read
// Description: Read SPI Rx buffer
// Parameters: none
// Return: Rx buffer
///////////////////////////////////////////////////////////////////////////////////
uint8 SPI_Read( void )
{
    uint32_t ulWordReceived = 0;
    SPI_GetRxAsync(&ulWordReceived);

    return (uint8)ulWordReceived;
}

///////////////////////////////////////////////////////////////////////////////////
// Name: SPI_WriteBuff
// Description: Start the SPI0 sending process
// Parameters:
//     const ApiMessageFrame * p_pMsgHeader -  message header
//     const uint8*  p_pMsgPayload - message payload
///////////////////////////////////////////////////////////////////////////////////
SC SPI_WriteBuff( const ApiMessageFrame * p_pMsgHeader,
                  const uint8*  p_pMsgPayload)
{
    SC oResult = SUCCESS;
    if( p_pMsgHeader )
    {
        uint8 pMsgSize = sizeof(ApiMessageFrame) + p_pMsgHeader->MessageSize;
        if ( g_unTxQueueSize + pMsgSize + 1 < sizeof(g_aucTxQueue) ) 
        {
            uint8 * pTxQueue = g_aucTxQueue + g_unTxQueueSize;
            *(pTxQueue++) = pMsgSize;
            
            memcpy( pTxQueue, p_pMsgHeader, sizeof(ApiMessageFrame) ); 
            pTxQueue += sizeof(ApiMessageFrame);
            
            memcpy( pTxQueue, p_pMsgPayload,p_pMsgHeader->MessageSize ); 
            
            g_unTxQueueSize += pMsgSize+1;
        }
        else
        {
            oResult = OUT_OF_MEMORY;
        }
    }
    
    SPI_sendPendingBuff();    
    return oResult;
}

uint8 * SPI_getNextSendMessage( void )
{
    if( g_unTxQueueSize )
    {
        if( !g_ucWaitResponseFlag ) // send first packet
        {
            if( !((ApiMessageFrame*)(g_aucTxQueue+1))->MessageHeader.m_bIsRsp ) // request
            {
                g_ucWaitResponseFlag = 1;
            }
            
            return g_aucTxQueue;
        }
        else // waiting flag
        {
            uint8 * pQueueMsg = g_aucTxQueue;
            g_ucWaitResponseFlag = 0;
            for( ;pQueueMsg < g_aucTxQueue + g_unTxQueueSize;  pQueueMsg += pQueueMsg[0] + 1 )
            {
                if( ((ApiMessageFrame*)(pQueueMsg+1))->MessageHeader.m_bIsRsp ) // response
                {
                    return pQueueMsg;
                }
            }            
        }
    }
    
    return NULL;
}

void SPI_removeMsg( uint8 * p_pMsg )
{
    uint16  unPkLen = 1+(uint16)(p_pMsg[0]);
    if( p_pMsg+unPkLen <= g_aucTxQueue+g_unTxQueueSize )
    {
        g_unTxQueueSize -= unPkLen;
        memmove( p_pMsg, p_pMsg+unPkLen, g_aucTxQueue+g_unTxQueueSize-p_pMsg );  // remove from queue
    }
    else // corrupted queue
    {
        g_unTxQueueSize = 0;
    }
    g_ucWaitResponseFlag = 0;    
}

void SPI_sendPendingBuff( void )
{
    if ( 0 == uc_TxLen ) 
    {
        uint8  ucPkLen;
        uint8 * pPk = SPI_getNextSendMessage();
        
        if( pPk )
        {        
            ucPkLen = pPk[0];
                    
            if( ucPkLen < sizeof(auc_Tx)-2 ) 
            {
                memcpy( auc_Tx, pPk+1, ucPkLen );
                if( ((ApiMessageFrame*)(pPk+1))->MessageHeader.m_bIsRsp ) // response, remove from queue
                {       
                    SPI_removeMsg( pPk );
                }
            }
            else
            {
                ucPkLen = 0;
                SPI_removeMsg( pPk );
            }
        }
        else
        {
            static const ApiMessageFrame stApiMsgFrame = { {0,1,API_COMMANDS}, API_HEARTBEAT, 0, 0 };
            
            memcpy( auc_Tx, &stApiMsgFrame, sizeof(stApiMsgFrame) );                
            ucPkLen = sizeof(stApiMsgFrame);
        }
            
        if( ucPkLen )
        {          
            if( WKUP_ckExtSignal() )
            {
#if !defined ( WAKEUP_ON_EXT_PIN )
                Gpio_SetPinData( SPI_SS, gGpioPinStateLow_c ); // manual control in order to allows remote 
                g_unHeartBeatTick = 0;
#endif            
                uint16 unCRC = ComputeCCITTCRC(auc_Tx, ucPkLen);
                auc_Tx[ ucPkLen+0 ] = (unCRC >> 8); // CRC -> HI byte
                auc_Tx[ ucPkLen+1 ] = unCRC & 0x00FF; // CRC -> LO byte
                 
                uc_TxIdx = 0;
                uc_TxLen = ucPkLen+2;
                            
                g_ucSpiBusy = 1;
                
                // Send STX as the first byte to start the process.
                SPI_Write(STX);
            }
            
#if defined( WAKEUP_ON_EXT_PIN ) // wake up for SPI interface
            CRM_ResetWakeUpFlag(); // delayed action in order to filter false wake up pulses
#endif
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////
// Name: SPI_TXIRQ_Handler
// Description: ISR when a byte has been transmitted
// Parameters: none
// Return: none
///////////////////////////////////////////////////////////////////////////////////
static void SPI_Interrupt(uint32 Data)
{
      static uint8 s_ucPrevChar;
      
      uint8 ucChar = SPI_Read();      
      
      // Byte sent, read byte from SPI before sending next byte
      if ( !Status.m_bRxComplete  ) 
      {
          if( ucChar == STX ) 
          {          
              uc_RxIdx = 0;  
          }
          else  if( (ucChar != CHX) && (uc_RxIdx || s_ucPrevChar == STX) && (uc_RxIdx < sizeof(auc_Rx)) )
          {
              if( s_ucPrevChar == CHX )
              {
                  auc_Rx[ uc_RxIdx++ ] = ~ucChar;
              }
              else
              {
                  auc_Rx[ uc_RxIdx++ ] = ucChar;
              }
              
              if( (uc_RxIdx == ((uint16)(auc_Rx[3]) + 6) ) ) // end of packet
              {
                   Status.m_bRxComplete = TRUE;
              }
          }
          s_ucPrevChar = ucChar;
      }
      
      if (uc_TxIdx < uc_TxLen)
      {
          // Send next byte in packet.
          ucChar = auc_Tx[uc_TxIdx];  
          if( (ucChar == STX) || (ucChar == CHX) )
          {
              auc_Tx[uc_TxIdx] = ~ucChar;
              ucChar = CHX;    
          }
          else
          {
              uc_TxIdx ++;
          }
      }
      else 
      {        
          if ( Status.m_bRxComplete || !uc_RxIdx || (uc_TxIdx > 2 * SPI_BUFFER_SIZE) )
          {
#if !defined ( WAKEUP_ON_EXT_PIN )
              Gpio_SetPinData( SPI_SS, gGpioPinStateHigh_c ); // manual control
#endif            
              g_ucSpiBusy = 0;
              return;
          }
          ucChar = STX;
          uc_TxIdx ++;
      }
      
      SPI_Write( ucChar );
            
      return;      
}

///////////////////////////////////////////////////////////////////////////////////
// Name: SPI0_ReadBuff
// Description: Get the data received.
// Parameters:
//    unsigned char* p_pSPI0Msg = data array
//    unsigned char p_ucMsgLen  = data array max size
// Returns: 0 if no data, Size of data if there was data
// Note: format is address + data
///////////////////////////////////////////////////////////////////////////////////
uint8 SPI_ReadBuff(unsigned char* p_pSPIMsg,
                    unsigned char p_ucMsgMaxSize)
{
    uint8 ucResultSize = 0;

    if ( !g_ucSpiBusy ) 
    {
        if ( Status.m_bRxComplete ) 
        {
            // As long as DataRxFlag is TRUE, the buffer does not
            // need protection from interrupts
        
            // Process results of DMA reception for full packet.
            // From STX find CRC and process.  CRC should be at STX+headersize(5)+(Size@STX+4)+CRC(2)
            uint8 ucPacketSize = uc_RxIdx;    
            if( ucPacketSize > 2 ) // valid size
            {
                if (0 == ComputeCCITTCRC(auc_Rx, ucPacketSize)) 
                {
                    ucPacketSize -= 2; // CRC[2]
                    if (ucPacketSize < p_ucMsgMaxSize) 
                    {
                        if(    ((ApiMessageFrame*)auc_Rx)->MessageHeader.m_bIsRsp  // it is response
                            && ((ApiMessageFrame*)auc_Rx)->MessageID == ((ApiMessageFrame*)(g_aucTxQueue+1))->MessageID
                            && !((ApiMessageFrame*)(g_aucTxQueue+1))->MessageHeader.m_bIsRsp   //it is waited response  
                              )
                        {
                              SPI_removeMsg( g_aucTxQueue );
                        }
                        memcpy(p_pSPIMsg, auc_Rx, ucPacketSize);
                        ucResultSize = ucPacketSize;
                    }
                }
            }            
            Status.m_bRxComplete = FALSE;
            uc_RxIdx = 0;
        }
        
        uc_TxIdx = 0;
        uc_TxLen = 0;        
    }

    return ucResultSize;
}

#ifdef WAKEUP_ON_EXT_PIN 
  ////////////////////////////////////////////////////////////////////////////////////
  // Name: WKUP_ckExtSignal
  // Description: Check 
  // Parameters: ucInitMode - initialization mode
  // Return: none
  ///////////////////////////////////////////////////////////////////////////////////

  uint8 WKUP_ckExtSignal( void )
  {      
        uint8  ucResult = 0;
        uint32 ulRtcCount = RTC_COUNT;
        
        CRM_ResetWakeUpFlag(); // reset any pending pulse
        
        SET_GPIO_HI( MT_RDY ); // set wake up signal to the application procesor
        
        do
        {
            if( GET_LO_GPIO_VALUE( EXT_WAKEUP ) || CRM_ResetWakeUpFlag() ) // accept both: pulse or edge
            {
                ucResult = 1;
            }
        } while( !ucResult && (RTC_COUNT-ulRtcCount) < USEC_TO_RTC(10000) ); // wait until app proc confirmation for max 10ms
        
        SET_GPIO_LO( MT_RDY ); // clean app processor wake up signal
                
        return ucResult;
  }
#endif // WAKEUP_ON_EXT_PIN

#endif // SPI1_MODE != NONE

