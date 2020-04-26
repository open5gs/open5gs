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

#include "event.h"
#include "context.h"
#include "gtp-path.h"
#include "fd-path.h"
#include "s5c-build.h"
#include "s5c-handler.h"
#include "pfcp-path.h"

#include "ipfw/ipfw2.h"

void smf_s5c_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[PGW] Receiving Echo Request");
    /* FIXME : Before implementing recovery counter correctly,
     *         I'll re-use the recovery value in request message */
    ogs_gtp_send_echo_response(xact, req->recovery.u8, 0);
}

void smf_s5c_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp_echo_response_t *req)
{
    /* Not Implemented */
}

void smf_s5c_handle_create_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_create_session_request_t *req)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_gtp_f_teid_t *sgw_s5c_teid, *gnb_n3_teid;
    smf_bearer_t *bearer = NULL;
    ogs_gtp_bearer_qos_t bearer_qos;
    ogs_gtp_ambr_t *ambr = NULL;
    uint16_t decoded = 0;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Create Session Reqeust");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (sess) {
        bearer = smf_default_bearer_in_sess(sess);
        ogs_assert(bearer);
    }
    if (!bearer) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->imsi.presence == 0) {
        ogs_error("No IMSI");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->sender_f_teid_for_control_plane.presence == 0) {
        ogs_error("No TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created.bearer_level_qos.presence == 0) {
        ogs_error("No EPS Bearer QoS");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.presence == 0) {
        ogs_error("No TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (!ogs_diam_peer_connected()) {
        ogs_error("No Diameter Peer");
        cause_value = OGS_GTP_CAUSE_REMOTE_PEER_NOT_RESPONDING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }
    
    /* Set IMSI */
    sess->imsi_len = req->imsi.len;
    memcpy(sess->imsi, req->imsi.data, sess->imsi_len);
    ogs_buffer_to_bcd(sess->imsi, sess->imsi_len, sess->imsi_bcd);

    /* Control Plane(DL) : SGW-S5C */
    sgw_s5c_teid = req->sender_f_teid_for_control_plane.data;
    ogs_assert(sgw_s5c_teid);
    sess->sgw_s5c_teid = be32toh(sgw_s5c_teid->teid);

    /* Data Plane(DL) : gNB-N3 */
    gnb_n3_teid = req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.data;
    ogs_assert(gnb_n3_teid);
    bearer->gnb_n3_teid = be32toh(gnb_n3_teid->teid);
    rv = ogs_gtp_f_teid_to_ip(gnb_n3_teid, &bearer->gnb_ip);
    ogs_assert(rv == OGS_OK);

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);
    ogs_debug("    gNB_N3_TEID[0x%x] UPF_N3_TEID[0x%x]",
            bearer->gnb_n3_teid, bearer->upf_n3_teid);

    decoded = ogs_gtp_parse_bearer_qos(&bearer_qos,
        &req->bearer_contexts_to_be_created.bearer_level_qos);
    ogs_assert(req->bearer_contexts_to_be_created.bearer_level_qos.len ==
            decoded);
    sess->pdn.qos.qci = bearer_qos.qci;
    sess->pdn.qos.arp.priority_level = bearer_qos.priority_level;
    sess->pdn.qos.arp.pre_emption_capability =
                    bearer_qos.pre_emption_capability;
    sess->pdn.qos.arp.pre_emption_vulnerability =
                    bearer_qos.pre_emption_vulnerability;

    /* Set AMBR if available */
    if (req->aggregate_maximum_bit_rate.presence) {
        /*
         * Ch 8.7. Aggregate Maximum Bit Rate(AMBR) in TS 29.274 V15.9.0
         *
         * AMBR is defined in clause 9.9.4.2 of 3GPP TS 24.301 [23],
         * but it shall be encoded as shown in Figure 8.7-1 as
         * Unsigned32 binary integer values in kbps (1000 bits per second).
         */
        ambr = req->aggregate_maximum_bit_rate.data;
        sess->pdn.ambr.downlink = be32toh(ambr->downlink) * 1000;
        sess->pdn.ambr.uplink = be32toh(ambr->uplink) * 1000;
    }

    /* PCO */
    if (req->protocol_configuration_options.presence) {
        OGS_TLV_STORE_DATA(&sess->ue_pco, &req->protocol_configuration_options);
    }

    /* Set User Location Information */
    if (req->user_location_information.presence) {
        OGS_TLV_STORE_DATA(&sess->user_location_information,
                &req->user_location_information);
    }

    /* Set UE Timezone */
    if (req->ue_time_zone.presence) {
        OGS_TLV_STORE_DATA(&sess->ue_timezone, &req->ue_time_zone);
    }
    
    smf_gx_send_ccr(sess, xact,
        OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST);
}

