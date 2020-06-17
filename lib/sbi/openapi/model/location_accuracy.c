
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_accuracy.h"

OpenAPI_location_accuracy_t *OpenAPI_location_accuracy_create(
    )
{
    OpenAPI_location_accuracy_t *location_accuracy_local_var = OpenAPI_malloc(sizeof(OpenAPI_location_accuracy_t));
    if (!location_accuracy_local_var) {
        return NULL;
    }

    return location_accuracy_local_var;
}

void OpenAPI_location_accuracy_free(OpenAPI_location_accuracy_t *location_accuracy)
{
    if (NULL == location_accuracy) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(location_accuracy);
}

cJSON *OpenAPI_location_accuracy_convertToJSON(OpenAPI_location_accuracy_t *location_accuracy)
{
    cJSON *item = NULL;

    if (location_accuracy == NULL) {
        ogs_error("OpenAPI_location_accuracy_convertToJSON() failed [LocationAccuracy]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_location_accuracy_t *OpenAPI_location_accuracy_parseFromJSON(cJSON *location_accuracyJSON)
{
    OpenAPI_location_accuracy_t *location_accuracy_local_var = NULL;
    location_accuracy_local_var = OpenAPI_location_accuracy_create (
        );

    return location_accuracy_local_var;
end:
    return NULL;
}

OpenAPI_location_accuracy_t *OpenAPI_location_accuracy_copy(OpenAPI_location_accuracy_t *dst, OpenAPI_location_accuracy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_accuracy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_accuracy_convertToJSON() failed");
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

    OpenAPI_location_accuracy_free(dst);
    dst = OpenAPI_location_accuracy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

