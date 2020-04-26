/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-pfcp.h"

ogs_pkbuf_t *ogs_pfcp_n4_build_heartbeat_request(uint8_t type)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_heartbeat_request_t *req = NULL;

    ogs_debug("[PFCP] Heartbeat Request");

    req = &pfcp_message.pfcp_heartbeat_request;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    req->recovery_time_stamp.presence = 1;
    req->recovery_time_stamp.u32 = ogs_pfcp_self()->pfcp_started;

    pfcp_message.h.type = type;
    return ogs_pfcp_build_msg(&pfcp_message);
}

ogs_pkbuf_t *ogs_pfcp_n4_build_heartbeat_response(uint8_t type)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_heartbeat_response_t *rsp = NULL;

    ogs_debug("[PFCP] Heartbeat Response");

    rsp = &pfcp_message.pfcp_heartbeat_response;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    rsp->recovery_time_stamp.presence = 1;
    rsp->recovery_time_stamp.u32 = ogs_pfcp_self()->pfcp_started;

    pfcp_message.h.type = type;
    return ogs_pfcp_build_msg(&pfcp_message);
}
