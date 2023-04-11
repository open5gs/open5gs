
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
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_policy_delete_data) {
        return;
    }
    if (sm_policy_delete_data->user_location_info) {
        OpenAPI_user_location_free(sm_policy_delete_data->user_location_info);
        sm_policy_delete_data->user_location_info = NULL;
    }
    if (sm_policy_delete_data->ue_time_zone) {
        ogs_free(sm_policy_delete_data->ue_time_zone);
        sm_policy_delete_data->ue_time_zone = NULL;
    }
    if (sm_policy_delete_data->serving_network) {
        OpenAPI_plmn_id_nid_free(sm_policy_delete_data->serving_network);
        sm_policy_delete_data->serving_network = NULL;
    }
    if (sm_policy_delete_data->user_location_info_time) {
        ogs_free(sm_policy_delete_data->user_location_info_time);
        sm_policy_delete_data->user_location_info_time = NULL;
    }
    if (sm_policy_delete_data->ran_nas_rel_causes) {
        OpenAPI_list_for_each(sm_policy_delete_data->ran_nas_rel_causes, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(sm_policy_delete_data->ran_nas_rel_causes);
        sm_policy_delete_data->ran_nas_rel_causes = NULL;
    }
    if (sm_policy_delete_data->accu_usage_reports) {
        OpenAPI_list_for_each(sm_policy_delete_data->accu_usage_reports, node) {
            OpenAPI_accu_usage_report_free(node->data);
        }
        OpenAPI_list_free(sm_policy_delete_data->accu_usage_reports);
        sm_policy_delete_data->accu_usage_reports = NULL;
    }
    if (sm_policy_delete_data->qos_mon_reports) {
        OpenAPI_list_for_each(sm_policy_delete_data->qos_mon_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(sm_policy_delete_data->qos_mon_reports);
        sm_policy_delete_data->qos_mon_reports = NULL;
    }
    ogs_free(sm_policy_delete_data);
}

cJSON *OpenAPI_sm_policy_delete_data_convertToJSON(OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(sm_policy_delete_data->ran_nas_rel_causes, node) {
        cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [ran_nas_rel_causes]");
            goto end;
        }
        cJSON_AddItemToArray(ran_nas_rel_causesList, itemLocal);
    }
    }

    if (sm_policy_delete_data->accu_usage_reports) {
    cJSON *accu_usage_reportsList = cJSON_AddArrayToObject(item, "accuUsageReports");
    if (accu_usage_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [accu_usage_reports]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_delete_data->accu_usage_reports, node) {
        cJSON *itemLocal = OpenAPI_accu_usage_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [accu_usage_reports]");
            goto end;
        }
        cJSON_AddItemToArray(accu_usage_reportsList, itemLocal);
    }
    }

    if (sm_policy_delete_data->pdu_sess_rel_cause != OpenAPI_pdu_session_rel_cause_NULL) {
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
    OpenAPI_list_for_each(sm_policy_delete_data->qos_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_delete_data_convertToJSON() failed [qos_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qos_mon_reportsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_delete_data_t *OpenAPI_sm_policy_delete_data_parseFromJSON(cJSON *sm_policy_delete_dataJSON)
{
    OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *user_location_info = NULL;
    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    cJSON *user_location_info_time = NULL;
    cJSON *ran_nas_rel_causes = NULL;
    OpenAPI_list_t *ran_nas_rel_causesList = NULL;
    cJSON *accu_usage_reports = NULL;
    OpenAPI_list_t *accu_usage_reportsList = NULL;
    cJSON *pdu_sess_rel_cause = NULL;
    OpenAPI_pdu_session_rel_cause_e pdu_sess_rel_causeVariable = 0;
    cJSON *qos_mon_reports = NULL;
    OpenAPI_list_t *qos_mon_reportsList = NULL;
    user_location_info = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "userLocationInfo");
    if (user_location_info) {
    user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    if (!user_location_info_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [user_location_info]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    user_location_info_time = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "userLocationInfoTime");
    if (user_location_info_time) {
    if (!cJSON_IsString(user_location_info_time) && !cJSON_IsNull(user_location_info_time)) {
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [user_location_info_time]");
        goto end;
    }
    }

    ran_nas_rel_causes = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "ranNasRelCauses");
    if (ran_nas_rel_causes) {
        cJSON *ran_nas_rel_causes_local = NULL;
        if (!cJSON_IsArray(ran_nas_rel_causes)) {
            ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [ran_nas_rel_causes]");
            goto end;
        }

        ran_nas_rel_causesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_nas_rel_causes_local, ran_nas_rel_causes) {
            if (!cJSON_IsObject(ran_nas_rel_causes_local)) {
                ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [ran_nas_rel_causes]");
                goto end;
            }
            OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_causesItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(ran_nas_rel_causes_local);
            if (!ran_nas_rel_causesItem) {
                ogs_error("No ran_nas_rel_causesItem");
                goto end;
            }
            OpenAPI_list_add(ran_nas_rel_causesList, ran_nas_rel_causesItem);
        }
    }

    accu_usage_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "accuUsageReports");
    if (accu_usage_reports) {
        cJSON *accu_usage_reports_local = NULL;
        if (!cJSON_IsArray(accu_usage_reports)) {
            ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [accu_usage_reports]");
            goto end;
        }

        accu_usage_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(accu_usage_reports_local, accu_usage_reports) {
            if (!cJSON_IsObject(accu_usage_reports_local)) {
                ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [accu_usage_reports]");
                goto end;
            }
            OpenAPI_accu_usage_report_t *accu_usage_reportsItem = OpenAPI_accu_usage_report_parseFromJSON(accu_usage_reports_local);
            if (!accu_usage_reportsItem) {
                ogs_error("No accu_usage_reportsItem");
                goto end;
            }
            OpenAPI_list_add(accu_usage_reportsList, accu_usage_reportsItem);
        }
    }

    pdu_sess_rel_cause = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "pduSessRelCause");
    if (pdu_sess_rel_cause) {
    if (!cJSON_IsString(pdu_sess_rel_cause)) {
        ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [pdu_sess_rel_cause]");
        goto end;
    }
    pdu_sess_rel_causeVariable = OpenAPI_pdu_session_rel_cause_FromString(pdu_sess_rel_cause->valuestring);
    }

    qos_mon_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_delete_dataJSON, "qosMonReports");
    if (qos_mon_reports) {
        cJSON *qos_mon_reports_local = NULL;
        if (!cJSON_IsArray(qos_mon_reports)) {
            ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [qos_mon_reports]");
            goto end;
        }

        qos_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_mon_reports_local, qos_mon_reports) {
            if (!cJSON_IsObject(qos_mon_reports_local)) {
                ogs_error("OpenAPI_sm_policy_delete_data_parseFromJSON() failed [qos_mon_reports]");
                goto end;
            }
            OpenAPI_qos_monitoring_report_t *qos_mon_reportsItem = OpenAPI_qos_monitoring_report_parseFromJSON(qos_mon_reports_local);
            if (!qos_mon_reportsItem) {
                ogs_error("No qos_mon_reportsItem");
                goto end;
            }
            OpenAPI_list_add(qos_mon_reportsList, qos_mon_reportsItem);
        }
    }

    sm_policy_delete_data_local_var = OpenAPI_sm_policy_delete_data_create (
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        user_location_info_time && !cJSON_IsNull(user_location_info_time) ? ogs_strdup(user_location_info_time->valuestring) : NULL,
        ran_nas_rel_causes ? ran_nas_rel_causesList : NULL,
        accu_usage_reports ? accu_usage_reportsList : NULL,
        pdu_sess_rel_cause ? pdu_sess_rel_causeVariable : 0,
        qos_mon_reports ? qos_mon_reportsList : NULL
    );

    return sm_policy_delete_data_local_var;
end:
    if (user_location_info_local_nonprim) {
        OpenAPI_user_location_free(user_location_info_local_nonprim);
        user_location_info_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (ran_nas_rel_causesList) {
        OpenAPI_list_for_each(ran_nas_rel_causesList, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(ran_nas_rel_causesList);
        ran_nas_rel_causesList = NULL;
    }
    if (accu_usage_reportsList) {
        OpenAPI_list_for_each(accu_usage_reportsList, node) {
            OpenAPI_accu_usage_report_free(node->data);
        }
        OpenAPI_list_free(accu_usage_reportsList);
        accu_usage_reportsList = NULL;
    }
    if (qos_mon_reportsList) {
        OpenAPI_list_for_each(qos_mon_reportsList, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(qos_mon_reportsList);
        qos_mon_reportsList = NULL;
    }
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

