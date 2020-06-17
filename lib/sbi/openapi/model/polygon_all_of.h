/*
 * polygon_all_of.h
 *
 *
 */

#ifndef _OpenAPI_polygon_all_of_H_
#define _OpenAPI_polygon_all_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geographical_coordinates.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_polygon_all_of_s OpenAPI_polygon_all_of_t;
typedef struct OpenAPI_polygon_all_of_s {
    OpenAPI_list_t *point_list;
} OpenAPI_polygon_all_of_t;

OpenAPI_polygon_all_of_t *OpenAPI_polygon_all_of_create(
    OpenAPI_list_t *point_list
    );
void OpenAPI_polygon_all_of_free(OpenAPI_polygon_all_of_t *polygon_all_of);
OpenAPI_polygon_all_of_t *OpenAPI_polygon_all_of_parseFromJSON(cJSON *polygon_all_ofJSON);
cJSON *OpenAPI_polygon_all_of_convertToJSON(OpenAPI_polygon_all_of_t *polygon_all_of);
OpenAPI_polygon_all_of_t *OpenAPI_polygon_all_of_copy(OpenAPI_polygon_all_of_t *dst, OpenAPI_polygon_all_of_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_polygon_all_of_H_ */

