
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sec_negotiate_req_data.h"

OpenAPI_sec_negotiate_req_data_t *OpenAPI_sec_negotiate_req_data_create(
    char *sender,
    OpenAPI_list_t *supported_sec_capability_list,
    bool is__3_gpp_sbi_target_api_root_supported,
    int _3_gpp_sbi_target_api_root_supported,
    OpenAPI_list_t *plmn_id_list,
    OpenAPI_list_t *snpn_id_list,
    OpenAPI_plmn_id_t *target_plmn_id,
    OpenAPI_plmn_id_nid_t *target_snpn_id,
    OpenAPI_list_t *intended_usage_purpose,
    char *supported_features
)
{
    OpenAPI_sec_negotiate_req_data_t *sec_negotiate_req_data_local_var = ogs_malloc(sizeof(OpenAPI_sec_negotiate_req_data_t));
    ogs_assert(sec_negotiate_req_data_local_var);

    sec_negotiate_req_data_local_var->sender = sender;
    sec_negotiate_req_data_local_var->supported_sec_capability_list = supported_sec_capability_list;
    sec_negotiate_req_data_local_var->is__3_gpp_sbi_target_api_root_supported = is__3_gpp_sbi_target_api_root_supported;
    sec_negotiate_req_data_local_var->_3_gpp_sbi_target_api_root_supported = _3_gpp_sbi_target_api_root_supported;
    sec_negotiate_req_data_local_var->plmn_id_list = plmn_id_list;
    sec_negotiate_req_data_local_var->snpn_id_list = snpn_id_list;
    sec_negotiate_req_data_local_var->target_plmn_id = target_plmn_id;
    sec_negotiate_req_data_local_var->target_snpn_id = target_snpn_id;
    sec_negotiate_req_data_local_var->intended_usage_purpose = intended_usage_purpose;
    sec_negotiate_req_data_local_var->supported_features = supported_features;

    return sec_negotiate_req_data_local_var;
}

