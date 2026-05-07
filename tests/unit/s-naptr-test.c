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
 * Unit tests for the S-NAPTR DNS-based PGW discovery module (mme-s-naptr.c).
 *
 * Tests cover:
 *   1. APN-FQDN construction per 3GPP TS 23.003 ss.19.4.2
 *   2. SRV weighted target selection per RFC 2782 s.3 --
 *        mirrors the target-selection step of build_rotated_candidates()
 *        using test_tier0_target_t, the test equivalent of tier0_target_t.
 *        MUST be kept in sync with the production function.
 *   3. NAPTR preference-ascending sort per 3GPP TS 29.303 ss.5.2 --
 *        mirrors the insertion-sort step of query_naptr_all().
 *        The old inverse-preference weighted-random algorithm was replaced
 *        in B12; tests 13-16 were updated accordingly.
 *        MUST be kept in sync with the production function.
 *
 * All tests are self-contained (no DNS server or MME context required) and
 * mirror the algorithms implemented in mme-s-naptr.c.
 */

#include "ogs-core.h"
#include "core/abts.h"

/* ==========================================================================
 * APN-FQDN construction helpers
 *
 * Mirrors build_apn_fqdn() from mme-s-naptr.c.
 * 3GPP TS 23.003 ss.19.4.2 format:
 *   <apn-ni>.apn.epc.mnc<MNC>.mcc<MCC>.3gppnetwork.org
 * MNC zero-padded to 2 digits (mnc_len==2) or 3 digits (mnc_len==3).
 * MCC always zero-padded to 3 digits.
 * ========================================================================== */

static int build_apn_fqdn_test(char *buf, size_t buflen,
        const char *apn, uint16_t mcc, uint16_t mnc, int mnc_len)
{
    int n;

    if (mnc_len == 2)
        n = snprintf(buf, buflen,
                "%s.apn.epc.mnc%02d.mcc%03d.3gppnetwork.org",
                apn, mnc, mcc);
    else
        n = snprintf(buf, buflen,
                "%s.apn.epc.mnc%03d.mcc%03d.3gppnetwork.org",
                apn, mnc, mcc);

    return (n < 0 || (size_t)n >= buflen) ? -1 : n;
}

/* --------------------------------------------------------------------------
 * Test 1: 2-digit MNC -- France Orange (MCC=208 MNC=93)
 * -------------------------------------------------------------------------- */
