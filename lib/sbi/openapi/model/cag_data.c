
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cag_data.h"

OpenAPI_cag_data_t *OpenAPI_cag_data_create(
    OpenAPI_list_t* cag_infos,
    char *provisioning_time
)
{
    OpenAPI_cag_data_t *cag_data_local_var = ogs_malloc(sizeof(OpenAPI_cag_data_t));
    ogs_assert(cag_data_local_var);

    cag_data_local_var->cag_infos = cag_infos;
    cag_data_local_var->provisioning_time = provisioning_time;

    return cag_data_local_var;
}

void OpenAPI_cag_data_free(OpenAPI_cag_data_t *cag_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cag_data) {
        return;
    }
    if (cag_data->cag_infos) {
        OpenAPI_list_for_each(cag_data->cag_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_cag_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(cag_data->cag_infos);
        cag_data->cag_infos = NULL;
    }
    if (cag_data->provisioning_time) {
        ogs_free(cag_data->provisioning_time);
        cag_data->provisioning_time = NULL;
    }
    ogs_free(cag_data);
}

cJSON *OpenAPI_cag_data_convertToJSON(OpenAPI_cag_data_t *cag_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cag_data == NULL) {
        ogs_error("OpenAPI_cag_data_convertToJSON() failed [CagData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cag_data->cag_infos) {
        ogs_error("OpenAPI_cag_data_convertToJSON() failed [cag_infos]");
        return NULL;
    }
    cJSON *cag_infos = cJSON_AddObjectToObject(item, "cagInfos");
    if (cag_infos == NULL) {
        ogs_error("OpenAPI_cag_data_convertToJSON() failed [cag_infos]");
        goto end;
    }
    cJSON *localMapObject = cag_infos;
    if (cag_data->cag_infos) {
        OpenAPI_list_for_each(cag_data->cag_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_cag_data_convertToJSON() failed [cag_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_cag_data_convertToJSON() failed [cag_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_cag_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_cag_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

    if (cag_data->provisioning_time) {
    if (cJSON_AddStringToObject(item, "provisioningTime", cag_data->provisioning_time) == NULL) {
        ogs_error("OpenAPI_cag_data_convertToJSON() failed [provisioning_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_cag_data_t *OpenAPI_cag_data_parseFromJSON(cJSON *cag_dataJSON)
{
    OpenAPI_cag_data_t *cag_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cag_infos = NULL;
    OpenAPI_list_t *cag_infosList = NULL;
    cJSON *provisioning_time = NULL;
    cag_infos = cJSON_GetObjectItemCaseSensitive(cag_dataJSON, "cagInfos");
    if (!cag_infos) {
        ogs_error("OpenAPI_cag_data_parseFromJSON() failed [cag_infos]");
        goto end;
    }
        cJSON *cag_infos_local_map = NULL;
        if (!cJSON_IsObject(cag_infos) && !cJSON_IsNull(cag_infos)) {
            ogs_error("OpenAPI_cag_data_parseFromJSON() failed [cag_infos]");
            goto end;
        }
        if (cJSON_IsObject(cag_infos)) {
            cag_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(cag_infos_local_map, cag_infos) {
                cJSON *localMapObject = cag_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_cag_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_cag_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(cag_infosList, localMapKeyPair);
            }
        }

    provisioning_time = cJSON_GetObjectItemCaseSensitive(cag_dataJSON, "provisioningTime");
    if (provisioning_time) {
    if (!cJSON_IsString(provisioning_time) && !cJSON_IsNull(provisioning_time)) {
        ogs_error("OpenAPI_cag_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    }

    cag_data_local_var = OpenAPI_cag_data_create (
        cag_infosList,
        provisioning_time && !cJSON_IsNull(provisioning_time) ? ogs_strdup(provisioning_time->valuestring) : NULL
    );

    return cag_data_local_var;
end:
    if (cag_infosList) {
        OpenAPI_list_for_each(cag_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_cag_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(cag_infosList);
        cag_infosList = NULL;
    }
    return NULL;
}

OpenAPI_cag_data_t *OpenAPI_cag_data_copy(OpenAPI_cag_data_t *dst, OpenAPI_cag_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cag_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cag_data_convertToJSON() failed");
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

    OpenAPI_cag_data_free(dst);
    dst = OpenAPI_cag_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

