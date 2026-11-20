
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "used_unit_container.h"

OpenAPI_used_unit_container_t *OpenAPI_used_unit_container_create(
    bool is_service_id,
    int service_id,
    OpenAPI_quota_management_indicator_e quota_management_indicator,
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
    bool is_service_specific_units,
    long service_specific_units,
    OpenAPI_list_t *event_time_stamps,
    int local_sequence_number,
    OpenAPI_pdu_container_information_t *p_du_container_information,
    OpenAPI_nspa_container_information_t *n_spa_container_information,
    OpenAPI_network_sharing_container_information_t *network_sharing_container_information,
    OpenAPI_pc5_container_information_t *p_c5_container_information,
    OpenAPI_mbs_container_information_t *m_bs_container_information
)
{
    OpenAPI_used_unit_container_t *used_unit_container_local_var = ogs_malloc(sizeof(OpenAPI_used_unit_container_t));
    ogs_assert(used_unit_container_local_var);

    used_unit_container_local_var->is_service_id = is_service_id;
    used_unit_container_local_var->service_id = service_id;
    used_unit_container_local_var->quota_management_indicator = quota_management_indicator;
    used_unit_container_local_var->triggers = triggers;
    used_unit_container_local_var->trigger_timestamp = trigger_timestamp;
    used_unit_container_local_var->is_time = is_time;
    used_unit_container_local_var->time = time;
    used_unit_container_local_var->is_total_volume = is_total_volume;
    used_unit_container_local_var->total_volume = total_volume;
    used_unit_container_local_var->is_uplink_volume = is_uplink_volume;
    used_unit_container_local_var->uplink_volume = uplink_volume;
    used_unit_container_local_var->is_downlink_volume = is_downlink_volume;
    used_unit_container_local_var->downlink_volume = downlink_volume;
    used_unit_container_local_var->is_service_specific_units = is_service_specific_units;
    used_unit_container_local_var->service_specific_units = service_specific_units;
    used_unit_container_local_var->event_time_stamps = event_time_stamps;
    used_unit_container_local_var->local_sequence_number = local_sequence_number;
    used_unit_container_local_var->p_du_container_information = p_du_container_information;
    used_unit_container_local_var->n_spa_container_information = n_spa_container_information;
    used_unit_container_local_var->network_sharing_container_information = network_sharing_container_information;
    used_unit_container_local_var->p_c5_container_information = p_c5_container_information;
    used_unit_container_local_var->m_bs_container_information = m_bs_container_information;

    return used_unit_container_local_var;
}

