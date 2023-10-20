
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_rule_data.h"

OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_list_t *req_data
)
{
    OpenAPI_requested_rule_data_t *requested_rule_data_local_var = ogs_malloc(sizeof(OpenAPI_requested_rule_data_t));
    ogs_assert(requested_rule_data_local_var);

    requested_rule_data_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;
    requested_rule_data_local_var->req_data = req_data;

    return requested_rule_data_local_var;
}

void OpenAPI_requested_rule_data_free(OpenAPI_requested_rule_data_t *requested_rule_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == requested_rule_data) {
        return;
    }
    if (requested_rule_data->ref_pcc_rule_ids) {
        OpenAPI_list_for_each(requested_rule_data->ref_pcc_rule_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(requested_rule_data->ref_pcc_rule_ids);
        requested_rule_data->ref_pcc_rule_ids = NULL;
    }
    if (requested_rule_data->req_data) {
        OpenAPI_list_free(requested_rule_data->req_data);
        requested_rule_data->req_data = NULL;
    }
    ogs_free(requested_rule_data);
}

cJSON *OpenAPI_requested_rule_data_convertToJSON(OpenAPI_requested_rule_data_t *requested_rule_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (requested_rule_data == NULL) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [RequestedRuleData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!requested_rule_data->ref_pcc_rule_ids) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [ref_pcc_rule_ids]");
        return NULL;
    }
    cJSON *ref_pcc_rule_idsList = cJSON_AddArrayToObject(item, "refPccRuleIds");
    if (ref_pcc_rule_idsList == NULL) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    OpenAPI_list_for_each(requested_rule_data->ref_pcc_rule_ids, node) {
        if (cJSON_AddStringToObject(ref_pcc_rule_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
    }

    if (requested_rule_data->req_data == OpenAPI_requested_rule_data_type_NULL) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [req_data]");
        return NULL;
    }
    cJSON *req_dataList = cJSON_AddArrayToObject(item, "reqData");
    if (req_dataList == NULL) {
        ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [req_data]");
        goto end;
    }
    OpenAPI_list_for_each(requested_rule_data->req_data, node) {
        if (cJSON_AddStringToObject(req_dataList, "", OpenAPI_requested_rule_data_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_requested_rule_data_convertToJSON() failed [req_data]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_parseFromJSON(cJSON *requested_rule_dataJSON)
{
    OpenAPI_requested_rule_data_t *requested_rule_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ref_pcc_rule_ids = NULL;
    OpenAPI_list_t *ref_pcc_rule_idsList = NULL;
    cJSON *req_data = NULL;
    OpenAPI_list_t *req_dataList = NULL;
    ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(requested_rule_dataJSON, "refPccRuleIds");
    if (!ref_pcc_rule_ids) {
        ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
        cJSON *ref_pcc_rule_ids_local = NULL;
        if (!cJSON_IsArray(ref_pcc_rule_ids)) {
            ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }

        ref_pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
                ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [ref_pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
        }

    req_data = cJSON_GetObjectItemCaseSensitive(requested_rule_dataJSON, "reqData");
    if (!req_data) {
        ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [req_data]");
        goto end;
    }
        cJSON *req_data_local = NULL;
        if (!cJSON_IsArray(req_data)) {
            ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [req_data]");
            goto end;
        }

        req_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_data_local, req_data) {
            OpenAPI_requested_rule_data_type_e localEnum = OpenAPI_requested_rule_data_type_NULL;
            if (!cJSON_IsString(req_data_local)) {
                ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed [req_data]");
                goto end;
            }
            localEnum = OpenAPI_requested_rule_data_type_FromString(req_data_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_data\" is not supported. Ignoring it ...",
                         req_data_local->valuestring);
            } else {
                OpenAPI_list_add(req_dataList, (void *)localEnum);
            }
        }
        if (req_dataList->count == 0) {
            ogs_error("OpenAPI_requested_rule_data_parseFromJSON() failed: Expected req_dataList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    requested_rule_data_local_var = OpenAPI_requested_rule_data_create (
        ref_pcc_rule_idsList,
        req_dataList
    );

    return requested_rule_data_local_var;
end:
    if (ref_pcc_rule_idsList) {
        OpenAPI_list_for_each(ref_pcc_rule_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_pcc_rule_idsList);
        ref_pcc_rule_idsList = NULL;
    }
    if (req_dataList) {
        OpenAPI_list_free(req_dataList);
        req_dataList = NULL;
    }
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

