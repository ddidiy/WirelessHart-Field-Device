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

#ifndef C818_WRITE_CHANNEL_BLACKLIST_H_
#define C818_WRITE_CHANNEL_BLACKLIST_H_

#include "../../Model/NetworkLayerCommands.h"
#include "../../../util/Binarization.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @Binarize all requests: CMDID + Req_Length + Req_Data
 * @param p_pReq
 * @param p_pBuffer
 */
uint8_t Compose_C818_WriteChannelBlacklist_Req ( C818_WriteChannelBlacklist_Req* request,
                                    BinaryStream* toStream );

/**
 * @Debinarize
 */
uint8_t Parse_C818_WriteChannelBlacklist_Req ( C818_WriteChannelBlacklist_Req* request,
                                    BinaryStream* fromStream );

/**
 *
 */
uint8_t Compose_C818_WriteChannelBlacklist_Resp ( C818_WriteChannelBlacklist_Resp* response,	
                                    BinaryStream* toStream );

/**
 *
 */
uint8_t Parse_C818_WriteChannelBlacklist_Resp ( C818_WriteChannelBlacklist_Resp* response,	
                                    BinaryStream* fromStream );



#ifdef __cplusplus
}
#endif

#endif /* C818_WRITE_CHANNEL_BLACKLIST_H_ */
