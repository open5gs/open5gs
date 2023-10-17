
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_created_data.h"

OpenAPI_ue_context_created_data_t *OpenAPI_ue_context_created_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_n2_info_content_t *target_to_source_data,
    OpenAPI_list_t *pdu_session_list,
    OpenAPI_list_t *failed_session_list,
    char *supported_features,
    bool is_pcf_reselected_ind,
    int pcf_reselected_ind,
    OpenAPI_list_t *analytics_not_used_list
)
{
    OpenAPI_ue_context_created_data_t *ue_context_created_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_created_data_t));
    ogs_assert(ue_context_created_data_local_var);

    ue_context_created_data_local_var->ue_context = ue_context;
    ue_context_created_data_local_var->target_to_source_data = target_to_source_data;
    ue_context_created_data_local_var->pdu_session_list = pdu_session_list;
    ue_context_created_data_local_var->failed_session_list = failed_session_list;
    ue_context_created_data_local_var->supported_features = supported_features;
    ue_context_created_data_local_var->is_pcf_reselected_ind = is_pcf_reselected_ind;
    ue_context_created_data_local_var->pcf_reselected_ind = pcf_reselected_ind;
    ue_context_created_data_local_var->analytics_not_used_list = analytics_not_used_list;

    return ue_context_created_data_local_var;
}

