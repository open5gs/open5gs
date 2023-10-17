
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_for_pdu_session_info.h"

OpenAPI_pcf_for_pdu_session_info_t *OpenAPI_pcf_for_pdu_session_info_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    bool is_ipv4_addr_null,
    char *ipv4_addr,
    char *ip_domain,
    OpenAPI_list_t *ipv6_prefixes,
    OpenAPI_list_t *mac_addrs,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level
)
{
    OpenAPI_pcf_for_pdu_session_info_t *pcf_for_pdu_session_info_local_var = ogs_malloc(sizeof(OpenAPI_pcf_for_pdu_session_info_t));
    ogs_assert(pcf_for_pdu_session_info_local_var);

    pcf_for_pdu_session_info_local_var->dnn = dnn;
    pcf_for_pdu_session_info_local_var->snssai = snssai;
    pcf_for_pdu_session_info_local_var->pcf_fqdn = pcf_fqdn;
    pcf_for_pdu_session_info_local_var->pcf_ip_end_points = pcf_ip_end_points;
    pcf_for_pdu_session_info_local_var->is_ipv4_addr_null = is_ipv4_addr_null;
    pcf_for_pdu_session_info_local_var->ipv4_addr = ipv4_addr;
    pcf_for_pdu_session_info_local_var->ip_domain = ip_domain;
    pcf_for_pdu_session_info_local_var->ipv6_prefixes = ipv6_prefixes;
    pcf_for_pdu_session_info_local_var->mac_addrs = mac_addrs;
    pcf_for_pdu_session_info_local_var->pcf_id = pcf_id;
    pcf_for_pdu_session_info_local_var->pcf_set_id = pcf_set_id;
    pcf_for_pdu_session_info_local_var->bind_level = bind_level;

    return pcf_for_pdu_session_info_local_var;
}

void OpenAPI_pcf_for_pdu_session_info_free(OpenAPI_pcf_for_pdu_session_info_t *pcf_for_pdu_session_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_for_pdu_session_info) {
        return;
    }
    if (pcf_for_pdu_session_info->dnn) {
        ogs_free(pcf_for_pdu_session_info->dnn);
        pcf_for_pdu_session_info->dnn = NULL;
    }
    if (pcf_for_pdu_session_info->snssai) {
        OpenAPI_snssai_free(pcf_for_pdu_session_info->snssai);
        pcf_for_pdu_session_info->snssai = NULL;
    }
    if (pcf_for_pdu_session_info->pcf_fqdn) {
        ogs_free(pcf_for_pdu_session_info->pcf_fqdn);
        pcf_for_pdu_session_info->pcf_fqdn = NULL;
    }
    if (pcf_for_pdu_session_info->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_for_pdu_session_info->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_for_pdu_session_info->pcf_ip_end_points);
        pcf_for_pdu_session_info->pcf_ip_end_points = NULL;
    }
    if (pcf_for_pdu_session_info->ipv4_addr) {
        ogs_free(pcf_for_pdu_session_info->ipv4_addr);
        pcf_for_pdu_session_info->ipv4_addr = NULL;
    }
    if (pcf_for_pdu_session_info->ip_domain) {
        ogs_free(pcf_for_pdu_session_info->ip_domain);
        pcf_for_pdu_session_info->ip_domain = NULL;
    }
    if (pcf_for_pdu_session_info->ipv6_prefixes) {
        OpenAPI_list_for_each(pcf_for_pdu_session_info->ipv6_prefixes, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_for_pdu_session_info->ipv6_prefixes);
        pcf_for_pdu_session_info->ipv6_prefixes = NULL;
    }
    if (pcf_for_pdu_session_info->mac_addrs) {
        OpenAPI_list_for_each(pcf_for_pdu_session_info->mac_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_for_pdu_session_info->mac_addrs);
        pcf_for_pdu_session_info->mac_addrs = NULL;
    }
    if (pcf_for_pdu_session_info->pcf_id) {
        ogs_free(pcf_for_pdu_session_info->pcf_id);
        pcf_for_pdu_session_info->pcf_id = NULL;
    }
    if (pcf_for_pdu_session_info->pcf_set_id) {
        ogs_free(pcf_for_pdu_session_info->pcf_set_id);
        pcf_for_pdu_session_info->pcf_set_id = NULL;
    }
    ogs_free(pcf_for_pdu_session_info);
}

