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
#include "mme-sm.h"
#include "mme-event.h"
#include "nas-path.h"
#include "s1ap-path.h"

#include "S1AP_CNDomain.h"

/*
 * The ogs_sbi server library calls our handler (ogs_sbi_server_handler,
 * from lib/sbi/path.c) which pushes a generic ogs_event_t of type
 * OGS_EVENT_SBI_SERVER onto ogs_app()->queue. mme-init.c intercepts
 * that event type in the main loop and delegates here.
 */

static void handle_admin_route(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message,
        ogs_sbi_request_t *request);

int mme_admin_server_open(void)
{
    if (!mme_self()->admin_config.enabled) {
        ogs_debug("admin endpoint disabled — skipping listener start");
        return OGS_OK;
    }

    if (ogs_sbi_server_first_by_interface("admin") == NULL) {
        ogs_warn("admin.enabled is set but no admin.server was parsed "
                "from mme.yaml — endpoint will not accept requests");
        return OGS_OK;
    }

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK) {
        ogs_error("ogs_sbi_server_start_all() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

void mme_admin_server_close(void)
{
    if (!mme_self()->admin_config.enabled)
        return;

    ogs_sbi_server_stop_all();
}

void mme_admin_process_sbi_server_event(ogs_event_t *e)
{
    int rv;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *sbi_request = NULL;
    ogs_sbi_message_t sbi_message;

    ogs_assert(e);
    ogs_assert(e->sbi.request);
    ogs_assert(e->sbi.data);

    sbi_request = e->sbi.request;
    stream = (ogs_sbi_stream_t *)ogs_sbi_stream_find_by_id(
            OGS_POINTER_TO_UINT(e->sbi.data));
    if (!stream) {
        ogs_error("STREAM has already been removed");
        ogs_sbi_request_free(sbi_request);
        return;
    }

    rv = ogs_sbi_parse_request(&sbi_message, sbi_request);
    if (rv != OGS_OK) {
        ogs_error("cannot parse HTTP sbi_message");
        ogs_assert(true ==
            ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                NULL, "cannot parse HTTP sbi_message", NULL,
                NULL));
        return;
    }

    /*
     * Cross-NF-convention guard: the ogs_sbi server library is used
     * here strictly for the admin namespace. Anything outside it is
     * out of scope for MME.
     */
    SWITCH(sbi_message.h.service.name)
    CASE("admin")
        if (!mme_self()->admin_config.enabled) {
            ogs_info("admin endpoint disabled (admin.enabled not set "
                    "in mme.yaml) — responding 404");
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND, &sbi_message,
                    "Not Found", NULL, NULL));
            break;
        }
        handle_admin_route(stream, &sbi_message, sbi_request);
        break;

    DEFAULT
        ogs_error("Invalid API name [%s]", sbi_message.h.service.name);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                "Invalid API name", sbi_message.h.service.name, NULL));
    END

    ogs_sbi_message_free(&sbi_message);
}

static void handle_admin_route(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message,
        ogs_sbi_request_t *request)
{
    SWITCH(message->h.resource.component[0])
    CASE("ue-contexts")
        SWITCH(message->h.method)
        CASE(OGS_SBI_HTTP_METHOD_DELETE)
            mme_admin_handle_delete_ue_context(stream, message, request);
            break;
        DEFAULT
            ogs_error("Invalid HTTP method [%s]", message->h.method);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                    "Invalid HTTP method", message->h.method, NULL));
        END
        break;
    DEFAULT
        ogs_error("Invalid resource name [%s]",
                message->h.resource.component[0]);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Invalid resource name",
                message->h.resource.component[0], NULL));
    END
}

