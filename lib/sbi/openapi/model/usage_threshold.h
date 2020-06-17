/*
 * usage_threshold.h
 *
 *
 */

#ifndef _OpenAPI_usage_threshold_H_
#define _OpenAPI_usage_threshold_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_threshold_s OpenAPI_usage_threshold_t;
typedef struct OpenAPI_usage_threshold_s {
    int duration;
    long total_volume;
    long downlink_volume;
    long uplink_volume;
} OpenAPI_usage_threshold_t;

OpenAPI_usage_threshold_t *OpenAPI_usage_threshold_create(
    int duration,
    long total_volume,
    long downlink_volume,
    long uplink_volume
    );
void OpenAPI_usage_threshold_free(OpenAPI_usage_threshold_t *usage_threshold);
OpenAPI_usage_threshold_t *OpenAPI_usage_threshold_parseFromJSON(cJSON *usage_thresholdJSON);
cJSON *OpenAPI_usage_threshold_convertToJSON(OpenAPI_usage_threshold_t *usage_threshold);
OpenAPI_usage_threshold_t *OpenAPI_usage_threshold_copy(OpenAPI_usage_threshold_t *dst, OpenAPI_usage_threshold_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_threshold_H_ */

