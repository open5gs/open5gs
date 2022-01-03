
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accumulated_usage.h"

OpenAPI_accumulated_usage_t *OpenAPI_accumulated_usage_create(
    bool is_duration,
    int duration,
    bool is_total_volume,
    long total_volume,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_uplink_volume,
    long uplink_volume
)
{
    OpenAPI_accumulated_usage_t *accumulated_usage_local_var = ogs_malloc(sizeof(OpenAPI_accumulated_usage_t));
    ogs_assert(accumulated_usage_local_var);

    accumulated_usage_local_var->is_duration = is_duration;
    accumulated_usage_local_var->duration = duration;
    accumulated_usage_local_var->is_total_volume = is_total_volume;
    accumulated_usage_local_var->total_volume = total_volume;
    accumulated_usage_local_var->is_downlink_volume = is_downlink_volume;
    accumulated_usage_local_var->downlink_volume = downlink_volume;
    accumulated_usage_local_var->is_uplink_volume = is_uplink_volume;
    accumulated_usage_local_var->uplink_volume = uplink_volume;

    return accumulated_usage_local_var;
}

void OpenAPI_accumulated_usage_free(OpenAPI_accumulated_usage_t *accumulated_usage)
{
    if (NULL == accumulated_usage) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(accumulated_usage);
}

cJSON *OpenAPI_accumulated_usage_convertToJSON(OpenAPI_accumulated_usage_t *accumulated_usage)
{
    cJSON *item = NULL;

    if (accumulated_usage == NULL) {
        ogs_error("OpenAPI_accumulated_usage_convertToJSON() failed [AccumulatedUsage]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (accumulated_usage->is_duration) {
    if (cJSON_AddNumberToObject(item, "duration", accumulated_usage->duration) == NULL) {
        ogs_error("OpenAPI_accumulated_usage_convertToJSON() failed [duration]");
        goto end;
    }
    }

    if (accumulated_usage->is_total_volume) {
    if (cJSON_AddNumberToObject(item, "totalVolume", accumulated_usage->total_volume) == NULL) {
        ogs_error("OpenAPI_accumulated_usage_convertToJSON() failed [total_volume]");
        goto end;
    }
    }

    if (accumulated_usage->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", accumulated_usage->downlink_volume) == NULL) {
        ogs_error("OpenAPI_accumulated_usage_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (accumulated_usage->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", accumulated_usage->uplink_volume) == NULL) {
        ogs_error("OpenAPI_accumulated_usage_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_accumulated_usage_t *OpenAPI_accumulated_usage_parseFromJSON(cJSON *accumulated_usageJSON)
{
    OpenAPI_accumulated_usage_t *accumulated_usage_local_var = NULL;
    cJSON *duration = cJSON_GetObjectItemCaseSensitive(accumulated_usageJSON, "duration");

    if (duration) {
    if (!cJSON_IsNumber(duration)) {
        ogs_error("OpenAPI_accumulated_usage_parseFromJSON() failed [duration]");
        goto end;
    }
    }

    cJSON *total_volume = cJSON_GetObjectItemCaseSensitive(accumulated_usageJSON, "totalVolume");

    if (total_volume) {
    if (!cJSON_IsNumber(total_volume)) {
        ogs_error("OpenAPI_accumulated_usage_parseFromJSON() failed [total_volume]");
        goto end;
    }
    }

    cJSON *downlink_volume = cJSON_GetObjectItemCaseSensitive(accumulated_usageJSON, "downlinkVolume");

    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_accumulated_usage_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    cJSON *uplink_volume = cJSON_GetObjectItemCaseSensitive(accumulated_usageJSON, "uplinkVolume");

    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_accumulated_usage_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    accumulated_usage_local_var = OpenAPI_accumulated_usage_create (
        duration ? true : false,
        duration ? duration->valuedouble : 0,
        total_volume ? true : false,
        total_volume ? total_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0
    );

    return accumulated_usage_local_var;
end:
    return NULL;
}

OpenAPI_accumulated_usage_t *OpenAPI_accumulated_usage_copy(OpenAPI_accumulated_usage_t *dst, OpenAPI_accumulated_usage_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_accumulated_usage_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_accumulated_usage_convertToJSON() failed");
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

    OpenAPI_accumulated_usage_free(dst);
    dst = OpenAPI_accumulated_usage_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

