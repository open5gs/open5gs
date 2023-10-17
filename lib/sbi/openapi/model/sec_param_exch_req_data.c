
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sec_param_exch_req_data.h"

OpenAPI_sec_param_exch_req_data_t *OpenAPI_sec_param_exch_req_data_create(
    char *n32f_context_id,
    OpenAPI_list_t *jwe_cipher_suite_list,
    OpenAPI_list_t *jws_cipher_suite_list,
    OpenAPI_protection_policy_t *protection_policy_info,
    OpenAPI_list_t *ipx_provider_sec_info_list,
    char *sender
)
{
    OpenAPI_sec_param_exch_req_data_t *sec_param_exch_req_data_local_var = ogs_malloc(sizeof(OpenAPI_sec_param_exch_req_data_t));
    ogs_assert(sec_param_exch_req_data_local_var);

    sec_param_exch_req_data_local_var->n32f_context_id = n32f_context_id;
    sec_param_exch_req_data_local_var->jwe_cipher_suite_list = jwe_cipher_suite_list;
    sec_param_exch_req_data_local_var->jws_cipher_suite_list = jws_cipher_suite_list;
    sec_param_exch_req_data_local_var->protection_policy_info = protection_policy_info;
    sec_param_exch_req_data_local_var->ipx_provider_sec_info_list = ipx_provider_sec_info_list;
    sec_param_exch_req_data_local_var->sender = sender;

    return sec_param_exch_req_data_local_var;
}

void OpenAPI_sec_param_exch_req_data_free(OpenAPI_sec_param_exch_req_data_t *sec_param_exch_req_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sec_param_exch_req_data) {
        return;
    }
    if (sec_param_exch_req_data->n32f_context_id) {
        ogs_free(sec_param_exch_req_data->n32f_context_id);
        sec_param_exch_req_data->n32f_context_id = NULL;
    }
    if (sec_param_exch_req_data->jwe_cipher_suite_list) {
        OpenAPI_list_for_each(sec_param_exch_req_data->jwe_cipher_suite_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sec_param_exch_req_data->jwe_cipher_suite_list);
        sec_param_exch_req_data->jwe_cipher_suite_list = NULL;
    }
    if (sec_param_exch_req_data->jws_cipher_suite_list) {
        OpenAPI_list_for_each(sec_param_exch_req_data->jws_cipher_suite_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sec_param_exch_req_data->jws_cipher_suite_list);
        sec_param_exch_req_data->jws_cipher_suite_list = NULL;
    }
    if (sec_param_exch_req_data->protection_policy_info) {
        OpenAPI_protection_policy_free(sec_param_exch_req_data->protection_policy_info);
        sec_param_exch_req_data->protection_policy_info = NULL;
    }
    if (sec_param_exch_req_data->ipx_provider_sec_info_list) {
        OpenAPI_list_for_each(sec_param_exch_req_data->ipx_provider_sec_info_list, node) {
            OpenAPI_ipx_provider_sec_info_free(node->data);
        }
        OpenAPI_list_free(sec_param_exch_req_data->ipx_provider_sec_info_list);
        sec_param_exch_req_data->ipx_provider_sec_info_list = NULL;
    }
    if (sec_param_exch_req_data->sender) {
        ogs_free(sec_param_exch_req_data->sender);
        sec_param_exch_req_data->sender = NULL;
    }
    ogs_free(sec_param_exch_req_data);
}

