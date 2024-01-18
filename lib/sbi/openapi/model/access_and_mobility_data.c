
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
    bool is_roaming_status,
    int roaming_status,
    char *roaming_status_ts,
    OpenAPI_plmn_id_1_t *current_plmn,
    char *current_plmn_ts,
    OpenAPI_list_t *rat_type,
    char *rat_types_ts,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_access_and_mobility_data_t *access_and_mobility_data_local_var = ogs_malloc(sizeof(OpenAPI_access_and_mobility_data_t));
    ogs_assert(access_and_mobility_data_local_var);

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
    access_and_mobility_data_local_var->is_roaming_status = is_roaming_status;
    access_and_mobility_data_local_var->roaming_status = roaming_status;
    access_and_mobility_data_local_var->roaming_status_ts = roaming_status_ts;
    access_and_mobility_data_local_var->current_plmn = current_plmn;
    access_and_mobility_data_local_var->current_plmn_ts = current_plmn_ts;
    access_and_mobility_data_local_var->rat_type = rat_type;
    access_and_mobility_data_local_var->rat_types_ts = rat_types_ts;
    access_and_mobility_data_local_var->supp_feat = supp_feat;
    access_and_mobility_data_local_var->reset_ids = reset_ids;

    return access_and_mobility_data_local_var;
}

void OpenAPI_access_and_mobility_data_free(OpenAPI_access_and_mobility_data_t *access_and_mobility_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_and_mobility_data) {
        return;
    }
    if (access_and_mobility_data->location) {
        OpenAPI_user_location_free(access_and_mobility_data->location);
        access_and_mobility_data->location = NULL;
    }
    if (access_and_mobility_data->location_ts) {
        ogs_free(access_and_mobility_data->location_ts);
        access_and_mobility_data->location_ts = NULL;
    }
    if (access_and_mobility_data->time_zone) {
        ogs_free(access_and_mobility_data->time_zone);
        access_and_mobility_data->time_zone = NULL;
    }
    if (access_and_mobility_data->time_zone_ts) {
        ogs_free(access_and_mobility_data->time_zone_ts);
        access_and_mobility_data->time_zone_ts = NULL;
    }
    if (access_and_mobility_data->reg_states) {
        OpenAPI_list_for_each(access_and_mobility_data->reg_states, node) {
            OpenAPI_rm_info_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_data->reg_states);
        access_and_mobility_data->reg_states = NULL;
    }
    if (access_and_mobility_data->reg_states_ts) {
        ogs_free(access_and_mobility_data->reg_states_ts);
        access_and_mobility_data->reg_states_ts = NULL;
    }
    if (access_and_mobility_data->conn_states) {
        OpenAPI_list_for_each(access_and_mobility_data->conn_states, node) {
            OpenAPI_cm_info_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_data->conn_states);
        access_and_mobility_data->conn_states = NULL;
    }
    if (access_and_mobility_data->conn_states_ts) {
        ogs_free(access_and_mobility_data->conn_states_ts);
        access_and_mobility_data->conn_states_ts = NULL;
    }
    if (access_and_mobility_data->reachability_status) {
        OpenAPI_ue_reachability_free(access_and_mobility_data->reachability_status);
        access_and_mobility_data->reachability_status = NULL;
    }
    if (access_and_mobility_data->reachability_status_ts) {
        ogs_free(access_and_mobility_data->reachability_status_ts);
        access_and_mobility_data->reachability_status_ts = NULL;
    }
    if (access_and_mobility_data->sms_over_nas_status_ts) {
        ogs_free(access_and_mobility_data->sms_over_nas_status_ts);
        access_and_mobility_data->sms_over_nas_status_ts = NULL;
    }
    if (access_and_mobility_data->roaming_status_ts) {
        ogs_free(access_and_mobility_data->roaming_status_ts);
        access_and_mobility_data->roaming_status_ts = NULL;
    }
    if (access_and_mobility_data->current_plmn) {
        OpenAPI_plmn_id_1_free(access_and_mobility_data->current_plmn);
        access_and_mobility_data->current_plmn = NULL;
    }
    if (access_and_mobility_data->current_plmn_ts) {
        ogs_free(access_and_mobility_data->current_plmn_ts);
        access_and_mobility_data->current_plmn_ts = NULL;
    }
    if (access_and_mobility_data->rat_type) {
        OpenAPI_list_free(access_and_mobility_data->rat_type);
        access_and_mobility_data->rat_type = NULL;
    }
    if (access_and_mobility_data->rat_types_ts) {
        ogs_free(access_and_mobility_data->rat_types_ts);
        access_and_mobility_data->rat_types_ts = NULL;
    }
    if (access_and_mobility_data->supp_feat) {
        ogs_free(access_and_mobility_data->supp_feat);
        access_and_mobility_data->supp_feat = NULL;
    }
    if (access_and_mobility_data->reset_ids) {
        OpenAPI_list_for_each(access_and_mobility_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_data->reset_ids);
        access_and_mobility_data->reset_ids = NULL;
    }
    ogs_free(access_and_mobility_data);
}

