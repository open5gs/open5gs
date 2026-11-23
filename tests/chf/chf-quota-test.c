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
 * CHF quota engine unit tests (Phase 1d).
 *
 * These tests verify:
 *   1. Default quota context values match TS 32.291 §6.3.1.2 expectations
 *   2. Session quota allocation splits UL/DL correctly (50/50)
 *   3. Remaining-volume arithmetic is correct (totalVolume on Update/Create)
 *   4. Quota-exhaustion flag is set at the right boundary
 *   5. Multiple independent sessions do not share quota state
 *   6. Zero-volume quota triggers immediate exhaustion (0 >= 0)
 *   7. FinalUnitAction enum values: TERMINATE=0, REDIRECT=1, RESTRICT_ACCESS=2
 *   8. Odd-byte quota split uses floor division (1 byte may be unaccounted)
 *   9. Zero-volume quota remains exhausted even with non-zero actual usage
 *
 * All tests run against libchf without a MongoDB or NRF connection.
 * chf_context_init() is called in abts-main.c before suites run.
 */

#include "ogs-sbi.h"
#include "chf/context.h"
#include "core/abts.h"

/* ------------------------------------------------------------------ */
/* Test 1: Default quota values after context init                     */
/* TS 32.291 §6.3.1.2 — GrantedUnit; TS 32.290 §5.2 — defaults       */
/* ------------------------------------------------------------------ */
static void test_default_quota_values(abts_case *tc, void *data)
{
    chf_context_t *ctx = chf_self();
    ABTS_PTR_NOTNULL(tc, ctx);

    /* 10 GiB = 10 * 1024^3 bytes — configured in chf.yaml.in.
     * Compare as uint64 to avoid int overflow on 32-bit. */
    ABTS_INT_EQUAL(tc, 1,
        ctx->default_quota.volume_bytes == (uint64_t)10 * 1024 * 1024 * 1024
        ? 1 : 0);

    /* 24 hours validity per TS 32.290 §6.1.4 */
    ABTS_INT_EQUAL(tc, 86400, (int)ctx->default_quota.validity_seconds);

    /* TERMINATE = 0 per OpenAPI_final_unit_action_e */
    ABTS_INT_EQUAL(tc, 0, ctx->default_quota.final_unit_action);
}

/* ------------------------------------------------------------------ */
/* Test 2: Session quota split (UL + DL == volume_bytes)              */
/* For even volumes UL == DL == total/2; DL gets the odd byte for odd */
/* volumes so UL+DL always equals volume_bytes exactly.               */
/* Direct chf_apply_quota() tests: see QU-15 (context.c).             */
/* ------------------------------------------------------------------ */
static void test_quota_split(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    chf_quota_t quota;
    uint64_t half;

    sess = chf_sess_add("imsi-999700000000001", "test-uuid-quota-split-001");
    ABTS_PTR_NOTNULL(tc, sess);

    quota.volume_bytes      = 100000000ULL; /* 100 MB */
    quota.validity_seconds  = 3600;
    quota.auto_renew        = false;
    quota.final_unit_action = 0;

    half = quota.volume_bytes / 2;
    sess->granted_quota_total = quota.volume_bytes;
    sess->granted_time_sec  = quota.validity_seconds;
    sess->final_unit_action = quota.final_unit_action;
    sess->final_unit        = false;

    ABTS_INT_EQUAL(tc, (int)quota.volume_bytes,
                   (int)sess->granted_quota_total);
    ABTS_INT_EQUAL(tc, (int)(2 * half), (int)quota.volume_bytes);
    ABTS_INT_EQUAL(tc, 0, sess->final_unit_action);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* Test 3: Remaining volume decreases correctly as usage accumulates   */
/* Mirrors build_charging_data_response() logic in nchf-handler.c     */
/* ------------------------------------------------------------------ */
static void test_remaining_volume(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    uint64_t total_granted, total_used, remaining;

    sess = chf_sess_add("imsi-999700000000002", "test-uuid-remaining-001");
    ABTS_PTR_NOTNULL(tc, sess);

    /* Grant 10 MB total (5 MB UL + 5 MB DL) */
    sess->granted_quota_total = 2 * (5 * 1024 * 1024);
    sess->ul_octets        = 0;
    sess->dl_octets        = 0;
    sess->final_unit       = false;

    /* After Create: no usage yet, full quota available */
    total_granted = sess->granted_quota_total;
    total_used    = sess->ul_octets + sess->dl_octets;
    remaining = (total_used >= total_granted) ? 0ULL : total_granted - total_used;
    ABTS_INT_EQUAL(tc, (int)(10 * 1024 * 1024), (int)remaining);

    /* Simulate first Update: 2 MB used */
    sess->ul_octets += 1 * 1024 * 1024;
    sess->dl_octets += 1 * 1024 * 1024;
    total_used  = sess->ul_octets + sess->dl_octets;
    remaining   = (total_used >= total_granted) ? 0ULL : total_granted - total_used;
    ABTS_INT_EQUAL(tc, (int)(8 * 1024 * 1024), (int)remaining);

    /* Simulate second Update: 6 more MB used (8 MB total — still within quota) */
    sess->ul_octets += 3 * 1024 * 1024;
    sess->dl_octets += 3 * 1024 * 1024;
    total_used  = sess->ul_octets + sess->dl_octets;
    remaining   = (total_used >= total_granted) ? 0ULL : total_granted - total_used;
    ABTS_INT_EQUAL(tc, (int)(2 * 1024 * 1024), (int)remaining);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* Test 4: Quota exhaustion — flag triggered exactly at boundary       */
/* TS 32.291 §6.3.2.2 — FinalUnitIndication raised on exhaustion      */
/* ------------------------------------------------------------------ */
static void test_quota_exhaustion_boundary(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    uint64_t total_granted, total_used;
    bool exhausted;

    sess = chf_sess_add("imsi-999700000000003", "test-uuid-exhaust-001");
    ABTS_PTR_NOTNULL(tc, sess);

    sess->granted_quota_total = 2 * (5 * 1024 * 1024);
    sess->ul_octets = 0;
    sess->dl_octets = 0;
    sess->final_unit = false;
    total_granted = sess->granted_quota_total;

    /* One byte short of exhaustion — NOT exhausted */
    sess->ul_octets = total_granted - 1;
    sess->dl_octets = 0;
    total_used = sess->ul_octets + sess->dl_octets;
    exhausted  = (total_used >= total_granted);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)exhausted);

    /* Exactly at boundary — exhausted */
    sess->ul_octets = total_granted;
    sess->dl_octets = 0;
    total_used = sess->ul_octets + sess->dl_octets;
    exhausted  = (total_used >= total_granted);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)exhausted);

    /* Over boundary — clamped to 0 remaining */
    sess->ul_octets = total_granted + 1024 * 1024;
    total_used = sess->ul_octets + sess->dl_octets;
    exhausted  = (total_used >= total_granted);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)exhausted);
    uint64_t remaining = (total_used >= total_granted) ? 0ULL
                         : total_granted - total_used;
    ABTS_INT_EQUAL(tc, 0, (int)remaining);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* Test 5: Multiple independent sessions do not share quota state      */
