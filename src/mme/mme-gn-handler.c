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

/* 3GPP TS 23.003 2.8.2.2 Mapping from RAI and P-TMSI to GUT */
static void rai_ptmsi_to_guti(const ogs_nas_rai_t *rai, mme_p_tmsi_t ptmsi, uint32_t ptmsi_sig, ogs_nas_eps_guti_t *nas_guti)
{
    uint16_t lac = be16toh(rai->lai.lac);;
    nas_guti->nas_plmn_id =rai->lai.nas_plmn_id;
    nas_guti->mme_gid = lac;
    nas_guti->mme_code = rai->rac;
    nas_guti->m_tmsi = 0xC0000000 | (ptmsi & 0x3f000000) | (ptmsi_sig & 0x00ff0000) | (ptmsi & 0x0000ffff);
}

/* TS 29.060 7.5.3 SGSN Context Request */
void mme_gn_handle_sgsn_context_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_sgsn_context_request_t *req)
{
    ogs_nas_eps_guti_t nas_guti;
    ogs_nas_rai_t *rai;
    mme_ue_t *mme_ue = NULL;
    int rv;

    ogs_debug("[Gn] Rx SGSN Context Request");

    if (!req->routeing_area_identity.presence) {
        ogs_warn("[Gn] Rx SGSN Context Request with no RAI!");
        mme_gtp1_send_sgsn_context_response(NULL, OGS_GTP1_CAUSE_MANDATORY_IE_MISSING, xact);
        return;
    }
    if (req->routeing_area_identity.len != sizeof(*rai)) {
        ogs_warn("[Gn] Rx SGSN Context Request RAI wrong size %u vs exp %zu!",
                 req->routeing_area_identity.len, sizeof(*rai));
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

    rai = req->routeing_area_identity.data;

    if (req->imsi.presence) {
        mme_ue = mme_ue_find_by_imsi(req->imsi.data, req->imsi.len);
    } else if (req->packet_tmsi.presence) { /* P-TMSI */
        if (!req->p_tmsi_signature.presence) {
            ogs_warn("[Gn] Rx SGSN Context Request with 'P-TMSI' but no P-TMSI Signature! Assuming value 0.");
            req->p_tmsi_signature.u24 = 0;
        }
        rai_ptmsi_to_guti(rai, req->packet_tmsi.u32, req->p_tmsi_signature.u24, &nas_guti);
        mme_ue = mme_ue_find_by_guti(&nas_guti);
    } else if (req->temporary_logical_link_identifier.presence) {
        if (!req->p_tmsi_signature.presence) {
            ogs_warn("[Gn] Rx SGSN Context Request with 'TLLI' but no P-TMSI Signature! Assuming value 0.");
            req->p_tmsi_signature.u24 = 0;
        }
        /* TS 29.060 7.5.3 "The TLLI/P-TMSI and RAI is a foreign TLLI/P-TMSI and an RAI in the old SGSN."
         * A foregin TLLI is "tlli = (p_tmsi & 0x3fffffff) | 0x80000000", and since we only use 0x3fffffff
         * bits of P-TMSI to derive the GUTI, it's totally fine passing the TLLI as P-TMSI. */
        rai_ptmsi_to_guti(rai, req->temporary_logical_link_identifier.u32, req->p_tmsi_signature.u24, &nas_guti);
        mme_ue = mme_ue_find_by_guti(&nas_guti);
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

/* TS 29.060 7.5.5 SGSN Context Acknowledge */
void mme_gn_handle_sgsn_context_acknowledge(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue, ogs_gtp1_sgsn_context_acknowledge_t *req)
{
    int rv;

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
    if (mme_ue->enb_ue) {
        rv = s1ap_send_ue_context_release_command(mme_ue->enb_ue,
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
