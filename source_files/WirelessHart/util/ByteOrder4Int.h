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

#ifndef BYTE_ORDER_4_INT_H_
#define BYTE_ORDER_4_INT_H_

#include <stdint.h>

#define BYTEORDER_REVERT16(x) \
	((uint16_t)(\
			(((uint16_t)(x) & 0x00ff) << 8) |\
		  (((uint16_t)(x) & 0xff00) >> 8)))

#define BYTEORDER_REVERT32(x) \
	((uint32_t)(\
			 (((uint32_t)(x) & 0x000000ffUL) << 24) |\
		   (((uint32_t)(x) & 0x0000ff00UL) <<  8) |\
		   (((uint32_t)(x) & 0x00ff0000UL) >>  8) |\
		   (((uint32_t)(x) & 0xff000000UL) >> 24)))

#define BYTEORDER_MAKE_UINT16(h, l) \
	((uint16_t)(\
			 (((uint16_t)(h) & 0x00ff) << 8) |\
			  ((uint16_t)(l) & 0x00ff)))

#define BYTEORDER_MAKE_UINT32(h, m1, m2, l) \
	((uint32_t)(\
			  (((uint32_t)(h)  & 0x000000ffUL) << 24) |\
			  (((uint32_t)(m1) & 0x000000ffUL) << 16) |\
			  (((uint32_t)(m2) & 0x000000ffUL) << 8)  |\
			   ((uint32_t)(l)  & 0x000000ffUL)))

#endif /* BYTEORDER4INT_H_ */