void OpenAPI_sec_negotiate_req_data_free(OpenAPI_sec_negotiate_req_data_t *sec_negotiate_req_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sec_negotiate_req_data) {
        return;
    }
    if (sec_negotiate_req_data->sender) {
        ogs_free(sec_negotiate_req_data->sender);
        sec_negotiate_req_data->sender = NULL;
    }
    if (sec_negotiate_req_data->supported_sec_capability_list) {
        OpenAPI_list_free(sec_negotiate_req_data->supported_sec_capability_list);
        sec_negotiate_req_data->supported_sec_capability_list = NULL;
    }
    if (sec_negotiate_req_data->plmn_id_list) {
        OpenAPI_list_for_each(sec_negotiate_req_data->plmn_id_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(sec_negotiate_req_data->plmn_id_list);
        sec_negotiate_req_data->plmn_id_list = NULL;
    }
    if (sec_negotiate_req_data->snpn_id_list) {
        OpenAPI_list_for_each(sec_negotiate_req_data->snpn_id_list, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(sec_negotiate_req_data->snpn_id_list);
        sec_negotiate_req_data->snpn_id_list = NULL;
    }
    if (sec_negotiate_req_data->target_plmn_id) {
        OpenAPI_plmn_id_free(sec_negotiate_req_data->target_plmn_id);
        sec_negotiate_req_data->target_plmn_id = NULL;
    }
    if (sec_negotiate_req_data->target_snpn_id) {
        OpenAPI_plmn_id_nid_free(sec_negotiate_req_data->target_snpn_id);
        sec_negotiate_req_data->target_snpn_id = NULL;
    }
    if (sec_negotiate_req_data->intended_usage_purpose) {
        OpenAPI_list_for_each(sec_negotiate_req_data->intended_usage_purpose, node) {
            OpenAPI_intended_n32_purpose_free(node->data);
        }
        OpenAPI_list_free(sec_negotiate_req_data->intended_usage_purpose);
        sec_negotiate_req_data->intended_usage_purpose = NULL;
    }
    if (sec_negotiate_req_data->supported_features) {
        ogs_free(sec_negotiate_req_data->supported_features);
        sec_negotiate_req_data->supported_features = NULL;
    }
    ogs_free(sec_negotiate_req_data);
}

cJSON *OpenAPI_sec_negotiate_req_data_convertToJSON(OpenAPI_sec_negotiate_req_data_t *sec_negotiate_req_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sec_negotiate_req_data == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [SecNegotiateReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sec_negotiate_req_data->sender) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [sender]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "sender", sec_negotiate_req_data->sender) == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [sender]");
        goto end;
    }

    if (sec_negotiate_req_data->supported_sec_capability_list == OpenAPI_security_capability_NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [supported_sec_capability_list]");
        return NULL;
    }
    cJSON *supported_sec_capability_listList = cJSON_AddArrayToObject(item, "supportedSecCapabilityList");
    if (supported_sec_capability_listList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [supported_sec_capability_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_req_data->supported_sec_capability_list, node) {
        if (cJSON_AddStringToObject(supported_sec_capability_listList, "", OpenAPI_security_capability_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [supported_sec_capability_list]");
            goto end;
        }
    }

    if (sec_negotiate_req_data->is__3_gpp_sbi_target_api_root_supported) {
    if (cJSON_AddBoolToObject(item, "3GppSbiTargetApiRootSupported", sec_negotiate_req_data->_3_gpp_sbi_target_api_root_supported) == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [_3_gpp_sbi_target_api_root_supported]");
        goto end;
    }
    }

    if (sec_negotiate_req_data->plmn_id_list) {
    cJSON *plmn_id_listList = cJSON_AddArrayToObject(item, "plmnIdList");
    if (plmn_id_listList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [plmn_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_req_data->plmn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [plmn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(plmn_id_listList, itemLocal);
    }
    }

    if (sec_negotiate_req_data->snpn_id_list) {
    cJSON *snpn_id_listList = cJSON_AddArrayToObject(item, "snpnIdList");
    if (snpn_id_listList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [snpn_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_req_data->snpn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [snpn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(snpn_id_listList, itemLocal);
    }
    }

    if (sec_negotiate_req_data->target_plmn_id) {
    cJSON *target_plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(sec_negotiate_req_data->target_plmn_id);
    if (target_plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [target_plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetPlmnId", target_plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [target_plmn_id]");
        goto end;
    }
    }

    if (sec_negotiate_req_data->target_snpn_id) {
    cJSON *target_snpn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sec_negotiate_req_data->target_snpn_id);
    if (target_snpn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [target_snpn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetSnpnId", target_snpn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [target_snpn_id]");
        goto end;
    }
    }

    if (sec_negotiate_req_data->intended_usage_purpose) {
    cJSON *intended_usage_purposeList = cJSON_AddArrayToObject(item, "intendedUsagePurpose");
    if (intended_usage_purposeList == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [intended_usage_purpose]");
        goto end;
    }
    OpenAPI_list_for_each(sec_negotiate_req_data->intended_usage_purpose, node) {
        cJSON *itemLocal = OpenAPI_intended_n32_purpose_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [intended_usage_purpose]");
            goto end;
        }
        cJSON_AddItemToArray(intended_usage_purposeList, itemLocal);
    }
    }

    if (sec_negotiate_req_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sec_negotiate_req_data->supported_features) == NULL) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sec_negotiate_req_data_t *OpenAPI_sec_negotiate_req_data_parseFromJSON(cJSON *sec_negotiate_req_dataJSON)
{
    OpenAPI_sec_negotiate_req_data_t *sec_negotiate_req_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sender = NULL;
    cJSON *supported_sec_capability_list = NULL;
    OpenAPI_list_t *supported_sec_capability_listList = NULL;
    cJSON *_3_gpp_sbi_target_api_root_supported = NULL;
    cJSON *plmn_id_list = NULL;
    OpenAPI_list_t *plmn_id_listList = NULL;
    cJSON *snpn_id_list = NULL;
    OpenAPI_list_t *snpn_id_listList = NULL;
    cJSON *target_plmn_id = NULL;
    OpenAPI_plmn_id_t *target_plmn_id_local_nonprim = NULL;
    cJSON *target_snpn_id = NULL;
    OpenAPI_plmn_id_nid_t *target_snpn_id_local_nonprim = NULL;
    cJSON *intended_usage_purpose = NULL;
    OpenAPI_list_t *intended_usage_purposeList = NULL;
    cJSON *supported_features = NULL;
    sender = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "sender");
    if (!sender) {
        ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [sender]");
        goto end;
    }
    if (!cJSON_IsString(sender)) {
        ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [sender]");
        goto end;
    }

    supported_sec_capability_list = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "supportedSecCapabilityList");
    if (!supported_sec_capability_list) {
        ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [supported_sec_capability_list]");
        goto end;
    }
        cJSON *supported_sec_capability_list_local = NULL;
        if (!cJSON_IsArray(supported_sec_capability_list)) {
            ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [supported_sec_capability_list]");
            goto end;
        }

        supported_sec_capability_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_sec_capability_list_local, supported_sec_capability_list) {
            OpenAPI_security_capability_e localEnum = OpenAPI_security_capability_NULL;
            if (!cJSON_IsString(supported_sec_capability_list_local)) {
                ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [supported_sec_capability_list]");
                goto end;
            }
            localEnum = OpenAPI_security_capability_FromString(supported_sec_capability_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"supported_sec_capability_list\" is not supported. Ignoring it ...",
                         supported_sec_capability_list_local->valuestring);
            } else {
                OpenAPI_list_add(supported_sec_capability_listList, (void *)localEnum);
            }
        }
        if (supported_sec_capability_listList->count == 0) {
            ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed: Expected supported_sec_capability_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    _3_gpp_sbi_target_api_root_supported = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "3GppSbiTargetApiRootSupported");
    if (_3_gpp_sbi_target_api_root_supported) {
    if (!cJSON_IsBool(_3_gpp_sbi_target_api_root_supported)) {
        ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [_3_gpp_sbi_target_api_root_supported]");
        goto end;
    }
    }

    plmn_id_list = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "plmnIdList");
    if (plmn_id_list) {
        cJSON *plmn_id_list_local = NULL;
        if (!cJSON_IsArray(plmn_id_list)) {
            ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [plmn_id_list]");
            goto end;
        }

        plmn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_id_list_local, plmn_id_list) {
            if (!cJSON_IsObject(plmn_id_list_local)) {
                ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [plmn_id_list]");
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

    snpn_id_list = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "snpnIdList");
    if (snpn_id_list) {
        cJSON *snpn_id_list_local = NULL;
        if (!cJSON_IsArray(snpn_id_list)) {
            ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [snpn_id_list]");
            goto end;
        }

        snpn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snpn_id_list_local, snpn_id_list) {
            if (!cJSON_IsObject(snpn_id_list_local)) {
                ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [snpn_id_list]");
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

    target_plmn_id = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "targetPlmnId");
    if (target_plmn_id) {
    target_plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(target_plmn_id);
    if (!target_plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [target_plmn_id]");
        goto end;
    }
    }

    target_snpn_id = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "targetSnpnId");
    if (target_snpn_id) {
    target_snpn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(target_snpn_id);
    if (!target_snpn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [target_snpn_id]");
        goto end;
    }
    }

    intended_usage_purpose = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "intendedUsagePurpose");
    if (intended_usage_purpose) {
        cJSON *intended_usage_purpose_local = NULL;
        if (!cJSON_IsArray(intended_usage_purpose)) {
            ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [intended_usage_purpose]");
            goto end;
        }

        intended_usage_purposeList = OpenAPI_list_create();

        cJSON_ArrayForEach(intended_usage_purpose_local, intended_usage_purpose) {
            if (!cJSON_IsObject(intended_usage_purpose_local)) {
                ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [intended_usage_purpose]");
                goto end;
            }
            OpenAPI_intended_n32_purpose_t *intended_usage_purposeItem = OpenAPI_intended_n32_purpose_parseFromJSON(intended_usage_purpose_local);
            if (!intended_usage_purposeItem) {
                ogs_error("No intended_usage_purposeItem");
                goto end;
            }
            OpenAPI_list_add(intended_usage_purposeList, intended_usage_purposeItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sec_negotiate_req_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sec_negotiate_req_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    sec_negotiate_req_data_local_var = OpenAPI_sec_negotiate_req_data_create (
        ogs_strdup(sender->valuestring),
        supported_sec_capability_listList,
        _3_gpp_sbi_target_api_root_supported ? true : false,
        _3_gpp_sbi_target_api_root_supported ? _3_gpp_sbi_target_api_root_supported->valueint : 0,
        plmn_id_list ? plmn_id_listList : NULL,
        snpn_id_list ? snpn_id_listList : NULL,
        target_plmn_id ? target_plmn_id_local_nonprim : NULL,
        target_snpn_id ? target_snpn_id_local_nonprim : NULL,
        intended_usage_purpose ? intended_usage_purposeList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return sec_negotiate_req_data_local_var;
end:
    if (supported_sec_capability_listList) {
        OpenAPI_list_free(supported_sec_capability_listList);
        supported_sec_capability_listList = NULL;
    }
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
    if (target_plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(target_plmn_id_local_nonprim);
        target_plmn_id_local_nonprim = NULL;
    }
    if (target_snpn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_free(target_snpn_id_local_nonprim);
        target_snpn_id_local_nonprim = NULL;
    }
    if (intended_usage_purposeList) {
        OpenAPI_list_for_each(intended_usage_purposeList, node) {
            OpenAPI_intended_n32_purpose_free(node->data);
        }
        OpenAPI_list_free(intended_usage_purposeList);
        intended_usage_purposeList = NULL;
    }
    return NULL;
}

OpenAPI_sec_negotiate_req_data_t *OpenAPI_sec_negotiate_req_data_copy(OpenAPI_sec_negotiate_req_data_t *dst, OpenAPI_sec_negotiate_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sec_negotiate_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sec_negotiate_req_data_convertToJSON() failed");
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

    OpenAPI_sec_negotiate_req_data_free(dst);
    dst = OpenAPI_sec_negotiate_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

