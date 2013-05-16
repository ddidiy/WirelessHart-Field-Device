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

#include "C801_DeleteService.h"

uint8_t Compose_C801_DeleteService_Req ( C801_DeleteService_Req* request,
                                        BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, request->m_ucServiceId);
  STREAM_WRITE_UINT8(toStream, request->m_ucReason);

  return RCS_N00_Success;
}

uint8_t Parse_C801_DeleteService_Req ( C801_DeleteService_Req* request,
                                      BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &request->m_ucServiceId);
  STREAM_READ_UINT8(fromStream, &request->m_ucReason);

// [doinel.alban] moved to command execution function
//  if( request->m_ucReason != ServiceDeletionReasonCodes_RequestedByPeer &&
//      request->m_ucReason != ServiceDeletionReasonCodes_ServiceCantBeEstablished &&
//      request->m_ucReason != ServiceDeletionReasonCodes_NetworkFailure )
//    return RCM_E66_InvalidReasonCode;

  return RCS_N00_Success;
}

uint8_t Compose_C801_DeleteService_Resp ( C801_DeleteService_Resp* response,
                                         BinaryStream* toStream )
{
  STREAM_WRITE_UINT8(toStream, response->m_ucServiceId);
  STREAM_WRITE_UINT8(toStream, response->m_ucReason);
  STREAM_WRITE_UINT8(toStream, response->m_ucNoOfServiceEntriesRemaining);

  return RCS_N00_Success;
}

uint8_t Parse_C801_DeleteService_Resp ( C801_DeleteService_Resp* response,
                                       BinaryStream* fromStream )
{
  STREAM_READ_UINT8(fromStream, &response->m_ucServiceId);
  STREAM_READ_UINT8(fromStream, &response->m_ucReason);
  STREAM_READ_UINT8(fromStream, &response->m_ucNoOfServiceEntriesRemaining);

// [doinel.alban] this check should be moved outside of parsing function
//  if( response->m_ucReason != ServiceDeletionReasonCodes_RequestedByPeer &&
//      response->m_ucReason != ServiceDeletionReasonCodes_ServiceCantBeEstablished &&
//      response->m_ucReason != ServiceDeletionReasonCodes_NetworkFailure )
//    return RCM_E66_InvalidReasonCode;

  return RCS_N00_Success;
}
