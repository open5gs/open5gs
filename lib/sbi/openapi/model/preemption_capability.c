
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "preemption_capability.h"

OpenAPI_preemption_capability_t *OpenAPI_preemption_capability_create(
    )
{
    OpenAPI_preemption_capability_t *preemption_capability_local_var = OpenAPI_malloc(sizeof(OpenAPI_preemption_capability_t));
    if (!preemption_capability_local_var) {
        return NULL;
    }

    return preemption_capability_local_var;
}

void OpenAPI_preemption_capability_free(OpenAPI_preemption_capability_t *preemption_capability)
{
    if (NULL == preemption_capability) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(preemption_capability);
}

cJSON *OpenAPI_preemption_capability_convertToJSON(OpenAPI_preemption_capability_t *preemption_capability)
{
    cJSON *item = NULL;

    if (preemption_capability == NULL) {
        ogs_error("OpenAPI_preemption_capability_convertToJSON() failed [PreemptionCapability]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_preemption_capability_t *OpenAPI_preemption_capability_parseFromJSON(cJSON *preemption_capabilityJSON)
{
    OpenAPI_preemption_capability_t *preemption_capability_local_var = NULL;
    preemption_capability_local_var = OpenAPI_preemption_capability_create (
        );

    return preemption_capability_local_var;
end:
    return NULL;
}

