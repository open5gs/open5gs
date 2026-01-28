
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "motion_event_info.h"

OpenAPI_motion_event_info_t *OpenAPI_motion_event_info_create(
    int linear_distance,
    OpenAPI_occurrence_info_t *occurrence_info,
    bool is_minimum_interval,
    int minimum_interval,
    bool is_maximum_interval,
    int maximum_interval,
    bool is_sampling_interval,
    int sampling_interval,
    bool is_reporting_duration,
    int reporting_duration,
    bool is_reporting_location_req,
    int reporting_location_req
)
{
    OpenAPI_motion_event_info_t *motion_event_info_local_var = ogs_malloc(sizeof(OpenAPI_motion_event_info_t));
    ogs_assert(motion_event_info_local_var);

    motion_event_info_local_var->linear_distance = linear_distance;
    motion_event_info_local_var->occurrence_info = occurrence_info;
    motion_event_info_local_var->is_minimum_interval = is_minimum_interval;
    motion_event_info_local_var->minimum_interval = minimum_interval;
    motion_event_info_local_var->is_maximum_interval = is_maximum_interval;
    motion_event_info_local_var->maximum_interval = maximum_interval;
    motion_event_info_local_var->is_sampling_interval = is_sampling_interval;
    motion_event_info_local_var->sampling_interval = sampling_interval;
    motion_event_info_local_var->is_reporting_duration = is_reporting_duration;
    motion_event_info_local_var->reporting_duration = reporting_duration;
    motion_event_info_local_var->is_reporting_location_req = is_reporting_location_req;
    motion_event_info_local_var->reporting_location_req = reporting_location_req;

    return motion_event_info_local_var;
}

void OpenAPI_motion_event_info_free(OpenAPI_motion_event_info_t *motion_event_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == motion_event_info) {
        return;
    }
    if (motion_event_info->occurrence_info) {
        OpenAPI_occurrence_info_free(motion_event_info->occurrence_info);
        motion_event_info->occurrence_info = NULL;
    }
    ogs_free(motion_event_info);
}

cJSON *OpenAPI_motion_event_info_convertToJSON(OpenAPI_motion_event_info_t *motion_event_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (motion_event_info == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [MotionEventInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "linearDistance", motion_event_info->linear_distance) == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [linear_distance]");
        goto end;
    }

    if (motion_event_info->occurrence_info) {
    cJSON *occurrence_info_local_JSON = OpenAPI_occurrence_info_convertToJSON(motion_event_info->occurrence_info);
    if (occurrence_info_local_JSON == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [occurrence_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "occurrenceInfo", occurrence_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [occurrence_info]");
        goto end;
    }
    }

    if (motion_event_info->is_minimum_interval) {
    if (cJSON_AddNumberToObject(item, "minimumInterval", motion_event_info->minimum_interval) == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [minimum_interval]");
        goto end;
    }
    }

    if (motion_event_info->is_maximum_interval) {
    if (cJSON_AddNumberToObject(item, "maximumInterval", motion_event_info->maximum_interval) == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [maximum_interval]");
        goto end;
    }
    }

    if (motion_event_info->is_sampling_interval) {
    if (cJSON_AddNumberToObject(item, "samplingInterval", motion_event_info->sampling_interval) == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [sampling_interval]");
        goto end;
    }
    }

    if (motion_event_info->is_reporting_duration) {
    if (cJSON_AddNumberToObject(item, "reportingDuration", motion_event_info->reporting_duration) == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [reporting_duration]");
        goto end;
    }
    }

    if (motion_event_info->is_reporting_location_req) {
    if (cJSON_AddBoolToObject(item, "reportingLocationReq", motion_event_info->reporting_location_req) == NULL) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed [reporting_location_req]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_motion_event_info_t *OpenAPI_motion_event_info_parseFromJSON(cJSON *motion_event_infoJSON)
{
    OpenAPI_motion_event_info_t *motion_event_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *linear_distance = NULL;
    cJSON *occurrence_info = NULL;
    OpenAPI_occurrence_info_t *occurrence_info_local_nonprim = NULL;
    cJSON *minimum_interval = NULL;
    cJSON *maximum_interval = NULL;
    cJSON *sampling_interval = NULL;
    cJSON *reporting_duration = NULL;
    cJSON *reporting_location_req = NULL;
    linear_distance = cJSON_GetObjectItemCaseSensitive(motion_event_infoJSON, "linearDistance");
    if (!linear_distance) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON() failed [linear_distance]");
        goto end;
    }
    if (!cJSON_IsNumber(linear_distance)) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON() failed [linear_distance]");
        goto end;
    }

    occurrence_info = cJSON_GetObjectItemCaseSensitive(motion_event_infoJSON, "occurrenceInfo");
    if (occurrence_info) {
    occurrence_info_local_nonprim = OpenAPI_occurrence_info_parseFromJSON(occurrence_info);
    if (!occurrence_info_local_nonprim) {
        ogs_error("OpenAPI_occurrence_info_parseFromJSON failed [occurrence_info]");
        goto end;
    }
    }

    minimum_interval = cJSON_GetObjectItemCaseSensitive(motion_event_infoJSON, "minimumInterval");
    if (minimum_interval) {
    if (!cJSON_IsNumber(minimum_interval)) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON() failed [minimum_interval]");
        goto end;
    }
    }

    maximum_interval = cJSON_GetObjectItemCaseSensitive(motion_event_infoJSON, "maximumInterval");
    if (maximum_interval) {
    if (!cJSON_IsNumber(maximum_interval)) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON() failed [maximum_interval]");
        goto end;
    }
    }

    sampling_interval = cJSON_GetObjectItemCaseSensitive(motion_event_infoJSON, "samplingInterval");
    if (sampling_interval) {
    if (!cJSON_IsNumber(sampling_interval)) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON() failed [sampling_interval]");
        goto end;
    }
    }

    reporting_duration = cJSON_GetObjectItemCaseSensitive(motion_event_infoJSON, "reportingDuration");
    if (reporting_duration) {
    if (!cJSON_IsNumber(reporting_duration)) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON() failed [reporting_duration]");
        goto end;
    }
    }

    reporting_location_req = cJSON_GetObjectItemCaseSensitive(motion_event_infoJSON, "reportingLocationReq");
    if (reporting_location_req) {
    if (!cJSON_IsBool(reporting_location_req)) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON() failed [reporting_location_req]");
        goto end;
    }
    }

    motion_event_info_local_var = OpenAPI_motion_event_info_create (
        
        linear_distance->valuedouble,
        occurrence_info ? occurrence_info_local_nonprim : NULL,
        minimum_interval ? true : false,
        minimum_interval ? minimum_interval->valuedouble : 0,
        maximum_interval ? true : false,
        maximum_interval ? maximum_interval->valuedouble : 0,
        sampling_interval ? true : false,
        sampling_interval ? sampling_interval->valuedouble : 0,
        reporting_duration ? true : false,
        reporting_duration ? reporting_duration->valuedouble : 0,
        reporting_location_req ? true : false,
        reporting_location_req ? reporting_location_req->valueint : 0
    );

    return motion_event_info_local_var;
end:
    if (occurrence_info_local_nonprim) {
        OpenAPI_occurrence_info_free(occurrence_info_local_nonprim);
        occurrence_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_motion_event_info_t *OpenAPI_motion_event_info_copy(OpenAPI_motion_event_info_t *dst, OpenAPI_motion_event_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_motion_event_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_motion_event_info_convertToJSON() failed");
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

    OpenAPI_motion_event_info_free(dst);
    dst = OpenAPI_motion_event_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

