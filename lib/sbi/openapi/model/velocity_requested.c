
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "velocity_requested.h"

OpenAPI_velocity_requested_t *OpenAPI_velocity_requested_create(
)
{
    OpenAPI_velocity_requested_t *velocity_requested_local_var = ogs_malloc(sizeof(OpenAPI_velocity_requested_t));
    ogs_assert(velocity_requested_local_var);


    return velocity_requested_local_var;
}

void OpenAPI_velocity_requested_free(OpenAPI_velocity_requested_t *velocity_requested)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == velocity_requested) {
        return;
    }
    ogs_free(velocity_requested);
}

cJSON *OpenAPI_velocity_requested_convertToJSON(OpenAPI_velocity_requested_t *velocity_requested)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (velocity_requested == NULL) {
        ogs_error("OpenAPI_velocity_requested_convertToJSON() failed [VelocityRequested]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_velocity_requested_t *OpenAPI_velocity_requested_parseFromJSON(cJSON *velocity_requestedJSON)
{
    OpenAPI_velocity_requested_t *velocity_requested_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    velocity_requested_local_var = OpenAPI_velocity_requested_create (
    );

    return velocity_requested_local_var;
end:
    return NULL;
}

OpenAPI_velocity_requested_t *OpenAPI_velocity_requested_copy(OpenAPI_velocity_requested_t *dst, OpenAPI_velocity_requested_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_velocity_requested_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_velocity_requested_convertToJSON() failed");
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

    OpenAPI_velocity_requested_free(dst);
    dst = OpenAPI_velocity_requested_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

