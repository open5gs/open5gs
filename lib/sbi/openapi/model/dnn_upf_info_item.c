
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_upf_info_item.h"

OpenAPI_dnn_upf_info_item_t *OpenAPI_dnn_upf_info_item_create(
    char *dnn,
    OpenAPI_list_t *dnai_list,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    OpenAPI_list_t* dnai_nw_instance_list
)
{
    OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item_local_var = ogs_malloc(sizeof(OpenAPI_dnn_upf_info_item_t));
    ogs_assert(dnn_upf_info_item_local_var);

    dnn_upf_info_item_local_var->dnn = dnn;
    dnn_upf_info_item_local_var->dnai_list = dnai_list;
    dnn_upf_info_item_local_var->pdu_session_types = pdu_session_types;
    dnn_upf_info_item_local_var->ipv4_address_ranges = ipv4_address_ranges;
    dnn_upf_info_item_local_var->ipv6_prefix_ranges = ipv6_prefix_ranges;
    dnn_upf_info_item_local_var->dnai_nw_instance_list = dnai_nw_instance_list;

    return dnn_upf_info_item_local_var;
}

void OpenAPI_dnn_upf_info_item_free(OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item)
{
    if (NULL == dnn_upf_info_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dnn_upf_info_item->dnn);
    OpenAPI_list_for_each(dnn_upf_info_item->dnai_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(dnn_upf_info_item->dnai_list);
    OpenAPI_list_free(dnn_upf_info_item->pdu_session_types);
    OpenAPI_list_for_each(dnn_upf_info_item->ipv4_address_ranges, node) {
        OpenAPI_ipv4_address_range_free(node->data);
    }
    OpenAPI_list_free(dnn_upf_info_item->ipv4_address_ranges);
    OpenAPI_list_for_each(dnn_upf_info_item->ipv6_prefix_ranges, node) {
        OpenAPI_ipv6_prefix_range_free(node->data);
    }
    OpenAPI_list_free(dnn_upf_info_item->ipv6_prefix_ranges);
    OpenAPI_list_for_each(dnn_upf_info_item->dnai_nw_instance_list, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(dnn_upf_info_item->dnai_nw_instance_list);
    ogs_free(dnn_upf_info_item);
}

cJSON *OpenAPI_dnn_upf_info_item_convertToJSON(OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item)
{
    cJSON *item = NULL;

    if (dnn_upf_info_item == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [DnnUpfInfoItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "dnn", dnn_upf_info_item->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_upf_info_item->dnai_list) {
    cJSON *dnai_list = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_list == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_list]");
        goto end;
    }

    OpenAPI_lnode_t *dnai_list_node;
    OpenAPI_list_for_each(dnn_upf_info_item->dnai_list, dnai_list_node)  {
    if (cJSON_AddStringToObject(dnai_list, "", (char*)dnai_list_node->data) == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_list]");
        goto end;
    }
                    }
    }

    if (dnn_upf_info_item->pdu_session_types) {
    cJSON *pdu_session_types = cJSON_AddArrayToObject(item, "pduSessionTypes");
    if (pdu_session_types == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    OpenAPI_lnode_t *pdu_session_types_node;
    OpenAPI_list_for_each(dnn_upf_info_item->pdu_session_types, pdu_session_types_node) {
        if (cJSON_AddStringToObject(pdu_session_types, "", OpenAPI_pdu_session_type_ToString((intptr_t)pdu_session_types_node->data)) == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [pdu_session_types]");
            goto end;
        }
    }
    }

    if (dnn_upf_info_item->ipv4_address_ranges) {
    cJSON *ipv4_address_rangesList = cJSON_AddArrayToObject(item, "ipv4AddressRanges");
    if (ipv4_address_rangesList == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv4_address_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *ipv4_address_ranges_node;
    if (dnn_upf_info_item->ipv4_address_ranges) {
        OpenAPI_list_for_each(dnn_upf_info_item->ipv4_address_ranges, ipv4_address_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(ipv4_address_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv4_address_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ipv4_address_rangesList, itemLocal);
        }
    }
    }

    if (dnn_upf_info_item->ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "ipv6PrefixRanges");
    if (ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *ipv6_prefix_ranges_node;
    if (dnn_upf_info_item->ipv6_prefix_ranges) {
        OpenAPI_list_for_each(dnn_upf_info_item->ipv6_prefix_ranges, ipv6_prefix_ranges_node) {
            cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(ipv6_prefix_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv6_prefix_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ipv6_prefix_rangesList, itemLocal);
        }
    }
    }

    if (dnn_upf_info_item->dnai_nw_instance_list) {
    cJSON *dnai_nw_instance_list = cJSON_AddObjectToObject(item, "dnaiNwInstanceList");
    if (dnai_nw_instance_list == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_nw_instance_list]");
        goto end;
    }
    cJSON *localMapObject = dnai_nw_instance_list;
    OpenAPI_lnode_t *dnai_nw_instance_list_node;
    if (dnn_upf_info_item->dnai_nw_instance_list) {
        OpenAPI_list_for_each(dnn_upf_info_item->dnai_nw_instance_list, dnai_nw_instance_list_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)dnai_nw_instance_list_node->data;
            }
        }
    }

end:
    return item;
}

