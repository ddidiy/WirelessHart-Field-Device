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

#ifndef WIRELESS_STACK_TYPES_H_
#define WIRELESS_STACK_TYPES_H_

/*
 * Use these macros for specified project.
 defined(WIRELESS_HART_DEVICE)
 defined(HART_GATEWAY)
 defined(HART_NETWORKMANAGER)
 defined(HART_ACCESSPOINT)
 defined(HART_HOSTAPPLICATION)
 */

#include <stdint.h>

#ifdef WIRELESS_HART_DEVICE
typedef uint8_t _command_size_t;
#else
typedef uint16_t _command_size_t;
#endif

// Representation of the Unsigned-40.
typedef uint8_t _device_address_t[5];

//The Date consists of three 8-bit binary unsigned integers representing, respectively,
//the day, month, and year minus 1900. Date is transmitted day first followed by the month and year bytes.
typedef union
{
  uint8_t date[3];
  struct
  {
      uint8_t day;
      uint8_t month;
      uint8_t year;
  };
} _date_t;

//The Time consists of a unsigned 32-bit binary integer
//with the least significant bit representing 1/32 of a millisecond (i.e., 0.03125 milliseconds).
typedef union
{
	uint32_t u32;
	struct
	{
		uint32_t scalemiliseconds :5; //represents 1/32 of a millisecond
		uint32_t miliseconds :27;
	} tm;
} _time_t;

typedef uint8_t _time40_t[5];

#define MAX_VALUE(x, y) ((x > y) ? (x) : (y))
#define MIN_VALUE(x, y) ((x < y) ? (x) : (y))

#endif /*WIRELESS_STACK_TYPES_H_*/
