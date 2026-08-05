/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef AMF_LOG_H
#define AMF_LOG_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct amf_log_context_s {
    ogs_pool_id_t gnb_id;
    ogs_pool_id_t ran_ue_id;
    ogs_pool_id_t amf_ue_id;
    ogs_pool_id_t sess_id;

    uint64_t ran_ue_ngap_id;
    uint64_t amf_ue_ngap_id;

    const char *suci;
    const char *supi;
} amf_log_context_t;

/*
 * All helpers below are READ-ONLY with respect to their input contexts.
 * They tolerate NULL inputs and simply leave the corresponding fields
 * of `ctx` untouched (the caller is expected to clear the struct first
 * with amf_log_context_clear()).
 *
 * Helpers never assert. They never allocate heap memory and never mutate
 * the AMF contexts they read from.
 */

void amf_log_context_clear(amf_log_context_t *ctx);

void amf_log_context_from_event(amf_log_context_t *ctx, amf_event_t *e);
void amf_log_context_from_ue(amf_log_context_t *ctx, amf_ue_t *amf_ue);
void amf_log_context_from_ran_ue(amf_log_context_t *ctx, ran_ue_t *ran_ue);
void amf_log_context_from_sess(amf_log_context_t *ctx, amf_sess_t *sess);

/*
 * AMF structured event names.
 *
 * Used as the `event.name` field in JSON log output.
 * Keep names lowercase with underscores.
 */
#define AMF_EVENT_INITIAL_UE_MESSAGE            "initial_ue_message"
#define AMF_EVENT_RAN_UE_ADD                     "ran_ue_add"
#define AMF_EVENT_AMF_UE_ADD                     "amf_ue_add"
#define AMF_EVENT_REGISTRATION_REQUEST           "registration_request"
#define AMF_EVENT_IDENTITY_REQUEST               "identity_request"
#define AMF_EVENT_IDENTITY_RESPONSE              "identity_response"
#define AMF_EVENT_SUCI_RECEIVED                  "suci_received"
#define AMF_EVENT_SBI_REQUEST                    "sbi_request"
#define AMF_EVENT_SBI_RESPONSE                   "sbi_response"
#define AMF_EVENT_AUTHENTICATION_COMPLETE        "authentication_complete"
#define AMF_EVENT_SECURITY_MODE_COMMAND           "security_mode_command"
#define AMF_EVENT_SECURITY_MODE_COMPLETE         "security_mode_complete"
#define AMF_EVENT_INITIAL_CONTEXT_SETUP_REQUEST  "initial_context_setup_request"
#define AMF_EVENT_REGISTRATION_ACCEPT            "registration_accept"
#define AMF_EVENT_REGISTRATION_COMPLETE          "registration_complete"
#define AMF_EVENT_CONFIGURATION_UPDATE_COMMAND   "configuration_update_command"
#define AMF_EVENT_GUTI_RECEIVED                  "guti_received"
#define AMF_EVENT_GUTI_ASSIGNED                  "guti_assigned"

#define AMF_EVENT_NGAP_DECODE_FAILED             "ngap_decode_failed"
#define AMF_EVENT_NAS_5GMM_DECODE_FAILED         "nas_5gmm_decode_failed"
#define AMF_EVENT_NAS_SECURITY_DECODE_FAILED     "nas_security_decode_failed"
#define AMF_EVENT_SBI_SERVER_PARSE_FAILED        "sbi_server_parse_failed"
#define AMF_EVENT_SBI_RESPONSE_PARSE_FAILED      "sbi_response_parse_failed"
#define AMF_EVENT_SBI_RESPONSE_HTTP_ERROR        "sbi_response_http_error"
#define AMF_EVENT_SBI_DISCOVERY_FAILED           "sbi_discovery_failed"
#define AMF_EVENT_SBI_TRANSACTION_MISSING        "sbi_transaction_missing"
#define AMF_EVENT_SBI_TIMEOUT                    "sbi_timeout"
#define AMF_EVENT_AMF_UE_CONTEXT_MISSING         "amf_ue_context_missing"
#define AMF_EVENT_RAN_UE_CONTEXT_MISSING         "ran_ue_context_missing"
#define AMF_EVENT_SESSION_CONTEXT_MISSING         "session_context_missing"
#define AMF_EVENT_TIMER_EXPIRED                  "timer_expired"
#define AMF_EVENT_TIMER_RETRY_EXHAUSTED          "timer_retry_exhausted"
#define AMF_EVENT_NG_SETUP_FAILURE               "ng_setup_failure"

#define AMF_EVENT_OUTCOME_SUCCESS                "success"
#define AMF_EVENT_OUTCOME_FAILURE                "failure"
#define AMF_EVENT_OUTCOME_UNKNOWN                "unknown"

#define AMF_EVENT_TYPE_PROCEDURE                 "procedure"
#define AMF_EVENT_TYPE_DEPENDENCY                "dependency"
#define AMF_EVENT_TYPE_TIMER                     "timer"

typedef struct amf_log_error_s {
    const char *reason;
    int code;
    const char *source;
} amf_log_error_t;

typedef struct amf_log_guti_s {
    const char *raw;
    const char *plmn_id_hex;
    const char *amf_id_hex;
    const char *m_tmsi_hex;
} amf_log_guti_t;

typedef struct amf_log_trace_s {
    const char *trace_id;
    const char *span_id;
    const char *correlation_id;
} amf_log_trace_t;

/*
 * Emit a structured AMF event log line.
 *
 * In JSON mode, produces a single-line JSON object matching the
 * AMF RCA architecture (event, ue, ngap, message, ts, level, domain).
 * In text mode, emits a plain text line via the standard logger.
 *
 * @param level       Log level (OGS_LOG_INFO, OGS_LOG_ERROR, ...)
 * @param event_name  One of the AMF_EVENT_* constants
 * @param outcome     AMF_EVENT_OUTCOME_SUCCESS / _FAILURE / _UNKNOWN
 * @param event_type  AMF_EVENT_TYPE_PROCEDURE / _DEPENDENCY (NULL = "procedure")
 * @param amf_ue      AMF UE context (may be NULL)
 * @param ran_ue      RAN UE context (may be NULL)
 * @param message     Human-readable message (may be NULL)
 */
void amf_log_ue_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message);

void amf_log_ue_error_event(ogs_log_level_e level,
    const char *event_name, const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_error_t *error);

void amf_log_ue_suci_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const char *suci_raw);

void amf_log_ue_guti_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_guti_t *guti);

void amf_log_ue_sbi_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_trace_t *trace);

void amf_log_ue_sbi_error_event(ogs_log_level_e level,
    const char *event_name,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_error_t *error,
    const amf_log_trace_t *trace);

#ifdef __cplusplus
}
#endif

#endif /* AMF_LOG_H */
