/*
 * spatial_validity_rm.h
 *
 * this data type is defined in the same way as the SpatialValidity data type, but with the OpenAPI nullable property set to true
 */

#ifndef _OpenAPI_spatial_validity_rm_H_
#define _OpenAPI_spatial_validity_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "presence_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_spatial_validity_rm_s OpenAPI_spatial_validity_rm_t;
typedef struct OpenAPI_spatial_validity_rm_s {
    OpenAPI_list_t* presence_info_list;
} OpenAPI_spatial_validity_rm_t;

OpenAPI_spatial_validity_rm_t *OpenAPI_spatial_validity_rm_create(
    OpenAPI_list_t* presence_info_list
);
void OpenAPI_spatial_validity_rm_free(OpenAPI_spatial_validity_rm_t *spatial_validity_rm);
OpenAPI_spatial_validity_rm_t *OpenAPI_spatial_validity_rm_parseFromJSON(cJSON *spatial_validity_rmJSON);
cJSON *OpenAPI_spatial_validity_rm_convertToJSON(OpenAPI_spatial_validity_rm_t *spatial_validity_rm);
OpenAPI_spatial_validity_rm_t *OpenAPI_spatial_validity_rm_copy(OpenAPI_spatial_validity_rm_t *dst, OpenAPI_spatial_validity_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_spatial_validity_rm_H_ */

