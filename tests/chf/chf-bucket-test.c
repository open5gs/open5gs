/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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
 * Quota buckets: which allowance a session is charged to.
 *
 * A bucket applies to a scope - a slice, a DNN, a home PLMN, a roaming
 * state - and any dimension it leaves open matches anything.  Every
 * dimension arrives in the ChargingDataRequest (TS 32.291): the slice in
 * networkSlicingInfo.sNSSAI, the DNN in dnnId, the home network in
 * hPlmnId and roaming in userInformation.roamerInOut.
 *
 * Among the buckets that match, the operator's priority decides, highest
 * first; equal priorities keep the older bucket so a lookup never depends
 * on the order the database happened to return.
 *
 * A pool is a bucket several SIMs share, listed in its members.  At equal
 * priority a SIM's own bucket is spent before its pool.
 */

#include "ogs-sbi.h"
#include "chf/context.h"
#include "chf/db-handler.h"
#include "core/abts.h"

/* A bucket for a scope.  NULL or 0 leaves a dimension open. */
static chf_bucket_t mk(const char *id, int priority,
        uint8_t sst, const char *sd, const char *dnn,
        const char *hplmn, const char *roaming)
{
    chf_bucket_t b;

    memset(&b, 0, sizeof(b));
    ogs_cpystrn(b.id, id, sizeof(b.id));
    b.priority = priority;
    b.scope.s_nssai.sst = sst;
    b.scope.s_nssai.sd.v = sd ? ogs_s_nssai_sd_from_string(sd).v
                              : OGS_S_NSSAI_NO_SD_VALUE;
    if (dnn) ogs_cpystrn(b.scope.dnn, dnn, sizeof(b.scope.dnn));
    if (hplmn) ogs_cpystrn(b.scope.hplmn, hplmn, sizeof(b.scope.hplmn));
    if (roaming)
        ogs_cpystrn(b.scope.roaming, roaming, sizeof(b.scope.roaming));

    return b;
}

static chf_bucket_key_t key_of(uint8_t sst, const char *sd, const char *dnn,
        const char *hplmn, const char *roaming)
{
    chf_bucket_key_t k;

    memset(&k, 0, sizeof(k));
    k.supi = "imsi-999700000000001";
    k.s_nssai.sst = sst;
    k.s_nssai.sd.v = sd ? ogs_s_nssai_sd_from_string(sd).v
                        : OGS_S_NSSAI_NO_SD_VALUE;
    k.dnn = dnn;
    k.hplmn = hplmn;
    k.roaming = roaming;

    return k;
}

/* ------------------------------------------------------------------ */
/* An open dimension matches anything; a named one must be carried      */
/* ------------------------------------------------------------------ */
static void test_scope_open_and_named(abts_case *tc, void *data)
{
    chf_bucket_t catch_all = mk("imsi-999700000000001", 0,
            0, NULL, NULL, NULL, NULL);
    chf_bucket_t slice1 = mk("b-slice1", 10, 1, NULL, NULL, NULL, NULL);
    chf_bucket_t ims = mk("b-ims", 10, 0, NULL, "ims", NULL, NULL);
    chf_bucket_key_t k;

    /* The catch-all covers every session, including one that carries
     * nothing but a SUPI. */
    k = key_of(1, "010203", "internet", "999-70", "home");
    ABTS_TRUE(tc, chf_bucket_scope_matches(&catch_all.scope, &k) == true);
    k = key_of(0, NULL, NULL, NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&catch_all.scope, &k) == true);

    /* A slice bucket takes any session on that slice... */
    k = key_of(1, "010203", "internet", "999-70", "home");
    ABTS_TRUE(tc, chf_bucket_scope_matches(&slice1.scope, &k) == true);
    /* ...but not another slice, and not a session with no slice at all:
     * a request that does not say must not spend a scoped allowance. */
    k = key_of(2, NULL, "internet", NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&slice1.scope, &k) == false);
    k = key_of(0, NULL, "internet", NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&slice1.scope, &k) == false);

    /* Likewise for the DNN */
    k = key_of(1, NULL, "ims", NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&ims.scope, &k) == true);
    k = key_of(1, NULL, "internet", NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&ims.scope, &k) == false);
    k = key_of(1, NULL, NULL, NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&ims.scope, &k) == false);
}

/* ------------------------------------------------------------------ */
/* An SST-only bucket covers every instance of that slice; one that     */
/* names an SD covers only that instance                                */
/* ------------------------------------------------------------------ */
static void test_scope_slice_differentiator(abts_case *tc, void *data)
{
    chf_bucket_t sst_only = mk("b-sst", 10, 1, NULL, NULL, NULL, NULL);
    chf_bucket_t with_sd  = mk("b-sd", 10, 1, "010203", NULL, NULL, NULL);
    chf_bucket_key_t k;

    k = key_of(1, "010203", NULL, NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&sst_only.scope, &k) == true);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&with_sd.scope, &k) == true);

    k = key_of(1, "aabbcc", NULL, NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&sst_only.scope, &k) == true);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&with_sd.scope, &k) == false);

    /* No SD in the request: the SST bucket still applies, the SD one
     * does not. */
    k = key_of(1, NULL, NULL, NULL, NULL);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&sst_only.scope, &k) == true);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&with_sd.scope, &k) == false);
}

