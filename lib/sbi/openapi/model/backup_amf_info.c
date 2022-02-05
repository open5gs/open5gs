
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "backup_amf_info.h"

OpenAPI_backup_amf_info_t *OpenAPI_backup_amf_info_create(
    char *backup_amf,
    OpenAPI_list_t *guami_list
)
{
    OpenAPI_backup_amf_info_t *backup_amf_info_local_var = ogs_malloc(sizeof(OpenAPI_backup_amf_info_t));
    ogs_assert(backup_amf_info_local_var);

    backup_amf_info_local_var->backup_amf = backup_amf;
    backup_amf_info_local_var->guami_list = guami_list;

    return backup_amf_info_local_var;
}

void OpenAPI_backup_amf_info_free(OpenAPI_backup_amf_info_t *backup_amf_info)
{
    if (NULL == backup_amf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(backup_amf_info->backup_amf);
    OpenAPI_list_for_each(backup_amf_info->guami_list, node) {
        OpenAPI_guami_free(node->data);
    }
    OpenAPI_list_free(backup_amf_info->guami_list);
    ogs_free(backup_amf_info);
}

cJSON *OpenAPI_backup_amf_info_convertToJSON(OpenAPI_backup_amf_info_t *backup_amf_info)
{
    cJSON *item = NULL;

    if (backup_amf_info == NULL) {
        ogs_error("OpenAPI_backup_amf_info_convertToJSON() failed [BackupAmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "backupAmf", backup_amf_info->backup_amf) == NULL) {
        ogs_error("OpenAPI_backup_amf_info_convertToJSON() failed [backup_amf]");
        goto end;
    }

    if (backup_amf_info->guami_list) {
    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_backup_amf_info_convertToJSON() failed [guami_list]");
        goto end;
    }

    OpenAPI_lnode_t *guami_list_node;
    if (backup_amf_info->guami_list) {
        OpenAPI_list_for_each(backup_amf_info->guami_list, guami_list_node) {
            cJSON *itemLocal = OpenAPI_guami_convertToJSON(guami_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_backup_amf_info_convertToJSON() failed [guami_list]");
                goto end;
            }
            cJSON_AddItemToArray(guami_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_backup_amf_info_t *OpenAPI_backup_amf_info_parseFromJSON(cJSON *backup_amf_infoJSON)
{
    OpenAPI_backup_amf_info_t *backup_amf_info_local_var = NULL;
    cJSON *backup_amf = cJSON_GetObjectItemCaseSensitive(backup_amf_infoJSON, "backupAmf");
    if (!backup_amf) {
        ogs_error("OpenAPI_backup_amf_info_parseFromJSON() failed [backup_amf]");
        goto end;
    }

    if (!cJSON_IsString(backup_amf)) {
        ogs_error("OpenAPI_backup_amf_info_parseFromJSON() failed [backup_amf]");
        goto end;
    }

    cJSON *guami_list = cJSON_GetObjectItemCaseSensitive(backup_amf_infoJSON, "guamiList");

    OpenAPI_list_t *guami_listList;
    if (guami_list) {
    cJSON *guami_list_local_nonprimitive;
    if (!cJSON_IsArray(guami_list)){
        ogs_error("OpenAPI_backup_amf_info_parseFromJSON() failed [guami_list]");
        goto end;
    }

    guami_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(guami_list_local_nonprimitive, guami_list ) {
        if (!cJSON_IsObject(guami_list_local_nonprimitive)) {
            ogs_error("OpenAPI_backup_amf_info_parseFromJSON() failed [guami_list]");
            goto end;
        }
        OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local_nonprimitive);

        if (!guami_listItem) {
            ogs_error("No guami_listItem");
            OpenAPI_list_free(guami_listList);
            goto end;
        }

        OpenAPI_list_add(guami_listList, guami_listItem);
    }
    }

    backup_amf_info_local_var = OpenAPI_backup_amf_info_create (
        ogs_strdup(backup_amf->valuestring),
        guami_list ? guami_listList : NULL
    );

    return backup_amf_info_local_var;
end:
    return NULL;
}

OpenAPI_backup_amf_info_t *OpenAPI_backup_amf_info_copy(OpenAPI_backup_amf_info_t *dst, OpenAPI_backup_amf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_backup_amf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_backup_amf_info_convertToJSON() failed");
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

    OpenAPI_backup_amf_info_free(dst);
    dst = OpenAPI_backup_amf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

