
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_profile.h"

OpenAPI_traffic_profile_t *OpenAPI_traffic_profile_create(
    )
{
    OpenAPI_traffic_profile_t *traffic_profile_local_var = OpenAPI_malloc(sizeof(OpenAPI_traffic_profile_t));
    if (!traffic_profile_local_var) {
        return NULL;
    }

    return traffic_profile_local_var;
}

void OpenAPI_traffic_profile_free(OpenAPI_traffic_profile_t *traffic_profile)
{
    if (NULL == traffic_profile) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(traffic_profile);
}

cJSON *OpenAPI_traffic_profile_convertToJSON(OpenAPI_traffic_profile_t *traffic_profile)
{
    cJSON *item = NULL;

    if (traffic_profile == NULL) {
        ogs_error("OpenAPI_traffic_profile_convertToJSON() failed [TrafficProfile]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_traffic_profile_t *OpenAPI_traffic_profile_parseFromJSON(cJSON *traffic_profileJSON)
{
    OpenAPI_traffic_profile_t *traffic_profile_local_var = NULL;
    traffic_profile_local_var = OpenAPI_traffic_profile_create (
        );

    return traffic_profile_local_var;
end:
    return NULL;
}

OpenAPI_traffic_profile_t *OpenAPI_traffic_profile_copy(OpenAPI_traffic_profile_t *dst, OpenAPI_traffic_profile_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_profile_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_profile_convertToJSON() failed");
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

    OpenAPI_traffic_profile_free(dst);
    dst = OpenAPI_traffic_profile_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

