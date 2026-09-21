/*
 * Copyright (C) 2026 by LetMeConnect
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

/*
 * ME Identity Check over S13 against the Open5GS EIR (open5gs-eird).
 *
 * The MME runs with eir.enabled (configs/s13.yaml) and the EIR serves S13
 * next to N5g-eir out of the same eir collection. Each case provisions
 * records for the test UE IMEISV, runs an EPS attach, and checks what
 * follows Security Mode Complete: ESM Information Request when the
 * equipment is admitted, Attach Reject #6 Illegal ME when it is not.
 */

#include "test-app.h"

/* test_ue_set_mobile_identity_imsisv(): IMEI 86650704004053 + SVN 01 */
#define S13_TEST_PEI "imeisv-8665070400405301"
#define S13_TEST_OTHER_SUPI "imsi-999700000000099"

typedef struct s13_case_s {
    const char *name;
    const char *generic_status;     /* eir record for the PEI only */
    const char *specific_status;    /* eir record for the PEI and a SUPI */
    bool specific_other_supi;       /* ...of another subscriber */
    ogs_nas_emm_cause_t reject_cause;   /* 0: attach accepted */
} s13_case_t;

static const s13_case_t cases[] = {
    { "whitelisted equipment attaches", "WHITELISTED", NULL, false, 0 },
    { "greylisted equipment attaches", "GREYLISTED", NULL, false, 0 },
    { "blacklisted equipment is rejected", "BLACKLISTED", NULL, false,
        OGS_NAS_EMM_CAUSE_ILLEGAL_ME },
    { "unknown equipment attaches with unknown_action allow",
        NULL, NULL, false, 0 },
    { "subscriber blacklist overrides generic whitelist",
        "WHITELISTED", "BLACKLISTED", false, OGS_NAS_EMM_CAUSE_ILLEGAL_ME },
    { "subscriber whitelist overrides generic blacklist",
        "BLACKLISTED", "WHITELISTED", false, 0 },
    { "record of another subscriber does not apply",
        "WHITELISTED", "BLACKLISTED", true, 0 },
};

typedef struct fixture_s {
    bson_oid_t ids[2];
    unsigned int count;
} fixture_t;

static void fixture_insert(abts_case *tc, fixture_t *fixture,
        const char *pei, const char *supi, const char *status)
{
    bson_error_t error;
    bson_t *document = NULL;
    unsigned int index = fixture->count++;

    ogs_assert(index < OGS_ARRAY_SIZE(fixture->ids));
    bson_oid_init(&fixture->ids[index], NULL);

    if (supi)
        document = BCON_NEW("_id", BCON_OID(&fixture->ids[index]),
                "pei", BCON_UTF8(pei), "supi", BCON_UTF8(supi),
                "status", BCON_UTF8(status));
    else
        document = BCON_NEW("_id", BCON_OID(&fixture->ids[index]),
                "pei", BCON_UTF8(pei), "status", BCON_UTF8(status));
    ogs_assert(document);

    if (!mongoc_collection_insert_one(ogs_mongoc()->collection.eir,
                document, NULL, NULL, &error))
        ABTS_FAIL(tc, error.message);
    bson_destroy(document);
}

static void fixture_remove(abts_case *tc, fixture_t *fixture)
{
    bson_error_t error;

    /* Remove only documents created by this case */
    while (fixture->count) {
        bson_t *query = BCON_NEW("_id",
                BCON_OID(&fixture->ids[--fixture->count]));

        if (!mongoc_collection_delete_one(ogs_mongoc()->collection.eir,
                    query, NULL, NULL, &error))
            ABTS_FAIL(tc, error.message);
        bson_destroy(query);
    }
}

static void s13_case(abts_case *tc, void *data)
{
    const s13_case_t *test = data;
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
    fixture_t fixture;
    char *supi = NULL;

    ogs_info("[S13] %s", test->name);
    memset(&fixture, 0, sizeof(fixture));

    /*
     * Every case uses the same IMEISV and mme.eir.max_age is 1 second in
     * s13.yaml: let the previous verdict expire from the MME cache so that
     * the ME-Identity-Check-Request really reaches the EIR.
     */
    ogs_msleep(1200);

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

    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, test_ue->imsi);
    ogs_assert(supi);

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

    /********** Insert equipment records in the eir collection */
    if (test->generic_status)
        fixture_insert(tc, &fixture, S13_TEST_PEI, NULL, test->generic_status);
    if (test->specific_status)
        fixture_insert(tc, &fixture, S13_TEST_PEI,
                test->specific_other_supi ? S13_TEST_OTHER_SUPI : supi,
                test->specific_status);

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

    /* Send Security mode complete, with the IMEISV the MME checks */
    test_ue->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    if (test->reject_cause) {
        /* Receive Attach Reject */
        test_ue->emm_message_type = 0;
        test_ue->attach_reject_cause = 0;
        recvbuf = testenb_s1ap_read(s1ap);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        tests1ap_recv(test_ue, recvbuf);
        ABTS_INT_EQUAL(tc, OGS_NAS_EPS_ATTACH_REJECT,
                test_ue->emm_message_type);
        ABTS_INT_EQUAL(tc, test->reject_cause, test_ue->attach_reject_cause);

        /* Receive UE Context Release Command */
        recvbuf = testenb_s1ap_read(s1ap);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        tests1ap_recv(test_ue, recvbuf);
        ABTS_INT_EQUAL(tc, S1AP_ProcedureCode_id_UEContextRelease,
                test_ue->s1ap_procedure_code);

        /* Send UE Context Release Complete */
        sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testenb_s1ap_send(s1ap, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        goto cleanup;
    }

    /* Receive ESM Information Request */
    test_ue->esm_message_type = 0;
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);
    ABTS_INT_EQUAL(tc, OGS_NAS_EPS_ESM_INFORMATION_REQUEST,
            test_ue->esm_message_type);

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
    ABTS_INT_EQUAL(tc, OGS_NAS_EPS_ATTACH_ACCEPT, test_ue->emm_message_type);

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

cleanup:
    ogs_msleep(300);

    /********** Remove equipment records and Subscriber in Database */
    fixture_remove(tc, &fixture);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    test_gtpu_close(gtpu);

    ogs_free(supi);
    test_ue_remove(test_ue);
}

abts_suite *test_s13(abts_suite *suite)
{
    int i;

    suite = ADD_SUITE(suite)

    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++)
        abts_run_test(suite, s13_case, (void *)&cases[i]);

    return suite;
}
