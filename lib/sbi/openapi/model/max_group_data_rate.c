
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "max_group_data_rate.h"

OpenAPI_max_group_data_rate_t *OpenAPI_max_group_data_rate_create(
    char *uplink,
    char *downlink
)
{
    OpenAPI_max_group_data_rate_t *max_group_data_rate_local_var = ogs_malloc(sizeof(OpenAPI_max_group_data_rate_t));
    ogs_assert(max_group_data_rate_local_var);

    max_group_data_rate_local_var->uplink = uplink;
    max_group_data_rate_local_var->downlink = downlink;

    return max_group_data_rate_local_var;
}

void OpenAPI_max_group_data_rate_free(OpenAPI_max_group_data_rate_t *max_group_data_rate)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == max_group_data_rate) {
        return;
    }
    if (max_group_data_rate->uplink) {
        ogs_free(max_group_data_rate->uplink);
        max_group_data_rate->uplink = NULL;
    }
    if (max_group_data_rate->downlink) {
        ogs_free(max_group_data_rate->downlink);
        max_group_data_rate->downlink = NULL;
    }
    ogs_free(max_group_data_rate);
}

cJSON *OpenAPI_max_group_data_rate_convertToJSON(OpenAPI_max_group_data_rate_t *max_group_data_rate)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (max_group_data_rate == NULL) {
        ogs_error("OpenAPI_max_group_data_rate_convertToJSON() failed [MaxGroupDataRate]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (max_group_data_rate->uplink) {
    if (cJSON_AddStringToObject(item, "uplink", max_group_data_rate->uplink) == NULL) {
        ogs_error("OpenAPI_max_group_data_rate_convertToJSON() failed [uplink]");
        goto end;
    }
    }

    if (max_group_data_rate->downlink) {
    if (cJSON_AddStringToObject(item, "downlink", max_group_data_rate->downlink) == NULL) {
        ogs_error("OpenAPI_max_group_data_rate_convertToJSON() failed [downlink]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_max_group_data_rate_t *OpenAPI_max_group_data_rate_parseFromJSON(cJSON *max_group_data_rateJSON)
{
    OpenAPI_max_group_data_rate_t *max_group_data_rate_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink = NULL;
    cJSON *downlink = NULL;
    uplink = cJSON_GetObjectItemCaseSensitive(max_group_data_rateJSON, "uplink");
    if (uplink) {
    if (!cJSON_IsString(uplink) && !cJSON_IsNull(uplink)) {
        ogs_error("OpenAPI_max_group_data_rate_parseFromJSON() failed [uplink]");
        goto end;
    }
    }

    downlink = cJSON_GetObjectItemCaseSensitive(max_group_data_rateJSON, "downlink");
    if (downlink) {
    if (!cJSON_IsString(downlink) && !cJSON_IsNull(downlink)) {
        ogs_error("OpenAPI_max_group_data_rate_parseFromJSON() failed [downlink]");
        goto end;
    }
    }

    max_group_data_rate_local_var = OpenAPI_max_group_data_rate_create (
        uplink && !cJSON_IsNull(uplink) ? ogs_strdup(uplink->valuestring) : NULL,
        downlink && !cJSON_IsNull(downlink) ? ogs_strdup(downlink->valuestring) : NULL
    );

    return max_group_data_rate_local_var;
end:
    return NULL;
}

OpenAPI_max_group_data_rate_t *OpenAPI_max_group_data_rate_copy(OpenAPI_max_group_data_rate_t *dst, OpenAPI_max_group_data_rate_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_max_group_data_rate_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_max_group_data_rate_convertToJSON() failed");
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

    OpenAPI_max_group_data_rate_free(dst);
    dst = OpenAPI_max_group_data_rate_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

