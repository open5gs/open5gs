
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "external_client_type.h"

OpenAPI_external_client_type_t *OpenAPI_external_client_type_create(
)
{
    OpenAPI_external_client_type_t *external_client_type_local_var = ogs_malloc(sizeof(OpenAPI_external_client_type_t));
    ogs_assert(external_client_type_local_var);


    return external_client_type_local_var;
}

void OpenAPI_external_client_type_free(OpenAPI_external_client_type_t *external_client_type)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == external_client_type) {
        return;
    }
    ogs_free(external_client_type);
}

cJSON *OpenAPI_external_client_type_convertToJSON(OpenAPI_external_client_type_t *external_client_type)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_lnode_t *node = NULL;
    external_client_type_local_var = OpenAPI_external_client_type_create (
    );

    return external_client_type_local_var;
end:
    return NULL;
}

OpenAPI_external_client_type_t *OpenAPI_external_client_type_copy(OpenAPI_external_client_type_t *dst, OpenAPI_external_client_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_external_client_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_external_client_type_convertToJSON() failed");
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

    OpenAPI_external_client_type_free(dst);
    dst = OpenAPI_external_client_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

