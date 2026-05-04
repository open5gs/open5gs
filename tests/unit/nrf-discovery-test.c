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

#include "ogs-sbi.h"
#include "core/abts.h"

/*
 * Helper: allocate a minimal nf_instance on the heap (no pool required).
 * Fields are zero-initialised so ogs_list_t members are empty.
 */
static ogs_sbi_nf_instance_t *test_nf_instance_new(OpenAPI_nf_type_e nf_type)
{
    ogs_sbi_nf_instance_t *nf_instance;

    nf_instance = ogs_calloc(1, sizeof(*nf_instance));
    ogs_assert(nf_instance);
    nf_instance->nf_type = nf_type;

    return nf_instance;
}

static void test_nf_instance_free(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_free(nf_instance);
}

/*
 * Helper: create a discovery_option with the given S-NSSAIs populated.
 * Caller must call ogs_sbi_discovery_option_free() when done.
 */
static ogs_sbi_discovery_option_t *test_discovery_option_with_snssais(
        ogs_s_nssai_t *snssais, int num)
{
    int i;
    ogs_sbi_discovery_option_t *opt;

    opt = ogs_sbi_discovery_option_new();
    ogs_assert(opt);
    for (i = 0; i < num; i++)
        ogs_sbi_discovery_option_add_snssais(opt, &snssais[i]);

    return opt;
}

/* ----------------------------------------------------------------
 * Test 1 – allowed_nssai bypass: NF has no allowed_nssai -> always match
 * ---------------------------------------------------------------- */
static void nrf_discovery_test1(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    /* num_of_allowed_nssai == 0 by calloc */

    query_snssai.sst = 1;
    query_snssai.sd.v = 0x000001;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == true);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 2 – no discovery snssais: discovery has no S-NSSAI -> bypass
 * ---------------------------------------------------------------- */
static void nrf_discovery_test2(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = 0x000001;

    opt = ogs_sbi_discovery_option_new();
    ogs_assert(opt);
    /* num_of_snssais == 0 */

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == true);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 3 – exact SST+SD match -> should match
 * ---------------------------------------------------------------- */
static void nrf_discovery_test3(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = 0x000ABC;

    query_snssai.sst = 1;
    query_snssai.sd.v = 0x000ABC;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == true);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 4 – SST match, SD mismatch (no wildcard) -> should NOT match
 * ---------------------------------------------------------------- */
static void nrf_discovery_test4(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = 0x000ABC;

    query_snssai.sst = 1;
    query_snssai.sd.v = 0x000DEF;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == false);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 5 – SST mismatch -> should NOT match
 * ---------------------------------------------------------------- */
static void nrf_discovery_test5(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    query_snssai.sst = 2;
    query_snssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == false);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 6 – allowed_nssai SD absent, discovery SD present -> no match
 * ---------------------------------------------------------------- */
static void nrf_discovery_test6(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    query_snssai.sst = 1;
    query_snssai.sd.v = 0x000ABC;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == false);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 7 – discovery SD absent, allowed_nssai SD present -> no match
 * ---------------------------------------------------------------- */
static void nrf_discovery_test7(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = 0x000ABC;

    query_snssai.sst = 1;
    query_snssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == false);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 8 – multiple allowed_nssai entries, match on second
 * ---------------------------------------------------------------- */
static void nrf_discovery_test8(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 2;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = 0x000001;
    nf->allowed_nssai[1].sst = 2;
    nf->allowed_nssai[1].sd.v = 0x000002;

    query_snssai.sst = 2;
    query_snssai.sd.v = 0x000002;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == true);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 9 – multiple discovery snssais, match on second
 * ---------------------------------------------------------------- */
static void nrf_discovery_test9(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssais[2];
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 3;
    nf->allowed_nssai[0].sd.v = 0x000003;

    query_snssais[0].sst = 1;
    query_snssais[0].sd.v = 0x000001;
    query_snssais[1].sst = 3;
    query_snssais[1].sd.v = 0x000003;
    opt = test_discovery_option_with_snssais(query_snssais, 2);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == true);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 10 – multiple allowed + multiple discovery, all mismatch
 * ---------------------------------------------------------------- */
