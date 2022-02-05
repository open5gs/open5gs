
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_binding_patch.h"

OpenAPI_pcf_binding_patch_t *OpenAPI_pcf_binding_patch_create(
    char *ipv4_addr,
    char *ip_domain,
    char *ipv6_prefix,
    OpenAPI_list_t *add_ipv6_prefixes,
    char *mac_addr48,
    OpenAPI_list_t *add_mac_addrs,
    char *pcf_id,
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_diam_host,
    char *pcf_diam_realm
)
{
    OpenAPI_pcf_binding_patch_t *pcf_binding_patch_local_var = ogs_malloc(sizeof(OpenAPI_pcf_binding_patch_t));
    ogs_assert(pcf_binding_patch_local_var);

    pcf_binding_patch_local_var->ipv4_addr = ipv4_addr;
    pcf_binding_patch_local_var->ip_domain = ip_domain;
    pcf_binding_patch_local_var->ipv6_prefix = ipv6_prefix;
    pcf_binding_patch_local_var->add_ipv6_prefixes = add_ipv6_prefixes;
    pcf_binding_patch_local_var->mac_addr48 = mac_addr48;
    pcf_binding_patch_local_var->add_mac_addrs = add_mac_addrs;
    pcf_binding_patch_local_var->pcf_id = pcf_id;
    pcf_binding_patch_local_var->pcf_fqdn = pcf_fqdn;
    pcf_binding_patch_local_var->pcf_ip_end_points = pcf_ip_end_points;
    pcf_binding_patch_local_var->pcf_diam_host = pcf_diam_host;
    pcf_binding_patch_local_var->pcf_diam_realm = pcf_diam_realm;

    return pcf_binding_patch_local_var;
}

void OpenAPI_pcf_binding_patch_free(OpenAPI_pcf_binding_patch_t *pcf_binding_patch)
{
    if (NULL == pcf_binding_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pcf_binding_patch->ipv4_addr);
    ogs_free(pcf_binding_patch->ip_domain);
    ogs_free(pcf_binding_patch->ipv6_prefix);
    OpenAPI_list_for_each(pcf_binding_patch->add_ipv6_prefixes, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcf_binding_patch->add_ipv6_prefixes);
    ogs_free(pcf_binding_patch->mac_addr48);
    OpenAPI_list_for_each(pcf_binding_patch->add_mac_addrs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcf_binding_patch->add_mac_addrs);
    ogs_free(pcf_binding_patch->pcf_id);
    ogs_free(pcf_binding_patch->pcf_fqdn);
    OpenAPI_list_for_each(pcf_binding_patch->pcf_ip_end_points, node) {
        OpenAPI_ip_end_point_free(node->data);
    }
    OpenAPI_list_free(pcf_binding_patch->pcf_ip_end_points);
    ogs_free(pcf_binding_patch->pcf_diam_host);
    ogs_free(pcf_binding_patch->pcf_diam_realm);
    ogs_free(pcf_binding_patch);
}

