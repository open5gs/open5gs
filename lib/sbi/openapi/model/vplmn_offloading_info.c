
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vplmn_offloading_info.h"

OpenAPI_vplmn_offloading_info_t *OpenAPI_vplmn_offloading_info_create(
    char *offload_identifier,
    OpenAPI_plmn_id_t *vplmn_id,
    bool is_allowed_traffic,
    int allowed_traffic,
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv4_addr_masks,
    OpenAPI_list_t *ipv6_address_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    OpenAPI_list_t *fqdn_list,
    OpenAPI_list_t *fqdn_patterns
)
{
    OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info_local_var = ogs_malloc(sizeof(OpenAPI_vplmn_offloading_info_t));
    ogs_assert(vplmn_offloading_info_local_var);

    vplmn_offloading_info_local_var->offload_identifier = offload_identifier;
    vplmn_offloading_info_local_var->vplmn_id = vplmn_id;
    vplmn_offloading_info_local_var->is_allowed_traffic = is_allowed_traffic;
    vplmn_offloading_info_local_var->allowed_traffic = allowed_traffic;
    vplmn_offloading_info_local_var->ipv4_address_ranges = ipv4_address_ranges;
    vplmn_offloading_info_local_var->ipv4_addr_masks = ipv4_addr_masks;
    vplmn_offloading_info_local_var->ipv6_address_ranges = ipv6_address_ranges;
    vplmn_offloading_info_local_var->ipv6_prefix_ranges = ipv6_prefix_ranges;
    vplmn_offloading_info_local_var->fqdn_list = fqdn_list;
    vplmn_offloading_info_local_var->fqdn_patterns = fqdn_patterns;

    return vplmn_offloading_info_local_var;
}

void OpenAPI_vplmn_offloading_info_free(OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vplmn_offloading_info) {
        return;
    }
    if (vplmn_offloading_info->offload_identifier) {
        ogs_free(vplmn_offloading_info->offload_identifier);
        vplmn_offloading_info->offload_identifier = NULL;
    }
    if (vplmn_offloading_info->vplmn_id) {
        OpenAPI_plmn_id_free(vplmn_offloading_info->vplmn_id);
        vplmn_offloading_info->vplmn_id = NULL;
    }
    if (vplmn_offloading_info->ipv4_address_ranges) {
        OpenAPI_list_for_each(vplmn_offloading_info->ipv4_address_ranges, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info->ipv4_address_ranges);
        vplmn_offloading_info->ipv4_address_ranges = NULL;
    }
    if (vplmn_offloading_info->ipv4_addr_masks) {
        OpenAPI_list_for_each(vplmn_offloading_info->ipv4_addr_masks, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info->ipv4_addr_masks);
        vplmn_offloading_info->ipv4_addr_masks = NULL;
    }
    if (vplmn_offloading_info->ipv6_address_ranges) {
        OpenAPI_list_for_each(vplmn_offloading_info->ipv6_address_ranges, node) {
            OpenAPI_ipv6_address_range_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info->ipv6_address_ranges);
        vplmn_offloading_info->ipv6_address_ranges = NULL;
    }
    if (vplmn_offloading_info->ipv6_prefix_ranges) {
        OpenAPI_list_for_each(vplmn_offloading_info->ipv6_prefix_ranges, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info->ipv6_prefix_ranges);
        vplmn_offloading_info->ipv6_prefix_ranges = NULL;
    }
    if (vplmn_offloading_info->fqdn_list) {
        OpenAPI_list_for_each(vplmn_offloading_info->fqdn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info->fqdn_list);
        vplmn_offloading_info->fqdn_list = NULL;
    }
    if (vplmn_offloading_info->fqdn_patterns) {
        OpenAPI_list_for_each(vplmn_offloading_info->fqdn_patterns, node) {
            OpenAPI_fqdn_pattern_matching_rule_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info->fqdn_patterns);
        vplmn_offloading_info->fqdn_patterns = NULL;
    }
    ogs_free(vplmn_offloading_info);
}

