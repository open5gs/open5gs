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

#include "context.h"
#include "gn-build.h"

#include "ipfw/ipfw2.h"

/* 3GPP TS 23.401 Annex E */
static void build_qos_profile_from_session(ogs_gtp1_qos_profile_decoded_t *qos_pdec,
        const smf_sess_t *sess, const smf_bearer_t *bearer)
{
    /* Initialize with defaults retrieved from MS/SGSN: */
    memcpy(qos_pdec, &sess->gtp.v1.qos_pdec, sizeof(*qos_pdec));

    qos_pdec->qos_profile.arp = sess->session.qos.arp.priority_level;

    /* 3GPP TS 23.107 "Delivery order should be set to 'no' for PDP Type =
     * 'IPv4' or 'IPv6'. The SGSN shall ensure that the appropriate value is set."
     * 3GPP TS 23.401 D.3.5 2b NOTE4: The GTP and PDCP sequence numbers are not
     * relevant as the network does not configure usage of "delivery order
     * required" [...] as described in clause "compatibility issues" (4.8.1) */
    qos_pdec->qos_profile.data.delivery_order = OGS_GTP1_DELIVERY_ORDER_NO;

    qos_pdec->qos_profile.data.delivery_erroneous_sdu = OGS_GTP1_DELIVERY_ERR_SDU_NO;

    /* Maximum SDU Size: If value is set to a valid value, reuse it: */
    if (sess->gtp.v1.qos_pdec.qos_profile.data.max_sdu_size >= 0x01 &&
        sess->gtp.v1.qos_pdec.qos_profile.data.max_sdu_size <= 0x99) {
        qos_pdec->qos_profile.data.max_sdu_size = sess->gtp.v1.qos_pdec.qos_profile.data.max_sdu_size;
    } else { /* Encode it as 1500, the maximum for IP 3GPP TS 23.107 Table 4, Note 4) */
        qos_pdec->qos_profile.data.max_sdu_size = 0x96;
    }

     /* 3GPP TS 23.401 Annex E table Table E.3 */
    /* Also take into account table 7 in 3GPP TS 23.107 9.1.2.2 */
    switch (sess->session.qos.index) { /* QCI */
    case 1:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_CONVERSATIONAL;
        qos_pdec->qos_profile.data.source_statistics_descriptor = OGS_GTP1_QOS_SRC_STATS_DESC_SPEECH;
        qos_pdec->dec_transfer_delay = 150;
        qos_pdec->qos_profile.data.delay_class = 1;
        break;
    case 2:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_CONVERSATIONAL;
        qos_pdec->qos_profile.data.source_statistics_descriptor = OGS_GTP1_QOS_SRC_STATS_DESC_UNKNOWN;
        qos_pdec->dec_transfer_delay = 150;
        qos_pdec->qos_profile.data.delay_class = 1;
        break;
    case 3:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_CONVERSATIONAL;
        qos_pdec->qos_profile.data.source_statistics_descriptor = OGS_GTP1_QOS_SRC_STATS_DESC_UNKNOWN;
        qos_pdec->dec_transfer_delay = 80;
        qos_pdec->qos_profile.data.delay_class = 1;
        break;
    case 4:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_STREAMING;
        qos_pdec->qos_profile.data.source_statistics_descriptor = OGS_GTP1_QOS_SRC_STATS_DESC_UNKNOWN;
        qos_pdec->qos_profile.data.sdu_error_ratio = 5; /* 10^-5*/
        qos_pdec->qos_profile.data.delay_class = 1;
        break;
    case 5:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_INTERACTIVE;
        qos_pdec->qos_profile.data.traffic_handling_priority = 1;
        qos_pdec->qos_profile.data.signalling_indication = 1;
        qos_pdec->qos_profile.data.delay_class = qos_pdec->qos_profile.data.traffic_handling_priority;
        break;
    case 6:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_INTERACTIVE;
        qos_pdec->qos_profile.data.traffic_handling_priority = 1;
        qos_pdec->qos_profile.data.delay_class = qos_pdec->qos_profile.data.traffic_handling_priority;
        break;
    case 7:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_INTERACTIVE;
        qos_pdec->qos_profile.data.traffic_handling_priority = 2;
        qos_pdec->qos_profile.data.delay_class = qos_pdec->qos_profile.data.traffic_handling_priority;
        break;
    case 8:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_INTERACTIVE;
        qos_pdec->qos_profile.data.traffic_handling_priority = 3;
        qos_pdec->qos_profile.data.delay_class = qos_pdec->qos_profile.data.traffic_handling_priority;
        break;
    case 9:
    default:
        qos_pdec->qos_profile.data.traffic_class = OGS_GTP1_QOS_TRAFFIC_CLASS_BACKGROUND;
        qos_pdec->qos_profile.data.delay_class = 4;
        break;
    }

    qos_pdec->data_octet6_to_13_present = true;
    qos_pdec->data_octet14_present = true;
    qos_pdec->dec_mbr_kbps_dl = sess->session.ambr.downlink / 1000;
    qos_pdec->dec_mbr_kbps_ul = sess->session.ambr.uplink / 1000;
    qos_pdec->dec_gbr_kbps_dl = bearer->qos.gbr.downlink / 1000;
    qos_pdec->dec_gbr_kbps_ul = bearer->qos.gbr.uplink / 1000;

    /* Don't upgrade values if Common Flags "Upgrade QoS Supported" is 0: */
    if (!sess->gtp.v1.common_flags.upgrade_qos_supported) {
        if (sess->gtp.v1.qos_pdec.dec_mbr_kbps_dl > 0)
            qos_pdec->dec_mbr_kbps_dl = ogs_min(qos_pdec->dec_mbr_kbps_dl,
                                            sess->gtp.v1.qos_pdec.dec_mbr_kbps_dl);
        if (sess->gtp.v1.qos_pdec.dec_mbr_kbps_ul > 0)
            qos_pdec->dec_mbr_kbps_ul = ogs_min(qos_pdec->dec_mbr_kbps_ul,
                                            sess->gtp.v1.qos_pdec.dec_mbr_kbps_ul);
        if (sess->gtp.v1.qos_pdec.dec_gbr_kbps_dl > 0)
            qos_pdec->dec_gbr_kbps_dl = ogs_min(qos_pdec->dec_gbr_kbps_dl,
                                            sess->gtp.v1.qos_pdec.dec_gbr_kbps_dl);
        if (sess->gtp.v1.qos_pdec.dec_gbr_kbps_ul > 0)
            qos_pdec->dec_gbr_kbps_ul = ogs_min(qos_pdec->dec_gbr_kbps_ul,
                                            sess->gtp.v1.qos_pdec.dec_gbr_kbps_ul);
    }
}

