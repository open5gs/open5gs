/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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
#include "nsmf-build.h"
#include "gsm-build.h"
#include "nnrf-build.h"
#include "npcf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int smf_sbi_open(void);
void smf_sbi_close(void);

bool smf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);

/*
 * PFCP delete triggers are defined in lib/pfcp/xact.h (values 1–7).
 * To avoid overlap with OGS_PFCP_DELETE_TRIGGER_*, SMF states use:
 *   - UPDATE_STATE_BASE at 0x10–0x14
 *   - UECM_STATE_BASE   at 0x20–0x23
 * HR flag is bit 7 (0x80).
 */

/* Common HR flag (bit 7) */
#define SMF_STATE_HR             (1U << 7)          /* 0x80 */

/* Base offset for SMF_CREATE states */
#define SMF_CREATE_STATE_BASE       0x10U           /* CREATE at 0x10 */

#define SMF_CREATE_STATE_NONE                       \
    (SMF_CREATE_STATE_BASE + 0x00U)  /* 0x00 */

/* Base offset for SMF_UPDATE states */
#define SMF_UPDATE_STATE_BASE       0x20U           /* UPDATE at 0x20–0x24 */

/* SMF_UPDATE base states */
#define SMF_UPDATE_STATE_NONE                       \
    (SMF_UPDATE_STATE_BASE + 0x00U)  /* 0x00 */
#define SMF_UPDATE_STATE_ACTIVATING                 \
    (SMF_UPDATE_STATE_BASE + 0x01U)  /* 0x01 */
#define SMF_UPDATE_STATE_ACTIVATED_FROM_ACTIVATING  \
    (SMF_UPDATE_STATE_BASE + 0x02U)  /* 0x02 */
#define SMF_UPDATE_STATE_ACTIVATED_FROM_NON_ACTIVATING  \
    (SMF_UPDATE_STATE_BASE + 0x03U)  /* 0x03 */
#define SMF_UPDATE_STATE_ACTIVATED_FROM_XN_HANDOVER \
    (SMF_UPDATE_STATE_BASE + 0x04U)  /* 0x04 */
#define SMF_UPDATE_STATE_ACTIVATED_FROM_N2_HANDOVER \
    (SMF_UPDATE_STATE_BASE + 0x05U)  /* 0x05 */
#define SMF_UPDATE_STATE_DEACTIVATED                \
    (SMF_UPDATE_STATE_BASE + 0x06U)  /* 0x06 */
#define SMF_UPDATE_STATE_UE_REQ_MOD                 \
    (SMF_UPDATE_STATE_BASE + 0x07U)  /* 0x07 */

/* Base offset for SMF_REMOVE states */
#define SMF_REMOVE_STATE_BASE       0x30U           /* REMOVE at 0x30 */

#define SMF_REMOVE_STATE_NONE                       \
    (SMF_REMOVE_STATE_BASE + 0x00U)  /* 0x00 */

/* Base offset for SMF_UECM states */
#define SMF_UECM_STATE_BASE         0x40U           /* UECM at 0x40–0x43 */

/* SMF_UECM base states */
#define SMF_UECM_STATE_NONE                 \
    (SMF_UECM_STATE_BASE + 0x00U)  /* 0x20 */
#define SMF_UECM_STATE_REGISTERED           \
    (SMF_UECM_STATE_BASE + 0x01U)  /* 0x21 */
#define SMF_UECM_STATE_DEREG_BY_AMF         \
    (SMF_UECM_STATE_BASE + 0x02U)  /* 0x22 */
#define SMF_UECM_STATE_DEREG_BY_N1N2        \
    (SMF_UECM_STATE_BASE + 0x03U)  /* 0x23 */

/* HR variants for SMF_UECM */
#define SMF_UECM_STATE_REGISTERED_HR                \
    (SMF_UECM_STATE_REGISTERED | SMF_STATE_HR)      /* 0xC1 */
#define SMF_UECM_STATE_DEREG_BY_AMF_HR              \
    (SMF_UECM_STATE_DEREG_BY_AMF | SMF_STATE_HR)    /* 0xC2 */
#define SMF_UECM_STATE_DEREG_BY_N1N2_HR             \
    (SMF_UECM_STATE_DEREG_BY_N1N2 | SMF_STATE_HR)   /* 0xC3 */

int smf_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(smf_sess_t *sess, void *data),
        smf_sess_t *sess, ogs_sbi_stream_t *stream, int state, void *data);

/**
 * Return true if the PDU session anchor SMF is in the HPLMN
 * (Home-Routed Roaming, HR)
 */
static inline bool smf_uecm_anchor_in_hplmn(int state)
{
    return !!(state & SMF_STATE_HR);
}

/**
 * Return true if the PDU session anchor SMF is in the VPLMN
 * (Non-Roaming or Local Break-Out Roaming, LBO)
 */
static inline bool smf_uecm_anchor_in_vplmn(int state)
{
    return !(state & SMF_STATE_HR);
}

