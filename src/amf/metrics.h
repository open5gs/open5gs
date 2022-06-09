#ifndef AMF_METRICS_H
#define AMF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum amf_metric_type_global_s {
    AMF_METR_GLOB_GAUGE_RAN_UE,
    AMF_METR_GLOB_GAUGE_AMF_SESS,
    AMF_METR_GLOB_GAUGE_GNB,
    _AMF_METR_GLOB_MAX,
} amf_metric_type_global_t;
extern ogs_metrics_inst_t *amf_metrics_inst_global[_AMF_METR_GLOB_MAX];

static inline void amf_metrics_inst_global_set(amf_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(amf_metrics_inst_global[t], val); }
static inline void amf_metrics_inst_global_add(amf_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(amf_metrics_inst_global[t], val); }
static inline void amf_metrics_inst_global_inc(amf_metric_type_global_t t)
{ ogs_metrics_inst_inc(amf_metrics_inst_global[t]); }
static inline void amf_metrics_inst_global_dec(amf_metric_type_global_t t)
{ ogs_metrics_inst_dec(amf_metrics_inst_global[t]); }

int amf_metrics_open(void);
int amf_metrics_close(void);

#ifdef __cplusplus
}
#endif

#endif /* AMF_METRICS_H */
