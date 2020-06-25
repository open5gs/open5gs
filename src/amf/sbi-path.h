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

#ifdef __cplusplus
extern "C" {
#endif

int amf_sbi_open(void);
void amf_sbi_close(void);

void amf_sbi_setup_client_callback(ogs_sbi_nf_instance_t *nf_instance);

void amf_ue_sbi_discover_and_send(
        OpenAPI_nf_type_e nf_type, amf_ue_t *amf_ue, void *data,
        ogs_sbi_request_t *(*build)(amf_ue_t *amf_ue, void *data));
void amf_sess_sbi_discover_and_send(
        OpenAPI_nf_type_e nf_type, amf_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(amf_sess_t *sess, void *data));

void amf_sbi_send_activating_session(amf_sess_t *sess);

void amf_sbi_send_deactivate_session(
        amf_sess_t *sess, int group, int cause);
void amf_sbi_send_deactivate_all_sessions(
        amf_ue_t *amf_ue, int group, int cause);

void amf_sbi_send_release_session(amf_sess_t *sess);
void amf_sbi_send_release_all_sessions(amf_ue_t *amf_ue);

#ifdef __cplusplus
}
#endif

#endif /* AMF_SBI_PATH_H */
