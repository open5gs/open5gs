
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_name_list_cond.h"

char *OpenAPI_condition_typeservice_name_list_cond_ToString(OpenAPI_service_name_list_cond_condition_type_e condition_type)
{
    const char *condition_typeArray[] =  { "NULL", "SERVICE_NAME_LIST_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    if (condition_type < sizeofArray)
        return (char *)condition_typeArray[condition_type];
    else
        return (char *)"Unknown";
}

OpenAPI_service_name_list_cond_condition_type_e OpenAPI_condition_typeservice_name_list_cond_FromString(char* condition_type)
{
    int stringToReturn = 0;
    const char *condition_typeArray[] =  { "NULL", "SERVICE_NAME_LIST_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(condition_type, condition_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_service_name_list_cond_t *OpenAPI_service_name_list_cond_create(
    OpenAPI_service_name_list_cond_condition_type_e condition_type,
    OpenAPI_list_t *service_name_list
)
{
    OpenAPI_service_name_list_cond_t *service_name_list_cond_local_var = ogs_malloc(sizeof(OpenAPI_service_name_list_cond_t));
    ogs_assert(service_name_list_cond_local_var);

    service_name_list_cond_local_var->condition_type = condition_type;
    service_name_list_cond_local_var->service_name_list = service_name_list;

    return service_name_list_cond_local_var;
}

void OpenAPI_service_name_list_cond_free(OpenAPI_service_name_list_cond_t *service_name_list_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_name_list_cond) {
        return;
    }
    if (service_name_list_cond->service_name_list) {
        OpenAPI_list_for_each(service_name_list_cond->service_name_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_name_list_cond->service_name_list);
        service_name_list_cond->service_name_list = NULL;
    }
    ogs_free(service_name_list_cond);
}

cJSON *OpenAPI_service_name_list_cond_convertToJSON(OpenAPI_service_name_list_cond_t *service_name_list_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_name_list_cond == NULL) {
        ogs_error("OpenAPI_service_name_list_cond_convertToJSON() failed [ServiceNameListCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_name_list_cond->condition_type == OpenAPI_service_name_list_cond_CONDITIONTYPE_NULL) {
        ogs_error("OpenAPI_service_name_list_cond_convertToJSON() failed [condition_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "conditionType", OpenAPI_condition_typeservice_name_list_cond_ToString(service_name_list_cond->condition_type)) == NULL) {
        ogs_error("OpenAPI_service_name_list_cond_convertToJSON() failed [condition_type]");
        goto end;
    }

    if (!service_name_list_cond->service_name_list) {
        ogs_error("OpenAPI_service_name_list_cond_convertToJSON() failed [service_name_list]");
        return NULL;
    }
    cJSON *service_name_listList = cJSON_AddArrayToObject(item, "serviceNameList");
    if (service_name_listList == NULL) {
        ogs_error("OpenAPI_service_name_list_cond_convertToJSON() failed [service_name_list]");
        goto end;
    }
    OpenAPI_list_for_each(service_name_list_cond->service_name_list, node) {
        if (cJSON_AddStringToObject(service_name_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_service_name_list_cond_convertToJSON() failed [service_name_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_service_name_list_cond_t *OpenAPI_service_name_list_cond_parseFromJSON(cJSON *service_name_list_condJSON)
{
    OpenAPI_service_name_list_cond_t *service_name_list_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *condition_type = NULL;
    OpenAPI_service_name_list_cond_condition_type_e condition_typeVariable = 0;
    cJSON *service_name_list = NULL;
    OpenAPI_list_t *service_name_listList = NULL;
    condition_type = cJSON_GetObjectItemCaseSensitive(service_name_list_condJSON, "conditionType");
    if (!condition_type) {
        ogs_error("OpenAPI_service_name_list_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    if (!cJSON_IsString(condition_type)) {
        ogs_error("OpenAPI_service_name_list_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    condition_typeVariable = OpenAPI_condition_typeservice_name_list_cond_FromString(condition_type->valuestring);

    service_name_list = cJSON_GetObjectItemCaseSensitive(service_name_list_condJSON, "serviceNameList");
    if (!service_name_list) {
        ogs_error("OpenAPI_service_name_list_cond_parseFromJSON() failed [service_name_list]");
        goto end;
    }
        cJSON *service_name_list_local = NULL;
        if (!cJSON_IsArray(service_name_list)) {
            ogs_error("OpenAPI_service_name_list_cond_parseFromJSON() failed [service_name_list]");
            goto end;
        }

        service_name_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(service_name_list_local, service_name_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(service_name_list_local)) {
                ogs_error("OpenAPI_service_name_list_cond_parseFromJSON() failed [service_name_list]");
                goto end;
            }
            OpenAPI_list_add(service_name_listList, ogs_strdup(service_name_list_local->valuestring));
        }

    service_name_list_cond_local_var = OpenAPI_service_name_list_cond_create (
        condition_typeVariable,
        service_name_listList
    );

    return service_name_list_cond_local_var;
end:
    if (service_name_listList) {
        OpenAPI_list_for_each(service_name_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_name_listList);
        service_name_listList = NULL;
    }
    return NULL;
}

OpenAPI_service_name_list_cond_t *OpenAPI_service_name_list_cond_copy(OpenAPI_service_name_list_cond_t *dst, OpenAPI_service_name_list_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_name_list_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_name_list_cond_convertToJSON() failed");
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

    OpenAPI_service_name_list_cond_free(dst);
    dst = OpenAPI_service_name_list_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

