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
                src[i].initial_val, src[i].num_labels, src[i].labels);
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
};
static int amf_metrics_init_inst_global(void)
{
    return amf_metrics_init_inst(amf_metrics_inst_global, amf_metrics_spec_global,
                _AMF_METR_GLOB_MAX, 0, NULL);
}
static int amf_metrics_free_inst_global(void)
{
    return amf_metrics_free_inst(amf_metrics_inst_global, _AMF_METR_GLOB_MAX);
}

int amf_metrics_open(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_open(ctx);

    amf_metrics_init_spec(ctx, amf_metrics_spec_global, amf_metrics_spec_def_global,
            _AMF_METR_GLOB_MAX);

    amf_metrics_init_inst_global();
    return 0;
}

int amf_metrics_close(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    amf_metrics_free_inst_global();
    ogs_metrics_context_close(ctx);
    return OGS_OK;
}