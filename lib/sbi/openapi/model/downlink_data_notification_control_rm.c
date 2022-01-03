
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "downlink_data_notification_control_rm.h"

OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_create(
    OpenAPI_list_t *notif_ctrl_inds,
    OpenAPI_list_t *types_of_notif
)
{
    OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm_local_var = ogs_malloc(sizeof(OpenAPI_downlink_data_notification_control_rm_t));
    ogs_assert(downlink_data_notification_control_rm_local_var);

    downlink_data_notification_control_rm_local_var->notif_ctrl_inds = notif_ctrl_inds;
    downlink_data_notification_control_rm_local_var->types_of_notif = types_of_notif;

    return downlink_data_notification_control_rm_local_var;
}

void OpenAPI_downlink_data_notification_control_rm_free(OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm)
{
    if (NULL == downlink_data_notification_control_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(downlink_data_notification_control_rm->notif_ctrl_inds);
    OpenAPI_list_free(downlink_data_notification_control_rm->types_of_notif);
    ogs_free(downlink_data_notification_control_rm);
}

cJSON *OpenAPI_downlink_data_notification_control_rm_convertToJSON(OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm)
{
    cJSON *item = NULL;

    if (downlink_data_notification_control_rm == NULL) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [DownlinkDataNotificationControlRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (downlink_data_notification_control_rm->notif_ctrl_inds) {
    cJSON *notif_ctrl_inds = cJSON_AddArrayToObject(item, "notifCtrlInds");
    if (notif_ctrl_inds == NULL) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [notif_ctrl_inds]");
        goto end;
    }
    OpenAPI_lnode_t *notif_ctrl_inds_node;
    OpenAPI_list_for_each(downlink_data_notification_control_rm->notif_ctrl_inds, notif_ctrl_inds_node) {
        if (cJSON_AddStringToObject(notif_ctrl_inds, "", OpenAPI_notification_control_indication_ToString((intptr_t)notif_ctrl_inds_node->data)) == NULL) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [notif_ctrl_inds]");
            goto end;
        }
    }
    }

    if (downlink_data_notification_control_rm->types_of_notif) {
    cJSON *types_of_notif = cJSON_AddArrayToObject(item, "typesOfNotif");
    if (types_of_notif == NULL) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [types_of_notif]");
        goto end;
    }
    OpenAPI_lnode_t *types_of_notif_node;
    OpenAPI_list_for_each(downlink_data_notification_control_rm->types_of_notif, types_of_notif_node) {
        if (cJSON_AddStringToObject(types_of_notif, "", OpenAPI_dl_data_delivery_status_ToString((intptr_t)types_of_notif_node->data)) == NULL) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [types_of_notif]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_parseFromJSON(cJSON *downlink_data_notification_control_rmJSON)
{
    OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm_local_var = NULL;
    cJSON *notif_ctrl_inds = cJSON_GetObjectItemCaseSensitive(downlink_data_notification_control_rmJSON, "notifCtrlInds");

    OpenAPI_list_t *notif_ctrl_indsList;
    if (notif_ctrl_inds) {
    cJSON *notif_ctrl_inds_local_nonprimitive;
    if (!cJSON_IsArray(notif_ctrl_inds)) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [notif_ctrl_inds]");
        goto end;
    }

    notif_ctrl_indsList = OpenAPI_list_create();

    cJSON_ArrayForEach(notif_ctrl_inds_local_nonprimitive, notif_ctrl_inds ) {
        if (!cJSON_IsString(notif_ctrl_inds_local_nonprimitive)){
            ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [notif_ctrl_inds]");
            goto end;
        }

        OpenAPI_list_add(notif_ctrl_indsList, (void *)OpenAPI_notification_control_indication_FromString(notif_ctrl_inds_local_nonprimitive->valuestring));
    }
    }

    cJSON *types_of_notif = cJSON_GetObjectItemCaseSensitive(downlink_data_notification_control_rmJSON, "typesOfNotif");

    OpenAPI_list_t *types_of_notifList;
    if (types_of_notif) {
    cJSON *types_of_notif_local_nonprimitive;
    if (!cJSON_IsArray(types_of_notif)) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [types_of_notif]");
        goto end;
    }

    types_of_notifList = OpenAPI_list_create();

    cJSON_ArrayForEach(types_of_notif_local_nonprimitive, types_of_notif ) {
        if (!cJSON_IsString(types_of_notif_local_nonprimitive)){
            ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [types_of_notif]");
            goto end;
        }

        OpenAPI_list_add(types_of_notifList, (void *)OpenAPI_dl_data_delivery_status_FromString(types_of_notif_local_nonprimitive->valuestring));
    }
    }

    downlink_data_notification_control_rm_local_var = OpenAPI_downlink_data_notification_control_rm_create (
        notif_ctrl_inds ? notif_ctrl_indsList : NULL,
        types_of_notif ? types_of_notifList : NULL
    );

    return downlink_data_notification_control_rm_local_var;
end:
    return NULL;
}

OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_copy(OpenAPI_downlink_data_notification_control_rm_t *dst, OpenAPI_downlink_data_notification_control_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_downlink_data_notification_control_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed");
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

    OpenAPI_downlink_data_notification_control_rm_free(dst);
    dst = OpenAPI_downlink_data_notification_control_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

