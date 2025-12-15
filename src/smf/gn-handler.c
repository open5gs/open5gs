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

#include "event.h"
#include "context.h"
#include "gtp-path.h"
#include "fd-path.h"
#include "gn-build.h"
#include "gn-handler.h"
#include "n4-handler.h"
#include "pfcp-path.h"

#include "ipfw/ipfw2.h"

void smf_gn_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[PGW] Receiving Echo Request");
    /* FIXME : Implementing recovery counter correctly */
    ogs_gtp1_send_echo_response(xact, 0);
}

void smf_gn_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_response_t *req)
{
    /* Not Implemented */
}

uint8_t smf_gn_handle_create_pdp_context_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp1_create_pdp_context_request_t *req)
{
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    int rv;
    uint8_t cause_value = 0;
    uint8_t pfcp_cause;

    ogs_gtp1_uli_t uli;

    smf_ue_t *smf_ue = NULL;
    ogs_eua_t *eua = NULL;
    smf_bearer_t *bearer = NULL;
    ogs_gtp1_qos_profile_decoded_t *qos_pdec;
    uint8_t qci = 9;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Create PDP Context Request");

    cause_value = OGS_GTP1_CAUSE_REQUEST_ACCEPTED;

    if (req->imsi.presence == 0) {
        ogs_error("No IMSI");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->selection_mode.presence == 0) {
        ogs_error("No Selection Mode");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->tunnel_endpoint_identifier_data_i.presence == 0) {
        ogs_error("No TEID");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->nsapi.presence == 0) {
        ogs_error("No NSAPI");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->sgsn_address_for_signalling.presence == 0) {
        ogs_error("No SGSN Address for signalling");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->sgsn_address_for_user_traffic.presence == 0) {
        ogs_error("No SGSN Address for user traffic");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->msisdn.presence == 0) {
        ogs_error("No MSISDN");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->quality_of_service_profile.presence == 0) {
        ogs_error("No QoS Profile");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->user_location_information.presence == 0) {
        ogs_error("No User Location Info");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }

    if (!ogs_diam_is_relay_or_app_advertised(OGS_DIAM_GX_APPLICATION_ID)) {
        ogs_error("No Gx Diameter Peer");
        cause_value = OGS_GTP1_CAUSE_NO_RESOURCES_AVAILABLE;
    }

    if (cause_value != OGS_GTP1_CAUSE_REQUEST_ACCEPTED)
        return cause_value;

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    /* Store NSAPI */
    sess->gtp.v1.nsapi = req->nsapi.u8;
    /* Selection Mode, TS 29.060 7.7.12 */
    sess->gtp.selection_mode = req->selection_mode.u8 & 0x03;
    if (sess->gtp.selection_mode > 2)
        sess->gtp.selection_mode = 2;

    if (req->charging_characteristics.presence) {
        OGS_TLV_STORE_DATA(&sess->gtp.charging_characteristics, &req->charging_characteristics);
    }

    /* Control Plane(DL) : SGW-S5C */
    sess->sgw_s5c_teid = req->tunnel_endpoint_identifier_control_plane.u32;
    rv = ogs_gtp1_gsn_addr_to_ip(req->sgsn_address_for_signalling.data,
                                 req->sgsn_address_for_signalling.len,
                                  &sess->sgw_s5c_ip);
    ogs_assert(rv == OGS_OK);
    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    /* User Location Information, TS 29.060 sec 7.7.51 */
    /* Note: the IE is content is different in GTPv1C and GTPv2C */
    OGS_TLV_STORE_DATA(&sess->gtp.user_location_information,
                       &req->user_location_information);
    if (ogs_gtp1_parse_uli(&uli, &req->user_location_information) == 0)
        return OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT;

    switch (uli.geo_loc_type) {
    case OGS_GTP1_GEO_LOC_TYPE_CGI:
        ogs_nas_to_plmn_id(&sess->serving_plmn_id, &uli.cgi.nas_plmn_id);
        break;
    case OGS_GTP1_GEO_LOC_TYPE_SAI:
        ogs_nas_to_plmn_id(&sess->serving_plmn_id, &uli.sai.nas_plmn_id);
        break;
    case  OGS_GTP1_GEO_LOC_TYPE_RAI:
        ogs_nas_to_plmn_id(&sess->serving_plmn_id, &uli.rai.nas_plmn_id);
        break;
    /* default: should not happen */
    }

    /* Set MSISDN: */
    /* TS 29.060 sec 7.7.33, TS 29.002 ISDN-AddressString
     * 1 byte offset: Get rid of address and numbering plan indicator  */
    if (req->msisdn.len == 0 ||
        (req->msisdn.len - 1) > sizeof(smf_ue->msisdn))  {
        ogs_error("MSISDN wrong size %u > %zu",
            (req->msisdn.len - 1), sizeof(smf_ue->msisdn));
        return OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT;
    }
    smf_ue->msisdn_len = req->msisdn.len - 1;
    if (smf_ue->msisdn_len > 0) {
        memcpy(smf_ue->msisdn,
            (uint8_t*)req->msisdn.data + 1, smf_ue->msisdn_len);
        ogs_buffer_to_bcd(
            smf_ue->msisdn, smf_ue->msisdn_len, smf_ue->msisdn_bcd);
    }

    /* Common Flags 7.7.48 */
    if (req->common_flags.presence) {
        sess->gtp.v1.common_flags = *(ogs_gtp1_common_flags_t*)req->common_flags.data;
    }

    /* Set some sane default if information not present in QoS Profile or APN-AMBR: */
    sess->session.ambr.downlink = 100000000;
    sess->session.ambr.uplink = 100000000;

    /* Set Bearer QoS */
    OGS_TLV_STORE_DATA(&sess->gtp.v1.qos, &req->quality_of_service_profile);
    qos_pdec = &sess->gtp.v1.qos_pdec;
    rv = ogs_gtp1_parse_qos_profile(qos_pdec, &req->quality_of_service_profile);
    if(rv < 0)
        return OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT;

    /* 3GPP TS 23.060 section 9.2.1A: "The QoS profiles of the PDP context and EPS bearer are mapped as specified in TS 23.401"
     * 3GPP TS 23.401 Annex E: "Mapping between EPS and Release 99 QoS parameters"
     */
    ogs_gtp1_qos_profile_to_qci(qos_pdec, &qci);
    sess->session.qos.index = qci;
    sess->session.qos.arp.priority_level = qos_pdec->qos_profile.arp; /* 3GPP TS 23.401 Annex E Table E.2 */
    sess->session.qos.arp.pre_emption_capability = 0; /* ignored as per 3GPP TS 23.401 Annex E */
    sess->session.qos.arp.pre_emption_vulnerability = 0; /* ignored as per 3GPP TS 23.401 Annex E */
    if (qos_pdec->data_octet6_to_13_present) {
        sess->session.ambr.downlink = qos_pdec->dec_mbr_kbps_dl * 1000;
        sess->session.ambr.uplink = qos_pdec->dec_mbr_kbps_ul * 1000;
    }

    /* APN-AMBR, 7.7.98 */
    if (req->apn_ambr.presence) {
        /* "The APN-AMBR IE shall be included as the authorized APN-AMBR if the
         * GGSN supports this IE and if the APN-AMBR IE has been included in the
         * corresponding request message." */
        sess->gtp.v1.peer_supports_apn_ambr = true;
        if (req->apn_ambr.len >= sizeof(ogs_gtp1_apn_ambr_t)) {
            ogs_gtp1_apn_ambr_t *ambr = req->apn_ambr.data;
            sess->session.ambr.uplink = be32toh(ambr->uplink) * 1000;
            sess->session.ambr.downlink = be32toh(ambr->downlink) * 1000;
        }
    }

    /* PCO */
    if (req->protocol_configuration_options.presence) {
        OGS_TLV_STORE_DATA(&sess->gtp.ue_pco,
                &req->protocol_configuration_options);
    }

    /* Set UE Timezone */
    if (req->ms_time_zone.presence) {
        /* value part is compatible between UE Time Zone and MS Time Zone */
        OGS_TLV_STORE_DATA(&sess->gtp.ue_timezone, &req->ms_time_zone);
    }

    /* Set IMEI(SV) */
    if (req->imei.presence && req->imei.len > 0) {
        smf_ue->imeisv_len = req->imei.len;
        memcpy(smf_ue->imeisv,
            (uint8_t*)req->imei.data, smf_ue->imeisv_len);
        ogs_buffer_to_bcd(
            smf_ue->imeisv, smf_ue->imeisv_len, smf_ue->imeisv_bcd);
    }

    /* UE IP Address */
    eua = req->end_user_address.data;
    ogs_assert(eua);
    rv = ogs_gtp1_eua_to_ip(eua, req->end_user_address.len, &sess->session.ue_ip,
            &sess->ue_session_type);
    if(rv != OGS_OK)
        return OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT;

    /* Initially Set Session Type from UE */
    sess->session.session_type = sess->ue_session_type;

    /* Remove all previous bearer */
    smf_bearer_remove_all(sess);

    /* Setup Default Bearer */
    bearer = smf_bearer_add(sess);
    ogs_assert(bearer);

    /* Set Bearer EBI */
    /* 3GPP TS 23.060 clause 9.2.1A: "1:1 mapping between NSAPI and EPS Bearer ID" */
    /* 3GPP TS 23.401 clause 5.2.1: "the same identity value is used for the EPS bearer ID and the NSAPI/RAB ID" */
    bearer->ebi = req->nsapi.u8;

    /* User Plane(DL) : SGW-S5C */
    bearer->sgw_s5u_teid = req->tunnel_endpoint_identifier_data_i.u32;
    rv = ogs_gtp1_gsn_addr_to_ip(req->sgsn_address_for_user_traffic.data,
                                 req->sgsn_address_for_user_traffic.len,
                                 &bearer->sgw_s5u_ip);
    ogs_assert(rv == OGS_OK);
    ogs_debug("    SGW_S5U_TEID[0x%x] PGW_S5U_TEID[0x%x]",
            bearer->sgw_s5u_teid, bearer->pgw_s5u_teid);
    if (qos_pdec->data_octet6_to_13_present) {
        bearer->qos.gbr.downlink = qos_pdec->dec_gbr_kbps_dl * 1000;
        bearer->qos.gbr.uplink = qos_pdec->dec_gbr_kbps_ul * 1000;
    } else {
        /* Set some sane default if infomation not present in Qos Profile IE: */
        bearer->qos.gbr.downlink = sess->session.ambr.downlink;
        bearer->qos.gbr.uplink = sess->session.ambr.uplink;
    }

    /* Select PGW based on UE Location Information */
    smf_sess_select_upf(sess);

    /* Check if selected PGW is associated with SMF */
    ogs_assert(sess->pfcp_node);
    if (!OGS_FSM_CHECK(&sess->pfcp_node->sm, smf_pfcp_state_associated))
        return OGS_GTP1_CAUSE_NO_RESOURCES_AVAILABLE;

    if ((pfcp_cause = smf_sess_set_ue_ip(sess)) != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
        cause_value = gtp_cause_from_pfcp(pfcp_cause, 1);
        return cause_value;
    }

    ogs_info("UE IMSI[%s] APN[%s] IPv4[%s] IPv6[%s]",
        smf_ue->imsi_bcd,
        sess->session.name,
        sess->ipv4 ? OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
        sess->ipv6 ? OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");

    return cause_value;
}

uint8_t smf_gn_handle_delete_pdp_context_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp1_delete_pdp_context_request_t *req)
{
    ogs_debug("Delete PDP Context Request");

    if (!ogs_diam_is_relay_or_app_advertised(OGS_DIAM_GX_APPLICATION_ID)) {
        ogs_error("No Gx Diameter Peer");
        return OGS_GTP1_CAUSE_NO_RESOURCES_AVAILABLE;
    }

    /* PCO */
    if (req->protocol_configuration_options.presence) {
        OGS_TLV_STORE_DATA(&sess->gtp.ue_pco,
                &req->protocol_configuration_options);
    } else {
        /*
         * Clear contents to reflect whether PCO IE was included or not as part
         * of Delete PDP context request
         */
        OGS_TLV_CLEAR_DATA(&sess->gtp.ue_pco);
    }

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);
    return OGS_GTP1_CAUSE_REQUEST_ACCEPTED;
}

