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
* Name:         spi.h
* Author:       Dorin Muica
* Date:         February 2009
* Description:  This header file is provided ...
*               This file holds definitions of the ...
* Changes:
* Revisions:
***************************************************************************************************/

#ifndef _NIVIS_SPI_H_
#define _NIVIS_SPI_H_

#include "typedef.h"

#if ( SPI1_MODE != NONE )
    #include "CommonAPI/RadioApi.h"

    void SPI_Init( void );

    uint8 SPI_ReadBuff(unsigned char* p_pSPI1Msg,
                       unsigned char p_ucMsgLen);

    SC SPI_WriteBuff( const ApiMessageFrame * p_pMsgHeader,
                      const uint8*  p_pMsgPayload);


    extern volatile uint8 g_ucSpiBusy;
    extern uint16 g_unTxQueueSize;
    uint8 SPI_UpdateSpiSpeed(enum SPI_SPEED p_NewSpeed);

    void SPI_CheckTimeout(uint16 TimeOutIn10Ms);
    #define  SPI_IsBusy() g_ucSpiBusy

    
    // 4 bytes header , 2 bytes APDU
    #define SPI_BUFFER_SIZE MAX_API_BUFFER_SIZE

#else

    #define SPI_Init()

#endif // SPI1_MODE


#endif /* _SPI_H_ */
