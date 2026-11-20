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
 * The metric tables, and the contract the rest of the tests rely on.
 *
 * These do not read metric values — that needs a running metrics context,
 * a Prometheus exporter and a scrape, which is the rig's job. What they
 * check is the part that silently rots:
 *
 *   - Both spec tables are sized by their enum's _MAX and filled with
 *     designated initialisers, so adding a metric to an enum and
 *     forgetting its spec leaves a zeroed entry whose name is NULL.
 *     chf_metrics_init_spec() passes that straight to
 *     ogs_metrics_spec_new(), so the mistake reaches the exporter rather
 *     than the compiler.
 *
 *   - Every other suite in this binary instruments code that calls
 *     chf_metrics_inst_*() without a metrics context. The header
 *     promises those are no-ops; if that ever stopped being true, every
 *     CDR and quota test would start crashing for a reason that has
 *     nothing to do with what they test. Worth one test of its own.
 *
 *   - METR-5 is the exception to all of the above: it starts a real
 *     metrics context, because chf_metrics_final() cannot be exercised
 *     without one, and the most damaging defect this module has had was in
 *     exactly that function.  Upstream's "make hash tables own their keys"
 *     turned its teardown loop into a double free, and nothing here could
 *     see it: the suite deliberately runs without a context, so
 *     chf_metrics_final() was unreachable.  METR-5 closes that.
 *
 *     It must leave the module as it found it - no context, no live
 *     instances - because the suites that run after this one instrument
 *     global metrics, and a stale non-NULL instance pointer would make
 *     those calls a use-after-free rather than a no-op.
 *
 * Test IDs: METR-1 .. METR-5
 */

#include "ogs-sbi.h"
#include "chf/context.h"
#include "chf/metrics.h"
#include "core/abts.h"

/* ------------------------------------------------------------------ */
/* METR-1: every metric spec is complete                               */
/* ------------------------------------------------------------------ */
static void test_metrics_specs_complete(abts_case *tc, void *data)
{
    /*
     * Zero is the only acceptable answer. A non-zero count means some
     * metric in chf_metric_global_t or chf_metric_labeled_t has no spec,
     * no description, or a label count the instance key cannot hold —
     * chf_metrics_spec_defects() logs which.
     */
    ABTS_INT_EQUAL(tc, 0, chf_metrics_spec_defects());
}

/* ------------------------------------------------------------------ */
/* METR-2: the enums are non-empty and within the key's capacity       */
/* ------------------------------------------------------------------ */
static void test_metrics_enum_bounds(abts_case *tc, void *data)
{
    ABTS_TRUE(tc, _CHF_METR_GLOB_MAX > 0);
    ABTS_TRUE(tc, _CHF_METR_LBL_MAX > 0);

    /* A labeled metric may carry at most CHF_METR_MAX_LABELS labels, and
     * the instance key is sized for exactly that. */
    ABTS_TRUE(tc, CHF_METR_MAX_LABELS >= 1);
    ABTS_TRUE(tc, CHF_METR_LABEL_LEN > 1);

    /* The series cap is what stops an unbounded label value from growing
     * the hash without limit. */
    ABTS_TRUE(tc, CHF_METR_MAX_LABELED_INSTANCES > 0);
}

/* ------------------------------------------------------------------ */
/* METR-3: instrumentation is a no-op before the context exists        */
/*                                                                      */
/* This is the contract every other suite here depends on.  None of     */
/* them starts a metrics context, yet all of them run code that         */
/* instruments.                                                         */
/* ------------------------------------------------------------------ */
static void test_metrics_noop_without_context(abts_case *tc, void *data)
{
    /* Global: set, add, inc, dec. */
    chf_metrics_inst_global_set(CHF_METR_GAUGE_SESSIONS_ACTIVE, 7);
    chf_metrics_inst_global_add(CHF_METR_CTR_CDR_BYTES, 1234);
    chf_metrics_inst_global_inc(CHF_METR_CTR_NOTIFY_SENT);
    chf_metrics_inst_global_dec(CHF_METR_GAUGE_SESSIONS_ACTIVE);

    /* Labeled, including the metrics added for slicing and the two CDR
     * encodings. */
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_WRITTEN,
            "success", NULL, NULL);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
            "asn1", "success", NULL);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_FILE_ROLLED,
            "json", "records", NULL);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_BUCKET_SELECTED,
            "scoped", "1/000001", NULL);

    /* Reaching here without a crash is the assertion. */
    ABTS_TRUE(tc, true);
}

