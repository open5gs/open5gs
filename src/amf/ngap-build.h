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

#ifndef AMF_NGAP_BUILD_H
#define AMF_NGAP_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ngap_build_ng_setup_response(void);
ogs_pkbuf_t *ngap_build_ng_setup_failure(
    NGAP_Cause_PR group, long cause, long time_to_wait);

ogs_pkbuf_t *ngap_build_ran_configuration_update_ack(void);
ogs_pkbuf_t *ngap_build_ran_configuration_update_failure(
        NGAP_Cause_PR group, long cause, long time_to_wait);

ogs_pkbuf_t *ngap_build_downlink_nas_transport(
    ran_ue_t *ran_ue, ogs_pkbuf_t *gmmbuf, bool ue_ambr, bool allowed_nssai);

ogs_pkbuf_t *ngap_ue_build_initial_context_setup_request(
    amf_ue_t *amf_ue, ogs_pkbuf_t *gmmbuf);
ogs_pkbuf_t *ngap_sess_build_initial_context_setup_request(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf);
ogs_pkbuf_t *ngap_build_ue_context_release_command(
    ran_ue_t *ran_ue, NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_ue_build_pdu_session_resource_setup_request(
    amf_ue_t *amf_ue, ogs_pkbuf_t *gmmbuf);
ogs_pkbuf_t *ngap_sess_build_pdu_session_resource_setup_request(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf);
ogs_pkbuf_t *ngap_build_pdu_session_resource_modify_request(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf);
ogs_pkbuf_t *ngap_build_pdu_session_resource_release_command(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf);

ogs_pkbuf_t *ngap_build_paging(amf_ue_t *amf_ue);

ogs_pkbuf_t *ngap_build_downlink_ran_configuration_transfer(
    NGAP_SONConfigurationTransfer_t *transfer);

ogs_pkbuf_t *ngap_build_path_switch_ack(amf_ue_t *amf_ue);

ogs_pkbuf_t *ngap_build_handover_request(ran_ue_t *target_ue);
ogs_pkbuf_t *ngap_build_handover_preparation_failure(
    ran_ue_t *source_ue, NGAP_Cause_t *cause);

ogs_pkbuf_t *ngap_build_handover_command(ran_ue_t *source_ue);
ogs_pkbuf_t *ngap_build_handover_cancel_ack(ran_ue_t *source_ue);

ogs_pkbuf_t *ngap_build_uplink_ran_status_transfer(
    ran_ue_t *target_ue,
    NGAP_RANStatusTransfer_TransparentContainer_t *transfer);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NGAP_BUILD_H */
