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

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

/***************************[ public includes ]*******************************/
#include <stddef.h>
#include <limits.h>
//#include "fff.h"

/***************************[ public defines ]********************************/

/***************************[ public datatypes ]******************************/


#ifdef  ULLONG_MAX

  /** \def U8_CONST(arg)
   *  Macro for unsigned eight byte constants.
   *
   *  The argument of this macro shall be a decimal,octal od hexadecimal
   *  constant with a value that does not excee the limits for the U8 type.
   *
   *  \param arg constant expression.
   *
   *  \hideinitializer
   *  \see U8
   */


  /** \def U8_MAX
   * Maximum value of U8 integer type.
   *
   *  \see U8 U8_CONST()
   *
   */

  /**
   *  Eight bytes unsigned type.
   *  \hideinitializer
   *  \see U8_CONST() U8_MAX
   */
  #if (0xFFFFFFFFFFFFFFFFULL == ULLONG_MAX)
    typedef unsigned long long U8;
    #define U8_CONST(arg) arg##ULL
    #define U8_MAX U8_CONST(18446744073709551615)
  #else
    #error No type for U8
  #endif

  /**
   *  Fast eight bytes unsigned type.
   *  \hideinitializer
   */
  #if (0xFFFFFFFFFFFFFFFFULL == ULLONG_MAX)
    typedef unsigned long  UX8;
    #define UX8_CONST(arg) arg##ULL
  #else
    #error No type for UX8
  #endif
#endif

#ifdef  LLONG_MAX
  /**
   *  Eight bytes signed type.
   *  \hideinitializer
   */
  #if (0x7FFFFFFFFFFFFFFFLL == LLONG_MAX)
    typedef signed long  S8;
    #define S8_CONST(arg) arg##LL
  #else
    #error No type for S8
  #endif
#endif

#ifdef  LLONG_MAX
  /**
   *  Fast four bytes signed type.
   *  \hideinitializer
   */
  #if (0x7FFFFFFFFFFFFFFFLL == LLONG_MAX)
    typedef signed long  SX8;
    #define SX8_CONST(arg) arg##LL
  #else
    #error No type for SX8
  #endif
#endif

/**
 *  Four bytes unsigned type.
 *  \hideinitializer
 */
#if (0xFFFFFFFFUL == ULONG_MAX)
  typedef unsigned long  U4;
  #define U4_CONST(arg) arg##UL
#else
  #error No type for U4
#endif

/**
 *  Fast four bytes unsigned type.
 *  \hideinitializer
 */
#if (0xFFFFFFFFUL == ULONG_MAX)
  typedef unsigned long  UX4;
  #define UX4_CONST(arg) arg##UL
#else
  #error No type for UX4
#endif

/**
 *  Four bytes signed type.
 *  \hideinitializer
 */
#if (0x7FFFFFFFL == LONG_MAX) && (0x7FFFFFFFL == LONG_MAX)
  typedef signed long  S4;
  #define S4_CONST(arg) arg##L
#else
  #error No type for S4
#endif

/**
 *  Fast four bytes signed type.
 *  \hideinitializer
 */
#if (0x7FFFFFFFL == LONG_MAX)
  typedef signed long  SX4;
 #define SX4_CONST(arg) arg##L
#else
  #error No type for SX4
#endif

/**
 *  Two bytes unsigned type.
 *  \hideinitializer
 */
#if (0xFFFFU == USHRT_MAX)
  typedef unsigned short U2;
  #define U2_CONST(arg) arg##U
#else
  #if (0xFFFFU == UINT_MAX)
    typedef unsigned int U2;
    #define U2_CONST(arg) arg##U
  #else
    #error No type for U2
  #endif
#endif


#define U2_MAX U2_CONST(0xFFFF)
#define U2_MIN U2_CONST(0x0000)


/**
 *  Fast two bytes unsigned type.
 *  \ToDo Has a weak condition to decide the fastest type, Should be observed.
 *  \hideinitializer
 */
#if (0xFFFFFFFFU==UINT_MAX)
  typedef unsigned int UX2;
  #define UX2_CONST(arg) arg##U
#else
  #if (0xFFFFU==UINT_MAX)
    typedef unsigned int UX2;
    #define UX2_CONST(arg) arg##U
  #else
    #error No type for UX2
  #endif
#endif




/**
 *  Two bytes signed type.
 *  \hideinitializer
 */
#if (0x7FFF == SHRT_MAX)
  typedef signed short S2;
   #define S2_CONST(arg) arg
#else
  #if (0x7FFF == INT_MAX)
    typedef signed int S2;
    #define S2_CONST(arg) arg##U
  #else
    #error No type for S2
  #endif
#endif


/**
 *  Fast two bytes signed type.
 *  \ToDo Has a weak condition to decide the fastest type, Should be observed.
 *  \hideinitializer
 */
#if  (0x7FFFFFFF == INT_MAX)
  typedef int SX2;
  #define SX2_CONST(arg) arg
#else
  #if (0x7FFF == INT_MAX)
    typedef int SX2;
    #define SX2_CONST(arg) arg
  #else
    #error No type for SX2
  #endif
#endif


/**
 *  One byte unsigned type.
 *  \hideinitializer
 */