cJSON *OpenAPI_sec_param_exch_req_data_convertToJSON(OpenAPI_sec_param_exch_req_data_t *sec_param_exch_req_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sec_param_exch_req_data == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [SecParamExchReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sec_param_exch_req_data->n32f_context_id) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [n32f_context_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n32fContextId", sec_param_exch_req_data->n32f_context_id) == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [n32f_context_id]");
        goto end;
    }

    if (sec_param_exch_req_data->jwe_cipher_suite_list) {
    cJSON *jwe_cipher_suite_listList = cJSON_AddArrayToObject(item, "jweCipherSuiteList");
    if (jwe_cipher_suite_listList == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [jwe_cipher_suite_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_param_exch_req_data->jwe_cipher_suite_list, node) {
        if (cJSON_AddStringToObject(jwe_cipher_suite_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [jwe_cipher_suite_list]");
            goto end;
        }
    }
    }

    if (sec_param_exch_req_data->jws_cipher_suite_list) {
    cJSON *jws_cipher_suite_listList = cJSON_AddArrayToObject(item, "jwsCipherSuiteList");
    if (jws_cipher_suite_listList == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [jws_cipher_suite_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_param_exch_req_data->jws_cipher_suite_list, node) {
        if (cJSON_AddStringToObject(jws_cipher_suite_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [jws_cipher_suite_list]");
            goto end;
        }
    }
    }

    if (sec_param_exch_req_data->protection_policy_info) {
    cJSON *protection_policy_info_local_JSON = OpenAPI_protection_policy_convertToJSON(sec_param_exch_req_data->protection_policy_info);
    if (protection_policy_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [protection_policy_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "protectionPolicyInfo", protection_policy_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [protection_policy_info]");
        goto end;
    }
    }

    if (sec_param_exch_req_data->ipx_provider_sec_info_list) {
    cJSON *ipx_provider_sec_info_listList = cJSON_AddArrayToObject(item, "ipxProviderSecInfoList");
    if (ipx_provider_sec_info_listList == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [ipx_provider_sec_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(sec_param_exch_req_data->ipx_provider_sec_info_list, node) {
        cJSON *itemLocal = OpenAPI_ipx_provider_sec_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [ipx_provider_sec_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipx_provider_sec_info_listList, itemLocal);
    }
    }

    if (sec_param_exch_req_data->sender) {
    if (cJSON_AddStringToObject(item, "sender", sec_param_exch_req_data->sender) == NULL) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed [sender]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sec_param_exch_req_data_t *OpenAPI_sec_param_exch_req_data_parseFromJSON(cJSON *sec_param_exch_req_dataJSON)
{
    OpenAPI_sec_param_exch_req_data_t *sec_param_exch_req_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n32f_context_id = NULL;
    cJSON *jwe_cipher_suite_list = NULL;
    OpenAPI_list_t *jwe_cipher_suite_listList = NULL;
    cJSON *jws_cipher_suite_list = NULL;
    OpenAPI_list_t *jws_cipher_suite_listList = NULL;
    cJSON *protection_policy_info = NULL;
    OpenAPI_protection_policy_t *protection_policy_info_local_nonprim = NULL;
    cJSON *ipx_provider_sec_info_list = NULL;
    OpenAPI_list_t *ipx_provider_sec_info_listList = NULL;
    cJSON *sender = NULL;
    n32f_context_id = cJSON_GetObjectItemCaseSensitive(sec_param_exch_req_dataJSON, "n32fContextId");
    if (!n32f_context_id) {
        ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [n32f_context_id]");
        goto end;
    }
    if (!cJSON_IsString(n32f_context_id)) {
        ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [n32f_context_id]");
        goto end;
    }

    jwe_cipher_suite_list = cJSON_GetObjectItemCaseSensitive(sec_param_exch_req_dataJSON, "jweCipherSuiteList");
    if (jwe_cipher_suite_list) {
        cJSON *jwe_cipher_suite_list_local = NULL;
        if (!cJSON_IsArray(jwe_cipher_suite_list)) {
            ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [jwe_cipher_suite_list]");
            goto end;
        }

        jwe_cipher_suite_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(jwe_cipher_suite_list_local, jwe_cipher_suite_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(jwe_cipher_suite_list_local)) {
                ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [jwe_cipher_suite_list]");
                goto end;
            }
            OpenAPI_list_add(jwe_cipher_suite_listList, ogs_strdup(jwe_cipher_suite_list_local->valuestring));
        }
    }

    jws_cipher_suite_list = cJSON_GetObjectItemCaseSensitive(sec_param_exch_req_dataJSON, "jwsCipherSuiteList");
    if (jws_cipher_suite_list) {
        cJSON *jws_cipher_suite_list_local = NULL;
        if (!cJSON_IsArray(jws_cipher_suite_list)) {
            ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [jws_cipher_suite_list]");
            goto end;
        }

        jws_cipher_suite_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(jws_cipher_suite_list_local, jws_cipher_suite_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(jws_cipher_suite_list_local)) {
                ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [jws_cipher_suite_list]");
                goto end;
            }
            OpenAPI_list_add(jws_cipher_suite_listList, ogs_strdup(jws_cipher_suite_list_local->valuestring));
        }
    }

    protection_policy_info = cJSON_GetObjectItemCaseSensitive(sec_param_exch_req_dataJSON, "protectionPolicyInfo");
    if (protection_policy_info) {
    protection_policy_info_local_nonprim = OpenAPI_protection_policy_parseFromJSON(protection_policy_info);
    if (!protection_policy_info_local_nonprim) {
        ogs_error("OpenAPI_protection_policy_parseFromJSON failed [protection_policy_info]");
        goto end;
    }
    }

    ipx_provider_sec_info_list = cJSON_GetObjectItemCaseSensitive(sec_param_exch_req_dataJSON, "ipxProviderSecInfoList");
    if (ipx_provider_sec_info_list) {
        cJSON *ipx_provider_sec_info_list_local = NULL;
        if (!cJSON_IsArray(ipx_provider_sec_info_list)) {
            ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [ipx_provider_sec_info_list]");
            goto end;
        }

        ipx_provider_sec_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipx_provider_sec_info_list_local, ipx_provider_sec_info_list) {
            if (!cJSON_IsObject(ipx_provider_sec_info_list_local)) {
                ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [ipx_provider_sec_info_list]");
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

    sender = cJSON_GetObjectItemCaseSensitive(sec_param_exch_req_dataJSON, "sender");
    if (sender) {
    if (!cJSON_IsString(sender) && !cJSON_IsNull(sender)) {
        ogs_error("OpenAPI_sec_param_exch_req_data_parseFromJSON() failed [sender]");
        goto end;
    }
    }

    sec_param_exch_req_data_local_var = OpenAPI_sec_param_exch_req_data_create (
        ogs_strdup(n32f_context_id->valuestring),
        jwe_cipher_suite_list ? jwe_cipher_suite_listList : NULL,
        jws_cipher_suite_list ? jws_cipher_suite_listList : NULL,
        protection_policy_info ? protection_policy_info_local_nonprim : NULL,
        ipx_provider_sec_info_list ? ipx_provider_sec_info_listList : NULL,
        sender && !cJSON_IsNull(sender) ? ogs_strdup(sender->valuestring) : NULL
    );

    return sec_param_exch_req_data_local_var;
end:
    if (jwe_cipher_suite_listList) {
        OpenAPI_list_for_each(jwe_cipher_suite_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(jwe_cipher_suite_listList);
        jwe_cipher_suite_listList = NULL;
    }
    if (jws_cipher_suite_listList) {
        OpenAPI_list_for_each(jws_cipher_suite_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(jws_cipher_suite_listList);
        jws_cipher_suite_listList = NULL;
    }
    if (protection_policy_info_local_nonprim) {
        OpenAPI_protection_policy_free(protection_policy_info_local_nonprim);
        protection_policy_info_local_nonprim = NULL;
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

OpenAPI_sec_param_exch_req_data_t *OpenAPI_sec_param_exch_req_data_copy(OpenAPI_sec_param_exch_req_data_t *dst, OpenAPI_sec_param_exch_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sec_param_exch_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sec_param_exch_req_data_convertToJSON() failed");
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

    OpenAPI_sec_param_exch_req_data_free(dst);
    dst = OpenAPI_sec_param_exch_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

