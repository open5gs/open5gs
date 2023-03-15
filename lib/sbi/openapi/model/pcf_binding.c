
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_binding.h"

OpenAPI_pcf_binding_t *OpenAPI_pcf_binding_create(
    char *supi,
    char *gpsi,
    char *ipv4_addr,
    char *ipv6_prefix,
    OpenAPI_list_t *add_ipv6_prefixes,
    char *ip_domain,
    char *mac_addr48,
    OpenAPI_list_t *add_mac_addrs,
    char *dnn,
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_diam_host,
    char *pcf_diam_realm,
    char *pcf_sm_fqdn,
    OpenAPI_list_t *pcf_sm_ip_end_points,
    OpenAPI_snssai_t *snssai,
    char *supp_feat,
    char *pcf_id,
    char *pcf_set_id,
    char *recovery_time,
    OpenAPI_parameter_combination_t *para_com,
    OpenAPI_binding_level_e bind_level,
    OpenAPI_list_t *ipv4_frame_route_list,
    OpenAPI_list_t *ipv6_frame_route_list
)
{
    OpenAPI_pcf_binding_t *pcf_binding_local_var = ogs_malloc(sizeof(OpenAPI_pcf_binding_t));
    ogs_assert(pcf_binding_local_var);

    pcf_binding_local_var->supi = supi;
    pcf_binding_local_var->gpsi = gpsi;
    pcf_binding_local_var->ipv4_addr = ipv4_addr;
    pcf_binding_local_var->ipv6_prefix = ipv6_prefix;
    pcf_binding_local_var->add_ipv6_prefixes = add_ipv6_prefixes;
    pcf_binding_local_var->ip_domain = ip_domain;
    pcf_binding_local_var->mac_addr48 = mac_addr48;
    pcf_binding_local_var->add_mac_addrs = add_mac_addrs;
    pcf_binding_local_var->dnn = dnn;
    pcf_binding_local_var->pcf_fqdn = pcf_fqdn;
    pcf_binding_local_var->pcf_ip_end_points = pcf_ip_end_points;
    pcf_binding_local_var->pcf_diam_host = pcf_diam_host;
    pcf_binding_local_var->pcf_diam_realm = pcf_diam_realm;
    pcf_binding_local_var->pcf_sm_fqdn = pcf_sm_fqdn;
    pcf_binding_local_var->pcf_sm_ip_end_points = pcf_sm_ip_end_points;
    pcf_binding_local_var->snssai = snssai;
    pcf_binding_local_var->supp_feat = supp_feat;
    pcf_binding_local_var->pcf_id = pcf_id;
    pcf_binding_local_var->pcf_set_id = pcf_set_id;
    pcf_binding_local_var->recovery_time = recovery_time;
    pcf_binding_local_var->para_com = para_com;
    pcf_binding_local_var->bind_level = bind_level;
    pcf_binding_local_var->ipv4_frame_route_list = ipv4_frame_route_list;
    pcf_binding_local_var->ipv6_frame_route_list = ipv6_frame_route_list;

    return pcf_binding_local_var;
}

