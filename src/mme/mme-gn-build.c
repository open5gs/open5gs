/*
 * Copyright (C) 2023 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "mme-gn-build.h"

static int sess_fill_mm_context_decoded(mme_sess_t *sess, ogs_gtp1_mm_context_decoded_t *mmctx_dec)
{
    mme_ue_t *mme_ue = sess->mme_ue;
    mme_bearer_t *bearer = NULL;
    *mmctx_dec = (ogs_gtp1_mm_context_decoded_t) {
        .gupii = 1, /* Integrity Protection not required */
        .ugipai = 1, /* Ignore "Used GPRS integrity protection algorithm" field" */
        .ksi = mme_ue->nas_eps.ksi,
        .sec_mode = OGS_GTP1_SEC_MODE_UMTS_KEY_AND_QUINTUPLETS,
        .num_vectors = 0, /* TODO: figure out how to fill the quintuplets */
        .drx_param = {
            .split_pg_cycle_code = 0, /* equivalent to no DRX */
            .cn_drx_cycle_len_coeff = 0,
            .split_on_ccch = 0,
            .non_drx_timer = 0,
        },
        .nrsrna = 0,
    };

    //TODO: derive cK Ki from mme_ue->kasme
    ogs_kdf_ck_ik_idle_mobility(mme_ue->ul_count.i32, mme_ue->kasme, &mmctx_dec->ck[0], &mmctx_dec->ik[0]);

    mmctx_dec->imeisv_len = sizeof(mme_ue->nas_mobile_identity_imeisv);
    memcpy(&mmctx_dec->imeisv[0], &mme_ue->nas_mobile_identity_imeisv, sizeof(mme_ue->nas_mobile_identity_imeisv));

    mmctx_dec->ms_network_capability_len = mme_ue->ms_network_capability.length;
    memcpy(&mmctx_dec->ms_network_capability[0], ((uint8_t*)&mme_ue->ms_network_capability)+1, sizeof(mme_ue->ms_network_capability) - 1);

    ogs_list_for_each(&sess->bearer_list, bearer) {

        /* FIXME: only 1 PDP Context supported in the message so far. */
        break;
    }

    return OGS_OK;
}

