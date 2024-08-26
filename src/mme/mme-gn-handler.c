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

#include "mme-event.h"
#include "mme-sm.h"
#include "mme-context.h"
#include "mme-timer.h"

#include "mme-gtp-path.h"
#include "mme-fd-path.h"
#include "mme-path.h"

#include "mme-gn-build.h"
#include "mme-gn-handler.h"

#include "s1ap-path.h"
#include "nas-path.h"

void mme_gn_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[Gn] Receiving Echo Request");
    /* FIXME : Implementing recovery counter correctly */
    ogs_gtp1_send_echo_response(xact, 0);
}

void mme_gn_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_response_t *req)
{
    /* Not Implemented */
}

static int decode_global_enb_id(S1AP_Global_ENB_ID_t *glob_enb_id, const uint8_t *buf, size_t buf_len)
{
    asn_dec_rval_t dec_ret = {0};

    memset(glob_enb_id, 0, sizeof(S1AP_Global_ENB_ID_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1AP_Global_ENB_ID, (void **)&glob_enb_id,
            buf, buf_len, 0, 0);

    if (dec_ret.code != RC_OK) {
        ogs_warn("Failed to decode ASN-PDU [code:%d,consumed:%d]",
                dec_ret.code, (int)dec_ret.consumed);
        return OGS_ERROR;
    }

    if (ogs_log_get_domain_level(OGS_LOG_DOMAIN) >= OGS_LOG_TRACE)
        asn_fprint(stdout, &asn_DEF_S1AP_Global_ENB_ID, glob_enb_id);

    return OGS_OK;
}

/* 3GPP TS 23.003 2.8.2.2 Mapping RAI and P-TMSI from GUTI (in the MME) */
void guti_to_rai_ptmsi(const ogs_nas_eps_guti_t *nas_guti, ogs_nas_rai_t *rai, mme_p_tmsi_t *ptmsi)
{
    rai->lai.nas_plmn_id = nas_guti->nas_plmn_id;
    rai->lai.lac = nas_guti->mme_gid;
    rai->rac = (nas_guti->m_tmsi >> 16) & 0xff;
    if (ptmsi)
        *ptmsi = 0xC0000000 |
                (nas_guti->m_tmsi & 0x3f000000) |
                (nas_guti->mme_code & 0x0ff) << 16 |
                (nas_guti->m_tmsi & 0x0000ffff);
}

/* 3GPP TS 23.003 2.8.2.1 Mapping GUTI from RAI, P-TMSI and P-TMSI signature (in the MME) */
static void rai_ptmsi_to_guti(const ogs_nas_rai_t *rai, mme_p_tmsi_t ptmsi, uint32_t ptmsi_sig, ogs_nas_eps_guti_t *nas_guti)
{
    nas_guti->nas_plmn_id = rai->lai.nas_plmn_id;
    nas_guti->mme_gid = rai->lai.lac;
    nas_guti->mme_code = rai->rac;
    nas_guti->m_tmsi = 0xC0000000 | (ptmsi & 0x3f000000) | (ptmsi_sig & 0x00ff0000) | (ptmsi & 0x0000ffff);
}

/* TS 29.060 7.5.3 SGSN Context Request */
void mme_gn_handle_sgsn_context_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_sgsn_context_request_t *req)
{
    ogs_nas_eps_guti_t nas_guti;
    ogs_plmn_id_t plmn_id;
    ogs_nas_rai_t rai;
    mme_ue_t *mme_ue = NULL;
    int rv;

    ogs_debug("[Gn] Rx SGSN Context Request");

    if (!req->routeing_area_identity.presence) {
        ogs_warn("[Gn] Rx SGSN Context Request with no RAI!");
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_MANDATORY_IE_MISSING, xact);
        return;
    }

    if (req->routeing_area_identity.len != sizeof(rai)) {
        ogs_warn("[Gn] Rx SGSN Context Request RAI wrong size %u vs exp %zu!",
                 req->routeing_area_identity.len, sizeof(rai));
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT, xact);
        return;
    }
    if (!req->tunnel_endpoint_identifier_control_plane.presence) {
        ogs_warn("[Gn] Rx SGSN Context Request with no Tunnel Endpoint Identifier Control Plane!");
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_MANDATORY_IE_MISSING, xact);
        return;
    }
    if (!req->sgsn_address_for_control_plane.presence) {
        ogs_warn("[Gn] Rx SGSN Context Request with no SGSN Address for Control Plane!");
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_MANDATORY_IE_MISSING, xact);
        return;
    }
    if (!req->imsi.presence &&
        !req->temporary_logical_link_identifier.presence &&
        !req->packet_tmsi.presence) {
        ogs_warn("[Gn] Rx SGSN Context Request with no IMSI/TLLI/P-TMSI!");
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_MANDATORY_IE_MISSING, xact);
        return;
    }

    if (req->ms_validated.presence &&
        (req->ms_validated.u8 & 0x01) /* 1=> "Yes" */
        && !req->imsi.presence) {
        ogs_warn("[Gn] Rx SGSN Context Request with 'MS Validated' but no IMSI!");
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_MANDATORY_IE_MISSING, xact);
        return;
    }

    memcpy(&rai, req->routeing_area_identity.data, sizeof(rai));
    rai.lai.lac = be16toh(rai.lai.lac);
    ogs_nas_to_plmn_id(&plmn_id, &rai.lai.nas_plmn_id);
    ogs_debug("    RAI[MCC:%u MNC:%u LAC:%u RAC:%u]",
             ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id),
             rai.lai.lac, rai.rac);

    if (req->imsi.presence) {
        char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
        ogs_buffer_to_bcd(req->imsi.data, req->imsi.len, imsi_bcd);
        ogs_debug("    IMSI[%s]", imsi_bcd);
        mme_ue = mme_ue_find_by_imsi(req->imsi.data, req->imsi.len);
        if (!mme_ue)
            ogs_warn("[Gn] Rx SGSN Context Request: Unknown UE with IMSI[%s]", imsi_bcd);
    } else if (req->packet_tmsi.presence) { /* P-TMSI */
        if (!req->p_tmsi_signature.presence) {
            ogs_warn("[Gn] Rx SGSN Context Request with 'P-TMSI' but no P-TMSI Signature! Assuming value 0.");
            req->p_tmsi_signature.u24 = 0;
        }
        rai_ptmsi_to_guti(&rai, req->packet_tmsi.u32, req->p_tmsi_signature.u24, &nas_guti);
        ogs_debug("    PTMSI[0x%08x] PTMSI_SIG[0x%06x] -> GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                  req->packet_tmsi.u32, req->p_tmsi_signature.u24,
                  nas_guti.mme_gid, nas_guti.mme_code, nas_guti.m_tmsi);
        mme_ue = mme_ue_find_by_guti(&nas_guti);
        if (!mme_ue)
            ogs_warn("[Gn] Rx SGSN Context Request: Unknown UE with RAI[MCC:%u MNC:%u LAC:%u RAC:%u] PTMSI[0x%08x] PTMSI_SIG[0x%06x] -> GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                     ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id), rai.lai.lac, rai.rac,
                     req->packet_tmsi.u32, req->p_tmsi_signature.u24,
                     nas_guti.mme_gid, nas_guti.mme_code, nas_guti.m_tmsi);
    } else if (req->temporary_logical_link_identifier.presence) {
        if (!req->p_tmsi_signature.presence) {
            ogs_warn("[Gn] Rx SGSN Context Request with 'TLLI' but no P-TMSI Signature! Assuming value 0.");
            req->p_tmsi_signature.u24 = 0;
        }
        /* TS 29.060 7.5.3 "The TLLI/P-TMSI and RAI is a foreign TLLI/P-TMSI and an RAI in the old SGSN."
         * A foregin TLLI is "tlli = (p_tmsi & 0x3fffffff) | 0x80000000", and since we only use 0x3fffffff
         * bits of P-TMSI to derive the GUTI, it's totally fine passing the TLLI as P-TMSI. */
        rai_ptmsi_to_guti(&rai, req->temporary_logical_link_identifier.u32, req->p_tmsi_signature.u24, &nas_guti);
        ogs_debug("    TLLI[0x%08x] PTMSI_SIG[0x%06x] -> GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                  req->temporary_logical_link_identifier.u32, req->p_tmsi_signature.u24,
                  nas_guti.mme_gid, nas_guti.mme_code, nas_guti.m_tmsi);
        mme_ue = mme_ue_find_by_guti(&nas_guti);
        if (!mme_ue)
            ogs_warn("[Gn] Rx SGSN Context Request: Unknown UE with RAI[MCC:%u MNC:%u LAC:%u RAC:%u] TLLI[0x%08x] PTMSI_SIG[0x%06x] -> GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                     ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id), rai.lai.lac, rai.rac,
                     req->temporary_logical_link_identifier.u32, req->p_tmsi_signature.u24,
                     nas_guti.mme_gid, nas_guti.mme_code, nas_guti.m_tmsi);
    }

    if (!mme_ue) {
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_IMSI_IMEI_NOT_KNOWN, xact);
        return;
    }

    mme_ue->gn.sgsn_gn_teid = req->tunnel_endpoint_identifier_control_plane.u32;

    rv = ogs_gtp1_gsn_addr_to_ip(req->sgsn_address_for_control_plane.data,
                                 req->sgsn_address_for_control_plane.len,
                                 &mme_ue->gn.sgsn_gn_ip);
    ogs_assert(rv == OGS_OK);

   if (req->alternative_sgsn_address_for_control_plane.presence) {
        rv = ogs_gtp1_gsn_addr_to_ip(req->alternative_sgsn_address_for_control_plane.data,
                                     req->alternative_sgsn_address_for_control_plane.len,
                                    &mme_ue->gn.sgsn_gn_ip_alt);
        ogs_assert(rv == OGS_OK);
   }

    /* 3GPP TS 23.401 Annex D.3.5 "Routing Area Update":
     * Step 2. "If the old P-TMSI Signature was valid or if the new SGSN indicates that it has authenticated the MS,
     * the old SGSN starts a timer."
     */
    ogs_timer_start(mme_ue->gn.t_gn_holding, mme_timer_cfg(MME_TIMER_GN_HOLDING)->duration);

    mme_gtp1_send_sgsn_context_response(mme_ue, OGS_GTP1_CAUSE_REQUEST_ACCEPTED, xact);
}

