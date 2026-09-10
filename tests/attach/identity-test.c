/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

/*
 * Attach naming another subscriber, on a registered context.
 *
 *   UE-2   attach, then go idle       -> context kept, session kept
 *   UE-1   attach, stay connected     -> owns the S1 context
 *   Attach Request(IMSI of UE-2) sent with the S1AP IDs of UE-1
 *
 * Before: mme_ue_set_imsi() moves UE-2's session into UE-1's context
 *         -> ogs_assert(ogs_list_empty(&mme_ue->sess_list)) -> abort
 * After:  access bearers released, S1 released, Attach dropped, and
 *         UE-1 keeps its own session
 *
 * In the field the S1 context was mis-associated by a stale GUTI, but
 * that only explains how UE-1's context ended up on this connection.
 * Any Attach naming someone else is enough.
 */
static void attach_identity_mismatch_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;

    /* UE-1 owns the S1 context that the ATTACH REQUEST arrives on */
    test_ue_t *test_ue1 = NULL;
    test_sess_t *sess1 = NULL;
    test_bearer_t *bearer1 = NULL;
    bson_t *doc1 = NULL;

    /* UE-2 is the subscriber named by that ATTACH REQUEST */
    test_ue_t *test_ue2 = NULL;
    test_sess_t *sess2 = NULL;
    test_bearer_t *bearer2 = NULL;
    bson_t *doc2 = NULL;

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

    test_ue1 = test_ue_add_by_suci(&mobile_identity_suci, "0000000030");
    ogs_assert(test_ue1);

    test_ue1->e_cgi.cell_id = 0x1079baf0;
    test_ue1->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue1->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue1->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue1->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess1 = test_sess_add_by_apn(
            test_ue1, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess1);

    test_ue2 = test_ue_add_by_suci(&mobile_identity_suci, "0000000031");
    ogs_assert(test_ue2);

    test_ue2->e_cgi.cell_id = 0x1079baf0;
    test_ue2->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue2->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue2->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue2->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess2 = test_sess_add_by_apn(
            test_ue2, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess2);

    /* Both UEs are served by the same eNB, so keep the eNB-UE-S1AP-ID
     * ranges apart */
    test_ue2->enb_ue_s1ap_id = 100;

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
    doc1 = test_db_new_simple(test_ue1);
    ABTS_PTR_NOTNULL(tc, doc1);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue1, doc1));

    doc2 = test_db_new_simple(test_ue2);
    ABTS_PTR_NOTNULL(tc, doc2);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue2, doc2));


    /* UE-2: attach, then go idle - context and session stay */
    /* Send Attach Request */
    memset(&sess2->pdn_connectivity_param,
            0, sizeof(sess2->pdn_connectivity_param));
    sess2->pdn_connectivity_param.eit = 1;
    sess2->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess2, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue2->attach_request_param,
            0, sizeof(test_ue2->attach_request_param));
    test_ue2->attach_request_param.drx_parameter = 1;
    test_ue2->attach_request_param.ms_network_capability = 1;
    test_ue2->attach_request_param.tmsi_status = 1;
    test_ue2->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue2->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue2, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue2->initial_ue_param, 0, sizeof(test_ue2->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue2, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Security mode complete */
    test_ue2->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send ESM Information Response */
    sess2->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess2);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue2->nr_cgi.cell_id = 0x1234502;
    bearer2 = test_bearer_find_by_ue_ebi(test_ue2, 5);
    ogs_assert(bearer2);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer2, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);
    /* Send UEContextReleaseRequest - UE-2 goes idle */
    sendbuf = test_s1ap_build_ue_context_release_request(test_ue2,
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_user_inactivity);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UEContextReleaseCommand */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UEContextReleaseComplete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* UE-1 attaches and stays connected with an active default bearer */
    /* Send Attach Request */
    memset(&sess1->pdn_connectivity_param,
            0, sizeof(sess1->pdn_connectivity_param));
    sess1->pdn_connectivity_param.eit = 1;
    sess1->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess1, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue1->attach_request_param,
            0, sizeof(test_ue1->attach_request_param));
    test_ue1->attach_request_param.drx_parameter = 1;
    test_ue1->attach_request_param.ms_network_capability = 1;
    test_ue1->attach_request_param.tmsi_status = 1;
    test_ue1->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue1->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue1, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue1->initial_ue_param, 0, sizeof(test_ue1->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue1, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Security mode complete */
    test_ue1->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send ESM Information Response */
    sess1->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess1);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue1->nr_cgi.cell_id = 0x1234502;
    bearer1 = test_bearer_find_by_ue_ebi(test_ue1, 5);
    ogs_assert(bearer1);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer1, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue1, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Attach for UE-2, delivered with the S1AP IDs of UE-1 */
    sess2->pti = 1;

    memset(&sess2->pdn_connectivity_param,
            0, sizeof(sess2->pdn_connectivity_param));
    sess2->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess2, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue2->attach_request_param,
            0, sizeof(test_ue2->attach_request_param));
    emmbuf = testemm_build_attach_request(test_ue2, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*
     * Expect the release, not a reply to the Attach.
     * Without the fix the MME is already gone by now.
     */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_UEContextRelease,
            test_ue1->s1ap_procedure_code);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*
     * Did UE-1 survive with its session?
     *
     * The MME answers a Service Request with an
     * InitialContextSetupRequest only while the context still has a
     * session with active EPS bearers. A re-labelled or emptied UE-1
     * gets a Service Reject in a downlinkNASTransport instead.
     */
    emmbuf = testemm_build_service_request(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    memset(&test_ue1->initial_ue_param, 0,
            sizeof(test_ue1->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue1, emmbuf, S1AP_RRC_Establishment_Cause_mo_Data, true);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive InitialContextSetupRequest */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_InitialContextSetup,
            test_ue1->s1ap_procedure_code);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send InitialContextSetupResponse */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Detach Request for UE-1 */
    emmbuf = testemm_build_detach_request(test_ue1, 1, true, true);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Detach Request for UE-2 on a new S1 context */
    emmbuf = testemm_build_detach_request(test_ue2, 1, true, true);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    memset(&test_ue2->initial_ue_param, 0,
            sizeof(test_ue2->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue2, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, true);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue2));
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue1));

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove(test_ue2);
    test_ue_remove(test_ue1);
}

