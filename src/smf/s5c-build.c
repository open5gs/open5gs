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
#include "s5c-build.h"

#include "ipfw/ipfw2.h"

ogs_pkbuf_t *smf_s5c_build_create_session_response(
        uint8_t type, smf_sess_t *sess)
{
    int rv;
    smf_bearer_t *bearer = NULL;

    ogs_gtp_message_t gtp_message;
    ogs_gtp_create_session_response_t *rsp = NULL;

    ogs_gtp_cause_t cause;
    ogs_gtp_cause_t bearer_cause;
    ogs_gtp_f_teid_t smf_s5c_teid, pgw_s5u_teid;
    ogs_gtp_ambr_t ambr;
    ogs_gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];
    int len;
    uint8_t pco_buf[OGS_MAX_PCO_LEN];
    int16_t pco_len;

    ogs_debug("[SMF] Create Session Response");

    ogs_assert(sess);
    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);
    ogs_debug("    SGW_S5U_TEID[0x%x] PGW_S5U_TEID[0x%x]",
            bearer->sgw_s5u_teid, bearer->pgw_s5u_teid);

    rsp = &gtp_message.create_session_response;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));

    /* Set Cause */
    memset(&cause, 0, sizeof(cause));
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;
    if (sess->ue_session_type != sess->session.session_type)
        cause.value = OGS_GTP_CAUSE_NEW_PDN_TYPE_DUE_TO_NETWORK_PREFERENCE;

    /* Control Plane(UL) : SMF-S5C */
    memset(&smf_s5c_teid, 0, sizeof(ogs_gtp_f_teid_t));
    switch (sess->gtp_rat_type) {
    case OGS_GTP_RAT_TYPE_EUTRAN:
        smf_s5c_teid.interface_type = OGS_GTP_F_TEID_S5_S8_PGW_GTP_C;
        break;
    case OGS_GTP_RAT_TYPE_WLAN:
        smf_s5c_teid.interface_type = OGS_GTP_F_TEID_S2B_PGW_GTP_C;
        break;
    default:
        ogs_error("Unknown RAT Type [%d]", sess->gtp_rat_type);
        ogs_assert_if_reached();
    }
    smf_s5c_teid.teid = htobe32(sess->smf_n4_teid);
    rv = ogs_gtp_sockaddr_to_f_teid(
            ogs_gtp_self()->gtpc_addr, ogs_gtp_self()->gtpc_addr6,
            &smf_s5c_teid, &len);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        presence = 1;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        data = &smf_s5c_teid;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        len = len;

    /* PDN Address Allocation */
    rsp->pdn_address_allocation.data = &sess->session.paa;
    if (sess->ipv4 && sess->ipv6)
        rsp->pdn_address_allocation.len = OGS_PAA_IPV4V6_LEN;
    else if (sess->ipv4)
        rsp->pdn_address_allocation.len = OGS_PAA_IPV4_LEN;
    else if (sess->ipv6)
        rsp->pdn_address_allocation.len = OGS_PAA_IPV6_LEN;
    else
        ogs_assert_if_reached();
    rsp->pdn_address_allocation.presence = 1;

    /* APN Restriction */
    switch (sess->gtp_rat_type) {
    case OGS_GTP_RAT_TYPE_EUTRAN:
        rsp->apn_restriction.presence = 1;
        rsp->apn_restriction.u8 = OGS_GTP_APN_NO_RESTRICTION;
        break;
    case OGS_GTP_RAT_TYPE_WLAN:
        break;
    default:
        ogs_error("Unknown RAT Type [%d]", sess->gtp_rat_type);
        ogs_assert_if_reached();
    }
    
    /* APN-AMBR
     * if PCRF changes APN-AMBR, this should be included. */
    if (sess->gtp.create_session_response_apn_ambr == true) {
        memset(&ambr, 0, sizeof(ogs_gtp_ambr_t));
        ambr.uplink = htobe32(sess->session.ambr.uplink / 1000);
        ambr.downlink = htobe32(sess->session.ambr.downlink / 1000);
        rsp->aggregate_maximum_bit_rate.presence = 1;
        rsp->aggregate_maximum_bit_rate.data = &ambr;
        rsp->aggregate_maximum_bit_rate.len = sizeof(ambr);
    }

    /* PCO */
    if (sess->gtp.ue_pco.presence &&
            sess->gtp.ue_pco.len && sess->gtp.ue_pco.data) {
        pco_len = smf_pco_build(
                pco_buf, sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
        ogs_assert(pco_len > 0);
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* Bearer EBI */
    rsp->bearer_contexts_created.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.u8 = bearer->ebi;

    /* Bearer Cause */
    memset(&bearer_cause, 0, sizeof(bearer_cause));
    rsp->bearer_contexts_created.cause.presence = 1;
    rsp->bearer_contexts_created.cause.len = sizeof(bearer_cause);
    rsp->bearer_contexts_created.cause.data = &bearer_cause;
    bearer_cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    /* Bearer QoS
     * if PCRF changes Bearer QoS, this should be included. */
    if (sess->gtp.create_session_response_bearer_qos == true) {
        memset(&bearer_qos, 0, sizeof(bearer_qos));
        bearer_qos.qci = sess->session.qos.index;
        bearer_qos.priority_level = sess->session.qos.arp.priority_level;
        bearer_qos.pre_emption_capability =
            sess->session.qos.arp.pre_emption_capability;
        bearer_qos.pre_emption_vulnerability =
            sess->session.qos.arp.pre_emption_vulnerability;

        rsp->bearer_contexts_created.bearer_level_qos.presence = 1;
        ogs_gtp_build_bearer_qos(&rsp->bearer_contexts_created.bearer_level_qos,
                &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);
    }

    /* Bearer Charging ID */
    rsp->bearer_contexts_created.charging_id.presence = 1;
    rsp->bearer_contexts_created.charging_id.u32 = sess->charging.id;

    /* Data Plane(UL) : SMF-S5U */
    memset(&pgw_s5u_teid, 0, sizeof(ogs_gtp_f_teid_t));
    pgw_s5u_teid.teid = htobe32(bearer->pgw_s5u_teid);
    ogs_assert(bearer->pgw_s5u_addr || bearer->pgw_s5u_addr6);
    rv = ogs_gtp_sockaddr_to_f_teid(
        bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6, &pgw_s5u_teid, &len);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);

    switch (sess->gtp_rat_type) {
    case OGS_GTP_RAT_TYPE_EUTRAN:
        pgw_s5u_teid.interface_type = OGS_GTP_F_TEID_S5_S8_PGW_GTP_U;
        rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence = 1;
        rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.data = &pgw_s5u_teid;
        rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.len = len;
        break;
    case OGS_GTP_RAT_TYPE_WLAN:
        pgw_s5u_teid.interface_type = OGS_GTP_F_TEID_S2B_U_PGW_GTP_U;
        rsp->bearer_contexts_created.s12_rnc_f_teid.presence = 1;
        rsp->bearer_contexts_created.s12_rnc_f_teid.data = &pgw_s5u_teid;
        rsp->bearer_contexts_created.s12_rnc_f_teid.len = len;
        break;
    default:
        ogs_error("Unknown RAT Type [%d]", sess->gtp_rat_type);
        ogs_assert_if_reached();
    }

    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}

