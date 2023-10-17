
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server_addressing_info.h"

OpenAPI_server_addressing_info_t *OpenAPI_server_addressing_info_create(
    OpenAPI_list_t *ipv4_addresses,
    OpenAPI_list_t *ipv6_addresses,
    OpenAPI_list_t *fqdn_list
)
{
    OpenAPI_server_addressing_info_t *server_addressing_info_local_var = ogs_malloc(sizeof(OpenAPI_server_addressing_info_t));
    ogs_assert(server_addressing_info_local_var);

    server_addressing_info_local_var->ipv4_addresses = ipv4_addresses;
    server_addressing_info_local_var->ipv6_addresses = ipv6_addresses;
    server_addressing_info_local_var->fqdn_list = fqdn_list;

    return server_addressing_info_local_var;
}

void OpenAPI_server_addressing_info_free(OpenAPI_server_addressing_info_t *server_addressing_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == server_addressing_info) {
        return;
    }
    if (server_addressing_info->ipv4_addresses) {
        OpenAPI_list_for_each(server_addressing_info->ipv4_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(server_addressing_info->ipv4_addresses);
        server_addressing_info->ipv4_addresses = NULL;
    }
    if (server_addressing_info->ipv6_addresses) {
        OpenAPI_list_for_each(server_addressing_info->ipv6_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(server_addressing_info->ipv6_addresses);
        server_addressing_info->ipv6_addresses = NULL;
    }
    if (server_addressing_info->fqdn_list) {
        OpenAPI_list_for_each(server_addressing_info->fqdn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(server_addressing_info->fqdn_list);
        server_addressing_info->fqdn_list = NULL;
    }
    ogs_free(server_addressing_info);
}

cJSON *OpenAPI_server_addressing_info_convertToJSON(OpenAPI_server_addressing_info_t *server_addressing_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (server_addressing_info == NULL) {
        ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed [ServerAddressingInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (server_addressing_info->ipv4_addresses) {
    cJSON *ipv4_addressesList = cJSON_AddArrayToObject(item, "ipv4Addresses");
    if (ipv4_addressesList == NULL) {
        ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed [ipv4_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(server_addressing_info->ipv4_addresses, node) {
        if (cJSON_AddStringToObject(ipv4_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed [ipv4_addresses]");
            goto end;
        }
    }
    }

    if (server_addressing_info->ipv6_addresses) {
    cJSON *ipv6_addressesList = cJSON_AddArrayToObject(item, "ipv6Addresses");
    if (ipv6_addressesList == NULL) {
        ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed [ipv6_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(server_addressing_info->ipv6_addresses, node) {
        if (cJSON_AddStringToObject(ipv6_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed [ipv6_addresses]");
            goto end;
        }
    }
    }

    if (server_addressing_info->fqdn_list) {
    cJSON *fqdn_listList = cJSON_AddArrayToObject(item, "fqdnList");
    if (fqdn_listList == NULL) {
        ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed [fqdn_list]");
        goto end;
    }
    OpenAPI_list_for_each(server_addressing_info->fqdn_list, node) {
        if (cJSON_AddStringToObject(fqdn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed [fqdn_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_server_addressing_info_t *OpenAPI_server_addressing_info_parseFromJSON(cJSON *server_addressing_infoJSON)
{
    OpenAPI_server_addressing_info_t *server_addressing_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addresses = NULL;
    OpenAPI_list_t *ipv4_addressesList = NULL;
    cJSON *ipv6_addresses = NULL;
    OpenAPI_list_t *ipv6_addressesList = NULL;
    cJSON *fqdn_list = NULL;
    OpenAPI_list_t *fqdn_listList = NULL;
    ipv4_addresses = cJSON_GetObjectItemCaseSensitive(server_addressing_infoJSON, "ipv4Addresses");
    if (ipv4_addresses) {
        cJSON *ipv4_addresses_local = NULL;
        if (!cJSON_IsArray(ipv4_addresses)) {
            ogs_error("OpenAPI_server_addressing_info_parseFromJSON() failed [ipv4_addresses]");
            goto end;
        }

        ipv4_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addresses_local, ipv4_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_addresses_local)) {
                ogs_error("OpenAPI_server_addressing_info_parseFromJSON() failed [ipv4_addresses]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addressesList, ogs_strdup(ipv4_addresses_local->valuestring));
        }
    }

    ipv6_addresses = cJSON_GetObjectItemCaseSensitive(server_addressing_infoJSON, "ipv6Addresses");
    if (ipv6_addresses) {
        cJSON *ipv6_addresses_local = NULL;
        if (!cJSON_IsArray(ipv6_addresses)) {
            ogs_error("OpenAPI_server_addressing_info_parseFromJSON() failed [ipv6_addresses]");
            goto end;
        }

        ipv6_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_addresses_local, ipv6_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_addresses_local)) {
                ogs_error("OpenAPI_server_addressing_info_parseFromJSON() failed [ipv6_addresses]");
                goto end;
            }
            OpenAPI_list_add(ipv6_addressesList, ogs_strdup(ipv6_addresses_local->valuestring));
        }
    }

    fqdn_list = cJSON_GetObjectItemCaseSensitive(server_addressing_infoJSON, "fqdnList");
    if (fqdn_list) {
        cJSON *fqdn_list_local = NULL;
        if (!cJSON_IsArray(fqdn_list)) {
            ogs_error("OpenAPI_server_addressing_info_parseFromJSON() failed [fqdn_list]");
            goto end;
        }

        fqdn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(fqdn_list_local, fqdn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(fqdn_list_local)) {
                ogs_error("OpenAPI_server_addressing_info_parseFromJSON() failed [fqdn_list]");
                goto end;
            }
            OpenAPI_list_add(fqdn_listList, ogs_strdup(fqdn_list_local->valuestring));
        }
    }

    server_addressing_info_local_var = OpenAPI_server_addressing_info_create (
        ipv4_addresses ? ipv4_addressesList : NULL,
        ipv6_addresses ? ipv6_addressesList : NULL,
        fqdn_list ? fqdn_listList : NULL
    );

    return server_addressing_info_local_var;
end:
    if (ipv4_addressesList) {
        OpenAPI_list_for_each(ipv4_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_addressesList);
        ipv4_addressesList = NULL;
    }
    if (ipv6_addressesList) {
        OpenAPI_list_for_each(ipv6_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_addressesList);
        ipv6_addressesList = NULL;
    }
    if (fqdn_listList) {
        OpenAPI_list_for_each(fqdn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(fqdn_listList);
        fqdn_listList = NULL;
    }
    return NULL;
}

OpenAPI_server_addressing_info_t *OpenAPI_server_addressing_info_copy(OpenAPI_server_addressing_info_t *dst, OpenAPI_server_addressing_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_server_addressing_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_server_addressing_info_convertToJSON() failed");
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

    OpenAPI_server_addressing_info_free(dst);
    dst = OpenAPI_server_addressing_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

