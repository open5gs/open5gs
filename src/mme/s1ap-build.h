/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef S1AP_BUILD_H
#define S1AP_BUILD_H

#include "mme-context.h"

#include "sbc-message.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *s1ap_build_setup_rsp(void);
ogs_pkbuf_t *s1ap_build_setup_failure(
    S1AP_Cause_PR group, long cause, long time_to_wait);

ogs_pkbuf_t *s1ap_build_downlink_nas_transport(
    enb_ue_t *enb_ue, ogs_pkbuf_t *emmbuf);

ogs_pkbuf_t *s1ap_build_initial_context_setup_request(
    mme_ue_t *mme_ue, ogs_pkbuf_t *emmbuf);
ogs_pkbuf_t *s1ap_build_ue_context_modification_request(mme_ue_t *mme_ue);
ogs_pkbuf_t *s1ap_build_ue_context_release_command(
    enb_ue_t *enb_ue, S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *s1ap_build_e_rab_setup_request(
    mme_bearer_t *bearer, ogs_pkbuf_t *esmbuf);
ogs_pkbuf_t *s1ap_build_e_rab_modify_request(
    mme_bearer_t *bearer, ogs_pkbuf_t *esmbuf);
ogs_pkbuf_t *s1ap_build_e_rab_release_command(
    mme_bearer_t *bearer, ogs_pkbuf_t *esmbuf, S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *s1ap_build_e_rab_modification_confirm(mme_ue_t *mme_ue);

ogs_pkbuf_t *s1ap_build_paging(
        mme_ue_t *mme_ue, S1AP_CNDomain_t cn_domain);

ogs_pkbuf_t *s1ap_build_mme_configuration_transfer(
    S1AP_SONConfigurationTransfer_t *son_configuration_transfer);

ogs_pkbuf_t *s1ap_build_path_switch_ack(mme_ue_t *mme_ue);
ogs_pkbuf_t *s1ap_build_path_switch_failure(
    uint32_t enb_ue_s1ap_id, uint32_t mme_ue_s1ap_id,
    S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *s1ap_build_handover_command(enb_ue_t *source_ue);
ogs_pkbuf_t *s1ap_build_handover_preparation_failure(
        enb_ue_t *source_ue, S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *s1ap_build_handover_request(
    enb_ue_t *target_ue, S1AP_HandoverType_t *handovertype, S1AP_Cause_t *cause,
    S1AP_Source_ToTarget_TransparentContainer_t
        *source_totarget_transparentContainer);

ogs_pkbuf_t *s1ap_build_handover_cancel_ack(
    enb_ue_t *source_ue);

ogs_pkbuf_t *s1ap_build_mme_status_transfer(
    enb_ue_t *target_ue,
    S1AP_ENB_StatusTransfer_TransparentContainer_t
        *enb_statustransfer_transparentContainer);

ogs_pkbuf_t *s1ap_build_write_replace_warning_request(
    sbc_pws_data_t *sbc_pws);

ogs_pkbuf_t *s1ap_build_kill_request(sbc_pws_data_t *sbc_pws);

#ifdef __cplusplus
}
#endif

#endif /* S1AP_BUILD_H */
