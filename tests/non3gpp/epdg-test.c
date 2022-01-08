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
#include "test-fd-path.h"
#include "gtp-path.h"

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_socknode_t *epdg_c;
    ogs_socknode_t *epdg_u;
    ogs_gtp_node_t *gnode;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    uint8_t *rx_sid = NULL;

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
    mobile_identity_suci.protection_scheme_id = OGS_NAS_5GS_NULL_SCHEME;
    mobile_identity_suci.home_network_pki_value = 0;
    mobile_identity_suci.scheme_output[0] = 0x10;
    mobile_identity_suci.scheme_output[1] = 0x32;
    mobile_identity_suci.scheme_output[2] = 0x54;
    mobile_identity_suci.scheme_output[3] = 0x86;
    mobile_identity_suci.scheme_output[4] = 0x91;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, 13);
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x1079baf;
    test_ue->nas.ksi = 0;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    /* eNB connects to MME */
    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* ePDG GTPv2-C */
    epdg_c = test_epdg_server(OGS_GTPV2_C_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, epdg_c);

    gnode = ogs_list_first(&test_self()->gtpc_list);
    ogs_assert(gnode);

    ABTS_PTR_NOTNULL(tc, epdg_c->sock);
    rv = ogs_gtp_connect(epdg_c->sock, NULL, gnode);
    ogs_assert(rv == OGS_OK);

    /* ePDG GTP-U */
    epdg_u = test_epdg_server(OGS_GTPV1_U_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, epdg_u);

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
    doc = test_db_new_non3gpp(test_ue);
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue, doc));

    /* Setup WLAN Session */
    sess = test_sess_add_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_WLAN);
    ogs_assert(sess);

    OGS_SETUP_GTP_NODE(sess, ogs_list_first(&test_self()->gtpc_list));

    /* Send SWx MAR */
    test_swx_send(sess, false, test_s2b_send_create_session_request);

    /* Receive S2b Create Session Response */
    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_sess_ebi(sess, 5);
    ogs_assert(bearer);

    rv = test_gtpu_send_ping(epdg_u, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_epdg_read(epdg_u);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send S2b Delete Session Request */
    rv = test_s2b_send_delete_session_request(sess);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S2b Delete Session Response */
    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* eNB disconnect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disconnect from SGW */
    test_gtpu_close(gtpu);

    /* ePDG diconnect */
    test_epdg_close(epdg_c);
    test_epdg_close(epdg_u);

    test_ue_remove(test_ue);
}

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_socknode_t *epdg_c;
    ogs_socknode_t *epdg_u;
    ogs_gtp_node_t *gnode;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    uint8_t *rx_sid = NULL;

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
    mobile_identity_suci.protection_scheme_id = OGS_NAS_5GS_NULL_SCHEME;
    mobile_identity_suci.home_network_pki_value = 0;
    mobile_identity_suci.scheme_output[0] = 0x10;
    mobile_identity_suci.scheme_output[1] = 0x32;
    mobile_identity_suci.scheme_output[2] = 0x54;
    mobile_identity_suci.scheme_output[3] = 0x86;
    mobile_identity_suci.scheme_output[4] = 0x91;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, 13);
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x1079baf;
    test_ue->nas.ksi = 0;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    /* eNB connects to MME */
    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* ePDG GTPv2-C */
    epdg_c = test_epdg_server(OGS_GTPV2_C_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, epdg_c);

    gnode = ogs_list_first(&test_self()->gtpc_list);
    ogs_assert(gnode);

    ABTS_PTR_NOTNULL(tc, epdg_c->sock);
    rv = ogs_gtp_connect(epdg_c->sock, NULL, gnode);
    ogs_assert(rv == OGS_OK);

    /* ePDG GTP-U */
    epdg_u = test_epdg_server(OGS_GTPV1_U_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, epdg_u);

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
    doc = test_db_new_session(test_ue);
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue, doc));

    /* Setup WLAN Session */
    sess = test_sess_add_by_apn(test_ue, "ims", OGS_GTP_RAT_TYPE_WLAN);
    ogs_assert(sess);

    OGS_SETUP_GTP_NODE(sess, ogs_list_first(&test_self()->gtpc_list));

    /* Send SWx MAR */
    test_swx_send(sess, false, test_s2b_send_create_session_request);

    /* Receive S2b Create Session Response */
    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    /* Receive S2b Create Bearer Request */
    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_sess_ebi(sess, 6);
    ogs_assert(bearer);

    rv = test_gtpu_send_ping(epdg_u, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_epdg_read(epdg_u);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send S2b Delete Session Request */
    rv = test_s2b_send_delete_session_request(sess);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S2b Delete Session Response */
    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* eNB disconnect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disconnect from SGW */
    test_gtpu_close(gtpu);

    /* ePDG diconnect */
    test_epdg_close(epdg_c);
    test_epdg_close(epdg_u);

    test_ue_remove(test_ue);
}

