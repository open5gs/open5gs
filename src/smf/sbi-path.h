/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SMF_SBI_PATH_H
#define SMF_SBI_PATH_H

#include "nudm-build.h"
#include "namf-build.h"
#include "gsm-build.h"
#include "nnrf-build.h"
#include "npcf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int smf_sbi_open(void);
void smf_sbi_close(void);

void smf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);
void smf_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        smf_sess_t *sess, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(smf_sess_t *sess, void *data));

void smf_sbi_send_response(ogs_sbi_stream_t *stream, int status);

void smf_sbi_send_sm_context_create_error(
        ogs_sbi_stream_t *stream,
        int status, const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf);
void smf_sbi_send_sm_context_updated_data(smf_sess_t *sess,
        ogs_sbi_stream_t *stream, OpenAPI_up_cnx_state_e up_cnx_state);
void smf_sbi_send_sm_context_updated_data_in_session_deletion(
        smf_sess_t *sess, ogs_sbi_stream_t *stream);
void smf_sbi_send_sm_context_update_error(
        ogs_sbi_stream_t *stream,
        int status, const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf);

void smf_sbi_send_sm_context_status_notify(smf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* SMF_SBI_PATH_H */
