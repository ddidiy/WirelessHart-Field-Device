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
* Name:         MC1322x.h
* Author:       Marius Vilvoi
* Date:         October 2007
* Description:  This header file
* Changes:
* Revisions:
****************************************************************************************************/
#ifndef _NIVIS_TYPEDEF_H_
#define _NIVIS_TYPEDEF_H_

#include <stdint.h>

/* Types definition */
typedef unsigned char   byte;
typedef unsigned short  word;
typedef unsigned long   dword;

typedef signed   char   int8;
typedef unsigned char   uint8;
typedef signed   short  int16;
typedef unsigned short  uint16;
typedef signed   long   int32;
typedef unsigned long   uint32;
typedef float           fp32;

typedef void (* handler_func_type)(void);

typedef uint16  HANDLE;
typedef uint16  SHORT_ADDR;
typedef uint8   UNIQUE_ID[5];
//typedef uint8   LONG_ADDR[8];
//typedef uint8   IPV6_ADDR[16];
typedef uint8   EUI64_ADDR[8];
typedef uint32  TIME;         // [1/32 ms]
typedef uint32  TIME_10MS;    // [10 ms]
typedef uint32  TIME_250MS;   // [250 ms]

typedef union        // bit-definitions
{
  struct
  {
    unsigned char b0:1;
    unsigned char b1:1;
    unsigned char b2:1;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char b6:1;
    unsigned char b7:1;
  }flags;
  unsigned char byte;
}Bit_Field;

/*
// enum from ISA100
typedef enum
{
  SUCCESS,
  UNSPECIFIED_ERROR,
  READ_ONLY,
  WRITE_ONLY,
  INVALID_PARAMETER,
  INVALID_ADDRESS,
  DUPLICATE,
  OUT_OF_MEMORY,
  UID_OUT_OF_RANGE,
  DATA_TOO_LONG,
  NO_CHANNEL,
  NO_TIMESLOT,
//  NO_NEIGHBOR,
//  NO_SUPERFRAME,
//  NO_GRAPH,
  NO_LINK,
  RX_LINK,
  TX_LINK,
  NO_ROUTE,
  NO_CONTRACT,
  NO_UID,
  TOO_BIG,
  QUEUE_FULL,
  NACK,
  TIMEOUT,
  MSG_NOT_FOUND,
  INVALID_SC
}SC;
*/

/* 
 *  Global status type for Wireless HART.
 *  This type should be maybe extended. 
 */
typedef enum
{
  SUCCESS = 0,
  UNSPECIFIED_ERROR,
  DLL_OUT_OF_MEMORY,
  DLL_DISCARD_PACKET,
  TL_OUT_OF_MEMORY,
  ULINK_OUT_OF_MEMORY,
  APP_DOMAIN_NOT_MATCH,
  NO_SESSION,
  NO_ROUTE,
  NO_SOURCE_ROUTE,
  PAYLOAD_TOO_LONG,
  PACKET_WRITTEN_TO_TRANSPORT_EXTRA_BUFFER,
//  INVALID_PARAMETER,
  INVALID_HANDLER,
  INVALID_SEQUENCE_NUMBER,
  FAILURE,
  OUT_OF_MEMORY
  // TBD
}SC;

//#define TRUE    1
//#define FALSE   0

typedef enum { FALSE, TRUE } Boolean;

#define BIT0    0x00000001UL
#define BIT1    0x00000002UL
#define BIT2    0x00000004UL
#define BIT3    0x00000008UL
#define BIT4    0x00000010UL
#define BIT5    0x00000020UL
#define BIT6    0x00000040UL
#define BIT7    0x00000080UL
#define BIT8    0x00000100UL
#define BIT9    0x00000200UL
#define BIT10   0x00000400UL
#define BIT11   0x00000800UL
#define BIT12   0x00001000UL
#define BIT13   0x00002000UL
#define BIT14   0x00004000UL
#define BIT15   0x00008000UL
#define BIT16   0x00010000UL
#define BIT17   0x00020000UL
#define BIT18   0x00040000UL
#define BIT19   0x00080000UL
#define BIT20   0x00100000UL
#define BIT21   0x00200000UL
#define BIT22   0x00400000UL
#define BIT23   0x00800000UL
#define BIT24   0x01000000UL
#define BIT25   0x02000000UL
#define BIT26   0x04000000UL
#define BIT27   0x08000000UL
#define BIT28   0x10000000UL
#define BIT29   0x20000000UL
#define BIT30   0x40000000UL
#define BIT31   0x80000000UL

#ifndef NULL
#define NULL (void *)(0)
#endif

#define UAP_TYPE_SIMPLE_API              1 // Used for modem running UAP.
#define UAP_TYPE_WHART_API               2 // Used for Application Processor running UAP.

#define EXT_BOARD_WAKE_UP_2_WIRES 1
#define EXT_BOARD_WAKE_UP_3_WIRES 2
#define EXT_BOARD_WAKE_UP_4_WIRES 3
#define EXT_BOARD_WAKE_UP_5_WIRES 4
#define EXT_BOARD_WAKE_UP_6_WIRES 5

//            
// Hardware Platform (ISA100)
//
//#define PLATFORM_UNKNOWN          1
//#define PLATFORM_DEVELOPMENT      2   /* Nivis ISA100 Raptor COMM Interface 52000127 Rev. D + Crow Development Board (Raptor form factor) 52000144 Rev. A + Crow Modem 51000143 Rev. D */
//#define PLATFORM_INTEGRATION_KIT  3   /* Nivis ISA100/WirelessHart Integration Kit platform 52000158 Rev. A + Crow Modem 51000143 Rev. E */
//#define PLATFORM_GE_DEVICE        4   /* GE Device */
//#define PLATFORM_YOKOGAWA         5   /* YOKOGAWA ISA100 simple API 51000160 + Crow Modem 51000143 rev.E */

#endif /* _TYPEDEF_H_ */


