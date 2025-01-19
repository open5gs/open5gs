#include "ogs-app.h"
#include "context.h"

#include "metrics.h"

typedef struct upf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
} upf_metrics_spec_def_t;

/* Helper generic functions: */
static int upf_metrics_init_inst(ogs_metrics_inst_t **inst,
        ogs_metrics_spec_t **specs, unsigned int len,
        unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int upf_metrics_free_inst(ogs_metrics_inst_t **inst,
        unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        ogs_metrics_inst_free(inst[i]);
    memset(inst, 0, sizeof(inst[0]) * len);
    return OGS_OK;
}

static int upf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, upf_metrics_spec_def_t *src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val, src[i].num_labels, src[i].labels,
                NULL);
    }
    return OGS_OK;
}

/* GLOBAL */
ogs_metrics_spec_t *upf_metrics_spec_global[_UPF_METR_GLOB_MAX];
ogs_metrics_inst_t *upf_metrics_inst_global[_UPF_METR_GLOB_MAX];
upf_metrics_spec_def_t upf_metrics_spec_def_global[_UPF_METR_GLOB_MAX] = {
/* Global Counters: */
[UPF_METR_GLOB_CTR_GTP_INDATAPKTN3UPF] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_ep_n3_gtp_indatapktn3upf",
    .description = "Number of incoming GTP data packets on the N3 interface",
},
[UPF_METR_GLOB_CTR_GTP_OUTDATAPKTN3UPF] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_ep_n3_gtp_outdatapktn3upf",
    .description = "Number of outgoing GTP data packets on the N3 interface",
},
[UPF_METR_GLOB_CTR_SM_N4SESSIONESTABREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_upffunction_sm_n4sessionestabreq",
    .description = "Number of requested N4 session establishments",
},
[UPF_METR_GLOB_CTR_SM_N4SESSIONREPORT] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_upffunction_sm_n4sessionreport",
    .description = "Number of requested N4 session reports",
},
[UPF_METR_GLOB_CTR_SM_N4SESSIONREPORTSUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_upffunction_sm_n4sessionreportsucc",
    .description = "Number of successful N4 session reports",
},
/* Global Gauges: */
[UPF_METR_GLOB_GAUGE_UPF_SESSIONNBR] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "fivegs_upffunction_upf_sessionnbr",
    .description = "Active Sessions",
},
[UPF_METR_GLOB_GAUGE_PFCP_PEERS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "pfcp_peers_active",
    .description = "Active PFCP peers",
},
};
int upf_metrics_init_inst_global(void)
{
    return upf_metrics_init_inst(upf_metrics_inst_global, upf_metrics_spec_global,
                _UPF_METR_GLOB_MAX, 0, NULL);
}
int upf_metrics_free_inst_global(void)
{
    return upf_metrics_free_inst(upf_metrics_inst_global, _UPF_METR_GLOB_MAX);
}

/* BY_QFI */
const char *labels_qfi[] = {
    "qfi"
};
#define UPF_METR_BY_QFI_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_qfi), \
        .labels = labels_qfi, \
    },
ogs_metrics_spec_t *upf_metrics_spec_by_qfi[_UPF_METR_BY_QFI_MAX];
ogs_hash_t *metrics_hash_by_qfi = NULL;   /* hash table for QFI label */
upf_metrics_spec_def_t upf_metrics_spec_def_by_qfi[_UPF_METR_BY_QFI_MAX] = {
/* Counters: */
UPF_METR_BY_QFI_CTR_ENTRY(
    UPF_METR_CTR_GTP_INDATAVOLUMEQOSLEVELN3UPF,
    "fivegs_ep_n3_gtp_indatavolumeqosleveln3upf",
    "Data volume of incoming GTP data packets per QoS level on the N3 interface")
UPF_METR_BY_QFI_CTR_ENTRY(
    UPF_METR_CTR_GTP_OUTDATAVOLUMEQOSLEVELN3UPF,
    "fivegs_ep_n3_gtp_outdatavolumeqosleveln3upf",
    "Data volume of outgoing GTP data packets per QoS level on the N3 interface")
};
void upf_metrics_init_by_qfi(void);
int upf_metrics_free_inst_by_qfi(ogs_metrics_inst_t **inst);
typedef struct upf_metric_key_by_qfi_s {
    uint8_t                     qfi;
    upf_metric_type_by_qfi_t    t;
} upf_metric_key_by_qfi_t;

