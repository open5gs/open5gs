
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecs_server_addr.h"

OpenAPI_ecs_server_addr_t *OpenAPI_ecs_server_addr_create(
    OpenAPI_list_t *ecs_fqdn_list,
    OpenAPI_list_t *ecs_ip_address_list,
    OpenAPI_list_t *ecs_uri_list,
    char *ecs_provider_id
)
{
    OpenAPI_ecs_server_addr_t *ecs_server_addr_local_var = ogs_malloc(sizeof(OpenAPI_ecs_server_addr_t));
    ogs_assert(ecs_server_addr_local_var);

    ecs_server_addr_local_var->ecs_fqdn_list = ecs_fqdn_list;
    ecs_server_addr_local_var->ecs_ip_address_list = ecs_ip_address_list;
    ecs_server_addr_local_var->ecs_uri_list = ecs_uri_list;
    ecs_server_addr_local_var->ecs_provider_id = ecs_provider_id;

    return ecs_server_addr_local_var;
}

void OpenAPI_ecs_server_addr_free(OpenAPI_ecs_server_addr_t *ecs_server_addr)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecs_server_addr) {
        return;
    }
    if (ecs_server_addr->ecs_fqdn_list) {
        OpenAPI_list_for_each(ecs_server_addr->ecs_fqdn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ecs_server_addr->ecs_fqdn_list);
        ecs_server_addr->ecs_fqdn_list = NULL;
    }
    if (ecs_server_addr->ecs_ip_address_list) {
        OpenAPI_list_for_each(ecs_server_addr->ecs_ip_address_list, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(ecs_server_addr->ecs_ip_address_list);
        ecs_server_addr->ecs_ip_address_list = NULL;
    }
    if (ecs_server_addr->ecs_uri_list) {
        OpenAPI_list_for_each(ecs_server_addr->ecs_uri_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ecs_server_addr->ecs_uri_list);
        ecs_server_addr->ecs_uri_list = NULL;
    }
    if (ecs_server_addr->ecs_provider_id) {
        ogs_free(ecs_server_addr->ecs_provider_id);
        ecs_server_addr->ecs_provider_id = NULL;
    }
    ogs_free(ecs_server_addr);
}

