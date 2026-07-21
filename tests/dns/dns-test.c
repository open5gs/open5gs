/*
 * Copyright (C) 2026 by Evgenii Grigorev <tothe8c@gmail.com>
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
#include "dns-server.h"

/* Serving PLMN 999/70, TAC 1 (see configs/dns.yaml.in) */
#define TAI_FQDN "tac-lb01.tac-hb00.tac.epc.mnc070.mcc999.3gppnetwork.org"
#define APN_FQDN "internet.apn.epc.mnc070.mcc999.3gppnetwork.org"

#define SRV_NAME "_gtp._udp.sgw.epc.mnc070.mcc999.3gppnetwork.org"
#define SGW_NAME "sgw.epc.mnc070.mcc999.3gppnetwork.org"
#define PGW_NAME "topon.s5.pgw.epc.mnc070.mcc999.3gppnetwork.org"

/*
 * Records exercising both S-NAPTR chains:
 *   SGW: NAPTR (flag "s") -> SRV -> A = 127.0.0.3
 *   PGW: NAPTR (flag "a") -> A = 127.0.0.4
 * The addresses match the SGW-C/SMF of configs/dns.yaml.in, so a
 * successful attach proves the Create Session Request followed the
 * DNS-provided gateways.
 */
static const test_dns_record_t good_records[] = {
    { .qname = TAI_FQDN, .qtype = TEST_DNS_T_NAPTR,
      .order = 10, .preference = 10, .flags = "s",
      .service = "x-3gpp-sgw:x-s5-gtp", .replacement = SRV_NAME },
    { .qname = SRV_NAME, .qtype = TEST_DNS_T_SRV,
      .priority = 10, .weight = 0, .port = 2123, .target = SGW_NAME },
    { .qname = SGW_NAME, .qtype = TEST_DNS_T_A, .ipv4 = "127.0.0.3" },
    { .qname = APN_FQDN, .qtype = TEST_DNS_T_NAPTR,
      .order = 10, .preference = 10, .flags = "a",
      .service = "x-3gpp-pgw:x-s5-gtp:x-s8-gtp", .replacement = PGW_NAME },
    { .qname = PGW_NAME, .qtype = TEST_DNS_T_A, .ipv4 = "127.0.0.4" },
};

/*
 * Regression records for the SRV "no usable target" case: the SGW
 * NAPTR (flag "s") points at an SRV whose only record has target "."
 * (service not available, RFC 2782). The MME must skip the candidate
 * and fall back - a broken MME re-reads the same cache entry forever
 * and the whole event loop hangs, so this test failing by TIMEOUT is
 * the regression signature. No PGW records: that leg falls back too.
 */
static const test_dns_record_t srv_dot_records[] = {
    { .qname = TAI_FQDN, .qtype = TEST_DNS_T_NAPTR,
      .order = 10, .preference = 10, .flags = "s",
      .service = "x-3gpp-sgw:x-s5-gtp", .replacement = SRV_NAME },
    { .qname = SRV_NAME, .qtype = TEST_DNS_T_SRV,
      .priority = 10, .weight = 0, .port = 0, .target = "." },
};

static void attach_and_detach(abts_case *tc, const char *scheme_output)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t enb_ue_s1ap_id;

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

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, scheme_output);
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x1079baf0;
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
    ABTS_PTR_NOTNULL(tc, sendbuf);
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

    /* Send ESM Information Response
     * (the MME then resolves the gateways via DNS and sends the
     *  Create Session Request) */
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

/* Attach with a fully working S-NAPTR chain: SGW via NAPTR->SRV->A,
 * PGW via NAPTR->A. */
static void test1_dns_selection(abts_case *tc, void *data)
{
    test_dns_server_set_records(
            good_records, OGS_ARRAY_SIZE(good_records));
    test_dns_server_reset_counters();

    attach_and_detach(tc, "3746000060");

    /* The MME must have queried both FQDNs */
    ABTS_TRUE(tc, test_dns_server_num_queries("tac-lb01.tac-hb00") >= 1);
    ABTS_TRUE(tc, test_dns_server_num_queries("internet.apn") >= 1);
}

/* DNS answers with NOERROR/0 records: the MME must fall back to the
 * static sgwc/smf configuration and the attach must still succeed. */
static void test2_empty_fallback(abts_case *tc, void *data)
{
    /* Let the cache entries of the previous case expire
     * (cache_ttl is 1s in dns.yaml.in) */
    ogs_msleep(1200);

    test_dns_server_set_mode(TEST_DNS_MODE_EMPTY);
    test_dns_server_reset_counters();

    attach_and_detach(tc, "3746000061");

    ABTS_TRUE(tc, test_dns_server_num_queries(NULL) >= 1);
}

/* DNS server drops all queries: the guard timer (1.5s in dns.yaml.in)
 * must fire and the attach must still succeed via static fallback. */
static void test3_timeout_fallback(abts_case *tc, void *data)
{
    /* Let the (negative) cache entries of the previous case expire */
    ogs_msleep(1200);

    test_dns_server_set_mode(TEST_DNS_MODE_DROP);
    test_dns_server_reset_counters();

    attach_and_detach(tc, "3746000062");

    ABTS_TRUE(tc, test_dns_server_num_queries(NULL) >= 1);
}

/* SRV answer whose only record has target ".": the MME must skip the
 * candidate and fall back to the static configuration. A regressed MME
 * hangs its event loop re-reading the same SRV cache entry, so this
 * test failing by TIMEOUT (no S1AP progress) is the regression
 * signature. */
static void test4_srv_dot_fallback(abts_case *tc, void *data)
{
    /* Let the (negative) cache entries of the previous case expire */
    ogs_msleep(1200);

    test_dns_server_set_records(
            srv_dot_records, OGS_ARRAY_SIZE(srv_dot_records));
    test_dns_server_reset_counters();

    attach_and_detach(tc, "3746000063");

    /* The chain must have been followed up to the unusable SRV */
    ABTS_TRUE(tc, test_dns_server_num_queries("tac-lb01.tac-hb00") >= 1);
    ABTS_TRUE(tc, test_dns_server_num_queries("_gtp._udp") >= 1);
}

abts_suite *test_dns_select(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_dns_selection, NULL);
    abts_run_test(suite, test2_empty_fallback, NULL);
    abts_run_test(suite, test3_timeout_fallback, NULL);
    abts_run_test(suite, test4_srv_dot_fallback, NULL);

    return suite;
}
