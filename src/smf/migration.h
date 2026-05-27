/*
 * Copyright (C) 2026 by Open5GS contributors
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
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SMF_MIGRATION_H
#define SMF_MIGRATION_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *smf_migration_state_name(smf_migration_state_e state);
bool smf_migration_active(const smf_sess_t *sess);
void smf_migration_clear(smf_sess_t *sess);
int smf_migration_send_path_switch_request(smf_sess_t *sess);
int smf_migration_handle_path_switch_response(
        smf_sess_t *sess, ogs_sbi_stream_t *stream);
int smf_migration_send_source_deletion(smf_sess_t *sess);
int smf_migration_send_target_deletion(smf_sess_t *sess);
void smf_migration_handle_source_deletion_response(
        smf_sess_t *sess, bool success);
void smf_migration_handle_target_deletion_response(
        smf_sess_t *sess, bool success);
void smf_migration_mark_failed(smf_sess_t *sess);
size_t smf_handle_pdu_migrate(char *buf, size_t buflen,
        const char *method, const char *body, size_t body_len,
        int *status_code);

#ifdef __cplusplus
}
#endif

#endif /* SMF_MIGRATION_H */
