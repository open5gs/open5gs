
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_interface_type.h"

OpenAPI_up_interface_type_t *OpenAPI_up_interface_type_create(
    )
{
    OpenAPI_up_interface_type_t *up_interface_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_up_interface_type_t));
    if (!up_interface_type_local_var) {
        return NULL;
    }

    return up_interface_type_local_var;
}

void OpenAPI_up_interface_type_free(OpenAPI_up_interface_type_t *up_interface_type)
{
    if (NULL == up_interface_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(up_interface_type);
}

cJSON *OpenAPI_up_interface_type_convertToJSON(OpenAPI_up_interface_type_t *up_interface_type)
{
    cJSON *item = NULL;

    if (up_interface_type == NULL) {
        ogs_error("OpenAPI_up_interface_type_convertToJSON() failed [UPInterfaceType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_up_interface_type_t *OpenAPI_up_interface_type_parseFromJSON(cJSON *up_interface_typeJSON)
{
    OpenAPI_up_interface_type_t *up_interface_type_local_var = NULL;
    up_interface_type_local_var = OpenAPI_up_interface_type_create (
        );

    return up_interface_type_local_var;
end:
    return NULL;
}

OpenAPI_up_interface_type_t *OpenAPI_up_interface_type_copy(OpenAPI_up_interface_type_t *dst, OpenAPI_up_interface_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_up_interface_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_up_interface_type_convertToJSON() failed");
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

    OpenAPI_up_interface_type_free(dst);
    dst = OpenAPI_up_interface_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

