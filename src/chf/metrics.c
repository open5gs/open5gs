/*
 * CHF Prometheus metrics — spec definitions, lazy labeled instances, lifecycle.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

#include "ogs-app.h"
#include "context.h"
#include "metrics.h"

typedef struct chf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
    ogs_metrics_histogram_params_t histogram_params;
} chf_metrics_spec_def_t;

/* Duration histogram buckets (milliseconds): 1,4,16,64,256,1024,4096,16384. */
#define CHF_HIST_MS_PARAMS { \
    .type = OGS_METRICS_HISTOGRAM_BUCKET_TYPE_EXPONENTIAL, \
    .count = 8, .exp = { .start = 1, .factor = 4 } }

/* ------------------------------------------------------------------ */
/* Generic spec/instance helpers                                       */
/* ------------------------------------------------------------------ */
static int chf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, chf_metrics_spec_def_t *src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val, src[i].num_labels, src[i].labels,
                src[i].type == OGS_METRICS_METRIC_TYPE_HISTOGRAM ?
                    &src[i].histogram_params : NULL);
    }
    return OGS_OK;
}

/* ------------------------------------------------------------------ */
/* GLOBAL (label-less)                                                 */
/* ------------------------------------------------------------------ */
ogs_metrics_spec_t *chf_metrics_spec_global[_CHF_METR_GLOB_MAX];
ogs_metrics_inst_t *chf_metrics_inst_global[_CHF_METR_GLOB_MAX];

static chf_metrics_spec_def_t chf_metrics_spec_def_global[_CHF_METR_GLOB_MAX] = {
[CHF_METR_GAUGE_SESSIONS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "chf_sessions_active",
    .description = "Active Nchf_ConvergedCharging sessions in memory", },
[CHF_METR_GAUGE_SBI_INFLIGHT] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "chf_sbi_inflight",
    .description = "Nchf requests currently being processed", },
[CHF_METR_GAUGE_PROVISIONED_SUBS] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "chf_provisioned_subscribers",
    .description = "Provisioned CHF subscriber quota profiles", },
[CHF_METR_CTR_QUOTA_GRANTED_KB] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_quota_granted_kilobytes_total",
    .description = "Total volume granted across all quota grants (kilobytes)", },
[CHF_METR_CTR_QUOTA_USED_KB] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_quota_used_kilobytes_total",
    .description = "Total reported usage (UL+DL) accumulated (kilobytes)", },
[CHF_METR_CTR_QUOTA_AUTO_RENEW] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_quota_auto_renew_total",
    .description = "Auto-renew rollovers at billing-period expiry", },
[CHF_METR_CTR_CDR_BYTES] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_cdr_bytes_total",
    .description = "Bytes of CDR written, summed across encodings", },
[CHF_METR_CTR_DB_UNAVAILABLE] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_db_unavailable_total",
    .description = "Times the memory-only DB fallback path was taken", },
[CHF_METR_CTR_NOTIFY_SENT] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_charging_notify_sent_total",
    .description = "ChargingNotify requests sent to an SMF", },
[CHF_METR_CTR_NOTIFY_FAILED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_charging_notify_failed_total",
    .description = "ChargingNotify requests that could not be delivered", },
[CHF_METR_HIST_CDR_WRITE_DURATION] = {
    .type = OGS_METRICS_METRIC_TYPE_HISTOGRAM,
    .name = "chf_cdr_write_duration_milliseconds",
    .description = "CDR write latency in milliseconds",
    .histogram_params = CHF_HIST_MS_PARAMS, },
};

/* ------------------------------------------------------------------ */
/* LABELED                                                             */
/* ------------------------------------------------------------------ */
static const char *labels_op_result[]     = { "operation", "result" };
static const char *labels_op[]            = { "operation" };
static const char *labels_result_dnn_smf[] = { "result", "dnn", "smf" };
static const char *labels_result[]        = { "result" };
static const char *labels_cause[]         = { "cause" };
static const char *labels_scope_dnn[]     = { "scope", "dnn" };
static const char *labels_action[]        = { "action" };
static const char *labels_source[]        = { "source" };
static const char *labels_coll_op_result[] = { "collection", "op", "result" };
static const char *labels_coll_op[]       = { "collection", "op" };
static const char *labels_kind[]          = { "kind" };
static const char *labels_format_result[] = { "format", "result" };
static const char *labels_format_reason[] = { "format", "reason" };
static const char *labels_kind_slice[]    = { "kind", "slice" };

