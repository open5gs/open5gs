/*
 * spatial_validity.h
 *
 * describes explicitly the route to an Application location
 */

#ifndef _OpenAPI_spatial_validity_H_
#define _OpenAPI_spatial_validity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "presence_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_spatial_validity_s OpenAPI_spatial_validity_t;
typedef struct OpenAPI_spatial_validity_s {
    OpenAPI_list_t* presence_info_list;
} OpenAPI_spatial_validity_t;

OpenAPI_spatial_validity_t *OpenAPI_spatial_validity_create(
    OpenAPI_list_t* presence_info_list
);
void OpenAPI_spatial_validity_free(OpenAPI_spatial_validity_t *spatial_validity);
OpenAPI_spatial_validity_t *OpenAPI_spatial_validity_parseFromJSON(cJSON *spatial_validityJSON);
cJSON *OpenAPI_spatial_validity_convertToJSON(OpenAPI_spatial_validity_t *spatial_validity);
OpenAPI_spatial_validity_t *OpenAPI_spatial_validity_copy(OpenAPI_spatial_validity_t *dst, OpenAPI_spatial_validity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_spatial_validity_H_ */

