
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sec_negotiate_rsp_data.h"

OpenAPI_sec_negotiate_rsp_data_t *OpenAPI_sec_negotiate_rsp_data_create(
    char *sender,
    OpenAPI_security_capability_e selected_sec_capability,
    bool is__3_gpp_sbi_target_api_root_supported,
    int _3_gpp_sbi_target_api_root_supported,
    OpenAPI_list_t *plmn_id_list,
    OpenAPI_list_t *snpn_id_list,
    OpenAPI_list_t *allowed_usage_purpose,
    OpenAPI_list_t *rejected_usage_purpose,
    char *supported_features
)
{
    OpenAPI_sec_negotiate_rsp_data_t *sec_negotiate_rsp_data_local_var = ogs_malloc(sizeof(OpenAPI_sec_negotiate_rsp_data_t));
    ogs_assert(sec_negotiate_rsp_data_local_var);

    sec_negotiate_rsp_data_local_var->sender = sender;
    sec_negotiate_rsp_data_local_var->selected_sec_capability = selected_sec_capability;
    sec_negotiate_rsp_data_local_var->is__3_gpp_sbi_target_api_root_supported = is__3_gpp_sbi_target_api_root_supported;
    sec_negotiate_rsp_data_local_var->_3_gpp_sbi_target_api_root_supported = _3_gpp_sbi_target_api_root_supported;
    sec_negotiate_rsp_data_local_var->plmn_id_list = plmn_id_list;
    sec_negotiate_rsp_data_local_var->snpn_id_list = snpn_id_list;
    sec_negotiate_rsp_data_local_var->allowed_usage_purpose = allowed_usage_purpose;
    sec_negotiate_rsp_data_local_var->rejected_usage_purpose = rejected_usage_purpose;
    sec_negotiate_rsp_data_local_var->supported_features = supported_features;

    return sec_negotiate_rsp_data_local_var;
}

