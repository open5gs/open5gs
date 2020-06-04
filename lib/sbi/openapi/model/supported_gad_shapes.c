
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supported_gad_shapes.h"

OpenAPI_supported_gad_shapes_t *OpenAPI_supported_gad_shapes_create(
    )
{
    OpenAPI_supported_gad_shapes_t *supported_gad_shapes_local_var = OpenAPI_malloc(sizeof(OpenAPI_supported_gad_shapes_t));
    if (!supported_gad_shapes_local_var) {
        return NULL;
    }

    return supported_gad_shapes_local_var;
}

void OpenAPI_supported_gad_shapes_free(OpenAPI_supported_gad_shapes_t *supported_gad_shapes)
{
    if (NULL == supported_gad_shapes) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(supported_gad_shapes);
}

cJSON *OpenAPI_supported_gad_shapes_convertToJSON(OpenAPI_supported_gad_shapes_t *supported_gad_shapes)
{
    cJSON *item = NULL;

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
    supported_gad_shapes_local_var = OpenAPI_supported_gad_shapes_create (
        );

    return supported_gad_shapes_local_var;
end:
    return NULL;
}

