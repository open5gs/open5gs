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

#ifndef AMF_SBI_PATH_H
#define AMF_SBI_PATH_H

#include "nausf-build.h"
#include "nudm-build.h"
#include "nsmf-build.h"
#include "npcf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int amf_sbi_open(void);
void amf_sbi_close(void);

void amf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);
void amf_ue_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        amf_ue_t *amf_ue, void *data,
        ogs_sbi_request_t *(*build)(amf_ue_t *amf_ue, void *data));

#define AMF_UPDATE_SM_CONTEXT_NO_STATE          0
#define AMF_UPDATE_SM_CONTEXT_ACTIVATED         1
#define AMF_UPDATE_SM_CONTEXT_DEACTIVATED       2
#define AMF_UPDATE_SM_CONTEXT_ACTIVATING        3
#define AMF_UPDATE_SM_CONTEXT_SUSPENDED         4
#define AMF_UPDATE_SM_CONTEXT_N2_RELEASED       5
#define AMF_UPDATE_SM_CONTEXT_N1_RELEASED       6
#define AMF_UPDATE_SM_CONTEXT_NG_RESET          7
void amf_sess_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        amf_sess_t *sess, int state, void *data,
        ogs_sbi_request_t *(*build)(amf_sess_t *sess, void *data));

void amf_sbi_send_activating_session(amf_sess_t *sess);

void amf_sbi_send_deactivate_session(
        amf_sess_t *sess, int state, int group, int cause);
void amf_sbi_send_deactivate_all_sessions(
        amf_ue_t *amf_ue, int state, int group, int cause);
void amf_sbi_send_deactivate_all_ue_in_gnb(amf_gnb_t *gnb, int state);

#define AMF_RELEASE_SM_CONTEXT_NO_STATE             0
#define AMF_RELEASE_SM_CONTEXT_NG_CONTEXT_REMOVE    1
#define AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT  2
#define AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT       3
void amf_sbi_send_release_session(amf_sess_t *sess, int state);
void amf_sbi_send_release_all_sessions(amf_ue_t *amf_ue, int state);

#ifdef __cplusplus
}
#endif

#endif /* AMF_SBI_PATH_H */
