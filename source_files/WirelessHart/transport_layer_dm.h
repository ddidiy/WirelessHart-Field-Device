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
// Name:         transport_layer_dm.h
// Author:       Nivis, LLC
// Date:         July 2008
// Description:  Transport Layer Management
// Changes:
// Revisions:
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NIVIS_TL_DM_H_
#define _NIVIS_TL_DM_H_


typedef enum
{
  TL_SUCCESS,
  TL_NO_SESSION,
  TL_PACKET_WRITTEN_TO_EXTRA_BUFFER,
  TL_NO_TRANSPORT_ENTRY,
  TL_PIPE_IS_FULL,
  TL_PAYLOAD_TOO_LONG,
  TL_INVALID_HANDLER,
  TL_INVALID_SEQUENCE_NUMBER,
  TL_MAX_RETRIES_EXCEEDED,
} TL_SC;



  ///////////////////////////////////////////////////////////////////////////////////
  // Name: FindTransportEntryByHandle
  // Author: NIVIS LLC, Dorin Pavel
  // Description: find an entry in TransportTable by its handler
  // Parameters: 
  //              p_hHandle - searched handler
  // Return: entry index
  ///////////////////////////////////////////////////////////////////////////////////
uint8 FindTransportEntryByHandle( HANDLE p_hHandle );

#endif // _NIVIS_TL_DM_H_ 

