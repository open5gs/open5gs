
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "multiple_qf_icontainer.h"

OpenAPI_multiple_qf_icontainer_t *OpenAPI_multiple_qf_icontainer_create(
    OpenAPI_list_t *triggers,
    char *trigger_timestamp,
    bool is_time,
    int time,
    bool is_total_volume,
    long total_volume,
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume,
    int local_sequence_number,
    OpenAPI_qfi_container_information_t *q_fi_container_information
)
{
    OpenAPI_multiple_qf_icontainer_t *multiple_qf_icontainer_local_var = ogs_malloc(sizeof(OpenAPI_multiple_qf_icontainer_t));
    ogs_assert(multiple_qf_icontainer_local_var);

    multiple_qf_icontainer_local_var->triggers = triggers;
    multiple_qf_icontainer_local_var->trigger_timestamp = trigger_timestamp;
    multiple_qf_icontainer_local_var->is_time = is_time;
    multiple_qf_icontainer_local_var->time = time;
    multiple_qf_icontainer_local_var->is_total_volume = is_total_volume;
    multiple_qf_icontainer_local_var->total_volume = total_volume;
    multiple_qf_icontainer_local_var->is_uplink_volume = is_uplink_volume;
    multiple_qf_icontainer_local_var->uplink_volume = uplink_volume;
    multiple_qf_icontainer_local_var->is_downlink_volume = is_downlink_volume;
    multiple_qf_icontainer_local_var->downlink_volume = downlink_volume;
    multiple_qf_icontainer_local_var->local_sequence_number = local_sequence_number;
    multiple_qf_icontainer_local_var->q_fi_container_information = q_fi_container_information;

    return multiple_qf_icontainer_local_var;
}

void OpenAPI_multiple_qf_icontainer_free(OpenAPI_multiple_qf_icontainer_t *multiple_qf_icontainer)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == multiple_qf_icontainer) {
        return;
    }
    if (multiple_qf_icontainer->triggers) {
        OpenAPI_list_for_each(multiple_qf_icontainer->triggers, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(multiple_qf_icontainer->triggers);
        multiple_qf_icontainer->triggers = NULL;
    }
    if (multiple_qf_icontainer->trigger_timestamp) {
        ogs_free(multiple_qf_icontainer->trigger_timestamp);
        multiple_qf_icontainer->trigger_timestamp = NULL;
    }
    if (multiple_qf_icontainer->q_fi_container_information) {
        OpenAPI_qfi_container_information_free(multiple_qf_icontainer->q_fi_container_information);
        multiple_qf_icontainer->q_fi_container_information = NULL;
    }
    ogs_free(multiple_qf_icontainer);
}

