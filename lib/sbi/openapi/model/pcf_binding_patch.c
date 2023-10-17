
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_binding_patch.h"

OpenAPI_pcf_binding_patch_t *OpenAPI_pcf_binding_patch_create(
    bool is_ipv4_addr_null,
    char *ipv4_addr,
    bool is_ip_domain_null,
    char *ip_domain,
    bool is_ipv6_prefix_null,
    char *ipv6_prefix,
    bool is_add_ipv6_prefixes_null,
    OpenAPI_list_t *add_ipv6_prefixes,
    bool is_mac_addr48_null,
    char *mac_addr48,
    bool is_add_mac_addrs_null,
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

    pcf_binding_patch_local_var->is_ipv4_addr_null = is_ipv4_addr_null;
    pcf_binding_patch_local_var->ipv4_addr = ipv4_addr;
    pcf_binding_patch_local_var->is_ip_domain_null = is_ip_domain_null;
    pcf_binding_patch_local_var->ip_domain = ip_domain;
    pcf_binding_patch_local_var->is_ipv6_prefix_null = is_ipv6_prefix_null;
    pcf_binding_patch_local_var->ipv6_prefix = ipv6_prefix;
    pcf_binding_patch_local_var->is_add_ipv6_prefixes_null = is_add_ipv6_prefixes_null;
    pcf_binding_patch_local_var->add_ipv6_prefixes = add_ipv6_prefixes;
    pcf_binding_patch_local_var->is_mac_addr48_null = is_mac_addr48_null;
    pcf_binding_patch_local_var->mac_addr48 = mac_addr48;
    pcf_binding_patch_local_var->is_add_mac_addrs_null = is_add_mac_addrs_null;
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
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_binding_patch) {
        return;
    }
    if (pcf_binding_patch->ipv4_addr) {
        ogs_free(pcf_binding_patch->ipv4_addr);
        pcf_binding_patch->ipv4_addr = NULL;
    }
    if (pcf_binding_patch->ip_domain) {
        ogs_free(pcf_binding_patch->ip_domain);
        pcf_binding_patch->ip_domain = NULL;
    }
    if (pcf_binding_patch->ipv6_prefix) {
        ogs_free(pcf_binding_patch->ipv6_prefix);
        pcf_binding_patch->ipv6_prefix = NULL;
    }
    if (pcf_binding_patch->add_ipv6_prefixes) {
        OpenAPI_list_for_each(pcf_binding_patch->add_ipv6_prefixes, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_binding_patch->add_ipv6_prefixes);
        pcf_binding_patch->add_ipv6_prefixes = NULL;
    }
    if (pcf_binding_patch->mac_addr48) {
        ogs_free(pcf_binding_patch->mac_addr48);
        pcf_binding_patch->mac_addr48 = NULL;
    }
    if (pcf_binding_patch->add_mac_addrs) {
        OpenAPI_list_for_each(pcf_binding_patch->add_mac_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_binding_patch->add_mac_addrs);
        pcf_binding_patch->add_mac_addrs = NULL;
    }
    if (pcf_binding_patch->pcf_id) {
        ogs_free(pcf_binding_patch->pcf_id);
        pcf_binding_patch->pcf_id = NULL;
    }
    if (pcf_binding_patch->pcf_fqdn) {
        ogs_free(pcf_binding_patch->pcf_fqdn);
        pcf_binding_patch->pcf_fqdn = NULL;
    }
    if (pcf_binding_patch->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_binding_patch->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_binding_patch->pcf_ip_end_points);
        pcf_binding_patch->pcf_ip_end_points = NULL;
    }
    if (pcf_binding_patch->pcf_diam_host) {
        ogs_free(pcf_binding_patch->pcf_diam_host);
        pcf_binding_patch->pcf_diam_host = NULL;
    }
    if (pcf_binding_patch->pcf_diam_realm) {
        ogs_free(pcf_binding_patch->pcf_diam_realm);
        pcf_binding_patch->pcf_diam_realm = NULL;
    }
    ogs_free(pcf_binding_patch);
}

