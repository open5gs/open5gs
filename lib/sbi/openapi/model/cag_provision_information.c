
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cag_provision_information.h"

OpenAPI_cag_provision_information_t *OpenAPI_cag_provision_information_create(
    OpenAPI_list_t *cag_info,
    OpenAPI_list_t *common_valid_time_period_list,
    OpenAPI_list_t* additional_valid_time_period_list,
    OpenAPI_cag_provision_operation_e provision_operation
)
{
    OpenAPI_cag_provision_information_t *cag_provision_information_local_var = ogs_malloc(sizeof(OpenAPI_cag_provision_information_t));
    ogs_assert(cag_provision_information_local_var);

    cag_provision_information_local_var->cag_info = cag_info;
    cag_provision_information_local_var->common_valid_time_period_list = common_valid_time_period_list;
    cag_provision_information_local_var->additional_valid_time_period_list = additional_valid_time_period_list;
    cag_provision_information_local_var->provision_operation = provision_operation;

    return cag_provision_information_local_var;
}

void OpenAPI_cag_provision_information_free(OpenAPI_cag_provision_information_t *cag_provision_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cag_provision_information) {
        return;
    }
    if (cag_provision_information->cag_info) {
        OpenAPI_list_for_each(cag_provision_information->cag_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cag_provision_information->cag_info);
        cag_provision_information->cag_info = NULL;
    }
    if (cag_provision_information->common_valid_time_period_list) {
        OpenAPI_list_for_each(cag_provision_information->common_valid_time_period_list, node) {
            OpenAPI_valid_time_period_free(node->data);
        }
        OpenAPI_list_free(cag_provision_information->common_valid_time_period_list);
        cag_provision_information->common_valid_time_period_list = NULL;
    }
    if (cag_provision_information->additional_valid_time_period_list) {
        OpenAPI_list_for_each(cag_provision_information->additional_valid_time_period_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_valid_time_period_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(cag_provision_information->additional_valid_time_period_list);
        cag_provision_information->additional_valid_time_period_list = NULL;
    }
    ogs_free(cag_provision_information);
}

