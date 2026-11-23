
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc5_data_container.h"

OpenAPI_pc5_data_container_t *OpenAPI_pc5_data_container_create(
    char *local_sequence_number,
    char *change_time,
    bool is_coverage_status,
    int coverage_status,
    OpenAPI_charging_user_location_t *user_location_information,
    bool is_data_volume,
    long data_volume,
    char *change_condition,
    OpenAPI_radio_resources_indicator_e radio_resources_id,
    char *radio_frequency,
    char *p_c5_radio_technology
)
{
    OpenAPI_pc5_data_container_t *pc5_data_container_local_var = ogs_malloc(sizeof(OpenAPI_pc5_data_container_t));
    ogs_assert(pc5_data_container_local_var);

    pc5_data_container_local_var->local_sequence_number = local_sequence_number;
    pc5_data_container_local_var->change_time = change_time;
    pc5_data_container_local_var->is_coverage_status = is_coverage_status;
    pc5_data_container_local_var->coverage_status = coverage_status;
    pc5_data_container_local_var->user_location_information = user_location_information;
    pc5_data_container_local_var->is_data_volume = is_data_volume;
    pc5_data_container_local_var->data_volume = data_volume;
    pc5_data_container_local_var->change_condition = change_condition;
    pc5_data_container_local_var->radio_resources_id = radio_resources_id;
    pc5_data_container_local_var->radio_frequency = radio_frequency;
    pc5_data_container_local_var->p_c5_radio_technology = p_c5_radio_technology;

    return pc5_data_container_local_var;
}

void OpenAPI_pc5_data_container_free(OpenAPI_pc5_data_container_t *pc5_data_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pc5_data_container) {
        return;
    }
    if (pc5_data_container->local_sequence_number) {
        ogs_free(pc5_data_container->local_sequence_number);
        pc5_data_container->local_sequence_number = NULL;
    }
    if (pc5_data_container->change_time) {
        ogs_free(pc5_data_container->change_time);
        pc5_data_container->change_time = NULL;
    }
    if (pc5_data_container->user_location_information) {
        OpenAPI_charging_user_location_free(pc5_data_container->user_location_information);
        pc5_data_container->user_location_information = NULL;
    }
    if (pc5_data_container->change_condition) {
        ogs_free(pc5_data_container->change_condition);
        pc5_data_container->change_condition = NULL;
    }
    if (pc5_data_container->radio_frequency) {
        ogs_free(pc5_data_container->radio_frequency);
        pc5_data_container->radio_frequency = NULL;
    }
    if (pc5_data_container->p_c5_radio_technology) {
        ogs_free(pc5_data_container->p_c5_radio_technology);
        pc5_data_container->p_c5_radio_technology = NULL;
    }
    ogs_free(pc5_data_container);
}

