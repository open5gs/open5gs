/*
 * spatial_validity_cond.h
 *
 * Contains the Spatial Validity Condition.
 */

#ifndef _OpenAPI_spatial_validity_cond_H_
#define _OpenAPI_spatial_validity_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geo_service_area.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_spatial_validity_cond_s OpenAPI_spatial_validity_cond_t;
typedef struct OpenAPI_spatial_validity_cond_s {
    OpenAPI_list_t *tracking_area_list;
    OpenAPI_list_t *countries;
    struct OpenAPI_geo_service_area_s *geographical_service_area;
} OpenAPI_spatial_validity_cond_t;

OpenAPI_spatial_validity_cond_t *OpenAPI_spatial_validity_cond_create(
    OpenAPI_list_t *tracking_area_list,
    OpenAPI_list_t *countries,
    OpenAPI_geo_service_area_t *geographical_service_area
);
void OpenAPI_spatial_validity_cond_free(OpenAPI_spatial_validity_cond_t *spatial_validity_cond);
OpenAPI_spatial_validity_cond_t *OpenAPI_spatial_validity_cond_parseFromJSON(cJSON *spatial_validity_condJSON);
cJSON *OpenAPI_spatial_validity_cond_convertToJSON(OpenAPI_spatial_validity_cond_t *spatial_validity_cond);
OpenAPI_spatial_validity_cond_t *OpenAPI_spatial_validity_cond_copy(OpenAPI_spatial_validity_cond_t *dst, OpenAPI_spatial_validity_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_spatial_validity_cond_H_ */

