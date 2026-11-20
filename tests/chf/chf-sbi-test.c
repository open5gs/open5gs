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
 * Unit tests for Phase 2 lib/sbi additions.
 *
 * Covers: ogs_sbi_check_chf_info_supi_range() (TS 29.510 §6.1.6.2.9) and
 * slice-restricted CHF selection via NFProfile.allowedNssais
 * (TS 33.518 §4.2.2.2.1).
 *
 * Test IDs: SBI-1 .. SBI-19
 *
 * Design:
 *   - No MongoDB, no NRF, no CHF process needed.
 *   - ogs_sbi_chf_info_t is a plain struct; tests fill it by hand.
 *   - All strings are string literals — no heap allocation required.
 *   - ogs_sbi_check_chf_info_supi_range() only reads the struct fields;
 *     it never calls free(), so using literals is safe.
 *
 * SUPI format used: "imsi-<15 digits>" (TS 23.003 §2.2 / TS 29.510 §6.1.6.2.9)
 * Range semantics:
 *   start/end  — lexicographic range [start, end] (both inclusive)
 *   pattern    — prefix match (strncmp(supi, pattern, strlen(pattern)) == 0)
 *
 * Reference: TS 29.510 §6.1.6.2.9 — ChfInfo / SupiRange
 */

#include "ogs-sbi.h"
#include "chf/notify-path.h"
#include "core/abts.h"

/* -----------------------------------------------------------------------
 * Helper: zero-initialize a chf_info and set num_of_supi_range to zero.
 * ----------------------------------------------------------------------- */
static void init_chf_info(ogs_sbi_chf_info_t *chf)
{
    memset(chf, 0, sizeof(*chf));
}

/* -----------------------------------------------------------------------
 * SBI-1: empty range list → match any SUPI (TS 29.510 — no restriction)
 * ----------------------------------------------------------------------- */
static void test_supi_range_empty_matches_all(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    /* num_of_supi_range == 0 → no restriction → must return true */
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010000000000"));
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-999999999999999"));
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010123456789"));
}

/* -----------------------------------------------------------------------
 * SBI-2: SUPI inside a single start/end range → true
 * ----------------------------------------------------------------------- */
static void test_supi_range_inside(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 1;
    chf.supi_range[0].start   = "imsi-001010000000000";
    chf.supi_range[0].end     = "imsi-001010999999999";
    chf.supi_range[0].pattern = NULL;

    /* Mid-range SUPI */
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010123456789"));
}

/* -----------------------------------------------------------------------
 * SBI-3: SUPI strictly before start → false
 * ----------------------------------------------------------------------- */
static void test_supi_range_before_start(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 1;
    chf.supi_range[0].start   = "imsi-001010100000000";
    chf.supi_range[0].end     = "imsi-001010199999999";
    chf.supi_range[0].pattern = NULL;

    ABTS_TRUE(tc, !ogs_sbi_check_chf_info_supi_range(&chf,
                         "imsi-001010099999999"));
}

/* -----------------------------------------------------------------------
 * SBI-4: SUPI strictly after end → false
 * ----------------------------------------------------------------------- */
static void test_supi_range_after_end(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 1;
    chf.supi_range[0].start   = "imsi-001010100000000";
    chf.supi_range[0].end     = "imsi-001010199999999";
    chf.supi_range[0].pattern = NULL;

    ABTS_TRUE(tc, !ogs_sbi_check_chf_info_supi_range(&chf,
                         "imsi-001010200000000"));
}

/* -----------------------------------------------------------------------
 * SBI-5: SUPI exactly at start boundary → true (inclusive)
 * ----------------------------------------------------------------------- */
static void test_supi_range_at_start(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 1;
    chf.supi_range[0].start   = "imsi-001010000000000";
    chf.supi_range[0].end     = "imsi-001010999999999";
    chf.supi_range[0].pattern = NULL;

    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010000000000"));
}

/* -----------------------------------------------------------------------
 * SBI-6: SUPI exactly at end boundary → true (inclusive)
 * ----------------------------------------------------------------------- */
