
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "immediate_report_1.h"

OpenAPI_immediate_report_1_t *OpenAPI_immediate_report_1_create(
    OpenAPI_provisioned_data_sets_t *provisioned_data_sets,
    OpenAPI_list_t *shared_data_list
)
{
    OpenAPI_immediate_report_1_t *immediate_report_1_local_var = ogs_malloc(sizeof(OpenAPI_immediate_report_1_t));
    ogs_assert(immediate_report_1_local_var);

    immediate_report_1_local_var->provisioned_data_sets = provisioned_data_sets;
    immediate_report_1_local_var->shared_data_list = shared_data_list;

    return immediate_report_1_local_var;
}

void OpenAPI_immediate_report_1_free(OpenAPI_immediate_report_1_t *immediate_report_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == immediate_report_1) {
        return;
    }
    if (immediate_report_1->provisioned_data_sets) {
        OpenAPI_provisioned_data_sets_free(immediate_report_1->provisioned_data_sets);
        immediate_report_1->provisioned_data_sets = NULL;
    }
    if (immediate_report_1->shared_data_list) {
        OpenAPI_list_for_each(immediate_report_1->shared_data_list, node) {
            OpenAPI_shared_data_1_free(node->data);
        }
        OpenAPI_list_free(immediate_report_1->shared_data_list);
        immediate_report_1->shared_data_list = NULL;
    }
    ogs_free(immediate_report_1);
}

cJSON *OpenAPI_immediate_report_1_convertToJSON(OpenAPI_immediate_report_1_t *immediate_report_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (immediate_report_1 == NULL) {
        ogs_error("OpenAPI_immediate_report_1_convertToJSON() failed [ImmediateReport_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (immediate_report_1->provisioned_data_sets) {
    cJSON *provisioned_data_sets_local_JSON = OpenAPI_provisioned_data_sets_convertToJSON(immediate_report_1->provisioned_data_sets);
    if (provisioned_data_sets_local_JSON == NULL) {
        ogs_error("OpenAPI_immediate_report_1_convertToJSON() failed [provisioned_data_sets]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ProvisionedDataSets", provisioned_data_sets_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_immediate_report_1_convertToJSON() failed [provisioned_data_sets]");
        goto end;
    }
    }

    if (immediate_report_1->shared_data_list) {
    cJSON *shared_data_listList = cJSON_AddArrayToObject(item, "SharedDataList");
    if (shared_data_listList == NULL) {
        ogs_error("OpenAPI_immediate_report_1_convertToJSON() failed [shared_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(immediate_report_1->shared_data_list, node) {
        cJSON *itemLocal = OpenAPI_shared_data_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_immediate_report_1_convertToJSON() failed [shared_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(shared_data_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_immediate_report_1_t *OpenAPI_immediate_report_1_parseFromJSON(cJSON *immediate_report_1JSON)
{
    OpenAPI_immediate_report_1_t *immediate_report_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *provisioned_data_sets = NULL;
    OpenAPI_provisioned_data_sets_t *provisioned_data_sets_local_nonprim = NULL;
    cJSON *shared_data_list = NULL;
    OpenAPI_list_t *shared_data_listList = NULL;
    provisioned_data_sets = cJSON_GetObjectItemCaseSensitive(immediate_report_1JSON, "ProvisionedDataSets");
    if (provisioned_data_sets) {
    provisioned_data_sets_local_nonprim = OpenAPI_provisioned_data_sets_parseFromJSON(provisioned_data_sets);
    if (!provisioned_data_sets_local_nonprim) {
        ogs_error("OpenAPI_provisioned_data_sets_parseFromJSON failed [provisioned_data_sets]");
        goto end;
    }
    }

    shared_data_list = cJSON_GetObjectItemCaseSensitive(immediate_report_1JSON, "SharedDataList");
    if (shared_data_list) {
        cJSON *shared_data_list_local = NULL;
        if (!cJSON_IsArray(shared_data_list)) {
            ogs_error("OpenAPI_immediate_report_1_parseFromJSON() failed [shared_data_list]");
            goto end;
        }

        shared_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_data_list_local, shared_data_list) {
            if (!cJSON_IsObject(shared_data_list_local)) {
                ogs_error("OpenAPI_immediate_report_1_parseFromJSON() failed [shared_data_list]");
                goto end;
            }
            OpenAPI_shared_data_1_t *shared_data_listItem = OpenAPI_shared_data_1_parseFromJSON(shared_data_list_local);
            if (!shared_data_listItem) {
                ogs_error("No shared_data_listItem");
                goto end;
            }
            OpenAPI_list_add(shared_data_listList, shared_data_listItem);
        }
    }

    immediate_report_1_local_var = OpenAPI_immediate_report_1_create (
        provisioned_data_sets ? provisioned_data_sets_local_nonprim : NULL,
        shared_data_list ? shared_data_listList : NULL
    );

    return immediate_report_1_local_var;
end:
    if (provisioned_data_sets_local_nonprim) {
        OpenAPI_provisioned_data_sets_free(provisioned_data_sets_local_nonprim);
        provisioned_data_sets_local_nonprim = NULL;
    }
    if (shared_data_listList) {
        OpenAPI_list_for_each(shared_data_listList, node) {
            OpenAPI_shared_data_1_free(node->data);
        }
        OpenAPI_list_free(shared_data_listList);
        shared_data_listList = NULL;
    }
    return NULL;
}

OpenAPI_immediate_report_1_t *OpenAPI_immediate_report_1_copy(OpenAPI_immediate_report_1_t *dst, OpenAPI_immediate_report_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_immediate_report_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_immediate_report_1_convertToJSON() failed");
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

    OpenAPI_immediate_report_1_free(dst);
    dst = OpenAPI_immediate_report_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

