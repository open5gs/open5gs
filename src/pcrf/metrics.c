#include "ogs-app.h"
#include "pcrf-context.h"

#include "metrics.h"

typedef struct pcrf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
} pcrf_metrics_spec_def_t;

/* Helper generic functions: */
static int pcrf_metrics_init_inst(ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs,
        unsigned int len, unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int pcrf_metrics_free_inst(ogs_metrics_inst_t **inst,
        unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        ogs_metrics_inst_free(inst[i]);
    memset(inst, 0, sizeof(inst[0]) * len);
    return OGS_OK;
}

static int pcrf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, pcrf_metrics_spec_def_t *src, unsigned int len)
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
ogs_metrics_spec_t *pcrf_metrics_spec_global[_PCRF_METR_GLOB_MAX];
ogs_metrics_inst_t *pcrf_metrics_inst_global[_PCRF_METR_GLOB_MAX];
pcrf_metrics_spec_def_t pcrf_metrics_spec_def_global[_PCRF_METR_GLOB_MAX] = {
/* Global Counters: */
/* Global Gauges: */
};
int pcrf_metrics_init_inst_global(void)
{
    return pcrf_metrics_init_inst(pcrf_metrics_inst_global, pcrf_metrics_spec_global,
                _PCRF_METR_GLOB_MAX, 0, NULL);
}
int pcrf_metrics_free_inst_global(void)
{
    return pcrf_metrics_free_inst(pcrf_metrics_inst_global, _PCRF_METR_GLOB_MAX);
}

void pcrf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_init();

    pcrf_metrics_init_spec(ctx, pcrf_metrics_spec_global, pcrf_metrics_spec_def_global,
            _PCRF_METR_GLOB_MAX);

    pcrf_metrics_init_inst_global();
}

void pcrf_metrics_final(void)
{
    ogs_metrics_context_final();
}