/* ------------------------------------------------------------------ */
/* METR-4: an over-long label value is tolerated on this path          */
/*                                                                      */
/* dnn and smf label values come off the wire, so an over-long one is   */
/* reachable from a peer rather than only from our own code.            */
/*                                                                      */
/* What this proves is narrow, and worth stating: without a metrics     */
/* context chf_metrics_inst_labeled_add() returns before it copies      */
/* anything, so this shows the uninitialised path is safe for any       */
/* input, not that truncation works.  Truncation is by construction —   */
/* the copy is an ogs_cpystrn() bounded by CHF_METR_LABEL_LEN — and     */
/* exercising it needs a live metrics context, which is the rig's job.  */
/* ------------------------------------------------------------------ */
static void test_metrics_overlong_label(abts_case *tc, void *data)
{
    char huge[CHF_METR_LABEL_LEN * 3];

    memset(huge, 'x', sizeof(huge) - 1);
    huge[sizeof(huge) - 1] = '\0';

    chf_metrics_inst_labeled_inc(CHF_METR_LBL_BUCKET_SELECTED,
            "scoped", huge, NULL);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE,
            "success", huge, huge);

    ABTS_TRUE(tc, true);
}

/* ------------------------------------------------------------------ */
/* METR-5: init, populate, and tear down a real metrics context        */
/*                                                                      */
/* The only case here that starts a context.  What it is really for is  */
/* chf_metrics_final(): with entries in the labeled table, the teardown */
/* has keys to release, which is where the double free lived.  talloc   */
/* aborts on a double free, so a regression here fails loudly rather    */
/* than silently corrupting the heap.                                   */
/*                                                                      */
/* ogs_metrics_context_init() is safe in a unit test: it sets up the    */
/* spec registry and the server list, and binds nothing - a port is     */
/* only opened by ogs_metrics_server_open(), which chf_metrics_init()   */
/* does not call.                                                       */
/* ------------------------------------------------------------------ */
static void test_metrics_init_populate_final(abts_case *tc, void *data)
{
    unsigned int populated;

    /* Nothing before init, and the accessor must tolerate that. */
    ABTS_INT_EQUAL(tc, 0, chf_metrics_labeled_count());

    chf_metrics_init();

    /* A context does not imply series: they appear on first use. */
    ABTS_INT_EQUAL(tc, 0, chf_metrics_labeled_count());

    /*
     * Four distinct label combinations, so the table holds four keys that
     * teardown has to release - including two that differ only in one
     * label, which is what the key's memset-then-fill exists for.
     */
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_WRITTEN,
            "success", NULL, NULL);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
            "json", "success", NULL);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
            "asn1", "success", NULL);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_BUCKET_SELECTED,
            "scoped", "1/000001", NULL);

    populated = chf_metrics_labeled_count();
    ABTS_INT_EQUAL(tc, 4, populated);

    /* Repeating a combination reuses its series rather than adding one. */
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
            "json", "success", NULL);
    ABTS_INT_EQUAL(tc, 4, chf_metrics_labeled_count());

    /* Globals are live now, which is the state final has to undo. */
    chf_metrics_inst_global_set(CHF_METR_GAUGE_SESSIONS_ACTIVE, 3);
    chf_metrics_inst_global_inc(CHF_METR_CTR_NOTIFY_SENT);

    /*
     * The assertion is that this returns at all.  Freeing each key after
     * removing its entry - correct when the table stored the caller's
     * pointer, a double free once the table owned its own copy - aborts
     * here under talloc.
     */
    chf_metrics_final();

    ABTS_INT_EQUAL(tc, 0, chf_metrics_labeled_count());

    /*
     * And the module is back where it started, so the suites after this
     * one instrument into no-ops instead of freed instances.
     */
    chf_metrics_inst_global_set(CHF_METR_GAUGE_SESSIONS_ACTIVE, 9);
    chf_metrics_inst_global_inc(CHF_METR_CTR_NOTIFY_SENT);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_WRITTEN,
            "success", NULL, NULL);
    ABTS_INT_EQUAL(tc, 0, chf_metrics_labeled_count());

    /*
     * Deliberately not tested: a second chf_metrics_init() after this
     * final.  It segfaults, and not because of anything in the CHF.
     * ogs_metrics_spec_final() calls
     * prom_collector_registry_destroy(PROM_COLLECTOR_REGISTRY_DEFAULT),
     * which frees the registry and clears only its own local parameter, so
     * the library's global still points at freed memory.
     * prom_collector_registry_default_init() then returns early on
     * "already initialised" and the next metric registration dereferences
     * it.  One init and one final per process is all the stack supports,
     * which is all a daemon does; asserting a re-init cycle here would be
     * asserting a property that does not exist.
     */
}

abts_suite *test_chf_metrics(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_metrics_specs_complete,      NULL);
    abts_run_test(suite, test_metrics_enum_bounds,         NULL);
    abts_run_test(suite, test_metrics_noop_without_context, NULL);
    abts_run_test(suite, test_metrics_overlong_label,      NULL);
    /* Last: it is the only case that starts a context, and the cases
     * above require that none is running. */
    abts_run_test(suite, test_metrics_init_populate_final, NULL);

    return suite;
}