static void test_supi_range_at_end(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 1;
    chf.supi_range[0].start   = "imsi-001010000000000";
    chf.supi_range[0].end     = "imsi-001010999999999";
    chf.supi_range[0].pattern = NULL;

    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010999999999"));
}

/* -----------------------------------------------------------------------
 * SBI-7: prefix pattern — matching SUPI → true
 * Pattern "imsi-00101" matches any SUPI starting with that prefix.
 * ----------------------------------------------------------------------- */
static void test_supi_range_pattern_match(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 1;
    chf.supi_range[0].start   = NULL;
    chf.supi_range[0].end     = NULL;
    chf.supi_range[0].pattern = "imsi-00101";

    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010123456789"));
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010000000000"));
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001019999999999"));
}

/* -----------------------------------------------------------------------
 * SBI-8: prefix pattern — non-matching SUPI → false
 * ----------------------------------------------------------------------- */
static void test_supi_range_pattern_no_match(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 1;
    chf.supi_range[0].start   = NULL;
    chf.supi_range[0].end     = NULL;
    chf.supi_range[0].pattern = "imsi-00101";

    /* Different MCC/MNC prefix */
    ABTS_TRUE(tc, !ogs_sbi_check_chf_info_supi_range(&chf,
                         "imsi-001020123456789"));
    ABTS_TRUE(tc, !ogs_sbi_check_chf_info_supi_range(&chf,
                         "imsi-999999999999999"));
}

/* -----------------------------------------------------------------------
 * SBI-9: multiple ranges — SUPI in second range → true
 * Range 0: 001010000000000..001010499999999
 * Range 1: 001010500000000..001010999999999
 * ----------------------------------------------------------------------- */
static void test_supi_range_multi_second_range(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 2;

    chf.supi_range[0].start   = "imsi-001010000000000";
    chf.supi_range[0].end     = "imsi-001010499999999";
    chf.supi_range[0].pattern = NULL;

    chf.supi_range[1].start   = "imsi-001010500000000";
    chf.supi_range[1].end     = "imsi-001010999999999";
    chf.supi_range[1].pattern = NULL;

    /* SUPI in second range */
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010750000000"));
    /* SUPI at boundary of second range */
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010500000000"));
    ABTS_TRUE(tc, ogs_sbi_check_chf_info_supi_range(&chf,
                        "imsi-001010999999999"));
}

/* -----------------------------------------------------------------------
 * SBI-10: multiple ranges — SUPI in gap between ranges → false
 *         Also verifies SUPI before all ranges → false.
 * ----------------------------------------------------------------------- */
static void test_supi_range_multi_gap_miss(abts_case *tc, void *data)
{
    ogs_sbi_chf_info_t chf;
    init_chf_info(&chf);
    chf.num_of_supi_range = 2;

    /* Gap: 001010300000000..001010399999999 is not covered */
    chf.supi_range[0].start   = "imsi-001010000000000";
    chf.supi_range[0].end     = "imsi-001010299999999";
    chf.supi_range[0].pattern = NULL;

    chf.supi_range[1].start   = "imsi-001010400000000";
    chf.supi_range[1].end     = "imsi-001010999999999";
    chf.supi_range[1].pattern = NULL;

    /* SUPI in gap */
    ABTS_TRUE(tc, !ogs_sbi_check_chf_info_supi_range(&chf,
                         "imsi-001010350000000"));
    /* SUPI before all ranges */
    ABTS_TRUE(tc, !ogs_sbi_check_chf_info_supi_range(&chf,
                         "imsi-001009999999999"));
    /* SUPI after all ranges */
    ABTS_TRUE(tc, !ogs_sbi_check_chf_info_supi_range(&chf,
                         "imsi-001011000000000"));
}

