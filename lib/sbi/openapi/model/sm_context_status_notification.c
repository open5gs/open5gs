
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_status_notification.h"

OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_create(
    OpenAPI_status_info_t *status_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    bool is_ddn_failure_status,
    int ddn_failure_status,
    OpenAPI_list_t *notify_correlation_ids_forddn_failure,
    char *new_intermediate_smf_id,
    char *new_smf_id,
    char *new_smf_set_id,
    char *old_smf_id,
    char *old_sm_context_ref,
    char *alt_anchor_smf_uri,
    char *alt_anchor_smf_id,
    OpenAPI_target_dnai_info_t *target_dnai_info,
    char *old_pdu_session_ref,
    char *inter_plmn_api_root
)
{
    OpenAPI_sm_context_status_notification_t *sm_context_status_notification_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_status_notification_t));
    ogs_assert(sm_context_status_notification_local_var);

    sm_context_status_notification_local_var->status_info = status_info;
    sm_context_status_notification_local_var->small_data_rate_status = small_data_rate_status;
    sm_context_status_notification_local_var->apn_rate_status = apn_rate_status;
    sm_context_status_notification_local_var->is_ddn_failure_status = is_ddn_failure_status;
    sm_context_status_notification_local_var->ddn_failure_status = ddn_failure_status;
    sm_context_status_notification_local_var->notify_correlation_ids_forddn_failure = notify_correlation_ids_forddn_failure;
    sm_context_status_notification_local_var->new_intermediate_smf_id = new_intermediate_smf_id;
    sm_context_status_notification_local_var->new_smf_id = new_smf_id;
    sm_context_status_notification_local_var->new_smf_set_id = new_smf_set_id;
    sm_context_status_notification_local_var->old_smf_id = old_smf_id;
    sm_context_status_notification_local_var->old_sm_context_ref = old_sm_context_ref;
    sm_context_status_notification_local_var->alt_anchor_smf_uri = alt_anchor_smf_uri;
    sm_context_status_notification_local_var->alt_anchor_smf_id = alt_anchor_smf_id;
    sm_context_status_notification_local_var->target_dnai_info = target_dnai_info;
    sm_context_status_notification_local_var->old_pdu_session_ref = old_pdu_session_ref;
    sm_context_status_notification_local_var->inter_plmn_api_root = inter_plmn_api_root;

    return sm_context_status_notification_local_var;
}

void OpenAPI_sm_context_status_notification_free(OpenAPI_sm_context_status_notification_t *sm_context_status_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_status_notification) {
        return;
    }
    if (sm_context_status_notification->status_info) {
        OpenAPI_status_info_free(sm_context_status_notification->status_info);
        sm_context_status_notification->status_info = NULL;
    }
    if (sm_context_status_notification->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(sm_context_status_notification->small_data_rate_status);
        sm_context_status_notification->small_data_rate_status = NULL;
    }
    if (sm_context_status_notification->apn_rate_status) {
        OpenAPI_apn_rate_status_free(sm_context_status_notification->apn_rate_status);
        sm_context_status_notification->apn_rate_status = NULL;
    }
    if (sm_context_status_notification->notify_correlation_ids_forddn_failure) {
        OpenAPI_list_for_each(sm_context_status_notification->notify_correlation_ids_forddn_failure, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_status_notification->notify_correlation_ids_forddn_failure);
        sm_context_status_notification->notify_correlation_ids_forddn_failure = NULL;
    }
    if (sm_context_status_notification->new_intermediate_smf_id) {
        ogs_free(sm_context_status_notification->new_intermediate_smf_id);
        sm_context_status_notification->new_intermediate_smf_id = NULL;
    }
    if (sm_context_status_notification->new_smf_id) {
        ogs_free(sm_context_status_notification->new_smf_id);
        sm_context_status_notification->new_smf_id = NULL;
    }
    if (sm_context_status_notification->new_smf_set_id) {
        ogs_free(sm_context_status_notification->new_smf_set_id);
        sm_context_status_notification->new_smf_set_id = NULL;
    }
    if (sm_context_status_notification->old_smf_id) {
        ogs_free(sm_context_status_notification->old_smf_id);
        sm_context_status_notification->old_smf_id = NULL;
    }
    if (sm_context_status_notification->old_sm_context_ref) {
        ogs_free(sm_context_status_notification->old_sm_context_ref);
        sm_context_status_notification->old_sm_context_ref = NULL;
    }
    if (sm_context_status_notification->alt_anchor_smf_uri) {
        ogs_free(sm_context_status_notification->alt_anchor_smf_uri);
        sm_context_status_notification->alt_anchor_smf_uri = NULL;
    }
    if (sm_context_status_notification->alt_anchor_smf_id) {
        ogs_free(sm_context_status_notification->alt_anchor_smf_id);
        sm_context_status_notification->alt_anchor_smf_id = NULL;
    }
    if (sm_context_status_notification->target_dnai_info) {
        OpenAPI_target_dnai_info_free(sm_context_status_notification->target_dnai_info);
        sm_context_status_notification->target_dnai_info = NULL;
    }
    if (sm_context_status_notification->old_pdu_session_ref) {
        ogs_free(sm_context_status_notification->old_pdu_session_ref);
        sm_context_status_notification->old_pdu_session_ref = NULL;
    }
    if (sm_context_status_notification->inter_plmn_api_root) {
        ogs_free(sm_context_status_notification->inter_plmn_api_root);
        sm_context_status_notification->inter_plmn_api_root = NULL;
    }
    ogs_free(sm_context_status_notification);
}

