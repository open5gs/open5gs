#include "ogs-app.h"
#include "context.h"

#include "metrics.h"

typedef struct amf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
    ogs_metrics_histogram_params_t histogram_params;
} amf_metrics_spec_def_t;

/* Helper generic functions: */
static int amf_metrics_init_inst(ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs,
        unsigned int len, unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int amf_metrics_free_inst(ogs_metrics_inst_t **inst,
        unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        ogs_metrics_inst_free(inst[i]);
    memset(inst, 0, sizeof(inst[0]) * len);
    return OGS_OK;
}

static int amf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, amf_metrics_spec_def_t *src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val, src[i].num_labels, src[i].labels,
                &src[i].histogram_params);
    }

    return OGS_OK;
}

/* GLOBAL */
ogs_metrics_spec_t *amf_metrics_spec_global[_AMF_METR_GLOB_MAX];
ogs_metrics_inst_t *amf_metrics_inst_global[_AMF_METR_GLOB_MAX];
amf_metrics_spec_def_t amf_metrics_spec_def_global[_AMF_METR_GLOB_MAX] = {
/* Global Gauges: */
[AMF_METR_GLOB_GAUGE_RAN_UE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "ran_ue",
    .description = "RAN UEs",
},
[AMF_METR_GLOB_GAUGE_AMF_SESS] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "amf_session",
    .description = "AMF Sessions",
},
[AMF_METR_GLOB_GAUGE_GNB] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gnb",
    .description = "gNodeBs",
},
/* Global Counters: */
[AMF_METR_GLOB_CTR_RM_REG_INIT_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_reginitreq",
    .description = "Number of initial registration requests received by the AMF",
},
[AMF_METR_GLOB_CTR_RM_REG_INIT_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_reginitsucc",
    .description = "Number of successful initial registrations at the AMF",
},
[AMF_METR_GLOB_CTR_RM_REG_MOB_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_regmobreq",
    .description = "Number of mobility registration update requests received by the AMF",
},
[AMF_METR_GLOB_CTR_RM_REG_MOB_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_regmobsucc",
    .description = "Number of successful mobility registration updates at the AMF",
},
[AMF_METR_GLOB_CTR_RM_REG_PERIOD_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_regperiodreq",
    .description = "Number of periodic registration update requests received by the AMF",
},
[AMF_METR_GLOB_CTR_RM_REG_PERIOD_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_regperiodsucc",
    .description = "Number of successful periodic registration update requests at the AMF",
},
[AMF_METR_GLOB_CTR_RM_REG_EMERG_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_regemergreq",
    .description = "Number of emergency registration requests received by the AMF",
},
[AMF_METR_GLOB_CTR_RM_REG_EMERG_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_rm_regemergsucc",
    .description = "Number of successful emergency registrations at the AMF",
},
[AMF_METR_GLOB_CTR_MM_PAGING_5G_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_mm_paging5greq",
    .description = "Number of 5G paging procedures initiated at the AMF",
},
[AMF_METR_GLOB_CTR_MM_PAGING_5G_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_mm_paging5gsucc",
    .description = "Number of successful 5G paging procedures initiated at the AMF",
},
[AMF_METR_GLOB_CTR_AMF_AUTH_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_amf_authreq",
    .description = "Number of authentication requests sent by the AMF",
},
[AMF_METR_GLOB_CTR_AMF_AUTH_REJECT] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_amf_authreject",
    .description = "Number of authentication rejections sent by the AMF",
},
[AMF_METR_GLOB_CTR_MM_CONF_UPDATE] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_mm_confupdate",
    .description = "Number of UE Configuration Update commands requested by the AMF",
},
[AMF_METR_GLOB_CTR_MM_CONF_UPDATE_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_amffunction_mm_confupdatesucc",
    .description = "Number of UE Configuration Update complete messages received by the AMF",
},
/* Global Histograms: */
[AMF_METR_GLOB_HIST_REG_TIME] = {
    .type = OGS_METRICS_METRIC_TYPE_HISTOGRAM,
    .name = "fivegs_amffunction_rm_regtime",
    .description = "Time of registration procedure",
    .histogram_params = {
        .type = OGS_METRICS_HISTOGRAM_BUCKET_TYPE_EXPONENTIAL,
        .count = 8,
        .exp.start = 20,
        .exp.factor = 2,
    },
},
};
int amf_metrics_init_inst_global(void)
{
    return amf_metrics_init_inst(amf_metrics_inst_global, amf_metrics_spec_global,
                _AMF_METR_GLOB_MAX, 0, NULL);
}
int amf_metrics_free_inst_global(void)
{
    return amf_metrics_free_inst(amf_metrics_inst_global, _AMF_METR_GLOB_MAX);
}

