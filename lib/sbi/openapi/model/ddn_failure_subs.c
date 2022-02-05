
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ddn_failure_subs.h"

OpenAPI_ddn_failure_subs_t *OpenAPI_ddn_failure_subs_create(
    bool is_ddn_failure_subs_ind,
    int ddn_failure_subs_ind,
    OpenAPI_list_t *ddn_failure_subs_info_list
)
{
    OpenAPI_ddn_failure_subs_t *ddn_failure_subs_local_var = ogs_malloc(sizeof(OpenAPI_ddn_failure_subs_t));
    ogs_assert(ddn_failure_subs_local_var);

    ddn_failure_subs_local_var->is_ddn_failure_subs_ind = is_ddn_failure_subs_ind;
    ddn_failure_subs_local_var->ddn_failure_subs_ind = ddn_failure_subs_ind;
    ddn_failure_subs_local_var->ddn_failure_subs_info_list = ddn_failure_subs_info_list;

    return ddn_failure_subs_local_var;
}

void OpenAPI_ddn_failure_subs_free(OpenAPI_ddn_failure_subs_t *ddn_failure_subs)
{
    if (NULL == ddn_failure_subs) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(ddn_failure_subs->ddn_failure_subs_info_list, node) {
        OpenAPI_ddn_failure_sub_info_free(node->data);
    }
    OpenAPI_list_free(ddn_failure_subs->ddn_failure_subs_info_list);
    ogs_free(ddn_failure_subs);
}

cJSON *OpenAPI_ddn_failure_subs_convertToJSON(OpenAPI_ddn_failure_subs_t *ddn_failure_subs)
{
    cJSON *item = NULL;

    if (ddn_failure_subs == NULL) {
        ogs_error("OpenAPI_ddn_failure_subs_convertToJSON() failed [DdnFailureSubs]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ddn_failure_subs->is_ddn_failure_subs_ind) {
    if (cJSON_AddBoolToObject(item, "ddnFailureSubsInd", ddn_failure_subs->ddn_failure_subs_ind) == NULL) {
        ogs_error("OpenAPI_ddn_failure_subs_convertToJSON() failed [ddn_failure_subs_ind]");
        goto end;
    }
    }

    if (ddn_failure_subs->ddn_failure_subs_info_list) {
    cJSON *ddn_failure_subs_info_listList = cJSON_AddArrayToObject(item, "ddnFailureSubsInfoList");
    if (ddn_failure_subs_info_listList == NULL) {
        ogs_error("OpenAPI_ddn_failure_subs_convertToJSON() failed [ddn_failure_subs_info_list]");
        goto end;
    }

    OpenAPI_lnode_t *ddn_failure_subs_info_list_node;
    if (ddn_failure_subs->ddn_failure_subs_info_list) {
        OpenAPI_list_for_each(ddn_failure_subs->ddn_failure_subs_info_list, ddn_failure_subs_info_list_node) {
            cJSON *itemLocal = OpenAPI_ddn_failure_sub_info_convertToJSON(ddn_failure_subs_info_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ddn_failure_subs_convertToJSON() failed [ddn_failure_subs_info_list]");
                goto end;
            }
            cJSON_AddItemToArray(ddn_failure_subs_info_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_ddn_failure_subs_t *OpenAPI_ddn_failure_subs_parseFromJSON(cJSON *ddn_failure_subsJSON)
{
    OpenAPI_ddn_failure_subs_t *ddn_failure_subs_local_var = NULL;
    cJSON *ddn_failure_subs_ind = cJSON_GetObjectItemCaseSensitive(ddn_failure_subsJSON, "ddnFailureSubsInd");

    if (ddn_failure_subs_ind) {
    if (!cJSON_IsBool(ddn_failure_subs_ind)) {
        ogs_error("OpenAPI_ddn_failure_subs_parseFromJSON() failed [ddn_failure_subs_ind]");
        goto end;
    }
    }

    cJSON *ddn_failure_subs_info_list = cJSON_GetObjectItemCaseSensitive(ddn_failure_subsJSON, "ddnFailureSubsInfoList");

    OpenAPI_list_t *ddn_failure_subs_info_listList;
    if (ddn_failure_subs_info_list) {
    cJSON *ddn_failure_subs_info_list_local_nonprimitive;
    if (!cJSON_IsArray(ddn_failure_subs_info_list)){
        ogs_error("OpenAPI_ddn_failure_subs_parseFromJSON() failed [ddn_failure_subs_info_list]");
        goto end;
    }

    ddn_failure_subs_info_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(ddn_failure_subs_info_list_local_nonprimitive, ddn_failure_subs_info_list ) {
        if (!cJSON_IsObject(ddn_failure_subs_info_list_local_nonprimitive)) {
            ogs_error("OpenAPI_ddn_failure_subs_parseFromJSON() failed [ddn_failure_subs_info_list]");
            goto end;
        }
        OpenAPI_ddn_failure_sub_info_t *ddn_failure_subs_info_listItem = OpenAPI_ddn_failure_sub_info_parseFromJSON(ddn_failure_subs_info_list_local_nonprimitive);

        if (!ddn_failure_subs_info_listItem) {
            ogs_error("No ddn_failure_subs_info_listItem");
            OpenAPI_list_free(ddn_failure_subs_info_listList);
            goto end;
        }

        OpenAPI_list_add(ddn_failure_subs_info_listList, ddn_failure_subs_info_listItem);
    }
    }

    ddn_failure_subs_local_var = OpenAPI_ddn_failure_subs_create (
        ddn_failure_subs_ind ? true : false,
        ddn_failure_subs_ind ? ddn_failure_subs_ind->valueint : 0,
        ddn_failure_subs_info_list ? ddn_failure_subs_info_listList : NULL
    );

    return ddn_failure_subs_local_var;
end:
    return NULL;
}

OpenAPI_ddn_failure_subs_t *OpenAPI_ddn_failure_subs_copy(OpenAPI_ddn_failure_subs_t *dst, OpenAPI_ddn_failure_subs_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ddn_failure_subs_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ddn_failure_subs_convertToJSON() failed");
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

    OpenAPI_ddn_failure_subs_free(dst);
    dst = OpenAPI_ddn_failure_subs_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