/* 3GPP TS 23.401 Annex E */
static void build_qos_profile_from_session(ogs_gtp1_qos_profile_decoded_t *qos_pdec,
        const mme_sess_t *sess, const mme_bearer_t *bearer)
{
    const mme_ue_t *mme_ue = sess->mme_ue;
    const ogs_session_t *session = sess->session;
    /* FIXME: Initialize with defaults: */
    memset(qos_pdec, 0, sizeof(*qos_pdec));

    qos_pdec->qos_profile.arp = session->qos.arp.priority_level;

     /* 3GPP TS 23.401 Annex E table Table E.3 */
    /* Also take into account table 7 in 3GPP TS 23.107 9.1.2.2 */
    switch (session->qos.index) { /* QCI */
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
    qos_pdec->dec_mbr_kbps_dl = mme_ue->ambr.downlink / 1000;
    qos_pdec->dec_mbr_kbps_ul = mme_ue->ambr.uplink / 1000;
    qos_pdec->dec_gbr_kbps_dl = bearer->qos.gbr.downlink / 1000;
    qos_pdec->dec_gbr_kbps_ul = bearer->qos.gbr.uplink / 1000;
}

static int sess_fill_pdp_context_decoded(mme_sess_t *sess, ogs_gtp1_pdp_context_decoded_t *pdpctx_dec)
{
    mme_bearer_t *bearer = NULL;

    *pdpctx_dec = (ogs_gtp1_pdp_context_decoded_t){
        .ea = OGS_GTP1_PDPCTX_EXT_EUA_NO,
        .vaa = OGS_GTP1_PDPCTX_VLPMN_ADDR_ALLOWED_YES,
        .asi = OGS_GTP1_PDPCTX_ACTIVITY_STATUS_IND_NO,
        .order = OGS_GTP1_PDPCTX_REORDERING_REQUIRED_NO,
        /* 3GPP TS 23.401 Annex D3.5.5 2b.:
         * "The GTP equence numbers received from the old 3G-SGSN are only relevant if
         * delivery order is required for the PDP context (QoS profile)."
         * NOTE 4: "The GTP and PDCP sequence numbers are not relevant" */
        .snd = 0,
        .snu = 0,
        .send_npdu_nr = 0,
        .receive_npdu_nr = 0,
        .ul_teic = sess->pgw_s5c_teid,
        .pdp_type_org = OGS_PDP_EUA_ORG_IETF,
        .pdp_type_num = {sess->session->session_type, },
        .pdp_address = {sess->session->ue_ip, },
        .ggsn_address_c = sess->pgw_s5c_ip,
        .trans_id = sess->pti,
    };

    ogs_cpystrn(pdpctx_dec->apn, sess->session->name, sizeof(pdpctx_dec->apn));

    ogs_list_for_each(&sess->bearer_list, bearer) {
        pdpctx_dec->nsapi  = bearer->ebi;
        pdpctx_dec->sapi = 3; /* FIXME. Using 3 = default for now. Maybe use 0 = UNASSIGNED ?*/
        build_qos_profile_from_session(&pdpctx_dec->qos_sub, sess, bearer);
        //FIXME: sort out where to get each one:
        memcpy(&pdpctx_dec->qos_req, &pdpctx_dec->qos_sub, sizeof(pdpctx_dec->qos_sub));
        memcpy(&pdpctx_dec->qos_neg, &pdpctx_dec->qos_sub, sizeof(pdpctx_dec->qos_sub));
        pdpctx_dec->ul_teid = bearer->pgw_s5u_teid;
        pdpctx_dec->pdp_ctx_id = 0; /* FIXME. */
        pdpctx_dec->ggsn_address_u = bearer->pgw_s5u_ip;

        /* FIXME: only 1 PDP Context supported in the message so far. */
        break;
    }

    return OGS_OK;
}

/* 3GPP TS 29.060 7.5.4 SGSN Context Response */
ogs_pkbuf_t *mme_gn_build_sgsn_context_response(
                mme_ue_t *mme_ue, uint8_t cause)
{
    ogs_gtp1_message_t gtp1_message;
    ogs_gtp1_sgsn_context_response_t *rsp = NULL;
    mme_sess_t *sess = NULL;
    ogs_gtp1_gsn_addr_t mme_gnc_gsnaddr;
    int gsn_len;
    int rv;
    ogs_gtp1_mm_context_decoded_t mmctx_dec;
    uint8_t mmctx_dec_buf[512];
    ogs_gtp1_pdp_context_decoded_t pdpctx_dec;
    uint8_t pdpctx_dec_buf[1024];

    ogs_debug("[Gn] build SGSN Context Response");

    rsp = &gtp1_message.sgsn_context_response;
    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));
    gtp1_message.h.type = OGS_GTP1_SGSN_CONTEXT_RESPONSE_TYPE;

    /* 3GPP TS 29.060 7.7.1 Cause, Mandatory */
    rsp->cause.presence = 1;
    rsp->cause.u8 = cause;

    /* 3GPP TS 29.060 7.7.2 IMSI, Conditional */
    rsp->imsi.presence = !!mme_ue;
    if (rsp->imsi.presence) {
        rsp->imsi.data = mme_ue->imsi;
        rsp->imsi.len = mme_ue->imsi_len;
    }

    if (cause != OGS_GTP1_CAUSE_REQUEST_ACCEPTED)
        goto build_ret;

    ogs_assert(mme_ue);

    rsp->tunnel_endpoint_identifier_control_plane.presence = 1;
    rsp->tunnel_endpoint_identifier_control_plane.u32 = mme_ue->gn.mme_gn_teid;

    ogs_list_for_each(&mme_ue->sess_list, sess) {
        if (!MME_HAVE_SGW_S1U_PATH(sess))
            continue;

        /* 7.7.28 MM Context */
        rv = sess_fill_mm_context_decoded(sess, &mmctx_dec);
        if (rv != OGS_OK) {
            ogs_error("sess_fill_mm_context_decoded() failed");
            return NULL;
        }
        rsp->mm_context.presence = 1;
        rv = ogs_gtp1_build_mm_context(&rsp->mm_context, &mmctx_dec,
                                &mmctx_dec_buf[0], sizeof(mmctx_dec_buf));
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_build_mm_context() failed");
            return NULL;
        }

        /* 7.7.29 PDP Context */
        rv = sess_fill_pdp_context_decoded(sess, &pdpctx_dec);
        if (rv != OGS_OK) {
            ogs_error("sess_fill_pdp_context_decoded() failed");
            return NULL;
        }
        rsp->pdp_context.presence = 1;
        rv = ogs_gtp1_build_pdp_context(&rsp->pdp_context, &pdpctx_dec,
                                        &pdpctx_dec_buf[0], sizeof(pdpctx_dec_buf));
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_build_pdp_context() failed");
            return NULL;
        }

        /* FIXME: right now we only support encoding 1 context in the message. */
        break;
    }

    /* SGSN Address for Control Plane */
    if (ogs_gtp_self()->gtpc_addr6 &&
        (mme_ue->gn.sgsn_gn_ip.ipv6 || mme_ue->gn.sgsn_gn_ip_alt.ipv6)) {
        rv = ogs_gtp1_sockaddr_to_gsn_addr(NULL, ogs_gtp_self()->gtpc_addr6,
                &mme_gnc_gsnaddr, &gsn_len);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
            return NULL;
        }
    } else {
        rv = ogs_gtp1_sockaddr_to_gsn_addr(ogs_gtp_self()->gtpc_addr, NULL,
                &mme_gnc_gsnaddr, &gsn_len);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp1_sockaddr_to_gsn_addr() failed");
            return NULL;
        }
    }
    rsp->sgsn_address_for_control_plane.presence = 1;
    rsp->sgsn_address_for_control_plane.data = &mme_gnc_gsnaddr;
    rsp->sgsn_address_for_control_plane.len = gsn_len;


