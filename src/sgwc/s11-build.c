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

#include "s11-build.h"

ogs_pkbuf_t *sgwc_s11_build_downlink_data_notification(sgwc_bearer_t *bearer)
{
    ogs_gtp_message_t message;
    ogs_gtp_downlink_data_notification_t *noti = NULL;
    ogs_gtp_arp_t arp;
    sgwc_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    /* Build downlink notification message */
    noti = &message.downlink_data_notification;
    memset(&message, 0, sizeof(ogs_gtp_message_t));

    noti->eps_bearer_id.presence = 1;
    noti->eps_bearer_id.u8 = bearer->ebi;

    memset(&arp, 0, sizeof(arp));
    arp.pre_emption_vulnerability = sess->pdn.qos.arp.pre_emption_vulnerability;
    arp.priority_level = sess->pdn.qos.arp.priority_level;
    arp.pre_emption_capability = sess->pdn.qos.arp.pre_emption_capability;

    noti->allocation_retention_priority.presence = 1;
    noti->allocation_retention_priority.data = &arp;
    noti->allocation_retention_priority.len = sizeof(arp);

    message.h.type = OGS_GTP_DOWNLINK_DATA_NOTIFICATION_TYPE;
    return ogs_gtp_build_msg(&message);
}