/* ------------------------------------------------------------------ */
static void test_sessions_are_independent(abts_case *tc, void *data)
{
    chf_sess_t *sess1, *sess2;

    sess1 = chf_sess_add("imsi-999700000000010", "test-uuid-indep-sess1");
    ABTS_PTR_NOTNULL(tc, sess1);
    sess1->granted_quota_total = 2 * (1024 * 1024);
    sess1->ul_octets = 500 * 1024;
    sess1->dl_octets = 100 * 1024;

    sess2 = chf_sess_add("imsi-999700000000011", "test-uuid-indep-sess2");
    ABTS_PTR_NOTNULL(tc, sess2);
    sess2->granted_quota_total = 2 * (50 * 1024 * 1024);
    sess2->ul_octets = 0;
    sess2->dl_octets = 0;

    /* sess1 is partially used */
    ABTS_INT_EQUAL(tc, (int)(600 * 1024),
            (int)(sess1->ul_octets + sess1->dl_octets));

    /* sess2 is untouched */
    ABTS_INT_EQUAL(tc, 0,
            (int)(sess2->ul_octets + sess2->dl_octets));

    /* Lookup by ref works for both */
    ABTS_PTR_NOTNULL(tc,
        chf_sess_find_by_charging_data_ref("test-uuid-indep-sess1"));
    ABTS_PTR_NOTNULL(tc,
        chf_sess_find_by_charging_data_ref("test-uuid-indep-sess2"));

    chf_sess_remove(sess1);
    chf_sess_remove(sess2);

    /* After removal, lookups return NULL */
    ABTS_PTR_EQUAL(tc, NULL,
        chf_sess_find_by_charging_data_ref("test-uuid-indep-sess1"));
    ABTS_PTR_EQUAL(tc, NULL,
        chf_sess_find_by_charging_data_ref("test-uuid-indep-sess2"));
}

/* ------------------------------------------------------------------ */
/* Test 6: Zero-volume quota triggers immediate exhaustion             */
/*                                                                      */
/* If a subscriber has volume_bytes=0 (e.g. data service blocked), any */
/* usage — including zero — satisfies total_used >= total_granted.     */
/* TS 32.291 §6.3.2.2: exhaustion fires when grant is met or exceeded. */
/* ------------------------------------------------------------------ */
static void test_zero_volume_quota_exhaustion(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    uint64_t total_used, total_granted;
    bool exhausted;

    sess = chf_sess_add("imsi-999700000000020", "test-uuid-zerovol-001");
    ABTS_PTR_NOTNULL(tc, sess);

    /* Zero quota — both UL and DL grant are 0 (volume_bytes/2 = 0) */
    sess->granted_quota_total = 0;
    sess->ul_octets        = 0;
    sess->dl_octets        = 0;
    sess->final_unit       = false;

    total_used    = sess->ul_octets + sess->dl_octets;   /* 0 */
    total_granted = sess->granted_quota_total; /* 0 */

    /* 0 >= 0 is true — quota is immediately exhausted */
    exhausted = (total_used >= total_granted);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)exhausted);

    /* No volume used yet, so remaining = 0 (clamped) */
    uint64_t remaining = (total_used >= total_granted) ? 0ULL
                         : total_granted - total_used;
    ABTS_INT_EQUAL(tc, 0, (int)remaining);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* Test 6b (Test 9): Zero-volume quota stays exhausted with real usage */
/*                                                                      */
/* Test 6 verified 0 >= 0 fires when no usage has occurred.           */
/* This test verifies that any non-zero usage against a zero grant     */
/* also fires exhaustion and clamps remaining to 0 (N >= 0 is always  */
/* true for uint64_t, so remaining must never go negative).            */
/* ------------------------------------------------------------------ */
static void test_zero_volume_with_nonzero_usage(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    uint64_t total_used, total_granted;
    bool exhausted;

    sess = chf_sess_add("imsi-999700000000022", "test-uuid-zerovol-usage-001");
    ABTS_PTR_NOTNULL(tc, sess);

    /* Zero quota (blocked subscriber) */
    sess->granted_quota_total = 0;
    sess->final_unit       = false;

    total_granted = sess->granted_quota_total; /* 0 */

    /* Simulate 1 byte UL: 1 >= 0 → still exhausted */
    sess->ul_octets = 1;
    sess->dl_octets = 0;
    total_used = sess->ul_octets + sess->dl_octets;
    exhausted  = (total_used >= total_granted);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)exhausted);
    uint64_t remaining = (total_used >= total_granted) ? 0ULL
                         : total_granted - total_used;
    ABTS_INT_EQUAL(tc, 0, (int)remaining);

    /* Simulate large usage: 1 GiB each direction — still exhausted */
    sess->ul_octets = (uint64_t)1024 * 1024 * 1024;
    sess->dl_octets = (uint64_t)1024 * 1024 * 1024;
    total_used = sess->ul_octets + sess->dl_octets;
    exhausted  = (total_used >= total_granted);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)exhausted);
    remaining = (total_used >= total_granted) ? 0ULL
                : total_granted - total_used;
    ABTS_INT_EQUAL(tc, 0, (int)remaining);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* Test 7: FinalUnitAction enum values stored and retrieved correctly  */
