
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_data.h"

OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_create(
    char *limit_id,
    OpenAPI_list_t* scopes,
    OpenAPI_usage_mon_level_t *um_level,
    OpenAPI_usage_threshold_t *allowed_usage,
    char *reset_time,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_usage_mon_data_t *usage_mon_data_local_var = ogs_malloc(sizeof(OpenAPI_usage_mon_data_t));
    ogs_assert(usage_mon_data_local_var);

    usage_mon_data_local_var->limit_id = limit_id;
    usage_mon_data_local_var->scopes = scopes;
    usage_mon_data_local_var->um_level = um_level;
    usage_mon_data_local_var->allowed_usage = allowed_usage;
    usage_mon_data_local_var->reset_time = reset_time;
    usage_mon_data_local_var->supp_feat = supp_feat;
    usage_mon_data_local_var->reset_ids = reset_ids;

    return usage_mon_data_local_var;
}

void OpenAPI_usage_mon_data_free(OpenAPI_usage_mon_data_t *usage_mon_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == usage_mon_data) {
        return;
    }
    if (usage_mon_data->limit_id) {
        ogs_free(usage_mon_data->limit_id);
        usage_mon_data->limit_id = NULL;
    }
    if (usage_mon_data->scopes) {
        OpenAPI_list_for_each(usage_mon_data->scopes, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_mon_data_scope_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(usage_mon_data->scopes);
        usage_mon_data->scopes = NULL;
    }
    if (usage_mon_data->um_level) {
        OpenAPI_usage_mon_level_free(usage_mon_data->um_level);
        usage_mon_data->um_level = NULL;
    }
    if (usage_mon_data->allowed_usage) {
        OpenAPI_usage_threshold_free(usage_mon_data->allowed_usage);
        usage_mon_data->allowed_usage = NULL;
    }
    if (usage_mon_data->reset_time) {
        ogs_free(usage_mon_data->reset_time);
        usage_mon_data->reset_time = NULL;
    }
    if (usage_mon_data->supp_feat) {
        ogs_free(usage_mon_data->supp_feat);
        usage_mon_data->supp_feat = NULL;
    }
    if (usage_mon_data->reset_ids) {
        OpenAPI_list_for_each(usage_mon_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(usage_mon_data->reset_ids);
        usage_mon_data->reset_ids = NULL;
    }
    ogs_free(usage_mon_data);
}

cJSON *OpenAPI_usage_mon_data_convertToJSON(OpenAPI_usage_mon_data_t *usage_mon_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (usage_mon_data == NULL) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [UsageMonData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!usage_mon_data->limit_id) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [limit_id]");
        return NULL;
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
    if (usage_mon_data->scopes) {
        OpenAPI_list_for_each(usage_mon_data->scopes, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [scopes]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [scopes]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_usage_mon_data_scope_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
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
    if (cJSON_AddStringToObject(item, "resetTime", usage_mon_data->reset_time) == NULL) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [reset_time]");
        goto end;
    }
    }

    if (usage_mon_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", usage_mon_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (usage_mon_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(usage_mon_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_usage_mon_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_parseFromJSON(cJSON *usage_mon_dataJSON)
{
    OpenAPI_usage_mon_data_t *usage_mon_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *limit_id = NULL;
    cJSON *scopes = NULL;
    OpenAPI_list_t *scopesList = NULL;
    cJSON *um_level = NULL;
    OpenAPI_usage_mon_level_t *um_level_local_nonprim = NULL;
    cJSON *allowed_usage = NULL;
    OpenAPI_usage_threshold_t *allowed_usage_local_nonprim = NULL;
    cJSON *reset_time = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    limit_id = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "limitId");
    if (!limit_id) {
        ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [limit_id]");
        goto end;
    }
    if (!cJSON_IsString(limit_id)) {
        ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [limit_id]");
        goto end;
    }

    scopes = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "scopes");
    if (scopes) {
        cJSON *scopes_local_map = NULL;
        if (!cJSON_IsObject(scopes) && !cJSON_IsNull(scopes)) {
            ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [scopes]");
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
                    ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(scopesList, localMapKeyPair);
            }
        }
    }

    um_level = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "umLevel");
    if (um_level) {
    um_level_local_nonprim = OpenAPI_usage_mon_level_parseFromJSON(um_level);
    if (!um_level_local_nonprim) {
        ogs_error("OpenAPI_usage_mon_level_parseFromJSON failed [um_level]");
        goto end;
    }
    }

    allowed_usage = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "allowedUsage");
    if (allowed_usage) {
    allowed_usage_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(allowed_usage);
    if (!allowed_usage_local_nonprim) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON failed [allowed_usage]");
        goto end;
    }
    }

    reset_time = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "resetTime");
    if (reset_time) {
    if (!cJSON_IsString(reset_time) && !cJSON_IsNull(reset_time)) {
        ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [reset_time]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(usage_mon_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_usage_mon_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    usage_mon_data_local_var = OpenAPI_usage_mon_data_create (
        ogs_strdup(limit_id->valuestring),
        scopes ? scopesList : NULL,
        um_level ? um_level_local_nonprim : NULL,
        allowed_usage ? allowed_usage_local_nonprim : NULL,
        reset_time && !cJSON_IsNull(reset_time) ? ogs_strdup(reset_time->valuestring) : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return usage_mon_data_local_var;
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
    if (allowed_usage_local_nonprim) {
        OpenAPI_usage_threshold_free(allowed_usage_local_nonprim);
        allowed_usage_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

