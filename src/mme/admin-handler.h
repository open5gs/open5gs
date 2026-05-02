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

#ifndef MME_ADMIN_HANDLER_H
#define MME_ADMIN_HANDLER_H

#include "ogs-sbi.h"
#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Lifecycle hooks called from mme-init.c. Open binds the admin HTTP/2
 * listener(s) configured under `mme.admin.server` in mme.yaml; close
 * stops them during termination. Both are no-ops when admin.enabled is
 * false.
 */
int mme_admin_server_open(void);
void mme_admin_server_close(void);

/*
 * Main-thread hook: consumes one OGS_EVENT_SBI_SERVER event pushed onto
 * the app queue by the shared ogs_sbi_server_handler callback. Performs
 * synchronous dispatch on the request's service/resource/method and
 * emits the HTTP response before returning. The event itself is freed
 * by the caller.
 */
void mme_admin_process_sbi_server_event(ogs_event_t *e);

/*
 * DELETE /admin/v1/ue-contexts/{mme_ue_id}
 *
 * Force-detach an EMM UE context that has been left out of sync after
 * an upstream NF restart (typically SMF, where EPS bearer context was
 * lost but the UE's EMM-REGISTERED state persists). Issues a spec-
 * compliant MME-initiated Detach Request with detach-type "re-attach
 * required" (3GPP TS 24.301 §5.5.2.3) so the UE rebuilds its NAS state
 * and re-attaches from scratch instead of keeping stale bearer context.
 *
 * Response: 204 No Content on accepted; detach runs asynchronously.
 */
void mme_admin_handle_delete_ue_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message,
        ogs_sbi_request_t *request);

/*
 * Internal helper: post a deferred mme_ue_remove() onto the main app
 * event queue (MME_EVENT_ADMIN_UE_PURGE). Used by the ?purge=true
 * path to keep state teardown out of the SBI handler's stack frame.
 */
void mme_admin_post_purge(ogs_pool_id_t mme_ue_id);

#ifdef __cplusplus
}
#endif

#endif /* MME_ADMIN_HANDLER_H */