ogs_metrics_spec_t *chf_metrics_spec_labeled[_CHF_METR_LBL_MAX];
static ogs_hash_t *chf_metrics_hash_labeled = NULL;

static chf_metrics_spec_def_t chf_metrics_spec_def_labeled[_CHF_METR_LBL_MAX] = {
[CHF_METR_LBL_SBI_REQUESTS] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_sbi_requests_total",
    .description = "Nchf_ConvergedCharging requests handled",
    .num_labels = OGS_ARRAY_SIZE(labels_op_result), .labels = labels_op_result, },
[CHF_METR_LBL_SBI_REQ_DURATION] = {
    .type = OGS_METRICS_METRIC_TYPE_HISTOGRAM,
    .name = "chf_sbi_request_duration_milliseconds",
    .description = "Nchf request handling latency in milliseconds",
    .num_labels = OGS_ARRAY_SIZE(labels_op), .labels = labels_op,
    .histogram_params = CHF_HIST_MS_PARAMS, },
[CHF_METR_LBL_SESSION_CREATE] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_session_create_total",
    .description = "CHF sessions created",
    .num_labels = OGS_ARRAY_SIZE(labels_result_dnn_smf),
    .labels = labels_result_dnn_smf, },
[CHF_METR_LBL_SESSION_UPDATE] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_session_update_total",
    .description = "CHF session updates handled",
    .num_labels = OGS_ARRAY_SIZE(labels_result), .labels = labels_result, },
[CHF_METR_LBL_SESSION_RELEASE] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_session_release_total",
    .description = "CHF sessions released",
    .num_labels = OGS_ARRAY_SIZE(labels_cause), .labels = labels_cause, },
[CHF_METR_LBL_QUOTA_EXHAUSTED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_quota_exhausted_total",
    .description = "Quota exhaustion events",
    .num_labels = OGS_ARRAY_SIZE(labels_scope_dnn), .labels = labels_scope_dnn, },
[CHF_METR_LBL_QUOTA_FINAL_UNIT] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_quota_final_unit_total",
    .description = "FinalUnitIndication issued",
    .num_labels = OGS_ARRAY_SIZE(labels_action), .labels = labels_action, },
[CHF_METR_LBL_QUOTA_LOOKUP] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_quota_lookup_total",
    .description = "Quota resolution by source (db|default|exempt)",
    .num_labels = OGS_ARRAY_SIZE(labels_source), .labels = labels_source, },
[CHF_METR_LBL_CDR_WRITTEN] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_cdr_written_total",
    .description = "CDRs written, one per record whatever the encodings",
    .num_labels = OGS_ARRAY_SIZE(labels_result), .labels = labels_result, },
[CHF_METR_LBL_CDR_ENCODED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_cdr_encoded_total",
    .description = "CDR encodings written, one per record per format",
    .num_labels = OGS_ARRAY_SIZE(labels_format_result),
    .labels = labels_format_result, },
[CHF_METR_LBL_CDR_FILE_ROLLED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_cdr_file_rolled_total",
    .description = "Aggregated CDR files rolled, by what tripped it",
    .num_labels = OGS_ARRAY_SIZE(labels_format_reason),
    .labels = labels_format_reason, },
[CHF_METR_LBL_BUCKET_SELECTED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_quota_bucket_selected_total",
    .description = "Allowance a session was charged to, by kind and slice",
    .num_labels = OGS_ARRAY_SIZE(labels_kind_slice),
    .labels = labels_kind_slice, },
[CHF_METR_LBL_DB_OPS] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_db_ops_total",
    .description = "MongoDB operations by collection/op/result",
    .num_labels = OGS_ARRAY_SIZE(labels_coll_op_result),
    .labels = labels_coll_op_result, },
[CHF_METR_LBL_DB_OP_DURATION] = {
    .type = OGS_METRICS_METRIC_TYPE_HISTOGRAM,
    .name = "chf_db_op_duration_milliseconds",
    .description = "MongoDB operation latency in milliseconds",
    .num_labels = OGS_ARRAY_SIZE(labels_coll_op), .labels = labels_coll_op,
    .histogram_params = CHF_HIST_MS_PARAMS, },
