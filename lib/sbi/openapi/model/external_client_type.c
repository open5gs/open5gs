
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "external_client_type.h"

OpenAPI_external_client_type_t *OpenAPI_external_client_type_create(
    )
{
    OpenAPI_external_client_type_t *external_client_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_external_client_type_t));
    if (!external_client_type_local_var) {
        return NULL;
    }

    return external_client_type_local_var;
}

void OpenAPI_external_client_type_free(OpenAPI_external_client_type_t *external_client_type)
{
    if (NULL == external_client_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(external_client_type);
}

cJSON *OpenAPI_external_client_type_convertToJSON(OpenAPI_external_client_type_t *external_client_type)
{
    cJSON *item = NULL;

    if (external_client_type == NULL) {
        ogs_error("OpenAPI_external_client_type_convertToJSON() failed [ExternalClientType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_external_client_type_t *OpenAPI_external_client_type_parseFromJSON(cJSON *external_client_typeJSON)
{
    OpenAPI_external_client_type_t *external_client_type_local_var = NULL;
    external_client_type_local_var = OpenAPI_external_client_type_create (
        );

    return external_client_type_local_var;
end:
    return NULL;
}