ogs_pkbuf_t *smf_s5c_build_delete_session_response(
        uint8_t type, smf_sess_t *sess)
{
    ogs_gtp_message_t gtp_message;
    ogs_gtp_delete_session_response_t *rsp = NULL;

    ogs_gtp_cause_t cause;
    uint8_t pco_buf[OGS_MAX_PCO_LEN];
    int16_t pco_len;
    
    /* prepare cause */
    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    rsp = &gtp_message.delete_session_response;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));

    /* Cause */
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    /* Recovery */

    /* PCO */
    if (sess->gtp.ue_pco.presence &&
            sess->gtp.ue_pco.len && sess->gtp.ue_pco.data) {
        pco_len = smf_pco_build(
                pco_buf, sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
        ogs_assert(pco_len > 0);
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* Private Extension */

    /* build */
    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}

ogs_pkbuf_t *smf_s5c_build_modify_bearer_response(
        uint8_t type, smf_sess_t *sess,
        ogs_gtp_modify_bearer_request_t *req)
{
    ogs_gtp_message_t gtp_message;
    ogs_gtp_modify_bearer_response_t *rsp = NULL;

    ogs_gtp_cause_t cause;

    ogs_assert(sess);
    ogs_assert(req);

    rsp = &gtp_message.modify_bearer_response;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    rsp->bearer_contexts_modified.presence = 1;
    rsp->bearer_contexts_modified.eps_bearer_id.presence = 1;
    rsp->bearer_contexts_modified.eps_bearer_id.u8 =
        req->bearer_contexts_to_be_modified.eps_bearer_id.u8;
    rsp->bearer_contexts_modified.s1_u_enodeb_f_teid.presence = 1;
    rsp->bearer_contexts_modified.s1_u_enodeb_f_teid.data =
        req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.data;
    rsp->bearer_contexts_modified.s1_u_enodeb_f_teid.len =
        req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.len;

    rsp->bearer_contexts_modified.cause.presence = 1;
    rsp->bearer_contexts_modified.cause.len = sizeof(cause);
    rsp->bearer_contexts_modified.cause.data = &cause;

    /* build */
    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}

ogs_pkbuf_t *smf_s5c_build_create_bearer_request(
        uint8_t type, smf_bearer_t *bearer, ogs_gtp_tft_t *tft)
{
    int rv;
    smf_sess_t *sess = NULL;
    smf_bearer_t *linked_bearer = NULL;

    ogs_gtp_message_t gtp_message;
    ogs_gtp_create_bearer_request_t *req = NULL;

    ogs_gtp_f_teid_t pgw_s5u_teid;
    ogs_gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];
    int len;
    char tft_buf[OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE];

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    linked_bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(linked_bearer);

    ogs_debug("[SMF] Create Bearer Request");
    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    req = &gtp_message.create_bearer_request;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));
 
    /* Linked EBI */
    req->linked_eps_bearer_id.presence = 1;
    req->linked_eps_bearer_id.u8 = linked_bearer->ebi;

    /* Bearer EBI */
    req->bearer_contexts.presence = 1;
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    /* Data Plane(UL) : PGW-S5U */
    memset(&pgw_s5u_teid, 0, sizeof(ogs_gtp_f_teid_t));
    pgw_s5u_teid.interface_type = OGS_GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.teid = htobe32(bearer->pgw_s5u_teid);
    ogs_assert(bearer->pgw_s5u_addr || bearer->pgw_s5u_addr6);
    rv = ogs_gtp_sockaddr_to_f_teid(
        bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6, &pgw_s5u_teid, &len);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);
    req->bearer_contexts.s4_u_sgsn_f_teid.presence = 1;
    req->bearer_contexts.s4_u_sgsn_f_teid.data = &pgw_s5u_teid;
    req->bearer_contexts.s4_u_sgsn_f_teid.len = len;

    /* Bearer QoS */
    memset(&bearer_qos, 0, sizeof(bearer_qos));
    bearer_qos.qci = bearer->qos.index;
    bearer_qos.priority_level = bearer->qos.arp.priority_level;
    bearer_qos.pre_emption_capability = 
        bearer->qos.arp.pre_emption_capability;
    bearer_qos.pre_emption_vulnerability =
        bearer->qos.arp.pre_emption_vulnerability;
    bearer_qos.dl_mbr = bearer->qos.mbr.downlink;
    bearer_qos.ul_mbr = bearer->qos.mbr.uplink;
    bearer_qos.dl_gbr = bearer->qos.gbr.downlink;
    bearer_qos.ul_gbr = bearer->qos.gbr.uplink;

    req->bearer_contexts.bearer_level_qos.presence = 1;
    ogs_gtp_build_bearer_qos(&req->bearer_contexts.bearer_level_qos,
            &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);

    /* Bearer TFT */
    if (tft && tft->num_of_packet_filter) {
        req->bearer_contexts.tft.presence = 1;
        ogs_gtp_build_tft(&req->bearer_contexts.tft,
                tft, tft_buf, OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE);
    }

    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}

