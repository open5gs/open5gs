/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef GSM_HANDLER_H
#define GSM_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GSM_QOS_FLOW_DESCRIPTION_IEI \
    OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_TYPE
#define GSM_QOS_FLOW_DESCRIPTION_IE_HEADER_LEN 3

uint8_t *gsm_decode_base64(const char *src, int *out_len);
uint8_t *gsm_decode_qos_flow_description_ie(const char *src, int *len);
bool gsm_qos_flow_description_ie_is_valid(const char *src);
int gsm_parse_qos_flow_description_ie(
        ogs_nas_qos_flow_description_t *description, const char *src);

int gsm_handle_pdu_session_establishment_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_nas_5gs_pdu_session_establishment_request_t *
            pdu_session_establishment_request);

int gsm_handle_pdu_session_modification_qos_rules(
        smf_sess_t *sess,
        ogs_nas_qos_rules_t *requested_qos_rules,
        uint64_t *pfcp_flags);
int gsm_handle_pdu_session_modification_qos_flow_descriptions(
        smf_sess_t *sess,
        ogs_nas_qos_flow_descriptions_t *requested_qos_flow_descriptions,
        uint64_t *pfcp_flags);

int gsm_handle_pdu_session_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_nas_5gs_pdu_session_modification_request_t *
            pdu_session_modification_request);

#ifdef __cplusplus
}
#endif

#endif /* GSM_HANDLER_H */
