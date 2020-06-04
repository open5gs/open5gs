
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_right_status.h"

OpenAPI_access_right_status_t *OpenAPI_access_right_status_create(
    )
{
    OpenAPI_access_right_status_t *access_right_status_local_var = OpenAPI_malloc(sizeof(OpenAPI_access_right_status_t));
    if (!access_right_status_local_var) {
        return NULL;
    }

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

