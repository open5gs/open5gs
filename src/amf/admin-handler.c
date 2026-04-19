/*
 * Copyright (C) 2026 by Daniel Brune <daniel.brune.so@outlook.com>
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

#include "admin-handler.h"
#include "amf-sm.h"
#include "nas-path.h"

void amf_admin_handle_delete_ue_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message,
        ogs_sbi_request_t *request)
{
    amf_ue_t *amf_ue = NULL;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    char *id_str = NULL;
    ogs_pool_id_t amf_ue_id;
    int r;

    ogs_assert(stream);
    ogs_assert(message);

    id_str = message->h.resource.component[1];
    if (!id_str || !*id_str) {
        ogs_error("Admin release: missing amf_ue_id in path");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Missing amf_ue_id", NULL, NULL));
        return;
    }

    amf_ue_id = (ogs_pool_id_t)atoll(id_str);
    amf_ue = amf_ue_find_by_id(amf_ue_id);
    if (!amf_ue) {
        ogs_info("Admin release: amf_ue_id [%s] not found "
                "(context already gone)", id_str);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_FOUND, message,
                "amf_ue_id not found", id_str, NULL));
        return;
    }

    /*
     * Force-purge opt-in (?purge=true): skip all NAS signalling and
     * tear the AMF-side context down locally. Mirrors the MME admin
     * endpoint's purge semantics on the 5GC side.
     *
     * Motivating cases:
     *   - UE left 5G coverage (e.g. gNB radio off). The default
     *     Deregistration path fails with "NG context has already been
     *     removed" (OGS_NOTFOUND in src/amf/nas-path.c), which the V1
     *     admin handler swallows — but without removing the local
     *     amf_ue. Purge closes that gap.
     *   - NB-IoT / LTE-M 5GC-capable UEs in PSM / eDRX that may sleep
     *     for up to ~14 days. Paging + NAS Deregistration would time
     *     out on T3513; operators need deterministic cleanup within
     *     shorter windows.
     *   - Cross-RAT artefacts: UE moved 5G → 4G without clean
     *     Deregistration; AMF holds stale 5GMM-REGISTERED context
     *     until T3512 (~58 min) implicit-deregister fires.
     *
     * UE-side recovery:
     *   - Local context removed, 5G-GUTI no longer indexed.
     *   - On next 5G contact with the old 5G-GUTI, AMF replies with
     *     Identity Request (TS 24.501 §5.4.3). UE supplies its SUCI;
     *     standard Registration procedure follows — spec-compliant.
     *
     * Safety: opt-in via explicit query parameter. Without
     * ?purge=true the existing default flow (Deregistration Request)
     * runs unchanged. Audit log emitted at WARNING level so purge
     * actions are retrievable via `journalctl -u open5gs-amfd`.
     */
    if (request && request->http.params) {
        const char *purge_str = ogs_sbi_header_get(
                request->http.params, "purge");
        if (purge_str && strcasecmp(purge_str, "true") == 0) {
            ogs_warn("[%s] Admin force-purge: amf_ue_id=%s — removing "
                    "local context without NAS signalling. UE will "
                    "SUCI-register on next 5G contact "
                    "(TS 24.501 §5.4.3 Identity Request)",
                    amf_ue->supi ? amf_ue->supi : "unknown", id_str);

            memset(&sendmsg, 0, sizeof(sendmsg));
            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true ==
                ogs_sbi_server_send_response(stream, response));

            amf_ue_remove(amf_ue);
            return;
        }
    }

    /*
     * Guard against concurrent DELETE or against a deregistration that
     * is already in flight (e.g. UE-initiated detach crossing our
     * request). Only 5GMM-REGISTERED contexts can accept a fresh
     * network-initiated Deregistration Request; any other state
     * means a teardown is already under way.
     */
    if (!OGS_FSM_CHECK(&amf_ue->sm, gmm_state_registered)) {
        ogs_info("[%s] Admin release: amf_ue_id=%s already in teardown "
                "or not yet registered — responding 204 idempotent",
                amf_ue->supi ? amf_ue->supi : "unknown", id_str);
        memset(&sendmsg, 0, sizeof(sendmsg));
        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return;
    }

    ogs_warn("[%s] Admin-initiated deregistration: amf_ue_id=%s cm=%s",
            amf_ue->supi ? amf_ue->supi : "unknown", id_str,
            CM_CONNECTED(amf_ue) ? "CONNECTED" : "IDLE");

    /*
     * Respond 204 immediately — the deregistration is asynchronous.
     * The caller must not infer completion from this response;
     * verification happens via a subsequent /ue-info poll or by
     * observing the UE re-register.
     */
    memset(&sendmsg, 0, sizeof(sendmsg));
    response = ogs_sbi_build_response(
            &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    /*
     * Drive the AMF-initiated Deregistration Request path per
     * 3GPP TS 24.501 §5.5.2.3. With reason = REREGISTRATION_REQUIRED
     * and cause = IMPLICITLY_DE_REGISTERED the UE is instructed to
     * clear its 5GMM state and re-register. The standard AMF FSM
     * starts T3522 and follows up with NGAP UEContextReleaseCommand
     * once the UE's Deregistration Accept arrives or the timer
     * expires.
     *
     * When the UE is CM-IDLE there is no NG UE to deliver the NAS
     * message to; nas_5gs_send_de_registration_request() returns
     * OGS_NOTFOUND in that case and logs it. The local AMF context
     * will still be cleaned up once downstream state (UDM dereg,
     * SMF release) finishes via the usual FSM paths.
     */
    r = nas_5gs_send_de_registration_request(amf_ue,
            OpenAPI_deregistration_reason_REREGISTRATION_REQUIRED,
            OGS_5GMM_CAUSE_IMPLICITLY_DE_REGISTERED);
    ogs_expect(r == OGS_OK || r == OGS_NOTFOUND);
}