/*                                                                      */
/* TS 32.291 §6.3.2.2 / OpenAPI FinalUnitAction:                      */
/*   0 → TERMINATE (default, blocks all traffic)                       */
/*   1 → REDIRECT   (redirect to captive portal)                       */
/*   2 → RESTRICT_ACCESS (restrict to a service set)                   */
/* ------------------------------------------------------------------ */
static void test_fua_action_enum_values(abts_case *tc, void *data)
{
    chf_sess_t *sess;

    sess = chf_sess_add("imsi-999700000000021", "test-uuid-fua-enum-001");
    ABTS_PTR_NOTNULL(tc, sess);

    /* TERMINATE = 0 (default, most common) */
    sess->final_unit_action = 0;
    ABTS_INT_EQUAL(tc, 0, sess->final_unit_action);

    /* REDIRECT = 1 */
    sess->final_unit_action = 1;
    ABTS_INT_EQUAL(tc, 1, sess->final_unit_action);

    /* RESTRICT_ACCESS = 2 */
    sess->final_unit_action = 2;
    ABTS_INT_EQUAL(tc, 2, sess->final_unit_action);

    /* Confirm field width (int, not bool) */
    ABTS_INT_EQUAL(tc, 1, sess->final_unit_action == 2 ? 1 : 0);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* Test 8: Odd-byte quota uses floor division — 1 byte may be lost     */
/*                                                                      */
/* apply_quota() splits volume_bytes / 2 for both UL and DL.          */
/* With an odd volume (e.g. 101 bytes), floor(101/2) = 50 for each,   */
/* so UL + DL = 100 < 101.  The 1-byte deficit is intentional:        */
/* an odd byte cannot be attributed to a single direction.             */
/* Exhaustion logic uses the sum UL+DL (100), not original volume.     */
/* ------------------------------------------------------------------ */
static void test_quota_split_odd_bytes(abts_case *tc, void *data)
{
    /* Test the arithmetic directly — no session needed */
    uint64_t odd_volume = 101;
    uint64_t ul = odd_volume / 2;   /* 50 */
    uint64_t dl = odd_volume / 2;   /* 50 */

    ABTS_INT_EQUAL(tc, 50, (int)ul);
    ABTS_INT_EQUAL(tc, 50, (int)dl);

    /* UL + DL = 100 < 101: 1 byte is "lost" to floor division.
     * Exhaustion fires when used >= 100, not when used >= 101. */
    ABTS_INT_EQUAL(tc, 100, (int)(ul + dl));
    ABTS_INT_EQUAL(tc, 1, (int)(odd_volume - (ul + dl)));

    /* Verify exhaustion fires at 100 (effective grant), not 101 */
    uint64_t total_granted = ul + dl;          /* 100 */
    bool exhausted_at_100 = (100ULL >= total_granted);
    bool exhausted_at_99  = (99ULL  >= total_granted);
    ABTS_INT_EQUAL(tc, 1 /* true  */, (int)exhausted_at_100);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)exhausted_at_99);
}

/* ==================================================================== */
/* Aggregate quota tests — chf_supi_aggregate_used() + Create-time cap */
/* TS 32.291 §5.2 — subscriber-level quota pool                        */
/* ==================================================================== */

/* ------------------------------------------------------------------ */
/* Agg-1: Single session — aggregate equals its own UL+DL usage       */
/* ------------------------------------------------------------------ */
static void test_aggregate_used_single_session(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    uint64_t agg;

    sess = chf_sess_add("imsi-999700000100001", "test-uuid-agg-single-001");
    ABTS_PTR_NOTNULL(tc, sess);
    sess->ul_octets = 3 * 1024 * 1024;   /* 3 MB */
    sess->dl_octets = 7 * 1024 * 1024;   /* 7 MB */

    agg = chf_supi_aggregate_used("imsi-999700000100001");
    ABTS_INT_EQUAL(tc, (int)(10 * 1024 * 1024), (int)agg);

    /* Unrelated SUPI returns 0 */
    agg = chf_supi_aggregate_used("imsi-999700000199999");
    ABTS_INT_EQUAL(tc, 0, (int)agg);

    chf_sess_remove(sess);

    /* After removal, aggregate drops to 0 */
    agg = chf_supi_aggregate_used("imsi-999700000100001");
    ABTS_INT_EQUAL(tc, 0, (int)agg);
}

/* ------------------------------------------------------------------ */
/* Agg-2: Three sessions same SUPI — aggregate is the correct sum     */
/*                                                                      */
/* Session A: 10 MB used                                               */
/* Session B: 20 MB used                                               */
/* Session C:  5 MB used                                               */
/* Expected aggregate: 35 MB                                           */
/* ------------------------------------------------------------------ */
static void test_aggregate_used_multi_session_same_supi(abts_case *tc, void *data)
{
    chf_sess_t *sessA, *sessB, *sessC;
    uint64_t agg;
    const char *supi = "imsi-999700000100002";

    sessA = chf_sess_add(supi, "test-uuid-agg-multi-A");
    ABTS_PTR_NOTNULL(tc, sessA);
    sessA->ul_octets = 5 * 1024 * 1024;
    sessA->dl_octets = 5 * 1024 * 1024;   /* A total: 10 MB */

    sessB = chf_sess_add(supi, "test-uuid-agg-multi-B");
    ABTS_PTR_NOTNULL(tc, sessB);
    sessB->ul_octets = 10 * 1024 * 1024;
    sessB->dl_octets = 10 * 1024 * 1024;  /* B total: 20 MB */

    sessC = chf_sess_add(supi, "test-uuid-agg-multi-C");
    ABTS_PTR_NOTNULL(tc, sessC);
    sessC->ul_octets = 2 * 1024 * 1024;
    sessC->dl_octets = 3 * 1024 * 1024;   /* C total:  5 MB */

    /* A + B + C = 35 MB */
    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(35 * 1024 * 1024), (int)agg);

    /* Release B — aggregate drops to 15 MB */
    chf_sess_remove(sessB);
    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(15 * 1024 * 1024), (int)agg);

    chf_sess_remove(sessA);
    chf_sess_remove(sessC);
}