cJSON *OpenAPI_multiple_qf_icontainer_convertToJSON(OpenAPI_multiple_qf_icontainer_t *multiple_qf_icontainer)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (multiple_qf_icontainer == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [MultipleQFIcontainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (multiple_qf_icontainer->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(multiple_qf_icontainer->triggers, node) {
        cJSON *itemLocal = OpenAPI_charging_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (multiple_qf_icontainer->trigger_timestamp) {
    if (cJSON_AddStringToObject(item, "triggerTimestamp", multiple_qf_icontainer->trigger_timestamp) == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [trigger_timestamp]");
        goto end;
    }
    }

    if (multiple_qf_icontainer->is_time) {
    if (cJSON_AddNumberToObject(item, "time", multiple_qf_icontainer->time) == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [time]");
        goto end;
    }
    }

    if (multiple_qf_icontainer->is_total_volume) {
    if (cJSON_AddNumberToObject(item, "totalVolume", multiple_qf_icontainer->total_volume) == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [total_volume]");
        goto end;
    }
    }

    if (multiple_qf_icontainer->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", multiple_qf_icontainer->uplink_volume) == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (multiple_qf_icontainer->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", multiple_qf_icontainer->downlink_volume) == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "localSequenceNumber", multiple_qf_icontainer->local_sequence_number) == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [local_sequence_number]");
        goto end;
    }

    if (multiple_qf_icontainer->q_fi_container_information) {
    cJSON *q_fi_container_information_local_JSON = OpenAPI_qfi_container_information_convertToJSON(multiple_qf_icontainer->q_fi_container_information);
    if (q_fi_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [q_fi_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qFIContainerInformation", q_fi_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed [q_fi_container_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_multiple_qf_icontainer_t *OpenAPI_multiple_qf_icontainer_parseFromJSON(cJSON *multiple_qf_icontainerJSON)
{
    OpenAPI_multiple_qf_icontainer_t *multiple_qf_icontainer_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *trigger_timestamp = NULL;
    cJSON *time = NULL;
    cJSON *total_volume = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *local_sequence_number = NULL;
    cJSON *q_fi_container_information = NULL;
    OpenAPI_qfi_container_information_t *q_fi_container_information_local_nonprim = NULL;
    triggers = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [triggers]");
                goto end;
            }
            OpenAPI_charging_trigger_t *triggersItem = OpenAPI_charging_trigger_parseFromJSON(triggers_local);
            if (!triggersItem) {
                ogs_error("No triggersItem");
                goto end;
            }
            OpenAPI_list_add(triggersList, triggersItem);
        }
    }

    trigger_timestamp = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "triggerTimestamp");
    if (trigger_timestamp) {
    if (!cJSON_IsString(trigger_timestamp) && !cJSON_IsNull(trigger_timestamp)) {
        ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [trigger_timestamp]");
        goto end;
    }
    }

    time = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "time");
    if (time) {
    if (!cJSON_IsNumber(time)) {
        ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [time]");
        goto end;
    }
    }

    total_volume = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "totalVolume");
    if (total_volume) {
    if (!cJSON_IsNumber(total_volume)) {
        ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [total_volume]");
        goto end;
    }
    }

    uplink_volume = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    local_sequence_number = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "localSequenceNumber");
    if (!local_sequence_number) {
        ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [local_sequence_number]");
        goto end;
    }
    if (!cJSON_IsNumber(local_sequence_number)) {
        ogs_error("OpenAPI_multiple_qf_icontainer_parseFromJSON() failed [local_sequence_number]");
        goto end;
    }

    q_fi_container_information = cJSON_GetObjectItemCaseSensitive(multiple_qf_icontainerJSON, "qFIContainerInformation");
    if (q_fi_container_information) {
    q_fi_container_information_local_nonprim = OpenAPI_qfi_container_information_parseFromJSON(q_fi_container_information);
    if (!q_fi_container_information_local_nonprim) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON failed [q_fi_container_information]");
        goto end;
    }
    }

    multiple_qf_icontainer_local_var = OpenAPI_multiple_qf_icontainer_create (
        triggers ? triggersList : NULL,
        trigger_timestamp && !cJSON_IsNull(trigger_timestamp) ? ogs_strdup(trigger_timestamp->valuestring) : NULL,
        time ? true : false,
        time ? time->valuedouble : 0,
        total_volume ? true : false,
        total_volume ? total_volume->valuedouble : 0,
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        
        local_sequence_number->valuedouble,
        q_fi_container_information ? q_fi_container_information_local_nonprim : NULL
    );

    return multiple_qf_icontainer_local_var;
end:
    if (triggersList) {
        OpenAPI_list_for_each(triggersList, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (q_fi_container_information_local_nonprim) {
        OpenAPI_qfi_container_information_free(q_fi_container_information_local_nonprim);
        q_fi_container_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_multiple_qf_icontainer_t *OpenAPI_multiple_qf_icontainer_copy(OpenAPI_multiple_qf_icontainer_t *dst, OpenAPI_multiple_qf_icontainer_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_multiple_qf_icontainer_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_multiple_qf_icontainer_convertToJSON() failed");
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

    OpenAPI_multiple_qf_icontainer_free(dst);
    dst = OpenAPI_multiple_qf_icontainer_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

