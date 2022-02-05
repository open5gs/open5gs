
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_slice_cond.h"

OpenAPI_network_slice_cond_t *OpenAPI_network_slice_cond_create(
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *nsi_list
)
{
    OpenAPI_network_slice_cond_t *network_slice_cond_local_var = ogs_malloc(sizeof(OpenAPI_network_slice_cond_t));
    ogs_assert(network_slice_cond_local_var);

    network_slice_cond_local_var->snssai_list = snssai_list;
    network_slice_cond_local_var->nsi_list = nsi_list;

    return network_slice_cond_local_var;
}

void OpenAPI_network_slice_cond_free(OpenAPI_network_slice_cond_t *network_slice_cond)
{
    if (NULL == network_slice_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(network_slice_cond->snssai_list, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(network_slice_cond->snssai_list);
    OpenAPI_list_for_each(network_slice_cond->nsi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(network_slice_cond->nsi_list);
    ogs_free(network_slice_cond);
}

cJSON *OpenAPI_network_slice_cond_convertToJSON(OpenAPI_network_slice_cond_t *network_slice_cond)
{
    cJSON *item = NULL;

    if (network_slice_cond == NULL) {
        ogs_error("OpenAPI_network_slice_cond_convertToJSON() failed [NetworkSliceCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_network_slice_cond_convertToJSON() failed [snssai_list]");
        goto end;
    }

    OpenAPI_lnode_t *snssai_list_node;
    if (network_slice_cond->snssai_list) {
        OpenAPI_list_for_each(network_slice_cond->snssai_list, snssai_list_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(snssai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_network_slice_cond_convertToJSON() failed [snssai_list]");
                goto end;
            }
            cJSON_AddItemToArray(snssai_listList, itemLocal);
        }
    }

    if (network_slice_cond->nsi_list) {
    cJSON *nsi_list = cJSON_AddArrayToObject(item, "nsiList");
    if (nsi_list == NULL) {
        ogs_error("OpenAPI_network_slice_cond_convertToJSON() failed [nsi_list]");
        goto end;
    }

    OpenAPI_lnode_t *nsi_list_node;
    OpenAPI_list_for_each(network_slice_cond->nsi_list, nsi_list_node)  {
    if (cJSON_AddStringToObject(nsi_list, "", (char*)nsi_list_node->data) == NULL) {
        ogs_error("OpenAPI_network_slice_cond_convertToJSON() failed [nsi_list]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_network_slice_cond_t *OpenAPI_network_slice_cond_parseFromJSON(cJSON *network_slice_condJSON)
{
    OpenAPI_network_slice_cond_t *network_slice_cond_local_var = NULL;
    cJSON *snssai_list = cJSON_GetObjectItemCaseSensitive(network_slice_condJSON, "snssaiList");
    if (!snssai_list) {
        ogs_error("OpenAPI_network_slice_cond_parseFromJSON() failed [snssai_list]");
        goto end;
    }

    OpenAPI_list_t *snssai_listList;
    cJSON *snssai_list_local_nonprimitive;
    if (!cJSON_IsArray(snssai_list)){
        ogs_error("OpenAPI_network_slice_cond_parseFromJSON() failed [snssai_list]");
        goto end;
    }

    snssai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(snssai_list_local_nonprimitive, snssai_list ) {
        if (!cJSON_IsObject(snssai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_network_slice_cond_parseFromJSON() failed [snssai_list]");
            goto end;
        }
        OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local_nonprimitive);

        if (!snssai_listItem) {
            ogs_error("No snssai_listItem");
            OpenAPI_list_free(snssai_listList);
            goto end;
        }

        OpenAPI_list_add(snssai_listList, snssai_listItem);
    }

    cJSON *nsi_list = cJSON_GetObjectItemCaseSensitive(network_slice_condJSON, "nsiList");

    OpenAPI_list_t *nsi_listList;
    if (nsi_list) {
    cJSON *nsi_list_local;
    if (!cJSON_IsArray(nsi_list)) {
        ogs_error("OpenAPI_network_slice_cond_parseFromJSON() failed [nsi_list]");
        goto end;
    }
    nsi_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(nsi_list_local, nsi_list) {
    if (!cJSON_IsString(nsi_list_local)) {
        ogs_error("OpenAPI_network_slice_cond_parseFromJSON() failed [nsi_list]");
        goto end;
    }
    OpenAPI_list_add(nsi_listList , ogs_strdup(nsi_list_local->valuestring));
    }
    }

    network_slice_cond_local_var = OpenAPI_network_slice_cond_create (
        snssai_listList,
        nsi_list ? nsi_listList : NULL
    );

    return network_slice_cond_local_var;
end:
    return NULL;
}

OpenAPI_network_slice_cond_t *OpenAPI_network_slice_cond_copy(OpenAPI_network_slice_cond_t *dst, OpenAPI_network_slice_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_slice_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_slice_cond_convertToJSON() failed");
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

    OpenAPI_network_slice_cond_free(dst);
    dst = OpenAPI_network_slice_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

