
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "default_unrelated_class.h"

OpenAPI_default_unrelated_class_t *OpenAPI_default_unrelated_class_create(
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_privacy_check_related_action_e privacy_check_related_action,
    OpenAPI_code_word_ind_e code_word_ind,
    OpenAPI_valid_time_period_t *valid_time_period,
    OpenAPI_list_t *code_word_list
)
{
    OpenAPI_default_unrelated_class_t *default_unrelated_class_local_var = ogs_malloc(sizeof(OpenAPI_default_unrelated_class_t));
    ogs_assert(default_unrelated_class_local_var);

    default_unrelated_class_local_var->allowed_geographic_area = allowed_geographic_area;
    default_unrelated_class_local_var->privacy_check_related_action = privacy_check_related_action;
    default_unrelated_class_local_var->code_word_ind = code_word_ind;
    default_unrelated_class_local_var->valid_time_period = valid_time_period;
    default_unrelated_class_local_var->code_word_list = code_word_list;

    return default_unrelated_class_local_var;
}

void OpenAPI_default_unrelated_class_free(OpenAPI_default_unrelated_class_t *default_unrelated_class)
{
    if (NULL == default_unrelated_class) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(default_unrelated_class->allowed_geographic_area, node) {
        OpenAPI_geographic_area_free(node->data);
    }
    OpenAPI_list_free(default_unrelated_class->allowed_geographic_area);
    OpenAPI_valid_time_period_free(default_unrelated_class->valid_time_period);
    OpenAPI_list_for_each(default_unrelated_class->code_word_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(default_unrelated_class->code_word_list);
    ogs_free(default_unrelated_class);
}

cJSON *OpenAPI_default_unrelated_class_convertToJSON(OpenAPI_default_unrelated_class_t *default_unrelated_class)
{
    cJSON *item = NULL;

    if (default_unrelated_class == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [DefaultUnrelatedClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (default_unrelated_class->allowed_geographic_area) {
    cJSON *allowed_geographic_areaList = cJSON_AddArrayToObject(item, "allowedGeographicArea");
    if (allowed_geographic_areaList == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [allowed_geographic_area]");
        goto end;
    }

    OpenAPI_lnode_t *allowed_geographic_area_node;
    if (default_unrelated_class->allowed_geographic_area) {
        OpenAPI_list_for_each(default_unrelated_class->allowed_geographic_area, allowed_geographic_area_node) {
            cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(allowed_geographic_area_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [allowed_geographic_area]");
                goto end;
            }
            cJSON_AddItemToArray(allowed_geographic_areaList, itemLocal);
        }
    }
    }

    if (default_unrelated_class->privacy_check_related_action) {
    if (cJSON_AddStringToObject(item, "privacyCheckRelatedAction", OpenAPI_privacy_check_related_action_ToString(default_unrelated_class->privacy_check_related_action)) == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [privacy_check_related_action]");
        goto end;
    }
    }

    if (default_unrelated_class->code_word_ind) {
    if (cJSON_AddStringToObject(item, "codeWordInd", OpenAPI_code_word_ind_ToString(default_unrelated_class->code_word_ind)) == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [code_word_ind]");
        goto end;
    }
    }

    if (default_unrelated_class->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(default_unrelated_class->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

    if (default_unrelated_class->code_word_list) {
    cJSON *code_word_list = cJSON_AddArrayToObject(item, "codeWordList");
    if (code_word_list == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [code_word_list]");
        goto end;
    }

    OpenAPI_lnode_t *code_word_list_node;
    OpenAPI_list_for_each(default_unrelated_class->code_word_list, code_word_list_node)  {
    if (cJSON_AddStringToObject(code_word_list, "", (char*)code_word_list_node->data) == NULL) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed [code_word_list]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_default_unrelated_class_t *OpenAPI_default_unrelated_class_parseFromJSON(cJSON *default_unrelated_classJSON)
{
    OpenAPI_default_unrelated_class_t *default_unrelated_class_local_var = NULL;
    cJSON *allowed_geographic_area = cJSON_GetObjectItemCaseSensitive(default_unrelated_classJSON, "allowedGeographicArea");

    OpenAPI_list_t *allowed_geographic_areaList;
    if (allowed_geographic_area) {
    cJSON *allowed_geographic_area_local_nonprimitive;
    if (!cJSON_IsArray(allowed_geographic_area)){
        ogs_error("OpenAPI_default_unrelated_class_parseFromJSON() failed [allowed_geographic_area]");
        goto end;
    }

    allowed_geographic_areaList = OpenAPI_list_create();

    cJSON_ArrayForEach(allowed_geographic_area_local_nonprimitive, allowed_geographic_area ) {
        if (!cJSON_IsObject(allowed_geographic_area_local_nonprimitive)) {
            ogs_error("OpenAPI_default_unrelated_class_parseFromJSON() failed [allowed_geographic_area]");
            goto end;
        }
        OpenAPI_geographic_area_t *allowed_geographic_areaItem = OpenAPI_geographic_area_parseFromJSON(allowed_geographic_area_local_nonprimitive);

        if (!allowed_geographic_areaItem) {
            ogs_error("No allowed_geographic_areaItem");
            OpenAPI_list_free(allowed_geographic_areaList);
            goto end;
        }

        OpenAPI_list_add(allowed_geographic_areaList, allowed_geographic_areaItem);
    }
    }

    cJSON *privacy_check_related_action = cJSON_GetObjectItemCaseSensitive(default_unrelated_classJSON, "privacyCheckRelatedAction");

    OpenAPI_privacy_check_related_action_e privacy_check_related_actionVariable;
    if (privacy_check_related_action) {
    if (!cJSON_IsString(privacy_check_related_action)) {
        ogs_error("OpenAPI_default_unrelated_class_parseFromJSON() failed [privacy_check_related_action]");
        goto end;
    }
    privacy_check_related_actionVariable = OpenAPI_privacy_check_related_action_FromString(privacy_check_related_action->valuestring);
    }

    cJSON *code_word_ind = cJSON_GetObjectItemCaseSensitive(default_unrelated_classJSON, "codeWordInd");

    OpenAPI_code_word_ind_e code_word_indVariable;
    if (code_word_ind) {
    if (!cJSON_IsString(code_word_ind)) {
        ogs_error("OpenAPI_default_unrelated_class_parseFromJSON() failed [code_word_ind]");
        goto end;
    }
    code_word_indVariable = OpenAPI_code_word_ind_FromString(code_word_ind->valuestring);
    }

    cJSON *valid_time_period = cJSON_GetObjectItemCaseSensitive(default_unrelated_classJSON, "validTimePeriod");

    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    }

    cJSON *code_word_list = cJSON_GetObjectItemCaseSensitive(default_unrelated_classJSON, "codeWordList");

    OpenAPI_list_t *code_word_listList;
    if (code_word_list) {
    cJSON *code_word_list_local;
    if (!cJSON_IsArray(code_word_list)) {
        ogs_error("OpenAPI_default_unrelated_class_parseFromJSON() failed [code_word_list]");
        goto end;
    }
    code_word_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(code_word_list_local, code_word_list) {
    if (!cJSON_IsString(code_word_list_local)) {
        ogs_error("OpenAPI_default_unrelated_class_parseFromJSON() failed [code_word_list]");
        goto end;
    }
    OpenAPI_list_add(code_word_listList , ogs_strdup(code_word_list_local->valuestring));
    }
    }

    default_unrelated_class_local_var = OpenAPI_default_unrelated_class_create (
        allowed_geographic_area ? allowed_geographic_areaList : NULL,
        privacy_check_related_action ? privacy_check_related_actionVariable : 0,
        code_word_ind ? code_word_indVariable : 0,
        valid_time_period ? valid_time_period_local_nonprim : NULL,
        code_word_list ? code_word_listList : NULL
    );

    return default_unrelated_class_local_var;
end:
    return NULL;
}

OpenAPI_default_unrelated_class_t *OpenAPI_default_unrelated_class_copy(OpenAPI_default_unrelated_class_t *dst, OpenAPI_default_unrelated_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_default_unrelated_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_default_unrelated_class_convertToJSON() failed");
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

    OpenAPI_default_unrelated_class_free(dst);
    dst = OpenAPI_default_unrelated_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

