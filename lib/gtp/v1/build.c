/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

ogs_pkbuf_t *ogs_gtp1_build_echo_request(uint8_t type)
{
    ogs_gtp1_message_t gtp1_message;

    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    gtp1_message.h.type = type;
    return ogs_gtp1_build_msg(&gtp1_message);
}

ogs_pkbuf_t *ogs_gtp1_build_echo_response(
        uint8_t type, uint8_t recovery)
{
    ogs_gtp1_message_t gtp1_message;
    ogs_gtp1_echo_response_t *rsp = NULL;

    rsp = &gtp1_message.echo_response;
    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    rsp->recovery.presence = 1;
    rsp->recovery.u8 = recovery;

    gtp1_message.h.type = type;
    return ogs_gtp1_build_msg(&gtp1_message);
}
