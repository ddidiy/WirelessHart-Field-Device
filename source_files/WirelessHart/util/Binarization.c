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

#include "Binarization.h"


void memcpy_reverse(void * p_pDst, const void * p_pSrc, unsigned int p_unLen )
{
  while( p_unLen-- )
  {
      *(unsigned char*)p_pSrc = *(unsigned char*)p_pDst; 
      p_pDst = (void*)((unsigned char*)p_pDst+1);
      p_pSrc = (void*)((unsigned char*)p_pSrc+1);
  }
}

void ReadStreamPacket(BinaryStream * fromStream, uint8_t * bytes, unsigned int len)
{
    uint8_t * pFromBytes = fromStream->nextByte;
    for (uint8_t i=0; i<len; i=i+3) 
    {
        uint8_t pack = pFromBytes[i]>>2;
        bytes[0] = (pack & 0x20 ? pack : pack | 0x40 );/* instead of (pack | ((~pack<<1)&0x40)) */
        pack = (((pFromBytes[i]<<4) & 0x3F) | (pFromBytes[i+1]>>4));
        bytes[1] = (pack & 0x20 ? pack : pack | 0x40 );
        pack = (((uint8_t)(pFromBytes[i+1]<<2)) & 0x3F) | ((uint8_t)(pFromBytes[i+2]>>6));
        bytes[2] = (pack & 0x20 ? pack : pack | 0x40 );
        pack = pFromBytes[i+2] & 0x3F;
        bytes[3] = (pack & 0x20 ? pack : pack | 0x40 );
        
        bytes += 4;
    }
    STREAM_SKIP(fromStream, len);
}

void WriteStreamPacket(BinaryStream * toStream, const uint8_t * bytes, unsigned int len)
{
    STREAM_SET_ERROR_ON_COND(toStream,BinaryStream_Err_TooFewBytes, (length%4)!=0);
    for (int i=0;i<len;i=i+4) 
    {
        toStream->nextByte[0] = (uint8_t)(bytes[i] << 2)   | ((bytes[i+1] & 0x3F)>>4);
        toStream->nextByte[1] = (uint8_t)(bytes[i+1] << 4) | ((bytes[i+2] & 0x3F)>>2);
        toStream->nextByte[2] = (uint8_t)(bytes[i+2] << 6) | ( bytes[i+3] & 0x3F);
        STREAM_SKIP(toStream, 3);
    }
}

void ReadUint32(const void* p_pValue, uint8_t* p_pBuf, uint8_t* p_ucSize)
{
  *(p_pBuf++) = (*(uint32_t*)(p_pValue)) >> 24;
  *(p_pBuf++) = (*(uint32_t*)(p_pValue)) >> 16;
  *(p_pBuf++) = (*(uint32_t*)(p_pValue)) >> 8;
  *(p_pBuf++) = (*(uint32_t*)(p_pValue)); 
}

void WriteUint32(void* p_pValue, const uint8_t* p_pBuf, uint8_t p_ucSize)
{
  *(uint32_t*)(p_pValue) = ((uint32_t)*(p_pBuf++)) << 24;
  *(uint32_t*)(p_pValue) |= ((uint32_t)*(p_pBuf++)) << 16;
  *(uint32_t*)(p_pValue) |= ((uint32_t)*(p_pBuf++)) << 8;
  *(uint32_t*)(p_pValue) |= ((uint32_t)*(p_pBuf++));
}