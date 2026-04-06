
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_pos_qos.h"

OpenAPI_ranging_sl_pos_qos_t *OpenAPI_ranging_sl_pos_qos_create(
    bool is_h_accuracy,
    float h_accuracy,
    bool is_v_accuracy,
    float v_accuracy,
    bool is_relative_h_accuracy,
    float relative_h_accuracy,
    bool is_relative_v_accuracy,
    float relative_v_accuracy,
    bool is_distance_accuracy,
    float distance_accuracy,
    bool is_direction_accuracy,
    float direction_accuracy,
    bool is_vertical_requested,
    int vertical_requested,
    OpenAPI_response_time_e response_time,
    OpenAPI_lcs_qos_class_e lcs_qos_class,
    bool is_ranging_sl_pos_priority_level,
    int ranging_sl_pos_priority_level,
    bool is_ranging_sl_pos_delay_budget,
    int ranging_sl_pos_delay_budget
)
{
    OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_pos_qos_t));
    ogs_assert(ranging_sl_pos_qos_local_var);

    ranging_sl_pos_qos_local_var->is_h_accuracy = is_h_accuracy;
    ranging_sl_pos_qos_local_var->h_accuracy = h_accuracy;
    ranging_sl_pos_qos_local_var->is_v_accuracy = is_v_accuracy;
    ranging_sl_pos_qos_local_var->v_accuracy = v_accuracy;
    ranging_sl_pos_qos_local_var->is_relative_h_accuracy = is_relative_h_accuracy;
    ranging_sl_pos_qos_local_var->relative_h_accuracy = relative_h_accuracy;
    ranging_sl_pos_qos_local_var->is_relative_v_accuracy = is_relative_v_accuracy;
    ranging_sl_pos_qos_local_var->relative_v_accuracy = relative_v_accuracy;
    ranging_sl_pos_qos_local_var->is_distance_accuracy = is_distance_accuracy;
    ranging_sl_pos_qos_local_var->distance_accuracy = distance_accuracy;
    ranging_sl_pos_qos_local_var->is_direction_accuracy = is_direction_accuracy;
    ranging_sl_pos_qos_local_var->direction_accuracy = direction_accuracy;
    ranging_sl_pos_qos_local_var->is_vertical_requested = is_vertical_requested;
    ranging_sl_pos_qos_local_var->vertical_requested = vertical_requested;
    ranging_sl_pos_qos_local_var->response_time = response_time;
    ranging_sl_pos_qos_local_var->lcs_qos_class = lcs_qos_class;
    ranging_sl_pos_qos_local_var->is_ranging_sl_pos_priority_level = is_ranging_sl_pos_priority_level;
    ranging_sl_pos_qos_local_var->ranging_sl_pos_priority_level = ranging_sl_pos_priority_level;
    ranging_sl_pos_qos_local_var->is_ranging_sl_pos_delay_budget = is_ranging_sl_pos_delay_budget;
    ranging_sl_pos_qos_local_var->ranging_sl_pos_delay_budget = ranging_sl_pos_delay_budget;

    return ranging_sl_pos_qos_local_var;
}

void OpenAPI_ranging_sl_pos_qos_free(OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_pos_qos) {
        return;
    }
    ogs_free(ranging_sl_pos_qos);
}

