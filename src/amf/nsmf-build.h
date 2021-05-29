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

#ifndef AMF_NSMF_BUILD_H
#define AMF_NSMF_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct amf_nsmf_pdusession_sm_context_param_s {
    ogs_pkbuf_t *n1smbuf;
    ogs_pkbuf_t *n2smbuf;
    OpenAPI_n2_sm_info_type_e n2SmInfoType;
    OpenAPI_up_cnx_state_e upCnxState;
    struct {
        int group;
        int value;
    } ngApCause;
    union {
        struct {
        ED3(uint8_t ue_location:1;,
            uint8_t ue_timezone:1;,
            uint8_t spare:6;)
        };
        uint8_t indications;
    };

    int release;
    OpenAPI_cause_e cause;
    int gmm_cause;

    OpenAPI_ho_state_e hoState;
    OpenAPI_ng_ran_target_id_t *targetId;
    NGAP_TargetID_t *TargetID;
} amf_nsmf_pdusession_sm_context_param_t;

ogs_sbi_request_t *amf_nsmf_pdusession_build_create_sm_context(
        amf_sess_t *sess, void *data);
ogs_sbi_request_t *amf_nsmf_pdusession_build_update_sm_context(
        amf_sess_t *sess, void *data);
ogs_sbi_request_t *amf_nsmf_pdusession_build_release_sm_context(
        amf_sess_t *sess, void *data);

ogs_sbi_request_t *amf_nsmf_callback_build_n1_n2_failure_notify(
        amf_sess_t *sess, OpenAPI_n1_n2_message_transfer_cause_e cause);

OpenAPI_ng_ran_target_id_t *amf_nsmf_pdusession_build_target_id(
        NGAP_TargetID_t *TargetID);

void amf_nsmf_pdusession_free_target_id(OpenAPI_ng_ran_target_id_t *targetId);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NSMF_BUILD_H */
