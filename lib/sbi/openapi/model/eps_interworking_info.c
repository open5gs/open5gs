
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_interworking_info.h"

OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_create(
    OpenAPI_list_t* eps_iwk_pgws
    )
{
    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_eps_interworking_info_t));
    if (!eps_interworking_info_local_var) {
        return NULL;
    }
    eps_interworking_info_local_var->eps_iwk_pgws = eps_iwk_pgws;

    return eps_interworking_info_local_var;
}

void OpenAPI_eps_interworking_info_free(OpenAPI_eps_interworking_info_t *eps_interworking_info)
{
    if (NULL == eps_interworking_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(eps_interworking_info->eps_iwk_pgws, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_object_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(eps_interworking_info->eps_iwk_pgws);
    ogs_free(eps_interworking_info);
}

cJSON *OpenAPI_eps_interworking_info_convertToJSON(OpenAPI_eps_interworking_info_t *eps_interworking_info)
{
    cJSON *item = NULL;

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
        OpenAPI_lnode_t *eps_iwk_pgws_node;
        if (eps_interworking_info->eps_iwk_pgws) {
            OpenAPI_list_for_each(eps_interworking_info->eps_iwk_pgws, eps_iwk_pgws_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)eps_iwk_pgws_node->data;
                cJSON *itemLocal = OpenAPI_object_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_eps_interworking_info_convertToJSON() failed [eps_iwk_pgws]");
                    goto end;
                }
                cJSON_AddItemToObject(eps_iwk_pgws, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_parseFromJSON(cJSON *eps_interworking_infoJSON)
{
    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_var = NULL;
    cJSON *eps_iwk_pgws = cJSON_GetObjectItemCaseSensitive(eps_interworking_infoJSON, "epsIwkPgws");

    OpenAPI_list_t *eps_iwk_pgwsList;
    if (eps_iwk_pgws) {
        cJSON *eps_iwk_pgws_local_map;
        if (!cJSON_IsObject(eps_iwk_pgws)) {
            ogs_error("OpenAPI_eps_interworking_info_parseFromJSON() failed [eps_iwk_pgws]");
            goto end;
        }
        eps_iwk_pgwsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(eps_iwk_pgws_local_map, eps_iwk_pgws) {
            cJSON *localMapObject = eps_iwk_pgws_local_map;
            if (!cJSON_IsObject(eps_iwk_pgws_local_map)) {
                ogs_error("OpenAPI_eps_interworking_info_parseFromJSON() failed [eps_iwk_pgws]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_object_parseFromJSON(localMapObject));
            OpenAPI_list_add(eps_iwk_pgwsList, localMapKeyPair);
        }
    }

    eps_interworking_info_local_var = OpenAPI_eps_interworking_info_create (
        eps_iwk_pgws ? eps_iwk_pgwsList : NULL
        );

    return eps_interworking_info_local_var;
end:
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

