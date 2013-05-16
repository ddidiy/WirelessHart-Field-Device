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
* Name:         global.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_GLOBAL_H_
#define _NIVIS_GLOBAL_H_

#include "typedef.h"

//=============================================================================
// Firmware Version: v X.Y.Z
//
//X: major release with identifiable new features 
//Y: minor release with existing feature improvements, addition of minor features and/or bug fixes (sent out to customers for update) or special customer target.
//Z: development build (development release, release to QA, internal bug fixing release)

#define MAJOR_VERSION         (uint8)1
#define MINOR_VERSION         (uint8)0
#define BUILD                 (uint8)0x00     // BUILD is defined like: 0x14, which means decimal 14
#define BUILD_LETTER          ' '             // 



//=============================================================================


// GLOBAL DEFINES 
#define AES_KEY_LENGTH      16

#define NM_NICKNAME       0xF980
#define GW_NICKNAME       0xF981

extern uint8 g_ucCanSleep;


#define POWER_ON_RUN          8900
#define POWER_ON_IDLE         1000
#define POWER_ON_HIBERNATE     22
#define POWER_ON_RX           (25800 - POWER_ON_RUN)
#define POWER_ON_TX           (59000 - POWER_ON_RUN)

void initZeroFunction(void);

extern const unsigned int g_aCCITTCrcTable[256];
#define COMPUTE_CRC(nAcumulator,chBuff) ((nAcumulator) << 8) ^ g_aCCITTCrcTable[((nAcumulator) >> 8) ^ (chBuff) ]

uint16 ComputeCCITTCRC(const unsigned char *p_pchBuf, unsigned char p_chLen);
void InverseMemcpy (void* p_pDst, void* p_pSrc, uint8 p_ucSize);
void DelayLoop(uint32 p_ulCount);

extern uint8 g_ucResetReason;
extern uint8 g_ucDllDisconnect;
inline void PrepareForReset(p_ucResetReason) { if(!g_ucDllDisconnect) g_ucResetReason = p_ucResetReason; }

  #define  Energy_AddConsumption( ... )

  
#endif /* _NIVIS_GLOBAL_H_ */

