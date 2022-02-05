
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_info.h"

OpenAPI_amf_info_t *OpenAPI_amf_info_create(
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *backup_info_amf_failure,
    OpenAPI_list_t *backup_info_amf_removal,
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info
)
{
    OpenAPI_amf_info_t *amf_info_local_var = ogs_malloc(sizeof(OpenAPI_amf_info_t));
    ogs_assert(amf_info_local_var);

    amf_info_local_var->amf_set_id = amf_set_id;
    amf_info_local_var->amf_region_id = amf_region_id;
    amf_info_local_var->guami_list = guami_list;
    amf_info_local_var->tai_list = tai_list;
    amf_info_local_var->tai_range_list = tai_range_list;
    amf_info_local_var->backup_info_amf_failure = backup_info_amf_failure;
    amf_info_local_var->backup_info_amf_removal = backup_info_amf_removal;
    amf_info_local_var->n2_interface_amf_info = n2_interface_amf_info;

    return amf_info_local_var;
}

void OpenAPI_amf_info_free(OpenAPI_amf_info_t *amf_info)
{
    if (NULL == amf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(amf_info->amf_set_id);
    ogs_free(amf_info->amf_region_id);
    OpenAPI_list_for_each(amf_info->guami_list, node) {
        OpenAPI_guami_free(node->data);
    }
    OpenAPI_list_free(amf_info->guami_list);
    OpenAPI_list_for_each(amf_info->tai_list, node) {
        OpenAPI_tai_free(node->data);
    }
    OpenAPI_list_free(amf_info->tai_list);
    OpenAPI_list_for_each(amf_info->tai_range_list, node) {
        OpenAPI_tai_range_free(node->data);
    }
    OpenAPI_list_free(amf_info->tai_range_list);
    OpenAPI_list_for_each(amf_info->backup_info_amf_failure, node) {
        OpenAPI_guami_free(node->data);
    }
    OpenAPI_list_free(amf_info->backup_info_amf_failure);
    OpenAPI_list_for_each(amf_info->backup_info_amf_removal, node) {
        OpenAPI_guami_free(node->data);
    }
    OpenAPI_list_free(amf_info->backup_info_amf_removal);
    OpenAPI_n2_interface_amf_info_free(amf_info->n2_interface_amf_info);
    ogs_free(amf_info);
}

cJSON *OpenAPI_amf_info_convertToJSON(OpenAPI_amf_info_t *amf_info)
{
    cJSON *item = NULL;

    if (amf_info == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [AmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "amfSetId", amf_info->amf_set_id) == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_set_id]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "amfRegionId", amf_info->amf_region_id) == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_region_id]");
        goto end;
    }

    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [guami_list]");
        goto end;
    }

    OpenAPI_lnode_t *guami_list_node;
    if (amf_info->guami_list) {
        OpenAPI_list_for_each(amf_info->guami_list, guami_list_node) {
            cJSON *itemLocal = OpenAPI_guami_convertToJSON(guami_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_info_convertToJSON() failed [guami_list]");
                goto end;
            }
            cJSON_AddItemToArray(guami_listList, itemLocal);
        }
    }

    if (amf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_list]");
        goto end;
    }

    OpenAPI_lnode_t *tai_list_node;
    if (amf_info->tai_list) {
        OpenAPI_list_for_each(amf_info->tai_list, tai_list_node) {
            cJSON *itemLocal = OpenAPI_tai_convertToJSON(tai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_list]");
                goto end;
            }
            cJSON_AddItemToArray(tai_listList, itemLocal);
        }
    }
    }

    if (amf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }

    OpenAPI_lnode_t *tai_range_list_node;
    if (amf_info->tai_range_list) {
        OpenAPI_list_for_each(amf_info->tai_range_list, tai_range_list_node) {
            cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(tai_range_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_range_list]");
                goto end;
            }
            cJSON_AddItemToArray(tai_range_listList, itemLocal);
        }
    }
    }

    if (amf_info->backup_info_amf_failure) {
    cJSON *backup_info_amf_failureList = cJSON_AddArrayToObject(item, "backupInfoAmfFailure");
    if (backup_info_amf_failureList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_failure]");
        goto end;
    }

    OpenAPI_lnode_t *backup_info_amf_failure_node;
    if (amf_info->backup_info_amf_failure) {
        OpenAPI_list_for_each(amf_info->backup_info_amf_failure, backup_info_amf_failure_node) {
            cJSON *itemLocal = OpenAPI_guami_convertToJSON(backup_info_amf_failure_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_failure]");
                goto end;
            }
            cJSON_AddItemToArray(backup_info_amf_failureList, itemLocal);
        }
    }
    }

    if (amf_info->backup_info_amf_removal) {
    cJSON *backup_info_amf_removalList = cJSON_AddArrayToObject(item, "backupInfoAmfRemoval");
    if (backup_info_amf_removalList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_removal]");
        goto end;
    }

    OpenAPI_lnode_t *backup_info_amf_removal_node;
    if (amf_info->backup_info_amf_removal) {
        OpenAPI_list_for_each(amf_info->backup_info_amf_removal, backup_info_amf_removal_node) {
            cJSON *itemLocal = OpenAPI_guami_convertToJSON(backup_info_amf_removal_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_removal]");
                goto end;
            }
            cJSON_AddItemToArray(backup_info_amf_removalList, itemLocal);
        }
    }
    }

    if (amf_info->n2_interface_amf_info) {
    cJSON *n2_interface_amf_info_local_JSON = OpenAPI_n2_interface_amf_info_convertToJSON(amf_info->n2_interface_amf_info);
    if (n2_interface_amf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [n2_interface_amf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2InterfaceAmfInfo", n2_interface_amf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [n2_interface_amf_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_info_t *OpenAPI_amf_info_parseFromJSON(cJSON *amf_infoJSON)
{
    OpenAPI_amf_info_t *amf_info_local_var = NULL;
    cJSON *amf_set_id = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "amfSetId");
    if (!amf_set_id) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_set_id]");
        goto end;
    }

    if (!cJSON_IsString(amf_set_id)) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_set_id]");
        goto end;
    }

    cJSON *amf_region_id = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "amfRegionId");
    if (!amf_region_id) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_region_id]");
        goto end;
    }

    if (!cJSON_IsString(amf_region_id)) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_region_id]");
        goto end;
    }

    cJSON *guami_list = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "guamiList");
    if (!guami_list) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [guami_list]");
        goto end;
    }

    OpenAPI_list_t *guami_listList;
    cJSON *guami_list_local_nonprimitive;
    if (!cJSON_IsArray(guami_list)){
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [guami_list]");
        goto end;
    }

    guami_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(guami_list_local_nonprimitive, guami_list ) {
        if (!cJSON_IsObject(guami_list_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [guami_list]");
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

    cJSON *tai_list = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "taiList");

    OpenAPI_list_t *tai_listList;
    if (tai_list) {
    cJSON *tai_list_local_nonprimitive;
    if (!cJSON_IsArray(tai_list)){
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_list]");
        goto end;
    }

    tai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(tai_list_local_nonprimitive, tai_list ) {
        if (!cJSON_IsObject(tai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }
        OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local_nonprimitive);

        if (!tai_listItem) {
            ogs_error("No tai_listItem");
            OpenAPI_list_free(tai_listList);
            goto end;
        }

        OpenAPI_list_add(tai_listList, tai_listItem);
    }
    }

    cJSON *tai_range_list = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "taiRangeList");

    OpenAPI_list_t *tai_range_listList;
    if (tai_range_list) {
    cJSON *tai_range_list_local_nonprimitive;
    if (!cJSON_IsArray(tai_range_list)){
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_range_list]");
        goto end;
    }

    tai_range_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(tai_range_list_local_nonprimitive, tai_range_list ) {
        if (!cJSON_IsObject(tai_range_list_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }
        OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local_nonprimitive);

        if (!tai_range_listItem) {
            ogs_error("No tai_range_listItem");
            OpenAPI_list_free(tai_range_listList);
            goto end;
        }

        OpenAPI_list_add(tai_range_listList, tai_range_listItem);
    }
    }

    cJSON *backup_info_amf_failure = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "backupInfoAmfFailure");

    OpenAPI_list_t *backup_info_amf_failureList;
    if (backup_info_amf_failure) {
    cJSON *backup_info_amf_failure_local_nonprimitive;
    if (!cJSON_IsArray(backup_info_amf_failure)){
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_failure]");
        goto end;
    }

    backup_info_amf_failureList = OpenAPI_list_create();

    cJSON_ArrayForEach(backup_info_amf_failure_local_nonprimitive, backup_info_amf_failure ) {
        if (!cJSON_IsObject(backup_info_amf_failure_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_failure]");
            goto end;
        }
        OpenAPI_guami_t *backup_info_amf_failureItem = OpenAPI_guami_parseFromJSON(backup_info_amf_failure_local_nonprimitive);

        if (!backup_info_amf_failureItem) {
            ogs_error("No backup_info_amf_failureItem");
            OpenAPI_list_free(backup_info_amf_failureList);
            goto end;
        }

        OpenAPI_list_add(backup_info_amf_failureList, backup_info_amf_failureItem);
    }
    }

    cJSON *backup_info_amf_removal = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "backupInfoAmfRemoval");

    OpenAPI_list_t *backup_info_amf_removalList;
    if (backup_info_amf_removal) {
    cJSON *backup_info_amf_removal_local_nonprimitive;
    if (!cJSON_IsArray(backup_info_amf_removal)){
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_removal]");
        goto end;
    }

    backup_info_amf_removalList = OpenAPI_list_create();

    cJSON_ArrayForEach(backup_info_amf_removal_local_nonprimitive, backup_info_amf_removal ) {
        if (!cJSON_IsObject(backup_info_amf_removal_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_removal]");
            goto end;
        }
        OpenAPI_guami_t *backup_info_amf_removalItem = OpenAPI_guami_parseFromJSON(backup_info_amf_removal_local_nonprimitive);

        if (!backup_info_amf_removalItem) {
            ogs_error("No backup_info_amf_removalItem");
            OpenAPI_list_free(backup_info_amf_removalList);
            goto end;
        }

        OpenAPI_list_add(backup_info_amf_removalList, backup_info_amf_removalItem);
    }
    }

    cJSON *n2_interface_amf_info = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "n2InterfaceAmfInfo");

    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info_local_nonprim = NULL;
    if (n2_interface_amf_info) {
    n2_interface_amf_info_local_nonprim = OpenAPI_n2_interface_amf_info_parseFromJSON(n2_interface_amf_info);
    }

    amf_info_local_var = OpenAPI_amf_info_create (
        ogs_strdup(amf_set_id->valuestring),
        ogs_strdup(amf_region_id->valuestring),
        guami_listList,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        backup_info_amf_failure ? backup_info_amf_failureList : NULL,
        backup_info_amf_removal ? backup_info_amf_removalList : NULL,
        n2_interface_amf_info ? n2_interface_amf_info_local_nonprim : NULL
    );

    return amf_info_local_var;
end:
    return NULL;
}

OpenAPI_amf_info_t *OpenAPI_amf_info_copy(OpenAPI_amf_info_t *dst, OpenAPI_amf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed");
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

    OpenAPI_amf_info_free(dst);
    dst = OpenAPI_amf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

