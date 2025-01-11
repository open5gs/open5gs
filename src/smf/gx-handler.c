/* Gx Interface, 3GPP TS 29.212 section 4
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

#include "context.h"
#include "gtp-path.h"
#include "pfcp-path.h"
#include "fd-path.h"
#include "gx-handler.h"
#include "binding.h"

/* Returns ER_DIAMETER_SUCCESS on success, Diameter error code on failue. */
uint32_t smf_gx_handle_cca_initial_request(
        smf_sess_t *sess, ogs_diam_gx_message_t *gx_message,
        ogs_gtp_xact_t *gtp_xact)
{
    int i;

    smf_bearer_t *bearer = NULL;

    ogs_pfcp_pdr_t *dl_pdr = NULL;
    ogs_pfcp_pdr_t *ul_pdr = NULL;
    ogs_pfcp_far_t *dl_far = NULL;
    ogs_pfcp_qer_t *qer = NULL;

    ogs_pfcp_pdr_t *cp2up_pdr = NULL;
    ogs_pfcp_pdr_t *up2cp_pdr = NULL;
    ogs_pfcp_far_t *up2cp_far = NULL;

    ogs_assert(sess);
    ogs_assert(gx_message);
    ogs_assert(gtp_xact);

    ogs_debug("[PGW] Create Session Response");
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (gx_message->result_code != ER_DIAMETER_SUCCESS)
        return gx_message->err ? *gx_message->err :
                                 ER_DIAMETER_AUTHENTICATION_REJECTED;


    sess->policy.num_of_pcc_rule = gx_message->session_data.num_of_pcc_rule;
    for (i = 0; i < gx_message->session_data.num_of_pcc_rule; i++)
        OGS_STORE_PCC_RULE(&sess->policy.pcc_rule[i],
                &gx_message->session_data.pcc_rule[i]);

    /* APN-AMBR
     * if PCRF changes APN-AMBR, this should be included. */
    sess->gtp.create_session_response_apn_ambr = false;
    if ((gx_message->session_data.session.ambr.uplink &&
            (sess->session.ambr.uplink / 1000) !=
                (gx_message->session_data.session.ambr.uplink / 1000)) ||
        (gx_message->session_data.session.ambr.downlink &&
            (sess->session.ambr.downlink / 1000) !=
                (gx_message->session_data.session.ambr.downlink / 1000))) {

        sess->session.ambr.downlink =
            gx_message->session_data.session.ambr.downlink;
        sess->session.ambr.uplink =
            gx_message->session_data.session.ambr.uplink;

        sess->gtp.create_session_response_apn_ambr = true;
    }

    /* Bearer QoS
     * if PCRF changes Bearer QoS, this should be included. */
    sess->gtp.create_session_response_bearer_qos = false;
    if ((gx_message->session_data.session.qos.index &&
        sess->session.qos.index !=
            gx_message->session_data.session.qos.index) ||
        (gx_message->session_data.session.qos.arp.priority_level &&
        sess->session.qos.arp.priority_level !=
            gx_message->session_data.session.qos.arp.priority_level) ||
        sess->session.qos.arp.pre_emption_capability !=
            gx_message->session_data.session.qos.arp.pre_emption_capability ||
        sess->session.qos.arp.pre_emption_vulnerability !=
            gx_message->session_data.
                session.qos.arp.pre_emption_vulnerability) {

        sess->session.qos.index = gx_message->session_data.session.qos.index;
        sess->session.qos.arp.priority_level =
            gx_message->session_data.session.qos.arp.priority_level;
        sess->session.qos.arp.pre_emption_capability =
            gx_message->session_data.session.qos.arp.pre_emption_capability;
        sess->session.qos.arp.pre_emption_vulnerability =
            gx_message->session_data.session.qos.arp.pre_emption_vulnerability;

        sess->gtp.create_session_response_bearer_qos = true;
    }

    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    /* Setup CP/UP Data Forwarding PDR/FAR */
    smf_sess_create_cp_up_data_forwarding(sess);

    /* Setup QER */
    if (sess->session.ambr.downlink || sess->session.ambr.uplink) {
        /* Only 1 QER is used per bearer */
        qer = bearer->qer;
        if (!qer) {
            qer = ogs_pfcp_qer_add(&sess->pfcp);
            ogs_assert(qer);
            bearer->qer = qer;
        }

        qer->mbr.uplink = sess->session.ambr.uplink;
        qer->mbr.downlink = sess->session.ambr.downlink;
    }

    /* Setup FAR */
    dl_far = bearer->dl_far;
    ogs_assert(dl_far);
    up2cp_far = sess->up2cp_far;
    ogs_assert(up2cp_far);

    dl_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

    /* Set Outer Header Creation to the Default DL FAR */
    ogs_assert(OGS_OK ==
        ogs_pfcp_ip_to_outer_header_creation(
            &bearer->sgw_s5u_ip,
            &dl_far->outer_header_creation,
            &dl_far->outer_header_creation_len));
    dl_far->outer_header_creation.teid = bearer->sgw_s5u_teid;

    /* Setup PDR */
    dl_pdr = bearer->dl_pdr;
    ogs_assert(dl_pdr);
    ul_pdr = bearer->ul_pdr;
    ogs_assert(ul_pdr);
    cp2up_pdr = sess->cp2up_pdr;
    ogs_assert(cp2up_pdr);
    up2cp_pdr = sess->up2cp_pdr;
    ogs_assert(up2cp_pdr);

    /* Set UE IP Address to the Default DL PDR */
    ogs_assert(OGS_OK ==
        ogs_pfcp_paa_to_ue_ip_addr(&sess->paa,
            &dl_pdr->ue_ip_addr, &dl_pdr->ue_ip_addr_len));
    dl_pdr->ue_ip_addr.sd = OGS_PFCP_UE_IP_DST;

    ogs_assert(OGS_OK ==
        ogs_pfcp_paa_to_ue_ip_addr(&sess->paa,
            &ul_pdr->ue_ip_addr, &ul_pdr->ue_ip_addr_len));

    /* Set UE-to-CP Flow-Description and Outer-Header-Creation */
    up2cp_pdr->flow[up2cp_pdr->num_of_flow].fd = 1;
    up2cp_pdr->flow[up2cp_pdr->num_of_flow].description =
        (char *)"permit out 58 from ff02::2/128 to assigned";
    up2cp_pdr->num_of_flow++;

    ogs_assert(OGS_OK ==
        ogs_pfcp_ip_to_outer_header_creation(
            &ogs_gtp_self()->gtpu_ip,
            &up2cp_far->outer_header_creation,
            &up2cp_far->outer_header_creation_len));
    up2cp_far->outer_header_creation.teid = sess->index;

    /* Set F-TEID */
    ogs_assert(sess->pfcp_node);
    if (sess->pfcp_node->up_function_features.ftup) {
        ul_pdr->f_teid.ipv4 = 1;
        ul_pdr->f_teid.ipv6 = 1;
        ul_pdr->f_teid.ch = 1;
        ul_pdr->f_teid.chid = 1;
        ul_pdr->f_teid.choose_id = OGS_PFCP_DEFAULT_CHOOSE_ID;
        ul_pdr->f_teid_len = 2;

        cp2up_pdr->f_teid.ipv4 = 1;
        cp2up_pdr->f_teid.ipv6 = 1;
        cp2up_pdr->f_teid.ch = 1;
        cp2up_pdr->f_teid_len = 1;

        up2cp_pdr->f_teid.ipv4 = 1;
        up2cp_pdr->f_teid.ipv6 = 1;
        up2cp_pdr->f_teid.ch = 1;
        up2cp_pdr->f_teid.chid = 1;
        up2cp_pdr->f_teid.choose_id = OGS_PFCP_DEFAULT_CHOOSE_ID;
        up2cp_pdr->f_teid_len = 2;
    } else {
        ogs_gtpu_resource_t *resource = NULL;
        resource = ogs_pfcp_find_gtpu_resource(
                &sess->pfcp_node->gtpu_resource_list,
                sess->session.name, ul_pdr->src_if);
        if (resource) {
            ogs_user_plane_ip_resource_info_to_sockaddr(&resource->info,
                &bearer->pgw_s5u_addr, &bearer->pgw_s5u_addr6);
            if (resource->info.teidri)
                bearer->pgw_s5u_teid = OGS_PFCP_GTPU_INDEX_TO_TEID(
                        ul_pdr->teid, resource->info.teidri,
                        resource->info.teid_range);
            else
                bearer->pgw_s5u_teid = ul_pdr->teid;
        } else {
            ogs_assert(sess->pfcp_node->addr_list);
            if (sess->pfcp_node->addr_list->ogs_sa_family == AF_INET)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &bearer->pgw_s5u_addr, sess->pfcp_node->addr_list));
            else if (sess->pfcp_node->addr_list->ogs_sa_family == AF_INET6)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &bearer->pgw_s5u_addr6, sess->pfcp_node->addr_list));
            else
                ogs_assert_if_reached();

            bearer->pgw_s5u_teid = ul_pdr->teid;
        }

        ogs_assert(OGS_OK ==
            ogs_pfcp_sockaddr_to_f_teid(
                bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6,
                &ul_pdr->f_teid, &ul_pdr->f_teid_len));
        ul_pdr->f_teid.teid = bearer->pgw_s5u_teid;

        ogs_assert(OGS_OK ==
            ogs_pfcp_sockaddr_to_f_teid(
                bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6,
                &cp2up_pdr->f_teid, &cp2up_pdr->f_teid_len));
        cp2up_pdr->f_teid.teid = cp2up_pdr->teid;

        ogs_assert(OGS_OK ==
            ogs_pfcp_sockaddr_to_f_teid(
                bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6,
                &up2cp_pdr->f_teid, &up2cp_pdr->f_teid_len));
        up2cp_pdr->f_teid.teid = bearer->pgw_s5u_teid;
    }

    dl_pdr->precedence = OGS_PFCP_DEFAULT_PDR_PRECEDENCE;
    ul_pdr->precedence = OGS_PFCP_DEFAULT_PDR_PRECEDENCE;

    cp2up_pdr->precedence = OGS_PFCP_CP2UP_PDR_PRECEDENCE;
    up2cp_pdr->precedence = OGS_PFCP_UP2CP_PDR_PRECEDENCE;

    if (qer) {
        ogs_pfcp_pdr_associate_qer(dl_pdr, qer);
        ogs_pfcp_pdr_associate_qer(ul_pdr, qer);
    }
    return ER_DIAMETER_SUCCESS;
}

uint32_t smf_gx_handle_cca_termination_request(
        smf_sess_t *sess, ogs_diam_gx_message_t *gx_message,
        ogs_gtp_xact_t *gtp_xact)
{
    ogs_assert(sess);
    ogs_assert(gx_message);

    ogs_debug("[SMF] Delete Session Response");
    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    return ER_DIAMETER_SUCCESS;
}

void smf_gx_handle_re_auth_request(
        smf_sess_t *sess, ogs_diam_gx_message_t *gx_message)
{
    int i;

    sess->policy.num_of_pcc_rule = gx_message->session_data.num_of_pcc_rule;
    for (i = 0; i < gx_message->session_data.num_of_pcc_rule; i++)
        OGS_STORE_PCC_RULE(&sess->policy.pcc_rule[i],
                &gx_message->session_data.pcc_rule[i]);

    smf_bearer_binding(sess);
}