static mme_sess_t *mme_ue_session_from_gtp1_pdp_ctx(mme_ue_t *mme_ue, const ogs_gtp1_pdp_context_decoded_t *gtp1_pdp_ctx)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    const ogs_gtp1_qos_profile_decoded_t *qos_pdec = &gtp1_pdp_ctx->qos_sub;
    uint8_t pti = 1; /* Default PTI : 1 */
    uint8_t qci = 0;
    ogs_session_t *ogs_sess;

    ogs_sess = mme_session_find_by_apn(mme_ue, gtp1_pdp_ctx->apn);
    if (!ogs_sess) {
        ogs_assert(mme_ue->num_of_session < OGS_MAX_NUM_OF_SESS);
        ogs_sess = &mme_ue->session[mme_ue->num_of_session];
        mme_ue->num_of_session++;
        ogs_sess->name = ogs_strdup(gtp1_pdp_ctx->apn);
    }
    ogs_sess->smf_ip = gtp1_pdp_ctx->ggsn_address_c;
    ogs_sess->context_identifier = gtp1_pdp_ctx->pdp_ctx_id;

    /* 3GPP TS 23.060 section 9.2.1A: "The QoS profiles of the PDP context and EPS bearer are mapped as specified in TS 23.401"
     * 3GPP TS 23.401 Annex E: "Mapping between EPS and Release 99 QoS parameters"
     */
    ogs_gtp1_qos_profile_to_qci(qos_pdec, &qci);
    ogs_sess->qos.index = qci;
    ogs_sess->qos.arp.priority_level = qos_pdec->qos_profile.arp; /* 3GPP TS 23.401 Annex E Table E.2 */
    ogs_sess->qos.arp.pre_emption_capability = 0; /* operator policy, hardcoded, 3GPP TS 23.401 Annex E */
    ogs_sess->qos.arp.pre_emption_vulnerability = 1; /* operator policy, hardcoded, 3GPP TS 23.401 Annex E */
    if (qos_pdec->data_octet6_to_13_present) {
        ogs_sess->ambr.downlink = qos_pdec->dec_mbr_kbps_dl * 1000;
        ogs_sess->ambr.uplink = qos_pdec->dec_mbr_kbps_ul * 1000;
    }

    sess = mme_sess_find_by_pti(mme_ue, pti);
    if (!sess) {
        sess = mme_sess_add(mme_ue, pti);
        ogs_assert(sess);
    }

    sess->session = ogs_sess;
    sess->pgw_s5c_teid = gtp1_pdp_ctx->ul_teic;
    sess->pgw_s5c_ip = gtp1_pdp_ctx->ggsn_address_c;
    ogs_ip_to_paa(&gtp1_pdp_ctx->pdp_address[0], &sess->paa);
    switch (gtp1_pdp_ctx->pdp_type_num[0]) {
    case OGS_PDU_SESSION_TYPE_IPV4:
        sess->ue_request_type.type = OGS_NAS_EPS_PDN_TYPE_IPV4;
        break;
    case OGS_PDU_SESSION_TYPE_IPV6:
        sess->ue_request_type.type = OGS_NAS_EPS_PDN_TYPE_IPV6;
        break;
    case OGS_PDU_SESSION_TYPE_IPV4V6:
        sess->ue_request_type.type = OGS_NAS_EPS_PDN_TYPE_IPV4V6;
        break;
    }
    sess->ue_request_type.value = OGS_NAS_EPS_REQUEST_TYPE_INITIAL;

    /* NSAPI = EBI: 3GPP TS 23.401 5.2.1, TS 23.060 14.4A */
    bearer = mme_bearer_find_by_sess_ebi(sess, gtp1_pdp_ctx->nsapi);
    if (!bearer) {
        bearer = mme_default_bearer_in_sess(sess);
        if (!bearer) {
            bearer = mme_bearer_add(sess);
            ogs_assert(bearer);
        }
    }
    bearer->pgw_s5u_teid = gtp1_pdp_ctx->ul_teid;
    bearer->pgw_s5u_ip = gtp1_pdp_ctx->ggsn_address_u;
    /* Send invalid Remote Address and TEID since it makes no sense that SGW
     * forwards GTPUv2 traffic to SGSN: */
    bearer->enb_s1u_ip.ipv4 = 1;
    bearer->enb_s1u_ip.addr = 0;
    bearer->enb_s1u_teid = 0xffffffff;
    bearer->qos.index = ogs_sess->qos.index;
    bearer->qos.arp.priority_level = ogs_sess->qos.arp.priority_level;
    bearer->qos.arp.pre_emption_capability = ogs_sess->qos.arp.pre_emption_capability;
    bearer->qos.arp.pre_emption_vulnerability = ogs_sess->qos.arp.pre_emption_vulnerability;

    return sess;
}

