
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_profile_rm.h"

OpenAPI_traffic_profile_rm_t *OpenAPI_traffic_profile_rm_create(
)
{
    OpenAPI_traffic_profile_rm_t *traffic_profile_rm_local_var = ogs_malloc(sizeof(OpenAPI_traffic_profile_rm_t));
    ogs_assert(traffic_profile_rm_local_var);


    return traffic_profile_rm_local_var;
}

void OpenAPI_traffic_profile_rm_free(OpenAPI_traffic_profile_rm_t *traffic_profile_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_profile_rm) {
        return;
    }
    ogs_free(traffic_profile_rm);
}

cJSON *OpenAPI_traffic_profile_rm_convertToJSON(OpenAPI_traffic_profile_rm_t *traffic_profile_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_lnode_t *node = NULL;
    traffic_profile_rm_local_var = OpenAPI_traffic_profile_rm_create (
    );

    return traffic_profile_rm_local_var;
end:
    return NULL;
}

OpenAPI_traffic_profile_rm_t *OpenAPI_traffic_profile_rm_copy(OpenAPI_traffic_profile_rm_t *dst, OpenAPI_traffic_profile_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_profile_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_profile_rm_convertToJSON() failed");
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

    OpenAPI_traffic_profile_rm_free(dst);
    dst = OpenAPI_traffic_profile_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

