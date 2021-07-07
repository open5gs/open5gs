/*
 * invalid_param_1.h
 *
 * 
 */

#ifndef _OpenAPI_invalid_param_1_H_
#define _OpenAPI_invalid_param_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_invalid_param_1_s OpenAPI_invalid_param_1_t;
typedef struct OpenAPI_invalid_param_1_s {
    char *param;
    char *reason;
} OpenAPI_invalid_param_1_t;

OpenAPI_invalid_param_1_t *OpenAPI_invalid_param_1_create(
    char *param,
    char *reason
);
void OpenAPI_invalid_param_1_free(OpenAPI_invalid_param_1_t *invalid_param_1);
OpenAPI_invalid_param_1_t *OpenAPI_invalid_param_1_parseFromJSON(cJSON *invalid_param_1JSON);
cJSON *OpenAPI_invalid_param_1_convertToJSON(OpenAPI_invalid_param_1_t *invalid_param_1);
OpenAPI_invalid_param_1_t *OpenAPI_invalid_param_1_copy(OpenAPI_invalid_param_1_t *dst, OpenAPI_invalid_param_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_invalid_param_1_H_ */

