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
 * Grant shaping (chf.yaml charging.grant)
 *
 * The CHF does not hand a session everything left in its billing cycle.  Each
 * grant is capped, carries a validityTime, and says how much of it should be
 * left when the SMF reports (volumeQuotaThreshold, TS 32.291
 * MultipleUnitInformation).  Those three numbers decide how quickly a plan
 * change reaches a live session, how much two devices on one plan can
 * overspend, and how many Updates it costs to use an allowance up.
 *
 * chf_grant_volume()    — the slice
 * chf_grant_validity()  — the timer
 * chf_grant_threshold() — when to report, and the floor that stops the
 *                         end of an allowance turning into a burst of
 *                         Updates that each grant almost nothing
 * chf_grant_holding()   — how long an idle session may hold the grant
 *                         before it has to hand the rest back
 */

#include "ogs-sbi.h"
#include "chf/context.h"
#include "core/abts.h"

#define MB ((uint64_t)1024 * 1024)

/* ------------------------------------------------------------------ */
/* Shipped defaults: 50 MiB slices, 5 min, no early report below 5 MiB */
/* ------------------------------------------------------------------ */
static void test_grant_defaults(abts_case *tc, void *data)
{
    chf_context_t *ctx = chf_self();
    ABTS_PTR_NOTNULL(tc, ctx);

    ABTS_TRUE(tc, ctx->grant.max_volume_bytes == 50 * MB);
    ABTS_INT_EQUAL(tc, 300, (int)ctx->grant.max_validity_seconds);
    ABTS_TRUE(tc, ctx->grant.min_report_bytes == 5 * MB);
    ABTS_INT_EQUAL(tc, 60, (int)ctx->grant.quota_holding_seconds);

    /* Unprovisioned SUPIs get the default quota unless configured to be
     * refused (chf.yaml charging.unprovisioned). */
    ABTS_TRUE(tc, ctx->reject_unprovisioned == false);
}

/* ------------------------------------------------------------------ */
/* The slice: min(remaining, cap); 0 disables the cap                  */
/* ------------------------------------------------------------------ */
static void test_grant_volume_slice(abts_case *tc, void *data)
{
    /* More left than one slice */
    ABTS_TRUE(tc, chf_grant_volume(10240 * MB, 50 * MB) == 50 * MB);
    /* Exactly one slice */
    ABTS_TRUE(tc, chf_grant_volume(50 * MB, 50 * MB) == 50 * MB);
    /* Less than a slice: grant what is left */
    ABTS_TRUE(tc, chf_grant_volume(12 * MB, 50 * MB) == 12 * MB);
    ABTS_TRUE(tc, chf_grant_volume(1, 50 * MB) == 1);
    /* Nothing left */
    ABTS_TRUE(tc, chf_grant_volume(0, 50 * MB) == 0);
    /* Cap disabled: the whole remainder, as before slicing */
    ABTS_TRUE(tc, chf_grant_volume(10240 * MB, 0) == 10240 * MB);
}

/* ------------------------------------------------------------------ */
/* validityTime: the lesser of the cycle's end and the cap             */
/* ------------------------------------------------------------------ */
static void test_grant_validity_cap(abts_case *tc, void *data)
{
    /* A monthly cycle is far away: the cap decides */
    ABTS_INT_EQUAL(tc, 300, (int)chf_grant_validity(2592000, 300));
    /* The cycle ends first: never promise time past it */
    ABTS_INT_EQUAL(tc, 42, (int)chf_grant_validity(42, 300));
    ABTS_INT_EQUAL(tc, 300, (int)chf_grant_validity(300, 300));
    /* No cycle end (validity_seconds 0): only the cap applies */
    ABTS_INT_EQUAL(tc, 300, (int)chf_grant_validity(0, 300));
    /* Cap disabled: run to the end of the cycle */
    ABTS_INT_EQUAL(tc, 2592000, (int)chf_grant_validity(2592000, 0));
    /* Neither: omit validityTime (0 means "do not send it") */
    ABTS_INT_EQUAL(tc, 0, (int)chf_grant_validity(0, 0));
}

/* ------------------------------------------------------------------ */
/* quotaHoldingTime: an idle session gives its grant back after this   */
/* long, capped at the life of the grant it is holding                 */
/* ------------------------------------------------------------------ */
static void test_grant_holding_cap(abts_case *tc, void *data)
{
    /* The usual case: 60 s of idleness inside a 300 s grant */
    ABTS_INT_EQUAL(tc, 60, (int)chf_grant_holding(300, 60));
    /* Never outlast the grant: validityTime would fire first, so a
     * holding time past it could never be reached */
    ABTS_INT_EQUAL(tc, 45, (int)chf_grant_holding(45, 60));
    ABTS_INT_EQUAL(tc, 60, (int)chf_grant_holding(60, 60));
    /* No validityTime on this grant: nothing to cap against */
    ABTS_INT_EQUAL(tc, 60, (int)chf_grant_holding(0, 60));
    /* Turned off (quota_holding_seconds 0): omit quotaHoldingTime, so
     * the session holds its grant until it is spent or expires */
    ABTS_INT_EQUAL(tc, 0, (int)chf_grant_holding(300, 0));
    ABTS_INT_EQUAL(tc, 0, (int)chf_grant_holding(0, 0));
}

