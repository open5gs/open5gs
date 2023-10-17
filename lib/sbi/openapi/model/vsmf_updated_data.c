
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vsmf_updated_data.h"

OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_create(
    OpenAPI_list_t *qos_flows_add_mod_list,
    OpenAPI_list_t *qos_flows_rel_list,
    OpenAPI_list_t *qos_flows_failedto_add_mod_list,
    OpenAPI_list_t *qos_flows_failedto_rel_list,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue,
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    OpenAPI_list_t *assigned_ebi_list,
    OpenAPI_list_t *failed_to_assign_ebi_list,
    OpenAPI_list_t *released_ebi_list,
    OpenAPI_list_t *secondary_rat_usage_report,
    OpenAPI_list_t *secondary_rat_usage_info,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    OpenAPI_n4_information_t *n4_info_ext3
)
{
    OpenAPI_vsmf_updated_data_t *vsmf_updated_data_local_var = ogs_malloc(sizeof(OpenAPI_vsmf_updated_data_t));
    ogs_assert(vsmf_updated_data_local_var);

    vsmf_updated_data_local_var->qos_flows_add_mod_list = qos_flows_add_mod_list;
    vsmf_updated_data_local_var->qos_flows_rel_list = qos_flows_rel_list;
    vsmf_updated_data_local_var->qos_flows_failedto_add_mod_list = qos_flows_failedto_add_mod_list;
    vsmf_updated_data_local_var->qos_flows_failedto_rel_list = qos_flows_failedto_rel_list;
    vsmf_updated_data_local_var->n1_sm_info_from_ue = n1_sm_info_from_ue;
    vsmf_updated_data_local_var->unknown_n1_sm_info = unknown_n1_sm_info;
    vsmf_updated_data_local_var->ue_location = ue_location;
    vsmf_updated_data_local_var->ue_time_zone = ue_time_zone;
    vsmf_updated_data_local_var->add_ue_location = add_ue_location;
    vsmf_updated_data_local_var->assigned_ebi_list = assigned_ebi_list;
    vsmf_updated_data_local_var->failed_to_assign_ebi_list = failed_to_assign_ebi_list;
    vsmf_updated_data_local_var->released_ebi_list = released_ebi_list;
    vsmf_updated_data_local_var->secondary_rat_usage_report = secondary_rat_usage_report;
    vsmf_updated_data_local_var->secondary_rat_usage_info = secondary_rat_usage_info;
    vsmf_updated_data_local_var->n4_info = n4_info;
    vsmf_updated_data_local_var->n4_info_ext1 = n4_info_ext1;
    vsmf_updated_data_local_var->n4_info_ext2 = n4_info_ext2;
    vsmf_updated_data_local_var->n4_info_ext3 = n4_info_ext3;

    return vsmf_updated_data_local_var;
}

