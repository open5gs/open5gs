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

/*
 * The ogs_sbi server library calls our handler (ogs_sbi_server_handler,
 * from lib/sbi/path.c) which pushes a generic ogs_event_t of type
 * OGS_EVENT_SBI_SERVER onto ogs_app()->queue. mme-init.c intercepts
 * that event type in the main loop and delegates here.
 */

static void handle_admin_route(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);

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
        handle_admin_route(stream, &sbi_message);
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
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    SWITCH(message->h.resource.component[0])
    CASE("ue-contexts")
        SWITCH(message->h.method)
        CASE(OGS_SBI_HTTP_METHOD_DELETE)
            mme_admin_handle_delete_ue_context(stream, message);
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
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
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
     * V1 intentionally serves only ECM-CONNECTED UEs. ECM-IDLE would
     * require paging the UE first (S1AP Paging with type=DETACH_TO_UE);
     * that path exists but adds a second round-trip and a failure
     * branch (UE unreachable). Defer to a follow-up patch.
     */
    if (!ECM_CONNECTED(mme_ue)) {
        ogs_warn("[%s] Admin release: mme_ue_id=%s is ECM-IDLE; "
                "paged-detach not implemented in V1 — returning 409",
                mme_ue->imsi_bcd, id_str);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_CONFLICT, message,
                "UE is ECM-IDLE", id_str, NULL));
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
