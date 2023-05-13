
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_interworking_info.h"

OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_create(
    OpenAPI_list_t* eps_iwk_pgws
)
{
    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_var = ogs_malloc(sizeof(OpenAPI_eps_interworking_info_t));
    ogs_assert(eps_interworking_info_local_var);

    eps_interworking_info_local_var->eps_iwk_pgws = eps_iwk_pgws;

    return eps_interworking_info_local_var;
}

void OpenAPI_eps_interworking_info_free(OpenAPI_eps_interworking_info_t *eps_interworking_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eps_interworking_info) {
        return;
    }
    if (eps_interworking_info->eps_iwk_pgws) {
        OpenAPI_list_for_each(eps_interworking_info->eps_iwk_pgws, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_eps_iwk_pgw_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(eps_interworking_info->eps_iwk_pgws);
        eps_interworking_info->eps_iwk_pgws = NULL;
    }
    ogs_free(eps_interworking_info);
}

cJSON *OpenAPI_eps_interworking_info_convertToJSON(OpenAPI_eps_interworking_info_t *eps_interworking_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eps_interworking_info == NULL) {
        ogs_error("OpenAPI_eps_interworking_info_convertToJSON() failed [EpsInterworkingInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (eps_interworking_info->eps_iwk_pgws) {
    cJSON *eps_iwk_pgws = cJSON_AddObjectToObject(item, "epsIwkPgws");
    if (eps_iwk_pgws == NULL) {
        ogs_error("OpenAPI_eps_interworking_info_convertToJSON() failed [eps_iwk_pgws]");
        goto end;
    }
    cJSON *localMapObject = eps_iwk_pgws;
    if (eps_interworking_info->eps_iwk_pgws) {
        OpenAPI_list_for_each(eps_interworking_info->eps_iwk_pgws, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_eps_interworking_info_convertToJSON() failed [eps_iwk_pgws]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_eps_interworking_info_convertToJSON() failed [eps_iwk_pgws]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_eps_iwk_pgw_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_eps_interworking_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_parseFromJSON(cJSON *eps_interworking_infoJSON)
{
    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *eps_iwk_pgws = NULL;
    OpenAPI_list_t *eps_iwk_pgwsList = NULL;
    eps_iwk_pgws = cJSON_GetObjectItemCaseSensitive(eps_interworking_infoJSON, "epsIwkPgws");
    if (eps_iwk_pgws) {
        cJSON *eps_iwk_pgws_local_map = NULL;
        if (!cJSON_IsObject(eps_iwk_pgws) && !cJSON_IsNull(eps_iwk_pgws)) {
            ogs_error("OpenAPI_eps_interworking_info_parseFromJSON() failed [eps_iwk_pgws]");
            goto end;
        }
        if (cJSON_IsObject(eps_iwk_pgws)) {
            eps_iwk_pgwsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(eps_iwk_pgws_local_map, eps_iwk_pgws) {
                cJSON *localMapObject = eps_iwk_pgws_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_eps_iwk_pgw_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_eps_interworking_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(eps_iwk_pgwsList, localMapKeyPair);
            }
        }
    }

    eps_interworking_info_local_var = OpenAPI_eps_interworking_info_create (
        eps_iwk_pgws ? eps_iwk_pgwsList : NULL
    );

    return eps_interworking_info_local_var;
end:
    if (eps_iwk_pgwsList) {
        OpenAPI_list_for_each(eps_iwk_pgwsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_eps_iwk_pgw_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(eps_iwk_pgwsList);
        eps_iwk_pgwsList = NULL;
    }
    return NULL;
}

OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_copy(OpenAPI_eps_interworking_info_t *dst, OpenAPI_eps_interworking_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eps_interworking_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eps_interworking_info_convertToJSON() failed");
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

    OpenAPI_eps_interworking_info_free(dst);
    dst = OpenAPI_eps_interworking_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

