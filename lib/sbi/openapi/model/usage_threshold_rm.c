
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_threshold_rm.h"

OpenAPI_usage_threshold_rm_t *OpenAPI_usage_threshold_rm_create(
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
    OpenAPI_usage_threshold_rm_t *usage_threshold_rm_local_var = ogs_malloc(sizeof(OpenAPI_usage_threshold_rm_t));
    ogs_assert(usage_threshold_rm_local_var);

    usage_threshold_rm_local_var->is_duration = is_duration;
    usage_threshold_rm_local_var->duration = duration;
    usage_threshold_rm_local_var->is_total_volume = is_total_volume;
    usage_threshold_rm_local_var->total_volume = total_volume;
    usage_threshold_rm_local_var->is_downlink_volume = is_downlink_volume;
    usage_threshold_rm_local_var->downlink_volume = downlink_volume;
    usage_threshold_rm_local_var->is_uplink_volume = is_uplink_volume;
    usage_threshold_rm_local_var->uplink_volume = uplink_volume;

    return usage_threshold_rm_local_var;
}

void OpenAPI_usage_threshold_rm_free(OpenAPI_usage_threshold_rm_t *usage_threshold_rm)
{
    if (NULL == usage_threshold_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(usage_threshold_rm);
}

cJSON *OpenAPI_usage_threshold_rm_convertToJSON(OpenAPI_usage_threshold_rm_t *usage_threshold_rm)
{
    cJSON *item = NULL;

    if (usage_threshold_rm == NULL) {
        ogs_error("OpenAPI_usage_threshold_rm_convertToJSON() failed [UsageThresholdRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (usage_threshold_rm->is_duration) {
    if (cJSON_AddNumberToObject(item, "duration", usage_threshold_rm->duration) == NULL) {
        ogs_error("OpenAPI_usage_threshold_rm_convertToJSON() failed [duration]");
        goto end;
    }
    }

    if (usage_threshold_rm->is_total_volume) {
    if (cJSON_AddNumberToObject(item, "totalVolume", usage_threshold_rm->total_volume) == NULL) {
        ogs_error("OpenAPI_usage_threshold_rm_convertToJSON() failed [total_volume]");
        goto end;
    }
    }

    if (usage_threshold_rm->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", usage_threshold_rm->downlink_volume) == NULL) {
        ogs_error("OpenAPI_usage_threshold_rm_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (usage_threshold_rm->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", usage_threshold_rm->uplink_volume) == NULL) {
        ogs_error("OpenAPI_usage_threshold_rm_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_usage_threshold_rm_t *OpenAPI_usage_threshold_rm_parseFromJSON(cJSON *usage_threshold_rmJSON)
{
    OpenAPI_usage_threshold_rm_t *usage_threshold_rm_local_var = NULL;
    cJSON *duration = cJSON_GetObjectItemCaseSensitive(usage_threshold_rmJSON, "duration");

    if (duration) {
    if (!cJSON_IsNumber(duration)) {
        ogs_error("OpenAPI_usage_threshold_rm_parseFromJSON() failed [duration]");
        goto end;
    }
    }

    cJSON *total_volume = cJSON_GetObjectItemCaseSensitive(usage_threshold_rmJSON, "totalVolume");

    if (total_volume) {
    if (!cJSON_IsNumber(total_volume)) {
        ogs_error("OpenAPI_usage_threshold_rm_parseFromJSON() failed [total_volume]");
        goto end;
    }
    }

    cJSON *downlink_volume = cJSON_GetObjectItemCaseSensitive(usage_threshold_rmJSON, "downlinkVolume");

    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_usage_threshold_rm_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    cJSON *uplink_volume = cJSON_GetObjectItemCaseSensitive(usage_threshold_rmJSON, "uplinkVolume");

    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_usage_threshold_rm_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    usage_threshold_rm_local_var = OpenAPI_usage_threshold_rm_create (
        duration ? true : false,
        duration ? duration->valuedouble : 0,
        total_volume ? true : false,
        total_volume ? total_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0
    );

    return usage_threshold_rm_local_var;
end:
    return NULL;
}

OpenAPI_usage_threshold_rm_t *OpenAPI_usage_threshold_rm_copy(OpenAPI_usage_threshold_rm_t *dst, OpenAPI_usage_threshold_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_threshold_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_threshold_rm_convertToJSON() failed");
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

    OpenAPI_usage_threshold_rm_free(dst);
    dst = OpenAPI_usage_threshold_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

