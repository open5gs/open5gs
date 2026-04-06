
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_access_behavior_report_item.h"

OpenAPI_ue_access_behavior_report_item_t *OpenAPI_ue_access_behavior_report_item_create(
    OpenAPI_access_state_transition_type_e state_transition_type,
    int spacing,
    bool is_spacing_var,
    float spacing_var,
    int duration,
    bool is_duration_var,
    float duration_var,
    bool is_occurrences,
    int occurrences,
    char *timestamp
)
{
    OpenAPI_ue_access_behavior_report_item_t *ue_access_behavior_report_item_local_var = ogs_malloc(sizeof(OpenAPI_ue_access_behavior_report_item_t));
    ogs_assert(ue_access_behavior_report_item_local_var);

    ue_access_behavior_report_item_local_var->state_transition_type = state_transition_type;
    ue_access_behavior_report_item_local_var->spacing = spacing;
    ue_access_behavior_report_item_local_var->is_spacing_var = is_spacing_var;
    ue_access_behavior_report_item_local_var->spacing_var = spacing_var;
    ue_access_behavior_report_item_local_var->duration = duration;
    ue_access_behavior_report_item_local_var->is_duration_var = is_duration_var;
    ue_access_behavior_report_item_local_var->duration_var = duration_var;
    ue_access_behavior_report_item_local_var->is_occurrences = is_occurrences;
    ue_access_behavior_report_item_local_var->occurrences = occurrences;
    ue_access_behavior_report_item_local_var->timestamp = timestamp;

    return ue_access_behavior_report_item_local_var;
}

void OpenAPI_ue_access_behavior_report_item_free(OpenAPI_ue_access_behavior_report_item_t *ue_access_behavior_report_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_access_behavior_report_item) {
        return;
    }
    if (ue_access_behavior_report_item->timestamp) {
        ogs_free(ue_access_behavior_report_item->timestamp);
        ue_access_behavior_report_item->timestamp = NULL;
    }
    ogs_free(ue_access_behavior_report_item);
}

cJSON *OpenAPI_ue_access_behavior_report_item_convertToJSON(OpenAPI_ue_access_behavior_report_item_t *ue_access_behavior_report_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_access_behavior_report_item == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [UeAccessBehaviorReportItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_access_behavior_report_item->state_transition_type == OpenAPI_access_state_transition_type_NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [state_transition_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "stateTransitionType", OpenAPI_access_state_transition_type_ToString(ue_access_behavior_report_item->state_transition_type)) == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [state_transition_type]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "spacing", ue_access_behavior_report_item->spacing) == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [spacing]");
        goto end;
    }

    if (ue_access_behavior_report_item->is_spacing_var) {
    if (cJSON_AddNumberToObject(item, "spacingVar", ue_access_behavior_report_item->spacing_var) == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [spacing_var]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "duration", ue_access_behavior_report_item->duration) == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [duration]");
        goto end;
    }

    if (ue_access_behavior_report_item->is_duration_var) {
    if (cJSON_AddNumberToObject(item, "durationVar", ue_access_behavior_report_item->duration_var) == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [duration_var]");
        goto end;
    }
    }

    if (ue_access_behavior_report_item->is_occurrences) {
    if (cJSON_AddNumberToObject(item, "occurrences", ue_access_behavior_report_item->occurrences) == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [occurrences]");
        goto end;
    }
    }

    if (ue_access_behavior_report_item->timestamp) {
    if (cJSON_AddStringToObject(item, "timestamp", ue_access_behavior_report_item->timestamp) == NULL) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed [timestamp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_access_behavior_report_item_t *OpenAPI_ue_access_behavior_report_item_parseFromJSON(cJSON *ue_access_behavior_report_itemJSON)
{
    OpenAPI_ue_access_behavior_report_item_t *ue_access_behavior_report_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *state_transition_type = NULL;
    OpenAPI_access_state_transition_type_e state_transition_typeVariable = 0;
    cJSON *spacing = NULL;
    cJSON *spacing_var = NULL;
    cJSON *duration = NULL;
    cJSON *duration_var = NULL;
    cJSON *occurrences = NULL;
    cJSON *timestamp = NULL;
    state_transition_type = cJSON_GetObjectItemCaseSensitive(ue_access_behavior_report_itemJSON, "stateTransitionType");
    if (!state_transition_type) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [state_transition_type]");
        goto end;
    }
    if (!cJSON_IsString(state_transition_type)) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [state_transition_type]");
        goto end;
    }
    state_transition_typeVariable = OpenAPI_access_state_transition_type_FromString(state_transition_type->valuestring);

    spacing = cJSON_GetObjectItemCaseSensitive(ue_access_behavior_report_itemJSON, "spacing");
    if (!spacing) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [spacing]");
        goto end;
    }
    if (!cJSON_IsNumber(spacing)) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [spacing]");
        goto end;
    }

    spacing_var = cJSON_GetObjectItemCaseSensitive(ue_access_behavior_report_itemJSON, "spacingVar");
    if (spacing_var) {
    if (!cJSON_IsNumber(spacing_var)) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [spacing_var]");
        goto end;
    }
    }

    duration = cJSON_GetObjectItemCaseSensitive(ue_access_behavior_report_itemJSON, "duration");
    if (!duration) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [duration]");
        goto end;
    }
    if (!cJSON_IsNumber(duration)) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [duration]");
        goto end;
    }

    duration_var = cJSON_GetObjectItemCaseSensitive(ue_access_behavior_report_itemJSON, "durationVar");
    if (duration_var) {
    if (!cJSON_IsNumber(duration_var)) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [duration_var]");
        goto end;
    }
    }

    occurrences = cJSON_GetObjectItemCaseSensitive(ue_access_behavior_report_itemJSON, "occurrences");
    if (occurrences) {
    if (!cJSON_IsNumber(occurrences)) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [occurrences]");
        goto end;
    }
    }

    timestamp = cJSON_GetObjectItemCaseSensitive(ue_access_behavior_report_itemJSON, "timestamp");
    if (timestamp) {
    if (!cJSON_IsString(timestamp) && !cJSON_IsNull(timestamp)) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_parseFromJSON() failed [timestamp]");
        goto end;
    }
    }

    ue_access_behavior_report_item_local_var = OpenAPI_ue_access_behavior_report_item_create (
        state_transition_typeVariable,
        
        spacing->valuedouble,
        spacing_var ? true : false,
        spacing_var ? spacing_var->valuedouble : 0,
        
        duration->valuedouble,
        duration_var ? true : false,
        duration_var ? duration_var->valuedouble : 0,
        occurrences ? true : false,
        occurrences ? occurrences->valuedouble : 0,
        timestamp && !cJSON_IsNull(timestamp) ? ogs_strdup(timestamp->valuestring) : NULL
    );

    return ue_access_behavior_report_item_local_var;
end:
    return NULL;
}

OpenAPI_ue_access_behavior_report_item_t *OpenAPI_ue_access_behavior_report_item_copy(OpenAPI_ue_access_behavior_report_item_t *dst, OpenAPI_ue_access_behavior_report_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_access_behavior_report_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_access_behavior_report_item_convertToJSON() failed");
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

    OpenAPI_ue_access_behavior_report_item_free(dst);
    dst = OpenAPI_ue_access_behavior_report_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

