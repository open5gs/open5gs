
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "downlink_data_notification_control_rm.h"

OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_create(
    bool is_notif_ctrl_inds_null,
    OpenAPI_list_t *notif_ctrl_inds,
    bool is_types_of_notif_null,
    OpenAPI_list_t *types_of_notif
)
{
    OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm_local_var = ogs_malloc(sizeof(OpenAPI_downlink_data_notification_control_rm_t));
    ogs_assert(downlink_data_notification_control_rm_local_var);

    downlink_data_notification_control_rm_local_var->is_notif_ctrl_inds_null = is_notif_ctrl_inds_null;
    downlink_data_notification_control_rm_local_var->notif_ctrl_inds = notif_ctrl_inds;
    downlink_data_notification_control_rm_local_var->is_types_of_notif_null = is_types_of_notif_null;
    downlink_data_notification_control_rm_local_var->types_of_notif = types_of_notif;

    return downlink_data_notification_control_rm_local_var;
}

void OpenAPI_downlink_data_notification_control_rm_free(OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == downlink_data_notification_control_rm) {
        return;
    }
    if (downlink_data_notification_control_rm->notif_ctrl_inds) {
        OpenAPI_list_free(downlink_data_notification_control_rm->notif_ctrl_inds);
        downlink_data_notification_control_rm->notif_ctrl_inds = NULL;
    }
    if (downlink_data_notification_control_rm->types_of_notif) {
        OpenAPI_list_free(downlink_data_notification_control_rm->types_of_notif);
        downlink_data_notification_control_rm->types_of_notif = NULL;
    }
    ogs_free(downlink_data_notification_control_rm);
}

cJSON *OpenAPI_downlink_data_notification_control_rm_convertToJSON(OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (downlink_data_notification_control_rm == NULL) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [DownlinkDataNotificationControlRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (downlink_data_notification_control_rm->notif_ctrl_inds != OpenAPI_notification_control_indication_NULL) {
    cJSON *notif_ctrl_indsList = cJSON_AddArrayToObject(item, "notifCtrlInds");
    if (notif_ctrl_indsList == NULL) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [notif_ctrl_inds]");
        goto end;
    }
    OpenAPI_list_for_each(downlink_data_notification_control_rm->notif_ctrl_inds, node) {
        if (cJSON_AddStringToObject(notif_ctrl_indsList, "", OpenAPI_notification_control_indication_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [notif_ctrl_inds]");
            goto end;
        }
    }
    } else if (downlink_data_notification_control_rm->is_notif_ctrl_inds_null) {
        if (cJSON_AddNullToObject(item, "notifCtrlInds") == NULL) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [notif_ctrl_inds]");
            goto end;
        }
    }

    if (downlink_data_notification_control_rm->types_of_notif != OpenAPI_dl_data_delivery_status_NULL) {
    cJSON *types_of_notifList = cJSON_AddArrayToObject(item, "typesOfNotif");
    if (types_of_notifList == NULL) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [types_of_notif]");
        goto end;
    }
    OpenAPI_list_for_each(downlink_data_notification_control_rm->types_of_notif, node) {
        if (cJSON_AddStringToObject(types_of_notifList, "", OpenAPI_dl_data_delivery_status_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [types_of_notif]");
            goto end;
        }
    }
    } else if (downlink_data_notification_control_rm->is_types_of_notif_null) {
        if (cJSON_AddNullToObject(item, "typesOfNotif") == NULL) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_convertToJSON() failed [types_of_notif]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_parseFromJSON(cJSON *downlink_data_notification_control_rmJSON)
{
    OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notif_ctrl_inds = NULL;
    OpenAPI_list_t *notif_ctrl_indsList = NULL;
    cJSON *types_of_notif = NULL;
    OpenAPI_list_t *types_of_notifList = NULL;
    notif_ctrl_inds = cJSON_GetObjectItemCaseSensitive(downlink_data_notification_control_rmJSON, "notifCtrlInds");
    if (notif_ctrl_inds) {
    if (!cJSON_IsNull(notif_ctrl_inds)) {
        cJSON *notif_ctrl_inds_local = NULL;
        if (!cJSON_IsArray(notif_ctrl_inds)) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [notif_ctrl_inds]");
            goto end;
        }

        notif_ctrl_indsList = OpenAPI_list_create();

        cJSON_ArrayForEach(notif_ctrl_inds_local, notif_ctrl_inds) {
            OpenAPI_notification_control_indication_e localEnum = OpenAPI_notification_control_indication_NULL;
            if (!cJSON_IsString(notif_ctrl_inds_local)) {
                ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [notif_ctrl_inds]");
                goto end;
            }
            localEnum = OpenAPI_notification_control_indication_FromString(notif_ctrl_inds_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"notif_ctrl_inds\" is not supported. Ignoring it ...",
                         notif_ctrl_inds_local->valuestring);
            } else {
                OpenAPI_list_add(notif_ctrl_indsList, (void *)localEnum);
            }
        }
        if (notif_ctrl_indsList->count == 0) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed: Expected notif_ctrl_indsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }
    }

    types_of_notif = cJSON_GetObjectItemCaseSensitive(downlink_data_notification_control_rmJSON, "typesOfNotif");
    if (types_of_notif) {
    if (!cJSON_IsNull(types_of_notif)) {
        cJSON *types_of_notif_local = NULL;
        if (!cJSON_IsArray(types_of_notif)) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [types_of_notif]");
            goto end;
        }

        types_of_notifList = OpenAPI_list_create();

        cJSON_ArrayForEach(types_of_notif_local, types_of_notif) {
            OpenAPI_dl_data_delivery_status_e localEnum = OpenAPI_dl_data_delivery_status_NULL;
            if (!cJSON_IsString(types_of_notif_local)) {
                ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed [types_of_notif]");
                goto end;
            }
            localEnum = OpenAPI_dl_data_delivery_status_FromString(types_of_notif_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"types_of_notif\" is not supported. Ignoring it ...",
                         types_of_notif_local->valuestring);
            } else {
                OpenAPI_list_add(types_of_notifList, (void *)localEnum);
            }
        }
        if (types_of_notifList->count == 0) {
            ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON() failed: Expected types_of_notifList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }
    }

    downlink_data_notification_control_rm_local_var = OpenAPI_downlink_data_notification_control_rm_create (
        notif_ctrl_inds && cJSON_IsNull(notif_ctrl_inds) ? true : false,
        notif_ctrl_inds ? notif_ctrl_indsList : NULL,
        types_of_notif && cJSON_IsNull(types_of_notif) ? true : false,
        types_of_notif ? types_of_notifList : NULL
    );

    return downlink_data_notification_control_rm_local_var;
end:
    if (notif_ctrl_indsList) {
        OpenAPI_list_free(notif_ctrl_indsList);
        notif_ctrl_indsList = NULL;
    }
    if (types_of_notifList) {
        OpenAPI_list_free(types_of_notifList);
        types_of_notifList = NULL;
    }
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