#if (0xFF == UCHAR_MAX)
  typedef unsigned char  U1;
  #define U1_CONST(arg) arg##U
#else
  ERROR No type for U1
#endif

/**
 *  Fast one byte unsigned type.
 *  \ToDo Has a weak condition to decide the fastest type, Should be observed.
 *  \hideinitializer
 */
#if (0xFFFFFFFFU==UINT_MAX)
  typedef unsigned int UX1;
  #define UX1_CONST(arg) arg##U
#else
  #if (0xFFFFU==UINT_MAX)
    typedef unsigned char UX1;
    #define U1_CONST(arg) arg##U
  #else
    #error No type for UX1
  #endif
#endif

/**
 *  Unsinged type capable of holding object pointers.
 *
 *  The U0 type designates a unsigned integer type with the property that any valid
 *  pointer to void can be converted to this type, then convertd back to pointer
 *  to void, and the result will compare eqal to the original pointer.
 *
 *  \test size of this type have to be bigger or equal to a void*.
 *
 *  \hideinitializer
 */
typedef unsigned int U0;


/**
 *  Singed type capable of holding object pointers.
 *
 *  The S0 type designates a signed integer type with the property that any valid
 *  pointer to void can be converted to this type, then convertd back to pointer
 *  to void, and the result will compare eqal to the original pointer.
 *
 *  \test size of this type have to be bigger or equal to a void*.
 *
 *  \hideinitializer
 */
typedef signed int S0;




/**
 *  One Byte signed type.
 *  \hideinitializer
 */
#if (0x7F==SCHAR_MAX)
  typedef signed   char  S1;
  #define S1_CONST(arg) arg
#else
  #error No type for S1
#endif


/**
 *  Fast one byte signed type.
 *  \ToDo Has a weak condition to decide the fastest type, Should be observed.
 *  \hideinitializer
 */
#if (0xFFFFFFFFU==UINT_MAX)
  typedef unsigned int SX1;
  #define SX_MAX  INT_MAX
  #define SX_MIN  INT_MIN
  #define SX1_CONST(arg) arg
#else
  #if (0xFFFFU==UINT_MAX)
    typedef unsigned char SX1;
    #define SX_MAX  CHAR_MAX
    #define SX_MIN  CHYR_MIN

    #define SX1_CONST(arg) arg
  #else
    #error No type for SX1
  #endif
#endif

/**
 *  Character type.
 *  \hideinitializer
 */
#if (CHAR_MAX == SCHAR_MAX)
  typedef signed char  CH;
  #define CH_CONST(arg) arg
    #define CH_MAX 127
    #define CH_MIN -128
    #define CH_CONST(arg) arg
#else
  #if (CHAR_MAX == UCHAR_MAX)
    typedef unsigned char  CH;
    #define CH_MAX 0xFF
    #define CH_MIN 0x00
    #define CH_CONST(arg) arg##U
  #else
    #error no type for CH
  #endif
#endif

/**
 *  Void type.
 *  \hideinitializer
 */
typedef void VO;

#ifdef TRUE
  #undef TRUE
  #define _UNDEF_TRUE_
#endif

#ifdef FALSE
  #undef FALSE
  #define _UNDEF_FALSE_
#endif

/**
 *  Boolean type.
 *  \hideinitializer
 */
typedef enum
{
  FALSE=0,
  TRUE=1
}BO;

#ifdef _UNDEF_TRUE_
  #undef  _UNDEF_TRUE_
  #define TRUE 0x01
#endif

#ifdef _UNDEF_FALSE_
  #define _UNDEF_FALSE_
  #define FALSE 0x00
#endif

/**
 *  Standard return type.
 *  This type should be used as return type for functions.
 *  \showinitializer
 */
typedef enum
{
  SUCCESS           = 0,  /**< Function was successfully completed.                            */
  ERROR             =-1,  /**< Non specific error.                                             */
  ERROR_NULL        =-2,  /**< A NULL pointer was passed when a non-NULL pointer was expected. */
  ERROR_ZERO        =-3,  /**< A passed value was zero when no zero can be accepted.           */
  ERROR_MEMORY      =-4,  /**< Out of memory.                                                  */
  ERROR_RANGE       =-5,  /**< A passed value was out of Range.                                */
  ERROR_OVERRUN     =-5,  /**< A passed value was to big.                                      */
  ERROR_UNDERRUN    =-7,  /**< A passed value was to small                                     */
  ERROR_BUFFER_FULL =-8,  /**< A buffer was full.                                              */
  ERROR_BUFFER_EMTPY=-9,  /**< A buffer was empty.                                             */
  ERROR_BUSY        =-10, /**< A resource was busy.                                            */
  ERROR_TIME_OUT    =-11, /**< Something take to much time.*/
  ERROR_OPEN        =-12  /**< A device can´t be opened.*/
}RC;

/**
 *  Bit-Definition type.
 */
typedef union        
{
  struct
  {
    U1 b0:1;
    U1 b1:1;
    U1 b2:1;
    U1 b3:1;
    U1 b4:1;
    U1 b5:1;
    U1 b6:1;
    U1 b7:1;
  }flags;
  U1 byte;
}Bit_Field;

#endif  /*_TYPES_H_*/