OpenAPI_dnn_upf_info_item_t *OpenAPI_dnn_upf_info_item_parseFromJSON(cJSON *dnn_upf_info_itemJSON)
{
    OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }

    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *dnai_list = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnaiList");

    OpenAPI_list_t *dnai_listList;
    if (dnai_list) {
    cJSON *dnai_list_local;
    if (!cJSON_IsArray(dnai_list)) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_list]");
        goto end;
    }
    dnai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnai_list_local, dnai_list) {
    if (!cJSON_IsString(dnai_list_local)) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_add(dnai_listList , ogs_strdup(dnai_list_local->valuestring));
    }
    }

    cJSON *pdu_session_types = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "pduSessionTypes");

    OpenAPI_list_t *pdu_session_typesList;
    if (pdu_session_types) {
    cJSON *pdu_session_types_local_nonprimitive;
    if (!cJSON_IsArray(pdu_session_types)) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [pdu_session_types]");
        goto end;
    }

    pdu_session_typesList = OpenAPI_list_create();

    cJSON_ArrayForEach(pdu_session_types_local_nonprimitive, pdu_session_types ) {
        if (!cJSON_IsString(pdu_session_types_local_nonprimitive)){
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [pdu_session_types]");
            goto end;
        }

        OpenAPI_list_add(pdu_session_typesList, (void *)OpenAPI_pdu_session_type_FromString(pdu_session_types_local_nonprimitive->valuestring));
    }
    }

    cJSON *ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "ipv4AddressRanges");

    OpenAPI_list_t *ipv4_address_rangesList;
    if (ipv4_address_ranges) {
    cJSON *ipv4_address_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ipv4_address_ranges)){
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv4_address_ranges]");
        goto end;
    }

    ipv4_address_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv4_address_ranges_local_nonprimitive, ipv4_address_ranges ) {
        if (!cJSON_IsObject(ipv4_address_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv4_address_ranges]");
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

    cJSON *ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "ipv6PrefixRanges");

    OpenAPI_list_t *ipv6_prefix_rangesList;
    if (ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ipv6_prefix_ranges)){
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }

    ipv6_prefix_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv6_prefix_ranges_local_nonprimitive, ipv6_prefix_ranges ) {
        if (!cJSON_IsObject(ipv6_prefix_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv6_prefix_ranges]");
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

    cJSON *dnai_nw_instance_list = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnaiNwInstanceList");

    OpenAPI_list_t *dnai_nw_instance_listList;
    if (dnai_nw_instance_list) {
    cJSON *dnai_nw_instance_list_local_map;
    if (!cJSON_IsObject(dnai_nw_instance_list)) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_nw_instance_list]");
        goto end;
    }
    dnai_nw_instance_listList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(dnai_nw_instance_list_local_map, dnai_nw_instance_list) {
        cJSON *localMapObject = dnai_nw_instance_list_local_map;
        OpenAPI_list_add(dnai_nw_instance_listList , localMapKeyPair);
    }
    }

    dnn_upf_info_item_local_var = OpenAPI_dnn_upf_info_item_create (
        ogs_strdup(dnn->valuestring),
        dnai_list ? dnai_listList : NULL,
        pdu_session_types ? pdu_session_typesList : NULL,
        ipv4_address_ranges ? ipv4_address_rangesList : NULL,
        ipv6_prefix_ranges ? ipv6_prefix_rangesList : NULL,
        dnai_nw_instance_list ? dnai_nw_instance_listList : NULL
    );

    return dnn_upf_info_item_local_var;
end:
    return NULL;
}

OpenAPI_dnn_upf_info_item_t *OpenAPI_dnn_upf_info_item_copy(OpenAPI_dnn_upf_info_item_t *dst, OpenAPI_dnn_upf_info_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_upf_info_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed");
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

    OpenAPI_dnn_upf_info_item_free(dst);
    dst = OpenAPI_dnn_upf_info_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