ogs_sbi_xact_t *smf_namf_comm_create_n1_n2_message_xact(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        smf_n1_n2_message_transfer_param_t *param);
void smf_namf_comm_send_n1_n2_message_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        smf_n1_n2_message_transfer_param_t *param);
void smf_namf_comm_send_n1_n2_pdu_establishment_reject(
        smf_sess_t *sess, ogs_sbi_stream_t *stream);

void smf_sbi_send_sm_context_created_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream);
void smf_sbi_send_sm_context_create_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf);

#define smf_sbi_send_sm_context_updated_data_up_cnx_state( \
            __sESS, __sTREAM, __uPCnxState) \
        smf_sbi_send_sm_context_updated_data(\
            __sESS, __sTREAM, __uPCnxState, 0, NULL, 0, NULL)
#define smf_sbi_send_sm_context_updated_data_n2smbuf( \
            __sESS, __sTREAM, __n2Type, __n2SmBuf) \
        smf_sbi_send_sm_context_updated_data(\
            __sESS, __sTREAM, 0, 0, NULL, __n2Type, __n2SmBuf)
#define smf_sbi_send_sm_context_updated_data_ho_state( \
            __sESS, __sTREAM, __hoState) \
        smf_sbi_send_sm_context_updated_data(\
            __sESS, __sTREAM, 0, __hoState, NULL, 0, NULL)
#define smf_sbi_send_sm_context_updated_data_n1_n2_message( \
            __sESS, __sTREAM, __n1SmBuf, __n2Type, __n2SmBuf) \
        smf_sbi_send_sm_context_updated_data(\
            __sESS, __sTREAM, 0, 0, __n1SmBuf, __n2Type, __n2SmBuf)
void smf_sbi_send_sm_context_updated_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        OpenAPI_up_cnx_state_e up_cnx_state,
        OpenAPI_ho_state_e ho_state,
        ogs_pkbuf_t *n1smbuf,
        OpenAPI_n2_sm_info_type_e n2type, ogs_pkbuf_t *n2smbuf);

#define smf_sbi_send_sm_context_update_error_log( \
        __sTREAM,  __sTATUS, __tITLE, __dETAIL) \
        smf_sbi_send_sm_context_update_error( \
                __sTREAM, __sTATUS, OGS_SBI_APP_ERRNO_NULL, \
                __tITLE, __dETAIL, NULL, NULL, \
                OpenAPI_n2_sm_info_type_NULL, OpenAPI_up_cnx_state_NULL)
#define smf_sbi_send_sm_context_update_error_n1_n2_message( \
        __sTREAM,  __sTATUS, __n1SmBuf, __n2Type, __n2SmBuf) \
        smf_sbi_send_sm_context_update_error( \
                __sTREAM, __sTATUS, OGS_SBI_APP_ERRNO_NULL, \
                NULL, NULL, __n1SmBuf, __n2SmBuf, __n2Type, \
                OpenAPI_up_cnx_state_NULL)
#define smf_sbi_send_sm_context_update_app_error( \
        __sTREAM,  __sTATUS, __aPPError, __uPCnxState) \
        smf_sbi_send_sm_context_update_error( \
                __sTREAM, __sTATUS, __aPPError, \
                NULL, NULL, NULL, NULL, \
                OpenAPI_n2_sm_info_type_NULL, __uPCnxState);
void smf_sbi_send_sm_context_update_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf,
        OpenAPI_n2_sm_info_type_e n2_sm_info_type,
        OpenAPI_up_cnx_state_e up_cnx_state);

/* Cleanup modes for session resources via SBI */
typedef enum {
    SMF_SBI_CLEANUP_MODE_POLICY_FIRST = 1,   /* Policy→Subscr→Context */
    SMF_SBI_CLEANUP_MODE_SUBSCRIPTION_FIRST, /* Subscr→Context */
    SMF_SBI_CLEANUP_MODE_CONTEXT_ONLY        /* Context only */
} smf_sbi_cleanup_mode_t;

/**
 * Send SBI request to clean up session resources.
 *
 * @param sess   Session object pointer.
 * @param stream SBI stream object pointer.
 * @param state  State code for the cleanup action.
 * @param mode   Cleanup mode to select the workflow.
 * @return OGS_OK on success, else OGS_ERROR.
 */
int smf_sbi_cleanup_session(
    smf_sess_t              *sess,
    ogs_sbi_stream_t        *stream,
    int                      state,
    smf_sbi_cleanup_mode_t   mode);

bool smf_sbi_send_sm_context_status_notify(smf_sess_t *sess);

void smf_sbi_send_pdu_session_created_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream);
void smf_sbi_send_pdu_session_create_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err, int n1SmCause,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1SmBufToUe);

void smf_sbi_send_hsmf_update_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err, int n1SmCause,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1SmBufToUe);
void smf_sbi_send_vsmf_update_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err, int n1SmCause,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1SmBufFromUe);

void smf_sbi_send_released_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream);

bool smf_sbi_send_status_notify(smf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* SMF_SBI_PATH_H */
