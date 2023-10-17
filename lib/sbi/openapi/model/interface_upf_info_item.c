
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "interface_upf_info_item.h"

OpenAPI_interface_upf_info_item_t *OpenAPI_interface_upf_info_item_create(
    OpenAPI_up_interface_type_e interface_type,
    OpenAPI_list_t *ipv4_endpoint_addresses,
    OpenAPI_list_t *ipv6_endpoint_addresses,
    char *endpoint_fqdn,
    char *network_instance
)
{
    OpenAPI_interface_upf_info_item_t *interface_upf_info_item_local_var = ogs_malloc(sizeof(OpenAPI_interface_upf_info_item_t));
    ogs_assert(interface_upf_info_item_local_var);

    interface_upf_info_item_local_var->interface_type = interface_type;
    interface_upf_info_item_local_var->ipv4_endpoint_addresses = ipv4_endpoint_addresses;
    interface_upf_info_item_local_var->ipv6_endpoint_addresses = ipv6_endpoint_addresses;
    interface_upf_info_item_local_var->endpoint_fqdn = endpoint_fqdn;
    interface_upf_info_item_local_var->network_instance = network_instance;

    return interface_upf_info_item_local_var;
}

void OpenAPI_interface_upf_info_item_free(OpenAPI_interface_upf_info_item_t *interface_upf_info_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == interface_upf_info_item) {
        return;
    }
    if (interface_upf_info_item->ipv4_endpoint_addresses) {
        OpenAPI_list_for_each(interface_upf_info_item->ipv4_endpoint_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(interface_upf_info_item->ipv4_endpoint_addresses);
        interface_upf_info_item->ipv4_endpoint_addresses = NULL;
    }
    if (interface_upf_info_item->ipv6_endpoint_addresses) {
        OpenAPI_list_for_each(interface_upf_info_item->ipv6_endpoint_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(interface_upf_info_item->ipv6_endpoint_addresses);
        interface_upf_info_item->ipv6_endpoint_addresses = NULL;
    }
    if (interface_upf_info_item->endpoint_fqdn) {
        ogs_free(interface_upf_info_item->endpoint_fqdn);
        interface_upf_info_item->endpoint_fqdn = NULL;
    }
    if (interface_upf_info_item->network_instance) {
        ogs_free(interface_upf_info_item->network_instance);
        interface_upf_info_item->network_instance = NULL;
    }
    ogs_free(interface_upf_info_item);
}

cJSON *OpenAPI_interface_upf_info_item_convertToJSON(OpenAPI_interface_upf_info_item_t *interface_upf_info_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (interface_upf_info_item == NULL) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [InterfaceUpfInfoItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (interface_upf_info_item->interface_type == OpenAPI_up_interface_type_NULL) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [interface_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "interfaceType", OpenAPI_up_interface_type_ToString(interface_upf_info_item->interface_type)) == NULL) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [interface_type]");
        goto end;
    }

    if (interface_upf_info_item->ipv4_endpoint_addresses) {
    cJSON *ipv4_endpoint_addressesList = cJSON_AddArrayToObject(item, "ipv4EndpointAddresses");
    if (ipv4_endpoint_addressesList == NULL) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [ipv4_endpoint_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(interface_upf_info_item->ipv4_endpoint_addresses, node) {
        if (cJSON_AddStringToObject(ipv4_endpoint_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [ipv4_endpoint_addresses]");
            goto end;
        }
    }
    }

    if (interface_upf_info_item->ipv6_endpoint_addresses) {
    cJSON *ipv6_endpoint_addressesList = cJSON_AddArrayToObject(item, "ipv6EndpointAddresses");
    if (ipv6_endpoint_addressesList == NULL) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [ipv6_endpoint_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(interface_upf_info_item->ipv6_endpoint_addresses, node) {
        if (cJSON_AddStringToObject(ipv6_endpoint_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [ipv6_endpoint_addresses]");
            goto end;
        }
    }
    }

    if (interface_upf_info_item->endpoint_fqdn) {
    if (cJSON_AddStringToObject(item, "endpointFqdn", interface_upf_info_item->endpoint_fqdn) == NULL) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [endpoint_fqdn]");
        goto end;
    }
    }

    if (interface_upf_info_item->network_instance) {
    if (cJSON_AddStringToObject(item, "networkInstance", interface_upf_info_item->network_instance) == NULL) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed [network_instance]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_interface_upf_info_item_t *OpenAPI_interface_upf_info_item_parseFromJSON(cJSON *interface_upf_info_itemJSON)
{
    OpenAPI_interface_upf_info_item_t *interface_upf_info_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *interface_type = NULL;
    OpenAPI_up_interface_type_e interface_typeVariable = 0;
    cJSON *ipv4_endpoint_addresses = NULL;
    OpenAPI_list_t *ipv4_endpoint_addressesList = NULL;
    cJSON *ipv6_endpoint_addresses = NULL;
    OpenAPI_list_t *ipv6_endpoint_addressesList = NULL;
    cJSON *endpoint_fqdn = NULL;
    cJSON *network_instance = NULL;
    interface_type = cJSON_GetObjectItemCaseSensitive(interface_upf_info_itemJSON, "interfaceType");
    if (!interface_type) {
        ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [interface_type]");
        goto end;
    }
    if (!cJSON_IsString(interface_type)) {
        ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [interface_type]");
        goto end;
    }
    interface_typeVariable = OpenAPI_up_interface_type_FromString(interface_type->valuestring);

    ipv4_endpoint_addresses = cJSON_GetObjectItemCaseSensitive(interface_upf_info_itemJSON, "ipv4EndpointAddresses");
    if (ipv4_endpoint_addresses) {
        cJSON *ipv4_endpoint_addresses_local = NULL;
        if (!cJSON_IsArray(ipv4_endpoint_addresses)) {
            ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [ipv4_endpoint_addresses]");
            goto end;
        }

        ipv4_endpoint_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_endpoint_addresses_local, ipv4_endpoint_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_endpoint_addresses_local)) {
                ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [ipv4_endpoint_addresses]");
                goto end;
            }
            OpenAPI_list_add(ipv4_endpoint_addressesList, ogs_strdup(ipv4_endpoint_addresses_local->valuestring));
        }
    }

    ipv6_endpoint_addresses = cJSON_GetObjectItemCaseSensitive(interface_upf_info_itemJSON, "ipv6EndpointAddresses");
    if (ipv6_endpoint_addresses) {
        cJSON *ipv6_endpoint_addresses_local = NULL;
        if (!cJSON_IsArray(ipv6_endpoint_addresses)) {
            ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [ipv6_endpoint_addresses]");
            goto end;
        }

        ipv6_endpoint_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_endpoint_addresses_local, ipv6_endpoint_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_endpoint_addresses_local)) {
                ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [ipv6_endpoint_addresses]");
                goto end;
            }
            OpenAPI_list_add(ipv6_endpoint_addressesList, ogs_strdup(ipv6_endpoint_addresses_local->valuestring));
        }
    }

    endpoint_fqdn = cJSON_GetObjectItemCaseSensitive(interface_upf_info_itemJSON, "endpointFqdn");
    if (endpoint_fqdn) {
    if (!cJSON_IsString(endpoint_fqdn) && !cJSON_IsNull(endpoint_fqdn)) {
        ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [endpoint_fqdn]");
        goto end;
    }
    }

    network_instance = cJSON_GetObjectItemCaseSensitive(interface_upf_info_itemJSON, "networkInstance");
    if (network_instance) {
    if (!cJSON_IsString(network_instance) && !cJSON_IsNull(network_instance)) {
        ogs_error("OpenAPI_interface_upf_info_item_parseFromJSON() failed [network_instance]");
        goto end;
    }
    }

    interface_upf_info_item_local_var = OpenAPI_interface_upf_info_item_create (
        interface_typeVariable,
        ipv4_endpoint_addresses ? ipv4_endpoint_addressesList : NULL,
        ipv6_endpoint_addresses ? ipv6_endpoint_addressesList : NULL,
        endpoint_fqdn && !cJSON_IsNull(endpoint_fqdn) ? ogs_strdup(endpoint_fqdn->valuestring) : NULL,
        network_instance && !cJSON_IsNull(network_instance) ? ogs_strdup(network_instance->valuestring) : NULL
    );

    return interface_upf_info_item_local_var;
end:
    if (ipv4_endpoint_addressesList) {
        OpenAPI_list_for_each(ipv4_endpoint_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_endpoint_addressesList);
        ipv4_endpoint_addressesList = NULL;
    }
    if (ipv6_endpoint_addressesList) {
        OpenAPI_list_for_each(ipv6_endpoint_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_endpoint_addressesList);
        ipv6_endpoint_addressesList = NULL;
    }
    return NULL;
}

OpenAPI_interface_upf_info_item_t *OpenAPI_interface_upf_info_item_copy(OpenAPI_interface_upf_info_item_t *dst, OpenAPI_interface_upf_info_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_interface_upf_info_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_interface_upf_info_item_convertToJSON() failed");
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

    OpenAPI_interface_upf_info_item_free(dst);
    dst = OpenAPI_interface_upf_info_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