/* ------------------------------------------------------------------ */
/* volumeQuotaThreshold: 10% left, but not for a grant at or below the */
/* floor - that one is reported only when it is used up                */
/* ------------------------------------------------------------------ */
static void test_grant_threshold_floor(abts_case *tc, void *data)
{
    /* Above the floor: report with 10% of the grant left */
    ABTS_TRUE(tc, chf_grant_threshold(50 * MB, 5 * MB) == 5 * MB);
    ABTS_TRUE(tc, chf_grant_threshold(45 * MB, 5 * MB) == 45 * MB / 10);
    /* At the floor, and below it: no early report */
    ABTS_TRUE(tc, chf_grant_threshold(5 * MB, 5 * MB) == 0);
    ABTS_TRUE(tc, chf_grant_threshold(4 * MB + 512 * 1024, 5 * MB) == 0);
    ABTS_TRUE(tc, chf_grant_threshold(1, 5 * MB) == 0);
    ABTS_TRUE(tc, chf_grant_threshold(0, 5 * MB) == 0);
    /* Floor disabled: always the 10% early report */
    ABTS_TRUE(tc, chf_grant_threshold(1000, 0) == 100);
    ABTS_TRUE(tc, chf_grant_threshold(50 * MB, 0) == 5 * MB);
}

/* ------------------------------------------------------------------ */
/* How an allowance is actually spent                                   */
/*                                                                      */
/* Replays what the SMF and UPF do with each grant: use it until the    */
/* threshold says to report (or all of it when there is no early        */
/* report), report, take the next grant.  Returns the number of reports */
/* and checks that exactly the allowance was used - never more.         */
/* ------------------------------------------------------------------ */
static int spend_allowance(abts_case *tc, uint64_t allowance,
        uint64_t slice, uint64_t min_report)
{
    uint64_t consumed = 0;
    int reports = 0;

    for (;;) {
        uint64_t grant = chf_grant_volume(allowance - consumed, slice);
        uint64_t threshold = chf_grant_threshold(grant, min_report);
        uint64_t used_before_report;

        if (grant == 0)
            break;      /* exhausted: the CHF answers with FinalUnit */

        /* The UPF reports either at the threshold or when the grant is
         * gone; traffic is assumed to keep flowing throughout. */
        used_before_report = grant - threshold;
        consumed += used_before_report;
        reports++;

        ABTS_TRUE(tc, consumed <= allowance);
        ABTS_TRUE(tc, reports < 10000);   /* no runaway loop */
        if (reports >= 10000)
            break;
    }

    /* Every octet of the allowance is accounted for, and no more. */
    ABTS_TRUE(tc, consumed == allowance);
    return reports;
}

static void test_spend_90mb_plan(abts_case *tc, void *data)
{
    /*
     * 90 MB plan with the shipped settings: 50 MB, then 45 MB, then the
     * 4.5 MB remainder, which is under the floor and so is reported only
     * once it is used up.  Three reports, matching what the CHF does
     * against a live SMF and UPF.
     */
    ABTS_INT_EQUAL(tc, 3, spend_allowance(tc, 90 * MB, 50 * MB, 5 * MB));

    /*
     * Without the floor, every report leaves 10% of the one before, and
     * the last megabyte costs a burst of Updates that each grant almost
     * nothing.  This is the regression the floor fixes, so the test names
     * the old cost rather than just "fewer".
     */
    ABTS_INT_EQUAL(tc, 9, spend_allowance(tc, 90 * MB, 50 * MB, 0));
}

static void test_spend_edge_sizes(abts_case *tc, void *data)
{
    /* A plan smaller than the floor: one grant, one report */
    ABTS_INT_EQUAL(tc, 1, spend_allowance(tc, 2 * MB, 50 * MB, 5 * MB));
    /* Exactly one slice */
    ABTS_INT_EQUAL(tc, 2, spend_allowance(tc, 50 * MB, 50 * MB, 5 * MB));
    /* A large plan still terminates, and bills exactly the allowance */
    ABTS_TRUE(tc, spend_allowance(tc, 10240 * MB, 50 * MB, 5 * MB) > 200);
    /* Slicing disabled: one grant for the whole plan, reported at 90%,
     * then 9 MB, then the remainder once it drops under the floor */
    ABTS_INT_EQUAL(tc, 3, spend_allowance(tc, 90 * MB, 0, 5 * MB));
}

/* ------------------------------------------------------------------ */
/* Suite registration                                                   */
/* ------------------------------------------------------------------ */
abts_suite *test_chf_grant(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_grant_defaults,        NULL);
    abts_run_test(suite, test_grant_volume_slice,    NULL);
    abts_run_test(suite, test_grant_validity_cap,    NULL);
    abts_run_test(suite, test_grant_holding_cap,     NULL);
    abts_run_test(suite, test_grant_threshold_floor, NULL);
    abts_run_test(suite, test_spend_90mb_plan,       NULL);
    abts_run_test(suite, test_spend_edge_sizes,      NULL);

    return suite;
}
