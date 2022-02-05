
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcscf_info.h"

OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_create(
    OpenAPI_list_t *access_type,
    OpenAPI_list_t *dnn_list,
    char *gm_fqdn,
    OpenAPI_list_t *gm_ipv4_addresses,
    OpenAPI_list_t *gm_ipv6_addresses,
    OpenAPI_list_t *served_ipv4_address_ranges,
    OpenAPI_list_t *served_ipv6_prefix_ranges
)
{
    OpenAPI_pcscf_info_t *pcscf_info_local_var = ogs_malloc(sizeof(OpenAPI_pcscf_info_t));
    ogs_assert(pcscf_info_local_var);

    pcscf_info_local_var->access_type = access_type;
    pcscf_info_local_var->dnn_list = dnn_list;
    pcscf_info_local_var->gm_fqdn = gm_fqdn;
    pcscf_info_local_var->gm_ipv4_addresses = gm_ipv4_addresses;
    pcscf_info_local_var->gm_ipv6_addresses = gm_ipv6_addresses;
    pcscf_info_local_var->served_ipv4_address_ranges = served_ipv4_address_ranges;
    pcscf_info_local_var->served_ipv6_prefix_ranges = served_ipv6_prefix_ranges;

    return pcscf_info_local_var;
}

void OpenAPI_pcscf_info_free(OpenAPI_pcscf_info_t *pcscf_info)
{
    if (NULL == pcscf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(pcscf_info->access_type);
    OpenAPI_list_for_each(pcscf_info->dnn_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcscf_info->dnn_list);
    ogs_free(pcscf_info->gm_fqdn);
    OpenAPI_list_for_each(pcscf_info->gm_ipv4_addresses, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcscf_info->gm_ipv4_addresses);
    OpenAPI_list_for_each(pcscf_info->gm_ipv6_addresses, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcscf_info->gm_ipv6_addresses);
    OpenAPI_list_for_each(pcscf_info->served_ipv4_address_ranges, node) {
        OpenAPI_ipv4_address_range_free(node->data);
    }
    OpenAPI_list_free(pcscf_info->served_ipv4_address_ranges);
    OpenAPI_list_for_each(pcscf_info->served_ipv6_prefix_ranges, node) {
        OpenAPI_ipv6_prefix_range_free(node->data);
    }
    OpenAPI_list_free(pcscf_info->served_ipv6_prefix_ranges);
    ogs_free(pcscf_info);
}

cJSON *OpenAPI_pcscf_info_convertToJSON(OpenAPI_pcscf_info_t *pcscf_info)
{
    cJSON *item = NULL;

    if (pcscf_info == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [PcscfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcscf_info->access_type) {
    cJSON *access_type = cJSON_AddArrayToObject(item, "accessType");
    if (access_type == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [access_type]");
        goto end;
    }
    OpenAPI_lnode_t *access_type_node;
    OpenAPI_list_for_each(pcscf_info->access_type, access_type_node) {
        if (cJSON_AddStringToObject(access_type, "", OpenAPI_access_type_ToString((intptr_t)access_type_node->data)) == NULL) {
            ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [access_type]");
            goto end;
        }
    }
    }

    if (pcscf_info->dnn_list) {
    cJSON *dnn_list = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_list == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [dnn_list]");
        goto end;
    }

    OpenAPI_lnode_t *dnn_list_node;
    OpenAPI_list_for_each(pcscf_info->dnn_list, dnn_list_node)  {
    if (cJSON_AddStringToObject(dnn_list, "", (char*)dnn_list_node->data) == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [dnn_list]");
        goto end;
    }
                    }
    }

    if (pcscf_info->gm_fqdn) {
    if (cJSON_AddStringToObject(item, "gmFqdn", pcscf_info->gm_fqdn) == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [gm_fqdn]");
        goto end;
    }
    }

    if (pcscf_info->gm_ipv4_addresses) {
    cJSON *gm_ipv4_addresses = cJSON_AddArrayToObject(item, "gmIpv4Addresses");
    if (gm_ipv4_addresses == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [gm_ipv4_addresses]");
        goto end;
    }

    OpenAPI_lnode_t *gm_ipv4_addresses_node;
    OpenAPI_list_for_each(pcscf_info->gm_ipv4_addresses, gm_ipv4_addresses_node)  {
    if (cJSON_AddStringToObject(gm_ipv4_addresses, "", (char*)gm_ipv4_addresses_node->data) == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [gm_ipv4_addresses]");
        goto end;
    }
                    }
    }

    if (pcscf_info->gm_ipv6_addresses) {
    cJSON *gm_ipv6_addresses = cJSON_AddArrayToObject(item, "gmIpv6Addresses");
    if (gm_ipv6_addresses == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [gm_ipv6_addresses]");
        goto end;
    }

    OpenAPI_lnode_t *gm_ipv6_addresses_node;
    OpenAPI_list_for_each(pcscf_info->gm_ipv6_addresses, gm_ipv6_addresses_node)  {
    if (cJSON_AddStringToObject(gm_ipv6_addresses, "", (char*)gm_ipv6_addresses_node->data) == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [gm_ipv6_addresses]");
        goto end;
    }
                    }
    }

    if (pcscf_info->served_ipv4_address_ranges) {
    cJSON *served_ipv4_address_rangesList = cJSON_AddArrayToObject(item, "servedIpv4AddressRanges");
    if (served_ipv4_address_rangesList == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [served_ipv4_address_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *served_ipv4_address_ranges_node;
    if (pcscf_info->served_ipv4_address_ranges) {
        OpenAPI_list_for_each(pcscf_info->served_ipv4_address_ranges, served_ipv4_address_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(served_ipv4_address_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [served_ipv4_address_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(served_ipv4_address_rangesList, itemLocal);
        }
    }
    }

    if (pcscf_info->served_ipv6_prefix_ranges) {
    cJSON *served_ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "servedIpv6PrefixRanges");
    if (served_ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [served_ipv6_prefix_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *served_ipv6_prefix_ranges_node;
    if (pcscf_info->served_ipv6_prefix_ranges) {
        OpenAPI_list_for_each(pcscf_info->served_ipv6_prefix_ranges, served_ipv6_prefix_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(served_ipv6_prefix_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [served_ipv6_prefix_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(served_ipv6_prefix_rangesList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_parseFromJSON(cJSON *pcscf_infoJSON)
{
    OpenAPI_pcscf_info_t *pcscf_info_local_var = NULL;
    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "accessType");

    OpenAPI_list_t *access_typeList;
    if (access_type) {
    cJSON *access_type_local_nonprimitive;
    if (!cJSON_IsArray(access_type)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [access_type]");
        goto end;
    }

    access_typeList = OpenAPI_list_create();

    cJSON_ArrayForEach(access_type_local_nonprimitive, access_type ) {
        if (!cJSON_IsString(access_type_local_nonprimitive)){
            ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [access_type]");
            goto end;
        }

        OpenAPI_list_add(access_typeList, (void *)OpenAPI_access_type_FromString(access_type_local_nonprimitive->valuestring));
    }
    }

    cJSON *dnn_list = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "dnnList");

    OpenAPI_list_t *dnn_listList;
    if (dnn_list) {
    cJSON *dnn_list_local;
    if (!cJSON_IsArray(dnn_list)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [dnn_list]");
        goto end;
    }
    dnn_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnn_list_local, dnn_list) {
    if (!cJSON_IsString(dnn_list_local)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_add(dnn_listList , ogs_strdup(dnn_list_local->valuestring));
    }
    }

    cJSON *gm_fqdn = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "gmFqdn");

    if (gm_fqdn) {
    if (!cJSON_IsString(gm_fqdn)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [gm_fqdn]");
        goto end;
    }
    }

    cJSON *gm_ipv4_addresses = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "gmIpv4Addresses");

    OpenAPI_list_t *gm_ipv4_addressesList;
    if (gm_ipv4_addresses) {
    cJSON *gm_ipv4_addresses_local;
    if (!cJSON_IsArray(gm_ipv4_addresses)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [gm_ipv4_addresses]");
        goto end;
    }
    gm_ipv4_addressesList = OpenAPI_list_create();

    cJSON_ArrayForEach(gm_ipv4_addresses_local, gm_ipv4_addresses) {
    if (!cJSON_IsString(gm_ipv4_addresses_local)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [gm_ipv4_addresses]");
        goto end;
    }
    OpenAPI_list_add(gm_ipv4_addressesList , ogs_strdup(gm_ipv4_addresses_local->valuestring));
    }
    }

    cJSON *gm_ipv6_addresses = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "gmIpv6Addresses");

    OpenAPI_list_t *gm_ipv6_addressesList;
    if (gm_ipv6_addresses) {
    cJSON *gm_ipv6_addresses_local;
    if (!cJSON_IsArray(gm_ipv6_addresses)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [gm_ipv6_addresses]");
        goto end;
    }
    gm_ipv6_addressesList = OpenAPI_list_create();

    cJSON_ArrayForEach(gm_ipv6_addresses_local, gm_ipv6_addresses) {
    if (!cJSON_IsString(gm_ipv6_addresses_local)) {
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [gm_ipv6_addresses]");
        goto end;
    }
    OpenAPI_list_add(gm_ipv6_addressesList , ogs_strdup(gm_ipv6_addresses_local->valuestring));
    }
    }

    cJSON *served_ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "servedIpv4AddressRanges");

    OpenAPI_list_t *served_ipv4_address_rangesList;
    if (served_ipv4_address_ranges) {
    cJSON *served_ipv4_address_ranges_local_nonprimitive;
    if (!cJSON_IsArray(served_ipv4_address_ranges)){
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [served_ipv4_address_ranges]");
        goto end;
    }

    served_ipv4_address_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(served_ipv4_address_ranges_local_nonprimitive, served_ipv4_address_ranges ) {
        if (!cJSON_IsObject(served_ipv4_address_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [served_ipv4_address_ranges]");
            goto end;
        }
        OpenAPI_ipv4_address_range_t *served_ipv4_address_rangesItem = OpenAPI_ipv4_address_range_parseFromJSON(served_ipv4_address_ranges_local_nonprimitive);

        if (!served_ipv4_address_rangesItem) {
            ogs_error("No served_ipv4_address_rangesItem");
            OpenAPI_list_free(served_ipv4_address_rangesList);
            goto end;
        }

        OpenAPI_list_add(served_ipv4_address_rangesList, served_ipv4_address_rangesItem);
    }
    }

    cJSON *served_ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "servedIpv6PrefixRanges");

    OpenAPI_list_t *served_ipv6_prefix_rangesList;
    if (served_ipv6_prefix_ranges) {
    cJSON *served_ipv6_prefix_ranges_local_nonprimitive;
    if (!cJSON_IsArray(served_ipv6_prefix_ranges)){
        ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [served_ipv6_prefix_ranges]");
        goto end;
    }

    served_ipv6_prefix_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(served_ipv6_prefix_ranges_local_nonprimitive, served_ipv6_prefix_ranges ) {
        if (!cJSON_IsObject(served_ipv6_prefix_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [served_ipv6_prefix_ranges]");
            goto end;
        }
        OpenAPI_ipv6_prefix_range_t *served_ipv6_prefix_rangesItem = OpenAPI_ipv6_prefix_range_parseFromJSON(served_ipv6_prefix_ranges_local_nonprimitive);

        if (!served_ipv6_prefix_rangesItem) {
            ogs_error("No served_ipv6_prefix_rangesItem");
            OpenAPI_list_free(served_ipv6_prefix_rangesList);
            goto end;
        }

        OpenAPI_list_add(served_ipv6_prefix_rangesList, served_ipv6_prefix_rangesItem);
    }
    }

    pcscf_info_local_var = OpenAPI_pcscf_info_create (
        access_type ? access_typeList : NULL,
        dnn_list ? dnn_listList : NULL,
        gm_fqdn ? ogs_strdup(gm_fqdn->valuestring) : NULL,
        gm_ipv4_addresses ? gm_ipv4_addressesList : NULL,
        gm_ipv6_addresses ? gm_ipv6_addressesList : NULL,
        served_ipv4_address_ranges ? served_ipv4_address_rangesList : NULL,
        served_ipv6_prefix_ranges ? served_ipv6_prefix_rangesList : NULL
    );

    return pcscf_info_local_var;
end:
    return NULL;
}

OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_copy(OpenAPI_pcscf_info_t *dst, OpenAPI_pcscf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcscf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed");
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

    OpenAPI_pcscf_info_free(dst);
    dst = OpenAPI_pcscf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