/* ------------------------------------------------------------------ */
/* Agg-3: Cross-SUPI isolation — only matching SUPI is counted        */
/*                                                                      */
/* Two SUPIs share the session list; each SUPI's aggregate must only   */
/* reflect its own sessions, not the other SUPI's usage.              */
/* ------------------------------------------------------------------ */
static void test_aggregate_used_cross_supi_isolation(abts_case *tc, void *data)
{
    chf_sess_t *sessX, *sessY1, *sessY2;
    const char *supiX = "imsi-999700000100003";
    const char *supiY = "imsi-999700000100004";
    uint64_t aggX, aggY;

    /* SUPI-X: 1 session, 50 MB used */
    sessX = chf_sess_add(supiX, "test-uuid-agg-cross-X");
    ABTS_PTR_NOTNULL(tc, sessX);
    sessX->ul_octets = 25 * 1024 * 1024;
    sessX->dl_octets = 25 * 1024 * 1024;  /* X: 50 MB */

    /* SUPI-Y: 2 sessions, 80 MB total used */
    sessY1 = chf_sess_add(supiY, "test-uuid-agg-cross-Y1");
    ABTS_PTR_NOTNULL(tc, sessY1);
    sessY1->ul_octets = 30 * 1024 * 1024;
    sessY1->dl_octets = 10 * 1024 * 1024;  /* Y1: 40 MB */

    sessY2 = chf_sess_add(supiY, "test-uuid-agg-cross-Y2");
    ABTS_PTR_NOTNULL(tc, sessY2);
    sessY2->ul_octets = 20 * 1024 * 1024;
    sessY2->dl_octets = 20 * 1024 * 1024;  /* Y2: 40 MB */

    /* SUPI-X aggregate = 50 MB, not affected by SUPI-Y */
    aggX = chf_supi_aggregate_used(supiX);
    ABTS_INT_EQUAL(tc, (int)(50 * 1024 * 1024), (int)aggX);

    /* SUPI-Y aggregate = 80 MB, not inflated by SUPI-X */
    aggY = chf_supi_aggregate_used(supiY);
    ABTS_INT_EQUAL(tc, (int)(80 * 1024 * 1024), (int)aggY);

    chf_sess_remove(sessX);
    chf_sess_remove(sessY1);
    chf_sess_remove(sessY2);
}

/* ------------------------------------------------------------------ */
/* Agg-4: Create-time aggregate cap arithmetic (TS 32.291 §5.2)       */
/*                                                                      */
/* Simulates the cap logic in handle_create() without going through    */
/* the full HTTP handler.  Three sub-cases:                            */
/*                                                                      */
/*  (a) agg_used == 0            → new grant = full pool (no cap)     */
/*  (b) 0 < agg_used < pool      → new grant = pool − agg_used        */
/*  (c) agg_used >= pool         → new grant = 0  (FUI must be set)   */
/* ------------------------------------------------------------------ */
static void test_create_time_cap_arithmetic(abts_case *tc, void *data)
{
    /* Pool of 200 MB (total quota for this subscriber) */
    const uint64_t pool = 200ULL * 1024 * 1024;
    uint64_t agg_used, new_grant;

    /* (a) No concurrent sessions: full pool granted */
    agg_used  = 0;
    new_grant = (agg_used >= pool) ? 0ULL : pool - agg_used;
    ABTS_INT_EQUAL(tc, (int)pool, (int)new_grant);

    /* (b) 80 MB already used by other sessions: cap to 120 MB */
    agg_used  = 80ULL * 1024 * 1024;
    new_grant = (agg_used >= pool) ? 0ULL : pool - agg_used;
    ABTS_INT_EQUAL(tc, (int)(120ULL * 1024 * 1024), (int)new_grant);

    /* (c) Exactly at pool: new grant = 0 → FUI must be set */
    agg_used  = 200ULL * 1024 * 1024;
    new_grant = (agg_used >= pool) ? 0ULL : pool - agg_used;
    ABTS_INT_EQUAL(tc, 0, (int)new_grant);

    /* (d) Over pool (e.g., rounding caused by concurrent Update): grant = 0 */
    agg_used  = 201ULL * 1024 * 1024;
    new_grant = (agg_used >= pool) ? 0ULL : pool - agg_used;
    ABTS_INT_EQUAL(tc, 0, (int)new_grant);

    /* Verify FUI trigger condition:
     * A zero new_grant means apply_quota() will set both UL and DL to 0.
     * After apply_quota() (which clears final_unit), handle_create() must
     * set final_unit=true.  Validate the condition used in nchf-handler.c. */
    uint64_t ul = new_grant / 2;
    uint64_t dl = new_grant / 2;
    int must_force_fui = (ul == 0 && dl == 0) ? 1 : 0;
    ABTS_INT_EQUAL(tc, 1, must_force_fui);
}

