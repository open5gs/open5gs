/*
 * geo_service_area.h
 *
 * List of geographic area or list of civic address info
 */

#ifndef _OpenAPI_geo_service_area_H_
#define _OpenAPI_geo_service_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "civic_address.h"
#include "geographic_area.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_geo_service_area_s OpenAPI_geo_service_area_t;
typedef struct OpenAPI_geo_service_area_s {
    OpenAPI_list_t *geographic_area_list;
    OpenAPI_list_t *civic_address_list;
} OpenAPI_geo_service_area_t;

OpenAPI_geo_service_area_t *OpenAPI_geo_service_area_create(
    OpenAPI_list_t *geographic_area_list,
    OpenAPI_list_t *civic_address_list
);
void OpenAPI_geo_service_area_free(OpenAPI_geo_service_area_t *geo_service_area);
OpenAPI_geo_service_area_t *OpenAPI_geo_service_area_parseFromJSON(cJSON *geo_service_areaJSON);
cJSON *OpenAPI_geo_service_area_convertToJSON(OpenAPI_geo_service_area_t *geo_service_area);
OpenAPI_geo_service_area_t *OpenAPI_geo_service_area_copy(OpenAPI_geo_service_area_t *dst, OpenAPI_geo_service_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geo_service_area_H_ */

