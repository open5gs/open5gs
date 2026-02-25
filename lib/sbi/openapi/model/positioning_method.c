
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "positioning_method.h"

OpenAPI_positioning_method_t *OpenAPI_positioning_method_create(
)
{
    OpenAPI_positioning_method_t *positioning_method_local_var = ogs_malloc(sizeof(OpenAPI_positioning_method_t));
    ogs_assert(positioning_method_local_var);


    return positioning_method_local_var;
}

void OpenAPI_positioning_method_free(OpenAPI_positioning_method_t *positioning_method)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == positioning_method) {
        return;
    }
    ogs_free(positioning_method);
}

cJSON *OpenAPI_positioning_method_convertToJSON(OpenAPI_positioning_method_t *positioning_method)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (positioning_method == NULL) {
        ogs_error("OpenAPI_positioning_method_convertToJSON() failed [PositioningMethod]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_positioning_method_t *OpenAPI_positioning_method_parseFromJSON(cJSON *positioning_methodJSON)
{
    OpenAPI_positioning_method_t *positioning_method_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    positioning_method_local_var = OpenAPI_positioning_method_create (
    );

    return positioning_method_local_var;
end:
    return NULL;
}

OpenAPI_positioning_method_t *OpenAPI_positioning_method_copy(OpenAPI_positioning_method_t *dst, OpenAPI_positioning_method_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_positioning_method_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_positioning_method_convertToJSON() failed");
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

    OpenAPI_positioning_method_free(dst);
    dst = OpenAPI_positioning_method_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

