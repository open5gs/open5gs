
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnai_change_type.h"

OpenAPI_dnai_change_type_t *OpenAPI_dnai_change_type_create(
    )
{
    OpenAPI_dnai_change_type_t *dnai_change_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_dnai_change_type_t));
    if (!dnai_change_type_local_var) {
        return NULL;
    }

    return dnai_change_type_local_var;
}

void OpenAPI_dnai_change_type_free(OpenAPI_dnai_change_type_t *dnai_change_type)
{
    if (NULL == dnai_change_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dnai_change_type);
}

cJSON *OpenAPI_dnai_change_type_convertToJSON(OpenAPI_dnai_change_type_t *dnai_change_type)
{
    cJSON *item = NULL;

    if (dnai_change_type == NULL) {
        ogs_error("OpenAPI_dnai_change_type_convertToJSON() failed [DnaiChangeType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dnai_change_type_t *OpenAPI_dnai_change_type_parseFromJSON(cJSON *dnai_change_typeJSON)
{
    OpenAPI_dnai_change_type_t *dnai_change_type_local_var = NULL;
    dnai_change_type_local_var = OpenAPI_dnai_change_type_create (
        );

    return dnai_change_type_local_var;
end:
    return NULL;
}

