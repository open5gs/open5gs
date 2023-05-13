
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_policy_data.h"

OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_create(
    OpenAPI_list_t* pra_infos,
    OpenAPI_list_t *subsc_cats
)
{
    OpenAPI_am_policy_data_t *am_policy_data_local_var = ogs_malloc(sizeof(OpenAPI_am_policy_data_t));
    ogs_assert(am_policy_data_local_var);

    am_policy_data_local_var->pra_infos = pra_infos;
    am_policy_data_local_var->subsc_cats = subsc_cats;

    return am_policy_data_local_var;
}

void OpenAPI_am_policy_data_free(OpenAPI_am_policy_data_t *am_policy_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == am_policy_data) {
        return;
    }
    if (am_policy_data->pra_infos) {
        OpenAPI_list_for_each(am_policy_data->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(am_policy_data->pra_infos);
        am_policy_data->pra_infos = NULL;
    }
    if (am_policy_data->subsc_cats) {
        OpenAPI_list_for_each(am_policy_data->subsc_cats, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(am_policy_data->subsc_cats);
        am_policy_data->subsc_cats = NULL;
    }
    ogs_free(am_policy_data);
}

cJSON *OpenAPI_am_policy_data_convertToJSON(OpenAPI_am_policy_data_t *am_policy_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (am_policy_data == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [AmPolicyData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (am_policy_data->pra_infos) {
    cJSON *pra_infos = cJSON_AddObjectToObject(item, "praInfos");
    if (pra_infos == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [pra_infos]");
        goto end;
    }
    cJSON *localMapObject = pra_infos;
    if (am_policy_data->pra_infos) {
        OpenAPI_list_for_each(am_policy_data->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [pra_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [pra_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (am_policy_data->subsc_cats) {
    cJSON *subsc_catsList = cJSON_AddArrayToObject(item, "subscCats");
    if (subsc_catsList == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [subsc_cats]");
        goto end;
    }
    OpenAPI_list_for_each(am_policy_data->subsc_cats, node) {
        if (cJSON_AddStringToObject(subsc_catsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [subsc_cats]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_parseFromJSON(cJSON *am_policy_dataJSON)
{
    OpenAPI_am_policy_data_t *am_policy_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pra_infos = NULL;
    OpenAPI_list_t *pra_infosList = NULL;
    cJSON *subsc_cats = NULL;
    OpenAPI_list_t *subsc_catsList = NULL;
    pra_infos = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "praInfos");
    if (pra_infos) {
        cJSON *pra_infos_local_map = NULL;
        if (!cJSON_IsObject(pra_infos) && !cJSON_IsNull(pra_infos)) {
            ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [pra_infos]");
            goto end;
        }
        if (cJSON_IsObject(pra_infos)) {
            pra_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pra_infos_local_map, pra_infos) {
                cJSON *localMapObject = pra_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_infosList, localMapKeyPair);
            }
        }
    }

    subsc_cats = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "subscCats");
    if (subsc_cats) {
        cJSON *subsc_cats_local = NULL;
        if (!cJSON_IsArray(subsc_cats)) {
            ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [subsc_cats]");
            goto end;
        }

        subsc_catsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subsc_cats_local, subsc_cats) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subsc_cats_local)) {
                ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [subsc_cats]");
                goto end;
            }
            OpenAPI_list_add(subsc_catsList, ogs_strdup(subsc_cats_local->valuestring));
        }
    }

    am_policy_data_local_var = OpenAPI_am_policy_data_create (
        pra_infos ? pra_infosList : NULL,
        subsc_cats ? subsc_catsList : NULL
    );

    return am_policy_data_local_var;
end:
    if (pra_infosList) {
        OpenAPI_list_for_each(pra_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pra_infosList);
        pra_infosList = NULL;
    }
    if (subsc_catsList) {
        OpenAPI_list_for_each(subsc_catsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subsc_catsList);
        subsc_catsList = NULL;
    }
    return NULL;
}

OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_copy(OpenAPI_am_policy_data_t *dst, OpenAPI_am_policy_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_am_policy_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed");
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

    OpenAPI_am_policy_data_free(dst);
    dst = OpenAPI_am_policy_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