void smf_s5c_handle_delete_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_delete_session_request_t *req)
{
    uint8_t cause_value = 0;

    ogs_debug("[SMF] Delete Session Request");

    ogs_assert(xact);
    ogs_assert(req);

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (!ogs_diam_peer_connected()) {
        ogs_error("No Diameter Peer");
        cause_value = OGS_GTP_CAUSE_REMOTE_PEER_NOT_RESPONDING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_DELETE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    smf_gx_send_ccr(sess, xact,
        OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST);
}

void smf_s5c_handle_create_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_create_bearer_response_t *rsp)
{
    int rv;
    ogs_gtp_f_teid_t *gnb_n3_teid, *smf_s5u_teid;
    smf_bearer_t *bearer = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[SMF] Create Bearer Response");

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    if (!sess) {
        ogs_warn("No Context in TEID");
        return;
    }

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        if (cause->value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("GTP Failed [CAUSE:%d]", cause->value);
            return;
        }
    }

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return;
    }
    if (rsp->bearer_contexts.s5_s8_u_pgw_f_teid.presence == 0) {
        ogs_error("No SMF TEID");
        return;
    }
    if (rsp->bearer_contexts.s5_s8_u_sgw_f_teid.presence == 0) {
        ogs_error("No SGW TEID");
        return;
    }

    /* Correlate with SMF-S5U-TEID */
    smf_s5u_teid = rsp->bearer_contexts.s5_s8_u_pgw_f_teid.data;
    ogs_assert(smf_s5u_teid);

    /* Find the Bearer by SMF-S5U-TEID */
    bearer = smf_bearer_find_by_smf_s5u_teid(be32toh(smf_s5u_teid->teid));
    ogs_assert(bearer);

    /* Set EBI */
    bearer->ebi = rsp->bearer_contexts.eps_bearer_id.u8;

    /* Data Plane(DL) : gNB-N3 */
    gnb_n3_teid =  rsp->bearer_contexts.s5_s8_u_sgw_f_teid.data;
    ogs_assert(gnb_n3_teid);
    bearer->gnb_n3_teid = be32toh(gnb_n3_teid->teid);
    rv = ogs_gtp_f_teid_to_ip(gnb_n3_teid, &bearer->gnb_ip);
    ogs_assert(rv == OGS_OK);

    ogs_debug("[SMF] Create Bearer Response : SGW[0x%x] --> SMF[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_pfcp_qer_t *qer = NULL;

        /* Only 1 QER is used per bearer */
        qer = ogs_list_first(&bearer->pfcp.qer_list);
        if (!qer) {
            qer = ogs_pfcp_qer_add(&bearer->pfcp);
            ogs_assert(qer);
            qer->id = OGS_NEXT_ID(sess->qer_id, 1, OGS_MAX_NUM_OF_QER+1);
        }

        ogs_list_for_each(&bearer->pfcp.pdr_list, pdr)
            ogs_pfcp_pdr_associate_qer(pdr, qer);

        qer->mbr.uplink = bearer->qos.mbr.uplink;
        qer->mbr.downlink = bearer->qos.mbr.downlink;
        qer->gbr.uplink = bearer->qos.gbr.uplink;
        qer->gbr.downlink = bearer->qos.gbr.downlink;
    }

    smf_pfcp_send_session_modification_request(bearer);
}

void smf_s5c_handle_update_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_update_bearer_response_t *rsp)
{
    int rv;
    smf_bearer_t *bearer = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[SMF] Update Bearer Response");

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    if (!sess) {
        ogs_warn("No Context in TEID");
        return;
    }

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        if (cause->value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("GTP Failed [CAUSE:%d]", cause->value);
            return;
        }
    }

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return;
    }

    bearer = smf_bearer_find_by_ebi(
            sess, rsp->bearer_contexts.eps_bearer_id.u8);
    ogs_assert(bearer);

    ogs_debug("[SMF] Update Bearer Response : SGW[0x%x] --> SMF[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (bearer->state.qos_updated) {
        ogs_pfcp_qer_t *qer = NULL;

        /* Only 1 QER is used per bearer */
        qer = ogs_list_first(&bearer->pfcp.qer_list);
        if (qer) {
            qer->mbr.uplink = bearer->qos.mbr.uplink;
            qer->mbr.downlink = bearer->qos.mbr.downlink;
            qer->gbr.uplink = bearer->qos.gbr.uplink;
            qer->gbr.downlink = bearer->qos.gbr.downlink;

        }
    }

    if (bearer->state.tft_updated)
        ogs_warn("Not Implemented");

#if 0 /* FIXME */
    if (bearer->state.qos_updated || bearer->state.tft_updated) {
#else
    if (bearer->state.qos_updated) {
#endif
        smf_pfcp_send_session_modification_request(bearer);
    }
}

