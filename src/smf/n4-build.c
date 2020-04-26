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

#include "context.h"
#include "n4-build.h"

ogs_pkbuf_t *smf_n4_build_association_setup_request(uint8_t type)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_association_setup_request_t *req = NULL;

    ogs_pfcp_node_id_t node_id;
    int node_id_len = 0;

    ogs_debug("[SMF] Association Setup Request");

    req = &pfcp_message.pfcp_association_setup_request;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    ogs_pfcp_sockaddr_to_node_id(
            ogs_pfcp_self()->pfcp_addr, ogs_pfcp_self()->pfcp_addr6,
            ogs_config()->parameter.prefer_ipv4,
            &node_id, &node_id_len);
    req->node_id.presence = 1;
    req->node_id.data = &node_id;
    req->node_id.len = node_id_len;
    
    req->recovery_time_stamp.presence = 1;
    req->recovery_time_stamp.u32 = ogs_pfcp_self()->pfcp_started;

    req->cp_function_features.presence = 1;
    req->cp_function_features.u8 = smf_self()->function_features;

    pfcp_message.h.type = type;
    return ogs_pfcp_build_msg(&pfcp_message);
}

ogs_pkbuf_t *smf_n4_build_association_setup_response(uint8_t type,
        uint8_t cause)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_association_setup_response_t *rsp = NULL;

    ogs_pfcp_node_id_t node_id;
    int node_id_len = 0;

    ogs_debug("[SMF] Association Setup Response");

    rsp = &pfcp_message.pfcp_association_setup_response;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    ogs_pfcp_sockaddr_to_node_id(
            ogs_pfcp_self()->pfcp_addr, ogs_pfcp_self()->pfcp_addr6,
            ogs_config()->parameter.prefer_ipv4,
            &node_id, &node_id_len);
    rsp->node_id.presence = 1;
    rsp->node_id.data = &node_id;
    rsp->node_id.len = node_id_len;

    rsp->cause.presence = 1;
    rsp->cause.u8 = cause;
    
    rsp->recovery_time_stamp.presence = 1;
    rsp->recovery_time_stamp.u32 = ogs_pfcp_self()->pfcp_started;

    rsp->cp_function_features.presence = 1;
    rsp->cp_function_features.u8 = smf_self()->function_features;

    pfcp_message.h.type = type;
    return ogs_pfcp_build_msg(&pfcp_message);
}

static struct {
    ogs_pfcp_ue_ip_addr_t addr;
    ogs_pfcp_outer_header_removal_t outer_header_removal;
    ogs_pfcp_f_teid_t f_teid;
    char apn[OGS_MAX_APN_LEN];
    char *sdf_filter[OGS_MAX_NUM_OF_RULE];
} create_pdr_buf[OGS_MAX_NUM_OF_PDR];

static void create_pdr_buf_init(void)
{
    memset(create_pdr_buf, 0, sizeof(create_pdr_buf));
}
static void create_pdr_buf_clear(void)
{
    int i, j;
    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        for (j = 0; j < OGS_MAX_NUM_OF_RULE; j++) {
            if (create_pdr_buf[i].sdf_filter[j])
                ogs_free(create_pdr_buf[i].sdf_filter[j]);
        }
    }
}

