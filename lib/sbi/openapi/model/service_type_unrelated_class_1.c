
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_type_unrelated_class_1.h"

OpenAPI_service_type_unrelated_class_1_t *OpenAPI_service_type_unrelated_class_1_create(
    int service_type,
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_privacy_check_related_action_e privacy_check_related_action,
    OpenAPI_code_word_ind_e code_word_ind,
    OpenAPI_valid_time_period_1_t *valid_time_period,
    OpenAPI_list_t *code_word_list
)
{
    OpenAPI_service_type_unrelated_class_1_t *service_type_unrelated_class_1_local_var = ogs_malloc(sizeof(OpenAPI_service_type_unrelated_class_1_t));
    ogs_assert(service_type_unrelated_class_1_local_var);

    service_type_unrelated_class_1_local_var->service_type = service_type;
    service_type_unrelated_class_1_local_var->allowed_geographic_area = allowed_geographic_area;
    service_type_unrelated_class_1_local_var->privacy_check_related_action = privacy_check_related_action;
    service_type_unrelated_class_1_local_var->code_word_ind = code_word_ind;
    service_type_unrelated_class_1_local_var->valid_time_period = valid_time_period;
    service_type_unrelated_class_1_local_var->code_word_list = code_word_list;

    return service_type_unrelated_class_1_local_var;
}

void OpenAPI_service_type_unrelated_class_1_free(OpenAPI_service_type_unrelated_class_1_t *service_type_unrelated_class_1)
{
    if (NULL == service_type_unrelated_class_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_type_unrelated_class_1->allowed_geographic_area, node) {
        OpenAPI_geographic_area_free(node->data);
    }
    OpenAPI_list_free(service_type_unrelated_class_1->allowed_geographic_area);
    OpenAPI_valid_time_period_1_free(service_type_unrelated_class_1->valid_time_period);
    OpenAPI_list_for_each(service_type_unrelated_class_1->code_word_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_type_unrelated_class_1->code_word_list);
    ogs_free(service_type_unrelated_class_1);
}