/* BY SLICE */
const char *labels_slice[] = {
    "plmnid",
    "snssai"
};

#define AMF_METR_BY_SLICE_GAUGE_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_GAUGE, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_slice), \
        .labels = labels_slice, \
    },
ogs_metrics_spec_t *amf_metrics_spec_by_slice[_AMF_METR_BY_SLICE_MAX];
ogs_hash_t *metrics_hash_by_slice = NULL;   /* hash table for SLICE labels */
amf_metrics_spec_def_t amf_metrics_spec_def_by_slice[_AMF_METR_BY_SLICE_MAX] = {
/* Gauges: */
AMF_METR_BY_SLICE_GAUGE_ENTRY(
    AMF_METR_GAUGE_RM_REGISTERED_SUB_NBR,
    "fivegs_amffunction_rm_registeredsubnbr",
    "Number of registered state subscribers per AMF")
};
void amf_metrics_init_by_slice(void);
int amf_metrics_free_inst_by_slice(ogs_metrics_inst_t **inst);
typedef struct amf_metric_key_by_slice_s {
    ogs_plmn_id_t               plmn_id;
    ogs_s_nssai_t               snssai;
    amf_metric_type_by_slice_t  t;
} amf_metric_key_by_slice_t;

void amf_metrics_init_by_slice(void)
{
    metrics_hash_by_slice = ogs_hash_make();
    ogs_assert(metrics_hash_by_slice);
}

