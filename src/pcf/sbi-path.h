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

#ifndef PCF_SBI_PATH_H
#define PCF_SBI_PATH_H

#include "nnrf-build.h"
#include "nudr-build.h"
#include "nbsf-build.h"
#include "namf-build.h"
#include "nsmf-build.h"
#include "naf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int pcf_sbi_open(void);
void pcf_sbi_close(void);

bool pcf_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance);

bool pcf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);
bool pcf_ue_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        pcf_ue_t *pcf_ue, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(pcf_ue_t *pcf_ue, void *data));
bool pcf_sess_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        pcf_sess_t *sess, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(pcf_sess_t *sess, void *data));

bool pcf_sbi_send_am_policy_control_notify(pcf_ue_t *pcf_ue);
bool pcf_sbi_send_smpolicycontrol_update_notify(
        pcf_sess_t *sess, OpenAPI_sm_policy_decision_t *SmPolicyDecision);
bool pcf_sbi_send_smpolicycontrol_delete_notify(
        pcf_sess_t *sess, pcf_app_t *app_session,
        OpenAPI_sm_policy_decision_t *SmPolicyDecision);
bool pcf_sbi_send_smpolicycontrol_terminate_notify(pcf_sess_t *sess);

bool pcf_sbi_send_policyauthorization_terminate_notify(pcf_app_t *app);

#ifdef __cplusplus
}
#endif

#endif /* PCF_SBI_PATH_H */
