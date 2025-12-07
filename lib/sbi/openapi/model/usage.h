/*
 * usage.h
 *
 * Indicates usage made of the location measurement.
 */

#ifndef _OpenAPI_usage_H_
#define _OpenAPI_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "usage_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_s OpenAPI_usage_t;
typedef struct OpenAPI_usage_s {
} OpenAPI_usage_t;

OpenAPI_usage_t *OpenAPI_usage_create(void);
void OpenAPI_usage_free(OpenAPI_usage_t *usage);
OpenAPI_usage_t *OpenAPI_usage_parseFromJSON(cJSON *usageJSON);
cJSON *OpenAPI_usage_convertToJSON(OpenAPI_usage_t *usage);
OpenAPI_usage_t *OpenAPI_usage_copy(OpenAPI_usage_t *dst, OpenAPI_usage_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_H_ */