/* ------------------------------------------------------------------ */
/* Home PLMN and roaming                                                */
/* ------------------------------------------------------------------ */
static void test_scope_plmn_and_roaming(abts_case *tc, void *data)
{
    chf_bucket_t home = mk("b-home", 10, 0, NULL, NULL, NULL, "home");
    chf_bucket_t abroad = mk("b-out", 20, 0, NULL, NULL, NULL, "outBound");
    chf_bucket_t mvno = mk("b-mvno", 10, 0, NULL, NULL, "999-70", NULL);
    chf_bucket_key_t k;

    k = key_of(1, NULL, "internet", "999-70", "home");
    ABTS_TRUE(tc, chf_bucket_scope_matches(&home.scope, &k) == true);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&abroad.scope, &k) == false);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&mvno.scope, &k) == true);

    k = key_of(1, NULL, "internet", "999-70", "outBound");
    ABTS_TRUE(tc, chf_bucket_scope_matches(&home.scope, &k) == false);
    ABTS_TRUE(tc, chf_bucket_scope_matches(&abroad.scope, &k) == true);

    /* Another operator's subscriber: not this MVNO's bucket */
    k = key_of(1, NULL, "internet", "001-01", "inBound");
    ABTS_TRUE(tc, chf_bucket_scope_matches(&mvno.scope, &k) == false);
}

/* ------------------------------------------------------------------ */
/* Selection: highest priority among those that match                   */
/* ------------------------------------------------------------------ */
static void test_select_by_priority(abts_case *tc, void *data)
{
    chf_bucket_t buckets[4];
    chf_bucket_key_t k;
    int chosen;

    /* oldest first, as the loader returns them */
    buckets[0] = mk("catch-all", 0,  0, NULL, NULL, NULL, NULL);
    buckets[1] = mk("slice1",   10,  1, NULL, NULL, NULL, NULL);
    buckets[2] = mk("ims",      20,  0, NULL, "ims", NULL, NULL);
    buckets[3] = mk("roaming",  30,  0, NULL, NULL, NULL, "outBound");

    /* Slice 1 at home on internet: only the catch-all and the slice
     * bucket match, and the slice bucket was given the higher number. */
    k = key_of(1, NULL, "internet", "999-70", "home");
    chosen = chf_bucket_select(buckets, 4, &k);
    ABTS_INT_EQUAL(tc, 1, chosen);

    /* Same session on the IMS DNN: the IMS bucket outranks the slice. */
    k = key_of(1, NULL, "ims", "999-70", "home");
    chosen = chf_bucket_select(buckets, 4, &k);
    ABTS_INT_EQUAL(tc, 2, chosen);

    /* Roaming: the roaming bucket outranks both. */
    k = key_of(1, NULL, "ims", "999-70", "outBound");
    chosen = chf_bucket_select(buckets, 4, &k);
    ABTS_INT_EQUAL(tc, 3, chosen);

    /* A slice nobody provisioned still lands in the catch-all. */
    k = key_of(9, NULL, "internet", "999-70", "home");
    chosen = chf_bucket_select(buckets, 4, &k);
    ABTS_INT_EQUAL(tc, 0, chosen);
}

/* ------------------------------------------------------------------ */
/* Ties, and the case where nothing matches                             */
/* ------------------------------------------------------------------ */
static void test_select_ties_and_no_match(abts_case *tc, void *data)
{
    chf_bucket_t buckets[2];
    chf_bucket_key_t k;

    /* Two buckets, same priority, both matching: the older wins, and it
     * keeps winning however often the question is asked. */
    buckets[0] = mk("older", 10, 1, NULL, NULL, NULL, NULL);
    buckets[1] = mk("newer", 10, 0, NULL, "internet", NULL, NULL);
    k = key_of(1, NULL, "internet", NULL, NULL);
    ABTS_INT_EQUAL(tc, 0, chf_bucket_select(buckets, 2, &k));
    ABTS_INT_EQUAL(tc, 0, chf_bucket_select(buckets, 2, &k));

    /* Provisioned, but only for slices this session is not on: no bucket.
     * The caller treats that like an unprovisioned SUPI. */
    buckets[0] = mk("slice2", 10, 2, NULL, NULL, NULL, NULL);
    buckets[1] = mk("slice3", 10, 3, NULL, NULL, NULL, NULL);
    k = key_of(1, NULL, "internet", NULL, NULL);
    ABTS_INT_EQUAL(tc, -1, chf_bucket_select(buckets, 2, &k));

    /* No buckets at all */
    ABTS_INT_EQUAL(tc, -1, chf_bucket_select(buckets, 0, &k));
    ABTS_INT_EQUAL(tc, -1, chf_bucket_select(NULL, 0, &k));
}

