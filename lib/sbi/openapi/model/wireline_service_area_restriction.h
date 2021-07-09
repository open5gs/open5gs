/*
 * wireline_service_area_restriction.h
 *
 * 
 */

#ifndef _OpenAPI_wireline_service_area_restriction_H_
#define _OpenAPI_wireline_service_area_restriction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "restriction_type.h"
#include "wireline_area.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wireline_service_area_restriction_s OpenAPI_wireline_service_area_restriction_t;
typedef struct OpenAPI_wireline_service_area_restriction_s {
    OpenAPI_restriction_type_e restriction_type;
    OpenAPI_list_t *areas;
} OpenAPI_wireline_service_area_restriction_t;

OpenAPI_wireline_service_area_restriction_t *OpenAPI_wireline_service_area_restriction_create(
    OpenAPI_restriction_type_e restriction_type,
    OpenAPI_list_t *areas
);
void OpenAPI_wireline_service_area_restriction_free(OpenAPI_wireline_service_area_restriction_t *wireline_service_area_restriction);
OpenAPI_wireline_service_area_restriction_t *OpenAPI_wireline_service_area_restriction_parseFromJSON(cJSON *wireline_service_area_restrictionJSON);
cJSON *OpenAPI_wireline_service_area_restriction_convertToJSON(OpenAPI_wireline_service_area_restriction_t *wireline_service_area_restriction);
OpenAPI_wireline_service_area_restriction_t *OpenAPI_wireline_service_area_restriction_copy(OpenAPI_wireline_service_area_restriction_t *dst, OpenAPI_wireline_service_area_restriction_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wireline_service_area_restriction_H_ */