void OpenAPI_vsmf_updated_data_free(OpenAPI_vsmf_updated_data_t *vsmf_updated_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vsmf_updated_data) {
        return;
    }
    if (vsmf_updated_data->qos_flows_add_mod_list) {
        OpenAPI_list_for_each(vsmf_updated_data->qos_flows_add_mod_list, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->qos_flows_add_mod_list);
        vsmf_updated_data->qos_flows_add_mod_list = NULL;
    }
    if (vsmf_updated_data->qos_flows_rel_list) {
        OpenAPI_list_for_each(vsmf_updated_data->qos_flows_rel_list, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->qos_flows_rel_list);
        vsmf_updated_data->qos_flows_rel_list = NULL;
    }
    if (vsmf_updated_data->qos_flows_failedto_add_mod_list) {
        OpenAPI_list_for_each(vsmf_updated_data->qos_flows_failedto_add_mod_list, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->qos_flows_failedto_add_mod_list);
        vsmf_updated_data->qos_flows_failedto_add_mod_list = NULL;
    }
    if (vsmf_updated_data->qos_flows_failedto_rel_list) {
        OpenAPI_list_for_each(vsmf_updated_data->qos_flows_failedto_rel_list, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->qos_flows_failedto_rel_list);
        vsmf_updated_data->qos_flows_failedto_rel_list = NULL;
    }
    if (vsmf_updated_data->n1_sm_info_from_ue) {
        OpenAPI_ref_to_binary_data_free(vsmf_updated_data->n1_sm_info_from_ue);
        vsmf_updated_data->n1_sm_info_from_ue = NULL;
    }
    if (vsmf_updated_data->unknown_n1_sm_info) {
        OpenAPI_ref_to_binary_data_free(vsmf_updated_data->unknown_n1_sm_info);
        vsmf_updated_data->unknown_n1_sm_info = NULL;
    }
    if (vsmf_updated_data->ue_location) {
        OpenAPI_user_location_free(vsmf_updated_data->ue_location);
        vsmf_updated_data->ue_location = NULL;
    }
    if (vsmf_updated_data->ue_time_zone) {
        ogs_free(vsmf_updated_data->ue_time_zone);
        vsmf_updated_data->ue_time_zone = NULL;
    }
    if (vsmf_updated_data->add_ue_location) {
        OpenAPI_user_location_free(vsmf_updated_data->add_ue_location);
        vsmf_updated_data->add_ue_location = NULL;
    }
    if (vsmf_updated_data->assigned_ebi_list) {
        OpenAPI_list_for_each(vsmf_updated_data->assigned_ebi_list, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->assigned_ebi_list);
        vsmf_updated_data->assigned_ebi_list = NULL;
    }
    if (vsmf_updated_data->failed_to_assign_ebi_list) {
        OpenAPI_list_for_each(vsmf_updated_data->failed_to_assign_ebi_list, node) {
            OpenAPI_arp_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->failed_to_assign_ebi_list);
        vsmf_updated_data->failed_to_assign_ebi_list = NULL;
    }
    if (vsmf_updated_data->released_ebi_list) {
        OpenAPI_list_for_each(vsmf_updated_data->released_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->released_ebi_list);
        vsmf_updated_data->released_ebi_list = NULL;
    }
    if (vsmf_updated_data->secondary_rat_usage_report) {
        OpenAPI_list_for_each(vsmf_updated_data->secondary_rat_usage_report, node) {
            OpenAPI_secondary_rat_usage_report_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->secondary_rat_usage_report);
        vsmf_updated_data->secondary_rat_usage_report = NULL;
    }
    if (vsmf_updated_data->secondary_rat_usage_info) {
        OpenAPI_list_for_each(vsmf_updated_data->secondary_rat_usage_info, node) {
            OpenAPI_secondary_rat_usage_info_free(node->data);
        }
        OpenAPI_list_free(vsmf_updated_data->secondary_rat_usage_info);
        vsmf_updated_data->secondary_rat_usage_info = NULL;
    }
    if (vsmf_updated_data->n4_info) {
        OpenAPI_n4_information_free(vsmf_updated_data->n4_info);
        vsmf_updated_data->n4_info = NULL;
    }
    if (vsmf_updated_data->n4_info_ext1) {
        OpenAPI_n4_information_free(vsmf_updated_data->n4_info_ext1);
        vsmf_updated_data->n4_info_ext1 = NULL;
    }
    if (vsmf_updated_data->n4_info_ext2) {
        OpenAPI_n4_information_free(vsmf_updated_data->n4_info_ext2);
        vsmf_updated_data->n4_info_ext2 = NULL;
    }
    if (vsmf_updated_data->n4_info_ext3) {
        OpenAPI_n4_information_free(vsmf_updated_data->n4_info_ext3);
        vsmf_updated_data->n4_info_ext3 = NULL;
    }
    ogs_free(vsmf_updated_data);
}

