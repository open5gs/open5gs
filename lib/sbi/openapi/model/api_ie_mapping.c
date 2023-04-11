
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "api_ie_mapping.h"

OpenAPI_api_ie_mapping_t *OpenAPI_api_ie_mapping_create(
    OpenAPI_api_signature_t *api_signature,
    OpenAPI_http_method_e api_method,
    OpenAPI_list_t *ie_list
)
{
    OpenAPI_api_ie_mapping_t *api_ie_mapping_local_var = ogs_malloc(sizeof(OpenAPI_api_ie_mapping_t));
    ogs_assert(api_ie_mapping_local_var);

    api_ie_mapping_local_var->api_signature = api_signature;
    api_ie_mapping_local_var->api_method = api_method;
    api_ie_mapping_local_var->ie_list = ie_list;

    return api_ie_mapping_local_var;
}

void OpenAPI_api_ie_mapping_free(OpenAPI_api_ie_mapping_t *api_ie_mapping)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == api_ie_mapping) {
        return;
    }
    if (api_ie_mapping->api_signature) {
        OpenAPI_api_signature_free(api_ie_mapping->api_signature);
        api_ie_mapping->api_signature = NULL;
    }
    if (api_ie_mapping->ie_list) {
        OpenAPI_list_for_each(api_ie_mapping->ie_list, node) {
            OpenAPI_ie_info_free(node->data);
        }
        OpenAPI_list_free(api_ie_mapping->ie_list);
        api_ie_mapping->ie_list = NULL;
    }
    ogs_free(api_ie_mapping);
}

cJSON *OpenAPI_api_ie_mapping_convertToJSON(OpenAPI_api_ie_mapping_t *api_ie_mapping)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (api_ie_mapping == NULL) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [ApiIeMapping]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!api_ie_mapping->api_signature) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [api_signature]");
        return NULL;
    }
    cJSON *api_signature_local_JSON = OpenAPI_api_signature_convertToJSON(api_ie_mapping->api_signature);
    if (api_signature_local_JSON == NULL) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [api_signature]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apiSignature", api_signature_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [api_signature]");
        goto end;
    }

    if (api_ie_mapping->api_method == OpenAPI_http_method_NULL) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [api_method]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "apiMethod", OpenAPI_http_method_ToString(api_ie_mapping->api_method)) == NULL) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [api_method]");
        goto end;
    }

    if (!api_ie_mapping->ie_list) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [ie_list]");
        return NULL;
    }
    cJSON *ie_listList = cJSON_AddArrayToObject(item, "IeList");
    if (ie_listList == NULL) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [ie_list]");
        goto end;
    }
    OpenAPI_list_for_each(api_ie_mapping->ie_list, node) {
        cJSON *itemLocal = OpenAPI_ie_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed [ie_list]");
            goto end;
        }
        cJSON_AddItemToArray(ie_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_api_ie_mapping_t *OpenAPI_api_ie_mapping_parseFromJSON(cJSON *api_ie_mappingJSON)
{
    OpenAPI_api_ie_mapping_t *api_ie_mapping_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *api_signature = NULL;
    OpenAPI_api_signature_t *api_signature_local_nonprim = NULL;
    cJSON *api_method = NULL;
    OpenAPI_http_method_e api_methodVariable = 0;
    cJSON *ie_list = NULL;
    OpenAPI_list_t *ie_listList = NULL;
    api_signature = cJSON_GetObjectItemCaseSensitive(api_ie_mappingJSON, "apiSignature");
    if (!api_signature) {
        ogs_error("OpenAPI_api_ie_mapping_parseFromJSON() failed [api_signature]");
        goto end;
    }
    api_signature_local_nonprim = OpenAPI_api_signature_parseFromJSON(api_signature);
    if (!api_signature_local_nonprim) {
        ogs_error("OpenAPI_api_signature_parseFromJSON failed [api_signature]");
        goto end;
    }

    api_method = cJSON_GetObjectItemCaseSensitive(api_ie_mappingJSON, "apiMethod");
    if (!api_method) {
        ogs_error("OpenAPI_api_ie_mapping_parseFromJSON() failed [api_method]");
        goto end;
    }
    if (!cJSON_IsString(api_method)) {
        ogs_error("OpenAPI_api_ie_mapping_parseFromJSON() failed [api_method]");
        goto end;
    }
    api_methodVariable = OpenAPI_http_method_FromString(api_method->valuestring);

    ie_list = cJSON_GetObjectItemCaseSensitive(api_ie_mappingJSON, "IeList");
    if (!ie_list) {
        ogs_error("OpenAPI_api_ie_mapping_parseFromJSON() failed [ie_list]");
        goto end;
    }
        cJSON *ie_list_local = NULL;
        if (!cJSON_IsArray(ie_list)) {
            ogs_error("OpenAPI_api_ie_mapping_parseFromJSON() failed [ie_list]");
            goto end;
        }

        ie_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ie_list_local, ie_list) {
            if (!cJSON_IsObject(ie_list_local)) {
                ogs_error("OpenAPI_api_ie_mapping_parseFromJSON() failed [ie_list]");
                goto end;
            }
            OpenAPI_ie_info_t *ie_listItem = OpenAPI_ie_info_parseFromJSON(ie_list_local);
            if (!ie_listItem) {
                ogs_error("No ie_listItem");
                goto end;
            }
            OpenAPI_list_add(ie_listList, ie_listItem);
        }

    api_ie_mapping_local_var = OpenAPI_api_ie_mapping_create (
        api_signature_local_nonprim,
        api_methodVariable,
        ie_listList
    );

    return api_ie_mapping_local_var;
end:
    if (api_signature_local_nonprim) {
        OpenAPI_api_signature_free(api_signature_local_nonprim);
        api_signature_local_nonprim = NULL;
    }
    if (ie_listList) {
        OpenAPI_list_for_each(ie_listList, node) {
            OpenAPI_ie_info_free(node->data);
        }
        OpenAPI_list_free(ie_listList);
        ie_listList = NULL;
    }
    return NULL;
}

OpenAPI_api_ie_mapping_t *OpenAPI_api_ie_mapping_copy(OpenAPI_api_ie_mapping_t *dst, OpenAPI_api_ie_mapping_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_api_ie_mapping_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_api_ie_mapping_convertToJSON() failed");
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

    OpenAPI_api_ie_mapping_free(dst);
    dst = OpenAPI_api_ie_mapping_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

