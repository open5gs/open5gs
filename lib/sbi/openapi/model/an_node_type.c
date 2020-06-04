
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

