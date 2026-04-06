
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_smf_info_item.h"

OpenAPI_dnn_smf_info_item_t *OpenAPI_dnn_smf_info_item_create(
    char *dnn,
    OpenAPI_list_t *dnai_list,
    OpenAPI_list_t *ue_plmn_range_list,
    OpenAPI_list_t *ipv4_index_list,
    OpenAPI_list_t *ipv6_index_list
)
{
    OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item_local_var = ogs_malloc(sizeof(OpenAPI_dnn_smf_info_item_t));
    ogs_assert(dnn_smf_info_item_local_var);

    dnn_smf_info_item_local_var->dnn = dnn;
    dnn_smf_info_item_local_var->dnai_list = dnai_list;
    dnn_smf_info_item_local_var->ue_plmn_range_list = ue_plmn_range_list;
    dnn_smf_info_item_local_var->ipv4_index_list = ipv4_index_list;
    dnn_smf_info_item_local_var->ipv6_index_list = ipv6_index_list;

    return dnn_smf_info_item_local_var;
}

void OpenAPI_dnn_smf_info_item_free(OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_smf_info_item) {
        return;
    }
    if (dnn_smf_info_item->dnn) {
        ogs_free(dnn_smf_info_item->dnn);
        dnn_smf_info_item->dnn = NULL;
    }
    if (dnn_smf_info_item->dnai_list) {
        OpenAPI_list_for_each(dnn_smf_info_item->dnai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_smf_info_item->dnai_list);
        dnn_smf_info_item->dnai_list = NULL;
    }
    if (dnn_smf_info_item->ue_plmn_range_list) {
        OpenAPI_list_for_each(dnn_smf_info_item->ue_plmn_range_list, node) {
            OpenAPI_plmn_range_free(node->data);
        }
        OpenAPI_list_free(dnn_smf_info_item->ue_plmn_range_list);
        dnn_smf_info_item->ue_plmn_range_list = NULL;
    }
    if (dnn_smf_info_item->ipv4_index_list) {
        OpenAPI_list_for_each(dnn_smf_info_item->ipv4_index_list, node) {
            OpenAPI_ip_index_free(node->data);
        }
        OpenAPI_list_free(dnn_smf_info_item->ipv4_index_list);
        dnn_smf_info_item->ipv4_index_list = NULL;
    }
    if (dnn_smf_info_item->ipv6_index_list) {
        OpenAPI_list_for_each(dnn_smf_info_item->ipv6_index_list, node) {
            OpenAPI_ip_index_free(node->data);
        }
        OpenAPI_list_free(dnn_smf_info_item->ipv6_index_list);
        dnn_smf_info_item->ipv6_index_list = NULL;
    }
    ogs_free(dnn_smf_info_item);
}

