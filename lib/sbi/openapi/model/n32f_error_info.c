
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32f_error_info.h"

OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_create(
    char *n32f_message_id,
    OpenAPI_n32f_error_type_e n32f_error_type,
    char *n32f_context_id,
    OpenAPI_list_t *failed_modification_list,
    OpenAPI_list_t *error_details_list
)
{
    OpenAPI_n32f_error_info_t *n32f_error_info_local_var = ogs_malloc(sizeof(OpenAPI_n32f_error_info_t));
    ogs_assert(n32f_error_info_local_var);

    n32f_error_info_local_var->n32f_message_id = n32f_message_id;
    n32f_error_info_local_var->n32f_error_type = n32f_error_type;
    n32f_error_info_local_var->n32f_context_id = n32f_context_id;
    n32f_error_info_local_var->failed_modification_list = failed_modification_list;
    n32f_error_info_local_var->error_details_list = error_details_list;

    return n32f_error_info_local_var;
}

void OpenAPI_n32f_error_info_free(OpenAPI_n32f_error_info_t *n32f_error_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n32f_error_info) {
        return;
    }
    if (n32f_error_info->n32f_message_id) {
        ogs_free(n32f_error_info->n32f_message_id);
        n32f_error_info->n32f_message_id = NULL;
    }
    if (n32f_error_info->n32f_context_id) {
        ogs_free(n32f_error_info->n32f_context_id);
        n32f_error_info->n32f_context_id = NULL;
    }
    if (n32f_error_info->failed_modification_list) {
        OpenAPI_list_for_each(n32f_error_info->failed_modification_list, node) {
            OpenAPI_failed_modification_info_free(node->data);
        }
        OpenAPI_list_free(n32f_error_info->failed_modification_list);
        n32f_error_info->failed_modification_list = NULL;
    }
    if (n32f_error_info->error_details_list) {
        OpenAPI_list_for_each(n32f_error_info->error_details_list, node) {
            OpenAPI_n32f_error_detail_free(node->data);
        }
        OpenAPI_list_free(n32f_error_info->error_details_list);
        n32f_error_info->error_details_list = NULL;
    }
    ogs_free(n32f_error_info);
}