static void build_create_pdr(
    ogs_pfcp_tlv_create_pdr_t *message, int i, ogs_pfcp_pdr_t *pdr)
{
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_sess_t *pfcp_sess = NULL;
    smf_sess_t *sess = NULL;
    smf_bearer_t *bearer = NULL;
    int j = 0;
    int len = 0;

    ogs_assert(message);

    ogs_assert(pdr);
    pfcp_sess = pdr->sess;
    ogs_assert(pfcp_sess);
    bearer = SMF_BEARER(pfcp_sess);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    far = pdr->far;
    ogs_assert(far);

    message->presence = 1;
    message->pdr_id.presence = 1;
    message->pdr_id.u16 = pdr->id;

    message->precedence.presence = 1;
    message->precedence.u32 = pdr->precedence;

    message->pdi.presence = 1;
    message->pdi.source_interface.presence = 1;
    message->pdi.source_interface.u8 = pdr->src_if;

    message->pdi.network_instance.presence = 1;
    message->pdi.network_instance.len = ogs_fqdn_build(
        create_pdr_buf[i].apn, sess->pdn.apn, strlen(sess->pdn.apn));
    message->pdi.network_instance.data = create_pdr_buf[i].apn;

    for (j = 0; j < pdr->num_of_flow; j++) {
        ogs_pfcp_sdf_filter_t pfcp_sdf_filter[OGS_MAX_NUM_OF_RULE];
        pfcp_sdf_filter[j].fd = 1;
        pfcp_sdf_filter[j].flow_description_len =
                strlen(pdr->flow_description[j]);
        pfcp_sdf_filter[j].flow_description = pdr->flow_description[j];
        len = sizeof(ogs_pfcp_sdf_filter_t) +
                pfcp_sdf_filter[j].flow_description_len;

        message->pdi.sdf_filter[j].presence = 1;
        create_pdr_buf[i].sdf_filter[j] = ogs_calloc(1, len);
        ogs_pfcp_build_sdf_filter(&message->pdi.sdf_filter[j],
                &pfcp_sdf_filter[j], create_pdr_buf[i].sdf_filter[j], len);
    }

    if (pdr->src_if == OGS_PFCP_INTERFACE_CORE &&
        far->dst_if == OGS_PFCP_INTERFACE_ACCESS) { /* Dowklink */
        if (smf_bearer_is_default(bearer)) { /* Default Bearer */
            ogs_pfcp_paa_to_ue_ip_addr(&sess->pdn.paa,
                    &create_pdr_buf[i].addr, &len);
            create_pdr_buf[i].addr.sd = OGS_PFCP_UE_IP_DST;

            message->pdi.ue_ip_address.presence = 1;
            message->pdi.ue_ip_address.data = &create_pdr_buf[i].addr;
            message->pdi.ue_ip_address.len = len;
        }

    } else if (pdr->src_if == OGS_PFCP_INTERFACE_ACCESS &&
                far->dst_if == OGS_PFCP_INTERFACE_CORE) { /* Uplink */
        ogs_pfcp_sockaddr_to_f_teid(
                bearer->upf_addr, bearer->upf_addr6,
                &create_pdr_buf[i].f_teid, &len);
        create_pdr_buf[i].f_teid.teid = htobe32(bearer->upf_n3_teid);

        message->pdi.local_f_teid.presence = 1;
        message->pdi.local_f_teid.data = &create_pdr_buf[i].f_teid;
        message->pdi.local_f_teid.len = len;

        if (sess->pdn.paa.pdn_type == OGS_GTP_PDN_TYPE_IPV4) {
            create_pdr_buf[i].outer_header_removal.description =
                OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;
        } else if (sess->pdn.paa.pdn_type == OGS_GTP_PDN_TYPE_IPV6) {
            create_pdr_buf[i].outer_header_removal.description =
                OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6;
        } else if (sess->pdn.paa.pdn_type == OGS_GTP_PDN_TYPE_IPV4V6) {
            create_pdr_buf[i].outer_header_removal.description =
                OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP;
        } else
            ogs_assert_if_reached();

        message->outer_header_removal.presence = 1;
        message->outer_header_removal.data =
            &create_pdr_buf[i].outer_header_removal.description;
        message->outer_header_removal.len = 1;
    }

    if (pdr->far) {
        message->far_id.presence = 1;
        message->far_id.u32 = pdr->far->id;
    }

    if (pdr->urr) {
        message->urr_id.presence = 1;
        message->urr_id.u32 = pdr->urr->id;
    }

    if (pdr->qer) {
        message->qer_id.presence = 1;
        message->qer_id.u32 = pdr->qer->id;
    }
}

static struct {
    ogs_pfcp_outer_header_creation_t outer_header_creation;
} create_far_buf[OGS_MAX_NUM_OF_FAR];

