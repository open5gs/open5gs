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