void OpenAPI_sec_negotiate_rsp_data_free(OpenAPI_sec_negotiate_rsp_data_t *sec_negotiate_rsp_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sec_negotiate_rsp_data) {
        return;
    }
    if (sec_negotiate_rsp_data->sender) {
        ogs_free(sec_negotiate_rsp_data->sender);
        sec_negotiate_rsp_data->sender = NULL;
    }
    if (sec_negotiate_rsp_data->plmn_id_list) {
        OpenAPI_list_for_each(sec_negotiate_rsp_data->plmn_id_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(sec_negotiate_rsp_data->plmn_id_list);
        sec_negotiate_rsp_data->plmn_id_list = NULL;
    }
    if (sec_negotiate_rsp_data->snpn_id_list) {
        OpenAPI_list_for_each(sec_negotiate_rsp_data->snpn_id_list, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(sec_negotiate_rsp_data->snpn_id_list);
        sec_negotiate_rsp_data->snpn_id_list = NULL;
    }
    if (sec_negotiate_rsp_data->allowed_usage_purpose) {
        OpenAPI_list_for_each(sec_negotiate_rsp_data->allowed_usage_purpose, node) {
            OpenAPI_intended_n32_purpose_free(node->data);
        }
        OpenAPI_list_free(sec_negotiate_rsp_data->allowed_usage_purpose);
        sec_negotiate_rsp_data->allowed_usage_purpose = NULL;
    }
    if (sec_negotiate_rsp_data->rejected_usage_purpose) {
        OpenAPI_list_for_each(sec_negotiate_rsp_data->rejected_usage_purpose, node) {
            OpenAPI_intended_n32_purpose_free(node->data);
        }
        OpenAPI_list_free(sec_negotiate_rsp_data->rejected_usage_purpose);
        sec_negotiate_rsp_data->rejected_usage_purpose = NULL;
    }
    if (sec_negotiate_rsp_data->supported_features) {
        ogs_free(sec_negotiate_rsp_data->supported_features);
        sec_negotiate_rsp_data->supported_features = NULL;
    }
    ogs_free(sec_negotiate_rsp_data);
}

cJSON *OpenAPI_sec_negotiate_rsp_data_convertToJSON(OpenAPI_sec_negotiate_rsp_data_t *sec_negotiate_rsp_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sec_negotiate_rsp_data == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [SecNegotiateRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sec_negotiate_rsp_data->sender) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [sender]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "sender", sec_negotiate_rsp_data->sender) == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [sender]");
        goto end;
    }

    if (sec_negotiate_rsp_data->selected_sec_capability == OpenAPI_security_capability_NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [selected_sec_capability]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "selectedSecCapability", OpenAPI_security_capability_ToString(sec_negotiate_rsp_data->selected_sec_capability)) == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [selected_sec_capability]");
        goto end;
    }

    if (sec_negotiate_rsp_data->is__3_gpp_sbi_target_api_root_supported) {
    if (cJSON_AddBoolToObject(item, "3GppSbiTargetApiRootSupported", sec_negotiate_rsp_data->_3_gpp_sbi_target_api_root_supported) == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [_3_gpp_sbi_target_api_root_supported]");
        goto end;
    }
    }

    if (sec_negotiate_rsp_data->plmn_id_list) {
    cJSON *plmn_id_listList = cJSON_AddArrayToObject(item, "plmnIdList");
    if (plmn_id_listList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [plmn_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_rsp_data->plmn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [plmn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(plmn_id_listList, itemLocal);
    }
    }

    if (sec_negotiate_rsp_data->snpn_id_list) {
    cJSON *snpn_id_listList = cJSON_AddArrayToObject(item, "snpnIdList");
    if (snpn_id_listList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [snpn_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_rsp_data->snpn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [snpn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(snpn_id_listList, itemLocal);
    }
    }

    if (sec_negotiate_rsp_data->allowed_usage_purpose) {
    cJSON *allowed_usage_purposeList = cJSON_AddArrayToObject(item, "allowedUsagePurpose");
    if (allowed_usage_purposeList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [allowed_usage_purpose]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_rsp_data->allowed_usage_purpose, node) {
        cJSON *itemLocal = OpenAPI_intended_n32_purpose_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [allowed_usage_purpose]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_usage_purposeList, itemLocal);
    }
    }

    if (sec_negotiate_rsp_data->rejected_usage_purpose) {
    cJSON *rejected_usage_purposeList = cJSON_AddArrayToObject(item, "rejectedUsagePurpose");
    if (rejected_usage_purposeList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [rejected_usage_purpose]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_rsp_data->rejected_usage_purpose, node) {
        cJSON *itemLocal = OpenAPI_intended_n32_purpose_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [rejected_usage_purpose]");
            goto end;
        }
        cJSON_AddItemToArray(rejected_usage_purposeList, itemLocal);
    }
    }

    if (sec_negotiate_rsp_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sec_negotiate_rsp_data->supported_features) == NULL) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sec_negotiate_rsp_data_t *OpenAPI_sec_negotiate_rsp_data_parseFromJSON(cJSON *sec_negotiate_rsp_dataJSON)
{
    OpenAPI_sec_negotiate_rsp_data_t *sec_negotiate_rsp_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sender = NULL;
    cJSON *selected_sec_capability = NULL;
    OpenAPI_security_capability_e selected_sec_capabilityVariable = 0;
    cJSON *_3_gpp_sbi_target_api_root_supported = NULL;
    cJSON *plmn_id_list = NULL;
    OpenAPI_list_t *plmn_id_listList = NULL;
    cJSON *snpn_id_list = NULL;
    OpenAPI_list_t *snpn_id_listList = NULL;
    cJSON *allowed_usage_purpose = NULL;
    OpenAPI_list_t *allowed_usage_purposeList = NULL;
    cJSON *rejected_usage_purpose = NULL;
    OpenAPI_list_t *rejected_usage_purposeList = NULL;
    cJSON *supported_features = NULL;
    sender = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "sender");
    if (!sender) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [sender]");
        goto end;
    }
    if (!cJSON_IsString(sender)) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [sender]");
        goto end;
    }

    selected_sec_capability = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "selectedSecCapability");
    if (!selected_sec_capability) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [selected_sec_capability]");
        goto end;
    }
    if (!cJSON_IsString(selected_sec_capability)) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [selected_sec_capability]");
        goto end;
    }
    selected_sec_capabilityVariable = OpenAPI_security_capability_FromString(selected_sec_capability->valuestring);

    _3_gpp_sbi_target_api_root_supported = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "3GppSbiTargetApiRootSupported");
    if (_3_gpp_sbi_target_api_root_supported) {
    if (!cJSON_IsBool(_3_gpp_sbi_target_api_root_supported)) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [_3_gpp_sbi_target_api_root_supported]");
        goto end;
    }
    }

    plmn_id_list = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "plmnIdList");
    if (plmn_id_list) {
        cJSON *plmn_id_list_local = NULL;
        if (!cJSON_IsArray(plmn_id_list)) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [plmn_id_list]");
            goto end;
        }

        plmn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_id_list_local, plmn_id_list) {
            if (!cJSON_IsObject(plmn_id_list_local)) {
                ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [plmn_id_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *plmn_id_listItem = OpenAPI_plmn_id_parseFromJSON(plmn_id_list_local);
            if (!plmn_id_listItem) {
                ogs_error("No plmn_id_listItem");
                goto end;
            }
            OpenAPI_list_add(plmn_id_listList, plmn_id_listItem);
        }
    }

    snpn_id_list = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "snpnIdList");
    if (snpn_id_list) {
        cJSON *snpn_id_list_local = NULL;
        if (!cJSON_IsArray(snpn_id_list)) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [snpn_id_list]");
            goto end;
        }

        snpn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snpn_id_list_local, snpn_id_list) {
            if (!cJSON_IsObject(snpn_id_list_local)) {
                ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [snpn_id_list]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *snpn_id_listItem = OpenAPI_plmn_id_nid_parseFromJSON(snpn_id_list_local);
            if (!snpn_id_listItem) {
                ogs_error("No snpn_id_listItem");
                goto end;
            }
            OpenAPI_list_add(snpn_id_listList, snpn_id_listItem);
        }
    }

    allowed_usage_purpose = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "allowedUsagePurpose");
    if (allowed_usage_purpose) {
        cJSON *allowed_usage_purpose_local = NULL;
        if (!cJSON_IsArray(allowed_usage_purpose)) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [allowed_usage_purpose]");
            goto end;
        }

        allowed_usage_purposeList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_usage_purpose_local, allowed_usage_purpose) {
            if (!cJSON_IsObject(allowed_usage_purpose_local)) {
                ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [allowed_usage_purpose]");
                goto end;
            }
            OpenAPI_intended_n32_purpose_t *allowed_usage_purposeItem = OpenAPI_intended_n32_purpose_parseFromJSON(allowed_usage_purpose_local);
            if (!allowed_usage_purposeItem) {
                ogs_error("No allowed_usage_purposeItem");
                goto end;
            }
            OpenAPI_list_add(allowed_usage_purposeList, allowed_usage_purposeItem);
        }
    }

    rejected_usage_purpose = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "rejectedUsagePurpose");
    if (rejected_usage_purpose) {
        cJSON *rejected_usage_purpose_local = NULL;
        if (!cJSON_IsArray(rejected_usage_purpose)) {
            ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [rejected_usage_purpose]");
            goto end;
        }

        rejected_usage_purposeList = OpenAPI_list_create();

        cJSON_ArrayForEach(rejected_usage_purpose_local, rejected_usage_purpose) {
            if (!cJSON_IsObject(rejected_usage_purpose_local)) {
                ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [rejected_usage_purpose]");
                goto end;
            }
            OpenAPI_intended_n32_purpose_t *rejected_usage_purposeItem = OpenAPI_intended_n32_purpose_parseFromJSON(rejected_usage_purpose_local);
            if (!rejected_usage_purposeItem) {
                ogs_error("No rejected_usage_purposeItem");
                goto end;
            }
            OpenAPI_list_add(rejected_usage_purposeList, rejected_usage_purposeItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sec_negotiate_rsp_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    sec_negotiate_rsp_data_local_var = OpenAPI_sec_negotiate_rsp_data_create (
        ogs_strdup(sender->valuestring),
        selected_sec_capabilityVariable,
        _3_gpp_sbi_target_api_root_supported ? true : false,
        _3_gpp_sbi_target_api_root_supported ? _3_gpp_sbi_target_api_root_supported->valueint : 0,
        plmn_id_list ? plmn_id_listList : NULL,
        snpn_id_list ? snpn_id_listList : NULL,
        allowed_usage_purpose ? allowed_usage_purposeList : NULL,
        rejected_usage_purpose ? rejected_usage_purposeList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return sec_negotiate_rsp_data_local_var;
end:
    if (plmn_id_listList) {
        OpenAPI_list_for_each(plmn_id_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(plmn_id_listList);
        plmn_id_listList = NULL;
    }
    if (snpn_id_listList) {
        OpenAPI_list_for_each(snpn_id_listList, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(snpn_id_listList);
        snpn_id_listList = NULL;
    }
    if (allowed_usage_purposeList) {
        OpenAPI_list_for_each(allowed_usage_purposeList, node) {
            OpenAPI_intended_n32_purpose_free(node->data);
        }
        OpenAPI_list_free(allowed_usage_purposeList);
        allowed_usage_purposeList = NULL;
    }
    if (rejected_usage_purposeList) {
        OpenAPI_list_for_each(rejected_usage_purposeList, node) {
            OpenAPI_intended_n32_purpose_free(node->data);
        }
        OpenAPI_list_free(rejected_usage_purposeList);
        rejected_usage_purposeList = NULL;
    }
    return NULL;
}

OpenAPI_sec_negotiate_rsp_data_t *OpenAPI_sec_negotiate_rsp_data_copy(OpenAPI_sec_negotiate_rsp_data_t *dst, OpenAPI_sec_negotiate_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sec_negotiate_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sec_negotiate_rsp_data_convertToJSON() failed");
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

    OpenAPI_sec_negotiate_rsp_data_free(dst);
    dst = OpenAPI_sec_negotiate_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