cJSON *OpenAPI_pcf_for_pdu_session_info_convertToJSON(OpenAPI_pcf_for_pdu_session_info_t *pcf_for_pdu_session_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_for_pdu_session_info == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [PcfForPduSessionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pcf_for_pdu_session_info->dnn) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", pcf_for_pdu_session_info->dnn) == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!pcf_for_pdu_session_info->snssai) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(pcf_for_pdu_session_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (pcf_for_pdu_session_info->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", pcf_for_pdu_session_info->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (pcf_for_pdu_session_info->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_for_pdu_session_info->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_for_pdu_session_info->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", pcf_for_pdu_session_info->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    } else if (pcf_for_pdu_session_info->is_ipv4_addr_null) {
        if (cJSON_AddNullToObject(item, "ipv4Addr") == NULL) {
            ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    if (pcf_for_pdu_session_info->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", pcf_for_pdu_session_info->ip_domain) == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (pcf_for_pdu_session_info->ipv6_prefixes) {
    cJSON *ipv6_prefixesList = cJSON_AddArrayToObject(item, "ipv6Prefixes");
    if (ipv6_prefixesList == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [ipv6_prefixes]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_for_pdu_session_info->ipv6_prefixes, node) {
        if (cJSON_AddStringToObject(ipv6_prefixesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [ipv6_prefixes]");
            goto end;
        }
    }
    }

    if (pcf_for_pdu_session_info->mac_addrs) {
    cJSON *mac_addrsList = cJSON_AddArrayToObject(item, "macAddrs");
    if (mac_addrsList == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [mac_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_for_pdu_session_info->mac_addrs, node) {
        if (cJSON_AddStringToObject(mac_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [mac_addrs]");
            goto end;
        }
    }
    }

    if (pcf_for_pdu_session_info->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_for_pdu_session_info->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pcf_for_pdu_session_info->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", pcf_for_pdu_session_info->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (pcf_for_pdu_session_info->bind_level != OpenAPI_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "bindLevel", OpenAPI_binding_level_ToString(pcf_for_pdu_session_info->bind_level)) == NULL) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed [bind_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_for_pdu_session_info_t *OpenAPI_pcf_for_pdu_session_info_parseFromJSON(cJSON *pcf_for_pdu_session_infoJSON)
{
    OpenAPI_pcf_for_pdu_session_info_t *pcf_for_pdu_session_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ip_domain = NULL;
    cJSON *ipv6_prefixes = NULL;
    OpenAPI_list_t *ipv6_prefixesList = NULL;
    cJSON *mac_addrs = NULL;
    OpenAPI_list_t *mac_addrsList = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *bind_level = NULL;
    OpenAPI_binding_level_e bind_levelVariable = 0;
    dnn = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [dnn]");
        goto end;
    }

    snssai = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [pcf_ip_end_points]");
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

    ipv4_addr = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsNull(ipv4_addr)) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }
    }

    ip_domain = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "ipDomain");
    if (ip_domain) {
    if (!cJSON_IsString(ip_domain) && !cJSON_IsNull(ip_domain)) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    ipv6_prefixes = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "ipv6Prefixes");
    if (ipv6_prefixes) {
        cJSON *ipv6_prefixes_local = NULL;
        if (!cJSON_IsArray(ipv6_prefixes)) {
            ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [ipv6_prefixes]");
            goto end;
        }

        ipv6_prefixesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefixes_local, ipv6_prefixes) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_prefixes_local)) {
                ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [ipv6_prefixes]");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefixesList, ogs_strdup(ipv6_prefixes_local->valuestring));
        }
    }

    mac_addrs = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "macAddrs");
    if (mac_addrs) {
        cJSON *mac_addrs_local = NULL;
        if (!cJSON_IsArray(mac_addrs)) {
            ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [mac_addrs]");
            goto end;
        }

        mac_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(mac_addrs_local, mac_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(mac_addrs_local)) {
                ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [mac_addrs]");
                goto end;
            }
            OpenAPI_list_add(mac_addrsList, ogs_strdup(mac_addrs_local->valuestring));
        }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    bind_level = cJSON_GetObjectItemCaseSensitive(pcf_for_pdu_session_infoJSON, "bindLevel");
    if (bind_level) {
    if (!cJSON_IsString(bind_level)) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_parseFromJSON() failed [bind_level]");
        goto end;
    }
    bind_levelVariable = OpenAPI_binding_level_FromString(bind_level->valuestring);
    }

    pcf_for_pdu_session_info_local_var = OpenAPI_pcf_for_pdu_session_info_create (
        ogs_strdup(dnn->valuestring),
        snssai_local_nonprim,
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        ipv4_addr && cJSON_IsNull(ipv4_addr) ? true : false,
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ip_domain && !cJSON_IsNull(ip_domain) ? ogs_strdup(ip_domain->valuestring) : NULL,
        ipv6_prefixes ? ipv6_prefixesList : NULL,
        mac_addrs ? mac_addrsList : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        bind_level ? bind_levelVariable : 0
    );

    return pcf_for_pdu_session_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (pcf_ip_end_pointsList) {
        OpenAPI_list_for_each(pcf_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_ip_end_pointsList);
        pcf_ip_end_pointsList = NULL;
    }
    if (ipv6_prefixesList) {
        OpenAPI_list_for_each(ipv6_prefixesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefixesList);
        ipv6_prefixesList = NULL;
    }
    if (mac_addrsList) {
        OpenAPI_list_for_each(mac_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mac_addrsList);
        mac_addrsList = NULL;
    }
    return NULL;
}

OpenAPI_pcf_for_pdu_session_info_t *OpenAPI_pcf_for_pdu_session_info_copy(OpenAPI_pcf_for_pdu_session_info_t *dst, OpenAPI_pcf_for_pdu_session_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_for_pdu_session_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_for_pdu_session_info_convertToJSON() failed");
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

    OpenAPI_pcf_for_pdu_session_info_free(dst);
    dst = OpenAPI_pcf_for_pdu_session_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

