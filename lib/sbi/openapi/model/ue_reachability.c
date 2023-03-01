
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reachability.h"

OpenAPI_ue_reachability_t *OpenAPI_ue_reachability_create(
)
{
    OpenAPI_ue_reachability_t *ue_reachability_local_var = ogs_malloc(sizeof(OpenAPI_ue_reachability_t));
    ogs_assert(ue_reachability_local_var);


    return ue_reachability_local_var;
}

void OpenAPI_ue_reachability_free(OpenAPI_ue_reachability_t *ue_reachability)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_reachability) {
        return;
    }
    ogs_free(ue_reachability);
}

cJSON *OpenAPI_ue_reachability_convertToJSON(OpenAPI_ue_reachability_t *ue_reachability)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_lnode_t *node = NULL;
    ue_reachability_local_var = OpenAPI_ue_reachability_create (
    );

    return ue_reachability_local_var;
end:
    return NULL;
}

OpenAPI_ue_reachability_t *OpenAPI_ue_reachability_copy(OpenAPI_ue_reachability_t *dst, OpenAPI_ue_reachability_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_reachability_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_reachability_convertToJSON() failed");
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

    OpenAPI_ue_reachability_free(dst);
    dst = OpenAPI_ue_reachability_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