cJSON *OpenAPI_vsmf_updated_data_convertToJSON(OpenAPI_vsmf_updated_data_t *vsmf_updated_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vsmf_updated_data == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [VsmfUpdatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vsmf_updated_data->qos_flows_add_mod_list) {
    cJSON *qos_flows_add_mod_listList = cJSON_AddArrayToObject(item, "qosFlowsAddModList");
    if (qos_flows_add_mod_listList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_add_mod_list]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->qos_flows_add_mod_list, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_add_mod_list]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_add_mod_listList, itemLocal);
    }
    }

    if (vsmf_updated_data->qos_flows_rel_list) {
    cJSON *qos_flows_rel_listList = cJSON_AddArrayToObject(item, "qosFlowsRelList");
    if (qos_flows_rel_listList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_rel_list]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->qos_flows_rel_list, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_rel_list]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_rel_listList, itemLocal);
    }
    }

    if (vsmf_updated_data->qos_flows_failedto_add_mod_list) {
    cJSON *qos_flows_failedto_add_mod_listList = cJSON_AddArrayToObject(item, "qosFlowsFailedtoAddModList");
    if (qos_flows_failedto_add_mod_listList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_failedto_add_mod_list]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->qos_flows_failedto_add_mod_list, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_failedto_add_mod_list]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_failedto_add_mod_listList, itemLocal);
    }
    }

    if (vsmf_updated_data->qos_flows_failedto_rel_list) {
    cJSON *qos_flows_failedto_rel_listList = cJSON_AddArrayToObject(item, "qosFlowsFailedtoRelList");
    if (qos_flows_failedto_rel_listList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_failedto_rel_list]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->qos_flows_failedto_rel_list, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [qos_flows_failedto_rel_list]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_failedto_rel_listList, itemLocal);
    }
    }

    if (vsmf_updated_data->n1_sm_info_from_ue) {
    cJSON *n1_sm_info_from_ue_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(vsmf_updated_data->n1_sm_info_from_ue);
    if (n1_sm_info_from_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n1_sm_info_from_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmInfoFromUe", n1_sm_info_from_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n1_sm_info_from_ue]");
        goto end;
    }
    }

    if (vsmf_updated_data->unknown_n1_sm_info) {
    cJSON *unknown_n1_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(vsmf_updated_data->unknown_n1_sm_info);
    if (unknown_n1_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [unknown_n1_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "unknownN1SmInfo", unknown_n1_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [unknown_n1_sm_info]");
        goto end;
    }
    }

    if (vsmf_updated_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(vsmf_updated_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (vsmf_updated_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", vsmf_updated_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (vsmf_updated_data->add_ue_location) {
    cJSON *add_ue_location_local_JSON = OpenAPI_user_location_convertToJSON(vsmf_updated_data->add_ue_location);
    if (add_ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addUeLocation", add_ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    }

    if (vsmf_updated_data->assigned_ebi_list) {
    cJSON *assigned_ebi_listList = cJSON_AddArrayToObject(item, "assignedEbiList");
    if (assigned_ebi_listList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [assigned_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->assigned_ebi_list, node) {
        cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [assigned_ebi_list]");
            goto end;
        }
        cJSON_AddItemToArray(assigned_ebi_listList, itemLocal);
    }
    }

    if (vsmf_updated_data->failed_to_assign_ebi_list) {
    cJSON *failed_to_assign_ebi_listList = cJSON_AddArrayToObject(item, "failedToAssignEbiList");
    if (failed_to_assign_ebi_listList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [failed_to_assign_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->failed_to_assign_ebi_list, node) {
        cJSON *itemLocal = OpenAPI_arp_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [failed_to_assign_ebi_list]");
            goto end;
        }
        cJSON_AddItemToArray(failed_to_assign_ebi_listList, itemLocal);
    }
    }

    if (vsmf_updated_data->released_ebi_list) {
    cJSON *released_ebi_listList = cJSON_AddArrayToObject(item, "releasedEbiList");
    if (released_ebi_listList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [released_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->released_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [released_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(released_ebi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [released_ebi_list]");
            goto end;
        }
    }
    }

    if (vsmf_updated_data->secondary_rat_usage_report) {
    cJSON *secondary_rat_usage_reportList = cJSON_AddArrayToObject(item, "secondaryRatUsageReport");
    if (secondary_rat_usage_reportList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [secondary_rat_usage_report]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->secondary_rat_usage_report, node) {
        cJSON *itemLocal = OpenAPI_secondary_rat_usage_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [secondary_rat_usage_report]");
            goto end;
        }
        cJSON_AddItemToArray(secondary_rat_usage_reportList, itemLocal);
    }
    }

    if (vsmf_updated_data->secondary_rat_usage_info) {
    cJSON *secondary_rat_usage_infoList = cJSON_AddArrayToObject(item, "secondaryRatUsageInfo");
    if (secondary_rat_usage_infoList == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [secondary_rat_usage_info]");
        goto end;
    }
    OpenAPI_list_for_each(vsmf_updated_data->secondary_rat_usage_info, node) {
        cJSON *itemLocal = OpenAPI_secondary_rat_usage_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [secondary_rat_usage_info]");
            goto end;
        }
        cJSON_AddItemToArray(secondary_rat_usage_infoList, itemLocal);
    }
    }

    if (vsmf_updated_data->n4_info) {
    cJSON *n4_info_local_JSON = OpenAPI_n4_information_convertToJSON(vsmf_updated_data->n4_info);
    if (n4_info_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4Info", n4_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info]");
        goto end;
    }
    }

    if (vsmf_updated_data->n4_info_ext1) {
    cJSON *n4_info_ext1_local_JSON = OpenAPI_n4_information_convertToJSON(vsmf_updated_data->n4_info_ext1);
    if (n4_info_ext1_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info_ext1]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4InfoExt1", n4_info_ext1_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info_ext1]");
        goto end;
    }
    }

    if (vsmf_updated_data->n4_info_ext2) {
    cJSON *n4_info_ext2_local_JSON = OpenAPI_n4_information_convertToJSON(vsmf_updated_data->n4_info_ext2);
    if (n4_info_ext2_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info_ext2]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4InfoExt2", n4_info_ext2_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info_ext2]");
        goto end;
    }
    }

    if (vsmf_updated_data->n4_info_ext3) {
    cJSON *n4_info_ext3_local_JSON = OpenAPI_n4_information_convertToJSON(vsmf_updated_data->n4_info_ext3);
    if (n4_info_ext3_local_JSON == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info_ext3]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4InfoExt3", n4_info_ext3_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed [n4_info_ext3]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_parseFromJSON(cJSON *vsmf_updated_dataJSON)
{
    OpenAPI_vsmf_updated_data_t *vsmf_updated_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qos_flows_add_mod_list = NULL;
    OpenAPI_list_t *qos_flows_add_mod_listList = NULL;
    cJSON *qos_flows_rel_list = NULL;
    OpenAPI_list_t *qos_flows_rel_listList = NULL;
    cJSON *qos_flows_failedto_add_mod_list = NULL;
    OpenAPI_list_t *qos_flows_failedto_add_mod_listList = NULL;
    cJSON *qos_flows_failedto_rel_list = NULL;
    OpenAPI_list_t *qos_flows_failedto_rel_listList = NULL;
    cJSON *n1_sm_info_from_ue = NULL;
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue_local_nonprim = NULL;
    cJSON *unknown_n1_sm_info = NULL;
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info_local_nonprim = NULL;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *add_ue_location = NULL;
    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    cJSON *assigned_ebi_list = NULL;
    OpenAPI_list_t *assigned_ebi_listList = NULL;
    cJSON *failed_to_assign_ebi_list = NULL;
    OpenAPI_list_t *failed_to_assign_ebi_listList = NULL;
    cJSON *released_ebi_list = NULL;
    OpenAPI_list_t *released_ebi_listList = NULL;
    cJSON *secondary_rat_usage_report = NULL;
    OpenAPI_list_t *secondary_rat_usage_reportList = NULL;
    cJSON *secondary_rat_usage_info = NULL;
    OpenAPI_list_t *secondary_rat_usage_infoList = NULL;
    cJSON *n4_info = NULL;
    OpenAPI_n4_information_t *n4_info_local_nonprim = NULL;
    cJSON *n4_info_ext1 = NULL;
    OpenAPI_n4_information_t *n4_info_ext1_local_nonprim = NULL;
    cJSON *n4_info_ext2 = NULL;
    OpenAPI_n4_information_t *n4_info_ext2_local_nonprim = NULL;
    cJSON *n4_info_ext3 = NULL;
    OpenAPI_n4_information_t *n4_info_ext3_local_nonprim = NULL;
    qos_flows_add_mod_list = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "qosFlowsAddModList");
    if (qos_flows_add_mod_list) {
        cJSON *qos_flows_add_mod_list_local = NULL;
        if (!cJSON_IsArray(qos_flows_add_mod_list)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_add_mod_list]");
            goto end;
        }

        qos_flows_add_mod_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_add_mod_list_local, qos_flows_add_mod_list) {
            if (!cJSON_IsObject(qos_flows_add_mod_list_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_add_mod_list]");
                goto end;
            }
            OpenAPI_qos_flow_item_t *qos_flows_add_mod_listItem = OpenAPI_qos_flow_item_parseFromJSON(qos_flows_add_mod_list_local);
            if (!qos_flows_add_mod_listItem) {
                ogs_error("No qos_flows_add_mod_listItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_add_mod_listList, qos_flows_add_mod_listItem);
        }
    }

    qos_flows_rel_list = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "qosFlowsRelList");
    if (qos_flows_rel_list) {
        cJSON *qos_flows_rel_list_local = NULL;
        if (!cJSON_IsArray(qos_flows_rel_list)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_rel_list]");
            goto end;
        }

        qos_flows_rel_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_rel_list_local, qos_flows_rel_list) {
            if (!cJSON_IsObject(qos_flows_rel_list_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_rel_list]");
                goto end;
            }
            OpenAPI_qos_flow_item_t *qos_flows_rel_listItem = OpenAPI_qos_flow_item_parseFromJSON(qos_flows_rel_list_local);
            if (!qos_flows_rel_listItem) {
                ogs_error("No qos_flows_rel_listItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_rel_listList, qos_flows_rel_listItem);
        }
    }

    qos_flows_failedto_add_mod_list = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "qosFlowsFailedtoAddModList");
    if (qos_flows_failedto_add_mod_list) {
        cJSON *qos_flows_failedto_add_mod_list_local = NULL;
        if (!cJSON_IsArray(qos_flows_failedto_add_mod_list)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_failedto_add_mod_list]");
            goto end;
        }

        qos_flows_failedto_add_mod_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_failedto_add_mod_list_local, qos_flows_failedto_add_mod_list) {
            if (!cJSON_IsObject(qos_flows_failedto_add_mod_list_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_failedto_add_mod_list]");
                goto end;
            }
            OpenAPI_qos_flow_item_t *qos_flows_failedto_add_mod_listItem = OpenAPI_qos_flow_item_parseFromJSON(qos_flows_failedto_add_mod_list_local);
            if (!qos_flows_failedto_add_mod_listItem) {
                ogs_error("No qos_flows_failedto_add_mod_listItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_failedto_add_mod_listList, qos_flows_failedto_add_mod_listItem);
        }
    }

    qos_flows_failedto_rel_list = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "qosFlowsFailedtoRelList");
    if (qos_flows_failedto_rel_list) {
        cJSON *qos_flows_failedto_rel_list_local = NULL;
        if (!cJSON_IsArray(qos_flows_failedto_rel_list)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_failedto_rel_list]");
            goto end;
        }

        qos_flows_failedto_rel_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_failedto_rel_list_local, qos_flows_failedto_rel_list) {
            if (!cJSON_IsObject(qos_flows_failedto_rel_list_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [qos_flows_failedto_rel_list]");
                goto end;
            }
            OpenAPI_qos_flow_item_t *qos_flows_failedto_rel_listItem = OpenAPI_qos_flow_item_parseFromJSON(qos_flows_failedto_rel_list_local);
            if (!qos_flows_failedto_rel_listItem) {
                ogs_error("No qos_flows_failedto_rel_listItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_failedto_rel_listList, qos_flows_failedto_rel_listItem);
        }
    }

    n1_sm_info_from_ue = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "n1SmInfoFromUe");
    if (n1_sm_info_from_ue) {
    n1_sm_info_from_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_from_ue);
    if (!n1_sm_info_from_ue_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n1_sm_info_from_ue]");
        goto end;
    }
    }

    unknown_n1_sm_info = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "unknownN1SmInfo");
    if (unknown_n1_sm_info) {
    unknown_n1_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(unknown_n1_sm_info);
    if (!unknown_n1_sm_info_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [unknown_n1_sm_info]");
        goto end;
    }
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    add_ue_location = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "addUeLocation");
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    if (!add_ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [add_ue_location]");
        goto end;
    }
    }

    assigned_ebi_list = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "assignedEbiList");
    if (assigned_ebi_list) {
        cJSON *assigned_ebi_list_local = NULL;
        if (!cJSON_IsArray(assigned_ebi_list)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [assigned_ebi_list]");
            goto end;
        }

        assigned_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(assigned_ebi_list_local, assigned_ebi_list) {
            if (!cJSON_IsObject(assigned_ebi_list_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [assigned_ebi_list]");
                goto end;
            }
            OpenAPI_ebi_arp_mapping_t *assigned_ebi_listItem = OpenAPI_ebi_arp_mapping_parseFromJSON(assigned_ebi_list_local);
            if (!assigned_ebi_listItem) {
                ogs_error("No assigned_ebi_listItem");
                goto end;
            }
            OpenAPI_list_add(assigned_ebi_listList, assigned_ebi_listItem);
        }
    }

    failed_to_assign_ebi_list = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "failedToAssignEbiList");
    if (failed_to_assign_ebi_list) {
        cJSON *failed_to_assign_ebi_list_local = NULL;
        if (!cJSON_IsArray(failed_to_assign_ebi_list)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [failed_to_assign_ebi_list]");
            goto end;
        }

        failed_to_assign_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(failed_to_assign_ebi_list_local, failed_to_assign_ebi_list) {
            if (!cJSON_IsObject(failed_to_assign_ebi_list_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [failed_to_assign_ebi_list]");
                goto end;
            }
            OpenAPI_arp_t *failed_to_assign_ebi_listItem = OpenAPI_arp_parseFromJSON(failed_to_assign_ebi_list_local);
            if (!failed_to_assign_ebi_listItem) {
                ogs_error("No failed_to_assign_ebi_listItem");
                goto end;
            }
            OpenAPI_list_add(failed_to_assign_ebi_listList, failed_to_assign_ebi_listItem);
        }
    }

    released_ebi_list = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "releasedEbiList");
    if (released_ebi_list) {
        cJSON *released_ebi_list_local = NULL;
        if (!cJSON_IsArray(released_ebi_list)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [released_ebi_list]");
            goto end;
        }

        released_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(released_ebi_list_local, released_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(released_ebi_list_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [released_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [released_ebi_list]");
                goto end;
            }
            *localDouble = released_ebi_list_local->valuedouble;
            OpenAPI_list_add(released_ebi_listList, localDouble);
        }
    }

    secondary_rat_usage_report = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "secondaryRatUsageReport");
    if (secondary_rat_usage_report) {
        cJSON *secondary_rat_usage_report_local = NULL;
        if (!cJSON_IsArray(secondary_rat_usage_report)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [secondary_rat_usage_report]");
            goto end;
        }

        secondary_rat_usage_reportList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_report_local, secondary_rat_usage_report) {
            if (!cJSON_IsObject(secondary_rat_usage_report_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [secondary_rat_usage_report]");
                goto end;
            }
            OpenAPI_secondary_rat_usage_report_t *secondary_rat_usage_reportItem = OpenAPI_secondary_rat_usage_report_parseFromJSON(secondary_rat_usage_report_local);
            if (!secondary_rat_usage_reportItem) {
                ogs_error("No secondary_rat_usage_reportItem");
                goto end;
            }
            OpenAPI_list_add(secondary_rat_usage_reportList, secondary_rat_usage_reportItem);
        }
    }

    secondary_rat_usage_info = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "secondaryRatUsageInfo");
    if (secondary_rat_usage_info) {
        cJSON *secondary_rat_usage_info_local = NULL;
        if (!cJSON_IsArray(secondary_rat_usage_info)) {
            ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [secondary_rat_usage_info]");
            goto end;
        }

        secondary_rat_usage_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_info_local, secondary_rat_usage_info) {
            if (!cJSON_IsObject(secondary_rat_usage_info_local)) {
                ogs_error("OpenAPI_vsmf_updated_data_parseFromJSON() failed [secondary_rat_usage_info]");
                goto end;
            }
            OpenAPI_secondary_rat_usage_info_t *secondary_rat_usage_infoItem = OpenAPI_secondary_rat_usage_info_parseFromJSON(secondary_rat_usage_info_local);
            if (!secondary_rat_usage_infoItem) {
                ogs_error("No secondary_rat_usage_infoItem");
                goto end;
            }
            OpenAPI_list_add(secondary_rat_usage_infoList, secondary_rat_usage_infoItem);
        }
    }

    n4_info = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "n4Info");
    if (n4_info) {
    n4_info_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info);
    if (!n4_info_local_nonprim) {
        ogs_error("OpenAPI_n4_information_parseFromJSON failed [n4_info]");
        goto end;
    }
    }

    n4_info_ext1 = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "n4InfoExt1");
    if (n4_info_ext1) {
    n4_info_ext1_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext1);
    if (!n4_info_ext1_local_nonprim) {
        ogs_error("OpenAPI_n4_information_parseFromJSON failed [n4_info_ext1]");
        goto end;
    }
    }

    n4_info_ext2 = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "n4InfoExt2");
    if (n4_info_ext2) {
    n4_info_ext2_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext2);
    if (!n4_info_ext2_local_nonprim) {
        ogs_error("OpenAPI_n4_information_parseFromJSON failed [n4_info_ext2]");
        goto end;
    }
    }

    n4_info_ext3 = cJSON_GetObjectItemCaseSensitive(vsmf_updated_dataJSON, "n4InfoExt3");
    if (n4_info_ext3) {
    n4_info_ext3_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext3);
    if (!n4_info_ext3_local_nonprim) {
        ogs_error("OpenAPI_n4_information_parseFromJSON failed [n4_info_ext3]");
        goto end;
    }
    }

    vsmf_updated_data_local_var = OpenAPI_vsmf_updated_data_create (
        qos_flows_add_mod_list ? qos_flows_add_mod_listList : NULL,
        qos_flows_rel_list ? qos_flows_rel_listList : NULL,
        qos_flows_failedto_add_mod_list ? qos_flows_failedto_add_mod_listList : NULL,
        qos_flows_failedto_rel_list ? qos_flows_failedto_rel_listList : NULL,
        n1_sm_info_from_ue ? n1_sm_info_from_ue_local_nonprim : NULL,
        unknown_n1_sm_info ? unknown_n1_sm_info_local_nonprim : NULL,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        assigned_ebi_list ? assigned_ebi_listList : NULL,
        failed_to_assign_ebi_list ? failed_to_assign_ebi_listList : NULL,
        released_ebi_list ? released_ebi_listList : NULL,
        secondary_rat_usage_report ? secondary_rat_usage_reportList : NULL,
        secondary_rat_usage_info ? secondary_rat_usage_infoList : NULL,
        n4_info ? n4_info_local_nonprim : NULL,
        n4_info_ext1 ? n4_info_ext1_local_nonprim : NULL,
        n4_info_ext2 ? n4_info_ext2_local_nonprim : NULL,
        n4_info_ext3 ? n4_info_ext3_local_nonprim : NULL
    );

    return vsmf_updated_data_local_var;
