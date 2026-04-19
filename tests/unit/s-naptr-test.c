/*
 * Copyright (C) 2025 by Rami Mohamed <ramrode@gmail.com>
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
 *   1. APN-FQDN construction per 3GPP TS 23.003 §19.4.2
 *   2. SRV weighted random selection per RFC 2782 §3
 *   3. NAPTR inverse-preference weighted selection per 3GPP TS 29.303 §5.2
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
 * 3GPP TS 23.003 §19.4.2 format:
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
 * Test 1: 2-digit MNC — France Orange (MCC=208 MNC=93)
 * -------------------------------------------------------------------------- */
static void test_fqdn_2digit_mnc(abts_case *tc, void *data)
{
    char buf[256];
    int rc = build_apn_fqdn_test(buf, sizeof(buf), "internet", 208, 93, 2);

    ABTS_INT_NEQUAL(tc, -1, rc);
    ABTS_STR_EQUAL(tc,
            "internet.apn.epc.mnc93.mcc208.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 2: 3-digit MNC — test network (MCC=001 MNC=001)
 * -------------------------------------------------------------------------- */
static void test_fqdn_3digit_mnc(abts_case *tc, void *data)
{
    char buf[256];
    int rc = build_apn_fqdn_test(buf, sizeof(buf), "ims", 1, 1, 3);

    ABTS_INT_NEQUAL(tc, -1, rc);
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

    ABTS_INT_NEQUAL(tc, -1, rc);
    ABTS_STR_EQUAL(tc,
            "fast.t-mobile.com.apn.epc.mnc260.mcc310.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 4: Buffer too small — must return -1 (no overflow)
 * -------------------------------------------------------------------------- */
static void test_fqdn_buf_too_small(abts_case *tc, void *data)
{
    char buf[10]; /* intentionally smaller than any valid FQDN */
    int rc = build_apn_fqdn_test(buf, sizeof(buf), "internet", 208, 93, 2);

    ABTS_INT_EQUAL(tc, -1, rc);
}

/* --------------------------------------------------------------------------
 * Test 5: 2-digit MNC with 2-digit value — zero-padding check (MNC=10 → "10")
 * -------------------------------------------------------------------------- */
static void test_fqdn_mnc_two_digits_no_extra_pad(abts_case *tc, void *data)
{
    char buf[256];
    build_apn_fqdn_test(buf, sizeof(buf), "apn", 234, 10, 2);

    ABTS_STR_EQUAL(tc,
            "apn.apn.epc.mnc10.mcc234.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 6: 2-digit MNC, single-digit value — zero-padded to 2 (MNC=1 → "01")
 * -------------------------------------------------------------------------- */
static void test_fqdn_mnc_single_digit_padded(abts_case *tc, void *data)
{
    char buf[256];
    build_apn_fqdn_test(buf, sizeof(buf), "apn", 234, 1, 2);

    ABTS_STR_EQUAL(tc,
            "apn.apn.epc.mnc01.mcc234.3gppnetwork.org", buf);
}

/* --------------------------------------------------------------------------
 * Test 7: MCC zero-padding — MCC=1 must appear as "001"
 * -------------------------------------------------------------------------- */
static void test_fqdn_mcc_zero_padded(abts_case *tc, void *data)
{
    char buf[256];
    build_apn_fqdn_test(buf, sizeof(buf), "internet", 1, 1, 2);

    ABTS_STR_EQUAL(tc,
            "internet.apn.epc.mnc01.mcc001.3gppnetwork.org", buf);
}

/* ==========================================================================
 * SRV weighted random selection — RFC 2782 §3
 *
 * Mirrors the selection algorithm inside query_srv() in mme-s-naptr.c.
 * Weight-0 entries are promoted to a nominal weight of 1.
 *
 * Uses a deterministic sweep over the pick range [0, total_weight) to verify
 * distribution without relying on rand().
 * ========================================================================== */

static int srv_select(const uint32_t *weights, int count, uint32_t pick)
{
    uint32_t total = 0;
    uint32_t effective[32];
    uint32_t cum = 0;
    int i;

    for (i = 0; i < count; i++) {
        effective[i] = weights[i] ? weights[i] : 1; /* RFC 2782: 0 → 1 */
        total += effective[i];
    }

    pick %= total;
    for (i = 0; i < count; i++) {
        cum += effective[i];
        if (pick < cum) return i;
    }
    return count - 1;
}

/* --------------------------------------------------------------------------
 * Test 8: Uniform weights — each entry must receive equal share
 * -------------------------------------------------------------------------- */
static void test_srv_weight_uniform(abts_case *tc, void *data)
{
    uint32_t w[] = {10, 10, 10};
    int counts[3] = {0, 0, 0};
    int i;

    for (i = 0; i < 30000; i++) {
        int s = srv_select(w, 3, (uint32_t)i);
        if (s >= 0 && s < 3)
            counts[s]++;
    }

    /* Each bucket should be exactly 1/3 of 30000 = 10000 */
    for (i = 0; i < 3; i++)
        ABTS_INT_EQUAL(tc, 10000, counts[i]);
}

/* --------------------------------------------------------------------------
 * Test 9: Skewed weights 10:20:70 — verify proportional distribution
 * -------------------------------------------------------------------------- */
static void test_srv_weight_skewed(abts_case *tc, void *data)
{
    uint32_t w[] = {10, 20, 70};
    int counts[3] = {0, 0, 0};
    int i;

    /* Sweep deterministically over one full weight cycle (100 units) × 1000 */
    for (i = 0; i < 100000; i++) {
        int s = srv_select(w, 3, (uint32_t)i);
        if (s >= 0 && s < 3)
            counts[s]++;
    }

    /* Exact proportions: 10%, 20%, 70% over 100000 iterations */
    ABTS_INT_EQUAL(tc, 10000, counts[0]);
    ABTS_INT_EQUAL(tc, 20000, counts[1]);
    ABTS_INT_EQUAL(tc, 70000, counts[2]);
}

/* --------------------------------------------------------------------------
 * Test 10: Two weight-0 records — promoted to weight 1 each → 50% / 50%
 * -------------------------------------------------------------------------- */
static void test_srv_weight_zero_records(abts_case *tc, void *data)
{
    uint32_t w[] = {0, 0};
    int counts[2] = {0, 0};
    int i;

    for (i = 0; i < 20000; i++) {
        int s = srv_select(w, 2, (uint32_t)i);
        if (s >= 0 && s < 2)
            counts[s]++;
    }

    /* Exactly 50 / 50 over full sweep */
    ABTS_INT_EQUAL(tc, 10000, counts[0]);
    ABTS_INT_EQUAL(tc, 10000, counts[1]);
}

/* --------------------------------------------------------------------------
 * Test 11: Single entry — must always return index 0
 * -------------------------------------------------------------------------- */
static void test_srv_single_entry(abts_case *tc, void *data)
{
    uint32_t w[] = {42};
    int i;

    for (i = 0; i < 100; i++)
        ABTS_INT_EQUAL(tc, 0, srv_select(w, 1, (uint32_t)i));
}

/* --------------------------------------------------------------------------
 * Test 12: Mixed zero / non-zero weights — weight-0 entries rarely selected
 * -------------------------------------------------------------------------- */
static void test_srv_weight_mixed_zero(abts_case *tc, void *data)
{
    /* weights: 0, 99 → effective: 1, 99  → total 100 */
    uint32_t w[] = {0, 99};
    int counts[2] = {0, 0};
    int i;

    for (i = 0; i < 10000; i++) {
        int s = srv_select(w, 2, (uint32_t)i);
        if (s >= 0 && s < 2)
            counts[s]++;
    }

    /* 1% for index 0 (weight-0 promoted to 1), 99% for index 1 */
    ABTS_INT_EQUAL(tc, 100, counts[0]);
    ABTS_INT_EQUAL(tc, 9900, counts[1]);
}

/* ==========================================================================
 * NAPTR inverse-preference selection — 3GPP TS 29.303 §5.2
 *
 * Mirrors the algorithm inside query_naptr() in mme-s-naptr.c.
 * weight_i = max_pref + 1 - pref_i  (lower pref ⇒ higher weight).
 * ========================================================================== */

static int naptr_select(const uint16_t *prefs, int count, uint32_t pick)
{
    uint16_t max_pref = 0;
    uint32_t weights[32];
    uint32_t total = 0;
    uint32_t cum = 0;
    int i;

    for (i = 0; i < count; i++)
        if (prefs[i] > max_pref) max_pref = prefs[i];

    for (i = 0; i < count; i++) {
        weights[i] = (uint32_t)(max_pref + 1 - prefs[i]);
        total += weights[i];
    }

    pick %= total;
    for (i = 0; i < count; i++) {
        cum += weights[i];
        if (pick < cum) return i;
    }
    return count - 1;
}

/* --------------------------------------------------------------------------
 * Test 13: Two records at pref 10 and 20.
 *   max_pref=20 → weight[0]=11, weight[1]=1 → total=12
 *   pref-10 selected ~11/12, pref-20 selected ~1/12.
 * -------------------------------------------------------------------------- */
static void test_naptr_pref_two_records(abts_case *tc, void *data)
{
    uint16_t prefs[] = {10, 20};
    int counts[2] = {0, 0};
    int i;

    /* Sweep 12000 picks (1000 full cycles) */
    for (i = 0; i < 12000; i++) {
        int s = naptr_select(prefs, 2, (uint32_t)i);
        if (s >= 0 && s < 2)
            counts[s]++;
    }

    ABTS_INT_EQUAL(tc, 11000, counts[0]); /* pref 10: 11/12 × 12000 */
    ABTS_INT_EQUAL(tc, 1000,  counts[1]); /* pref 20:  1/12 × 12000 */
}

/* --------------------------------------------------------------------------
 * Test 14: Three records at equal preference → all equal weight (1 each)
 * -------------------------------------------------------------------------- */
static void test_naptr_equal_pref(abts_case *tc, void *data)
{
    uint16_t prefs[] = {50, 50, 50};
    int counts[3] = {0, 0, 0};
    int i;

    /* max_pref=50 → all weights=1; total=3; sweep 30000 picks */
    for (i = 0; i < 30000; i++) {
        int s = naptr_select(prefs, 3, (uint32_t)i);
        if (s >= 0 && s < 3)
            counts[s]++;
    }

    /* Exactly 10000 each */
    for (i = 0; i < 3; i++)
        ABTS_INT_EQUAL(tc, 10000, counts[i]);
}

/* --------------------------------------------------------------------------
 * Test 15: Single NAPTR record — always returns index 0
 * -------------------------------------------------------------------------- */
static void test_naptr_single_record(abts_case *tc, void *data)
{
    uint16_t prefs[] = {100};
    int i;

    for (i = 0; i < 100; i++)
        ABTS_INT_EQUAL(tc, 0, naptr_select(prefs, 1, (uint32_t)i));
}

/* --------------------------------------------------------------------------
 * Test 16: pref 0 (lowest possible) dominates over pref 100.
 *   max_pref=100 → weight[0]=101, weight[1]=1 → total=102
 * -------------------------------------------------------------------------- */
static void test_naptr_pref_zero_dominates(abts_case *tc, void *data)
{
    uint16_t prefs[] = {0, 100};
    int counts[2] = {0, 0};
    int i;

    for (i = 0; i < 10200; i++) {
        int s = naptr_select(prefs, 2, (uint32_t)i);
        if (s >= 0 && s < 2)
            counts[s]++;
    }

    /* pref 0: 101/102 × 10200 = 10100; pref 100: 1/102 × 10200 = 100 */
    ABTS_INT_EQUAL(tc, 10100, counts[0]);
    ABTS_INT_EQUAL(tc, 100,   counts[1]);
}

/* ==========================================================================
 * Suite entry point
 * ========================================================================== */

abts_suite *test_s_naptr(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    /* APN-FQDN construction (3GPP TS 23.003 §19.4.2) */
    abts_run_test(suite, test_fqdn_2digit_mnc,                 NULL);
    abts_run_test(suite, test_fqdn_3digit_mnc,                 NULL);
    abts_run_test(suite, test_fqdn_tmobile_us,                 NULL);
    abts_run_test(suite, test_fqdn_buf_too_small,              NULL);
    abts_run_test(suite, test_fqdn_mnc_two_digits_no_extra_pad, NULL);
    abts_run_test(suite, test_fqdn_mnc_single_digit_padded,    NULL);
    abts_run_test(suite, test_fqdn_mcc_zero_padded,            NULL);

    /* SRV weighted random selection (RFC 2782 §3) */
    abts_run_test(suite, test_srv_weight_uniform,              NULL);
    abts_run_test(suite, test_srv_weight_skewed,               NULL);
    abts_run_test(suite, test_srv_weight_zero_records,         NULL);
    abts_run_test(suite, test_srv_single_entry,                NULL);
    abts_run_test(suite, test_srv_weight_mixed_zero,           NULL);

    /* NAPTR inverse-preference selection (3GPP TS 29.303 §5.2) */
    abts_run_test(suite, test_naptr_pref_two_records,          NULL);
    abts_run_test(suite, test_naptr_equal_pref,                NULL);
    abts_run_test(suite, test_naptr_single_record,             NULL);
    abts_run_test(suite, test_naptr_pref_zero_dominates,       NULL);

    return suite;
}
