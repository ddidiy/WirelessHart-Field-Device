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
* Name:         asm.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_ASM_H_
#define _NIVIS_ASM_H_

#include "typedef.h"

//#define AES_CANNOT_ENCRYPT_ON_SAME_BUFFER

#define AES_SUCCES              0
#define AES_ERROR               1

#define MAX_NONCE_LEN           13


__arm uint32 rotate4Bytes( uint32 p_ulData );

void ASM_Init(void);


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
///////////////////////////////////////////////////////////////////////////////////
__arm uint8 AES_Crypt
                        ( const uint8 * p_pucKey,
                          const uint8 * p_pucNonce,
                          const uint8 * p_pucToAuthOnly,
                          uint16        p_unToAuthOnlyLen,
                          const uint8 * p_pucToEncrypt,
                          uint16        p_unToEncryptLen,
                          uint8 *       p_pucEncryptedBuff,
                          uint8         p_ucInterruptFlag );


#define AES_Crypt_Intr(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff) \
        AES_Crypt(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff,1)


#define AES_Crypt_User(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff) \
        AES_Crypt(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff,0)

///////////////////////////////////////////////////////////////////////////////////
// Name: AES_Decrypt
// Author: NIVIS LLC,
// Description: Perfrorms 128 AES decryption over p_pucToDecrypt buffer and
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
                          uint8         p_ucInterruptFlag );

#define AES_Decrypt_Intr(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff) \
        AES_Decrypt(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff,1)


#define AES_Decrypt_User(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff) \
        AES_Decrypt(p_pucKey,p_pucNonce,p_pucToAuthOnly,p_unToAuthOnlyLen,p_pucToEncrypt,p_unToEncryptLen,p_pucEncryptedBuff,0)

#endif /* _NIVIS_ASM_H_ */

