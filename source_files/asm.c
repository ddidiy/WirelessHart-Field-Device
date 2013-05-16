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
* Name:         asm.c
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#include "asm.h"
#include "itc.h"
#include "global.h"
#include "MC1322x.h"
#include "digitals.h"
#include <string.h>


#define AES_BUFFER_LENGTH     (128)
#define AES_BLOCK_LENGTH      (16)

#define TO_ENCRYPT_MAX_LEN    (1500) // ???

#define AES_ON_MODE_CTR()         ( ASM_CONTROL1_CRYPTMODE = 2 )    // ASM_CONTROL1_CTR = 1; ASM_CONTROL1_CBC = 0;
#define AES_ON_MODE_CBC()         ( ASM_CONTROL1_CRYPTMODE = 1 )    // ASM_CONTROL1_CTR = 0; ASM_CONTROL1_CBC = 1;
#define AES_ON_MODE_CCM()         ( ASM_CONTROL1_CRYPTMODE = 3 )    // ASM_CONTROL1_CTR = 1; ASM_CONTROL1_CBC = 1;
#define AES_OFF()                 ( ASM_CONTROL1_CRYPTMODE = 0 )    // ASM_CONTROL1_CTR = 0; ASM_CONTROL1_CBC = 0;





#define DISABLE_ASM_MODULE()      ( ASM_CONTROL1_ON = 0 )
#define ENABLE_ASM_MODULE()       ( ASM_CONTROL1_ON = 1 )

#define ASM_SELFTEST_ON()         ( ASM_CONTROL1_SELFTEST = 1 )
#define ASM_SELFTEST_OFF()        ( ASM_CONTROL1_SELFTEST = 0 )

#define ASM_NORMALMODE_ON()       ( ASM_CONTROL1_MODE = 1 )
#define ASM_NORMALMODE_OFF()      ( ASM_CONTROL1_MODE = 0 )

#define ASM_CTR_ON()              ( ASM_CONTROL1_CRYPTMODE = 2 )    // ASM_CONTROL1_CTR = 1; ASM_CONTROL1_CBC = 0;
#define ASM_CTR_OFF()             ( ASM_CONTROL1_CRYPTMODE = 0 )    // ASM_CONTROL1_CTR = 0; ASM_CONTROL1_CBC = 0;

#define ASM_CBC_ON()              ( ASM_CONTROL1_CRYPTMODE = 1 )    // ASM_CONTROL1_CTR = 0; ASM_CONTROL1_CBC = 1;
#define ASM_CBC_OFF()             ( ASM_CONTROL1_CRYPTMODE = 0 )    // ASM_CONTROL1_CTR = 0; ASM_CONTROL1_CBC = 0;

#define ASM_CCM_ON()              ( ASM_CONTROL1_CRYPTMODE = 3 )    // ASM_CONTROL1_CTR = 1; ASM_CONTROL1_CBC = 1;
#define ASM_CCM_OFF()             ( ASM_CONTROL1_CRYPTMODE = 0 )    // ASM_CONTROL1_CTR = 0; ASM_CONTROL1_CBC = 0;

// self clear bits
#define ASM_START_OPERATION()     ( ASM_CONTROL0_START = 1 )
#define ASM_CLEAR()               ( ASM_CONTROL0_CLEAR = 1 )
#define ASM_LOAD_MAC()            ( ASM_CONTROL0_LOAD_MAC = 1 )



#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER
  uint8 g_ucAsmDirtyFlag = 0;
#else // support for same buffer -> not interrupts allowed
  #define g_ucAsmDirtyFlag 0
#endif 


#ifdef BACKBONE_SUPPORT
//  #define ASM_BEGIN_EXCLUSIVE_ZONE() { UART1_RXTX_INT_DI(); DISABLE_DLL_TMRIRQ(); }
//  #define ASM_END_EXCLUSIVE_ZONE()   { ENABLE_DLL_TMRIRQ(); UART1_RXTX_INT_EN(); }
//.  #define ASM_BEGIN_EXCLUSIVE_ZONE() { DISABLE_DLL_TMRIRQ(); }
//.  #define ASM_END_EXCLUSIVE_ZONE()   { ENABLE_DLL_TMRIRQ(); }
  #define ASM_BEGIN_EXCLUSIVE_ZONE() {ITC_DISABLE_INTERRUPT(TMR_INT); ITC_DISABLE_INTERRUPT(MACA_INT);}
  #define ASM_END_EXCLUSIVE_ZONE()   {ITC_ENABLE_INTERRUPT(MACA_INT); ITC_ENABLE_INTERRUPT(TMR_INT);}
