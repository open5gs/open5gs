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

#include "test-common.h"

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *ngap1, *ngap2; /* ngap1 - Old AMF ; ngap2 - Target AMF */
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *gsmbuf;
    ogs_pkbuf_t *nasbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_pkbuf_t *recvbuf_target;
    ogs_ngap_message_t message;
    int i;

    uint8_t tmp[OGS_HUGE_LEN];
    char *_gtp_payload = "34ff0024"
        "0000000100000085 010002004500001c 0c0b000040015a7a 0a2d00010a2d0002"
        "00000964cd7c291f";

    /* Test: 1 UE, Old AMF, Target AMF,
     *       register to Old AMF, PDU session establishment, session release, deregister,
     *       register to Target AMF with empty 5G GUTI, identity request and response,
     *       NO UE context transfer, NO Registration status update,
     *       PDU session establishment, session release, deregister */

    #define NUM_OF_TEST_UE_1 1

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue[NUM_OF_TEST_UE_1];
    test_ue_t *test_ue_target[NUM_OF_TEST_UE_1];
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;

    bson_t *doc = NULL;

    /* First gNB connects to Target AMF */
    ngap2 = testngap_client(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap2);

    /* Second gNB connects to Old AMF */
    ngap1 = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap1);

    /* Two gNB connects to UPF */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* NG-Setup Reqeust/Response for Source gNB */
    sendbuf = testngap_build_ng_setup_request(0x4000, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testgnb_ngap_read(ngap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* NG-Setup Reqeust/Response for Target gNB */
    sendbuf = testngap_build_ng_setup_request(0x4001, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf_target = testgnb_ngap_read(ngap1);
    ABTS_PTR_NOTNULL(tc, recvbuf_target);
    testngap_recv(test_ue_target, recvbuf_target);

    /* Register to Old AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
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

        test_ue[i]->nr_cgi.cell_id = 0x40001;

        test_ue[i]->nas.registration.tsc = 0;
        test_ue[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue[i]->nas.registration.follow_on_request = 1;
        test_ue[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

        test_ue[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue[i]->ran_ue_ngap_id = test_ue[i-1]->ran_ue_ngap_id;
        else
            test_ue[i]->ran_ue_ngap_id = 0;

        /* Send PDU session establishment request */
        sess = test_sess_add_by_dnn_and_psi(test_ue[i], "internet", 5);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue[i], doc));

        /* Send Registration request */
        test_ue[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue[i]->registration_request_param.gmm_capability = 1;
        test_ue[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue[i]->registration_request_param.requested_nssai = 1;
        test_ue[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Identity request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Authentication request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Authentication response */
        gmmbuf = testgmm_build_authentication_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Security mode command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Security mode complete */
        gmmbuf = testgmm_build_security_mode_complete(test_ue[i], nasbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive InitialContextSetupRequest +
         * Registration accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_InitialContextSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send UERadioCapabilityInfoIndication */
        sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send InitialContextSetupResponse */
        sendbuf = testngap_build_initial_context_setup_response(test_ue[i], false);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send Registration complete */
        gmmbuf = testgmm_build_registration_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Configuration update command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        sess->ul_nas_transport_param.request_type =
            OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
        sess->ul_nas_transport_param.dnn = 1;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 1;
        sess->pdu_session_establishment_param.epco = 1;

        gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceSetupRequest +
         * DL NAS transport +
         * PDU session establishment accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceSetupResponse */
        sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send PDU session release request */
        sess = test_sess_find_by_psi(test_ue[i], 5);
        ogs_assert(sess);

        /* Send PDU Session release request */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceReleaseCommand +
         * DL NAS transport +
         * PDU session release command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceReleaseResponse */
        sendbuf = testngap_build_pdu_session_resource_release_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send UplinkNASTransport +
         * UL NAS trasnport +
         * PDU session resource release complete */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_complete(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    /* Stay registered on Old AMF */
#if 0
    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send De-registration request */
        gmmbuf = testgmm_build_de_registration_request(test_ue[i], 1, true, true);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive UEContextReleaseCommand */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* Clear Test UE Context */
    test_ue_remove_all();
#endif

    ogs_msleep(100);

    /* Register to Target AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        const char *scheme_output_target[] = {
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

        test_ue_target[i] = test_ue_add_by_suci(
                &mobile_identity_suci, scheme_output_target[i]);
        ogs_assert(test_ue_target[i]);

        test_ue_target[i]->nr_cgi.cell_id = 0x40000;

        test_ue_target[i]->nas.registration.tsc = 0;
        test_ue_target[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue_target[i]->nas.registration.follow_on_request = 1;
        test_ue_target[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

        test_ue_target[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue_target[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue_target[i]->ran_ue_ngap_id = test_ue_target[i-1]->ran_ue_ngap_id;
        else
            test_ue_target[i]->ran_ue_ngap_id = 0;

        /* Send PDU session establishment request */
        // sess = test_sess_add_by_dnn_and_psi(test_ue[i], "internet", 5);
        sess = test_sess_add_by_dnn_and_psi(test_ue_target[i], "internet", 6);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue_target[i], doc));

        /* Send Registration request */
        test_ue_target[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue_target[i]->registration_request_param.gmm_capability = 1;
        test_ue_target[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue_target[i]->registration_request_param.requested_nssai = 1;
        test_ue_target[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue_target[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue_target[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Identity request */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Authentication request */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Authentication response */
        gmmbuf = testgmm_build_authentication_response(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Security mode command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Security mode complete */
        gmmbuf = testgmm_build_security_mode_complete(test_ue_target[i], nasbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive InitialContextSetupRequest +
         * Registration accept */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_InitialContextSetup,
                test_ue_target[i]->ngap_procedure_code);

        /* Send UERadioCapabilityInfoIndication */
        sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send InitialContextSetupResponse */
        sendbuf = testngap_build_initial_context_setup_response(test_ue_target[i], false);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send Registration complete */
        gmmbuf = testgmm_build_registration_complete(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Configuration update command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        sess->ul_nas_transport_param.request_type =
            OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
        sess->ul_nas_transport_param.dnn = 1;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 1;
        sess->pdu_session_establishment_param.epco = 1;

        gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceSetupRequest +
         * DL NAS transport +
         * PDU session establishment accept */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                test_ue_target[i]->ngap_procedure_code);

        /* Send PDUSessionResourceSetupResponse */
        sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send PDU session release request */
        sess = test_sess_find_by_psi(test_ue_target[i], 6);
        ogs_assert(sess);

        /* Send PDU Session release request */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceReleaseCommand +
         * DL NAS transport +
         * PDU session release command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                test_ue_target[i]->ngap_procedure_code);

        /* Send PDUSessionResourceReleaseResponse */
        sendbuf = testngap_build_pdu_session_resource_release_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send UplinkNASTransport +
         * UL NAS trasnport +
         * PDU session resource release complete */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_complete(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send De-registration request */
        gmmbuf = testgmm_build_de_registration_request(test_ue_target[i], 1, true, true);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive UEContextReleaseCommand */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue_target[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue_target[i]));
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* Clear Test UE Context */
    test_ue_remove_all();

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu2);
    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu1);

    /* gNB disonncect from Target AMF */
    testgnb_ngap_close(ngap2);
    /* gNB disonncect from Old AMF */
    testgnb_ngap_close(ngap1);

}

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *ngap1, *ngap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *gsmbuf;
    ogs_pkbuf_t *nasbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_pkbuf_t *recvbuf_target;
    ogs_ngap_message_t message;
    int i;

    uint8_t tmp[OGS_HUGE_LEN];
    char *_gtp_payload = "34ff0024"
        "0000000100000085 010002004500001c 0c0b000040015a7a 0a2d00010a2d0002"
        "00000964cd7c291f";

    #define NUM_OF_TEST_UE_1 1

    /* Test: 1 UE, Old AMF, Target AMF,
     *       register to Old AMF, PDU session establishment, session release, deregister,
     *       register to Target AMF with Old AMF's 5G GUTI,
     *       UE context transfer, Registration status update
     *       PDU session establishment, session release, deregister */

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue[NUM_OF_TEST_UE_1];
    test_ue_t *test_ue_target[NUM_OF_TEST_UE_1];
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;

    bson_t *doc = NULL;

    /* First gNB connects to Target AMF */
    ngap2 = testngap_client(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap2);

    /* Second gNB connects to Old AMF */
    ngap1 = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap1);

    /* Two gNB connects to UPF */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* NG-Setup Reqeust/Response for Target gNB - with Target AMF */
    sendbuf = testngap_build_ng_setup_request(0x4000, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testgnb_ngap_read(ngap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* NG-Setup Reqeust/Response for Source gNB - with Old AMF */
    sendbuf = testngap_build_ng_setup_request(0x4001, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf_target = testgnb_ngap_read(ngap1);
    ABTS_PTR_NOTNULL(tc, recvbuf_target);
    testngap_recv(test_ue, recvbuf_target);

    /* Register to Old AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
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

        test_ue[i]->nr_cgi.cell_id = 0x40001;

        test_ue[i]->nas.registration.tsc = 0;
        test_ue[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue[i]->nas.registration.follow_on_request = 1;
        test_ue[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

        test_ue[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue[i]->ran_ue_ngap_id = test_ue[i-1]->ran_ue_ngap_id;
        else
            test_ue[i]->ran_ue_ngap_id = 0;

        /* pdu_id == 5 */

        /* Send PDU session establishment request */
        sess = test_sess_add_by_dnn_and_psi(test_ue[i], "internet", 5);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue[i], doc));

        /* Send Registration request - with SUCI */
        // test_ue[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue[i]->registration_request_param.gmm_capability = 1;
        test_ue[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue[i]->registration_request_param.requested_nssai = 1;
        test_ue[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);

        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
        /* Receive Identity request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

        /* Receive Authentication request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Authentication response */
        gmmbuf = testgmm_build_authentication_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Security mode command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Security mode complete */
        gmmbuf = testgmm_build_security_mode_complete(test_ue[i], nasbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive InitialContextSetupRequest +
         * Registration accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_InitialContextSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send UERadioCapabilityInfoIndication */
        sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send InitialContextSetupResponse */
        sendbuf = testngap_build_initial_context_setup_response(test_ue[i], false);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send Registration complete */
        gmmbuf = testgmm_build_registration_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Configuration update command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        sess->ul_nas_transport_param.request_type =
            OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
        sess->ul_nas_transport_param.dnn = 1;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 1;
        sess->pdu_session_establishment_param.epco = 1;

        gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceSetupRequest +
         * DL NAS transport +
         * PDU session establishment accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceSetupResponse */
        sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send PDU session release request */
        sess = test_sess_find_by_psi(test_ue[i], 5);
        ogs_assert(sess);

        /* Send PDU Session release request */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceReleaseCommand +
         * DL NAS transport +
         * PDU session release command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceReleaseResponse */
        sendbuf = testngap_build_pdu_session_resource_release_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send UplinkNASTransport +
         * UL NAS trasnport +
         * PDU session resource release complete */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_complete(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send De-registration request */
        gmmbuf = testgmm_build_de_registration_request(test_ue[i], 1, true, true);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive UEContextReleaseCommand */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    /* Register to Target AMF */

    /* Make the same UE from scratch and give it the 5G GUTI from Old AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        const char *scheme_output_target[] = {
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

        test_ue_target[i] = test_ue_add_by_suci(
                &mobile_identity_suci, scheme_output_target[i]);
        ogs_assert(test_ue_target[i]);

        test_ue_target[i]->nr_cgi.cell_id = 0x40000;

        test_ue_target[i]->nas.registration.tsc = 0;
        test_ue_target[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue_target[i]->nas.registration.follow_on_request = 1;
        test_ue_target[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

        test_ue_target[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue_target[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue_target[i]->ran_ue_ngap_id = test_ue_target[i-1]->ran_ue_ngap_id;
        else
            test_ue_target[i]->ran_ue_ngap_id = 0;

        /* pdu_id == 6 */

        /* Send PDU session establishment request */
        sess = test_sess_add_by_dnn_and_psi(test_ue_target[i], "internet", 6);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue_target[i], doc));

        /* Set the 5G GUTI to Old AMF's 5G GUTI */
        test_ue_target[i]->nas_5gs_guti = test_ue[i]->nas_5gs_guti;

        /* Send Registration request */
        test_ue_target[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue_target[i]->registration_request_param.gmm_capability = 1;
        test_ue_target[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue_target[i]->registration_request_param.requested_nssai = 1;
        test_ue_target[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue_target[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue_target[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
        /* Receive Identity request */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

        /* Receive Authentication request */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Authentication response */
        gmmbuf = testgmm_build_authentication_response(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Security mode command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Security mode complete */
        gmmbuf = testgmm_build_security_mode_complete(test_ue_target[i], nasbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive InitialContextSetupRequest +
         * Registration accept */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_InitialContextSetup,
                test_ue_target[i]->ngap_procedure_code);

        /* Send UERadioCapabilityInfoIndication */
        sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send InitialContextSetupResponse */
        sendbuf = testngap_build_initial_context_setup_response(test_ue_target[i], false);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send Registration complete */
        gmmbuf = testgmm_build_registration_complete(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Configuration update command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        sess->ul_nas_transport_param.request_type =
            OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
        sess->ul_nas_transport_param.dnn = 1;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 1;
        sess->pdu_session_establishment_param.epco = 1;

        gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceSetupRequest +
         * DL NAS transport +
         * PDU session establishment accept */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                test_ue_target[i]->ngap_procedure_code);

        /* Send PDUSessionResourceSetupResponse */
        sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send PDU session release request */
        sess = test_sess_find_by_psi(test_ue_target[i], 6);
        ogs_assert(sess);

        /* Send PDU Session release request */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceReleaseCommand +
         * DL NAS transport +
         * PDU session release command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                test_ue_target[i]->ngap_procedure_code);

        /* Send PDUSessionResourceReleaseResponse */
        sendbuf = testngap_build_pdu_session_resource_release_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send UplinkNASTransport +
         * UL NAS trasnport +
         * PDU session resource release complete */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_complete(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send De-registration request */
        gmmbuf = testgmm_build_de_registration_request(test_ue_target[i], 1, true, true);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive UEContextReleaseCommand */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue_target[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue_target[i]));
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* Clear Test UE Context */
    test_ue_remove_all();

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu2);
    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu1);

    /* gNB disonncect from Target AMF */
    testgnb_ngap_close(ngap2);
    /* gNB disonncect from Old AMF */
    testgnb_ngap_close(ngap1);
}

static void test23_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *ngap1, *ngap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *gsmbuf;
    ogs_pkbuf_t *nasbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_pkbuf_t *recvbuf_target;
    ogs_ngap_message_t message;
    int i,j;

    uint8_t tmp[OGS_HUGE_LEN];
    char *_gtp_payload = "34ff0024"
        "0000000100000085 010002004500001c 0c0b000040015a7a 0a2d00010a2d0002"
        "00000964cd7c291f";

    #define NUM_OF_TEST_UE_3 3

    /* Test: same test as the previous one, now 3 UEs are registered, one after another */

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue[NUM_OF_TEST_UE_3];
    test_ue_t *test_ue_target[NUM_OF_TEST_UE_3];
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;

    bson_t *doc = NULL;

    /* First gNB connects to Target AMF */
    ngap2 = testngap_client(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap2);

    /* Second gNB connects to Old AMF */
    ngap1 = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap1);

    /* Two gNB connects to UPF */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* NG-Setup Reqeust/Response for Target gNB - with Target AMF */
    sendbuf = testngap_build_ng_setup_request(0x4000, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testgnb_ngap_read(ngap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* NG-Setup Reqeust/Response for Source gNB - with Old AMF */
    sendbuf = testngap_build_ng_setup_request(0x4001, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf_target = testgnb_ngap_read(ngap1);
    ABTS_PTR_NOTNULL(tc, recvbuf_target);
    testngap_recv(test_ue, recvbuf_target);

    for (j = 0; j < 3; j++) {

        /* Register to Old AMF */

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
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

            test_ue[i]->nr_cgi.cell_id = 0x40001;

            test_ue[i]->nas.registration.tsc = 0;
            test_ue[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
            test_ue[i]->nas.registration.follow_on_request = 1;
            test_ue[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

            test_ue[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
            test_ue[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
        }

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            if (i > 0)
                test_ue[i]->ran_ue_ngap_id = test_ue[i-1]->ran_ue_ngap_id;
            else
                test_ue[i]->ran_ue_ngap_id = 0;

            /* Send PDU session establishment request */
            sess = test_sess_add_by_dnn_and_psi(test_ue[i], "internet", 5);
            ogs_assert(sess);

            /********** Insert Subscriber in Database */
            doc = test_db_new_simple(test_ue[i]);
            ABTS_PTR_NOTNULL(tc, doc);
            ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue[i], doc));

            /* Send Registration request - with SUCI */
            // test_ue[i]->registration_request_param.guti = 1;
            gmmbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
            ABTS_PTR_NOTNULL(tc, gmmbuf);

            test_ue[i]->registration_request_param.gmm_capability = 1;
            test_ue[i]->registration_request_param.s1_ue_network_capability = 1;
            test_ue[i]->registration_request_param.requested_nssai = 1;
            test_ue[i]->registration_request_param.last_visited_registered_tai = 1;
            test_ue[i]->registration_request_param.ue_usage_setting = 1;
            nasbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
            ABTS_PTR_NOTNULL(tc, nasbuf);

            sendbuf = testngap_build_initial_ue_message(test_ue[i], gmmbuf,
                        NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
            ABTS_PTR_NOTNULL(tc, sendbuf);

            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
            /* Receive Identity request */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);

            /* Send Identity response */
            gmmbuf = testgmm_build_identity_response(test_ue[i]);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

            /* Receive Authentication request */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);

            /* Send Authentication response */
            gmmbuf = testgmm_build_authentication_response(test_ue[i]);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive Security mode command */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);

            /* Send Security mode complete */
            gmmbuf = testgmm_build_security_mode_complete(test_ue[i], nasbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive InitialContextSetupRequest +
            * Registration accept */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_InitialContextSetup,
                    test_ue[i]->ngap_procedure_code);

            /* Send UERadioCapabilityInfoIndication */
            sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue[i]);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Send InitialContextSetupResponse */
            sendbuf = testngap_build_initial_context_setup_response(test_ue[i], false);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Send Registration complete */
            gmmbuf = testgmm_build_registration_complete(test_ue[i]);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive Configuration update command */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);

            sess->ul_nas_transport_param.request_type =
                OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
            sess->ul_nas_transport_param.dnn = 1;
            sess->ul_nas_transport_param.s_nssai = 0;

            sess->pdu_session_establishment_param.ssc_mode = 1;
            sess->pdu_session_establishment_param.epco = 1;

            gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
            ABTS_PTR_NOTNULL(tc, gsmbuf);
            gmmbuf = testgmm_build_ul_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive PDUSessionResourceSetupRequest +
            * DL NAS transport +
            * PDU session establishment accept */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                    test_ue[i]->ngap_procedure_code);

            /* Send PDUSessionResourceSetupResponse */
            sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
        }

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            /* Send PDU session release request */
            sess = test_sess_find_by_psi(test_ue[i], 5);
            ogs_assert(sess);

            /* Send PDU Session release request */
            sess->ul_nas_transport_param.request_type = 0;
            sess->ul_nas_transport_param.dnn = 0;
            sess->ul_nas_transport_param.s_nssai = 0;

            sess->pdu_session_establishment_param.ssc_mode = 0;
            sess->pdu_session_establishment_param.epco = 0;

            gsmbuf = testgsm_build_pdu_session_release_request(sess);
            ABTS_PTR_NOTNULL(tc, gsmbuf);
            gmmbuf = testgmm_build_ul_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive PDUSessionResourceReleaseCommand +
            * DL NAS transport +
            * PDU session release command */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                    test_ue[i]->ngap_procedure_code);

            /* Send PDUSessionResourceReleaseResponse */
            sendbuf = testngap_build_pdu_session_resource_release_response(sess);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Send UplinkNASTransport +
            * UL NAS trasnport +
            * PDU session resource release complete */
            sess->ul_nas_transport_param.request_type = 0;
            sess->ul_nas_transport_param.dnn = 0;
            sess->ul_nas_transport_param.s_nssai = 0;

            sess->pdu_session_establishment_param.ssc_mode = 0;
            sess->pdu_session_establishment_param.epco = 0;

            gsmbuf = testgsm_build_pdu_session_release_complete(sess);
            ABTS_PTR_NOTNULL(tc, gsmbuf);
            gmmbuf = testgmm_build_ul_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
        }

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            /* Send De-registration request */
            gmmbuf = testgmm_build_de_registration_request(test_ue[i], 1, true, true);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive UEContextReleaseCommand */
            recvbuf = testgnb_ngap_read(ngap1);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            testngap_recv(test_ue[i], recvbuf);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_UEContextRelease,
                    test_ue[i]->ngap_procedure_code);

            /* Send UEContextReleaseComplete */
            sendbuf = testngap_build_ue_context_release_complete(test_ue[i]);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap1, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
        }

        ogs_msleep(300);

        /* Register to Target AMF */

        /* Make the same UE from scratch and give it the 5G GUTI from Old AMF */

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            const char *scheme_output_target[] = {
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

            test_ue_target[i] = test_ue_add_by_suci(
                    &mobile_identity_suci, scheme_output_target[i]);
            ogs_assert(test_ue_target[i]);

            test_ue_target[i]->nr_cgi.cell_id = 0x40000;

            test_ue_target[i]->nas.registration.tsc = 0;
            test_ue_target[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
            test_ue_target[i]->nas.registration.follow_on_request = 1;
            test_ue_target[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

            test_ue_target[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
            test_ue_target[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
        }

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            if (i > 0)
                test_ue_target[i]->ran_ue_ngap_id = test_ue_target[i-1]->ran_ue_ngap_id;
            else
                test_ue_target[i]->ran_ue_ngap_id = 0;

            /* Send PDU session establishment request */
            sess = test_sess_add_by_dnn_and_psi(test_ue_target[i], "internet", 6);
            ogs_assert(sess);

            /********** Insert Subscriber in Database */
            doc = test_db_new_simple(test_ue_target[i]);
            ABTS_PTR_NOTNULL(tc, doc);
            ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue_target[i], doc));

            /* Set the 5G GUTI to Old AMF's 5G GUTI */
            test_ue_target[i]->nas_5gs_guti = test_ue[i]->nas_5gs_guti;

            /* Send Registration request */
            test_ue_target[i]->registration_request_param.guti = 1;
            gmmbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
            ABTS_PTR_NOTNULL(tc, gmmbuf);

            test_ue_target[i]->registration_request_param.gmm_capability = 1;
            test_ue_target[i]->registration_request_param.s1_ue_network_capability = 1;
            test_ue_target[i]->registration_request_param.requested_nssai = 1;
            test_ue_target[i]->registration_request_param.last_visited_registered_tai = 1;
            test_ue_target[i]->registration_request_param.ue_usage_setting = 1;
            nasbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
            ABTS_PTR_NOTNULL(tc, nasbuf);

            sendbuf = testngap_build_initial_ue_message(test_ue_target[i], gmmbuf,
                        NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
            /* Receive Identity request */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);

            /* Send Identity response */
            gmmbuf = testgmm_build_identity_response(test_ue_target[i]);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

            /* Receive Authentication request */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);

            /* Send Authentication response */
            gmmbuf = testgmm_build_authentication_response(test_ue_target[i]);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive Security mode command */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);

            /* Send Security mode complete */
            gmmbuf = testgmm_build_security_mode_complete(test_ue_target[i], nasbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive InitialContextSetupRequest +
            * Registration accept */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_InitialContextSetup,
                    test_ue_target[i]->ngap_procedure_code);

            /* Send UERadioCapabilityInfoIndication */
            sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue_target[i]);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Send InitialContextSetupResponse */
            sendbuf = testngap_build_initial_context_setup_response(test_ue_target[i], false);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Send Registration complete */
            gmmbuf = testgmm_build_registration_complete(test_ue_target[i]);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive Configuration update command */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);

            sess->ul_nas_transport_param.request_type =
                OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
            sess->ul_nas_transport_param.dnn = 1;
            sess->ul_nas_transport_param.s_nssai = 0;

            sess->pdu_session_establishment_param.ssc_mode = 1;
            sess->pdu_session_establishment_param.epco = 1;

            gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
            ABTS_PTR_NOTNULL(tc, gsmbuf);
            gmmbuf = testgmm_build_ul_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive PDUSessionResourceSetupRequest +
            * DL NAS transport +
            * PDU session establishment accept */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                    test_ue_target[i]->ngap_procedure_code);

            /* Send PDUSessionResourceSetupResponse */
            sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
        }

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            /* Send PDU session release request */
            sess = test_sess_find_by_psi(test_ue_target[i], 6);
            ogs_assert(sess);

            /* Send PDU Session release request */
            sess->ul_nas_transport_param.request_type = 0;
            sess->ul_nas_transport_param.dnn = 0;
            sess->ul_nas_transport_param.s_nssai = 0;

            sess->pdu_session_establishment_param.ssc_mode = 0;
            sess->pdu_session_establishment_param.epco = 0;

            gsmbuf = testgsm_build_pdu_session_release_request(sess);
            ABTS_PTR_NOTNULL(tc, gsmbuf);
            gmmbuf = testgmm_build_ul_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive PDUSessionResourceReleaseCommand +
            * DL NAS transport +
            * PDU session release command */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                    test_ue_target[i]->ngap_procedure_code);

            /* Send PDUSessionResourceReleaseResponse */
            sendbuf = testngap_build_pdu_session_resource_release_response(sess);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Send UplinkNASTransport +
            * UL NAS trasnport +
            * PDU session resource release complete */
            sess->ul_nas_transport_param.request_type = 0;
            sess->ul_nas_transport_param.dnn = 0;
            sess->ul_nas_transport_param.s_nssai = 0;

            sess->pdu_session_establishment_param.ssc_mode = 0;
            sess->pdu_session_establishment_param.epco = 0;

            gsmbuf = testgsm_build_pdu_session_release_complete(sess);
            ABTS_PTR_NOTNULL(tc, gsmbuf);
            gmmbuf = testgmm_build_ul_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
        }

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            /* Send De-registration request */
            gmmbuf = testgmm_build_de_registration_request(test_ue_target[i], 1, true, true);
            ABTS_PTR_NOTNULL(tc, gmmbuf);
            sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);

            /* Receive UEContextReleaseCommand */
            recvbuf_target = testgnb_ngap_read(ngap2);
            ABTS_PTR_NOTNULL(tc, recvbuf_target);
            testngap_recv(test_ue_target[i], recvbuf_target);
            ABTS_INT_EQUAL(tc,
                    NGAP_ProcedureCode_id_UEContextRelease,
                    test_ue_target[i]->ngap_procedure_code);

            /* Send UEContextReleaseComplete */
            sendbuf = testngap_build_ue_context_release_complete(test_ue_target[i]);
            ABTS_PTR_NOTNULL(tc, sendbuf);
            rv = testgnb_ngap_send(ngap2, sendbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, rv);
        }

        ogs_msleep(300);

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            /********** Remove Subscriber in Database */
            ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue_target[i]));
        }

        for (i = 0; i < NUM_OF_TEST_UE_3; i++) {
            /********** Remove Subscriber in Database */
            ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
        }
    }

    /* Clear Test UE Context */
    test_ue_remove_all();

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu2);
    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu1);

    /* gNB disonncect from Target AMF */
    testgnb_ngap_close(ngap2);
    /* gNB disonncect from Old AMF */
    testgnb_ngap_close(ngap1);
}