cJSON *OpenAPI_ecs_server_addr_convertToJSON(OpenAPI_ecs_server_addr_t *ecs_server_addr)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecs_server_addr == NULL) {
        ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [EcsServerAddr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ecs_server_addr->ecs_fqdn_list) {
    cJSON *ecs_fqdn_listList = cJSON_AddArrayToObject(item, "ecsFqdnList");
    if (ecs_fqdn_listList == NULL) {
        ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [ecs_fqdn_list]");
        goto end;
    }
    OpenAPI_list_for_each(ecs_server_addr->ecs_fqdn_list, node) {
        if (cJSON_AddStringToObject(ecs_fqdn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [ecs_fqdn_list]");
            goto end;
        }
    }
    }

    if (ecs_server_addr->ecs_ip_address_list) {
    cJSON *ecs_ip_address_listList = cJSON_AddArrayToObject(item, "ecsIpAddressList");
    if (ecs_ip_address_listList == NULL) {
        ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [ecs_ip_address_list]");
        goto end;
    }
    OpenAPI_list_for_each(ecs_server_addr->ecs_ip_address_list, node) {
        cJSON *itemLocal = OpenAPI_ip_addr_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [ecs_ip_address_list]");
            goto end;
        }
        cJSON_AddItemToArray(ecs_ip_address_listList, itemLocal);
    }
    }

    if (ecs_server_addr->ecs_uri_list) {
    cJSON *ecs_uri_listList = cJSON_AddArrayToObject(item, "ecsUriList");
    if (ecs_uri_listList == NULL) {
        ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [ecs_uri_list]");
        goto end;
    }
    OpenAPI_list_for_each(ecs_server_addr->ecs_uri_list, node) {
        if (cJSON_AddStringToObject(ecs_uri_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [ecs_uri_list]");
            goto end;
        }
    }
    }

    if (ecs_server_addr->ecs_provider_id) {
    if (cJSON_AddStringToObject(item, "ecsProviderId", ecs_server_addr->ecs_provider_id) == NULL) {
        ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed [ecs_provider_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ecs_server_addr_t *OpenAPI_ecs_server_addr_parseFromJSON(cJSON *ecs_server_addrJSON)
{
    OpenAPI_ecs_server_addr_t *ecs_server_addr_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ecs_fqdn_list = NULL;
    OpenAPI_list_t *ecs_fqdn_listList = NULL;
    cJSON *ecs_ip_address_list = NULL;
    OpenAPI_list_t *ecs_ip_address_listList = NULL;
    cJSON *ecs_uri_list = NULL;
    OpenAPI_list_t *ecs_uri_listList = NULL;
    cJSON *ecs_provider_id = NULL;
    ecs_fqdn_list = cJSON_GetObjectItemCaseSensitive(ecs_server_addrJSON, "ecsFqdnList");
    if (ecs_fqdn_list) {
        cJSON *ecs_fqdn_list_local = NULL;
        if (!cJSON_IsArray(ecs_fqdn_list)) {
            ogs_error("OpenAPI_ecs_server_addr_parseFromJSON() failed [ecs_fqdn_list]");
            goto end;
        }

        ecs_fqdn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecs_fqdn_list_local, ecs_fqdn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ecs_fqdn_list_local)) {
                ogs_error("OpenAPI_ecs_server_addr_parseFromJSON() failed [ecs_fqdn_list]");
                goto end;
            }
            OpenAPI_list_add(ecs_fqdn_listList, ogs_strdup(ecs_fqdn_list_local->valuestring));
        }
    }

    ecs_ip_address_list = cJSON_GetObjectItemCaseSensitive(ecs_server_addrJSON, "ecsIpAddressList");
    if (ecs_ip_address_list) {
        cJSON *ecs_ip_address_list_local = NULL;
        if (!cJSON_IsArray(ecs_ip_address_list)) {
            ogs_error("OpenAPI_ecs_server_addr_parseFromJSON() failed [ecs_ip_address_list]");
            goto end;
        }

        ecs_ip_address_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecs_ip_address_list_local, ecs_ip_address_list) {
            if (!cJSON_IsObject(ecs_ip_address_list_local)) {
                ogs_error("OpenAPI_ecs_server_addr_parseFromJSON() failed [ecs_ip_address_list]");
                goto end;
            }
            OpenAPI_ip_addr_t *ecs_ip_address_listItem = OpenAPI_ip_addr_parseFromJSON(ecs_ip_address_list_local);
            if (!ecs_ip_address_listItem) {
                ogs_error("No ecs_ip_address_listItem");
                goto end;
            }
            OpenAPI_list_add(ecs_ip_address_listList, ecs_ip_address_listItem);
        }
    }

    ecs_uri_list = cJSON_GetObjectItemCaseSensitive(ecs_server_addrJSON, "ecsUriList");
    if (ecs_uri_list) {
        cJSON *ecs_uri_list_local = NULL;
        if (!cJSON_IsArray(ecs_uri_list)) {
            ogs_error("OpenAPI_ecs_server_addr_parseFromJSON() failed [ecs_uri_list]");
            goto end;
        }

        ecs_uri_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecs_uri_list_local, ecs_uri_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ecs_uri_list_local)) {
                ogs_error("OpenAPI_ecs_server_addr_parseFromJSON() failed [ecs_uri_list]");
                goto end;
            }
            OpenAPI_list_add(ecs_uri_listList, ogs_strdup(ecs_uri_list_local->valuestring));
        }
    }

    ecs_provider_id = cJSON_GetObjectItemCaseSensitive(ecs_server_addrJSON, "ecsProviderId");
    if (ecs_provider_id) {
    if (!cJSON_IsString(ecs_provider_id) && !cJSON_IsNull(ecs_provider_id)) {
        ogs_error("OpenAPI_ecs_server_addr_parseFromJSON() failed [ecs_provider_id]");
        goto end;
    }
    }

    ecs_server_addr_local_var = OpenAPI_ecs_server_addr_create (
        ecs_fqdn_list ? ecs_fqdn_listList : NULL,
        ecs_ip_address_list ? ecs_ip_address_listList : NULL,
        ecs_uri_list ? ecs_uri_listList : NULL,
        ecs_provider_id && !cJSON_IsNull(ecs_provider_id) ? ogs_strdup(ecs_provider_id->valuestring) : NULL
    );

    return ecs_server_addr_local_var;
end:
    if (ecs_fqdn_listList) {
        OpenAPI_list_for_each(ecs_fqdn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ecs_fqdn_listList);
        ecs_fqdn_listList = NULL;
    }
    if (ecs_ip_address_listList) {
        OpenAPI_list_for_each(ecs_ip_address_listList, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(ecs_ip_address_listList);
        ecs_ip_address_listList = NULL;
    }
    if (ecs_uri_listList) {
        OpenAPI_list_for_each(ecs_uri_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ecs_uri_listList);
        ecs_uri_listList = NULL;
    }
    return NULL;
}

OpenAPI_ecs_server_addr_t *OpenAPI_ecs_server_addr_copy(OpenAPI_ecs_server_addr_t *dst, OpenAPI_ecs_server_addr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecs_server_addr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecs_server_addr_convertToJSON() failed");
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

    OpenAPI_ecs_server_addr_free(dst);
    dst = OpenAPI_ecs_server_addr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

