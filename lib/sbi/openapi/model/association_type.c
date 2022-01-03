
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "association_type.h"

OpenAPI_association_type_t *OpenAPI_association_type_create(
)
{
    OpenAPI_association_type_t *association_type_local_var = ogs_malloc(sizeof(OpenAPI_association_type_t));
    ogs_assert(association_type_local_var);


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

OpenAPI_association_type_t *OpenAPI_association_type_copy(OpenAPI_association_type_t *dst, OpenAPI_association_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_association_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_association_type_convertToJSON() failed");
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

    OpenAPI_association_type_free(dst);
    dst = OpenAPI_association_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

