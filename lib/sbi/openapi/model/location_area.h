/*
 * location_area.h
 *
 * 
 */

#ifndef _OpenAPI_location_area_H_
#define _OpenAPI_location_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "civic_address.h"
#include "geographic_area.h"
#include "network_area_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_location_area_s OpenAPI_location_area_t;
typedef struct OpenAPI_location_area_s {
    OpenAPI_list_t *geographic_areas;
    OpenAPI_list_t *civic_addresses;
    struct OpenAPI_network_area_info_s *nw_area_info;
} OpenAPI_location_area_t;

OpenAPI_location_area_t *OpenAPI_location_area_create(
    OpenAPI_list_t *geographic_areas,
    OpenAPI_list_t *civic_addresses,
    OpenAPI_network_area_info_t *nw_area_info
);
void OpenAPI_location_area_free(OpenAPI_location_area_t *location_area);
OpenAPI_location_area_t *OpenAPI_location_area_parseFromJSON(cJSON *location_areaJSON);
cJSON *OpenAPI_location_area_convertToJSON(OpenAPI_location_area_t *location_area);
OpenAPI_location_area_t *OpenAPI_location_area_copy(OpenAPI_location_area_t *dst, OpenAPI_location_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_area_H_ */

