/*
 * location_area_1.h
 *
 * Contains Location Area
 */

#ifndef _OpenAPI_location_area_1_H_
#define _OpenAPI_location_area_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_location_area_1_s OpenAPI_location_area_1_t;
#include "civic_address.h"
#include "geographic_area.h"
#include "network_area_info_2.h"
#include "umt_time_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_location_area_1_s {
    OpenAPI_list_t *geographic_areas;
    OpenAPI_list_t *civic_addresses;
    struct OpenAPI_network_area_info_2_s *nw_area_info;
    struct OpenAPI_umt_time_1_s *umt_time;
};

OpenAPI_location_area_1_t *OpenAPI_location_area_1_create(
    OpenAPI_list_t *geographic_areas,
    OpenAPI_list_t *civic_addresses,
    OpenAPI_network_area_info_2_t *nw_area_info,
    OpenAPI_umt_time_1_t *umt_time
);
void OpenAPI_location_area_1_free(OpenAPI_location_area_1_t *location_area_1);
OpenAPI_location_area_1_t *OpenAPI_location_area_1_parseFromJSON(cJSON *location_area_1JSON);
cJSON *OpenAPI_location_area_1_convertToJSON(OpenAPI_location_area_1_t *location_area_1);
OpenAPI_location_area_1_t *OpenAPI_location_area_1_copy(OpenAPI_location_area_1_t *dst, OpenAPI_location_area_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_area_1_H_ */