cJSON *OpenAPI_pc5_data_container_convertToJSON(OpenAPI_pc5_data_container_t *pc5_data_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pc5_data_container == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [PC5DataContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pc5_data_container->local_sequence_number) {
    if (cJSON_AddStringToObject(item, "localSequenceNumber", pc5_data_container->local_sequence_number) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [local_sequence_number]");
        goto end;
    }
    }

    if (pc5_data_container->change_time) {
    if (cJSON_AddStringToObject(item, "changeTime", pc5_data_container->change_time) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [change_time]");
        goto end;
    }
    }

    if (pc5_data_container->is_coverage_status) {
    if (cJSON_AddBoolToObject(item, "coverageStatus", pc5_data_container->coverage_status) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [coverage_status]");
        goto end;
    }
    }

    if (pc5_data_container->user_location_information) {
    cJSON *user_location_information_local_JSON = OpenAPI_charging_user_location_convertToJSON(pc5_data_container->user_location_information);
    if (user_location_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [user_location_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInformation", user_location_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [user_location_information]");
        goto end;
    }
    }

    if (pc5_data_container->is_data_volume) {
    if (cJSON_AddNumberToObject(item, "dataVolume", pc5_data_container->data_volume) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [data_volume]");
        goto end;
    }
    }

    if (pc5_data_container->change_condition) {
    if (cJSON_AddStringToObject(item, "changeCondition", pc5_data_container->change_condition) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [change_condition]");
        goto end;
    }
    }

    if (pc5_data_container->radio_resources_id != OpenAPI_radio_resources_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "radioResourcesId", OpenAPI_radio_resources_indicator_ToString(pc5_data_container->radio_resources_id)) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [radio_resources_id]");
        goto end;
    }
    }

    if (pc5_data_container->radio_frequency) {
    if (cJSON_AddStringToObject(item, "radioFrequency", pc5_data_container->radio_frequency) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [radio_frequency]");
        goto end;
    }
    }

    if (pc5_data_container->p_c5_radio_technology) {
    if (cJSON_AddStringToObject(item, "pC5RadioTechnology", pc5_data_container->p_c5_radio_technology) == NULL) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed [p_c5_radio_technology]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pc5_data_container_t *OpenAPI_pc5_data_container_parseFromJSON(cJSON *pc5_data_containerJSON)
{
    OpenAPI_pc5_data_container_t *pc5_data_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *local_sequence_number = NULL;
    cJSON *change_time = NULL;
    cJSON *coverage_status = NULL;
    cJSON *user_location_information = NULL;
    OpenAPI_charging_user_location_t *user_location_information_local_nonprim = NULL;
    cJSON *data_volume = NULL;
    cJSON *change_condition = NULL;
    cJSON *radio_resources_id = NULL;
    OpenAPI_radio_resources_indicator_e radio_resources_idVariable = 0;
    cJSON *radio_frequency = NULL;
    cJSON *p_c5_radio_technology = NULL;
    local_sequence_number = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "localSequenceNumber");
    if (local_sequence_number) {
    if (!cJSON_IsString(local_sequence_number) && !cJSON_IsNull(local_sequence_number)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [local_sequence_number]");
        goto end;
    }
    }

    change_time = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "changeTime");
    if (change_time) {
    if (!cJSON_IsString(change_time) && !cJSON_IsNull(change_time)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [change_time]");
        goto end;
    }
    }

    coverage_status = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "coverageStatus");
    if (coverage_status) {
    if (!cJSON_IsBool(coverage_status)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [coverage_status]");
        goto end;
    }
    }

    user_location_information = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "userLocationInformation");
    if (user_location_information) {
    user_location_information_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_location_information);
    if (!user_location_information_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_location_information]");
        goto end;
    }
    }

    data_volume = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "dataVolume");
    if (data_volume) {
    if (!cJSON_IsNumber(data_volume)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [data_volume]");
        goto end;
    }
    }

    change_condition = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "changeCondition");
    if (change_condition) {
    if (!cJSON_IsString(change_condition) && !cJSON_IsNull(change_condition)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [change_condition]");
        goto end;
    }
    }

    radio_resources_id = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "radioResourcesId");
    if (radio_resources_id) {
    if (!cJSON_IsString(radio_resources_id)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [radio_resources_id]");
        goto end;
    }
    radio_resources_idVariable = OpenAPI_radio_resources_indicator_FromString(radio_resources_id->valuestring);
    }

    radio_frequency = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "radioFrequency");
    if (radio_frequency) {
    if (!cJSON_IsString(radio_frequency) && !cJSON_IsNull(radio_frequency)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [radio_frequency]");
        goto end;
    }
    }

    p_c5_radio_technology = cJSON_GetObjectItemCaseSensitive(pc5_data_containerJSON, "pC5RadioTechnology");
    if (p_c5_radio_technology) {
    if (!cJSON_IsString(p_c5_radio_technology) && !cJSON_IsNull(p_c5_radio_technology)) {
        ogs_error("OpenAPI_pc5_data_container_parseFromJSON() failed [p_c5_radio_technology]");
        goto end;
    }
    }

    pc5_data_container_local_var = OpenAPI_pc5_data_container_create (
        local_sequence_number && !cJSON_IsNull(local_sequence_number) ? ogs_strdup(local_sequence_number->valuestring) : NULL,
        change_time && !cJSON_IsNull(change_time) ? ogs_strdup(change_time->valuestring) : NULL,
        coverage_status ? true : false,
        coverage_status ? coverage_status->valueint : 0,
        user_location_information ? user_location_information_local_nonprim : NULL,
        data_volume ? true : false,
        data_volume ? data_volume->valuedouble : 0,
        change_condition && !cJSON_IsNull(change_condition) ? ogs_strdup(change_condition->valuestring) : NULL,
        radio_resources_id ? radio_resources_idVariable : 0,
        radio_frequency && !cJSON_IsNull(radio_frequency) ? ogs_strdup(radio_frequency->valuestring) : NULL,
        p_c5_radio_technology && !cJSON_IsNull(p_c5_radio_technology) ? ogs_strdup(p_c5_radio_technology->valuestring) : NULL
    );

    return pc5_data_container_local_var;
end:
    if (user_location_information_local_nonprim) {
        OpenAPI_charging_user_location_free(user_location_information_local_nonprim);
        user_location_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pc5_data_container_t *OpenAPI_pc5_data_container_copy(OpenAPI_pc5_data_container_t *dst, OpenAPI_pc5_data_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pc5_data_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pc5_data_container_convertToJSON() failed");
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

    OpenAPI_pc5_data_container_free(dst);
    dst = OpenAPI_pc5_data_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