build_ret:
    return ogs_gtp1_build_msg(&gtp1_message);
}

/* 3GPP TS 29.060 7.5.14.1 RAN Information Relay */
ogs_pkbuf_t *mme_gn_build_ran_information_relay(
                uint8_t type, const uint8_t *buf, size_t len,
                const ogs_nas_rai_t *rai, uint16_t cell_id)
{
    ogs_gtp1_message_t gtp1_message;
    ogs_gtp1_ran_information_relay_t *req = NULL;
    uint8_t rt_addr[sizeof(*rai) + sizeof(cell_id)];

    ogs_debug("[Gn] build RAN Information Relay");


    req = &gtp1_message.ran_information_relay;
    memset(&gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    /* 3GPP TS 29.060 7.7.43 RAN Transparent Container, Mandatory */
    req->ran_transparent_container.presence = 1;
    req->ran_transparent_container.data = (void*)buf;
    req->ran_transparent_container.len = len;

    /* 3GPP TS 29.060 7.7.57 RAN RIM Routing Address, Optional */
    req->rim_routing_address.presence = !!rai;
    if (req->rim_routing_address.presence) {
        memcpy(&rt_addr[0], rai, sizeof(*rai));
        memcpy(&rt_addr[sizeof(*rai)], &cell_id, sizeof(cell_id));
        req->rim_routing_address.data = &rt_addr[0];
        req->rim_routing_address.len = sizeof(rt_addr);
    }

    /* 3GPP TS 29.060 7.7.77 RIM Routing Address Discriminator, Optional */
    /* 3GPP TS 48.018 11.3.70 RIM Routing Information IE: */
    uint8_t rim_routing_address_disc = 0; /* "A Cell Identifier is used to identify a GERAN cell."" */
    req->rim_routing_address_discriminator.presence = req->rim_routing_address.presence;
    req->rim_routing_address_discriminator.data = &rim_routing_address_disc;
    req->rim_routing_address_discriminator.len = 1;

    gtp1_message.h.type = type;
    return ogs_gtp1_build_msg(&gtp1_message);
}
