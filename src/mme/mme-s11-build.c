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

#include "mme-context.h"

#include "mme-s11-build.h"

ogs_pkbuf_t *mme_s11_build_create_session_request(
        uint8_t type, mme_sess_t *sess)
{
    int rv;
    ogs_session_t *session = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_bearer_t *bearer = NULL;
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_create_session_request_t *req = &gtp_message.create_session_request;

    ogs_gtp2_uli_t uli;
    char uli_buf[OGS_GTP2_MAX_ULI_LEN];
    ogs_gtp2_f_teid_t mme_s11_teid, pgw_s5c_teid;
    int len;
    ogs_gtp2_ambr_t ambr;
    ogs_gtp2_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP2_BEARER_QOS_LEN];
    ogs_gtp2_ue_timezone_t ue_timezone;
    struct timeval now;
    struct tm time_exp;
    char apn[OGS_MAX_APN_LEN+1];
	
    ogs_gtp2_indication_t indication;

    ogs_assert(sess);
    session = sess->session;
    ogs_assert(session);
    ogs_assert(session->name);
    bearer = mme_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_debug("Create Session Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);
    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    ogs_assert(mme_ue->imsi_len);
    req->imsi.presence = 1;
    req->imsi.data = mme_ue->imsi;
    req->imsi.len = mme_ue->imsi_len;

    if (mme_ue->imeisv_len) {
        req->me_identity.presence = 1;
        req->me_identity.data = mme_ue->imeisv;
        req->me_identity.len = mme_ue->imeisv_len;
    }

    if (mme_ue->msisdn_len) {
        req->msisdn.presence = 1;
        req->msisdn.data = mme_ue->msisdn;
        req->msisdn.len = mme_ue->msisdn_len;
    }

    memset(&uli, 0, sizeof(ogs_gtp2_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    ogs_nas_from_plmn_id(&uli.tai.nas_plmn_id, &mme_ue->tai.plmn_id);
    uli.tai.tac = mme_ue->tai.tac;
    ogs_nas_from_plmn_id(&uli.e_cgi.nas_plmn_id, &mme_ue->e_cgi.plmn_id);
    uli.e_cgi.cell_id = mme_ue->e_cgi.cell_id;
    req->user_location_information.presence = 1;
    ogs_gtp2_build_uli(&req->user_location_information, &uli, 
            uli_buf, OGS_GTP2_MAX_ULI_LEN);

    req->serving_network.presence = 1;
    req->serving_network.data = &uli.tai.nas_plmn_id;
    req->serving_network.len = sizeof(uli.tai.nas_plmn_id);

    req->rat_type.presence = 1;
    req->rat_type.u8 = OGS_GTP2_RAT_TYPE_EUTRAN;

    memset(&mme_s11_teid, 0, sizeof(ogs_gtp2_f_teid_t));
    mme_s11_teid.interface_type = OGS_GTP2_F_TEID_S11_MME_GTP_C;
    mme_s11_teid.teid = htobe32(mme_ue->mme_s11_teid);
    rv = ogs_gtp2_sockaddr_to_f_teid(
            ogs_gtp_self()->gtpc_addr, ogs_gtp_self()->gtpc_addr6,
            &mme_s11_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &mme_s11_teid;
    req->sender_f_teid_for_control_plane.len = len;

    memset(&pgw_s5c_teid, 0, sizeof(ogs_gtp2_f_teid_t));
    pgw_s5c_teid.interface_type = OGS_GTP2_F_TEID_S5_S8_PGW_GTP_C;
    if (session->smf_ip.ipv4 || session->smf_ip.ipv6) {
        pgw_s5c_teid.ipv4 = session->smf_ip.ipv4;
        pgw_s5c_teid.ipv6 = session->smf_ip.ipv6;
        if (pgw_s5c_teid.ipv4 && pgw_s5c_teid.ipv6) {
            pgw_s5c_teid.both.addr = session->smf_ip.addr;
            memcpy(pgw_s5c_teid.both.addr6, session->smf_ip.addr6,
                    sizeof session->smf_ip.addr6);
            req->pgw_s5_s8_address_for_control_plane_or_pmip.len =
                OGS_GTP2_F_TEID_IPV4V6_LEN;
        } else if (pgw_s5c_teid.ipv4) {
            pgw_s5c_teid.addr = session->smf_ip.addr;
            req->pgw_s5_s8_address_for_control_plane_or_pmip.len =
                OGS_GTP2_F_TEID_IPV4_LEN;
        } else if (pgw_s5c_teid.ipv6) {
            memcpy(pgw_s5c_teid.addr6, session->smf_ip.addr6,
                    sizeof session->smf_ip.addr6);
            req->pgw_s5_s8_address_for_control_plane_or_pmip.len =
                OGS_GTP2_F_TEID_IPV6_LEN;
        }
        req->pgw_s5_s8_address_for_control_plane_or_pmip.presence = 1;
        req->pgw_s5_s8_address_for_control_plane_or_pmip.data =
            &pgw_s5c_teid;
    } else {
        ogs_sockaddr_t *pgw_addr = NULL;
        ogs_sockaddr_t *pgw_addr6 = NULL;

        pgw_addr = mme_pgw_addr_find_by_apn(
                &mme_self()->pgw_list, AF_INET, session->name);
        pgw_addr6 = mme_pgw_addr_find_by_apn(
                &mme_self()->pgw_list, AF_INET6, session->name);
        if (!pgw_addr && !pgw_addr6) {
            pgw_addr = mme_self()->pgw_addr;
            pgw_addr6 = mme_self()->pgw_addr6;
        }

        rv = ogs_gtp2_sockaddr_to_f_teid(
                pgw_addr, pgw_addr6, &pgw_s5c_teid, &len);
        ogs_assert(rv == OGS_OK);
        req->pgw_s5_s8_address_for_control_plane_or_pmip.presence = 1;
        req->pgw_s5_s8_address_for_control_plane_or_pmip.data = &pgw_s5c_teid;
        req->pgw_s5_s8_address_for_control_plane_or_pmip.len = len;
    }

    req->access_point_name.presence = 1;
    req->access_point_name.len = ogs_fqdn_build(
            apn, session->name, strlen(session->name));
    req->access_point_name.data = apn;

    req->selection_mode.presence = 1;
    req->selection_mode.u8 = 
        OGS_GTP2_SELECTION_MODE_MS_OR_NETWORK_PROVIDED_APN;

    ogs_assert(sess->request_type.type == OGS_NAS_EPS_PDN_TYPE_IPV4 ||
            sess->request_type.type == OGS_NAS_EPS_PDN_TYPE_IPV6 ||
            sess->request_type.type == OGS_NAS_EPS_PDN_TYPE_IPV4V6);

    if (session->session_type == OGS_PDU_SESSION_TYPE_IPV4 ||
        session->session_type == OGS_PDU_SESSION_TYPE_IPV6 ||
        session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        req->pdn_type.u8 = (session->session_type & sess->request_type.type);
        if (req->pdn_type.u8 == 0) {
            ogs_fatal("Cannot derive PDN Type [UE:%d,HSS:%d]",
                sess->request_type.type, session->session_type);
            ogs_assert_if_reached();
        }
    } else {
        ogs_fatal("Invalid PDN_TYPE[%d]", session->session_type);
        ogs_assert_if_reached();
    }
    req->pdn_type.presence = 1;

    /* If we started with both addrs (IPV4V6) but the above code 
     * (pdn_type & sess->request_type) truncates us down to just one,
     * we need to change position of addresses in struct. */
    if (req->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV4 &&
        session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
	    uint32_t addr = session->paa.both.addr;
	    session->paa.addr = addr;
    }
    if (req->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV6 &&
        session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
	    uint8_t addr[16];
	    memcpy(&addr, session->paa.both.addr6, OGS_IPV6_LEN);
	    memcpy(session->paa.addr6, &addr, OGS_IPV6_LEN);
    }

    memset(&indication, 0, sizeof(ogs_gtp2_indication_t));
    req->indication_flags.presence = 1;
    req->indication_flags.data = &indication;
    req->indication_flags.len = sizeof(ogs_gtp2_indication_t);

    indication.change_reporting_support_indication = 1;
    indication.enb_change_reporting_support_indication = 1;

    if (req->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV4V6) {
	    indication.dual_address_bearer_flag = 1;
    }

    if (sess->request_type.value == OGS_NAS_EPS_REQUEST_TYPE_HANDOVER) {
	    indication.handover_indication = 1;
	    req->indication_flags.presence = 1;
    }

    session->paa.session_type = req->pdn_type.u8;
    req->pdn_address_allocation.data = &session->paa;
    if (req->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV4)
        req->pdn_address_allocation.len = OGS_PAA_IPV4_LEN;
    else if (req->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV6)
        req->pdn_address_allocation.len = OGS_PAA_IPV6_LEN;
    else if (req->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV4V6)
        req->pdn_address_allocation.len = OGS_PAA_IPV4V6_LEN;
    else
        ogs_assert_if_reached();
    req->pdn_address_allocation.presence = 1;

    req->maximum_apn_restriction.presence = 1;
    req->maximum_apn_restriction.u8 = OGS_GTP2_APN_NO_RESTRICTION;

    if (session->ambr.uplink || session->ambr.downlink) {
        /*
         * Ch 8.7. Aggregate Maximum Bit Rate(AMBR) in TS 29.274 V15.9.0
         *
         * AMBR is defined in clause 9.9.4.2 of 3GPP TS 24.301 [23],
         * but it shall be encoded as shown in Figure 8.7-1 as
         * Unsigned32 binary integer values in kbps (1000 bits per second).
         */
        memset(&ambr, 0, sizeof(ogs_gtp2_ambr_t));
        ambr.uplink = htobe32(session->ambr.uplink / 1000);
        ambr.downlink = htobe32(session->ambr.downlink / 1000);
        req->aggregate_maximum_bit_rate.presence = 1;
        req->aggregate_maximum_bit_rate.data = &ambr;
        req->aggregate_maximum_bit_rate.len = sizeof(ambr);
    }

    if (sess->ue_pco.length && sess->ue_pco.buffer) {
        req->protocol_configuration_options.presence = 1;
        req->protocol_configuration_options.data = sess->ue_pco.buffer;
        req->protocol_configuration_options.len = sess->ue_pco.length;
    }

    req->bearer_contexts_to_be_created.presence = 1;
    req->bearer_contexts_to_be_created.eps_bearer_id.presence = 1;
    req->bearer_contexts_to_be_created.eps_bearer_id.u8 = bearer->ebi;

    memset(&bearer_qos, 0, sizeof(bearer_qos));
    bearer_qos.qci = session->qos.index;
    bearer_qos.priority_level = session->qos.arp.priority_level;
    bearer_qos.pre_emption_capability = session->qos.arp.pre_emption_capability;
    bearer_qos.pre_emption_vulnerability =
        session->qos.arp.pre_emption_vulnerability;
    req->bearer_contexts_to_be_created.bearer_level_qos.presence = 1;
    ogs_gtp2_build_bearer_qos(
            &req->bearer_contexts_to_be_created.bearer_level_qos,
            &bearer_qos, bearer_qos_buf, GTP2_BEARER_QOS_LEN);

    /* UE Time Zone */
    memset(&ue_timezone, 0, sizeof(ue_timezone));
    ogs_gettimeofday(&now);
    ogs_localtime(now.tv_sec, &time_exp);
    if (time_exp.tm_gmtoff >= 0) {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD(time_exp.tm_gmtoff / 900);
    } else {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD((-time_exp.tm_gmtoff) / 900);
        ue_timezone.timezone |= 0x08;
    }
    /* quarters of an hour */
    ue_timezone.daylight_saving_time = 
        OGS_GTP2_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME;
    req->ue_time_zone.presence = 1;
    req->ue_time_zone.data = &ue_timezone;
    req->ue_time_zone.len = sizeof(ue_timezone);

    req->charging_characteristics.presence = 1;
    req->charging_characteristics.data = (uint8_t *)"\x54\x00";
    req->charging_characteristics.len = 2;

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_modify_bearer_request(
        uint8_t type, mme_bearer_t *bearer, int uli_presence)
{
    int rv;
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_modify_bearer_request_t *req = &gtp_message.modify_bearer_request;

    ogs_gtp2_f_teid_t enb_s1u_teid;
    int len;
    ogs_gtp2_uli_t uli;
    char uli_buf[OGS_GTP2_MAX_ULI_LEN];

    ogs_gtp2_indication_t indication;

    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_debug("Modifty Bearer Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        bearer->enb_s1u_teid, bearer->sgw_s1u_teid);

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    if (sess->request_type.value == OGS_NAS_EPS_REQUEST_TYPE_HANDOVER) {
	    memset(&indication, 0, sizeof(ogs_gtp2_indication_t));
	    indication.handover_indication = 1;
	    req->indication_flags.presence = 1;
	    req->indication_flags.data = &indication;
	    req->indication_flags.len = sizeof(ogs_gtp2_indication_t);
    }

    /* Bearer Context : EBI */
    req->bearer_contexts_to_be_modified.presence = 1;
    req->bearer_contexts_to_be_modified.eps_bearer_id.presence = 1;
    req->bearer_contexts_to_be_modified.eps_bearer_id.u8 = bearer->ebi;

    /* Data Plane(DL) : ENB-S1U */
    memset(&enb_s1u_teid, 0, sizeof(ogs_gtp2_f_teid_t));
    enb_s1u_teid.interface_type = OGS_GTP2_F_TEID_S1_U_ENODEB_GTP_U;
    enb_s1u_teid.teid = htobe32(bearer->enb_s1u_teid);
    rv = ogs_gtp2_ip_to_f_teid(&bearer->enb_s1u_ip, &enb_s1u_teid, &len);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);
    req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.presence = 1;
    req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.data = &enb_s1u_teid;
    req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.len = len;

    if (uli_presence) {
        /* User Location Information(ULI) */
        memset(&uli, 0, sizeof(ogs_gtp2_uli_t));
        uli.flags.e_cgi = 1;
        uli.flags.tai = 1;
        ogs_nas_from_plmn_id(&uli.tai.nas_plmn_id, &mme_ue->tai.plmn_id);
        uli.tai.tac = mme_ue->tai.tac;
        ogs_nas_from_plmn_id(&uli.e_cgi.nas_plmn_id, &mme_ue->e_cgi.plmn_id);
        uli.e_cgi.cell_id = mme_ue->e_cgi.cell_id;
        req->user_location_information.presence = 1;
        ogs_gtp2_build_uli(&req->user_location_information, &uli, 
                uli_buf, OGS_GTP2_MAX_ULI_LEN);
    }

    /*
     * 7.2.7 Modify Bearer Request
     *
     * Table 7.2.7-1: Information Elements in a Modify Bearer Request
     *
     * Delay Downlink Packet Nofication Request : Delay Value
     *
     * This IE shall be sent on the S11 interface for a UE triggered
     * Packet Notification Service Request and UE initiated Connection
     * Resume Request procedures. It shall contain the delay the SGW
     * shall apply between receiving downlink data and sending Downlink
     * Data Notification for all UEs served by that MME
     * (see clause 5.3.4.2 of 3GPP TS 23.401 [3]).
     */
    if (mme_ue->nas_eps.type == MME_EPS_TYPE_SERVICE_REQUEST) {
        req->delay_downlink_packet_notification_request.presence = 1;
        req->delay_downlink_packet_notification_request.u8 = 0;
    }

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_delete_session_request(
        uint8_t type, mme_sess_t *sess)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_delete_session_request_t *req = &gtp_message.delete_session_request;

    ogs_gtp2_uli_t uli;
    char uli_buf[OGS_GTP2_MAX_ULI_LEN];
    ogs_gtp2_indication_t indication;

    mme_bearer_t *bearer = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);
    bearer = mme_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    ogs_debug("Delete Session Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    req->linked_eps_bearer_id.presence = 1;
    req->linked_eps_bearer_id.u8 = bearer->ebi;

    /* User Location Information(ULI) */
    memset(&uli, 0, sizeof(ogs_gtp2_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    ogs_nas_from_plmn_id(&uli.tai.nas_plmn_id, &mme_ue->tai.plmn_id);
    uli.tai.tac = mme_ue->tai.tac;
    ogs_nas_from_plmn_id(&uli.e_cgi.nas_plmn_id, &mme_ue->e_cgi.plmn_id);
    uli.e_cgi.cell_id = mme_ue->e_cgi.cell_id;
    req->user_location_information.presence = 1;
    ogs_gtp2_build_uli(&req->user_location_information, &uli,
            uli_buf, OGS_GTP2_MAX_ULI_LEN);

    memset(&indication, 0, sizeof(ogs_gtp2_indication_t));
    indication.operation_indication = 1;
    req->indication_flags.presence = 1;
    req->indication_flags.data = &indication;
    req->indication_flags.len = sizeof(ogs_gtp2_indication_t);

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_create_bearer_response(
        uint8_t type, mme_bearer_t *bearer, uint8_t cause_value)
{
    int rv;
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_create_bearer_response_t *rsp = &gtp_message.create_bearer_response;

    ogs_gtp2_cause_t cause;
    ogs_gtp2_f_teid_t enb_s1u_teid, sgw_s1u_teid;
    int len;
    ogs_gtp2_uli_t uli;
    char uli_buf[OGS_GTP2_MAX_ULI_LEN];
    ogs_gtp2_ue_timezone_t ue_timezone;
    struct timeval now;
    struct tm time_exp;

    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    ogs_debug("Create Bearer Response");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    /* Set Cause */
    memset(&cause, 0, sizeof(cause));
    cause.value = cause_value;
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        /* Bearer Context : EBI */
        rsp->bearer_contexts.presence = 1;
        rsp->bearer_contexts.eps_bearer_id.presence = 1;
        rsp->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

        /* Data Plane(DL) : ENB-S1U */
        memset(&enb_s1u_teid, 0, sizeof(ogs_gtp2_f_teid_t));
        enb_s1u_teid.interface_type = OGS_GTP2_F_TEID_S1_U_ENODEB_GTP_U;
        enb_s1u_teid.teid = htobe32(bearer->enb_s1u_teid);
        rv = ogs_gtp2_ip_to_f_teid(&bearer->enb_s1u_ip, &enb_s1u_teid, &len);
        ogs_expect_or_return_val(rv == OGS_OK, NULL);
        rsp->bearer_contexts.s1_u_enodeb_f_teid.presence = 1;
        rsp->bearer_contexts.s1_u_enodeb_f_teid.data = &enb_s1u_teid;
        rsp->bearer_contexts.s1_u_enodeb_f_teid.len = len;

        /* Data Plane(UL) : SGW-S1U */
        memset(&sgw_s1u_teid, 0, sizeof(ogs_gtp2_f_teid_t));
        sgw_s1u_teid.interface_type = OGS_GTP2_F_TEID_S1_U_SGW_GTP_U;
        sgw_s1u_teid.teid = htobe32(bearer->sgw_s1u_teid);
        rv = ogs_gtp2_ip_to_f_teid(&bearer->sgw_s1u_ip, &sgw_s1u_teid, &len);
        ogs_expect_or_return_val(rv == OGS_OK, NULL);
        rsp->bearer_contexts.s4_u_sgsn_f_teid.presence = 1;
        rsp->bearer_contexts.s4_u_sgsn_f_teid.data = &sgw_s1u_teid;
        rsp->bearer_contexts.s4_u_sgsn_f_teid.len = OGS_GTP2_F_TEID_IPV4_LEN;

        /* Bearer Context : Cause */
        rsp->bearer_contexts.cause.presence = 1;
        rsp->bearer_contexts.cause.len = sizeof(cause);
        rsp->bearer_contexts.cause.data = &cause;
    }

    /* User Location Information(ULI) */
    memset(&uli, 0, sizeof(ogs_gtp2_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    ogs_nas_from_plmn_id(&uli.tai.nas_plmn_id, &mme_ue->tai.plmn_id);
    uli.tai.tac = mme_ue->tai.tac;
    ogs_nas_from_plmn_id(&uli.e_cgi.nas_plmn_id, &mme_ue->e_cgi.plmn_id);
    uli.e_cgi.cell_id = mme_ue->e_cgi.cell_id;
    rsp->user_location_information.presence = 1;
    ogs_gtp2_build_uli(&rsp->user_location_information, &uli, 
            uli_buf, OGS_GTP2_MAX_ULI_LEN);

    /* UE Time Zone */
    memset(&ue_timezone, 0, sizeof(ue_timezone));
    ogs_gettimeofday(&now);
    ogs_localtime(now.tv_sec, &time_exp);
    if (time_exp.tm_gmtoff >= 0) {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD(time_exp.tm_gmtoff / 900);
    } else {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD((-time_exp.tm_gmtoff) / 900);
        ue_timezone.timezone |= 0x08;
    }
    ue_timezone.daylight_saving_time = 
        OGS_GTP2_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME;
    rsp->ue_time_zone.presence = 1;
    rsp->ue_time_zone.data = &ue_timezone;
    rsp->ue_time_zone.len = sizeof(ue_timezone);

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_update_bearer_response(
        uint8_t type, mme_bearer_t *bearer, uint8_t cause_value)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_update_bearer_response_t *rsp = &gtp_message.update_bearer_response;

    ogs_gtp2_cause_t cause;
    ogs_gtp2_uli_t uli;
    char uli_buf[OGS_GTP2_MAX_ULI_LEN];
    ogs_gtp2_ue_timezone_t ue_timezone;
    struct timeval now;
    struct tm time_exp;

    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    ogs_debug("Update Bearer Response");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    /* Set Cause */
    memset(&cause, 0, sizeof(cause));
    cause.value = cause_value;
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        /* Bearer Context : EBI */
        rsp->bearer_contexts.presence = 1;
        rsp->bearer_contexts.eps_bearer_id.presence = 1;
        rsp->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

        /* Bearer Context : Cause */
        rsp->bearer_contexts.cause.presence = 1;
        rsp->bearer_contexts.cause.len = sizeof(cause);
        rsp->bearer_contexts.cause.data = &cause;
    }

    /* User Location Information(ULI) */
    memset(&uli, 0, sizeof(ogs_gtp2_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    ogs_nas_from_plmn_id(&uli.tai.nas_plmn_id, &mme_ue->tai.plmn_id);
    uli.tai.tac = mme_ue->tai.tac;
    ogs_nas_from_plmn_id(&uli.e_cgi.nas_plmn_id, &mme_ue->e_cgi.plmn_id);
    uli.e_cgi.cell_id = mme_ue->e_cgi.cell_id;
    rsp->user_location_information.presence = 1;
    ogs_gtp2_build_uli(&rsp->user_location_information, &uli, 
            uli_buf, OGS_GTP2_MAX_ULI_LEN);

    /* UE Time Zone */
    memset(&ue_timezone, 0, sizeof(ue_timezone));
    ogs_gettimeofday(&now);
    ogs_localtime(now.tv_sec, &time_exp);
    if (time_exp.tm_gmtoff >= 0) {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD(time_exp.tm_gmtoff / 900);
    } else {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD((-time_exp.tm_gmtoff) / 900);
        ue_timezone.timezone |= 0x08;
    }
    ue_timezone.daylight_saving_time = 
        OGS_GTP2_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME;
    rsp->ue_time_zone.presence = 1;
    rsp->ue_time_zone.data = &ue_timezone;
    rsp->ue_time_zone.len = sizeof(ue_timezone);

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_delete_bearer_response(
        uint8_t type, mme_bearer_t *bearer, uint8_t cause_value)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_delete_bearer_response_t *rsp = &gtp_message.delete_bearer_response;

    ogs_gtp2_cause_t cause;
    ogs_gtp2_uli_t uli;
    char uli_buf[OGS_GTP2_MAX_ULI_LEN];
    ogs_gtp2_ue_timezone_t ue_timezone;
    struct timeval now;
    struct tm time_exp;

    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    ogs_debug("Delete Bearer Response");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    /* Set Cause */
    memset(&cause, 0, sizeof(cause));
    cause.value = cause_value;
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        mme_bearer_t *linked_bearer = mme_linked_bearer(bearer);
        ogs_assert(linked_bearer);

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
            rsp->linked_eps_bearer_id.presence = 1;
            rsp->linked_eps_bearer_id.u8 = bearer->ebi;
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

            /* Bearer Context : EBI */
            rsp->bearer_contexts.presence = 1;
            rsp->bearer_contexts.eps_bearer_id.presence = 1;
            rsp->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

            /* Bearer Context : Cause */
            rsp->bearer_contexts.cause.presence = 1;
            rsp->bearer_contexts.cause.len = sizeof(cause);
            rsp->bearer_contexts.cause.data = &cause;
        }
    }

    /* User Location Information(ULI) */
    memset(&uli, 0, sizeof(ogs_gtp2_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    ogs_nas_from_plmn_id(&uli.tai.nas_plmn_id, &mme_ue->tai.plmn_id);
    uli.tai.tac = mme_ue->tai.tac;
    ogs_nas_from_plmn_id(&uli.e_cgi.nas_plmn_id, &mme_ue->e_cgi.plmn_id);
    uli.e_cgi.cell_id = mme_ue->e_cgi.cell_id;
    rsp->user_location_information.presence = 1;
    ogs_gtp2_build_uli(&rsp->user_location_information, &uli, 
            uli_buf, OGS_GTP2_MAX_ULI_LEN);

    /* UE Time Zone */
    memset(&ue_timezone, 0, sizeof(ue_timezone));
    ogs_gettimeofday(&now);
    ogs_localtime(now.tv_sec, &time_exp);
    if (time_exp.tm_gmtoff >= 0) {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD(time_exp.tm_gmtoff / 900);
    } else {
        ue_timezone.timezone = OGS_GTP2_TIME_TO_BCD((-time_exp.tm_gmtoff) / 900);
        ue_timezone.timezone |= 0x08;
    }
    ue_timezone.daylight_saving_time = 
        OGS_GTP2_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME;
    rsp->ue_time_zone.presence = 1;
    rsp->ue_time_zone.data = &ue_timezone;
    rsp->ue_time_zone.len = sizeof(ue_timezone);

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_release_access_bearers_request(uint8_t type)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_release_access_bearers_request_t *req = 
        &gtp_message.release_access_bearers_request;

    ogs_debug("Release Access Bearers Request");
    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    req->originating_node.presence = 1;
    req->originating_node.u8 = OGS_GTP2_NODE_TYPE_MME;

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_downlink_data_notification_ack(
        uint8_t type, uint8_t cause_value)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_downlink_data_notification_acknowledge_t *ack = 
        &gtp_message.downlink_data_notification_acknowledge;

    ogs_gtp2_cause_t cause;

    ogs_debug("Downlink Data Notification Ackknowledge");

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.value = cause_value;

    ack->cause.presence = 1;
    ack->cause.data = &cause;
    ack->cause.len = sizeof(cause);

    ack->data_notification_delay.presence = 1;
    ack->data_notification_delay.u8 = 0;

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_create_indirect_data_forwarding_tunnel_request(
        uint8_t type, mme_ue_t *mme_ue)
{
    int rv;
    int i;
    
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_create_indirect_data_forwarding_tunnel_request_t *req =
        &gtp_message.create_indirect_data_forwarding_tunnel_request;
    
    ogs_gtp2_f_teid_t dl_teid[OGS_GTP2_MAX_INDIRECT_TUNNEL];
    ogs_gtp2_f_teid_t ul_teid[OGS_GTP2_MAX_INDIRECT_TUNNEL];
    int len;

    ogs_assert(mme_ue);

    ogs_debug("Create Indirect Data Forwarding Tunnel Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    i = 0;
    sess = mme_sess_first(mme_ue);
    while (sess != NULL) {
        bearer = mme_bearer_first(sess);
        while (bearer != NULL) {
            if (MME_HAVE_ENB_DL_INDIRECT_TUNNEL(bearer)) {
                memset(&dl_teid[i], 0, sizeof(ogs_gtp2_f_teid_t));
                dl_teid[i].interface_type =
                    OGS_GTP2_F_TEID_ENODEB_GTP_U_FOR_DL_DATA_FORWARDING;
                dl_teid[i].teid = htobe32(bearer->enb_dl_teid);
                rv = ogs_gtp2_ip_to_f_teid(
                        &bearer->enb_dl_ip, &dl_teid[i], &len);
                ogs_expect_or_return_val(rv == OGS_OK, NULL);
                req->bearer_contexts[i].s1_u_enodeb_f_teid.presence = 1;
                req->bearer_contexts[i].s1_u_enodeb_f_teid.data = &dl_teid[i];
                req->bearer_contexts[i].s1_u_enodeb_f_teid.len = len;
            }

            if (MME_HAVE_ENB_UL_INDIRECT_TUNNEL(bearer)) {
                memset(&ul_teid[i], 0, sizeof(ogs_gtp2_f_teid_t));
                ul_teid[i].interface_type =
                    OGS_GTP2_F_TEID_ENODEB_GTP_U_FOR_UL_DATA_FORWARDING;
                ul_teid[i].teid = htobe32(bearer->enb_ul_teid);
                rv = ogs_gtp2_ip_to_f_teid(
                        &bearer->enb_ul_ip, &ul_teid[i], &len);
                ogs_expect_or_return_val(rv == OGS_OK, NULL);
                req->bearer_contexts[i].s12_rnc_f_teid.presence = 1;
                req->bearer_contexts[i].s12_rnc_f_teid.data = &ul_teid[i];
                req->bearer_contexts[i].s12_rnc_f_teid.len = len;
            }

            if (MME_HAVE_ENB_DL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_ENB_UL_INDIRECT_TUNNEL(bearer)) {
                req->bearer_contexts[i].presence = 1;
                req->bearer_contexts[i].eps_bearer_id.presence = 1;
                req->bearer_contexts[i].eps_bearer_id.u8 = bearer->ebi;
                i++;
            }

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *mme_s11_build_bearer_resource_command(
        uint8_t type, mme_bearer_t *bearer, ogs_nas_eps_message_t *nas_message)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_bearer_resource_command_t *cmd =
        &gtp_message.bearer_resource_command;
    ogs_nas_eps_bearer_resource_allocation_request_t *allocation = NULL;
    ogs_nas_eps_bearer_resource_modification_request_t *modification = NULL;

    ogs_nas_eps_quality_of_service_t *qos = NULL;
    ogs_nas_traffic_flow_aggregate_description_t *tad = NULL;

    ogs_gtp2_flow_qos_t flow_qos;
    char flow_qos_buf[GTP2_FLOW_QOS_LEN];

    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *linked_bearer = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_debug("Bearer Resource Command");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    ogs_assert(nas_message);
    switch (nas_message->esm.h.message_type) {
    case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST:
        allocation = &nas_message->esm.bearer_resource_allocation_request;
        qos = &allocation->required_traffic_flow_qos;
        tad = &allocation->traffic_flow_aggregate;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST:
        modification = &nas_message->esm.bearer_resource_modification_request;
        if (modification->presencemask &
            OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT) {
            qos = &modification->required_traffic_flow_qos;
        }
        tad = &modification->traffic_flow_aggregate;
        break;
    default:
        ogs_error("Invalid NAS ESM Type[%d]", nas_message->esm.h.message_type);
        return NULL;
    }

    linked_bearer = mme_linked_bearer(bearer);
    ogs_assert(linked_bearer);

    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    /* Linked Bearer Context : EBI */
    cmd->linked_eps_bearer_id.presence = 1;
    cmd->linked_eps_bearer_id.u8 = linked_bearer->ebi;

    /* Procedure Transaction ID(PTI) */
    cmd->procedure_transaction_id.presence = 1;
    cmd->procedure_transaction_id.u8 = sess->pti;

    /* Flow Quality of Service(QoS) */
    if (qos) {
        memset(&flow_qos, 0, sizeof(flow_qos));
        flow_qos.qci = qos->qci;

        /* Octet 4
         *
         * In UE to network direction:
         * 00000000 Subscribed maximum bit rate
         *
         * In network to UE direction:
         * 00000000 Reserved
         */
        flow_qos.ul_mbr = qos->ul_mbr == 0 ? bearer->qos.mbr.uplink :
            ogs_gtp2_qos_to_bps(
                qos->ul_mbr, qos->ul_mbr_extended, qos->ul_mbr_extended2);
        flow_qos.dl_mbr = qos->dl_mbr == 0 ? bearer->qos.mbr.downlink :
            ogs_gtp2_qos_to_bps(
                qos->dl_mbr, qos->dl_mbr_extended, qos->dl_mbr_extended2);
        flow_qos.ul_gbr = qos->ul_gbr == 0 ? bearer->qos.gbr.uplink :
            ogs_gtp2_qos_to_bps(
                qos->ul_gbr, qos->ul_gbr_extended, qos->ul_gbr_extended2);
        flow_qos.dl_gbr = qos->dl_gbr == 0 ? bearer->qos.gbr.downlink :
            ogs_gtp2_qos_to_bps(
                qos->dl_gbr, qos->dl_gbr_extended, qos->dl_gbr_extended2);

        ogs_gtp2_build_flow_qos(
                &cmd->flow_quality_of_service,
                &flow_qos, flow_qos_buf, GTP2_FLOW_QOS_LEN);
        cmd->flow_quality_of_service.presence = 1;
    }

    /* Traffic Aggregate Description(TAD) */
    cmd->traffic_aggregate_description.presence = 1;
    cmd->traffic_aggregate_description.data = tad->buffer;
    cmd->traffic_aggregate_description.len = tad->length;

    switch (nas_message->esm.h.message_type) {
    case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST:
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST:
        /* Bearer Context : EBI */
        cmd->eps_bearer_id.presence = 1;
        cmd->eps_bearer_id.u8 = bearer->ebi;
        break;
    default:
        ogs_error("Invalid NAS ESM Type[%d]", nas_message->esm.h.message_type);
        return NULL;
    }

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}
