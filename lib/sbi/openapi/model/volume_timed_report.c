
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "volume_timed_report.h"

OpenAPI_volume_timed_report_t *OpenAPI_volume_timed_report_create(
    char *start_time_stamp,
    char *end_time_stamp,
    long downlink_volume,
    long uplink_volume
)
{
    OpenAPI_volume_timed_report_t *volume_timed_report_local_var = ogs_malloc(sizeof(OpenAPI_volume_timed_report_t));
    ogs_assert(volume_timed_report_local_var);

    volume_timed_report_local_var->start_time_stamp = start_time_stamp;
    volume_timed_report_local_var->end_time_stamp = end_time_stamp;
    volume_timed_report_local_var->downlink_volume = downlink_volume;
    volume_timed_report_local_var->uplink_volume = uplink_volume;

    return volume_timed_report_local_var;
}

void OpenAPI_volume_timed_report_free(OpenAPI_volume_timed_report_t *volume_timed_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == volume_timed_report) {
        return;
    }
    if (volume_timed_report->start_time_stamp) {
        ogs_free(volume_timed_report->start_time_stamp);
        volume_timed_report->start_time_stamp = NULL;
    }
    if (volume_timed_report->end_time_stamp) {
        ogs_free(volume_timed_report->end_time_stamp);
        volume_timed_report->end_time_stamp = NULL;
    }
    ogs_free(volume_timed_report);
}

cJSON *OpenAPI_volume_timed_report_convertToJSON(OpenAPI_volume_timed_report_t *volume_timed_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (volume_timed_report == NULL) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed [VolumeTimedReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!volume_timed_report->start_time_stamp) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed [start_time_stamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "startTimeStamp", volume_timed_report->start_time_stamp) == NULL) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed [start_time_stamp]");
        goto end;
    }

    if (!volume_timed_report->end_time_stamp) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed [end_time_stamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "endTimeStamp", volume_timed_report->end_time_stamp) == NULL) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed [end_time_stamp]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "downlinkVolume", volume_timed_report->downlink_volume) == NULL) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed [downlink_volume]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "uplinkVolume", volume_timed_report->uplink_volume) == NULL) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed [uplink_volume]");
        goto end;
    }

end:
    return item;
}

OpenAPI_volume_timed_report_t *OpenAPI_volume_timed_report_parseFromJSON(cJSON *volume_timed_reportJSON)
{
    OpenAPI_volume_timed_report_t *volume_timed_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start_time_stamp = NULL;
    cJSON *end_time_stamp = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *uplink_volume = NULL;
    start_time_stamp = cJSON_GetObjectItemCaseSensitive(volume_timed_reportJSON, "startTimeStamp");
    if (!start_time_stamp) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [start_time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(start_time_stamp) && !cJSON_IsNull(start_time_stamp)) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [start_time_stamp]");
        goto end;
    }

    end_time_stamp = cJSON_GetObjectItemCaseSensitive(volume_timed_reportJSON, "endTimeStamp");
    if (!end_time_stamp) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [end_time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(end_time_stamp) && !cJSON_IsNull(end_time_stamp)) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [end_time_stamp]");
        goto end;
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(volume_timed_reportJSON, "downlinkVolume");
    if (!downlink_volume) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [downlink_volume]");
        goto end;
    }

    uplink_volume = cJSON_GetObjectItemCaseSensitive(volume_timed_reportJSON, "uplinkVolume");
    if (!uplink_volume) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_volume_timed_report_parseFromJSON() failed [uplink_volume]");
        goto end;
    }

    volume_timed_report_local_var = OpenAPI_volume_timed_report_create (
        ogs_strdup(start_time_stamp->valuestring),
        ogs_strdup(end_time_stamp->valuestring),
        
        downlink_volume->valuedouble,
        
        uplink_volume->valuedouble
    );

    return volume_timed_report_local_var;
end:
    return NULL;
}

OpenAPI_volume_timed_report_t *OpenAPI_volume_timed_report_copy(OpenAPI_volume_timed_report_t *dst, OpenAPI_volume_timed_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_volume_timed_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_volume_timed_report_convertToJSON() failed");
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

    OpenAPI_volume_timed_report_free(dst);
    dst = OpenAPI_volume_timed_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

