
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_threshold.h"

OpenAPI_usage_threshold_t *OpenAPI_usage_threshold_create(
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
    OpenAPI_usage_threshold_t *usage_threshold_local_var = ogs_malloc(sizeof(OpenAPI_usage_threshold_t));
    ogs_assert(usage_threshold_local_var);

    usage_threshold_local_var->is_duration = is_duration;
    usage_threshold_local_var->duration = duration;
    usage_threshold_local_var->is_total_volume = is_total_volume;
    usage_threshold_local_var->total_volume = total_volume;
    usage_threshold_local_var->is_downlink_volume = is_downlink_volume;
    usage_threshold_local_var->downlink_volume = downlink_volume;
    usage_threshold_local_var->is_uplink_volume = is_uplink_volume;
    usage_threshold_local_var->uplink_volume = uplink_volume;

    return usage_threshold_local_var;
}

void OpenAPI_usage_threshold_free(OpenAPI_usage_threshold_t *usage_threshold)
{
    if (NULL == usage_threshold) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(usage_threshold);
}

cJSON *OpenAPI_usage_threshold_convertToJSON(OpenAPI_usage_threshold_t *usage_threshold)
{
    cJSON *item = NULL;

    if (usage_threshold == NULL) {
        ogs_error("OpenAPI_usage_threshold_convertToJSON() failed [UsageThreshold]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (usage_threshold->is_duration) {
    if (cJSON_AddNumberToObject(item, "duration", usage_threshold->duration) == NULL) {
        ogs_error("OpenAPI_usage_threshold_convertToJSON() failed [duration]");
        goto end;
    }
    }

    if (usage_threshold->is_total_volume) {
    if (cJSON_AddNumberToObject(item, "totalVolume", usage_threshold->total_volume) == NULL) {
        ogs_error("OpenAPI_usage_threshold_convertToJSON() failed [total_volume]");
        goto end;
    }
    }

    if (usage_threshold->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", usage_threshold->downlink_volume) == NULL) {
        ogs_error("OpenAPI_usage_threshold_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (usage_threshold->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", usage_threshold->uplink_volume) == NULL) {
        ogs_error("OpenAPI_usage_threshold_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_usage_threshold_t *OpenAPI_usage_threshold_parseFromJSON(cJSON *usage_thresholdJSON)
{
    OpenAPI_usage_threshold_t *usage_threshold_local_var = NULL;
    cJSON *duration = cJSON_GetObjectItemCaseSensitive(usage_thresholdJSON, "duration");

    if (duration) {
    if (!cJSON_IsNumber(duration)) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON() failed [duration]");
        goto end;
    }
    }

    cJSON *total_volume = cJSON_GetObjectItemCaseSensitive(usage_thresholdJSON, "totalVolume");

    if (total_volume) {
    if (!cJSON_IsNumber(total_volume)) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON() failed [total_volume]");
        goto end;
    }
    }

    cJSON *downlink_volume = cJSON_GetObjectItemCaseSensitive(usage_thresholdJSON, "downlinkVolume");

    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    cJSON *uplink_volume = cJSON_GetObjectItemCaseSensitive(usage_thresholdJSON, "uplinkVolume");

    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    usage_threshold_local_var = OpenAPI_usage_threshold_create (
        duration ? true : false,
        duration ? duration->valuedouble : 0,
        total_volume ? true : false,
        total_volume ? total_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0
    );

    return usage_threshold_local_var;
end:
    return NULL;
}

OpenAPI_usage_threshold_t *OpenAPI_usage_threshold_copy(OpenAPI_usage_threshold_t *dst, OpenAPI_usage_threshold_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_threshold_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_threshold_convertToJSON() failed");
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

    OpenAPI_usage_threshold_free(dst);
    dst = OpenAPI_usage_threshold_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

