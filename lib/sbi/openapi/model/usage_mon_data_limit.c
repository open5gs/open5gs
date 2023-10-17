
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_data_limit.h"

OpenAPI_usage_mon_data_limit_t *OpenAPI_usage_mon_data_limit_create(
    char *limit_id,
    OpenAPI_list_t* scopes,
    OpenAPI_usage_mon_level_t *um_level,
    char *start_date,
    char *end_date,
    OpenAPI_usage_threshold_t *usage_limit,
    OpenAPI_time_period_t *reset_period
)
{
    OpenAPI_usage_mon_data_limit_t *usage_mon_data_limit_local_var = ogs_malloc(sizeof(OpenAPI_usage_mon_data_limit_t));
    ogs_assert(usage_mon_data_limit_local_var);

    usage_mon_data_limit_local_var->limit_id = limit_id;
    usage_mon_data_limit_local_var->scopes = scopes;
    usage_mon_data_limit_local_var->um_level = um_level;
    usage_mon_data_limit_local_var->start_date = start_date;
    usage_mon_data_limit_local_var->end_date = end_date;
    usage_mon_data_limit_local_var->usage_limit = usage_limit;
    usage_mon_data_limit_local_var->reset_period = reset_period;

    return usage_mon_data_limit_local_var;
}

void OpenAPI_usage_mon_data_limit_free(OpenAPI_usage_mon_data_limit_t *usage_mon_data_limit)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == usage_mon_data_limit) {
        return;
    }
    if (usage_mon_data_limit->limit_id) {
        ogs_free(usage_mon_data_limit->limit_id);
        usage_mon_data_limit->limit_id = NULL;
    }
    if (usage_mon_data_limit->scopes) {
        OpenAPI_list_for_each(usage_mon_data_limit->scopes, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_mon_data_scope_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(usage_mon_data_limit->scopes);
        usage_mon_data_limit->scopes = NULL;
    }
    if (usage_mon_data_limit->um_level) {
        OpenAPI_usage_mon_level_free(usage_mon_data_limit->um_level);
        usage_mon_data_limit->um_level = NULL;
    }
    if (usage_mon_data_limit->start_date) {
        ogs_free(usage_mon_data_limit->start_date);
        usage_mon_data_limit->start_date = NULL;
    }
    if (usage_mon_data_limit->end_date) {
        ogs_free(usage_mon_data_limit->end_date);
        usage_mon_data_limit->end_date = NULL;
    }
    if (usage_mon_data_limit->usage_limit) {
        OpenAPI_usage_threshold_free(usage_mon_data_limit->usage_limit);
        usage_mon_data_limit->usage_limit = NULL;
    }
    if (usage_mon_data_limit->reset_period) {
        OpenAPI_time_period_free(usage_mon_data_limit->reset_period);
        usage_mon_data_limit->reset_period = NULL;
    }
    ogs_free(usage_mon_data_limit);
}

