#include "ogs-app.h"
#include "mme-context.h"

#include "metrics.h"

typedef struct mme_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
} mme_metrics_spec_def_t;

ogs_metrics_spec_t *mme_metrics_spec_global[_MME_METR_GLOB_MAX];
ogs_metrics_inst_t *mme_metrics_inst_global[_MME_METR_GLOB_MAX];
mme_metrics_spec_def_t mme_metrics_spec_def_global[_MME_METR_GLOB_MAX] = {
[MME_METR_GLOB_GAUGE_ENB_UE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "enb_ue",
    .description = "Number of UEs connected to eNodeBs",
    .initial_val = 0,
    .num_labels = 0,
    .labels = NULL,
},
[MME_METR_GLOB_GAUGE_MME_SESS] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "mme_session",
    .description = "MME Sessions",
    .initial_val = 0,
    .num_labels = 0,
    .labels = NULL,
},
[MME_METR_GLOB_GAUGE_ENB] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "enb",
    .description = "eNodeBs",
    .initial_val = 0,
    .num_labels = 0,
    .labels = NULL,
},
};

static int mme_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, mme_metrics_spec_def_t *src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val, src[i].num_labels, src[i].labels);
    }
    return OGS_OK;
}


static int mme_metrics_init_inst_global(ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], 0, NULL);
    return OGS_OK;
}

int mme_metrics_open(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_open(ctx);

    mme_metrics_init_spec(ctx, mme_metrics_spec_global, mme_metrics_spec_def_global,
            _MME_METR_GLOB_MAX);

    mme_metrics_init_inst_global(mme_metrics_inst_global, mme_metrics_spec_global,
            _MME_METR_GLOB_MAX);
    return 0;
}

int mme_metrics_close(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_close(ctx);
    return OGS_OK;
}
