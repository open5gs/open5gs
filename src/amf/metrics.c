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

ogs_metrics_spec_t *amf_metrics_spec_global[_AMF_METR_GLOB_MAX];
ogs_metrics_inst_t *amf_metrics_inst_global[_AMF_METR_GLOB_MAX];
amf_metrics_spec_def_t amf_metrics_spec_def_global[_AMF_METR_GLOB_MAX] = {
[AMF_METR_GLOB_GAUGE_RAN_UE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "ran_ue",
    .description = "RAN UEs",
    .initial_val = 0,
    .num_labels = 0,
    .labels = NULL,
},
[AMF_METR_GLOB_GAUGE_AMF_SESS] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "amf_session",
    .description = "AMF Sessions",
    .initial_val = 0,
    .num_labels = 0,
    .labels = NULL,
},
[AMF_METR_GLOB_GAUGE_GNB] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gnb",
    .description = "gNodeBs",
    .initial_val = 0,
    .num_labels = 0,
    .labels = NULL,
},
};

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


static int amf_metrics_init_inst_global(ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], 0, NULL);
    return OGS_OK;
}

int amf_metrics_open(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_open(ctx);

    amf_metrics_init_spec(ctx, amf_metrics_spec_global, amf_metrics_spec_def_global,
            _AMF_METR_GLOB_MAX);

    amf_metrics_init_inst_global(amf_metrics_inst_global, amf_metrics_spec_global,
            _AMF_METR_GLOB_MAX);
    return 0;
}

int amf_metrics_close(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_close(ctx);
    return OGS_OK;
}
