#include "ogs-app.h"
#include "context.h"

#include "metrics.h"

typedef struct pcf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
} pcf_metrics_spec_def_t;

/* Helper generic functions: */
static int pcf_metrics_init_inst(ogs_metrics_inst_t **inst,
        ogs_metrics_spec_t **specs, unsigned int len,
        unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int pcf_metrics_free_inst(ogs_metrics_inst_t **inst,
        unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        ogs_metrics_inst_free(inst[i]);
    memset(inst, 0, sizeof(inst[0]) * len);
    return OGS_OK;
}

static int pcf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, pcf_metrics_spec_def_t *src, unsigned int len)
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
ogs_metrics_spec_t *pcf_metrics_spec_global[_PCF_METR_GLOB_MAX];
ogs_metrics_inst_t *pcf_metrics_inst_global[_PCF_METR_GLOB_MAX];
pcf_metrics_spec_def_t pcf_metrics_spec_def_global[_PCF_METR_GLOB_MAX] = {
/* Global Counters: */
/* Global Gauges: */
};
int pcf_metrics_init_inst_global(void)
{
    return pcf_metrics_init_inst(pcf_metrics_inst_global,
            pcf_metrics_spec_global, _PCF_METR_GLOB_MAX, 0, NULL);
}
int pcf_metrics_free_inst_global(void)
{
    return pcf_metrics_free_inst(pcf_metrics_inst_global, _PCF_METR_GLOB_MAX);
}

/* BY_SLICE */
const char *labels_slice[] = {
    "plmnid",
    "snssai"
};

#define PCF_METR_BY_SLICE_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_slice), \
        .labels = labels_slice, \
    },
#define PCF_METR_BY_SLICE_GAUGE_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_GAUGE, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_slice), \
        .labels = labels_slice, \
    },
ogs_metrics_spec_t *pcf_metrics_spec_by_slice[_PCF_METR_BY_SLICE_MAX];
ogs_hash_t *metrics_hash_by_slice = NULL;   /* hash table for SLICE labels */
pcf_metrics_spec_def_t pcf_metrics_spec_def_by_slice[_PCF_METR_BY_SLICE_MAX] = {
/* Counters: */
PCF_METR_BY_SLICE_CTR_ENTRY(
    PCF_METR_CTR_PA_POLICYAMASSOREQ,
    "fivegs_pcffunction_pa_policyamassoreq",
    "Number of AM policy association requests")
PCF_METR_BY_SLICE_CTR_ENTRY(
    PCF_METR_CTR_PA_POLICYAMASSOSUCC,
    "fivegs_pcffunction_pa_policyamassosucc",
    "Number of successful AM policy associations")
PCF_METR_BY_SLICE_CTR_ENTRY(
    PCF_METR_CTR_PA_POLICYSMASSOREQ,
    "fivegs_pcffunction_pa_policysmassoreq",
    "Number of SM policy association requests")
PCF_METR_BY_SLICE_CTR_ENTRY(
    PCF_METR_CTR_PA_POLICYSMASSOSUCC,
    "fivegs_pcffunction_pa_policysmassosucc",
    "Number of successful SM policy associations")
/* Gauges: */
PCF_METR_BY_SLICE_GAUGE_ENTRY(
    PCF_METR_GAUGE_PA_SESSIONNBR,
    "fivegs_pcffunction_pa_sessionnbr",
    "Active Sessions")
};
void pcf_metrics_init_by_slice(void);
int pcf_metrics_free_inst_by_slice(ogs_metrics_inst_t **inst);
typedef struct pcf_metric_key_by_slice_s {
    ogs_plmn_id_t               plmn_id;
    ogs_s_nssai_t               snssai;
    pcf_metric_type_by_slice_t  t;
} pcf_metric_key_by_slice_t;

void pcf_metrics_init_by_slice(void)
{
    metrics_hash_by_slice = ogs_hash_make();
    ogs_assert(metrics_hash_by_slice);
}

void pcf_metrics_inst_by_slice_add(ogs_plmn_id_t *plmn,
        ogs_s_nssai_t *snssai, pcf_metric_type_by_slice_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    pcf_metric_key_by_slice_t *slice_key;

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

        metrics = ogs_metrics_inst_new(pcf_metrics_spec_by_slice[t],
                pcf_metrics_spec_def_by_slice->num_labels,
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

int pcf_metrics_free_inst_by_slice(ogs_metrics_inst_t **inst)
{
    return pcf_metrics_free_inst(inst, _PCF_METR_BY_SLICE_MAX);
}

void pcf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_init();

    pcf_metrics_init_spec(ctx, pcf_metrics_spec_global,
            pcf_metrics_spec_def_global, _PCF_METR_GLOB_MAX);
    pcf_metrics_init_spec(ctx, pcf_metrics_spec_by_slice,
            pcf_metrics_spec_def_by_slice, _PCF_METR_BY_SLICE_MAX);

    pcf_metrics_init_inst_global();
    pcf_metrics_init_by_slice();
}

void pcf_metrics_final(void)
{
    ogs_hash_index_t *hi;

    if (metrics_hash_by_slice) {
        for (hi = ogs_hash_first(metrics_hash_by_slice); hi; hi = ogs_hash_next(hi)) {
            pcf_metric_key_by_slice_t *key =
                (pcf_metric_key_by_slice_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_slice, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric itself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_slice);
    }

    ogs_metrics_context_final();
}
