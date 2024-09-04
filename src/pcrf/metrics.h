#ifndef PCRF_METRICS_H
#define PCRF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GLOBAL */
typedef enum pcrf_metric_type_global_s {
    /* TODO: add PCRF specific metrics here. */
    _PCRF_METR_GLOB_MAX,
} pcrf_metric_type_global_t;
extern ogs_metrics_inst_t *pcrf_metrics_inst_global[_PCRF_METR_GLOB_MAX];

int pcrf_metrics_init_inst_global(void);
int pcrf_metrics_free_inst_global(void);

static inline void pcrf_metrics_inst_global_set(pcrf_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(pcrf_metrics_inst_global[t], val); }
static inline void pcrf_metrics_inst_global_add(pcrf_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(pcrf_metrics_inst_global[t], val); }
static inline void pcrf_metrics_inst_global_inc(pcrf_metric_type_global_t t)
{ ogs_metrics_inst_inc(pcrf_metrics_inst_global[t]); }
static inline void pcrf_metrics_inst_global_dec(pcrf_metric_type_global_t t)
{ ogs_metrics_inst_dec(pcrf_metrics_inst_global[t]); }

void pcrf_metrics_init(void);
void pcrf_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* PCRF_METRICS_H */