void smf_s5c_handle_delete_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_delete_bearer_response_t *rsp)
{
    int rv;
    smf_bearer_t *bearer = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[SMF] Delete Bearer Response");

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    if (!sess) {
        ogs_warn("No Context in TEID");
        return;
    }

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        if (cause->value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("GTP Failed [CAUSE:%d]", cause->value);
            return;
        }
    }

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return;
    }

    bearer = smf_bearer_find_by_ebi(
            sess, rsp->bearer_contexts.eps_bearer_id.u8);
    ogs_assert(bearer);

    ogs_debug("[SMF] Delete Bearer Response : SGW[0x%x] --> SMF[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    smf_pfcp_send_session_modification_request(bearer);
}

static int reconfigure_packet_filter(smf_pf_t *pf, ogs_gtp_tft_t *tft, int i)
{
    int j;

    for (j = 0; j < tft->pf[i].num_of_component; j++) {
        switch(tft->pf[i].component[j].type) {
        case GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE:
            pf->rule.proto = tft->pf[i].component[j].proto;
            break;
        case GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE:
            pf->rule.ipv4_remote = 1;
            pf->rule.ip.remote.addr[0] = tft->pf[i].component[j].ipv4.addr;
            pf->rule.ip.remote.mask[0] = tft->pf[i].component[j].ipv4.mask;
            break;
        case GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE:
            pf->rule.ipv4_local = 1;
            pf->rule.ip.local.addr[0] = tft->pf[i].component[j].ipv4.addr;
            pf->rule.ip.local.mask[0] = tft->pf[i].component[j].ipv4.mask;
            break;
        case GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE:
            pf->rule.ipv6_remote = 1;
            memcpy(pf->rule.ip.remote.addr,
                tft->pf[i].component[j].ipv6_mask.addr,
                sizeof(pf->rule.ip.remote.addr));
            memcpy(pf->rule.ip.remote.mask,
                tft->pf[i].component[j].ipv6_mask.mask,
                sizeof(pf->rule.ip.remote.mask));
            break;
        case GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE:
            pf->rule.ipv6_remote = 1;
            memcpy(pf->rule.ip.remote.addr,
                tft->pf[i].component[j].ipv6_mask.addr,
                sizeof(pf->rule.ip.remote.addr));
            n2mask((struct in6_addr *)pf->rule.ip.remote.mask,
                tft->pf[i].component[j].ipv6.prefixlen);
            break;
        case GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_TYPE:
            pf->rule.ipv6_local = 1;
            memcpy(pf->rule.ip.local.addr,
                tft->pf[i].component[j].ipv6_mask.addr,
                sizeof(pf->rule.ip.local.addr));
            memcpy(pf->rule.ip.local.mask,
                tft->pf[i].component[j].ipv6_mask.mask,
                sizeof(pf->rule.ip.local.mask));
            break;
        case GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE:
            pf->rule.ipv6_local = 1;
            memcpy(pf->rule.ip.local.addr,
                tft->pf[i].component[j].ipv6_mask.addr,
                sizeof(pf->rule.ip.local.addr));
            n2mask((struct in6_addr *)pf->rule.ip.local.mask,
                tft->pf[i].component[j].ipv6.prefixlen);
            break;
        case GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE:
            pf->rule.port.local.low = pf->rule.port.local.high =
                    tft->pf[i].component[j].port.low;
            break;
        case GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE:
            pf->rule.port.remote.low = pf->rule.port.remote.high =
                    tft->pf[i].component[j].port.low;
            break;
        case GTP_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE:
            pf->rule.port.local.low = tft->pf[i].component[j].port.low;
            pf->rule.port.local.high = tft->pf[i].component[j].port.high;
            break;
        case GTP_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE:
            pf->rule.port.remote.low = tft->pf[i].component[j].port.low;
            pf->rule.port.remote.high = tft->pf[i].component[j].port.high;
            break;
        default:
            ogs_error("Unknown Packet Filter Type(%d)",
                    tft->pf[i].component[j].type);
            return OGS_ERROR;
        }
    }

    return j;
}

