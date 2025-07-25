/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef AMF_SBI_PATH_H
#define AMF_SBI_PATH_H

#include "nausf-build.h"
#include "nudm-build.h"
#include "nsmf-build.h"
#include "npcf-build.h"
#include "nnssf-build.h"
#include "nnrf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int amf_sbi_open(void);
void amf_sbi_close(void);

bool amf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);

#define AMF_CREATE_SM_CONTEXT_NO_STATE                  0

#define AMF_UE_INITIATED_DE_REGISTERED                  1
#define AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED    2
#define AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED    3

#define AMF_UPDATE_SM_CONTEXT_ACTIVATED                 11
#define AMF_UPDATE_SM_CONTEXT_SETUP_FAIL                12
#define AMF_UPDATE_SM_CONTEXT_DEACTIVATED               13
#define AMF_UPDATE_SM_CONTEXT_REGISTRATION_REQUEST      14
#define AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST           15
#define AMF_UPDATE_SM_CONTEXT_MODIFIED                  16
#define AMF_UPDATE_SM_CONTEXT_N2_RELEASED               17
#define AMF_UPDATE_SM_CONTEXT_N1_RELEASED               18
#define AMF_UPDATE_SM_CONTEXT_DUPLICATED_PDU_SESSION_ID 19
#define AMF_UPDATE_SM_CONTEXT_PATH_SWITCH_REQUEST       20
#define AMF_UPDATE_SM_CONTEXT_HANDOVER_REQUIRED         21
#define AMF_UPDATE_SM_CONTEXT_HANDOVER_REQ_ACK          22
#define AMF_UPDATE_SM_CONTEXT_HANDOVER_NOTIFY           23
#define AMF_UPDATE_SM_CONTEXT_HANDOVER_CANCEL           24
#define AMF_RELEASE_SM_CONTEXT_NO_STATE                 31
#define AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT      33
#define AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT           34
#define AMF_REMOVE_S1_CONTEXT_BY_LO_CONNREFUSED         51
#define AMF_REMOVE_S1_CONTEXT_BY_RESET_ALL              52
#define AMF_REMOVE_S1_CONTEXT_BY_RESET_PARTIAL          53

/*
 * TS23.502
 * 4.3.2.2.3 SMF selection
 */
#define AMF_SMF_SELECTION_IN_VPLMN_IN_NON_ROAMING_OR_LBO    1
#define AMF_SMF_SELECTION_IN_VPLMN_IN_HOME_ROUTED           2
#define AMF_SMF_SELECTION_IN_HPLMN_IN_HOME_ROUTED           3

int amf_ue_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(amf_ue_t *amf_ue, void *data),
        amf_ue_t *amf_ue, int state, void *data);
int amf_sess_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(amf_sess_t *sess, void *data),
        ran_ue_t *ran_ue, amf_sess_t *sess, int state, void *data);

int amf_sess_sbi_discover_by_nsi(
        ran_ue_t *ran_ue, amf_sess_t *sess,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option, int state);

void amf_sbi_send_activating_session(
        ran_ue_t *ran_ue, amf_sess_t *sess, int state);

void amf_sbi_send_deactivate_session(
        ran_ue_t *ran_ue, amf_sess_t *sess, int state, int group, int cause);
void amf_sbi_send_deactivate_all_sessions(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue, int state, int group, int cause);
void amf_sbi_send_deactivate_all_ue_in_gnb(amf_gnb_t *gnb, int state);

void amf_sbi_send_release_session(
        ran_ue_t *ran_ue, amf_sess_t *sess, int state, void *data);
void amf_sbi_send_release_all_sessions(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue, int state, void *data);

bool amf_sbi_send_n1_n2_failure_notify(
        amf_sess_t *sess, OpenAPI_n1_n2_message_transfer_cause_e cause);

bool amf_ue_have_session_release_pending(amf_ue_t *amf_ue);
bool amf_sess_have_session_release_pending(amf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* AMF_SBI_PATH_H */
