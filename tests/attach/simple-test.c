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

    /* Send GTP-U Router Solicitation */
    rv = test_gtpu_send_slacc_rs(gtpu, bearer);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U Router Solicitation */
    recvbuf = test_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testgtpu_recv(test_ue, recvbuf);

/*
 * Discussion #1776 was raised,
 * but we decided not to allow unspecified addresses
 * because Open5GS has already sent interface identifiers
 * in the registgration/attach process.
 *
 *
 * RFC4861
 * 4.  Message Formats
 * 4.1.  Router Solicitation Message Format
 * IP Fields:
 *    Source Address
 *                  An IP address assigned to the sending interface, or
 *                  the unspecified address if no address is assigned
 *                  to the sending interface.
 *
 * 6.1.  Message Validation
 * 6.1.1.  Validation of Router Solicitation Messages
 *  Hosts MUST silently discard any received Router Solicitation
 *  Messages.
 *
 *  A router MUST silently discard any received Router Solicitation
 *  messages that do not satisfy all of the following validity checks:
 *
 *  ..
 *  ..
 *
 *  - If the IP source address is the unspecified address, there is no
 *    source link-layer address option in the message.
 */
#if 0
    /* Send GTP-U Router Solicitation with an unspecified source address */
    rv = test_gtpu_send_slacc_rs_with_unspecified_source_address(gtpu, bearer);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U Router Solicitation */
    recvbuf = test_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testgtpu_recv(test_ue, recvbuf);
#endif

    /* Send GTP-U ICMP Packet */
    rv = test_gtpu_send_ping(gtpu, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

#if !defined(__FreeBSD__)
    /* Send GTP-U ICMP Packet */
    rv = test_gtpu_send_ping(gtpu, bearer, TEST_PING_IPV6);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);
#endif

    /* Send Detach Request */
    emmbuf = testemm_build_detach_request(test_ue, 1, true, false);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, true);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive OLD UE Context Release Command */
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id;

    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send OLD UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test_ue->enb_ue_s1ap_id = enb_ue_s1ap_id;

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

abts_suite *test_simple(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);

    return suite;
}
