
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reachability.h"

OpenAPI_ue_reachability_t *OpenAPI_ue_reachability_create(
    )
{
    OpenAPI_ue_reachability_t *ue_reachability_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_reachability_t));
    if (!ue_reachability_local_var) {
        return NULL;
    }

    return ue_reachability_local_var;
}

void OpenAPI_ue_reachability_free(OpenAPI_ue_reachability_t *ue_reachability)
{
    if (NULL == ue_reachability) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ue_reachability);
}

cJSON *OpenAPI_ue_reachability_convertToJSON(OpenAPI_ue_reachability_t *ue_reachability)
{
    cJSON *item = NULL;

    if (ue_reachability == NULL) {
        ogs_error("OpenAPI_ue_reachability_convertToJSON() failed [UeReachability]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ue_reachability_t *OpenAPI_ue_reachability_parseFromJSON(cJSON *ue_reachabilityJSON)
{
    OpenAPI_ue_reachability_t *ue_reachability_local_var = NULL;
    ue_reachability_local_var = OpenAPI_ue_reachability_create (
        );

    return ue_reachability_local_var;
end:
    return NULL;
}

