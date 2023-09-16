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

#ifndef TEST_S1AP_BUILD_H
#define TEST_S1AP_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *test_s1ap_build_s1_setup_request(
        S1AP_ENB_ID_PR present, uint32_t enb_id);

ogs_pkbuf_t *test_s1ap_build_initial_ue_message(
        test_ue_t *test_ue, ogs_pkbuf_t *emmbuf, uint8_t cause, bool s_tmsi);
ogs_pkbuf_t *test_s1ap_build_uplink_nas_transport(
        test_ue_t *test_ue, ogs_pkbuf_t *nasbuf);
ogs_pkbuf_t *tests1ap_build_ue_radio_capability_info_indication(
        test_ue_t *test_ue);
ogs_pkbuf_t *test_s1ap_build_initial_context_setup_response(test_ue_t *test_ue);
ogs_pkbuf_t *test_s1ap_build_initial_context_setup_failure(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *test_s1ap_build_ue_context_modification_response(
        test_ue_t *test_ue);
ogs_pkbuf_t *test_s1ap_build_ue_context_release_request(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *test_s1ap_build_ue_context_release_complete(test_ue_t *test_ue);
ogs_pkbuf_t *test_s1ap_build_e_rab_setup_response(test_bearer_t *bearer);
ogs_pkbuf_t *test_s1ap_build_e_rab_failed_setup_response(
        test_bearer_t *bearer, S1AP_Cause_PR group, long cause);
ogs_pkbuf_t *test_s1ap_build_e_rab_modify_response(test_bearer_t *bearer);
ogs_pkbuf_t *test_s1ap_build_e_rab_release_response(test_bearer_t *bearer);

ogs_pkbuf_t *test_s1ap_build_e_rab_modification_indication(test_ue_t *test_ue);

ogs_pkbuf_t *test_s1ap_build_path_switch_request(test_ue_t *test_ue);

ogs_pkbuf_t *test_s1ap_build_handover_required(
        test_ue_t *test_ue, S1AP_HandoverType_t handover_type,
        S1AP_ENB_ID_PR present, uint32_t enb_id,
        S1AP_Cause_PR group, long cause);
ogs_pkbuf_t *test_s1ap_build_handover_request_ack(test_ue_t *test_ue);
ogs_pkbuf_t *test_s1ap_build_enb_status_transfer(test_ue_t *test_ue);
ogs_pkbuf_t *test_s1ap_build_handover_notify(test_ue_t *test_ue);

ogs_pkbuf_t *test_s1ap_build_handover_cancel(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause);
ogs_pkbuf_t *test_s1ap_build_handover_failure(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *test_s1ap_build_enb_configuration_transfer(int i);
ogs_pkbuf_t *test_s1ap_build_enb_configuration_update(int i);

ogs_pkbuf_t *test_s1ap_build_malformed_s1_setup_request(int i);
ogs_pkbuf_t *test_s1ap_build_malformed_enb_status_transfer(int i);
ogs_pkbuf_t *test_s1ap_build_malformed_e_rab_modification_indication(int i);
ogs_pkbuf_t *test_s1ap_build_oversized_message(int i);

#ifdef __cplusplus
}
#endif

#endif /* TEST_S1AP_BUILD_H */
