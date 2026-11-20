/*
 * api_operation.h
 *
 * 
 */

#ifndef _OpenAPI_api_operation_H_
#define _OpenAPI_api_operation_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_api_operation_s OpenAPI_api_operation_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_api_operation_s {
    char *name;
    char *description;
};

OpenAPI_api_operation_t *OpenAPI_api_operation_create(
    char *name,
    char *description
);
void OpenAPI_api_operation_free(OpenAPI_api_operation_t *api_operation);
OpenAPI_api_operation_t *OpenAPI_api_operation_parseFromJSON(cJSON *api_operationJSON);
cJSON *OpenAPI_api_operation_convertToJSON(OpenAPI_api_operation_t *api_operation);
OpenAPI_api_operation_t *OpenAPI_api_operation_copy(OpenAPI_api_operation_t *dst, OpenAPI_api_operation_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_api_operation_H_ */

