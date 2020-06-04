
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stationary_indication_rm.h"

OpenAPI_stationary_indication_rm_t *OpenAPI_stationary_indication_rm_create(
    )
{
    OpenAPI_stationary_indication_rm_t *stationary_indication_rm_local_var = OpenAPI_malloc(sizeof(OpenAPI_stationary_indication_rm_t));
    if (!stationary_indication_rm_local_var) {
        return NULL;
    }

    return stationary_indication_rm_local_var;
}

void OpenAPI_stationary_indication_rm_free(OpenAPI_stationary_indication_rm_t *stationary_indication_rm)
{
    if (NULL == stationary_indication_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(stationary_indication_rm);
}

cJSON *OpenAPI_stationary_indication_rm_convertToJSON(OpenAPI_stationary_indication_rm_t *stationary_indication_rm)
{
    cJSON *item = NULL;

    if (stationary_indication_rm == NULL) {
        ogs_error("OpenAPI_stationary_indication_rm_convertToJSON() failed [StationaryIndicationRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_stationary_indication_rm_t *OpenAPI_stationary_indication_rm_parseFromJSON(cJSON *stationary_indication_rmJSON)
{
    OpenAPI_stationary_indication_rm_t *stationary_indication_rm_local_var = NULL;
    stationary_indication_rm_local_var = OpenAPI_stationary_indication_rm_create (
        );

    return stationary_indication_rm_local_var;
end:
    return NULL;
}