ogs_pkbuf_t *smf_s5c_build_update_bearer_request(
        uint8_t type, smf_bearer_t *bearer, uint8_t pti,
        ogs_gtp_tft_t *tft, bool qos_presence)
{
    smf_sess_t *sess = NULL;

    ogs_gtp_message_t gtp_message;
    ogs_gtp_update_bearer_request_t *req = NULL;

    ogs_gtp_ambr_t ambr;
    ogs_gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];
    char tft_buf[OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE];

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    ogs_debug("[SMF] Update Bearer Request");
    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);
    req = &gtp_message.update_bearer_request;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));
 
    /* Bearer EBI */
    req->bearer_contexts.presence = 1;
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    if (sess->session.ambr.uplink || sess->session.ambr.downlink) {
        /*
         * Ch 8.7. Aggregate Maximum Bit Rate(AMBR) in TS 29.274 V15.9.0
         *
         * AMBR is defined in clause 9.9.4.2 of 3GPP TS 24.301 [23],
         * but it shall be encoded as shown in Figure 8.7-1 as
         * Unsigned32 binary integer values in kbps (1000 bits per second).
         */
        memset(&ambr, 0, sizeof(ogs_gtp_ambr_t));
        ambr.uplink = htobe32(sess->session.ambr.uplink / 1000);
        ambr.downlink = htobe32(sess->session.ambr.downlink / 1000);
        req->aggregate_maximum_bit_rate.presence = 1;
        req->aggregate_maximum_bit_rate.data = &ambr;
        req->aggregate_maximum_bit_rate.len = sizeof(ambr);
    }

    /* PTI */
    if (pti) {
        req->procedure_transaction_id.presence = 1;
        req->procedure_transaction_id.u8 = pti;
    }

    /* Bearer QoS */
    if (qos_presence == true) {
        memset(&bearer_qos, 0, sizeof(bearer_qos));
        bearer_qos.qci = bearer->qos.index;
        bearer_qos.priority_level = bearer->qos.arp.priority_level;
        bearer_qos.pre_emption_capability = 
            bearer->qos.arp.pre_emption_capability;
        bearer_qos.pre_emption_vulnerability =
            bearer->qos.arp.pre_emption_vulnerability;
        bearer_qos.dl_mbr = bearer->qos.mbr.downlink;
        bearer_qos.ul_mbr = bearer->qos.mbr.uplink;
        bearer_qos.dl_gbr = bearer->qos.gbr.downlink;
        bearer_qos.ul_gbr = bearer->qos.gbr.uplink;

        req->bearer_contexts.bearer_level_qos.presence = 1;
        ogs_gtp_build_bearer_qos(&req->bearer_contexts.bearer_level_qos,
                &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);
    }

    /* Bearer TFT */
    if (tft && tft->num_of_packet_filter) {
        req->bearer_contexts.tft.presence = 1;
        ogs_gtp_build_tft(&req->bearer_contexts.tft,
                tft, tft_buf, OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE);
    }

    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}

