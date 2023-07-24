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
int smf_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(smf_sess_t *sess, void *data),
        smf_sess_t *sess, ogs_sbi_stream_t *stream, int state, void *data);

void smf_namf_comm_send_n1_n2_message_transfer(
        smf_sess_t *sess, smf_n1_n2_message_transfer_param_t *param);
void smf_namf_comm_send_n1_n2_pdu_establishment_reject(
        smf_sess_t *sess);

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

bool smf_sbi_send_sm_context_status_notify(smf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* SMF_SBI_PATH_H */
