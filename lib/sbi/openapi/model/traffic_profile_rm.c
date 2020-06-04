
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_profile_rm.h"

OpenAPI_traffic_profile_rm_t *OpenAPI_traffic_profile_rm_create(
    )
{
    OpenAPI_traffic_profile_rm_t *traffic_profile_rm_local_var = OpenAPI_malloc(sizeof(OpenAPI_traffic_profile_rm_t));
    if (!traffic_profile_rm_local_var) {
        return NULL;
    }

    return traffic_profile_rm_local_var;
}

void OpenAPI_traffic_profile_rm_free(OpenAPI_traffic_profile_rm_t *traffic_profile_rm)
{
    if (NULL == traffic_profile_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(traffic_profile_rm);
}

cJSON *OpenAPI_traffic_profile_rm_convertToJSON(OpenAPI_traffic_profile_rm_t *traffic_profile_rm)
{
    cJSON *item = NULL;

    if (traffic_profile_rm == NULL) {
        ogs_error("OpenAPI_traffic_profile_rm_convertToJSON() failed [TrafficProfileRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_traffic_profile_rm_t *OpenAPI_traffic_profile_rm_parseFromJSON(cJSON *traffic_profile_rmJSON)
{
    OpenAPI_traffic_profile_rm_t *traffic_profile_rm_local_var = NULL;
    traffic_profile_rm_local_var = OpenAPI_traffic_profile_rm_create (
        );

    return traffic_profile_rm_local_var;
end:
    return NULL;
}

