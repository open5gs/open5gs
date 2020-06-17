/*
 * usage_mon_level.h
 *
 *
 */

#ifndef _OpenAPI_usage_mon_level_H_
#define _OpenAPI_usage_mon_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_mon_level_s OpenAPI_usage_mon_level_t;
typedef struct OpenAPI_usage_mon_level_s {
} OpenAPI_usage_mon_level_t;

OpenAPI_usage_mon_level_t *OpenAPI_usage_mon_level_create(
    );
void OpenAPI_usage_mon_level_free(OpenAPI_usage_mon_level_t *usage_mon_level);
OpenAPI_usage_mon_level_t *OpenAPI_usage_mon_level_parseFromJSON(cJSON *usage_mon_levelJSON);
cJSON *OpenAPI_usage_mon_level_convertToJSON(OpenAPI_usage_mon_level_t *usage_mon_level);
OpenAPI_usage_mon_level_t *OpenAPI_usage_mon_level_copy(OpenAPI_usage_mon_level_t *dst, OpenAPI_usage_mon_level_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_mon_level_H_ */