cJSON *OpenAPI_vplmn_offloading_info_convertToJSON(OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vplmn_offloading_info == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [VplmnOffloadingInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vplmn_offloading_info->offload_identifier) {
    if (cJSON_AddStringToObject(item, "offloadIdentifier", vplmn_offloading_info->offload_identifier) == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [offload_identifier]");
        goto end;
    }
    }

    if (vplmn_offloading_info->vplmn_id) {
    cJSON *vplmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(vplmn_offloading_info->vplmn_id);
    if (vplmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [vplmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnId", vplmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [vplmn_id]");
        goto end;
    }
    }

    if (vplmn_offloading_info->is_allowed_traffic) {
    if (cJSON_AddBoolToObject(item, "allowedTraffic", vplmn_offloading_info->allowed_traffic) == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [allowed_traffic]");
        goto end;
    }
    }

    if (vplmn_offloading_info->ipv4_address_ranges) {
    cJSON *ipv4_address_rangesList = cJSON_AddArrayToObject(item, "ipv4AddressRanges");
    if (ipv4_address_rangesList == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv4_address_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(vplmn_offloading_info->ipv4_address_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv4_address_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_address_rangesList, itemLocal);
    }
    }

    if (vplmn_offloading_info->ipv4_addr_masks) {
    cJSON *ipv4_addr_masksList = cJSON_AddArrayToObject(item, "ipv4AddrMasks");
    if (ipv4_addr_masksList == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv4_addr_masks]");
        goto end;
    }
    OpenAPI_list_for_each(vplmn_offloading_info->ipv4_addr_masks, node) {
        if (cJSON_AddStringToObject(ipv4_addr_masksList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv4_addr_masks]");
            goto end;
        }
    }
    }

    if (vplmn_offloading_info->ipv6_address_ranges) {
    cJSON *ipv6_address_rangesList = cJSON_AddArrayToObject(item, "ipv6AddressRanges");
    if (ipv6_address_rangesList == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv6_address_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(vplmn_offloading_info->ipv6_address_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv6_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv6_address_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_address_rangesList, itemLocal);
    }
    }

    if (vplmn_offloading_info->ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "ipv6PrefixRanges");
    if (ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(vplmn_offloading_info->ipv6_prefix_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_prefix_rangesList, itemLocal);
    }
    }

    if (vplmn_offloading_info->fqdn_list) {
    cJSON *fqdn_listList = cJSON_AddArrayToObject(item, "fqdnList");
    if (fqdn_listList == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [fqdn_list]");
        goto end;
    }
    OpenAPI_list_for_each(vplmn_offloading_info->fqdn_list, node) {
        if (cJSON_AddStringToObject(fqdn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [fqdn_list]");
            goto end;
        }
    }
    }

    if (vplmn_offloading_info->fqdn_patterns) {
    cJSON *fqdn_patternsList = cJSON_AddArrayToObject(item, "fqdnPatterns");
    if (fqdn_patternsList == NULL) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [fqdn_patterns]");
        goto end;
    }
    OpenAPI_list_for_each(vplmn_offloading_info->fqdn_patterns, node) {
        cJSON *itemLocal = OpenAPI_fqdn_pattern_matching_rule_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed [fqdn_patterns]");
            goto end;
        }
        cJSON_AddItemToArray(fqdn_patternsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_vplmn_offloading_info_t *OpenAPI_vplmn_offloading_info_parseFromJSON(cJSON *vplmn_offloading_infoJSON)
{
    OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *offload_identifier = NULL;
    cJSON *vplmn_id = NULL;
    OpenAPI_plmn_id_t *vplmn_id_local_nonprim = NULL;
    cJSON *allowed_traffic = NULL;
    cJSON *ipv4_address_ranges = NULL;
    OpenAPI_list_t *ipv4_address_rangesList = NULL;
    cJSON *ipv4_addr_masks = NULL;
    OpenAPI_list_t *ipv4_addr_masksList = NULL;
    cJSON *ipv6_address_ranges = NULL;
    OpenAPI_list_t *ipv6_address_rangesList = NULL;
    cJSON *ipv6_prefix_ranges = NULL;
    OpenAPI_list_t *ipv6_prefix_rangesList = NULL;
    cJSON *fqdn_list = NULL;
    OpenAPI_list_t *fqdn_listList = NULL;
    cJSON *fqdn_patterns = NULL;
    OpenAPI_list_t *fqdn_patternsList = NULL;
    offload_identifier = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "offloadIdentifier");
    if (offload_identifier) {
    if (!cJSON_IsString(offload_identifier) && !cJSON_IsNull(offload_identifier)) {
        ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [offload_identifier]");
        goto end;
    }
    }

    vplmn_id = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "vplmnId");
    if (vplmn_id) {
    vplmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(vplmn_id);
    if (!vplmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [vplmn_id]");
        goto end;
    }
    }

    allowed_traffic = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "allowedTraffic");
    if (allowed_traffic) {
    if (!cJSON_IsBool(allowed_traffic)) {
        ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [allowed_traffic]");
        goto end;
    }
    }

    ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "ipv4AddressRanges");
    if (ipv4_address_ranges) {
        cJSON *ipv4_address_ranges_local = NULL;
        if (!cJSON_IsArray(ipv4_address_ranges)) {
            ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv4_address_ranges]");
            goto end;
        }

        ipv4_address_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_address_ranges_local, ipv4_address_ranges) {
            if (!cJSON_IsObject(ipv4_address_ranges_local)) {
                ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv4_address_ranges]");
                goto end;
            }
            OpenAPI_ipv4_address_range_t *ipv4_address_rangesItem = OpenAPI_ipv4_address_range_parseFromJSON(ipv4_address_ranges_local);
            if (!ipv4_address_rangesItem) {
                ogs_error("No ipv4_address_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ipv4_address_rangesList, ipv4_address_rangesItem);
        }
    }

    ipv4_addr_masks = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "ipv4AddrMasks");
    if (ipv4_addr_masks) {
        cJSON *ipv4_addr_masks_local = NULL;
        if (!cJSON_IsArray(ipv4_addr_masks)) {
            ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv4_addr_masks]");
            goto end;
        }

        ipv4_addr_masksList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addr_masks_local, ipv4_addr_masks) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_addr_masks_local)) {
                ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv4_addr_masks]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addr_masksList, ogs_strdup(ipv4_addr_masks_local->valuestring));
        }
    }

    ipv6_address_ranges = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "ipv6AddressRanges");
    if (ipv6_address_ranges) {
        cJSON *ipv6_address_ranges_local = NULL;
        if (!cJSON_IsArray(ipv6_address_ranges)) {
            ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv6_address_ranges]");
            goto end;
        }

        ipv6_address_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_address_ranges_local, ipv6_address_ranges) {
            if (!cJSON_IsObject(ipv6_address_ranges_local)) {
                ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv6_address_ranges]");
                goto end;
            }
            OpenAPI_ipv6_address_range_t *ipv6_address_rangesItem = OpenAPI_ipv6_address_range_parseFromJSON(ipv6_address_ranges_local);
            if (!ipv6_address_rangesItem) {
                ogs_error("No ipv6_address_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ipv6_address_rangesList, ipv6_address_rangesItem);
        }
    }

    ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "ipv6PrefixRanges");
    if (ipv6_prefix_ranges) {
        cJSON *ipv6_prefix_ranges_local = NULL;
        if (!cJSON_IsArray(ipv6_prefix_ranges)) {
            ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }

        ipv6_prefix_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_ranges_local, ipv6_prefix_ranges) {
            if (!cJSON_IsObject(ipv6_prefix_ranges_local)) {
                ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [ipv6_prefix_ranges]");
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

    fqdn_list = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "fqdnList");
    if (fqdn_list) {
        cJSON *fqdn_list_local = NULL;
        if (!cJSON_IsArray(fqdn_list)) {
            ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [fqdn_list]");
            goto end;
        }

        fqdn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(fqdn_list_local, fqdn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(fqdn_list_local)) {
                ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [fqdn_list]");
                goto end;
            }
            OpenAPI_list_add(fqdn_listList, ogs_strdup(fqdn_list_local->valuestring));
        }
    }

    fqdn_patterns = cJSON_GetObjectItemCaseSensitive(vplmn_offloading_infoJSON, "fqdnPatterns");
    if (fqdn_patterns) {
        cJSON *fqdn_patterns_local = NULL;
        if (!cJSON_IsArray(fqdn_patterns)) {
            ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [fqdn_patterns]");
            goto end;
        }

        fqdn_patternsList = OpenAPI_list_create();

        cJSON_ArrayForEach(fqdn_patterns_local, fqdn_patterns) {
            if (!cJSON_IsObject(fqdn_patterns_local)) {
                ogs_error("OpenAPI_vplmn_offloading_info_parseFromJSON() failed [fqdn_patterns]");
                goto end;
            }
            OpenAPI_fqdn_pattern_matching_rule_t *fqdn_patternsItem = OpenAPI_fqdn_pattern_matching_rule_parseFromJSON(fqdn_patterns_local);
            if (!fqdn_patternsItem) {
                ogs_error("No fqdn_patternsItem");
                goto end;
            }
            OpenAPI_list_add(fqdn_patternsList, fqdn_patternsItem);
        }
    }

    vplmn_offloading_info_local_var = OpenAPI_vplmn_offloading_info_create (
        offload_identifier && !cJSON_IsNull(offload_identifier) ? ogs_strdup(offload_identifier->valuestring) : NULL,
        vplmn_id ? vplmn_id_local_nonprim : NULL,
        allowed_traffic ? true : false,
        allowed_traffic ? allowed_traffic->valueint : 0,
        ipv4_address_ranges ? ipv4_address_rangesList : NULL,
        ipv4_addr_masks ? ipv4_addr_masksList : NULL,
        ipv6_address_ranges ? ipv6_address_rangesList : NULL,
        ipv6_prefix_ranges ? ipv6_prefix_rangesList : NULL,
        fqdn_list ? fqdn_listList : NULL,
        fqdn_patterns ? fqdn_patternsList : NULL
    );

    return vplmn_offloading_info_local_var;
