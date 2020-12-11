
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_rule_data.h"

OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_list_t *req_data
    )
{
    OpenAPI_requested_rule_data_t *requested_rule_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_requested_rule_data_t));
    if (!requested_rule_data_local_var) {
        return NULL;
    }
    requested_rule_data_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;
    requested_rule_data_local_var->req_data = req_data;

    return requested_rule_data_local_var;
}

void OpenAPI_requested_rule_data_free(OpenAPI_requested_rule_data_t *requested_rule_data)
{
    if (NULL == requested_rule_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(requested_rule_data->ref_pcc_rule_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(requested_rule_data->ref_pcc_rule_ids);
    OpenAPI_list_for_each(requested_rule_data->req_data, node) {
        OpenAPI_requested_rule_data_type_free(node->data);
    }
    OpenAPI_list_free(requested_rule_data->req_data);
    ogs_free(requested_rule_data);
}

cJSON *OpenAPI_requested_rule_data_convertToJSON(OpenAPI_requested_rule_data_t *requested_rule_data)
{
    cJSON *item = NULL;

    if (requested_rule_data == NULL) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [RequestedRuleData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!requested_rule_data->ref_pcc_rule_ids) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    cJSON *ref_pcc_rule_ids = cJSON_AddArrayToObject(item, "refPccRuleIds");
    if (ref_pcc_rule_ids == NULL) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ref_pcc_rule_ids_node;
    OpenAPI_list_for_each(requested_rule_data->ref_pcc_rule_ids, ref_pcc_rule_ids_node)  {
        if (cJSON_AddStringToObject(ref_pcc_rule_ids, "", (char*)ref_pcc_rule_ids_node->data) == NULL) {
            ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
    }

    if (!requested_rule_data->req_data) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [req_data]");
        goto end;
    }
    cJSON *req_dataList = cJSON_AddArrayToObject(item, "reqData");
    if (req_dataList == NULL) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [req_data]");
        goto end;
    }

    OpenAPI_lnode_t *req_data_node;
    if (requested_rule_data->req_data) {
        OpenAPI_list_for_each(requested_rule_data->req_data, req_data_node) {
            cJSON *itemLocal = OpenAPI_requested_rule_data_type_convertToJSON(req_data_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [req_data]");
                goto end;
            }
            cJSON_AddItemToArray(req_dataList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_parseFromJSON(cJSON *requested_rule_dataJSON)
{
    OpenAPI_requested_rule_data_t *requested_rule_data_local_var = NULL;
    cJSON *ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(requested_rule_dataJSON, "refPccRuleIds");
    if (!ref_pcc_rule_ids) {
        ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }

    OpenAPI_list_t *ref_pcc_rule_idsList;

    cJSON *ref_pcc_rule_ids_local;
    if (!cJSON_IsArray(ref_pcc_rule_ids)) {
        ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    ref_pcc_rule_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
        if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
            ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
        OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
    }

    cJSON *req_data = cJSON_GetObjectItemCaseSensitive(requested_rule_dataJSON, "reqData");
    if (!req_data) {
        ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [req_data]");
        goto end;
    }

    OpenAPI_list_t *req_dataList;

    cJSON *req_data_local_nonprimitive;
    if (!cJSON_IsArray(req_data)) {
        ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [req_data]");
        goto end;
    }

    req_dataList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_data_local_nonprimitive, req_data ) {
        if (!cJSON_IsObject(req_data_local_nonprimitive)) {
            ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [req_data]");
            goto end;
        }
        OpenAPI_requested_rule_data_type_t *req_dataItem = OpenAPI_requested_rule_data_type_parseFromJSON(req_data_local_nonprimitive);

        OpenAPI_list_add(req_dataList, req_dataItem);
    }

    requested_rule_data_local_var = OpenAPI_requested_rule_data_create (
        ref_pcc_rule_idsList,
        req_dataList
        );

    return requested_rule_data_local_var;
end:
    return NULL;
}

OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_copy(OpenAPI_requested_rule_data_t *dst, OpenAPI_requested_rule_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_requested_rule_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed");
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

    OpenAPI_requested_rule_data_free(dst);
    dst = OpenAPI_requested_rule_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

