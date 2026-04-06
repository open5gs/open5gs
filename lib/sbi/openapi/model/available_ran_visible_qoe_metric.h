/*
 * available_ran_visible_qoe_metric.h
 *
 * The enumeration AvailableRanVisibleQoeMetric indicates different available RAN-visible QoE metrics to the gNB. It shall comply with the provisions defined in TS 29.571, table 5.6.3.22-1. 
 */

#ifndef _OpenAPI_available_ran_visible_qoe_metric_H_
#define _OpenAPI_available_ran_visible_qoe_metric_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_available_ran_visible_qoe_metric_NULL = 0, OpenAPI_available_ran_visible_qoe_metric_APPLICATION_LAYER_BUFFER_LEVEL_LIST, OpenAPI_available_ran_visible_qoe_metric_PLAYOUT_DELAY_FOR_MEDIA_STARTUP } OpenAPI_available_ran_visible_qoe_metric_e;

char* OpenAPI_available_ran_visible_qoe_metric_ToString(OpenAPI_available_ran_visible_qoe_metric_e available_ran_visible_qoe_metric);

OpenAPI_available_ran_visible_qoe_metric_e OpenAPI_available_ran_visible_qoe_metric_FromString(char* available_ran_visible_qoe_metric);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_available_ran_visible_qoe_metric_H_ */