/* -----------------------------------------------------------------------
 * Slice-restricted CHF selection
 *
 * A CHF that lists charging.slices registers them as
 * NFProfile.allowedNssais, and the NRF then only returns it to a requester
 * whose S-NSSAI is in that set (TS 33.518 §4.2.2.2.1).  This is the
 * NF-profile-level mechanism rather than chfInfo, because TS 29.510's
 * ChfInfo has no S-NSSAI member at all.
 *
 * These exercise ogs_sbi_discovery_option_is_matched() directly, the same
 * function the NRF and the local NF cache both use.
 *
 * Test IDs: SBI-11 .. SBI-16
 * ----------------------------------------------------------------------- */

/* A bare CHF instance: no chfInfo blocks, no slice restriction. */
static void init_chf_instance(ogs_sbi_nf_instance_t *nf)
{
    memset(nf, 0, sizeof(*nf));
    nf->nf_type = OpenAPI_nf_type_CHF;
    ogs_list_init(&nf->nf_info_list);
}

static void add_allowed_slice(ogs_sbi_nf_instance_t *nf,
        uint8_t sst, ogs_uint24_t sd)
{
    nf->allowed_nssai[nf->num_of_allowed_nssai].sst = sst;
    nf->allowed_nssai[nf->num_of_allowed_nssai].sd = sd;
    nf->num_of_allowed_nssai++;
}

static ogs_uint24_t sd_of(uint32_t v)
{
    ogs_uint24_t sd;
    sd.v = v;
    return sd;
}

static void ask_for_slice(ogs_sbi_discovery_option_t *opt,
        uint8_t sst, ogs_uint24_t sd)
{
    opt->snssais[opt->num_of_snssais].sst = sst;
    opt->snssais[opt->num_of_snssais].sd = sd;
    opt->num_of_snssais++;
}

/* SBI-11: a CHF with no slice restriction serves every slice. */
static void test_slice_unrestricted_serves_all(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t nf;
    ogs_sbi_discovery_option_t opt;

    init_chf_instance(&nf);
    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(0x000001));

    /* This is what every chf.yaml written before charging.slices existed
     * gets, so it must keep matching. */
    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt));
}

/* SBI-12: a restricted CHF matches a slice it listed. */
static void test_slice_restricted_match(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t nf;
    ogs_sbi_discovery_option_t opt;

    init_chf_instance(&nf);
    add_allowed_slice(&nf, 1, sd_of(0x000001));
    add_allowed_slice(&nf, 1, sd_of(0x000002));

    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(0x000002));

    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt));
}

/* SBI-13: and does not match one it did not. */
static void test_slice_restricted_no_match(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t nf;
    ogs_sbi_discovery_option_t opt;

    init_chf_instance(&nf);
    add_allowed_slice(&nf, 1, sd_of(0x000001));

    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(0x000002));

    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt) == false);
}

/*
 * SBI-14: matching is exact on SD, not open like a bucket scope.
 *
 * A CHF that lists `sst: 1` with no SD serves the S-NSSAI "SST 1, no SD".
 * It does NOT serve SST 1 / SD 000001.  This is the one place the two
 * slice mechanisms in this work differ, and the trap is worth a test: a
 * quota bucket scoped {sst:1} with no SD matches any SD, because an
 * omitted dimension there is open.
 */
static void test_slice_sd_match_is_exact(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t nf;
    ogs_sbi_discovery_option_t opt;

    init_chf_instance(&nf);
    add_allowed_slice(&nf, 1, sd_of(OGS_S_NSSAI_NO_SD_VALUE));

    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(0x000001));
    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt) == false);

    /* The same CHF does serve a session that carries no SD. */
    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(OGS_S_NSSAI_NO_SD_VALUE));
    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt));
}

/* SBI-15: the filter only applies when the requester asks for a slice. */
static void test_slice_filter_only_when_asked(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t nf;
    ogs_sbi_discovery_option_t opt;

    init_chf_instance(&nf);
    add_allowed_slice(&nf, 1, sd_of(0x000001));

    memset(&opt, 0, sizeof(opt));   /* no snssais requested */

    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt));
}

