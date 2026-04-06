/*
 * traffic_data_set.h
 *
 * Represents a set of traffic filters and the corresponding N6 traffic routing requirements. 
 */

#ifndef _OpenAPI_traffic_data_set_H_
#define _OpenAPI_traffic_data_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_data_set_s OpenAPI_traffic_data_set_t;
#include "eth_flow_description.h"
#include "flow_info.h"
#include "route_to_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_data_set_s {
    char *set_id;
    OpenAPI_list_t *traffic_filters;
    OpenAPI_list_t *eth_traffic_filters;
    OpenAPI_list_t *traffic_routes;
};

OpenAPI_traffic_data_set_t *OpenAPI_traffic_data_set_create(
    char *set_id,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_list_t *traffic_routes
);
void OpenAPI_traffic_data_set_free(OpenAPI_traffic_data_set_t *traffic_data_set);
OpenAPI_traffic_data_set_t *OpenAPI_traffic_data_set_parseFromJSON(cJSON *traffic_data_setJSON);
cJSON *OpenAPI_traffic_data_set_convertToJSON(OpenAPI_traffic_data_set_t *traffic_data_set);
OpenAPI_traffic_data_set_t *OpenAPI_traffic_data_set_copy(OpenAPI_traffic_data_set_t *dst, OpenAPI_traffic_data_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_data_set_H_ */

