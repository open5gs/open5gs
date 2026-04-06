
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_identifiers.h"

OpenAPI_ue_identifiers_t *OpenAPI_ue_identifiers_create(
    OpenAPI_list_t* ue_id_list,
    OpenAPI_list_t* ue_id_gpsi_list
)
{
    OpenAPI_ue_identifiers_t *ue_identifiers_local_var = ogs_malloc(sizeof(OpenAPI_ue_identifiers_t));
    ogs_assert(ue_identifiers_local_var);

    ue_identifiers_local_var->ue_id_list = ue_id_list;
    ue_identifiers_local_var->ue_id_gpsi_list = ue_id_gpsi_list;

    return ue_identifiers_local_var;
}

void OpenAPI_ue_identifiers_free(OpenAPI_ue_identifiers_t *ue_identifiers)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_identifiers) {
        return;
    }
    if (ue_identifiers->ue_id_list) {
        OpenAPI_list_for_each(ue_identifiers->ue_id_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_supi_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_identifiers->ue_id_list);
        ue_identifiers->ue_id_list = NULL;
    }
    if (ue_identifiers->ue_id_gpsi_list) {
        OpenAPI_list_for_each(ue_identifiers->ue_id_gpsi_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_gpsi_information_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_identifiers->ue_id_gpsi_list);
        ue_identifiers->ue_id_gpsi_list = NULL;
    }
    ogs_free(ue_identifiers);
}

cJSON *OpenAPI_ue_identifiers_convertToJSON(OpenAPI_ue_identifiers_t *ue_identifiers)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_identifiers == NULL) {
        ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [UeIdentifiers]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_identifiers->ue_id_list) {
    cJSON *ue_id_list = cJSON_AddObjectToObject(item, "ueIdList");
    if (ue_id_list == NULL) {
        ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [ue_id_list]");
        goto end;
    }
    cJSON *localMapObject = ue_id_list;
    if (ue_identifiers->ue_id_list) {
        OpenAPI_list_for_each(ue_identifiers->ue_id_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [ue_id_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [ue_id_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_supi_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_identifiers->ue_id_gpsi_list) {
    cJSON *ue_id_gpsi_list = cJSON_AddObjectToObject(item, "ueIdGpsiList");
    if (ue_id_gpsi_list == NULL) {
        ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [ue_id_gpsi_list]");
        goto end;
    }
    cJSON *localMapObject = ue_id_gpsi_list;
    if (ue_identifiers->ue_id_gpsi_list) {
        OpenAPI_list_for_each(ue_identifiers->ue_id_gpsi_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [ue_id_gpsi_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [ue_id_gpsi_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_gpsi_information_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_ue_identifiers_t *OpenAPI_ue_identifiers_parseFromJSON(cJSON *ue_identifiersJSON)
{
    OpenAPI_ue_identifiers_t *ue_identifiers_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_id_list = NULL;
    OpenAPI_list_t *ue_id_listList = NULL;
    cJSON *ue_id_gpsi_list = NULL;
    OpenAPI_list_t *ue_id_gpsi_listList = NULL;
    ue_id_list = cJSON_GetObjectItemCaseSensitive(ue_identifiersJSON, "ueIdList");
    if (ue_id_list) {
        cJSON *ue_id_list_local_map = NULL;
        if (!cJSON_IsObject(ue_id_list) && !cJSON_IsNull(ue_id_list)) {
            ogs_error("OpenAPI_ue_identifiers_parseFromJSON() failed [ue_id_list]");
            goto end;
        }
        if (cJSON_IsObject(ue_id_list)) {
            ue_id_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(ue_id_list_local_map, ue_id_list) {
                cJSON *localMapObject = ue_id_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_supi_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_identifiers_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(ue_id_listList, localMapKeyPair);
            }
        }
    }

    ue_id_gpsi_list = cJSON_GetObjectItemCaseSensitive(ue_identifiersJSON, "ueIdGpsiList");
    if (ue_id_gpsi_list) {
        cJSON *ue_id_gpsi_list_local_map = NULL;
        if (!cJSON_IsObject(ue_id_gpsi_list) && !cJSON_IsNull(ue_id_gpsi_list)) {
            ogs_error("OpenAPI_ue_identifiers_parseFromJSON() failed [ue_id_gpsi_list]");
            goto end;
        }
        if (cJSON_IsObject(ue_id_gpsi_list)) {
            ue_id_gpsi_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(ue_id_gpsi_list_local_map, ue_id_gpsi_list) {
                cJSON *localMapObject = ue_id_gpsi_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_gpsi_information_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_identifiers_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(ue_id_gpsi_listList, localMapKeyPair);
            }
        }
    }

    ue_identifiers_local_var = OpenAPI_ue_identifiers_create (
        ue_id_list ? ue_id_listList : NULL,
        ue_id_gpsi_list ? ue_id_gpsi_listList : NULL
    );

    return ue_identifiers_local_var;
end:
    if (ue_id_listList) {
        OpenAPI_list_for_each(ue_id_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_supi_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_id_listList);
        ue_id_listList = NULL;
    }
    if (ue_id_gpsi_listList) {
        OpenAPI_list_for_each(ue_id_gpsi_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_gpsi_information_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_id_gpsi_listList);
        ue_id_gpsi_listList = NULL;
    }
    return NULL;
}

OpenAPI_ue_identifiers_t *OpenAPI_ue_identifiers_copy(OpenAPI_ue_identifiers_t *dst, OpenAPI_ue_identifiers_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_identifiers_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_identifiers_convertToJSON() failed");
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

    OpenAPI_ue_identifiers_free(dst);
    dst = OpenAPI_ue_identifiers_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

