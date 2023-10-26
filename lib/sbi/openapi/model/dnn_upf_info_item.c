
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
    OpenAPI_list_t *ipv4_index_list,
    OpenAPI_list_t *ipv6_index_list,
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
    dnn_upf_info_item_local_var->ipv4_index_list = ipv4_index_list;
    dnn_upf_info_item_local_var->ipv6_index_list = ipv6_index_list;
    dnn_upf_info_item_local_var->dnai_nw_instance_list = dnai_nw_instance_list;

    return dnn_upf_info_item_local_var;
}

void OpenAPI_dnn_upf_info_item_free(OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_upf_info_item) {
        return;
    }
    if (dnn_upf_info_item->dnn) {
        ogs_free(dnn_upf_info_item->dnn);
        dnn_upf_info_item->dnn = NULL;
    }
    if (dnn_upf_info_item->dnai_list) {
        OpenAPI_list_for_each(dnn_upf_info_item->dnai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_upf_info_item->dnai_list);
        dnn_upf_info_item->dnai_list = NULL;
    }
    if (dnn_upf_info_item->pdu_session_types) {
        OpenAPI_list_free(dnn_upf_info_item->pdu_session_types);
        dnn_upf_info_item->pdu_session_types = NULL;
    }
    if (dnn_upf_info_item->ipv4_address_ranges) {
        OpenAPI_list_for_each(dnn_upf_info_item->ipv4_address_ranges, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(dnn_upf_info_item->ipv4_address_ranges);
        dnn_upf_info_item->ipv4_address_ranges = NULL;
    }
    if (dnn_upf_info_item->ipv6_prefix_ranges) {
        OpenAPI_list_for_each(dnn_upf_info_item->ipv6_prefix_ranges, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(dnn_upf_info_item->ipv6_prefix_ranges);
        dnn_upf_info_item->ipv6_prefix_ranges = NULL;
    }
    if (dnn_upf_info_item->ipv4_index_list) {
        OpenAPI_list_for_each(dnn_upf_info_item->ipv4_index_list, node) {
            OpenAPI_ip_index_free(node->data);
        }
        OpenAPI_list_free(dnn_upf_info_item->ipv4_index_list);
        dnn_upf_info_item->ipv4_index_list = NULL;
    }
    if (dnn_upf_info_item->ipv6_index_list) {
        OpenAPI_list_for_each(dnn_upf_info_item->ipv6_index_list, node) {
            OpenAPI_ip_index_free(node->data);
        }
        OpenAPI_list_free(dnn_upf_info_item->ipv6_index_list);
        dnn_upf_info_item->ipv6_index_list = NULL;
    }
    if (dnn_upf_info_item->dnai_nw_instance_list) {
        OpenAPI_list_for_each(dnn_upf_info_item->dnai_nw_instance_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(dnn_upf_info_item->dnai_nw_instance_list);
        dnn_upf_info_item->dnai_nw_instance_list = NULL;
    }
    ogs_free(dnn_upf_info_item);
}

cJSON *OpenAPI_dnn_upf_info_item_convertToJSON(OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_upf_info_item == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [DnnUpfInfoItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_upf_info_item->dnn) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_upf_info_item->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_upf_info_item->dnai_list) {
    cJSON *dnai_listList = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_listList == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_upf_info_item->dnai_list, node) {
        if (cJSON_AddStringToObject(dnai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_list]");
            goto end;
        }
    }
    }

    if (dnn_upf_info_item->pdu_session_types != OpenAPI_pdu_session_type_NULL) {
    cJSON *pdu_session_typesList = cJSON_AddArrayToObject(item, "pduSessionTypes");
    if (pdu_session_typesList == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_upf_info_item->pdu_session_types, node) {
        if (cJSON_AddStringToObject(pdu_session_typesList, "", OpenAPI_pdu_session_type_ToString((intptr_t)node->data)) == NULL) {
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
    OpenAPI_list_for_each(dnn_upf_info_item->ipv4_address_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv4_address_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_address_rangesList, itemLocal);
    }
    }

    if (dnn_upf_info_item->ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "ipv6PrefixRanges");
    if (ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_upf_info_item->ipv6_prefix_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_prefix_rangesList, itemLocal);
    }
    }

    if (dnn_upf_info_item->ipv4_index_list) {
    cJSON *ipv4_index_listList = cJSON_AddArrayToObject(item, "ipv4IndexList");
    if (ipv4_index_listList == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv4_index_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_upf_info_item->ipv4_index_list, node) {
        cJSON *itemLocal = OpenAPI_ip_index_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv4_index_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_index_listList, itemLocal);
    }
    }

    if (dnn_upf_info_item->ipv6_index_list) {
    cJSON *ipv6_index_listList = cJSON_AddArrayToObject(item, "ipv6IndexList");
    if (ipv6_index_listList == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv6_index_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_upf_info_item->ipv6_index_list, node) {
        cJSON *itemLocal = OpenAPI_ip_index_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [ipv6_index_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_index_listList, itemLocal);
    }
    }

    if (dnn_upf_info_item->dnai_nw_instance_list) {
    cJSON *dnai_nw_instance_list = cJSON_AddObjectToObject(item, "dnaiNwInstanceList");
    if (dnai_nw_instance_list == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_nw_instance_list]");
        goto end;
    }
    cJSON *localMapObject = dnai_nw_instance_list;
    if (dnn_upf_info_item->dnai_nw_instance_list) {
        OpenAPI_list_for_each(dnn_upf_info_item->dnai_nw_instance_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_nw_instance_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_nw_instance_list]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, (char*)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

end:
    return item;
}

