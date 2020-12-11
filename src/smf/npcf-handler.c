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

#include "npcf-handler.h"

#include "sbi-path.h"
#include "pfcp-path.h"
#include "nas-path.h"

bool smf_npcf_smpolicycontrol_handle_create(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    char *strerror = NULL;
    smf_ue_t *smf_ue = NULL;

    smf_bearer_t *qos_flow = NULL;
    ogs_pfcp_gtpu_resource_t *resource = NULL;
    ogs_pfcp_pdr_t *dl_pdr = NULL;
    ogs_pfcp_pdr_t *ul_pdr = NULL;
    ogs_pfcp_qer_t *qer = NULL;

    OpenAPI_sm_policy_decision_t *SmPolicyDecision = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    ogs_assert(sess);
    ogs_assert(stream);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_assert(recvmsg);

    if (!recvmsg->http.location) {
        strerror = ogs_msprintf("[%s:%d] No http.location",
                smf_ue->supi, sess->psi);
        goto cleanup;
    }

    SmPolicyDecision = recvmsg->SmPolicyDecision;
    if (!SmPolicyDecision) {
        strerror = ogs_msprintf("[%s:%d] No SmPolicyDecision",
                smf_ue->supi, sess->psi);
        goto cleanup;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        strerror = ogs_msprintf("[%s:%d] Cannot parse http.location [%s]",
                smf_ue->supi, sess->psi, recvmsg->http.location);
        goto cleanup;
    }

    if (!message.h.resource.component[1]) {
        strerror = ogs_msprintf("[%s:%d] No Assocication ID [%s]",
                smf_ue->supi, sess->psi, recvmsg->http.location);

        ogs_sbi_header_free(&header);
        goto cleanup;
    }

    if (sess->policy_association_id)
        ogs_free(sess->policy_association_id);
    sess->policy_association_id = ogs_strdup(message.h.resource.component[1]);

    ogs_sbi_header_free(&header);

    /*********************************************************************
     * Send PFCP Session Establiashment Request to the UPF
     *********************************************************************/

    /* Select UPF based on UE Location Information */
    smf_sess_select_upf(sess);

    /* Check if selected UPF is associated with SMF */
    ogs_assert(sess->pfcp_node);
    if (!OGS_FSM_CHECK(&sess->pfcp_node->sm, smf_pfcp_state_associated)) {
        ogs_error("[%s] No associated UPF", smf_ue->supi);
        return false;
    }

    /* Remove all previous QoS flow */
    smf_bearer_remove_all(sess);

    /* Setup Default QoS flow */
    qos_flow = smf_qos_flow_add(sess);
    ogs_assert(qos_flow);

    /* Setup QER */
    qer = qos_flow->qer;
    ogs_assert(qer);
    qer->mbr.uplink = sess->pdn.ambr.uplink;
    qer->mbr.downlink = sess->pdn.ambr.downlink;

    /* Setup PDR */
    dl_pdr = qos_flow->dl_pdr;
    ogs_assert(dl_pdr);
    ul_pdr = qos_flow->ul_pdr;
    ogs_assert(ul_pdr);

    /* Set UE IP Address to the Default DL PDR */
    smf_sess_set_ue_ip(sess);

    ogs_pfcp_paa_to_ue_ip_addr(&sess->pdn.paa,
            &dl_pdr->ue_ip_addr, &dl_pdr->ue_ip_addr_len);
    dl_pdr->ue_ip_addr.sd = OGS_PFCP_UE_IP_DST;

    ogs_info("UE SUPI:[%s] DNN:[%s] IPv4:[%s] IPv6:[%s]",
	    smf_ue->supi, sess->pdn.dnn,
        sess->ipv4 ? OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
        sess->ipv6 ? OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");

    /* Set UPF-N3 TEID & ADDR to the Default UL PDR */
    if (sess->pfcp_node->up_function_features.ftup) {
        ul_pdr->f_teid.ch = 1;
        ul_pdr->f_teid_len = 1;
    } else {
        resource = ogs_pfcp_gtpu_resource_find(
                &sess->pfcp_node->gtpu_resource_list,
                sess->pdn.dnn, OGS_PFCP_INTERFACE_ACCESS);
        if (resource) {
            ogs_pfcp_user_plane_ip_resource_info_to_sockaddr(&resource->info,
                &sess->upf_n3_addr, &sess->upf_n3_addr6);
            if (resource->info.teidri)
                sess->upf_n3_teid = OGS_PFCP_GTPU_INDEX_TO_TEID(
                        sess->index, resource->info.teidri,
                        resource->info.teid_range);
            else
                sess->upf_n3_teid = sess->index;
        } else {
            if (sess->pfcp_node->addr.ogs_sa_family == AF_INET)
                ogs_copyaddrinfo(&sess->upf_n3_addr, &sess->pfcp_node->addr);
            else if (sess->pfcp_node->addr.ogs_sa_family == AF_INET6)
                ogs_copyaddrinfo(&sess->upf_n3_addr6, &sess->pfcp_node->addr);
            else
                ogs_assert_if_reached();

            sess->upf_n3_teid = sess->index;
        }

        ogs_assert(sess->upf_n3_addr || sess->upf_n3_addr6);
        ogs_pfcp_sockaddr_to_f_teid(sess->upf_n3_addr, sess->upf_n3_addr6,
                &ul_pdr->f_teid, &ul_pdr->f_teid_len);
        ul_pdr->f_teid.teid = sess->upf_n3_teid;
    }

    /* Default PDRs is set to lowest precedence(highest precedence value) */
    dl_pdr->precedence = 0xffffffff;
    ul_pdr->precedence = 0xffffffff;

    smf_5gc_pfcp_send_session_establishment_request(sess, stream);

    return true;

cleanup:
    ogs_assert(strerror);

    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            recvmsg, strerror, NULL);
    ogs_free(strerror);

    return false;
}
