/*
 * supported_gad_shapes.h
 *
 *
 */

#ifndef _OpenAPI_supported_gad_shapes_H_
#define _OpenAPI_supported_gad_shapes_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_supported_gad_shapes_s OpenAPI_supported_gad_shapes_t;
typedef struct OpenAPI_supported_gad_shapes_s {
} OpenAPI_supported_gad_shapes_t;

OpenAPI_supported_gad_shapes_t *OpenAPI_supported_gad_shapes_create(
    );
void OpenAPI_supported_gad_shapes_free(OpenAPI_supported_gad_shapes_t *supported_gad_shapes);
OpenAPI_supported_gad_shapes_t *OpenAPI_supported_gad_shapes_parseFromJSON(cJSON *supported_gad_shapesJSON);
cJSON *OpenAPI_supported_gad_shapes_convertToJSON(OpenAPI_supported_gad_shapes_t *supported_gad_shapes);
OpenAPI_supported_gad_shapes_t *OpenAPI_supported_gad_shapes_copy(OpenAPI_supported_gad_shapes_t *dst, OpenAPI_supported_gad_shapes_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supported_gad_shapes_H_ */