/*
 * SBI-16: slice and SUPI range AND together.
 *
 * This is what a mixed deployment turns on: a CHF restricted to a slice
 * AND a SUPI range answers only when both hold, which is why an instance
 * meant to serve a SUPI range regardless of slice must leave
 * charging.slices empty.
 */
static void test_slice_and_supi_range_and_together(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t nf;
    ogs_sbi_nf_info_t *nf_info = NULL;
    ogs_sbi_discovery_option_t opt;

    init_chf_instance(&nf);
    add_allowed_slice(&nf, 1, sd_of(0x000001));

    nf_info = ogs_sbi_nf_info_add(&nf.nf_info_list, OpenAPI_nf_type_CHF);
    ABTS_PTR_NOTNULL(tc, nf_info);
    nf_info->chf.supi_range[0].start = (char *)"imsi-001010000000000";
    nf_info->chf.supi_range[0].end   = (char *)"imsi-001010000049999";
    nf_info->chf.num_of_supi_range = 1;

    /* Right slice, SUPI in range → matched. */
    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(0x000001));
    opt.supi = (char *)"imsi-001010000000001";
    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt));

    /* Right slice, SUPI out of range → not matched. */
    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(0x000001));
    opt.supi = (char *)"imsi-001019999999999";
    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt) == false);

    /* Wrong slice, SUPI in range → not matched either. */
    memset(&opt, 0, sizeof(opt));
    ask_for_slice(&opt, 1, sd_of(0x000002));
    opt.supi = (char *)"imsi-001010000000001";
    ABTS_TRUE(tc, ogs_sbi_discovery_option_is_matched(
                &nf, OpenAPI_nf_type_SMF, &opt) == false);

    /* The literals above are not heap strings, so clear them before the
     * list is torn down. */
    nf_info->chf.supi_range[0].start = NULL;
    nf_info->chf.supi_range[0].end = NULL;
    nf_info->chf.num_of_supi_range = 0;
    ogs_sbi_nf_info_remove(&nf.nf_info_list, nf_info);
}

/* -----------------------------------------------------------------------
 * SBI-17: an SBI timer carries a pool id, not a pointer
 *
 * This is the contract src/chf/chf-sm.c's OGS_EVENT_SBI_TIMER handler
 * depends on.  lib/sbi stores OGS_UINT_TO_POINTER(nf_instance->pool_id)
 * in e->h.sbi.data and the handler must look the instance back up;
 * dereferencing the id as a pointer segfaults the CHF on its first
 * heartbeat, which is exactly what happened on the replant onto
 * 310fea94e ("avoid stale pointers in NF timers").
 *
 * The CHF is the one NF upstream does not maintain, so when lib/sbi
 * changes an event contract nothing updates it for us.  Pinning the
 * round-trip here means a future change to it fails a test instead of
 * crashing the daemon in a rig run.
 * ----------------------------------------------------------------------- */
static void test_sbi_timer_carries_pool_id(abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *self_nf = ogs_sbi_self()->nf_instance;
    ogs_sbi_nf_instance_t *found = NULL;
    void *as_timer_data = NULL;

    ABTS_PTR_NOTNULL(tc, self_nf);
    if (!self_nf)
        return;

    /* A pool id is what the timer carries, exactly as lib/sbi/nf-sm.c
     * stores it. */
    as_timer_data = OGS_UINT_TO_POINTER(self_nf->pool_id);

    found = ogs_sbi_nf_instance_find_by_pool_id(
            OGS_POINTER_TO_UINT(as_timer_data));
    ABTS_PTR_EQUAL(tc, self_nf, found);

    /* And an id that was never allocated resolves to nothing rather than
     * to a wild pointer, which is what lets the handler drop the event
     * instead of asserting. */
    found = ogs_sbi_nf_instance_find_by_pool_id((ogs_pool_id_t)0);
    ABTS_PTR_EQUAL(tc, NULL, found);
}

/* -----------------------------------------------------------------------
 * Suite registration
 * ----------------------------------------------------------------------- */