[CHF_METR_LBL_ERRORS] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "chf_errors_total",
    .description = "Structured CHF errors by kind",
    .num_labels = OGS_ARRAY_SIZE(labels_kind), .labels = labels_kind, },
};

typedef struct chf_metric_key_labeled_s {
    chf_metric_labeled_t t;
    char label[CHF_METR_MAX_LABELS][CHF_METR_LABEL_LEN];
} chf_metric_key_labeled_t;

void chf_metrics_inst_labeled_add(chf_metric_labeled_t t,
        const char *l0, const char *l1, const char *l2, int val)
{
    chf_metric_key_labeled_t key;
    ogs_metrics_inst_t *inst = NULL;
    const char *in[CHF_METR_MAX_LABELS];
    unsigned int i, nlabels;

    /* Assigned rather than brace-initialised: ISO C90 (the Linux build uses
     * -std=gnu89) requires constant initialisers for automatic aggregates. */
    in[0] = l0; in[1] = l1; in[2] = l2;

    if (t < 0 || t >= _CHF_METR_LBL_MAX) {
        ogs_error("Invalid labeled metric [%d]", t);
        return;
    }
    if (!chf_metrics_hash_labeled)
        return; /* metrics not initialised (e.g. unit tests) */

    nlabels = chf_metrics_spec_def_labeled[t].num_labels;

    /* Zero the whole key so unused label slots and struct padding do not
     * make otherwise-identical keys hash differently. */
    memset(&key, 0, sizeof(key));
    key.t = t;
    for (i = 0; i < nlabels && i < CHF_METR_MAX_LABELS; i++)
        ogs_cpystrn(key.label[i], in[i] ? in[i] : "", CHF_METR_LABEL_LEN);

    inst = ogs_hash_get(chf_metrics_hash_labeled, &key, sizeof(key));
    if (!inst) {
        const char *values[CHF_METR_MAX_LABELS];

        /*
         * Cardinality guard.  All labels are meant to be bounded, but a
         * misbehaving peer (or an unexpectedly dynamic dnn/smf value) must not
         * be able to grow this hash — and the Prometheus series count —
         * without limit.  Past the cap we stop creating series and drop the
         * sample rather than risk exhausting memory.
         */
        if (ogs_hash_count(chf_metrics_hash_labeled) >=
                CHF_METR_MAX_LABELED_INSTANCES) {
            static bool warned = false;
            if (!warned) {
                warned = true;
                ogs_warn("CHF metrics: labeled series cap (%d) reached — "
                         "further label combinations are not recorded. "
                         "Check for unbounded dnn/smf label values.",
                         CHF_METR_MAX_LABELED_INSTANCES);
            }
            return;
        }

        for (i = 0; i < CHF_METR_MAX_LABELS; i++)
            values[i] = key.label[i];

        inst = ogs_metrics_inst_new(chf_metrics_spec_labeled[t],
                nlabels, values);
        ogs_assert(inst);

        /*
         * ogs_hash_set() copies the key, so the stack copy is enough and
         * must not be heap-allocated: an ogs_memdup() here would be a leak
         * the table never frees.  This changed with upstream's "make hash
         * tables own their keys"; before it, the allocation was required
         * because the table stored the caller's pointer.
         */
        ogs_hash_set(chf_metrics_hash_labeled, &key, sizeof(key), inst);
    }

    ogs_metrics_inst_add(inst, val);
}

/* ------------------------------------------------------------------ */
/* Spec completeness                                                   */
/* ------------------------------------------------------------------ */

/*
 * Both spec tables are sized by their enum's _MAX and filled with
 * designated initialisers, so adding a metric to the enum and forgetting
 * its spec leaves a zeroed entry — a NULL name and description, which
 * chf_metrics_init_spec() would hand to ogs_metrics_spec_new() as-is.
 * That is a quiet, easy mistake, so it is checkable: this returns the
 * number of malformed entries and tests/chf asserts it is zero.
 *
 * Malformed means a missing name or description, or more labels than the
 * instance key can hold.
 */