#else
//.  #define ASM_BEGIN_EXCLUSIVE_ZONE() { DISABLE_DLL_TMRIRQ(); }
//.  #define ASM_END_EXCLUSIVE_ZONE()   { ENABLE_DLL_TMRIRQ(); }
  #define ASM_BEGIN_EXCLUSIVE_ZONE() {ITC_DISABLE_INTERRUPT(TMR_INT); ITC_DISABLE_INTERRUPT(MACA_INT);}
  #define ASM_END_EXCLUSIVE_ZONE()   {ITC_ENABLE_INTERRUPT(MACA_INT); ITC_ENABLE_INTERRUPT(TMR_INT);}
#endif

__arm uint32 rotate4Bytes(uint32 p_ulData )
{
    uint32 v = p_ulData;
    uint32 t = v ^ ((v << 16) | (v >> 16));
    t &= ~0xff0000;
    v = (v << 24) | (v >> 8);
    return v ^ (t >> 8);
}

__arm uint32 rotUnaligned4Bytes2(const uint8 * p_pData )
{
   return ( (uint32)p_pData[0] << 24 ) | ( (uint32)p_pData[1] << 16 ) | ( (uint32)p_pData[2] << 8 ) | p_pData[3];
}

__arm uint32 rotUnaligned4Bytes3(const uint8 * p_pData, int32 p_nDataLen )
{
    if( p_nDataLen >= 4 )
    {
        return ( (uint32)p_pData[0] << 24 ) | ( (uint32)p_pData[1] << 16 ) | ( (uint32)p_pData[2] << 8 ) | p_pData[3];
    }

    switch( p_nDataLen )
    {
    case 1: return  ( (uint32)p_pData[0] << 24 );
    case 2: return  ( (uint32)p_pData[0] << 24 ) | ( (uint32)p_pData[1] << 16 );
    case 3: return  ( (uint32)p_pData[0] << 24 ) | ( (uint32)p_pData[1] << 16 ) | ( (uint32)p_pData[2] << 8 );
    }

    // <= 0
    return  0;
}

__arm uint32 getZeroFilled4Bytes(uint32  p_ulData, int32 p_nDataBytesLen )
{
    if( p_nDataBytesLen >= 4 )
        return p_ulData;

    switch( p_nDataBytesLen )
    {
    case 1: return (p_ulData & 0xFF000000);
    case 2: return (p_ulData & 0xFFFF0000);
    case 3: return (p_ulData & 0xFFFFFF00);
    }

    // <= 0
    return  0;
}


__arm void loadDataReg(const uint8 * p_pData, int32 p_nDataLen )
{
    ASM_DATA0 = rotUnaligned4Bytes3( p_pData, p_nDataLen );
    ASM_DATA1 = rotUnaligned4Bytes3( p_pData+4, p_nDataLen-4 );
    ASM_DATA2 = rotUnaligned4Bytes3( p_pData+8, p_nDataLen-8 );
    ASM_DATA3 = rotUnaligned4Bytes3( p_pData+12, p_nDataLen-12 );
}

