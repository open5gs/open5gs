
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stationary_indication.h"

OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_create(
    )
{
    OpenAPI_stationary_indication_t *stationary_indication_local_var = OpenAPI_malloc(sizeof(OpenAPI_stationary_indication_t));
    if (!stationary_indication_local_var) {
        return NULL;
    }

    return stationary_indication_local_var;
}

void OpenAPI_stationary_indication_free(OpenAPI_stationary_indication_t *stationary_indication)
{
    if (NULL == stationary_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(stationary_indication);
}

cJSON *OpenAPI_stationary_indication_convertToJSON(OpenAPI_stationary_indication_t *stationary_indication)
{
    cJSON *item = NULL;

    if (stationary_indication == NULL) {
        ogs_error("OpenAPI_stationary_indication_convertToJSON() failed [StationaryIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_parseFromJSON(cJSON *stationary_indicationJSON)
{
    OpenAPI_stationary_indication_t *stationary_indication_local_var = NULL;
    stationary_indication_local_var = OpenAPI_stationary_indication_create (
        );

    return stationary_indication_local_var;
end:
    return NULL;
}