/* TS 29.060 7.5.4 SGSN Context Response */
int mme_gn_handle_sgsn_context_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue, ogs_gtp1_sgsn_context_response_t *resp)
{
    int rv;
    int gtp1_cause, emm_cause = OGS_NAS_EMM_CAUSE_NETWORK_FAILURE;
    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    ogs_gtp1_mm_context_decoded_t gtp1_mm_ctx;
    ogs_gtp1_pdp_context_decoded_t gtp1_pdp_ctx;
    enb_ue_t *enb_ue = NULL;
    mme_sess_t *sess = NULL;
    uint8_t ret_cause = OGS_GTP1_CAUSE_REQUEST_ACCEPTED;

    ogs_debug("[Gn] Rx SGSN Context Response");

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return OGS_GTP1_CAUSE_SYSTEM_FAILURE;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return OGS_GTP1_CAUSE_IMSI_IMEI_NOT_KNOWN;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);

    switch (resp->cause.u8) {
     case OGS_GTP1_CAUSE_REQUEST_ACCEPTED:
        break; /* Handle below */
    case OGS_GTP1_CAUSE_TGT_ACC_RESTRICTED_SUBSCRIBER:
        emm_cause = OGS_NAS_EMM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_AUTHORIZED_IN_THIS_PLMN;
        break;
    case OGS_GTP1_CAUSE_IMSI_IMEI_NOT_KNOWN:
    case OGS_GTP1_CAUSE_SYSTEM_FAILURE:
    case OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT:
    case OGS_GTP1_CAUSE_MANDATORY_IE_MISSING:
    case OGS_GTP1_CAUSE_OPTIONAL_IE_INCORRECT:
    case OGS_GTP1_CAUSE_INVALID_MESSAGE_FORMAT:
    case OGS_GTP1_CAUSE_P_TMSI_SIGNATURE_MISMATCH:
    default:
        emm_cause = OGS_NAS_EMM_CAUSE_NETWORK_FAILURE;
        break;
    }

    if (resp->cause.u8 != OGS_GTP1_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("[Gn] Rx SGSN Context Response cause:%u", resp->cause.u8);
        rv = nas_eps_send_tau_reject(enb_ue, mme_ue, emm_cause);
        return OGS_GTP1_CAUSE_SYSTEM_FAILURE;
    }

    if (!resp->imsi.presence) {
        ogs_error("[Gn] Rx SGSN Context Response with no IMSI!");
        gtp1_cause = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
        goto nack_and_reject;
    }

    ogs_buffer_to_bcd(resp->imsi.data, resp->imsi.len, imsi_bcd);
    ogs_info("    IMSI[%s]", imsi_bcd);
    mme_ue_set_imsi(mme_ue, imsi_bcd);

    if (!resp->tunnel_endpoint_identifier_control_plane.presence) {
        ogs_error("[Gn] Rx SGSN Context Response with no Tunnel Endpoint Identifier Control Plane!");
        gtp1_cause = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
        goto nack_and_reject;
    }

    mme_ue->gn.sgsn_gn_teid = resp->tunnel_endpoint_identifier_control_plane.u32;

    if (!resp->mm_context.presence) {
        ogs_error("[Gn] Rx SGSN Context Response with no MM Context!");
        gtp1_cause = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
        goto nack_and_reject;
    }

    if (!resp->pdp_context.presence) {
        ogs_error("[Gn] Rx SGSN Context Response with no PDP Context!");
        gtp1_cause = OGS_GTP1_CAUSE_MANDATORY_IE_MISSING;
        goto nack_and_reject;
    }

    rv = ogs_gtp1_parse_mm_context(&gtp1_mm_ctx, &resp->mm_context);
    if (rv != OGS_OK) {
        ogs_error("[Gn] Rx SGSN Context Response: Failed parsing MM Context");
        gtp1_cause = OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT;
        goto nack_and_reject;
    }

    rv = ogs_gtp1_parse_pdp_context(&gtp1_pdp_ctx, &resp->pdp_context);
    if (rv != OGS_OK) {
        ogs_error("[Gn] Rx SGSN Context Response: Failed parsing PDP Context");
        gtp1_cause = OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT;
        goto nack_and_reject;
    }

    if (gtp1_mm_ctx.imeisv_len > 0) {
        memcpy(&mme_ue->nas_mobile_identity_imeisv, &gtp1_mm_ctx.imeisv[0],
               ogs_min(gtp1_mm_ctx.imeisv_len, sizeof(mme_ue->nas_mobile_identity_imeisv)));
    } else {
        /* 3GPP TS 23.401 D3.6: we need to request IMEI to the UE over EUTRAN */
        ret_cause = OGS_GTP1_CAUSE_REQUEST_IMEI;
    }

    mme_ue->ms_network_capability.length = gtp1_mm_ctx.ms_network_capability_len;
    if (gtp1_mm_ctx.ms_network_capability_len > 0)
        memcpy(((uint8_t*)&mme_ue->ms_network_capability)+1, &gtp1_mm_ctx.ms_network_capability[0],
             ogs_min(gtp1_mm_ctx.ms_network_capability_len, sizeof(mme_ue->ms_network_capability) - 1));
    /* TODO: how to fill first byte of mme_ue->ms_network_capability ? */

    mme_ue->nas_eps.ksi = gtp1_mm_ctx.ksi;
    /* 3GPP TS 33.401 A.10, A.11: */
    mme_ue->noncemme = ogs_random32();
    /* 3GPP TS 33.401 7.2.6.2 Establishment of keys for cryptographically protected radio bearers: */
    /* See also 3GPP TS 33.401 9.1.2 */
    ogs_kdf_kasme_idle_mobility(gtp1_mm_ctx.ck, gtp1_mm_ctx.ik, mme_ue->nonceue, mme_ue->noncemme, mme_ue->kasme);
    ogs_kdf_kenb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
    ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
    mme_ue->nhcc = 1;

    if (gtp1_mm_ctx.num_vectors > 0) {
        mme_ue->xres_len = gtp1_mm_ctx.auth_quintuplets[0].xres_len;
        memcpy(mme_ue->xres, gtp1_mm_ctx.auth_quintuplets[0].xres, mme_ue->xres_len);
        memcpy(mme_ue->rand, gtp1_mm_ctx.auth_quintuplets[0].rand, OGS_RAND_LEN);
        memcpy(mme_ue->autn, gtp1_mm_ctx.auth_quintuplets[0].autn, OGS_AUTN_LEN);
    }

    sess = mme_ue_session_from_gtp1_pdp_ctx(mme_ue, &gtp1_pdp_ctx);
    if (!sess) {
        gtp1_cause = OGS_GTP1_CAUSE_SYSTEM_FAILURE;
        goto nack_and_reject;
    }
    /* Store sess id to be able to retrieve it later on from xact: */
    xact->data = OGS_UINT_TO_POINTER(sess->pti);

    return ret_cause;

