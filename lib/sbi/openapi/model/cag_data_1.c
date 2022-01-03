
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cag_data_1.h"

OpenAPI_cag_data_1_t *OpenAPI_cag_data_1_create(
    OpenAPI_list_t* cag_infos,
    char *provisioning_time
)
{
    OpenAPI_cag_data_1_t *cag_data_1_local_var = ogs_malloc(sizeof(OpenAPI_cag_data_1_t));
    ogs_assert(cag_data_1_local_var);

    cag_data_1_local_var->cag_infos = cag_infos;
    cag_data_1_local_var->provisioning_time = provisioning_time;

    return cag_data_1_local_var;
}

void OpenAPI_cag_data_1_free(OpenAPI_cag_data_1_t *cag_data_1)
{
    if (NULL == cag_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(cag_data_1->cag_infos, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_cag_info_1_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(cag_data_1->cag_infos);
    ogs_free(cag_data_1->provisioning_time);
    ogs_free(cag_data_1);
}

cJSON *OpenAPI_cag_data_1_convertToJSON(OpenAPI_cag_data_1_t *cag_data_1)
{
    cJSON *item = NULL;

    if (cag_data_1 == NULL) {
        ogs_error("OpenAPI_cag_data_1_convertToJSON() failed [CagData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *cag_infos = cJSON_AddObjectToObject(item, "cagInfos");
    if (cag_infos == NULL) {
        ogs_error("OpenAPI_cag_data_1_convertToJSON() failed [cag_infos]");
        goto end;
    }
    cJSON *localMapObject = cag_infos;
    OpenAPI_lnode_t *cag_infos_node;
    if (cag_data_1->cag_infos) {
        OpenAPI_list_for_each(cag_data_1->cag_infos, cag_infos_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)cag_infos_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_cag_info_1_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_cag_data_1_convertToJSON() failed [cag_infos]");
            goto end;
        }
        cJSON_AddItemToObject(cag_infos, localKeyValue->key, itemLocal);
            }
        }

    if (cag_data_1->provisioning_time) {
    if (cJSON_AddStringToObject(item, "provisioningTime", cag_data_1->provisioning_time) == NULL) {
        ogs_error("OpenAPI_cag_data_1_convertToJSON() failed [provisioning_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_cag_data_1_t *OpenAPI_cag_data_1_parseFromJSON(cJSON *cag_data_1JSON)
{
    OpenAPI_cag_data_1_t *cag_data_1_local_var = NULL;
    cJSON *cag_infos = cJSON_GetObjectItemCaseSensitive(cag_data_1JSON, "cagInfos");
    if (!cag_infos) {
        ogs_error("OpenAPI_cag_data_1_parseFromJSON() failed [cag_infos]");
        goto end;
    }

    OpenAPI_list_t *cag_infosList;
    cJSON *cag_infos_local_map;
    if (!cJSON_IsObject(cag_infos)) {
        ogs_error("OpenAPI_cag_data_1_parseFromJSON() failed [cag_infos]");
        goto end;
    }
    cag_infosList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(cag_infos_local_map, cag_infos) {
        cJSON *localMapObject = cag_infos_local_map;
        if (cJSON_IsObject(cag_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_cag_info_1_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(cag_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_cag_data_1_parseFromJSON() failed [cag_infos]");
            goto end;
        }
        OpenAPI_list_add(cag_infosList , localMapKeyPair);
    }

    cJSON *provisioning_time = cJSON_GetObjectItemCaseSensitive(cag_data_1JSON, "provisioningTime");

    if (provisioning_time) {
    if (!cJSON_IsString(provisioning_time)) {
        ogs_error("OpenAPI_cag_data_1_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    }

    cag_data_1_local_var = OpenAPI_cag_data_1_create (
        cag_infosList,
        provisioning_time ? ogs_strdup(provisioning_time->valuestring) : NULL
    );

    return cag_data_1_local_var;
end:
    return NULL;
}

OpenAPI_cag_data_1_t *OpenAPI_cag_data_1_copy(OpenAPI_cag_data_1_t *dst, OpenAPI_cag_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cag_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cag_data_1_convertToJSON() failed");
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

    OpenAPI_cag_data_1_free(dst);
    dst = OpenAPI_cag_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

