/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
#include "gtpu-check.h"
#include "multi-pdn.h"

static void test1_func(abts_case *tc, void *data)
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

    ogs_socknode_t *target;
    ogs_sockaddr_t *saved_addr, *target_addr;
    uint32_t previous_ip;
    ogs_socknode_t *moving, *receiving;
    int direction;
    bool fallback = data == (void *)1 || data == (void *)3;
    bool multi = data == (void *)2 || data == (void *)3;
    test_bearer_t *extra = NULL;
    uint32_t extra_ip = 0;

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
    test_ue->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
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
    if (multi) doc = s10_two_apns(doc);
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue, doc));

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(
            sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue->attach_request_param,
            0, sizeof(test_ue->attach_request_param));
    test_ue->attach_request_param.drx_parameter = 1;
    test_ue->attach_request_param.ms_network_capability = 1;
    test_ue->attach_request_param.tmsi_status = 1;
    test_ue->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue, esmbuf, true, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue->initial_ue_param, 0, sizeof(test_ue->initial_ue_param));
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

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
    sess->esm_information_param.epco = 1;
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

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue);
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

    if (multi) {
        extra = s10_add_pdn(tc, test_ue, s1ap);
        extra_ip = extra->sess->ue_ip.addr;
    }
    previous_ip = sess->ue_ip.addr;

    /* A different SCTP association reaches a second, independent MME. */
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_getaddrinfo(&target_addr, AF_INET, "127.0.0.12", 36412, 0));
    saved_addr = test_self()->s1ap_addr;
    test_self()->s1ap_addr = target_addr;
    target = tests1ap_client(AF_INET);
    test_self()->s1ap_addr = saved_addr;
    ogs_freeaddrinfo(target_addr);
    ABTS_PTR_NOTNULL(tc, target);
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x54f65);
    ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(target, sendbuf));
    recvbuf = testenb_s1ap_read(target);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    for (direction = 0; direction < (data == (void *)4 ? 1 : 2); direction++) {
        moving = direction ? target : s1ap;
        receiving = direction ? s1ap : target;
        /* Release access on the source MME, retaining the EPS sessions. */
        sendbuf = test_s1ap_build_ue_context_release_request(test_ue,
                S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_user_inactivity);
        ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(moving, sendbuf));
        recvbuf = testenb_s1ap_read(moving);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        tests1ap_recv(test_ue, recvbuf);
        sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
        ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(moving, sendbuf));

        memset(&test_ue->tau_request_param, 0, sizeof(test_ue->tau_request_param));
        test_ue->tau_request_param.ue_network_capability = 1;
        emmbuf = testemm_build_tau_request(test_ue, true,
                OGS_NAS_EPS_UPDATE_TYPE_TA_UPDATING, !fallback, false);
        sendbuf = test_s1ap_build_initial_ue_message(test_ue, emmbuf,
                S1AP_RRC_Establishment_Cause_mo_Signalling, false);
        ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
        recvbuf = testenb_s1ap_read(receiving);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        tests1ap_recv(test_ue, recvbuf);
        if (data == (void *)4) {
            ABTS_INT_EQUAL(tc, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT, test_ue->emm_message_type);
            recvbuf = testenb_s1ap_read(receiving);
            tests1ap_recv(test_ue, recvbuf);
            sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
            ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
            receiving = moving;
            emmbuf = testemm_build_tau_request(test_ue, true,
                    OGS_NAS_EPS_UPDATE_TYPE_TA_UPDATING, true, false);
            sendbuf = test_s1ap_build_initial_ue_message(test_ue, emmbuf,
                    S1AP_RRC_Establishment_Cause_mo_Signalling, false);
            ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
            recvbuf = testenb_s1ap_read(receiving);
            tests1ap_recv(test_ue, recvbuf);
            ABTS_INT_EQUAL(tc, OGS_NAS_EPS_AUTHENTICATION_REQUEST, test_ue->emm_message_type);
            emmbuf = testemm_build_authentication_response(test_ue);
            sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
            recvbuf = testenb_s1ap_read(receiving);
            tests1ap_recv(test_ue, recvbuf);
            emmbuf = testemm_build_security_mode_complete(test_ue);
            sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
            recvbuf = testenb_s1ap_read(receiving);
            tests1ap_recv(test_ue, recvbuf);
        }
        if (fallback) {
            /* Failed old-context integrity triggers Identity and fresh AKA;
             * security completion retries Context Request with MS Validated. */
            emmbuf = testemm_build_identity_response(test_ue);
            sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
            recvbuf = testenb_s1ap_read(receiving);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            tests1ap_recv(test_ue, recvbuf);
            emmbuf = testemm_build_authentication_response(test_ue);
            sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
            recvbuf = testenb_s1ap_read(receiving);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            tests1ap_recv(test_ue, recvbuf);
            test_ue->mobile_identity_imeisv_presence = true;
            emmbuf = testemm_build_security_mode_complete(test_ue);
            sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
            ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
            recvbuf = testenb_s1ap_read(receiving);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            tests1ap_recv(test_ue, recvbuf);
        }
        ABTS_INT_EQUAL(tc, S1AP_ProcedureCode_id_InitialContextSetup,
                test_ue->s1ap_procedure_code);
        if (test_ue->s1ap_procedure_code != S1AP_ProcedureCode_id_InitialContextSetup) {
            test_db_remove_ue(test_ue);
            testenb_s1ap_close(target);
            testenb_s1ap_close(s1ap);
            test_gtpu_close(gtpu);
            test_ue_remove(test_ue);
            return;
        }
        ABTS_INT_EQUAL(tc, direction || data == (void *)4 ? 1 : 2, test_ue->nas_eps_guti.mme_code);
        ABTS_INT_EQUAL(tc, previous_ip, sess->ue_ip.addr);
        sendbuf = test_s1ap_build_initial_context_setup_response(test_ue);
        ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));
        emmbuf = testemm_build_tau_complete(test_ue);
        sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(receiving, sendbuf));

        /* Verify the returned IPv4 ICMP echo, including its UE destination.
         * testgtpu_recv() is the IPv6 router-advertisement helper. */
        rv = test_gtpu_send_ping(gtpu, bearer, TEST_PING_IPV4);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
        recvbuf = s10_read_echo(tc, gtpu, bearer);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        {
            ogs_gtp2_header_desc_t header = {0};
            int hlen = ogs_gtpu_parse_header(&header, recvbuf);
            uint32_t destination;
            uint8_t *ip;
            ABTS_TRUE(tc, hlen >= OGS_GTPV1U_HEADER_LEN);
            ABTS_INT_EQUAL(tc, OGS_GTPU_MSGTYPE_GPDU, header.type);
            ABTS_TRUE(tc, hlen > 0 && recvbuf->len >= hlen + 28);
            if (hlen > 0 && recvbuf->len >= hlen + 28) {
                ip = recvbuf->data + hlen;
                ABTS_INT_EQUAL(tc, 4, ip[0] >> 4);
                ABTS_INT_EQUAL(tc, 1, ip[9]); /* ICMP */
                memcpy(&destination, ip + 16, sizeof(destination));
                ABTS_INT_EQUAL(tc, previous_ip, destination);
                if (recvbuf->len >= hlen + (ip[0] & 15) * 4 + 8)
                    ABTS_INT_EQUAL(tc, 0, ip[(ip[0] & 15) * 4]); /* Echo Reply */
                else ABTS_TRUE(tc, false);
            }
        }
        ogs_pkbuf_free(recvbuf);

        if (extra) {
            ABTS_INT_EQUAL(tc, extra_ip, extra->sess->ue_ip.addr);
            ABTS_INT_EQUAL(tc, OGS_OK, test_gtpu_send_ping(gtpu, extra, TEST_PING_IPV4));
            recvbuf = s10_read_echo(tc, gtpu, extra);
            ABTS_PTR_NOTNULL(tc, recvbuf);
            ogs_pkbuf_free(recvbuf);
        }
    }

    emmbuf = testemm_build_detach_request(test_ue, false, true, true);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(s1ap, sendbuf));
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(s1ap, sendbuf));
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));
    testenb_s1ap_close(target);
    testenb_s1ap_close(s1ap);
    test_gtpu_close(gtpu);
    test_ue_remove(test_ue);
}

abts_suite *test_s10_mobility(abts_suite *suite)
{
    suite = ADD_SUITE(suite);
#ifdef TEST_S10_SGW_FAILURE
    abts_run_test(suite, test1_func, (void *)4);
#else
    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test1_func, (void *)1);
    abts_run_test(suite, test1_func, (void *)2);
    abts_run_test(suite, test1_func, (void *)3);
#endif
    return suite;
}