static void test3_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_socknode_t *epdg_c;
    ogs_socknode_t *epdg_u;
    ogs_gtp_node_t *gnode;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    uint8_t *rx_sid = NULL;

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
    mobile_identity_suci.protection_scheme_id = OGS_NAS_5GS_NULL_SCHEME;
    mobile_identity_suci.home_network_pki_value = 0;
    mobile_identity_suci.scheme_output[0] = 0x10;
    mobile_identity_suci.scheme_output[1] = 0x32;
    mobile_identity_suci.scheme_output[2] = 0x54;
    mobile_identity_suci.scheme_output[3] = 0x86;
    mobile_identity_suci.scheme_output[4] = 0x91;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, 13);
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x1079baf;
    test_ue->nas.ksi = 0;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    test_ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    /* eNB connects to MME */
    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* ePDG GTPv2-C */
    epdg_c = test_epdg_server(OGS_GTPV2_C_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, epdg_c);

    gnode = ogs_list_first(&test_self()->gtpc_list);
    ogs_assert(gnode);

    ABTS_PTR_NOTNULL(tc, epdg_c->sock);
    rv = ogs_gtp_connect(epdg_c->sock, NULL, gnode);
    ogs_assert(rv == OGS_OK);

    /* ePDG GTP-U */
    epdg_u = test_epdg_server(OGS_GTPV1_U_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, epdg_u);

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
    doc = test_db_new_non3gpp(test_ue);
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue, doc));

    /* Setup WLAN Session */
    sess = test_sess_add_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_WLAN);
    ogs_assert(sess);

    OGS_SETUP_GTP_NODE(sess, ogs_list_first(&test_self()->gtpc_list));

    /* Send SWx MAR */
    test_swx_send(sess, false, test_s2b_send_create_session_request);

    /* Receive S2b Create Session Response */
    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    bearer = test_bearer_find_by_sess_ebi(sess, 5);
    ogs_assert(bearer);

    /* Send GTP-U ICMP Packet */
    rv = test_gtpu_send_ping(epdg_u, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_epdg_read(epdg_u);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Setup EUTRAN Session */
    sess = test_sess_add_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_EUTRAN);
    ogs_assert(sess);

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_HANDOVER;
    esmbuf = testesm_build_pdn_connectivity_request(sess, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue->attach_request_param,
            0, sizeof(test_ue->attach_request_param));
    test_ue->attach_request_param.drx_parameter = 1;
    test_ue->attach_request_param.tmsi_status = 1;
    test_ue->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue->attach_request_param.additional_update_type = 1;
    test_ue->attach_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_attach_request(test_ue, esmbuf, false, false);
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

    /* Send GTP-U ICMP Packet */
    rv = test_gtpu_send_ping(gtpu, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive S2b Delete Session Response */
    sess = test_sess_find_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_WLAN);
    ogs_assert(sess);

    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    /* Send PDN Connectivity Request */
    sess = test_sess_add_by_apn(test_ue, "internet", OGS_GTP_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 5;

    sess->pdn_connectivity_param.apn = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esmbuf = testesm_build_pdn_connectivity_request(sess, true);
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
            S1AP_ProcedureCode_id_E_RABSetup,
            test_ue->s1ap_procedure_code);

    /* Send E-RABSetupResponse */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 6);
    ogs_assert(bearer);
    sendbuf = test_s1ap_build_e_rab_setup_response(bearer);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Activate default EPS bearer context accept */
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, true);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Setup WLAN Session */
    sess = test_sess_add_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_WLAN);
    ogs_assert(sess);

    OGS_SETUP_GTP_NODE(sess, ogs_list_first(&test_self()->gtpc_list));

    /* Send SWx MAR */
    test_swx_send(sess, true, test_s2b_send_create_session_request);

    /* Receive S2b Create Session Response */
    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_sess_ebi(sess, 5);
    ogs_assert(bearer);

    rv = test_gtpu_send_ping(epdg_u, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_epdg_read(epdg_u);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive E-RABReleaseCommand +
     * Deactivate EPS bearer context request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);
    ABTS_INT_EQUAL(tc,
            S1AP_ProcedureCode_id_E_RABRelease,
            test_ue->s1ap_procedure_code);

    /* Send E-RABReleaseResponse */
    sess = test_sess_find_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_EUTRAN);
    ogs_assert(sess);

    bearer = test_bearer_find_by_sess_ebi(sess, 5);
    ogs_assert(bearer);
    sendbuf = test_s1ap_build_e_rab_release_response(bearer);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Deactivate EPS bearer context accept */
    esmbuf = testesm_build_deactivate_eps_bearer_context_accept(bearer);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Remove EUTRAN Session */
    test_sess_remove(sess);

    ogs_msleep(300);

    /* Setup EUTRAN Session */
    sess = test_sess_add_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_EUTRAN);
    ogs_assert(sess);

    sess->pdn_connectivity_param.apn = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type =
        OGS_NAS_EPS_REQUEST_TYPE_HANDOVER;
    esmbuf = testesm_build_pdn_connectivity_request(sess, true);
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
            S1AP_ProcedureCode_id_E_RABSetup,
            test_ue->s1ap_procedure_code);

    /* Send E-RABSetupResponse */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 7);
    ogs_assert(bearer);
    sendbuf = test_s1ap_build_e_rab_setup_response(bearer);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Activate default EPS bearer context accept */
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, true);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send GTP-U ICMP Packet */
    rv = test_gtpu_send_ping(gtpu, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive S2b Delete Session Response */
    sess = test_sess_find_by_apn(test_ue, "wlan", OGS_GTP_RAT_TYPE_WLAN);
    ogs_assert(sess);

    recvbuf = test_epdg_read(epdg_c);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    test_s2b_recv(sess, recvbuf);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* eNB disconnect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disconnect from SGW */
    test_gtpu_close(gtpu);

    /* ePDG diconnect */
    test_epdg_close(epdg_c);
    test_epdg_close(epdg_u);

    test_ue_remove(test_ue);
}

abts_suite *test_epdg(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);

    return suite;
}
