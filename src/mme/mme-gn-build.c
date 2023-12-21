/*
 * Copyright (C) 2023 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "mme-context.h"

#include "mme-gn-build.h"

/* 3GPP TS 29.060 7.5.14.1 RAN Information Relay */
ogs_pkbuf_t *mme_gn_build_ran_information_relay(
                uint8_t type, const uint8_t *buf, size_t len,
                const ogs_nas_rai_t *rai, uint16_t cell_id)
{
    ogs_gtp1_message_t gtp1_message;
    ogs_gtp1_ran_information_relay_t *req = NULL;
    uint8_t rt_addr[sizeof(*rai) + sizeof(cell_id)];

    ogs_debug("[Gn] build RAN Information Relay");


    req = &gtp1_message.ran_information_relay;
    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    /* 3GPP TS 29.060 7.7.43 RAN Transparent Container, Mandatory */
    req->ran_transparent_container.presence = 1;
    req->ran_transparent_container.data = (void*)buf;
    req->ran_transparent_container.len = len;

    /* 3GPP TS 29.060 7.7.57 RAN RIM Routing Address, Optional */
    req->rim_routing_address.presence = !!rai;
    if (req->rim_routing_address.presence) {
        memcpy(&rt_addr[0], rai, sizeof(*rai));
        memcpy(&rt_addr[sizeof(*rai)], &cell_id, sizeof(cell_id));
        req->rim_routing_address.data = &rt_addr[0];
        req->rim_routing_address.len = sizeof(rt_addr);
    }

    /* 3GPP TS 29.060 7.7.77 RIM Routing Address Discriminator, Optional */
    /* 3GPP TS 48.018 11.3.70 RIM Routing Information IE: */
    uint8_t rim_routing_address_disc = 0; /* "A Cell Identifier is used to identify a GERAN cell."" */
    req->rim_routing_address_discriminator.presence = req->rim_routing_address.presence;
    req->rim_routing_address_discriminator.data = &rim_routing_address_disc;
    req->rim_routing_address_discriminator.len = 1;

    gtp1_message.h.type = type;
    return ogs_gtp1_build_msg(&gtp1_message);
}