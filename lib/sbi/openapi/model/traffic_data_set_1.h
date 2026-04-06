/*
 * traffic_data_set_1.h
 *
 * Represents a set of traffic filters and the corresponding N6 traffic routing requirements. 
 */

#ifndef _OpenAPI_traffic_data_set_1_H_
#define _OpenAPI_traffic_data_set_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_data_set_1_s OpenAPI_traffic_data_set_1_t;
#include "eth_flow_description_1.h"
#include "flow_info_1.h"
#include "route_to_location_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_data_set_1_s {
    char *set_id;
    OpenAPI_list_t *traffic_filters;
    OpenAPI_list_t *eth_traffic_filters;
    OpenAPI_list_t *traffic_routes;
};

OpenAPI_traffic_data_set_1_t *OpenAPI_traffic_data_set_1_create(
    char *set_id,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_list_t *traffic_routes
);
void OpenAPI_traffic_data_set_1_free(OpenAPI_traffic_data_set_1_t *traffic_data_set_1);
OpenAPI_traffic_data_set_1_t *OpenAPI_traffic_data_set_1_parseFromJSON(cJSON *traffic_data_set_1JSON);
cJSON *OpenAPI_traffic_data_set_1_convertToJSON(OpenAPI_traffic_data_set_1_t *traffic_data_set_1);
OpenAPI_traffic_data_set_1_t *OpenAPI_traffic_data_set_1_copy(OpenAPI_traffic_data_set_1_t *dst, OpenAPI_traffic_data_set_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_data_set_1_H_ */

