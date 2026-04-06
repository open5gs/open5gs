/*
 * geo_location.h
 *
 * 
 */

#ifndef _OpenAPI_geo_location_H_
#define _OpenAPI_geo_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_geo_location_s OpenAPI_geo_location_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_geo_location_s {
    char *value;
};

OpenAPI_geo_location_t *OpenAPI_geo_location_create(
char *value
);
void OpenAPI_geo_location_free(OpenAPI_geo_location_t *geo_location);
OpenAPI_geo_location_t *OpenAPI_geo_location_parseFromJSON(cJSON *geo_locationJSON);
cJSON *OpenAPI_geo_location_convertToJSON(OpenAPI_geo_location_t *geo_location);
OpenAPI_geo_location_t *OpenAPI_geo_location_copy(OpenAPI_geo_location_t *dst, OpenAPI_geo_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geo_location_H_ */

