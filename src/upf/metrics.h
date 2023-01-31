#ifndef UPF_METRICS_H
#define UPF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GLOBAL */
typedef enum upf_metric_type_global_s {
    UPF_METR_GLOB_CTR_GTP_INDATAPKTN3UPF = 0,
    UPF_METR_GLOB_CTR_GTP_OUTDATAPKTN3UPF,
    UPF_METR_GLOB_CTR_SM_N4SESSIONESTABREQ,
    UPF_METR_GLOB_CTR_SM_N4SESSIONREPORT,
    UPF_METR_GLOB_CTR_SM_N4SESSIONREPORTSUCC,
    UPF_METR_GLOB_GAUGE_UPF_SESSIONNBR,
    _UPF_METR_GLOB_MAX,
} upf_metric_type_global_t;
extern ogs_metrics_inst_t *upf_metrics_inst_global[_UPF_METR_GLOB_MAX];

int upf_metrics_init_inst_global(void);
int upf_metrics_free_inst_global(void);

static inline void upf_metrics_inst_global_set(upf_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(upf_metrics_inst_global[t], val); }
static inline void upf_metrics_inst_global_add(upf_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(upf_metrics_inst_global[t], val); }
static inline void upf_metrics_inst_global_inc(upf_metric_type_global_t t)
{ ogs_metrics_inst_inc(upf_metrics_inst_global[t]); }
static inline void upf_metrics_inst_global_dec(upf_metric_type_global_t t)
{ ogs_metrics_inst_dec(upf_metrics_inst_global[t]); }

/* BY QFI */
typedef enum upf_metric_type_by_qfi_s {
    UPF_METR_CTR_GTP_INDATAVOLUMEQOSLEVELN3UPF = 0,
    UPF_METR_CTR_GTP_OUTDATAVOLUMEQOSLEVELN3UPF,
    _UPF_METR_BY_QFI_MAX,
} upf_metric_type_by_qfi_t;

void upf_metrics_inst_by_qfi_add(
    uint8_t qfi, upf_metric_type_by_qfi_t t, int val);

/* BY CAUSE */
typedef enum upf_metric_type_by_cause_s {
    UPF_METR_CTR_SM_N4SESSIONESTABFAIL = 0,
    _UPF_METR_BY_CAUSE_MAX,
} upf_metric_type_by_cause_t;

void upf_metrics_inst_by_cause_add(
    uint8_t cause, upf_metric_type_by_cause_t t, int val);

/* BY DNN */
typedef enum upf_metric_type_by_dnn_s {
    UPF_METR_GAUGE_UPF_QOSFLOWS = 0,
    _UPF_METR_BY_DNN_MAX,
} upf_metric_type_by_dnn_t;

void upf_metrics_inst_by_dnn_add(
    char *dnn, upf_metric_type_by_dnn_t t, int val);

void upf_metrics_init(void);
void upf_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* UPF_METRICS_H */