/*
 * Attach naming a subscriber the MME has never seen.
 *
 *   UE-2   provisioned, never attached  -> no MME-UE context
 *   UE-1   attach, stay connected       -> owns the S1 context
 *   Attach Request(IMSI of UE-2) sent with the S1AP IDs of UE-1
 *
 * mme_ue_find_by_message() finds nothing, so there is no migration and
 * no abort. Instead UE-1's context is quietly re-labelled as UE-2 and
 * its session goes with it - which is why a fix that only acts when the
 * identity resolves to another context is not enough.
 *
 * The Service Request at the end is what catches it.
 */
static void attach_unknown_identity_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;

    /* UE-1 owns the S1 context that the ATTACH REQUEST arrives on */
    test_ue_t *test_ue1 = NULL;
    test_sess_t *sess1 = NULL;
    test_bearer_t *bearer1 = NULL;
    bson_t *doc1 = NULL;

    /* UE-2 is provisioned but has never attached */
    test_ue_t *test_ue2 = NULL;
    test_sess_t *sess2 = NULL;
    bson_t *doc2 = NULL;

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

    test_ue1 = test_ue_add_by_suci(&mobile_identity_suci, "0000000032");
    ogs_assert(test_ue1);

    test_ue1->e_cgi.cell_id = 0x1079baf0;
    test_ue1->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue1->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue1->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue1->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess1 = test_sess_add_by_apn(
            test_ue1, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess1);

    test_ue2 = test_ue_add_by_suci(&mobile_identity_suci, "0000000033");
    ogs_assert(test_ue2);

    test_ue2->e_cgi.cell_id = 0x1079baf0;
    test_ue2->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue2->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue2->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue2->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess2 = test_sess_add_by_apn(
            test_ue2, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess2);

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
    doc1 = test_db_new_simple(test_ue1);
    ABTS_PTR_NOTNULL(tc, doc1);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue1, doc1));

    doc2 = test_db_new_simple(test_ue2);
    ABTS_PTR_NOTNULL(tc, doc2);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue2, doc2));


    /* UE-1 attaches and stays connected with an active default bearer */
    /* Send Attach Request */
    memset(&sess1->pdn_connectivity_param,
            0, sizeof(sess1->pdn_connectivity_param));
    sess1->pdn_connectivity_param.eit = 1;
    sess1->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess1, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue1->attach_request_param,
            0, sizeof(test_ue1->attach_request_param));
    test_ue1->attach_request_param.drx_parameter = 1;
    test_ue1->attach_request_param.ms_network_capability = 1;
    test_ue1->attach_request_param.tmsi_status = 1;
    test_ue1->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue1->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue1, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue1->initial_ue_param, 0, sizeof(test_ue1->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue1, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Security mode complete */
    test_ue1->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send ESM Information Response */
    sess1->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess1);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue1->nr_cgi.cell_id = 0x1234502;
    bearer1 = test_bearer_find_by_ue_ebi(test_ue1, 5);
    ogs_assert(bearer1);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer1, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue1, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Attach for UE-2, delivered with the S1AP IDs of UE-1 */
    sess2->pti = 1;

    memset(&sess2->pdn_connectivity_param,
            0, sizeof(sess2->pdn_connectivity_param));
    sess2->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess2, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue2->attach_request_param,
            0, sizeof(test_ue2->attach_request_param));
    emmbuf = testemm_build_attach_request(test_ue2, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*
     * Expect the release, not a reply to the Attach.
     * Without the fix the MME is already gone by now.
     */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_UEContextRelease,
            test_ue1->s1ap_procedure_code);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*
     * Did UE-1 survive with its session?
     *
     * The MME answers a Service Request with an
     * InitialContextSetupRequest only while the context still has a
     * session with active EPS bearers. A re-labelled or emptied UE-1
     * gets a Service Reject in a downlinkNASTransport instead.
     */
    emmbuf = testemm_build_service_request(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    memset(&test_ue1->initial_ue_param, 0,
            sizeof(test_ue1->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue1, emmbuf, S1AP_RRC_Establishment_Cause_mo_Data, true);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive InitialContextSetupRequest */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_InitialContextSetup,
            test_ue1->s1ap_procedure_code);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send InitialContextSetupResponse */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Detach Request for UE-1 */
    emmbuf = testemm_build_detach_request(test_ue1, 1, true, true);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue2));
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue1));

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove(test_ue2);
    test_ue_remove(test_ue1);
}