__arm void startAesAndAuthOnly
                        ( const uint8 * p_pucKey,
                          const uint8 * p_pucNonce,
                          const uint8 * p_pucToAuthOnly,
                          int32         p_lToAuthOnlyLen,
                          uint16        p_unToEncryptLen,
                          uint8         p_ucInterruptFlag )
{
    uint32 ulTmp;

#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER          
    g_ucAsmDirtyFlag = p_ucInterruptFlag;
#endif
    
    if( !p_ucInterruptFlag )
    {
      ASM_BEGIN_EXCLUSIVE_ZONE();
    }

    AES_OFF();
    ASM_CLEAR();                        // clear all memory in ASM to obtain the corect MIC.

    // set keys
    ASM_KEY0 = rotUnaligned4Bytes2( p_pucKey );
    ASM_KEY1 = rotUnaligned4Bytes2( p_pucKey+4 );
    ASM_KEY2 = rotUnaligned4Bytes2( p_pucKey+8 );
    ASM_KEY3 = rotUnaligned4Bytes2( p_pucKey+12 );


    // building B0 and A1 (in counter)
    ulTmp = ((uint32)p_pucNonce[0] << 16) | ((uint32)p_pucNonce[1] << 8) | p_pucNonce[2];
    ASM_DATA0 = (0x49L << 24) | ulTmp;
    ASM_CTR0  = (0x01L << 24) | ulTmp;

    ASM_DATA1 = ASM_CTR1 = rotUnaligned4Bytes2(p_pucNonce+3);
    ASM_DATA2 = ASM_CTR2 = rotUnaligned4Bytes2(p_pucNonce+7);

    ulTmp = ((uint32)p_pucNonce[11] << 24) | ((uint32)p_pucNonce[12] << 16);
    ASM_DATA3 = ulTmp | p_unToEncryptLen;
    ASM_CTR3  = ulTmp | 0x0001;

    // start with authetication only
    AES_ON_MODE_CBC();
    ASM_START_OPERATION();

    // building B1

    ulTmp  =  p_lToAuthOnlyLen << 16;
    if( p_lToAuthOnlyLen > 1 )
    {
        ulTmp |= ( (uint32)p_pucToAuthOnly[0] << 8 ) | p_pucToAuthOnly[1];
    }
    else if( p_lToAuthOnlyLen == 1 )
    {
        ulTmp |= ( (uint32)p_pucToAuthOnly[0] << 8 );
    }

     while ( ! (ASM_STATUS_DONE) )
        ;
    // done with B0

    ASM_DATA0 = ulTmp;
    ASM_DATA1 = rotUnaligned4Bytes3( p_pucToAuthOnly+2, p_lToAuthOnlyLen-2 );
    ASM_DATA2 = rotUnaligned4Bytes3( p_pucToAuthOnly+6, p_lToAuthOnlyLen-6 );
    ASM_DATA3 = rotUnaligned4Bytes3( p_pucToAuthOnly+10, p_lToAuthOnlyLen-10 );

    // proccesing B1
    ASM_START_OPERATION();

    p_pucToAuthOnly += 14;
    p_lToAuthOnlyLen -= 14;

    while ( p_lToAuthOnlyLen > 0 )
    {
        while ( ! (ASM_STATUS_DONE) )
          ;

        loadDataReg( p_pucToAuthOnly, p_lToAuthOnlyLen );

        ASM_START_OPERATION();

        p_lToAuthOnlyLen -= 16;
        p_pucToAuthOnly += 16;
    }

    while ( ! (ASM_STATUS_DONE) )
       ;
}

__arm void putbackReg(uint8 * p_pData, uint32 volatile * p_pRegister )
{
  uint8 i = 3;
  do
  {
    uint32 a = * (p_pRegister++);

    p_pData[0] =  (uint8) (a >> 24);
    p_pData[1] =  (uint8) (a >> 16);
    p_pData[2] =  (uint8) (a >> 8);
    p_pData[3] =  (uint8) (a);

    p_pData += 4;
  }
  while ( i-- );

}

// This function allows to carry out a self test to verify the operation of the encryption engine.
void ASM_Init(void)
{
  volatile uint16 unTestTimeout = 3330;

  ENABLE_ASM_MODULE();
  if( ! ASM_STATUS_TESTPASS )
  {
      ASM_SELFTEST_ON();
      ASM_START_OPERATION();

      // wait timeout
      while(  !(ASM_STATUS_TESTPASS) && (unTestTimeout))
      {
        unTestTimeout--;
      }

      ASM_SELFTEST_OFF();

      ASM_NORMALMODE_ON();
  }
}



