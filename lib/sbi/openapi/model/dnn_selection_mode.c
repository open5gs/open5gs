
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_selection_mode.h"

OpenAPI_dnn_selection_mode_t *OpenAPI_dnn_selection_mode_create(
    )
{
    OpenAPI_dnn_selection_mode_t *dnn_selection_mode_local_var = OpenAPI_malloc(sizeof(OpenAPI_dnn_selection_mode_t));
    if (!dnn_selection_mode_local_var) {
        return NULL;
    }

    return dnn_selection_mode_local_var;
}

void OpenAPI_dnn_selection_mode_free(OpenAPI_dnn_selection_mode_t *dnn_selection_mode)
{
    if (NULL == dnn_selection_mode) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dnn_selection_mode);
}

cJSON *OpenAPI_dnn_selection_mode_convertToJSON(OpenAPI_dnn_selection_mode_t *dnn_selection_mode)
{
    cJSON *item = NULL;

    if (dnn_selection_mode == NULL) {
        ogs_error("OpenAPI_dnn_selection_mode_convertToJSON() failed [DnnSelectionMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dnn_selection_mode_t *OpenAPI_dnn_selection_mode_parseFromJSON(cJSON *dnn_selection_modeJSON)
{
    OpenAPI_dnn_selection_mode_t *dnn_selection_mode_local_var = NULL;
    dnn_selection_mode_local_var = OpenAPI_dnn_selection_mode_create (
        );

    return dnn_selection_mode_local_var;
end:
    return NULL;
}

OpenAPI_dnn_selection_mode_t *OpenAPI_dnn_selection_mode_copy(OpenAPI_dnn_selection_mode_t *dst, OpenAPI_dnn_selection_mode_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_selection_mode_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_selection_mode_convertToJSON() failed");
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

    OpenAPI_dnn_selection_mode_free(dst);
    dst = OpenAPI_dnn_selection_mode_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