int chf_metrics_spec_defects(void)
{
    unsigned int i;
    int defects = 0;

    for (i = 0; i < _CHF_METR_GLOB_MAX; i++) {
        chf_metrics_spec_def_t *d = &chf_metrics_spec_def_global[i];
        if (!d->name || !*d->name || !d->description || !*d->description) {
            ogs_error("CHF metrics: global spec %u has no name/description", i);
            defects++;
        }
        if (d->num_labels) {
            ogs_error("CHF metrics: global spec %s carries %d label(s); "
                      "a label-less metric must have none",
                      d->name ? d->name : "(unnamed)", d->num_labels);
            defects++;
        }
    }

    for (i = 0; i < _CHF_METR_LBL_MAX; i++) {
        chf_metrics_spec_def_t *d = &chf_metrics_spec_def_labeled[i];
        if (!d->name || !*d->name || !d->description || !*d->description) {
            ogs_error("CHF metrics: labeled spec %u has no name/description",
                    i);
            defects++;
        }
        if (d->num_labels < 1 || d->num_labels > CHF_METR_MAX_LABELS) {
            ogs_error("CHF metrics: labeled spec %s has %d label(s), "
                      "outside 1..%d",
                      d->name ? d->name : "(unnamed)", d->num_labels,
                      CHF_METR_MAX_LABELS);
            defects++;
        }
        if (!d->labels) {
            ogs_error("CHF metrics: labeled spec %s has no label names",
                    d->name ? d->name : "(unnamed)");
            defects++;
        }
    }

    return defects;
}

/* ------------------------------------------------------------------ */
/* Lifecycle                                                           */
/* ------------------------------------------------------------------ */
void chf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    unsigned int i;

    ogs_metrics_context_init();

    chf_metrics_init_spec(ctx, chf_metrics_spec_global,
            chf_metrics_spec_def_global, _CHF_METR_GLOB_MAX);
    chf_metrics_init_spec(ctx, chf_metrics_spec_labeled,
            chf_metrics_spec_def_labeled, _CHF_METR_LBL_MAX);

    for (i = 0; i < _CHF_METR_GLOB_MAX; i++)
        chf_metrics_inst_global[i] =
            ogs_metrics_inst_new(chf_metrics_spec_global[i], 0, NULL);

    chf_metrics_hash_labeled = ogs_hash_make();
    ogs_assert(chf_metrics_hash_labeled);
}

unsigned int chf_metrics_labeled_count(void)
{
    if (!chf_metrics_hash_labeled)
        return 0;
    return ogs_hash_count(chf_metrics_hash_labeled);
}

void chf_metrics_final(void)
{
    unsigned int i;

    if (chf_metrics_hash_labeled) {
        /*
         * ogs_hash_destroy() releases the table's own key copies, and the
         * metric instances are freed by ogs_metrics_context_final() below,
         * so there is nothing left to walk.
         *
         * This used to iterate the table, take each key from
         * ogs_hash_this_key() and ogs_free() it.  That was correct while the
         * table stored the caller's pointer.  Once upstream made hash tables
         * own their keys, ogs_hash_this_key() began handing out the table's
         * copy, and the removal already frees it - so the loop freed it a
         * second time, corrupting the heap on CHF shutdown.
         */
        ogs_hash_destroy(chf_metrics_hash_labeled);
        chf_metrics_hash_labeled = NULL;
    }

    /*
     * ogs_metrics_context_final() frees the instances, so the pointers this
     * array holds are about to dangle.  The guards in metrics.h treat
     * non-NULL as "live", which is what makes instrumentation a no-op before
     * init - so leaving stale pointers here would turn any instrumentation
     * after final into a use-after-free.  The daemon never does that, since
     * final is shutdown, but a process that re-initialises (or a test that
     * exercises this function) would.  Leave the module exactly as it was
     * before init.
     */
    for (i = 0; i < _CHF_METR_GLOB_MAX; i++)
        chf_metrics_inst_global[i] = NULL;

    /* The specs are freed by ogs_metrics_context_final() too.  Nothing reads
     * these after final - the labeled path stops at the NULL hash above -
     * but "exactly as it was before init" should be true, not nearly true. */
    for (i = 0; i < _CHF_METR_GLOB_MAX; i++)
        chf_metrics_spec_global[i] = NULL;
    for (i = 0; i < _CHF_METR_LBL_MAX; i++)
        chf_metrics_spec_labeled[i] = NULL;

    ogs_metrics_context_final();
}
