
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_trigger.h"

OpenAPI_charging_trigger_t *OpenAPI_charging_trigger_create(
    OpenAPI_trigger_type_e trigger_type,
    OpenAPI_trigger_category_e trigger_category,
    bool is_time_limit,
    int time_limit,
    bool is_volume_limit,
    int volume_limit,
    bool is_volume_limit64,
    long volume_limit64,
    bool is_event_limit,
    int event_limit,
    bool is_max_number_ofccc,
    int max_number_ofccc,
    char *tariff_time_change
)
{
    OpenAPI_charging_trigger_t *charging_trigger_local_var = ogs_malloc(sizeof(OpenAPI_charging_trigger_t));
    ogs_assert(charging_trigger_local_var);

    charging_trigger_local_var->trigger_type = trigger_type;
    charging_trigger_local_var->trigger_category = trigger_category;
    charging_trigger_local_var->is_time_limit = is_time_limit;
    charging_trigger_local_var->time_limit = time_limit;
    charging_trigger_local_var->is_volume_limit = is_volume_limit;
    charging_trigger_local_var->volume_limit = volume_limit;
    charging_trigger_local_var->is_volume_limit64 = is_volume_limit64;
    charging_trigger_local_var->volume_limit64 = volume_limit64;
    charging_trigger_local_var->is_event_limit = is_event_limit;
    charging_trigger_local_var->event_limit = event_limit;
    charging_trigger_local_var->is_max_number_ofccc = is_max_number_ofccc;
    charging_trigger_local_var->max_number_ofccc = max_number_ofccc;
    charging_trigger_local_var->tariff_time_change = tariff_time_change;

    return charging_trigger_local_var;
}

void OpenAPI_charging_trigger_free(OpenAPI_charging_trigger_t *charging_trigger)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_trigger) {
        return;
    }
    if (charging_trigger->tariff_time_change) {
        ogs_free(charging_trigger->tariff_time_change);
        charging_trigger->tariff_time_change = NULL;
    }
    ogs_free(charging_trigger);
}

