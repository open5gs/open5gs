
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_differentiation_info.h"

OpenAPI_ue_differentiation_info_t *OpenAPI_ue_differentiation_info_create(
    OpenAPI_periodic_communication_indicator_e periodic_com_ind,
    bool is_periodic_time,
    int periodic_time,
    OpenAPI_scheduled_communication_time_t *scheduled_com_time,
    OpenAPI_stationary_indication_e stationary_ind,
    OpenAPI_traffic_profile_e traffic_profile,
    OpenAPI_battery_indication_t *battery_ind,
    char *validity_time
)
{
    OpenAPI_ue_differentiation_info_t *ue_differentiation_info_local_var = ogs_malloc(sizeof(OpenAPI_ue_differentiation_info_t));
    ogs_assert(ue_differentiation_info_local_var);

    ue_differentiation_info_local_var->periodic_com_ind = periodic_com_ind;
    ue_differentiation_info_local_var->is_periodic_time = is_periodic_time;
    ue_differentiation_info_local_var->periodic_time = periodic_time;
    ue_differentiation_info_local_var->scheduled_com_time = scheduled_com_time;
    ue_differentiation_info_local_var->stationary_ind = stationary_ind;
    ue_differentiation_info_local_var->traffic_profile = traffic_profile;
    ue_differentiation_info_local_var->battery_ind = battery_ind;
    ue_differentiation_info_local_var->validity_time = validity_time;

    return ue_differentiation_info_local_var;
}

void OpenAPI_ue_differentiation_info_free(OpenAPI_ue_differentiation_info_t *ue_differentiation_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_differentiation_info) {
        return;
    }
    if (ue_differentiation_info->scheduled_com_time) {
        OpenAPI_scheduled_communication_time_free(ue_differentiation_info->scheduled_com_time);
        ue_differentiation_info->scheduled_com_time = NULL;
    }
    if (ue_differentiation_info->battery_ind) {
        OpenAPI_battery_indication_free(ue_differentiation_info->battery_ind);
        ue_differentiation_info->battery_ind = NULL;
    }
    if (ue_differentiation_info->validity_time) {
        ogs_free(ue_differentiation_info->validity_time);
        ue_differentiation_info->validity_time = NULL;
    }
    ogs_free(ue_differentiation_info);
}

