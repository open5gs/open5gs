
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exposure_data_change_notification.h"

OpenAPI_exposure_data_change_notification_t *OpenAPI_exposure_data_change_notification_create(
    char *ue_id,
    OpenAPI_access_and_mobility_data_t *access_and_mobility_data,
    OpenAPI_list_t *pdu_session_management_data,
    OpenAPI_list_t *del_resources
)
{
    OpenAPI_exposure_data_change_notification_t *exposure_data_change_notification_local_var = ogs_malloc(sizeof(OpenAPI_exposure_data_change_notification_t));
    ogs_assert(exposure_data_change_notification_local_var);

    exposure_data_change_notification_local_var->ue_id = ue_id;
    exposure_data_change_notification_local_var->access_and_mobility_data = access_and_mobility_data;
    exposure_data_change_notification_local_var->pdu_session_management_data = pdu_session_management_data;
    exposure_data_change_notification_local_var->del_resources = del_resources;

    return exposure_data_change_notification_local_var;
}

void OpenAPI_exposure_data_change_notification_free(OpenAPI_exposure_data_change_notification_t *exposure_data_change_notification)
{
    if (NULL == exposure_data_change_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(exposure_data_change_notification->ue_id);
    OpenAPI_access_and_mobility_data_free(exposure_data_change_notification->access_and_mobility_data);
    OpenAPI_list_for_each(exposure_data_change_notification->pdu_session_management_data, node) {
        OpenAPI_pdu_session_management_data_free(node->data);
    }
    OpenAPI_list_free(exposure_data_change_notification->pdu_session_management_data);
    OpenAPI_list_for_each(exposure_data_change_notification->del_resources, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(exposure_data_change_notification->del_resources);
    ogs_free(exposure_data_change_notification);
}

cJSON *OpenAPI_exposure_data_change_notification_convertToJSON(OpenAPI_exposure_data_change_notification_t *exposure_data_change_notification)
{
    cJSON *item = NULL;

    if (exposure_data_change_notification == NULL) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [ExposureDataChangeNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (exposure_data_change_notification->ue_id) {
    if (cJSON_AddStringToObject(item, "ueId", exposure_data_change_notification->ue_id) == NULL) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [ue_id]");
        goto end;
    }
    }

    if (exposure_data_change_notification->access_and_mobility_data) {
    cJSON *access_and_mobility_data_local_JSON = OpenAPI_access_and_mobility_data_convertToJSON(exposure_data_change_notification->access_and_mobility_data);
    if (access_and_mobility_data_local_JSON == NULL) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [access_and_mobility_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessAndMobilityData", access_and_mobility_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [access_and_mobility_data]");
        goto end;
    }
    }

    if (exposure_data_change_notification->pdu_session_management_data) {
    cJSON *pdu_session_management_dataList = cJSON_AddArrayToObject(item, "pduSessionManagementData");
    if (pdu_session_management_dataList == NULL) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [pdu_session_management_data]");
        goto end;
    }

    OpenAPI_lnode_t *pdu_session_management_data_node;
    if (exposure_data_change_notification->pdu_session_management_data) {
        OpenAPI_list_for_each(exposure_data_change_notification->pdu_session_management_data, pdu_session_management_data_node) {
            cJSON *itemLocal = OpenAPI_pdu_session_management_data_convertToJSON(pdu_session_management_data_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [pdu_session_management_data]");
                goto end;
            }
            cJSON_AddItemToArray(pdu_session_management_dataList, itemLocal);
        }
    }
    }

    if (exposure_data_change_notification->del_resources) {
    cJSON *del_resources = cJSON_AddArrayToObject(item, "delResources");
    if (del_resources == NULL) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [del_resources]");
        goto end;
    }

    OpenAPI_lnode_t *del_resources_node;
    OpenAPI_list_for_each(exposure_data_change_notification->del_resources, del_resources_node)  {
    if (cJSON_AddStringToObject(del_resources, "", (char*)del_resources_node->data) == NULL) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed [del_resources]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_exposure_data_change_notification_t *OpenAPI_exposure_data_change_notification_parseFromJSON(cJSON *exposure_data_change_notificationJSON)
{
    OpenAPI_exposure_data_change_notification_t *exposure_data_change_notification_local_var = NULL;
    cJSON *ue_id = cJSON_GetObjectItemCaseSensitive(exposure_data_change_notificationJSON, "ueId");

    if (ue_id) {
    if (!cJSON_IsString(ue_id)) {
        ogs_error("OpenAPI_exposure_data_change_notification_parseFromJSON() failed [ue_id]");
        goto end;
    }
    }

    cJSON *access_and_mobility_data = cJSON_GetObjectItemCaseSensitive(exposure_data_change_notificationJSON, "accessAndMobilityData");

    OpenAPI_access_and_mobility_data_t *access_and_mobility_data_local_nonprim = NULL;
    if (access_and_mobility_data) {
    access_and_mobility_data_local_nonprim = OpenAPI_access_and_mobility_data_parseFromJSON(access_and_mobility_data);
    }

    cJSON *pdu_session_management_data = cJSON_GetObjectItemCaseSensitive(exposure_data_change_notificationJSON, "pduSessionManagementData");

    OpenAPI_list_t *pdu_session_management_dataList;
    if (pdu_session_management_data) {
    cJSON *pdu_session_management_data_local_nonprimitive;
    if (!cJSON_IsArray(pdu_session_management_data)){
        ogs_error("OpenAPI_exposure_data_change_notification_parseFromJSON() failed [pdu_session_management_data]");
        goto end;
    }

    pdu_session_management_dataList = OpenAPI_list_create();

    cJSON_ArrayForEach(pdu_session_management_data_local_nonprimitive, pdu_session_management_data ) {
        if (!cJSON_IsObject(pdu_session_management_data_local_nonprimitive)) {
            ogs_error("OpenAPI_exposure_data_change_notification_parseFromJSON() failed [pdu_session_management_data]");
            goto end;
        }
        OpenAPI_pdu_session_management_data_t *pdu_session_management_dataItem = OpenAPI_pdu_session_management_data_parseFromJSON(pdu_session_management_data_local_nonprimitive);

        if (!pdu_session_management_dataItem) {
            ogs_error("No pdu_session_management_dataItem");
            OpenAPI_list_free(pdu_session_management_dataList);
            goto end;
        }

        OpenAPI_list_add(pdu_session_management_dataList, pdu_session_management_dataItem);
    }
    }

    cJSON *del_resources = cJSON_GetObjectItemCaseSensitive(exposure_data_change_notificationJSON, "delResources");

    OpenAPI_list_t *del_resourcesList;
    if (del_resources) {
    cJSON *del_resources_local;
    if (!cJSON_IsArray(del_resources)) {
        ogs_error("OpenAPI_exposure_data_change_notification_parseFromJSON() failed [del_resources]");
        goto end;
    }
    del_resourcesList = OpenAPI_list_create();

    cJSON_ArrayForEach(del_resources_local, del_resources) {
    if (!cJSON_IsString(del_resources_local)) {
        ogs_error("OpenAPI_exposure_data_change_notification_parseFromJSON() failed [del_resources]");
        goto end;
    }
    OpenAPI_list_add(del_resourcesList , ogs_strdup(del_resources_local->valuestring));
    }
    }

    exposure_data_change_notification_local_var = OpenAPI_exposure_data_change_notification_create (
        ue_id ? ogs_strdup(ue_id->valuestring) : NULL,
        access_and_mobility_data ? access_and_mobility_data_local_nonprim : NULL,
        pdu_session_management_data ? pdu_session_management_dataList : NULL,
        del_resources ? del_resourcesList : NULL
    );

    return exposure_data_change_notification_local_var;
end:
    return NULL;
}

OpenAPI_exposure_data_change_notification_t *OpenAPI_exposure_data_change_notification_copy(OpenAPI_exposure_data_change_notification_t *dst, OpenAPI_exposure_data_change_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_exposure_data_change_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_exposure_data_change_notification_convertToJSON() failed");
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

    OpenAPI_exposure_data_change_notification_free(dst);
    dst = OpenAPI_exposure_data_change_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