///////////////////////////////////////////////////////////////////////////////////
// Name: AES_Crypt
// Author: NIVIS LLC,
// Description: Perfrorms 128 AES encryption over p_pucToEncrypt buffer and
//                  32 bit authenthication over p_pucToAuthOnly | p_pucToEncrypt buffer
// Parameters:
//                p_pucKey  - encryption key (16 bytes long)
//                p_pucNonce  - nonce (13 bytes long)
//                p_pucToAuthOnly  - to authenticate buffer
//                p_unToAuthOnlyLen  - to authenticate buffer length
//                p_pucToEncrypt  - to autehnticate and encrypt buffer
//                p_unToEncryptLen  - to autehnticate and encrypt buffer len
//                p_pucEncryptedBuff  - encrypted buffer + 32 MIC
//                p_ucInterruptFlag - 1 if the call is from  interrupt, 0 from user space
// Return:        SUCCES or ERROR
// Obs:
//      Access level: Interrupt level for p_ucInterruptFlag = 1 and user level for p_ucInterruptFlag = 0
// NOTE :         Built for Auth
//          B0 = AuthFlags || Nonce N || l(m).         // l(m) = p_unToEncryptLen
//          Ai = EncrFlags || Nonce N || Counter i, for i = 0, 1, 2, …
///////////////////////////////////////////////////////////////////////////////////
__arm uint8 AES_Crypt
                        ( const uint8 * p_pucKey,
                          const uint8 * p_pucNonce,
                          const uint8 * p_pucToAuthOnly,
                          uint16        p_unToAuthOnlyLen,
                          const uint8 * p_pucToEncrypt,
                          uint16        p_unToEncryptLen,
                          uint8 *       p_pucEncryptedBuff,
                          uint8         p_ucInterruptFlag )
{
int32 lLen;

//SET_GPIO_HI( TEST_LINE_3 );

  do
  {
      startAesAndAuthOnly( p_pucKey, p_pucNonce, p_pucToAuthOnly, p_unToAuthOnlyLen, p_unToEncryptLen, p_ucInterruptFlag );
      AES_ON_MODE_CCM();          // continue with authetication and encryption of plain text

      // Encryption with Auth
      lLen = (uint32)p_unToEncryptLen;
      while ( lLen > 0 )
      {
          loadDataReg( p_pucToEncrypt, lLen );

          ASM_START_OPERATION();


#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER          
          if( !p_ucInterruptFlag )
          {
            ASM_END_EXCLUSIVE_ZONE();
            __asm("  Nop ");
            ASM_BEGIN_EXCLUSIVE_ZONE();
            if( g_ucAsmDirtyFlag )
            {
                len = p_unToEncryptLen-lLen;
                p_pucToEncrypt -= lLen;
                p_pucEncryptedBuff -= lLen;
                break;
            }
          }
#endif //AES_CANNOT_ENCRYPT_ON_SAME_BUFFER
          
          p_pucToEncrypt += 16;

          while ( ! (ASM_STATUS_DONE) )
            ;

          if( lLen < 16 )
          {
              uint32 ulX[4];
              putbackReg( (uint8*)ulX, &ASM_CTR_RESULT0 );
              memcpy( p_pucEncryptedBuff, ulX, lLen );
          }
          else
          {
              putbackReg( (uint8*)p_pucEncryptedBuff, &ASM_CTR_RESULT0 );
          }

          lLen -= 16;
          p_pucEncryptedBuff += 16;

          ASM_CTR3++;
      }

      AES_ON_MODE_CTR(); // encrypt 32 bits MIC

      ASM_DATA0 = ASM_CBC_RESULT0;

      ((uint8*)(&ASM_CTR3))[0] = 0;

      ASM_START_OPERATION();

      p_pucEncryptedBuff += lLen;

      while ( ! (ASM_STATUS_DONE) )
        ;

      lLen = ASM_CTR_RESULT0;
      AES_OFF();

      if( !p_ucInterruptFlag )
      {
        ASM_END_EXCLUSIVE_ZONE();
      }
  } while(g_ucAsmDirtyFlag);

  p_pucEncryptedBuff[0] =  (uint8) (((uint32)lLen) >> 24);
  p_pucEncryptedBuff[1] =  (uint8) (((uint32)lLen) >> 16);
  p_pucEncryptedBuff[2] =  (uint8) (((uint32)lLen) >> 8);
  p_pucEncryptedBuff[3] =  (uint8) ((uint32)lLen);

//  SET_GPIO_LO( TEST_LINE_3 );
  return AES_SUCCES;
}


