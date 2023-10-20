
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "protection_policy.h"

OpenAPI_protection_policy_t *OpenAPI_protection_policy_create(
    OpenAPI_list_t *api_ie_mapping_list,
    OpenAPI_list_t *data_type_enc_policy
)
{
    OpenAPI_protection_policy_t *protection_policy_local_var = ogs_malloc(sizeof(OpenAPI_protection_policy_t));
    ogs_assert(protection_policy_local_var);

    protection_policy_local_var->api_ie_mapping_list = api_ie_mapping_list;
    protection_policy_local_var->data_type_enc_policy = data_type_enc_policy;

    return protection_policy_local_var;
}

void OpenAPI_protection_policy_free(OpenAPI_protection_policy_t *protection_policy)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == protection_policy) {
        return;
    }
    if (protection_policy->api_ie_mapping_list) {
        OpenAPI_list_for_each(protection_policy->api_ie_mapping_list, node) {
            OpenAPI_api_ie_mapping_free(node->data);
        }
        OpenAPI_list_free(protection_policy->api_ie_mapping_list);
        protection_policy->api_ie_mapping_list = NULL;
    }
    if (protection_policy->data_type_enc_policy) {
        OpenAPI_list_free(protection_policy->data_type_enc_policy);
        protection_policy->data_type_enc_policy = NULL;
    }
    ogs_free(protection_policy);
}

cJSON *OpenAPI_protection_policy_convertToJSON(OpenAPI_protection_policy_t *protection_policy)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (protection_policy == NULL) {
        ogs_error("OpenAPI_protection_policy_convertToJSON() failed [ProtectionPolicy]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!protection_policy->api_ie_mapping_list) {
        ogs_error("OpenAPI_protection_policy_convertToJSON() failed [api_ie_mapping_list]");
        return NULL;
    }
    cJSON *api_ie_mapping_listList = cJSON_AddArrayToObject(item, "apiIeMappingList");
    if (api_ie_mapping_listList == NULL) {
        ogs_error("OpenAPI_protection_policy_convertToJSON() failed [api_ie_mapping_list]");
        goto end;
    }
    OpenAPI_list_for_each(protection_policy->api_ie_mapping_list, node) {
        cJSON *itemLocal = OpenAPI_api_ie_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_protection_policy_convertToJSON() failed [api_ie_mapping_list]");
            goto end;
        }
        cJSON_AddItemToArray(api_ie_mapping_listList, itemLocal);
    }

    if (protection_policy->data_type_enc_policy != OpenAPI_ie_type_NULL) {
    cJSON *data_type_enc_policyList = cJSON_AddArrayToObject(item, "dataTypeEncPolicy");
    if (data_type_enc_policyList == NULL) {
        ogs_error("OpenAPI_protection_policy_convertToJSON() failed [data_type_enc_policy]");
        goto end;
    }
    OpenAPI_list_for_each(protection_policy->data_type_enc_policy, node) {
        if (cJSON_AddStringToObject(data_type_enc_policyList, "", OpenAPI_ie_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_protection_policy_convertToJSON() failed [data_type_enc_policy]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_protection_policy_t *OpenAPI_protection_policy_parseFromJSON(cJSON *protection_policyJSON)
{
    OpenAPI_protection_policy_t *protection_policy_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *api_ie_mapping_list = NULL;
    OpenAPI_list_t *api_ie_mapping_listList = NULL;
    cJSON *data_type_enc_policy = NULL;
    OpenAPI_list_t *data_type_enc_policyList = NULL;
    api_ie_mapping_list = cJSON_GetObjectItemCaseSensitive(protection_policyJSON, "apiIeMappingList");
    if (!api_ie_mapping_list) {
        ogs_error("OpenAPI_protection_policy_parseFromJSON() failed [api_ie_mapping_list]");
        goto end;
    }
        cJSON *api_ie_mapping_list_local = NULL;
        if (!cJSON_IsArray(api_ie_mapping_list)) {
            ogs_error("OpenAPI_protection_policy_parseFromJSON() failed [api_ie_mapping_list]");
            goto end;
        }

        api_ie_mapping_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(api_ie_mapping_list_local, api_ie_mapping_list) {
            if (!cJSON_IsObject(api_ie_mapping_list_local)) {
                ogs_error("OpenAPI_protection_policy_parseFromJSON() failed [api_ie_mapping_list]");
                goto end;
            }
            OpenAPI_api_ie_mapping_t *api_ie_mapping_listItem = OpenAPI_api_ie_mapping_parseFromJSON(api_ie_mapping_list_local);
            if (!api_ie_mapping_listItem) {
                ogs_error("No api_ie_mapping_listItem");
                goto end;
            }
            OpenAPI_list_add(api_ie_mapping_listList, api_ie_mapping_listItem);
        }

    data_type_enc_policy = cJSON_GetObjectItemCaseSensitive(protection_policyJSON, "dataTypeEncPolicy");
    if (data_type_enc_policy) {
        cJSON *data_type_enc_policy_local = NULL;
        if (!cJSON_IsArray(data_type_enc_policy)) {
            ogs_error("OpenAPI_protection_policy_parseFromJSON() failed [data_type_enc_policy]");
            goto end;
        }

        data_type_enc_policyList = OpenAPI_list_create();

        cJSON_ArrayForEach(data_type_enc_policy_local, data_type_enc_policy) {
            OpenAPI_ie_type_e localEnum = OpenAPI_ie_type_NULL;
            if (!cJSON_IsString(data_type_enc_policy_local)) {
                ogs_error("OpenAPI_protection_policy_parseFromJSON() failed [data_type_enc_policy]");
                goto end;
            }
            localEnum = OpenAPI_ie_type_FromString(data_type_enc_policy_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"data_type_enc_policy\" is not supported. Ignoring it ...",
                         data_type_enc_policy_local->valuestring);
            } else {
                OpenAPI_list_add(data_type_enc_policyList, (void *)localEnum);
            }
        }
        if (data_type_enc_policyList->count == 0) {
            ogs_error("OpenAPI_protection_policy_parseFromJSON() failed: Expected data_type_enc_policyList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    protection_policy_local_var = OpenAPI_protection_policy_create (
        api_ie_mapping_listList,
        data_type_enc_policy ? data_type_enc_policyList : NULL
    );

    return protection_policy_local_var;
end:
    if (api_ie_mapping_listList) {
        OpenAPI_list_for_each(api_ie_mapping_listList, node) {
            OpenAPI_api_ie_mapping_free(node->data);
        }
        OpenAPI_list_free(api_ie_mapping_listList);
        api_ie_mapping_listList = NULL;
    }
    if (data_type_enc_policyList) {
        OpenAPI_list_free(data_type_enc_policyList);
        data_type_enc_policyList = NULL;
    }
    return NULL;
}

OpenAPI_protection_policy_t *OpenAPI_protection_policy_copy(OpenAPI_protection_policy_t *dst, OpenAPI_protection_policy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_protection_policy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_protection_policy_convertToJSON() failed");
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

    OpenAPI_protection_policy_free(dst);
    dst = OpenAPI_protection_policy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

