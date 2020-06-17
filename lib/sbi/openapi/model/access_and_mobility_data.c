
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_and_mobility_data.h"

OpenAPI_access_and_mobility_data_t *OpenAPI_access_and_mobility_data_create(
    OpenAPI_user_location_t *location,
    char *location_ts,
    char *time_zone,
    char *time_zone_ts,
    OpenAPI_access_type_e access_type,
    OpenAPI_list_t *reg_states,
    char *reg_states_ts,
    OpenAPI_list_t *conn_states,
    char *conn_states_ts,
    OpenAPI_ue_reachability_t *reachability_status,
    char *reachability_status_ts,
    OpenAPI_sms_support_e sms_over_nas_status,
    char *sms_over_nas_status_ts,
    int roaming_status,
    char *roaming_status_ts,
    OpenAPI_plmn_id_t *current_plmn,
    char *current_plmn_ts,
    OpenAPI_list_t *rat_type,
    char *rat_types_ts
    )
{
    OpenAPI_access_and_mobility_data_t *access_and_mobility_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_access_and_mobility_data_t));
    if (!access_and_mobility_data_local_var) {
        return NULL;
    }
    access_and_mobility_data_local_var->location = location;
    access_and_mobility_data_local_var->location_ts = location_ts;
    access_and_mobility_data_local_var->time_zone = time_zone;
    access_and_mobility_data_local_var->time_zone_ts = time_zone_ts;
    access_and_mobility_data_local_var->access_type = access_type;
    access_and_mobility_data_local_var->reg_states = reg_states;
    access_and_mobility_data_local_var->reg_states_ts = reg_states_ts;
    access_and_mobility_data_local_var->conn_states = conn_states;
    access_and_mobility_data_local_var->conn_states_ts = conn_states_ts;
    access_and_mobility_data_local_var->reachability_status = reachability_status;
    access_and_mobility_data_local_var->reachability_status_ts = reachability_status_ts;
    access_and_mobility_data_local_var->sms_over_nas_status = sms_over_nas_status;
    access_and_mobility_data_local_var->sms_over_nas_status_ts = sms_over_nas_status_ts;
    access_and_mobility_data_local_var->roaming_status = roaming_status;
    access_and_mobility_data_local_var->roaming_status_ts = roaming_status_ts;
    access_and_mobility_data_local_var->current_plmn = current_plmn;
    access_and_mobility_data_local_var->current_plmn_ts = current_plmn_ts;
    access_and_mobility_data_local_var->rat_type = rat_type;
    access_and_mobility_data_local_var->rat_types_ts = rat_types_ts;

    return access_and_mobility_data_local_var;
}