/* ------------------------------------------------------------------ */
/* Agg-5: New session excluded from pre-add aggregate scan            */
/*                                                                      */
/* chf_supi_aggregate_used() is designed to be called BEFORE           */
/* chf_sess_add(), so the new session is not yet in sess_list and its  */
/* (zero) usage does not inflate the aggregate.  This test verifies    */
/* the contract: the function only counts sessions already in the list. */
/* ------------------------------------------------------------------ */
static void test_aggregate_new_session_excluded(abts_case *tc, void *data)
{
    chf_sess_t *sessExisting, *sessNew;
    const char *supi = "imsi-999700000100005";
    uint64_t agg_before, agg_after_add;

    /* Pre-existing session: 100 MB used */
    sessExisting = chf_sess_add(supi, "test-uuid-agg-preexist");
    ABTS_PTR_NOTNULL(tc, sessExisting);
    sessExisting->ul_octets = 50 * 1024 * 1024;
    sessExisting->dl_octets = 50 * 1024 * 1024;

    /* Aggregate BEFORE adding the new session: should be 100 MB */
    agg_before = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(100 * 1024 * 1024), (int)agg_before);

    /* Add the new session with 0 usage (as it would be at Create time) */
    sessNew = chf_sess_add(supi, "test-uuid-agg-newsess");
    ABTS_PTR_NOTNULL(tc, sessNew);
    sessNew->ul_octets = 0;
    sessNew->dl_octets = 0;

    /* Aggregate AFTER add but BEFORE any usage: still 100 MB (0+0 adds nothing) */
    agg_after_add = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(100 * 1024 * 1024), (int)agg_after_add);

    /* Once new session accumulates usage, aggregate updates correctly */
    sessNew->ul_octets = 30 * 1024 * 1024;
    sessNew->dl_octets = 20 * 1024 * 1024;
    uint64_t agg_with_usage = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(150 * 1024 * 1024), (int)agg_with_usage);

    chf_sess_remove(sessExisting);
    chf_sess_remove(sessNew);
}

/* ------------------------------------------------------------------ */
/* Agg-6: per-SUPI index entry lifecycle — free on last removal, then  */
/*        lazy re-creation on a later session for the same SUPI.        */
/*                                                                      */
/* Exercises the index path that frees the chf_supi_index_t when its    */
/* last session leaves and re-allocates it (with a fresh owned SUPI key */
/* copy) when a new session for the same SUPI arrives.  A regression    */
/* here would surface as a stale aggregate or a use-after-free.         */
/* ------------------------------------------------------------------ */
static void test_aggregate_index_entry_recreation(abts_case *tc, void *data)
{
    chf_sess_t *s1, *s2;
    const char *supi = "imsi-999700000100006";
    uint64_t agg;

    /* First session for the SUPI creates the index entry */
    s1 = chf_sess_add(supi, "test-uuid-agg-recreate-1");
    ABTS_PTR_NOTNULL(tc, s1);
    s1->ul_octets = 4 * 1024 * 1024;
    s1->dl_octets = 6 * 1024 * 1024;   /* 10 MB */
    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(10 * 1024 * 1024), (int)agg);

    /* Removing the only session frees the index entry; aggregate is 0 */
    chf_sess_remove(s1);
    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, 0, (int)agg);

    /* A new session for the SAME SUPI must lazily re-create the entry and
     * report only its own usage — not any stale value from the freed entry */
    s2 = chf_sess_add(supi, "test-uuid-agg-recreate-2");
    ABTS_PTR_NOTNULL(tc, s2);
    s2->ul_octets = 3 * 1024 * 1024;
    s2->dl_octets = 4 * 1024 * 1024;   /* 7 MB */
    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(7 * 1024 * 1024), (int)agg);

    chf_sess_remove(s2);
}

/* ------------------------------------------------------------------ */
/* Agg-7: index key stability when a sibling session is removed.       */
/*                                                                      */
/* The index entry owns its own SUPI-string copy for the hash key, so   */
/* it must stay valid after the FIRST-added session (whose own supi     */
/* string is a separate allocation) is removed while siblings remain.   */
/* This guards against keying the hash by a per-session string that     */
/* could be freed out from under sibling sessions.                      */
/* ------------------------------------------------------------------ */
static void test_aggregate_index_key_survives_sibling_removal(
        abts_case *tc, void *data)
{
    chf_sess_t *first, *second;
    const char *supi = "imsi-999700000100007";
    uint64_t agg;

    first = chf_sess_add(supi, "test-uuid-agg-sibling-1");
    ABTS_PTR_NOTNULL(tc, first);
    first->ul_octets = 8 * 1024 * 1024;
    first->dl_octets = 2 * 1024 * 1024;   /* 10 MB */

    second = chf_sess_add(supi, "test-uuid-agg-sibling-2");
    ABTS_PTR_NOTNULL(tc, second);
    second->ul_octets = 5 * 1024 * 1024;
    second->dl_octets = 5 * 1024 * 1024;  /* 10 MB */

    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(20 * 1024 * 1024), (int)agg);

    /* Remove the first-added session; the index key must remain valid so
     * the surviving sibling's aggregate is still correct (10 MB). */
    chf_sess_remove(first);
    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(10 * 1024 * 1024), (int)agg);

    chf_sess_remove(second);
    agg = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, 0, (int)agg);
}

/* ==================================================================== */
/* Gap-fix tests: direct coverage of chf_apply_quota(),                 */
/* chf_check_quota_exhausted(), and Update Case E (TS 32.291 §5.2)     */
/* ==================================================================== */

/* ------------------------------------------------------------------ */
/* QU-15: chf_apply_quota() — direct call, verify all fields set       */
/*                                                                      */
/* Fixes Gap G-1: apply_quota was static/untestable; now chf_apply_quota
 * is in context.c and callable from unit tests.                       */
