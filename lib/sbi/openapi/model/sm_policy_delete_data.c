
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_delete_data.h"

OpenAPI_sm_policy_delete_data_t *OpenAPI_sm_policy_delete_data_create(
    OpenAPI_user_location_t *user_location_info,
    char *ue_time_zone,
    OpenAPI_plmn_id_nid_t *serving_network,
    char *user_location_info_time,
    OpenAPI_list_t *ran_nas_rel_causes,
    OpenAPI_list_t *accu_usage_reports,
    OpenAPI_pdu_session_rel_cause_e pdu_sess_rel_cause,
    OpenAPI_list_t *qos_mon_reports
)
{
    OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_delete_data_t));
    ogs_assert(sm_policy_delete_data_local_var);

    sm_policy_delete_data_local_var->user_location_info = user_location_info;
    sm_policy_delete_data_local_var->ue_time_zone = ue_time_zone;
    sm_policy_delete_data_local_var->serving_network = serving_network;
    sm_policy_delete_data_local_var->user_location_info_time = user_location_info_time;
    sm_policy_delete_data_local_var->ran_nas_rel_causes = ran_nas_rel_causes;
    sm_policy_delete_data_local_var->accu_usage_reports = accu_usage_reports;
    sm_policy_delete_data_local_var->pdu_sess_rel_cause = pdu_sess_rel_cause;
    sm_policy_delete_data_local_var->qos_mon_reports = qos_mon_reports;

    return sm_policy_delete_data_local_var;
}

