/*
 * positioning_method.h
 *
 * Indicates supported positioning methods.
 */

#ifndef _OpenAPI_positioning_method_H_
#define _OpenAPI_positioning_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "positioning_method_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_positioning_method_s OpenAPI_positioning_method_t;
typedef struct OpenAPI_positioning_method_s {
} OpenAPI_positioning_method_t;

OpenAPI_positioning_method_t *OpenAPI_positioning_method_create(void);
void OpenAPI_positioning_method_free(OpenAPI_positioning_method_t *positioning_method);
OpenAPI_positioning_method_t *OpenAPI_positioning_method_parseFromJSON(cJSON *positioning_methodJSON);
cJSON *OpenAPI_positioning_method_convertToJSON(OpenAPI_positioning_method_t *positioning_method);
OpenAPI_positioning_method_t *OpenAPI_positioning_method_copy(OpenAPI_positioning_method_t *dst, OpenAPI_positioning_method_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_method_H_ */

