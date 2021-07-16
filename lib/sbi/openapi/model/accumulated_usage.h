/*
 * accumulated_usage.h
 *
 * 
 */

#ifndef _OpenAPI_accumulated_usage_H_
#define _OpenAPI_accumulated_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_accumulated_usage_s OpenAPI_accumulated_usage_t;
typedef struct OpenAPI_accumulated_usage_s {
    bool is_duration;
    int duration;
    bool is_total_volume;
    long total_volume;
    bool is_downlink_volume;
    long downlink_volume;
    bool is_uplink_volume;
    long uplink_volume;
} OpenAPI_accumulated_usage_t;

OpenAPI_accumulated_usage_t *OpenAPI_accumulated_usage_create(
    bool is_duration,
    int duration,
    bool is_total_volume,
    long total_volume,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_uplink_volume,
    long uplink_volume
);
void OpenAPI_accumulated_usage_free(OpenAPI_accumulated_usage_t *accumulated_usage);
OpenAPI_accumulated_usage_t *OpenAPI_accumulated_usage_parseFromJSON(cJSON *accumulated_usageJSON);
cJSON *OpenAPI_accumulated_usage_convertToJSON(OpenAPI_accumulated_usage_t *accumulated_usage);
OpenAPI_accumulated_usage_t *OpenAPI_accumulated_usage_copy(OpenAPI_accumulated_usage_t *dst, OpenAPI_accumulated_usage_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accumulated_usage_H_ */

