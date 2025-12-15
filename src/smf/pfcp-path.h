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

#include "n4-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int smf_pfcp_open(void);
void smf_pfcp_close(void);

int smf_pfcp_send_modify_list(
        smf_sess_t *sess,
        ogs_pkbuf_t *(*modify_list)(
            uint8_t type, smf_sess_t *sess, ogs_pfcp_xact_t *xact),
        ogs_pfcp_xact_t *xact, ogs_time_t duration);

int smf_5gc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, uint64_t flags);
int smf_5gc_pfcp_send_all_pdr_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        uint64_t flags, ogs_time_t duration);
int smf_5gc_pfcp_send_qos_flow_list_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        uint64_t flags, ogs_time_t duration);
int smf_5gc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, int trigger);

int smf_epc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, ogs_pool_id_t gtp_xact_id, uint64_t flags);
int smf_epc_pfcp_send_all_pdr_modification_request(
        smf_sess_t *sess, ogs_pool_id_t gtp_xact_id, ogs_pkbuf_t *gtpbuf,
        uint64_t flags, uint8_t gtp_pti, uint8_t gtp_cause);
int smf_epc_pfcp_send_one_bearer_modification_request(
        smf_bearer_t *bearer, ogs_pool_id_t gtp_xact_id,
        uint64_t flags, uint8_t gtp_pti, uint8_t gtp_cause);
int smf_epc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, ogs_pool_id_t gtp_xact_id);

int smf_epc_pfcp_send_deactivation(smf_sess_t *sess, uint8_t gtp_cause);

int smf_pfcp_send_session_report_response(
        ogs_pfcp_xact_t *xact, smf_sess_t *sess, uint8_t cause);

uint32_t smf_pfcp_urr_usage_report_trigger2diam_gy_reporting_reason(
            ogs_pfcp_usage_report_trigger_t *rep_trigger);

#ifdef __cplusplus
}
#endif

#endif /* SMF_PFCP_PATH_H */
