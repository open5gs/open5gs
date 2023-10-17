
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_type_unrelated_class.h"

OpenAPI_service_type_unrelated_class_t *OpenAPI_service_type_unrelated_class_create(
    int service_type,
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_privacy_check_related_action_e privacy_check_related_action,
    OpenAPI_code_word_ind_e code_word_ind,
    OpenAPI_valid_time_period_t *valid_time_period,
    OpenAPI_list_t *code_word_list
)
{
    OpenAPI_service_type_unrelated_class_t *service_type_unrelated_class_local_var = ogs_malloc(sizeof(OpenAPI_service_type_unrelated_class_t));
    ogs_assert(service_type_unrelated_class_local_var);

    service_type_unrelated_class_local_var->service_type = service_type;
    service_type_unrelated_class_local_var->allowed_geographic_area = allowed_geographic_area;
    service_type_unrelated_class_local_var->privacy_check_related_action = privacy_check_related_action;
    service_type_unrelated_class_local_var->code_word_ind = code_word_ind;
    service_type_unrelated_class_local_var->valid_time_period = valid_time_period;
    service_type_unrelated_class_local_var->code_word_list = code_word_list;

    return service_type_unrelated_class_local_var;
}

void OpenAPI_service_type_unrelated_class_free(OpenAPI_service_type_unrelated_class_t *service_type_unrelated_class)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_type_unrelated_class) {
        return;
    }
    if (service_type_unrelated_class->allowed_geographic_area) {
        OpenAPI_list_for_each(service_type_unrelated_class->allowed_geographic_area, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(service_type_unrelated_class->allowed_geographic_area);
        service_type_unrelated_class->allowed_geographic_area = NULL;
    }
    if (service_type_unrelated_class->valid_time_period) {
        OpenAPI_valid_time_period_free(service_type_unrelated_class->valid_time_period);
        service_type_unrelated_class->valid_time_period = NULL;
    }
    if (service_type_unrelated_class->code_word_list) {
        OpenAPI_list_for_each(service_type_unrelated_class->code_word_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_type_unrelated_class->code_word_list);
        service_type_unrelated_class->code_word_list = NULL;
    }
    ogs_free(service_type_unrelated_class);
}

