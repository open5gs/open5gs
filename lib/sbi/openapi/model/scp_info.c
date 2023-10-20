
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_info.h"

OpenAPI_scp_info_t *OpenAPI_scp_info_create(
    OpenAPI_list_t* scp_domain_info_list,
    char *scp_prefix,
    OpenAPI_list_t* scp_ports,
    OpenAPI_list_t *address_domains,
    OpenAPI_list_t *ipv4_addresses,
    OpenAPI_list_t *ipv6_prefixes,
    OpenAPI_list_t *ipv4_addr_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    OpenAPI_list_t *served_nf_set_id_list,
    OpenAPI_list_t *remote_plmn_list,
    OpenAPI_list_t *remote_snpn_list,
    OpenAPI_ip_reachability_e ip_reachability,
    OpenAPI_list_t *scp_capabilities
)
{
    OpenAPI_scp_info_t *scp_info_local_var = ogs_malloc(sizeof(OpenAPI_scp_info_t));
    ogs_assert(scp_info_local_var);

    scp_info_local_var->scp_domain_info_list = scp_domain_info_list;
    scp_info_local_var->scp_prefix = scp_prefix;
    scp_info_local_var->scp_ports = scp_ports;
    scp_info_local_var->address_domains = address_domains;
    scp_info_local_var->ipv4_addresses = ipv4_addresses;
    scp_info_local_var->ipv6_prefixes = ipv6_prefixes;
    scp_info_local_var->ipv4_addr_ranges = ipv4_addr_ranges;
    scp_info_local_var->ipv6_prefix_ranges = ipv6_prefix_ranges;
    scp_info_local_var->served_nf_set_id_list = served_nf_set_id_list;
    scp_info_local_var->remote_plmn_list = remote_plmn_list;
    scp_info_local_var->remote_snpn_list = remote_snpn_list;
    scp_info_local_var->ip_reachability = ip_reachability;
    scp_info_local_var->scp_capabilities = scp_capabilities;

    return scp_info_local_var;
}

