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

#ifndef AMF_ADMIN_HANDLER_H
#define AMF_ADMIN_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DELETE /admin/v1/ue-contexts/{amf_ue_id}
 *
 * Force-detach a 5GMM UE context that has been left out of sync after an
 * upstream NF restart (typically SMF, where the PDU session context was
 * lost but the UE's 5GMM-REGISTERED state persists). Issues a spec-
 * compliant AMF-initiated Deregistration Request with
 * RE_REGISTRATION_REQUIRED so the UE rebuilds its NAS state and re-
 * registers from scratch instead of keeping stale EPS bearer context.
 *
 * Response: 204 No Content on accepted; deregistration runs asynchronously.
 * Behaviour when the UE is CM-IDLE: handler triggers a local teardown of
 * the AMF-side context without attempting a DL-NAS transport that would
 * have nowhere to go.
 */
void amf_admin_handle_delete_ue_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message,
        ogs_sbi_request_t *request);

#ifdef __cplusplus
}
#endif

#endif /* AMF_ADMIN_HANDLER_H */
