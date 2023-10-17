
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ipx_provider_sec_info.h"

OpenAPI_ipx_provider_sec_info_t *OpenAPI_ipx_provider_sec_info_create(
    char *ipx_provider_id,
    OpenAPI_list_t *raw_public_key_list,
    OpenAPI_list_t *certificate_list
)
{
    OpenAPI_ipx_provider_sec_info_t *ipx_provider_sec_info_local_var = ogs_malloc(sizeof(OpenAPI_ipx_provider_sec_info_t));
    ogs_assert(ipx_provider_sec_info_local_var);

    ipx_provider_sec_info_local_var->ipx_provider_id = ipx_provider_id;
    ipx_provider_sec_info_local_var->raw_public_key_list = raw_public_key_list;
    ipx_provider_sec_info_local_var->certificate_list = certificate_list;

    return ipx_provider_sec_info_local_var;
}

void OpenAPI_ipx_provider_sec_info_free(OpenAPI_ipx_provider_sec_info_t *ipx_provider_sec_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ipx_provider_sec_info) {
        return;
    }
    if (ipx_provider_sec_info->ipx_provider_id) {
        ogs_free(ipx_provider_sec_info->ipx_provider_id);
        ipx_provider_sec_info->ipx_provider_id = NULL;
    }
    if (ipx_provider_sec_info->raw_public_key_list) {
        OpenAPI_list_for_each(ipx_provider_sec_info->raw_public_key_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipx_provider_sec_info->raw_public_key_list);
        ipx_provider_sec_info->raw_public_key_list = NULL;
    }
    if (ipx_provider_sec_info->certificate_list) {
        OpenAPI_list_for_each(ipx_provider_sec_info->certificate_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipx_provider_sec_info->certificate_list);
        ipx_provider_sec_info->certificate_list = NULL;
    }
    ogs_free(ipx_provider_sec_info);
}

cJSON *OpenAPI_ipx_provider_sec_info_convertToJSON(OpenAPI_ipx_provider_sec_info_t *ipx_provider_sec_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ipx_provider_sec_info == NULL) {
        ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed [IpxProviderSecInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ipx_provider_sec_info->ipx_provider_id) {
        ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed [ipx_provider_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ipxProviderId", ipx_provider_sec_info->ipx_provider_id) == NULL) {
        ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed [ipx_provider_id]");
        goto end;
    }

    if (ipx_provider_sec_info->raw_public_key_list) {
    cJSON *raw_public_key_listList = cJSON_AddArrayToObject(item, "rawPublicKeyList");
    if (raw_public_key_listList == NULL) {
        ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed [raw_public_key_list]");
        goto end;
    }
    OpenAPI_list_for_each(ipx_provider_sec_info->raw_public_key_list, node) {
        if (cJSON_AddStringToObject(raw_public_key_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed [raw_public_key_list]");
            goto end;
        }
    }
    }

    if (ipx_provider_sec_info->certificate_list) {
    cJSON *certificate_listList = cJSON_AddArrayToObject(item, "certificateList");
    if (certificate_listList == NULL) {
        ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed [certificate_list]");
        goto end;
    }
    OpenAPI_list_for_each(ipx_provider_sec_info->certificate_list, node) {
        if (cJSON_AddStringToObject(certificate_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed [certificate_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_ipx_provider_sec_info_t *OpenAPI_ipx_provider_sec_info_parseFromJSON(cJSON *ipx_provider_sec_infoJSON)
{
    OpenAPI_ipx_provider_sec_info_t *ipx_provider_sec_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipx_provider_id = NULL;
    cJSON *raw_public_key_list = NULL;
    OpenAPI_list_t *raw_public_key_listList = NULL;
    cJSON *certificate_list = NULL;
    OpenAPI_list_t *certificate_listList = NULL;
    ipx_provider_id = cJSON_GetObjectItemCaseSensitive(ipx_provider_sec_infoJSON, "ipxProviderId");
    if (!ipx_provider_id) {
        ogs_error("OpenAPI_ipx_provider_sec_info_parseFromJSON() failed [ipx_provider_id]");
        goto end;
    }
    if (!cJSON_IsString(ipx_provider_id)) {
        ogs_error("OpenAPI_ipx_provider_sec_info_parseFromJSON() failed [ipx_provider_id]");
        goto end;
    }

    raw_public_key_list = cJSON_GetObjectItemCaseSensitive(ipx_provider_sec_infoJSON, "rawPublicKeyList");
    if (raw_public_key_list) {
        cJSON *raw_public_key_list_local = NULL;
        if (!cJSON_IsArray(raw_public_key_list)) {
            ogs_error("OpenAPI_ipx_provider_sec_info_parseFromJSON() failed [raw_public_key_list]");
            goto end;
        }

        raw_public_key_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(raw_public_key_list_local, raw_public_key_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(raw_public_key_list_local)) {
                ogs_error("OpenAPI_ipx_provider_sec_info_parseFromJSON() failed [raw_public_key_list]");
                goto end;
            }
            OpenAPI_list_add(raw_public_key_listList, ogs_strdup(raw_public_key_list_local->valuestring));
        }
    }

    certificate_list = cJSON_GetObjectItemCaseSensitive(ipx_provider_sec_infoJSON, "certificateList");
    if (certificate_list) {
        cJSON *certificate_list_local = NULL;
        if (!cJSON_IsArray(certificate_list)) {
            ogs_error("OpenAPI_ipx_provider_sec_info_parseFromJSON() failed [certificate_list]");
            goto end;
        }

        certificate_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(certificate_list_local, certificate_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(certificate_list_local)) {
                ogs_error("OpenAPI_ipx_provider_sec_info_parseFromJSON() failed [certificate_list]");
                goto end;
            }
            OpenAPI_list_add(certificate_listList, ogs_strdup(certificate_list_local->valuestring));
        }
    }

    ipx_provider_sec_info_local_var = OpenAPI_ipx_provider_sec_info_create (
        ogs_strdup(ipx_provider_id->valuestring),
        raw_public_key_list ? raw_public_key_listList : NULL,
        certificate_list ? certificate_listList : NULL
    );

    return ipx_provider_sec_info_local_var;
end:
    if (raw_public_key_listList) {
        OpenAPI_list_for_each(raw_public_key_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(raw_public_key_listList);
        raw_public_key_listList = NULL;
    }
    if (certificate_listList) {
        OpenAPI_list_for_each(certificate_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(certificate_listList);
        certificate_listList = NULL;
    }
    return NULL;
}

OpenAPI_ipx_provider_sec_info_t *OpenAPI_ipx_provider_sec_info_copy(OpenAPI_ipx_provider_sec_info_t *dst, OpenAPI_ipx_provider_sec_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ipx_provider_sec_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ipx_provider_sec_info_convertToJSON() failed");
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

    OpenAPI_ipx_provider_sec_info_free(dst);
    dst = OpenAPI_ipx_provider_sec_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

