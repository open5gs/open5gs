
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sec_param_exch_rsp_data.h"

OpenAPI_sec_param_exch_rsp_data_t *OpenAPI_sec_param_exch_rsp_data_create(
    char *n32f_context_id,
    char *selected_jwe_cipher_suite,
    char *selected_jws_cipher_suite,
    OpenAPI_protection_policy_t *sel_protection_policy_info,
    OpenAPI_list_t *ipx_provider_sec_info_list,
    char *sender
)
{
    OpenAPI_sec_param_exch_rsp_data_t *sec_param_exch_rsp_data_local_var = ogs_malloc(sizeof(OpenAPI_sec_param_exch_rsp_data_t));
    ogs_assert(sec_param_exch_rsp_data_local_var);

    sec_param_exch_rsp_data_local_var->n32f_context_id = n32f_context_id;
    sec_param_exch_rsp_data_local_var->selected_jwe_cipher_suite = selected_jwe_cipher_suite;
    sec_param_exch_rsp_data_local_var->selected_jws_cipher_suite = selected_jws_cipher_suite;
    sec_param_exch_rsp_data_local_var->sel_protection_policy_info = sel_protection_policy_info;
    sec_param_exch_rsp_data_local_var->ipx_provider_sec_info_list = ipx_provider_sec_info_list;
    sec_param_exch_rsp_data_local_var->sender = sender;

    return sec_param_exch_rsp_data_local_var;
}

void OpenAPI_sec_param_exch_rsp_data_free(OpenAPI_sec_param_exch_rsp_data_t *sec_param_exch_rsp_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sec_param_exch_rsp_data) {
        return;
    }
    if (sec_param_exch_rsp_data->n32f_context_id) {
        ogs_free(sec_param_exch_rsp_data->n32f_context_id);
        sec_param_exch_rsp_data->n32f_context_id = NULL;
    }
    if (sec_param_exch_rsp_data->selected_jwe_cipher_suite) {
        ogs_free(sec_param_exch_rsp_data->selected_jwe_cipher_suite);
        sec_param_exch_rsp_data->selected_jwe_cipher_suite = NULL;
    }
    if (sec_param_exch_rsp_data->selected_jws_cipher_suite) {
        ogs_free(sec_param_exch_rsp_data->selected_jws_cipher_suite);
        sec_param_exch_rsp_data->selected_jws_cipher_suite = NULL;
    }
    if (sec_param_exch_rsp_data->sel_protection_policy_info) {
        OpenAPI_protection_policy_free(sec_param_exch_rsp_data->sel_protection_policy_info);
        sec_param_exch_rsp_data->sel_protection_policy_info = NULL;
    }
    if (sec_param_exch_rsp_data->ipx_provider_sec_info_list) {
        OpenAPI_list_for_each(sec_param_exch_rsp_data->ipx_provider_sec_info_list, node) {
            OpenAPI_ipx_provider_sec_info_free(node->data);
        }
        OpenAPI_list_free(sec_param_exch_rsp_data->ipx_provider_sec_info_list);
        sec_param_exch_rsp_data->ipx_provider_sec_info_list = NULL;
    }
    if (sec_param_exch_rsp_data->sender) {
        ogs_free(sec_param_exch_rsp_data->sender);
        sec_param_exch_rsp_data->sender = NULL;
    }
    ogs_free(sec_param_exch_rsp_data);
}

