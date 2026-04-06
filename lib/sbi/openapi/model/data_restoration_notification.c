
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
    char *udm_group_id,
    bool is_rediscovery_ind,
    int rediscovery_ind,
    bool is_no_resynchronization_required,
    int no_resynchronization_required,
    char *resynchronization_time,
    OpenAPI_list_t *data_to_resync,
    bool is_any_ue_ind,
    int any_ue_ind,
    bool is_ausf_rediscovery_ind,
    int ausf_rediscovery_ind
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
    data_restoration_notification_local_var->is_rediscovery_ind = is_rediscovery_ind;
    data_restoration_notification_local_var->rediscovery_ind = rediscovery_ind;
    data_restoration_notification_local_var->is_no_resynchronization_required = is_no_resynchronization_required;
    data_restoration_notification_local_var->no_resynchronization_required = no_resynchronization_required;
    data_restoration_notification_local_var->resynchronization_time = resynchronization_time;
    data_restoration_notification_local_var->data_to_resync = data_to_resync;
    data_restoration_notification_local_var->is_any_ue_ind = is_any_ue_ind;
    data_restoration_notification_local_var->any_ue_ind = any_ue_ind;
    data_restoration_notification_local_var->is_ausf_rediscovery_ind = is_ausf_rediscovery_ind;
    data_restoration_notification_local_var->ausf_rediscovery_ind = ausf_rediscovery_ind;

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
    if (data_restoration_notification->resynchronization_time) {
        ogs_free(data_restoration_notification->resynchronization_time);
        data_restoration_notification->resynchronization_time = NULL;
    }
    if (data_restoration_notification->data_to_resync) {
        OpenAPI_list_free(data_restoration_notification->data_to_resync);
        data_restoration_notification->data_to_resync = NULL;
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

    if (data_restoration_notification->is_rediscovery_ind) {
    if (cJSON_AddBoolToObject(item, "rediscoveryInd", data_restoration_notification->rediscovery_ind) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [rediscovery_ind]");
        goto end;
    }
    }

    if (data_restoration_notification->is_no_resynchronization_required) {
    if (cJSON_AddBoolToObject(item, "noResynchronizationRequired", data_restoration_notification->no_resynchronization_required) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [no_resynchronization_required]");
        goto end;
    }
    }

    if (data_restoration_notification->resynchronization_time) {
    if (cJSON_AddStringToObject(item, "resynchronizationTime", data_restoration_notification->resynchronization_time) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [resynchronization_time]");
        goto end;
    }
    }

    if (data_restoration_notification->data_to_resync != OpenAPI_udm_data_to_resynchronize_NULL) {
    cJSON *data_to_resyncList = cJSON_AddArrayToObject(item, "dataToResync");
    if (data_to_resyncList == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [data_to_resync]");
        goto end;
    }
    OpenAPI_list_for_each(data_restoration_notification->data_to_resync, node) {
        if (cJSON_AddStringToObject(data_to_resyncList, "", OpenAPI_udm_data_to_resynchronize_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [data_to_resync]");
            goto end;
        }
    }
    }

    if (data_restoration_notification->is_any_ue_ind) {
    if (cJSON_AddBoolToObject(item, "anyUeInd", data_restoration_notification->any_ue_ind) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    if (data_restoration_notification->is_ausf_rediscovery_ind) {
    if (cJSON_AddBoolToObject(item, "ausfRediscoveryInd", data_restoration_notification->ausf_rediscovery_ind) == NULL) {
        ogs_error("OpenAPI_data_restoration_notification_convertToJSON() failed [ausf_rediscovery_ind]");
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
    cJSON *rediscovery_ind = NULL;
    cJSON *no_resynchronization_required = NULL;
    cJSON *resynchronization_time = NULL;
    cJSON *data_to_resync = NULL;
    OpenAPI_list_t *data_to_resyncList = NULL;
    cJSON *any_ue_ind = NULL;
    cJSON *ausf_rediscovery_ind = NULL;
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

    rediscovery_ind = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "rediscoveryInd");
    if (rediscovery_ind) {
    if (!cJSON_IsBool(rediscovery_ind)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [rediscovery_ind]");
        goto end;
    }
    }

    no_resynchronization_required = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "noResynchronizationRequired");
    if (no_resynchronization_required) {
    if (!cJSON_IsBool(no_resynchronization_required)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [no_resynchronization_required]");
        goto end;
    }
    }

    resynchronization_time = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "resynchronizationTime");
    if (resynchronization_time) {
    if (!cJSON_IsString(resynchronization_time) && !cJSON_IsNull(resynchronization_time)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [resynchronization_time]");
        goto end;
    }
    }

    data_to_resync = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "dataToResync");
    if (data_to_resync) {
        cJSON *data_to_resync_local = NULL;
        if (!cJSON_IsArray(data_to_resync)) {
            ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [data_to_resync]");
            goto end;
        }

        data_to_resyncList = OpenAPI_list_create();

        cJSON_ArrayForEach(data_to_resync_local, data_to_resync) {
            OpenAPI_udm_data_to_resynchronize_e localEnum = OpenAPI_udm_data_to_resynchronize_NULL;
            if (!cJSON_IsString(data_to_resync_local)) {
                ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [data_to_resync]");
                goto end;
            }
            localEnum = OpenAPI_udm_data_to_resynchronize_FromString(data_to_resync_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"data_to_resync\" is not supported. Ignoring it ...",
                         data_to_resync_local->valuestring);
            } else {
                OpenAPI_list_add(data_to_resyncList, (void *)localEnum);
            }
        }
        if (data_to_resyncList->count == 0) {
            ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed: Expected data_to_resyncList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    any_ue_ind = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "anyUeInd");
    if (any_ue_ind) {
    if (!cJSON_IsBool(any_ue_ind)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    ausf_rediscovery_ind = cJSON_GetObjectItemCaseSensitive(data_restoration_notificationJSON, "ausfRediscoveryInd");
    if (ausf_rediscovery_ind) {
    if (!cJSON_IsBool(ausf_rediscovery_ind)) {
        ogs_error("OpenAPI_data_restoration_notification_parseFromJSON() failed [ausf_rediscovery_ind]");
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
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL,
        rediscovery_ind ? true : false,
        rediscovery_ind ? rediscovery_ind->valueint : 0,
        no_resynchronization_required ? true : false,
        no_resynchronization_required ? no_resynchronization_required->valueint : 0,
        resynchronization_time && !cJSON_IsNull(resynchronization_time) ? ogs_strdup(resynchronization_time->valuestring) : NULL,
        data_to_resync ? data_to_resyncList : NULL,
        any_ue_ind ? true : false,
        any_ue_ind ? any_ue_ind->valueint : 0,
        ausf_rediscovery_ind ? true : false,
        ausf_rediscovery_ind ? ausf_rediscovery_ind->valueint : 0
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
    if (data_to_resyncList) {
        OpenAPI_list_free(data_to_resyncList);
        data_to_resyncList = NULL;
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

