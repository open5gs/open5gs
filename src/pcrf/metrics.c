#include "metrics.h"

void pcrf_metrics_init(void)
{
    ogs_metrics_context_init();
}

void pcrf_metrics_final(void)
{
    ogs_metrics_context_final();
}