ogs_pkbuf_t *smf_gn_build_create_pdp_context_response(
        uint8_t type, smf_sess_t *sess)
{
    int rv;
    smf_bearer_t *bearer = NULL;

    ogs_gtp1_message_t gtp1_message;
    ogs_gtp1_create_pdp_context_response_t *rsp = NULL;

    ogs_gtp1_gsn_addr_t smf_gnc_gsnaddr, pgw_gnu_gsnaddr;
    ogs_gtp1_gsn_addr_t smf_gnc_altgsnaddr, pgw_gnu_altgsnaddr;
    int gsn_len, gsn_altlen;
    ogs_gtp1_apn_ambr_t ambr;
    ogs_ip_t ip_eua;
    ogs_eua_t eua;
    uint8_t eua_len = 0;
    uint8_t pco_buf[OGS_MAX_PCO_LEN];
    int16_t pco_len;
    ogs_gtp1_qos_profile_decoded_t qos_pdec;
    char qos_pdec_buf[OGS_GTP1_QOS_PROFILE_MAX_LEN];

    ogs_debug("[SMF] Create PDP Context Response");

    ogs_assert(sess);
    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);
    ogs_debug("    SGW_S5U_TEID[0x%x] PGW_S5U_TEID[0x%x]",
            bearer->sgw_s5u_teid, bearer->pgw_s5u_teid);

    rsp = &gtp1_message.create_pdp_context_response;
    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    /* Set Cause */
    rsp->cause.presence = 1;
    if (sess->ue_session_type == sess->session.session_type)
        rsp->cause.u8 = OGS_GTP1_CAUSE_REQUEST_ACCEPTED;
    else
        rsp->cause.u8 = OGS_GTP1_CAUSE_NEW_PDP_TYPE_DUE_TO_NETWORK_PREFERENCE;

    /* TODO: Reordering required: should be set based on Qos Profile
       delivery_order field, see TS 23.107 Table 7  */
   rsp->reordering_required.presence = 1;
   rsp->reordering_required.u8 = 0; /* No */

    /* TODO: Recovery */

    rsp->tunnel_endpoint_identifier_data_i.presence = 1;
    rsp->tunnel_endpoint_identifier_data_i.u32 = bearer->pgw_s5u_teid;
    rsp->tunnel_endpoint_identifier_control_plane.presence = 1;
    rsp->tunnel_endpoint_identifier_control_plane.u32 = sess->smf_n4_teid;

    /* TODO: NSAPI ? */
    rsp->nsapi.presence = 1;
    rsp->nsapi.u8 = sess->gtp.v1.nsapi;

    /* Charging ID */
    rsp->charging_id.presence = 1;
    rsp->charging_id.u32 = sess->charging.id;

    /* End User Address */
    rv = ogs_paa_to_ip(&sess->paa, &ip_eua);
    /* Clang scan-build SA: Value stored is not used: add check for rv error. */
    if (rv != OGS_OK) {
        ogs_error("ogs_paa_to_ip() failed");
        return NULL;
    }
    rv = ogs_gtp1_ip_to_eua(sess->session.session_type, &ip_eua, &eua,
            &eua_len);
    /* Clang scan-build SA: Value stored is not used: add check for rv error. */
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp1_ip_to_eua() failed");
        return NULL;
    }

    rsp->end_user_address.presence = 1;
    rsp->end_user_address.data = &eua;
    rsp->end_user_address.len = eua_len;

    /* Protocol Configuration Options (PCO) */
    if (sess->gtp.ue_pco.presence &&
            sess->gtp.ue_pco.len && sess->gtp.ue_pco.data) {
        pco_len = smf_pco_build(
                pco_buf, sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
        if (pco_len <= 0) {
            ogs_error("smf_pco_build() failed");
            ogs_log_hexdump(OGS_LOG_ERROR,
                    sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
            return NULL;
        }
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* GGSN Address for Control Plane */
    /* Alternative GGSN Address for Control Plane */
    if (ogs_gtp_self()->gtpc_addr && ogs_gtp_self()->gtpc_addr6) {
        if (sess->sgw_s5c_ip.ipv4) {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(ogs_gtp_self()->gtpc_addr, NULL,
                        &smf_gnc_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, ogs_gtp_self()->gtpc_addr6,
                        &smf_gnc_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        } else {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, ogs_gtp_self()->gtpc_addr6,
                        &smf_gnc_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(ogs_gtp_self()->gtpc_addr, NULL,
                        &smf_gnc_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        }
        rsp->alternative_ggsn_address_for_control_plane.presence = 1;
        rsp->alternative_ggsn_address_for_control_plane.data = &smf_gnc_altgsnaddr;
        rsp->alternative_ggsn_address_for_control_plane.len = gsn_altlen;
    } else {
        rv = ogs_gtp1_sockaddr_to_gsn_addr(
                    ogs_gtp_self()->gtpc_addr, ogs_gtp_self()->gtpc_addr6,
                    &smf_gnc_gsnaddr, &gsn_len);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
            return NULL;
        }
    }
    rsp->ggsn_address_for_control_plane.presence = 1;
    rsp->ggsn_address_for_control_plane.data = &smf_gnc_gsnaddr;
    rsp->ggsn_address_for_control_plane.len = gsn_len;

    /* GGSN Address for user traffic */
    /* Alternative GGSN Address for user traffic */
    if (bearer->pgw_s5u_addr && bearer->pgw_s5u_addr6) {
        if (bearer->sgw_s5u_ip.ipv4) {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(bearer->pgw_s5u_addr, NULL,
                        &pgw_gnu_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, bearer->pgw_s5u_addr6,
                        &pgw_gnu_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        } else {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, bearer->pgw_s5u_addr6,
                        &pgw_gnu_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(bearer->pgw_s5u_addr, NULL,
                        &pgw_gnu_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        }
        rsp->alternative_ggsn_address_for_user_traffic.presence = 1;
        rsp->alternative_ggsn_address_for_user_traffic.data = &pgw_gnu_altgsnaddr;
        rsp->alternative_ggsn_address_for_user_traffic.len = gsn_altlen;
    } else {
        rv = ogs_gtp1_sockaddr_to_gsn_addr(
                bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6,
                &pgw_gnu_gsnaddr, &gsn_len);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
            return NULL;
        }
    }
    rsp->ggsn_address_for_user_traffic.presence = 1;
    rsp->ggsn_address_for_user_traffic.data = &pgw_gnu_gsnaddr;
    rsp->ggsn_address_for_user_traffic.len = gsn_len;

    /* QoS Profile: if PCRF changes Bearer QoS, apply changes. */
    if (sess->gtp.create_session_response_bearer_qos == true) {
        build_qos_profile_from_session(&qos_pdec, sess, bearer);
        rsp->quality_of_service_profile.presence = 1;
        ogs_gtp1_build_qos_profile(&rsp->quality_of_service_profile,
               &qos_pdec, qos_pdec_buf, OGS_GTP1_QOS_PROFILE_MAX_LEN);
    } else {
        /* Copy over received QoS Profile from originating Request: */
        memcpy(&rsp->quality_of_service_profile, &sess->gtp.v1.qos,
               sizeof(rsp->quality_of_service_profile));
    }

    /* TODO: Charging Gateway Address */
    /* TODO: Alternative Charging Gateway Address */

    /* TODO: Common Flags */

    /* APN Restriction */
    rsp->apn_restriction.presence = 1;
    rsp->apn_restriction.u8 = OGS_GTP1_APN_NO_RESTRICTION;

    /* TODO: MS Info Change Reporting Action */
    /* TODO: Bearer Control Mode */
    /* TODO: Evolved Allocation/Retention Priority I */
    /* TODO: Extended Common Flag */
    /* TODO: CSG Information Reporting Action */

    /* APN-AMBR, TS 29.060 7.7.98
     * if PCRF changes APN-AMBR, this should be included. */
    if (sess->gtp.v1.peer_supports_apn_ambr == true &&
        sess->gtp.create_session_response_apn_ambr == true) {
        memset(&ambr, 0, sizeof(ogs_gtp1_apn_ambr_t));
        /* "the values [...] shall be rounded upwards" */
        ambr.uplink = htobe32((sess->session.ambr.uplink + 999)/ 1000);
        ambr.downlink = htobe32((sess->session.ambr.downlink + 999) / 1000);
        rsp->apn_ambr.presence = 1;
        rsp->apn_ambr.data = &ambr;
        rsp->apn_ambr.len = sizeof(ambr);
    }

    /* TODO: GGSN Back-Off Time */
    /* TODO: Extended Common Flags II */

    gtp1_message.h.type = type;
    return ogs_gtp1_build_msg(&gtp1_message);
}

ogs_pkbuf_t *smf_gn_build_delete_pdp_context_response(
        uint8_t type, smf_sess_t *sess)
{
    ogs_gtp1_message_t gtp1_message;
    ogs_gtp1_delete_pdp_context_response_t *rsp = NULL;

    uint8_t pco_buf[OGS_MAX_PCO_LEN];
    int16_t pco_len;

    ogs_debug("[SMF] Delete PDP Context Response");

    rsp = &gtp1_message.delete_pdp_context_response;
    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    /* Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_GTP1_CAUSE_REQUEST_ACCEPTED;

    /* PCO */
    if (sess->gtp.ue_pco.presence &&
            sess->gtp.ue_pco.len && sess->gtp.ue_pco.data) {
        pco_len = smf_pco_build(
                pco_buf, sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
        if (pco_len <= 0) {
            ogs_error("smf_pco_build() failed");
            ogs_log_hexdump(OGS_LOG_ERROR,
                    sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
            return NULL;
        }
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* Private Extension */

    /* build */
    gtp1_message.h.type = type;
    return ogs_gtp1_build_msg(&gtp1_message);
}

ogs_pkbuf_t *smf_gn_build_update_pdp_context_response(
        uint8_t type, smf_sess_t *sess, const smf_bearer_t *bearer)
{
    int rv;
    ogs_gtp1_message_t gtp1_message;
    ogs_gtp1_update_pdp_context_response_t *rsp = NULL;
    uint8_t pco_buf[OGS_MAX_PCO_LEN];
    int16_t pco_len;
    ogs_gtp1_gsn_addr_t smf_gnc_gsnaddr, pgw_gnu_gsnaddr;
    ogs_gtp1_gsn_addr_t smf_gnc_altgsnaddr, pgw_gnu_altgsnaddr;
    int gsn_len, gsn_altlen;
    ogs_gtp1_qos_profile_decoded_t qos_pdec;
    char qos_pdec_buf[OGS_GTP1_QOS_PROFILE_MAX_LEN];
    ogs_gtp1_apn_ambr_t ambr;

    ogs_debug("[SMF] Update PDP Context Response");

    ogs_assert(sess);
    ogs_assert(bearer);

    rsp = &gtp1_message.update_pdp_context_response;
    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    /* Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_GTP1_CAUSE_REQUEST_ACCEPTED;

    /* TODO: Recovery */

    /* Tunnel Endpoint Identifier Data I */
    rsp->tunnel_endpoint_identifier_data_i.presence = 1;
    rsp->tunnel_endpoint_identifier_data_i.u32 = bearer->pgw_s5u_teid;
    /* Tunnel Endpoint Identifier Control Plane */
    rsp->tunnel_endpoint_identifier_control_plane.presence = 1;
    rsp->tunnel_endpoint_identifier_control_plane.u32 = sess->smf_n4_teid;

    /* Charging ID */
    rsp->charging_id.presence = 1;
    rsp->charging_id.u32 = sess->charging.id;

    /* Protocol Configuration Options (PCO):
     * If the "No QoS negotiation" bit of the Common Flags IE in the Update PDP
     * Context Request message was set to 1, then the GGSN [...] shall not
     * include the Protocol Configuration Options (PCO) information element in
     * the message) */
    if (!sess->gtp.v1.common_flags.no_qos_negotiation &&
        sess->gtp.ue_pco.presence &&
        sess->gtp.ue_pco.len && sess->gtp.ue_pco.data) {
        pco_len = smf_pco_build(
                pco_buf, sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
        if (pco_len <= 0) {
            ogs_error("smf_pco_build() failed");
            ogs_log_hexdump(OGS_LOG_ERROR,
                    sess->gtp.ue_pco.data, sess->gtp.ue_pco.len);
            return NULL;
        }
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* GGSN Address for Control Plane */
    /* Alternative GGSN Address for Control Plane */
    if (ogs_gtp_self()->gtpc_addr && ogs_gtp_self()->gtpc_addr6) {
        if (sess->sgw_s5c_ip.ipv4) {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(ogs_gtp_self()->gtpc_addr, NULL,
                        &smf_gnc_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, ogs_gtp_self()->gtpc_addr6,
                        &smf_gnc_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        } else {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, ogs_gtp_self()->gtpc_addr6,
                        &smf_gnc_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(ogs_gtp_self()->gtpc_addr, NULL,
                        &smf_gnc_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        }
        rsp->alternative_ggsn_address_for_control_plane.presence = 1;
        rsp->alternative_ggsn_address_for_control_plane.data = &smf_gnc_altgsnaddr;
        rsp->alternative_ggsn_address_for_control_plane.len = gsn_altlen;
    } else {
        rv = ogs_gtp1_sockaddr_to_gsn_addr(
                    ogs_gtp_self()->gtpc_addr, ogs_gtp_self()->gtpc_addr6,
                    &smf_gnc_gsnaddr, &gsn_len);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
            return NULL;
        }
    }
    rsp->ggsn_address_for_control_plane.presence = 1;
    rsp->ggsn_address_for_control_plane.data = &smf_gnc_gsnaddr;
    rsp->ggsn_address_for_control_plane.len = gsn_len;

    /* GGSN Address for user traffic */
    /* Alternative GGSN Address for user traffic */
    if (bearer->pgw_s5u_addr && bearer->pgw_s5u_addr6) {
        if (bearer->sgw_s5u_ip.ipv4) {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(bearer->pgw_s5u_addr, NULL,
                        &pgw_gnu_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, bearer->pgw_s5u_addr6,
                        &pgw_gnu_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        } else {
            rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, bearer->pgw_s5u_addr6,
                        &pgw_gnu_gsnaddr, &gsn_len);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
            rv = ogs_gtp1_sockaddr_to_gsn_addr(bearer->pgw_s5u_addr, NULL,
                        &pgw_gnu_altgsnaddr, &gsn_altlen);
            if (rv != OGS_OK) {
                ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
                return NULL;
            }
        }
        rsp->alternative_ggsn_address_for_user_traffic.presence = 1;
        rsp->alternative_ggsn_address_for_user_traffic.data = &pgw_gnu_altgsnaddr;
        rsp->alternative_ggsn_address_for_user_traffic.len = gsn_altlen;
    } else {
        rv = ogs_gtp1_sockaddr_to_gsn_addr(
                bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6,
                &pgw_gnu_gsnaddr, &gsn_len);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
            return NULL;
        }
    }
    rsp->ggsn_address_for_user_traffic.presence = 1;
    rsp->ggsn_address_for_user_traffic.data = &pgw_gnu_gsnaddr;
    rsp->ggsn_address_for_user_traffic.len = gsn_len;

    /* QoS Profile: if SGSN supports QoS re-negotiation and PCRF changes Bearer
     * QoS, apply changes: */
    if (!sess->gtp.v1.common_flags.no_qos_negotiation &&
        sess->gtp.create_session_response_bearer_qos == true) {
        build_qos_profile_from_session(&qos_pdec, sess, bearer);
        rsp->quality_of_service_profile.presence = 1;
        ogs_gtp1_build_qos_profile(&rsp->quality_of_service_profile,
               &qos_pdec, qos_pdec_buf, OGS_GTP1_QOS_PROFILE_MAX_LEN);
    } else {
        /* Copy over received QoS Profile from originating Request: */
        memcpy(&rsp->quality_of_service_profile, &sess->gtp.v1.qos,
               sizeof(rsp->quality_of_service_profile));
    }

   /* TODO: Charging Gateway Address */
   /* TODO: Alternative Charging Gateway Address */
   /* TODO: Common Flags */

   /* APN Restriction */
   rsp->apn_restriction.presence = 1;
   rsp->apn_restriction.u8 = OGS_GTP1_APN_NO_RESTRICTION;

   /* TODO: Bearer Control Mode */
   /* TODO: MS Info Change Reporting Action */
   /* TODO: Evolved Allocation/Retention Priority I */
   /* TODO: CSG Information Reporting Action */

   /* APN-AMBR, TS 29.060 7.7.98
    * if PCRF changes APN-AMBR, this should be included. */
   if (sess->gtp.v1.peer_supports_apn_ambr == true &&
       sess->gtp.create_session_response_apn_ambr == true) {
       memset(&ambr, 0, sizeof(ogs_gtp1_apn_ambr_t));
       /* "the values [...] shall be rounded upwards" */
       ambr.uplink = htobe32((sess->session.ambr.uplink + 999)/ 1000);
       ambr.downlink = htobe32((sess->session.ambr.downlink + 999) / 1000);
       rsp->apn_ambr.presence = 1;
       rsp->apn_ambr.data = &ambr;
       rsp->apn_ambr.len = sizeof(ambr);
   }

    /* build */
    gtp1_message.h.type = type;
    return ogs_gtp1_build_msg(&gtp1_message);
}