cJSON *OpenAPI_pcf_binding_patch_convertToJSON(OpenAPI_pcf_binding_patch_t *pcf_binding_patch)
{
    cJSON *item = NULL;

    if (pcf_binding_patch == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [PcfBindingPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcf_binding_patch->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", pcf_binding_patch->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (pcf_binding_patch->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", pcf_binding_patch->ip_domain) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (pcf_binding_patch->ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6Prefix", pcf_binding_patch->ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

    if (pcf_binding_patch->add_ipv6_prefixes) {
    cJSON *add_ipv6_prefixes = cJSON_AddArrayToObject(item, "addIpv6Prefixes");
    if (add_ipv6_prefixes == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_ipv6_prefixes]");
        goto end;
    }

    OpenAPI_lnode_t *add_ipv6_prefixes_node;
    OpenAPI_list_for_each(pcf_binding_patch->add_ipv6_prefixes, add_ipv6_prefixes_node)  {
    if (cJSON_AddStringToObject(add_ipv6_prefixes, "", (char*)add_ipv6_prefixes_node->data) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_ipv6_prefixes]");
        goto end;
    }
                    }
    }

    if (pcf_binding_patch->mac_addr48) {
    if (cJSON_AddStringToObject(item, "macAddr48", pcf_binding_patch->mac_addr48) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [mac_addr48]");
        goto end;
    }
    }

    if (pcf_binding_patch->add_mac_addrs) {
    cJSON *add_mac_addrs = cJSON_AddArrayToObject(item, "addMacAddrs");
    if (add_mac_addrs == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_mac_addrs]");
        goto end;
    }

    OpenAPI_lnode_t *add_mac_addrs_node;
    OpenAPI_list_for_each(pcf_binding_patch->add_mac_addrs, add_mac_addrs_node)  {
    if (cJSON_AddStringToObject(add_mac_addrs, "", (char*)add_mac_addrs_node->data) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_mac_addrs]");
        goto end;
    }
                    }
    }

    if (pcf_binding_patch->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_binding_patch->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pcf_binding_patch->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", pcf_binding_patch->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (pcf_binding_patch->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }

    OpenAPI_lnode_t *pcf_ip_end_points_node;
    if (pcf_binding_patch->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_binding_patch->pcf_ip_end_points, pcf_ip_end_points_node) {
            cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(pcf_ip_end_points_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [pcf_ip_end_points]");
                goto end;
            }
            cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
        }
    }
    }

    if (pcf_binding_patch->pcf_diam_host) {
    if (cJSON_AddStringToObject(item, "pcfDiamHost", pcf_binding_patch->pcf_diam_host) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [pcf_diam_host]");
        goto end;
    }
    }

    if (pcf_binding_patch->pcf_diam_realm) {
    if (cJSON_AddStringToObject(item, "pcfDiamRealm", pcf_binding_patch->pcf_diam_realm) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [pcf_diam_realm]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_binding_patch_t *OpenAPI_pcf_binding_patch_parseFromJSON(cJSON *pcf_binding_patchJSON)
{
    OpenAPI_pcf_binding_patch_t *pcf_binding_patch_local_var = NULL;
    cJSON *ipv4_addr = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "ipv4Addr");

    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    cJSON *ip_domain = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "ipDomain");

    if (ip_domain) {
    if (!cJSON_IsString(ip_domain)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    cJSON *ipv6_prefix = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "ipv6Prefix");

    if (ipv6_prefix) {
    if (!cJSON_IsString(ipv6_prefix)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

    cJSON *add_ipv6_prefixes = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "addIpv6Prefixes");

    OpenAPI_list_t *add_ipv6_prefixesList;
    if (add_ipv6_prefixes) {
    cJSON *add_ipv6_prefixes_local;
    if (!cJSON_IsArray(add_ipv6_prefixes)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_ipv6_prefixes]");
        goto end;
    }
    add_ipv6_prefixesList = OpenAPI_list_create();

    cJSON_ArrayForEach(add_ipv6_prefixes_local, add_ipv6_prefixes) {
    if (!cJSON_IsString(add_ipv6_prefixes_local)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_ipv6_prefixes]");
        goto end;
    }
    OpenAPI_list_add(add_ipv6_prefixesList , ogs_strdup(add_ipv6_prefixes_local->valuestring));
    }
    }

    cJSON *mac_addr48 = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "macAddr48");

    if (mac_addr48) {
    if (!cJSON_IsString(mac_addr48)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [mac_addr48]");
        goto end;
    }
    }

    cJSON *add_mac_addrs = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "addMacAddrs");

    OpenAPI_list_t *add_mac_addrsList;
    if (add_mac_addrs) {
    cJSON *add_mac_addrs_local;
    if (!cJSON_IsArray(add_mac_addrs)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_mac_addrs]");
        goto end;
    }
    add_mac_addrsList = OpenAPI_list_create();

    cJSON_ArrayForEach(add_mac_addrs_local, add_mac_addrs) {
    if (!cJSON_IsString(add_mac_addrs_local)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_mac_addrs]");
        goto end;
    }
    OpenAPI_list_add(add_mac_addrsList , ogs_strdup(add_mac_addrs_local->valuestring));
    }
    }

    cJSON *pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfId");

    if (pcf_id) {
    if (!cJSON_IsString(pcf_id)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    cJSON *pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfFqdn");

    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    cJSON *pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfIpEndPoints");

    OpenAPI_list_t *pcf_ip_end_pointsList;
    if (pcf_ip_end_points) {
    cJSON *pcf_ip_end_points_local_nonprimitive;
    if (!cJSON_IsArray(pcf_ip_end_points)){
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_ip_end_points]");
        goto end;
    }

    pcf_ip_end_pointsList = OpenAPI_list_create();

    cJSON_ArrayForEach(pcf_ip_end_points_local_nonprimitive, pcf_ip_end_points ) {
        if (!cJSON_IsObject(pcf_ip_end_points_local_nonprimitive)) {
            ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        OpenAPI_ip_end_point_t *pcf_ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(pcf_ip_end_points_local_nonprimitive);

        if (!pcf_ip_end_pointsItem) {
            ogs_error("No pcf_ip_end_pointsItem");
            OpenAPI_list_free(pcf_ip_end_pointsList);
            goto end;
        }

        OpenAPI_list_add(pcf_ip_end_pointsList, pcf_ip_end_pointsItem);
    }
    }

    cJSON *pcf_diam_host = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfDiamHost");

    if (pcf_diam_host) {
    if (!cJSON_IsString(pcf_diam_host)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_diam_host]");
        goto end;
    }
    }

    cJSON *pcf_diam_realm = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfDiamRealm");

    if (pcf_diam_realm) {
    if (!cJSON_IsString(pcf_diam_realm)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_diam_realm]");
        goto end;
    }
    }

    pcf_binding_patch_local_var = OpenAPI_pcf_binding_patch_create (
        ipv4_addr ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ip_domain ? ogs_strdup(ip_domain->valuestring) : NULL,
        ipv6_prefix ? ogs_strdup(ipv6_prefix->valuestring) : NULL,
        add_ipv6_prefixes ? add_ipv6_prefixesList : NULL,
        mac_addr48 ? ogs_strdup(mac_addr48->valuestring) : NULL,
        add_mac_addrs ? add_mac_addrsList : NULL,
        pcf_id ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_fqdn ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        pcf_diam_host ? ogs_strdup(pcf_diam_host->valuestring) : NULL,
        pcf_diam_realm ? ogs_strdup(pcf_diam_realm->valuestring) : NULL
    );

    return pcf_binding_patch_local_var;
end:
    return NULL;
}

OpenAPI_pcf_binding_patch_t *OpenAPI_pcf_binding_patch_copy(OpenAPI_pcf_binding_patch_t *dst, OpenAPI_pcf_binding_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_binding_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed");
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

    OpenAPI_pcf_binding_patch_free(dst);
    dst = OpenAPI_pcf_binding_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

