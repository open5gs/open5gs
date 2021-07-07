/*
 * gad_shape.h
 *
 * 
 */

#ifndef _OpenAPI_gad_shape_H_
#define _OpenAPI_gad_shape_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gad_shape_s OpenAPI_gad_shape_t;
typedef struct OpenAPI_gad_shape_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
} OpenAPI_gad_shape_t;

OpenAPI_gad_shape_t *OpenAPI_gad_shape_create(
    OpenAPI_supported_gad_shapes_t *shape
);
void OpenAPI_gad_shape_free(OpenAPI_gad_shape_t *gad_shape);
OpenAPI_gad_shape_t *OpenAPI_gad_shape_parseFromJSON(cJSON *gad_shapeJSON);
cJSON *OpenAPI_gad_shape_convertToJSON(OpenAPI_gad_shape_t *gad_shape);
OpenAPI_gad_shape_t *OpenAPI_gad_shape_copy(OpenAPI_gad_shape_t *dst, OpenAPI_gad_shape_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gad_shape_H_ */

