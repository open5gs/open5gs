
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allocated_unit.h"

OpenAPI_allocated_unit_t *OpenAPI_allocated_unit_create(
    OpenAPI_quota_management_indicator_e quota_management_indicator,
    OpenAPI_list_t *triggers,
    char *trigger_timestamp,
    int local_sequence_number,
    OpenAPI_nsac_container_information_t *n_sac_container_information
)
{
    OpenAPI_allocated_unit_t *allocated_unit_local_var = ogs_malloc(sizeof(OpenAPI_allocated_unit_t));
    ogs_assert(allocated_unit_local_var);

    allocated_unit_local_var->quota_management_indicator = quota_management_indicator;
    allocated_unit_local_var->triggers = triggers;
    allocated_unit_local_var->trigger_timestamp = trigger_timestamp;
    allocated_unit_local_var->local_sequence_number = local_sequence_number;
    allocated_unit_local_var->n_sac_container_information = n_sac_container_information;

    return allocated_unit_local_var;
}

void OpenAPI_allocated_unit_free(OpenAPI_allocated_unit_t *allocated_unit)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == allocated_unit) {
        return;
    }
    if (allocated_unit->triggers) {
        OpenAPI_list_for_each(allocated_unit->triggers, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(allocated_unit->triggers);
        allocated_unit->triggers = NULL;
    }
    if (allocated_unit->trigger_timestamp) {
        ogs_free(allocated_unit->trigger_timestamp);
        allocated_unit->trigger_timestamp = NULL;
    }
    if (allocated_unit->n_sac_container_information) {
        OpenAPI_nsac_container_information_free(allocated_unit->n_sac_container_information);
        allocated_unit->n_sac_container_information = NULL;
    }
    ogs_free(allocated_unit);
}

cJSON *OpenAPI_allocated_unit_convertToJSON(OpenAPI_allocated_unit_t *allocated_unit)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (allocated_unit == NULL) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [AllocatedUnit]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (allocated_unit->quota_management_indicator != OpenAPI_quota_management_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "quotaManagementIndicator", OpenAPI_quota_management_indicator_ToString(allocated_unit->quota_management_indicator)) == NULL) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [quota_management_indicator]");
        goto end;
    }
    }

    if (allocated_unit->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(allocated_unit->triggers, node) {
        cJSON *itemLocal = OpenAPI_charging_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (allocated_unit->trigger_timestamp) {
    if (cJSON_AddStringToObject(item, "triggerTimestamp", allocated_unit->trigger_timestamp) == NULL) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [trigger_timestamp]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "localSequenceNumber", allocated_unit->local_sequence_number) == NULL) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [local_sequence_number]");
        goto end;
    }

    if (allocated_unit->n_sac_container_information) {
    cJSON *n_sac_container_information_local_JSON = OpenAPI_nsac_container_information_convertToJSON(allocated_unit->n_sac_container_information);
    if (n_sac_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [n_sac_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nSACContainerInformation", n_sac_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed [n_sac_container_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_allocated_unit_t *OpenAPI_allocated_unit_parseFromJSON(cJSON *allocated_unitJSON)
{
    OpenAPI_allocated_unit_t *allocated_unit_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *quota_management_indicator = NULL;
    OpenAPI_quota_management_indicator_e quota_management_indicatorVariable = 0;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *trigger_timestamp = NULL;
    cJSON *local_sequence_number = NULL;
    cJSON *n_sac_container_information = NULL;
    OpenAPI_nsac_container_information_t *n_sac_container_information_local_nonprim = NULL;
    quota_management_indicator = cJSON_GetObjectItemCaseSensitive(allocated_unitJSON, "quotaManagementIndicator");
    if (quota_management_indicator) {
    if (!cJSON_IsString(quota_management_indicator)) {
        ogs_error("OpenAPI_allocated_unit_parseFromJSON() failed [quota_management_indicator]");
        goto end;
    }
    quota_management_indicatorVariable = OpenAPI_quota_management_indicator_FromString(quota_management_indicator->valuestring);
    }

    triggers = cJSON_GetObjectItemCaseSensitive(allocated_unitJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_allocated_unit_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI_allocated_unit_parseFromJSON() failed [triggers]");
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

    trigger_timestamp = cJSON_GetObjectItemCaseSensitive(allocated_unitJSON, "triggerTimestamp");
    if (trigger_timestamp) {
    if (!cJSON_IsString(trigger_timestamp) && !cJSON_IsNull(trigger_timestamp)) {
        ogs_error("OpenAPI_allocated_unit_parseFromJSON() failed [trigger_timestamp]");
        goto end;
    }
    }

    local_sequence_number = cJSON_GetObjectItemCaseSensitive(allocated_unitJSON, "localSequenceNumber");
    if (!local_sequence_number) {
        ogs_error("OpenAPI_allocated_unit_parseFromJSON() failed [local_sequence_number]");
        goto end;
    }
    if (!cJSON_IsNumber(local_sequence_number)) {
        ogs_error("OpenAPI_allocated_unit_parseFromJSON() failed [local_sequence_number]");
        goto end;
    }

    n_sac_container_information = cJSON_GetObjectItemCaseSensitive(allocated_unitJSON, "nSACContainerInformation");
    if (n_sac_container_information) {
    n_sac_container_information_local_nonprim = OpenAPI_nsac_container_information_parseFromJSON(n_sac_container_information);
    if (!n_sac_container_information_local_nonprim) {
        ogs_error("OpenAPI_nsac_container_information_parseFromJSON failed [n_sac_container_information]");
        goto end;
    }
    }

    allocated_unit_local_var = OpenAPI_allocated_unit_create (
        quota_management_indicator ? quota_management_indicatorVariable : 0,
        triggers ? triggersList : NULL,
        trigger_timestamp && !cJSON_IsNull(trigger_timestamp) ? ogs_strdup(trigger_timestamp->valuestring) : NULL,
        
        local_sequence_number->valuedouble,
        n_sac_container_information ? n_sac_container_information_local_nonprim : NULL
    );

    return allocated_unit_local_var;
end:
    if (triggersList) {
        OpenAPI_list_for_each(triggersList, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (n_sac_container_information_local_nonprim) {
        OpenAPI_nsac_container_information_free(n_sac_container_information_local_nonprim);
        n_sac_container_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_allocated_unit_t *OpenAPI_allocated_unit_copy(OpenAPI_allocated_unit_t *dst, OpenAPI_allocated_unit_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_allocated_unit_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_allocated_unit_convertToJSON() failed");
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

    OpenAPI_allocated_unit_free(dst);
    dst = OpenAPI_allocated_unit_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

