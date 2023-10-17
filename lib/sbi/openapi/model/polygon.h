/*
 * polygon.h
 *
 * Polygon.
 */

#ifndef _OpenAPI_polygon_H_
#define _OpenAPI_polygon_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_polygon_s OpenAPI_polygon_t;
typedef struct OpenAPI_polygon_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    OpenAPI_list_t *point_list;
} OpenAPI_polygon_t;

OpenAPI_polygon_t *OpenAPI_polygon_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_list_t *point_list
);
void OpenAPI_polygon_free(OpenAPI_polygon_t *polygon);
OpenAPI_polygon_t *OpenAPI_polygon_parseFromJSON(cJSON *polygonJSON);
cJSON *OpenAPI_polygon_convertToJSON(OpenAPI_polygon_t *polygon);
OpenAPI_polygon_t *OpenAPI_polygon_copy(OpenAPI_polygon_t *dst, OpenAPI_polygon_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_polygon_H_ */

