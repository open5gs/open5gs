/*
 * retainability_threshold.h
 *
 * Represents a QoS flow retainability threshold.
 */

#ifndef _OpenAPI_retainability_threshold_H_
#define _OpenAPI_retainability_threshold_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "time_unit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_retainability_threshold_s OpenAPI_retainability_threshold_t;
typedef struct OpenAPI_retainability_threshold_s {
    bool is_rel_flow_num;
    int rel_flow_num;
    struct OpenAPI_time_unit_s *rel_time_unit;
    bool is_rel_flow_ratio;
    int rel_flow_ratio;
} OpenAPI_retainability_threshold_t;

OpenAPI_retainability_threshold_t *OpenAPI_retainability_threshold_create(
    bool is_rel_flow_num,
    int rel_flow_num,
    OpenAPI_time_unit_t *rel_time_unit,
    bool is_rel_flow_ratio,
    int rel_flow_ratio
);
void OpenAPI_retainability_threshold_free(OpenAPI_retainability_threshold_t *retainability_threshold);
OpenAPI_retainability_threshold_t *OpenAPI_retainability_threshold_parseFromJSON(cJSON *retainability_thresholdJSON);
cJSON *OpenAPI_retainability_threshold_convertToJSON(OpenAPI_retainability_threshold_t *retainability_threshold);
OpenAPI_retainability_threshold_t *OpenAPI_retainability_threshold_copy(OpenAPI_retainability_threshold_t *dst, OpenAPI_retainability_threshold_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_retainability_threshold_H_ */