nack_and_reject:
    rv = mme_gtp1_send_sgsn_context_ack(mme_ue, gtp1_cause, xact);
    ogs_info("[%s] TAU Reject [OGS_NAS_EMM_CAUSE:%d]", mme_ue->imsi_bcd, emm_cause);
    rv = nas_eps_send_tau_reject(enb_ue, mme_ue, emm_cause);
    return OGS_GTP1_CAUSE_SYSTEM_FAILURE;
}

/* TS 29.060 7.5.5 SGSN Context Acknowledge */
void mme_gn_handle_sgsn_context_acknowledge(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue, ogs_gtp1_sgsn_context_acknowledge_t *req)
{
    int rv;
    enb_ue_t *enb_ue = NULL;

    ogs_debug("[Gn] Rx SGSN Context Acknowledge");

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);

    /* 3GPP TS 23.060 6.9.1.2.2 Step 4), 3GPP TS 23.401 D.3.5 Step 4)
    * The new SGSN sends an SGSN Context Acknowledge message to the old SGSN. The old MME (which is the old
    * SGSN from the new SGSN's point of view) marks in its context that the information in the GWs and the HSS are
    * invalid. This triggers the GWs, and the HSS to be updated if the UE initiates a Tracking Area Update procedure
    * back to the old MME before completing the ongoing Routing Area Update procedure. If the security functions do
    * not authenticate the MS correctly, then the routing area update shall be rejected, and the new SGSN shall send a
    * reject indication to the old SGSN. The old MME shall continue as if the SGSN Context Request was never
    * received.
    * "NOTE 6: The new SGSN's operation is unmodified compared to pre-Rel-8. The old MME/S-GW (old SGSN from
    * the new SGSN's point of view) does not forward any data towards the new SGSN." "*/

    if (req->cause.u8 != OGS_GTP1_CAUSE_REQUEST_ACCEPTED) {
        ogs_timer_stop(mme_ue->gn.t_gn_holding);
        return;
    }

    /* 3GPP TS 23.060 6.9.1.2.2 Step 13)
    * "If the old MME has an S1-MME association for the UE, the source MME sends a S1-U Release Command to the
    * source eNodeB when receiving the SGSN Context Acknowledge message from the new SGSN. The RRC
    * connection is released by the source eNodeB. The source eNodeB confirms the release of the RRC connection
    * and of the S1-U connection by sending a S1-U Release Complete message to the source MME."
    */
    if (enb_ue) {
        rv = s1ap_send_ue_context_release_command(enb_ue,
            S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
            S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
        ogs_expect(rv == OGS_OK);
    }
}