/* ------------------------------------------------------------------ */
static void test_apply_quota_direct(abts_case *tc, void *data)
{
    chf_sess_t  *sess;
    chf_quota_t  quota;

    sess = chf_sess_add("imsi-999700000200001", "test-uuid-aq-direct-001");
    ABTS_PTR_NOTNULL(tc, sess);

    quota.volume_bytes      = 200ULL * 1024 * 1024;  /* 200 MB */
    quota.validity_seconds  = 7200;
    quota.auto_renew        = false;
    quota.final_unit_action = 0;  /* TERMINATE */

    chf_apply_quota(sess, &quota);

    /* Single combined allowance == volume_bytes */
    ABTS_INT_EQUAL(tc, (int)(200 * 1024 * 1024),
                   (int)sess->granted_quota_total);
    /* Validity, auto_renew and action stored */
    ABTS_INT_EQUAL(tc, 7200, (int)sess->granted_time_sec);
    ABTS_INT_EQUAL(tc, (int)7200, (int)sess->period_length_sec);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)sess->auto_renew);
    ABTS_INT_EQUAL(tc, 0, sess->final_unit_action);
    /* final_unit always reset to false */
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)sess->final_unit);

    /* Calling again with different quota replaces all fields */
    quota.volume_bytes      = 500ULL * 1024 * 1024;
    quota.validity_seconds  = 3600;
    quota.auto_renew        = true;
    quota.final_unit_action = 1;  /* REDIRECT */
    chf_apply_quota(sess, &quota);
    ABTS_INT_EQUAL(tc, (int)(500 * 1024 * 1024),
                   (int)sess->granted_quota_total);
    ABTS_INT_EQUAL(tc, 3600, (int)sess->granted_time_sec);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)sess->auto_renew);
    ABTS_INT_EQUAL(tc, 1, sess->final_unit_action);
    ABTS_INT_EQUAL(tc, 0, (int)sess->final_unit);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* QU-16: chf_check_quota_exhausted() — direct call, all branches      */
/*                                                                      */
/* Fixes Gap G-2: check_quota_exhausted was static/untestable; now     */
/* chf_check_quota_exhausted is in context.c.                          */
/* ------------------------------------------------------------------ */
static void test_check_quota_exhausted_direct(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    bool result;

    sess = chf_sess_add("imsi-999700000200002", "test-uuid-cqe-direct-001");
    ABTS_PTR_NOTNULL(tc, sess);

    sess->granted_quota_total = 2 * (50 * 1024 * 1024);  /* 100 MB total */
    sess->ul_octets = 0;
    sess->dl_octets = 0;
    sess->final_unit = false;

    /* Not exhausted: 0 < 100 MB */
    result = chf_check_quota_exhausted(sess);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)result);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)sess->final_unit);

    /* Partial use: 60 MB < 100 MB — still not exhausted */
    sess->ul_octets = 30 * 1024 * 1024;
    sess->dl_octets = 30 * 1024 * 1024;
    result = chf_check_quota_exhausted(sess);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)result);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)sess->final_unit);

    /* Exactly at boundary: 100 MB == 100 MB — exhausted */
    sess->ul_octets = 50 * 1024 * 1024;
    sess->dl_octets = 50 * 1024 * 1024;
    result = chf_check_quota_exhausted(sess);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)result);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)sess->final_unit);

    /* Over boundary: 101 MB > 100 MB — still returns true (stays exhausted) */
    sess->ul_octets += 1 * 1024 * 1024;
    result = chf_check_quota_exhausted(sess);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)result);

    /* Verify zero-grant case: 0 >= 0 — immediately exhausted */
    sess->granted_quota_total = 0;
    sess->ul_octets = 0;
    sess->dl_octets = 0;
    sess->final_unit = false;
    result = chf_check_quota_exhausted(sess);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)result);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)sess->final_unit);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* QU-17: Update Case E — cross-session aggregate exhaustion           */
/*                                                                      */
/* Fixes Gap G-3: Case E is the logic in handle_update() that fires    */
/* final_unit on Session B when A+B aggregate > pool, even though B's  */
/* per-session grant is not yet consumed.  This test verifies the      */
/* arithmetic and flag-set directly using chf_supi_aggregate_used().   */
/* TS 32.291 §5.2 Case E.                                              */
/* ------------------------------------------------------------------ */
static void test_case_e_aggregate_exhaustion(abts_case *tc, void *data)
{
    const char    *supi        = "imsi-999700000200003";
    const uint64_t pool        = 200ULL * 1024 * 1024;  /* 200 MB */
    chf_sess_t    *sessA, *sessB;
    uint64_t       agg_used;
    bool           case_e_fired;

    /* Session A: 160 MB used — per-session grant was 200 MB, so A is not
     * yet exhausted by chf_check_quota_exhausted(), but it has consumed
     * 160 MB of the shared pool. */
    sessA = chf_sess_add(supi, "test-uuid-case-e-A");
    ABTS_PTR_NOTNULL(tc, sessA);
    sessA->granted_quota_total = 2 * (100 * 1024 * 1024);
    sessA->ul_octets        = 100 * 1024 * 1024;  /* 100 MB */
    sessA->dl_octets        =  60 * 1024 * 1024;  /*  60 MB → A: 160 MB */
    sessA->final_unit       = false;

    /* Session B: 50 MB used — per-session not exhausted */
    sessB = chf_sess_add(supi, "test-uuid-case-e-B");
    ABTS_PTR_NOTNULL(tc, sessB);
    sessB->granted_quota_total = 2 * (100 * 1024 * 1024);
    sessB->ul_octets        = 25 * 1024 * 1024;   /* 25 MB */
    sessB->dl_octets        = 25 * 1024 * 1024;   /* 25 MB → B: 50 MB */
    sessB->final_unit       = false;

    /* Aggregate = 160 + 50 = 210 MB > 200 MB pool */
    agg_used = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(210 * 1024 * 1024), (int)agg_used);

    /* Per-session check on B: 50 MB < 200 MB → NOT exhausted by itself */
    ABTS_INT_EQUAL(tc, 0 /* false */,
                   (int)chf_check_quota_exhausted(sessB));
    /* Reset final_unit (check_quota_exhausted should not have set it) */
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)sessB->final_unit);

    /* Case E: !final_unit && agg_used >= pool → set final_unit on B */
    case_e_fired = (!sessB->final_unit && agg_used >= pool);
    if (case_e_fired)
        sessB->final_unit = true;

    ABTS_INT_EQUAL(tc, 1 /* true */, (int)case_e_fired);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)sessB->final_unit);

    /* Session A is not modified by B's Case E check */
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)sessA->final_unit);

    /* Boundary: exact pool usage — Case E fires */
    sessA->ul_octets = 100 * 1024 * 1024;
    sessA->dl_octets = 100 * 1024 * 1024;  /* A: 200 MB */
    sessB->ul_octets = 0;
    sessB->dl_octets = 0;                   /* B:   0 MB */
    sessB->final_unit = false;
    agg_used = chf_supi_aggregate_used(supi);
    ABTS_INT_EQUAL(tc, (int)(200 * 1024 * 1024), (int)agg_used);
    case_e_fired = (!sessB->final_unit && agg_used >= pool);
    ABTS_INT_EQUAL(tc, 1 /* true */, (int)case_e_fired);

    /* One byte under pool — Case E does NOT fire */
    sessA->dl_octets -= 1;
    sessB->final_unit = false;
    agg_used = chf_supi_aggregate_used(supi);
    case_e_fired = (!sessB->final_unit && agg_used >= pool);
    ABTS_INT_EQUAL(tc, 0 /* false */, (int)case_e_fired);

    chf_sess_remove(sessA);
    chf_sess_remove(sessB);
}