cJSON *OpenAPI_usage_mon_data_limit_convertToJSON(OpenAPI_usage_mon_data_limit_t *usage_mon_data_limit)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (usage_mon_data_limit == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [UsageMonDataLimit]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!usage_mon_data_limit->limit_id) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [limit_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "limitId", usage_mon_data_limit->limit_id) == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [limit_id]");
        goto end;
    }

    if (usage_mon_data_limit->scopes) {
    cJSON *scopes = cJSON_AddObjectToObject(item, "scopes");
    if (scopes == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [scopes]");
        goto end;
    }
    cJSON *localMapObject = scopes;
    if (usage_mon_data_limit->scopes) {
        OpenAPI_list_for_each(usage_mon_data_limit->scopes, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [scopes]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [scopes]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_usage_mon_data_scope_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (usage_mon_data_limit->um_level) {
    cJSON *um_level_local_JSON = OpenAPI_usage_mon_level_convertToJSON(usage_mon_data_limit->um_level);
    if (um_level_local_JSON == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [um_level]");
        goto end;
    }
    cJSON_AddItemToObject(item, "umLevel", um_level_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [um_level]");
        goto end;
    }
    }

    if (usage_mon_data_limit->start_date) {
    if (cJSON_AddStringToObject(item, "startDate", usage_mon_data_limit->start_date) == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [start_date]");
        goto end;
    }
    }

    if (usage_mon_data_limit->end_date) {
    if (cJSON_AddStringToObject(item, "endDate", usage_mon_data_limit->end_date) == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [end_date]");
        goto end;
    }
    }

    if (usage_mon_data_limit->usage_limit) {
    cJSON *usage_limit_local_JSON = OpenAPI_usage_threshold_convertToJSON(usage_mon_data_limit->usage_limit);
    if (usage_limit_local_JSON == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [usage_limit]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usageLimit", usage_limit_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [usage_limit]");
        goto end;
    }
    }

    if (usage_mon_data_limit->reset_period) {
    cJSON *reset_period_local_JSON = OpenAPI_time_period_convertToJSON(usage_mon_data_limit->reset_period);
    if (reset_period_local_JSON == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [reset_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resetPeriod", reset_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed [reset_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_usage_mon_data_limit_t *OpenAPI_usage_mon_data_limit_parseFromJSON(cJSON *usage_mon_data_limitJSON)
{
    OpenAPI_usage_mon_data_limit_t *usage_mon_data_limit_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *limit_id = NULL;
    cJSON *scopes = NULL;
    OpenAPI_list_t *scopesList = NULL;
    cJSON *um_level = NULL;
    OpenAPI_usage_mon_level_t *um_level_local_nonprim = NULL;
    cJSON *start_date = NULL;
    cJSON *end_date = NULL;
    cJSON *usage_limit = NULL;
    OpenAPI_usage_threshold_t *usage_limit_local_nonprim = NULL;
    cJSON *reset_period = NULL;
    OpenAPI_time_period_t *reset_period_local_nonprim = NULL;
    limit_id = cJSON_GetObjectItemCaseSensitive(usage_mon_data_limitJSON, "limitId");
    if (!limit_id) {
        ogs_error("OpenAPI_usage_mon_data_limit_parseFromJSON() failed [limit_id]");
        goto end;
    }
    if (!cJSON_IsString(limit_id)) {
        ogs_error("OpenAPI_usage_mon_data_limit_parseFromJSON() failed [limit_id]");
        goto end;
    }

    scopes = cJSON_GetObjectItemCaseSensitive(usage_mon_data_limitJSON, "scopes");
    if (scopes) {
        cJSON *scopes_local_map = NULL;
        if (!cJSON_IsObject(scopes) && !cJSON_IsNull(scopes)) {
            ogs_error("OpenAPI_usage_mon_data_limit_parseFromJSON() failed [scopes]");
            goto end;
        }
        if (cJSON_IsObject(scopes)) {
            scopesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(scopes_local_map, scopes) {
                cJSON *localMapObject = scopes_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_usage_mon_data_scope_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_usage_mon_data_limit_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(scopesList, localMapKeyPair);
            }
        }
    }

    um_level = cJSON_GetObjectItemCaseSensitive(usage_mon_data_limitJSON, "umLevel");
    if (um_level) {
    um_level_local_nonprim = OpenAPI_usage_mon_level_parseFromJSON(um_level);
    if (!um_level_local_nonprim) {
        ogs_error("OpenAPI_usage_mon_level_parseFromJSON failed [um_level]");
        goto end;
    }
    }

    start_date = cJSON_GetObjectItemCaseSensitive(usage_mon_data_limitJSON, "startDate");
    if (start_date) {
    if (!cJSON_IsString(start_date) && !cJSON_IsNull(start_date)) {
        ogs_error("OpenAPI_usage_mon_data_limit_parseFromJSON() failed [start_date]");
        goto end;
    }
    }

    end_date = cJSON_GetObjectItemCaseSensitive(usage_mon_data_limitJSON, "endDate");
    if (end_date) {
    if (!cJSON_IsString(end_date) && !cJSON_IsNull(end_date)) {
        ogs_error("OpenAPI_usage_mon_data_limit_parseFromJSON() failed [end_date]");
        goto end;
    }
    }

    usage_limit = cJSON_GetObjectItemCaseSensitive(usage_mon_data_limitJSON, "usageLimit");
    if (usage_limit) {
    usage_limit_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(usage_limit);
    if (!usage_limit_local_nonprim) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON failed [usage_limit]");
        goto end;
    }
    }

    reset_period = cJSON_GetObjectItemCaseSensitive(usage_mon_data_limitJSON, "resetPeriod");
    if (reset_period) {
    reset_period_local_nonprim = OpenAPI_time_period_parseFromJSON(reset_period);
    if (!reset_period_local_nonprim) {
        ogs_error("OpenAPI_time_period_parseFromJSON failed [reset_period]");
        goto end;
    }
    }

    usage_mon_data_limit_local_var = OpenAPI_usage_mon_data_limit_create (
        ogs_strdup(limit_id->valuestring),
        scopes ? scopesList : NULL,
        um_level ? um_level_local_nonprim : NULL,
        start_date && !cJSON_IsNull(start_date) ? ogs_strdup(start_date->valuestring) : NULL,
        end_date && !cJSON_IsNull(end_date) ? ogs_strdup(end_date->valuestring) : NULL,
        usage_limit ? usage_limit_local_nonprim : NULL,
        reset_period ? reset_period_local_nonprim : NULL
    );

    return usage_mon_data_limit_local_var;
end:
    if (scopesList) {
        OpenAPI_list_for_each(scopesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_mon_data_scope_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scopesList);
        scopesList = NULL;
    }
    if (um_level_local_nonprim) {
        OpenAPI_usage_mon_level_free(um_level_local_nonprim);
        um_level_local_nonprim = NULL;
    }
    if (usage_limit_local_nonprim) {
        OpenAPI_usage_threshold_free(usage_limit_local_nonprim);
        usage_limit_local_nonprim = NULL;
    }
    if (reset_period_local_nonprim) {
        OpenAPI_time_period_free(reset_period_local_nonprim);
        reset_period_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_usage_mon_data_limit_t *OpenAPI_usage_mon_data_limit_copy(OpenAPI_usage_mon_data_limit_t *dst, OpenAPI_usage_mon_data_limit_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_mon_data_limit_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_mon_data_limit_convertToJSON() failed");
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

    OpenAPI_usage_mon_data_limit_free(dst);
    dst = OpenAPI_usage_mon_data_limit_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

