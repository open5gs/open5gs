
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "established_connection_info.h"

OpenAPI_established_connection_info_t *OpenAPI_established_connection_info_create(
    OpenAPI_list_t *u_pfids,
    OpenAPI_list_t *ran_node_ids
)
{
    OpenAPI_established_connection_info_t *established_connection_info_local_var = ogs_malloc(sizeof(OpenAPI_established_connection_info_t));
    ogs_assert(established_connection_info_local_var);

    established_connection_info_local_var->u_pfids = u_pfids;
    established_connection_info_local_var->ran_node_ids = ran_node_ids;

    return established_connection_info_local_var;
}

void OpenAPI_established_connection_info_free(OpenAPI_established_connection_info_t *established_connection_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == established_connection_info) {
        return;
    }
    if (established_connection_info->u_pfids) {
        OpenAPI_list_for_each(established_connection_info->u_pfids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(established_connection_info->u_pfids);
        established_connection_info->u_pfids = NULL;
    }
    if (established_connection_info->ran_node_ids) {
        OpenAPI_list_for_each(established_connection_info->ran_node_ids, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(established_connection_info->ran_node_ids);
        established_connection_info->ran_node_ids = NULL;
    }
    ogs_free(established_connection_info);
}

cJSON *OpenAPI_established_connection_info_convertToJSON(OpenAPI_established_connection_info_t *established_connection_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (established_connection_info == NULL) {
        ogs_error("OpenAPI_established_connection_info_convertToJSON() failed [EstablishedConnectionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (established_connection_info->u_pfids) {
    cJSON *u_pfidsList = cJSON_AddArrayToObject(item, "uPFIDs");
    if (u_pfidsList == NULL) {
        ogs_error("OpenAPI_established_connection_info_convertToJSON() failed [u_pfids]");
        goto end;
    }
    OpenAPI_list_for_each(established_connection_info->u_pfids, node) {
        if (cJSON_AddStringToObject(u_pfidsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_established_connection_info_convertToJSON() failed [u_pfids]");
            goto end;
        }
    }
    }

    if (established_connection_info->ran_node_ids) {
    cJSON *ran_node_idsList = cJSON_AddArrayToObject(item, "ranNodeIDs");
    if (ran_node_idsList == NULL) {
        ogs_error("OpenAPI_established_connection_info_convertToJSON() failed [ran_node_ids]");
        goto end;
    }
    OpenAPI_list_for_each(established_connection_info->ran_node_ids, node) {
        cJSON *itemLocal = OpenAPI_global_ran_node_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_established_connection_info_convertToJSON() failed [ran_node_ids]");
            goto end;
        }
        cJSON_AddItemToArray(ran_node_idsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_established_connection_info_t *OpenAPI_established_connection_info_parseFromJSON(cJSON *established_connection_infoJSON)
{
    OpenAPI_established_connection_info_t *established_connection_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *u_pfids = NULL;
    OpenAPI_list_t *u_pfidsList = NULL;
    cJSON *ran_node_ids = NULL;
    OpenAPI_list_t *ran_node_idsList = NULL;
    u_pfids = cJSON_GetObjectItemCaseSensitive(established_connection_infoJSON, "uPFIDs");
    if (u_pfids) {
        cJSON *u_pfids_local = NULL;
        if (!cJSON_IsArray(u_pfids)) {
            ogs_error("OpenAPI_established_connection_info_parseFromJSON() failed [u_pfids]");
            goto end;
        }

        u_pfidsList = OpenAPI_list_create();

        cJSON_ArrayForEach(u_pfids_local, u_pfids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(u_pfids_local)) {
                ogs_error("OpenAPI_established_connection_info_parseFromJSON() failed [u_pfids]");
                goto end;
            }
            OpenAPI_list_add(u_pfidsList, ogs_strdup(u_pfids_local->valuestring));
        }
    }

    ran_node_ids = cJSON_GetObjectItemCaseSensitive(established_connection_infoJSON, "ranNodeIDs");
    if (ran_node_ids) {
        cJSON *ran_node_ids_local = NULL;
        if (!cJSON_IsArray(ran_node_ids)) {
            ogs_error("OpenAPI_established_connection_info_parseFromJSON() failed [ran_node_ids]");
            goto end;
        }

        ran_node_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_node_ids_local, ran_node_ids) {
            if (!cJSON_IsObject(ran_node_ids_local)) {
                ogs_error("OpenAPI_established_connection_info_parseFromJSON() failed [ran_node_ids]");
                goto end;
            }
            OpenAPI_global_ran_node_id_t *ran_node_idsItem = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_ids_local);
            if (!ran_node_idsItem) {
                ogs_error("No ran_node_idsItem");
                goto end;
            }
            OpenAPI_list_add(ran_node_idsList, ran_node_idsItem);
        }
    }

    established_connection_info_local_var = OpenAPI_established_connection_info_create (
        u_pfids ? u_pfidsList : NULL,
        ran_node_ids ? ran_node_idsList : NULL
    );

    return established_connection_info_local_var;
end:
    if (u_pfidsList) {
        OpenAPI_list_for_each(u_pfidsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(u_pfidsList);
        u_pfidsList = NULL;
    }
    if (ran_node_idsList) {
        OpenAPI_list_for_each(ran_node_idsList, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(ran_node_idsList);
        ran_node_idsList = NULL;
    }
    return NULL;
}

OpenAPI_established_connection_info_t *OpenAPI_established_connection_info_copy(OpenAPI_established_connection_info_t *dst, OpenAPI_established_connection_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_established_connection_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_established_connection_info_convertToJSON() failed");
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

    OpenAPI_established_connection_info_free(dst);
    dst = OpenAPI_established_connection_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