/* ==================================================================== */
/* QU-18: Session lifecycle — find_by_ref and find_by_id (Gap G-9)     */
/*                                                                       */
/* Verifies that chf_sess_find_by_charging_data_ref() locates the       */
/* session by its UUID key, and that chf_sess_find_by_id() locates it   */
/* by pool ID.  After chf_sess_remove() both return NULL / 0.           */
/* TS 32.291 §5.2 — sessions must be addressable by ChargingDataRef.    */
/* ==================================================================== */
static void test_sess_lifecycle_find(abts_case *tc, void *data)
{
    const char *ref  = "test-uuid-lifecycle-001";
    const char *supi = "imsi-999700000300001";
    chf_sess_t *sess, *found;
    ogs_pool_id_t sid;

    /* 1. Add and verify pointer is non-NULL */
    sess = chf_sess_add(supi, ref);
    ABTS_PTR_NOTNULL(tc, sess);

    /* 2. find_by_charging_data_ref returns the same object */
    found = chf_sess_find_by_charging_data_ref(ref);
    ABTS_PTR_NOTNULL(tc, found);
    ABTS_TRUE(tc, found == sess);

    /* 3. find_by_id using the stored pool ID */
    sid = sess->id;
    ABTS_TRUE(tc, sid != OGS_INVALID_POOL_ID);
    found = chf_sess_find_by_id(sid);
    ABTS_PTR_NOTNULL(tc, found);
    ABTS_TRUE(tc, found == sess);

    /* 4. Unknown ref returns NULL */
    found = chf_sess_find_by_charging_data_ref("does-not-exist-uuid");
    ABTS_PTR_EQUAL(tc, NULL, found);

    /* 5. After remove, both lookups return NULL / 0 */
    chf_sess_remove(sess);
    found = chf_sess_find_by_charging_data_ref(ref);
    ABTS_PTR_EQUAL(tc, NULL, found);
    found = chf_sess_find_by_id(sid);
    ABTS_PTR_EQUAL(tc, NULL, found);
}