void OpenAPI_pcf_binding_free(OpenAPI_pcf_binding_t *pcf_binding)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_binding) {
        return;
    }
    if (pcf_binding->supi) {
        ogs_free(pcf_binding->supi);
        pcf_binding->supi = NULL;
    }
    if (pcf_binding->gpsi) {
        ogs_free(pcf_binding->gpsi);
        pcf_binding->gpsi = NULL;
    }
    if (pcf_binding->ipv4_addr) {
        ogs_free(pcf_binding->ipv4_addr);
        pcf_binding->ipv4_addr = NULL;
    }
    if (pcf_binding->ipv6_prefix) {
        ogs_free(pcf_binding->ipv6_prefix);
        pcf_binding->ipv6_prefix = NULL;
    }
    if (pcf_binding->add_ipv6_prefixes) {
        OpenAPI_list_for_each(pcf_binding->add_ipv6_prefixes, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_binding->add_ipv6_prefixes);
        pcf_binding->add_ipv6_prefixes = NULL;
    }
    if (pcf_binding->ip_domain) {
        ogs_free(pcf_binding->ip_domain);
        pcf_binding->ip_domain = NULL;
    }
    if (pcf_binding->mac_addr48) {
        ogs_free(pcf_binding->mac_addr48);
        pcf_binding->mac_addr48 = NULL;
    }
    if (pcf_binding->add_mac_addrs) {
        OpenAPI_list_for_each(pcf_binding->add_mac_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_binding->add_mac_addrs);
        pcf_binding->add_mac_addrs = NULL;
    }
    if (pcf_binding->dnn) {
        ogs_free(pcf_binding->dnn);
        pcf_binding->dnn = NULL;
    }
    if (pcf_binding->pcf_fqdn) {
        ogs_free(pcf_binding->pcf_fqdn);
        pcf_binding->pcf_fqdn = NULL;
    }
    if (pcf_binding->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_binding->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_binding->pcf_ip_end_points);
        pcf_binding->pcf_ip_end_points = NULL;
    }
    if (pcf_binding->pcf_diam_host) {
        ogs_free(pcf_binding->pcf_diam_host);
        pcf_binding->pcf_diam_host = NULL;
    }
    if (pcf_binding->pcf_diam_realm) {
        ogs_free(pcf_binding->pcf_diam_realm);
        pcf_binding->pcf_diam_realm = NULL;
    }
    if (pcf_binding->pcf_sm_fqdn) {
        ogs_free(pcf_binding->pcf_sm_fqdn);
        pcf_binding->pcf_sm_fqdn = NULL;
    }
    if (pcf_binding->pcf_sm_ip_end_points) {
        OpenAPI_list_for_each(pcf_binding->pcf_sm_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_binding->pcf_sm_ip_end_points);
        pcf_binding->pcf_sm_ip_end_points = NULL;
    }
    if (pcf_binding->snssai) {
        OpenAPI_snssai_free(pcf_binding->snssai);
        pcf_binding->snssai = NULL;
    }
    if (pcf_binding->supp_feat) {
        ogs_free(pcf_binding->supp_feat);
        pcf_binding->supp_feat = NULL;
    }
    if (pcf_binding->pcf_id) {
        ogs_free(pcf_binding->pcf_id);
        pcf_binding->pcf_id = NULL;
    }
    if (pcf_binding->pcf_set_id) {
        ogs_free(pcf_binding->pcf_set_id);
        pcf_binding->pcf_set_id = NULL;
    }
    if (pcf_binding->recovery_time) {
        ogs_free(pcf_binding->recovery_time);
        pcf_binding->recovery_time = NULL;
    }
    if (pcf_binding->para_com) {
        OpenAPI_parameter_combination_free(pcf_binding->para_com);
        pcf_binding->para_com = NULL;
    }
    if (pcf_binding->ipv4_frame_route_list) {
        OpenAPI_list_for_each(pcf_binding->ipv4_frame_route_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_binding->ipv4_frame_route_list);
        pcf_binding->ipv4_frame_route_list = NULL;
    }
    if (pcf_binding->ipv6_frame_route_list) {
        OpenAPI_list_for_each(pcf_binding->ipv6_frame_route_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_binding->ipv6_frame_route_list);
        pcf_binding->ipv6_frame_route_list = NULL;
    }
    ogs_free(pcf_binding);
}

