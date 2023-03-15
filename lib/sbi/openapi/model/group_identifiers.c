
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "group_identifiers.h"

OpenAPI_group_identifiers_t *OpenAPI_group_identifiers_create(
    char *ext_group_id,
    char *int_group_id,
    OpenAPI_list_t *ue_id_list
)
{
    OpenAPI_group_identifiers_t *group_identifiers_local_var = ogs_malloc(sizeof(OpenAPI_group_identifiers_t));
    ogs_assert(group_identifiers_local_var);

    group_identifiers_local_var->ext_group_id = ext_group_id;
    group_identifiers_local_var->int_group_id = int_group_id;
    group_identifiers_local_var->ue_id_list = ue_id_list;

    return group_identifiers_local_var;
}

void OpenAPI_group_identifiers_free(OpenAPI_group_identifiers_t *group_identifiers)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == group_identifiers) {
        return;
    }
    if (group_identifiers->ext_group_id) {
        ogs_free(group_identifiers->ext_group_id);
        group_identifiers->ext_group_id = NULL;
    }
    if (group_identifiers->int_group_id) {
        ogs_free(group_identifiers->int_group_id);
        group_identifiers->int_group_id = NULL;
    }
    if (group_identifiers->ue_id_list) {
        OpenAPI_list_for_each(group_identifiers->ue_id_list, node) {
            OpenAPI_ue_id_free(node->data);
        }
        OpenAPI_list_free(group_identifiers->ue_id_list);
        group_identifiers->ue_id_list = NULL;
    }
    ogs_free(group_identifiers);
}

cJSON *OpenAPI_group_identifiers_convertToJSON(OpenAPI_group_identifiers_t *group_identifiers)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (group_identifiers == NULL) {
        ogs_error("OpenAPI_group_identifiers_convertToJSON() failed [GroupIdentifiers]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (group_identifiers->ext_group_id) {
    if (cJSON_AddStringToObject(item, "extGroupId", group_identifiers->ext_group_id) == NULL) {
        ogs_error("OpenAPI_group_identifiers_convertToJSON() failed [ext_group_id]");
        goto end;
    }
    }

    if (group_identifiers->int_group_id) {
    if (cJSON_AddStringToObject(item, "intGroupId", group_identifiers->int_group_id) == NULL) {
        ogs_error("OpenAPI_group_identifiers_convertToJSON() failed [int_group_id]");
        goto end;
    }
    }

    if (group_identifiers->ue_id_list) {
    cJSON *ue_id_listList = cJSON_AddArrayToObject(item, "ueIdList");
    if (ue_id_listList == NULL) {
        ogs_error("OpenAPI_group_identifiers_convertToJSON() failed [ue_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(group_identifiers->ue_id_list, node) {
        cJSON *itemLocal = OpenAPI_ue_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_group_identifiers_convertToJSON() failed [ue_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(ue_id_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_group_identifiers_t *OpenAPI_group_identifiers_parseFromJSON(cJSON *group_identifiersJSON)
{
    OpenAPI_group_identifiers_t *group_identifiers_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ext_group_id = NULL;
    cJSON *int_group_id = NULL;
    cJSON *ue_id_list = NULL;
    OpenAPI_list_t *ue_id_listList = NULL;
    ext_group_id = cJSON_GetObjectItemCaseSensitive(group_identifiersJSON, "extGroupId");
    if (ext_group_id) {
    if (!cJSON_IsString(ext_group_id) && !cJSON_IsNull(ext_group_id)) {
        ogs_error("OpenAPI_group_identifiers_parseFromJSON() failed [ext_group_id]");
        goto end;
    }
    }

    int_group_id = cJSON_GetObjectItemCaseSensitive(group_identifiersJSON, "intGroupId");
    if (int_group_id) {
    if (!cJSON_IsString(int_group_id) && !cJSON_IsNull(int_group_id)) {
        ogs_error("OpenAPI_group_identifiers_parseFromJSON() failed [int_group_id]");
        goto end;
    }
    }

    ue_id_list = cJSON_GetObjectItemCaseSensitive(group_identifiersJSON, "ueIdList");
    if (ue_id_list) {
        cJSON *ue_id_list_local = NULL;
        if (!cJSON_IsArray(ue_id_list)) {
            ogs_error("OpenAPI_group_identifiers_parseFromJSON() failed [ue_id_list]");
            goto end;
        }

        ue_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_id_list_local, ue_id_list) {
            if (!cJSON_IsObject(ue_id_list_local)) {
                ogs_error("OpenAPI_group_identifiers_parseFromJSON() failed [ue_id_list]");
                goto end;
            }
            OpenAPI_ue_id_t *ue_id_listItem = OpenAPI_ue_id_parseFromJSON(ue_id_list_local);
            if (!ue_id_listItem) {
                ogs_error("No ue_id_listItem");
                goto end;
            }
            OpenAPI_list_add(ue_id_listList, ue_id_listItem);
        }
    }

    group_identifiers_local_var = OpenAPI_group_identifiers_create (
        ext_group_id && !cJSON_IsNull(ext_group_id) ? ogs_strdup(ext_group_id->valuestring) : NULL,
        int_group_id && !cJSON_IsNull(int_group_id) ? ogs_strdup(int_group_id->valuestring) : NULL,
        ue_id_list ? ue_id_listList : NULL
    );

    return group_identifiers_local_var;
end:
    if (ue_id_listList) {
        OpenAPI_list_for_each(ue_id_listList, node) {
            OpenAPI_ue_id_free(node->data);
        }
        OpenAPI_list_free(ue_id_listList);
        ue_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_group_identifiers_t *OpenAPI_group_identifiers_copy(OpenAPI_group_identifiers_t *dst, OpenAPI_group_identifiers_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_group_identifiers_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_group_identifiers_convertToJSON() failed");
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

    OpenAPI_group_identifiers_free(dst);
    dst = OpenAPI_group_identifiers_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