void smf_gn_handle_update_pdp_context_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp1_update_pdp_context_request_t *req)
{
    int rv;
    uint8_t cause_value = OGS_GTP1_CAUSE_REQUEST_ACCEPTED;

    ogs_gtp1_header_t h;
    ogs_pfcp_pdr_t *pdr = NULL;
    smf_bearer_t *bearer = NULL;
    smf_ue_t *smf_ue = NULL;
    ogs_gtp1_qos_profile_decoded_t *qos_pdec;
    uint8_t qci;

    ogs_debug("Update PDP Context Request");

    ogs_assert(xact);
    ogs_assert(req);

    if (req->nsapi.presence == 0) {
        ogs_error("No NSAPI");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->tunnel_endpoint_identifier_data_i.presence == 0) {
        ogs_error("No TEID");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->sgsn_address_for_control_plane.presence == 0) {
        ogs_error("SGSN Address for signalling");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->sgsn_address_for_user_traffic.presence == 0) {
        ogs_error("SGSN Address for user traffic");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->quality_of_service_profile.presence == 0) {
        ogs_error("No QoS Profile");
        cause_value = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
    }

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP1_CAUSE_NON_EXISTENT;
    }

    if (cause_value != OGS_GTP1_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp1_send_error_message(xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    bearer = smf_bearer_find_by_ebi(sess, req->nsapi.u8);
    if (!bearer) {
        ogs_warn("No bearer with id %u, taking default", req->nsapi.u8);
        bearer = smf_default_bearer_in_sess(sess);
        if (!bearer) {
            ogs_warn("No bearer");
            ogs_gtp1_send_error_message(xact, sess->sgw_s5c_teid,
                    OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE,
                    OGS_GTP1_CAUSE_NON_EXISTENT);
            return;
        }
    }

    /* Common Flags 7.7.48 */
    if (req->common_flags.presence) {
        sess->gtp.v1.common_flags = *(ogs_gtp1_common_flags_t*)req->common_flags.data;
    } else {
        /* Reset it to overwrite what was received during CreatePDPCtxReq time */
        sess->gtp.v1.common_flags = (ogs_gtp1_common_flags_t){0};
    }

    /* Control Plane(DL) : SGW-S5C */
    if (req->tunnel_endpoint_identifier_control_plane.presence) {
        sess->sgw_s5c_teid = req->tunnel_endpoint_identifier_control_plane.u32;
        rv = ogs_gtp1_gsn_addr_to_ip(req->sgsn_address_for_control_plane.data,
                                     req->sgsn_address_for_control_plane.len,
                                      &sess->sgw_s5c_ip);
        ogs_assert(rv == OGS_OK);
        ogs_debug("    Updated SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
                sess->sgw_s5c_teid, sess->smf_n4_teid);
    }

    /* User Plane(DL) : SGW-S5U */
    bearer->sgw_s5u_teid = req->tunnel_endpoint_identifier_data_i.u32;
    rv = ogs_gtp1_gsn_addr_to_ip(req->sgsn_address_for_user_traffic.data,
                                 req->sgsn_address_for_user_traffic.len,
                                 &bearer->sgw_s5u_ip);
    ogs_assert(rv == OGS_OK);
    ogs_debug("    Updated SGW_S5U_TEID[0x%x] PGW_S5U_TEID[0x%x]",
            bearer->sgw_s5u_teid, bearer->pgw_s5u_teid);


    /* Set Bearer QoS */
    OGS_TLV_STORE_DATA(&sess->gtp.v1.qos, &req->quality_of_service_profile);
    qos_pdec = &sess->gtp.v1.qos_pdec;
    rv = ogs_gtp1_parse_qos_profile(qos_pdec, &req->quality_of_service_profile);
    if(rv < 0) {
        ogs_gtp1_send_error_message(xact, sess->sgw_s5c_teid,
                OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE,
                OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT);
        return;
    }

    /* 3GPP TS 23.060 section 9.2.1A: "The QoS profiles of the PDP context and EPS bearer are mapped as specified in TS 23.401"
     * 3GPP TS 23.401 Annex E: "Mapping between EPS and Release 99 QoS parameters"
     */
    ogs_gtp1_qos_profile_to_qci(qos_pdec, &qci);
    sess->session.qos.index = qci;
    sess->session.qos.arp.priority_level = qos_pdec->qos_profile.arp; /* 3GPP TS 23.401 Annex E Table E.2 */
    sess->session.qos.arp.pre_emption_capability = 0; /* ignored as per 3GPP TS 23.401 Annex E */
    sess->session.qos.arp.pre_emption_vulnerability = 0; /* ignored as per 3GPP TS 23.401 Annex E */
    if (qos_pdec->data_octet6_to_13_present) {
        sess->session.ambr.downlink = qos_pdec->dec_mbr_kbps_dl * 1000;
        sess->session.ambr.uplink = qos_pdec->dec_mbr_kbps_ul * 1000;
    }

    /* APN-AMBR, 7.7.98 */
    if (req->apn_ambr.presence) {
        /* "The APN-AMBR IE shall be included as the authorized APN-AMBR if the
         * GGSN supports this IE and if the APN-AMBR IE has been included in the
         * corresponding request message." */
        sess->gtp.v1.peer_supports_apn_ambr = true;
        if (req->apn_ambr.len >= sizeof(ogs_gtp1_apn_ambr_t)) {
            ogs_gtp1_apn_ambr_t *ambr = req->apn_ambr.data;
            sess->session.ambr.uplink = be32toh(ambr->uplink) * 1000;
            sess->session.ambr.downlink = be32toh(ambr->downlink) * 1000;
        }
    }

    /* PCO */
    if (req->protocol_configuration_options.presence) {
        OGS_TLV_STORE_DATA(&sess->gtp.ue_pco,
                &req->protocol_configuration_options);
    } else {
        /*
         * Clear contents to reflect whether PCO IE was included or not as part
         * of Update PDP context request
         */
        OGS_TLV_CLEAR_DATA(&sess->gtp.ue_pco);
    }

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    /* Set bearer so it's accessible later when handling PFCP Session Modification Response */
    xact->data = OGS_UINT_TO_POINTER(bearer->id);

    /* Update remote TEID and GTP-U IP address on the UPF. UpdatePDPContextResp
     * will be sent when UPF answers back this request
     */
     ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        ogs_pfcp_far_t *far = pdr->far;
        ogs_assert(far);

        if (pdr->src_if != OGS_PFCP_INTERFACE_CORE ||
            far->dst_if != OGS_PFCP_INTERFACE_ACCESS)
            continue;
        if (!(far->apply_action & OGS_PFCP_APPLY_ACTION_FORW))
            continue;

        if (pdr->id == bearer->dl_pdr->id) {
            rv = ogs_pfcp_ip_to_outer_header_creation(&bearer->sgw_s5u_ip,
                                                &far->outer_header_creation,
                                                &far->outer_header_creation_len);
            ogs_assert(rv == OGS_OK);
            far->outer_header_creation.teid = bearer->sgw_s5u_teid;
        }
    }

    rv = smf_epc_pfcp_send_all_pdr_modification_request(sess, xact->id, NULL,
            OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE,
            OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            OGS_GTP1_CAUSE_REACTIACTION_REQUESTED);
    ogs_assert(rv == OGS_OK);

    /* TODO: TS 29.061: Upon reception of an UpdatePDPContextRequest from the
       SGSN, the GGSN may send an Accounting Request (Interim) to the Diameter
       server to update the necessary information related to this PDP context. */
    /* The P-GW need not wait for the Diameter Accounting Answer message from
       the Diameter server before sending the response for the triggering
       signalling message (e.g. Modify Bearer Response). The P-GW may delete the
       bearer if the Accounting Answer is not received from the Diameter
       server.*/
}