void amf_metrics_inst_by_slice_add(ogs_plmn_id_t *plmn,
        ogs_s_nssai_t *snssai, amf_metric_type_by_slice_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    amf_metric_key_by_slice_t *slice_key;

    slice_key = ogs_calloc(1, sizeof(*slice_key));
    ogs_assert(slice_key);

    if (plmn) {
        slice_key->plmn_id = *plmn;
    }

    if (snssai) {
        slice_key->snssai = *snssai;
    } else {
        slice_key->snssai.sst = 0;
        slice_key->snssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    }

    slice_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_slice,
            slice_key, sizeof(*slice_key));

    if (!metrics) {
        char plmn_id[OGS_PLMNIDSTRLEN] = "";
        char *s_nssai = NULL;

        if (plmn) {
            ogs_plmn_id_to_string(plmn, plmn_id);
        }

        if (snssai) {
            s_nssai = ogs_sbi_s_nssai_to_string(snssai);
        } else {
            s_nssai = ogs_strdup("");
        }

        metrics = ogs_metrics_inst_new(amf_metrics_spec_by_slice[t],
                amf_metrics_spec_def_by_slice->num_labels,
                (const char *[]){ plmn_id, s_nssai });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_slice,
                slice_key, sizeof(*slice_key), metrics);

        if (s_nssai)
            ogs_free(s_nssai);
    } else {
        ogs_free(slice_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int amf_metrics_free_inst_by_slice(ogs_metrics_inst_t **inst)
{
    return amf_metrics_free_inst(inst, _AMF_METR_BY_SLICE_MAX);
}

/* BY CAUSE */
const char *labels_cause[] = {
    "cause"
};

#define AMF_METR_BY_CAUSE_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_cause), \
        .labels = labels_cause, \
    },
ogs_metrics_spec_t *amf_metrics_spec_by_cause[_AMF_METR_BY_CAUSE_MAX];
ogs_hash_t *metrics_hash_by_cause = NULL;   /* hash table for CAUSE labels */
amf_metrics_spec_def_t amf_metrics_spec_def_by_cause[_AMF_METR_BY_CAUSE_MAX] = {
/* Counters: */
AMF_METR_BY_CAUSE_CTR_ENTRY(
    AMF_METR_CTR_RM_REG_INIT_FAIL,
    "fivegs_amffunction_rm_reginitfail",
    "Number of failed initial registrations at the AMF")
AMF_METR_BY_CAUSE_CTR_ENTRY(
    AMF_METR_CTR_RM_REG_MOB_FAIL,
    "fivegs_amffunction_rm_regmobfail",
    "Number of failed mobility registration updates at the AMF")
AMF_METR_BY_CAUSE_CTR_ENTRY(
    AMF_METR_CTR_RM_REG_PERIOD_FAIL,
    "fivegs_amffunction_rm_regperiodfail",
    "Number of failed periodic registration update requests at the AMF")
AMF_METR_BY_CAUSE_CTR_ENTRY(
    AMF_METR_CTR_RM_REG_EMERG_FAIL,
    "fivegs_amffunction_rm_regemergfail",
    "Number of failed emergency registrations at the AMF")
AMF_METR_BY_CAUSE_CTR_ENTRY(
    AMF_METR_CTR_AMF_AUTH_FAIL,
    "fivegs_amffunction_amf_authfail",
    "Number of authentication failure messages received by the AMF")
};
void amf_metrics_init_by_cause(void);
int amf_metrics_free_inst_by_cause(ogs_metrics_inst_t **inst);
typedef struct amf_metric_key_by_cause_s {
    uint8_t                     cause;
    amf_metric_type_by_cause_t  t;
} amf_metric_key_by_cause_t;

void amf_metrics_init_by_cause(void)
{
    metrics_hash_by_cause = ogs_hash_make();
    ogs_assert(metrics_hash_by_cause);
}

void amf_metrics_inst_by_cause_add(uint8_t cause,
        amf_metric_type_by_cause_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    amf_metric_key_by_cause_t *cause_key;

    cause_key = ogs_calloc(1, sizeof(*cause_key));
    ogs_assert(cause_key);

    cause_key->cause = cause;
    cause_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_cause,
            cause_key, sizeof(*cause_key));

    if (!metrics) {
        char cause_str[4];
        ogs_snprintf(cause_str, sizeof(cause_str), "%d", cause);

        metrics = ogs_metrics_inst_new(amf_metrics_spec_by_cause[t],
                amf_metrics_spec_def_by_cause->num_labels,
                (const char *[]){ cause_str });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_cause,
                cause_key, sizeof(*cause_key), metrics);
    } else {
        ogs_free(cause_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int amf_metrics_free_inst_by_cause(ogs_metrics_inst_t **inst)
{
    return amf_metrics_free_inst(inst, _AMF_METR_BY_CAUSE_MAX);
}

void amf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_init();

    amf_metrics_init_spec(ctx, amf_metrics_spec_global, amf_metrics_spec_def_global,
            _AMF_METR_GLOB_MAX);

    amf_metrics_init_spec(ctx, amf_metrics_spec_by_slice,
            amf_metrics_spec_def_by_slice, _AMF_METR_BY_SLICE_MAX);
    amf_metrics_init_spec(ctx, amf_metrics_spec_by_cause,
            amf_metrics_spec_def_by_cause, _AMF_METR_BY_CAUSE_MAX);

    amf_metrics_init_inst_global();

    amf_metrics_init_by_slice();
    amf_metrics_init_by_cause();
}

void amf_metrics_final(void)
{
    ogs_hash_index_t *hi;

    if (metrics_hash_by_slice) {
        for (hi = ogs_hash_first(metrics_hash_by_slice); hi; hi = ogs_hash_next(hi)) {
            amf_metric_key_by_slice_t *key =
                (amf_metric_key_by_slice_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_slice, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric ifself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_slice);
    }
    if (metrics_hash_by_cause) {
        for (hi = ogs_hash_first(metrics_hash_by_cause); hi; hi = ogs_hash_next(hi)) {
            amf_metric_key_by_cause_t *key =
                (amf_metric_key_by_cause_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_cause, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric ifself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_cause);
    }

    ogs_metrics_context_final();
}