void OpenAPI_sm_policy_delete_data_free(OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data)
{
    if (NULL == sm_policy_delete_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_user_location_free(sm_policy_delete_data->user_location_info);
    ogs_free(sm_policy_delete_data->ue_time_zone);
    OpenAPI_plmn_id_nid_free(sm_policy_delete_data->serving_network);
    ogs_free(sm_policy_delete_data->user_location_info_time);
    OpenAPI_list_for_each(sm_policy_delete_data->ran_nas_rel_causes, node) {
        OpenAPI_ran_nas_rel_cause_free(node->data);
    }
    OpenAPI_list_free(sm_policy_delete_data->ran_nas_rel_causes);
    OpenAPI_list_for_each(sm_policy_delete_data->accu_usage_reports, node) {
        OpenAPI_accu_usage_report_free(node->data);
    }
    OpenAPI_list_free(sm_policy_delete_data->accu_usage_reports);
    OpenAPI_list_for_each(sm_policy_delete_data->qos_mon_reports, node) {
        OpenAPI_qos_monitoring_report_free(node->data);
    }
    OpenAPI_list_free(sm_policy_delete_data->qos_mon_reports);
    ogs_free(sm_policy_delete_data);
}

cJSON *OpenAPI_sm_policy_delete_data_convertToJSON(OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data)
{
    cJSON *item = NULL;

    if (sm_policy_delete_data == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [SmPolicyDeleteData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_policy_delete_data->user_location_info) {
    cJSON *user_location_info_local_JSON = OpenAPI_user_location_convertToJSON(sm_policy_delete_data->user_location_info);
    if (user_location_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [user_location_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInfo", user_location_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [user_location_info]");
        goto end;
    }
    }

    if (sm_policy_delete_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sm_policy_delete_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (sm_policy_delete_data->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sm_policy_delete_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

    if (sm_policy_delete_data->user_location_info_time) {
    if (cJSON_AddStringToObject(item, "userLocationInfoTime", sm_policy_delete_data->user_location_info_time) == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [user_location_info_time]");
        goto end;
    }
    }

    if (sm_policy_delete_data->ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causesList = cJSON_AddArrayToObject(item, "ranNasRelCauses");
    if (ran_nas_rel_causesList == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [ran_nas_rel_causes]");
        goto end;
    }

    OpenAPI_lnode_t *ran_nas_rel_causes_node;
    if (sm_policy_delete_data->ran_nas_rel_causes) {
        OpenAPI_list_for_each(sm_policy_delete_data->ran_nas_rel_causes, ran_nas_rel_causes_node) {
            cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(ran_nas_rel_causes_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [ran_nas_rel_causes]");
                goto end;
            }
            cJSON_AddItemToArray(ran_nas_rel_causesList, itemLocal);
        }
    }
    }

    if (sm_policy_delete_data->accu_usage_reports) {
    cJSON *accu_usage_reportsList = cJSON_AddArrayToObject(item, "accuUsageReports");
    if (accu_usage_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [accu_usage_reports]");
        goto end;
    }

    OpenAPI_lnode_t *accu_usage_reports_node;
    if (sm_policy_delete_data->accu_usage_reports) {
        OpenAPI_list_for_each(sm_policy_delete_data->accu_usage_reports, accu_usage_reports_node) {
            cJSON *itemLocal = OpenAPI_accu_usage_report_convertToJSON(accu_usage_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [accu_usage_reports]");
                goto end;
            }
            cJSON_AddItemToArray(accu_usage_reportsList, itemLocal);
        }
    }
    }

    if (sm_policy_delete_data->pdu_sess_rel_cause) {
    if (cJSON_AddStringToObject(item, "pduSessRelCause", OpenAPI_pdu_session_rel_cause_ToString(sm_policy_delete_data->pdu_sess_rel_cause)) == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [pdu_sess_rel_cause]");
        goto end;
    }
    }

    if (sm_policy_delete_data->qos_mon_reports) {
    cJSON *qos_mon_reportsList = cJSON_AddArrayToObject(item, "qosMonReports");
    if (qos_mon_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [qos_mon_reports]");
        goto end;
    }

    OpenAPI_lnode_t *qos_mon_reports_node;
    if (sm_policy_delete_data->qos_mon_reports) {
        OpenAPI_list_for_each(sm_policy_delete_data->qos_mon_reports, qos_mon_reports_node) {
            cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(qos_mon_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [qos_mon_reports]");
                goto end;
            }
            cJSON_AddItemToArray(qos_mon_reportsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_delete_data_t *OpenAPI_sm_policy_delete_data_parseFromJSON(cJSON *sm_policy_delete_dataJSON)
{
    OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data_local_var = NULL;
    cJSON *user_location_info = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "userLocationInfo");

    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    if (user_location_info) {
    user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone)) {
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "servingNetwork");

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    }

    cJSON *user_location_info_time = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "userLocationInfoTime");

    if (user_location_info_time) {
    if (!cJSON_IsString(user_location_info_time)) {
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [user_location_info_time]");
        goto end;
    }
    }

    cJSON *ran_nas_rel_causes = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "ranNasRelCauses");

    OpenAPI_list_t *ran_nas_rel_causesList;
    if (ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causes_local_nonprimitive;
    if (!cJSON_IsArray(ran_nas_rel_causes)){
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [ran_nas_rel_causes]");
        goto end;
    }

    ran_nas_rel_causesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ran_nas_rel_causes_local_nonprimitive, ran_nas_rel_causes ) {
        if (!cJSON_IsObject(ran_nas_rel_causes_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [ran_nas_rel_causes]");
            goto end;
        }
        OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_causesItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(ran_nas_rel_causes_local_nonprimitive);

        if (!ran_nas_rel_causesItem) {
            ogs_error("No ran_nas_rel_causesItem");
            OpenAPI_list_free(ran_nas_rel_causesList);
            goto end;
        }

        OpenAPI_list_add(ran_nas_rel_causesList, ran_nas_rel_causesItem);
    }
    }

    cJSON *accu_usage_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "accuUsageReports");

    OpenAPI_list_t *accu_usage_reportsList;
    if (accu_usage_reports) {
    cJSON *accu_usage_reports_local_nonprimitive;
    if (!cJSON_IsArray(accu_usage_reports)){
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [accu_usage_reports]");
        goto end;
    }

    accu_usage_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(accu_usage_reports_local_nonprimitive, accu_usage_reports ) {
        if (!cJSON_IsObject(accu_usage_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [accu_usage_reports]");
            goto end;
        }
        OpenAPI_accu_usage_report_t *accu_usage_reportsItem = OpenAPI_accu_usage_report_parseFromJSON(accu_usage_reports_local_nonprimitive);

        if (!accu_usage_reportsItem) {
            ogs_error("No accu_usage_reportsItem");
            OpenAPI_list_free(accu_usage_reportsList);
            goto end;
        }

        OpenAPI_list_add(accu_usage_reportsList, accu_usage_reportsItem);
    }
    }

    cJSON *pdu_sess_rel_cause = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "pduSessRelCause");

    OpenAPI_pdu_session_rel_cause_e pdu_sess_rel_causeVariable;
    if (pdu_sess_rel_cause) {
    if (!cJSON_IsString(pdu_sess_rel_cause)) {
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [pdu_sess_rel_cause]");
        goto end;
    }
    pdu_sess_rel_causeVariable = OpenAPI_pdu_session_rel_cause_FromString(pdu_sess_rel_cause->valuestring);
    }

    cJSON *qos_mon_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "qosMonReports");

    OpenAPI_list_t *qos_mon_reportsList;
    if (qos_mon_reports) {
    cJSON *qos_mon_reports_local_nonprimitive;
    if (!cJSON_IsArray(qos_mon_reports)){
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [qos_mon_reports]");
        goto end;
    }

    qos_mon_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(qos_mon_reports_local_nonprimitive, qos_mon_reports ) {
        if (!cJSON_IsObject(qos_mon_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [qos_mon_reports]");
            goto end;
        }
        OpenAPI_qos_monitoring_report_t *qos_mon_reportsItem = OpenAPI_qos_monitoring_report_parseFromJSON(qos_mon_reports_local_nonprimitive);

        if (!qos_mon_reportsItem) {
            ogs_error("No qos_mon_reportsItem");
            OpenAPI_list_free(qos_mon_reportsList);
            goto end;
        }

        OpenAPI_list_add(qos_mon_reportsList, qos_mon_reportsItem);
    }
    }

    sm_policy_delete_data_local_var = OpenAPI_sm_policy_delete_data_create (
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        user_location_info_time ? ogs_strdup(user_location_info_time->valuestring) : NULL,
        ran_nas_rel_causes ? ran_nas_rel_causesList : NULL,
        accu_usage_reports ? accu_usage_reportsList : NULL,
        pdu_sess_rel_cause ? pdu_sess_rel_causeVariable : 0,
        qos_mon_reports ? qos_mon_reportsList : NULL
    );

    return sm_policy_delete_data_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_delete_data_t *OpenAPI_sm_policy_delete_data_copy(OpenAPI_sm_policy_delete_data_t *dst, OpenAPI_sm_policy_delete_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_delete_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed");
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

    OpenAPI_sm_policy_delete_data_free(dst);
    dst = OpenAPI_sm_policy_delete_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