void upf_metrics_init_by_qfi(void)
{
    metrics_hash_by_qfi = ogs_hash_make();
    ogs_assert(metrics_hash_by_qfi);
}
void upf_metrics_inst_by_qfi_add(uint8_t qfi,
        upf_metric_type_by_qfi_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    upf_metric_key_by_qfi_t *qfi_key;

    qfi_key = ogs_calloc(1, sizeof(*qfi_key));
    ogs_assert(qfi_key);

    qfi_key->qfi = qfi;
    qfi_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_qfi,
            qfi_key, sizeof(*qfi_key));

    if (!metrics) {
        char qfi_str[4];
        ogs_snprintf(qfi_str, sizeof(qfi_str), "%d", qfi);

        metrics = ogs_metrics_inst_new(upf_metrics_spec_by_qfi[t],
                upf_metrics_spec_def_by_qfi->num_labels,
                (const char *[]){ qfi_str });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_qfi,
                qfi_key, sizeof(*qfi_key), metrics);
    } else {
        ogs_free(qfi_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int upf_metrics_free_inst_by_qfi(ogs_metrics_inst_t **inst)
{
    return upf_metrics_free_inst(inst, _UPF_METR_BY_QFI_MAX);
}

/* BY_CAUSE */
const char *labels_cause[] = {
    "cause"
};

#define UPF_METR_BY_CAUSE_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_cause), \
        .labels = labels_cause, \
    },
ogs_metrics_spec_t *upf_metrics_spec_by_cause[_UPF_METR_BY_CAUSE_MAX];
ogs_hash_t *metrics_hash_by_cause = NULL;   /* hash table for CAUSE labels */
upf_metrics_spec_def_t upf_metrics_spec_def_by_cause[_UPF_METR_BY_CAUSE_MAX] = {
/* Counters: */
UPF_METR_BY_CAUSE_CTR_ENTRY(
    UPF_METR_CTR_SM_N4SESSIONESTABFAIL,
    "fivegs_upffunction_sm_n4sessionestabfail",
    "Number of failed N4 session establishments")
};
void upf_metrics_init_by_cause(void);
int upf_metrics_free_inst_by_cause(ogs_metrics_inst_t **inst);
typedef struct upf_metric_key_by_cause_s {
    uint8_t                     cause;
    upf_metric_type_by_cause_t  t;
} upf_metric_key_by_cause_t;

void upf_metrics_init_by_cause(void)
{
    metrics_hash_by_cause = ogs_hash_make();
    ogs_assert(metrics_hash_by_cause);
}

void upf_metrics_inst_by_cause_add(uint8_t cause,
        upf_metric_type_by_cause_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    upf_metric_key_by_cause_t *cause_key;

    cause_key = ogs_calloc(1, sizeof(*cause_key));
    ogs_assert(cause_key);

    cause_key->cause = cause;
    cause_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_cause,
            cause_key, sizeof(*cause_key));

    if (!metrics) {
        char cause_str[4];
        ogs_snprintf(cause_str, sizeof(cause_str), "%d", cause);

        metrics = ogs_metrics_inst_new(upf_metrics_spec_by_cause[t],
                upf_metrics_spec_def_by_cause->num_labels,
                (const char *[]){ cause_str });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_cause,
                cause_key, sizeof(*cause_key), metrics);
    } else {
        ogs_free(cause_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int upf_metrics_free_inst_by_cause(ogs_metrics_inst_t **inst)
{
    return upf_metrics_free_inst(inst, _UPF_METR_BY_CAUSE_MAX);
}

/* BY_DNN */
const char *labels_dnn[] = {
    "dnn"
};

#define UPF_METR_BY_DNN_GAUGE_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_GAUGE, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_dnn), \
        .labels = labels_dnn, \
    },
ogs_metrics_spec_t *upf_metrics_spec_by_dnn[_UPF_METR_BY_DNN_MAX];
ogs_hash_t *metrics_hash_by_dnn = NULL;   /* hash table for DNN labels */
upf_metrics_spec_def_t upf_metrics_spec_def_by_dnn[_UPF_METR_BY_DNN_MAX] = {
/* Gauges: */
UPF_METR_BY_DNN_GAUGE_ENTRY(
    UPF_METR_GAUGE_UPF_QOSFLOWS,
    "fivegs_upffunction_upf_qosflows",
    "Number of QoS flows of UPF")
};
void upf_metrics_init_by_dnn(void);
int upf_metrics_free_inst_by_dnn(ogs_metrics_inst_t **inst);
typedef struct upf_metric_key_by_dnn_s {
    char                        dnn[OGS_MAX_DNN_LEN+1];
    upf_metric_type_by_dnn_t    t;
} upf_metric_key_by_dnn_t;

void upf_metrics_init_by_dnn(void)
{
    metrics_hash_by_dnn = ogs_hash_make();
    ogs_assert(metrics_hash_by_dnn);
}

void upf_metrics_inst_by_dnn_add(char *dnn,
        upf_metric_type_by_dnn_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    upf_metric_key_by_dnn_t *dnn_key;

    dnn_key = ogs_calloc(1, sizeof(*dnn_key));
    ogs_assert(dnn_key);

    if (dnn) {
        ogs_cpystrn(dnn_key->dnn, dnn, ogs_min(strlen(dnn), OGS_MAX_DNN_LEN)+1);
    } else {
        dnn_key->dnn[0] = '\0';
    }

    dnn_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_dnn,
            dnn_key, sizeof(*dnn_key));

    if (!metrics) {
        metrics = ogs_metrics_inst_new(upf_metrics_spec_by_dnn[t],
                upf_metrics_spec_def_by_dnn->num_labels,
                (const char *[]){ dnn_key->dnn });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_dnn,
                dnn_key, sizeof(*dnn_key), metrics);
    } else {
        ogs_free(dnn_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int upf_metrics_free_inst_by_dnn(ogs_metrics_inst_t **inst)
{
    return upf_metrics_free_inst(inst, _UPF_METR_BY_DNN_MAX);
}

void upf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_init();

    upf_metrics_init_spec(ctx, upf_metrics_spec_global, upf_metrics_spec_def_global,
            _UPF_METR_GLOB_MAX);
    upf_metrics_init_spec(ctx, upf_metrics_spec_by_qfi,
            upf_metrics_spec_def_by_qfi, _UPF_METR_BY_QFI_MAX);
    upf_metrics_init_spec(ctx, upf_metrics_spec_by_cause,
            upf_metrics_spec_def_by_cause, _UPF_METR_BY_CAUSE_MAX);
    upf_metrics_init_spec(ctx, upf_metrics_spec_by_dnn,
            upf_metrics_spec_def_by_dnn, _UPF_METR_BY_DNN_MAX);

    upf_metrics_init_inst_global();
    upf_metrics_init_by_qfi();
    upf_metrics_init_by_cause();
    upf_metrics_init_by_dnn();
}

void upf_metrics_final(void)
{
    ogs_hash_index_t *hi;

    if (metrics_hash_by_qfi) {
        for (hi = ogs_hash_first(metrics_hash_by_qfi); hi; hi = ogs_hash_next(hi)) {
            upf_metric_key_by_qfi_t *key =
                (upf_metric_key_by_qfi_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_qfi, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric itself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_qfi);
    }
    if (metrics_hash_by_cause) {
        for (hi = ogs_hash_first(metrics_hash_by_cause); hi; hi = ogs_hash_next(hi)) {
            upf_metric_key_by_cause_t *key =
                (upf_metric_key_by_cause_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_cause, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric itself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_cause);
    }
    if (metrics_hash_by_dnn) {
        for (hi = ogs_hash_first(metrics_hash_by_dnn); hi; hi = ogs_hash_next(hi)) {
            upf_metric_key_by_dnn_t *key =
                (upf_metric_key_by_dnn_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_dnn, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric itself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_dnn);
    }

    ogs_metrics_context_final();
}