void OpenAPI_scp_info_free(OpenAPI_scp_info_t *scp_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scp_info) {
        return;
    }
    if (scp_info->scp_domain_info_list) {
        OpenAPI_list_for_each(scp_info->scp_domain_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_scp_domain_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_info->scp_domain_info_list);
        scp_info->scp_domain_info_list = NULL;
    }
    if (scp_info->scp_prefix) {
        ogs_free(scp_info->scp_prefix);
        scp_info->scp_prefix = NULL;
    }
    if (scp_info->scp_ports) {
        OpenAPI_list_for_each(scp_info->scp_ports, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_info->scp_ports);
        scp_info->scp_ports = NULL;
    }
    if (scp_info->address_domains) {
        OpenAPI_list_for_each(scp_info->address_domains, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_info->address_domains);
        scp_info->address_domains = NULL;
    }
    if (scp_info->ipv4_addresses) {
        OpenAPI_list_for_each(scp_info->ipv4_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_info->ipv4_addresses);
        scp_info->ipv4_addresses = NULL;
    }
    if (scp_info->ipv6_prefixes) {
        OpenAPI_list_for_each(scp_info->ipv6_prefixes, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_info->ipv6_prefixes);
        scp_info->ipv6_prefixes = NULL;
    }
    if (scp_info->ipv4_addr_ranges) {
        OpenAPI_list_for_each(scp_info->ipv4_addr_ranges, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(scp_info->ipv4_addr_ranges);
        scp_info->ipv4_addr_ranges = NULL;
    }
    if (scp_info->ipv6_prefix_ranges) {
        OpenAPI_list_for_each(scp_info->ipv6_prefix_ranges, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(scp_info->ipv6_prefix_ranges);
        scp_info->ipv6_prefix_ranges = NULL;
    }
    if (scp_info->served_nf_set_id_list) {
        OpenAPI_list_for_each(scp_info->served_nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_info->served_nf_set_id_list);
        scp_info->served_nf_set_id_list = NULL;
    }
    if (scp_info->remote_plmn_list) {
        OpenAPI_list_for_each(scp_info->remote_plmn_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(scp_info->remote_plmn_list);
        scp_info->remote_plmn_list = NULL;
    }
    if (scp_info->remote_snpn_list) {
        OpenAPI_list_for_each(scp_info->remote_snpn_list, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(scp_info->remote_snpn_list);
        scp_info->remote_snpn_list = NULL;
    }
    if (scp_info->scp_capabilities) {
        OpenAPI_list_free(scp_info->scp_capabilities);
        scp_info->scp_capabilities = NULL;
    }
    ogs_free(scp_info);
}

cJSON *OpenAPI_scp_info_convertToJSON(OpenAPI_scp_info_t *scp_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scp_info == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ScpInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (scp_info->scp_domain_info_list) {
    cJSON *scp_domain_info_list = cJSON_AddObjectToObject(item, "scpDomainInfoList");
    if (scp_domain_info_list == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_domain_info_list]");
        goto end;
    }
    cJSON *localMapObject = scp_domain_info_list;
    if (scp_info->scp_domain_info_list) {
        OpenAPI_list_for_each(scp_info->scp_domain_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_domain_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_domain_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_scp_domain_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (scp_info->scp_prefix) {
    if (cJSON_AddStringToObject(item, "scpPrefix", scp_info->scp_prefix) == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_prefix]");
        goto end;
    }
    }

    if (scp_info->scp_ports) {
    cJSON *scp_ports = cJSON_AddObjectToObject(item, "scpPorts");
    if (scp_ports == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_ports]");
        goto end;
    }
    cJSON *localMapObject = scp_ports;
    if (scp_info->scp_ports) {
        OpenAPI_list_for_each(scp_info->scp_ports, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_ports]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_ports]");
                goto end;
            }
            if (localKeyValue->value == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [inner]");
                goto end;
            }
            if (cJSON_AddNumberToObject(localMapObject, localKeyValue->key, *(double *)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (scp_info->address_domains) {
    cJSON *address_domainsList = cJSON_AddArrayToObject(item, "addressDomains");
    if (address_domainsList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [address_domains]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->address_domains, node) {
        if (cJSON_AddStringToObject(address_domainsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [address_domains]");
            goto end;
        }
    }
    }

    if (scp_info->ipv4_addresses) {
    cJSON *ipv4_addressesList = cJSON_AddArrayToObject(item, "ipv4Addresses");
    if (ipv4_addressesList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv4_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->ipv4_addresses, node) {
        if (cJSON_AddStringToObject(ipv4_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv4_addresses]");
            goto end;
        }
    }
    }

    if (scp_info->ipv6_prefixes) {
    cJSON *ipv6_prefixesList = cJSON_AddArrayToObject(item, "ipv6Prefixes");
    if (ipv6_prefixesList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv6_prefixes]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->ipv6_prefixes, node) {
        if (cJSON_AddStringToObject(ipv6_prefixesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv6_prefixes]");
            goto end;
        }
    }
    }

    if (scp_info->ipv4_addr_ranges) {
    cJSON *ipv4_addr_rangesList = cJSON_AddArrayToObject(item, "ipv4AddrRanges");
    if (ipv4_addr_rangesList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv4_addr_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->ipv4_addr_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv4_addr_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_addr_rangesList, itemLocal);
    }
    }

    if (scp_info->ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "ipv6PrefixRanges");
    if (ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->ipv6_prefix_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_prefix_rangesList, itemLocal);
    }
    }

    if (scp_info->served_nf_set_id_list) {
    cJSON *served_nf_set_id_listList = cJSON_AddArrayToObject(item, "servedNfSetIdList");
    if (served_nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [served_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->served_nf_set_id_list, node) {
        if (cJSON_AddStringToObject(served_nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [served_nf_set_id_list]");
            goto end;
        }
    }
    }

    if (scp_info->remote_plmn_list) {
    cJSON *remote_plmn_listList = cJSON_AddArrayToObject(item, "remotePlmnList");
    if (remote_plmn_listList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [remote_plmn_list]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->remote_plmn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [remote_plmn_list]");
            goto end;
        }
        cJSON_AddItemToArray(remote_plmn_listList, itemLocal);
    }
    }

    if (scp_info->remote_snpn_list) {
    cJSON *remote_snpn_listList = cJSON_AddArrayToObject(item, "remoteSnpnList");
    if (remote_snpn_listList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [remote_snpn_list]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->remote_snpn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [remote_snpn_list]");
            goto end;
        }
        cJSON_AddItemToArray(remote_snpn_listList, itemLocal);
    }
    }

    if (scp_info->ip_reachability != OpenAPI_ip_reachability_NULL) {
    if (cJSON_AddStringToObject(item, "ipReachability", OpenAPI_ip_reachability_ToString(scp_info->ip_reachability)) == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ip_reachability]");
        goto end;
    }
    }

    if (scp_info->scp_capabilities != OpenAPI_scp_capability_NULL) {
    cJSON *scp_capabilitiesList = cJSON_AddArrayToObject(item, "scpCapabilities");
    if (scp_capabilitiesList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_capabilities]");
        goto end;
    }
    OpenAPI_list_for_each(scp_info->scp_capabilities, node) {
        if (cJSON_AddStringToObject(scp_capabilitiesList, "", OpenAPI_scp_capability_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_scp_info_convertToJSON() failed [scp_capabilities]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_scp_info_t *OpenAPI_scp_info_parseFromJSON(cJSON *scp_infoJSON)
{
    OpenAPI_scp_info_t *scp_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *scp_domain_info_list = NULL;
    OpenAPI_list_t *scp_domain_info_listList = NULL;
    cJSON *scp_prefix = NULL;
    cJSON *scp_ports = NULL;
    OpenAPI_list_t *scp_portsList = NULL;
    cJSON *address_domains = NULL;
    OpenAPI_list_t *address_domainsList = NULL;
    cJSON *ipv4_addresses = NULL;
    OpenAPI_list_t *ipv4_addressesList = NULL;
    cJSON *ipv6_prefixes = NULL;
    OpenAPI_list_t *ipv6_prefixesList = NULL;
    cJSON *ipv4_addr_ranges = NULL;
    OpenAPI_list_t *ipv4_addr_rangesList = NULL;
    cJSON *ipv6_prefix_ranges = NULL;
    OpenAPI_list_t *ipv6_prefix_rangesList = NULL;
    cJSON *served_nf_set_id_list = NULL;
    OpenAPI_list_t *served_nf_set_id_listList = NULL;
    cJSON *remote_plmn_list = NULL;
    OpenAPI_list_t *remote_plmn_listList = NULL;
    cJSON *remote_snpn_list = NULL;
    OpenAPI_list_t *remote_snpn_listList = NULL;
    cJSON *ip_reachability = NULL;
    OpenAPI_ip_reachability_e ip_reachabilityVariable = 0;
    cJSON *scp_capabilities = NULL;
    OpenAPI_list_t *scp_capabilitiesList = NULL;
    scp_domain_info_list = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "scpDomainInfoList");
    if (scp_domain_info_list) {
        cJSON *scp_domain_info_list_local_map = NULL;
        if (!cJSON_IsObject(scp_domain_info_list) && !cJSON_IsNull(scp_domain_info_list)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_domain_info_list]");
            goto end;
        }
        if (cJSON_IsObject(scp_domain_info_list)) {
            scp_domain_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(scp_domain_info_list_local_map, scp_domain_info_list) {
                cJSON *localMapObject = scp_domain_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_scp_domain_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_scp_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(scp_domain_info_listList, localMapKeyPair);
            }
        }
    }

    scp_prefix = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "scpPrefix");
    if (scp_prefix) {
    if (!cJSON_IsString(scp_prefix) && !cJSON_IsNull(scp_prefix)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_prefix]");
        goto end;
    }
    }

    scp_ports = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "scpPorts");
    if (scp_ports) {
        cJSON *scp_ports_local_map = NULL;
        if (!cJSON_IsObject(scp_ports) && !cJSON_IsNull(scp_ports)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_ports]");
            goto end;
        }
        if (cJSON_IsObject(scp_ports)) {
            scp_portsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(scp_ports_local_map, scp_ports) {
                cJSON *localMapObject = scp_ports_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsNumber(localMapObject)) {
                    ogs_error("OpenAPI_scp_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                localDouble = (double *)ogs_calloc(1, sizeof(double));
                if (!localDouble) {
                    ogs_error("OpenAPI_scp_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localDouble = localMapObject->valuedouble;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localDouble);
                OpenAPI_list_add(scp_portsList, localMapKeyPair);
            }
        }
    }

    address_domains = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "addressDomains");
    if (address_domains) {
        cJSON *address_domains_local = NULL;
        if (!cJSON_IsArray(address_domains)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [address_domains]");
            goto end;
        }

        address_domainsList = OpenAPI_list_create();

        cJSON_ArrayForEach(address_domains_local, address_domains) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(address_domains_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [address_domains]");
                goto end;
            }
            OpenAPI_list_add(address_domainsList, ogs_strdup(address_domains_local->valuestring));
        }
    }

    ipv4_addresses = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv4Addresses");
    if (ipv4_addresses) {
        cJSON *ipv4_addresses_local = NULL;
        if (!cJSON_IsArray(ipv4_addresses)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addresses]");
            goto end;
        }

        ipv4_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addresses_local, ipv4_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_addresses_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addresses]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addressesList, ogs_strdup(ipv4_addresses_local->valuestring));
        }
    }

    ipv6_prefixes = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv6Prefixes");
    if (ipv6_prefixes) {
        cJSON *ipv6_prefixes_local = NULL;
        if (!cJSON_IsArray(ipv6_prefixes)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefixes]");
            goto end;
        }

        ipv6_prefixesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefixes_local, ipv6_prefixes) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_prefixes_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefixes]");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefixesList, ogs_strdup(ipv6_prefixes_local->valuestring));
        }
    }

    ipv4_addr_ranges = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv4AddrRanges");
    if (ipv4_addr_ranges) {
        cJSON *ipv4_addr_ranges_local = NULL;
        if (!cJSON_IsArray(ipv4_addr_ranges)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addr_ranges]");
            goto end;
        }

        ipv4_addr_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addr_ranges_local, ipv4_addr_ranges) {
            if (!cJSON_IsObject(ipv4_addr_ranges_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addr_ranges]");
                goto end;
            }
            OpenAPI_ipv4_address_range_t *ipv4_addr_rangesItem = OpenAPI_ipv4_address_range_parseFromJSON(ipv4_addr_ranges_local);
            if (!ipv4_addr_rangesItem) {
                ogs_error("No ipv4_addr_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ipv4_addr_rangesList, ipv4_addr_rangesItem);
        }
    }

    ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv6PrefixRanges");
    if (ipv6_prefix_ranges) {
        cJSON *ipv6_prefix_ranges_local = NULL;
        if (!cJSON_IsArray(ipv6_prefix_ranges)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }

        ipv6_prefix_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_ranges_local, ipv6_prefix_ranges) {
            if (!cJSON_IsObject(ipv6_prefix_ranges_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefix_ranges]");
                goto end;
            }
            OpenAPI_ipv6_prefix_range_t *ipv6_prefix_rangesItem = OpenAPI_ipv6_prefix_range_parseFromJSON(ipv6_prefix_ranges_local);
            if (!ipv6_prefix_rangesItem) {
                ogs_error("No ipv6_prefix_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefix_rangesList, ipv6_prefix_rangesItem);
        }
    }

    served_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "servedNfSetIdList");
    if (served_nf_set_id_list) {
        cJSON *served_nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(served_nf_set_id_list)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [served_nf_set_id_list]");
            goto end;
        }

        served_nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(served_nf_set_id_list_local, served_nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(served_nf_set_id_list_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [served_nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(served_nf_set_id_listList, ogs_strdup(served_nf_set_id_list_local->valuestring));
        }
    }

    remote_plmn_list = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "remotePlmnList");
    if (remote_plmn_list) {
        cJSON *remote_plmn_list_local = NULL;
        if (!cJSON_IsArray(remote_plmn_list)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [remote_plmn_list]");
            goto end;
        }

        remote_plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(remote_plmn_list_local, remote_plmn_list) {
            if (!cJSON_IsObject(remote_plmn_list_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [remote_plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *remote_plmn_listItem = OpenAPI_plmn_id_parseFromJSON(remote_plmn_list_local);
            if (!remote_plmn_listItem) {
                ogs_error("No remote_plmn_listItem");
                goto end;
            }
            OpenAPI_list_add(remote_plmn_listList, remote_plmn_listItem);
        }
    }

    remote_snpn_list = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "remoteSnpnList");
    if (remote_snpn_list) {
        cJSON *remote_snpn_list_local = NULL;
        if (!cJSON_IsArray(remote_snpn_list)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [remote_snpn_list]");
            goto end;
        }

        remote_snpn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(remote_snpn_list_local, remote_snpn_list) {
            if (!cJSON_IsObject(remote_snpn_list_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [remote_snpn_list]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *remote_snpn_listItem = OpenAPI_plmn_id_nid_parseFromJSON(remote_snpn_list_local);
            if (!remote_snpn_listItem) {
                ogs_error("No remote_snpn_listItem");
                goto end;
            }
            OpenAPI_list_add(remote_snpn_listList, remote_snpn_listItem);
        }
    }

    ip_reachability = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipReachability");
    if (ip_reachability) {
    if (!cJSON_IsString(ip_reachability)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ip_reachability]");
        goto end;
    }
    ip_reachabilityVariable = OpenAPI_ip_reachability_FromString(ip_reachability->valuestring);
    }

    scp_capabilities = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "scpCapabilities");
    if (scp_capabilities) {
        cJSON *scp_capabilities_local = NULL;
        if (!cJSON_IsArray(scp_capabilities)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_capabilities]");
            goto end;
        }

        scp_capabilitiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(scp_capabilities_local, scp_capabilities) {
            OpenAPI_scp_capability_e localEnum = OpenAPI_scp_capability_NULL;
            if (!cJSON_IsString(scp_capabilities_local)) {
                ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_capabilities]");
                goto end;
            }
            localEnum = OpenAPI_scp_capability_FromString(scp_capabilities_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"scp_capabilities\" is not supported. Ignoring it ...",
                         scp_capabilities_local->valuestring);
            } else {
                OpenAPI_list_add(scp_capabilitiesList, (void *)localEnum);
            }
        }
        if (scp_capabilitiesList->count == 0) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed: Expected scp_capabilitiesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    scp_info_local_var = OpenAPI_scp_info_create (
        scp_domain_info_list ? scp_domain_info_listList : NULL,
        scp_prefix && !cJSON_IsNull(scp_prefix) ? ogs_strdup(scp_prefix->valuestring) : NULL,
        scp_ports ? scp_portsList : NULL,
        address_domains ? address_domainsList : NULL,
        ipv4_addresses ? ipv4_addressesList : NULL,
        ipv6_prefixes ? ipv6_prefixesList : NULL,
        ipv4_addr_ranges ? ipv4_addr_rangesList : NULL,
        ipv6_prefix_ranges ? ipv6_prefix_rangesList : NULL,
        served_nf_set_id_list ? served_nf_set_id_listList : NULL,
        remote_plmn_list ? remote_plmn_listList : NULL,
        remote_snpn_list ? remote_snpn_listList : NULL,
        ip_reachability ? ip_reachabilityVariable : 0,
        scp_capabilities ? scp_capabilitiesList : NULL
    );

    return scp_info_local_var;
end:
    if (scp_domain_info_listList) {
        OpenAPI_list_for_each(scp_domain_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_scp_domain_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_domain_info_listList);
        scp_domain_info_listList = NULL;
    }
    if (scp_portsList) {
        OpenAPI_list_for_each(scp_portsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_portsList);
        scp_portsList = NULL;
    }
    if (address_domainsList) {
        OpenAPI_list_for_each(address_domainsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(address_domainsList);
        address_domainsList = NULL;
    }
    if (ipv4_addressesList) {
        OpenAPI_list_for_each(ipv4_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_addressesList);
        ipv4_addressesList = NULL;
    }
    if (ipv6_prefixesList) {
        OpenAPI_list_for_each(ipv6_prefixesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefixesList);
        ipv6_prefixesList = NULL;
    }
    if (ipv4_addr_rangesList) {
        OpenAPI_list_for_each(ipv4_addr_rangesList, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(ipv4_addr_rangesList);
        ipv4_addr_rangesList = NULL;
    }
    if (ipv6_prefix_rangesList) {
        OpenAPI_list_for_each(ipv6_prefix_rangesList, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefix_rangesList);
        ipv6_prefix_rangesList = NULL;
    }
    if (served_nf_set_id_listList) {
        OpenAPI_list_for_each(served_nf_set_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(served_nf_set_id_listList);
        served_nf_set_id_listList = NULL;
    }
    if (remote_plmn_listList) {
        OpenAPI_list_for_each(remote_plmn_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(remote_plmn_listList);
        remote_plmn_listList = NULL;
    }
    if (remote_snpn_listList) {
        OpenAPI_list_for_each(remote_snpn_listList, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(remote_snpn_listList);
        remote_snpn_listList = NULL;
    }
    if (scp_capabilitiesList) {
        OpenAPI_list_free(scp_capabilitiesList);
        scp_capabilitiesList = NULL;
    }
    return NULL;
}

OpenAPI_scp_info_t *OpenAPI_scp_info_copy(OpenAPI_scp_info_t *dst, OpenAPI_scp_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scp_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed");
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

    OpenAPI_scp_info_free(dst);
    dst = OpenAPI_scp_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

