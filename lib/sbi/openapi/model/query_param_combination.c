
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "query_param_combination.h"

OpenAPI_query_param_combination_t *OpenAPI_query_param_combination_create(
    OpenAPI_list_t *query_params
)
{
    OpenAPI_query_param_combination_t *query_param_combination_local_var = ogs_malloc(sizeof(OpenAPI_query_param_combination_t));
    ogs_assert(query_param_combination_local_var);

    query_param_combination_local_var->query_params = query_params;

    return query_param_combination_local_var;
}

void OpenAPI_query_param_combination_free(OpenAPI_query_param_combination_t *query_param_combination)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == query_param_combination) {
        return;
    }
    if (query_param_combination->query_params) {
        OpenAPI_list_for_each(query_param_combination->query_params, node) {
            OpenAPI_query_parameter_free(node->data);
        }
        OpenAPI_list_free(query_param_combination->query_params);
        query_param_combination->query_params = NULL;
    }
    ogs_free(query_param_combination);
}

cJSON *OpenAPI_query_param_combination_convertToJSON(OpenAPI_query_param_combination_t *query_param_combination)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (query_param_combination == NULL) {
        ogs_error("OpenAPI_query_param_combination_convertToJSON() failed [QueryParamCombination]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!query_param_combination->query_params) {
        ogs_error("OpenAPI_query_param_combination_convertToJSON() failed [query_params]");
        return NULL;
    }
    cJSON *query_paramsList = cJSON_AddArrayToObject(item, "queryParams");
    if (query_paramsList == NULL) {
        ogs_error("OpenAPI_query_param_combination_convertToJSON() failed [query_params]");
        goto end;
    }
    OpenAPI_list_for_each(query_param_combination->query_params, node) {
        cJSON *itemLocal = OpenAPI_query_parameter_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_query_param_combination_convertToJSON() failed [query_params]");
            goto end;
        }
        cJSON_AddItemToArray(query_paramsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_query_param_combination_t *OpenAPI_query_param_combination_parseFromJSON(cJSON *query_param_combinationJSON)
{
    OpenAPI_query_param_combination_t *query_param_combination_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *query_params = NULL;
    OpenAPI_list_t *query_paramsList = NULL;
    query_params = cJSON_GetObjectItemCaseSensitive(query_param_combinationJSON, "queryParams");
    if (!query_params) {
        ogs_error("OpenAPI_query_param_combination_parseFromJSON() failed [query_params]");
        goto end;
    }
        cJSON *query_params_local = NULL;
        if (!cJSON_IsArray(query_params)) {
            ogs_error("OpenAPI_query_param_combination_parseFromJSON() failed [query_params]");
            goto end;
        }

        query_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(query_params_local, query_params) {
            if (!cJSON_IsObject(query_params_local)) {
                ogs_error("OpenAPI_query_param_combination_parseFromJSON() failed [query_params]");
                goto end;
            }
            OpenAPI_query_parameter_t *query_paramsItem = OpenAPI_query_parameter_parseFromJSON(query_params_local);
            if (!query_paramsItem) {
                ogs_error("No query_paramsItem");
                goto end;
            }
            OpenAPI_list_add(query_paramsList, query_paramsItem);
        }

    query_param_combination_local_var = OpenAPI_query_param_combination_create (
        query_paramsList
    );

    return query_param_combination_local_var;
end:
    if (query_paramsList) {
        OpenAPI_list_for_each(query_paramsList, node) {
            OpenAPI_query_parameter_free(node->data);
        }
        OpenAPI_list_free(query_paramsList);
        query_paramsList = NULL;
    }
    return NULL;
}

OpenAPI_query_param_combination_t *OpenAPI_query_param_combination_copy(OpenAPI_query_param_combination_t *dst, OpenAPI_query_param_combination_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_query_param_combination_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_query_param_combination_convertToJSON() failed");
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

    OpenAPI_query_param_combination_free(dst);
    dst = OpenAPI_query_param_combination_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

