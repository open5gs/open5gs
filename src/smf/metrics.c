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

/* Helper generic functions: */
static int smf_metrics_init_inst(ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs,
        unsigned int len, unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int smf_metrics_free_inst(ogs_metrics_inst_t **inst,
        unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        ogs_metrics_inst_free(inst[i]);
    memset(inst, 0, sizeof(inst[0]) * len);
    return OGS_OK;
}

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

/* GLOBAL */
ogs_metrics_spec_t *smf_metrics_spec_global[_SMF_METR_GLOB_MAX];
ogs_metrics_inst_t *smf_metrics_inst_global[_SMF_METR_GLOB_MAX];
smf_metrics_spec_def_t smf_metrics_spec_def_global[_SMF_METR_GLOB_MAX] = {
/* Global Counters: */
[SMF_METR_GLOB_CTR_GTP_NEW_NODE_FAILED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gtp_new_node_failed",
    .description = "Unable to allocate new GTP (peer) Node",
},
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
static int smf_metrics_init_inst_global(void)
{
    return smf_metrics_init_inst(smf_metrics_inst_global, smf_metrics_spec_global,
                _SMF_METR_GLOB_MAX, 0, NULL);
}
static int smf_metrics_free_inst_global(void)
{
    return smf_metrics_free_inst(smf_metrics_inst_global, _SMF_METR_GLOB_MAX);
}

/* GTP NODE: */
const char *labels_gtp_node[] = {
    "addr"
};
#define SMF_METR_GTP_NODE_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_gtp_node), \
        .labels = labels_gtp_node, \
    },
ogs_metrics_spec_t *smf_metrics_spec_gtp_node[_SMF_METR_GTP_NODE_MAX];
smf_metrics_spec_def_t smf_metrics_spec_def_gtp_node[_SMF_METR_GTP_NODE_MAX] = {
/* Global Counters: */
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_GN_RX_PARSE_FAILED,
    "gtp_node_gn_rx_parse_failed",
    "Received GTPv1C messages discarded due to parsing failure")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_GN_RX_CREATEPDPCTXREQ,
    "gtp_node_gn_rx_createpdpcontextreq",
    "Received GTPv1C CreatePDPContextRequest messages")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_GN_RX_DELETEPDPCTXREQ,
    "gtp_node_gn_rx_deletepdpcontextreq",
    "Received GTPv1C DeletePDPContextRequest messages")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_S5C_RX_PARSE_FAILED,
    "gtp_node_s5c_rx_parse_failed",
    "Received GTPv2C messages discarded due to parsing failure")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_S5C_RX_CREATESESSIONREQ,
    "gtp_node_s5c_rx_createsession",
    "Received GTPv2C CreateSessionRequest messages")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_S5C_RX_DELETESESSIONREQ,
    "gtp_node_s5c_rx_deletesession",
    "Received GTPv2C DeleteSessionRequest messages")
};
int smf_metrics_init_inst_gtp_node(ogs_metrics_inst_t **inst, const char *addr)
{
    return smf_metrics_init_inst(inst,
                smf_metrics_spec_gtp_node, _SMF_METR_GTP_NODE_MAX,
                smf_metrics_spec_def_gtp_node->num_labels, (const char *[]){ addr });
}
int smf_metrics_free_inst_gtp_node(ogs_metrics_inst_t **inst)
{
    return smf_metrics_free_inst(inst, _SMF_METR_GTP_NODE_MAX);
}

int smf_metrics_open(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_open(ctx);

    smf_metrics_init_spec(ctx, smf_metrics_spec_global, smf_metrics_spec_def_global,
            _SMF_METR_GLOB_MAX);

    smf_metrics_init_spec(ctx, smf_metrics_spec_gtp_node, smf_metrics_spec_def_gtp_node,
            _SMF_METR_GTP_NODE_MAX);

    smf_metrics_init_inst_global();
    return 0;
}

int smf_metrics_close(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    smf_metrics_free_inst_global();
    ogs_metrics_context_close(ctx);
    return OGS_OK;
}