cJSON *OpenAPI_ranging_sl_pos_qos_convertToJSON(OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_pos_qos == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [RangingSlPosQos]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_pos_qos->is_h_accuracy) {
    if (cJSON_AddNumberToObject(item, "hAccuracy", ranging_sl_pos_qos->h_accuracy) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [h_accuracy]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_v_accuracy) {
    if (cJSON_AddNumberToObject(item, "vAccuracy", ranging_sl_pos_qos->v_accuracy) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [v_accuracy]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_relative_h_accuracy) {
    if (cJSON_AddNumberToObject(item, "relativeHAccuracy", ranging_sl_pos_qos->relative_h_accuracy) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [relative_h_accuracy]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_relative_v_accuracy) {
    if (cJSON_AddNumberToObject(item, "relativeVAccuracy", ranging_sl_pos_qos->relative_v_accuracy) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [relative_v_accuracy]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_distance_accuracy) {
    if (cJSON_AddNumberToObject(item, "distanceAccuracy", ranging_sl_pos_qos->distance_accuracy) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [distance_accuracy]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_direction_accuracy) {
    if (cJSON_AddNumberToObject(item, "directionAccuracy", ranging_sl_pos_qos->direction_accuracy) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [direction_accuracy]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_vertical_requested) {
    if (cJSON_AddBoolToObject(item, "verticalRequested", ranging_sl_pos_qos->vertical_requested) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [vertical_requested]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->response_time != OpenAPI_response_time_NULL) {
    if (cJSON_AddStringToObject(item, "responseTime", OpenAPI_response_time_ToString(ranging_sl_pos_qos->response_time)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [response_time]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->lcs_qos_class != OpenAPI_lcs_qos_class_NULL) {
    if (cJSON_AddStringToObject(item, "lcsQosClass", OpenAPI_lcs_qos_class_ToString(ranging_sl_pos_qos->lcs_qos_class)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [lcs_qos_class]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_ranging_sl_pos_priority_level) {
    if (cJSON_AddNumberToObject(item, "rangingSlPosPriorityLevel", ranging_sl_pos_qos->ranging_sl_pos_priority_level) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [ranging_sl_pos_priority_level]");
        goto end;
    }
    }

    if (ranging_sl_pos_qos->is_ranging_sl_pos_delay_budget) {
    if (cJSON_AddNumberToObject(item, "rangingSlPosDelayBudget", ranging_sl_pos_qos->ranging_sl_pos_delay_budget) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed [ranging_sl_pos_delay_budget]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_pos_qos_t *OpenAPI_ranging_sl_pos_qos_parseFromJSON(cJSON *ranging_sl_pos_qosJSON)
{
    OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_accuracy = NULL;
    cJSON *v_accuracy = NULL;
    cJSON *relative_h_accuracy = NULL;
    cJSON *relative_v_accuracy = NULL;
    cJSON *distance_accuracy = NULL;
    cJSON *direction_accuracy = NULL;
    cJSON *vertical_requested = NULL;
    cJSON *response_time = NULL;
    OpenAPI_response_time_e response_timeVariable = 0;
    cJSON *lcs_qos_class = NULL;
    OpenAPI_lcs_qos_class_e lcs_qos_classVariable = 0;
    cJSON *ranging_sl_pos_priority_level = NULL;
    cJSON *ranging_sl_pos_delay_budget = NULL;
    h_accuracy = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "hAccuracy");
    if (h_accuracy) {
    if (!cJSON_IsNumber(h_accuracy)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [h_accuracy]");
        goto end;
    }
    }

    v_accuracy = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "vAccuracy");
    if (v_accuracy) {
    if (!cJSON_IsNumber(v_accuracy)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [v_accuracy]");
        goto end;
    }
    }

    relative_h_accuracy = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "relativeHAccuracy");
    if (relative_h_accuracy) {
    if (!cJSON_IsNumber(relative_h_accuracy)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [relative_h_accuracy]");
        goto end;
    }
    }

    relative_v_accuracy = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "relativeVAccuracy");
    if (relative_v_accuracy) {
    if (!cJSON_IsNumber(relative_v_accuracy)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [relative_v_accuracy]");
        goto end;
    }
    }

    distance_accuracy = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "distanceAccuracy");
    if (distance_accuracy) {
    if (!cJSON_IsNumber(distance_accuracy)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [distance_accuracy]");
        goto end;
    }
    }

    direction_accuracy = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "directionAccuracy");
    if (direction_accuracy) {
    if (!cJSON_IsNumber(direction_accuracy)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [direction_accuracy]");
        goto end;
    }
    }

    vertical_requested = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "verticalRequested");
    if (vertical_requested) {
    if (!cJSON_IsBool(vertical_requested)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [vertical_requested]");
        goto end;
    }
    }

    response_time = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "responseTime");
    if (response_time) {
    if (!cJSON_IsString(response_time)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [response_time]");
        goto end;
    }
    response_timeVariable = OpenAPI_response_time_FromString(response_time->valuestring);
    }

    lcs_qos_class = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "lcsQosClass");
    if (lcs_qos_class) {
    if (!cJSON_IsString(lcs_qos_class)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [lcs_qos_class]");
        goto end;
    }
    lcs_qos_classVariable = OpenAPI_lcs_qos_class_FromString(lcs_qos_class->valuestring);
    }

    ranging_sl_pos_priority_level = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "rangingSlPosPriorityLevel");
    if (ranging_sl_pos_priority_level) {
    if (!cJSON_IsNumber(ranging_sl_pos_priority_level)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [ranging_sl_pos_priority_level]");
        goto end;
    }
    }

    ranging_sl_pos_delay_budget = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_qosJSON, "rangingSlPosDelayBudget");
    if (ranging_sl_pos_delay_budget) {
    if (!cJSON_IsNumber(ranging_sl_pos_delay_budget)) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON() failed [ranging_sl_pos_delay_budget]");
        goto end;
    }
    }

    ranging_sl_pos_qos_local_var = OpenAPI_ranging_sl_pos_qos_create (
        h_accuracy ? true : false,
        h_accuracy ? h_accuracy->valuedouble : 0,
        v_accuracy ? true : false,
        v_accuracy ? v_accuracy->valuedouble : 0,
        relative_h_accuracy ? true : false,
        relative_h_accuracy ? relative_h_accuracy->valuedouble : 0,
        relative_v_accuracy ? true : false,
        relative_v_accuracy ? relative_v_accuracy->valuedouble : 0,
        distance_accuracy ? true : false,
        distance_accuracy ? distance_accuracy->valuedouble : 0,
        direction_accuracy ? true : false,
        direction_accuracy ? direction_accuracy->valuedouble : 0,
        vertical_requested ? true : false,
        vertical_requested ? vertical_requested->valueint : 0,
        response_time ? response_timeVariable : 0,
        lcs_qos_class ? lcs_qos_classVariable : 0,
        ranging_sl_pos_priority_level ? true : false,
        ranging_sl_pos_priority_level ? ranging_sl_pos_priority_level->valuedouble : 0,
        ranging_sl_pos_delay_budget ? true : false,
        ranging_sl_pos_delay_budget ? ranging_sl_pos_delay_budget->valuedouble : 0
    );

    return ranging_sl_pos_qos_local_var;
end:
    return NULL;
}

OpenAPI_ranging_sl_pos_qos_t *OpenAPI_ranging_sl_pos_qos_copy(OpenAPI_ranging_sl_pos_qos_t *dst, OpenAPI_ranging_sl_pos_qos_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_pos_qos_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_convertToJSON() failed");
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

    OpenAPI_ranging_sl_pos_qos_free(dst);
    dst = OpenAPI_ranging_sl_pos_qos_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

