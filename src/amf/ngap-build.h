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

ogs_pkbuf_t *ngap_build_downlink_nas_transport(
    ran_ue_t *ran_ue, ogs_pkbuf_t *gmmbuf);

ogs_pkbuf_t *ngap_build_initial_context_setup_request(
    amf_ue_t *amf_ue, ogs_pkbuf_t *gmmbuf);
ogs_pkbuf_t *ngap_build_ue_context_modification_request(amf_ue_t *amf_ue);
ogs_pkbuf_t *ngap_build_ue_context_release_command(
    ran_ue_t *ran_ue, NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_build_pdu_session_resource_setup_request(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf);
ogs_pkbuf_t *ngap_build_pdu_session_resource_release_command(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf);

#if 0
ogs_pkbuf_t *ngap_build_paging(
        amf_ue_t *amf_ue, NGAP_CNDomain_t cn_domain);
#endif

ogs_pkbuf_t *ngap_build_amf_configuration_transfer(
    NGAP_SONConfigurationTransfer_t *son_configuration_transfer);

ogs_pkbuf_t *ngap_build_path_switch_ack(amf_ue_t *amf_ue);
ogs_pkbuf_t *ngap_build_path_switch_failure(
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id,
    NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_build_handover_command(ran_ue_t *source_ue);
ogs_pkbuf_t *ngap_build_handover_preparation_failure(
    ran_ue_t *source_ue, NGAP_Cause_t *cause);

ogs_pkbuf_t *ngap_build_handover_request(
    amf_ue_t *amf_ue, ran_ue_t *target_ue,
    NGAP_RAN_UE_NGAP_ID_t *ran_ue_ngap_id,
    NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
    NGAP_HandoverType_t *handovertype,
    NGAP_Cause_t *cause,
    NGAP_SourceToTarget_TransparentContainer_t
        *source_totarget_transparentContainer);

ogs_pkbuf_t *ngap_build_handover_cancel_ack(
    ran_ue_t *source_ue);

ogs_pkbuf_t *ngap_build_amf_status_transfer(
    ran_ue_t *target_ue,
    NGAP_RANStatusTransfer_TransparentContainer_t
        *gnb_statustransfer_transparentContainer);

ogs_pkbuf_t *ngap_build_error_indication(
    uint32_t *ran_ue_ngap_id,
    uint64_t *amf_ue_ngap_id,
    NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_build_s1_reset(
    NGAP_Cause_PR group, long cause,
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfS1_Interface);

ogs_pkbuf_t *ngap_build_s1_reset_partial(
    NGAP_Cause_PR group, long cause,
    NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
    NGAP_RAN_UE_NGAP_ID_t *ran_ue_ngap_id);

ogs_pkbuf_t *ngap_build_s1_reset_ack(
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfS1_Interface);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NGAP_BUILD_H */
