/*
 * partial_record_method.h
 *
 *
 */

#ifndef _OpenAPI_partial_record_method_H_
#define _OpenAPI_partial_record_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_partial_record_method_s OpenAPI_partial_record_method_t;
typedef struct OpenAPI_partial_record_method_s {
} OpenAPI_partial_record_method_t;

OpenAPI_partial_record_method_t *OpenAPI_partial_record_method_create(
    );
void OpenAPI_partial_record_method_free(OpenAPI_partial_record_method_t *partial_record_method);
OpenAPI_partial_record_method_t *OpenAPI_partial_record_method_parseFromJSON(cJSON *partial_record_methodJSON);
cJSON *OpenAPI_partial_record_method_convertToJSON(OpenAPI_partial_record_method_t *partial_record_method);
OpenAPI_partial_record_method_t *OpenAPI_partial_record_method_copy(OpenAPI_partial_record_method_t *dst, OpenAPI_partial_record_method_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_partial_record_method_H_ */