ogs_pkbuf_t *smf_s5c_build_delete_bearer_request(
        uint8_t type, smf_bearer_t *bearer, uint8_t pti, uint8_t cause_value)
{
    smf_sess_t *sess = NULL;
    smf_bearer_t *linked_bearer = NULL;

    ogs_gtp_message_t gtp_message;
    ogs_gtp_delete_bearer_request_t *req = NULL;

    ogs_gtp_cause_t cause;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    linked_bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(linked_bearer);

    ogs_debug("[SMF] Delete Bearer Request");
    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);
    req = &gtp_message.delete_bearer_request;
    memset(&gtp_message, 0, sizeof(ogs_gtp_message_t));
 
    if (bearer->ebi == linked_bearer->ebi) {
       /*
        * << Linked EPS Bearer ID >>
        *
        * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to SGW/MME.
        * 2. MME sends Delete Bearer Response to SGW/SMF.
        *
        * OR
        *
        * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to ePDG.
        * 2. ePDG sends Delete Bearer Response(DEFAULT BEARER) to SMF.
        */
        req->linked_eps_bearer_id.presence = 1;
        req->linked_eps_bearer_id.u8 = bearer->ebi;
    } else {
       /*
        * << EPS Bearer IDs >>
        *
        * 1. MME sends Bearer Resource Command to SGW/SMF.
        * 2. SMF sends Delete Bearer Request(DEDICATED BEARER) to SGW/MME.
        * 3. MME sends Delete Bearer Response(DEDICATED BEARER) to SGW/SMF.
        *
        * OR
        *
        * 1. SMF sends Delete Bearer Request(DEDICATED BEARER) to SGW/MME.
        * 2. MME sends Delete Bearer Response(DEDICATED BEARER) to SGW/SMF.
        */
        req->eps_bearer_ids.presence = 1;
        req->eps_bearer_ids.u8 = bearer->ebi;
    }

    if (pti) {
        req->procedure_transaction_id.presence = 1;
        req->procedure_transaction_id.u8 = pti;
    }

    if (cause_value != OGS_GTP_CAUSE_UNDEFINED_VALUE) {
        memset(&cause, 0, sizeof(cause));
        cause.value = cause_value;
        req->cause.presence = 1;
        req->cause.len = sizeof(cause);
        req->cause.data = &cause;
    }

    gtp_message.h.type = type;
    return ogs_gtp_build_msg(&gtp_message);
}
