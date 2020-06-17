
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "chf_info.h"

OpenAPI_chf_info_t *OpenAPI_chf_info_create(
    OpenAPI_list_t *supi_range_list,
    OpenAPI_list_t *gpsi_range_list,
    OpenAPI_list_t *plmn_range_list,
    char *group_id
    )
{
    OpenAPI_chf_info_t *chf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_chf_info_t));
    if (!chf_info_local_var) {
        return NULL;
    }
    chf_info_local_var->supi_range_list = supi_range_list;
    chf_info_local_var->gpsi_range_list = gpsi_range_list;
    chf_info_local_var->plmn_range_list = plmn_range_list;
    chf_info_local_var->group_id = group_id;

    return chf_info_local_var;
}

void OpenAPI_chf_info_free(OpenAPI_chf_info_t *chf_info)
{
    if (NULL == chf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(chf_info->supi_range_list, node) {
        OpenAPI_supi_range_free(node->data);
    }
    OpenAPI_list_free(chf_info->supi_range_list);
    OpenAPI_list_for_each(chf_info->gpsi_range_list, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(chf_info->gpsi_range_list);
    OpenAPI_list_for_each(chf_info->plmn_range_list, node) {
        OpenAPI_plmn_range_free(node->data);
    }
    OpenAPI_list_free(chf_info->plmn_range_list);
    ogs_free(chf_info->group_id);
    ogs_free(chf_info);
}

cJSON *OpenAPI_chf_info_convertToJSON(OpenAPI_chf_info_t *chf_info)
{
    cJSON *item = NULL;

    if (chf_info == NULL) {
        ogs_error("OpenAPI_chf_info_convertToJSON() failed [ChfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (chf_info->supi_range_list) {
        cJSON *supi_range_listList = cJSON_AddArrayToObject(item, "supiRangeList");
        if (supi_range_listList == NULL) {
            ogs_error("OpenAPI_chf_info_convertToJSON() failed [supi_range_list]");
            goto end;
        }

        OpenAPI_lnode_t *supi_range_list_node;
        if (chf_info->supi_range_list) {
            OpenAPI_list_for_each(chf_info->supi_range_list, supi_range_list_node) {
                cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(supi_range_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_chf_info_convertToJSON() failed [supi_range_list]");
                    goto end;
                }
                cJSON_AddItemToArray(supi_range_listList, itemLocal);
            }
        }
    }

    if (chf_info->gpsi_range_list) {
        cJSON *gpsi_range_listList = cJSON_AddArrayToObject(item, "gpsiRangeList");
        if (gpsi_range_listList == NULL) {
            ogs_error("OpenAPI_chf_info_convertToJSON() failed [gpsi_range_list]");
            goto end;
        }

        OpenAPI_lnode_t *gpsi_range_list_node;
        if (chf_info->gpsi_range_list) {
            OpenAPI_list_for_each(chf_info->gpsi_range_list, gpsi_range_list_node) {
                cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(gpsi_range_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_chf_info_convertToJSON() failed [gpsi_range_list]");
                    goto end;
                }
                cJSON_AddItemToArray(gpsi_range_listList, itemLocal);
            }
        }
    }

    if (chf_info->plmn_range_list) {
        cJSON *plmn_range_listList = cJSON_AddArrayToObject(item, "plmnRangeList");
        if (plmn_range_listList == NULL) {
            ogs_error("OpenAPI_chf_info_convertToJSON() failed [plmn_range_list]");
            goto end;
        }

        OpenAPI_lnode_t *plmn_range_list_node;
        if (chf_info->plmn_range_list) {
            OpenAPI_list_for_each(chf_info->plmn_range_list, plmn_range_list_node) {
                cJSON *itemLocal = OpenAPI_plmn_range_convertToJSON(plmn_range_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_chf_info_convertToJSON() failed [plmn_range_list]");
                    goto end;
                }
                cJSON_AddItemToArray(plmn_range_listList, itemLocal);
            }
        }
    }

    if (chf_info->group_id) {
        if (cJSON_AddStringToObject(item, "groupId", chf_info->group_id) == NULL) {
            ogs_error("OpenAPI_chf_info_convertToJSON() failed [group_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_chf_info_t *OpenAPI_chf_info_parseFromJSON(cJSON *chf_infoJSON)
{
    OpenAPI_chf_info_t *chf_info_local_var = NULL;
    cJSON *supi_range_list = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "supiRangeList");

    OpenAPI_list_t *supi_range_listList;
    if (supi_range_list) {
        cJSON *supi_range_list_local_nonprimitive;
        if (!cJSON_IsArray(supi_range_list)) {
            ogs_error("OpenAPI_chf_info_parseFromJSON() failed [supi_range_list]");
            goto end;
        }

        supi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_range_list_local_nonprimitive, supi_range_list ) {
            if (!cJSON_IsObject(supi_range_list_local_nonprimitive)) {
                ogs_error("OpenAPI_chf_info_parseFromJSON() failed [supi_range_list]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_range_listItem = OpenAPI_supi_range_parseFromJSON(supi_range_list_local_nonprimitive);

            OpenAPI_list_add(supi_range_listList, supi_range_listItem);
        }
    }

    cJSON *gpsi_range_list = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "gpsiRangeList");

    OpenAPI_list_t *gpsi_range_listList;
    if (gpsi_range_list) {
        cJSON *gpsi_range_list_local_nonprimitive;
        if (!cJSON_IsArray(gpsi_range_list)) {
            ogs_error("OpenAPI_chf_info_parseFromJSON() failed [gpsi_range_list]");
            goto end;
        }

        gpsi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_range_list_local_nonprimitive, gpsi_range_list ) {
            if (!cJSON_IsObject(gpsi_range_list_local_nonprimitive)) {
                ogs_error("OpenAPI_chf_info_parseFromJSON() failed [gpsi_range_list]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_range_listItem = OpenAPI_identity_range_parseFromJSON(gpsi_range_list_local_nonprimitive);

            OpenAPI_list_add(gpsi_range_listList, gpsi_range_listItem);
        }
    }

    cJSON *plmn_range_list = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "plmnRangeList");

    OpenAPI_list_t *plmn_range_listList;
    if (plmn_range_list) {
        cJSON *plmn_range_list_local_nonprimitive;
        if (!cJSON_IsArray(plmn_range_list)) {
            ogs_error("OpenAPI_chf_info_parseFromJSON() failed [plmn_range_list]");
            goto end;
        }

        plmn_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_range_list_local_nonprimitive, plmn_range_list ) {
            if (!cJSON_IsObject(plmn_range_list_local_nonprimitive)) {
                ogs_error("OpenAPI_chf_info_parseFromJSON() failed [plmn_range_list]");
                goto end;
            }
            OpenAPI_plmn_range_t *plmn_range_listItem = OpenAPI_plmn_range_parseFromJSON(plmn_range_list_local_nonprimitive);

            OpenAPI_list_add(plmn_range_listList, plmn_range_listItem);
        }
    }

    cJSON *group_id = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "groupId");

    if (group_id) {
        if (!cJSON_IsString(group_id)) {
            ogs_error("OpenAPI_chf_info_parseFromJSON() failed [group_id]");
            goto end;
        }
    }

    chf_info_local_var = OpenAPI_chf_info_create (
        supi_range_list ? supi_range_listList : NULL,
        gpsi_range_list ? gpsi_range_listList : NULL,
        plmn_range_list ? plmn_range_listList : NULL,
        group_id ? ogs_strdup(group_id->valuestring) : NULL
        );

    return chf_info_local_var;
end:
    return NULL;
}

OpenAPI_chf_info_t *OpenAPI_chf_info_copy(OpenAPI_chf_info_t *dst, OpenAPI_chf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_chf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_chf_info_convertToJSON() failed");
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

    OpenAPI_chf_info_free(dst);
    dst = OpenAPI_chf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

