
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "multicast_access_control.h"

OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_create(
)
{
    OpenAPI_multicast_access_control_t *multicast_access_control_local_var = ogs_malloc(sizeof(OpenAPI_multicast_access_control_t));
    ogs_assert(multicast_access_control_local_var);


    return multicast_access_control_local_var;
}

void OpenAPI_multicast_access_control_free(OpenAPI_multicast_access_control_t *multicast_access_control)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == multicast_access_control) {
        return;
    }
    ogs_free(multicast_access_control);
}

cJSON *OpenAPI_multicast_access_control_convertToJSON(OpenAPI_multicast_access_control_t *multicast_access_control)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (multicast_access_control == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [MulticastAccessControl]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_parseFromJSON(cJSON *multicast_access_controlJSON)
{
    OpenAPI_multicast_access_control_t *multicast_access_control_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    multicast_access_control_local_var = OpenAPI_multicast_access_control_create (
    );

    return multicast_access_control_local_var;
end:
    return NULL;
}

OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_copy(OpenAPI_multicast_access_control_t *dst, OpenAPI_multicast_access_control_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_multicast_access_control_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed");
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

    OpenAPI_multicast_access_control_free(dst);
    dst = OpenAPI_multicast_access_control_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