static void build_create_far(
    ogs_pfcp_tlv_create_far_t *message, int i, ogs_pfcp_far_t *far)
{
    ogs_pfcp_sess_t *pfcp_sess = NULL;
    smf_bearer_t *bearer = NULL;
    int len;

    ogs_assert(message);
    ogs_assert(far);
    pfcp_sess = far->sess;
    ogs_assert(pfcp_sess);
    bearer = SMF_BEARER(pfcp_sess);
    ogs_assert(bearer);

    message->presence = 1;
    message->far_id.presence = 1;
    message->far_id.u32 = far->id;

    message->apply_action.presence = 1;
    message->apply_action.u8 = far->apply_action;

    message->forwarding_parameters.presence = 1;
    message->forwarding_parameters.destination_interface.presence = 1;
    message->forwarding_parameters.destination_interface.u8 = far->dst_if;

    if (far->dst_if == OGS_PFCP_INTERFACE_ACCESS) { /* Downlink */
        ogs_pfcp_ip_to_outer_header_creation(&bearer->gnb_ip,
                &create_far_buf[i].outer_header_creation, &len);
        create_far_buf[i].outer_header_creation.teid =
                htobe32(bearer->gnb_n3_teid);

        message->forwarding_parameters.outer_header_creation.presence = 1;
        message->forwarding_parameters.outer_header_creation.data =
                &create_far_buf[i].outer_header_creation;
        message->forwarding_parameters.outer_header_creation.len = len;
    }
}

static void build_create_urr(
    ogs_pfcp_tlv_create_urr_t *message, int i, ogs_pfcp_urr_t *urr)
{
    ogs_assert(message);
    ogs_assert(urr);

    message->presence = 1;
    message->urr_id.presence = 1;
    message->urr_id.u32 = urr->id;
}

static struct {
    char mbr[OGS_PFCP_BITRATE_LEN];
    char gbr[OGS_PFCP_BITRATE_LEN];
} create_qer_buf[OGS_MAX_NUM_OF_QER], update_qer_buf[OGS_MAX_NUM_OF_QER];

static void build_create_qer(
    ogs_pfcp_tlv_create_qer_t *message, int i, ogs_pfcp_qer_t *qer)
{
    ogs_assert(message);
    ogs_assert(qer);

    message->presence = 1;
    message->qer_id.presence = 1;
    message->qer_id.u32 = qer->id;

    message->gate_status.presence = 1;
    message->gate_status.u8 = qer->gate_status.value;

    if (qer->mbr.uplink || qer->mbr.downlink) {
        message->maximum_bitrate.presence = 1;
        ogs_pfcp_build_bitrate(
                &message->maximum_bitrate,
                &qer->mbr, create_qer_buf[i].mbr, OGS_PFCP_BITRATE_LEN);
    }
    if (qer->gbr.uplink || qer->gbr.downlink) {
        message->guaranteed_bitrate.presence = 1;
        ogs_pfcp_build_bitrate(
                &message->guaranteed_bitrate,
                &qer->gbr, create_qer_buf[i].gbr, OGS_PFCP_BITRATE_LEN);
    }
}

static void build_update_qer(
    ogs_pfcp_tlv_update_qer_t *message, int i, ogs_pfcp_qer_t *qer)
{
    ogs_assert(message);
    ogs_assert(qer);

    message->presence = 1;
    message->qer_id.presence = 1;
    message->qer_id.u32 = qer->id;

    if (qer->mbr.uplink || qer->mbr.downlink) {
        message->maximum_bitrate.presence = 1;
        ogs_pfcp_build_bitrate(
                &message->maximum_bitrate,
                &qer->mbr, update_qer_buf[i].mbr, OGS_PFCP_BITRATE_LEN);
    }
    if (qer->gbr.uplink || qer->gbr.downlink) {
        message->guaranteed_bitrate.presence = 1;
        ogs_pfcp_build_bitrate(
                &message->guaranteed_bitrate,
                &qer->gbr, update_qer_buf[i].gbr, OGS_PFCP_BITRATE_LEN);
    }
}

