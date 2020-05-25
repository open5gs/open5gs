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

#ifndef NGAP_BUILD_H
#define NGAP_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ngap_build_setup_rsp(void);
ogs_pkbuf_t *ngap_build_setup_failure(
    NGAP_Cause_PR group, long cause, long time_to_wait);

#if 0
ogs_pkbuf_t *ngap_build_downlink_nas_transport(
    gnb_ue_t *gnb_ue, ogs_pkbuf_t *emmbuf);

ogs_pkbuf_t *ngap_build_initial_context_setup_request(
    amf_ue_t *amf_ue, ogs_pkbuf_t *emmbuf);
ogs_pkbuf_t *ngap_build_ue_context_modification_request(amf_ue_t *amf_ue);
ogs_pkbuf_t *ngap_build_ue_context_release_command(
    gnb_ue_t *gnb_ue, NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_build_e_rab_setup_request(
    amf_bearer_t *bearer, ogs_pkbuf_t *esmbuf);
ogs_pkbuf_t *ngap_build_e_rab_modify_request(
    amf_bearer_t *bearer, ogs_pkbuf_t *esmbuf);
ogs_pkbuf_t *ngap_build_e_rab_release_command(
    amf_bearer_t *bearer, ogs_pkbuf_t *esmbuf, NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_build_paging(
        amf_ue_t *amf_ue, NGAP_CNDomain_t cn_domain);

ogs_pkbuf_t *ngap_build_amf_configuration_transfer(
    NGAP_SONConfigurationTransfer_t *son_configuration_transfer);

ogs_pkbuf_t *ngap_build_path_switch_ack(amf_ue_t *amf_ue);
ogs_pkbuf_t *ngap_build_path_switch_failure(
    uint32_t gnb_ue_ngap_id, uint32_t amf_ue_ngap_id,
    NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_build_handover_command(gnb_ue_t *source_ue);
ogs_pkbuf_t *ngap_build_handover_preparation_failure(
    gnb_ue_t *source_ue, NGAP_Cause_t *cause);

ogs_pkbuf_t *ngap_build_handover_request(
    amf_ue_t *amf_ue, gnb_ue_t *target_ue,
    NGAP_ENB_UE_NGAP_ID_t *gnb_ue_ngap_id,
    NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
    NGAP_HandoverType_t *handovertype,
    NGAP_Cause_t *cause,
    NGAP_Source_ToTarget_TransparentContainer_t
        *source_totarget_transparentContainer);

ogs_pkbuf_t *ngap_build_handover_cancel_ack(
    gnb_ue_t *source_ue);

ogs_pkbuf_t *ngap_build_amf_status_transfer(
    gnb_ue_t *target_ue,
    NGAP_ENB_StatusTransfer_TransparentContainer_t
        *gnb_statustransfer_transparentContainer);

ogs_pkbuf_t *ngap_build_error_indication(
    NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
    NGAP_ENB_UE_NGAP_ID_t *gnb_ue_ngap_id,
    NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ngap_build_s1_reset(
    NGAP_Cause_PR group, long cause,
    NGAP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface);

ogs_pkbuf_t *ngap_build_s1_reset_partial(
    NGAP_Cause_PR group, long cause,
    NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
    NGAP_ENB_UE_NGAP_ID_t *gnb_ue_ngap_id);

ogs_pkbuf_t *ngap_build_s1_reset_ack(
    NGAP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface);

ogs_pkbuf_t *ngap_build_write_replace_warning_request(
    sbc_pws_data_t *sbc_pws);

ogs_pkbuf_t *ngap_build_kill_request(sbc_pws_data_t *sbc_pws);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NGAP_BUILD_H */
