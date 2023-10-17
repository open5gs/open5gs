
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_domain_routing_information.h"

OpenAPI_scp_domain_routing_information_t *OpenAPI_scp_domain_routing_information_create(
    OpenAPI_list_t* scp_domain_list
)
{
    OpenAPI_scp_domain_routing_information_t *scp_domain_routing_information_local_var = ogs_malloc(sizeof(OpenAPI_scp_domain_routing_information_t));
    ogs_assert(scp_domain_routing_information_local_var);

    scp_domain_routing_information_local_var->scp_domain_list = scp_domain_list;

    return scp_domain_routing_information_local_var;
}

void OpenAPI_scp_domain_routing_information_free(OpenAPI_scp_domain_routing_information_t *scp_domain_routing_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scp_domain_routing_information) {
        return;
    }
    if (scp_domain_routing_information->scp_domain_list) {
        OpenAPI_list_for_each(scp_domain_routing_information->scp_domain_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_scp_domain_connectivity_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_domain_routing_information->scp_domain_list);
        scp_domain_routing_information->scp_domain_list = NULL;
    }
    ogs_free(scp_domain_routing_information);
}

cJSON *OpenAPI_scp_domain_routing_information_convertToJSON(OpenAPI_scp_domain_routing_information_t *scp_domain_routing_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scp_domain_routing_information == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_information_convertToJSON() failed [ScpDomainRoutingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!scp_domain_routing_information->scp_domain_list) {
        ogs_error("OpenAPI_scp_domain_routing_information_convertToJSON() failed [scp_domain_list]");
        return NULL;
    }
    cJSON *scp_domain_list = cJSON_AddObjectToObject(item, "scpDomainList");
    if (scp_domain_list == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_information_convertToJSON() failed [scp_domain_list]");
        goto end;
    }
    cJSON *localMapObject = scp_domain_list;
    if (scp_domain_routing_information->scp_domain_list) {
        OpenAPI_list_for_each(scp_domain_routing_information->scp_domain_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_scp_domain_routing_information_convertToJSON() failed [scp_domain_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_scp_domain_routing_information_convertToJSON() failed [scp_domain_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_scp_domain_connectivity_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_scp_domain_routing_information_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_scp_domain_routing_information_t *OpenAPI_scp_domain_routing_information_parseFromJSON(cJSON *scp_domain_routing_informationJSON)
{
    OpenAPI_scp_domain_routing_information_t *scp_domain_routing_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *scp_domain_list = NULL;
    OpenAPI_list_t *scp_domain_listList = NULL;
    scp_domain_list = cJSON_GetObjectItemCaseSensitive(scp_domain_routing_informationJSON, "scpDomainList");
    if (!scp_domain_list) {
        ogs_error("OpenAPI_scp_domain_routing_information_parseFromJSON() failed [scp_domain_list]");
        goto end;
    }
        cJSON *scp_domain_list_local_map = NULL;
        if (!cJSON_IsObject(scp_domain_list) && !cJSON_IsNull(scp_domain_list)) {
            ogs_error("OpenAPI_scp_domain_routing_information_parseFromJSON() failed [scp_domain_list]");
            goto end;
        }
        if (cJSON_IsObject(scp_domain_list)) {
            scp_domain_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(scp_domain_list_local_map, scp_domain_list) {
                cJSON *localMapObject = scp_domain_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_scp_domain_connectivity_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_scp_domain_routing_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(scp_domain_listList, localMapKeyPair);
            }
        }

    scp_domain_routing_information_local_var = OpenAPI_scp_domain_routing_information_create (
        scp_domain_listList
    );

    return scp_domain_routing_information_local_var;
end:
    if (scp_domain_listList) {
        OpenAPI_list_for_each(scp_domain_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_scp_domain_connectivity_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_domain_listList);
        scp_domain_listList = NULL;
    }
    return NULL;
}

OpenAPI_scp_domain_routing_information_t *OpenAPI_scp_domain_routing_information_copy(OpenAPI_scp_domain_routing_information_t *dst, OpenAPI_scp_domain_routing_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scp_domain_routing_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scp_domain_routing_information_convertToJSON() failed");
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

    OpenAPI_scp_domain_routing_information_free(dst);
    dst = OpenAPI_scp_domain_routing_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