ogs_pkbuf_t *smf_n4_build_session_establishment_request(
        uint8_t type, smf_sess_t *sess)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_session_establishment_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_bearer_t *bearer = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_qer_t *qer = NULL;
    int i;

    ogs_pfcp_node_id_t node_id;
    ogs_pfcp_f_seid_t f_seid;
    int len;

    ogs_debug("[SMF] Session Establishment Request");
    ogs_assert(sess);

    req = &pfcp_message.pfcp_session_establishment_request;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    /* Node ID */
    ogs_pfcp_sockaddr_to_node_id(
            ogs_pfcp_self()->pfcp_addr, ogs_pfcp_self()->pfcp_addr6,
            ogs_config()->parameter.prefer_ipv4,
            &node_id, &len);
    req->node_id.presence = 1;
    req->node_id.data = &node_id;
    req->node_id.len = len;

    /* F-SEID */
    ogs_pfcp_sockaddr_to_f_seid(
            ogs_pfcp_self()->pfcp_addr, ogs_pfcp_self()->pfcp_addr6,
            &f_seid, &len);
    f_seid.seid = htobe64(sess->smf_n4_seid);
    req->cp_f_seid.presence = 1;
    req->cp_f_seid.data = &f_seid;
    req->cp_f_seid.len = len;

    create_pdr_buf_init();

    /* Create PDR */
    i = 0;
    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_list_for_each(&bearer->pfcp.pdr_list, pdr) {
            build_create_pdr(&req->create_pdr[i], i, pdr);
            i++;
        }
    }

    /* Create FAR */
    i = 0;
    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_list_for_each(&bearer->pfcp.far_list, far) {
            build_create_far(&req->create_far[i], i, far);
            i++;
        }
    }

    /* Create URR */
    i = 0;
    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_list_for_each(&bearer->pfcp.urr_list, urr) {
            build_create_urr(&req->create_urr[i], i, urr);
            i++;
        }
    }

    /* Create QER */
    i = 0;
    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_list_for_each(&bearer->pfcp.qer_list, qer) {
            build_create_qer(&req->create_qer[i], i, qer);
            i++;
        }
    }

    /* PDN Type */
    req->pdn_type.presence = 1;
    req->pdn_type.u8 = sess->pdn.paa.pdn_type;

    pfcp_message.h.type = type;
    pkbuf = ogs_pfcp_build_msg(&pfcp_message);

    create_pdr_buf_clear();

    return pkbuf;
}

ogs_pkbuf_t *smf_n4_build_session_modification_request(
        uint8_t type, smf_bearer_t *bearer)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_session_modification_request_t *req = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_qer_t *qer = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    int i;

    smf_sess_t *sess = NULL;

    ogs_debug("[SMF] Session Modification Request");
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    req = &pfcp_message.pfcp_session_modification_request;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    if (bearer->state.removed) {
        /* Remove PDR */
        i = 0;
        ogs_list_for_each(&bearer->pfcp.pdr_list, pdr) {
            ogs_pfcp_tlv_remove_pdr_t *message = &req->remove_pdr[i];

            message->presence = 1;
            message->pdr_id.presence = 1;
            message->pdr_id.u16 = pdr->id;
            i++;
        }

        /* Remove FAR */
        i = 0;
        ogs_list_for_each(&bearer->pfcp.far_list, far) {
            ogs_pfcp_tlv_remove_far_t *message = &req->remove_far[i];

            message->presence = 1;
            message->far_id.presence = 1;
            message->far_id.u32 = far->id;
            i++;
        }

        /* Remove QER */
        i = 0;
        ogs_list_for_each(&bearer->pfcp.qer_list, qer) {
            ogs_pfcp_tlv_remove_qer_t *message = &req->remove_qer[i];

            message->presence = 1;
            message->qer_id.presence = 1;
            message->qer_id.u32 = qer->id;
            i++;
        }
    } else {
        if (bearer->state.created) {
            create_pdr_buf_init();

            /* Create PDR */
            i = 0;
            ogs_list_for_each(&bearer->pfcp.pdr_list, pdr) {
                build_create_pdr(&req->create_pdr[i], i, pdr);
                i++;
            }

            /* Create FAR */
            i = 0;
            ogs_list_for_each(&bearer->pfcp.far_list, far) {
                build_create_far(&req->create_far[i], i, far);
                i++;
            }

            /* Create QER */
            i = 0;
            ogs_list_for_each(&bearer->pfcp.qer_list, qer) {
                build_create_qer(&req->create_qer[i], i, qer);
                i++;
            }
        }
        if (bearer->state.qos_updated) {
            /* Update QER */
            i = 0;
            ogs_list_for_each(&bearer->pfcp.qer_list, qer) {
                build_update_qer(&req->update_qer[i], i, qer);
                i++;
            }
        }
    }

    pfcp_message.h.type = type;
    pkbuf = ogs_pfcp_build_msg(&pfcp_message);

    if (bearer->state.created) {
        create_pdr_buf_clear();
    }

    return pkbuf;
}

ogs_pkbuf_t *smf_n4_build_session_deletion_request(
        uint8_t type, smf_sess_t *sess)
{
    ogs_pfcp_message_t pfcp_message;

    ogs_debug("[SMF] Session Deletion Request");
    ogs_assert(sess);

    pfcp_message.h.type = type;
    return ogs_pfcp_build_msg(&pfcp_message);
}
