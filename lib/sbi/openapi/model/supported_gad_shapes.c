
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supported_gad_shapes.h"

OpenAPI_supported_gad_shapes_t *OpenAPI_supported_gad_shapes_create(
)
{
    OpenAPI_supported_gad_shapes_t *supported_gad_shapes_local_var = ogs_malloc(sizeof(OpenAPI_supported_gad_shapes_t));
    ogs_assert(supported_gad_shapes_local_var);


    return supported_gad_shapes_local_var;
}

void OpenAPI_supported_gad_shapes_free(OpenAPI_supported_gad_shapes_t *supported_gad_shapes)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == supported_gad_shapes) {
        return;
    }
    ogs_free(supported_gad_shapes);
}

cJSON *OpenAPI_supported_gad_shapes_convertToJSON(OpenAPI_supported_gad_shapes_t *supported_gad_shapes)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (supported_gad_shapes == NULL) {
        ogs_error("OpenAPI_supported_gad_shapes_convertToJSON() failed [SupportedGADShapes]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_supported_gad_shapes_t *OpenAPI_supported_gad_shapes_parseFromJSON(cJSON *supported_gad_shapesJSON)
{
    OpenAPI_supported_gad_shapes_t *supported_gad_shapes_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    supported_gad_shapes_local_var = OpenAPI_supported_gad_shapes_create (
    );

    return supported_gad_shapes_local_var;
end:
    return NULL;
}

OpenAPI_supported_gad_shapes_t *OpenAPI_supported_gad_shapes_copy(OpenAPI_supported_gad_shapes_t *dst, OpenAPI_supported_gad_shapes_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_supported_gad_shapes_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_supported_gad_shapes_convertToJSON() failed");
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

    OpenAPI_supported_gad_shapes_free(dst);
    dst = OpenAPI_supported_gad_shapes_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