OpenAPI_dnn_upf_info_item_t *OpenAPI_dnn_upf_info_item_parseFromJSON(cJSON *dnn_upf_info_itemJSON)
{
    OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *dnai_list = NULL;
    OpenAPI_list_t *dnai_listList = NULL;
    cJSON *pdu_session_types = NULL;
    OpenAPI_list_t *pdu_session_typesList = NULL;
    cJSON *ipv4_address_ranges = NULL;
    OpenAPI_list_t *ipv4_address_rangesList = NULL;
    cJSON *ipv6_prefix_ranges = NULL;
    OpenAPI_list_t *ipv6_prefix_rangesList = NULL;
    cJSON *ipv4_index_list = NULL;
    OpenAPI_list_t *ipv4_index_listList = NULL;
    cJSON *ipv6_index_list = NULL;
    OpenAPI_list_t *ipv6_index_listList = NULL;
    cJSON *dnai_nw_instance_list = NULL;
    OpenAPI_list_t *dnai_nw_instance_listList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }

    dnai_list = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnaiList");
    if (dnai_list) {
        cJSON *dnai_list_local = NULL;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_list]");
            goto end;
        }

        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    pdu_session_types = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "pduSessionTypes");
    if (pdu_session_types) {
        cJSON *pdu_session_types_local = NULL;
        if (!cJSON_IsArray(pdu_session_types)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [pdu_session_types]");
            goto end;
        }

        pdu_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_types_local, pdu_session_types) {
            OpenAPI_pdu_session_type_e localEnum = OpenAPI_pdu_session_type_NULL;
            if (!cJSON_IsString(pdu_session_types_local)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [pdu_session_types]");
                goto end;
            }
            localEnum = OpenAPI_pdu_session_type_FromString(pdu_session_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"pdu_session_types\" is not supported. Ignoring it ...",
                         pdu_session_types_local->valuestring);
            } else {
                OpenAPI_list_add(pdu_session_typesList, (void *)localEnum);
            }
        }
        if (pdu_session_typesList->count == 0) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed: Expected pdu_session_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "ipv4AddressRanges");
    if (ipv4_address_ranges) {
        cJSON *ipv4_address_ranges_local = NULL;
        if (!cJSON_IsArray(ipv4_address_ranges)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv4_address_ranges]");
            goto end;
        }

        ipv4_address_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_address_ranges_local, ipv4_address_ranges) {
            if (!cJSON_IsObject(ipv4_address_ranges_local)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv4_address_ranges]");
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

    ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "ipv6PrefixRanges");
    if (ipv6_prefix_ranges) {
        cJSON *ipv6_prefix_ranges_local = NULL;
        if (!cJSON_IsArray(ipv6_prefix_ranges)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }

        ipv6_prefix_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_ranges_local, ipv6_prefix_ranges) {
            if (!cJSON_IsObject(ipv6_prefix_ranges_local)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv6_prefix_ranges]");
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

    ipv4_index_list = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "ipv4IndexList");
    if (ipv4_index_list) {
        cJSON *ipv4_index_list_local = NULL;
        if (!cJSON_IsArray(ipv4_index_list)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv4_index_list]");
            goto end;
        }

        ipv4_index_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_index_list_local, ipv4_index_list) {
            if (!cJSON_IsObject(ipv4_index_list_local)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv4_index_list]");
                goto end;
            }
            OpenAPI_ip_index_t *ipv4_index_listItem = OpenAPI_ip_index_parseFromJSON(ipv4_index_list_local);
            if (!ipv4_index_listItem) {
                ogs_error("No ipv4_index_listItem");
                goto end;
            }
            OpenAPI_list_add(ipv4_index_listList, ipv4_index_listItem);
        }
    }

    ipv6_index_list = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "ipv6IndexList");
    if (ipv6_index_list) {
        cJSON *ipv6_index_list_local = NULL;
        if (!cJSON_IsArray(ipv6_index_list)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv6_index_list]");
            goto end;
        }

        ipv6_index_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_index_list_local, ipv6_index_list) {
            if (!cJSON_IsObject(ipv6_index_list_local)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [ipv6_index_list]");
                goto end;
            }
            OpenAPI_ip_index_t *ipv6_index_listItem = OpenAPI_ip_index_parseFromJSON(ipv6_index_list_local);
            if (!ipv6_index_listItem) {
                ogs_error("No ipv6_index_listItem");
                goto end;
            }
            OpenAPI_list_add(ipv6_index_listList, ipv6_index_listItem);
        }
    }

    dnai_nw_instance_list = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnaiNwInstanceList");
    if (dnai_nw_instance_list) {
        cJSON *dnai_nw_instance_list_local_map = NULL;
        if (!cJSON_IsObject(dnai_nw_instance_list) && !cJSON_IsNull(dnai_nw_instance_list)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_nw_instance_list]");
            goto end;
        }
        if (cJSON_IsObject(dnai_nw_instance_list)) {
            dnai_nw_instance_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(dnai_nw_instance_list_local_map, dnai_nw_instance_list) {
                cJSON *localMapObject = dnai_nw_instance_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [inner]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), ogs_strdup(localMapObject->valuestring));
                OpenAPI_list_add(dnai_nw_instance_listList, localMapKeyPair);
            }
        }
    }

    dnn_upf_info_item_local_var = OpenAPI_dnn_upf_info_item_create (
        ogs_strdup(dnn->valuestring),
        dnai_list ? dnai_listList : NULL,
        pdu_session_types ? pdu_session_typesList : NULL,
        ipv4_address_ranges ? ipv4_address_rangesList : NULL,
        ipv6_prefix_ranges ? ipv6_prefix_rangesList : NULL,
        ipv4_index_list ? ipv4_index_listList : NULL,
        ipv6_index_list ? ipv6_index_listList : NULL,
        dnai_nw_instance_list ? dnai_nw_instance_listList : NULL
    );

    return dnn_upf_info_item_local_var;
