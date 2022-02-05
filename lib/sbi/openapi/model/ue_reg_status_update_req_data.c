
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reg_status_update_req_data.h"

OpenAPI_ue_reg_status_update_req_data_t *OpenAPI_ue_reg_status_update_req_data_create(
    OpenAPI_ue_context_transfer_status_e transfer_status,
    OpenAPI_list_t *to_release_session_list,
    bool is_pcf_reselected_ind,
    int pcf_reselected_ind,
    OpenAPI_list_t *smf_change_info_list
)
{
    OpenAPI_ue_reg_status_update_req_data_t *ue_reg_status_update_req_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_reg_status_update_req_data_t));
    ogs_assert(ue_reg_status_update_req_data_local_var);

    ue_reg_status_update_req_data_local_var->transfer_status = transfer_status;
    ue_reg_status_update_req_data_local_var->to_release_session_list = to_release_session_list;
    ue_reg_status_update_req_data_local_var->is_pcf_reselected_ind = is_pcf_reselected_ind;
    ue_reg_status_update_req_data_local_var->pcf_reselected_ind = pcf_reselected_ind;
    ue_reg_status_update_req_data_local_var->smf_change_info_list = smf_change_info_list;

    return ue_reg_status_update_req_data_local_var;
}

void OpenAPI_ue_reg_status_update_req_data_free(OpenAPI_ue_reg_status_update_req_data_t *ue_reg_status_update_req_data)
{
    if (NULL == ue_reg_status_update_req_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(ue_reg_status_update_req_data->to_release_session_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_reg_status_update_req_data->to_release_session_list);
    OpenAPI_list_for_each(ue_reg_status_update_req_data->smf_change_info_list, node) {
        OpenAPI_smf_change_info_free(node->data);
    }
    OpenAPI_list_free(ue_reg_status_update_req_data->smf_change_info_list);
    ogs_free(ue_reg_status_update_req_data);
}

cJSON *OpenAPI_ue_reg_status_update_req_data_convertToJSON(OpenAPI_ue_reg_status_update_req_data_t *ue_reg_status_update_req_data)
{
    cJSON *item = NULL;

    if (ue_reg_status_update_req_data == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed [UeRegStatusUpdateReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "transferStatus", OpenAPI_ue_context_transfer_status_ToString(ue_reg_status_update_req_data->transfer_status)) == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed [transfer_status]");
        goto end;
    }

    if (ue_reg_status_update_req_data->to_release_session_list) {
    cJSON *to_release_session_list = cJSON_AddArrayToObject(item, "toReleaseSessionList");
    if (to_release_session_list == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed [to_release_session_list]");
        goto end;
    }

    OpenAPI_lnode_t *to_release_session_list_node;
    OpenAPI_list_for_each(ue_reg_status_update_req_data->to_release_session_list, to_release_session_list_node)  {
    if (cJSON_AddNumberToObject(to_release_session_list, "", *(double *)to_release_session_list_node->data) == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed [to_release_session_list]");
        goto end;
    }
                    }
    }

    if (ue_reg_status_update_req_data->is_pcf_reselected_ind) {
    if (cJSON_AddBoolToObject(item, "pcfReselectedInd", ue_reg_status_update_req_data->pcf_reselected_ind) == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed [pcf_reselected_ind]");
        goto end;
    }
    }

    if (ue_reg_status_update_req_data->smf_change_info_list) {
    cJSON *smf_change_info_listList = cJSON_AddArrayToObject(item, "smfChangeInfoList");
    if (smf_change_info_listList == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed [smf_change_info_list]");
        goto end;
    }

    OpenAPI_lnode_t *smf_change_info_list_node;
    if (ue_reg_status_update_req_data->smf_change_info_list) {
        OpenAPI_list_for_each(ue_reg_status_update_req_data->smf_change_info_list, smf_change_info_list_node) {
            cJSON *itemLocal = OpenAPI_smf_change_info_convertToJSON(smf_change_info_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed [smf_change_info_list]");
                goto end;
            }
            cJSON_AddItemToArray(smf_change_info_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_ue_reg_status_update_req_data_t *OpenAPI_ue_reg_status_update_req_data_parseFromJSON(cJSON *ue_reg_status_update_req_dataJSON)
{
    OpenAPI_ue_reg_status_update_req_data_t *ue_reg_status_update_req_data_local_var = NULL;
    cJSON *transfer_status = cJSON_GetObjectItemCaseSensitive(ue_reg_status_update_req_dataJSON, "transferStatus");
    if (!transfer_status) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_parseFromJSON() failed [transfer_status]");
        goto end;
    }

    OpenAPI_ue_context_transfer_status_e transfer_statusVariable;
    if (!cJSON_IsString(transfer_status)) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_parseFromJSON() failed [transfer_status]");
        goto end;
    }
    transfer_statusVariable = OpenAPI_ue_context_transfer_status_FromString(transfer_status->valuestring);

    cJSON *to_release_session_list = cJSON_GetObjectItemCaseSensitive(ue_reg_status_update_req_dataJSON, "toReleaseSessionList");

    OpenAPI_list_t *to_release_session_listList;
    if (to_release_session_list) {
    cJSON *to_release_session_list_local;
    if (!cJSON_IsArray(to_release_session_list)) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_parseFromJSON() failed [to_release_session_list]");
        goto end;
    }
    to_release_session_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(to_release_session_list_local, to_release_session_list) {
    if (!cJSON_IsNumber(to_release_session_list_local)) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_parseFromJSON() failed [to_release_session_list]");
        goto end;
    }
    OpenAPI_list_add(to_release_session_listList , &to_release_session_list_local->valuedouble);
    }
    }

    cJSON *pcf_reselected_ind = cJSON_GetObjectItemCaseSensitive(ue_reg_status_update_req_dataJSON, "pcfReselectedInd");

    if (pcf_reselected_ind) {
    if (!cJSON_IsBool(pcf_reselected_ind)) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_parseFromJSON() failed [pcf_reselected_ind]");
        goto end;
    }
    }

    cJSON *smf_change_info_list = cJSON_GetObjectItemCaseSensitive(ue_reg_status_update_req_dataJSON, "smfChangeInfoList");

    OpenAPI_list_t *smf_change_info_listList;
    if (smf_change_info_list) {
    cJSON *smf_change_info_list_local_nonprimitive;
    if (!cJSON_IsArray(smf_change_info_list)){
        ogs_error("OpenAPI_ue_reg_status_update_req_data_parseFromJSON() failed [smf_change_info_list]");
        goto end;
    }

    smf_change_info_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(smf_change_info_list_local_nonprimitive, smf_change_info_list ) {
        if (!cJSON_IsObject(smf_change_info_list_local_nonprimitive)) {
            ogs_error("OpenAPI_ue_reg_status_update_req_data_parseFromJSON() failed [smf_change_info_list]");
            goto end;
        }
        OpenAPI_smf_change_info_t *smf_change_info_listItem = OpenAPI_smf_change_info_parseFromJSON(smf_change_info_list_local_nonprimitive);

        if (!smf_change_info_listItem) {
            ogs_error("No smf_change_info_listItem");
            OpenAPI_list_free(smf_change_info_listList);
            goto end;
        }

        OpenAPI_list_add(smf_change_info_listList, smf_change_info_listItem);
    }
    }

    ue_reg_status_update_req_data_local_var = OpenAPI_ue_reg_status_update_req_data_create (
        transfer_statusVariable,
        to_release_session_list ? to_release_session_listList : NULL,
        pcf_reselected_ind ? true : false,
        pcf_reselected_ind ? pcf_reselected_ind->valueint : 0,
        smf_change_info_list ? smf_change_info_listList : NULL
    );

    return ue_reg_status_update_req_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_reg_status_update_req_data_t *OpenAPI_ue_reg_status_update_req_data_copy(OpenAPI_ue_reg_status_update_req_data_t *dst, OpenAPI_ue_reg_status_update_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_reg_status_update_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_reg_status_update_req_data_convertToJSON() failed");
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

    OpenAPI_ue_reg_status_update_req_data_free(dst);
    dst = OpenAPI_ue_reg_status_update_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

