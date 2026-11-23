/*
 * CHF Prometheus metrics.
 *
 * Two metric families, mirroring the pattern used by src/pcf/metrics.c:
 *   - GLOBAL: label-less gauges / counters / histograms, pre-created once.
 *   - LABELED: metrics with bounded string labels, whose per-label-combination
 *     instances are created lazily and cached in a hash (so dynamic but
 *     bounded label values such as dnn / smf do not need to be known up front).
 *
 * Cardinality guardrail: only bounded labels are used (operation, result,
 * cause, scope, action, source, collection, op, kind, dnn, smf).  Never label
 * by SUPI, chargingDataRef, or rating group.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

#ifndef CHF_METRICS_H
#define CHF_METRICS_H

#include <limits.h>

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Clamp a 64-bit quantity to a non-negative int.
 *
 * ogs_metrics_inst_add() takes an int and asserts val >= 0 for counters and
 * histograms, so any value derived from an untrusted or unbounded source
 * (e.g. usedUnitContainer volumes parsed from a peer's JSON) must be clamped
 * before it is recorded — an overflowing cast would otherwise abort the NF.
 */
static inline int chf_metrics_clamp_u64(uint64_t v)
{
    return v > (uint64_t)INT_MAX ? INT_MAX : (int)v;
}

/* Maximum string labels on any CHF metric, and per-label value length. */
#define CHF_METR_MAX_LABELS 3
#define CHF_METR_LABEL_LEN  64

/*
 * Upper bound on distinct label combinations (Prometheus series) tracked.
 * A safety net against unbounded label values; well above what a sane
 * deployment produces (bounded operation/result/cause/scope/action/source/
 * collection/op/kind values times a handful of dnn/smf values).
 */
#define CHF_METR_MAX_LABELED_INSTANCES 1024

/* ------------------------------------------------------------------ */
/* GLOBAL (label-less) metrics                                         */
/* ------------------------------------------------------------------ */
typedef enum chf_metric_global_e {
    CHF_METR_GAUGE_SESSIONS_ACTIVE = 0,
    CHF_METR_GAUGE_SBI_INFLIGHT,
    CHF_METR_GAUGE_PROVISIONED_SUBS,

    CHF_METR_CTR_QUOTA_GRANTED_KB,      /* kilobytes (int-safe for GB grants) */
    CHF_METR_CTR_QUOTA_USED_KB,         /* kilobytes */
    CHF_METR_CTR_QUOTA_AUTO_RENEW,
    CHF_METR_CTR_CDR_BYTES,
    CHF_METR_CTR_DB_UNAVAILABLE,
    CHF_METR_CTR_NOTIFY_SENT,           /* ChargingNotify delivered  */
    CHF_METR_CTR_NOTIFY_FAILED,         /* ... and could not be      */

    CHF_METR_HIST_CDR_WRITE_DURATION,   /* milliseconds */

    _CHF_METR_GLOB_MAX,
} chf_metric_global_t;

extern ogs_metrics_inst_t *chf_metrics_inst_global[_CHF_METR_GLOB_MAX];

/*
 * The NULL guards let instrumented code run before chf_metrics_init() (e.g.
 * unit tests that link libchf but never start the metrics context): the
 * instance array is zero-initialised, so calls are no-ops until init populates
 * it.
 */
static inline void chf_metrics_inst_global_set(chf_metric_global_t t, int val)
{ if (chf_metrics_inst_global[t]) ogs_metrics_inst_set(chf_metrics_inst_global[t], val); }
static inline void chf_metrics_inst_global_add(chf_metric_global_t t, int val)
{ if (chf_metrics_inst_global[t]) ogs_metrics_inst_add(chf_metrics_inst_global[t], val); }
static inline void chf_metrics_inst_global_inc(chf_metric_global_t t)
{ if (chf_metrics_inst_global[t]) ogs_metrics_inst_inc(chf_metrics_inst_global[t]); }
static inline void chf_metrics_inst_global_dec(chf_metric_global_t t)
{ if (chf_metrics_inst_global[t]) ogs_metrics_inst_dec(chf_metrics_inst_global[t]); }

