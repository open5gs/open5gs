
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsf_info.h"

OpenAPI_bsf_info_t *OpenAPI_bsf_info_create(
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *ip_domain_list,
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges
)
{
    OpenAPI_bsf_info_t *bsf_info_local_var = ogs_malloc(sizeof(OpenAPI_bsf_info_t));
    ogs_assert(bsf_info_local_var);

    bsf_info_local_var->dnn_list = dnn_list;
    bsf_info_local_var->ip_domain_list = ip_domain_list;
    bsf_info_local_var->ipv4_address_ranges = ipv4_address_ranges;
    bsf_info_local_var->ipv6_prefix_ranges = ipv6_prefix_ranges;

    return bsf_info_local_var;
}

void OpenAPI_bsf_info_free(OpenAPI_bsf_info_t *bsf_info)
{
    if (NULL == bsf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(bsf_info->dnn_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(bsf_info->dnn_list);
    OpenAPI_list_for_each(bsf_info->ip_domain_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(bsf_info->ip_domain_list);
    OpenAPI_list_for_each(bsf_info->ipv4_address_ranges, node) {
        OpenAPI_ipv4_address_range_free(node->data);
    }
    OpenAPI_list_free(bsf_info->ipv4_address_ranges);
    OpenAPI_list_for_each(bsf_info->ipv6_prefix_ranges, node) {
        OpenAPI_ipv6_prefix_range_free(node->data);
    }
    OpenAPI_list_free(bsf_info->ipv6_prefix_ranges);
    ogs_free(bsf_info);
}

cJSON *OpenAPI_bsf_info_convertToJSON(OpenAPI_bsf_info_t *bsf_info)
{
    cJSON *item = NULL;

    if (bsf_info == NULL) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed [BsfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bsf_info->dnn_list) {
    cJSON *dnn_list = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_list == NULL) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed [dnn_list]");
        goto end;
    }

    OpenAPI_lnode_t *dnn_list_node;
    OpenAPI_list_for_each(bsf_info->dnn_list, dnn_list_node)  {
    if (cJSON_AddStringToObject(dnn_list, "", (char*)dnn_list_node->data) == NULL) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed [dnn_list]");
        goto end;
    }
                    }
    }

    if (bsf_info->ip_domain_list) {
    cJSON *ip_domain_list = cJSON_AddArrayToObject(item, "ipDomainList");
    if (ip_domain_list == NULL) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed [ip_domain_list]");
        goto end;
    }

    OpenAPI_lnode_t *ip_domain_list_node;
    OpenAPI_list_for_each(bsf_info->ip_domain_list, ip_domain_list_node)  {
    if (cJSON_AddStringToObject(ip_domain_list, "", (char*)ip_domain_list_node->data) == NULL) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed [ip_domain_list]");
        goto end;
    }
                    }
    }

    if (bsf_info->ipv4_address_ranges) {
    cJSON *ipv4_address_rangesList = cJSON_AddArrayToObject(item, "ipv4AddressRanges");
    if (ipv4_address_rangesList == NULL) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed [ipv4_address_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *ipv4_address_ranges_node;
    if (bsf_info->ipv4_address_ranges) {
        OpenAPI_list_for_each(bsf_info->ipv4_address_ranges, ipv4_address_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(ipv4_address_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_bsf_info_convertToJSON() failed [ipv4_address_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ipv4_address_rangesList, itemLocal);
        }
    }
    }

    if (bsf_info->ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "ipv6PrefixRanges");
    if (ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *ipv6_prefix_ranges_node;
    if (bsf_info->ipv6_prefix_ranges) {
        OpenAPI_list_for_each(bsf_info->ipv6_prefix_ranges, ipv6_prefix_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(ipv6_prefix_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_bsf_info_convertToJSON() failed [ipv6_prefix_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ipv6_prefix_rangesList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_bsf_info_t *OpenAPI_bsf_info_parseFromJSON(cJSON *bsf_infoJSON)
{
    OpenAPI_bsf_info_t *bsf_info_local_var = NULL;
    cJSON *dnn_list = cJSON_GetObjectItemCaseSensitive(bsf_infoJSON, "dnnList");

    OpenAPI_list_t *dnn_listList;
    if (dnn_list) {
    cJSON *dnn_list_local;
    if (!cJSON_IsArray(dnn_list)) {
        ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [dnn_list]");
        goto end;
    }
    dnn_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnn_list_local, dnn_list) {
    if (!cJSON_IsString(dnn_list_local)) {
        ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_add(dnn_listList , ogs_strdup(dnn_list_local->valuestring));
    }
    }

    cJSON *ip_domain_list = cJSON_GetObjectItemCaseSensitive(bsf_infoJSON, "ipDomainList");

    OpenAPI_list_t *ip_domain_listList;
    if (ip_domain_list) {
    cJSON *ip_domain_list_local;
    if (!cJSON_IsArray(ip_domain_list)) {
        ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [ip_domain_list]");
        goto end;
    }
    ip_domain_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(ip_domain_list_local, ip_domain_list) {
    if (!cJSON_IsString(ip_domain_list_local)) {
        ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [ip_domain_list]");
        goto end;
    }
    OpenAPI_list_add(ip_domain_listList , ogs_strdup(ip_domain_list_local->valuestring));
    }
    }

    cJSON *ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(bsf_infoJSON, "ipv4AddressRanges");

    OpenAPI_list_t *ipv4_address_rangesList;
    if (ipv4_address_ranges) {
    cJSON *ipv4_address_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ipv4_address_ranges)){
        ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [ipv4_address_ranges]");
        goto end;
    }

    ipv4_address_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv4_address_ranges_local_nonprimitive, ipv4_address_ranges ) {
        if (!cJSON_IsObject(ipv4_address_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [ipv4_address_ranges]");
            goto end;
        }
        OpenAPI_ipv4_address_range_t *ipv4_address_rangesItem = OpenAPI_ipv4_address_range_parseFromJSON(ipv4_address_ranges_local_nonprimitive);

        if (!ipv4_address_rangesItem) {
            ogs_error("No ipv4_address_rangesItem");
            OpenAPI_list_free(ipv4_address_rangesList);
            goto end;
        }

        OpenAPI_list_add(ipv4_address_rangesList, ipv4_address_rangesItem);
    }
    }

    cJSON *ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(bsf_infoJSON, "ipv6PrefixRanges");

    OpenAPI_list_t *ipv6_prefix_rangesList;
    if (ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ipv6_prefix_ranges)){
        ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }

    ipv6_prefix_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv6_prefix_ranges_local_nonprimitive, ipv6_prefix_ranges ) {
        if (!cJSON_IsObject(ipv6_prefix_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_bsf_info_parseFromJSON() failed [ipv6_prefix_ranges]");
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

    bsf_info_local_var = OpenAPI_bsf_info_create (
        dnn_list ? dnn_listList : NULL,
        ip_domain_list ? ip_domain_listList : NULL,
        ipv4_address_ranges ? ipv4_address_rangesList : NULL,
        ipv6_prefix_ranges ? ipv6_prefix_rangesList : NULL
    );

    return bsf_info_local_var;
end:
    return NULL;
}

OpenAPI_bsf_info_t *OpenAPI_bsf_info_copy(OpenAPI_bsf_info_t *dst, OpenAPI_bsf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bsf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bsf_info_convertToJSON() failed");
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

    OpenAPI_bsf_info_free(dst);
    dst = OpenAPI_bsf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