void OpenAPI_ue_context_created_data_free(OpenAPI_ue_context_created_data_t *ue_context_created_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_created_data) {
        return;
    }
    if (ue_context_created_data->ue_context) {
        OpenAPI_ue_context_free(ue_context_created_data->ue_context);
        ue_context_created_data->ue_context = NULL;
    }
    if (ue_context_created_data->target_to_source_data) {
        OpenAPI_n2_info_content_free(ue_context_created_data->target_to_source_data);
        ue_context_created_data->target_to_source_data = NULL;
    }
    if (ue_context_created_data->pdu_session_list) {
        OpenAPI_list_for_each(ue_context_created_data->pdu_session_list, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(ue_context_created_data->pdu_session_list);
        ue_context_created_data->pdu_session_list = NULL;
    }
    if (ue_context_created_data->failed_session_list) {
        OpenAPI_list_for_each(ue_context_created_data->failed_session_list, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(ue_context_created_data->failed_session_list);
        ue_context_created_data->failed_session_list = NULL;
    }
    if (ue_context_created_data->supported_features) {
        ogs_free(ue_context_created_data->supported_features);
        ue_context_created_data->supported_features = NULL;
    }
    if (ue_context_created_data->analytics_not_used_list) {
        OpenAPI_list_for_each(ue_context_created_data->analytics_not_used_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_context_created_data->analytics_not_used_list);
        ue_context_created_data->analytics_not_used_list = NULL;
    }
    ogs_free(ue_context_created_data);
}

cJSON *OpenAPI_ue_context_created_data_convertToJSON(OpenAPI_ue_context_created_data_t *ue_context_created_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_created_data == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [UeContextCreatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_context_created_data->ue_context) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [ue_context]");
        return NULL;
    }
    cJSON *ue_context_local_JSON = OpenAPI_ue_context_convertToJSON(ue_context_created_data->ue_context);
    if (ue_context_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [ue_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueContext", ue_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [ue_context]");
        goto end;
    }

    if (!ue_context_created_data->target_to_source_data) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [target_to_source_data]");
        return NULL;
    }
    cJSON *target_to_source_data_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_created_data->target_to_source_data);
    if (target_to_source_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [target_to_source_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetToSourceData", target_to_source_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [target_to_source_data]");
        goto end;
    }

    if (!ue_context_created_data->pdu_session_list) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [pdu_session_list]");
        return NULL;
    }
    cJSON *pdu_session_listList = cJSON_AddArrayToObject(item, "pduSessionList");
    if (pdu_session_listList == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [pdu_session_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context_created_data->pdu_session_list, node) {
        cJSON *itemLocal = OpenAPI_n2_sm_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [pdu_session_list]");
            goto end;
        }
        cJSON_AddItemToArray(pdu_session_listList, itemLocal);
    }

    if (ue_context_created_data->failed_session_list) {
    cJSON *failed_session_listList = cJSON_AddArrayToObject(item, "failedSessionList");
    if (failed_session_listList == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [failed_session_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context_created_data->failed_session_list, node) {
        cJSON *itemLocal = OpenAPI_n2_sm_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [failed_session_list]");
            goto end;
        }
        cJSON_AddItemToArray(failed_session_listList, itemLocal);
    }
    }

    if (ue_context_created_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ue_context_created_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (ue_context_created_data->is_pcf_reselected_ind) {
    if (cJSON_AddBoolToObject(item, "pcfReselectedInd", ue_context_created_data->pcf_reselected_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [pcf_reselected_ind]");
        goto end;
    }
    }

    if (ue_context_created_data->analytics_not_used_list) {
    cJSON *analytics_not_used_listList = cJSON_AddArrayToObject(item, "analyticsNotUsedList");
    if (analytics_not_used_listList == NULL) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [analytics_not_used_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context_created_data->analytics_not_used_list, node) {
        if (cJSON_AddStringToObject(analytics_not_used_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed [analytics_not_used_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_ue_context_created_data_t *OpenAPI_ue_context_created_data_parseFromJSON(cJSON *ue_context_created_dataJSON)
{
    OpenAPI_ue_context_created_data_t *ue_context_created_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_context = NULL;
    OpenAPI_ue_context_t *ue_context_local_nonprim = NULL;
    cJSON *target_to_source_data = NULL;
    OpenAPI_n2_info_content_t *target_to_source_data_local_nonprim = NULL;
    cJSON *pdu_session_list = NULL;
    OpenAPI_list_t *pdu_session_listList = NULL;
    cJSON *failed_session_list = NULL;
    OpenAPI_list_t *failed_session_listList = NULL;
    cJSON *supported_features = NULL;
    cJSON *pcf_reselected_ind = NULL;
    cJSON *analytics_not_used_list = NULL;
    OpenAPI_list_t *analytics_not_used_listList = NULL;
    ue_context = cJSON_GetObjectItemCaseSensitive(ue_context_created_dataJSON, "ueContext");
    if (!ue_context) {
        ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [ue_context]");
        goto end;
    }
    ue_context_local_nonprim = OpenAPI_ue_context_parseFromJSON(ue_context);
    if (!ue_context_local_nonprim) {
        ogs_error("OpenAPI_ue_context_parseFromJSON failed [ue_context]");
        goto end;
    }

    target_to_source_data = cJSON_GetObjectItemCaseSensitive(ue_context_created_dataJSON, "targetToSourceData");
    if (!target_to_source_data) {
        ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [target_to_source_data]");
        goto end;
    }
    target_to_source_data_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(target_to_source_data);
    if (!target_to_source_data_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [target_to_source_data]");
        goto end;
    }

    pdu_session_list = cJSON_GetObjectItemCaseSensitive(ue_context_created_dataJSON, "pduSessionList");
    if (!pdu_session_list) {
        ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [pdu_session_list]");
        goto end;
    }
        cJSON *pdu_session_list_local = NULL;
        if (!cJSON_IsArray(pdu_session_list)) {
            ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [pdu_session_list]");
            goto end;
        }

        pdu_session_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_list_local, pdu_session_list) {
            if (!cJSON_IsObject(pdu_session_list_local)) {
                ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [pdu_session_list]");
                goto end;
            }
            OpenAPI_n2_sm_information_t *pdu_session_listItem = OpenAPI_n2_sm_information_parseFromJSON(pdu_session_list_local);
            if (!pdu_session_listItem) {
                ogs_error("No pdu_session_listItem");
                goto end;
            }
            OpenAPI_list_add(pdu_session_listList, pdu_session_listItem);
        }

    failed_session_list = cJSON_GetObjectItemCaseSensitive(ue_context_created_dataJSON, "failedSessionList");
    if (failed_session_list) {
        cJSON *failed_session_list_local = NULL;
        if (!cJSON_IsArray(failed_session_list)) {
            ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [failed_session_list]");
            goto end;
        }

        failed_session_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(failed_session_list_local, failed_session_list) {
            if (!cJSON_IsObject(failed_session_list_local)) {
                ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [failed_session_list]");
                goto end;
            }
            OpenAPI_n2_sm_information_t *failed_session_listItem = OpenAPI_n2_sm_information_parseFromJSON(failed_session_list_local);
            if (!failed_session_listItem) {
                ogs_error("No failed_session_listItem");
                goto end;
            }
            OpenAPI_list_add(failed_session_listList, failed_session_listItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(ue_context_created_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pcf_reselected_ind = cJSON_GetObjectItemCaseSensitive(ue_context_created_dataJSON, "pcfReselectedInd");
    if (pcf_reselected_ind) {
    if (!cJSON_IsBool(pcf_reselected_ind)) {
        ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [pcf_reselected_ind]");
        goto end;
    }
    }

    analytics_not_used_list = cJSON_GetObjectItemCaseSensitive(ue_context_created_dataJSON, "analyticsNotUsedList");
    if (analytics_not_used_list) {
        cJSON *analytics_not_used_list_local = NULL;
        if (!cJSON_IsArray(analytics_not_used_list)) {
            ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [analytics_not_used_list]");
            goto end;
        }

        analytics_not_used_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(analytics_not_used_list_local, analytics_not_used_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(analytics_not_used_list_local)) {
                ogs_error("OpenAPI_ue_context_created_data_parseFromJSON() failed [analytics_not_used_list]");
                goto end;
            }
            OpenAPI_list_add(analytics_not_used_listList, ogs_strdup(analytics_not_used_list_local->valuestring));
        }
    }

    ue_context_created_data_local_var = OpenAPI_ue_context_created_data_create (
        ue_context_local_nonprim,
        target_to_source_data_local_nonprim,
        pdu_session_listList,
        failed_session_list ? failed_session_listList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        pcf_reselected_ind ? true : false,
        pcf_reselected_ind ? pcf_reselected_ind->valueint : 0,
        analytics_not_used_list ? analytics_not_used_listList : NULL
    );

    return ue_context_created_data_local_var;
end:
    if (ue_context_local_nonprim) {
        OpenAPI_ue_context_free(ue_context_local_nonprim);
        ue_context_local_nonprim = NULL;
    }
    if (target_to_source_data_local_nonprim) {
        OpenAPI_n2_info_content_free(target_to_source_data_local_nonprim);
        target_to_source_data_local_nonprim = NULL;
    }
    if (pdu_session_listList) {
        OpenAPI_list_for_each(pdu_session_listList, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(pdu_session_listList);
        pdu_session_listList = NULL;
    }
    if (failed_session_listList) {
        OpenAPI_list_for_each(failed_session_listList, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(failed_session_listList);
        failed_session_listList = NULL;
    }
    if (analytics_not_used_listList) {
        OpenAPI_list_for_each(analytics_not_used_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(analytics_not_used_listList);
        analytics_not_used_listList = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_created_data_t *OpenAPI_ue_context_created_data_copy(OpenAPI_ue_context_created_data_t *dst, OpenAPI_ue_context_created_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_created_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_created_data_convertToJSON() failed");
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

    OpenAPI_ue_context_created_data_free(dst);
    dst = OpenAPI_ue_context_created_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