cJSON *OpenAPI_cag_provision_information_convertToJSON(OpenAPI_cag_provision_information_t *cag_provision_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cag_provision_information == NULL) {
        ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [CagProvisionInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cag_provision_information->cag_info) {
        ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [cag_info]");
        return NULL;
    }
    cJSON *cag_infoList = cJSON_AddArrayToObject(item, "cagInfo");
    if (cag_infoList == NULL) {
        ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [cag_info]");
        goto end;
    }
    OpenAPI_list_for_each(cag_provision_information->cag_info, node) {
        if (cJSON_AddStringToObject(cag_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [cag_info]");
            goto end;
        }
    }

    if (cag_provision_information->common_valid_time_period_list) {
    cJSON *common_valid_time_period_listList = cJSON_AddArrayToObject(item, "commonValidTimePeriodList");
    if (common_valid_time_period_listList == NULL) {
        ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [common_valid_time_period_list]");
        goto end;
    }
    OpenAPI_list_for_each(cag_provision_information->common_valid_time_period_list, node) {
        cJSON *itemLocal = OpenAPI_valid_time_period_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [common_valid_time_period_list]");
            goto end;
        }
        cJSON_AddItemToArray(common_valid_time_period_listList, itemLocal);
    }
    }

    if (cag_provision_information->additional_valid_time_period_list) {
    cJSON *additional_valid_time_period_list = cJSON_AddObjectToObject(item, "additionalValidTimePeriodList");
    if (additional_valid_time_period_list == NULL) {
        ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [additional_valid_time_period_list]");
        goto end;
    }
    cJSON *localMapObject = additional_valid_time_period_list;
    if (cag_provision_information->additional_valid_time_period_list) {
        OpenAPI_list_for_each(cag_provision_information->additional_valid_time_period_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [additional_valid_time_period_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [additional_valid_time_period_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_valid_time_period_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (cag_provision_information->provision_operation != OpenAPI_cag_provision_operation_NULL) {
    if (cJSON_AddStringToObject(item, "provisionOperation", OpenAPI_cag_provision_operation_ToString(cag_provision_information->provision_operation)) == NULL) {
        ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed [provision_operation]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_cag_provision_information_t *OpenAPI_cag_provision_information_parseFromJSON(cJSON *cag_provision_informationJSON)
{
    OpenAPI_cag_provision_information_t *cag_provision_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cag_info = NULL;
    OpenAPI_list_t *cag_infoList = NULL;
    cJSON *common_valid_time_period_list = NULL;
    OpenAPI_list_t *common_valid_time_period_listList = NULL;
    cJSON *additional_valid_time_period_list = NULL;
    OpenAPI_list_t *additional_valid_time_period_listList = NULL;
    cJSON *provision_operation = NULL;
    OpenAPI_cag_provision_operation_e provision_operationVariable = 0;
    cag_info = cJSON_GetObjectItemCaseSensitive(cag_provision_informationJSON, "cagInfo");
    if (!cag_info) {
        ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [cag_info]");
        goto end;
    }
        cJSON *cag_info_local = NULL;
        if (!cJSON_IsArray(cag_info)) {
            ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [cag_info]");
            goto end;
        }

        cag_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(cag_info_local, cag_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(cag_info_local)) {
                ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [cag_info]");
                goto end;
            }
            OpenAPI_list_add(cag_infoList, ogs_strdup(cag_info_local->valuestring));
        }

    common_valid_time_period_list = cJSON_GetObjectItemCaseSensitive(cag_provision_informationJSON, "commonValidTimePeriodList");
    if (common_valid_time_period_list) {
        cJSON *common_valid_time_period_list_local = NULL;
        if (!cJSON_IsArray(common_valid_time_period_list)) {
            ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [common_valid_time_period_list]");
            goto end;
        }

        common_valid_time_period_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(common_valid_time_period_list_local, common_valid_time_period_list) {
            if (!cJSON_IsObject(common_valid_time_period_list_local)) {
                ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [common_valid_time_period_list]");
                goto end;
            }
            OpenAPI_valid_time_period_t *common_valid_time_period_listItem = OpenAPI_valid_time_period_parseFromJSON(common_valid_time_period_list_local);
            if (!common_valid_time_period_listItem) {
                ogs_error("No common_valid_time_period_listItem");
                goto end;
            }
            OpenAPI_list_add(common_valid_time_period_listList, common_valid_time_period_listItem);
        }
    }

    additional_valid_time_period_list = cJSON_GetObjectItemCaseSensitive(cag_provision_informationJSON, "additionalValidTimePeriodList");
    if (additional_valid_time_period_list) {
        cJSON *additional_valid_time_period_list_local_map = NULL;
        if (!cJSON_IsObject(additional_valid_time_period_list) && !cJSON_IsNull(additional_valid_time_period_list)) {
            ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [additional_valid_time_period_list]");
            goto end;
        }
        if (cJSON_IsObject(additional_valid_time_period_list)) {
            additional_valid_time_period_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(additional_valid_time_period_list_local_map, additional_valid_time_period_list) {
                cJSON *localMapObject = additional_valid_time_period_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_valid_time_period_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(additional_valid_time_period_listList, localMapKeyPair);
            }
        }
    }

    provision_operation = cJSON_GetObjectItemCaseSensitive(cag_provision_informationJSON, "provisionOperation");
    if (provision_operation) {
    if (!cJSON_IsString(provision_operation)) {
        ogs_error("OpenAPI_cag_provision_information_parseFromJSON() failed [provision_operation]");
        goto end;
    }
    provision_operationVariable = OpenAPI_cag_provision_operation_FromString(provision_operation->valuestring);
    }

    cag_provision_information_local_var = OpenAPI_cag_provision_information_create (
        cag_infoList,
        common_valid_time_period_list ? common_valid_time_period_listList : NULL,
        additional_valid_time_period_list ? additional_valid_time_period_listList : NULL,
        provision_operation ? provision_operationVariable : 0
    );

    return cag_provision_information_local_var;
end:
    if (cag_infoList) {
        OpenAPI_list_for_each(cag_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cag_infoList);
        cag_infoList = NULL;
    }
    if (common_valid_time_period_listList) {
        OpenAPI_list_for_each(common_valid_time_period_listList, node) {
            OpenAPI_valid_time_period_free(node->data);
        }
        OpenAPI_list_free(common_valid_time_period_listList);
        common_valid_time_period_listList = NULL;
    }
    if (additional_valid_time_period_listList) {
        OpenAPI_list_for_each(additional_valid_time_period_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_valid_time_period_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(additional_valid_time_period_listList);
        additional_valid_time_period_listList = NULL;
    }
    return NULL;
}

OpenAPI_cag_provision_information_t *OpenAPI_cag_provision_information_copy(OpenAPI_cag_provision_information_t *dst, OpenAPI_cag_provision_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cag_provision_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cag_provision_information_convertToJSON() failed");
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

    OpenAPI_cag_provision_information_free(dst);
    dst = OpenAPI_cag_provision_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

