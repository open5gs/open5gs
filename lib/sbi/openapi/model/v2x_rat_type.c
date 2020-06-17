
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "v2x_rat_type.h"

OpenAPI_v2x_rat_type_t *OpenAPI_v2x_rat_type_create(
    )
{
    OpenAPI_v2x_rat_type_t *v2x_rat_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_v2x_rat_type_t));
    if (!v2x_rat_type_local_var) {
        return NULL;
    }

    return v2x_rat_type_local_var;
}

void OpenAPI_v2x_rat_type_free(OpenAPI_v2x_rat_type_t *v2x_rat_type)
{
    if (NULL == v2x_rat_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(v2x_rat_type);
}

cJSON *OpenAPI_v2x_rat_type_convertToJSON(OpenAPI_v2x_rat_type_t *v2x_rat_type)
{
    cJSON *item = NULL;

    if (v2x_rat_type == NULL) {
        ogs_error("OpenAPI_v2x_rat_type_convertToJSON() failed [V2xRatType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_v2x_rat_type_t *OpenAPI_v2x_rat_type_parseFromJSON(cJSON *v2x_rat_typeJSON)
{
    OpenAPI_v2x_rat_type_t *v2x_rat_type_local_var = NULL;
    v2x_rat_type_local_var = OpenAPI_v2x_rat_type_create (
        );

    return v2x_rat_type_local_var;
end:
    return NULL;
}

OpenAPI_v2x_rat_type_t *OpenAPI_v2x_rat_type_copy(OpenAPI_v2x_rat_type_t *dst, OpenAPI_v2x_rat_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_v2x_rat_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_v2x_rat_type_convertToJSON() failed");
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

    OpenAPI_v2x_rat_type_free(dst);
    dst = OpenAPI_v2x_rat_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

