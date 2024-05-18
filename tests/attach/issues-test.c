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

#include "test-common.h"

static void issues_1431_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t old_enb_ue_s1ap_id;
    uint64_t old_mme_ue_s1ap_id;
    uint32_t new_enb_ue_s1ap_id;
    uint64_t new_mme_ue_s1ap_id;

    bson_t *doc = NULL;

    /* Setup Test UE & Session Context */
    memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

    mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    mobile_identity_suci.routing_indicator1 = 0;
    mobile_identity_suci.routing_indicator2 = 0xf;
    mobile_identity_suci.routing_indicator3 = 0xf;
    mobile_identity_suci.routing_indicator4 = 0xf;
    mobile_identity_suci.protection_scheme_id = OGS_PROTECTION_SCHEME_NULL;
    mobile_identity_suci.home_network_pki_value = 0;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, "3746000006");
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x00003c01;
    test_ue->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_EPS_ATTACH;

    test_ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess = test_sess_add_by_apn(test_ue, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);

    /* eNB connects to MME */
    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /********** Insert Subscriber in Database */
    doc = test_db_new_simple(test_ue);
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue, doc));

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue->attach_request_param,
            0, sizeof(test_ue->attach_request_param));
    test_ue->attach_request_param.ue_usage_setting = 1;
    test_ue->attach_request_param.ms_network_feature_support = 1;
    emmbuf = testemm_build_attach_request(test_ue, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue->initial_ue_param, 0, sizeof(test_ue->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*** Store eNB-UE-S1AP-ID */
    old_enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id;

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /*** Store MME-UE-S1AP-ID */
    old_mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id;

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Security mode complete */
    test_ue->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send ESM Information Response */
    sess->esm_information_param.pco = 1;
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Wait for Modify Bearer Request/Response */
    ogs_msleep(100);

    /* Send PDN Connectivity Request */
    sess = test_sess_find_by_apn(test_ue, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 2;

    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.apn = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, true, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive E-RABSetupRequest +
     * Activate default EPS bearer context request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_downlinkNASTransport,
            test_ue->s1ap_procedure_code);

    /* Just Delay */
    ogs_msleep(100);

    /* Send Attach Request - Integrity */
    sess = test_sess_find_by_apn(test_ue, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 3;

    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue->attach_request_param,
            0, sizeof(test_ue->attach_request_param));
    test_ue->attach_request_param.guti = 1;
    test_ue->attach_request_param.last_visited_registered_tai = 1;
    test_ue->attach_request_param.ue_usage_setting = 1;
    test_ue->attach_request_param.old_guti_type = 1;
    test_ue->attach_request_param.ms_network_feature_support = 1;
    emmbuf = testemm_build_attach_request(test_ue, esmbuf, true, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*** Store eNB-UE-S1AP-ID */
    new_enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id;

    /* Receive OLD UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /*** Store MME-UE-S1AP-ID */
    new_mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id;

    /* Send OLD UE Context Release Complete */
    test_ue->enb_ue_s1ap_id = old_enb_ue_s1ap_id;
    test_ue->mme_ue_s1ap_id = old_mme_ue_s1ap_id;

    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Just Delay */
    ogs_msleep(100);

    /* Send ESM Information Response */
    test_ue->enb_ue_s1ap_id = new_enb_ue_s1ap_id;
    test_ue->mme_ue_s1ap_id = new_mme_ue_s1ap_id;

    sess->esm_information_param.pco = 0;
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Context Release Request */
    sendbuf = test_s1ap_build_ue_context_release_request(test_ue,
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_user_inactivity);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove(test_ue);
}

/*
- v2.6.3 Analysis
05/03 13:01:08.318 1101136515 486875/209 12/322  Attach request
05/03 13:01:08.318 1101136515 486875/209 12/322  emm_state_initial_context_setup(): ENTRY
05/03 13:01:08.318 1101136515 486875/209 12/322  mme_bearer_find_or_add_by_message() [PTI:7, EBI:0]
05/03 13:01:08.318 1101136515 486875/209 12/322  esm_state_inactive(): ENTRY
05/03 13:01:08.318 1101136515 486875/209 12/322  PDN Connectivity request
05/03 13:01:08.318 1101136515 486875/209 12/322  ESM information request
05/03 13:01:08.331 1101133226 486874/881 582/1029  mme_bearer_find_or_add_by_message() [PTI:2, EBI:0]
05/03 13:01:08.331 1101133226 486874/881 582/1029  esm_state_inactive(): ENTRY
05/03 13:01:08.331 1101133226 486874/881 582/1029  PDN Connectivity request
05/03 13:01:08.331 1101133226 486874/881 582/1029  PDN connectivity reject(ims)
05/03 13:01:08.331 1101133226 486874/881 582/1029  esm_state_exception(): ENTRY
05/03 13:01:08.331 1101133226 486874/881 582/1029  [Removed] Number of MME-Sessions is now 28
05/03 13:01:08.335 1101136515 486875/209 12/322  mme_bearer_find_or_add_by_message() [PTI:7, EBI:0]
05/03 13:01:08.335 1101136515 486875/209 12/322  ESM information response
05/03 13:01:08.338 1101136515 486875/209 12/322  Attach accept
05/03 13:01:08.338 1101136515 486875/209 12/322  Activate default bearer context request
05/03 13:01:08.338 1101136515 486875/209 12/322  InitialContextSetupRequest
05/03 13:01:08.631 1101136515 486875/209 12/322  InitialContextSetupResponse
05/03 13:01:08.633 1101136515 486875/209 12/322  UECapabilityInfoIndication
05/03 13:01:08.636 1101136515 486875/209 12/322  emm_state_initial_context_setup(): MME_EVENT_EMM_MESSAGE
05/03 13:01:08.636 1101136515 486875/209 12/322  Attach complete
05/03 13:01:08.636 1101136515 486875/209 12/322  EMM information
05/03 13:01:08.636 1101136515 486875/209 12/322  mme_bearer_find_or_add_by_message() [PTI:0, EBI:5]
05/03 13:01:08.636 1101136515 486875/209 12/322  esm_state_inactive(): MME_EVENT_ESM_MESSAGE
05/03 13:01:08.636 1101136515 486875/209 12/322  Activate default EPS bearer context accept
05/03 13:01:08.636 1101136515 486875/209 12/322  esm_state_active(): ENTRY
05/03 13:01:08.671 1101136515 486875/209 12/322  mme_bearer_find_or_add_by_message() [PTI:8, EBI:0]
05/03 13:01:08.671 1101136515 486875/209 12/322  esm_state_inactive(): ENTRY
05/03 13:01:08.671 1101136515 486875/209 12/322  PDN Connectivity request
05/03 13:01:08.671 1101136515 486875/209 12/322  PDN connectivity reject(ims)
05/03 13:01:08.671 1101136515 486875/209 12/322  esm_state_exception(): ENTRY
05/03 13:01:08.671 1101136515 486875/209 12/322  [Removed] Number of MME-Sessions is now 28
05/03 13:02:09.026 1101136515 655305/195 798/839 Attach request
05/03 13:02:09.026 1101136515 655305/195 798/839 emm_state_initial_context_setup() : ENTRY
05/03 13:02:09.026 1101136515 655305/195 798/839 mme_bearer_find_or_add_by_message() [PTI:21, EBI:0]
05/03 13:02:09.026 1101136515 655305/195 798/839 esm_state_inactive(): ENTRY
05/03 13:02:09.026 1101136515 655305/195 798/839 PDN Connectivity request
05/03 13:02:09.026 1101136515 655305/195 798/839 ESM information request
05/03 13:02:09.042 1101136515 655305/195 798/839 ESM information response
05/03 13:02:09.046 1101136515 655305/195 798/839 Attach accept
05/03 13:02:09.046 1101136515 655305/195 798/839 Activate default bearer context request
05/03 13:02:09.046 1101136515 655305/195 798/839 InitialContextSetupRequest
05/03 13:02:09.289 1101136515 655305/195 798/839 Attach complete
05/03 13:02:09.290 1101136515 655305/195 798/839 mme_bearer_find_or_add_by_message() [PTI:0, EBI:5]
05/03 13:02:09.290 1101136515 655305/195 798/839 Activate default EPS bearer context accept
05/03 13:02:09.290 1101136515 655305/195 798/839 esm_state_active(): ENTRY
05/03 13:02:09.290 1101136515 655305/195 798/839 InitialContextSetupResponse
05/03 13:02:09.209 1101136515 655305/195 798/839 UECapabilityInfoIndication
05/03 13:02:17.442 1101136515 655305/195 798/839 Detach request
05/03 13:02:17.445 1101136515 655305/195 798/839 Detach accept
05/03 13:02:17.445 1101136515 655305/195 798/839 UEContextReleaseCommand
05/03 13:02:17.463 1101136515 655305/195 798/839 UEContextReleaseComplete
05/03 13:02:26.537 1101136519 486875/209 12/322  Detach request
05/03 13:02:26.540 1101136519 486875/209 12/322  Detach accept
05/03 13:02:26.540 1101136519 486875/209 12/322  UEContextReleaseCommand
05/03 13:02:26.542 1101136519 486875/209 12/322  UEContextReleaseComplete
05/03 13:02:27.706 1101136515 655308/176 798/17  Attach request
05/03 13:02:27.706 1101136515 655308/176 798/17  emm_state_initial_context_setup(): ENTRY
05/03 13:02:27.706 1101136515 655308/176 798/17  mme_bearer_find_or_add_by_message() [PTI:22, EBI:0]
05/03 13:02:27.706 1101136515 655308/176 798/17  esm_state_inactive(): ENTRY
05/03 13:02:27.706 1101136515 655308/176 798/17  PDN Connectivity request
05/03 13:02:27.706 1101136515 655308/176 798/17  ESM information request
05/03 13:02:27.918 1101136517 486880/200 995/841 UEContextReleaseRequest
05/03 13:02:27.918 1101136517 486880/200 995/841 UEContextReleaseCommand
05/03 13:02:27.921 1101136517 486880/200 995/841 UEContextReleaseComplete
05/03 13:02:27.936 1101122519 655307/192 314/424 UEContextReleaseRequest
05/03 13:02:27.936 1101122519 655307/192 314/424 UEContextReleaseCommand
05/03 13:02:29.249 1101122519 655307/192 314/424 Detach request
05/03 13:02:29.251 1101122519 655307/192 314/424 Detach accept
05/03 13:02:30.130 1101136512 486719/913 881/969 Implicit MME Detach
05/03 13:02:30.134 1101136512 486719/913 881/969 [MME] Purge-UE-Request
05/03 13:02:30.134 1101136512 486719/913 881/969 [MME] Purge-UE-Answer
05/03 13:02:30.740 1101226519 655307/192 314/424 UEContextReleaseComplete


05/03 13:02:30.756 1101136515 655308/176 798/17  mme_bearer_find_or_add_by_message() [PTI:22, EBI:0]
05/03 13:02:30.756 1101136515 655308/176 798/17  esm_state_inactive(): MME_EVENT_ESM_MESSAGE
05/03 13:02:30.756 1101136515 655308/176 798/17  ESM information response
05/03 13:02:30.756 1101136515 655308/176 798/0/0/47960/22915 Create Session Response
05/03 13:02:30.760 1101136515 655308/176 798/17  Attach accept
05/03 13:02:30.760 1101136515 655308/176 798/17  Activate default bearer context request
05/03 13:02:30.760 1101136515 655308/176 798/0/0/47960 InitialContextSetupRequest


05/03 13:02:32.118 1101136519 486882/214 12/656  InitialUEMessage(M_TMSI:0xc0000428)
05/03 13:02:32.118 1101136519 486882/214 12/656  Attach request
05/03 13:02:32.118 1101136519 486882/214 12/656  emm_state_initial_context_setup(): ENTRY
05/03 13:02:32.118 1101136519 486882/214 12/656  mme_bearer_find_or_add_by_message() [PTI:9, EBI:0]
05/03 13:02:32.118 1101136519 486882/214 12/656  esm_state_inactive(): ENTRY
05/03 13:02:32.118 1101136519 486882/214 12/656  PDN Connectivity request
05/03 13:02:32.118 1101136519 486882/214 12/656  ESM information request
05/03 13:02:32.135 1101136519 486882/214 12/656  mme_bearer_find_or_add_by_message() [PTI:9, EBI:0]
05/03 13:02:32.135 1101136519 486882/214 12/656  esm_state_inactive(): MME_EVENT_ESM_MESSAGE
05/03 13:02:32.135 1101136519 486882/214 12/656  ESM information response
05/03 13:02:32.135 1101136519 486882/214 12/0/0/26037/17607 Create Session Response
05/03 13:02:32.138 1101136519 486882/214 12/656  Attch accept
05/03 13:02:32.138 1101136519 486882/214 12/656  Activate default bearer context request
05/03 13:02:32.139 1101136519 486882/214 12/0/0/26037 InitialContextSetupRequest
05/03 13:02:32.433 1101136519 486882/214 12/656  InitialContextSetupResponse
05/03 13:02:32.434 1101136519 486882/214 12/656  UECapabilityInfoIndication
05/03 13:02:32.455 1101136519 486882/214 12/656  Attach complete
05/03 13:02:32.455 1101136519 486882/214 12/656  mme_bearer_find_or_add_by_message() [PTI:0, EBI:5]
05/03 13:02:32.455 1101136519 486882/214 12/656  esm_state_inactive(): MME_EVENT_ESM_MESSAGE
05/03 13:02:32.455 1101136519 486882/214 12/656/4154952/26037 Activate default EPS bearer context accept
05/03 13:02:32.455 1101136519 486882/214 12/656  esm_state_active(): ENTRY
05/03 13:02:32.511 1101136519 486882/214 12/656  mme_bearer_find_or_add_by_message() [PTI:10, EBI:0]
05/03 13:02:32.511 1101136519 486882/214 12/656  esm_state_inactive(): ENTRY
05/03 13:02:32.511 1101136519 486882/214 12/656  PDN Connectivity request
05/03 13:02:32.511 1101136519 486882/214 12/656  PDN Connectivity reject(ims)
05/03 13:02:32.511 1101136519 486882/214 12/656  esm_state_exception(): ENTRY
05/03 13:02:32.511 1101136519 486882/214 12/656  [Removed] Number of MME-Sessions is now 27


05/03 13:02:33.190 1101136515 655308/176 798/17/55919304/26037 InitialContextSetupResponse
05/03 13:02:33.191 1101136515 655308/176 798/17  UECapabilityInfoIndication
05/03 13:02:33.209 1101136515 655308/176 798/17  Attach complete
05/03 13:02:33.209 1101136515 655308/176 798/17  mme_bearer_find_or_add_by_message() [PTI:0, EBI:5]
05/03 13:02:33.209 1101136515 655308/176 798/17  esm_state_active(): MME_EVENT_ESM_MESSAGE
05/03 13:02:33.209 1101136515 655308/176 798/17  ERROR: Unknown message(type:194)


05/03 13:02:33.636 1101136519 486882/214 12/656  UECapabilityInfoIndication
05/03 13:02:33.713 1101136519 486882/214 12/656  E_RABModificationIndication
05/03 13:02:33.713 1101136519 486882/214 12/656  E-RABModificationConfirm
05/03 13:02:36.067 1101136515 655308/176 798/17  UECapabilityInfoIndication
05/03 13:02:36.067 1101136515 655308/176 798/17  Detach request
05/03 13:02:36.069 1101136519 486882/214 12/656  Detach accept
05/03 13:02:36.069 1101136519 486882/214 12/656  UEContextReleaseCommand
05/03 13:02:36.072 1101136519 486882/214 12/656  UEContextReleaseComplete
05/03 13:02:36.178 1101122519 655309/223 314/412 Attach request
05/03 13:02:36.178 1101122519 655309/223 314/412 emm_state_initial_context_setup(): ENTRY
05/03 13:02:36.178 1101122519 655309/223 314/412 mme_bearer_find_or_add_by_message() [PTI:15, EBI:0]
05/03 13:02:36.178 1101122519 655309/223 314/412 esm_state_inactive()
05/03 13:02:36.178 1101122519 655309/223 314/412 PDN Connectivity request
05/03 13:02:36.178 1101122519 655309/223 314/412 ESM information request
05/03 13:02:36.194 1101122519 655309/223 314/412 ESM information response
05/03 13:02:36.197 1101122519 655309/223 314/412 Attach accept
05/03 13:02:36.197 1101122519 655309/223 314/412 Activate default bearer context request
05/03 13:02:36.197 1101122519 655309/223 314/412 InitialContextSetupRequest
05/03 13:02:36.254 1101136515 655308/176 798/17  E_RABModificationIndication
05/03 13:02:36.254 1101136515 655308/176 798/17  ERROR: No Bearer [5]
05/03 13:02:36.408 1101136519 486883/907  Service request
05/03 13:02:36.408 1101136519 486883/907  ERROR: No Session Context
05/03 13:02:36.408 1101136519 486883/907  Service reject
05/03 13:02:36.408 1101136519 486883/907  UEContextReleaseCommand
05/03 13:02:36.410 1101136519 486883/907  UEContextReleaseComplete
05/03 13:02:36.410 1101122519 655309/223 314/412 [MME] Purge-UE-Request
05/03 13:02:36.413 1101122519 655309/223 314/412 [MME] Purge-UE-Answer
05/03 13:02:36.514 1101122519 655309/223 314/412 InitialContextSetupResponse
05/03 13:02:36.515 1101122519 655309/223 314/412 UECapabilityInfoIndication
05/03 13:02:36.545 1101122519 655309/223 314/412 Attach complete
05/03 13:02:36.545 1101122519 655309/223 314/412 mme_bearer_find_or_add_by_message() [PTI:0, EBI:5]
05/03 13:02:36.545 1101122519 655309/223 314/412 Activate default EPS bearer context accept
05/03 13:02:36.545 1101122519 655309/223 314/412 esm_state_active()
05/03 13:02:36.584 1101136519 486884/261 473/992 Attach request
05/03 13:02:36.587 1101136519 486884/261 473/992 Authentication request
05/03 13:02:36.639 1101136519 486884/261 473/992 Authentication response
05/03 13:02:36.639 1101136519 486884/261 473/992 Security mode command
05/03 13:02:36.656 1101136519 486884/261 473/992 Security mode complete
05/03 13:02:36.661 1101136519 486884/261 473/992 mme_bearer_find_or_add_by_message() [PTI:11, EBI:0]
05/03 13:02:36.661 1101136519 486884/261 473/992 esm_state_inactive()
05/03 13:02:36.661 1101136519 486884/261 473/992 PDN connection request
05/03 13:02:36.661 1101136519 486884/261 473/992 ESM information request
05/03 13:02:36.678 1101136519 486884/261 473/992 ESM information response
05/03 13:02:36.680 1101136519 486884/261 473/992 Attach accept
05/03 13:02:36.680 1101136519 486884/261 473/992 InitialContextSetupRequest
05/03 13:02:36.968 1101136519 486884/261 473/992 InitialContextSetupResponse
05/03 13:02:36.969 1101136519 486884/261 473/992 UECapabilityInfoIndication
05/03 13:02:36.991 1101136519 486884/261 473/992 Attach complete
05/03 13:02:36.991 1101136519 486884/261 473/992 Activate default EPS bearer context accept
05/03 13:02:36.991 1101136519 486884/261 473/992 esm_state_inactive()
05/03 13:02:37.253 1101136515 655308/176 798/17  UECapabilityInfoIndication
05/03 13:02:38.162 1101136519 486884/261 473/992 UECapabilityInfoIndication
05/03 13:02:38.232 1101136519 486884/261 473/992 E_RABModificationIndication
05/03 13:02:38.232 1101136519 486884/261 473/992 E-RABModificationConfirm
05/03 13:02:38.260 1101136515 655308/176 798/17  UEContextReleaseRequest
05/03 13:02:38.767 1101136514 486877/270 874/207 UEContextReleaseRequest
05/03 13:02:38.768 1101136514 486877/270 874/207 UEContextReleaseCommand
05/03 13:02:38.769 1101136514 486877/270 874/207 UEContextReleaseComplete
05/03 13:02:45.768 1101136515 655308/176 798/17  LOCAL  No Response. Give up! for step 1 type 170 peer
05/03 13:02:45.768 1101136515 655308/176 798/17  UEContextReleaseCommand
05/03 13:02:45.768 1101136515 655308/176 798/17  GTP Timeout : IMSI[1101136515] Message-Type[170]
05/03 13:02:45.996 1101136515 655310/123         Attach Request
05/03 13:02:45.966 1101136515 655310/123         FATAL
*/

#define NUM_OF_TEST_UE 3

static void issues_2287_v263_func(abts_case *tc, void *data)
{
    int rv, i;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue[NUM_OF_TEST_UE];
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t enb_ue_s1ap_id;
    uint64_t mme_ue_s1ap_id;

    bson_t *doc = NULL;

    /* eNB connects to MME */
    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    for (i = 0; i < NUM_OF_TEST_UE; i++) {
        const char *scheme_output[] = {
            "0000000001",
            "0000000002",
            "0000000003",
            "0000000004",
            "0000000005",
        };

        /* Setup Test UE & Session Context */
        memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

        mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
        mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
        mobile_identity_suci.routing_indicator1 = 0;
        mobile_identity_suci.routing_indicator2 = 0xf;
        mobile_identity_suci.routing_indicator3 = 0xf;
        mobile_identity_suci.routing_indicator4 = 0xf;
        mobile_identity_suci.protection_scheme_id = OGS_PROTECTION_SCHEME_NULL;
        mobile_identity_suci.home_network_pki_value = 0;

        test_ue[i] = test_ue_add_by_suci(
                &mobile_identity_suci, scheme_output[i]);
        ogs_assert(test_ue[i]);

        test_ue[i]->e_cgi.cell_id = 0x54f6401;
        test_ue[i]->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue[i]->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

        test_ue[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue[i], doc));
    }

    /**********************************************************
     * First UE - INITIAL
     **********************************************************/
    test_ue[0]->enb_ue_s1ap_id = 100;

    sess = test_sess_add_by_apn(
            test_ue[0], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 21;

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue[0]->attach_request_param,
            0, sizeof(test_ue[0]->attach_request_param));
    test_ue[0]->attach_request_param.drx_parameter = 1;
    test_ue[0]->attach_request_param.ms_network_capability = 1;
    test_ue[0]->attach_request_param.tmsi_status = 1;
    test_ue[0]->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue[0]->attach_request_param.supported_codecs = 1;
    test_ue[0]->attach_request_param.ue_usage_setting = 1;
    test_ue[0]->attach_request_param.ms_network_feature_support = 1;
    emmbuf = testemm_build_attach_request(test_ue[0], esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue[0]->initial_ue_param, 0,
            sizeof(test_ue[0]->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue[0], emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send Security mode complete */
    test_ue[0]->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue[0]->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue[0], 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue[0], esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send Detach Request */
    emmbuf = testemm_build_detach_request(test_ue[0], 1, true, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /**********************************************************
     * Second UE - INITIAL
     **********************************************************/
    test_ue[1]->enb_ue_s1ap_id = 200;

    sess = test_sess_add_by_apn(
            test_ue[1], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 8;

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue[1]->attach_request_param,
            0, sizeof(test_ue[1]->attach_request_param));
    test_ue[1]->attach_request_param.drx_parameter = 1;
    test_ue[1]->attach_request_param.ms_network_capability = 1;
    test_ue[1]->attach_request_param.tmsi_status = 1;
    test_ue[1]->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue[1]->attach_request_param.supported_codecs = 1;
    test_ue[1]->attach_request_param.ue_usage_setting = 1;
    test_ue[1]->attach_request_param.ms_network_feature_support = 1;
    emmbuf = testemm_build_attach_request(test_ue[1], esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue[1]->initial_ue_param, 0,
            sizeof(test_ue[1]->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue[1], emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Security mode complete */
    test_ue[1]->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf =
        tests1ap_build_ue_radio_capability_info_indication(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue[1]->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue[1], 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Detach Request */
    emmbuf = testemm_build_detach_request(test_ue[1], 1, true, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /**********************************************************
     * First UE - GUTI
     **********************************************************/
    sess = test_sess_find_by_apn(
            test_ue[0], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    test_sess_remove(sess);

    sess = test_sess_add_by_apn(
            test_ue[0], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 22;

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.epco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue[0]->attach_request_param,
            0, sizeof(test_ue[0]->attach_request_param));
    test_ue[0]->attach_request_param.guti = 1;
    test_ue[0]->attach_request_param.last_visited_registered_tai = 1;
    test_ue[0]->attach_request_param.drx_parameter = 1;
    test_ue[0]->attach_request_param.ms_network_capability = 1;
    test_ue[0]->attach_request_param.location_area_identication = 1;
    test_ue[0]->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue[0]->attach_request_param.mobile_station_classmark_3 = 1;
    test_ue[0]->attach_request_param.supported_codecs = 1;
    test_ue[0]->attach_request_param.ue_usage_setting = 1;
    test_ue[0]->attach_request_param.old_guti_type = 1;
    emmbuf = testemm_build_attach_request(test_ue[0], esmbuf, true, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue[0]->initial_ue_param, 0, sizeof(test_ue[0]->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue[0], emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);


    /**********************************************************
     * Second UE - GUTI
     **********************************************************/
    sess = test_sess_find_by_apn(
            test_ue[1], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    test_sess_remove(sess);

    sess = test_sess_add_by_apn(
            test_ue[1], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 9;

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.epco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue[1]->attach_request_param,
            0, sizeof(test_ue[1]->attach_request_param));
    test_ue[1]->attach_request_param.guti = 1;
    test_ue[1]->attach_request_param.last_visited_registered_tai = 1;
    test_ue[1]->attach_request_param.drx_parameter = 1;
    test_ue[1]->attach_request_param.ms_network_capability = 1;
    test_ue[1]->attach_request_param.location_area_identication = 1;
    test_ue[1]->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue[1]->attach_request_param.mobile_station_classmark_3 = 1;
    test_ue[1]->attach_request_param.supported_codecs = 1;
    test_ue[1]->attach_request_param.ue_usage_setting = 1;
    test_ue[1]->attach_request_param.old_guti_type = 1;
    emmbuf = testemm_build_attach_request(test_ue[1], esmbuf, true, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue[1]->initial_ue_param, 0, sizeof(test_ue[1]->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue[1], emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send UE Capability Info Indication */
    sendbuf =
        tests1ap_build_ue_radio_capability_info_indication(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue[1]->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue[1], 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send PDN Connectivity Request */
    sess = test_sess_add_by_apn(test_ue[1], "ims", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 10;

    sess->pdn_connectivity_param.apn = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, true, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive PDN Connectivity reject */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    test_sess_remove(sess);

    /**********************************************************
     * First UE - GUTI Resume
     **********************************************************/

    sess = test_sess_add_by_apn(
            test_ue[0], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue[0]->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue[0], 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue[0], esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove_all();
}

/*
- v2.6.4 Analysis
05/29 15:02:25.141 1101122519 490/222 267/300/-974782142/30908 E_RABModificationIndication
05/29 15:02:25.141 1101122519 490/222 267/300/-974782142/30908 E-RABModificationConfirm


05/29 15:02:28.090 1101135526 485/106 44/54/41120/45321 E_RABModificationIndication
05/29 15:02:28.090 1101135526 485/106 44/54/41120/45321 E-RABModificationConfirm
05/29 15:02:28.360 1101136515 488/377 692/574/-1025113818/17073 E_RABModificationIndication
05/29 15:02:28.361 1101136515 488/377 692/574/-1025113818/17073 E-RABModificationConfirm
05/29 15:02:30.444 1101135526 485/106 44/54/41120/45321 UEContextReleaseRequest
05/29 15:02:30.445 1101135526 485/106 44/54/41120/45321 UEContextReleaseCommand
05/29 15:02:30.445 1101135526 485/106 44/54/41120/45321 Downlink Data Notification
05/29 15:02:30.445 1101135526 485/106 44/54/41120/45321 [1101135526] Clear Paging Info
05/29 15:02:30.445 1101135526 485/106 44/54/41120/45321 Downlink Data Notification Ackknowledge
05/29 15:02:30.476 1101135526 485/106 44/54/41120/45321 UEContextReleaseComplete
05/29 15:02:31.791 1101135526 491/826 44/54/-1053359805/45321 InitialUEMessage
05/29 15:02:31.791 1101135526 491/826 44/54/-1053359805/45321 Tracking area update request
05/29 15:02:31.792 1101135526 491/826 44/54/-1053359805/45321 Tracking area update accept
05/29 15:02:31.792 1101135526 491/826 44/54/-1053359805/45321 UEContextReleaseCommand
05/29 15:02:31.793 1101135526 491/826 44/54/-1053359805/45321 UEContextReleaseComplete
05/29 15:02:32.129 1101135526 492/65  44/54/41672/45321 InitialUEMessage
05/29 15:02:32.129 1101135526 492/65  44/54/41672/45321 Service request
05/29 15:02:32.129 1101135526 492/65  44/54/41672/45321 InitialContextSetupRequest
05/29 15:02:32.191 1101135526 492/65  44/54/41672/45321 InitialContextSetupResponse
05/29 15:02:32.656 1101135526 492/65  44/54/-1053359805/45321 E_RABModificationIndication
05/29 15:02:32.657 1101135526 492/65  44/54/-1053359805/45321 E-RABModificationConfirm
05/29 15:02:33.530 1101135526 492/65  44/54/41672/45321 E_RABModificationIndication
05/29 15:02:33.531 1101135526 492/65  44/54/41672/45321 E-RABModificationConfirm
05/29 15:02:35.889 1101135526 492/65  44/54/41672/45321 UEContextReleaseRequest
05/29 15:02:35.890 1101135526 492/65  44/54/41672/45321 UEContextReleaseCommand
05/29 15:02:35.890 1101135526 492/65  44/54/41672/45321 Downlink Data Notification
05/29 15:02:35.890 1101135526 492/65  44/54/41672/45321 [1101135526] Clear Paging Info
05/29 15:02:35.890 1101135526 492/65  44/54/41672/45321 Downlink Data Notification Ackknowledge
05/29 15:02:35.891 1101135526 492/65  44/54/41672/45321 UEContextReleaseComplete
05/29 15:02:40.311 1101135526 493/969 44/54/41984/45321 InitialUEMessage
05/29 15:02:40.311 1101135526 493/969 44/54/41984/45321 Service request
05/29 15:02:40.311 1101135526 493/969 44/54/41984/45321 InitialContextSetupRequest
05/29 15:02:40.375 1101135526 493/969 44/54/41984/45321 InitialContextSetupResponse
05/29 15:02:42.402 1101135526 493/969 44/54/41984/45321 UEContextReleaseRequest
05/29 15:02:42.403 1101135526 493/969 44/54/41984/45321 UEContextReleaseCommand
05/29 15:02:42.403 1101135526 493/969 44/54/41984/45321 Downlink Data Notification
05/29 15:02:42.403 1101135526 493/969 44/54/41984/45321 [1101135526] Clear Paging Info
05/29 15:02:42.403 1101135526 493/969 44/54/41984/45321 Downlink Data Notification Ackknowledge
05/29 15:02:42.511 1101135526 494/123 44/54/41984/45321 InitialUEMessage
05/29 15:02:42.511 1101135526 493/969 44/54/41984/45321 UEContextReleaseCommand
05/29 15:02:42.512 1101135526 494/123 44/54/41984/45321 Tracking area update request
05/29 15:02:42.512 1101135526 494/123 44/54/41984/45321 Tracking area update accept
05/29 15:02:42.512 1101135526 494/123 44/54/41984/45321 UEContextReleaseCommand
05/29 15:02:43.412 1101135526 493/969 44/54/41984/45321 UEContextReleaseComplete
05/29 15:02:43.412 1101135526 494/123 44/54/41984/45321 UEContextReleaseComplete
05/29 15:02:43.586 1101135526 495/85  44/54/42064/45321 InitialUEMessage
05/29 15:02:43.586 1101135526 495/85  44/54/42064/45321 Servie request
05/29 15:02:43.586 1101135526 495/85  44/54/42064/45321 InitialContextSetupRequest
05/29 15:02:43.671 1101135526 495/85  44/54/42064/45321 InitialContextSetupResponse
05/29 15:02:44.644 1101135526 495/85  44/54/-991887070/45321 E_RABModificationIndication
05/29 15:02:44.645 1101135526 495/85  44/54/-991887070/45321 E-RABModificationConfirm
05/29 15:02:45.422 1101136514 613657/812 625/399 UEContextReleaseRequest
05/29 15:02:45.423 1101136514 613657/812 625/399 UEContextReleaseCommand
05/29 15:02:45.424 1101136514 613657/812 625/399 UEContextReleaseComplete
05/29 15:02:46.659 1101135526 495/85  44/54/-991887070/45321 UEContextReleaseRequest
05/29 15:02:46.660 1101135526 495/85  44/54/-991887070/45321 UEContextReleaseCommand
05/29 15:02:46.660 1101135526 495/85  44/54/-991887070/45321 Downlink Data Notification
05/29 15:02:46.660 1101135526 495/85  44/54/-991887070/45321 [1101135526] Clear Paging Info
05/29 15:02:46.660 1101135526 495/85  44/54/-991887070/45321 Downlink Data Notification Ackknowledge
05/29 15:02:46.668 1101135526 495/85  44/54/-991887070/45321 UEContextReleaseComplete
05/29 15:02:46.668 1101135526 496/913 44/54/42104/45321 InitialUEMessage
05/29 15:02:48.146 1101135526 496/913 44/54/42104/45321 Servie request
05/29 15:02:48.146 1101135526 496/913 44/54/42104/45321 InitialContextSetupRequest
05/29 15:02:48.231 1101135526 496/913 44/54/42104/45321 InitialContextSetupResponse
05/29 15:02:48.788 1101135526 496/913 44/54/-991821534/45321 E_RABModificationIndication
05/29 15:02:48.789 1101135526 496/913 44/54/-991821534/45321 E-RABModificationConfirm

05/29 15:02:49.067 1101133203 613658/898 64/808 InitialUEMessage
05/29 15:02:49.067 1101133203 613658/898 64/808 Attach Request
05/29 15:02:49.067 1101133203 613658/898 64/808 Delete Session Request
05/29 15:02:49.071 1101133203 613658/898 64/808 Delete Session Response
05/29 15:02:49.071 1101133203 613658/898 64/808 INFO: Removed Session: UE IMSI:[1101133203] APN:[internet]
05/29 15:02:49.073 1101133203 613658/898 64/0 Authentication request
05/29 15:02:49.127 1101133203 613658/898 64/0 Authentication response
05/29 15:02:49.127 1101133203 613658/898 64/0 Security mode command
05/29 15:02:49.144 1101133203 613658/898 64/0 Security mode complete
05/29 15:02:49.149 1101133203 613658/898 64/0 mme_bearer_find_or_add_by_message() : ESM message type:208, PTI:1, EBI:0
05/29 15:02:49.149 1101133203 613658/898 64/0 esm_state_inactive(): ENTRY
05/29 15:02:49.149 1101133203 613658/898 64/0 [Added] Number of MME-Sessions is now 28
05/29 15:02:49.149 1101133203 613658/898 64/0 PDN Connectivity request
05/29 15:02:49.149 1101133203 613658/898 64/0 ESM information request
05/29 15:02:49.165 1101133203 613658/898 64/0 mme_bearer_find_or_add_by_message() : ESM message type:218, PTI:1, EBI:0
05/29 15:02:49.165 1101133203 613658/898 64/0/0/26394 esm_state_inactive(): ENTRY
05/29 15:02:49.165 1101133203 613658/898 64/0/0/26394 ESM information response
05/29 15:02:49.165 1101133203 613658/898 64/0/0/26394 Create Session Request
05/29 15:02:49.170 1101133203 613658/898 64/845/0/26394/60543 Create Session Response
05/29 15:02:49.170 1101133203 613658/898 64/845/0/26394 Attach accept
05/29 15:02:49.170 1101133203 613658/898 64/845/0/26394 InitialContextSetupRequest
05/29 15:02:49.538 1101133203 613658/898 64/845/0/26394 InitialContextSetupResponse
05/29 15:02:49.539 1101133203 613658/898 64/845/0/26394 UECapabilityInfoIndication
05/29 15:02:49.577 1101133203 613658/898 64/845/0/26394 Attach complete
05/29 15:02:49.577 1101133203 613658/898 64/845/0/26394 EMM information
05/29 15:02:49.577 1101133203 613658/898 64/845/0/26394 mme_bearer_find_or_add_by_message() : ESM message type:194, PTI:0, EBI:5
05/29 15:02:49.577 1101133203 613658/898 64/845/0/26394 esm_state_inactive(): MME_EVENT_ESM_MESSAGE
05/29 15:02:49.577 1101133203 613658/898 64/845/52365352/26394 Activate default EPS bearer context accept
05/29 15:02:49.577 1101133203 613658/898 64/845/52365352/26394 esm_state_active(): ENTRY
05/29 15:02:50.840 1101133203 613658/898 64/845/52365352/26394 UECapabilityInfoIndication

05/29 15:02:50.886 1101135526 496/913 44/54/42104/45321 E_RABModificationIndication
05/29 15:02:50.887 1101135526 496/913 44/54/42104/45321 E-RABModificationConfirm

05/29 15:02:50.904 1101133203 613658/898 64/845/-1069940446/26394 E_RABModificationIndication
05/29 15:02:50.905 1101133203 613658/898 64/845/-1069940446/26394 E-RABModificationConfirm

05/29 15:02:51.402 1101135526 496/913 44/54/-1053294269/45321 E_RABModificationIndication
05/29 15:02:51.403 1101135526 496/913 44/54/-1053294269/45321 E-RABModificationConfirm

05/29 15:02:52.260 1101122519 490/222 267/300/41592/26394 E_RABModificationIndication
05/29 15:02:52.260 1101122519 490/222 267/300/41592/26394 E-RABModificationConfirm


05/29 15:02:59.275 1101133203 613658/898 64/845  UEContextReleaseRequest
05/29 15:02:59.275 1101133203 613658/898 64/845  UEContextReleaseCommand
05/29 15:02:59.277 1101133203 613658/898 64/845  UEContextReleaseComplete
05/29 15:03:26.951 1101122519 501/104    InitialUEMessage
05/29 15:03:26.951 1101122519 490/222    UEContextReleaseCommand
05/29 15:03:26.951 1101122519 501/104        ///26394 Service request
05/29 15:03:26.951 1101122519 501/104        ///26394 InitialContextSetupRequest
05/29 15:03:31.956 1101122519 502/32     267/300 InitialUEMessage
05/29 15:03:31.956 1101122519 501/104    UEContextReleaseCommand
05/29 15:03:31.956 1101122519 502/32     267/300 Service request
05/29 15:03:31.956 1101122519 502/32     267/300//26394 InitialContextSetupRequest
05/29 15:03:34.128 1101136523 613659/44  781/369 InitialContextSetupResponse
05/29 15:03:34.222 1101136523 613659/44  781/369 Attach complete
05/29 15:03:34.223 1101136523 613659/44  781/369 mme_bearer_find_or_add_by_message() : ESM message type:194, PTI:0, EBI:5
05/29 15:03:34.223 1101136523 613659/44  781/369 esm_state_inactive()
05/29 15:03:34.223 1101136523 613659/44  781/369 Activate default EPS bearer context accept
05/29 15:03:34.257 1101136523 613659/44  781/369 PDN Connectivity request
05/29 15:03:34.257 1101136523 613659/44  781/369 PDN Connectivity reject(ims)
05/29 15:03:34.257 1101136523 613659/44  781/369 esm_state_inactive(): EXIT
05/29 15:03:34.257 1101136523 613659/44  781/369 esm_state_exception(): ENTRY
05/29 15:03:34.257 1101136523 613659/44  781/369 Removed Session: UE IMSI:[1101136523] APN:[Unknown]
05/29 15:03:34.257 1101136523 613659/44  781/369 esm_state_exception(): EXIT
05/29 15:03:34.373 1101135526 500/620    44/54   UEContextReleaseComplete
05/29 15:03:34.423 1101122519 502/32     267/300/42616/26394 InitialContextSetupResponse
05/29 15:03:34.896 1101122519 502/32     267/300/-991690462/26394 E_RABModificationIndication
05/29 15:03:34.897 1101122519 502/32     267/300/-991690462/26394 E-RABModificationConfirm
05/29 15:03:35.078 1101135526 503/123    44/54   Service request
05/29 15:03:35.078 1101135526 503/123    44/54   InitialContextSetupRequest
05/29 15:03:35.123 1101135526 503/123    44/54   InitialContextSetupResponse
05/29 15:03:35.222 1101133205 613635/84  164/377 UEContextReleaseRequest
05/29 15:03:35.223 1101133205 613635/84  164/377 UEContextReleaseCommand
05/29 15:03:35.225 1101133205 613635/84  164/377 UEContextReleaseCommplete
05/29 15:03:34.421 1101136523 613659/44  781/369 UECapabilityInfoIndication
05/29 15:03:34.498 1101136523 613659/44  781/369 E_RABModificationIndication
05/29 15:03:34.499 1101136523 613659/44  781/369 E-RABModificationConfirm
05/29 15:03:35.619 1101135526 503/123    44/54   E_RABModificationIndication
05/29 15:03:35.619 1101135526 503/123    44/54   E-RABModificationConfirm
05/29 15:03:36.088 1101122519 501/104    InitialContextSetupFailure
05/29 15:03:36.088 1101122519 501/104    UEContextReleaseCommand
05/29 15:03:36.088 1101122519 501/104    UEContextReleaseComplete
05/29 15:03:36.328 1101133205 613660/844 164/377 Service request
05/29 15:03:36.328 1101133205 613660/844 164/377 InitialContextSetupRequest
05/29 15:03:36.388 1101133205 613660/844 164/377/52365432/22946 InitialContextSetupResponse
05/29 15:03:37.618 1101133205 613660/844 164/377/-942800606/22946 E_RABModificationIndication
05/29 15:03:37.618 1101133205 613660/844 164/377/-942800606/22946 E-RABModificationConfirm
05/29 15:04:26.776 1101122519 502/32     267/300/-991559390/26394 E_RABModificationIndication
05/29 15:04:26.777 1101122519 502/32     267/300/-991559390/26394 E-RABModificationConfirm
05/29 15:03:37.618 1101133205 613660/844 164/377/-942800606/22946 E-RABModificationConfirm
05/29 15:04:31.533 1101133205 613664/975 164/377 Servie request
05/29 15:04:31.533 1101133205 613664/975 164/0/0/22946 InitialContextSetupRequest
05/29 15:04:31.586 1101133205 613664/975 164/377 Downlink Data Notification
05/29 15:04:31.586 1101133205 613664/975 164/377 [1101133205] Clear Paging Info
05/29 15:04:31.586 1101133205 613664/975 164/377 Downlink Data Notification Ackknowledge
05/29 15:04:31.587 1101133205 613664/975 164/377/52365864/22946 InitialContextSetupResponse
05/29 15:04:34.042 1101136514 613665/870 625/870 Servie request
05/29 15:04:34.042 1101136514 613665/870 625/870 InitialContextSetupRequest
05/29 15:04:34.099 1101136514 613665/870 625/870 mme_bearer_find_or_add_by_message() : ESM message type:208, PTI:89, EBI:0
05/29 15:04:34.099 1101136514 613665/870 625/870 esm_state_inactive(): ENTRY
05/29 15:04:34.099 1101136514 613665/870 625/870 [Added] Number of MME-Sessions is now 29 (../src/mme/mme-context.c:4049)
05/29 15:04:34.099 1101136514 613665/870 625/870 PDN Connectivity request
05/29 15:04:34.099 1101136514 613665/870 625/870 PDN Connectivity reject(ims)
05/29 15:04:34.099 1101136514 613665/870 625/870 Removed Session: UE IMSI:[1101136514] APN:[Unknown]
05/29 15:04:34.099 1101136514 613665/870 625/870 InitialContextSetupResponse
05/29 15:04:45.037 1101136515 506/91     692/546 E_RABModificationIndication
05/29 15:04:45.037 1101136515 506/91     692/546 E-RABModificationConfirm
05/29 15:04:48.070 1101122519 502/32     267/300 Detach request
05/29 15:04:48.073 1101133023 613658/898 64/845  ERROR: S1 context has already been removed
05/29 15:04:48.073 1101133023 613658/898 64/845  ERROR: mme_s11_handle_delete_session_response: Expectation `r == OGS_OK' failed.
05/29 15:04:48.073 1101133023 613658/898 64/845  Removed Session: UE IMSI:[1101133203] APN:[internet]
05/29 15:04:48.073 1101133023 613658/898 64/845  esm_stae_active(): EXIT
05/29 15:04:48.474 1101136515 506/91     692/546 E_RABModificationIndication
05/29 15:04:48.475 1101136515 506/91     692/546 E-RABModificationConfirm
05/29 15:04:53.029 1101136515 506/91     692/546 E_RABModificationIndication
05/29 15:04:53.030 1101136515 506/91     692/546 E-RABModificationConfirm
05/29 15:04:53.278 1101135526 505/150    44/54   E_RABModificationIndication
05/29 15:04:53.279 1101135526 505/150    44/54   E-RABModificationConfirm
05/29 15:04:53.933 1101122519 502/32     267/300 E_RABModificationIndication
05/29 15:04:53.933 1101122519 502/32     267/300 ERROR: No Bearer [5]
05/29 15:04:53.933 1101122519 502/32     267/300 ErrorIndication
05/29 15:04:53.990 1101135526 505/150    44/54   E_RABModificationIndication
05/29 15:04:53.991 1101135526 505/150    44/54   E-RABModificationConfirm
05/29 15:04:55.146 1101122519 502/32     267/300 Attach request
05/29 15:04:55.146 1101133023 613658/898 64/845  Removed Session: UE IMSI:[1101133203] APN:[internet]
05/29 15:04:55.146 1101122519 502/32     267/300 emm_state_initial_context_setup(): ENTRY
05/29 15:04:55.146 1101122519 502/32     267/300 mme_bearer_find_or_add_by_message() : ESM message type:208, PTI:16, EBI:0
05/29 15:04:55.147 1101122519 502/32     267/300 ERROR: No Bearer(1) : ESM message type:208, PTI:16, EBI:0
*/

static void issues_2287_v264_func(abts_case *tc, void *data)
{
    int rv, i;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue[NUM_OF_TEST_UE];
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t enb_ue_s1ap_id;
    uint64_t mme_ue_s1ap_id;

    bson_t *doc = NULL;

    /* eNB connects to MME */
    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    for (i = 0; i < NUM_OF_TEST_UE; i++) {
        const char *scheme_output[] = {
            "0000000001",
            "0000000002",
            "0000000003",
            "0000000004",
            "0000000005",
        };

        /* Setup Test UE & Session Context */
        memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

        mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
        mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
        mobile_identity_suci.routing_indicator1 = 0;
        mobile_identity_suci.routing_indicator2 = 0xf;
        mobile_identity_suci.routing_indicator3 = 0xf;
        mobile_identity_suci.routing_indicator4 = 0xf;
        mobile_identity_suci.protection_scheme_id = OGS_PROTECTION_SCHEME_NULL;
        mobile_identity_suci.home_network_pki_value = 0;

        test_ue[i] = test_ue_add_by_suci(
                &mobile_identity_suci, scheme_output[i]);
        ogs_assert(test_ue[i]);

        test_ue[i]->e_cgi.cell_id = 0x54f6401;
        test_ue[i]->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue[i]->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

        test_ue[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue[i], doc));
    }

    /**********************************************************
     * Second UE - INITIAL
     **********************************************************/
    test_ue[1]->enb_ue_s1ap_id = 200;

    sess = test_sess_add_by_apn(
            test_ue[1], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 8;

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue[1]->attach_request_param,
            0, sizeof(test_ue[1]->attach_request_param));
    test_ue[1]->attach_request_param.drx_parameter = 1;
    test_ue[1]->attach_request_param.ms_network_capability = 1;
    test_ue[1]->attach_request_param.tmsi_status = 1;
    test_ue[1]->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue[1]->attach_request_param.supported_codecs = 1;
    test_ue[1]->attach_request_param.ue_usage_setting = 1;
    test_ue[1]->attach_request_param.ms_network_feature_support = 1;
    emmbuf = testemm_build_attach_request(test_ue[1], esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue[1]->initial_ue_param, 0,
            sizeof(test_ue[1]->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue[1], emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Security mode complete */
    test_ue[1]->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf =
        tests1ap_build_ue_radio_capability_info_indication(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue[1]->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue[1], 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send UE Context Release Request */
    sendbuf = test_s1ap_build_ue_context_release_request(test_ue[1],
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_user_inactivity);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /**********************************************************
     * First UE - INITIAL
     **********************************************************/
    test_ue[0]->enb_ue_s1ap_id = 100;

    sess = test_sess_add_by_apn(
            test_ue[0], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 21;

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue[0]->attach_request_param,
            0, sizeof(test_ue[0]->attach_request_param));
    test_ue[0]->attach_request_param.drx_parameter = 1;
    test_ue[0]->attach_request_param.ms_network_capability = 1;
    test_ue[0]->attach_request_param.tmsi_status = 1;
    test_ue[0]->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue[0]->attach_request_param.supported_codecs = 1;
    test_ue[0]->attach_request_param.ue_usage_setting = 1;
    test_ue[0]->attach_request_param.ms_network_feature_support = 1;
    emmbuf = testemm_build_attach_request(test_ue[0], esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue[0]->initial_ue_param, 0,
            sizeof(test_ue[0]->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue[0], emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send Security mode complete */
    test_ue[0]->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue[0]->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue[0], 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue[0], esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[0], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /* Send E-RABModificationIndication */
    ogs_list_for_each(&sess->bearer_list, bearer)
        bearer->enb_s1u_teid++;

    sendbuf = test_s1ap_build_e_rab_modification_indication(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive E-RABModificationConfirm */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    /**********************************************************
     * Second UE - AUTH
     **********************************************************/
    sess = test_sess_find_by_apn(
            test_ue[1], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    test_sess_remove(sess);

    sess = test_sess_add_by_apn(
            test_ue[1], "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 1;

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue[1]->attach_request_param,
            0, sizeof(test_ue[1]->attach_request_param));
    test_ue[1]->attach_request_param.drx_parameter = 1;
    test_ue[1]->attach_request_param.ms_network_capability = 1;
    test_ue[1]->attach_request_param.tmsi_status = 1;
    test_ue[1]->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue[1]->attach_request_param.supported_codecs = 1;
    test_ue[1]->attach_request_param.ue_usage_setting = 1;
    test_ue[1]->attach_request_param.ms_network_feature_support = 1;
    emmbuf = testemm_build_attach_request(test_ue[1], esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue[1]->initial_ue_param, 0,
            sizeof(test_ue[1]->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue[1], emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Security mode complete */
    test_ue[1]->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send UE Capability Info Indication */
    sendbuf =
        tests1ap_build_ue_radio_capability_info_indication(test_ue[1]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue[1]->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue[1], 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue[1], esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue[1], emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[1], recvbuf);


    /**********************************************************
     * First UE - AUTH
     **********************************************************/

    /* Send E-RABModificationIndication */
    ogs_list_for_each(&sess->bearer_list, bearer)
        bearer->enb_s1u_teid++;

    sendbuf = test_s1ap_build_e_rab_modification_indication(test_ue[0]);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive E-RABModificationConfirm */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue[0], recvbuf);

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove_all();
}

#if 0 /* Deprecated to resolve issue #3131 */
static void pull_3122_v270_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t enb_ue_s1ap_id;
    uint64_t mme_ue_s1ap_id;

    bson_t *doc = NULL;

    /* Setup Test UE & Session Context */
    memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

    mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    mobile_identity_suci.routing_indicator1 = 0;
    mobile_identity_suci.routing_indicator2 = 0xf;
    mobile_identity_suci.routing_indicator3 = 0xf;
    mobile_identity_suci.routing_indicator4 = 0xf;
    mobile_identity_suci.protection_scheme_id = OGS_PROTECTION_SCHEME_NULL;
    mobile_identity_suci.home_network_pki_value = 0;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, "3746000006");
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x1079baf0;
    test_ue->nas.ksi = 0;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess = test_sess_add_by_apn(test_ue, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);

    /* eNB connects to MME */
    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /********** Insert Subscriber in Database */
    doc = test_db_new_simple(test_ue);
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue, doc));

    /* Send Attach Request */
    emmbuf = testemm_build_identity_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue->initial_ue_param, 0, sizeof(test_ue->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ErrorIndication */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive UEContextReleaseCommand */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UEContextReleaseComplete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove(test_ue);
}
#endif

abts_suite *test_issues(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, issues_1431_func, NULL);
    abts_run_test(suite, issues_2287_v263_func, NULL);
    abts_run_test(suite, issues_2287_v264_func, NULL);
#if 0 /* Deprecated to resolve issue #3131 */
    abts_run_test(suite, pull_3122_v270_func, NULL);
#endif

    return suite;
}
