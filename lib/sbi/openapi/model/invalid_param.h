/*
 * invalid_param.h
 *
 * 
 */

#ifndef _OpenAPI_invalid_param_H_
#define _OpenAPI_invalid_param_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_invalid_param_s OpenAPI_invalid_param_t;
typedef struct OpenAPI_invalid_param_s {
    char *param;
    char *reason;
} OpenAPI_invalid_param_t;

OpenAPI_invalid_param_t *OpenAPI_invalid_param_create(
    char *param,
    char *reason
);
void OpenAPI_invalid_param_free(OpenAPI_invalid_param_t *invalid_param);
OpenAPI_invalid_param_t *OpenAPI_invalid_param_parseFromJSON(cJSON *invalid_paramJSON);
cJSON *OpenAPI_invalid_param_convertToJSON(OpenAPI_invalid_param_t *invalid_param);
OpenAPI_invalid_param_t *OpenAPI_invalid_param_copy(OpenAPI_invalid_param_t *dst, OpenAPI_invalid_param_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_invalid_param_H_ */