/*
 * Attach naming another subscriber, on a context with nothing to lose.
 *
 *   UE-2   attach, then go idle       -> context kept, session kept
 *   UE-1   attach, stop at the Authentication Request
 *          -> has an IMSI, not registered, no session yet
 *   Attach Request(IMSI of UE-2) sent with the S1AP IDs of UE-1
 *
 * Nothing is released here. The context is re-used for UE-2 and UE-2's
 * old context is migrated into it, exactly as before the fix - there is
 * no idle state for such a context to fall back to, so releasing it
 * would leave it behind with no timer to age it out.
 *
 * The two subscribers use different K, so an authentication vector
 * taken from the wrong context cannot pass unnoticed.
 */
static void attach_during_authentication_func(
        abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;

    /* UE-1 starts an attach and never finishes it */
    test_ue_t *test_ue1 = NULL;
    test_sess_t *sess1 = NULL;
    bson_t *doc1 = NULL;

    /* UE-2 is attached and idle, so its context exists with a session */
    test_ue_t *test_ue2 = NULL;
    test_sess_t *sess2 = NULL;
    test_bearer_t *bearer2 = NULL;
    bson_t *doc2 = NULL;

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

    test_ue1 = test_ue_add_by_suci(&mobile_identity_suci, "0000000034");
    ogs_assert(test_ue1);

    test_ue1->e_cgi.cell_id = 0x1079baf0;
    test_ue1->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue1->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    /*
     * Use a different K from UE-2, so that an authentication vector
     * taken from the wrong context cannot pass unnoticed.
     */
    test_ue1->k_string = "8baf473f2f8fd09487cccbd7097c6862";
    test_ue1->opc_string = "8e27b6af0e692e750f32667a3b14605d";

    sess1 = test_sess_add_by_apn(
            test_ue1, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess1);

    test_ue2 = test_ue_add_by_suci(&mobile_identity_suci, "0000000035");
    ogs_assert(test_ue2);

    test_ue2->e_cgi.cell_id = 0x1079baf0;
    test_ue2->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue2->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue2->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue2->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess2 = test_sess_add_by_apn(
            test_ue2, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess2);

    /* Both UEs are served by the same eNB, so keep the eNB-UE-S1AP-ID
     * ranges apart */
    test_ue2->enb_ue_s1ap_id = 100;

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
    doc1 = test_db_new_simple(test_ue1);
    ABTS_PTR_NOTNULL(tc, doc1);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue1, doc1));

    doc2 = test_db_new_simple(test_ue2);
    ABTS_PTR_NOTNULL(tc, doc2);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue2, doc2));

    /* Send Attach Request */
    memset(&sess2->pdn_connectivity_param,
            0, sizeof(sess2->pdn_connectivity_param));
    sess2->pdn_connectivity_param.eit = 1;
    sess2->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess2, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue2->attach_request_param,
            0, sizeof(test_ue2->attach_request_param));
    test_ue2->attach_request_param.drx_parameter = 1;
    test_ue2->attach_request_param.ms_network_capability = 1;
    test_ue2->attach_request_param.tmsi_status = 1;
    test_ue2->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue2->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue2, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue2->initial_ue_param, 0, sizeof(test_ue2->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue2, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Security mode complete */
    test_ue2->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send ESM Information Response */
    sess2->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess2);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue2->nr_cgi.cell_id = 0x1234502;
    bearer2 = test_bearer_find_by_ue_ebi(test_ue2, 5);
    ogs_assert(bearer2);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer2, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);
    /* Send UEContextReleaseRequest - UE-2 goes idle */
    sendbuf = test_s1ap_build_ue_context_release_request(test_ue2,
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_user_inactivity);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UEContextReleaseCommand */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UEContextReleaseComplete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* UE-1: attach only as far as the Authentication Request */

    /* Send Attach Request */
    memset(&sess1->pdn_connectivity_param,
            0, sizeof(sess1->pdn_connectivity_param));
    sess1->pdn_connectivity_param.eit = 1;
    sess1->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess1, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue1->attach_request_param,
            0, sizeof(test_ue1->attach_request_param));
    test_ue1->attach_request_param.drx_parameter = 1;
    test_ue1->attach_request_param.ms_network_capability = 1;
    test_ue1->attach_request_param.tmsi_status = 1;
    test_ue1->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue1->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue1, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue1->initial_ue_param, 0, sizeof(test_ue1->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue1, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request - and do not answer it */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);
    ABTS_INT_EQUAL(tc,
            OGS_NAS_EPS_AUTHENTICATION_REQUEST,
            test_ue1->emm_message_type);

    /* Send ATTACH REQUEST for UE-2 on the S1 context of UE-1 */
    sess2->pti = 1;

    memset(&sess2->pdn_connectivity_param,
            0, sizeof(sess2->pdn_connectivity_param));
    sess2->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess2, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue2->attach_request_param,
            0, sizeof(test_ue2->attach_request_param));
    emmbuf = testemm_build_attach_request(test_ue2, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* The S1 context now carries UE-2 */
    test_ue2->enb_ue_s1ap_id = test_ue1->enb_ue_s1ap_id;
    test_ue2->mme_ue_s1ap_id = test_ue1->mme_ue_s1ap_id;

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_downlinkNASTransport,
            test_ue2->s1ap_procedure_code);
    ABTS_INT_EQUAL(tc,
            OGS_NAS_EPS_AUTHENTICATION_REQUEST,
            test_ue2->emm_message_type);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Security mode complete */
    test_ue2->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send ESM Information Response */
    sess2->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess2);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    bearer2 = test_bearer_find_by_ue_ebi(test_ue2, 5);
    ogs_assert(bearer2);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer2, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Detach Request for UE-2 */
    emmbuf = testemm_build_detach_request(test_ue2, 1, true, true);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue2));
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue1));

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove(test_ue2);
    test_ue_remove(test_ue1);
}

