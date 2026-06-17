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
#include "smf-sm.h"
#include "local-path.h"

void smf_admin_handle_delete_sm_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    smf_sess_t *sess = NULL;
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    char *ref = NULL;

    ogs_assert(stream);
    ogs_assert(message);

    ref = message->h.resource.component[1];
    if (!ref || !*ref) {
        ogs_error("Admin release: missing smContextRef in path");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Missing smContextRef", NULL, NULL));
        return;
    }

    sess = smf_sess_find_by_sm_context_ref(ref);
    if (!sess) {
        ogs_info("Admin release: smContextRef [%s] not found "
                "(session already gone)", ref);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_FOUND, message,
                "smContextRef not found", ref, NULL));
        return;
    }

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    /*
     * Guard against concurrent DELETE on the same session, or against a
     * DELETE arriving while a 3GPP-initiated release is already in
     * flight (e.g. AMF-driven UE detach). Only sessions in the steady
     * smf_gsm_state_operational accept a fresh local-release trigger;
     * any other state means a teardown is already in progress and the
     * SMF_EVT_SESSION_RELEASE handler in that state is not guaranteed
     * to be a no-op.
     *
     * Treat already-terminating as idempotent success (204): from the
     * caller's perspective the session will be gone momentarily anyway.
     */
    if (!OGS_FSM_CHECK(&sess->sm, smf_gsm_state_operational)) {
        ogs_info("[%s] Admin release: smContextRef=%s already in "
                "teardown — responding 204 idempotent",
                smf_ue->supi, ref);
        memset(&sendmsg, 0, sizeof(sendmsg));
        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return;
    }

    ogs_warn("[%s] Admin-initiated release: "
            "smContextRef=%s psi=%d dnn=%s rat=%s",
            smf_ue->supi, ref, sess->psi,
            sess->session.name ? sess->session.name : "",
            sess->epc ? "EPC" : "5GC");

    /*
     * Respond 204 immediately — the release is asynchronous. The caller
     * must not infer completion from this response; verification happens
     * via a subsequent /pdu-info poll.
     */
    memset(&sendmsg, 0, sizeof(sendmsg));
    response = ogs_sbi_build_response(
            &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    /*
     * Drive the standard local teardown path. Enqueues SMF_EVT_SESSION_
     * RELEASE with trigger LOCAL_INITIATED, which from smf_gsm_state_
     * operational transitions into smf_gsm_state_wait_pfcp_deletion.
     * That state issues PFCP Session Delete to the UPF; on response the
     * 5G path proceeds to PCF SmPolicy Delete + UDM Dereg, while the
     * EPC path terminates Gx/Gy/S6b via Diameter CCR-Termination.
     * Neither path contacts the peer AMF or MME — deliberately so, since
     * the typical caller (the EP5G reconciler) only invokes this when
     * those peers have already forgotten the UE.
     */
    smf_trigger_session_release(
            sess, NULL, OGS_PFCP_DELETE_TRIGGER_LOCAL_INITIATED);
}
