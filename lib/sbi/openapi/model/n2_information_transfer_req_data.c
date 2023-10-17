
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_information_transfer_req_data.h"

OpenAPI_n2_information_transfer_req_data_t *OpenAPI_n2_information_transfer_req_data_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_rat_selector_e rat_selector,
    OpenAPI_list_t *global_ran_node_list,
    OpenAPI_n2_info_container_t *n2_information,
    char *supported_features
)
{
    OpenAPI_n2_information_transfer_req_data_t *n2_information_transfer_req_data_local_var = ogs_malloc(sizeof(OpenAPI_n2_information_transfer_req_data_t));
    ogs_assert(n2_information_transfer_req_data_local_var);

    n2_information_transfer_req_data_local_var->tai_list = tai_list;
    n2_information_transfer_req_data_local_var->rat_selector = rat_selector;
    n2_information_transfer_req_data_local_var->global_ran_node_list = global_ran_node_list;
    n2_information_transfer_req_data_local_var->n2_information = n2_information;
    n2_information_transfer_req_data_local_var->supported_features = supported_features;

    return n2_information_transfer_req_data_local_var;
}

void OpenAPI_n2_information_transfer_req_data_free(OpenAPI_n2_information_transfer_req_data_t *n2_information_transfer_req_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_information_transfer_req_data) {
        return;
    }
    if (n2_information_transfer_req_data->tai_list) {
        OpenAPI_list_for_each(n2_information_transfer_req_data->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(n2_information_transfer_req_data->tai_list);
        n2_information_transfer_req_data->tai_list = NULL;
    }
    if (n2_information_transfer_req_data->global_ran_node_list) {
        OpenAPI_list_for_each(n2_information_transfer_req_data->global_ran_node_list, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(n2_information_transfer_req_data->global_ran_node_list);
        n2_information_transfer_req_data->global_ran_node_list = NULL;
    }
    if (n2_information_transfer_req_data->n2_information) {
        OpenAPI_n2_info_container_free(n2_information_transfer_req_data->n2_information);
        n2_information_transfer_req_data->n2_information = NULL;
    }
    if (n2_information_transfer_req_data->supported_features) {
        ogs_free(n2_information_transfer_req_data->supported_features);
        n2_information_transfer_req_data->supported_features = NULL;
    }
    ogs_free(n2_information_transfer_req_data);
}

cJSON *OpenAPI_n2_information_transfer_req_data_convertToJSON(OpenAPI_n2_information_transfer_req_data_t *n2_information_transfer_req_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_information_transfer_req_data == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [N2InformationTransferReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_information_transfer_req_data->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_information_transfer_req_data->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (n2_information_transfer_req_data->rat_selector != OpenAPI_rat_selector_NULL) {
    if (cJSON_AddStringToObject(item, "ratSelector", OpenAPI_rat_selector_ToString(n2_information_transfer_req_data->rat_selector)) == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [rat_selector]");
        goto end;
    }
    }

    if (n2_information_transfer_req_data->global_ran_node_list) {
    cJSON *global_ran_node_listList = cJSON_AddArrayToObject(item, "globalRanNodeList");
    if (global_ran_node_listList == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [global_ran_node_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_information_transfer_req_data->global_ran_node_list, node) {
        cJSON *itemLocal = OpenAPI_global_ran_node_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [global_ran_node_list]");
            goto end;
        }
        cJSON_AddItemToArray(global_ran_node_listList, itemLocal);
    }
    }

    if (!n2_information_transfer_req_data->n2_information) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [n2_information]");
        return NULL;
    }
    cJSON *n2_information_local_JSON = OpenAPI_n2_info_container_convertToJSON(n2_information_transfer_req_data->n2_information);
    if (n2_information_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [n2_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2Information", n2_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [n2_information]");
        goto end;
    }

    if (n2_information_transfer_req_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", n2_information_transfer_req_data->supported_features) == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_information_transfer_req_data_t *OpenAPI_n2_information_transfer_req_data_parseFromJSON(cJSON *n2_information_transfer_req_dataJSON)
{
    OpenAPI_n2_information_transfer_req_data_t *n2_information_transfer_req_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *rat_selector = NULL;
    OpenAPI_rat_selector_e rat_selectorVariable = 0;
    cJSON *global_ran_node_list = NULL;
    OpenAPI_list_t *global_ran_node_listList = NULL;
    cJSON *n2_information = NULL;
    OpenAPI_n2_info_container_t *n2_information_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    tai_list = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_req_dataJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_n2_information_transfer_req_data_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_n2_information_transfer_req_data_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    rat_selector = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_req_dataJSON, "ratSelector");
    if (rat_selector) {
    if (!cJSON_IsString(rat_selector)) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_parseFromJSON() failed [rat_selector]");
        goto end;
    }
    rat_selectorVariable = OpenAPI_rat_selector_FromString(rat_selector->valuestring);
    }

    global_ran_node_list = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_req_dataJSON, "globalRanNodeList");
    if (global_ran_node_list) {
        cJSON *global_ran_node_list_local = NULL;
        if (!cJSON_IsArray(global_ran_node_list)) {
            ogs_error("OpenAPI_n2_information_transfer_req_data_parseFromJSON() failed [global_ran_node_list]");
            goto end;
        }

        global_ran_node_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(global_ran_node_list_local, global_ran_node_list) {
            if (!cJSON_IsObject(global_ran_node_list_local)) {
                ogs_error("OpenAPI_n2_information_transfer_req_data_parseFromJSON() failed [global_ran_node_list]");
                goto end;
            }
            OpenAPI_global_ran_node_id_t *global_ran_node_listItem = OpenAPI_global_ran_node_id_parseFromJSON(global_ran_node_list_local);
            if (!global_ran_node_listItem) {
                ogs_error("No global_ran_node_listItem");
                goto end;
            }
            OpenAPI_list_add(global_ran_node_listList, global_ran_node_listItem);
        }
    }

    n2_information = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_req_dataJSON, "n2Information");
    if (!n2_information) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_parseFromJSON() failed [n2_information]");
        goto end;
    }
    n2_information_local_nonprim = OpenAPI_n2_info_container_parseFromJSON(n2_information);
    if (!n2_information_local_nonprim) {
        ogs_error("OpenAPI_n2_info_container_parseFromJSON failed [n2_information]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_req_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    n2_information_transfer_req_data_local_var = OpenAPI_n2_information_transfer_req_data_create (
        tai_list ? tai_listList : NULL,
        rat_selector ? rat_selectorVariable : 0,
        global_ran_node_list ? global_ran_node_listList : NULL,
        n2_information_local_nonprim,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return n2_information_transfer_req_data_local_var;
end:
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (global_ran_node_listList) {
        OpenAPI_list_for_each(global_ran_node_listList, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(global_ran_node_listList);
        global_ran_node_listList = NULL;
    }
    if (n2_information_local_nonprim) {
        OpenAPI_n2_info_container_free(n2_information_local_nonprim);
        n2_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n2_information_transfer_req_data_t *OpenAPI_n2_information_transfer_req_data_copy(OpenAPI_n2_information_transfer_req_data_t *dst, OpenAPI_n2_information_transfer_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_information_transfer_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_information_transfer_req_data_convertToJSON() failed");
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

    OpenAPI_n2_information_transfer_req_data_free(dst);
    dst = OpenAPI_n2_information_transfer_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

