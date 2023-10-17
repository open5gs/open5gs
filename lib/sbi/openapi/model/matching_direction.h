/*
 * matching_direction.h
 *
 * Possible values are: - ASCENDING: Threshold is crossed in ascending direction. - DESCENDING: Threshold is crossed in descending direction. - CROSSED: Threshold is crossed either in ascending or descending direction. 
 */

#ifndef _OpenAPI_matching_direction_H_
#define _OpenAPI_matching_direction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "matching_direction_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_matching_direction_s OpenAPI_matching_direction_t;
typedef struct OpenAPI_matching_direction_s {
} OpenAPI_matching_direction_t;

OpenAPI_matching_direction_t *OpenAPI_matching_direction_create(
);
void OpenAPI_matching_direction_free(OpenAPI_matching_direction_t *matching_direction);
OpenAPI_matching_direction_t *OpenAPI_matching_direction_parseFromJSON(cJSON *matching_directionJSON);
cJSON *OpenAPI_matching_direction_convertToJSON(OpenAPI_matching_direction_t *matching_direction);
OpenAPI_matching_direction_t *OpenAPI_matching_direction_copy(OpenAPI_matching_direction_t *dst, OpenAPI_matching_direction_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_matching_direction_H_ */

