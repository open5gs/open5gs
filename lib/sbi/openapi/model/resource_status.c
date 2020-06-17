
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resource_status.h"

OpenAPI_resource_status_t *OpenAPI_resource_status_create(
    )
{
    OpenAPI_resource_status_t *resource_status_local_var = OpenAPI_malloc(sizeof(OpenAPI_resource_status_t));
    if (!resource_status_local_var) {
        return NULL;
    }

    return resource_status_local_var;
}

void OpenAPI_resource_status_free(OpenAPI_resource_status_t *resource_status)
{
    if (NULL == resource_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(resource_status);
}

cJSON *OpenAPI_resource_status_convertToJSON(OpenAPI_resource_status_t *resource_status)
{
    cJSON *item = NULL;

    if (resource_status == NULL) {
        ogs_error("OpenAPI_resource_status_convertToJSON() failed [ResourceStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_resource_status_t *OpenAPI_resource_status_parseFromJSON(cJSON *resource_statusJSON)
{
    OpenAPI_resource_status_t *resource_status_local_var = NULL;
    resource_status_local_var = OpenAPI_resource_status_create (
        );

    return resource_status_local_var;
end:
    return NULL;
}

OpenAPI_resource_status_t *OpenAPI_resource_status_copy(OpenAPI_resource_status_t *dst, OpenAPI_resource_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_resource_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_resource_status_convertToJSON() failed");
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

    OpenAPI_resource_status_free(dst);
    dst = OpenAPI_resource_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

