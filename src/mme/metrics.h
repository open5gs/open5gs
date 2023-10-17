#ifndef MME_METRICS_H
#define MME_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GLOBAL */
typedef enum mme_metric_type_global_s {
    MME_METR_GLOB_GAUGE_ENB_UE,
    MME_METR_GLOB_GAUGE_MME_SESS,
    MME_METR_GLOB_GAUGE_ENB,
    _MME_METR_GLOB_MAX,
} mme_metric_type_global_t;
extern ogs_metrics_inst_t *mme_metrics_inst_global[_MME_METR_GLOB_MAX];

int mme_metrics_init_inst_global(void);
int mme_metrics_free_inst_global(void);

static inline void mme_metrics_inst_global_set(mme_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(mme_metrics_inst_global[t], val); }
static inline void mme_metrics_inst_global_add(mme_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(mme_metrics_inst_global[t], val); }
static inline void mme_metrics_inst_global_inc(mme_metric_type_global_t t)
{ ogs_metrics_inst_inc(mme_metrics_inst_global[t]); }
static inline void mme_metrics_inst_global_dec(mme_metric_type_global_t t)
{ ogs_metrics_inst_dec(mme_metrics_inst_global[t]); }

void mme_metrics_init(void);
void mme_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* MME_METRICS_H */