void mme_admin_handle_delete_ue_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message,
        ogs_sbi_request_t *request)
{
    mme_ue_t *mme_ue = NULL;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    char *id_str = NULL;
    ogs_pool_id_t mme_ue_id;
    int r;

    ogs_assert(stream);
    ogs_assert(message);

    id_str = message->h.resource.component[1];
    if (!id_str || !*id_str) {
        ogs_error("Admin release: missing mme_ue_id in path");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Missing mme_ue_id", NULL, NULL));
        return;
    }

    mme_ue_id = (ogs_pool_id_t)atoll(id_str);
    mme_ue = mme_ue_find_by_id(mme_ue_id);
    if (!mme_ue) {
        ogs_info("Admin release: mme_ue_id [%s] not found "
                "(context already gone)", id_str);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_FOUND, message,
                "mme_ue_id not found", id_str, NULL));
        return;
    }

    /*
     * Force-purge opt-in (?purge=true): skip all NAS signalling and
     * tear the MME-side context down locally. Intended for operator
     * cleanup of stale contexts left behind by UEs that cannot or
     * will not respond to the standard detach flow:
     *   - NB-IoT / LTE-M UEs in Power Saving Mode (PSM) or extended
     *     DRX (eDRX) that may sleep for up to ~14 days — a Paging
     *     or Detach Request would time out and the operator wants
     *     deterministic cleanup within a shorter window.
     *   - UEs that have physically left coverage without ever sending
     *     a Detach Request (implicit leave).
     *
     * Semantics on the UE side:
     *   - Local context removed, GUTI no longer indexed in MME.
     *   - On next UE contact with the old GUTI, MME replies with an
     *     Identity Request (TS 24.301 §5.4.4). UE supplies its IMSI;
     *     a fresh attach procedure ensues — spec-compliant.
     *
     * Safety: the explicit query parameter is opt-in only. Without
     * ?purge=true the default flow (paged-detach / immediate Detach
     * Request) runs as before. Audit log emitted at WARNING level so
     * purge actions are retrievable via `journalctl -u open5gs-mmed`.
     */
    if (request && request->http.params) {
        const char *purge_str = ogs_sbi_header_get(
                request->http.params, "purge");
        if (purge_str && strcasecmp(purge_str, "true") == 0) {
            ogs_warn("[%s] Admin force-purge: mme_ue_id=%s — removing "
                    "local context without NAS signalling. UE will "
                    "IMSI-attach on next contact "
                    "(TS 24.301 §5.4.4 Identity Request)",
                    mme_ue->imsi_bcd[0] ? mme_ue->imsi_bcd : "unknown",
                    id_str);

            memset(&sendmsg, 0, sizeof(sendmsg));
            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true ==
                ogs_sbi_server_send_response(stream, response));

            mme_ue_remove(mme_ue);
            return;
        }
    }

    /*
     * Guard against concurrent DELETE or against a detach that is
     * already in flight. Only EMM-REGISTERED contexts can accept a
     * fresh network-initiated Detach Request; any other state means
     * a teardown is already under way.
     */
    if (!OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered)) {
        ogs_info("[%s] Admin release: mme_ue_id=%s not in "
                "emm_state_registered — responding 204 idempotent",
                mme_ue->imsi_bcd[0] ? mme_ue->imsi_bcd : "unknown",
                id_str);
        memset(&sendmsg, 0, sizeof(sendmsg));
        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return;
    }

    /*
     * ECM-IDLE path: the UE is not reachable for an immediate NAS
     * Detach Request. Trigger S1AP Paging; the standard MME paging
     * machinery (T3413 retry, paging-response handling in emm-sm.c)
     * takes over. When the UE responds with a Service Request, the
     * `admin_detach_pending` flag set here tells the EMM state machine
     * to send the Detach Request right after the Service Request's
     * Initial Context Setup completes — completing the paged-detach
     * flow.
     *
     * If the UE never responds, T3413 expires and the MME performs
     * Implicit Detach (existing behaviour) which tears the context
     * down locally. No extra timer needed in this handler.
     *
     * HTTP 202 Accepted signals async completion — the caller can
     * poll /ue-info to observe the UE disappear when the flow
     * finishes (order of seconds to T3413-bounded). 204 would be
     * misleading because the detach has only been initiated, not
     * confirmed.
     */
    if (!ECM_CONNECTED(mme_ue)) {
        int pr;
        ogs_warn("[%s] Admin-paged detach: mme_ue_id=%s is ECM-IDLE, "
                "triggering S1AP Paging — Detach Request will follow "
                "on Service Request response (or Implicit Detach on "
                "T3413 expiry)",
                mme_ue->imsi_bcd, id_str);

        mme_ue->admin_detach_pending = true;
        pr = s1ap_send_paging(mme_ue, S1AP_CNDomain_ps);
        if (pr != OGS_OK) {
            mme_ue->admin_detach_pending = false;
            ogs_error("[%s] s1ap_send_paging() failed [%d] — aborting "
                    "paged-detach", mme_ue->imsi_bcd, pr);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, message,
                    "Paging failed", id_str, NULL));
            return;
        }

        memset(&sendmsg, 0, sizeof(sendmsg));
        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_ACCEPTED);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return;
    }

    ogs_warn("[%s] Admin-initiated detach: mme_ue_id=%s detach_type="
            "re-attach-required", mme_ue->imsi_bcd, id_str);

    /*
     * Respond 204 immediately — the detach is asynchronous. UE will
     * reply with Detach Accept or T3422 will fire; either way the
     * existing EMM state machine follows up with
     * s1ap_send_ue_context_release_command(). From the caller's
     * perspective completion is observable via /ue-info polling.
     */
    memset(&sendmsg, 0, sizeof(sendmsg));
    response = ogs_sbi_build_response(
            &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    /*
     * Build the MME-initiated Detach Request per 3GPP TS 24.301
     * §5.5.2.3 with detach-type = "re-attach required". This clears
     * both NAS context on the UE and ensures the UE performs a fresh
     * Attach procedure, re-creating the ESM/EPS bearer context the
     * upstream SMF has forgotten. The standard EMM FSM handles the
     * rest (T3422, Detach Accept, UEContextReleaseCommand).
     */
    memset(&mme_ue->nas_eps.detach, 0, sizeof(ogs_nas_detach_type_t));
    mme_ue->nas_eps.detach.value =
        OGS_NAS_DETACH_TYPE_TO_UE_RE_ATTACH_REQUIRED;
    mme_ue->nas_eps.type = MME_EPS_TYPE_DETACH_REQUEST_TO_UE;

    r = nas_eps_send_detach_request(mme_ue);
    if (r != OGS_OK)
        ogs_error("[%s] nas_eps_send_detach_request() failed [%d]",
                mme_ue->imsi_bcd, r);
}
