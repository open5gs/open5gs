
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_functionality.h"

OpenAPI_steering_functionality_t *OpenAPI_steering_functionality_create(
    )
{
    OpenAPI_steering_functionality_t *steering_functionality_local_var = OpenAPI_malloc(sizeof(OpenAPI_steering_functionality_t));
    if (!steering_functionality_local_var) {
        return NULL;
    }

    return steering_functionality_local_var;
}

void OpenAPI_steering_functionality_free(OpenAPI_steering_functionality_t *steering_functionality)
{
    if (NULL == steering_functionality) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(steering_functionality);
}

cJSON *OpenAPI_steering_functionality_convertToJSON(OpenAPI_steering_functionality_t *steering_functionality)
{
    cJSON *item = NULL;

    if (steering_functionality == NULL) {
        ogs_error("OpenAPI_steering_functionality_convertToJSON() failed [SteeringFunctionality]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_steering_functionality_t *OpenAPI_steering_functionality_parseFromJSON(cJSON *steering_functionalityJSON)
{
    OpenAPI_steering_functionality_t *steering_functionality_local_var = NULL;
    steering_functionality_local_var = OpenAPI_steering_functionality_create (
        );

    return steering_functionality_local_var;
end:
    return NULL;
}

OpenAPI_steering_functionality_t *OpenAPI_steering_functionality_copy(OpenAPI_steering_functionality_t *dst, OpenAPI_steering_functionality_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steering_functionality_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steering_functionality_convertToJSON() failed");
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

    OpenAPI_steering_functionality_free(dst);
    dst = OpenAPI_steering_functionality_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