cJSON *OpenAPI_sm_context_status_notification_convertToJSON(OpenAPI_sm_context_status_notification_t *sm_context_status_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context_status_notification == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [SmContextStatusNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_context_status_notification->status_info) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [status_info]");
        return NULL;
    }
    cJSON *status_info_local_JSON = OpenAPI_status_info_convertToJSON(sm_context_status_notification->status_info);
    if (status_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "statusInfo", status_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }

    if (sm_context_status_notification->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(sm_context_status_notification->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (sm_context_status_notification->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(sm_context_status_notification->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

    if (sm_context_status_notification->is_ddn_failure_status) {
    if (cJSON_AddBoolToObject(item, "ddnFailureStatus", sm_context_status_notification->ddn_failure_status) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [ddn_failure_status]");
        goto end;
    }
    }

    if (sm_context_status_notification->notify_correlation_ids_forddn_failure) {
    cJSON *notify_correlation_ids_forddn_failureList = cJSON_AddArrayToObject(item, "notifyCorrelationIdsForddnFailure");
    if (notify_correlation_ids_forddn_failureList == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [notify_correlation_ids_forddn_failure]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_status_notification->notify_correlation_ids_forddn_failure, node) {
        if (cJSON_AddStringToObject(notify_correlation_ids_forddn_failureList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [notify_correlation_ids_forddn_failure]");
            goto end;
        }
    }
    }

    if (sm_context_status_notification->new_intermediate_smf_id) {
    if (cJSON_AddStringToObject(item, "newIntermediateSmfId", sm_context_status_notification->new_intermediate_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [new_intermediate_smf_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->new_smf_id) {
    if (cJSON_AddStringToObject(item, "newSmfId", sm_context_status_notification->new_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [new_smf_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->new_smf_set_id) {
    if (cJSON_AddStringToObject(item, "newSmfSetId", sm_context_status_notification->new_smf_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [new_smf_set_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->old_smf_id) {
    if (cJSON_AddStringToObject(item, "oldSmfId", sm_context_status_notification->old_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [old_smf_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->old_sm_context_ref) {
    if (cJSON_AddStringToObject(item, "oldSmContextRef", sm_context_status_notification->old_sm_context_ref) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    if (sm_context_status_notification->alt_anchor_smf_uri) {
    if (cJSON_AddStringToObject(item, "altAnchorSmfUri", sm_context_status_notification->alt_anchor_smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [alt_anchor_smf_uri]");
        goto end;
    }
    }

    if (sm_context_status_notification->alt_anchor_smf_id) {
    if (cJSON_AddStringToObject(item, "altAnchorSmfId", sm_context_status_notification->alt_anchor_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [alt_anchor_smf_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->target_dnai_info) {
    cJSON *target_dnai_info_local_JSON = OpenAPI_target_dnai_info_convertToJSON(sm_context_status_notification->target_dnai_info);
    if (target_dnai_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [target_dnai_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetDnaiInfo", target_dnai_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [target_dnai_info]");
        goto end;
    }
    }

    if (sm_context_status_notification->old_pdu_session_ref) {
    if (cJSON_AddStringToObject(item, "oldPduSessionRef", sm_context_status_notification->old_pdu_session_ref) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    if (sm_context_status_notification->inter_plmn_api_root) {
    if (cJSON_AddStringToObject(item, "interPlmnApiRoot", sm_context_status_notification->inter_plmn_api_root) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_parseFromJSON(cJSON *sm_context_status_notificationJSON)
{
    OpenAPI_sm_context_status_notification_t *sm_context_status_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *status_info = NULL;
    OpenAPI_status_info_t *status_info_local_nonprim = NULL;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    cJSON *apn_rate_status = NULL;
    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    cJSON *ddn_failure_status = NULL;
    cJSON *notify_correlation_ids_forddn_failure = NULL;
    OpenAPI_list_t *notify_correlation_ids_forddn_failureList = NULL;
    cJSON *new_intermediate_smf_id = NULL;
    cJSON *new_smf_id = NULL;
    cJSON *new_smf_set_id = NULL;
    cJSON *old_smf_id = NULL;
    cJSON *old_sm_context_ref = NULL;
    cJSON *alt_anchor_smf_uri = NULL;
    cJSON *alt_anchor_smf_id = NULL;
    cJSON *target_dnai_info = NULL;
    OpenAPI_target_dnai_info_t *target_dnai_info_local_nonprim = NULL;
    cJSON *old_pdu_session_ref = NULL;
    cJSON *inter_plmn_api_root = NULL;
    status_info = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "statusInfo");
    if (!status_info) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [status_info]");
        goto end;
    }
    status_info_local_nonprim = OpenAPI_status_info_parseFromJSON(status_info);
    if (!status_info_local_nonprim) {
        ogs_error("OpenAPI_status_info_parseFromJSON failed [status_info]");
        goto end;
    }

    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "smallDataRateStatus");
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }
    }

    apn_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "apnRateStatus");
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    if (!apn_rate_status_local_nonprim) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON failed [apn_rate_status]");
        goto end;
    }
    }

    ddn_failure_status = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "ddnFailureStatus");
    if (ddn_failure_status) {
    if (!cJSON_IsBool(ddn_failure_status)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [ddn_failure_status]");
        goto end;
    }
    }

    notify_correlation_ids_forddn_failure = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "notifyCorrelationIdsForddnFailure");
    if (notify_correlation_ids_forddn_failure) {
        cJSON *notify_correlation_ids_forddn_failure_local = NULL;
        if (!cJSON_IsArray(notify_correlation_ids_forddn_failure)) {
            ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [notify_correlation_ids_forddn_failure]");
            goto end;
        }

        notify_correlation_ids_forddn_failureList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_correlation_ids_forddn_failure_local, notify_correlation_ids_forddn_failure) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(notify_correlation_ids_forddn_failure_local)) {
                ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [notify_correlation_ids_forddn_failure]");
                goto end;
            }
            OpenAPI_list_add(notify_correlation_ids_forddn_failureList, ogs_strdup(notify_correlation_ids_forddn_failure_local->valuestring));
        }
    }

    new_intermediate_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "newIntermediateSmfId");
    if (new_intermediate_smf_id) {
    if (!cJSON_IsString(new_intermediate_smf_id) && !cJSON_IsNull(new_intermediate_smf_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [new_intermediate_smf_id]");
        goto end;
    }
    }

    new_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "newSmfId");
    if (new_smf_id) {
    if (!cJSON_IsString(new_smf_id) && !cJSON_IsNull(new_smf_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [new_smf_id]");
        goto end;
    }
    }

    new_smf_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "newSmfSetId");
    if (new_smf_set_id) {
    if (!cJSON_IsString(new_smf_set_id) && !cJSON_IsNull(new_smf_set_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [new_smf_set_id]");
        goto end;
    }
    }

    old_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "oldSmfId");
    if (old_smf_id) {
    if (!cJSON_IsString(old_smf_id) && !cJSON_IsNull(old_smf_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [old_smf_id]");
        goto end;
    }
    }

    old_sm_context_ref = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "oldSmContextRef");
    if (old_sm_context_ref) {
    if (!cJSON_IsString(old_sm_context_ref) && !cJSON_IsNull(old_sm_context_ref)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    alt_anchor_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "altAnchorSmfUri");
    if (alt_anchor_smf_uri) {
    if (!cJSON_IsString(alt_anchor_smf_uri) && !cJSON_IsNull(alt_anchor_smf_uri)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [alt_anchor_smf_uri]");
        goto end;
    }
    }

    alt_anchor_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "altAnchorSmfId");
    if (alt_anchor_smf_id) {
    if (!cJSON_IsString(alt_anchor_smf_id) && !cJSON_IsNull(alt_anchor_smf_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [alt_anchor_smf_id]");
        goto end;
    }
    }

    target_dnai_info = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "targetDnaiInfo");
    if (target_dnai_info) {
    target_dnai_info_local_nonprim = OpenAPI_target_dnai_info_parseFromJSON(target_dnai_info);
    if (!target_dnai_info_local_nonprim) {
        ogs_error("OpenAPI_target_dnai_info_parseFromJSON failed [target_dnai_info]");
        goto end;
    }
    }

    old_pdu_session_ref = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "oldPduSessionRef");
    if (old_pdu_session_ref) {
    if (!cJSON_IsString(old_pdu_session_ref) && !cJSON_IsNull(old_pdu_session_ref)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    inter_plmn_api_root = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "interPlmnApiRoot");
    if (inter_plmn_api_root) {
    if (!cJSON_IsString(inter_plmn_api_root) && !cJSON_IsNull(inter_plmn_api_root)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    sm_context_status_notification_local_var = OpenAPI_sm_context_status_notification_create (
        status_info_local_nonprim,
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL,
        ddn_failure_status ? true : false,
        ddn_failure_status ? ddn_failure_status->valueint : 0,
        notify_correlation_ids_forddn_failure ? notify_correlation_ids_forddn_failureList : NULL,
        new_intermediate_smf_id && !cJSON_IsNull(new_intermediate_smf_id) ? ogs_strdup(new_intermediate_smf_id->valuestring) : NULL,
        new_smf_id && !cJSON_IsNull(new_smf_id) ? ogs_strdup(new_smf_id->valuestring) : NULL,
        new_smf_set_id && !cJSON_IsNull(new_smf_set_id) ? ogs_strdup(new_smf_set_id->valuestring) : NULL,
        old_smf_id && !cJSON_IsNull(old_smf_id) ? ogs_strdup(old_smf_id->valuestring) : NULL,
        old_sm_context_ref && !cJSON_IsNull(old_sm_context_ref) ? ogs_strdup(old_sm_context_ref->valuestring) : NULL,
        alt_anchor_smf_uri && !cJSON_IsNull(alt_anchor_smf_uri) ? ogs_strdup(alt_anchor_smf_uri->valuestring) : NULL,
        alt_anchor_smf_id && !cJSON_IsNull(alt_anchor_smf_id) ? ogs_strdup(alt_anchor_smf_id->valuestring) : NULL,
        target_dnai_info ? target_dnai_info_local_nonprim : NULL,
        old_pdu_session_ref && !cJSON_IsNull(old_pdu_session_ref) ? ogs_strdup(old_pdu_session_ref->valuestring) : NULL,
        inter_plmn_api_root && !cJSON_IsNull(inter_plmn_api_root) ? ogs_strdup(inter_plmn_api_root->valuestring) : NULL
    );

    return sm_context_status_notification_local_var;
end:
    if (status_info_local_nonprim) {
        OpenAPI_status_info_free(status_info_local_nonprim);
        status_info_local_nonprim = NULL;
    }
    if (small_data_rate_status_local_nonprim) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_local_nonprim);
        small_data_rate_status_local_nonprim = NULL;
    }
    if (apn_rate_status_local_nonprim) {
        OpenAPI_apn_rate_status_free(apn_rate_status_local_nonprim);
        apn_rate_status_local_nonprim = NULL;
    }
    if (notify_correlation_ids_forddn_failureList) {
        OpenAPI_list_for_each(notify_correlation_ids_forddn_failureList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(notify_correlation_ids_forddn_failureList);
        notify_correlation_ids_forddn_failureList = NULL;
    }
    if (target_dnai_info_local_nonprim) {
        OpenAPI_target_dnai_info_free(target_dnai_info_local_nonprim);
        target_dnai_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_copy(OpenAPI_sm_context_status_notification_t *dst, OpenAPI_sm_context_status_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_status_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed");
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

    OpenAPI_sm_context_status_notification_free(dst);
    dst = OpenAPI_sm_context_status_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

