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

#ifndef TEST_PACKET_H
#define TEST_PACKET_H

#include "ogs-s1ap.h"
#include "mme/s1ap-build.h"
#include "core/abts.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool test_no_mme_self;

ogs_socknode_t *testsctp_server(const char *ipstr);
ogs_socknode_t *testsctp_client(const char *ipstr);
ogs_pkbuf_t *testsctp_read(ogs_socknode_t *node, int type);

#define testenb_s1ap_client testsctp_client
#define testenb_s1ap_read(x) testsctp_read(x, 0);
int testenb_s1ap_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf);
#define testenb_s1ap_close ogs_socknode_free

#define testvlr_sgsap_server testsctp_server
#define testvlr_sgsap_read(x) testsctp_read(x, 1);
int testvlr_sgsap_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf);
#define testvlr_sgsap_close ogs_socknode_free

ogs_socknode_t *testenb_gtpu_server(const char *ipstr);
ogs_pkbuf_t *testenb_gtpu_read(ogs_socknode_t *node);
int testenb_gtpu_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf);
void testenb_gtpu_close(ogs_socknode_t *node);

int tests1ap_build_invalid_packet(ogs_pkbuf_t **pkbuf, int i);

int tests1ap_build_setup_req(
        ogs_pkbuf_t **pkbuf, S1AP_ENB_ID_PR present, uint32_t enb_id,
        int tac, uint16_t mcc, uint16_t mnc, uint16_t mnc_len);
int tests1ap_build_initial_ue_msg(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_identity_response(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_authentication_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_authentication_failure(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_security_mode_complete(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_esm_information_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_ue_capability_info_indication(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_initial_context_setup_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t ebi, uint32_t teid, const char *ipstr);
int tests1ap_build_ue_context_modification_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id);
int tests1ap_build_initial_context_setup_failure(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_attach_complete(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_emm_status(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_detach_request(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_ue_context_release_request(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_ue_context_release_complete(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_service_request(ogs_pkbuf_t **pkbuf,
        uint32_t enb_ue_s1ap_id, uint8_t seq,
        uint16_t mac, uint32_t m_tmsi);
int tests1ap_build_tau_request(ogs_pkbuf_t **pkbuf, int i,
    uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id, uint8_t active_flag,
    uint32_t m_tmsi, uint8_t seq, uint32_t mac, uint8_t *knas_int);
int tests1ap_build_extended_service_request(ogs_pkbuf_t **pkbuf, int i,
        uint8_t service_type, uint32_t m_tmsi, uint8_t seq, uint8_t *knas_int);
int tests1ap_build_pdn_connectivity_request(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_pdn_disconnectivity_request(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_e_rab_setup_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t ebi, uint32_t teid, const char *ipstr);
int tests1ap_build_e_rab_modify_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_e_rab_release_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_activate_default_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_activate_dedicated_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_modify_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_deactivate_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_bearer_resource_allocation_request(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_bearer_resource_modification_request(
        ogs_pkbuf_t **pkbuf,
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t pti, uint32_t mac, uint8_t seq, uint8_t ebi,
        uint8_t tft_code, uint8_t qci,
        uint8_t ul_mbr, uint8_t dl_mbr, uint8_t ul_gbr, uint8_t dl_gbr);
int tests1ap_build_bearer_resource_modification_request2(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_path_switch_request(
        ogs_pkbuf_t **pkbuf, int target, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        int num_of_bearer, uint8_t ebi, uint32_t teid,
        const char *ipstr1, const char *ipstr2);
int tests1ap_build_handover_required(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_failure(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_request_ack(
        ogs_pkbuf_t **pkbuf, int target,
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        int num_of_bearer, uint8_t ebi, uint32_t teid,
        const char *ipstr1, const char *ipstr2);
int tests1ap_build_handover_request_ack_static(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_enb_status_transfer(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_enb_configuration_transfer(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_notify(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_cancel(ogs_pkbuf_t **pkbuf, int i);

int tests1ap_build_s1_reset(ogs_pkbuf_t **pkbuf, int i);

int tests1ap_build_uplink_nas_transport(ogs_pkbuf_t **pkbuf, int i);

int testgtpu_build_ping(ogs_pkbuf_t **sendbuf,
        const uint32_t teid, const char *src_ip, const char *dst_ip);
int testgtpu_build_slacc_rs(ogs_pkbuf_t **sendbuf, int i);

int testsgsap_location_update_accept(ogs_pkbuf_t **pkbuf, int i);
int testsgsap_location_update_reject(ogs_pkbuf_t **pkbuf, int i);
int testsgsap_imsi_detach_ack(ogs_pkbuf_t **pkbuf, int i);
int testsgsap_paging_request(ogs_pkbuf_t **pkbuf, int i);
int testsgsap_reset_indication(ogs_pkbuf_t **pkbuf, int i);
int testsgsap_release_request(ogs_pkbuf_t **pkbuf, int i);
int testsgsap_downlink_unitdata(ogs_pkbuf_t **pkbuf, int i);
int testsgsap_mm_information_request(ogs_pkbuf_t **pkbuf, int i);

#ifdef __cplusplus
}
#endif

#endif /* TEST_PACKET_H */
