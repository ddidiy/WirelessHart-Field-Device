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

#ifndef BINARIZATION_H_
#define BINARIZATION_H_

#include "WirelessStackTypes.h"
#include <string.h>

typedef enum
{
	//operation succeeds
	BinaryStream_Success = 0,
	//last operation failed, to few bytes written or read from stream
	BinaryStream_Err_TooFewBytes = 1
} BinaryStreamErrors;


/**
 * Abstracts a stream of bytes. Provides the reading/writing primitive types (int8,16,24,32,strings,bytes, etc.)
 *  in little-endian or big-endian bytes order.
 *  To enable little-endian add a #define IS_MACHINE_BIG_ENDIAN
 *
 *  The stream hold a single pointer for writing or reading(Normaly will be used only for reading or only for writing);
 *  If you want to have diffrent pointer fro read & read declar 2 stream over the same buffer.
 */
typedef struct
{
	uint8_t* nextByte ;// a pointer of the next written/read byte from the stream
	uint8_t* endByte ;// a pointer of the next last byte from the stream

#if defined(_DEBUG)
	//TODO [nicu.dascalu] - add another info for debug
	BinaryStreamErrors error; //holds the latest error operation
#endif
} BinaryStream;

inline
int32_t STREAM_GetRemainingSize(BinaryStream * pStream)
{
    return (pStream->endByte - pStream->nextByte);
}

 #define STREAM_CHECK_AVAILABLE(stream, availableBytes)\
	if ( STREAM_GetRemainingSize(stream)  < availableBytes) return RCS_E05_TooFewDataBytesReceived; //HARCODED 

void memcpy_reverse(void * p_pDst, const void * p_pSrc, unsigned int p_unLen );

/**
 * Internaly used. for converting int 2 bytes.
 */
typedef union
{
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
	float fl;
	uint8_t bytes[4];
} Binarization_Type2Bytes;

#ifndef internal_nocall_
#		define internal_nocall_ (void)0
#endif

#if defined(IS_MACHINE_BIG_ENDIAN) && defined(IS_MACHINE_LITTLE_ENDIAN)
#	error "Only one byte order(IS_MACHINE_BIG_ENDIAN or IS_MACHINE_LITTLE_ENDIAN) should be defined!"

#elif !defined(IS_MACHINE_BIG_ENDIAN) && !defined(IS_MACHINE_LITTLE_ENDIAN)
#	error "No byte order(IS_MACHINE_LITTLE_ENDIAN or IS_MACHINE_BIG_ENDIAN) specified!"

#endif

/**
 * Used just for debugging purpose, for assembly generated code.
 */
//#ifdef _DEBUG
//#	define STREAM_PRINT_FUNC_NAME(name) {char macro_##name; macro_##name;}
//#else
#	define STREAM_PRINT_FUNC_NAME(name)
//#endif

/**
 * Initialize the stream. This method should be called before using the stream (is like a c++ ctor).
 * Alos can be called more than once (second time will reset the stream)
 */
#define STREAM_INIT(stream, buffer, bufferLength)\
	{\
		STREAM_PRINT_FUNC_NAME(STREAM_INIT) \
		(stream)->nextByte = (uint8_t*)(buffer);\
		(stream)->endByte = (uint8_t*)(buffer) + (bufferLength);\
		STREAM_RESET_ERROR(stream);\
	}

/**
 * Stream error code access macros.
 */
#ifdef _DEBUG
#	define STREAM_RESET_ERROR(stream)	(stream)->error = BinaryStream_Success

#	define STREAM_SET_ERROR_ON_COND(stream, err, cond)\
		if (cond) (stream)->error = (err)

#	define STREAM_GET_ERROR(stream) (stream)->error

#else
#	define STREAM_RESET_ERROR(stream) internal_nocall_
#	define STREAM_SET_ERROR_ON_COND(stream, err, cond) internal_nocall_
# define STREAM_GET_ERROR(stream) BinaryStream_Success
#endif

/**
 * Navigate forward into stream.
 */
#define STREAM_SKIP(stream, count)\
	{\
		STREAM_SET_ERROR_ON_COND(stream, BinaryStream_Err_TooFewBytes, count > STREAM_GetRemainingSize(stream));\
		(stream)->nextByte += count;\
	}


/**
 * Writes bytes into stream. The stream memory should not overlap with provided buffer.
 * @stream - pointer to writable stream
 * @bytes - address of starting bytes buffer
 * @length - number of bytes from buffer that should be written
 */
#define STREAM_WRITE_BYTES(stream, bytes, length)\
	{\
		memcpy((stream)->nextByte, (void*)(bytes), (length));\
		STREAM_SKIP(stream, length);\
	}
/**
 * Read bytes from stream.. The stream memory should not overlap with provided buffer.
 * @stream - pointer to readable stream
 * @bytes - address of starting bytes buffer
 * @length - number of bytes from buffer that should be written
 */
#define STREAM_READ_BYTES(stream, bytes, length)\
	{\
		memcpy((void*)(bytes), (stream)->nextByte, (length));\
		STREAM_SKIP(stream, length);\
	}
inline
void STREAM_WRITE_UINT8(BinaryStream * toStream, uint8_t value)
{
    *toStream->nextByte++ = value;
}

#define STREAM_WRITE_INT8(stream, value) STREAM_WRITE_UINT8(stream, (uint8_t)value)

#define STREAM_READ_UINT8(fromStream, value) *(value) = *(fromStream)->nextByte++

inline 
void STREAM_READ_INT8(BinaryStream * fromStream, int8_t * value)
{
    STREAM_READ_UINT8( fromStream, (uint8_t *) value );
}