void OpenAPI_access_and_mobility_data_free(OpenAPI_access_and_mobility_data_t *access_and_mobility_data)
{
    if (NULL == access_and_mobility_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_user_location_free(access_and_mobility_data->location);
    ogs_free(access_and_mobility_data->location_ts);
    ogs_free(access_and_mobility_data->time_zone);
    ogs_free(access_and_mobility_data->time_zone_ts);
    OpenAPI_list_for_each(access_and_mobility_data->reg_states, node) {
        OpenAPI_rm_info_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_data->reg_states);
    ogs_free(access_and_mobility_data->reg_states_ts);
    OpenAPI_list_for_each(access_and_mobility_data->conn_states, node) {
        OpenAPI_cm_info_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_data->conn_states);
    ogs_free(access_and_mobility_data->conn_states_ts);
    OpenAPI_ue_reachability_free(access_and_mobility_data->reachability_status);
    ogs_free(access_and_mobility_data->reachability_status_ts);
    ogs_free(access_and_mobility_data->sms_over_nas_status_ts);
    ogs_free(access_and_mobility_data->roaming_status_ts);
    OpenAPI_plmn_id_free(access_and_mobility_data->current_plmn);
    ogs_free(access_and_mobility_data->current_plmn_ts);
    OpenAPI_list_free(access_and_mobility_data->rat_type);
    ogs_free(access_and_mobility_data->rat_types_ts);
    ogs_free(access_and_mobility_data);
}

cJSON *OpenAPI_access_and_mobility_data_convertToJSON(OpenAPI_access_and_mobility_data_t *access_and_mobility_data)
{
    cJSON *item = NULL;

    if (access_and_mobility_data == NULL) {
        ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [AccessAndMobilityData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (access_and_mobility_data->location) {
        cJSON *location_local_JSON = OpenAPI_user_location_convertToJSON(access_and_mobility_data->location);
        if (location_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [location]");
            goto end;
        }
        cJSON_AddItemToObject(item, "location", location_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [location]");
            goto end;
        }
    }

    if (access_and_mobility_data->location_ts) {
        if (cJSON_AddStringToObject(item, "locationTs", access_and_mobility_data->location_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [location_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->time_zone) {
        if (cJSON_AddStringToObject(item, "timeZone", access_and_mobility_data->time_zone) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [time_zone]");
            goto end;
        }
    }

    if (access_and_mobility_data->time_zone_ts) {
        if (cJSON_AddStringToObject(item, "timeZoneTs", access_and_mobility_data->time_zone_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [time_zone_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->access_type) {
        if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(access_and_mobility_data->access_type)) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [access_type]");
            goto end;
        }
    }

    if (access_and_mobility_data->reg_states) {
        cJSON *reg_statesList = cJSON_AddArrayToObject(item, "regStates");
        if (reg_statesList == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reg_states]");
            goto end;
        }

        OpenAPI_lnode_t *reg_states_node;
        if (access_and_mobility_data->reg_states) {
            OpenAPI_list_for_each(access_and_mobility_data->reg_states, reg_states_node) {
                cJSON *itemLocal = OpenAPI_rm_info_convertToJSON(reg_states_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reg_states]");
                    goto end;
                }
                cJSON_AddItemToArray(reg_statesList, itemLocal);
            }
        }
    }

    if (access_and_mobility_data->reg_states_ts) {
        if (cJSON_AddStringToObject(item, "regStatesTs", access_and_mobility_data->reg_states_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reg_states_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->conn_states) {
        cJSON *conn_statesList = cJSON_AddArrayToObject(item, "connStates");
        if (conn_statesList == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [conn_states]");
            goto end;
        }

        OpenAPI_lnode_t *conn_states_node;
        if (access_and_mobility_data->conn_states) {
            OpenAPI_list_for_each(access_and_mobility_data->conn_states, conn_states_node) {
                cJSON *itemLocal = OpenAPI_cm_info_convertToJSON(conn_states_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [conn_states]");
                    goto end;
                }
                cJSON_AddItemToArray(conn_statesList, itemLocal);
            }
        }
    }

    if (access_and_mobility_data->conn_states_ts) {
        if (cJSON_AddStringToObject(item, "connStatesTs", access_and_mobility_data->conn_states_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [conn_states_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->reachability_status) {
        cJSON *reachability_status_local_JSON = OpenAPI_ue_reachability_convertToJSON(access_and_mobility_data->reachability_status);
        if (reachability_status_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reachability_status]");
            goto end;
        }
        cJSON_AddItemToObject(item, "reachabilityStatus", reachability_status_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reachability_status]");
            goto end;
        }
    }

    if (access_and_mobility_data->reachability_status_ts) {
        if (cJSON_AddStringToObject(item, "reachabilityStatusTs", access_and_mobility_data->reachability_status_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reachability_status_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->sms_over_nas_status) {
        if (cJSON_AddStringToObject(item, "smsOverNasStatus", OpenAPI_sms_support_ToString(access_and_mobility_data->sms_over_nas_status)) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [sms_over_nas_status]");
            goto end;
        }
    }

    if (access_and_mobility_data->sms_over_nas_status_ts) {
        if (cJSON_AddStringToObject(item, "smsOverNasStatusTs", access_and_mobility_data->sms_over_nas_status_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [sms_over_nas_status_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->roaming_status) {
        if (cJSON_AddBoolToObject(item, "roamingStatus", access_and_mobility_data->roaming_status) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [roaming_status]");
            goto end;
        }
    }

    if (access_and_mobility_data->roaming_status_ts) {
        if (cJSON_AddStringToObject(item, "roamingStatusTs", access_and_mobility_data->roaming_status_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [roaming_status_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->current_plmn) {
        cJSON *current_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(access_and_mobility_data->current_plmn);
        if (current_plmn_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [current_plmn]");
            goto end;
        }
        cJSON_AddItemToObject(item, "currentPlmn", current_plmn_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [current_plmn]");
            goto end;
        }
    }

    if (access_and_mobility_data->current_plmn_ts) {
        if (cJSON_AddStringToObject(item, "currentPlmnTs", access_and_mobility_data->current_plmn_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [current_plmn_ts]");
            goto end;
        }
    }

    if (access_and_mobility_data->rat_type) {
        cJSON *rat_type = cJSON_AddArrayToObject(item, "ratType");
        if (rat_type == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [rat_type]");
            goto end;
        }
        OpenAPI_lnode_t *rat_type_node;
        OpenAPI_list_for_each(access_and_mobility_data->rat_type, rat_type_node) {
            if (cJSON_AddStringToObject(rat_type, "", OpenAPI_rat_type_ToString((OpenAPI_rat_type_e)rat_type_node->data)) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [rat_type]");
                goto end;
            }
        }
    }

    if (access_and_mobility_data->rat_types_ts) {
        if (cJSON_AddStringToObject(item, "ratTypesTs", access_and_mobility_data->rat_types_ts) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [rat_types_ts]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_access_and_mobility_data_t *OpenAPI_access_and_mobility_data_parseFromJSON(cJSON *access_and_mobility_dataJSON)
{
    OpenAPI_access_and_mobility_data_t *access_and_mobility_data_local_var = NULL;
    cJSON *location = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "location");

    OpenAPI_user_location_t *location_local_nonprim = NULL;
    if (location) {
        location_local_nonprim = OpenAPI_user_location_parseFromJSON(location);
    }

    cJSON *location_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "locationTs");

    if (location_ts) {
        if (!cJSON_IsString(location_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [location_ts]");
            goto end;
        }
    }

    cJSON *time_zone = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "timeZone");

    if (time_zone) {
        if (!cJSON_IsString(time_zone)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [time_zone]");
            goto end;
        }
    }

    cJSON *time_zone_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "timeZoneTs");

    if (time_zone_ts) {
        if (!cJSON_IsString(time_zone_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [time_zone_ts]");
            goto end;
        }
    }

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "accessType");

    OpenAPI_access_type_e access_typeVariable;
    if (access_type) {
        if (!cJSON_IsString(access_type)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [access_type]");
            goto end;
        }
        access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    cJSON *reg_states = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "regStates");

    OpenAPI_list_t *reg_statesList;
    if (reg_states) {
        cJSON *reg_states_local_nonprimitive;
        if (!cJSON_IsArray(reg_states)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reg_states]");
            goto end;
        }

        reg_statesList = OpenAPI_list_create();

        cJSON_ArrayForEach(reg_states_local_nonprimitive, reg_states ) {
            if (!cJSON_IsObject(reg_states_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reg_states]");
                goto end;
            }
            OpenAPI_rm_info_t *reg_statesItem = OpenAPI_rm_info_parseFromJSON(reg_states_local_nonprimitive);

            OpenAPI_list_add(reg_statesList, reg_statesItem);
        }
    }

    cJSON *reg_states_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "regStatesTs");

    if (reg_states_ts) {
        if (!cJSON_IsString(reg_states_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reg_states_ts]");
            goto end;
        }
    }

    cJSON *conn_states = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "connStates");

    OpenAPI_list_t *conn_statesList;
    if (conn_states) {
        cJSON *conn_states_local_nonprimitive;
        if (!cJSON_IsArray(conn_states)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [conn_states]");
            goto end;
        }

        conn_statesList = OpenAPI_list_create();

        cJSON_ArrayForEach(conn_states_local_nonprimitive, conn_states ) {
            if (!cJSON_IsObject(conn_states_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [conn_states]");
                goto end;
            }
            OpenAPI_cm_info_t *conn_statesItem = OpenAPI_cm_info_parseFromJSON(conn_states_local_nonprimitive);

            OpenAPI_list_add(conn_statesList, conn_statesItem);
        }
    }

    cJSON *conn_states_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "connStatesTs");

    if (conn_states_ts) {
        if (!cJSON_IsString(conn_states_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [conn_states_ts]");
            goto end;
        }
    }

    cJSON *reachability_status = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "reachabilityStatus");

    OpenAPI_ue_reachability_t *reachability_status_local_nonprim = NULL;
    if (reachability_status) {
        reachability_status_local_nonprim = OpenAPI_ue_reachability_parseFromJSON(reachability_status);
    }

    cJSON *reachability_status_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "reachabilityStatusTs");

    if (reachability_status_ts) {
        if (!cJSON_IsString(reachability_status_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reachability_status_ts]");
            goto end;
        }
    }

    cJSON *sms_over_nas_status = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "smsOverNasStatus");

    OpenAPI_sms_support_e sms_over_nas_statusVariable;
    if (sms_over_nas_status) {
        if (!cJSON_IsString(sms_over_nas_status)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [sms_over_nas_status]");
            goto end;
        }
        sms_over_nas_statusVariable = OpenAPI_sms_support_FromString(sms_over_nas_status->valuestring);
    }

    cJSON *sms_over_nas_status_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "smsOverNasStatusTs");

    if (sms_over_nas_status_ts) {
        if (!cJSON_IsString(sms_over_nas_status_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [sms_over_nas_status_ts]");
            goto end;
        }
    }

    cJSON *roaming_status = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "roamingStatus");

    if (roaming_status) {
        if (!cJSON_IsBool(roaming_status)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [roaming_status]");
            goto end;
        }
    }

    cJSON *roaming_status_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "roamingStatusTs");

    if (roaming_status_ts) {
        if (!cJSON_IsString(roaming_status_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [roaming_status_ts]");
            goto end;
        }
    }

    cJSON *current_plmn = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "currentPlmn");

    OpenAPI_plmn_id_t *current_plmn_local_nonprim = NULL;
    if (current_plmn) {
        current_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(current_plmn);
    }

    cJSON *current_plmn_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "currentPlmnTs");

    if (current_plmn_ts) {
        if (!cJSON_IsString(current_plmn_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [current_plmn_ts]");
            goto end;
        }
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "ratType");

    OpenAPI_list_t *rat_typeList;
    if (rat_type) {
        cJSON *rat_type_local_nonprimitive;
        if (!cJSON_IsArray(rat_type)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [rat_type]");
            goto end;
        }

        rat_typeList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_type_local_nonprimitive, rat_type ) {
            if (!cJSON_IsString(rat_type_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [rat_type]");
                goto end;
            }

            OpenAPI_list_add(rat_typeList, (void *)OpenAPI_rat_type_FromString(rat_type_local_nonprimitive->valuestring));
        }
    }

    cJSON *rat_types_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "ratTypesTs");

    if (rat_types_ts) {
        if (!cJSON_IsString(rat_types_ts)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [rat_types_ts]");
            goto end;
        }
    }

    access_and_mobility_data_local_var = OpenAPI_access_and_mobility_data_create (
        location ? location_local_nonprim : NULL,
        location_ts ? ogs_strdup(location_ts->valuestring) : NULL,
        time_zone ? ogs_strdup(time_zone->valuestring) : NULL,
        time_zone_ts ? ogs_strdup(time_zone_ts->valuestring) : NULL,
        access_type ? access_typeVariable : 0,
        reg_states ? reg_statesList : NULL,
        reg_states_ts ? ogs_strdup(reg_states_ts->valuestring) : NULL,
        conn_states ? conn_statesList : NULL,
        conn_states_ts ? ogs_strdup(conn_states_ts->valuestring) : NULL,
        reachability_status ? reachability_status_local_nonprim : NULL,
        reachability_status_ts ? ogs_strdup(reachability_status_ts->valuestring) : NULL,
        sms_over_nas_status ? sms_over_nas_statusVariable : 0,
        sms_over_nas_status_ts ? ogs_strdup(sms_over_nas_status_ts->valuestring) : NULL,
        roaming_status ? roaming_status->valueint : 0,
        roaming_status_ts ? ogs_strdup(roaming_status_ts->valuestring) : NULL,
        current_plmn ? current_plmn_local_nonprim : NULL,
        current_plmn_ts ? ogs_strdup(current_plmn_ts->valuestring) : NULL,
        rat_type ? rat_typeList : NULL,
        rat_types_ts ? ogs_strdup(rat_types_ts->valuestring) : NULL
        );

    return access_and_mobility_data_local_var;
end:
    return NULL;
}

OpenAPI_access_and_mobility_data_t *OpenAPI_access_and_mobility_data_copy(OpenAPI_access_and_mobility_data_t *dst, OpenAPI_access_and_mobility_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_and_mobility_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed");
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

    OpenAPI_access_and_mobility_data_free(dst);
    dst = OpenAPI_access_and_mobility_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

