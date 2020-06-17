
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_data.h"

OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_create(
    char *limit_id,
    OpenAPI_list_t* scopes,
    OpenAPI_usage_mon_level_t *um_level,
    OpenAPI_usage_threshold_t *allowed_usage,
    OpenAPI_time_period_t *reset_time
    )
{
    OpenAPI_usage_mon_data_t *usage_mon_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_usage_mon_data_t));
    if (!usage_mon_data_local_var) {
        return NULL;
    }
    usage_mon_data_local_var->limit_id = limit_id;
    usage_mon_data_local_var->scopes = scopes;
    usage_mon_data_local_var->um_level = um_level;
    usage_mon_data_local_var->allowed_usage = allowed_usage;
    usage_mon_data_local_var->reset_time = reset_time;

    return usage_mon_data_local_var;
}

void OpenAPI_usage_mon_data_free(OpenAPI_usage_mon_data_t *usage_mon_data)
{
    if (NULL == usage_mon_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(usage_mon_data->limit_id);
    OpenAPI_list_for_each(usage_mon_data->scopes, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_usage_mon_data_scope_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(usage_mon_data->scopes);
    OpenAPI_usage_mon_level_free(usage_mon_data->um_level);
    OpenAPI_usage_threshold_free(usage_mon_data->allowed_usage);
    OpenAPI_time_period_free(usage_mon_data->reset_time);
    ogs_free(usage_mon_data);
}

cJSON *OpenAPI_usage_mon_data_convertToJSON(OpenAPI_usage_mon_data_t *usage_mon_data)
{
    cJSON *item = NULL;

    if (usage_mon_data == NULL) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [UsageMonData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!usage_mon_data->limit_id) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [limit_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "limitId", usage_mon_data->limit_id) == NULL) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [limit_id]");
        goto end;
    }

    if (usage_mon_data->scopes) {
        cJSON *scopes = cJSON_AddObjectToObject(item, "scopes");
        if (scopes == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [scopes]");
            goto end;
        }
        cJSON *localMapObject = scopes;
        OpenAPI_lnode_t *scopes_node;
        if (usage_mon_data->scopes) {
            OpenAPI_list_for_each(usage_mon_data->scopes, scopes_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)scopes_node->data;
                cJSON *itemLocal = OpenAPI_usage_mon_data_scope_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [scopes]");
                    goto end;
                }
                cJSON_AddItemToObject(scopes, localKeyValue->key, itemLocal);
            }
        }
    }

    if (usage_mon_data->um_level) {
        cJSON *um_level_local_JSON = OpenAPI_usage_mon_level_convertToJSON(usage_mon_data->um_level);
        if (um_level_local_JSON == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [um_level]");
            goto end;
        }
        cJSON_AddItemToObject(item, "umLevel", um_level_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [um_level]");
            goto end;
        }
    }

    if (usage_mon_data->allowed_usage) {
        cJSON *allowed_usage_local_JSON = OpenAPI_usage_threshold_convertToJSON(usage_mon_data->allowed_usage);
        if (allowed_usage_local_JSON == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [allowed_usage]");
            goto end;
        }
        cJSON_AddItemToObject(item, "allowedUsage", allowed_usage_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [allowed_usage]");
            goto end;
        }
    }

    if (usage_mon_data->reset_time) {
        cJSON *reset_time_local_JSON = OpenAPI_time_period_convertToJSON(usage_mon_data->reset_time);
        if (reset_time_local_JSON == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [reset_time]");
            goto end;
        }
        cJSON_AddItemToObject(item, "resetTime", reset_time_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [reset_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_parseFromJSON(cJSON *usage_mon_dataJSON)
{
    OpenAPI_usage_mon_data_t *usage_mon_data_local_var = NULL;
    cJSON *limit_id = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "limitId");
    if (!limit_id) {
        ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [limit_id]");
        goto end;
    }


    if (!cJSON_IsString(limit_id)) {
        ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [limit_id]");
        goto end;
    }

    cJSON *scopes = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "scopes");

    OpenAPI_list_t *scopesList;
    if (scopes) {
        cJSON *scopes_local_map;
        if (!cJSON_IsObject(scopes)) {
            ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [scopes]");
            goto end;
        }
        scopesList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(scopes_local_map, scopes) {
            cJSON *localMapObject = scopes_local_map;
            if (!cJSON_IsObject(scopes_local_map)) {
                ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [scopes]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_usage_mon_data_scope_parseFromJSON(localMapObject));
            OpenAPI_list_add(scopesList, localMapKeyPair);
        }
    }

    cJSON *um_level = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "umLevel");

    OpenAPI_usage_mon_level_t *um_level_local_nonprim = NULL;
    if (um_level) {
        um_level_local_nonprim = OpenAPI_usage_mon_level_parseFromJSON(um_level);
    }

    cJSON *allowed_usage = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "allowedUsage");

    OpenAPI_usage_threshold_t *allowed_usage_local_nonprim = NULL;
    if (allowed_usage) {
        allowed_usage_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(allowed_usage);
    }

    cJSON *reset_time = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "resetTime");

    OpenAPI_time_period_t *reset_time_local_nonprim = NULL;
    if (reset_time) {
        reset_time_local_nonprim = OpenAPI_time_period_parseFromJSON(reset_time);
    }

    usage_mon_data_local_var = OpenAPI_usage_mon_data_create (
        ogs_strdup(limit_id->valuestring),
        scopes ? scopesList : NULL,
        um_level ? um_level_local_nonprim : NULL,
        allowed_usage ? allowed_usage_local_nonprim : NULL,
        reset_time ? reset_time_local_nonprim : NULL
        );

    return usage_mon_data_local_var;
end:
    return NULL;
}

OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_copy(OpenAPI_usage_mon_data_t *dst, OpenAPI_usage_mon_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_mon_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed");
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

    OpenAPI_usage_mon_data_free(dst);
    dst = OpenAPI_usage_mon_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

