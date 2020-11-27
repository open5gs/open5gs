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

#ifndef SMF_PFCP_PATH_H
#define SMF_PFCP_PATH_H

#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

int smf_pfcp_open(void);
void smf_pfcp_close(void);

void smf_5gc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream);
void smf_5gc_pfcp_send_session_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, uint64_t flags);
void smf_5gc_pfcp_send_qos_flow_modification_request(smf_bearer_t *qos_flow,
        ogs_sbi_stream_t *stream, uint64_t flags);
void smf_5gc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, int trigger);

void smf_epc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, void *gtp_xact);
void smf_epc_pfcp_send_bearer_modification_request(
        smf_bearer_t *bearer, uint64_t flags);
void smf_epc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, void *gtp_xact);

#ifdef __cplusplus
}
#endif

#endif /* SMF_PFCP_PATH_H */
