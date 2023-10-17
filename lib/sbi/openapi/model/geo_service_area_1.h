/*
 * geo_service_area_1.h
 *
 * List of geographic area or list of civic address info
 */

#ifndef _OpenAPI_geo_service_area_1_H_
#define _OpenAPI_geo_service_area_1_H_

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

typedef struct OpenAPI_geo_service_area_1_s OpenAPI_geo_service_area_1_t;
typedef struct OpenAPI_geo_service_area_1_s {
    OpenAPI_list_t *geographic_area_list;
    OpenAPI_list_t *civic_address_list;
} OpenAPI_geo_service_area_1_t;

OpenAPI_geo_service_area_1_t *OpenAPI_geo_service_area_1_create(
    OpenAPI_list_t *geographic_area_list,
    OpenAPI_list_t *civic_address_list
);
void OpenAPI_geo_service_area_1_free(OpenAPI_geo_service_area_1_t *geo_service_area_1);
OpenAPI_geo_service_area_1_t *OpenAPI_geo_service_area_1_parseFromJSON(cJSON *geo_service_area_1JSON);
cJSON *OpenAPI_geo_service_area_1_convertToJSON(OpenAPI_geo_service_area_1_t *geo_service_area_1);
OpenAPI_geo_service_area_1_t *OpenAPI_geo_service_area_1_copy(OpenAPI_geo_service_area_1_t *dst, OpenAPI_geo_service_area_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geo_service_area_1_H_ */