/* ------------------------------------------------------------------ */
/* Pools                                                                */
/* ------------------------------------------------------------------ */
static void test_select_own_before_pool(abts_case *tc, void *data)
{
    chf_bucket_t buckets[3];
    chf_bucket_key_t k = key_of(1, NULL, "internet", NULL, NULL);

    /* The family pool is older, but at equal priority the SIM's own add-on
     * is spent first. */
    buckets[0] = mk("pool-family", 0, 0, NULL, NULL, NULL, NULL);
    buckets[0].shared = true;
    buckets[1] = mk("imsi-999700000000001", 0, 0, NULL, NULL, NULL, NULL);
    ABTS_INT_EQUAL(tc, 1, chf_bucket_select(buckets, 2, &k));

    /* Either order. */
    buckets[0] = mk("imsi-999700000000001", 0, 0, NULL, NULL, NULL, NULL);
    buckets[1] = mk("pool-family", 0, 0, NULL, NULL, NULL, NULL);
    buckets[1].shared = true;
    ABTS_INT_EQUAL(tc, 0, chf_bucket_select(buckets, 2, &k));

    /* The operator can rank the pool first. */
    buckets[1].priority = 5;
    ABTS_INT_EQUAL(tc, 1, chf_bucket_select(buckets, 2, &k));

    /* A pool still loses to a scope it does not cover, and wins when the
     * SIM's own bucket does not cover the session. */
    buckets[0] = mk("own-slice2", 50, 2, NULL, NULL, NULL, NULL);
    buckets[1] = mk("pool-family", 0, 0, NULL, NULL, NULL, NULL);
    buckets[1].shared = true;
    buckets[2] = mk("pool-video", 90, 0, NULL, "video", NULL, NULL);
    buckets[2].shared = true;
    ABTS_INT_EQUAL(tc, 1, chf_bucket_select(buckets, 3, &k));
}

static void test_pool_membership(abts_case *tc, void *data)
{
    bson_t *pool = BCON_NEW("bucketId", BCON_UTF8("pool-0123456789ab"),
            "members", "[",
                BCON_UTF8("imsi-999700000000001"),
                BCON_UTF8("imsi-999700000000002"),
                BCON_UTF8("imsi-999700000000003"), "]");
    bson_t *own = BCON_NEW("bucketId", BCON_UTF8("imsi-999700000000001"),
            "supi", BCON_UTF8("imsi-999700000000001"));
    bson_t *odd = BCON_NEW("members", BCON_UTF8("imsi-999700000000001"));
    bson_t *mixed = BCON_NEW("members", "[", BCON_INT32(7),
            BCON_UTF8("imsi-999700000000009"), "]");

    ABTS_TRUE(tc, chf_db_is_pool(pool));
    ABTS_TRUE(tc, !chf_db_is_pool(own));
    /* A members field that is not an array does not make a pool. */
    ABTS_TRUE(tc, !chf_db_is_pool(odd));
    ABTS_TRUE(tc, !chf_db_is_pool(NULL));

    /* Positions count from 0, in list order: the CHF charges a pool to its
     * first charging.pool.max_members members. */
    ABTS_INT_EQUAL(tc, 0, chf_db_pool_position(pool, "imsi-999700000000001"));
    ABTS_INT_EQUAL(tc, 2, chf_db_pool_position(pool, "imsi-999700000000003"));
    ABTS_INT_EQUAL(tc, -1, chf_db_pool_position(pool, "imsi-999700000000004"));
    ABTS_INT_EQUAL(tc, -1, chf_db_pool_position(pool, "imsi-99970000000000"));
    ABTS_INT_EQUAL(tc, -1, chf_db_pool_position(pool, NULL));
    ABTS_INT_EQUAL(tc, -1, chf_db_pool_position(own, "imsi-999700000000001"));
    ABTS_INT_EQUAL(tc, -1, chf_db_pool_position(odd, "imsi-999700000000001"));
    /* An element that is not a string still takes its place. */
    ABTS_INT_EQUAL(tc, 1, chf_db_pool_position(mixed, "imsi-999700000000009"));

    bson_destroy(mixed);
    bson_destroy(odd);
    bson_destroy(own);
    bson_destroy(pool);
}

/* ------------------------------------------------------------------ */
/* Suite registration                                                   */
/* ------------------------------------------------------------------ */
abts_suite *test_chf_bucket(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_scope_open_and_named,      NULL);
    abts_run_test(suite, test_scope_slice_differentiator, NULL);
    abts_run_test(suite, test_scope_plmn_and_roaming,    NULL);
    abts_run_test(suite, test_select_by_priority,        NULL);
    abts_run_test(suite, test_select_ties_and_no_match,  NULL);
    abts_run_test(suite, test_select_own_before_pool,    NULL);
    abts_run_test(suite, test_pool_membership,           NULL);

    return suite;
}