/*
 * Attach naming another subscriber, on a half-finished attach.
 *
 *   UE-2   attach, then go idle       -> context kept, session kept
 *   UE-1   attach, stop before ATTACH COMPLETE
 *          -> not registered, but the session already exists
 *   Attach Request(IMSI of UE-2) sent with the S1AP IDs of UE-1
 *
 * This context cannot be re-labelled - moving UE-2's session into it is
 * what breaks Phase-2 of mme_ue_set_imsi() - so it is released like any
 * other. UE-1 then attaches again to show that what the MME kept does
 * not get in the way, and UE-2 is detached to show it was untouched.
 */
static void attach_during_session_setup_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;

    /* UE-1 stops before ATTACH COMPLETE */
    test_ue_t *test_ue1 = NULL;
    test_sess_t *sess1 = NULL;
    test_bearer_t *bearer1 = NULL;
    bson_t *doc1 = NULL;

    /* UE-2 is attached and idle, so its context has a session */
    test_ue_t *test_ue2 = NULL;
    test_sess_t *sess2 = NULL;
    test_bearer_t *bearer2 = NULL;
    bson_t *doc2 = NULL;

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

    test_ue1 = test_ue_add_by_suci(&mobile_identity_suci, "0000000036");
    ogs_assert(test_ue1);

    test_ue1->e_cgi.cell_id = 0x1079baf0;
    test_ue1->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue1->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue1->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue1->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess1 = test_sess_add_by_apn(
            test_ue1, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess1);

    test_ue2 = test_ue_add_by_suci(&mobile_identity_suci, "0000000037");
    ogs_assert(test_ue2);

    test_ue2->e_cgi.cell_id = 0x1079baf0;
    test_ue2->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue2->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue2->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue2->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    sess2 = test_sess_add_by_apn(
            test_ue2, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess2);

    /* Both UEs are served by the same eNB, so keep the eNB-UE-S1AP-ID
     * ranges apart */
    test_ue2->enb_ue_s1ap_id = 100;

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
    doc1 = test_db_new_simple(test_ue1);
    ABTS_PTR_NOTNULL(tc, doc1);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue1, doc1));

    doc2 = test_db_new_simple(test_ue2);
    ABTS_PTR_NOTNULL(tc, doc2);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue2, doc2));

    /* Send Attach Request */
    memset(&sess2->pdn_connectivity_param,
            0, sizeof(sess2->pdn_connectivity_param));
    sess2->pdn_connectivity_param.eit = 1;
    sess2->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess2, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue2->attach_request_param,
            0, sizeof(test_ue2->attach_request_param));
    test_ue2->attach_request_param.drx_parameter = 1;
    test_ue2->attach_request_param.ms_network_capability = 1;
    test_ue2->attach_request_param.tmsi_status = 1;
    test_ue2->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue2->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue2, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue2->initial_ue_param, 0, sizeof(test_ue2->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue2, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send Security mode complete */
    test_ue2->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send ESM Information Response */
    sess2->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess2);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue2->nr_cgi.cell_id = 0x1234502;
    bearer2 = test_bearer_find_by_ue_ebi(test_ue2, 5);
    ogs_assert(bearer2);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer2, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue2, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue2, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);
    /* Send UEContextReleaseRequest - UE-2 goes idle */
    sendbuf = test_s1ap_build_ue_context_release_request(test_ue2,
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_user_inactivity);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UEContextReleaseCommand */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UEContextReleaseComplete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* UE-1 attaches but never sends ATTACH COMPLETE */
    /* Send Attach Request */
    memset(&sess1->pdn_connectivity_param,
            0, sizeof(sess1->pdn_connectivity_param));
    sess1->pdn_connectivity_param.eit = 1;
    sess1->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess1, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue1->attach_request_param,
            0, sizeof(test_ue1->attach_request_param));
    test_ue1->attach_request_param.drx_parameter = 1;
    test_ue1->attach_request_param.ms_network_capability = 1;
    test_ue1->attach_request_param.tmsi_status = 1;
    test_ue1->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue1->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue1, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue1->initial_ue_param, 0, sizeof(test_ue1->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue1, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Security mode complete */
    test_ue1->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send ESM Information Response */
    sess1->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess1);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Stop here: the session exists, the attach does not finish */


    /* Attach for UE-2, delivered with the S1AP IDs of UE-1 */
    sess2->pti = 1;

    memset(&sess2->pdn_connectivity_param,
            0, sizeof(sess2->pdn_connectivity_param));
    sess2->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess2, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue2->attach_request_param,
            0, sizeof(test_ue2->attach_request_param));
    emmbuf = testemm_build_attach_request(test_ue2, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*
     * Expect the release, not a reply to the Attach.
     * Without the fix the MME is already gone by now.
     */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_UEContextRelease,
            test_ue1->s1ap_procedure_code);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /*
     * UE-1 attaches again on a new S1 context.
     *
     * The MME keeps UE-1's context across the release above, the same
     * way it keeps one when an E-RAB setup failure ends an attach. What
     * matters is that the leftover does not block the next attach.
     */
    sess1->pti = 1;
    /* Send Attach Request */
    memset(&sess1->pdn_connectivity_param,
            0, sizeof(sess1->pdn_connectivity_param));
    sess1->pdn_connectivity_param.eit = 1;
    sess1->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess1, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue1->attach_request_param,
            0, sizeof(test_ue1->attach_request_param));
    test_ue1->attach_request_param.drx_parameter = 1;
    test_ue1->attach_request_param.ms_network_capability = 1;
    test_ue1->attach_request_param.tmsi_status = 1;
    test_ue1->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue1->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue1, esmbuf, false, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue1->initial_ue_param, 0, sizeof(test_ue1->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue1, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Security mode complete */
    test_ue1->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send ESM Information Response */
    sess1->esm_information_param.epco = 1;
    esmbuf = testesm_build_esm_information_response(sess1);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue1->nr_cgi.cell_id = 0x1234502;
    bearer1 = test_bearer_find_by_ue_ebi(test_ue1, 5);
    ogs_assert(bearer1);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer1, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue1, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send Detach Request for UE-1 */
    emmbuf = testemm_build_detach_request(test_ue1, 1, true, true);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue1, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue1, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* UE-2 was not touched by any of this */

    /* Send Detach Request for UE-2 on a new S1 context */
    emmbuf = testemm_build_detach_request(test_ue2, 1, true, true);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    memset(&test_ue2->initial_ue_param, 0,
            sizeof(test_ue2->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue2, emmbuf,
            S1AP_RRC_Establishment_Cause_mo_Signalling, true);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue2, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue2));
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue1));

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    test_ue_remove(test_ue2);
    test_ue_remove(test_ue1);
}

abts_suite *test_identity(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, attach_identity_mismatch_func, NULL);
    abts_run_test(suite, attach_unknown_identity_func, NULL);
    abts_run_test(suite, attach_during_authentication_func, NULL);
    abts_run_test(suite, attach_during_session_setup_func, NULL);

    return suite;
}
