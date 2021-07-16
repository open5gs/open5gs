/*
 * usage_threshold_rm.h
 *
 * 
 */

#ifndef _OpenAPI_usage_threshold_rm_H_
#define _OpenAPI_usage_threshold_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_threshold_rm_s OpenAPI_usage_threshold_rm_t;
typedef struct OpenAPI_usage_threshold_rm_s {
    bool is_duration;
    int duration;
    bool is_total_volume;
    long total_volume;
    bool is_downlink_volume;
    long downlink_volume;
    bool is_uplink_volume;
    long uplink_volume;
} OpenAPI_usage_threshold_rm_t;

OpenAPI_usage_threshold_rm_t *OpenAPI_usage_threshold_rm_create(
    bool is_duration,
    int duration,
    bool is_total_volume,
    long total_volume,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_uplink_volume,
    long uplink_volume
);
void OpenAPI_usage_threshold_rm_free(OpenAPI_usage_threshold_rm_t *usage_threshold_rm);
OpenAPI_usage_threshold_rm_t *OpenAPI_usage_threshold_rm_parseFromJSON(cJSON *usage_threshold_rmJSON);
cJSON *OpenAPI_usage_threshold_rm_convertToJSON(OpenAPI_usage_threshold_rm_t *usage_threshold_rm);
OpenAPI_usage_threshold_rm_t *OpenAPI_usage_threshold_rm_copy(OpenAPI_usage_threshold_rm_t *dst, OpenAPI_usage_threshold_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_threshold_rm_H_ */

