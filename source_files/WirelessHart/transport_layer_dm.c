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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name:         transport_layer_dm.c
// Author:       Nivis, LLC
// Date:         July 2008
// Description:  Transport Layer Management
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "transport_layer.h"
#include "provision.h"
#include "../asm.h"
#include "network_layer_dm.h"
#include "transport_layer_dm.h"



  ///////////////////////////////////////////////////////////////////////////////////
  // Name: FindTransportEntryByHandle
  // Author: NIVIS LLC, Dorin Pavel
  // Description: find an entry in TransportTable by its handler
  // Parameters: 
  //              p_hHandle - searched handler
  // Return: entry index
  ///////////////////////////////////////////////////////////////////////////////////
uint8 FindTransportEntryByHandle( HANDLE p_hHandle )
{
  uint8 ucIdx=0;
  for(; ucIdx < MAX_TRANSPORT_NO; ucIdx++ )
  {
    if( (0 == (g_aTransportTable[ucIdx].m_ucRetryCount & UNUSED_ENTRY)) // used entry
       && g_aTransportTable[ucIdx].m_bActive ) // entry is active
    {
      if( g_aTransportTable[ucIdx].m_hTPDUHandle == p_hHandle )
        return ucIdx;
    }
  }
  return ucIdx;
}