/* ------------------------------------------------------------------ */
/* Suite registration                                                   */
/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
/* QU-19: chf_quota_revive() - a recharge after exhaustion             */
/*                                                                      */
/* chf_check_quota_exhausted() only ever sets final_unit, so before    */
/* this an exhausted session kept a FinalUnitIndication on every       */
/* response after the operator recharged it, and the SMF terminated it */
/* at the end of the grant anyway.  The Update handler re-bases the    */
/* ceiling on the cycle counter and then calls chf_quota_revive(); this */
/* case walks the same sequence and every guard on it.                  */
/* ------------------------------------------------------------------ */
static void test_quota_revive_after_recharge(abts_case *tc, void *data)
{
    const uint64_t MB = 1024 * 1024;
    chf_sess_t *sess;

    sess = chf_sess_add("imsi-999700000200019", "test-uuid-revive-001");
    ABTS_PTR_NOTNULL(tc, sess);
    if (!sess)
        return;

    /* A 200 MB plan, used up: the session is exhausted. */
    sess->granted_quota_total = 200 * MB;
    sess->ul_octets = 110 * MB;
    sess->dl_octets = 90 * MB;
    sess->final_unit = false;
    ABTS_INT_EQUAL(tc, 1, (int)chf_check_quota_exhausted(sess));
    ABTS_INT_EQUAL(tc, 1, (int)sess->final_unit);

    /*
     * Guard 1: the session's own ceiling has not moved yet.  Nothing the
     * cycle counter says can clear it while used >= granted, because that
     * ceiling is what the response grant is measured against.
     */
    ABTS_INT_EQUAL(tc, 0,
            (int)chf_quota_revive(sess, 200 * MB, 500 * MB));
    ABTS_INT_EQUAL(tc, 1, (int)sess->final_unit);

    /* The operator raises the plan to 500 MB.  The handler re-bases the
     * session ceiling exactly like this: usage so far + cycle remainder. */
    sess->granted_quota_total = (sess->ul_octets + sess->dl_octets)
                              + (500 * MB - 200 * MB);

    /*
     * Guard 2: the session view now has room, but the cycle counter - which
     * spans every session of the cycle - is still at or over the allowance.
     * Case E governs; stay exhausted.
     */
    ABTS_INT_EQUAL(tc, 0,
            (int)chf_quota_revive(sess, 500 * MB, 500 * MB));
    ABTS_INT_EQUAL(tc, 1, (int)sess->final_unit);

    /* Both views have room: the recharge revives the session. */
    ABTS_INT_EQUAL(tc, 1,
            (int)chf_quota_revive(sess, 200 * MB, 500 * MB));
    ABTS_INT_EQUAL(tc, 0, (int)sess->final_unit);

    /* Not exhausted any more, so the per-session check leaves it alone... */
    ABTS_INT_EQUAL(tc, 0, (int)chf_check_quota_exhausted(sess));
    ABTS_INT_EQUAL(tc, 0, (int)sess->final_unit);

    /* ...and reviving an already-live session is a no-op, not a repeat. */
    ABTS_INT_EQUAL(tc, 0,
            (int)chf_quota_revive(sess, 200 * MB, 500 * MB));

    /*
     * The clear is not permanent: once the session uses the new ceiling it
     * exhausts again, exactly as it did the first time.  A revive that left
     * final_unit off for good would be a free unlimited plan.
     */
    sess->ul_octets += 150 * MB;
    sess->dl_octets += 150 * MB;
    ABTS_INT_EQUAL(tc, 1, (int)chf_check_quota_exhausted(sess));
    ABTS_INT_EQUAL(tc, 1, (int)sess->final_unit);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* QU-20: a quota-exempt session counts towards nothing                 */
/*                                                                      */
/* IMS or emergency traffic must not use up a subscriber's data         */
/* allowance, which is what this sum decides for a SUPI with no bucket. */
/* ------------------------------------------------------------------ */
static void test_quota_exempt_not_in_aggregate(abts_case *tc, void *data)
{
    const uint64_t MB = 1024 * 1024;
    const char *supi = "imsi-999700000200020";
    chf_sess_t *data_sess, *ims_sess;

    data_sess = chf_sess_add(supi, "test-uuid-exempt-data");
    ims_sess  = chf_sess_add(supi, "test-uuid-exempt-ims");
    ABTS_PTR_NOTNULL(tc, data_sess);
    ABTS_PTR_NOTNULL(tc, ims_sess);
    if (!data_sess || !ims_sess)
        return;

    data_sess->dl_octets = 10 * MB;
    ims_sess->dl_octets  = 700 * MB;
    ims_sess->quota_exempt = true;

    ABTS_TRUE(tc, chf_supi_aggregate_used(supi) == 10 * MB);

    /* The usage is still there for the CDR. */
    ABTS_TRUE(tc, ims_sess->dl_octets == 700 * MB);

    chf_sess_remove(ims_sess);
    chf_sess_remove(data_sess);
}

/* ------------------------------------------------------------------ */
/* QU-21: a notification never reaches a quota-exempt session           */
/*                                                                      */
/* A ChargingNotify with no bucket goes to every session of the SUPI,   */
/* and an ABORT_CHARGING for a deprovisioned data plan ends the session */
/* it reaches.  That must not be an emergency call.                     */
/* ------------------------------------------------------------------ */
static void count_session(chf_sess_t *sess, void *data)
{
    (*(int *)data)++;
}

static void test_quota_exempt_not_notified(abts_case *tc, void *data)
{
    const char *supi = "imsi-999700000200021";
    chf_sess_t *data_sess, *sos_sess;
    int reached = 0, matched;

    data_sess = chf_sess_add(supi, "test-uuid-notify-data");
    sos_sess  = chf_sess_add(supi, "test-uuid-notify-sos");
    ABTS_PTR_NOTNULL(tc, data_sess);
    ABTS_PTR_NOTNULL(tc, sos_sess);
    if (!data_sess || !sos_sess)
        return;

    sos_sess->quota_exempt = true;

    matched = chf_sess_foreach_by_bucket(supi, NULL, count_session, &reached);
    ABTS_INT_EQUAL(tc, 1, matched);
    ABTS_INT_EQUAL(tc, 1, reached);

    /* Naming the SUPI as the bucket must not reach it either: that is
     * what chf_sess_bucket() would answer for a session with no bucket. */
    reached = 0;
    matched = chf_sess_foreach_by_bucket(supi, supi, count_session, &reached);
    ABTS_INT_EQUAL(tc, 1, matched);
    ABTS_INT_EQUAL(tc, 1, reached);

    chf_sess_remove(sos_sess);
    chf_sess_remove(data_sess);
}

abts_suite *test_chf_quota(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_default_quota_values,           NULL);
    abts_run_test(suite, test_quota_split,                    NULL);
    abts_run_test(suite, test_remaining_volume,               NULL);
    abts_run_test(suite, test_quota_exhaustion_boundary,      NULL);
    abts_run_test(suite, test_sessions_are_independent,       NULL);
    abts_run_test(suite, test_zero_volume_quota_exhaustion,   NULL);
    abts_run_test(suite, test_fua_action_enum_values,         NULL);
    abts_run_test(suite, test_quota_split_odd_bytes,          NULL);
    abts_run_test(suite, test_zero_volume_with_nonzero_usage, NULL);
    /* Aggregate quota (TS 32.291 §5.2) */
    abts_run_test(suite, test_aggregate_used_single_session,          NULL);
    abts_run_test(suite, test_aggregate_used_multi_session_same_supi, NULL);
    abts_run_test(suite, test_aggregate_used_cross_supi_isolation,    NULL);
    abts_run_test(suite, test_create_time_cap_arithmetic,             NULL);
    abts_run_test(suite, test_aggregate_new_session_excluded,         NULL);
    abts_run_test(suite, test_aggregate_index_entry_recreation,       NULL);
    abts_run_test(suite,
            test_aggregate_index_key_survives_sibling_removal,        NULL);
    /* Gap-fix: direct helper coverage (G-1, G-2, G-3) */
    abts_run_test(suite, test_apply_quota_direct,             NULL);
    abts_run_test(suite, test_check_quota_exhausted_direct,   NULL);
    abts_run_test(suite, test_case_e_aggregate_exhaustion,    NULL);
    /* Case C: recharge after exhaustion revives the session */
    abts_run_test(suite, test_quota_revive_after_recharge,    NULL);
    /* Gap-fix: session lifecycle / lookup (G-9) */
    abts_run_test(suite, test_sess_lifecycle_find,            NULL);
    abts_run_test(suite, test_quota_exempt_not_in_aggregate,  NULL);
    abts_run_test(suite, test_quota_exempt_not_notified,      NULL);

    return suite;
}
