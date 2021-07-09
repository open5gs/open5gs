/*
 * route_to_location.h
 *
 * 
 */

#ifndef _OpenAPI_route_to_location_H_
#define _OpenAPI_route_to_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "route_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_route_to_location_s OpenAPI_route_to_location_t;
typedef struct OpenAPI_route_to_location_s {
    char *dnai;
    struct OpenAPI_route_information_s *route_info;
    char *route_prof_id;
} OpenAPI_route_to_location_t;

OpenAPI_route_to_location_t *OpenAPI_route_to_location_create(
    char *dnai,
    OpenAPI_route_information_t *route_info,
    char *route_prof_id
);
void OpenAPI_route_to_location_free(OpenAPI_route_to_location_t *route_to_location);
OpenAPI_route_to_location_t *OpenAPI_route_to_location_parseFromJSON(cJSON *route_to_locationJSON);
cJSON *OpenAPI_route_to_location_convertToJSON(OpenAPI_route_to_location_t *route_to_location);
OpenAPI_route_to_location_t *OpenAPI_route_to_location_copy(OpenAPI_route_to_location_t *dst, OpenAPI_route_to_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_route_to_location_H_ */

