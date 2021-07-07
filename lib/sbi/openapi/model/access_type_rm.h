/*
 * access_type_rm.h
 *
 * 
 */

#ifndef _OpenAPI_access_type_rm_H_
#define _OpenAPI_access_type_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_type_rm_s OpenAPI_access_type_rm_t;
typedef struct OpenAPI_access_type_rm_s {
} OpenAPI_access_type_rm_t;

OpenAPI_access_type_rm_t *OpenAPI_access_type_rm_create(
);
void OpenAPI_access_type_rm_free(OpenAPI_access_type_rm_t *access_type_rm);
OpenAPI_access_type_rm_t *OpenAPI_access_type_rm_parseFromJSON(cJSON *access_type_rmJSON);
cJSON *OpenAPI_access_type_rm_convertToJSON(OpenAPI_access_type_rm_t *access_type_rm);
OpenAPI_access_type_rm_t *OpenAPI_access_type_rm_copy(OpenAPI_access_type_rm_t *dst, OpenAPI_access_type_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_type_rm_H_ */

