
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "chf_info.h"

OpenAPI_chf_info_t *OpenAPI_chf_info_create(
    OpenAPI_list_t *supi_range_list,
    OpenAPI_list_t *gpsi_range_list,
    OpenAPI_list_t *plmn_range_list,
    char *group_id,
    char *primary_chf_instance,
    char *secondary_chf_instance
)
{
    OpenAPI_chf_info_t *chf_info_local_var = ogs_malloc(sizeof(OpenAPI_chf_info_t));
    ogs_assert(chf_info_local_var);

    chf_info_local_var->supi_range_list = supi_range_list;
    chf_info_local_var->gpsi_range_list = gpsi_range_list;
    chf_info_local_var->plmn_range_list = plmn_range_list;
    chf_info_local_var->group_id = group_id;
    chf_info_local_var->primary_chf_instance = primary_chf_instance;
    chf_info_local_var->secondary_chf_instance = secondary_chf_instance;

    return chf_info_local_var;
}

void OpenAPI_chf_info_free(OpenAPI_chf_info_t *chf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == chf_info) {
        return;
    }
    if (chf_info->supi_range_list) {
        OpenAPI_list_for_each(chf_info->supi_range_list, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(chf_info->supi_range_list);
        chf_info->supi_range_list = NULL;
    }
    if (chf_info->gpsi_range_list) {
        OpenAPI_list_for_each(chf_info->gpsi_range_list, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(chf_info->gpsi_range_list);
        chf_info->gpsi_range_list = NULL;
    }
    if (chf_info->plmn_range_list) {
        OpenAPI_list_for_each(chf_info->plmn_range_list, node) {
            OpenAPI_plmn_range_free(node->data);
        }
        OpenAPI_list_free(chf_info->plmn_range_list);
        chf_info->plmn_range_list = NULL;
    }
    if (chf_info->group_id) {
        ogs_free(chf_info->group_id);
        chf_info->group_id = NULL;
    }
    if (chf_info->primary_chf_instance) {
        ogs_free(chf_info->primary_chf_instance);
        chf_info->primary_chf_instance = NULL;
    }
    if (chf_info->secondary_chf_instance) {
        ogs_free(chf_info->secondary_chf_instance);
        chf_info->secondary_chf_instance = NULL;
    }
    ogs_free(chf_info);
}

cJSON *OpenAPI_chf_info_convertToJSON(OpenAPI_chf_info_t *chf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(chf_info->supi_range_list, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_chf_info_convertToJSON() failed [supi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(supi_range_listList, itemLocal);
    }
    }

    if (chf_info->gpsi_range_list) {
    cJSON *gpsi_range_listList = cJSON_AddArrayToObject(item, "gpsiRangeList");
    if (gpsi_range_listList == NULL) {
        ogs_error("OpenAPI_chf_info_convertToJSON() failed [gpsi_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(chf_info->gpsi_range_list, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_chf_info_convertToJSON() failed [gpsi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_range_listList, itemLocal);
    }
    }

    if (chf_info->plmn_range_list) {
    cJSON *plmn_range_listList = cJSON_AddArrayToObject(item, "plmnRangeList");
    if (plmn_range_listList == NULL) {
        ogs_error("OpenAPI_chf_info_convertToJSON() failed [plmn_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(chf_info->plmn_range_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_chf_info_convertToJSON() failed [plmn_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(plmn_range_listList, itemLocal);
    }
    }

    if (chf_info->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", chf_info->group_id) == NULL) {
        ogs_error("OpenAPI_chf_info_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (chf_info->primary_chf_instance) {
    if (cJSON_AddStringToObject(item, "primaryChfInstance", chf_info->primary_chf_instance) == NULL) {
        ogs_error("OpenAPI_chf_info_convertToJSON() failed [primary_chf_instance]");
        goto end;
    }
    }

    if (chf_info->secondary_chf_instance) {
    if (cJSON_AddStringToObject(item, "secondaryChfInstance", chf_info->secondary_chf_instance) == NULL) {
        ogs_error("OpenAPI_chf_info_convertToJSON() failed [secondary_chf_instance]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_chf_info_t *OpenAPI_chf_info_parseFromJSON(cJSON *chf_infoJSON)
{
    OpenAPI_chf_info_t *chf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi_range_list = NULL;
    OpenAPI_list_t *supi_range_listList = NULL;
    cJSON *gpsi_range_list = NULL;
    OpenAPI_list_t *gpsi_range_listList = NULL;
    cJSON *plmn_range_list = NULL;
    OpenAPI_list_t *plmn_range_listList = NULL;
    cJSON *group_id = NULL;
    cJSON *primary_chf_instance = NULL;
    cJSON *secondary_chf_instance = NULL;
    supi_range_list = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "supiRangeList");
    if (supi_range_list) {
        cJSON *supi_range_list_local = NULL;
        if (!cJSON_IsArray(supi_range_list)) {
            ogs_error("OpenAPI_chf_info_parseFromJSON() failed [supi_range_list]");
            goto end;
        }

        supi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_range_list_local, supi_range_list) {
            if (!cJSON_IsObject(supi_range_list_local)) {
                ogs_error("OpenAPI_chf_info_parseFromJSON() failed [supi_range_list]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_range_listItem = OpenAPI_supi_range_parseFromJSON(supi_range_list_local);
            if (!supi_range_listItem) {
                ogs_error("No supi_range_listItem");
                goto end;
            }
            OpenAPI_list_add(supi_range_listList, supi_range_listItem);
        }
    }

    gpsi_range_list = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "gpsiRangeList");
    if (gpsi_range_list) {
        cJSON *gpsi_range_list_local = NULL;
        if (!cJSON_IsArray(gpsi_range_list)) {
            ogs_error("OpenAPI_chf_info_parseFromJSON() failed [gpsi_range_list]");
            goto end;
        }

        gpsi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_range_list_local, gpsi_range_list) {
            if (!cJSON_IsObject(gpsi_range_list_local)) {
                ogs_error("OpenAPI_chf_info_parseFromJSON() failed [gpsi_range_list]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_range_listItem = OpenAPI_identity_range_parseFromJSON(gpsi_range_list_local);
            if (!gpsi_range_listItem) {
                ogs_error("No gpsi_range_listItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_range_listList, gpsi_range_listItem);
        }
    }

    plmn_range_list = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "plmnRangeList");
    if (plmn_range_list) {
        cJSON *plmn_range_list_local = NULL;
        if (!cJSON_IsArray(plmn_range_list)) {
            ogs_error("OpenAPI_chf_info_parseFromJSON() failed [plmn_range_list]");
            goto end;
        }

        plmn_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_range_list_local, plmn_range_list) {
            if (!cJSON_IsObject(plmn_range_list_local)) {
                ogs_error("OpenAPI_chf_info_parseFromJSON() failed [plmn_range_list]");
                goto end;
            }
            OpenAPI_plmn_range_t *plmn_range_listItem = OpenAPI_plmn_range_parseFromJSON(plmn_range_list_local);
            if (!plmn_range_listItem) {
                ogs_error("No plmn_range_listItem");
                goto end;
            }
            OpenAPI_list_add(plmn_range_listList, plmn_range_listItem);
        }
    }

    group_id = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_chf_info_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    primary_chf_instance = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "primaryChfInstance");
    if (primary_chf_instance) {
    if (!cJSON_IsString(primary_chf_instance) && !cJSON_IsNull(primary_chf_instance)) {
        ogs_error("OpenAPI_chf_info_parseFromJSON() failed [primary_chf_instance]");
        goto end;
    }
    }

    secondary_chf_instance = cJSON_GetObjectItemCaseSensitive(chf_infoJSON, "secondaryChfInstance");
    if (secondary_chf_instance) {
    if (!cJSON_IsString(secondary_chf_instance) && !cJSON_IsNull(secondary_chf_instance)) {
        ogs_error("OpenAPI_chf_info_parseFromJSON() failed [secondary_chf_instance]");
        goto end;
    }
    }

    chf_info_local_var = OpenAPI_chf_info_create (
        supi_range_list ? supi_range_listList : NULL,
        gpsi_range_list ? gpsi_range_listList : NULL,
        plmn_range_list ? plmn_range_listList : NULL,
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        primary_chf_instance && !cJSON_IsNull(primary_chf_instance) ? ogs_strdup(primary_chf_instance->valuestring) : NULL,
        secondary_chf_instance && !cJSON_IsNull(secondary_chf_instance) ? ogs_strdup(secondary_chf_instance->valuestring) : NULL
    );

    return chf_info_local_var;
end:
    if (supi_range_listList) {
        OpenAPI_list_for_each(supi_range_listList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_range_listList);
        supi_range_listList = NULL;
    }
    if (gpsi_range_listList) {
        OpenAPI_list_for_each(gpsi_range_listList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_range_listList);
        gpsi_range_listList = NULL;
    }
    if (plmn_range_listList) {
        OpenAPI_list_for_each(plmn_range_listList, node) {
            OpenAPI_plmn_range_free(node->data);
        }
        OpenAPI_list_free(plmn_range_listList);
        plmn_range_listList = NULL;
    }
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