cJSON *OpenAPI_service_type_unrelated_class_1_convertToJSON(OpenAPI_service_type_unrelated_class_1_t *service_type_unrelated_class_1)
{
    cJSON *item = NULL;

    if (service_type_unrelated_class_1 == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [ServiceTypeUnrelatedClass_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "serviceType", service_type_unrelated_class_1->service_type) == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [service_type]");
        goto end;
    }

    if (service_type_unrelated_class_1->allowed_geographic_area) {
    cJSON *allowed_geographic_areaList = cJSON_AddArrayToObject(item, "allowedGeographicArea");
    if (allowed_geographic_areaList == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [allowed_geographic_area]");
        goto end;
    }

    OpenAPI_lnode_t *allowed_geographic_area_node;
    if (service_type_unrelated_class_1->allowed_geographic_area) {
        OpenAPI_list_for_each(service_type_unrelated_class_1->allowed_geographic_area, allowed_geographic_area_node) {
            cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(allowed_geographic_area_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [allowed_geographic_area]");
                goto end;
            }
            cJSON_AddItemToArray(allowed_geographic_areaList, itemLocal);
        }
    }
    }

    if (service_type_unrelated_class_1->privacy_check_related_action) {
    if (cJSON_AddStringToObject(item, "privacyCheckRelatedAction", OpenAPI_privacy_check_related_action_ToString(service_type_unrelated_class_1->privacy_check_related_action)) == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [privacy_check_related_action]");
        goto end;
    }
    }

    if (service_type_unrelated_class_1->code_word_ind) {
    if (cJSON_AddStringToObject(item, "codeWordInd", OpenAPI_code_word_ind_ToString(service_type_unrelated_class_1->code_word_ind)) == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [code_word_ind]");
        goto end;
    }
    }

    if (service_type_unrelated_class_1->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_1_convertToJSON(service_type_unrelated_class_1->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

    if (service_type_unrelated_class_1->code_word_list) {
    cJSON *code_word_list = cJSON_AddArrayToObject(item, "codeWordList");
    if (code_word_list == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [code_word_list]");
        goto end;
    }

    OpenAPI_lnode_t *code_word_list_node;
    OpenAPI_list_for_each(service_type_unrelated_class_1->code_word_list, code_word_list_node)  {
    if (cJSON_AddStringToObject(code_word_list, "", (char*)code_word_list_node->data) == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed [code_word_list]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_service_type_unrelated_class_1_t *OpenAPI_service_type_unrelated_class_1_parseFromJSON(cJSON *service_type_unrelated_class_1JSON)
{
    OpenAPI_service_type_unrelated_class_1_t *service_type_unrelated_class_1_local_var = NULL;
    cJSON *service_type = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_class_1JSON, "serviceType");
    if (!service_type) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [service_type]");
        goto end;
    }

    if (!cJSON_IsNumber(service_type)) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [service_type]");
        goto end;
    }

    cJSON *allowed_geographic_area = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_class_1JSON, "allowedGeographicArea");

    OpenAPI_list_t *allowed_geographic_areaList;
    if (allowed_geographic_area) {
    cJSON *allowed_geographic_area_local_nonprimitive;
    if (!cJSON_IsArray(allowed_geographic_area)){
        ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [allowed_geographic_area]");
        goto end;
    }

    allowed_geographic_areaList = OpenAPI_list_create();

    cJSON_ArrayForEach(allowed_geographic_area_local_nonprimitive, allowed_geographic_area ) {
        if (!cJSON_IsObject(allowed_geographic_area_local_nonprimitive)) {
            ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [allowed_geographic_area]");
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

    cJSON *privacy_check_related_action = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_class_1JSON, "privacyCheckRelatedAction");

    OpenAPI_privacy_check_related_action_e privacy_check_related_actionVariable;
    if (privacy_check_related_action) {
    if (!cJSON_IsString(privacy_check_related_action)) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [privacy_check_related_action]");
        goto end;
    }
    privacy_check_related_actionVariable = OpenAPI_privacy_check_related_action_FromString(privacy_check_related_action->valuestring);
    }

    cJSON *code_word_ind = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_class_1JSON, "codeWordInd");

    OpenAPI_code_word_ind_e code_word_indVariable;
    if (code_word_ind) {
    if (!cJSON_IsString(code_word_ind)) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [code_word_ind]");
        goto end;
    }
    code_word_indVariable = OpenAPI_code_word_ind_FromString(code_word_ind->valuestring);
    }

    cJSON *valid_time_period = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_class_1JSON, "validTimePeriod");

    OpenAPI_valid_time_period_1_t *valid_time_period_local_nonprim = NULL;
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_1_parseFromJSON(valid_time_period);
    }

    cJSON *code_word_list = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_class_1JSON, "codeWordList");

    OpenAPI_list_t *code_word_listList;
    if (code_word_list) {
    cJSON *code_word_list_local;
    if (!cJSON_IsArray(code_word_list)) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [code_word_list]");
        goto end;
    }
    code_word_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(code_word_list_local, code_word_list) {
    if (!cJSON_IsString(code_word_list_local)) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_parseFromJSON() failed [code_word_list]");
        goto end;
    }
    OpenAPI_list_add(code_word_listList , ogs_strdup(code_word_list_local->valuestring));
    }
    }

    service_type_unrelated_class_1_local_var = OpenAPI_service_type_unrelated_class_1_create (
        
        service_type->valuedouble,
        allowed_geographic_area ? allowed_geographic_areaList : NULL,
        privacy_check_related_action ? privacy_check_related_actionVariable : 0,
        code_word_ind ? code_word_indVariable : 0,
        valid_time_period ? valid_time_period_local_nonprim : NULL,
        code_word_list ? code_word_listList : NULL
    );

    return service_type_unrelated_class_1_local_var;
end:
    return NULL;
}

OpenAPI_service_type_unrelated_class_1_t *OpenAPI_service_type_unrelated_class_1_copy(OpenAPI_service_type_unrelated_class_1_t *dst, OpenAPI_service_type_unrelated_class_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_type_unrelated_class_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_type_unrelated_class_1_convertToJSON() failed");
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

    OpenAPI_service_type_unrelated_class_1_free(dst);
    dst = OpenAPI_service_type_unrelated_class_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