inline
void STREAM_WRITE_UINT16(BinaryStream * toStream, uint16_t value)
{
    toStream->nextByte[0] = (uint8_t)(value >> 8);
    toStream->nextByte[1] = (uint8_t)value;
    STREAM_SKIP(toStream, 2);
}

#define STREAM_WRITE_INT16(stream, value) STREAM_WRITE_UINT16(stream, (uint16_t)value)

inline
void STREAM_READ_UINT16(BinaryStream * fromStream, uint16_t * value)
{
    *value = ((uint16_t)(fromStream->nextByte)[0] << 8) | fromStream->nextByte[1];
    STREAM_SKIP(fromStream, 2);
}

inline
void STREAM_READ_INT16(BinaryStream * fromStream, int16_t * value)
{
    STREAM_READ_UINT16( fromStream, (uint16_t *) value );
}

inline
void STREAM_WRITE_UINT24(BinaryStream * toStream, uint32_t value)
{
    toStream->nextByte[0]= (uint8_t)(value >> 16);
    toStream->nextByte[1] = (uint8_t)(value >> 8);
    toStream->nextByte[2] = (uint8_t)(value);
    STREAM_SKIP(toStream, 3);
}
#define STREAM_WRITE_INT24(stream, value) STREAM_WRITE_UINT24(stream, (uint32_t)value)

inline
void STREAM_READ_UINT24(BinaryStream * fromStream, uint32_t * value)
{
  *value =  ((uint32_t)(fromStream->nextByte[0]) << 16) 
            | ((uint32_t)(fromStream->nextByte[1]) << 8) 
            | ((uint32_t)(fromStream->nextByte[2])); 
  STREAM_SKIP(fromStream, 3);
}
inline
void STREAM_READ_INT24(BinaryStream * fromStream, int32_t * value)
{
  STREAM_READ_UINT24(fromStream, (uint32_t *) value);
}

inline
void STREAM_WRITE_UINT32(BinaryStream * toStream, uint32_t value)
{
    toStream->nextByte[ 0 ] = (uint8_t)(value >> 24);
    toStream->nextByte[ 1 ] = (uint8_t)(value >> 16);
    toStream->nextByte[ 2 ] = (uint8_t)(value >> 8);
    toStream->nextByte[ 3 ] = (uint8_t)(value);
    STREAM_SKIP(toStream, 4);
}

#define STREAM_WRITE_INT32(stream, value) STREAM_WRITE_UINT32(stream, (uint32_t)value)

inline
void STREAM_READ_UINT32(BinaryStream * fromStream, uint32_t * value)
{
    *value =  ((uint32_t)(fromStream->nextByte[0]) << 24) 
              | ((uint32_t)(fromStream->nextByte[1]) << 16) 
              | ((uint32_t)(fromStream->nextByte[2]) << 8) 
              | fromStream->nextByte[3];
    STREAM_SKIP(fromStream, 4);
}

inline
void STREAM_READ_INT32(BinaryStream * fromStream, int32_t * value)
{
    STREAM_READ_UINT32(fromStream, (uint32_t *) value);
}

#define STREAM_WRITE_TIME(stream, value)\
	{\
		STREAM_WRITE_UINT32(stream, value.u32);\
	}

#define STREAM_READ_TIME(stream, value)\
	{\
		STREAM_READ_UINT32(stream, &(value)->u32);\
	}

#define STREAM_WRITE_DATE(stream, value) \
	{\
		STREAM_WRITE_UINT8(stream, value.day);\
		STREAM_WRITE_UINT8(stream, value.month);\
		STREAM_WRITE_UINT8(stream, value.year);\
	}

#define STREAM_READ_DATE(stream, value) \
	{\
		STREAM_READ_UINT8(stream, &(value)->day);\
		STREAM_READ_UINT8(stream, &(value)->month);\
		STREAM_READ_UINT8(stream, &(value)->year);\
	}

/*
 * A string using the 8-bit ISO Latin-1 character set. Latin-1 strings are padded out
 *	with zeroes (0x00).
 */
#define STREAM_WRITE_LATIN(stream, str, length) STREAM_WRITE_BYTES(stream, str, length)
#define STREAM_READ_LATIN(stream, str, length) STREAM_READ_BYTES(stream, str, length)

/*
 * A string consisting of 6-bit alpha-numeric characters that are a subset of the
 *	ASCII character set. This allows four characters to be packed into three bytes.
 *	Packed ASCII strings are padded out with space (0x20) characters.
 * The memory shouldn't be overlapped.
 * length must be a constant (such as sizeof()); it represents the no of unpacked chars
 *      and must be multiple of 4, while the consumed stream bytes will be a multiple of 3.
 */
void ReadStreamPacket(BinaryStream * fromStream, uint8_t * bytes, unsigned int length);
void WriteStreamPacket(BinaryStream * toStream, const uint8_t * bytes, unsigned int len);

#define STREAM_READ_PACKED(stream, bytes, length) ReadStreamPacket( stream, bytes, (length) * 6 /8 )
#define STREAM_WRITE_PACKED(stream, bytes, length) WriteStreamPacket(stream, bytes, length)

void ReadUint32(const void * p_pValue, uint8_t * p_pBuf, uint8_t* p_ucSize);
void WriteUint32(void * p_pValue, const uint8_t * p_pBuf, uint8_t p_ucSize);

//FLOAT binarization
#include <float.h>

inline
void  STREAM_WRITE_FLOAT(BinaryStream * toStream, float  value) 
{
    STREAM_WRITE_UINT32( toStream, *(uint32_t*)&value);
}

inline
void  STREAM_READ_FLOAT(BinaryStream * fromStream, float * value)
{
    STREAM_READ_UINT32( fromStream, (uint32_t*)value);
}

#endif /*BINARIZATION_H_*/
