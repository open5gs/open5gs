/*
 * spatial_validity_rm_1.h
 *
 * This data type is defined in the same way as the SpatialValidity data type, but with the OpenAPI nullable property set to true. 
 */

#ifndef _OpenAPI_spatial_validity_rm_1_H_
#define _OpenAPI_spatial_validity_rm_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_spatial_validity_rm_1_s OpenAPI_spatial_validity_rm_1_t;
#include "presence_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_spatial_validity_rm_1_s {
    OpenAPI_list_t* presence_info_list;
};

OpenAPI_spatial_validity_rm_1_t *OpenAPI_spatial_validity_rm_1_create(
    OpenAPI_list_t* presence_info_list
);
void OpenAPI_spatial_validity_rm_1_free(OpenAPI_spatial_validity_rm_1_t *spatial_validity_rm_1);
OpenAPI_spatial_validity_rm_1_t *OpenAPI_spatial_validity_rm_1_parseFromJSON(cJSON *spatial_validity_rm_1JSON);
cJSON *OpenAPI_spatial_validity_rm_1_convertToJSON(OpenAPI_spatial_validity_rm_1_t *spatial_validity_rm_1);
OpenAPI_spatial_validity_rm_1_t *OpenAPI_spatial_validity_rm_1_copy(OpenAPI_spatial_validity_rm_1_t *dst, OpenAPI_spatial_validity_rm_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_spatial_validity_rm_1_H_ */