cJSON *OpenAPI_dnn_smf_info_item_convertToJSON(OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_smf_info_item == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [DnnSmfInfoItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_smf_info_item->dnn) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_smf_info_item->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_smf_info_item->dnai_list) {
    cJSON *dnai_listList = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_listList == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_smf_info_item->dnai_list, node) {
        if (cJSON_AddStringToObject(dnai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnai_list]");
            goto end;
        }
    }
    }

    if (dnn_smf_info_item->ue_plmn_range_list) {
    cJSON *ue_plmn_range_listList = cJSON_AddArrayToObject(item, "uePlmnRangeList");
    if (ue_plmn_range_listList == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [ue_plmn_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_smf_info_item->ue_plmn_range_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [ue_plmn_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(ue_plmn_range_listList, itemLocal);
    }
    }

    if (dnn_smf_info_item->ipv4_index_list) {
    cJSON *ipv4_index_listList = cJSON_AddArrayToObject(item, "ipv4IndexList");
    if (ipv4_index_listList == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [ipv4_index_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_smf_info_item->ipv4_index_list, node) {
        cJSON *itemLocal = OpenAPI_ip_index_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [ipv4_index_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_index_listList, itemLocal);
    }
    }

    if (dnn_smf_info_item->ipv6_index_list) {
    cJSON *ipv6_index_listList = cJSON_AddArrayToObject(item, "ipv6IndexList");
    if (ipv6_index_listList == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [ipv6_index_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_smf_info_item->ipv6_index_list, node) {
        cJSON *itemLocal = OpenAPI_ip_index_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [ipv6_index_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_index_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_dnn_smf_info_item_t *OpenAPI_dnn_smf_info_item_parseFromJSON(cJSON *dnn_smf_info_itemJSON)
{
    OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *dnai_list = NULL;
    OpenAPI_list_t *dnai_listList = NULL;
    cJSON *ue_plmn_range_list = NULL;
    OpenAPI_list_t *ue_plmn_range_listList = NULL;
    cJSON *ipv4_index_list = NULL;
    OpenAPI_list_t *ipv4_index_listList = NULL;
    cJSON *ipv6_index_list = NULL;
    OpenAPI_list_t *ipv6_index_listList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_smf_info_itemJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }

    dnai_list = cJSON_GetObjectItemCaseSensitive(dnn_smf_info_itemJSON, "dnaiList");
    if (dnai_list) {
        cJSON *dnai_list_local = NULL;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnai_list]");
            goto end;
        }

        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    ue_plmn_range_list = cJSON_GetObjectItemCaseSensitive(dnn_smf_info_itemJSON, "uePlmnRangeList");
    if (ue_plmn_range_list) {
        cJSON *ue_plmn_range_list_local = NULL;
        if (!cJSON_IsArray(ue_plmn_range_list)) {
            ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [ue_plmn_range_list]");
            goto end;
        }

        ue_plmn_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_plmn_range_list_local, ue_plmn_range_list) {
            if (!cJSON_IsObject(ue_plmn_range_list_local)) {
                ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [ue_plmn_range_list]");
                goto end;
            }
            OpenAPI_plmn_range_t *ue_plmn_range_listItem = OpenAPI_plmn_range_parseFromJSON(ue_plmn_range_list_local);
            if (!ue_plmn_range_listItem) {
                ogs_error("No ue_plmn_range_listItem");
                goto end;
            }
            OpenAPI_list_add(ue_plmn_range_listList, ue_plmn_range_listItem);
        }
    }

    ipv4_index_list = cJSON_GetObjectItemCaseSensitive(dnn_smf_info_itemJSON, "ipv4IndexList");
    if (ipv4_index_list) {
        cJSON *ipv4_index_list_local = NULL;
        if (!cJSON_IsArray(ipv4_index_list)) {
            ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [ipv4_index_list]");
            goto end;
        }

        ipv4_index_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_index_list_local, ipv4_index_list) {
            if (!cJSON_IsObject(ipv4_index_list_local)) {
                ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [ipv4_index_list]");
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

    ipv6_index_list = cJSON_GetObjectItemCaseSensitive(dnn_smf_info_itemJSON, "ipv6IndexList");
    if (ipv6_index_list) {
        cJSON *ipv6_index_list_local = NULL;
        if (!cJSON_IsArray(ipv6_index_list)) {
            ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [ipv6_index_list]");
            goto end;
        }

        ipv6_index_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_index_list_local, ipv6_index_list) {
            if (!cJSON_IsObject(ipv6_index_list_local)) {
                ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [ipv6_index_list]");
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

    dnn_smf_info_item_local_var = OpenAPI_dnn_smf_info_item_create (
        ogs_strdup(dnn->valuestring),
        dnai_list ? dnai_listList : NULL,
        ue_plmn_range_list ? ue_plmn_range_listList : NULL,
        ipv4_index_list ? ipv4_index_listList : NULL,
        ipv6_index_list ? ipv6_index_listList : NULL
    );

    return dnn_smf_info_item_local_var;
end:
    if (dnai_listList) {
        OpenAPI_list_for_each(dnai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnai_listList);
        dnai_listList = NULL;
    }
    if (ue_plmn_range_listList) {
        OpenAPI_list_for_each(ue_plmn_range_listList, node) {
            OpenAPI_plmn_range_free(node->data);
        }
        OpenAPI_list_free(ue_plmn_range_listList);
        ue_plmn_range_listList = NULL;
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
    return NULL;
}

OpenAPI_dnn_smf_info_item_t *OpenAPI_dnn_smf_info_item_copy(OpenAPI_dnn_smf_info_item_t *dst, OpenAPI_dnn_smf_info_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_smf_info_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed");
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

    OpenAPI_dnn_smf_info_item_free(dst);
    dst = OpenAPI_dnn_smf_info_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

