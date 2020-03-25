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

#include "ogs-gtp.h"

ogs_pkbuf_t *ogs_gtp_build_echo_request(
        uint8_t type, uint8_t recovery, uint8_t features)
{
    ogs_gtp_message_t gtp_message;
    ogs_gtp_echo_request_t *req = NULL;

    req = &gtp_message.echo_request;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));

    req->recovery.presence = 1;
    req->recovery.u8 = recovery;

    req->sending_node_features.presence = 1;
    req->sending_node_features.u8 = features;

    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}

ogs_pkbuf_t *ogs_gtp_build_echo_response(
        uint8_t type, uint8_t recovery, uint8_t features)
{
    ogs_gtp_message_t gtp_message;
    ogs_gtp_echo_response_t *rsp = NULL;

    rsp = &gtp_message.echo_response;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));

    rsp->recovery.presence = 1;
    rsp->recovery.u8 = recovery;

    rsp->sending_node_features.presence = 1;
    rsp->sending_node_features.u8 = features;

    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}
