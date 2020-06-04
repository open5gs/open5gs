
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "association_type.h"

OpenAPI_association_type_t *OpenAPI_association_type_create(
    )
{
    OpenAPI_association_type_t *association_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_association_type_t));
    if (!association_type_local_var) {
        return NULL;
    }

    return association_type_local_var;
}

void OpenAPI_association_type_free(OpenAPI_association_type_t *association_type)
{
    if (NULL == association_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(association_type);
}

cJSON *OpenAPI_association_type_convertToJSON(OpenAPI_association_type_t *association_type)
{
    cJSON *item = NULL;

    if (association_type == NULL) {
        ogs_error("OpenAPI_association_type_convertToJSON() failed [AssociationType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_association_type_t *OpenAPI_association_type_parseFromJSON(cJSON *association_typeJSON)
{
    OpenAPI_association_type_t *association_type_local_var = NULL;
    association_type_local_var = OpenAPI_association_type_create (
        );

    return association_type_local_var;
end:
    return NULL;
}

