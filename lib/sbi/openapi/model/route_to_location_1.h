/*
 * route_to_location_1.h
 *
 * At least one of the \&quot;routeInfo\&quot; attribute and the \&quot;routeProfId\&quot; attribute shall be included in the \&quot;RouteToLocation\&quot; data type. 
 */

#ifndef _OpenAPI_route_to_location_1_H_
#define _OpenAPI_route_to_location_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_route_to_location_1_s OpenAPI_route_to_location_1_t;
#include "route_information_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_route_to_location_1_s {
    char *dnai;
    bool is_route_info_null;
    struct OpenAPI_route_information_1_s *route_info;
    bool is_route_prof_id_null;
    char *route_prof_id;
};

OpenAPI_route_to_location_1_t *OpenAPI_route_to_location_1_create(
    char *dnai,
    bool is_route_info_null,
    OpenAPI_route_information_1_t *route_info,
    bool is_route_prof_id_null,
    char *route_prof_id
);
void OpenAPI_route_to_location_1_free(OpenAPI_route_to_location_1_t *route_to_location_1);
OpenAPI_route_to_location_1_t *OpenAPI_route_to_location_1_parseFromJSON(cJSON *route_to_location_1JSON);
cJSON *OpenAPI_route_to_location_1_convertToJSON(OpenAPI_route_to_location_1_t *route_to_location_1);
OpenAPI_route_to_location_1_t *OpenAPI_route_to_location_1_copy(OpenAPI_route_to_location_1_t *dst, OpenAPI_route_to_location_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_route_to_location_1_H_ */

