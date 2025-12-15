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

#ifndef GMM_HANDLER_H
#define GMM_HANDLER_H

#include "context.h"
#include "namf-handler.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_nas_5gmm_cause_t gmm_handle_registration_request(amf_ue_t *amf_ue,
        ogs_nas_security_header_type_t h, NGAP_ProcedureCode_t ngap_code,
        ogs_nas_5gs_registration_request_t *registration_request);
ogs_nas_5gmm_cause_t gmm_handle_registration_update(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue,
        ogs_nas_5gs_registration_request_t *registration_request);
bool gmm_registration_request_from_old_amf(amf_ue_t *amf_ue,
        ogs_nas_5gs_registration_request_t *registration_request);

ogs_nas_5gmm_cause_t gmm_handle_service_request(amf_ue_t *amf_ue,
        ogs_nas_security_header_type_t h, NGAP_ProcedureCode_t ngap_code,
        ogs_nas_5gs_service_request_t *service_request);
ogs_nas_5gmm_cause_t gmm_handle_service_update(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue,
        ogs_nas_5gs_service_request_t *service_request);

int gmm_handle_deregistration_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_deregistration_request_from_ue_t *deregistration_request);

int gmm_handle_authentication_response(amf_ue_t *amf_ue,
        ogs_nas_5gs_authentication_response_t *authentication_response);

ogs_nas_5gmm_cause_t gmm_handle_identity_response(amf_ue_t *amf_ue,
        ogs_nas_5gs_identity_response_t *identity_response);

ogs_nas_5gmm_cause_t gmm_handle_security_mode_complete(amf_ue_t *amf_ue,
        ogs_nas_5gs_security_mode_complete_t *security_mode_complete);

int gmm_handle_ul_nas_transport(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue,
        ogs_nas_5gs_ul_nas_transport_t *ul_nas_transport);

#ifdef __cplusplus
}
#endif

#endif /* GMM_HANDLER_H */
