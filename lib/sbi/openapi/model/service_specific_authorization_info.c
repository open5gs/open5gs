
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_specific_authorization_info.h"

OpenAPI_service_specific_authorization_info_t *OpenAPI_service_specific_authorization_info_create(
    OpenAPI_list_t* service_specific_authorization_list
)
{
    OpenAPI_service_specific_authorization_info_t *service_specific_authorization_info_local_var = ogs_malloc(sizeof(OpenAPI_service_specific_authorization_info_t));
    ogs_assert(service_specific_authorization_info_local_var);

    service_specific_authorization_info_local_var->service_specific_authorization_list = service_specific_authorization_list;

    return service_specific_authorization_info_local_var;
}

void OpenAPI_service_specific_authorization_info_free(OpenAPI_service_specific_authorization_info_t *service_specific_authorization_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_specific_authorization_info) {
        return;
    }
    if (service_specific_authorization_info->service_specific_authorization_list) {
        OpenAPI_list_for_each(service_specific_authorization_info->service_specific_authorization_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_authorization_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(service_specific_authorization_info->service_specific_authorization_list);
        service_specific_authorization_info->service_specific_authorization_list = NULL;
    }
    ogs_free(service_specific_authorization_info);
}

cJSON *OpenAPI_service_specific_authorization_info_convertToJSON(OpenAPI_service_specific_authorization_info_t *service_specific_authorization_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_specific_authorization_info == NULL) {
        ogs_error("OpenAPI_service_specific_authorization_info_convertToJSON() failed [ServiceSpecificAuthorizationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_specific_authorization_info->service_specific_authorization_list) {
        ogs_error("OpenAPI_service_specific_authorization_info_convertToJSON() failed [service_specific_authorization_list]");
        return NULL;
    }
    cJSON *service_specific_authorization_list = cJSON_AddObjectToObject(item, "serviceSpecificAuthorizationList");
    if (service_specific_authorization_list == NULL) {
        ogs_error("OpenAPI_service_specific_authorization_info_convertToJSON() failed [service_specific_authorization_list]");
        goto end;
    }
    cJSON *localMapObject = service_specific_authorization_list;
    if (service_specific_authorization_info->service_specific_authorization_list) {
        OpenAPI_list_for_each(service_specific_authorization_info->service_specific_authorization_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_service_specific_authorization_info_convertToJSON() failed [service_specific_authorization_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_service_specific_authorization_info_convertToJSON() failed [service_specific_authorization_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_authorization_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_service_specific_authorization_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_service_specific_authorization_info_t *OpenAPI_service_specific_authorization_info_parseFromJSON(cJSON *service_specific_authorization_infoJSON)
{
    OpenAPI_service_specific_authorization_info_t *service_specific_authorization_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *service_specific_authorization_list = NULL;
    OpenAPI_list_t *service_specific_authorization_listList = NULL;
    service_specific_authorization_list = cJSON_GetObjectItemCaseSensitive(service_specific_authorization_infoJSON, "serviceSpecificAuthorizationList");
    if (!service_specific_authorization_list) {
        ogs_error("OpenAPI_service_specific_authorization_info_parseFromJSON() failed [service_specific_authorization_list]");
        goto end;
    }
        cJSON *service_specific_authorization_list_local_map = NULL;
        if (!cJSON_IsObject(service_specific_authorization_list) && !cJSON_IsNull(service_specific_authorization_list)) {
            ogs_error("OpenAPI_service_specific_authorization_info_parseFromJSON() failed [service_specific_authorization_list]");
            goto end;
        }
        if (cJSON_IsObject(service_specific_authorization_list)) {
            service_specific_authorization_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(service_specific_authorization_list_local_map, service_specific_authorization_list) {
                cJSON *localMapObject = service_specific_authorization_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_authorization_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_service_specific_authorization_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(service_specific_authorization_listList, localMapKeyPair);
            }
        }

    service_specific_authorization_info_local_var = OpenAPI_service_specific_authorization_info_create (
        service_specific_authorization_listList
    );

    return service_specific_authorization_info_local_var;
end:
    if (service_specific_authorization_listList) {
        OpenAPI_list_for_each(service_specific_authorization_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_authorization_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(service_specific_authorization_listList);
        service_specific_authorization_listList = NULL;
    }
    return NULL;
}

OpenAPI_service_specific_authorization_info_t *OpenAPI_service_specific_authorization_info_copy(OpenAPI_service_specific_authorization_info_t *dst, OpenAPI_service_specific_authorization_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_specific_authorization_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_specific_authorization_info_convertToJSON() failed");
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

    OpenAPI_service_specific_authorization_info_free(dst);
    dst = OpenAPI_service_specific_authorization_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

