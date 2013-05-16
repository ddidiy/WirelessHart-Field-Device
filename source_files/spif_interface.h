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

#ifndef _SPIF_INTERFACE_H_
#define _SPIF_INTERFACE_H_

#include "LibInterface/EmbeddedTypes.h"
#include "LibInterface/NVM.h" 

/***************************[ public includes ]*******************************/
/***************************[ public defines ]********************************/

/***************************[ public datatypes ]******************************/
/***************************[ public variables ]******************************/
/***************************[ public functions ]******************************/
void NVM_FlashInit( void );

// 4k sector, defined by bits (last sector is 0x80000000 ) 
#define NVM_FlashErase(sector)         NVM_Erase( gNvmInternalInterface_c, gNvmType_SST_c, sector)
#define NVM_FlashWrite(pSrc,addr,size) NVM_Write( gNvmInternalInterface_c, gNvmType_SST_c, pSrc, addr, size)
#define NVM_FlashRead( pDst,addr,size) NVM_Read(  gNvmInternalInterface_c, gNvmType_SST_c, pDst, addr, size)
#define NVM_FlashBlankCheck(addr,size) NVM_BlankCheck(gNvmInternalInterface_c, gNvmType_SST_c, addr, size)


#endif  /*_SPIF_INTERFACE_H_ */


