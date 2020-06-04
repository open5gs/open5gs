
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_container.h"

OpenAPI_steering_container_t *OpenAPI_steering_container_create(
    )
{
    OpenAPI_steering_container_t *steering_container_local_var = OpenAPI_malloc(sizeof(OpenAPI_steering_container_t));
    if (!steering_container_local_var) {
        return NULL;
    }

    return steering_container_local_var;
}

void OpenAPI_steering_container_free(OpenAPI_steering_container_t *steering_container)
{
    if (NULL == steering_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(steering_container);
}

cJSON *OpenAPI_steering_container_convertToJSON(OpenAPI_steering_container_t *steering_container)
{
    cJSON *item = NULL;

    if (steering_container == NULL) {
        ogs_error("OpenAPI_steering_container_convertToJSON() failed [SteeringContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_steering_container_t *OpenAPI_steering_container_parseFromJSON(cJSON *steering_containerJSON)
{
    OpenAPI_steering_container_t *steering_container_local_var = NULL;
    steering_container_local_var = OpenAPI_steering_container_create (
        );

    return steering_container_local_var;
end:
    return NULL;
}

