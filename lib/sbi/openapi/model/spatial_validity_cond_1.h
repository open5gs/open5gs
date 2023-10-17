/*
 * spatial_validity_cond_1.h
 *
 * Contains the Spatial Validity Condition.
 */

#ifndef _OpenAPI_spatial_validity_cond_1_H_
#define _OpenAPI_spatial_validity_cond_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geo_service_area_1.h"
#include "tai_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_spatial_validity_cond_1_s OpenAPI_spatial_validity_cond_1_t;
typedef struct OpenAPI_spatial_validity_cond_1_s {
    OpenAPI_list_t *tracking_area_list;
    OpenAPI_list_t *countries;
    struct OpenAPI_geo_service_area_1_s *geographical_service_area;
} OpenAPI_spatial_validity_cond_1_t;

OpenAPI_spatial_validity_cond_1_t *OpenAPI_spatial_validity_cond_1_create(
    OpenAPI_list_t *tracking_area_list,
    OpenAPI_list_t *countries,
    OpenAPI_geo_service_area_1_t *geographical_service_area
);
void OpenAPI_spatial_validity_cond_1_free(OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond_1);
OpenAPI_spatial_validity_cond_1_t *OpenAPI_spatial_validity_cond_1_parseFromJSON(cJSON *spatial_validity_cond_1JSON);
cJSON *OpenAPI_spatial_validity_cond_1_convertToJSON(OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond_1);
OpenAPI_spatial_validity_cond_1_t *OpenAPI_spatial_validity_cond_1_copy(OpenAPI_spatial_validity_cond_1_t *dst, OpenAPI_spatial_validity_cond_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_spatial_validity_cond_1_H_ */