cJSON *OpenAPI_ue_differentiation_info_convertToJSON(OpenAPI_ue_differentiation_info_t *ue_differentiation_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_differentiation_info == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [UeDifferentiationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_differentiation_info->periodic_com_ind != OpenAPI_periodic_communication_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "periodicComInd", OpenAPI_periodic_communication_indicator_ToString(ue_differentiation_info->periodic_com_ind)) == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [periodic_com_ind]");
        goto end;
    }
    }

    if (ue_differentiation_info->is_periodic_time) {
    if (cJSON_AddNumberToObject(item, "periodicTime", ue_differentiation_info->periodic_time) == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [periodic_time]");
        goto end;
    }
    }

    if (ue_differentiation_info->scheduled_com_time) {
    cJSON *scheduled_com_time_local_JSON = OpenAPI_scheduled_communication_time_convertToJSON(ue_differentiation_info->scheduled_com_time);
    if (scheduled_com_time_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [scheduled_com_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "scheduledComTime", scheduled_com_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [scheduled_com_time]");
        goto end;
    }
    }

    if (ue_differentiation_info->stationary_ind != OpenAPI_stationary_indication_NULL) {
    if (cJSON_AddStringToObject(item, "stationaryInd", OpenAPI_stationary_indication_ToString(ue_differentiation_info->stationary_ind)) == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [stationary_ind]");
        goto end;
    }
    }

    if (ue_differentiation_info->traffic_profile != OpenAPI_traffic_profile_NULL) {
    if (cJSON_AddStringToObject(item, "trafficProfile", OpenAPI_traffic_profile_ToString(ue_differentiation_info->traffic_profile)) == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [traffic_profile]");
        goto end;
    }
    }

    if (ue_differentiation_info->battery_ind) {
    cJSON *battery_ind_local_JSON = OpenAPI_battery_indication_convertToJSON(ue_differentiation_info->battery_ind);
    if (battery_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [battery_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "batteryInd", battery_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [battery_ind]");
        goto end;
    }
    }

    if (ue_differentiation_info->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", ue_differentiation_info->validity_time) == NULL) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_differentiation_info_t *OpenAPI_ue_differentiation_info_parseFromJSON(cJSON *ue_differentiation_infoJSON)
{
    OpenAPI_ue_differentiation_info_t *ue_differentiation_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *periodic_com_ind = NULL;
    OpenAPI_periodic_communication_indicator_e periodic_com_indVariable = 0;
    cJSON *periodic_time = NULL;
    cJSON *scheduled_com_time = NULL;
    OpenAPI_scheduled_communication_time_t *scheduled_com_time_local_nonprim = NULL;
    cJSON *stationary_ind = NULL;
    OpenAPI_stationary_indication_e stationary_indVariable = 0;
    cJSON *traffic_profile = NULL;
    OpenAPI_traffic_profile_e traffic_profileVariable = 0;
    cJSON *battery_ind = NULL;
    OpenAPI_battery_indication_t *battery_ind_local_nonprim = NULL;
    cJSON *validity_time = NULL;
    periodic_com_ind = cJSON_GetObjectItemCaseSensitive(ue_differentiation_infoJSON, "periodicComInd");
    if (periodic_com_ind) {
    if (!cJSON_IsString(periodic_com_ind)) {
        ogs_error("OpenAPI_ue_differentiation_info_parseFromJSON() failed [periodic_com_ind]");
        goto end;
    }
    periodic_com_indVariable = OpenAPI_periodic_communication_indicator_FromString(periodic_com_ind->valuestring);
    }

    periodic_time = cJSON_GetObjectItemCaseSensitive(ue_differentiation_infoJSON, "periodicTime");
    if (periodic_time) {
    if (!cJSON_IsNumber(periodic_time)) {
        ogs_error("OpenAPI_ue_differentiation_info_parseFromJSON() failed [periodic_time]");
        goto end;
    }
    }

    scheduled_com_time = cJSON_GetObjectItemCaseSensitive(ue_differentiation_infoJSON, "scheduledComTime");
    if (scheduled_com_time) {
    scheduled_com_time_local_nonprim = OpenAPI_scheduled_communication_time_parseFromJSON(scheduled_com_time);
    if (!scheduled_com_time_local_nonprim) {
        ogs_error("OpenAPI_scheduled_communication_time_parseFromJSON failed [scheduled_com_time]");
        goto end;
    }
    }

    stationary_ind = cJSON_GetObjectItemCaseSensitive(ue_differentiation_infoJSON, "stationaryInd");
    if (stationary_ind) {
    if (!cJSON_IsString(stationary_ind)) {
        ogs_error("OpenAPI_ue_differentiation_info_parseFromJSON() failed [stationary_ind]");
        goto end;
    }
    stationary_indVariable = OpenAPI_stationary_indication_FromString(stationary_ind->valuestring);
    }

    traffic_profile = cJSON_GetObjectItemCaseSensitive(ue_differentiation_infoJSON, "trafficProfile");
    if (traffic_profile) {
    if (!cJSON_IsString(traffic_profile)) {
        ogs_error("OpenAPI_ue_differentiation_info_parseFromJSON() failed [traffic_profile]");
        goto end;
    }
    traffic_profileVariable = OpenAPI_traffic_profile_FromString(traffic_profile->valuestring);
    }

    battery_ind = cJSON_GetObjectItemCaseSensitive(ue_differentiation_infoJSON, "batteryInd");
    if (battery_ind) {
    battery_ind_local_nonprim = OpenAPI_battery_indication_parseFromJSON(battery_ind);
    if (!battery_ind_local_nonprim) {
        ogs_error("OpenAPI_battery_indication_parseFromJSON failed [battery_ind]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(ue_differentiation_infoJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_ue_differentiation_info_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    ue_differentiation_info_local_var = OpenAPI_ue_differentiation_info_create (
        periodic_com_ind ? periodic_com_indVariable : 0,
        periodic_time ? true : false,
        periodic_time ? periodic_time->valuedouble : 0,
        scheduled_com_time ? scheduled_com_time_local_nonprim : NULL,
        stationary_ind ? stationary_indVariable : 0,
        traffic_profile ? traffic_profileVariable : 0,
        battery_ind ? battery_ind_local_nonprim : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL
    );

    return ue_differentiation_info_local_var;
end:
    if (scheduled_com_time_local_nonprim) {
        OpenAPI_scheduled_communication_time_free(scheduled_com_time_local_nonprim);
        scheduled_com_time_local_nonprim = NULL;
    }
    if (battery_ind_local_nonprim) {
        OpenAPI_battery_indication_free(battery_ind_local_nonprim);
        battery_ind_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_differentiation_info_t *OpenAPI_ue_differentiation_info_copy(OpenAPI_ue_differentiation_info_t *dst, OpenAPI_ue_differentiation_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_differentiation_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_differentiation_info_convertToJSON() failed");
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

    OpenAPI_ue_differentiation_info_free(dst);
    dst = OpenAPI_ue_differentiation_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