cJSON *OpenAPI_service_type_unrelated_class_convertToJSON(OpenAPI_service_type_unrelated_class_t *service_type_unrelated_class)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_type_unrelated_class == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [ServiceTypeUnrelatedClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "serviceType", service_type_unrelated_class->service_type) == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [service_type]");
        goto end;
    }

    if (service_type_unrelated_class->allowed_geographic_area) {
    cJSON *allowed_geographic_areaList = cJSON_AddArrayToObject(item, "allowedGeographicArea");
    if (allowed_geographic_areaList == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [allowed_geographic_area]");
        goto end;
    }
    OpenAPI_list_for_each(service_type_unrelated_class->allowed_geographic_area, node) {
        cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [allowed_geographic_area]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_geographic_areaList, itemLocal);
    }
    }

    if (service_type_unrelated_class->privacy_check_related_action != OpenAPI_privacy_check_related_action_NULL) {
    if (cJSON_AddStringToObject(item, "privacyCheckRelatedAction", OpenAPI_privacy_check_related_action_ToString(service_type_unrelated_class->privacy_check_related_action)) == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [privacy_check_related_action]");
        goto end;
    }
    }

    if (service_type_unrelated_class->code_word_ind != OpenAPI_code_word_ind_NULL) {
    if (cJSON_AddStringToObject(item, "codeWordInd", OpenAPI_code_word_ind_ToString(service_type_unrelated_class->code_word_ind)) == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [code_word_ind]");
        goto end;
    }
    }

    if (service_type_unrelated_class->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(service_type_unrelated_class->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

    if (service_type_unrelated_class->code_word_list) {
    cJSON *code_word_listList = cJSON_AddArrayToObject(item, "codeWordList");
    if (code_word_listList == NULL) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [code_word_list]");
        goto end;
    }
    OpenAPI_list_for_each(service_type_unrelated_class->code_word_list, node) {
        if (cJSON_AddStringToObject(code_word_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed [code_word_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_service_type_unrelated_class_t *OpenAPI_service_type_unrelated_class_parseFromJSON(cJSON *service_type_unrelated_classJSON)
{
    OpenAPI_service_type_unrelated_class_t *service_type_unrelated_class_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *service_type = NULL;
    cJSON *allowed_geographic_area = NULL;
    OpenAPI_list_t *allowed_geographic_areaList = NULL;
    cJSON *privacy_check_related_action = NULL;
    OpenAPI_privacy_check_related_action_e privacy_check_related_actionVariable = 0;
    cJSON *code_word_ind = NULL;
    OpenAPI_code_word_ind_e code_word_indVariable = 0;
    cJSON *valid_time_period = NULL;
    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    cJSON *code_word_list = NULL;
    OpenAPI_list_t *code_word_listList = NULL;
    service_type = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_classJSON, "serviceType");
    if (!service_type) {
        ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [service_type]");
        goto end;
    }
    if (!cJSON_IsNumber(service_type)) {
        ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [service_type]");
        goto end;
    }

    allowed_geographic_area = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_classJSON, "allowedGeographicArea");
    if (allowed_geographic_area) {
        cJSON *allowed_geographic_area_local = NULL;
        if (!cJSON_IsArray(allowed_geographic_area)) {
            ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [allowed_geographic_area]");
            goto end;
        }

        allowed_geographic_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_geographic_area_local, allowed_geographic_area) {
            if (!cJSON_IsObject(allowed_geographic_area_local)) {
                ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [allowed_geographic_area]");
                goto end;
            }
            OpenAPI_geographic_area_t *allowed_geographic_areaItem = OpenAPI_geographic_area_parseFromJSON(allowed_geographic_area_local);
            if (!allowed_geographic_areaItem) {
                ogs_error("No allowed_geographic_areaItem");
                goto end;
            }
            OpenAPI_list_add(allowed_geographic_areaList, allowed_geographic_areaItem);
        }
    }

    privacy_check_related_action = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_classJSON, "privacyCheckRelatedAction");
    if (privacy_check_related_action) {
    if (!cJSON_IsString(privacy_check_related_action)) {
        ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [privacy_check_related_action]");
        goto end;
    }
    privacy_check_related_actionVariable = OpenAPI_privacy_check_related_action_FromString(privacy_check_related_action->valuestring);
    }

    code_word_ind = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_classJSON, "codeWordInd");
    if (code_word_ind) {
    if (!cJSON_IsString(code_word_ind)) {
        ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [code_word_ind]");
        goto end;
    }
    code_word_indVariable = OpenAPI_code_word_ind_FromString(code_word_ind->valuestring);
    }

    valid_time_period = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_classJSON, "validTimePeriod");
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    if (!valid_time_period_local_nonprim) {
        ogs_error("OpenAPI_valid_time_period_parseFromJSON failed [valid_time_period]");
        goto end;
    }
    }

    code_word_list = cJSON_GetObjectItemCaseSensitive(service_type_unrelated_classJSON, "codeWordList");
    if (code_word_list) {
        cJSON *code_word_list_local = NULL;
        if (!cJSON_IsArray(code_word_list)) {
            ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [code_word_list]");
            goto end;
        }

        code_word_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(code_word_list_local, code_word_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(code_word_list_local)) {
                ogs_error("OpenAPI_service_type_unrelated_class_parseFromJSON() failed [code_word_list]");
                goto end;
            }
            OpenAPI_list_add(code_word_listList, ogs_strdup(code_word_list_local->valuestring));
        }
    }

    service_type_unrelated_class_local_var = OpenAPI_service_type_unrelated_class_create (
        
        service_type->valuedouble,
        allowed_geographic_area ? allowed_geographic_areaList : NULL,
        privacy_check_related_action ? privacy_check_related_actionVariable : 0,
        code_word_ind ? code_word_indVariable : 0,
        valid_time_period ? valid_time_period_local_nonprim : NULL,
        code_word_list ? code_word_listList : NULL
    );

    return service_type_unrelated_class_local_var;
end:
    if (allowed_geographic_areaList) {
        OpenAPI_list_for_each(allowed_geographic_areaList, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(allowed_geographic_areaList);
        allowed_geographic_areaList = NULL;
    }
    if (valid_time_period_local_nonprim) {
        OpenAPI_valid_time_period_free(valid_time_period_local_nonprim);
        valid_time_period_local_nonprim = NULL;
    }
    if (code_word_listList) {
        OpenAPI_list_for_each(code_word_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(code_word_listList);
        code_word_listList = NULL;
    }
    return NULL;
}

OpenAPI_service_type_unrelated_class_t *OpenAPI_service_type_unrelated_class_copy(OpenAPI_service_type_unrelated_class_t *dst, OpenAPI_service_type_unrelated_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_type_unrelated_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_type_unrelated_class_convertToJSON() failed");
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

    OpenAPI_service_type_unrelated_class_free(dst);
    dst = OpenAPI_service_type_unrelated_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

