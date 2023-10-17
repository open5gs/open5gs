
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_interface_amf_info.h"

OpenAPI_n2_interface_amf_info_t *OpenAPI_n2_interface_amf_info_create(
    OpenAPI_list_t *ipv4_endpoint_address,
    OpenAPI_list_t *ipv6_endpoint_address,
    char *amf_name
)
{
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info_local_var = ogs_malloc(sizeof(OpenAPI_n2_interface_amf_info_t));
    ogs_assert(n2_interface_amf_info_local_var);

    n2_interface_amf_info_local_var->ipv4_endpoint_address = ipv4_endpoint_address;
    n2_interface_amf_info_local_var->ipv6_endpoint_address = ipv6_endpoint_address;
    n2_interface_amf_info_local_var->amf_name = amf_name;

    return n2_interface_amf_info_local_var;
}

void OpenAPI_n2_interface_amf_info_free(OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_interface_amf_info) {
        return;
    }
    if (n2_interface_amf_info->ipv4_endpoint_address) {
        OpenAPI_list_for_each(n2_interface_amf_info->ipv4_endpoint_address, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(n2_interface_amf_info->ipv4_endpoint_address);
        n2_interface_amf_info->ipv4_endpoint_address = NULL;
    }
    if (n2_interface_amf_info->ipv6_endpoint_address) {
        OpenAPI_list_for_each(n2_interface_amf_info->ipv6_endpoint_address, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(n2_interface_amf_info->ipv6_endpoint_address);
        n2_interface_amf_info->ipv6_endpoint_address = NULL;
    }
    if (n2_interface_amf_info->amf_name) {
        ogs_free(n2_interface_amf_info->amf_name);
        n2_interface_amf_info->amf_name = NULL;
    }
    ogs_free(n2_interface_amf_info);
}

cJSON *OpenAPI_n2_interface_amf_info_convertToJSON(OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_interface_amf_info == NULL) {
        ogs_error("OpenAPI_n2_interface_amf_info_convertToJSON() failed [N2InterfaceAmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_interface_amf_info->ipv4_endpoint_address) {
    cJSON *ipv4_endpoint_addressList = cJSON_AddArrayToObject(item, "ipv4EndpointAddress");
    if (ipv4_endpoint_addressList == NULL) {
        ogs_error("OpenAPI_n2_interface_amf_info_convertToJSON() failed [ipv4_endpoint_address]");
        goto end;
    }
    OpenAPI_list_for_each(n2_interface_amf_info->ipv4_endpoint_address, node) {
        if (cJSON_AddStringToObject(ipv4_endpoint_addressList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_n2_interface_amf_info_convertToJSON() failed [ipv4_endpoint_address]");
            goto end;
        }
    }
    }

    if (n2_interface_amf_info->ipv6_endpoint_address) {
    cJSON *ipv6_endpoint_addressList = cJSON_AddArrayToObject(item, "ipv6EndpointAddress");
    if (ipv6_endpoint_addressList == NULL) {
        ogs_error("OpenAPI_n2_interface_amf_info_convertToJSON() failed [ipv6_endpoint_address]");
        goto end;
    }
    OpenAPI_list_for_each(n2_interface_amf_info->ipv6_endpoint_address, node) {
        if (cJSON_AddStringToObject(ipv6_endpoint_addressList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_n2_interface_amf_info_convertToJSON() failed [ipv6_endpoint_address]");
            goto end;
        }
    }
    }

    if (n2_interface_amf_info->amf_name) {
    if (cJSON_AddStringToObject(item, "amfName", n2_interface_amf_info->amf_name) == NULL) {
        ogs_error("OpenAPI_n2_interface_amf_info_convertToJSON() failed [amf_name]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_interface_amf_info_t *OpenAPI_n2_interface_amf_info_parseFromJSON(cJSON *n2_interface_amf_infoJSON)
{
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_endpoint_address = NULL;
    OpenAPI_list_t *ipv4_endpoint_addressList = NULL;
    cJSON *ipv6_endpoint_address = NULL;
    OpenAPI_list_t *ipv6_endpoint_addressList = NULL;
    cJSON *amf_name = NULL;
    ipv4_endpoint_address = cJSON_GetObjectItemCaseSensitive(n2_interface_amf_infoJSON, "ipv4EndpointAddress");
    if (ipv4_endpoint_address) {
        cJSON *ipv4_endpoint_address_local = NULL;
        if (!cJSON_IsArray(ipv4_endpoint_address)) {
            ogs_error("OpenAPI_n2_interface_amf_info_parseFromJSON() failed [ipv4_endpoint_address]");
            goto end;
        }

        ipv4_endpoint_addressList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_endpoint_address_local, ipv4_endpoint_address) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_endpoint_address_local)) {
                ogs_error("OpenAPI_n2_interface_amf_info_parseFromJSON() failed [ipv4_endpoint_address]");
                goto end;
            }
            OpenAPI_list_add(ipv4_endpoint_addressList, ogs_strdup(ipv4_endpoint_address_local->valuestring));
        }
    }

    ipv6_endpoint_address = cJSON_GetObjectItemCaseSensitive(n2_interface_amf_infoJSON, "ipv6EndpointAddress");
    if (ipv6_endpoint_address) {
        cJSON *ipv6_endpoint_address_local = NULL;
        if (!cJSON_IsArray(ipv6_endpoint_address)) {
            ogs_error("OpenAPI_n2_interface_amf_info_parseFromJSON() failed [ipv6_endpoint_address]");
            goto end;
        }

        ipv6_endpoint_addressList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_endpoint_address_local, ipv6_endpoint_address) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_endpoint_address_local)) {
                ogs_error("OpenAPI_n2_interface_amf_info_parseFromJSON() failed [ipv6_endpoint_address]");
                goto end;
            }
            OpenAPI_list_add(ipv6_endpoint_addressList, ogs_strdup(ipv6_endpoint_address_local->valuestring));
        }
    }

    amf_name = cJSON_GetObjectItemCaseSensitive(n2_interface_amf_infoJSON, "amfName");
    if (amf_name) {
    if (!cJSON_IsString(amf_name) && !cJSON_IsNull(amf_name)) {
        ogs_error("OpenAPI_n2_interface_amf_info_parseFromJSON() failed [amf_name]");
        goto end;
    }
    }

    n2_interface_amf_info_local_var = OpenAPI_n2_interface_amf_info_create (
        ipv4_endpoint_address ? ipv4_endpoint_addressList : NULL,
        ipv6_endpoint_address ? ipv6_endpoint_addressList : NULL,
        amf_name && !cJSON_IsNull(amf_name) ? ogs_strdup(amf_name->valuestring) : NULL
    );

    return n2_interface_amf_info_local_var;
end:
    if (ipv4_endpoint_addressList) {
        OpenAPI_list_for_each(ipv4_endpoint_addressList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_endpoint_addressList);
        ipv4_endpoint_addressList = NULL;
    }
    if (ipv6_endpoint_addressList) {
        OpenAPI_list_for_each(ipv6_endpoint_addressList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_endpoint_addressList);
        ipv6_endpoint_addressList = NULL;
    }
    return NULL;
}

OpenAPI_n2_interface_amf_info_t *OpenAPI_n2_interface_amf_info_copy(OpenAPI_n2_interface_amf_info_t *dst, OpenAPI_n2_interface_amf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_interface_amf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_interface_amf_info_convertToJSON() failed");
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

    OpenAPI_n2_interface_amf_info_free(dst);
    dst = OpenAPI_n2_interface_amf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