static void test3_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *ngap1, *ngap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *gsmbuf;
    ogs_pkbuf_t *nasbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_pkbuf_t *recvbuf_target;
    ogs_ngap_message_t message;
    int i;

    uint8_t tmp[OGS_HUGE_LEN];
    char *_gtp_payload = "34ff0024"
        "0000000100000085 010002004500001c 0c0b000040015a7a 0a2d00010a2d0002"
        "00000964cd7c291f";

    /* Test: 1 UE, Old AMF, Target AMF,
     *       register to Old AMF, PDU session establishment, session release, (stay registered)
     *       register to Target AMF with Old AMF's 5G GUTI,
     *       UE context transfer, Registration status update
     *       PDU session establishment, session release, deregister */

    #define NUM_OF_TEST_UE_1 1

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue[NUM_OF_TEST_UE_1];
    test_ue_t *test_ue_target[NUM_OF_TEST_UE_1];
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;

    bson_t *doc = NULL;

    /* First gNB connects to Target AMF */
    ngap2 = testngap_client(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap2);

    /* Second gNB connects to Old AMF */
    ngap1 = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap1);

    /* Two gNB connects to UPF */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* NG-Setup Reqeust/Response for Target gNB - with Target AMF */
    sendbuf = testngap_build_ng_setup_request(0x4000, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testgnb_ngap_read(ngap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* NG-Setup Reqeust/Response for Source gNB - with Old AMF */
    sendbuf = testngap_build_ng_setup_request(0x4001, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf_target = testgnb_ngap_read(ngap1);
    ABTS_PTR_NOTNULL(tc, recvbuf_target);
    testngap_recv(test_ue, recvbuf_target);

    /* Register to Old AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
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

        test_ue[i]->nr_cgi.cell_id = 0x40001;

        test_ue[i]->nas.registration.tsc = 0;
        test_ue[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue[i]->nas.registration.follow_on_request = 1;
        test_ue[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

        test_ue[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue[i]->ran_ue_ngap_id = test_ue[i-1]->ran_ue_ngap_id;
        else
            test_ue[i]->ran_ue_ngap_id = 0;

        /* pdu_id == 5 */

        /* Send PDU session establishment request */
        sess = test_sess_add_by_dnn_and_psi(test_ue[i], "internet", 5);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue[i], doc));

        /* Send Registration request - with SUCI */
        // test_ue[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue[i]->registration_request_param.gmm_capability = 1;
        test_ue[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue[i]->registration_request_param.requested_nssai = 1;
        test_ue[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);

        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
        /* Receive Identity request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

        /* Receive Authentication request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Authentication response */
        gmmbuf = testgmm_build_authentication_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Security mode command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Security mode complete */
        gmmbuf = testgmm_build_security_mode_complete(test_ue[i], nasbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive InitialContextSetupRequest +
         * Registration accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_InitialContextSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send UERadioCapabilityInfoIndication */
        sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send InitialContextSetupResponse */
        sendbuf = testngap_build_initial_context_setup_response(test_ue[i], false);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send Registration complete */
        gmmbuf = testgmm_build_registration_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Configuration update command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        sess->ul_nas_transport_param.request_type =
            OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
        sess->ul_nas_transport_param.dnn = 1;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 1;
        sess->pdu_session_establishment_param.epco = 1;

        gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceSetupRequest +
         * DL NAS transport +
         * PDU session establishment accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceSetupResponse */
        sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    /* Stay registered on Old AMF, with active session ?
     *  - This scenario should be made with "Inter NG-RAN node N2 based handover" beforehand,
     *  but then the UE context transfer is skipped during the registartion
     *  TS 23.502, 4.9.1.3.3, step 12
     */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send PDU session release request */
        sess = test_sess_find_by_psi(test_ue[i], 5);
        ogs_assert(sess);

        /* Send PDU Session release request */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceReleaseCommand +
         * DL NAS transport +
         * PDU session release command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceReleaseResponse */
        sendbuf = testngap_build_pdu_session_resource_release_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send UplinkNASTransport +
         * UL NAS trasnport +
         * PDU session resource release complete */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_complete(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

#if 0
    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send De-registration request */
        gmmbuf = testgmm_build_de_registration_request(test_ue[i], 1, true, true);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive UEContextReleaseCommand */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* Clear Test UE Context */
    test_ue_remove_all();
#endif

    ogs_msleep(100);

    /* Register to Target AMF */

    /* Make the same subscriber from scratch and give him the 5G GUTI from Old AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        const char *scheme_output_target[] = {
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

        test_ue_target[i] = test_ue_add_by_suci(
                &mobile_identity_suci, scheme_output_target[i]);
        ogs_assert(test_ue_target[i]);

        /* Different cell id */
        test_ue_target[i]->nr_cgi.cell_id = 0x40000;

        test_ue_target[i]->nas.registration.tsc = 0;
        test_ue_target[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue_target[i]->nas.registration.follow_on_request = 1;
        /* Mobility registration update */
        test_ue_target[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_MOBILITY_UPDATING;

        test_ue_target[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue_target[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue_target[i]->ran_ue_ngap_id = test_ue_target[i-1]->ran_ue_ngap_id;
        else
            test_ue_target[i]->ran_ue_ngap_id = 0;

        /* pdu_id == 5 */

        /* Send PDU session establishment request */
        sess = test_sess_add_by_dnn_and_psi(test_ue_target[i], "internet", 5);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue_target[i], doc));

        /* Set the 5G GUTI to Old AMF's 5G GUTI */
        test_ue_target[i]->nas_5gs_guti = test_ue[i]->nas_5gs_guti;

        /* Send Registration request */
        test_ue_target[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue_target[i]->registration_request_param.gmm_capability = 1;
        test_ue_target[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue_target[i]->registration_request_param.requested_nssai = 1;
        test_ue_target[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue_target[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue_target[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
        /* Receive Identity request */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

        /* Receive Authentication request */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Authentication response */
        gmmbuf = testgmm_build_authentication_response(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Security mode command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Security mode complete */
        gmmbuf = testgmm_build_security_mode_complete(test_ue_target[i], nasbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive InitialContextSetupRequest +
         * Registration accept */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_InitialContextSetup,
                test_ue_target[i]->ngap_procedure_code);

        /* Send UERadioCapabilityInfoIndication */
        sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send InitialContextSetupResponse */
        sendbuf = testngap_build_initial_context_setup_response(test_ue_target[i], false);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send Registration complete */
        gmmbuf = testgmm_build_registration_complete(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Configuration update command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        sess->ul_nas_transport_param.request_type =
            OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
        sess->ul_nas_transport_param.dnn = 1;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 1;
        sess->pdu_session_establishment_param.epco = 1;

        gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceSetupRequest +
         * DL NAS transport +
         * PDU session establishment accept */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                test_ue_target[i]->ngap_procedure_code);

        /* Send PDUSessionResourceSetupResponse */
        sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send PDU session release request */
        sess = test_sess_find_by_psi(test_ue_target[i], 5);
        ogs_assert(sess);

        /* Send PDU Session release request */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceReleaseCommand +
         * DL NAS transport +
         * PDU session release command */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                test_ue_target[i]->ngap_procedure_code);

        /* Send PDUSessionResourceReleaseResponse */
        sendbuf = testngap_build_pdu_session_resource_release_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send UplinkNASTransport +
         * UL NAS trasnport +
         * PDU session resource release complete */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_complete(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send De-registration request */
        gmmbuf = testgmm_build_de_registration_request(test_ue_target[i], 1, true, true);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive UEContextReleaseCommand */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue_target[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue_target[i]));
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* Clear Test UE Context */
    test_ue_remove_all();

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu2);
    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu1);

    /* gNB disonncect from Target AMF */
    testgnb_ngap_close(ngap2);
    /* gNB disonncect from Old AMF */
    testgnb_ngap_close(ngap1);
}

static void test4_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *ngap1, *ngap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *gsmbuf;
    ogs_pkbuf_t *nasbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_pkbuf_t *recvbuf_target;
    ogs_ngap_message_t message;
    int i;

    uint8_t tmp[OGS_HUGE_LEN];
    char *_gtp_payload = "34ff0024"
        "0000000100000085 010002004500001c 0c0b000040015a7a 0a2d00010a2d0002"
        "00000964cd7c291f";

    #define NUM_OF_TEST_UE_1 1

    /* Test: 1 UE, Old AMF, Target AMF, register to Old AMF, register to Target AMF
     *       with Old AMF's 5G GUTI - AUTH FAIL,
     *       UE context transfer, Registration status update - NOT TRANSFERRED
     *       The old AMF continues
     *       as if the UE context transfer service operation was never received */

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue[NUM_OF_TEST_UE_1];
    test_ue_t *test_ue_target[NUM_OF_TEST_UE_1];
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;

    bson_t *doc = NULL;

    /* First gNB connects to Target AMF */
    ngap2 = testngap_client(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap2);

    /* Second gNB connects to Old AMF */
    ngap1 = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap1);

    /* Two gNB connects to UPF */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* NG-Setup Reqeust/Response for Target gNB - with Target AMF */
    sendbuf = testngap_build_ng_setup_request(0x4000, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testgnb_ngap_read(ngap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* NG-Setup Reqeust/Response for Source gNB - with Old AMF */
    sendbuf = testngap_build_ng_setup_request(0x4001, 28);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf_target = testgnb_ngap_read(ngap1);
    ABTS_PTR_NOTNULL(tc, recvbuf_target);
    testngap_recv(test_ue, recvbuf_target);


    /* Register to Old AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
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

        test_ue[i]->nr_cgi.cell_id = 0x40001;

        test_ue[i]->nas.registration.tsc = 0;
        test_ue[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue[i]->nas.registration.follow_on_request = 1;
        test_ue[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

        test_ue[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue[i]->ran_ue_ngap_id = test_ue[i-1]->ran_ue_ngap_id;
        else
            test_ue[i]->ran_ue_ngap_id = 0;

        /* Send PDU session establishment request */
        sess = test_sess_add_by_dnn_and_psi(test_ue[i], "internet", 5);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue[i], doc));

        /* Send Registration request - with SUCI */
        // test_ue[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue[i]->registration_request_param.gmm_capability = 1;
        test_ue[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue[i]->registration_request_param.requested_nssai = 1;
        test_ue[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);

        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
        /* Receive Identity request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

        /* Receive Authentication request */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Authentication response */
        gmmbuf = testgmm_build_authentication_response(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Security mode command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        /* Send Security mode complete */
        gmmbuf = testgmm_build_security_mode_complete(test_ue[i], nasbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive InitialContextSetupRequest +
         * Registration accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_InitialContextSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send UERadioCapabilityInfoIndication */
        sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send InitialContextSetupResponse */
        sendbuf = testngap_build_initial_context_setup_response(test_ue[i], false);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send Registration complete */
        gmmbuf = testgmm_build_registration_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Configuration update command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);

        sess->ul_nas_transport_param.request_type =
            OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
        sess->ul_nas_transport_param.dnn = 1;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 1;
        sess->pdu_session_establishment_param.epco = 1;

        gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceSetupRequest +
         * DL NAS transport +
         * PDU session establishment accept */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceSetupResponse */
        sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send PDU session release request */
        sess = test_sess_find_by_psi(test_ue[i], 5);
        ogs_assert(sess);

        /* Send PDU Session release request */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_request(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive PDUSessionResourceReleaseCommand +
         * DL NAS transport +
         * PDU session release command */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_PDUSessionResourceRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send PDUSessionResourceReleaseResponse */
        sendbuf = testngap_build_pdu_session_resource_release_response(sess);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Send UplinkNASTransport +
         * UL NAS trasnport +
         * PDU session resource release complete */
        sess->ul_nas_transport_param.request_type = 0;
        sess->ul_nas_transport_param.dnn = 0;
        sess->ul_nas_transport_param.s_nssai = 0;

        sess->pdu_session_establishment_param.ssc_mode = 0;
        sess->pdu_session_establishment_param.epco = 0;

        gsmbuf = testgsm_build_pdu_session_release_complete(sess);
        ABTS_PTR_NOTNULL(tc, gsmbuf);
        gmmbuf = testgmm_build_ul_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /* Send De-registration request */
        gmmbuf = testgmm_build_de_registration_request(test_ue[i], 1, true, true);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive UEContextReleaseCommand */
        recvbuf = testgnb_ngap_read(ngap1);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap1, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    /* Register to Target AMF */

    /* Make the same UE from scratch and give it the 5G GUTI from Old AMF */

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        const char *scheme_output_target[] = {
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

        test_ue_target[i] = test_ue_add_by_suci(
                &mobile_identity_suci, scheme_output_target[i]);
        ogs_assert(test_ue_target[i]);

        test_ue_target[i]->nr_cgi.cell_id = 0x40000;

        test_ue_target[i]->nas.registration.tsc = 0;
        test_ue_target[i]->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        test_ue_target[i]->nas.registration.follow_on_request = 1;
        test_ue_target[i]->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

        test_ue_target[i]->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
        test_ue_target[i]->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        if (i > 0)
            test_ue_target[i]->ran_ue_ngap_id = test_ue_target[i-1]->ran_ue_ngap_id;
        else
            test_ue_target[i]->ran_ue_ngap_id = 0;

        /* pdu_id == 6 */

        /* Send PDU session establishment request */
        sess = test_sess_add_by_dnn_and_psi(test_ue_target[i], "internet", 6);
        ogs_assert(sess);

        /********** Insert Subscriber in Database */
        doc = test_db_new_simple(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, doc);
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue_target[i], doc));

        /* Set the 5G GUTI to Old AMF's 5G GUTI */
        test_ue_target[i]->nas_5gs_guti = test_ue[i]->nas_5gs_guti;

        /* Send Registration request */
        test_ue_target[i]->registration_request_param.guti = 1;
        gmmbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, gmmbuf);

        test_ue_target[i]->registration_request_param.gmm_capability = 1;
        test_ue_target[i]->registration_request_param.s1_ue_network_capability = 1;
        test_ue_target[i]->registration_request_param.requested_nssai = 1;
        test_ue_target[i]->registration_request_param.last_visited_registered_tai = 1;
        test_ue_target[i]->registration_request_param.ue_usage_setting = 1;
        nasbuf = testgmm_build_registration_request(test_ue_target[i], NULL, false, false);
        ABTS_PTR_NOTNULL(tc, nasbuf);

        sendbuf = testngap_build_initial_ue_message(test_ue_target[i], gmmbuf,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
        /* Receive Identity request */
        recvbuf_target = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf_target);
        testngap_recv(test_ue_target[i], recvbuf_target);

        /* Send Identity response */
        gmmbuf = testgmm_build_identity_response(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

        /* Receive Authentication request */
        recvbuf = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue_target[i], recvbuf);

        /* Send Authentication failure - SYNCH failure */
        gmmbuf = testgmm_build_authentication_failure(
                test_ue_target[i], OGS_5GMM_CAUSE_SYNCH_FAILURE, 0x11223344);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Authentication request */
        recvbuf = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue_target[i], recvbuf);

        /* Send Authentication failure - MAC failure */
        gmmbuf = testgmm_build_authentication_failure(
                test_ue_target[i], OGS_5GMM_CAUSE_MAC_FAILURE, 0);
        ABTS_PTR_NOTNULL(tc, gmmbuf);
        sendbuf = testngap_build_uplink_nas_transport(test_ue_target[i], gmmbuf);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive Authentication reject */
        recvbuf = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue_target[i], recvbuf);

        /* Receive UEContextReleaseCommand */
        recvbuf = testgnb_ngap_read(ngap2);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue_target[i], recvbuf);
        ABTS_INT_EQUAL(tc,
                NGAP_ProcedureCode_id_UEContextRelease,
                test_ue_target[i]->ngap_procedure_code);

        /* Send UEContextReleaseComplete */
        sendbuf = testngap_build_ue_context_release_complete(test_ue_target[i]);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap2, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    ogs_msleep(300);

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue_target[i]));
    }

    for (i = 0; i < NUM_OF_TEST_UE_1; i++) {
        /********** Remove Subscriber in Database */
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue[i]));
    }

    /* Clear Test UE Context */
    test_ue_remove_all();

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu2);
    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu1);

    /* gNB disonncect from Target AMF */
    testgnb_ngap_close(ngap2);
    /* gNB disonncect from Old AMF */
    testgnb_ngap_close(ngap1);
}

abts_suite *test_ue_context_transfer(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test23_func, NULL);
    abts_run_test(suite, test3_func, NULL);
    abts_run_test(suite, test4_func, NULL);

    return suite;
}