cJSON *OpenAPI_charging_trigger_convertToJSON(OpenAPI_charging_trigger_t *charging_trigger)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_trigger == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [ChargingTrigger]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_trigger->trigger_type != OpenAPI_trigger_type_NULL) {
    if (cJSON_AddStringToObject(item, "triggerType", OpenAPI_trigger_type_ToString(charging_trigger->trigger_type)) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [trigger_type]");
        goto end;
    }
    }

    if (charging_trigger->trigger_category == OpenAPI_trigger_category_NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [trigger_category]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "triggerCategory", OpenAPI_trigger_category_ToString(charging_trigger->trigger_category)) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [trigger_category]");
        goto end;
    }

    if (charging_trigger->is_time_limit) {
    if (cJSON_AddNumberToObject(item, "timeLimit", charging_trigger->time_limit) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [time_limit]");
        goto end;
    }
    }

    if (charging_trigger->is_volume_limit) {
    if (cJSON_AddNumberToObject(item, "volumeLimit", charging_trigger->volume_limit) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [volume_limit]");
        goto end;
    }
    }

    if (charging_trigger->is_volume_limit64) {
    if (cJSON_AddNumberToObject(item, "volumeLimit64", charging_trigger->volume_limit64) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [volume_limit64]");
        goto end;
    }
    }

    if (charging_trigger->is_event_limit) {
    if (cJSON_AddNumberToObject(item, "eventLimit", charging_trigger->event_limit) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [event_limit]");
        goto end;
    }
    }

    if (charging_trigger->is_max_number_ofccc) {
    if (cJSON_AddNumberToObject(item, "maxNumberOfccc", charging_trigger->max_number_ofccc) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [max_number_ofccc]");
        goto end;
    }
    }

    if (charging_trigger->tariff_time_change) {
    if (cJSON_AddStringToObject(item, "tariffTimeChange", charging_trigger->tariff_time_change) == NULL) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed [tariff_time_change]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_trigger_t *OpenAPI_charging_trigger_parseFromJSON(cJSON *charging_triggerJSON)
{
    OpenAPI_charging_trigger_t *charging_trigger_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *trigger_type = NULL;
    OpenAPI_trigger_type_e trigger_typeVariable = 0;
    cJSON *trigger_category = NULL;
    OpenAPI_trigger_category_e trigger_categoryVariable = 0;
    cJSON *time_limit = NULL;
    cJSON *volume_limit = NULL;
    cJSON *volume_limit64 = NULL;
    cJSON *event_limit = NULL;
    cJSON *max_number_ofccc = NULL;
    cJSON *tariff_time_change = NULL;
    trigger_type = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "triggerType");
    if (trigger_type) {
    if (!cJSON_IsString(trigger_type)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [trigger_type]");
        goto end;
    }
    trigger_typeVariable = OpenAPI_trigger_type_FromString(trigger_type->valuestring);
    }

    trigger_category = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "triggerCategory");
    if (!trigger_category) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [trigger_category]");
        goto end;
    }
    if (!cJSON_IsString(trigger_category)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [trigger_category]");
        goto end;
    }
    trigger_categoryVariable = OpenAPI_trigger_category_FromString(trigger_category->valuestring);

    time_limit = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "timeLimit");
    if (time_limit) {
    if (!cJSON_IsNumber(time_limit)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [time_limit]");
        goto end;
    }
    }

    volume_limit = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "volumeLimit");
    if (volume_limit) {
    if (!cJSON_IsNumber(volume_limit)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [volume_limit]");
        goto end;
    }
    }

    volume_limit64 = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "volumeLimit64");
    if (volume_limit64) {
    if (!cJSON_IsNumber(volume_limit64)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [volume_limit64]");
        goto end;
    }
    }

    event_limit = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "eventLimit");
    if (event_limit) {
    if (!cJSON_IsNumber(event_limit)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [event_limit]");
        goto end;
    }
    }

    max_number_ofccc = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "maxNumberOfccc");
    if (max_number_ofccc) {
    if (!cJSON_IsNumber(max_number_ofccc)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [max_number_ofccc]");
        goto end;
    }
    }

    tariff_time_change = cJSON_GetObjectItemCaseSensitive(charging_triggerJSON, "tariffTimeChange");
    if (tariff_time_change) {
    if (!cJSON_IsString(tariff_time_change) && !cJSON_IsNull(tariff_time_change)) {
        ogs_error("OpenAPI_charging_trigger_parseFromJSON() failed [tariff_time_change]");
        goto end;
    }
    }

    charging_trigger_local_var = OpenAPI_charging_trigger_create (
        trigger_type ? trigger_typeVariable : 0,
        trigger_categoryVariable,
        time_limit ? true : false,
        time_limit ? time_limit->valuedouble : 0,
        volume_limit ? true : false,
        volume_limit ? volume_limit->valuedouble : 0,
        volume_limit64 ? true : false,
        volume_limit64 ? volume_limit64->valuedouble : 0,
        event_limit ? true : false,
        event_limit ? event_limit->valuedouble : 0,
        max_number_ofccc ? true : false,
        max_number_ofccc ? max_number_ofccc->valuedouble : 0,
        tariff_time_change && !cJSON_IsNull(tariff_time_change) ? ogs_strdup(tariff_time_change->valuestring) : NULL
    );

    return charging_trigger_local_var;
end:
    return NULL;
}

OpenAPI_charging_trigger_t *OpenAPI_charging_trigger_copy(OpenAPI_charging_trigger_t *dst, OpenAPI_charging_trigger_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_trigger_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_trigger_convertToJSON() failed");
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

    OpenAPI_charging_trigger_free(dst);
    dst = OpenAPI_charging_trigger_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