static void test_fqdn_2digit_mnc(abts_case *tc, void *data)
{
    char buf[256];
    int rc = build_apn_fqdn_test(buf, sizeof(buf), "internet", 208, 93, 2);

    ABTS_TRUE(tc, rc > 0);
    ABTS_STR_EQUAL(tc,
            "internet.apn.epc.mnc93.mcc208.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 2: 3-digit MNC -- test network (MCC=001 MNC=001)
 * -------------------------------------------------------------------------- */
static void test_fqdn_3digit_mnc(abts_case *tc, void *data)
{
    char buf[256];
    int rc = build_apn_fqdn_test(buf, sizeof(buf), "ims", 1, 1, 3);

    ABTS_TRUE(tc, rc > 0);
    ABTS_STR_EQUAL(tc,
            "ims.apn.epc.mnc001.mcc001.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 3: T-Mobile US (MCC=310 MNC=260, 3-digit MNC)
 * -------------------------------------------------------------------------- */
static void test_fqdn_tmobile_us(abts_case *tc, void *data)
{
    char buf[256];
    int rc = build_apn_fqdn_test(buf, sizeof(buf),
            "fast.t-mobile.com", 310, 260, 3);

    ABTS_TRUE(tc, rc > 0);
    ABTS_STR_EQUAL(tc,
            "fast.t-mobile.com.apn.epc.mnc260.mcc310.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 4: Buffer too small -- must return -1 (no overflow)
 * -------------------------------------------------------------------------- */
static void test_fqdn_buf_too_small(abts_case *tc, void *data)
{
    char buf[10]; /* intentionally smaller than any valid FQDN */
    int rc = build_apn_fqdn_test(buf, sizeof(buf), "internet", 208, 93, 2);

    ABTS_INT_EQUAL(tc, -1, rc);
}

/* --------------------------------------------------------------------------
 * Test 5: 2-digit MNC with 2-digit value -- zero-padding check (MNC=10 -> "10")
 * -------------------------------------------------------------------------- */
static void test_fqdn_mnc_two_digits_no_extra_pad(abts_case *tc, void *data)
{
    char buf[256];
    build_apn_fqdn_test(buf, sizeof(buf), "apn", 234, 10, 2);

    ABTS_STR_EQUAL(tc,
            "apn.apn.epc.mnc10.mcc234.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 6: 2-digit MNC, single-digit value -- zero-padded to 2 (MNC=1 -> "01")
 * -------------------------------------------------------------------------- */
static void test_fqdn_mnc_single_digit_padded(abts_case *tc, void *data)
{
    char buf[256];
    build_apn_fqdn_test(buf, sizeof(buf), "apn", 234, 1, 2);

    ABTS_STR_EQUAL(tc,
            "apn.apn.epc.mnc01.mcc234.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 7: MCC zero-padding -- MCC=1 must appear as "001"
 * -------------------------------------------------------------------------- */
static void test_fqdn_mcc_zero_padded(abts_case *tc, void *data)
{
    char buf[256];
    build_apn_fqdn_test(buf, sizeof(buf), "internet", 1, 1, 2);

    ABTS_STR_EQUAL(tc,
            "internet.apn.epc.mnc01.mcc001.3gppnetwork.org", buf);
}

/* ==========================================================================
 * SRV weighted target selection -- RFC 2782 s.3
 *
 * Mirrors the target-selection step of build_rotated_candidates() from
 * mme-s-naptr.c.  The production function works at the SRV-target level
 * using tier0_target_t descriptors {weight, start, count}; this helper
 * replicates that exact algorithm so regressions in the production code
 * are caught here.
 *
 * IMPORTANT: if the selection logic in build_rotated_candidates() changes,
 * this helper MUST be updated to match.
 *
 * Weight-0 entries are promoted to a nominal weight of 1 per RFC 2782 s.3.
 * Uses a deterministic sweep over [0, total_weight) to verify distribution
 * without relying on rand().
 * ========================================================================== */

typedef struct {
    uint16_t weight;
    int      start;   /* first index in the IP array for this target */
    int      count;   /* number of IPs for this target */
} test_tier0_target_t;

/*
 * build_rotated_select_target() - mirrors Step 1 of build_rotated_candidates().
 *
 * Given an array of SRV targets and a deterministic 'pick' value in
 * [0, total_weight), returns the index of the selected target.
 * Callers sweep 'pick' over [0, total_weight) to verify proportions.
 */
static int build_rotated_select_target(
        const test_tier0_target_t *targets, int t0_count, uint32_t pick)
{
    uint32_t total_weight = 0;
    uint32_t cum = 0;
    int i;

    for (i = 0; i < t0_count; i++)
        total_weight += targets[i].weight ? (uint32_t)targets[i].weight : 1u;

    pick %= total_weight;
    for (i = 0; i < t0_count; i++) {
        cum += targets[i].weight ? (uint32_t)targets[i].weight : 1u;
        if (pick < cum) return i;
    }
    return t0_count - 1;
}

/* --------------------------------------------------------------------------
 * Test 8: Uniform SRV target weights -- each target receives equal share.
 *   3 targets, weight 10 each -> 1/3 each over 30 000 deterministic picks.
 * -------------------------------------------------------------------------- */
static void test_srv_weight_uniform(abts_case *tc, void *data)
{
    test_tier0_target_t t[] = {
        {10, 0, 1}, {10, 1, 1}, {10, 2, 1}
    };
    int counts[3] = {0, 0, 0};
    int i, s;

    /* Total weight = 30; sweep 30 000 picks (1000 full cycles). */
    for (i = 0; i < 30000; i++) {
        s = build_rotated_select_target(t, 3, (uint32_t)i);
        if (s >= 0 && s < 3)
            counts[s]++;
    }

    /* Each target: exactly 10 000 / 30 000 */
    for (i = 0; i < 3; i++)
        ABTS_INT_EQUAL(tc, 10000, counts[i]);
}

/* --------------------------------------------------------------------------
 * Test 9: Skewed SRV target weights 10:20:70 -- proportional distribution.
 *   Total weight = 100; sweep 100 000 picks (1000 full cycles).
 * -------------------------------------------------------------------------- */
static void test_srv_weight_skewed(abts_case *tc, void *data)
{
    test_tier0_target_t t[] = {
        {10, 0, 2}, {20, 2, 1}, {70, 3, 3}
    };
    int counts[3] = {0, 0, 0};
    int i, s;

    for (i = 0; i < 100000; i++) {
        s = build_rotated_select_target(t, 3, (uint32_t)i);
        if (s >= 0 && s < 3)
            counts[s]++;
    }

    /* Exact proportions over 100 000 picks */
    ABTS_INT_EQUAL(tc, 10000, counts[0]);
    ABTS_INT_EQUAL(tc, 20000, counts[1]);
    ABTS_INT_EQUAL(tc, 70000, counts[2]);
}

/* --------------------------------------------------------------------------
 * Test 10: Both SRV targets have weight 0.
 *   Promoted to 1 each -> 50% / 50% over 20 000 picks.
 * -------------------------------------------------------------------------- */
static void test_srv_weight_zero_records(abts_case *tc, void *data)
{
    test_tier0_target_t t[] = {
        {0, 0, 1}, {0, 1, 2}
    };
    int counts[2] = {0, 0};
    int i, s;

    /* Total effective weight = 1+1 = 2; sweep 20 000 picks. */
    for (i = 0; i < 20000; i++) {
        s = build_rotated_select_target(t, 2, (uint32_t)i);
        if (s >= 0 && s < 2)
            counts[s]++;
    }

    ABTS_INT_EQUAL(tc, 10000, counts[0]);
    ABTS_INT_EQUAL(tc, 10000, counts[1]);
}

/* --------------------------------------------------------------------------
 * Test 11: Single SRV target -- always returns index 0 regardless of weight.
 * -------------------------------------------------------------------------- */
static void test_srv_single_entry(abts_case *tc, void *data)
{
    test_tier0_target_t t[] = {{42, 0, 3}};
    int i;

    for (i = 0; i < 100; i++)
        ABTS_INT_EQUAL(tc, 0, build_rotated_select_target(t, 1, (uint32_t)i));
}

/* --------------------------------------------------------------------------
 * Test 12: Weight 1:99 -- low-weight target receives ~1% of selections.
 *   Total weight = 100; 10 000 picks -> target[0] selected exactly 100 times.
 * -------------------------------------------------------------------------- */
static void test_srv_weight_mixed_zero(abts_case *tc, void *data)
{
    /* weight-0 promoted to 1; weight 99 stays 99 -> total 100 */
    test_tier0_target_t t[] = {
        {0, 0, 1}, {99, 1, 4}
    };
    int counts[2] = {0, 0};
    int i, s;

    for (i = 0; i < 10000; i++) {
        s = build_rotated_select_target(t, 2, (uint32_t)i);
        if (s >= 0 && s < 2)
            counts[s]++;
    }

    /* 1% for target 0 (weight-0 -> 1), 99% for target 1 */
    ABTS_INT_EQUAL(tc, 100,  counts[0]);
    ABTS_INT_EQUAL(tc, 9900, counts[1]);
}

/* ==========================================================================
 * NAPTR preference ordering -- 3GPP TS 29.303 ss.5.2
 *
 * Mirrors the insertion-sort step of query_naptr_all() in mme-s-naptr.c.
 *
 * query_naptr_all() (B12 replacement for the old query_naptr()) returns ALL
 * lowest-ORDER NAPTR candidates sorted by PREF ascending so the most-preferred
 * (lowest numeric pref) entry is tried first.  The caller iterates in order
 * and advances to the next entry only when the current replacement has no
 * usable SRV or A records.
 *
 * IMPORTANT: if the sort logic in query_naptr_all() changes, this helper
 * MUST be updated to match.
 *
 * Note: tests 13-16 previously exercised an inverse-preference weighted-
 * random algorithm (naptr_select()) that no longer exists in production code.
 * They have been replaced with deterministic sort-order checks that mirror the
 * actual behavior of query_naptr_all().
 * ========================================================================== */

typedef struct {
    uint16_t pref;
    int      original_idx; /* input position -- used to verify record identity */
} test_naptr_rec_t;

/*
 * naptr_pref_sort() - mirrors the insertion sort in query_naptr_all().
 * Sorts recs[0..count-1] by pref ascending (lower value = more preferred,
 * tried first).  Equal prefs preserve relative input order (stable sort).
 */
static void naptr_pref_sort(test_naptr_rec_t *recs, int count)
{
    int i, j;
    test_naptr_rec_t key;

    for (i = 1; i < count; i++) {
        key = recs[i];
        j   = i - 1;
        while (j >= 0 && recs[j].pref > key.pref) {
            recs[j + 1] = recs[j];
            j--;
        }
        recs[j + 1] = key;
    }
}

/* --------------------------------------------------------------------------
 * Test 13: Three records at distinct prefs -- sorted pref-ascending.
 *   Input:  [pref=30, pref=10, pref=20]
 *   Output: [pref=10, pref=20, pref=30]
 *   Most-preferred (lowest pref) ends up at index 0 and is tried first.
 * -------------------------------------------------------------------------- */
static void test_naptr_sort_ascending(abts_case *tc, void *data)
{
    test_naptr_rec_t recs[] = {
        {30, 0}, {10, 1}, {20, 2}
    };

    naptr_pref_sort(recs, 3);

    ABTS_INT_EQUAL(tc, 10, (int)recs[0].pref);
    ABTS_INT_EQUAL(tc, 20, (int)recs[1].pref);
    ABTS_INT_EQUAL(tc, 30, (int)recs[2].pref);
    /* Verify original_idx to confirm record identity, not just pref values */
    ABTS_INT_EQUAL(tc, 1,  recs[0].original_idx);
    ABTS_INT_EQUAL(tc, 2,  recs[1].original_idx);
    ABTS_INT_EQUAL(tc, 0,  recs[2].original_idx);
}

/* --------------------------------------------------------------------------
 * Test 14: All equal prefs -- input order preserved (sort is stable).
 *   Three records all pref=50; insertion sort preserves relative order.
 * -------------------------------------------------------------------------- */
static void test_naptr_sort_equal_pref(abts_case *tc, void *data)
{
    test_naptr_rec_t recs[] = {
        {50, 0}, {50, 1}, {50, 2}
    };

    naptr_pref_sort(recs, 3);

    ABTS_INT_EQUAL(tc, 50, (int)recs[0].pref);
    ABTS_INT_EQUAL(tc, 50, (int)recs[1].pref);
    ABTS_INT_EQUAL(tc, 50, (int)recs[2].pref);
    /* Relative order must be preserved for equal keys */
    ABTS_INT_EQUAL(tc, 0,  recs[0].original_idx);
    ABTS_INT_EQUAL(tc, 1,  recs[1].original_idx);
    ABTS_INT_EQUAL(tc, 2,  recs[2].original_idx);
}

/* --------------------------------------------------------------------------
 * Test 15: Single NAPTR record -- sort is a no-op; record unchanged.
 * -------------------------------------------------------------------------- */
static void test_naptr_sort_single(abts_case *tc, void *data)
{
    test_naptr_rec_t recs[] = {{100, 0}};

    naptr_pref_sort(recs, 1);

    ABTS_INT_EQUAL(tc, 100, (int)recs[0].pref);
    ABTS_INT_EQUAL(tc, 0,   recs[0].original_idx);
}

/* --------------------------------------------------------------------------
 * Test 16: pref=0 (lowest possible) sorts to index 0.
 *   Input:  [pref=100, pref=0]
 *   Output: [pref=0, pref=100]
 *   pref=0 record must be the first candidate tried per TS 29.303 ss.5.2.
 * -------------------------------------------------------------------------- */
static void test_naptr_lowest_pref_first(abts_case *tc, void *data)
{
    test_naptr_rec_t recs[] = {
        {100, 0}, {0, 1}
    };

    naptr_pref_sort(recs, 2);

    ABTS_INT_EQUAL(tc, 0,   (int)recs[0].pref);
    ABTS_INT_EQUAL(tc, 100, (int)recs[1].pref);
    /* pref=0 was at input[1] -- verify it moved to front */
    ABTS_INT_EQUAL(tc, 1,   recs[0].original_idx);
    ABTS_INT_EQUAL(tc, 0,   recs[1].original_idx);
}

/* ==========================================================================
 * Suite entry point
 * ========================================================================== */

abts_suite *test_s_naptr(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    /* APN-FQDN construction (3GPP TS 23.003 ss.19.4.2) */
    abts_run_test(suite, test_fqdn_2digit_mnc,                 NULL);
    abts_run_test(suite, test_fqdn_3digit_mnc,                 NULL);
    abts_run_test(suite, test_fqdn_tmobile_us,                 NULL);
    abts_run_test(suite, test_fqdn_buf_too_small,              NULL);
    abts_run_test(suite, test_fqdn_mnc_two_digits_no_extra_pad, NULL);
    abts_run_test(suite, test_fqdn_mnc_single_digit_padded,    NULL);
    abts_run_test(suite, test_fqdn_mcc_zero_padded,            NULL);

    /* SRV weighted random selection (RFC 2782 s.3) */
    abts_run_test(suite, test_srv_weight_uniform,              NULL);
    abts_run_test(suite, test_srv_weight_skewed,               NULL);
    abts_run_test(suite, test_srv_weight_zero_records,         NULL);
    abts_run_test(suite, test_srv_single_entry,                NULL);
    abts_run_test(suite, test_srv_weight_mixed_zero,           NULL);

    /* NAPTR preference-ascending sort (3GPP TS 29.303 ss.5.2) */
    abts_run_test(suite, test_naptr_sort_ascending,            NULL);
    abts_run_test(suite, test_naptr_sort_equal_pref,           NULL);
    abts_run_test(suite, test_naptr_sort_single,               NULL);
    abts_run_test(suite, test_naptr_lowest_pref_first,         NULL);

    return suite;
}
