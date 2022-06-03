#include "ogs-app.h"
#include "context.h"

#include "metrics.h"

typedef struct smf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
} smf_metrics_spec_def_t;

ogs_metrics_spec_t *smf_metrics_spec_global[_SMF_METR_GLOB_MAX];
ogs_metrics_inst_t *smf_metrics_inst_global[_SMF_METR_GLOB_MAX];
smf_metrics_spec_def_t smf_metrics_spec_def_global[_SMF_METR_GLOB_MAX] = {
/* Global Counters: */
[SMF_METR_GLOB_CTR_GN_RX_PARSE_FAILED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gn_rx_parse_failed",
    .description = "Received GTPv1C messages discarded due to parsing failure",
},
[SMF_METR_GLOB_CTR_GN_RX_CREATEPDPCTXREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gn_rx_createpdpcontextreq",
    .description = "Received GTPv1C CreatePDPContextRequest messages",
},
[SMF_METR_GLOB_CTR_GN_RX_DELETEPDPCTXREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gn_rx_deletepdpcontextreq",
    .description = "Received GTPv1C DeletePDPContextRequest messages",
},
[SMF_METR_GLOB_CTR_S5C_RX_PARSE_FAILED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "s5c_rx_parse_failed",
    .description = "Received GTPv2C messages discarded due to parsing failure",
},
[SMF_METR_GLOB_CTR_S5C_RX_CREATESESSIONREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "s5c_rx_createsession",
    .description = "Received GTPv2C CreateSessionRequest messages",
},
[SMF_METR_GLOB_CTR_S5C_RX_DELETESESSIONREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "s5c_rx_deletesession",
    .description = "Received GTPv2C DeleteSessionRequest messages",
},
/* Global Gauges: */
[SMF_METR_GLOB_GAUGE_UES_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "ues_active",
    .description = "Active User Equipments",
},
[SMF_METR_GLOB_GAUGE_SESSIONS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "sessions_active",
    .description = "Active Sessions",
},
[SMF_METR_GLOB_GAUGE_BEARERS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "bearers_active",
    .description = "Active Bearers",
},
[SMF_METR_GLOB_GAUGE_GTP1_PDPCTXS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gtp1_pdpctxs_active",
    .description = "Active GTPv1 PDP Contexts (GGSN)",
},
[SMF_METR_GLOB_GAUGE_GTP2_SESSIONS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gtp2_sessions_active",
    .description = "Active GTPv2 Sessions (PGW)",
},
[SMF_METR_GLOB_GAUGE_GTP_PEERS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gtp_peers_active",
    .description = "Active GTP peers",
},
};

const char *labels_pdpctx[] = {
    "apn"
};

static int smf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, smf_metrics_spec_def_t *src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val, src[i].num_labels, src[i].labels);
    }
    return OGS_OK;
}


static int smf_metrics_init_inst_global(ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], 0, NULL);
    return OGS_OK;
}

int smf_metrics_open(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_open(ctx);

    smf_metrics_init_spec(ctx, smf_metrics_spec_global, smf_metrics_spec_def_global,
            _SMF_METR_GLOB_MAX);

    smf_metrics_init_inst_global(smf_metrics_inst_global, smf_metrics_spec_global,
            _SMF_METR_GLOB_MAX);
    return 0;
}

int smf_metrics_close(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_close(ctx);
    return OGS_OK;
}