cJSON *OpenAPI_sec_param_exch_rsp_data_convertToJSON(OpenAPI_sec_param_exch_rsp_data_t *sec_param_exch_rsp_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sec_param_exch_rsp_data == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [SecParamExchRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sec_param_exch_rsp_data->n32f_context_id) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [n32f_context_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n32fContextId", sec_param_exch_rsp_data->n32f_context_id) == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [n32f_context_id]");
        goto end;
    }

    if (sec_param_exch_rsp_data->selected_jwe_cipher_suite) {
    if (cJSON_AddStringToObject(item, "selectedJweCipherSuite", sec_param_exch_rsp_data->selected_jwe_cipher_suite) == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [selected_jwe_cipher_suite]");
        goto end;
    }
    }

    if (sec_param_exch_rsp_data->selected_jws_cipher_suite) {
    if (cJSON_AddStringToObject(item, "selectedJwsCipherSuite", sec_param_exch_rsp_data->selected_jws_cipher_suite) == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [selected_jws_cipher_suite]");
        goto end;
    }
    }

    if (sec_param_exch_rsp_data->sel_protection_policy_info) {
    cJSON *sel_protection_policy_info_local_JSON = OpenAPI_protection_policy_convertToJSON(sec_param_exch_rsp_data->sel_protection_policy_info);
    if (sel_protection_policy_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [sel_protection_policy_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "selProtectionPolicyInfo", sel_protection_policy_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [sel_protection_policy_info]");
        goto end;
    }
    }

    if (sec_param_exch_rsp_data->ipx_provider_sec_info_list) {
    cJSON *ipx_provider_sec_info_listList = cJSON_AddArrayToObject(item, "ipxProviderSecInfoList");
    if (ipx_provider_sec_info_listList == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [ipx_provider_sec_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_param_exch_rsp_data->ipx_provider_sec_info_list, node) {
        cJSON *itemLocal = OpenAPI_ipx_provider_sec_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [ipx_provider_sec_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipx_provider_sec_info_listList, itemLocal);
    }
    }

    if (sec_param_exch_rsp_data->sender) {
    if (cJSON_AddStringToObject(item, "sender", sec_param_exch_rsp_data->sender) == NULL) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed [sender]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sec_param_exch_rsp_data_t *OpenAPI_sec_param_exch_rsp_data_parseFromJSON(cJSON *sec_param_exch_rsp_dataJSON)
{
    OpenAPI_sec_param_exch_rsp_data_t *sec_param_exch_rsp_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n32f_context_id = NULL;
    cJSON *selected_jwe_cipher_suite = NULL;
    cJSON *selected_jws_cipher_suite = NULL;
    cJSON *sel_protection_policy_info = NULL;
    OpenAPI_protection_policy_t *sel_protection_policy_info_local_nonprim = NULL;
    cJSON *ipx_provider_sec_info_list = NULL;
    OpenAPI_list_t *ipx_provider_sec_info_listList = NULL;
    cJSON *sender = NULL;
    n32f_context_id = cJSON_GetObjectItemCaseSensitive(sec_param_exch_rsp_dataJSON, "n32fContextId");
    if (!n32f_context_id) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_parseFromJSON() failed [n32f_context_id]");
        goto end;
    }
    if (!cJSON_IsString(n32f_context_id)) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_parseFromJSON() failed [n32f_context_id]");
        goto end;
    }

    selected_jwe_cipher_suite = cJSON_GetObjectItemCaseSensitive(sec_param_exch_rsp_dataJSON, "selectedJweCipherSuite");
    if (selected_jwe_cipher_suite) {
    if (!cJSON_IsString(selected_jwe_cipher_suite) && !cJSON_IsNull(selected_jwe_cipher_suite)) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_parseFromJSON() failed [selected_jwe_cipher_suite]");
        goto end;
    }
    }

    selected_jws_cipher_suite = cJSON_GetObjectItemCaseSensitive(sec_param_exch_rsp_dataJSON, "selectedJwsCipherSuite");
    if (selected_jws_cipher_suite) {
    if (!cJSON_IsString(selected_jws_cipher_suite) && !cJSON_IsNull(selected_jws_cipher_suite)) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_parseFromJSON() failed [selected_jws_cipher_suite]");
        goto end;
    }
    }

    sel_protection_policy_info = cJSON_GetObjectItemCaseSensitive(sec_param_exch_rsp_dataJSON, "selProtectionPolicyInfo");
    if (sel_protection_policy_info) {
    sel_protection_policy_info_local_nonprim = OpenAPI_protection_policy_parseFromJSON(sel_protection_policy_info);
    if (!sel_protection_policy_info_local_nonprim) {
        ogs_error("OpenAPI_protection_policy_parseFromJSON failed [sel_protection_policy_info]");
        goto end;
    }
    }

    ipx_provider_sec_info_list = cJSON_GetObjectItemCaseSensitive(sec_param_exch_rsp_dataJSON, "ipxProviderSecInfoList");
    if (ipx_provider_sec_info_list) {
        cJSON *ipx_provider_sec_info_list_local = NULL;
        if (!cJSON_IsArray(ipx_provider_sec_info_list)) {
            ogs_error("OpenAPI_sec_param_exch_rsp_data_parseFromJSON() failed [ipx_provider_sec_info_list]");
            goto end;
        }

        ipx_provider_sec_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipx_provider_sec_info_list_local, ipx_provider_sec_info_list) {
            if (!cJSON_IsObject(ipx_provider_sec_info_list_local)) {
                ogs_error("OpenAPI_sec_param_exch_rsp_data_parseFromJSON() failed [ipx_provider_sec_info_list]");
                goto end;
            }
            OpenAPI_ipx_provider_sec_info_t *ipx_provider_sec_info_listItem = OpenAPI_ipx_provider_sec_info_parseFromJSON(ipx_provider_sec_info_list_local);
            if (!ipx_provider_sec_info_listItem) {
                ogs_error("No ipx_provider_sec_info_listItem");
                goto end;
            }
            OpenAPI_list_add(ipx_provider_sec_info_listList, ipx_provider_sec_info_listItem);
        }
    }

    sender = cJSON_GetObjectItemCaseSensitive(sec_param_exch_rsp_dataJSON, "sender");
    if (sender) {
    if (!cJSON_IsString(sender) && !cJSON_IsNull(sender)) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_parseFromJSON() failed [sender]");
        goto end;
    }
    }

    sec_param_exch_rsp_data_local_var = OpenAPI_sec_param_exch_rsp_data_create (
        ogs_strdup(n32f_context_id->valuestring),
        selected_jwe_cipher_suite && !cJSON_IsNull(selected_jwe_cipher_suite) ? ogs_strdup(selected_jwe_cipher_suite->valuestring) : NULL,
        selected_jws_cipher_suite && !cJSON_IsNull(selected_jws_cipher_suite) ? ogs_strdup(selected_jws_cipher_suite->valuestring) : NULL,
        sel_protection_policy_info ? sel_protection_policy_info_local_nonprim : NULL,
        ipx_provider_sec_info_list ? ipx_provider_sec_info_listList : NULL,
        sender && !cJSON_IsNull(sender) ? ogs_strdup(sender->valuestring) : NULL
    );

    return sec_param_exch_rsp_data_local_var;
end:
    if (sel_protection_policy_info_local_nonprim) {
        OpenAPI_protection_policy_free(sel_protection_policy_info_local_nonprim);
        sel_protection_policy_info_local_nonprim = NULL;
    }
    if (ipx_provider_sec_info_listList) {
        OpenAPI_list_for_each(ipx_provider_sec_info_listList, node) {
            OpenAPI_ipx_provider_sec_info_free(node->data);
        }
        OpenAPI_list_free(ipx_provider_sec_info_listList);
        ipx_provider_sec_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_sec_param_exch_rsp_data_t *OpenAPI_sec_param_exch_rsp_data_copy(OpenAPI_sec_param_exch_rsp_data_t *dst, OpenAPI_sec_param_exch_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sec_param_exch_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sec_param_exch_rsp_data_convertToJSON() failed");
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

    OpenAPI_sec_param_exch_rsp_data_free(dst);
    dst = OpenAPI_sec_param_exch_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