void smf_s5c_handle_bearer_resource_command(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_bearer_resource_command_t *cmd)
{
    int rv;
    uint8_t cause_value = 0;
    int i;

    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_bearer_t *bearer = NULL;
    smf_pf_t *pf = NULL;

    int16_t decoded;
    ogs_gtp_tft_t tft;

    int tft_update = 0;
    int qos_update = 0;
    int tft_delete = 0;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(cmd);

    ogs_debug("[PGW] Bearer Resource Command");
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (cmd->linked_eps_bearer_id.presence == 0) {
        ogs_error("No Linked EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    } else {
        bearer = smf_bearer_find_by_ebi(
                sess, cmd->linked_eps_bearer_id.u8);
        if (!bearer) {
            ogs_error("No Context for Linked EPS Bearer ID[%d]",
                    cmd->linked_eps_bearer_id.u8);
            cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
        }
    }

    if (cmd->procedure_transaction_id.presence == 0) {
        ogs_error("No PTI");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (cmd->traffic_aggregate_description.presence == 0) {
        ogs_error("No Traffic aggregate description(TAD)");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE, cause_value);
        return;
    }

    ogs_assert(bearer);

    decoded = ogs_gtp_parse_tft(&tft, &cmd->traffic_aggregate_description);
    ogs_assert(cmd->traffic_aggregate_description.len == decoded);

    if (tft.code == OGS_GTP_TFT_CODE_NO_TFT_OPERATION) {
        /* No operation */
    } else if (tft.code == OGS_GTP_TFT_CODE_DELETE_EXISTING_TFT) {
        smf_pf_remove_all(bearer);
        tft_delete = 1;
    } else if (tft.code ==
            OGS_GTP_TFT_CODE_REPLACE_PACKET_FILTERS_IN_EXISTING) {
        for (i = 0; i < tft.num_of_packet_filter; i++) {
            pf = smf_pf_find_by_id(bearer, tft.pf[i].identifier+1);
            if (pf) {
                if (reconfigure_packet_filter(pf, &tft, i) < 0) {
                    ogs_gtp_send_error_message(
                        xact, sess ? sess->sgw_s5c_teid : 0,
                        OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE,
                        OGS_GTP_CAUSE_SEMANTIC_ERROR_IN_THE_TAD_OPERATION);
                    return;
                }
            }

            tft_update = 1;
        }
    } else if (tft.code ==
                OGS_GTP_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT ||
                tft.code == OGS_GTP_TFT_CODE_CREATE_NEW_TFT) {
        if (tft.code == OGS_GTP_TFT_CODE_CREATE_NEW_TFT)
            smf_pf_remove_all(bearer);

        for (i = 0; i < tft.num_of_packet_filter; i++) {
            pf = smf_pf_find_by_id(bearer, tft.pf[i].identifier+1);
            if (!pf)
                pf = smf_pf_add(bearer, tft.pf[i].precedence);
            ogs_assert(pf);

            if (reconfigure_packet_filter(pf, &tft, i) < 0) {
                ogs_gtp_send_error_message(
                    xact, sess ? sess->sgw_s5c_teid : 0,
                    OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE,
                    OGS_GTP_CAUSE_SEMANTIC_ERROR_IN_THE_TAD_OPERATION);
                return;
            }

            tft_update = 1;
        }
    } else if (tft.code ==
            OGS_GTP_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING) {
        for (i = 0; i < tft.num_of_packet_filter; i++) {
            pf = smf_pf_find_by_id(bearer, tft.pf[i].identifier+1);
            if (pf)
                smf_pf_remove(pf);

            if (ogs_list_count(&bearer->pf_list))
                tft_update = 1;
            else
                tft_delete = 1;
        }
    }

    if (cmd->flow_quality_of_service.presence) {
        ogs_gtp_flow_qos_t flow_qos;

        decoded = ogs_gtp_parse_flow_qos(
                &flow_qos, &cmd->flow_quality_of_service);
        ogs_assert(cmd->flow_quality_of_service.len == decoded);

        bearer->qos.mbr.uplink = flow_qos.ul_mbr;
        bearer->qos.mbr.downlink = flow_qos.dl_mbr;
        bearer->qos.gbr.uplink = flow_qos.ul_gbr;
        bearer->qos.gbr.downlink = flow_qos.dl_gbr;

        qos_update = 1;
    }

    if (tft_update == 0 && tft_delete == 0 && qos_update == 0) {
        /* No modification */
        ogs_gtp_send_error_message(xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE,
                OGS_GTP_CAUSE_SERVICE_NOT_SUPPORTED);
        return;
    }

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.teid = sess->sgw_s5c_teid;

    if (tft_delete) {
        memset(&bearer->state, 0, sizeof(bearer->state));
        bearer->state.removed = true;

        h.type = OGS_GTP_DELETE_BEARER_REQUEST_TYPE;
        pkbuf = smf_s5c_build_delete_bearer_request(
                h.type, bearer, cmd->procedure_transaction_id.u8);
        ogs_expect_or_return(pkbuf);
    } else {
        memset(&bearer->state, 0, sizeof(bearer->state));
        if (tft_update)
            bearer->state.tft_updated = true;
        if (qos_update)
            bearer->state.qos_updated = true;

        h.type = OGS_GTP_UPDATE_BEARER_REQUEST_TYPE;
        pkbuf = smf_s5c_build_update_bearer_request(
                h.type, bearer, cmd->procedure_transaction_id.u8,
                tft_update ? &tft : NULL, qos_update);
        ogs_expect_or_return(pkbuf);
    }


    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}
