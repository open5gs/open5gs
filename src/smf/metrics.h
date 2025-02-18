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
    SMF_METR_GLOB_CTR_SM_N4SESSIONESTABREQ,
    SMF_METR_GLOB_CTR_SM_N4SESSIONREPORT,
    SMF_METR_GLOB_CTR_SM_N4SESSIONREPORTSUCC,
    SMF_METR_GLOB_GAUGE_UES_ACTIVE,
    SMF_METR_GLOB_GAUGE_BEARERS_ACTIVE,
    SMF_METR_GLOB_GAUGE_GTP1_PDPCTXS_ACTIVE,
    SMF_METR_GLOB_GAUGE_GTP2_SESSIONS_ACTIVE,
    SMF_METR_GLOB_GAUGE_GTP_PEERS_ACTIVE,
    SMF_METR_GLOB_GAUGE_PFCP_SESSIONS_ACTIVE,
    SMF_METR_GLOB_GAUGE_PFCP_PEERS_ACTIVE,
    _SMF_METR_GLOB_MAX,
} smf_metric_type_global_t;
extern ogs_metrics_inst_t *smf_metrics_inst_global[_SMF_METR_GLOB_MAX];
int smf_metrics_init_inst_global(void);
int smf_metrics_free_inst_global(void);

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

/* BY SLICE */
typedef enum smf_metric_type_by_slice_s {
    SMF_METR_GAUGE_SM_SESSIONNBR = 0,
    SMF_METR_CTR_SM_PDUSESSIONCREATIONREQ,
    SMF_METR_CTR_SM_PDUSESSIONCREATIONSUCC,
    _SMF_METR_BY_SLICE_MAX,
} smf_metric_type_by_slice_t;

void smf_metrics_inst_by_slice_add(
    ogs_plmn_id_t *plmn, ogs_s_nssai_t *snssai,
    smf_metric_type_by_slice_t t, int val);

/* BY SLICE and 5QI */
typedef enum smf_metric_type_by_5qi_s {
    SMF_METR_GAUGE_SM_QOSFLOWNBR = 0,
    _SMF_METR_BY_5QI_MAX,
} smf_metric_type_by_5qi_t;

void smf_metrics_inst_by_5qi_add(
    ogs_plmn_id_t *plmn, ogs_s_nssai_t *snssai,
    uint8_t fiveqi, smf_metric_type_by_5qi_t t, int val);

/* BY CAUSE */
typedef enum smf_metric_type_by_cause_s {
    SMF_METR_CTR_SM_N4SESSIONESTABFAIL = 0,
    SMF_METR_CTR_SM_PDUSESSIONCREATIONFAIL,
    _SMF_METR_BY_CAUSE_MAX,
} smf_metric_type_by_cause_t;

void smf_metrics_inst_by_cause_add(
    int cause, smf_metric_type_by_cause_t t, int val);
void smf_metrics_init(void);
void smf_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* SMF_METRICS_H */
