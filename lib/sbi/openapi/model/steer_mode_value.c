
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steer_mode_value.h"

OpenAPI_steer_mode_value_t *OpenAPI_steer_mode_value_create(
    )
{
    OpenAPI_steer_mode_value_t *steer_mode_value_local_var = OpenAPI_malloc(sizeof(OpenAPI_steer_mode_value_t));
    if (!steer_mode_value_local_var) {
        return NULL;
    }

    return steer_mode_value_local_var;
}

void OpenAPI_steer_mode_value_free(OpenAPI_steer_mode_value_t *steer_mode_value)
{
    if (NULL == steer_mode_value) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(steer_mode_value);
}

cJSON *OpenAPI_steer_mode_value_convertToJSON(OpenAPI_steer_mode_value_t *steer_mode_value)
{
    cJSON *item = NULL;

    if (steer_mode_value == NULL) {
        ogs_error("OpenAPI_steer_mode_value_convertToJSON() failed [SteerModeValue]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_steer_mode_value_t *OpenAPI_steer_mode_value_parseFromJSON(cJSON *steer_mode_valueJSON)
{
    OpenAPI_steer_mode_value_t *steer_mode_value_local_var = NULL;
    steer_mode_value_local_var = OpenAPI_steer_mode_value_create (
        );

    return steer_mode_value_local_var;
end:
    return NULL;
}

OpenAPI_steer_mode_value_t *OpenAPI_steer_mode_value_copy(OpenAPI_steer_mode_value_t *dst, OpenAPI_steer_mode_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steer_mode_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steer_mode_value_convertToJSON() failed");
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

    OpenAPI_steer_mode_value_free(dst);
    dst = OpenAPI_steer_mode_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

