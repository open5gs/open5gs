#ifndef PCF_METRICS_H
#define PCF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum pcf_metric_type_global_s {
    _PCF_METR_GLOB_MAX,
} pcf_metric_type_global_t;
extern ogs_metrics_inst_t *pcf_metrics_inst_global[_PCF_METR_GLOB_MAX];

int pcf_metrics_init_inst_global(void);
int pcf_metrics_free_inst_global(void);

static inline void pcf_metrics_inst_global_set(pcf_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(pcf_metrics_inst_global[t], val); }
static inline void pcf_metrics_inst_global_add(pcf_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(pcf_metrics_inst_global[t], val); }
static inline void pcf_metrics_inst_global_inc(pcf_metric_type_global_t t)
{ ogs_metrics_inst_inc(pcf_metrics_inst_global[t]); }
static inline void pcf_metrics_inst_global_dec(pcf_metric_type_global_t t)
{ ogs_metrics_inst_dec(pcf_metrics_inst_global[t]); }

/* BY_SLICE */
typedef enum pcf_metric_type_by_slice_s {
    PCF_METR_CTR_PA_POLICYAMASSOREQ = 0,
    PCF_METR_CTR_PA_POLICYAMASSOSUCC,
    PCF_METR_CTR_PA_POLICYSMASSOREQ,
    PCF_METR_CTR_PA_POLICYSMASSOSUCC,
    PCF_METR_GAUGE_PA_SESSIONNBR,
    _PCF_METR_BY_SLICE_MAX,
} pcf_metric_type_by_slice_t;

void pcf_metrics_inst_by_slice_add(
    ogs_plmn_id_t *plmn, ogs_s_nssai_t *snssai,
    pcf_metric_type_by_slice_t t, int val);

void pcf_metrics_init(void);
void pcf_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* PCF_METRICS_H */
