/*
 * trigger_category.h
 *
 * 
 */

#ifndef _OpenAPI_trigger_category_H_
#define _OpenAPI_trigger_category_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "trigger_category_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trigger_category_s OpenAPI_trigger_category_t;
typedef struct OpenAPI_trigger_category_s {
} OpenAPI_trigger_category_t;

OpenAPI_trigger_category_t *OpenAPI_trigger_category_create(
);
void OpenAPI_trigger_category_free(OpenAPI_trigger_category_t *trigger_category);
OpenAPI_trigger_category_t *OpenAPI_trigger_category_parseFromJSON(cJSON *trigger_categoryJSON);
cJSON *OpenAPI_trigger_category_convertToJSON(OpenAPI_trigger_category_t *trigger_category);
OpenAPI_trigger_category_t *OpenAPI_trigger_category_copy(OpenAPI_trigger_category_t *dst, OpenAPI_trigger_category_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trigger_category_H_ */

