/*
 * traffic_data_set_rm.h
 *
 * This data type is defined in the same way as the TrafficDataSet data, but with the OpenAPI nullable property set to true. Removable attributes trafficFilters and ethTrafficFilters and trafficRoutes are defined as nullable in the OpenAPI. 
 */

#ifndef _OpenAPI_traffic_data_set_rm_H_
#define _OpenAPI_traffic_data_set_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_data_set_rm_s OpenAPI_traffic_data_set_rm_t;
#include "eth_flow_description_1.h"
#include "flow_info_1.h"
#include "route_to_location_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_data_set_rm_s {
    char *set_id;
    bool is_traffic_filters_null;
    OpenAPI_list_t *traffic_filters;
    bool is_eth_traffic_filters_null;
    OpenAPI_list_t *eth_traffic_filters;
    bool is_traffic_routes_null;
    OpenAPI_list_t *traffic_routes;
};

OpenAPI_traffic_data_set_rm_t *OpenAPI_traffic_data_set_rm_create(
    char *set_id,
    bool is_traffic_filters_null,
    OpenAPI_list_t *traffic_filters,
    bool is_eth_traffic_filters_null,
    OpenAPI_list_t *eth_traffic_filters,
    bool is_traffic_routes_null,
    OpenAPI_list_t *traffic_routes
);
void OpenAPI_traffic_data_set_rm_free(OpenAPI_traffic_data_set_rm_t *traffic_data_set_rm);
OpenAPI_traffic_data_set_rm_t *OpenAPI_traffic_data_set_rm_parseFromJSON(cJSON *traffic_data_set_rmJSON);
cJSON *OpenAPI_traffic_data_set_rm_convertToJSON(OpenAPI_traffic_data_set_rm_t *traffic_data_set_rm);
OpenAPI_traffic_data_set_rm_t *OpenAPI_traffic_data_set_rm_copy(OpenAPI_traffic_data_set_rm_t *dst, OpenAPI_traffic_data_set_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_data_set_rm_H_ */