void OpenAPI_used_unit_container_free(OpenAPI_used_unit_container_t *used_unit_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == used_unit_container) {
        return;
    }
    if (used_unit_container->triggers) {
        OpenAPI_list_for_each(used_unit_container->triggers, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(used_unit_container->triggers);
        used_unit_container->triggers = NULL;
    }
    if (used_unit_container->trigger_timestamp) {
        ogs_free(used_unit_container->trigger_timestamp);
        used_unit_container->trigger_timestamp = NULL;
    }
    if (used_unit_container->event_time_stamps) {
        OpenAPI_list_for_each(used_unit_container->event_time_stamps, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(used_unit_container->event_time_stamps);
        used_unit_container->event_time_stamps = NULL;
    }
    if (used_unit_container->p_du_container_information) {
        OpenAPI_pdu_container_information_free(used_unit_container->p_du_container_information);
        used_unit_container->p_du_container_information = NULL;
    }
    if (used_unit_container->n_spa_container_information) {
        OpenAPI_nspa_container_information_free(used_unit_container->n_spa_container_information);
        used_unit_container->n_spa_container_information = NULL;
    }
    if (used_unit_container->network_sharing_container_information) {
        OpenAPI_network_sharing_container_information_free(used_unit_container->network_sharing_container_information);
        used_unit_container->network_sharing_container_information = NULL;
    }
    if (used_unit_container->p_c5_container_information) {
        OpenAPI_pc5_container_information_free(used_unit_container->p_c5_container_information);
        used_unit_container->p_c5_container_information = NULL;
    }
    if (used_unit_container->m_bs_container_information) {
        OpenAPI_mbs_container_information_free(used_unit_container->m_bs_container_information);
        used_unit_container->m_bs_container_information = NULL;
    }
    ogs_free(used_unit_container);
}

cJSON *OpenAPI_used_unit_container_convertToJSON(OpenAPI_used_unit_container_t *used_unit_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (used_unit_container == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [UsedUnitContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (used_unit_container->is_service_id) {
    if (cJSON_AddNumberToObject(item, "serviceId", used_unit_container->service_id) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [service_id]");
        goto end;
    }
    }

    if (used_unit_container->quota_management_indicator != OpenAPI_quota_management_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "quotaManagementIndicator", OpenAPI_quota_management_indicator_ToString(used_unit_container->quota_management_indicator)) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [quota_management_indicator]");
        goto end;
    }
    }

    if (used_unit_container->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(used_unit_container->triggers, node) {
        cJSON *itemLocal = OpenAPI_charging_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (used_unit_container->trigger_timestamp) {
    if (cJSON_AddStringToObject(item, "triggerTimestamp", used_unit_container->trigger_timestamp) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [trigger_timestamp]");
        goto end;
    }
    }

    if (used_unit_container->is_time) {
    if (cJSON_AddNumberToObject(item, "time", used_unit_container->time) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [time]");
        goto end;
    }
    }

    if (used_unit_container->is_total_volume) {
    if (cJSON_AddNumberToObject(item, "totalVolume", used_unit_container->total_volume) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [total_volume]");
        goto end;
    }
    }

    if (used_unit_container->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", used_unit_container->uplink_volume) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (used_unit_container->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", used_unit_container->downlink_volume) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (used_unit_container->is_service_specific_units) {
    if (cJSON_AddNumberToObject(item, "serviceSpecificUnits", used_unit_container->service_specific_units) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [service_specific_units]");
        goto end;
    }
    }

    if (used_unit_container->event_time_stamps) {
    cJSON *event_time_stampsList = cJSON_AddArrayToObject(item, "eventTimeStamps");
    if (event_time_stampsList == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [event_time_stamps]");
        goto end;
    }
    OpenAPI_list_for_each(used_unit_container->event_time_stamps, node) {
    }
    }

    if (cJSON_AddNumberToObject(item, "localSequenceNumber", used_unit_container->local_sequence_number) == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [local_sequence_number]");
        goto end;
    }

    if (used_unit_container->p_du_container_information) {
    cJSON *p_du_container_information_local_JSON = OpenAPI_pdu_container_information_convertToJSON(used_unit_container->p_du_container_information);
    if (p_du_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [p_du_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pDUContainerInformation", p_du_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [p_du_container_information]");
        goto end;
    }
    }

    if (used_unit_container->n_spa_container_information) {
    cJSON *n_spa_container_information_local_JSON = OpenAPI_nspa_container_information_convertToJSON(used_unit_container->n_spa_container_information);
    if (n_spa_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [n_spa_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nSPAContainerInformation", n_spa_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [n_spa_container_information]");
        goto end;
    }
    }

    if (used_unit_container->network_sharing_container_information) {
    cJSON *network_sharing_container_information_local_JSON = OpenAPI_network_sharing_container_information_convertToJSON(used_unit_container->network_sharing_container_information);
    if (network_sharing_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [network_sharing_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkSharingContainerInformation", network_sharing_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [network_sharing_container_information]");
        goto end;
    }
    }

    if (used_unit_container->p_c5_container_information) {
    cJSON *p_c5_container_information_local_JSON = OpenAPI_pc5_container_information_convertToJSON(used_unit_container->p_c5_container_information);
    if (p_c5_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [p_c5_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pC5ContainerInformation", p_c5_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [p_c5_container_information]");
        goto end;
    }
    }

    if (used_unit_container->m_bs_container_information) {
    cJSON *m_bs_container_information_local_JSON = OpenAPI_mbs_container_information_convertToJSON(used_unit_container->m_bs_container_information);
    if (m_bs_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [m_bs_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mBSContainerInformation", m_bs_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed [m_bs_container_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_used_unit_container_t *OpenAPI_used_unit_container_parseFromJSON(cJSON *used_unit_containerJSON)
{
    OpenAPI_used_unit_container_t *used_unit_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *service_id = NULL;
    cJSON *quota_management_indicator = NULL;
    OpenAPI_quota_management_indicator_e quota_management_indicatorVariable = 0;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *trigger_timestamp = NULL;
    cJSON *time = NULL;
    cJSON *total_volume = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *service_specific_units = NULL;
    cJSON *event_time_stamps = NULL;
    OpenAPI_list_t *event_time_stampsList = NULL;
    cJSON *local_sequence_number = NULL;
    cJSON *p_du_container_information = NULL;
    OpenAPI_pdu_container_information_t *p_du_container_information_local_nonprim = NULL;
    cJSON *n_spa_container_information = NULL;
    OpenAPI_nspa_container_information_t *n_spa_container_information_local_nonprim = NULL;
    cJSON *network_sharing_container_information = NULL;
    OpenAPI_network_sharing_container_information_t *network_sharing_container_information_local_nonprim = NULL;
    cJSON *p_c5_container_information = NULL;
    OpenAPI_pc5_container_information_t *p_c5_container_information_local_nonprim = NULL;
    cJSON *m_bs_container_information = NULL;
    OpenAPI_mbs_container_information_t *m_bs_container_information_local_nonprim = NULL;
    service_id = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "serviceId");
    if (service_id) {
    if (!cJSON_IsNumber(service_id)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [service_id]");
        goto end;
    }
    }

    quota_management_indicator = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "quotaManagementIndicator");
    if (quota_management_indicator) {
    if (!cJSON_IsString(quota_management_indicator)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [quota_management_indicator]");
        goto end;
    }
    quota_management_indicatorVariable = OpenAPI_quota_management_indicator_FromString(quota_management_indicator->valuestring);
    }

    triggers = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [triggers]");
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

    trigger_timestamp = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "triggerTimestamp");
    if (trigger_timestamp) {
    if (!cJSON_IsString(trigger_timestamp) && !cJSON_IsNull(trigger_timestamp)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [trigger_timestamp]");
        goto end;
    }
    }

    time = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "time");
    if (time) {
    if (!cJSON_IsNumber(time)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [time]");
        goto end;
    }
    }

    total_volume = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "totalVolume");
    if (total_volume) {
    if (!cJSON_IsNumber(total_volume)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [total_volume]");
        goto end;
    }
    }

    uplink_volume = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    service_specific_units = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "serviceSpecificUnits");
    if (service_specific_units) {
    if (!cJSON_IsNumber(service_specific_units)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [service_specific_units]");
        goto end;
    }
    }

    event_time_stamps = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "eventTimeStamps");
    if (event_time_stamps) {
        cJSON *event_time_stamps_local = NULL;
        if (!cJSON_IsArray(event_time_stamps)) {
            ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [event_time_stamps]");
            goto end;
        }

        event_time_stampsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_time_stamps_local, event_time_stamps) {
            double *localDouble = NULL;
            int *localInt = NULL;
        }
    }

    local_sequence_number = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "localSequenceNumber");
    if (!local_sequence_number) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [local_sequence_number]");
        goto end;
    }
    if (!cJSON_IsNumber(local_sequence_number)) {
        ogs_error("OpenAPI_used_unit_container_parseFromJSON() failed [local_sequence_number]");
        goto end;
    }

    p_du_container_information = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "pDUContainerInformation");
    if (p_du_container_information) {
    p_du_container_information_local_nonprim = OpenAPI_pdu_container_information_parseFromJSON(p_du_container_information);
    if (!p_du_container_information_local_nonprim) {
        ogs_error("OpenAPI_pdu_container_information_parseFromJSON failed [p_du_container_information]");
        goto end;
    }
    }

    n_spa_container_information = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "nSPAContainerInformation");
    if (n_spa_container_information) {
    n_spa_container_information_local_nonprim = OpenAPI_nspa_container_information_parseFromJSON(n_spa_container_information);
    if (!n_spa_container_information_local_nonprim) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON failed [n_spa_container_information]");
        goto end;
    }
    }

    network_sharing_container_information = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "networkSharingContainerInformation");
    if (network_sharing_container_information) {
    network_sharing_container_information_local_nonprim = OpenAPI_network_sharing_container_information_parseFromJSON(network_sharing_container_information);
    if (!network_sharing_container_information_local_nonprim) {
        ogs_error("OpenAPI_network_sharing_container_information_parseFromJSON failed [network_sharing_container_information]");
        goto end;
    }
    }

    p_c5_container_information = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "pC5ContainerInformation");
    if (p_c5_container_information) {
    p_c5_container_information_local_nonprim = OpenAPI_pc5_container_information_parseFromJSON(p_c5_container_information);
    if (!p_c5_container_information_local_nonprim) {
        ogs_error("OpenAPI_pc5_container_information_parseFromJSON failed [p_c5_container_information]");
        goto end;
    }
    }

    m_bs_container_information = cJSON_GetObjectItemCaseSensitive(used_unit_containerJSON, "mBSContainerInformation");
    if (m_bs_container_information) {
    m_bs_container_information_local_nonprim = OpenAPI_mbs_container_information_parseFromJSON(m_bs_container_information);
    if (!m_bs_container_information_local_nonprim) {
        ogs_error("OpenAPI_mbs_container_information_parseFromJSON failed [m_bs_container_information]");
        goto end;
    }
    }

    used_unit_container_local_var = OpenAPI_used_unit_container_create (
        service_id ? true : false,
        service_id ? service_id->valuedouble : 0,
        quota_management_indicator ? quota_management_indicatorVariable : 0,
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
        service_specific_units ? true : false,
        service_specific_units ? service_specific_units->valuedouble : 0,
        event_time_stamps ? event_time_stampsList : NULL,
        
        local_sequence_number->valuedouble,
        p_du_container_information ? p_du_container_information_local_nonprim : NULL,
        n_spa_container_information ? n_spa_container_information_local_nonprim : NULL,
        network_sharing_container_information ? network_sharing_container_information_local_nonprim : NULL,
        p_c5_container_information ? p_c5_container_information_local_nonprim : NULL,
        m_bs_container_information ? m_bs_container_information_local_nonprim : NULL
    );

    return used_unit_container_local_var;
