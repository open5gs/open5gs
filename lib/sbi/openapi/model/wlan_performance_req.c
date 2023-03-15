
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_performance_req.h"

OpenAPI_wlan_performance_req_t *OpenAPI_wlan_performance_req_create(
    OpenAPI_list_t *ss_ids,
    OpenAPI_list_t *bss_ids,
    OpenAPI_wlan_ordering_criterion_t *wlan_order_criter,
    OpenAPI_matching_direction_t *order
)
{
    OpenAPI_wlan_performance_req_t *wlan_performance_req_local_var = ogs_malloc(sizeof(OpenAPI_wlan_performance_req_t));
    ogs_assert(wlan_performance_req_local_var);

    wlan_performance_req_local_var->ss_ids = ss_ids;
    wlan_performance_req_local_var->bss_ids = bss_ids;
    wlan_performance_req_local_var->wlan_order_criter = wlan_order_criter;
    wlan_performance_req_local_var->order = order;

    return wlan_performance_req_local_var;
}

void OpenAPI_wlan_performance_req_free(OpenAPI_wlan_performance_req_t *wlan_performance_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == wlan_performance_req) {
        return;
    }
    if (wlan_performance_req->ss_ids) {
        OpenAPI_list_for_each(wlan_performance_req->ss_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(wlan_performance_req->ss_ids);
        wlan_performance_req->ss_ids = NULL;
    }
    if (wlan_performance_req->bss_ids) {
        OpenAPI_list_for_each(wlan_performance_req->bss_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(wlan_performance_req->bss_ids);
        wlan_performance_req->bss_ids = NULL;
    }
    if (wlan_performance_req->wlan_order_criter) {
        OpenAPI_wlan_ordering_criterion_free(wlan_performance_req->wlan_order_criter);
        wlan_performance_req->wlan_order_criter = NULL;
    }
    if (wlan_performance_req->order) {
        OpenAPI_matching_direction_free(wlan_performance_req->order);
        wlan_performance_req->order = NULL;
    }
    ogs_free(wlan_performance_req);
}

cJSON *OpenAPI_wlan_performance_req_convertToJSON(OpenAPI_wlan_performance_req_t *wlan_performance_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (wlan_performance_req == NULL) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [WlanPerformanceReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (wlan_performance_req->ss_ids) {
    cJSON *ss_idsList = cJSON_AddArrayToObject(item, "ssIds");
    if (ss_idsList == NULL) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [ss_ids]");
        goto end;
    }
    OpenAPI_list_for_each(wlan_performance_req->ss_ids, node) {
        if (cJSON_AddStringToObject(ss_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [ss_ids]");
            goto end;
        }
    }
    }

    if (wlan_performance_req->bss_ids) {
    cJSON *bss_idsList = cJSON_AddArrayToObject(item, "bssIds");
    if (bss_idsList == NULL) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [bss_ids]");
        goto end;
    }
    OpenAPI_list_for_each(wlan_performance_req->bss_ids, node) {
        if (cJSON_AddStringToObject(bss_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [bss_ids]");
            goto end;
        }
    }
    }

    if (wlan_performance_req->wlan_order_criter) {
    cJSON *wlan_order_criter_local_JSON = OpenAPI_wlan_ordering_criterion_convertToJSON(wlan_performance_req->wlan_order_criter);
    if (wlan_order_criter_local_JSON == NULL) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [wlan_order_criter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wlanOrderCriter", wlan_order_criter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [wlan_order_criter]");
        goto end;
    }
    }

    if (wlan_performance_req->order) {
    cJSON *order_local_JSON = OpenAPI_matching_direction_convertToJSON(wlan_performance_req->order);
    if (order_local_JSON == NULL) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [order]");
        goto end;
    }
    cJSON_AddItemToObject(item, "order", order_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed [order]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_wlan_performance_req_t *OpenAPI_wlan_performance_req_parseFromJSON(cJSON *wlan_performance_reqJSON)
{
    OpenAPI_wlan_performance_req_t *wlan_performance_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ss_ids = NULL;
    OpenAPI_list_t *ss_idsList = NULL;
    cJSON *bss_ids = NULL;
    OpenAPI_list_t *bss_idsList = NULL;
    cJSON *wlan_order_criter = NULL;
    OpenAPI_wlan_ordering_criterion_t *wlan_order_criter_local_nonprim = NULL;
    cJSON *order = NULL;
    OpenAPI_matching_direction_t *order_local_nonprim = NULL;
    ss_ids = cJSON_GetObjectItemCaseSensitive(wlan_performance_reqJSON, "ssIds");
    if (ss_ids) {
        cJSON *ss_ids_local = NULL;
        if (!cJSON_IsArray(ss_ids)) {
            ogs_error("OpenAPI_wlan_performance_req_parseFromJSON() failed [ss_ids]");
            goto end;
        }

        ss_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ss_ids_local, ss_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ss_ids_local)) {
                ogs_error("OpenAPI_wlan_performance_req_parseFromJSON() failed [ss_ids]");
                goto end;
            }
            OpenAPI_list_add(ss_idsList, ogs_strdup(ss_ids_local->valuestring));
        }
    }

    bss_ids = cJSON_GetObjectItemCaseSensitive(wlan_performance_reqJSON, "bssIds");
    if (bss_ids) {
        cJSON *bss_ids_local = NULL;
        if (!cJSON_IsArray(bss_ids)) {
            ogs_error("OpenAPI_wlan_performance_req_parseFromJSON() failed [bss_ids]");
            goto end;
        }

        bss_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(bss_ids_local, bss_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(bss_ids_local)) {
                ogs_error("OpenAPI_wlan_performance_req_parseFromJSON() failed [bss_ids]");
                goto end;
            }
            OpenAPI_list_add(bss_idsList, ogs_strdup(bss_ids_local->valuestring));
        }
    }

    wlan_order_criter = cJSON_GetObjectItemCaseSensitive(wlan_performance_reqJSON, "wlanOrderCriter");
    if (wlan_order_criter) {
    wlan_order_criter_local_nonprim = OpenAPI_wlan_ordering_criterion_parseFromJSON(wlan_order_criter);
    if (!wlan_order_criter_local_nonprim) {
        ogs_error("OpenAPI_wlan_ordering_criterion_parseFromJSON failed [wlan_order_criter]");
        goto end;
    }
    }

    order = cJSON_GetObjectItemCaseSensitive(wlan_performance_reqJSON, "order");
    if (order) {
    order_local_nonprim = OpenAPI_matching_direction_parseFromJSON(order);
    if (!order_local_nonprim) {
        ogs_error("OpenAPI_matching_direction_parseFromJSON failed [order]");
        goto end;
    }
    }

    wlan_performance_req_local_var = OpenAPI_wlan_performance_req_create (
        ss_ids ? ss_idsList : NULL,
        bss_ids ? bss_idsList : NULL,
        wlan_order_criter ? wlan_order_criter_local_nonprim : NULL,
        order ? order_local_nonprim : NULL
    );

    return wlan_performance_req_local_var;
end:
    if (ss_idsList) {
        OpenAPI_list_for_each(ss_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ss_idsList);
        ss_idsList = NULL;
    }
    if (bss_idsList) {
        OpenAPI_list_for_each(bss_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(bss_idsList);
        bss_idsList = NULL;
    }
    if (wlan_order_criter_local_nonprim) {
        OpenAPI_wlan_ordering_criterion_free(wlan_order_criter_local_nonprim);
        wlan_order_criter_local_nonprim = NULL;
    }
    if (order_local_nonprim) {
        OpenAPI_matching_direction_free(order_local_nonprim);
        order_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_wlan_performance_req_t *OpenAPI_wlan_performance_req_copy(OpenAPI_wlan_performance_req_t *dst, OpenAPI_wlan_performance_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wlan_performance_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wlan_performance_req_convertToJSON() failed");
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

    OpenAPI_wlan_performance_req_free(dst);
    dst = OpenAPI_wlan_performance_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

