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

ogs_pkbuf_t *sgwc_s11_build_create_session_response(
        uint8_t type, sgwc_sess_t *sess)
{
    int rv, i;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *ul_tunnel = NULL;
    sgwc_ue_t *sgwc_ue = NULL;

    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_create_session_response_t *rsp = NULL;

    ogs_gtp2_cause_t cause;

    ogs_gtp2_f_teid_t sgw_s11_teid;
    int len;

    ogs_gtp2_cause_t bearer_cause[OGS_BEARER_PER_UE];
    ogs_gtp2_f_teid_t sgw_s1u_teid[OGS_BEARER_PER_UE];
    int sgw_s1u_len[OGS_BEARER_PER_UE];
    ogs_gtp2_f_teid_t pgw_s5u_teid[OGS_BEARER_PER_UE];
    int pgw_s5u_len[OGS_BEARER_PER_UE];

    ogs_debug("[SGWC] Create Session Response");

    ogs_assert(sess);
    sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
    ogs_assert(sgwc_ue);

    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    rsp = &gtp_message.create_session_response;
    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    /* Set Cause */
    memset(&cause, 0, sizeof(cause));
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    /* Send Control Plane(UL) : SGW-S11 */
    memset(&sgw_s11_teid, 0, sizeof(ogs_gtp2_f_teid_t));
    sgw_s11_teid.interface_type = OGS_GTP2_F_TEID_S11_S4_SGW_GTP_C;
    sgw_s11_teid.teid = htobe32(sgwc_ue->sgw_s11_teid);
    rv = ogs_gtp2_sockaddr_to_f_teid(
            ogs_gtp_self()->gtpc_addr, ogs_gtp_self()->gtpc_addr6,
            &sgw_s11_teid, &len);
    ogs_assert(rv == OGS_OK);
    rsp->sender_f_teid_for_control_plane.presence = 1;
    rsp->sender_f_teid_for_control_plane.data = &sgw_s11_teid;
    rsp->sender_f_teid_for_control_plane.len = len;

    i = 0;
    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_assert(i < OGS_BEARER_PER_UE);

        /* Bearer EBI */
        rsp->bearer_contexts_created[i].presence = 1;
        rsp->bearer_contexts_created[i].eps_bearer_id.presence = 1;
        rsp->bearer_contexts_created[i].eps_bearer_id.u8 = bearer->ebi;

        /* Bearer Cause */
        memset(&bearer_cause[i], 0, sizeof(bearer_cause[i]));
        rsp->bearer_contexts_created[i].cause.presence = 1;
        rsp->bearer_contexts_created[i].cause.len = sizeof(bearer_cause[i]);
        rsp->bearer_contexts_created[i].cause.data = &bearer_cause[i];
        bearer_cause[i].value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

        /* Data Plane(UL) */
        ul_tunnel = sgwc_ul_tunnel_in_bearer(bearer);
        ogs_assert(ul_tunnel);

        ogs_debug("    SGW_S1U_TEID[0x%x] PGW_S5U_TEID[0x%x]",
                ul_tunnel->local_teid, ul_tunnel->remote_teid);

        /* Data Plane(UL) : SGW-S1U */
        memset(&sgw_s1u_teid[i], 0, sizeof(ogs_gtp2_f_teid_t));
        sgw_s1u_teid[i].interface_type = ul_tunnel->interface_type;
        sgw_s1u_teid[i].teid = htobe32(ul_tunnel->local_teid);
        ogs_assert(ul_tunnel->local_addr || ul_tunnel->local_addr6);
        rv = ogs_gtp2_sockaddr_to_f_teid(
            ul_tunnel->local_addr, ul_tunnel->local_addr6,
            &sgw_s1u_teid[i], &sgw_s1u_len[i]);
        ogs_assert(rv == OGS_OK);
        rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.presence = 1;
        rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.data =
            &sgw_s1u_teid[i];
        rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.len =
            sgw_s1u_len[i];

        /* Data Plane(UL) : PGW-S5U */
        memset(&pgw_s5u_teid[i], 0, sizeof(ogs_gtp2_f_teid_t));
        pgw_s5u_teid[i].interface_type = OGS_GTP2_F_TEID_S5_S8_PGW_GTP_U;
        pgw_s5u_teid[i].teid = htobe32(ul_tunnel->remote_teid);
        rv = ogs_gtp2_ip_to_f_teid(&ul_tunnel->remote_ip,
                &pgw_s5u_teid[i], &pgw_s5u_len[i]);
        ogs_assert(rv == OGS_OK);
        rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.presence = 1;
        rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.data =
            &pgw_s5u_teid[i];
        rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.len =
            pgw_s5u_len[i];

        i++;
    }

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *sgwc_s11_build_downlink_data_notification(
        uint8_t cause_value, sgwc_bearer_t *bearer)
{
    ogs_gtp2_message_t message;
    ogs_gtp2_downlink_data_notification_t *noti = NULL;
    ogs_gtp2_cause_t cause;
    ogs_gtp2_arp_t arp;
    sgwc_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = sgwc_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    /* Build downlink notification message */
    noti = &message.downlink_data_notification;
    memset(&message, 0, sizeof(ogs_gtp2_message_t));

    /*
     * TS29.274 8.4 Cause Value
     *
     * 0 : Reserved. Shall not be sent and
     *     if received the Cause shall be treated as an invalid IE
     */
    if (cause_value != OGS_GTP2_CAUSE_UNDEFINED_VALUE) {
        memset(&cause, 0, sizeof(cause));
        cause.value = cause_value;
        noti->cause.presence = 1;
        noti->cause.len = sizeof(cause);
        noti->cause.data = &cause;
    }

    noti->eps_bearer_id.presence = 1;
    noti->eps_bearer_id.u8 = bearer->ebi;

    memset(&arp, 0, sizeof(arp));
    arp.pre_emption_vulnerability =
        sess->session.qos.arp.pre_emption_vulnerability;
    arp.priority_level = sess->session.qos.arp.priority_level;
    arp.pre_emption_capability = sess->session.qos.arp.pre_emption_capability;

    noti->allocation_retention_priority.presence = 1;
    noti->allocation_retention_priority.data = &arp;
    noti->allocation_retention_priority.len = sizeof(arp);

    message.h.type = OGS_GTP2_DOWNLINK_DATA_NOTIFICATION_TYPE;
    return ogs_gtp2_build_msg(&message);
}