static void nrf_discovery_test10(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssais[2];
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 2;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = 0x000001;
    nf->allowed_nssai[1].sst = 2;
    nf->allowed_nssai[1].sd.v = 0x000002;

    query_snssais[0].sst = 3;
    query_snssais[0].sd.v = 0x000003;
    query_snssais[1].sst = 4;
    query_snssais[1].sd.v = 0x000004;
    opt = test_discovery_option_with_snssais(query_snssais, 2);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == false);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 11 – both sides use NO_SD_VALUE, SST matches -> match
 * ---------------------------------------------------------------- */
static void nrf_discovery_test11(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = 1;
    nf->allowed_nssai[0].sst = 1;
    nf->allowed_nssai[0].sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    query_snssai.sst = 1;
    query_snssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == true);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

/* ----------------------------------------------------------------
 * Test 12 – nf_instance_clear resets s_nssai and allowed_nssai counts
 * ---------------------------------------------------------------- */
static void nrf_discovery_test12(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;

    nf = ogs_calloc(1, sizeof(*nf));
    ogs_assert(nf);

    nf->num_of_s_nssai = 3;
    nf->s_nssai[0].sst = 1;
    nf->s_nssai[1].sst = 2;
    nf->s_nssai[2].sst = 3;

    nf->num_of_allowed_nssai = 2;
    nf->allowed_nssai[0].sst = 10;
    nf->allowed_nssai[1].sst = 20;

    ogs_sbi_nf_instance_clear(nf);

    ABTS_INT_EQUAL(tc, 0, nf->num_of_s_nssai);
    ABTS_INT_EQUAL(tc, 0, nf->num_of_allowed_nssai);
    ABTS_INT_EQUAL(tc, 0, nf->num_of_allowed_nf_type);

    ogs_free(nf);
}

/* ----------------------------------------------------------------
 * Test 13 – boundary: max allowed_nssai entries (OGS_MAX_NUM_OF_SLICE)
 * ---------------------------------------------------------------- */
static void nrf_discovery_test13(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf;
    ogs_sbi_discovery_option_t *opt;
    ogs_s_nssai_t query_snssai;
    bool result;
    int i;

    nf = test_nf_instance_new(OpenAPI_nf_type_UDR);
    nf->num_of_allowed_nssai = OGS_MAX_NUM_OF_SLICE;
    for (i = 0; i < OGS_MAX_NUM_OF_SLICE; i++) {
        nf->allowed_nssai[i].sst = (i + 1);
        nf->allowed_nssai[i].sd.v = (i + 1);
    }

    /* Query the last entry -> should match */
    query_snssai.sst = OGS_MAX_NUM_OF_SLICE;
    query_snssai.sd.v = OGS_MAX_NUM_OF_SLICE;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == true);

    ogs_sbi_discovery_option_free(opt);

    /* Query something not in the list -> should NOT match */
    query_snssai.sst = OGS_MAX_NUM_OF_SLICE + 1;
    query_snssai.sd.v = OGS_MAX_NUM_OF_SLICE + 1;
    opt = test_discovery_option_with_snssais(&query_snssai, 1);

    result = ogs_sbi_discovery_option_is_matched(
                nf, OpenAPI_nf_type_AMF, opt);
    ABTS_TRUE(tc, result == false);

    ogs_sbi_discovery_option_free(opt);
    test_nf_instance_free(nf);
}

abts_suite *test_nrf_discovery(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, nrf_discovery_test1, NULL);
    abts_run_test(suite, nrf_discovery_test2, NULL);
    abts_run_test(suite, nrf_discovery_test3, NULL);
    abts_run_test(suite, nrf_discovery_test4, NULL);
    abts_run_test(suite, nrf_discovery_test5, NULL);
    abts_run_test(suite, nrf_discovery_test6, NULL);
    abts_run_test(suite, nrf_discovery_test7, NULL);
    abts_run_test(suite, nrf_discovery_test8, NULL);
    abts_run_test(suite, nrf_discovery_test9, NULL);
    abts_run_test(suite, nrf_discovery_test10, NULL);
    abts_run_test(suite, nrf_discovery_test11, NULL);
    abts_run_test(suite, nrf_discovery_test12, NULL);
    abts_run_test(suite, nrf_discovery_test13, NULL);

    return suite;
}