/* ------------------------------------------------------------------ */
/* LABELED metrics                                                     */
/* ------------------------------------------------------------------ */
typedef enum chf_metric_labeled_e {
    CHF_METR_LBL_SBI_REQUESTS = 0,      /* {operation, result}          */
    CHF_METR_LBL_SBI_REQ_DURATION,      /* {operation}       histogram  */
    CHF_METR_LBL_SESSION_CREATE,        /* {result, dnn, smf}           */
    CHF_METR_LBL_SESSION_UPDATE,        /* {result}                     */
    CHF_METR_LBL_SESSION_RELEASE,       /* {cause}                      */
    CHF_METR_LBL_QUOTA_EXHAUSTED,       /* {scope, dnn}                 */
    CHF_METR_LBL_QUOTA_FINAL_UNIT,      /* {action}                     */
    CHF_METR_LBL_QUOTA_LOOKUP,          /* {source}                     */
    CHF_METR_LBL_CDR_WRITTEN,           /* {result}                     */
    /*
     * One per encoding actually written, where CDR_WRITTEN counts one per
     * record.  With charging.cdr.format: both, a record that reaches the
     * JSON file but fails the BER one is a divergence an operator has to
     * see, and the record-level counter cannot show it.
     */
    CHF_METR_LBL_CDR_ENCODED,           /* {format, result}             */
    /*
     * Aggregate file rolls, by what tripped the threshold.  This is how
     * max_records / max_size_mb get tuned: rolls attributed almost
     * entirely to one reason mean the other threshold is doing nothing.
     */
    CHF_METR_LBL_CDR_FILE_ROLLED,       /* {format, reason}             */
    /*
     * Which allowance a session was charged to, and on what slice.  The
     * point of per-slice buckets is that a scoped one is chosen over the
     * catch-all; without this, confirming that in a live network means
     * reading CHF logs, which is what the slicing regression had to do.
     */
    CHF_METR_LBL_BUCKET_SELECTED,       /* {kind, slice}                */
    CHF_METR_LBL_DB_OPS,                /* {collection, op, result}     */
    CHF_METR_LBL_DB_OP_DURATION,        /* {collection, op}  histogram  */
    CHF_METR_LBL_ERRORS,                /* {kind}                       */

    _CHF_METR_LBL_MAX,
} chf_metric_labeled_t;

/*
 * Add `val` to a labeled metric, creating the per-label-combination instance
 * on first use.  Pass label values in declaration order; unused trailing
 * labels may be NULL.  For counters/gauges `val` is the delta; for histograms
 * it is the observed value (milliseconds).
 */
void chf_metrics_inst_labeled_add(chf_metric_labeled_t t,
        const char *l0, const char *l1, const char *l2, int val);

static inline void chf_metrics_inst_labeled_inc(chf_metric_labeled_t t,
        const char *l0, const char *l1, const char *l2)
{ chf_metrics_inst_labeled_add(t, l0, l1, l2, 1); }

/* ------------------------------------------------------------------ */
/* Lifecycle                                                           */
/* ------------------------------------------------------------------ */
/*
 * How many metric specs are malformed — a missing name or description, or
 * a label count the instance key cannot hold.  Zero in a correct build;
 * tests/chf/chf-metrics-test.c asserts that, so adding a metric to an
 * enum and forgetting its spec fails a test instead of registering a
 * nameless series.
 */
int chf_metrics_spec_defects(void);

void chf_metrics_init(void);
void chf_metrics_final(void);

/*
 * How many labeled series exist right now, or 0 before chf_metrics_init().
 *
 * This is what makes the cardinality cap and the teardown observable from a
 * test: without it a suite cannot tell an empty table from one that was
 * never populated, and a teardown test would pass vacuously.
 */
unsigned int chf_metrics_labeled_count(void);

#ifdef __cplusplus
}
#endif

/*
 * ogs-metrics.h switches OGS_LOG_DOMAIN to __ogs_metrics_domain; restore the
 * CHF log domain so any .c file that includes this header keeps logging under
 * "chf" regardless of include order.
 *
 * __chf_log_domain is declared by context.h, which every CHF translation unit
 * includes; it is deliberately NOT re-declared here because GCC's
 * -Wredundant-decls (enabled on Linux) flags a duplicate extern declaration.
 */
#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __chf_log_domain

#endif /* CHF_METRICS_H */
