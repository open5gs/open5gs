
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "no_profile_match_info.h"

OpenAPI_no_profile_match_info_t *OpenAPI_no_profile_match_info_create(
    OpenAPI_no_profile_match_reason_t *reason,
    OpenAPI_list_t *query_param_combination_list
)
{
    OpenAPI_no_profile_match_info_t *no_profile_match_info_local_var = ogs_malloc(sizeof(OpenAPI_no_profile_match_info_t));
    ogs_assert(no_profile_match_info_local_var);

    no_profile_match_info_local_var->reason = reason;
    no_profile_match_info_local_var->query_param_combination_list = query_param_combination_list;

    return no_profile_match_info_local_var;
}

void OpenAPI_no_profile_match_info_free(OpenAPI_no_profile_match_info_t *no_profile_match_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == no_profile_match_info) {
        return;
    }
    if (no_profile_match_info->reason) {
        OpenAPI_no_profile_match_reason_free(no_profile_match_info->reason);
        no_profile_match_info->reason = NULL;
    }
    if (no_profile_match_info->query_param_combination_list) {
        OpenAPI_list_for_each(no_profile_match_info->query_param_combination_list, node) {
            OpenAPI_query_param_combination_free(node->data);
        }
        OpenAPI_list_free(no_profile_match_info->query_param_combination_list);
        no_profile_match_info->query_param_combination_list = NULL;
    }
    ogs_free(no_profile_match_info);
}

cJSON *OpenAPI_no_profile_match_info_convertToJSON(OpenAPI_no_profile_match_info_t *no_profile_match_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (no_profile_match_info == NULL) {
        ogs_error("OpenAPI_no_profile_match_info_convertToJSON() failed [NoProfileMatchInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!no_profile_match_info->reason) {
        ogs_error("OpenAPI_no_profile_match_info_convertToJSON() failed [reason]");
        return NULL;
    }
    cJSON *reason_local_JSON = OpenAPI_no_profile_match_reason_convertToJSON(no_profile_match_info->reason);
    if (reason_local_JSON == NULL) {
        ogs_error("OpenAPI_no_profile_match_info_convertToJSON() failed [reason]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reason", reason_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_no_profile_match_info_convertToJSON() failed [reason]");
        goto end;
    }

    if (no_profile_match_info->query_param_combination_list) {
    cJSON *query_param_combination_listList = cJSON_AddArrayToObject(item, "queryParamCombinationList");
    if (query_param_combination_listList == NULL) {
        ogs_error("OpenAPI_no_profile_match_info_convertToJSON() failed [query_param_combination_list]");
        goto end;
    }
    OpenAPI_list_for_each(no_profile_match_info->query_param_combination_list, node) {
        cJSON *itemLocal = OpenAPI_query_param_combination_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_no_profile_match_info_convertToJSON() failed [query_param_combination_list]");
            goto end;
        }
        cJSON_AddItemToArray(query_param_combination_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_no_profile_match_info_t *OpenAPI_no_profile_match_info_parseFromJSON(cJSON *no_profile_match_infoJSON)
{
    OpenAPI_no_profile_match_info_t *no_profile_match_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *reason = NULL;
    OpenAPI_no_profile_match_reason_t *reason_local_nonprim = NULL;
    cJSON *query_param_combination_list = NULL;
    OpenAPI_list_t *query_param_combination_listList = NULL;
    reason = cJSON_GetObjectItemCaseSensitive(no_profile_match_infoJSON, "reason");
    if (!reason) {
        ogs_error("OpenAPI_no_profile_match_info_parseFromJSON() failed [reason]");
        goto end;
    }
    reason_local_nonprim = OpenAPI_no_profile_match_reason_parseFromJSON(reason);
    if (!reason_local_nonprim) {
        ogs_error("OpenAPI_no_profile_match_reason_parseFromJSON failed [reason]");
        goto end;
    }

    query_param_combination_list = cJSON_GetObjectItemCaseSensitive(no_profile_match_infoJSON, "queryParamCombinationList");
    if (query_param_combination_list) {
        cJSON *query_param_combination_list_local = NULL;
        if (!cJSON_IsArray(query_param_combination_list)) {
            ogs_error("OpenAPI_no_profile_match_info_parseFromJSON() failed [query_param_combination_list]");
            goto end;
        }

        query_param_combination_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(query_param_combination_list_local, query_param_combination_list) {
            if (!cJSON_IsObject(query_param_combination_list_local)) {
                ogs_error("OpenAPI_no_profile_match_info_parseFromJSON() failed [query_param_combination_list]");
                goto end;
            }
            OpenAPI_query_param_combination_t *query_param_combination_listItem = OpenAPI_query_param_combination_parseFromJSON(query_param_combination_list_local);
            if (!query_param_combination_listItem) {
                ogs_error("No query_param_combination_listItem");
                goto end;
            }
            OpenAPI_list_add(query_param_combination_listList, query_param_combination_listItem);
        }
    }

    no_profile_match_info_local_var = OpenAPI_no_profile_match_info_create (
        reason_local_nonprim,
        query_param_combination_list ? query_param_combination_listList : NULL
    );

    return no_profile_match_info_local_var;
end:
    if (reason_local_nonprim) {
        OpenAPI_no_profile_match_reason_free(reason_local_nonprim);
        reason_local_nonprim = NULL;
    }
    if (query_param_combination_listList) {
        OpenAPI_list_for_each(query_param_combination_listList, node) {
            OpenAPI_query_param_combination_free(node->data);
        }
        OpenAPI_list_free(query_param_combination_listList);
        query_param_combination_listList = NULL;
    }
    return NULL;
}

OpenAPI_no_profile_match_info_t *OpenAPI_no_profile_match_info_copy(OpenAPI_no_profile_match_info_t *dst, OpenAPI_no_profile_match_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_no_profile_match_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_no_profile_match_info_convertToJSON() failed");
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

    OpenAPI_no_profile_match_info_free(dst);
    dst = OpenAPI_no_profile_match_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

