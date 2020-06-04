
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