cJSON *OpenAPI_pcf_binding_patch_convertToJSON(OpenAPI_pcf_binding_patch_t *pcf_binding_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    } else if (pcf_binding_patch->is_ipv4_addr_null) {
        if (cJSON_AddNullToObject(item, "ipv4Addr") == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    if (pcf_binding_patch->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", pcf_binding_patch->ip_domain) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ip_domain]");
        goto end;
    }
    } else if (pcf_binding_patch->is_ip_domain_null) {
        if (cJSON_AddNullToObject(item, "ipDomain") == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ip_domain]");
            goto end;
        }
    }

    if (pcf_binding_patch->ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6Prefix", pcf_binding_patch->ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ipv6_prefix]");
        goto end;
    }
    } else if (pcf_binding_patch->is_ipv6_prefix_null) {
        if (cJSON_AddNullToObject(item, "ipv6Prefix") == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [ipv6_prefix]");
            goto end;
        }
    }

    if (pcf_binding_patch->add_ipv6_prefixes) {
    cJSON *add_ipv6_prefixesList = cJSON_AddArrayToObject(item, "addIpv6Prefixes");
    if (add_ipv6_prefixesList == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_ipv6_prefixes]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding_patch->add_ipv6_prefixes, node) {
        if (cJSON_AddStringToObject(add_ipv6_prefixesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_ipv6_prefixes]");
            goto end;
        }
    }
    } else if (pcf_binding_patch->is_add_ipv6_prefixes_null) {
        if (cJSON_AddNullToObject(item, "addIpv6Prefixes") == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_ipv6_prefixes]");
            goto end;
        }
    }

    if (pcf_binding_patch->mac_addr48) {
    if (cJSON_AddStringToObject(item, "macAddr48", pcf_binding_patch->mac_addr48) == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [mac_addr48]");
        goto end;
    }
    } else if (pcf_binding_patch->is_mac_addr48_null) {
        if (cJSON_AddNullToObject(item, "macAddr48") == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [mac_addr48]");
            goto end;
        }
    }

    if (pcf_binding_patch->add_mac_addrs) {
    cJSON *add_mac_addrsList = cJSON_AddArrayToObject(item, "addMacAddrs");
    if (add_mac_addrsList == NULL) {
        ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_mac_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding_patch->add_mac_addrs, node) {
        if (cJSON_AddStringToObject(add_mac_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_mac_addrs]");
            goto end;
        }
    }
    } else if (pcf_binding_patch->is_add_mac_addrs_null) {
        if (cJSON_AddNullToObject(item, "addMacAddrs") == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [add_mac_addrs]");
            goto end;
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
    OpenAPI_list_for_each(pcf_binding_patch->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_binding_patch_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ip_domain = NULL;
    cJSON *ipv6_prefix = NULL;
    cJSON *add_ipv6_prefixes = NULL;
    OpenAPI_list_t *add_ipv6_prefixesList = NULL;
    cJSON *mac_addr48 = NULL;
    cJSON *add_mac_addrs = NULL;
    OpenAPI_list_t *add_mac_addrsList = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    cJSON *pcf_diam_host = NULL;
    cJSON *pcf_diam_realm = NULL;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsNull(ipv4_addr)) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }
    }

    ip_domain = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "ipDomain");
    if (ip_domain) {
    if (!cJSON_IsNull(ip_domain)) {
    if (!cJSON_IsString(ip_domain) && !cJSON_IsNull(ip_domain)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }
    }

    ipv6_prefix = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "ipv6Prefix");
    if (ipv6_prefix) {
    if (!cJSON_IsNull(ipv6_prefix)) {
    if (!cJSON_IsString(ipv6_prefix) && !cJSON_IsNull(ipv6_prefix)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [ipv6_prefix]");
        goto end;
    }
    }
    }

    add_ipv6_prefixes = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "addIpv6Prefixes");
    if (add_ipv6_prefixes) {
    if (!cJSON_IsNull(add_ipv6_prefixes)) {
        cJSON *add_ipv6_prefixes_local = NULL;
        if (!cJSON_IsArray(add_ipv6_prefixes)) {
            ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_ipv6_prefixes]");
            goto end;
        }

        add_ipv6_prefixesList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_ipv6_prefixes_local, add_ipv6_prefixes) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(add_ipv6_prefixes_local)) {
                ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_ipv6_prefixes]");
                goto end;
            }
            OpenAPI_list_add(add_ipv6_prefixesList, ogs_strdup(add_ipv6_prefixes_local->valuestring));
        }
    }
    }

    mac_addr48 = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "macAddr48");
    if (mac_addr48) {
    if (!cJSON_IsNull(mac_addr48)) {
    if (!cJSON_IsString(mac_addr48) && !cJSON_IsNull(mac_addr48)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [mac_addr48]");
        goto end;
    }
    }
    }

    add_mac_addrs = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "addMacAddrs");
    if (add_mac_addrs) {
    if (!cJSON_IsNull(add_mac_addrs)) {
        cJSON *add_mac_addrs_local = NULL;
        if (!cJSON_IsArray(add_mac_addrs)) {
            ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_mac_addrs]");
            goto end;
        }

        add_mac_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_mac_addrs_local, add_mac_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(add_mac_addrs_local)) {
                ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [add_mac_addrs]");
                goto end;
            }
            OpenAPI_list_add(add_mac_addrsList, ogs_strdup(add_mac_addrs_local->valuestring));
        }
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *pcf_ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(pcf_ip_end_points_local);
            if (!pcf_ip_end_pointsItem) {
                ogs_error("No pcf_ip_end_pointsItem");
                goto end;
            }
            OpenAPI_list_add(pcf_ip_end_pointsList, pcf_ip_end_pointsItem);
        }
    }

    pcf_diam_host = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfDiamHost");
    if (pcf_diam_host) {
    if (!cJSON_IsString(pcf_diam_host) && !cJSON_IsNull(pcf_diam_host)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_diam_host]");
        goto end;
    }
    }

    pcf_diam_realm = cJSON_GetObjectItemCaseSensitive(pcf_binding_patchJSON, "pcfDiamRealm");
    if (pcf_diam_realm) {
    if (!cJSON_IsString(pcf_diam_realm) && !cJSON_IsNull(pcf_diam_realm)) {
        ogs_error("OpenAPI_pcf_binding_patch_parseFromJSON() failed [pcf_diam_realm]");
        goto end;
    }
    }

    pcf_binding_patch_local_var = OpenAPI_pcf_binding_patch_create (
        ipv4_addr && cJSON_IsNull(ipv4_addr) ? true : false,
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ip_domain && cJSON_IsNull(ip_domain) ? true : false,
        ip_domain && !cJSON_IsNull(ip_domain) ? ogs_strdup(ip_domain->valuestring) : NULL,
        ipv6_prefix && cJSON_IsNull(ipv6_prefix) ? true : false,
        ipv6_prefix && !cJSON_IsNull(ipv6_prefix) ? ogs_strdup(ipv6_prefix->valuestring) : NULL,
        add_ipv6_prefixes && cJSON_IsNull(add_ipv6_prefixes) ? true : false,
        add_ipv6_prefixes ? add_ipv6_prefixesList : NULL,
        mac_addr48 && cJSON_IsNull(mac_addr48) ? true : false,
        mac_addr48 && !cJSON_IsNull(mac_addr48) ? ogs_strdup(mac_addr48->valuestring) : NULL,
        add_mac_addrs && cJSON_IsNull(add_mac_addrs) ? true : false,
        add_mac_addrs ? add_mac_addrsList : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        pcf_diam_host && !cJSON_IsNull(pcf_diam_host) ? ogs_strdup(pcf_diam_host->valuestring) : NULL,
        pcf_diam_realm && !cJSON_IsNull(pcf_diam_realm) ? ogs_strdup(pcf_diam_realm->valuestring) : NULL
    );

    return pcf_binding_patch_local_var;
end:
    if (add_ipv6_prefixesList) {
        OpenAPI_list_for_each(add_ipv6_prefixesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(add_ipv6_prefixesList);
        add_ipv6_prefixesList = NULL;
    }
    if (add_mac_addrsList) {
        OpenAPI_list_for_each(add_mac_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(add_mac_addrsList);
        add_mac_addrsList = NULL;
    }
    if (pcf_ip_end_pointsList) {
        OpenAPI_list_for_each(pcf_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_ip_end_pointsList);
        pcf_ip_end_pointsList = NULL;
    }
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

