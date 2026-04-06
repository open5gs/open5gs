
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "no_profile_match_info_1.h"

OpenAPI_no_profile_match_info_1_t *OpenAPI_no_profile_match_info_1_create(
    OpenAPI_no_profile_match_reason_e reason,
    OpenAPI_list_t *query_param_combination_list
)
{
    OpenAPI_no_profile_match_info_1_t *no_profile_match_info_1_local_var = ogs_malloc(sizeof(OpenAPI_no_profile_match_info_1_t));
    ogs_assert(no_profile_match_info_1_local_var);

    no_profile_match_info_1_local_var->reason = reason;
    no_profile_match_info_1_local_var->query_param_combination_list = query_param_combination_list;

    return no_profile_match_info_1_local_var;
}

void OpenAPI_no_profile_match_info_1_free(OpenAPI_no_profile_match_info_1_t *no_profile_match_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == no_profile_match_info_1) {
        return;
    }
    if (no_profile_match_info_1->query_param_combination_list) {
        OpenAPI_list_for_each(no_profile_match_info_1->query_param_combination_list, node) {
            OpenAPI_query_param_combination_free(node->data);
        }
        OpenAPI_list_free(no_profile_match_info_1->query_param_combination_list);
        no_profile_match_info_1->query_param_combination_list = NULL;
    }
    ogs_free(no_profile_match_info_1);
}

cJSON *OpenAPI_no_profile_match_info_1_convertToJSON(OpenAPI_no_profile_match_info_1_t *no_profile_match_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (no_profile_match_info_1 == NULL) {
        ogs_error("OpenAPI_no_profile_match_info_1_convertToJSON() failed [NoProfileMatchInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (no_profile_match_info_1->reason == OpenAPI_no_profile_match_reason_NULL) {
        ogs_error("OpenAPI_no_profile_match_info_1_convertToJSON() failed [reason]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "reason", OpenAPI_no_profile_match_reason_ToString(no_profile_match_info_1->reason)) == NULL) {
        ogs_error("OpenAPI_no_profile_match_info_1_convertToJSON() failed [reason]");
        goto end;
    }

    if (no_profile_match_info_1->query_param_combination_list) {
    cJSON *query_param_combination_listList = cJSON_AddArrayToObject(item, "queryParamCombinationList");
    if (query_param_combination_listList == NULL) {
        ogs_error("OpenAPI_no_profile_match_info_1_convertToJSON() failed [query_param_combination_list]");
        goto end;
    }
    OpenAPI_list_for_each(no_profile_match_info_1->query_param_combination_list, node) {
        cJSON *itemLocal = OpenAPI_query_param_combination_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_no_profile_match_info_1_convertToJSON() failed [query_param_combination_list]");
            goto end;
        }
        cJSON_AddItemToArray(query_param_combination_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_no_profile_match_info_1_t *OpenAPI_no_profile_match_info_1_parseFromJSON(cJSON *no_profile_match_info_1JSON)
{
    OpenAPI_no_profile_match_info_1_t *no_profile_match_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *reason = NULL;
    OpenAPI_no_profile_match_reason_e reasonVariable = 0;
    cJSON *query_param_combination_list = NULL;
    OpenAPI_list_t *query_param_combination_listList = NULL;
    reason = cJSON_GetObjectItemCaseSensitive(no_profile_match_info_1JSON, "reason");
    if (!reason) {
        ogs_error("OpenAPI_no_profile_match_info_1_parseFromJSON() failed [reason]");
        goto end;
    }
    if (!cJSON_IsString(reason)) {
        ogs_error("OpenAPI_no_profile_match_info_1_parseFromJSON() failed [reason]");
        goto end;
    }
    reasonVariable = OpenAPI_no_profile_match_reason_FromString(reason->valuestring);

    query_param_combination_list = cJSON_GetObjectItemCaseSensitive(no_profile_match_info_1JSON, "queryParamCombinationList");
    if (query_param_combination_list) {
        cJSON *query_param_combination_list_local = NULL;
        if (!cJSON_IsArray(query_param_combination_list)) {
            ogs_error("OpenAPI_no_profile_match_info_1_parseFromJSON() failed [query_param_combination_list]");
            goto end;
        }

        query_param_combination_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(query_param_combination_list_local, query_param_combination_list) {
            if (!cJSON_IsObject(query_param_combination_list_local)) {
                ogs_error("OpenAPI_no_profile_match_info_1_parseFromJSON() failed [query_param_combination_list]");
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

    no_profile_match_info_1_local_var = OpenAPI_no_profile_match_info_1_create (
        reasonVariable,
        query_param_combination_list ? query_param_combination_listList : NULL
    );

    return no_profile_match_info_1_local_var;
end:
    if (query_param_combination_listList) {
        OpenAPI_list_for_each(query_param_combination_listList, node) {
            OpenAPI_query_param_combination_free(node->data);
        }
        OpenAPI_list_free(query_param_combination_listList);
        query_param_combination_listList = NULL;
    }
    return NULL;
}

OpenAPI_no_profile_match_info_1_t *OpenAPI_no_profile_match_info_1_copy(OpenAPI_no_profile_match_info_1_t *dst, OpenAPI_no_profile_match_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_no_profile_match_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_no_profile_match_info_1_convertToJSON() failed");
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

    OpenAPI_no_profile_match_info_1_free(dst);
    dst = OpenAPI_no_profile_match_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

