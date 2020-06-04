
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core_network_type.h"

OpenAPI_core_network_type_t *OpenAPI_core_network_type_create(
    )
{
    OpenAPI_core_network_type_t *core_network_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_core_network_type_t));
    if (!core_network_type_local_var) {
        return NULL;
    }

    return core_network_type_local_var;
}

void OpenAPI_core_network_type_free(OpenAPI_core_network_type_t *core_network_type)
{
    if (NULL == core_network_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(core_network_type);
}

cJSON *OpenAPI_core_network_type_convertToJSON(OpenAPI_core_network_type_t *core_network_type)
{
    cJSON *item = NULL;

    if (core_network_type == NULL) {
        ogs_error("OpenAPI_core_network_type_convertToJSON() failed [CoreNetworkType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_core_network_type_t *OpenAPI_core_network_type_parseFromJSON(cJSON *core_network_typeJSON)
{
    OpenAPI_core_network_type_t *core_network_type_local_var = NULL;
    core_network_type_local_var = OpenAPI_core_network_type_create (
        );

    return core_network_type_local_var;
end:
    return NULL;
}

