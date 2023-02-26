#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "object.h"

OpenAPI_object_t *OpenAPI_object_create(void)
{
    OpenAPI_object_t *object = ogs_calloc(1, sizeof(OpenAPI_object_t));

    return object;
}

void OpenAPI_object_free(OpenAPI_object_t *object)
{
    if (!object) {
        return;
    }

    if (object->temporary) {
        ogs_free(object->temporary);
        object->temporary = NULL;
    }

    ogs_free (object);
}

cJSON *OpenAPI_object_convertToJSON(OpenAPI_object_t *object)
{
    if (!object) {
        return NULL;
    }

    if (!object->temporary) {
        return cJSON_Parse("{}");
    }

    return cJSON_Parse(object->temporary);
}

OpenAPI_object_t *OpenAPI_object_parseFromJSON(cJSON *json)
{
    if (!json) {
        goto end;
    }

    OpenAPI_object_t *object = OpenAPI_object_create();
    if (!object) {
        goto end;
    }
    object->temporary = cJSON_Print(json);
    return object;

end:
    return NULL;
}