cJSON *OpenAPI_pcf_binding_convertToJSON(OpenAPI_pcf_binding_t *pcf_binding)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_binding == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [PcfBinding]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcf_binding->supi) {
    if (cJSON_AddStringToObject(item, "supi", pcf_binding->supi) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (pcf_binding->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", pcf_binding->gpsi) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (pcf_binding->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", pcf_binding->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (pcf_binding->ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6Prefix", pcf_binding->ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

    if (pcf_binding->add_ipv6_prefixes) {
    cJSON *add_ipv6_prefixesList = cJSON_AddArrayToObject(item, "addIpv6Prefixes");
    if (add_ipv6_prefixesList == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [add_ipv6_prefixes]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding->add_ipv6_prefixes, node) {
        if (cJSON_AddStringToObject(add_ipv6_prefixesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [add_ipv6_prefixes]");
            goto end;
        }
    }
    }

    if (pcf_binding->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", pcf_binding->ip_domain) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (pcf_binding->mac_addr48) {
    if (cJSON_AddStringToObject(item, "macAddr48", pcf_binding->mac_addr48) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [mac_addr48]");
        goto end;
    }
    }

    if (pcf_binding->add_mac_addrs) {
    cJSON *add_mac_addrsList = cJSON_AddArrayToObject(item, "addMacAddrs");
    if (add_mac_addrsList == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [add_mac_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding->add_mac_addrs, node) {
        if (cJSON_AddStringToObject(add_mac_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [add_mac_addrs]");
            goto end;
        }
    }
    }

    if (!pcf_binding->dnn) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", pcf_binding->dnn) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [dnn]");
        goto end;
    }

    if (pcf_binding->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", pcf_binding->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (pcf_binding->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_binding->pcf_diam_host) {
    if (cJSON_AddStringToObject(item, "pcfDiamHost", pcf_binding->pcf_diam_host) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_diam_host]");
        goto end;
    }
    }

    if (pcf_binding->pcf_diam_realm) {
    if (cJSON_AddStringToObject(item, "pcfDiamRealm", pcf_binding->pcf_diam_realm) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_diam_realm]");
        goto end;
    }
    }

    if (pcf_binding->pcf_sm_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfSmFqdn", pcf_binding->pcf_sm_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_sm_fqdn]");
        goto end;
    }
    }

    if (pcf_binding->pcf_sm_ip_end_points) {
    cJSON *pcf_sm_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfSmIpEndPoints");
    if (pcf_sm_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_sm_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding->pcf_sm_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_sm_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_sm_ip_end_pointsList, itemLocal);
    }
    }

    if (!pcf_binding->snssai) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(pcf_binding->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [snssai]");
        goto end;
    }

    if (pcf_binding->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", pcf_binding->supp_feat) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (pcf_binding->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_binding->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pcf_binding->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", pcf_binding->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (pcf_binding->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", pcf_binding->recovery_time) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

    if (pcf_binding->para_com) {
    cJSON *para_com_local_JSON = OpenAPI_parameter_combination_convertToJSON(pcf_binding->para_com);
    if (para_com_local_JSON == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [para_com]");
        goto end;
    }
    cJSON_AddItemToObject(item, "paraCom", para_com_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [para_com]");
        goto end;
    }
    }

    if (pcf_binding->bind_level != OpenAPI_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "bindLevel", OpenAPI_binding_level_ToString(pcf_binding->bind_level)) == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [bind_level]");
        goto end;
    }
    }

    if (pcf_binding->ipv4_frame_route_list) {
    cJSON *ipv4_frame_route_listList = cJSON_AddArrayToObject(item, "ipv4FrameRouteList");
    if (ipv4_frame_route_listList == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [ipv4_frame_route_list]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding->ipv4_frame_route_list, node) {
        if (cJSON_AddStringToObject(ipv4_frame_route_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [ipv4_frame_route_list]");
            goto end;
        }
    }
    }

    if (pcf_binding->ipv6_frame_route_list) {
    cJSON *ipv6_frame_route_listList = cJSON_AddArrayToObject(item, "ipv6FrameRouteList");
    if (ipv6_frame_route_listList == NULL) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [ipv6_frame_route_list]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_binding->ipv6_frame_route_list, node) {
        if (cJSON_AddStringToObject(ipv6_frame_route_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_binding_convertToJSON() failed [ipv6_frame_route_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pcf_binding_t *OpenAPI_pcf_binding_parseFromJSON(cJSON *pcf_bindingJSON)
{
    OpenAPI_pcf_binding_t *pcf_binding_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_prefix = NULL;
    cJSON *add_ipv6_prefixes = NULL;
    OpenAPI_list_t *add_ipv6_prefixesList = NULL;
    cJSON *ip_domain = NULL;
    cJSON *mac_addr48 = NULL;
    cJSON *add_mac_addrs = NULL;
    OpenAPI_list_t *add_mac_addrsList = NULL;
    cJSON *dnn = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    cJSON *pcf_diam_host = NULL;
    cJSON *pcf_diam_realm = NULL;
    cJSON *pcf_sm_fqdn = NULL;
    cJSON *pcf_sm_ip_end_points = NULL;
    OpenAPI_list_t *pcf_sm_ip_end_pointsList = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *supp_feat = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *recovery_time = NULL;
    cJSON *para_com = NULL;
    OpenAPI_parameter_combination_t *para_com_local_nonprim = NULL;
    cJSON *bind_level = NULL;
    OpenAPI_binding_level_e bind_levelVariable = 0;
    cJSON *ipv4_frame_route_list = NULL;
    OpenAPI_list_t *ipv4_frame_route_listList = NULL;
    cJSON *ipv6_frame_route_list = NULL;
    OpenAPI_list_t *ipv6_frame_route_listList = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    ipv4_addr = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_prefix = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "ipv6Prefix");
    if (ipv6_prefix) {
    if (!cJSON_IsString(ipv6_prefix) && !cJSON_IsNull(ipv6_prefix)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

    add_ipv6_prefixes = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "addIpv6Prefixes");
    if (add_ipv6_prefixes) {
        cJSON *add_ipv6_prefixes_local = NULL;
        if (!cJSON_IsArray(add_ipv6_prefixes)) {
            ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [add_ipv6_prefixes]");
            goto end;
        }

        add_ipv6_prefixesList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_ipv6_prefixes_local, add_ipv6_prefixes) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(add_ipv6_prefixes_local)) {
                ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [add_ipv6_prefixes]");
                goto end;
            }
            OpenAPI_list_add(add_ipv6_prefixesList, ogs_strdup(add_ipv6_prefixes_local->valuestring));
        }
    }

    ip_domain = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "ipDomain");
    if (ip_domain) {
    if (!cJSON_IsString(ip_domain) && !cJSON_IsNull(ip_domain)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    mac_addr48 = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "macAddr48");
    if (mac_addr48) {
    if (!cJSON_IsString(mac_addr48) && !cJSON_IsNull(mac_addr48)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [mac_addr48]");
        goto end;
    }
    }

    add_mac_addrs = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "addMacAddrs");
    if (add_mac_addrs) {
        cJSON *add_mac_addrs_local = NULL;
        if (!cJSON_IsArray(add_mac_addrs)) {
            ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [add_mac_addrs]");
            goto end;
        }

        add_mac_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_mac_addrs_local, add_mac_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(add_mac_addrs_local)) {
                ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [add_mac_addrs]");
                goto end;
            }
            OpenAPI_list_add(add_mac_addrsList, ogs_strdup(add_mac_addrs_local->valuestring));
        }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [dnn]");
        goto end;
    }

    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_ip_end_points]");
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

    pcf_diam_host = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfDiamHost");
    if (pcf_diam_host) {
    if (!cJSON_IsString(pcf_diam_host) && !cJSON_IsNull(pcf_diam_host)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_diam_host]");
        goto end;
    }
    }

    pcf_diam_realm = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfDiamRealm");
    if (pcf_diam_realm) {
    if (!cJSON_IsString(pcf_diam_realm) && !cJSON_IsNull(pcf_diam_realm)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_diam_realm]");
        goto end;
    }
    }

    pcf_sm_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfSmFqdn");
    if (pcf_sm_fqdn) {
    if (!cJSON_IsString(pcf_sm_fqdn) && !cJSON_IsNull(pcf_sm_fqdn)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_sm_fqdn]");
        goto end;
    }
    }

    pcf_sm_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfSmIpEndPoints");
    if (pcf_sm_ip_end_points) {
        cJSON *pcf_sm_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_sm_ip_end_points)) {
            ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_sm_ip_end_points]");
            goto end;
        }

        pcf_sm_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_sm_ip_end_points_local, pcf_sm_ip_end_points) {
            if (!cJSON_IsObject(pcf_sm_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_sm_ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *pcf_sm_ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(pcf_sm_ip_end_points_local);
            if (!pcf_sm_ip_end_pointsItem) {
                ogs_error("No pcf_sm_ip_end_pointsItem");
                goto end;
            }
            OpenAPI_list_add(pcf_sm_ip_end_pointsList, pcf_sm_ip_end_pointsItem);
        }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    para_com = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "paraCom");
    if (para_com) {
    para_com_local_nonprim = OpenAPI_parameter_combination_parseFromJSON(para_com);
    if (!para_com_local_nonprim) {
        ogs_error("OpenAPI_parameter_combination_parseFromJSON failed [para_com]");
        goto end;
    }
    }

    bind_level = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "bindLevel");
    if (bind_level) {
    if (!cJSON_IsString(bind_level)) {
        ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [bind_level]");
        goto end;
    }
    bind_levelVariable = OpenAPI_binding_level_FromString(bind_level->valuestring);
    }

    ipv4_frame_route_list = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "ipv4FrameRouteList");
    if (ipv4_frame_route_list) {
        cJSON *ipv4_frame_route_list_local = NULL;
        if (!cJSON_IsArray(ipv4_frame_route_list)) {
            ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [ipv4_frame_route_list]");
            goto end;
        }

        ipv4_frame_route_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_frame_route_list_local, ipv4_frame_route_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_frame_route_list_local)) {
                ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [ipv4_frame_route_list]");
                goto end;
            }
            OpenAPI_list_add(ipv4_frame_route_listList, ogs_strdup(ipv4_frame_route_list_local->valuestring));
        }
    }

    ipv6_frame_route_list = cJSON_GetObjectItemCaseSensitive(pcf_bindingJSON, "ipv6FrameRouteList");
    if (ipv6_frame_route_list) {
        cJSON *ipv6_frame_route_list_local = NULL;
        if (!cJSON_IsArray(ipv6_frame_route_list)) {
            ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [ipv6_frame_route_list]");
            goto end;
        }

        ipv6_frame_route_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_frame_route_list_local, ipv6_frame_route_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_frame_route_list_local)) {
                ogs_error("OpenAPI_pcf_binding_parseFromJSON() failed [ipv6_frame_route_list]");
                goto end;
            }
            OpenAPI_list_add(ipv6_frame_route_listList, ogs_strdup(ipv6_frame_route_list_local->valuestring));
        }
    }

    pcf_binding_local_var = OpenAPI_pcf_binding_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_prefix && !cJSON_IsNull(ipv6_prefix) ? ogs_strdup(ipv6_prefix->valuestring) : NULL,
        add_ipv6_prefixes ? add_ipv6_prefixesList : NULL,
        ip_domain && !cJSON_IsNull(ip_domain) ? ogs_strdup(ip_domain->valuestring) : NULL,
        mac_addr48 && !cJSON_IsNull(mac_addr48) ? ogs_strdup(mac_addr48->valuestring) : NULL,
        add_mac_addrs ? add_mac_addrsList : NULL,
        ogs_strdup(dnn->valuestring),
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        pcf_diam_host && !cJSON_IsNull(pcf_diam_host) ? ogs_strdup(pcf_diam_host->valuestring) : NULL,
        pcf_diam_realm && !cJSON_IsNull(pcf_diam_realm) ? ogs_strdup(pcf_diam_realm->valuestring) : NULL,
        pcf_sm_fqdn && !cJSON_IsNull(pcf_sm_fqdn) ? ogs_strdup(pcf_sm_fqdn->valuestring) : NULL,
        pcf_sm_ip_end_points ? pcf_sm_ip_end_pointsList : NULL,
        snssai_local_nonprim,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL,
        para_com ? para_com_local_nonprim : NULL,
        bind_level ? bind_levelVariable : 0,
        ipv4_frame_route_list ? ipv4_frame_route_listList : NULL,
        ipv6_frame_route_list ? ipv6_frame_route_listList : NULL
    );

    return pcf_binding_local_var;
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
    if (pcf_sm_ip_end_pointsList) {
        OpenAPI_list_for_each(pcf_sm_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_sm_ip_end_pointsList);
        pcf_sm_ip_end_pointsList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (para_com_local_nonprim) {
        OpenAPI_parameter_combination_free(para_com_local_nonprim);
        para_com_local_nonprim = NULL;
    }
    if (ipv4_frame_route_listList) {
        OpenAPI_list_for_each(ipv4_frame_route_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_frame_route_listList);
        ipv4_frame_route_listList = NULL;
    }
    if (ipv6_frame_route_listList) {
        OpenAPI_list_for_each(ipv6_frame_route_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_frame_route_listList);
        ipv6_frame_route_listList = NULL;
    }
    return NULL;
}

OpenAPI_pcf_binding_t *OpenAPI_pcf_binding_copy(OpenAPI_pcf_binding_t *dst, OpenAPI_pcf_binding_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_binding_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_binding_convertToJSON() failed");
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

    OpenAPI_pcf_binding_free(dst);
    dst = OpenAPI_pcf_binding_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

