
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
    OpenAPI_ip_reachability_e ip_reachability
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
    scp_info_local_var->ip_reachability = ip_reachability;

    return scp_info_local_var;
}

void OpenAPI_scp_info_free(OpenAPI_scp_info_t *scp_info)
{
    if (NULL == scp_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(scp_info->scp_domain_info_list, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(scp_info->scp_domain_info_list);
    ogs_free(scp_info->scp_prefix);
    OpenAPI_list_for_each(scp_info->scp_ports, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(scp_info->scp_ports);
    OpenAPI_list_for_each(scp_info->address_domains, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(scp_info->address_domains);
    OpenAPI_list_for_each(scp_info->ipv4_addresses, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(scp_info->ipv4_addresses);
    OpenAPI_list_for_each(scp_info->ipv6_prefixes, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(scp_info->ipv6_prefixes);
    OpenAPI_list_for_each(scp_info->ipv4_addr_ranges, node) {
        OpenAPI_ipv4_address_range_free(node->data);
    }
    OpenAPI_list_free(scp_info->ipv4_addr_ranges);
    OpenAPI_list_for_each(scp_info->ipv6_prefix_ranges, node) {
        OpenAPI_ipv6_prefix_range_free(node->data);
    }
    OpenAPI_list_free(scp_info->ipv6_prefix_ranges);
    OpenAPI_list_for_each(scp_info->served_nf_set_id_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(scp_info->served_nf_set_id_list);
    OpenAPI_list_for_each(scp_info->remote_plmn_list, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(scp_info->remote_plmn_list);
    ogs_free(scp_info);
}

cJSON *OpenAPI_scp_info_convertToJSON(OpenAPI_scp_info_t *scp_info)
{
    cJSON *item = NULL;

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
    OpenAPI_lnode_t *scp_domain_info_list_node;
    if (scp_info->scp_domain_info_list) {
        OpenAPI_list_for_each(scp_info->scp_domain_info_list, scp_domain_info_list_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)scp_domain_info_list_node->data;
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
    OpenAPI_lnode_t *scp_ports_node;
    if (scp_info->scp_ports) {
        OpenAPI_list_for_each(scp_info->scp_ports, scp_ports_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)scp_ports_node->data;
            }
        }
    }

    if (scp_info->address_domains) {
    cJSON *address_domains = cJSON_AddArrayToObject(item, "addressDomains");
    if (address_domains == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [address_domains]");
        goto end;
    }

    OpenAPI_lnode_t *address_domains_node;
    OpenAPI_list_for_each(scp_info->address_domains, address_domains_node)  {
    if (cJSON_AddStringToObject(address_domains, "", (char*)address_domains_node->data) == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [address_domains]");
        goto end;
    }
                    }
    }

    if (scp_info->ipv4_addresses) {
    cJSON *ipv4_addresses = cJSON_AddArrayToObject(item, "ipv4Addresses");
    if (ipv4_addresses == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv4_addresses]");
        goto end;
    }

    OpenAPI_lnode_t *ipv4_addresses_node;
    OpenAPI_list_for_each(scp_info->ipv4_addresses, ipv4_addresses_node)  {
    if (cJSON_AddStringToObject(ipv4_addresses, "", (char*)ipv4_addresses_node->data) == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv4_addresses]");
        goto end;
    }
                    }
    }

    if (scp_info->ipv6_prefixes) {
    cJSON *ipv6_prefixes = cJSON_AddArrayToObject(item, "ipv6Prefixes");
    if (ipv6_prefixes == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv6_prefixes]");
        goto end;
    }

    OpenAPI_lnode_t *ipv6_prefixes_node;
    OpenAPI_list_for_each(scp_info->ipv6_prefixes, ipv6_prefixes_node)  {
    if (cJSON_AddStringToObject(ipv6_prefixes, "", (char*)ipv6_prefixes_node->data) == NULL) {
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

    OpenAPI_lnode_t *ipv4_addr_ranges_node;
    if (scp_info->ipv4_addr_ranges) {
        OpenAPI_list_for_each(scp_info->ipv4_addr_ranges, ipv4_addr_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(ipv4_addr_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv4_addr_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ipv4_addr_rangesList, itemLocal);
        }
    }
    }

    if (scp_info->ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "ipv6PrefixRanges");
    if (ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *ipv6_prefix_ranges_node;
    if (scp_info->ipv6_prefix_ranges) {
        OpenAPI_list_for_each(scp_info->ipv6_prefix_ranges, ipv6_prefix_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(ipv6_prefix_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [ipv6_prefix_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ipv6_prefix_rangesList, itemLocal);
        }
    }
    }

    if (scp_info->served_nf_set_id_list) {
    cJSON *served_nf_set_id_list = cJSON_AddArrayToObject(item, "servedNfSetIdList");
    if (served_nf_set_id_list == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [served_nf_set_id_list]");
        goto end;
    }

    OpenAPI_lnode_t *served_nf_set_id_list_node;
    OpenAPI_list_for_each(scp_info->served_nf_set_id_list, served_nf_set_id_list_node)  {
    if (cJSON_AddStringToObject(served_nf_set_id_list, "", (char*)served_nf_set_id_list_node->data) == NULL) {
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

    OpenAPI_lnode_t *remote_plmn_list_node;
    if (scp_info->remote_plmn_list) {
        OpenAPI_list_for_each(scp_info->remote_plmn_list, remote_plmn_list_node) {
            cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(remote_plmn_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_scp_info_convertToJSON() failed [remote_plmn_list]");
                goto end;
            }
            cJSON_AddItemToArray(remote_plmn_listList, itemLocal);
        }
    }
    }

    if (scp_info->ip_reachability) {
    if (cJSON_AddStringToObject(item, "ipReachability", OpenAPI_ip_reachability_ToString(scp_info->ip_reachability)) == NULL) {
        ogs_error("OpenAPI_scp_info_convertToJSON() failed [ip_reachability]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_scp_info_t *OpenAPI_scp_info_parseFromJSON(cJSON *scp_infoJSON)
{
    OpenAPI_scp_info_t *scp_info_local_var = NULL;
    cJSON *scp_domain_info_list = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "scpDomainInfoList");

    OpenAPI_list_t *scp_domain_info_listList;
    if (scp_domain_info_list) {
    cJSON *scp_domain_info_list_local_map;
    if (!cJSON_IsObject(scp_domain_info_list)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_domain_info_list]");
        goto end;
    }
    scp_domain_info_listList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(scp_domain_info_list_local_map, scp_domain_info_list) {
        cJSON *localMapObject = scp_domain_info_list_local_map;
        OpenAPI_list_add(scp_domain_info_listList , localMapKeyPair);
    }
    }

    cJSON *scp_prefix = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "scpPrefix");

    if (scp_prefix) {
    if (!cJSON_IsString(scp_prefix)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_prefix]");
        goto end;
    }
    }

    cJSON *scp_ports = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "scpPorts");

    OpenAPI_list_t *scp_portsList;
    if (scp_ports) {
    cJSON *scp_ports_local_map;
    if (!cJSON_IsObject(scp_ports)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [scp_ports]");
        goto end;
    }
    scp_portsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(scp_ports_local_map, scp_ports) {
        cJSON *localMapObject = scp_ports_local_map;
        OpenAPI_list_add(scp_portsList , localMapKeyPair);
    }
    }

    cJSON *address_domains = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "addressDomains");

    OpenAPI_list_t *address_domainsList;
    if (address_domains) {
    cJSON *address_domains_local;
    if (!cJSON_IsArray(address_domains)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [address_domains]");
        goto end;
    }
    address_domainsList = OpenAPI_list_create();

    cJSON_ArrayForEach(address_domains_local, address_domains) {
    if (!cJSON_IsString(address_domains_local)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [address_domains]");
        goto end;
    }
    OpenAPI_list_add(address_domainsList , ogs_strdup(address_domains_local->valuestring));
    }
    }

    cJSON *ipv4_addresses = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv4Addresses");

    OpenAPI_list_t *ipv4_addressesList;
    if (ipv4_addresses) {
    cJSON *ipv4_addresses_local;
    if (!cJSON_IsArray(ipv4_addresses)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addresses]");
        goto end;
    }
    ipv4_addressesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv4_addresses_local, ipv4_addresses) {
    if (!cJSON_IsString(ipv4_addresses_local)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addresses]");
        goto end;
    }
    OpenAPI_list_add(ipv4_addressesList , ogs_strdup(ipv4_addresses_local->valuestring));
    }
    }

    cJSON *ipv6_prefixes = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv6Prefixes");

    OpenAPI_list_t *ipv6_prefixesList;
    if (ipv6_prefixes) {
    cJSON *ipv6_prefixes_local;
    if (!cJSON_IsArray(ipv6_prefixes)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefixes]");
        goto end;
    }
    ipv6_prefixesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv6_prefixes_local, ipv6_prefixes) {
    if (!cJSON_IsString(ipv6_prefixes_local)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefixes]");
        goto end;
    }
    OpenAPI_list_add(ipv6_prefixesList , ogs_strdup(ipv6_prefixes_local->valuestring));
    }
    }

    cJSON *ipv4_addr_ranges = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv4AddrRanges");

    OpenAPI_list_t *ipv4_addr_rangesList;
    if (ipv4_addr_ranges) {
    cJSON *ipv4_addr_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ipv4_addr_ranges)){
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addr_ranges]");
        goto end;
    }

    ipv4_addr_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv4_addr_ranges_local_nonprimitive, ipv4_addr_ranges ) {
        if (!cJSON_IsObject(ipv4_addr_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv4_addr_ranges]");
            goto end;
        }
        OpenAPI_ipv4_address_range_t *ipv4_addr_rangesItem = OpenAPI_ipv4_address_range_parseFromJSON(ipv4_addr_ranges_local_nonprimitive);

        if (!ipv4_addr_rangesItem) {
            ogs_error("No ipv4_addr_rangesItem");
            OpenAPI_list_free(ipv4_addr_rangesList);
            goto end;
        }

        OpenAPI_list_add(ipv4_addr_rangesList, ipv4_addr_rangesItem);
    }
    }

    cJSON *ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipv6PrefixRanges");

    OpenAPI_list_t *ipv6_prefix_rangesList;
    if (ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ipv6_prefix_ranges)){
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }

    ipv6_prefix_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv6_prefix_ranges_local_nonprimitive, ipv6_prefix_ranges ) {
        if (!cJSON_IsObject(ipv6_prefix_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }
        OpenAPI_ipv6_prefix_range_t *ipv6_prefix_rangesItem = OpenAPI_ipv6_prefix_range_parseFromJSON(ipv6_prefix_ranges_local_nonprimitive);

        if (!ipv6_prefix_rangesItem) {
            ogs_error("No ipv6_prefix_rangesItem");
            OpenAPI_list_free(ipv6_prefix_rangesList);
            goto end;
        }

        OpenAPI_list_add(ipv6_prefix_rangesList, ipv6_prefix_rangesItem);
    }
    }

    cJSON *served_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "servedNfSetIdList");

    OpenAPI_list_t *served_nf_set_id_listList;
    if (served_nf_set_id_list) {
    cJSON *served_nf_set_id_list_local;
    if (!cJSON_IsArray(served_nf_set_id_list)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [served_nf_set_id_list]");
        goto end;
    }
    served_nf_set_id_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(served_nf_set_id_list_local, served_nf_set_id_list) {
    if (!cJSON_IsString(served_nf_set_id_list_local)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [served_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_add(served_nf_set_id_listList , ogs_strdup(served_nf_set_id_list_local->valuestring));
    }
    }

    cJSON *remote_plmn_list = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "remotePlmnList");

    OpenAPI_list_t *remote_plmn_listList;
    if (remote_plmn_list) {
    cJSON *remote_plmn_list_local_nonprimitive;
    if (!cJSON_IsArray(remote_plmn_list)){
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [remote_plmn_list]");
        goto end;
    }

    remote_plmn_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(remote_plmn_list_local_nonprimitive, remote_plmn_list ) {
        if (!cJSON_IsObject(remote_plmn_list_local_nonprimitive)) {
            ogs_error("OpenAPI_scp_info_parseFromJSON() failed [remote_plmn_list]");
            goto end;
        }
        OpenAPI_plmn_id_t *remote_plmn_listItem = OpenAPI_plmn_id_parseFromJSON(remote_plmn_list_local_nonprimitive);

        if (!remote_plmn_listItem) {
            ogs_error("No remote_plmn_listItem");
            OpenAPI_list_free(remote_plmn_listList);
            goto end;
        }

        OpenAPI_list_add(remote_plmn_listList, remote_plmn_listItem);
    }
    }

    cJSON *ip_reachability = cJSON_GetObjectItemCaseSensitive(scp_infoJSON, "ipReachability");

    OpenAPI_ip_reachability_e ip_reachabilityVariable;
    if (ip_reachability) {
    if (!cJSON_IsString(ip_reachability)) {
        ogs_error("OpenAPI_scp_info_parseFromJSON() failed [ip_reachability]");
        goto end;
    }
    ip_reachabilityVariable = OpenAPI_ip_reachability_FromString(ip_reachability->valuestring);
    }

    scp_info_local_var = OpenAPI_scp_info_create (
        scp_domain_info_list ? scp_domain_info_listList : NULL,
        scp_prefix ? ogs_strdup(scp_prefix->valuestring) : NULL,
        scp_ports ? scp_portsList : NULL,
        address_domains ? address_domainsList : NULL,
        ipv4_addresses ? ipv4_addressesList : NULL,
        ipv6_prefixes ? ipv6_prefixesList : NULL,
        ipv4_addr_ranges ? ipv4_addr_rangesList : NULL,
        ipv6_prefix_ranges ? ipv6_prefix_rangesList : NULL,
        served_nf_set_id_list ? served_nf_set_id_listList : NULL,
        remote_plmn_list ? remote_plmn_listList : NULL,
        ip_reachability ? ip_reachabilityVariable : 0
    );

    return scp_info_local_var;
end:
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

