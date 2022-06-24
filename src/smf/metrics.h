#ifndef SMF_METRICS_H
#define SMF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GLOBAL */
typedef enum smf_metric_type_global_s {
    SMF_METR_GLOB_CTR_GTP_NEW_NODE_FAILED = 0,
    SMF_METR_GLOB_CTR_GN_RX_PARSE_FAILED,
    SMF_METR_GLOB_CTR_GN_RX_CREATEPDPCTXREQ,
    SMF_METR_GLOB_CTR_GN_RX_DELETEPDPCTXREQ,
    SMF_METR_GLOB_CTR_S5C_RX_PARSE_FAILED,
    SMF_METR_GLOB_CTR_S5C_RX_CREATESESSIONREQ,
    SMF_METR_GLOB_CTR_S5C_RX_DELETESESSIONREQ,
    SMF_METR_GLOB_GAUGE_UES_ACTIVE,
    SMF_METR_GLOB_GAUGE_SESSIONS_ACTIVE,
    SMF_METR_GLOB_GAUGE_BEARERS_ACTIVE,
    SMF_METR_GLOB_GAUGE_GTP1_PDPCTXS_ACTIVE,
    SMF_METR_GLOB_GAUGE_GTP2_SESSIONS_ACTIVE,
    SMF_METR_GLOB_GAUGE_GTP_PEERS_ACTIVE,
    _SMF_METR_GLOB_MAX,
} smf_metric_type_global_t;
extern ogs_metrics_inst_t *smf_metrics_inst_global[_SMF_METR_GLOB_MAX];

static inline void smf_metrics_inst_global_set(smf_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(smf_metrics_inst_global[t], val); }
static inline void smf_metrics_inst_global_add(smf_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(smf_metrics_inst_global[t], val); }
static inline void smf_metrics_inst_global_inc(smf_metric_type_global_t t)
{ ogs_metrics_inst_inc(smf_metrics_inst_global[t]); }
static inline void smf_metrics_inst_global_dec(smf_metric_type_global_t t)
{ ogs_metrics_inst_dec(smf_metrics_inst_global[t]); }

/* GTP NODE */
typedef enum smf_metric_type_gtp_node_s {
    SMF_METR_GTP_NODE_CTR_GN_RX_PARSE_FAILED = 0,
    SMF_METR_GTP_NODE_CTR_GN_RX_CREATEPDPCTXREQ,
    SMF_METR_GTP_NODE_CTR_GN_RX_DELETEPDPCTXREQ,
    SMF_METR_GTP_NODE_CTR_S5C_RX_PARSE_FAILED,
    SMF_METR_GTP_NODE_CTR_S5C_RX_CREATESESSIONREQ,
    SMF_METR_GTP_NODE_CTR_S5C_RX_DELETESESSIONREQ,
    _SMF_METR_GTP_NODE_MAX,
} smf_metric_type_gtp_node_t;
int smf_metrics_init_inst_gtp_node(ogs_metrics_inst_t **inst, const char *addr);
int smf_metrics_free_inst_gtp_node(ogs_metrics_inst_t **inst);

static inline void smf_metrics_inst_gtp_node_set(
        ogs_metrics_inst_t **inst, smf_metric_type_gtp_node_t t, int val)
{ ogs_metrics_inst_set(inst[t], val); }
static inline void smf_metrics_inst_gtp_node_add(
        ogs_metrics_inst_t **inst, smf_metric_type_gtp_node_t t, int val)
{ ogs_metrics_inst_add(inst[t], val); }
static inline void smf_metrics_inst_gtp_node_inc(
    ogs_metrics_inst_t **inst, smf_metric_type_gtp_node_t t)
{ ogs_metrics_inst_inc(inst[t]); }
static inline void smf_metrics_inst_gtp_node_dec(
    ogs_metrics_inst_t **inst, smf_metric_type_gtp_node_t t)
{ ogs_metrics_inst_dec(inst[t]); }

int smf_metrics_open(void);
int smf_metrics_close(void);

#ifdef __cplusplus
}
#endif

#endif /* SMF_METRICS_H */
