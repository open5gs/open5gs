
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "address_list.h"

OpenAPI_address_list_t *OpenAPI_address_list_create(
    OpenAPI_list_t *ipv4_addrs,
    OpenAPI_list_t *ipv6_addrs
)
{
    OpenAPI_address_list_t *address_list_local_var = ogs_malloc(sizeof(OpenAPI_address_list_t));
    ogs_assert(address_list_local_var);

    address_list_local_var->ipv4_addrs = ipv4_addrs;
    address_list_local_var->ipv6_addrs = ipv6_addrs;

    return address_list_local_var;
}

void OpenAPI_address_list_free(OpenAPI_address_list_t *address_list)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == address_list) {
        return;
    }
    if (address_list->ipv4_addrs) {
        OpenAPI_list_for_each(address_list->ipv4_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(address_list->ipv4_addrs);
        address_list->ipv4_addrs = NULL;
    }
    if (address_list->ipv6_addrs) {
        OpenAPI_list_for_each(address_list->ipv6_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(address_list->ipv6_addrs);
        address_list->ipv6_addrs = NULL;
    }
    ogs_free(address_list);
}

cJSON *OpenAPI_address_list_convertToJSON(OpenAPI_address_list_t *address_list)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (address_list == NULL) {
        ogs_error("OpenAPI_address_list_convertToJSON() failed [AddressList]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (address_list->ipv4_addrs) {
    cJSON *ipv4_addrsList = cJSON_AddArrayToObject(item, "ipv4Addrs");
    if (ipv4_addrsList == NULL) {
        ogs_error("OpenAPI_address_list_convertToJSON() failed [ipv4_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(address_list->ipv4_addrs, node) {
        if (cJSON_AddStringToObject(ipv4_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_address_list_convertToJSON() failed [ipv4_addrs]");
            goto end;
        }
    }
    }

    if (address_list->ipv6_addrs) {
    cJSON *ipv6_addrsList = cJSON_AddArrayToObject(item, "ipv6Addrs");
    if (ipv6_addrsList == NULL) {
        ogs_error("OpenAPI_address_list_convertToJSON() failed [ipv6_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(address_list->ipv6_addrs, node) {
        if (cJSON_AddStringToObject(ipv6_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_address_list_convertToJSON() failed [ipv6_addrs]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_address_list_t *OpenAPI_address_list_parseFromJSON(cJSON *address_listJSON)
{
    OpenAPI_address_list_t *address_list_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addrs = NULL;
    OpenAPI_list_t *ipv4_addrsList = NULL;
    cJSON *ipv6_addrs = NULL;
    OpenAPI_list_t *ipv6_addrsList = NULL;
    ipv4_addrs = cJSON_GetObjectItemCaseSensitive(address_listJSON, "ipv4Addrs");
    if (ipv4_addrs) {
        cJSON *ipv4_addrs_local = NULL;
        if (!cJSON_IsArray(ipv4_addrs)) {
            ogs_error("OpenAPI_address_list_parseFromJSON() failed [ipv4_addrs]");
            goto end;
        }

        ipv4_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addrs_local, ipv4_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_addrs_local)) {
                ogs_error("OpenAPI_address_list_parseFromJSON() failed [ipv4_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addrsList, ogs_strdup(ipv4_addrs_local->valuestring));
        }
    }

    ipv6_addrs = cJSON_GetObjectItemCaseSensitive(address_listJSON, "ipv6Addrs");
    if (ipv6_addrs) {
        cJSON *ipv6_addrs_local = NULL;
        if (!cJSON_IsArray(ipv6_addrs)) {
            ogs_error("OpenAPI_address_list_parseFromJSON() failed [ipv6_addrs]");
            goto end;
        }

        ipv6_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_addrs_local, ipv6_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_addrs_local)) {
                ogs_error("OpenAPI_address_list_parseFromJSON() failed [ipv6_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv6_addrsList, ogs_strdup(ipv6_addrs_local->valuestring));
        }
    }

    address_list_local_var = OpenAPI_address_list_create (
        ipv4_addrs ? ipv4_addrsList : NULL,
        ipv6_addrs ? ipv6_addrsList : NULL
    );

    return address_list_local_var;
end:
    if (ipv4_addrsList) {
        OpenAPI_list_for_each(ipv4_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_addrsList);
        ipv4_addrsList = NULL;
    }
    if (ipv6_addrsList) {
        OpenAPI_list_for_each(ipv6_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_addrsList);
        ipv6_addrsList = NULL;
    }
    return NULL;
}

OpenAPI_address_list_t *OpenAPI_address_list_copy(OpenAPI_address_list_t *dst, OpenAPI_address_list_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_address_list_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_address_list_convertToJSON() failed");
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

    OpenAPI_address_list_free(dst);
    dst = OpenAPI_address_list_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

