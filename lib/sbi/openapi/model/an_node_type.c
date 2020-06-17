
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "an_node_type.h"

OpenAPI_an_node_type_t *OpenAPI_an_node_type_create(
    )
{
    OpenAPI_an_node_type_t *an_node_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_an_node_type_t));
    if (!an_node_type_local_var) {
        return NULL;
    }

    return an_node_type_local_var;
}

void OpenAPI_an_node_type_free(OpenAPI_an_node_type_t *an_node_type)
{
    if (NULL == an_node_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(an_node_type);
}

cJSON *OpenAPI_an_node_type_convertToJSON(OpenAPI_an_node_type_t *an_node_type)
{
    cJSON *item = NULL;

    if (an_node_type == NULL) {
        ogs_error("OpenAPI_an_node_type_convertToJSON() failed [AnNodeType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_an_node_type_t *OpenAPI_an_node_type_parseFromJSON(cJSON *an_node_typeJSON)
{
    OpenAPI_an_node_type_t *an_node_type_local_var = NULL;
    an_node_type_local_var = OpenAPI_an_node_type_create (
        );

    return an_node_type_local_var;
end:
    return NULL;
}

OpenAPI_an_node_type_t *OpenAPI_an_node_type_copy(OpenAPI_an_node_type_t *dst, OpenAPI_an_node_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_an_node_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_an_node_type_convertToJSON() failed");
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

    OpenAPI_an_node_type_free(dst);
    dst = OpenAPI_an_node_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

