
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_right_status.h"

OpenAPI_access_right_status_t *OpenAPI_access_right_status_create(
)
{
    OpenAPI_access_right_status_t *access_right_status_local_var = ogs_malloc(sizeof(OpenAPI_access_right_status_t));
    ogs_assert(access_right_status_local_var);


    return access_right_status_local_var;
}

void OpenAPI_access_right_status_free(OpenAPI_access_right_status_t *access_right_status)
{
    if (NULL == access_right_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(access_right_status);
}

cJSON *OpenAPI_access_right_status_convertToJSON(OpenAPI_access_right_status_t *access_right_status)
{
    cJSON *item = NULL;

    if (access_right_status == NULL) {
        ogs_error("OpenAPI_access_right_status_convertToJSON() failed [AccessRightStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_access_right_status_t *OpenAPI_access_right_status_parseFromJSON(cJSON *access_right_statusJSON)
{
    OpenAPI_access_right_status_t *access_right_status_local_var = NULL;
    access_right_status_local_var = OpenAPI_access_right_status_create (
    );

    return access_right_status_local_var;
end:
    return NULL;
}

OpenAPI_access_right_status_t *OpenAPI_access_right_status_copy(OpenAPI_access_right_status_t *dst, OpenAPI_access_right_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_right_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_right_status_convertToJSON() failed");
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

    OpenAPI_access_right_status_free(dst);
    dst = OpenAPI_access_right_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