cJSON *OpenAPI_access_and_mobility_data_convertToJSON(OpenAPI_access_and_mobility_data_t *access_and_mobility_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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

    if (access_and_mobility_data->access_type != OpenAPI_access_type_NULL) {
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
    OpenAPI_list_for_each(access_and_mobility_data->reg_states, node) {
        cJSON *itemLocal = OpenAPI_rm_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reg_states]");
            goto end;
        }
        cJSON_AddItemToArray(reg_statesList, itemLocal);
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
    OpenAPI_list_for_each(access_and_mobility_data->conn_states, node) {
        cJSON *itemLocal = OpenAPI_cm_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [conn_states]");
            goto end;
        }
        cJSON_AddItemToArray(conn_statesList, itemLocal);
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

    if (access_and_mobility_data->sms_over_nas_status != OpenAPI_sms_support_NULL) {
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

    if (access_and_mobility_data->is_roaming_status) {
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
    cJSON *current_plmn_local_JSON = OpenAPI_plmn_id_1_convertToJSON(access_and_mobility_data->current_plmn);
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

    if (access_and_mobility_data->rat_type != OpenAPI_rat_type_NULL) {
    cJSON *rat_typeList = cJSON_AddArrayToObject(item, "ratType");
    if (rat_typeList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_data->rat_type, node) {
        if (cJSON_AddStringToObject(rat_typeList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
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

    if (access_and_mobility_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", access_and_mobility_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (access_and_mobility_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_access_and_mobility_data_t *OpenAPI_access_and_mobility_data_parseFromJSON(cJSON *access_and_mobility_dataJSON)
{
    OpenAPI_access_and_mobility_data_t *access_and_mobility_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *location = NULL;
    OpenAPI_user_location_t *location_local_nonprim = NULL;
    cJSON *location_ts = NULL;
    cJSON *time_zone = NULL;
    cJSON *time_zone_ts = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *reg_states = NULL;
    OpenAPI_list_t *reg_statesList = NULL;
    cJSON *reg_states_ts = NULL;
    cJSON *conn_states = NULL;
    OpenAPI_list_t *conn_statesList = NULL;
    cJSON *conn_states_ts = NULL;
    cJSON *reachability_status = NULL;
    OpenAPI_ue_reachability_t *reachability_status_local_nonprim = NULL;
    cJSON *reachability_status_ts = NULL;
    cJSON *sms_over_nas_status = NULL;
    OpenAPI_sms_support_e sms_over_nas_statusVariable = 0;
    cJSON *sms_over_nas_status_ts = NULL;
    cJSON *roaming_status = NULL;
    cJSON *roaming_status_ts = NULL;
    cJSON *current_plmn = NULL;
    OpenAPI_plmn_id_1_t *current_plmn_local_nonprim = NULL;
    cJSON *current_plmn_ts = NULL;
    cJSON *rat_type = NULL;
    OpenAPI_list_t *rat_typeList = NULL;
    cJSON *rat_types_ts = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    location = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "location");
    if (location) {
    location_local_nonprim = OpenAPI_user_location_parseFromJSON(location);
    if (!location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [location]");
        goto end;
    }
    }

    location_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "locationTs");
    if (location_ts) {
    if (!cJSON_IsString(location_ts) && !cJSON_IsNull(location_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [location_ts]");
        goto end;
    }
    }

    time_zone = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "timeZone");
    if (time_zone) {
    if (!cJSON_IsString(time_zone) && !cJSON_IsNull(time_zone)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [time_zone]");
        goto end;
    }
    }

    time_zone_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "timeZoneTs");
    if (time_zone_ts) {
    if (!cJSON_IsString(time_zone_ts) && !cJSON_IsNull(time_zone_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [time_zone_ts]");
        goto end;
    }
    }

    access_type = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    reg_states = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "regStates");
    if (reg_states) {
        cJSON *reg_states_local = NULL;
        if (!cJSON_IsArray(reg_states)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reg_states]");
            goto end;
        }

        reg_statesList = OpenAPI_list_create();

        cJSON_ArrayForEach(reg_states_local, reg_states) {
            if (!cJSON_IsObject(reg_states_local)) {
                ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reg_states]");
                goto end;
            }
            OpenAPI_rm_info_t *reg_statesItem = OpenAPI_rm_info_parseFromJSON(reg_states_local);
            if (!reg_statesItem) {
                ogs_error("No reg_statesItem");
                goto end;
            }
            OpenAPI_list_add(reg_statesList, reg_statesItem);
        }
    }

    reg_states_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "regStatesTs");
    if (reg_states_ts) {
    if (!cJSON_IsString(reg_states_ts) && !cJSON_IsNull(reg_states_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reg_states_ts]");
        goto end;
    }
    }

    conn_states = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "connStates");
    if (conn_states) {
        cJSON *conn_states_local = NULL;
        if (!cJSON_IsArray(conn_states)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [conn_states]");
            goto end;
        }

        conn_statesList = OpenAPI_list_create();

        cJSON_ArrayForEach(conn_states_local, conn_states) {
            if (!cJSON_IsObject(conn_states_local)) {
                ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [conn_states]");
                goto end;
            }
            OpenAPI_cm_info_t *conn_statesItem = OpenAPI_cm_info_parseFromJSON(conn_states_local);
            if (!conn_statesItem) {
                ogs_error("No conn_statesItem");
                goto end;
            }
            OpenAPI_list_add(conn_statesList, conn_statesItem);
        }
    }

    conn_states_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "connStatesTs");
    if (conn_states_ts) {
    if (!cJSON_IsString(conn_states_ts) && !cJSON_IsNull(conn_states_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [conn_states_ts]");
        goto end;
    }
    }

    reachability_status = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "reachabilityStatus");
    if (reachability_status) {
    reachability_status_local_nonprim = OpenAPI_ue_reachability_parseFromJSON(reachability_status);
    if (!reachability_status_local_nonprim) {
        ogs_error("OpenAPI_ue_reachability_parseFromJSON failed [reachability_status]");
        goto end;
    }
    }

    reachability_status_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "reachabilityStatusTs");
    if (reachability_status_ts) {
    if (!cJSON_IsString(reachability_status_ts) && !cJSON_IsNull(reachability_status_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reachability_status_ts]");
        goto end;
    }
    }

    sms_over_nas_status = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "smsOverNasStatus");
    if (sms_over_nas_status) {
    if (!cJSON_IsString(sms_over_nas_status)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [sms_over_nas_status]");
        goto end;
    }
    sms_over_nas_statusVariable = OpenAPI_sms_support_FromString(sms_over_nas_status->valuestring);
    }

    sms_over_nas_status_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "smsOverNasStatusTs");
    if (sms_over_nas_status_ts) {
    if (!cJSON_IsString(sms_over_nas_status_ts) && !cJSON_IsNull(sms_over_nas_status_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [sms_over_nas_status_ts]");
        goto end;
    }
    }

    roaming_status = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "roamingStatus");
    if (roaming_status) {
    if (!cJSON_IsBool(roaming_status)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [roaming_status]");
        goto end;
    }
    }

    roaming_status_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "roamingStatusTs");
    if (roaming_status_ts) {
    if (!cJSON_IsString(roaming_status_ts) && !cJSON_IsNull(roaming_status_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [roaming_status_ts]");
        goto end;
    }
    }

    current_plmn = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "currentPlmn");
    if (current_plmn) {
    current_plmn_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(current_plmn);
    if (!current_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON failed [current_plmn]");
        goto end;
    }
    }

    current_plmn_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "currentPlmnTs");
    if (current_plmn_ts) {
    if (!cJSON_IsString(current_plmn_ts) && !cJSON_IsNull(current_plmn_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [current_plmn_ts]");
        goto end;
    }
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "ratType");
    if (rat_type) {
        cJSON *rat_type_local = NULL;
        if (!cJSON_IsArray(rat_type)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [rat_type]");
            goto end;
        }

        rat_typeList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_type_local, rat_type) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_type_local)) {
                ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [rat_type]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_type_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_type\" is not supported. Ignoring it ...",
                         rat_type_local->valuestring);
            } else {
                OpenAPI_list_add(rat_typeList, (void *)localEnum);
            }
        }
        if (rat_typeList->count == 0) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed: Expected rat_typeList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    rat_types_ts = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "ratTypesTs");
    if (rat_types_ts) {
    if (!cJSON_IsString(rat_types_ts) && !cJSON_IsNull(rat_types_ts)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [rat_types_ts]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(access_and_mobility_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_access_and_mobility_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    access_and_mobility_data_local_var = OpenAPI_access_and_mobility_data_create (
        location ? location_local_nonprim : NULL,
        location_ts && !cJSON_IsNull(location_ts) ? ogs_strdup(location_ts->valuestring) : NULL,
        time_zone && !cJSON_IsNull(time_zone) ? ogs_strdup(time_zone->valuestring) : NULL,
        time_zone_ts && !cJSON_IsNull(time_zone_ts) ? ogs_strdup(time_zone_ts->valuestring) : NULL,
        access_type ? access_typeVariable : 0,
        reg_states ? reg_statesList : NULL,
        reg_states_ts && !cJSON_IsNull(reg_states_ts) ? ogs_strdup(reg_states_ts->valuestring) : NULL,
        conn_states ? conn_statesList : NULL,
        conn_states_ts && !cJSON_IsNull(conn_states_ts) ? ogs_strdup(conn_states_ts->valuestring) : NULL,
        reachability_status ? reachability_status_local_nonprim : NULL,
        reachability_status_ts && !cJSON_IsNull(reachability_status_ts) ? ogs_strdup(reachability_status_ts->valuestring) : NULL,
        sms_over_nas_status ? sms_over_nas_statusVariable : 0,
        sms_over_nas_status_ts && !cJSON_IsNull(sms_over_nas_status_ts) ? ogs_strdup(sms_over_nas_status_ts->valuestring) : NULL,
        roaming_status ? true : false,
        roaming_status ? roaming_status->valueint : 0,
        roaming_status_ts && !cJSON_IsNull(roaming_status_ts) ? ogs_strdup(roaming_status_ts->valuestring) : NULL,
        current_plmn ? current_plmn_local_nonprim : NULL,
        current_plmn_ts && !cJSON_IsNull(current_plmn_ts) ? ogs_strdup(current_plmn_ts->valuestring) : NULL,
        rat_type ? rat_typeList : NULL,
        rat_types_ts && !cJSON_IsNull(rat_types_ts) ? ogs_strdup(rat_types_ts->valuestring) : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return access_and_mobility_data_local_var;
end:
    if (location_local_nonprim) {
        OpenAPI_user_location_free(location_local_nonprim);
        location_local_nonprim = NULL;
    }
    if (reg_statesList) {
        OpenAPI_list_for_each(reg_statesList, node) {
            OpenAPI_rm_info_free(node->data);
        }
        OpenAPI_list_free(reg_statesList);
        reg_statesList = NULL;
    }
    if (conn_statesList) {
        OpenAPI_list_for_each(conn_statesList, node) {
            OpenAPI_cm_info_free(node->data);
        }
        OpenAPI_list_free(conn_statesList);
        conn_statesList = NULL;
    }
    if (reachability_status_local_nonprim) {
        OpenAPI_ue_reachability_free(reachability_status_local_nonprim);
        reachability_status_local_nonprim = NULL;
    }
    if (current_plmn_local_nonprim) {
        OpenAPI_plmn_id_1_free(current_plmn_local_nonprim);
        current_plmn_local_nonprim = NULL;
    }
    if (rat_typeList) {
        OpenAPI_list_free(rat_typeList);
        rat_typeList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

