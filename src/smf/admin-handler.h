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

#ifndef SMF_ADMIN_HANDLER_H
#define SMF_ADMIN_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DELETE /admin/v1/pdu-sessions/{smContextRef}
 *
 * Force-release a PDU session without involving the peer AMF or MME. The
 * release is driven through the standard local-initiated teardown path
 * (wait_pfcp_deletion), so PCF SmPolicy, UPF PFCP session, UE-IP pool
 * entry, and SMF context are all cleaned up normally. Intended for OAM
 * consistency audits that find stranded sessions on the SMF while the
 * peer NF has already forgotten the UE (e.g. AMF returns HTTP 504 to
 * N1N2MessageTransfer, breaking the usual lazy-release path that only
 * fires on 404).
 *
 * Response: 204 No Content on accepted; release runs asynchronously.
 */
void smf_admin_handle_delete_sm_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* SMF_ADMIN_HANDLER_H */