end:
    if (triggersList) {
        OpenAPI_list_for_each(triggersList, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (event_time_stampsList) {
        OpenAPI_list_for_each(event_time_stampsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_time_stampsList);
        event_time_stampsList = NULL;
    }
    if (p_du_container_information_local_nonprim) {
        OpenAPI_pdu_container_information_free(p_du_container_information_local_nonprim);
        p_du_container_information_local_nonprim = NULL;
    }
    if (n_spa_container_information_local_nonprim) {
        OpenAPI_nspa_container_information_free(n_spa_container_information_local_nonprim);
        n_spa_container_information_local_nonprim = NULL;
    }
    if (network_sharing_container_information_local_nonprim) {
        OpenAPI_network_sharing_container_information_free(network_sharing_container_information_local_nonprim);
        network_sharing_container_information_local_nonprim = NULL;
    }
    if (p_c5_container_information_local_nonprim) {
        OpenAPI_pc5_container_information_free(p_c5_container_information_local_nonprim);
        p_c5_container_information_local_nonprim = NULL;
    }
    if (m_bs_container_information_local_nonprim) {
        OpenAPI_mbs_container_information_free(m_bs_container_information_local_nonprim);
        m_bs_container_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_used_unit_container_t *OpenAPI_used_unit_container_copy(OpenAPI_used_unit_container_t *dst, OpenAPI_used_unit_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_used_unit_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_used_unit_container_convertToJSON() failed");
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

    OpenAPI_used_unit_container_free(dst);
    dst = OpenAPI_used_unit_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