/* -------------------------------------------------------------------
 * SBI-18: which DNNs are well known
 *
 * The CHF and the SMF both ask this, and each answer exempts a session from
 * quota, so a near miss matters as much as a hit.  The Network Identifier
 * alone decides (TS 23.003 clause 9.1): case does not matter, and neither
 * does an Operator Identifier, which is how a roaming session's DNN can look.
 * ------------------------------------------------------------------- */
static void test_dnn_well_known(abts_case *tc, void *data)
{
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("ims"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("sos"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("mcptt"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("mcx"));

    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("IMS"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("Sos"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("ims.mnc070.mcc999.gprs"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("SOS.mnc070.mcc999.gprs"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("MCX"));
    ABTS_TRUE(tc, ogs_sbi_dnn_is_well_known("mcx.mnc070.mcc999.gprs"));

    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("internet"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("imsx"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("im"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("mcpttx.mnc070.mcc999.gprs"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("mcxx"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("mc"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("mcxdata.mnc070.mcc999.gprs"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known("internet.ims"));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known(""));
    ABTS_TRUE(tc, !ogs_sbi_dnn_is_well_known(NULL));
}

/* -------------------------------------------------------------------
 * SBI-19: how long a ChargingNotify queue row outlives its first reader
 *
 * Several CHFs share chf.notifications, and each reads every row once, so a
 * row may only be deleted after all of them have polled.  The first CHF to
 * read it starts the clock, and whichever polls first after that deletes it.
 * Five minutes covers sixty polls at the shipped five seconds; a CHF set to
 * poll more slowly keeps rows for three of its own intervals instead, and
 * the arithmetic must not wrap for an absurd setting.
 * ------------------------------------------------------------------- */
static void test_notify_keep_outlives_every_poll(abts_case *tc, void *data)
{
    ABTS_INT_EQUAL(tc, 300, (int)chf_notify_keep_seconds(1));
    ABTS_INT_EQUAL(tc, 300, (int)chf_notify_keep_seconds(5));
    ABTS_INT_EQUAL(tc, 300, (int)chf_notify_keep_seconds(100));
    ABTS_INT_EQUAL(tc, 303, (int)chf_notify_keep_seconds(101));
    ABTS_INT_EQUAL(tc, 1800, (int)chf_notify_keep_seconds(600));
    ABTS_TRUE(tc, chf_notify_keep_seconds(UINT32_MAX) == UINT32_MAX);
    ABTS_TRUE(tc, chf_notify_keep_seconds(UINT32_MAX / 3 + 1) == UINT32_MAX);
}

abts_suite *test_chf_sbi(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_supi_range_empty_matches_all, NULL);
    abts_run_test(suite, test_supi_range_inside,            NULL);
    abts_run_test(suite, test_supi_range_before_start,      NULL);
    abts_run_test(suite, test_supi_range_after_end,         NULL);
    abts_run_test(suite, test_supi_range_at_start,          NULL);
    abts_run_test(suite, test_supi_range_at_end,            NULL);
    abts_run_test(suite, test_supi_range_pattern_match,     NULL);
    abts_run_test(suite, test_supi_range_pattern_no_match,  NULL);
    abts_run_test(suite, test_supi_range_multi_second_range, NULL);
    abts_run_test(suite, test_supi_range_multi_gap_miss,    NULL);

    abts_run_test(suite, test_slice_unrestricted_serves_all, NULL);
    abts_run_test(suite, test_slice_restricted_match,        NULL);
    abts_run_test(suite, test_slice_restricted_no_match,     NULL);
    abts_run_test(suite, test_slice_sd_match_is_exact,       NULL);
    abts_run_test(suite, test_slice_filter_only_when_asked,  NULL);
    abts_run_test(suite, test_slice_and_supi_range_and_together, NULL);
    abts_run_test(suite, test_sbi_timer_carries_pool_id, NULL);
    abts_run_test(suite, test_dnn_well_known, NULL);
    abts_run_test(suite, test_notify_keep_outlives_every_poll, NULL);

    return suite;
}
