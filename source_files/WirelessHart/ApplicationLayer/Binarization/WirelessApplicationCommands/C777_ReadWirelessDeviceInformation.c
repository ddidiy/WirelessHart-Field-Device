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

#include "C777_ReadWirelessDeviceInformation.h"

uint8_t Compose_C777_ReadWirelessDeviceInformation_Req ( C777_ReadWirelessDeviceInformation_Req* request,
                                    BinaryStream* toStream )
{
  return RCS_N00_Success; // success
}

uint8_t Parse_C777_ReadWirelessDeviceInformation_Req ( C777_ReadWirelessDeviceInformation_Req* request,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > 0)
    return RCS_E01_Undefined1; // too many bytes
#endif

  return RCS_N00_Success; // success
}

uint8_t Compose_C777_ReadWirelessDeviceInformation_Resp ( C777_ReadWirelessDeviceInformation_Resp* response,
                                    BinaryStream* toStream )
{
  response->m_ucPowerSource &= 0x0F;      // bits 7-4 are set to 0

  STREAM_WRITE_UINT8(toStream, response->m_ucPowerSource);
  STREAM_WRITE_FLOAT(toStream, response->m_fPeakPacketsPerSecond);
  STREAM_WRITE_TIME(toStream, response->m_tDurationAtPeakPacketLoadBeforePowerDrained);
  STREAM_WRITE_TIME(toStream, response->m_tTimeToRecoverFromPowerDrain);
  STREAM_WRITE_INT8(toStream, response->m_cRSLThresholdGoodConnection);
  STREAM_WRITE_TIME(toStream, response->m_tMinRequiredKeepAliveTime);
  STREAM_WRITE_UINT16(toStream, response->m_unMaxNoOfNeighbors);
  STREAM_WRITE_UINT16(toStream, response->m_unMaxNoOfPacketBuffers);

  return RCS_N00_Success; // success
}

uint8_t Parse_C777_ReadWirelessDeviceInformation_Resp ( C777_ReadWirelessDeviceInformation_Resp* response,
                                    BinaryStream* fromStream )
{
#if defined(_DEBUG)
  if (STREAM_GetRemainingSize(fromStream) > C777_RespSize)
    return RCS_E01_Undefined1; // too many bytes
#endif

  response->m_ucPowerSource &= 0x0F;      // bits 7-4 are ignored

  STREAM_READ_UINT8(fromStream, &response->m_ucPowerSource);
  STREAM_READ_FLOAT(fromStream, &response->m_fPeakPacketsPerSecond);
  STREAM_READ_TIME(fromStream, &response->m_tDurationAtPeakPacketLoadBeforePowerDrained);
  STREAM_READ_TIME(fromStream, &response->m_tTimeToRecoverFromPowerDrain);
  STREAM_READ_INT8(fromStream, &response->m_cRSLThresholdGoodConnection);
  STREAM_READ_TIME(fromStream, &response->m_tMinRequiredKeepAliveTime);
  STREAM_READ_UINT16(fromStream, &response->m_unMaxNoOfNeighbors);
  STREAM_READ_UINT16(fromStream, &response->m_unMaxNoOfPacketBuffers);

  return RCS_N00_Success; // success
}
