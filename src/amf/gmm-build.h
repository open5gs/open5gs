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

#ifndef GMM_BUILD_H
#define GMM_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *gmm_build_registration_accept(amf_ue_t *amf_ue);
ogs_pkbuf_t *gmm_build_registration_reject(ogs_nas_5gmm_cause_t gmm_cause);

ogs_pkbuf_t *gmm_build_service_accept(amf_ue_t *amf_ue);
ogs_pkbuf_t *gmm_build_service_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause);

ogs_pkbuf_t *gmm_build_de_registration_accept(amf_ue_t *amf_ue);

ogs_pkbuf_t *gmm_build_identity_request(amf_ue_t *amf_ue);
ogs_pkbuf_t *gmm_build_security_mode_command(amf_ue_t *amf_ue);

ogs_pkbuf_t *gmm_build_authentication_request(amf_ue_t *amf_ue);
ogs_pkbuf_t *gmm_build_authentication_reject(void);

typedef struct gmm_configuration_update_command_param_s {
    int registration_requested;
    int acknowledgement_requested;
    int nitz;
} gmm_configuration_update_command_param_t;

ogs_pkbuf_t *gmm_build_configuration_update_command(
        amf_ue_t *amf_ue, gmm_configuration_update_command_param_t *param);

ogs_pkbuf_t *gmm_build_dl_nas_transport(amf_sess_t *sess,
        uint8_t payload_container_type, ogs_pkbuf_t *payload,
        ogs_nas_5gmm_cause_t cause, uint8_t backoff_time);

ogs_pkbuf_t *gmm_build_tau_accept(amf_ue_t *amf_ue);
ogs_pkbuf_t *gmm_build_tau_reject(
        ogs_nas_5gmm_cause_t gmm_cause, amf_ue_t *amf_ue);

ogs_pkbuf_t *gmm_build_cs_service_notification(amf_ue_t *amf_ue);
ogs_pkbuf_t *gmm_build_downlink_nas_transport(
        amf_ue_t *amf_ue, uint8_t *buffer, uint8_t length);

ogs_pkbuf_t *gmm_build_status(amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t cause);

#ifdef __cplusplus
}
#endif

#endif /* GMM_BUILD_H */
