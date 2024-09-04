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
/* Global Counters: Gx */
[PCRF_METR_GLOB_CTR_Gx_RX_UNKNOWN] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gx_rx_unknown",
    .description = "Received Gx unknown messages",
},
[PCRF_METR_GLOB_CTR_Gx_RX_CCR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gx_rx_ccr",
    .description = "Received Gx CCR messages",
},
[PCRF_METR_GLOB_CTR_Gx_RX_CCR_ERROR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gx_rx_ccr_error",
    .description = "Received Gx CCR messages failed",
},
[PCRF_METR_GLOB_CTR_Gx_RX_RAA] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gx_rx_raa",
    .description = "Received Gx RAA messages",
},
[PCRF_METR_GLOB_CTR_Gx_TX_CCA] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gx_tx_cca",
    .description = "Received Gx RAA messages failed",
},
[PCRF_METR_GLOB_CTR_Gx_TX_RAR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gx_tx_rar",
    .description = "Transmitted Gx RAR messages",
},
[PCRF_METR_GLOB_CTR_Gx_TX_RAR_ERROR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gx_tx_rar_error",
    .description = "Failed to transmit Gx RAR messages",
},
/* Global Counters: Rx */
[PCRF_METR_GLOB_CTR_Rx_RX_UNKNOWN] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_rx_unknown",
    .description = "Received Rx unknown messages",
},
[PCRF_METR_GLOB_CTR_Rx_RX_AAR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_rx_aar",
    .description = "Received Rx AAR messages",
},
[PCRF_METR_GLOB_CTR_Rx_RX_AAR_ERROR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_rx_aar_error",
    .description = "Received Rx AAR messages failed",
},
[PCRF_METR_GLOB_CTR_Rx_RX_ASA] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_rx_asa",
    .description = "Received Rx ASA messages",
},
[PCRF_METR_GLOB_CTR_Rx_RX_STR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_rx_asa_error",
    .description = "Received Rx ASA messages failed",
},
[PCRF_METR_GLOB_CTR_Rx_RX_STR_ERROR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_rx_str_error",
    .description = "Received Rx STR messages failed",
},
[PCRF_METR_GLOB_CTR_Rx_TX_AAA] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_tx_aaa",
    .description = "Transmitted Rx AAA messages",
},
[PCRF_METR_GLOB_CTR_Rx_TX_SAR] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_tx_sar",
    .description = "Transmitted Rx SAR messages",
},
[PCRF_METR_GLOB_CTR_Rx_TX_STA] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "rx_tx_sta",
    .description = "Transmitted Rx STA messages",
},
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
