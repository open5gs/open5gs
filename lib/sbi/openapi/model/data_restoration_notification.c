
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_restoration_notification.h"

OpenAPI_data_restoration_notification_t *OpenAPI_data_restoration_notification_create(
    char *last_replication_time,
    char *recovery_time,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *reset_ids,
    OpenAPI_list_t *s_nssai_list,
    OpenAPI_list_t *dnn_list,
    char *udm_group_id
)
{
    OpenAPI_data_restoration_notification_t *data_restoration_notification_local_var = ogs_malloc(sizeof(OpenAPI_data_restoration_notification_t));
    ogs_assert(data_restoration_notification_local_var);

    data_restoration_notification_local_var->last_replication_time = last_replication_time;
    data_restoration_notification_local_var->recovery_time = recovery_time;
    data_restoration_notification_local_var->plmn_id = plmn_id;
    data_restoration_notification_local_var->supi_ranges = supi_ranges;
    data_restoration_notification_local_var->gpsi_ranges = gpsi_ranges;
    data_restoration_notification_local_var->reset_ids = reset_ids;
    data_restoration_notification_local_var->s_nssai_list = s_nssai_list;
    data_restoration_notification_local_var->dnn_list = dnn_list;
    data_restoration_notification_local_var->udm_group_id = udm_group_id;

    return data_restoration_notification_local_var;
}

void OpenAPI_data_restoration_notification_free(OpenAPI_data_restoration_notification_t *data_restoration_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == data_restoration_notification) {
        return;
    }
    if (data_restoration_notification->last_replication_time) {
        ogs_free(data_restoration_notification->last_replication_time);
        data_restoration_notification->last_replication_time = NULL;
    }
    if (data_restoration_notification->recovery_time) {
        ogs_free(data_restoration_notification->recovery_time);
        data_restoration_notification->recovery_time = NULL;
    }
    if (data_restoration_notification->plmn_id) {
        OpenAPI_plmn_id_free(data_restoration_notification->plmn_id);
        data_restoration_notification->plmn_id = NULL;
    }
    if (data_restoration_notification->supi_ranges) {
        OpenAPI_list_for_each(data_restoration_notification->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(data_restoration_notification->supi_ranges);
        data_restoration_notification->supi_ranges = NULL;
    }
    if (data_restoration_notification->gpsi_ranges) {
        OpenAPI_list_for_each(data_restoration_notification->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(data_restoration_notification->gpsi_ranges);
        data_restoration_notification->gpsi_ranges = NULL;
    }
    if (data_restoration_notification->reset_ids) {
        OpenAPI_list_for_each(data_restoration_notification->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_restoration_notification->reset_ids);
        data_restoration_notification->reset_ids = NULL;
    }
    if (data_restoration_notification->s_nssai_list) {
        OpenAPI_list_for_each(data_restoration_notification->s_nssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(data_restoration_notification->s_nssai_list);
        data_restoration_notification->s_nssai_list = NULL;
    }
    if (data_restoration_notification->dnn_list) {
        OpenAPI_list_for_each(data_restoration_notification->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_restoration_notification->dnn_list);
        data_restoration_notification->dnn_list = NULL;
    }
    if (data_restoration_notification->udm_group_id) {
        ogs_free(data_restoration_notification->udm_group_id);
        data_restoration_notification->udm_group_id = NULL;
    }
    ogs_free(data_restoration_notification);
}

cJSON *OpenAPI_data_restoration_notification_convertToJSON(OpenAPI_data_restoration_notification_t *data_restoration_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (data_restoration_notification == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [DataRestorationNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (data_restoration_notification->last_replication_time) {
    if (cJSON_AddStringToObject(item, "lastReplicationTime", data_restoration_notification->last_replication_time) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [last_replication_time]");
        goto end;
    }
    }

    if (data_restoration_notification->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", data_restoration_notification->recovery_time) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

    if (data_restoration_notification->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(data_restoration_notification->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (data_restoration_notification->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(data_restoration_notification->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (data_restoration_notification->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(data_restoration_notification->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (data_restoration_notification->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(data_restoration_notification->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (data_restoration_notification->s_nssai_list) {
    cJSON *s_nssai_listList = cJSON_AddArrayToObject(item, "sNssaiList");
    if (s_nssai_listList == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [s_nssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(data_restoration_notification->s_nssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [s_nssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_listList, itemLocal);
    }
    }

    if (data_restoration_notification->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(data_restoration_notification->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

    if (data_restoration_notification->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", data_restoration_notification->udm_group_id) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_data_restoration_notification_t *OpenAPI_data_restoration_notification_parseFromJSON(cJSON *data_restoration_notificationJSON)
{
    OpenAPI_data_restoration_notification_t *data_restoration_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *last_replication_time = NULL;
    cJSON *recovery_time = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *s_nssai_list = NULL;
    OpenAPI_list_t *s_nssai_listList = NULL;
    cJSON *dnn_list = NULL;
    OpenAPI_list_t *dnn_listList = NULL;
    cJSON *udm_group_id = NULL;
    last_replication_time = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "lastReplicationTime");
    if (last_replication_time) {
    if (!cJSON_IsString(last_replication_time) && !cJSON_IsNull(last_replication_time)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [last_replication_time]");
        goto end;
    }
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local);
            if (!supi_rangesItem) {
                ogs_error("No supi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local);
            if (!gpsi_rangesItem) {
                ogs_error("No gpsi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    s_nssai_list = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "sNssaiList");
    if (s_nssai_list) {
        cJSON *s_nssai_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_list)) {
            ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [s_nssai_list]");
            goto end;
        }

        s_nssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_list_local, s_nssai_list) {
            if (!cJSON_IsObject(s_nssai_list_local)) {
                ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [s_nssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *s_nssai_listItem = OpenAPI_snssai_parseFromJSON(s_nssai_list_local);
            if (!s_nssai_listItem) {
                ogs_error("No s_nssai_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_listList, s_nssai_listItem);
        }
    }

    dnn_list = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    udm_group_id = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    data_restoration_notification_local_var = OpenAPI_data_restoration_notification_create (
        last_replication_time && !cJSON_IsNull(last_replication_time) ? ogs_strdup(last_replication_time->valuestring) : NULL,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        reset_ids ? reset_idsList : NULL,
        s_nssai_list ? s_nssai_listList : NULL,
        dnn_list ? dnn_listList : NULL,
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL
    );

    return data_restoration_notification_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (gpsi_rangesList) {
        OpenAPI_list_for_each(gpsi_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_rangesList);
        gpsi_rangesList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    if (s_nssai_listList) {
        OpenAPI_list_for_each(s_nssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(s_nssai_listList);
        s_nssai_listList = NULL;
    }
    if (dnn_listList) {
        OpenAPI_list_for_each(dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_listList);
        dnn_listList = NULL;
    }
    return NULL;
}

OpenAPI_data_restoration_notification_t *OpenAPI_data_restoration_notification_copy(OpenAPI_data_restoration_notification_t *dst, OpenAPI_data_restoration_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_data_restoration_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed");
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

    OpenAPI_data_restoration_notification_free(dst);
    dst = OpenAPI_data_restoration_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

