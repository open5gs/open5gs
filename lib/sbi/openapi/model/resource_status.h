/*
 * resource_status.h
 *
 * Possible values are - RELEASED - UNCHANGED - UPDATED
 */

#ifndef _OpenAPI_resource_status_H_
#define _OpenAPI_resource_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_resource_status_s OpenAPI_resource_status_t;
typedef struct OpenAPI_resource_status_s {
} OpenAPI_resource_status_t;

OpenAPI_resource_status_t *OpenAPI_resource_status_create(
    );
void OpenAPI_resource_status_free(OpenAPI_resource_status_t *resource_status);
OpenAPI_resource_status_t *OpenAPI_resource_status_parseFromJSON(cJSON *resource_statusJSON);
cJSON *OpenAPI_resource_status_convertToJSON(OpenAPI_resource_status_t *resource_status);
OpenAPI_resource_status_t *OpenAPI_resource_status_copy(OpenAPI_resource_status_t *dst, OpenAPI_resource_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resource_status_H_ */