end:
    if (qos_flows_add_mod_listList) {
        OpenAPI_list_for_each(qos_flows_add_mod_listList, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(qos_flows_add_mod_listList);
        qos_flows_add_mod_listList = NULL;
    }
    if (qos_flows_rel_listList) {
        OpenAPI_list_for_each(qos_flows_rel_listList, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(qos_flows_rel_listList);
        qos_flows_rel_listList = NULL;
    }
    if (qos_flows_failedto_add_mod_listList) {
        OpenAPI_list_for_each(qos_flows_failedto_add_mod_listList, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(qos_flows_failedto_add_mod_listList);
        qos_flows_failedto_add_mod_listList = NULL;
    }
    if (qos_flows_failedto_rel_listList) {
        OpenAPI_list_for_each(qos_flows_failedto_rel_listList, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(qos_flows_failedto_rel_listList);
        qos_flows_failedto_rel_listList = NULL;
    }
    if (n1_sm_info_from_ue_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n1_sm_info_from_ue_local_nonprim);
        n1_sm_info_from_ue_local_nonprim = NULL;
    }
    if (unknown_n1_sm_info_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(unknown_n1_sm_info_local_nonprim);
        unknown_n1_sm_info_local_nonprim = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    if (add_ue_location_local_nonprim) {
        OpenAPI_user_location_free(add_ue_location_local_nonprim);
        add_ue_location_local_nonprim = NULL;
    }
    if (assigned_ebi_listList) {
        OpenAPI_list_for_each(assigned_ebi_listList, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(assigned_ebi_listList);
        assigned_ebi_listList = NULL;
    }
    if (failed_to_assign_ebi_listList) {
        OpenAPI_list_for_each(failed_to_assign_ebi_listList, node) {
            OpenAPI_arp_free(node->data);
        }
        OpenAPI_list_free(failed_to_assign_ebi_listList);
        failed_to_assign_ebi_listList = NULL;
    }
    if (released_ebi_listList) {
        OpenAPI_list_for_each(released_ebi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(released_ebi_listList);
        released_ebi_listList = NULL;
    }
    if (secondary_rat_usage_reportList) {
        OpenAPI_list_for_each(secondary_rat_usage_reportList, node) {
            OpenAPI_secondary_rat_usage_report_free(node->data);
        }
        OpenAPI_list_free(secondary_rat_usage_reportList);
        secondary_rat_usage_reportList = NULL;
    }
    if (secondary_rat_usage_infoList) {
        OpenAPI_list_for_each(secondary_rat_usage_infoList, node) {
            OpenAPI_secondary_rat_usage_info_free(node->data);
        }
        OpenAPI_list_free(secondary_rat_usage_infoList);
        secondary_rat_usage_infoList = NULL;
    }
    if (n4_info_local_nonprim) {
        OpenAPI_n4_information_free(n4_info_local_nonprim);
        n4_info_local_nonprim = NULL;
    }
    if (n4_info_ext1_local_nonprim) {
        OpenAPI_n4_information_free(n4_info_ext1_local_nonprim);
        n4_info_ext1_local_nonprim = NULL;
    }
    if (n4_info_ext2_local_nonprim) {
        OpenAPI_n4_information_free(n4_info_ext2_local_nonprim);
        n4_info_ext2_local_nonprim = NULL;
    }
    if (n4_info_ext3_local_nonprim) {
        OpenAPI_n4_information_free(n4_info_ext3_local_nonprim);
        n4_info_ext3_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_copy(OpenAPI_vsmf_updated_data_t *dst, OpenAPI_vsmf_updated_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vsmf_updated_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vsmf_updated_data_convertToJSON() failed");
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

    OpenAPI_vsmf_updated_data_free(dst);
    dst = OpenAPI_vsmf_updated_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

