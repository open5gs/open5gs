
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gad_shape.h"

OpenAPI_gad_shape_t *OpenAPI_gad_shape_create(
    OpenAPI_supported_gad_shapes_e shape
)
{
    OpenAPI_gad_shape_t *gad_shape_local_var = ogs_malloc(sizeof(OpenAPI_gad_shape_t));
    ogs_assert(gad_shape_local_var);

    gad_shape_local_var->shape = shape;

    return gad_shape_local_var;
}

void OpenAPI_gad_shape_free(OpenAPI_gad_shape_t *gad_shape)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gad_shape) {
        return;
    }
    ogs_free(gad_shape);
}

cJSON *OpenAPI_gad_shape_convertToJSON(OpenAPI_gad_shape_t *gad_shape)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gad_shape == NULL) {
        ogs_error("OpenAPI_gad_shape_convertToJSON() failed [GADShape]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (gad_shape->shape == OpenAPI_supported_gad_shapes_NULL) {
        ogs_error("OpenAPI_gad_shape_convertToJSON() failed [shape]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "shape", OpenAPI_supported_gad_shapes_ToString(gad_shape->shape)) == NULL) {
        ogs_error("OpenAPI_gad_shape_convertToJSON() failed [shape]");
        goto end;
    }

end:
    return item;
}

OpenAPI_gad_shape_t *OpenAPI_gad_shape_parseFromJSON(cJSON *gad_shapeJSON)
{
    OpenAPI_gad_shape_t *gad_shape_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_e shapeVariable = 0;
    shape = cJSON_GetObjectItemCaseSensitive(gad_shapeJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_gad_shape_parseFromJSON() failed [shape]");
        goto end;
    }
    if (!cJSON_IsString(shape)) {
        ogs_error("OpenAPI_gad_shape_parseFromJSON() failed [shape]");
        goto end;
    }
    shapeVariable = OpenAPI_supported_gad_shapes_FromString(shape->valuestring);

    gad_shape_local_var = OpenAPI_gad_shape_create (
        shapeVariable
    );

    return gad_shape_local_var;
end:
    return NULL;
}

OpenAPI_gad_shape_t *OpenAPI_gad_shape_copy(OpenAPI_gad_shape_t *dst, OpenAPI_gad_shape_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gad_shape_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gad_shape_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_gad_shape_free(dst);
    dst = OpenAPI_gad_shape_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

