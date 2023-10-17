/*
 * wireline_service_area_restriction_1.h
 *
 * The \&quot;restrictionType\&quot; attribute and the \&quot;areas\&quot; attribute shall be either both present or absent.  The empty array of areas is used when service is allowed/restricted nowhere. 
 */

#ifndef _OpenAPI_wireline_service_area_restriction_1_H_
#define _OpenAPI_wireline_service_area_restriction_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "restriction_type.h"
#include "wireline_area_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wireline_service_area_restriction_1_s OpenAPI_wireline_service_area_restriction_1_t;
typedef struct OpenAPI_wireline_service_area_restriction_1_s {
    OpenAPI_restriction_type_e restriction_type;
    OpenAPI_list_t *areas;
} OpenAPI_wireline_service_area_restriction_1_t;

OpenAPI_wireline_service_area_restriction_1_t *OpenAPI_wireline_service_area_restriction_1_create(
    OpenAPI_restriction_type_e restriction_type,
    OpenAPI_list_t *areas
);
void OpenAPI_wireline_service_area_restriction_1_free(OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction_1);
OpenAPI_wireline_service_area_restriction_1_t *OpenAPI_wireline_service_area_restriction_1_parseFromJSON(cJSON *wireline_service_area_restriction_1JSON);
cJSON *OpenAPI_wireline_service_area_restriction_1_convertToJSON(OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction_1);
OpenAPI_wireline_service_area_restriction_1_t *OpenAPI_wireline_service_area_restriction_1_copy(OpenAPI_wireline_service_area_restriction_1_t *dst, OpenAPI_wireline_service_area_restriction_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wireline_service_area_restriction_1_H_ */