/* TS 29.060 7.5.14.1 */
void mme_gn_handle_ran_information_relay(
        ogs_gtp_xact_t *xact, ogs_gtp1_ran_information_relay_t *req)
{
    uint8_t discr;
    ogs_eps_tai_t *target_tai;
    S1AP_Global_ENB_ID_t global_ENB_ID;
    uint32_t target_enb_id;
    mme_enb_t *target_enb = NULL;
    int rv;

    ogs_debug("[Gn] Receiving RAN Information Relay");

    if (req->ran_transparent_container.len == 0 ||
        req->ran_transparent_container.data == NULL) {
        ogs_warn("[Gn] Receiving RAN Information Relay with empty container!");
        return;
    }

    /* "If RIM Routing Address Discriminator IE is not included, the RIM Routing
     * Address shall be processed as an RNC identifier". Hence, not for us! */
    if (req->rim_routing_address_discriminator.presence == false) {
        ogs_warn("[Gn] Rx RAN Information Relay: no RIM Routing Address Discriminator!");
        return;
    }

    if (req->rim_routing_address_discriminator.len != 1) {
        ogs_warn("[Gn] Rx RAN Information Relay: RIM Routing Address Discriminator len != 1");
        return;
    }

    /* 3GPP TS 48.018 RIM Routing Information IE */
    discr = *((uint8_t*)req->rim_routing_address_discriminator.data) & 0x0f;
    if (discr != 2) {/* 2: An eNB identifier is used to identify an E-UTRAN eNodeB or HeNB */
        ogs_warn("[Gn] Rx RAN Information Relay: RIM Routing Address Discriminator %u unexpected", discr);
        return;
    }

    if (req->rim_routing_address.presence == false) {
        ogs_warn("[Gn] Rx RAN Information Relay: no RIM Routing Address!");
        return;
    }

    if (req->rim_routing_address.len < sizeof(ogs_eps_tai_t)) {
        ogs_warn("[Gn] Rx RAN Information Relay: RIM Routing Address len < %zu", sizeof(ogs_eps_tai_t));
        return;
    }
    target_tai = req->rim_routing_address.data;

    rv = decode_global_enb_id(&global_ENB_ID,
            ((uint8_t *)req->rim_routing_address.data) + sizeof(ogs_eps_tai_t),
            req->rim_routing_address.len - sizeof(ogs_eps_tai_t));
    if (rv != OGS_OK)
        return;
    ogs_s1ap_ENB_ID_to_uint32(&global_ENB_ID.eNB_ID, &target_enb_id);

    ogs_debug("    Target : ENB_ID[%s:%d], TAC[%d]",
              global_ENB_ID.eNB_ID.present ==
                  S1AP_ENB_ID_PR_homeENB_ID ? "Home" :
              global_ENB_ID.eNB_ID.present ==
                  S1AP_ENB_ID_PR_macroENB_ID ? "Macro" : "Others",
              target_enb_id, target_tai->tac);

    target_enb = mme_enb_find_by_enb_id(target_enb_id);
    if (target_enb == NULL) {
        ogs_warn("[Gn] Rx RAN Information Relay: cannot find target eNB-id[0x%x]", target_enb_id);
        return;
    }

    s1ap_send_mme_direct_information_transfer(
        target_enb,
        req->ran_transparent_container.data,
        req->ran_transparent_container.len);
}
