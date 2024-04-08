#ifndef HSS_METRICS_H
#define HSS_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GLOBAL */
typedef enum hss_metric_type_global_s {
    HSS_METR_GLOB_GAUGE_IMSI,
    HSS_METR_GLOB_GAUGE_IMPI,
    HSS_METR_GLOB_GAUGE_IMPU,
    _HSS_METR_GLOB_MAX,
} hss_metric_type_global_t;
extern ogs_metrics_inst_t *hss_metrics_inst_global[_HSS_METR_GLOB_MAX];

int hss_metrics_init_inst_global(void);
int hss_metrics_free_inst_global(void);

static inline void hss_metrics_inst_global_set(hss_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(hss_metrics_inst_global[t], val); }
static inline void hss_metrics_inst_global_add(hss_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(hss_metrics_inst_global[t], val); }
static inline void hss_metrics_inst_global_inc(hss_metric_type_global_t t)
{ ogs_metrics_inst_inc(hss_metrics_inst_global[t]); }
static inline void hss_metrics_inst_global_dec(hss_metric_type_global_t t)
{ ogs_metrics_inst_dec(hss_metrics_inst_global[t]); }

void hss_metrics_init(void);
void hss_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* HSS_METRICS_H */
