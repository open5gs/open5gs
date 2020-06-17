
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cm_info_report.h"

OpenAPI_cm_info_report_t *OpenAPI_cm_info_report_create(
    OpenAPI_list_t *old_cm_info_list,
    OpenAPI_list_t *new_cm_info_list
    )
{
    OpenAPI_cm_info_report_t *cm_info_report_local_var = OpenAPI_malloc(sizeof(OpenAPI_cm_info_report_t));
    if (!cm_info_report_local_var) {
        return NULL;
    }
    cm_info_report_local_var->old_cm_info_list = old_cm_info_list;
    cm_info_report_local_var->new_cm_info_list = new_cm_info_list;

    return cm_info_report_local_var;
}

void OpenAPI_cm_info_report_free(OpenAPI_cm_info_report_t *cm_info_report)
{
    if (NULL == cm_info_report) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(cm_info_report->old_cm_info_list, node) {
        OpenAPI_cm_info_free(node->data);
    }
    OpenAPI_list_free(cm_info_report->old_cm_info_list);
    OpenAPI_list_for_each(cm_info_report->new_cm_info_list, node) {
        OpenAPI_cm_info_free(node->data);
    }
    OpenAPI_list_free(cm_info_report->new_cm_info_list);
    ogs_free(cm_info_report);
}

cJSON *OpenAPI_cm_info_report_convertToJSON(OpenAPI_cm_info_report_t *cm_info_report)
{
    cJSON *item = NULL;

    if (cm_info_report == NULL) {
        ogs_error("OpenAPI_cm_info_report_convertToJSON() failed [CmInfoReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cm_info_report->old_cm_info_list) {
        cJSON *old_cm_info_listList = cJSON_AddArrayToObject(item, "oldCmInfoList");
        if (old_cm_info_listList == NULL) {
            ogs_error("OpenAPI_cm_info_report_convertToJSON() failed [old_cm_info_list]");
            goto end;
        }

        OpenAPI_lnode_t *old_cm_info_list_node;
        if (cm_info_report->old_cm_info_list) {
            OpenAPI_list_for_each(cm_info_report->old_cm_info_list, old_cm_info_list_node) {
                cJSON *itemLocal = OpenAPI_cm_info_convertToJSON(old_cm_info_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_cm_info_report_convertToJSON() failed [old_cm_info_list]");
                    goto end;
                }
                cJSON_AddItemToArray(old_cm_info_listList, itemLocal);
            }
        }
    }

    if (!cm_info_report->new_cm_info_list) {
        ogs_error("OpenAPI_cm_info_report_convertToJSON() failed [new_cm_info_list]");
        goto end;
    }
    cJSON *new_cm_info_listList = cJSON_AddArrayToObject(item, "newCmInfoList");
    if (new_cm_info_listList == NULL) {
        ogs_error("OpenAPI_cm_info_report_convertToJSON() failed [new_cm_info_list]");
        goto end;
    }

    OpenAPI_lnode_t *new_cm_info_list_node;
    if (cm_info_report->new_cm_info_list) {
        OpenAPI_list_for_each(cm_info_report->new_cm_info_list, new_cm_info_list_node) {
            cJSON *itemLocal = OpenAPI_cm_info_convertToJSON(new_cm_info_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_cm_info_report_convertToJSON() failed [new_cm_info_list]");
                goto end;
            }
            cJSON_AddItemToArray(new_cm_info_listList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_cm_info_report_t *OpenAPI_cm_info_report_parseFromJSON(cJSON *cm_info_reportJSON)
{
    OpenAPI_cm_info_report_t *cm_info_report_local_var = NULL;
    cJSON *old_cm_info_list = cJSON_GetObjectItemCaseSensitive(cm_info_reportJSON, "oldCmInfoList");

    OpenAPI_list_t *old_cm_info_listList;
    if (old_cm_info_list) {
        cJSON *old_cm_info_list_local_nonprimitive;
        if (!cJSON_IsArray(old_cm_info_list)) {
            ogs_error("OpenAPI_cm_info_report_parseFromJSON() failed [old_cm_info_list]");
            goto end;
        }

        old_cm_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(old_cm_info_list_local_nonprimitive, old_cm_info_list ) {
            if (!cJSON_IsObject(old_cm_info_list_local_nonprimitive)) {
                ogs_error("OpenAPI_cm_info_report_parseFromJSON() failed [old_cm_info_list]");
                goto end;
            }
            OpenAPI_cm_info_t *old_cm_info_listItem = OpenAPI_cm_info_parseFromJSON(old_cm_info_list_local_nonprimitive);

            OpenAPI_list_add(old_cm_info_listList, old_cm_info_listItem);
        }
    }

    cJSON *new_cm_info_list = cJSON_GetObjectItemCaseSensitive(cm_info_reportJSON, "newCmInfoList");
    if (!new_cm_info_list) {
        ogs_error("OpenAPI_cm_info_report_parseFromJSON() failed [new_cm_info_list]");
        goto end;
    }

    OpenAPI_list_t *new_cm_info_listList;

    cJSON *new_cm_info_list_local_nonprimitive;
    if (!cJSON_IsArray(new_cm_info_list)) {
        ogs_error("OpenAPI_cm_info_report_parseFromJSON() failed [new_cm_info_list]");
        goto end;
    }

    new_cm_info_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(new_cm_info_list_local_nonprimitive, new_cm_info_list ) {
        if (!cJSON_IsObject(new_cm_info_list_local_nonprimitive)) {
            ogs_error("OpenAPI_cm_info_report_parseFromJSON() failed [new_cm_info_list]");
            goto end;
        }
        OpenAPI_cm_info_t *new_cm_info_listItem = OpenAPI_cm_info_parseFromJSON(new_cm_info_list_local_nonprimitive);

        OpenAPI_list_add(new_cm_info_listList, new_cm_info_listItem);
    }

    cm_info_report_local_var = OpenAPI_cm_info_report_create (
        old_cm_info_list ? old_cm_info_listList : NULL,
        new_cm_info_listList
        );

    return cm_info_report_local_var;
end:
    return NULL;
}

OpenAPI_cm_info_report_t *OpenAPI_cm_info_report_copy(OpenAPI_cm_info_report_t *dst, OpenAPI_cm_info_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cm_info_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cm_info_report_convertToJSON() failed");
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

    OpenAPI_cm_info_report_free(dst);
    dst = OpenAPI_cm_info_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