///////////////////////////////////////////////////////////////////////////////////
// Name: AES_Decrypt
// Author: NIVIS LLC,
// Description: Perfrorms 128 AES decryption over p_pucToEncrypt buffer and
//                  32 bit authenthication over p_pucToAuthOnly | p_pucDecryptedBuff buffer
// Parameters:
//                p_pucKey  - encryption key (16 bytes long)
//                p_pucNonce  - nonce (13 bytes long)
//                p_pucToAuthOnly  - to authenticate buffer
//                p_unToAuthOnlyLen  - to authenticate buffer length
//                p_pucToDecrypt  - to autehnticate and decrypt buffer
//                p_unToDecryptLen  - to autehnticate and decrypt buffer len. !!! ??? ATTENTION :  must contain to decrypt len + 4 len of MIC 32
//                p_pucDecryptedBuff  - decrypted buffer ( fara 32 MIC )
//                p_ucInterruptFlag - 1 if the call is from  interrupt, 0 from user space
// Return:        SUCCES or ERROR
// Obs:
//      Access level: Interrupt level for p_ucInterruptFlag = 1 and user level for p_ucInterruptFlag = 0
// Obs:
//      Access level: Interrupt level only
///////////////////////////////////////////////////////////////////////////////////
__arm uint8 AES_Decrypt
                        ( const uint8 * p_pucKey,
                          const uint8 * p_pucNonce,
                          const uint8 * p_pucToAuthOnly,
                          uint16        p_unToAuthOnlyLen,
                          const uint8 * p_pucToDecrypt,
                          uint16        p_unToDecryptLen,
                          uint8 *       p_pucDecryptedBuff ,
                          uint8         p_ucInterruptFlag )
{
int32  lLen;

//SET_GPIO_HI( TEST_LINE_3 );

  if ( p_unToDecryptLen < 4 )  return AES_ERROR;

  // recover the crypted MIC to decrypt it
  p_unToDecryptLen -=4;

  do
  {
      startAesAndAuthOnly( p_pucKey, p_pucNonce, p_pucToAuthOnly, p_unToAuthOnlyLen, p_unToDecryptLen, p_ucInterruptFlag );

      lLen = (uint32)p_unToDecryptLen;
      while ( lLen > 0 )
      {
          loadDataReg( p_pucToDecrypt, lLen );

          AES_ON_MODE_CTR();   // decryption only
          ASM_START_OPERATION();
          
#ifdef AES_CANNOT_ENCRYPT_ON_SAME_BUFFER          
          if( !p_ucInterruptFlag )
          {
            ASM_END_EXCLUSIVE_ZONE();
            __asm("  Nop ");
            ASM_BEGIN_EXCLUSIVE_ZONE();
            if( g_ucAsmDirtyFlag )
            {
                len = p_pucToDecrypt-lLen;
                p_pucToDecrypt -= lLen;
                p_pucDecryptedBuff -= lLen;
                break;
            }
          }
#endif //AES_CANNOT_ENCRYPT_ON_SAME_BUFFER
          
          p_pucToDecrypt += 16;
          
          while ( ! (ASM_STATUS_DONE) )
            ;

          if( lLen < 16 )
          {
              ASM_DATA0 = getZeroFilled4Bytes( ASM_CTR_RESULT0, lLen );
              ASM_DATA1 = getZeroFilled4Bytes( ASM_CTR_RESULT1, lLen-4 );
              ASM_DATA2 = getZeroFilled4Bytes( ASM_CTR_RESULT2, lLen-8 );
              ASM_DATA3 = getZeroFilled4Bytes( ASM_CTR_RESULT3, lLen-12 );
          }
          else
          {
              ASM_DATA0 = ASM_CTR_RESULT0;
              ASM_DATA1 = ASM_CTR_RESULT1;
              ASM_DATA2 = ASM_CTR_RESULT2;
              ASM_DATA3 = ASM_CTR_RESULT3;
          }

          AES_ON_MODE_CBC();   // auth only
          ASM_START_OPERATION();

          if( lLen < 16 )
          {
              uint32 ulX[4];
              putbackReg( (uint8*)ulX, &ASM_DATA0 );
              memcpy( p_pucDecryptedBuff, ulX, lLen );
          }
          else
          {
              putbackReg( (uint8*)p_pucDecryptedBuff, &ASM_DATA0 );
          }

          lLen -= 16;
          p_pucDecryptedBuff += 16;

//          while ( ! (ASM_STATUS_DONE) )
//            ;

          ASM_CTR3++;
      }

  } while(g_ucAsmDirtyFlag);

  AES_ON_MODE_CTR();   // decryption only

  ASM_DATA0 = ASM_CBC_RESULT0; // the computed MIC 32

  ((uint8*)(&ASM_CTR3))[0] = 0;

  ASM_START_OPERATION();

  p_pucToDecrypt += lLen;

  while ( ! (ASM_STATUS_DONE) )
  ;

  lLen = ASM_CTR_RESULT0;
  AES_OFF();

  if( !p_ucInterruptFlag )
  {
    ASM_END_EXCLUSIVE_ZONE();
  }

//  SET_GPIO_LO( TEST_LINE_3 );
  return (lLen == rotUnaligned4Bytes2( p_pucToDecrypt ) ? AES_SUCCES : AES_ERROR);
}


