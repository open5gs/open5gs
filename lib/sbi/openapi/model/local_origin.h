/*
 * local_origin.h
 *
 * Indicates a Local origin in a reference system
 */

#ifndef _OpenAPI_local_origin_H_
#define _OpenAPI_local_origin_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geographical_coordinates.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_local_origin_s OpenAPI_local_origin_t;
typedef struct OpenAPI_local_origin_s {
    char *coordinate_id;
    struct OpenAPI_geographical_coordinates_s *point;
} OpenAPI_local_origin_t;

OpenAPI_local_origin_t *OpenAPI_local_origin_create(
    char *coordinate_id,
    OpenAPI_geographical_coordinates_t *point
);
void OpenAPI_local_origin_free(OpenAPI_local_origin_t *local_origin);
OpenAPI_local_origin_t *OpenAPI_local_origin_parseFromJSON(cJSON *local_originJSON);
cJSON *OpenAPI_local_origin_convertToJSON(OpenAPI_local_origin_t *local_origin);
OpenAPI_local_origin_t *OpenAPI_local_origin_copy(OpenAPI_local_origin_t *dst, OpenAPI_local_origin_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local_origin_H_ */

