/*
 * access_right_status.h
 *
 * Possible values are - FULLY_ALLOWED: The User is fully allowed to access to the channel. - PREVIEW_ALLOWED: The User is preview allowed to access to the channel. - NO_ALLOWED: The User is not allowed to access to the channel. 
 */

#ifndef _OpenAPI_access_right_status_H_
#define _OpenAPI_access_right_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_right_status_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_right_status_s OpenAPI_access_right_status_t;
typedef struct OpenAPI_access_right_status_s {
} OpenAPI_access_right_status_t;

OpenAPI_access_right_status_t *OpenAPI_access_right_status_create(
);
void OpenAPI_access_right_status_free(OpenAPI_access_right_status_t *access_right_status);
OpenAPI_access_right_status_t *OpenAPI_access_right_status_parseFromJSON(cJSON *access_right_statusJSON);
cJSON *OpenAPI_access_right_status_convertToJSON(OpenAPI_access_right_status_t *access_right_status);
OpenAPI_access_right_status_t *OpenAPI_access_right_status_copy(OpenAPI_access_right_status_t *dst, OpenAPI_access_right_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_right_status_H_ */

