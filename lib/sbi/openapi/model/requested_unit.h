/*
 * requested_unit.h
 *
 * 
 */

#ifndef _OpenAPI_requested_unit_H_
#define _OpenAPI_requested_unit_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_requested_unit_s OpenAPI_requested_unit_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_requested_unit_s {
    bool is_time;
    int time;
    bool is_total_volume;
    long total_volume;
    bool is_uplink_volume;
    long uplink_volume;
    bool is_downlink_volume;
    long downlink_volume;
    bool is_service_specific_units;
    long service_specific_units;
};

OpenAPI_requested_unit_t *OpenAPI_requested_unit_create(
    bool is_time,
    int time,
    bool is_total_volume,
    long total_volume,
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_service_specific_units,
    long service_specific_units
);
void OpenAPI_requested_unit_free(OpenAPI_requested_unit_t *requested_unit);
OpenAPI_requested_unit_t *OpenAPI_requested_unit_parseFromJSON(cJSON *requested_unitJSON);
cJSON *OpenAPI_requested_unit_convertToJSON(OpenAPI_requested_unit_t *requested_unit);
OpenAPI_requested_unit_t *OpenAPI_requested_unit_copy(OpenAPI_requested_unit_t *dst, OpenAPI_requested_unit_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_unit_H_ */

