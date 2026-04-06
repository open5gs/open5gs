
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "odb_exempted_dnn_info.h"

OpenAPI_odb_exempted_dnn_info_t *OpenAPI_odb_exempted_dnn_info_create(
    int odb_exempted_ind,
    OpenAPI_dnn_configuration_t *dnn_configuration,
    OpenAPI_list_t* odb_exempted_conditions_data
)
{
    OpenAPI_odb_exempted_dnn_info_t *odb_exempted_dnn_info_local_var = ogs_malloc(sizeof(OpenAPI_odb_exempted_dnn_info_t));
    ogs_assert(odb_exempted_dnn_info_local_var);

    odb_exempted_dnn_info_local_var->odb_exempted_ind = odb_exempted_ind;
    odb_exempted_dnn_info_local_var->dnn_configuration = dnn_configuration;
    odb_exempted_dnn_info_local_var->odb_exempted_conditions_data = odb_exempted_conditions_data;

    return odb_exempted_dnn_info_local_var;
}

void OpenAPI_odb_exempted_dnn_info_free(OpenAPI_odb_exempted_dnn_info_t *odb_exempted_dnn_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == odb_exempted_dnn_info) {
        return;
    }
    if (odb_exempted_dnn_info->dnn_configuration) {
        OpenAPI_dnn_configuration_free(odb_exempted_dnn_info->dnn_configuration);
        odb_exempted_dnn_info->dnn_configuration = NULL;
    }
    if (odb_exempted_dnn_info->odb_exempted_conditions_data) {
        OpenAPI_list_for_each(odb_exempted_dnn_info->odb_exempted_conditions_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_odb_exempted_conditions_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(odb_exempted_dnn_info->odb_exempted_conditions_data);
        odb_exempted_dnn_info->odb_exempted_conditions_data = NULL;
    }
    ogs_free(odb_exempted_dnn_info);
}

cJSON *OpenAPI_odb_exempted_dnn_info_convertToJSON(OpenAPI_odb_exempted_dnn_info_t *odb_exempted_dnn_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (odb_exempted_dnn_info == NULL) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [OdbExemptedDnnInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "odbExemptedInd", odb_exempted_dnn_info->odb_exempted_ind) == NULL) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [odb_exempted_ind]");
        goto end;
    }

    if (!odb_exempted_dnn_info->dnn_configuration) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [dnn_configuration]");
        return NULL;
    }
    cJSON *dnn_configuration_local_JSON = OpenAPI_dnn_configuration_convertToJSON(odb_exempted_dnn_info->dnn_configuration);
    if (dnn_configuration_local_JSON == NULL) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [dnn_configuration]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnnConfiguration", dnn_configuration_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [dnn_configuration]");
        goto end;
    }

    if (odb_exempted_dnn_info->odb_exempted_conditions_data) {
    cJSON *odb_exempted_conditions_data = cJSON_AddObjectToObject(item, "odbExemptedConditionsData");
    if (odb_exempted_conditions_data == NULL) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [odb_exempted_conditions_data]");
        goto end;
    }
    cJSON *localMapObject = odb_exempted_conditions_data;
    if (odb_exempted_dnn_info->odb_exempted_conditions_data) {
        OpenAPI_list_for_each(odb_exempted_dnn_info->odb_exempted_conditions_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [odb_exempted_conditions_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [odb_exempted_conditions_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_odb_exempted_conditions_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_odb_exempted_dnn_info_t *OpenAPI_odb_exempted_dnn_info_parseFromJSON(cJSON *odb_exempted_dnn_infoJSON)
{
    OpenAPI_odb_exempted_dnn_info_t *odb_exempted_dnn_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *odb_exempted_ind = NULL;
    cJSON *dnn_configuration = NULL;
    OpenAPI_dnn_configuration_t *dnn_configuration_local_nonprim = NULL;
    cJSON *odb_exempted_conditions_data = NULL;
    OpenAPI_list_t *odb_exempted_conditions_dataList = NULL;
    odb_exempted_ind = cJSON_GetObjectItemCaseSensitive(odb_exempted_dnn_infoJSON, "odbExemptedInd");
    if (!odb_exempted_ind) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_parseFromJSON() failed [odb_exempted_ind]");
        goto end;
    }
    if (!cJSON_IsBool(odb_exempted_ind)) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_parseFromJSON() failed [odb_exempted_ind]");
        goto end;
    }

    dnn_configuration = cJSON_GetObjectItemCaseSensitive(odb_exempted_dnn_infoJSON, "dnnConfiguration");
    if (!dnn_configuration) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_parseFromJSON() failed [dnn_configuration]");
        goto end;
    }
    dnn_configuration_local_nonprim = OpenAPI_dnn_configuration_parseFromJSON(dnn_configuration);
    if (!dnn_configuration_local_nonprim) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON failed [dnn_configuration]");
        goto end;
    }

    odb_exempted_conditions_data = cJSON_GetObjectItemCaseSensitive(odb_exempted_dnn_infoJSON, "odbExemptedConditionsData");
    if (odb_exempted_conditions_data) {
        cJSON *odb_exempted_conditions_data_local_map = NULL;
        if (!cJSON_IsObject(odb_exempted_conditions_data) && !cJSON_IsNull(odb_exempted_conditions_data)) {
            ogs_error("OpenAPI_odb_exempted_dnn_info_parseFromJSON() failed [odb_exempted_conditions_data]");
            goto end;
        }
        if (cJSON_IsObject(odb_exempted_conditions_data)) {
            odb_exempted_conditions_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(odb_exempted_conditions_data_local_map, odb_exempted_conditions_data) {
                cJSON *localMapObject = odb_exempted_conditions_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_odb_exempted_conditions_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_odb_exempted_dnn_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(odb_exempted_conditions_dataList, localMapKeyPair);
            }
        }
    }

    odb_exempted_dnn_info_local_var = OpenAPI_odb_exempted_dnn_info_create (
        
        odb_exempted_ind->valueint,
        dnn_configuration_local_nonprim,
        odb_exempted_conditions_data ? odb_exempted_conditions_dataList : NULL
    );

    return odb_exempted_dnn_info_local_var;
end:
    if (dnn_configuration_local_nonprim) {
        OpenAPI_dnn_configuration_free(dnn_configuration_local_nonprim);
        dnn_configuration_local_nonprim = NULL;
    }
    if (odb_exempted_conditions_dataList) {
        OpenAPI_list_for_each(odb_exempted_conditions_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_odb_exempted_conditions_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(odb_exempted_conditions_dataList);
        odb_exempted_conditions_dataList = NULL;
    }
    return NULL;
}

OpenAPI_odb_exempted_dnn_info_t *OpenAPI_odb_exempted_dnn_info_copy(OpenAPI_odb_exempted_dnn_info_t *dst, OpenAPI_odb_exempted_dnn_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_odb_exempted_dnn_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_odb_exempted_dnn_info_convertToJSON() failed");
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

    OpenAPI_odb_exempted_dnn_info_free(dst);
    dst = OpenAPI_odb_exempted_dnn_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

