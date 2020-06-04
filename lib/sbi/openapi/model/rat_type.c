
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rat_type.h"

OpenAPI_rat_type_t *OpenAPI_rat_type_create(
    )
{
    OpenAPI_rat_type_t *rat_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_rat_type_t));
    if (!rat_type_local_var) {
        return NULL;
    }

    return rat_type_local_var;
}

void OpenAPI_rat_type_free(OpenAPI_rat_type_t *rat_type)
{
    if (NULL == rat_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(rat_type);
}

cJSON *OpenAPI_rat_type_convertToJSON(OpenAPI_rat_type_t *rat_type)
{
    cJSON *item = NULL;

    if (rat_type == NULL) {
        ogs_error("OpenAPI_rat_type_convertToJSON() failed [RatType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_rat_type_t *OpenAPI_rat_type_parseFromJSON(cJSON *rat_typeJSON)
{
    OpenAPI_rat_type_t *rat_type_local_var = NULL;
    rat_type_local_var = OpenAPI_rat_type_create (
        );

    return rat_type_local_var;
end:
    return NULL;
}

