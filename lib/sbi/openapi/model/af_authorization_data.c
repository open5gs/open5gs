
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_authorization_data.h"

OpenAPI_af_authorization_data_t *OpenAPI_af_authorization_data_create(
    OpenAPI_list_t* af_auth_data
)
{
    OpenAPI_af_authorization_data_t *af_authorization_data_local_var = ogs_malloc(sizeof(OpenAPI_af_authorization_data_t));
    ogs_assert(af_authorization_data_local_var);

    af_authorization_data_local_var->af_auth_data = af_auth_data;

    return af_authorization_data_local_var;
}

void OpenAPI_af_authorization_data_free(OpenAPI_af_authorization_data_t *af_authorization_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_authorization_data) {
        return;
    }
    if (af_authorization_data->af_auth_data) {
        OpenAPI_list_for_each(af_authorization_data->af_auth_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_individual_af_authorization_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(af_authorization_data->af_auth_data);
        af_authorization_data->af_auth_data = NULL;
    }
    ogs_free(af_authorization_data);
}

cJSON *OpenAPI_af_authorization_data_convertToJSON(OpenAPI_af_authorization_data_t *af_authorization_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_authorization_data == NULL) {
        ogs_error("OpenAPI_af_authorization_data_convertToJSON() failed [AfAuthorizationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!af_authorization_data->af_auth_data) {
        ogs_error("OpenAPI_af_authorization_data_convertToJSON() failed [af_auth_data]");
        return NULL;
    }
    cJSON *af_auth_data = cJSON_AddObjectToObject(item, "afAuthData");
    if (af_auth_data == NULL) {
        ogs_error("OpenAPI_af_authorization_data_convertToJSON() failed [af_auth_data]");
        goto end;
    }
    cJSON *localMapObject = af_auth_data;
    if (af_authorization_data->af_auth_data) {
        OpenAPI_list_for_each(af_authorization_data->af_auth_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_af_authorization_data_convertToJSON() failed [af_auth_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_af_authorization_data_convertToJSON() failed [af_auth_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_individual_af_authorization_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_af_authorization_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_af_authorization_data_t *OpenAPI_af_authorization_data_parseFromJSON(cJSON *af_authorization_dataJSON)
{
    OpenAPI_af_authorization_data_t *af_authorization_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_auth_data = NULL;
    OpenAPI_list_t *af_auth_dataList = NULL;
    af_auth_data = cJSON_GetObjectItemCaseSensitive(af_authorization_dataJSON, "afAuthData");
    if (!af_auth_data) {
        ogs_error("OpenAPI_af_authorization_data_parseFromJSON() failed [af_auth_data]");
        goto end;
    }
        cJSON *af_auth_data_local_map = NULL;
        if (!cJSON_IsObject(af_auth_data) && !cJSON_IsNull(af_auth_data)) {
            ogs_error("OpenAPI_af_authorization_data_parseFromJSON() failed [af_auth_data]");
            goto end;
        }
        if (cJSON_IsObject(af_auth_data)) {
            af_auth_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(af_auth_data_local_map, af_auth_data) {
                cJSON *localMapObject = af_auth_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_individual_af_authorization_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_af_authorization_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(af_auth_dataList, localMapKeyPair);
            }
        }

    af_authorization_data_local_var = OpenAPI_af_authorization_data_create (
        af_auth_dataList
    );

    return af_authorization_data_local_var;
end:
    if (af_auth_dataList) {
        OpenAPI_list_for_each(af_auth_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_individual_af_authorization_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(af_auth_dataList);
        af_auth_dataList = NULL;
    }
    return NULL;
}

OpenAPI_af_authorization_data_t *OpenAPI_af_authorization_data_copy(OpenAPI_af_authorization_data_t *dst, OpenAPI_af_authorization_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_authorization_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_authorization_data_convertToJSON() failed");
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

    OpenAPI_af_authorization_data_free(dst);
    dst = OpenAPI_af_authorization_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

