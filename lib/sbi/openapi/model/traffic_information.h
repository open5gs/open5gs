/*
 * traffic_information.h
 *
 * Traffic information including UL/DL data rate and/or Traffic volume.
 */

#ifndef _OpenAPI_traffic_information_H_
#define _OpenAPI_traffic_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_information_s OpenAPI_traffic_information_t;
typedef struct OpenAPI_traffic_information_s {
    char *uplink_rate;
    char *downlink_rate;
    bool is_uplink_volume;
    long uplink_volume;
    bool is_downlink_volume;
    long downlink_volume;
    bool is_total_volume;
    long total_volume;
} OpenAPI_traffic_information_t;

OpenAPI_traffic_information_t *OpenAPI_traffic_information_create(
    char *uplink_rate,
    char *downlink_rate,
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_total_volume,
    long total_volume
);
void OpenAPI_traffic_information_free(OpenAPI_traffic_information_t *traffic_information);
OpenAPI_traffic_information_t *OpenAPI_traffic_information_parseFromJSON(cJSON *traffic_informationJSON);
cJSON *OpenAPI_traffic_information_convertToJSON(OpenAPI_traffic_information_t *traffic_information);
OpenAPI_traffic_information_t *OpenAPI_traffic_information_copy(OpenAPI_traffic_information_t *dst, OpenAPI_traffic_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_information_H_ */

