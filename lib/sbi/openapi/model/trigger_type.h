/*
 * trigger_type.h
 *
 * 
 */

#ifndef _OpenAPI_trigger_type_H_
#define _OpenAPI_trigger_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "trigger_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trigger_type_s OpenAPI_trigger_type_t;
typedef struct OpenAPI_trigger_type_s {
} OpenAPI_trigger_type_t;

OpenAPI_trigger_type_t *OpenAPI_trigger_type_create(
);
void OpenAPI_trigger_type_free(OpenAPI_trigger_type_t *trigger_type);
OpenAPI_trigger_type_t *OpenAPI_trigger_type_parseFromJSON(cJSON *trigger_typeJSON);
cJSON *OpenAPI_trigger_type_convertToJSON(OpenAPI_trigger_type_t *trigger_type);
OpenAPI_trigger_type_t *OpenAPI_trigger_type_copy(OpenAPI_trigger_type_t *dst, OpenAPI_trigger_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trigger_type_H_ */

