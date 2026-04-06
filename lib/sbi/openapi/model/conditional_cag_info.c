
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "conditional_cag_info.h"

OpenAPI_conditional_cag_info_t *OpenAPI_conditional_cag_info_create(
    OpenAPI_list_t *allowed_cag_list,
    bool is_cag_only_indicator,
    int cag_only_indicator,
    OpenAPI_valid_time_period_t *valid_time_period,
    OpenAPI_list_t* cag_specific_valid_time_period_list
)
{
    OpenAPI_conditional_cag_info_t *conditional_cag_info_local_var = ogs_malloc(sizeof(OpenAPI_conditional_cag_info_t));
    ogs_assert(conditional_cag_info_local_var);

    conditional_cag_info_local_var->allowed_cag_list = allowed_cag_list;
    conditional_cag_info_local_var->is_cag_only_indicator = is_cag_only_indicator;
    conditional_cag_info_local_var->cag_only_indicator = cag_only_indicator;
    conditional_cag_info_local_var->valid_time_period = valid_time_period;
    conditional_cag_info_local_var->cag_specific_valid_time_period_list = cag_specific_valid_time_period_list;

    return conditional_cag_info_local_var;
}

void OpenAPI_conditional_cag_info_free(OpenAPI_conditional_cag_info_t *conditional_cag_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == conditional_cag_info) {
        return;
    }
    if (conditional_cag_info->allowed_cag_list) {
        OpenAPI_list_for_each(conditional_cag_info->allowed_cag_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(conditional_cag_info->allowed_cag_list);
        conditional_cag_info->allowed_cag_list = NULL;
    }
    if (conditional_cag_info->valid_time_period) {
        OpenAPI_valid_time_period_free(conditional_cag_info->valid_time_period);
        conditional_cag_info->valid_time_period = NULL;
    }
    if (conditional_cag_info->cag_specific_valid_time_period_list) {
        OpenAPI_list_for_each(conditional_cag_info->cag_specific_valid_time_period_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_valid_time_period_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(conditional_cag_info->cag_specific_valid_time_period_list);
        conditional_cag_info->cag_specific_valid_time_period_list = NULL;
    }
    ogs_free(conditional_cag_info);
}

cJSON *OpenAPI_conditional_cag_info_convertToJSON(OpenAPI_conditional_cag_info_t *conditional_cag_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (conditional_cag_info == NULL) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [ConditionalCagInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!conditional_cag_info->allowed_cag_list) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [allowed_cag_list]");
        return NULL;
    }
    cJSON *allowed_cag_listList = cJSON_AddArrayToObject(item, "allowedCagList");
    if (allowed_cag_listList == NULL) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [allowed_cag_list]");
        goto end;
    }
    OpenAPI_list_for_each(conditional_cag_info->allowed_cag_list, node) {
        if (cJSON_AddStringToObject(allowed_cag_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [allowed_cag_list]");
            goto end;
        }
    }

    if (conditional_cag_info->is_cag_only_indicator) {
    if (cJSON_AddBoolToObject(item, "cagOnlyIndicator", conditional_cag_info->cag_only_indicator) == NULL) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [cag_only_indicator]");
        goto end;
    }
    }

    if (conditional_cag_info->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(conditional_cag_info->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

    if (conditional_cag_info->cag_specific_valid_time_period_list) {
    cJSON *cag_specific_valid_time_period_list = cJSON_AddObjectToObject(item, "cagSpecificValidTimePeriodList");
    if (cag_specific_valid_time_period_list == NULL) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [cag_specific_valid_time_period_list]");
        goto end;
    }
    cJSON *localMapObject = cag_specific_valid_time_period_list;
    if (conditional_cag_info->cag_specific_valid_time_period_list) {
        OpenAPI_list_for_each(conditional_cag_info->cag_specific_valid_time_period_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [cag_specific_valid_time_period_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [cag_specific_valid_time_period_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_valid_time_period_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_conditional_cag_info_t *OpenAPI_conditional_cag_info_parseFromJSON(cJSON *conditional_cag_infoJSON)
{
    OpenAPI_conditional_cag_info_t *conditional_cag_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *allowed_cag_list = NULL;
    OpenAPI_list_t *allowed_cag_listList = NULL;
    cJSON *cag_only_indicator = NULL;
    cJSON *valid_time_period = NULL;
    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    cJSON *cag_specific_valid_time_period_list = NULL;
    OpenAPI_list_t *cag_specific_valid_time_period_listList = NULL;
    allowed_cag_list = cJSON_GetObjectItemCaseSensitive(conditional_cag_infoJSON, "allowedCagList");
    if (!allowed_cag_list) {
        ogs_error("OpenAPI_conditional_cag_info_parseFromJSON() failed [allowed_cag_list]");
        goto end;
    }
        cJSON *allowed_cag_list_local = NULL;
        if (!cJSON_IsArray(allowed_cag_list)) {
            ogs_error("OpenAPI_conditional_cag_info_parseFromJSON() failed [allowed_cag_list]");
            goto end;
        }

        allowed_cag_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_cag_list_local, allowed_cag_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(allowed_cag_list_local)) {
                ogs_error("OpenAPI_conditional_cag_info_parseFromJSON() failed [allowed_cag_list]");
                goto end;
            }
            OpenAPI_list_add(allowed_cag_listList, ogs_strdup(allowed_cag_list_local->valuestring));
        }

    cag_only_indicator = cJSON_GetObjectItemCaseSensitive(conditional_cag_infoJSON, "cagOnlyIndicator");
    if (cag_only_indicator) {
    if (!cJSON_IsBool(cag_only_indicator)) {
        ogs_error("OpenAPI_conditional_cag_info_parseFromJSON() failed [cag_only_indicator]");
        goto end;
    }
    }

    valid_time_period = cJSON_GetObjectItemCaseSensitive(conditional_cag_infoJSON, "validTimePeriod");
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    if (!valid_time_period_local_nonprim) {
        ogs_error("OpenAPI_valid_time_period_parseFromJSON failed [valid_time_period]");
        goto end;
    }
    }

    cag_specific_valid_time_period_list = cJSON_GetObjectItemCaseSensitive(conditional_cag_infoJSON, "cagSpecificValidTimePeriodList");
    if (cag_specific_valid_time_period_list) {
        cJSON *cag_specific_valid_time_period_list_local_map = NULL;
        if (!cJSON_IsObject(cag_specific_valid_time_period_list) && !cJSON_IsNull(cag_specific_valid_time_period_list)) {
            ogs_error("OpenAPI_conditional_cag_info_parseFromJSON() failed [cag_specific_valid_time_period_list]");
            goto end;
        }
        if (cJSON_IsObject(cag_specific_valid_time_period_list)) {
            cag_specific_valid_time_period_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(cag_specific_valid_time_period_list_local_map, cag_specific_valid_time_period_list) {
                cJSON *localMapObject = cag_specific_valid_time_period_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_valid_time_period_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_conditional_cag_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(cag_specific_valid_time_period_listList, localMapKeyPair);
            }
        }
    }

    conditional_cag_info_local_var = OpenAPI_conditional_cag_info_create (
        allowed_cag_listList,
        cag_only_indicator ? true : false,
        cag_only_indicator ? cag_only_indicator->valueint : 0,
        valid_time_period ? valid_time_period_local_nonprim : NULL,
        cag_specific_valid_time_period_list ? cag_specific_valid_time_period_listList : NULL
    );

    return conditional_cag_info_local_var;
end:
    if (allowed_cag_listList) {
        OpenAPI_list_for_each(allowed_cag_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(allowed_cag_listList);
        allowed_cag_listList = NULL;
    }
    if (valid_time_period_local_nonprim) {
        OpenAPI_valid_time_period_free(valid_time_period_local_nonprim);
        valid_time_period_local_nonprim = NULL;
    }
    if (cag_specific_valid_time_period_listList) {
        OpenAPI_list_for_each(cag_specific_valid_time_period_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_valid_time_period_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(cag_specific_valid_time_period_listList);
        cag_specific_valid_time_period_listList = NULL;
    }
    return NULL;
}

OpenAPI_conditional_cag_info_t *OpenAPI_conditional_cag_info_copy(OpenAPI_conditional_cag_info_t *dst, OpenAPI_conditional_cag_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_conditional_cag_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_conditional_cag_info_convertToJSON() failed");
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

    OpenAPI_conditional_cag_info_free(dst);
    dst = OpenAPI_conditional_cag_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

