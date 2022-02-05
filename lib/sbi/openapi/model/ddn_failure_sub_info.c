
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ddn_failure_sub_info.h"

OpenAPI_ddn_failure_sub_info_t *OpenAPI_ddn_failure_sub_info_create(
    char *notify_correlation_id,
    OpenAPI_list_t *ddd_traffic_descriptor_list
)
{
    OpenAPI_ddn_failure_sub_info_t *ddn_failure_sub_info_local_var = ogs_malloc(sizeof(OpenAPI_ddn_failure_sub_info_t));
    ogs_assert(ddn_failure_sub_info_local_var);

    ddn_failure_sub_info_local_var->notify_correlation_id = notify_correlation_id;
    ddn_failure_sub_info_local_var->ddd_traffic_descriptor_list = ddd_traffic_descriptor_list;

    return ddn_failure_sub_info_local_var;
}

void OpenAPI_ddn_failure_sub_info_free(OpenAPI_ddn_failure_sub_info_t *ddn_failure_sub_info)
{
    if (NULL == ddn_failure_sub_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ddn_failure_sub_info->notify_correlation_id);
    OpenAPI_list_for_each(ddn_failure_sub_info->ddd_traffic_descriptor_list, node) {
        OpenAPI_ddd_traffic_descriptor_free(node->data);
    }
    OpenAPI_list_free(ddn_failure_sub_info->ddd_traffic_descriptor_list);
    ogs_free(ddn_failure_sub_info);
}

cJSON *OpenAPI_ddn_failure_sub_info_convertToJSON(OpenAPI_ddn_failure_sub_info_t *ddn_failure_sub_info)
{
    cJSON *item = NULL;

    if (ddn_failure_sub_info == NULL) {
        ogs_error("OpenAPI_ddn_failure_sub_info_convertToJSON() failed [DdnFailureSubInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "notifyCorrelationId", ddn_failure_sub_info->notify_correlation_id) == NULL) {
        ogs_error("OpenAPI_ddn_failure_sub_info_convertToJSON() failed [notify_correlation_id]");
        goto end;
    }

    if (ddn_failure_sub_info->ddd_traffic_descriptor_list) {
    cJSON *ddd_traffic_descriptor_listList = cJSON_AddArrayToObject(item, "dddTrafficDescriptorList");
    if (ddd_traffic_descriptor_listList == NULL) {
        ogs_error("OpenAPI_ddn_failure_sub_info_convertToJSON() failed [ddd_traffic_descriptor_list]");
        goto end;
    }

    OpenAPI_lnode_t *ddd_traffic_descriptor_list_node;
    if (ddn_failure_sub_info->ddd_traffic_descriptor_list) {
        OpenAPI_list_for_each(ddn_failure_sub_info->ddd_traffic_descriptor_list, ddd_traffic_descriptor_list_node) {
            cJSON *itemLocal = OpenAPI_ddd_traffic_descriptor_convertToJSON(ddd_traffic_descriptor_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ddn_failure_sub_info_convertToJSON() failed [ddd_traffic_descriptor_list]");
                goto end;
            }
            cJSON_AddItemToArray(ddd_traffic_descriptor_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_ddn_failure_sub_info_t *OpenAPI_ddn_failure_sub_info_parseFromJSON(cJSON *ddn_failure_sub_infoJSON)
{
    OpenAPI_ddn_failure_sub_info_t *ddn_failure_sub_info_local_var = NULL;
    cJSON *notify_correlation_id = cJSON_GetObjectItemCaseSensitive(ddn_failure_sub_infoJSON, "notifyCorrelationId");
    if (!notify_correlation_id) {
        ogs_error("OpenAPI_ddn_failure_sub_info_parseFromJSON() failed [notify_correlation_id]");
        goto end;
    }

    if (!cJSON_IsString(notify_correlation_id)) {
        ogs_error("OpenAPI_ddn_failure_sub_info_parseFromJSON() failed [notify_correlation_id]");
        goto end;
    }

    cJSON *ddd_traffic_descriptor_list = cJSON_GetObjectItemCaseSensitive(ddn_failure_sub_infoJSON, "dddTrafficDescriptorList");

    OpenAPI_list_t *ddd_traffic_descriptor_listList;
    if (ddd_traffic_descriptor_list) {
    cJSON *ddd_traffic_descriptor_list_local_nonprimitive;
    if (!cJSON_IsArray(ddd_traffic_descriptor_list)){
        ogs_error("OpenAPI_ddn_failure_sub_info_parseFromJSON() failed [ddd_traffic_descriptor_list]");
        goto end;
    }

    ddd_traffic_descriptor_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(ddd_traffic_descriptor_list_local_nonprimitive, ddd_traffic_descriptor_list ) {
        if (!cJSON_IsObject(ddd_traffic_descriptor_list_local_nonprimitive)) {
            ogs_error("OpenAPI_ddn_failure_sub_info_parseFromJSON() failed [ddd_traffic_descriptor_list]");
            goto end;
        }
        OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_descriptor_listItem = OpenAPI_ddd_traffic_descriptor_parseFromJSON(ddd_traffic_descriptor_list_local_nonprimitive);

        if (!ddd_traffic_descriptor_listItem) {
            ogs_error("No ddd_traffic_descriptor_listItem");
            OpenAPI_list_free(ddd_traffic_descriptor_listList);
            goto end;
        }

        OpenAPI_list_add(ddd_traffic_descriptor_listList, ddd_traffic_descriptor_listItem);
    }
    }

    ddn_failure_sub_info_local_var = OpenAPI_ddn_failure_sub_info_create (
        ogs_strdup(notify_correlation_id->valuestring),
        ddd_traffic_descriptor_list ? ddd_traffic_descriptor_listList : NULL
    );

    return ddn_failure_sub_info_local_var;
end:
    return NULL;
}

OpenAPI_ddn_failure_sub_info_t *OpenAPI_ddn_failure_sub_info_copy(OpenAPI_ddn_failure_sub_info_t *dst, OpenAPI_ddn_failure_sub_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ddn_failure_sub_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ddn_failure_sub_info_convertToJSON() failed");
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

    OpenAPI_ddn_failure_sub_info_free(dst);
    dst = OpenAPI_ddn_failure_sub_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