end:
    if (vplmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(vplmn_id_local_nonprim);
        vplmn_id_local_nonprim = NULL;
    }
    if (ipv4_address_rangesList) {
        OpenAPI_list_for_each(ipv4_address_rangesList, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(ipv4_address_rangesList);
        ipv4_address_rangesList = NULL;
    }
    if (ipv4_addr_masksList) {
        OpenAPI_list_for_each(ipv4_addr_masksList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_addr_masksList);
        ipv4_addr_masksList = NULL;
    }
    if (ipv6_address_rangesList) {
        OpenAPI_list_for_each(ipv6_address_rangesList, node) {
            OpenAPI_ipv6_address_range_free(node->data);
        }
        OpenAPI_list_free(ipv6_address_rangesList);
        ipv6_address_rangesList = NULL;
    }
    if (ipv6_prefix_rangesList) {
        OpenAPI_list_for_each(ipv6_prefix_rangesList, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefix_rangesList);
        ipv6_prefix_rangesList = NULL;
    }
    if (fqdn_listList) {
        OpenAPI_list_for_each(fqdn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(fqdn_listList);
        fqdn_listList = NULL;
    }
    if (fqdn_patternsList) {
        OpenAPI_list_for_each(fqdn_patternsList, node) {
            OpenAPI_fqdn_pattern_matching_rule_free(node->data);
        }
        OpenAPI_list_free(fqdn_patternsList);
        fqdn_patternsList = NULL;
    }
    return NULL;
}

OpenAPI_vplmn_offloading_info_t *OpenAPI_vplmn_offloading_info_copy(OpenAPI_vplmn_offloading_info_t *dst, OpenAPI_vplmn_offloading_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vplmn_offloading_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vplmn_offloading_info_convertToJSON() failed");
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

    OpenAPI_vplmn_offloading_info_free(dst);
    dst = OpenAPI_vplmn_offloading_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