end:
    if (dnai_listList) {
        OpenAPI_list_for_each(dnai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnai_listList);
        dnai_listList = NULL;
    }
    if (pdu_session_typesList) {
        OpenAPI_list_free(pdu_session_typesList);
        pdu_session_typesList = NULL;
    }
    if (ipv4_address_rangesList) {
        OpenAPI_list_for_each(ipv4_address_rangesList, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(ipv4_address_rangesList);
        ipv4_address_rangesList = NULL;
    }
    if (ipv6_prefix_rangesList) {
        OpenAPI_list_for_each(ipv6_prefix_rangesList, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefix_rangesList);
        ipv6_prefix_rangesList = NULL;
    }
    if (ipv4_index_listList) {
        OpenAPI_list_for_each(ipv4_index_listList, node) {
            OpenAPI_ip_index_free(node->data);
        }
        OpenAPI_list_free(ipv4_index_listList);
        ipv4_index_listList = NULL;
    }
    if (ipv6_index_listList) {
        OpenAPI_list_for_each(ipv6_index_listList, node) {
            OpenAPI_ip_index_free(node->data);
        }
        OpenAPI_list_free(ipv6_index_listList);
        ipv6_index_listList = NULL;
    }
    if (dnai_nw_instance_listList) {
        OpenAPI_list_for_each(dnai_nw_instance_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(dnai_nw_instance_listList);
        dnai_nw_instance_listList = NULL;
    }
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