cJSON *OpenAPI_n32f_error_info_convertToJSON(OpenAPI_n32f_error_info_t *n32f_error_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n32f_error_info == NULL) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [N32fErrorInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n32f_error_info->n32f_message_id) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [n32f_message_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n32fMessageId", n32f_error_info->n32f_message_id) == NULL) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [n32f_message_id]");
        goto end;
    }

    if (n32f_error_info->n32f_error_type == OpenAPI_n32f_error_type_NULL) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [n32f_error_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n32fErrorType", OpenAPI_n32f_error_type_ToString(n32f_error_info->n32f_error_type)) == NULL) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [n32f_error_type]");
        goto end;
    }

    if (n32f_error_info->n32f_context_id) {
    if (cJSON_AddStringToObject(item, "n32fContextId", n32f_error_info->n32f_context_id) == NULL) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [n32f_context_id]");
        goto end;
    }
    }

    if (n32f_error_info->failed_modification_list) {
    cJSON *failed_modification_listList = cJSON_AddArrayToObject(item, "failedModificationList");
    if (failed_modification_listList == NULL) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [failed_modification_list]");
        goto end;
    }
    OpenAPI_list_for_each(n32f_error_info->failed_modification_list, node) {
        cJSON *itemLocal = OpenAPI_failed_modification_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [failed_modification_list]");
            goto end;
        }
        cJSON_AddItemToArray(failed_modification_listList, itemLocal);
    }
    }

    if (n32f_error_info->error_details_list) {
    cJSON *error_details_listList = cJSON_AddArrayToObject(item, "errorDetailsList");
    if (error_details_listList == NULL) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [error_details_list]");
        goto end;
    }
    OpenAPI_list_for_each(n32f_error_info->error_details_list, node) {
        cJSON *itemLocal = OpenAPI_n32f_error_detail_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed [error_details_list]");
            goto end;
        }
        cJSON_AddItemToArray(error_details_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_parseFromJSON(cJSON *n32f_error_infoJSON)
{
    OpenAPI_n32f_error_info_t *n32f_error_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n32f_message_id = NULL;
    cJSON *n32f_error_type = NULL;
    OpenAPI_n32f_error_type_e n32f_error_typeVariable = 0;
    cJSON *n32f_context_id = NULL;
    cJSON *failed_modification_list = NULL;
    OpenAPI_list_t *failed_modification_listList = NULL;
    cJSON *error_details_list = NULL;
    OpenAPI_list_t *error_details_listList = NULL;
    n32f_message_id = cJSON_GetObjectItemCaseSensitive(n32f_error_infoJSON, "n32fMessageId");
    if (!n32f_message_id) {
        ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [n32f_message_id]");
        goto end;
    }
    if (!cJSON_IsString(n32f_message_id)) {
        ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [n32f_message_id]");
        goto end;
    }

    n32f_error_type = cJSON_GetObjectItemCaseSensitive(n32f_error_infoJSON, "n32fErrorType");
    if (!n32f_error_type) {
        ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [n32f_error_type]");
        goto end;
    }
    if (!cJSON_IsString(n32f_error_type)) {
        ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [n32f_error_type]");
        goto end;
    }
    n32f_error_typeVariable = OpenAPI_n32f_error_type_FromString(n32f_error_type->valuestring);

    n32f_context_id = cJSON_GetObjectItemCaseSensitive(n32f_error_infoJSON, "n32fContextId");
    if (n32f_context_id) {
    if (!cJSON_IsString(n32f_context_id) && !cJSON_IsNull(n32f_context_id)) {
        ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [n32f_context_id]");
        goto end;
    }
    }

    failed_modification_list = cJSON_GetObjectItemCaseSensitive(n32f_error_infoJSON, "failedModificationList");
    if (failed_modification_list) {
        cJSON *failed_modification_list_local = NULL;
        if (!cJSON_IsArray(failed_modification_list)) {
            ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [failed_modification_list]");
            goto end;
        }

        failed_modification_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(failed_modification_list_local, failed_modification_list) {
            if (!cJSON_IsObject(failed_modification_list_local)) {
                ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [failed_modification_list]");
                goto end;
            }
            OpenAPI_failed_modification_info_t *failed_modification_listItem = OpenAPI_failed_modification_info_parseFromJSON(failed_modification_list_local);
            if (!failed_modification_listItem) {
                ogs_error("No failed_modification_listItem");
                goto end;
            }
            OpenAPI_list_add(failed_modification_listList, failed_modification_listItem);
        }
    }

    error_details_list = cJSON_GetObjectItemCaseSensitive(n32f_error_infoJSON, "errorDetailsList");
    if (error_details_list) {
        cJSON *error_details_list_local = NULL;
        if (!cJSON_IsArray(error_details_list)) {
            ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [error_details_list]");
            goto end;
        }

        error_details_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(error_details_list_local, error_details_list) {
            if (!cJSON_IsObject(error_details_list_local)) {
                ogs_error("OpenAPI_n32f_error_info_parseFromJSON() failed [error_details_list]");
                goto end;
            }
            OpenAPI_n32f_error_detail_t *error_details_listItem = OpenAPI_n32f_error_detail_parseFromJSON(error_details_list_local);
            if (!error_details_listItem) {
                ogs_error("No error_details_listItem");
                goto end;
            }
            OpenAPI_list_add(error_details_listList, error_details_listItem);
        }
    }

    n32f_error_info_local_var = OpenAPI_n32f_error_info_create (
        ogs_strdup(n32f_message_id->valuestring),
        n32f_error_typeVariable,
        n32f_context_id && !cJSON_IsNull(n32f_context_id) ? ogs_strdup(n32f_context_id->valuestring) : NULL,
        failed_modification_list ? failed_modification_listList : NULL,
        error_details_list ? error_details_listList : NULL
    );

    return n32f_error_info_local_var;
end:
    if (failed_modification_listList) {
        OpenAPI_list_for_each(failed_modification_listList, node) {
            OpenAPI_failed_modification_info_free(node->data);
        }
        OpenAPI_list_free(failed_modification_listList);
        failed_modification_listList = NULL;
    }
    if (error_details_listList) {
        OpenAPI_list_for_each(error_details_listList, node) {
            OpenAPI_n32f_error_detail_free(node->data);
        }
        OpenAPI_list_free(error_details_listList);
        error_details_listList = NULL;
    }
    return NULL;
}

OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_copy(OpenAPI_n32f_error_info_t *dst, OpenAPI_n32f_error_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n32f_error_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n32f_error_info_convertToJSON() failed");
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

    OpenAPI_n32f_error_info_free(dst);
    dst = OpenAPI_n32f_error_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

