
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "status_notification.h"

OpenAPI_status_notification_t *OpenAPI_status_notification_create(
    OpenAPI_status_info_t *status_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    OpenAPI_target_dnai_info_t *target_dnai_info,
    char *old_pdu_session_ref,
    char *new_smf_id,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    char *inter_plmn_api_root,
    char *intra_plmn_api_root
)
{
    OpenAPI_status_notification_t *status_notification_local_var = ogs_malloc(sizeof(OpenAPI_status_notification_t));
    ogs_assert(status_notification_local_var);

    status_notification_local_var->status_info = status_info;
    status_notification_local_var->small_data_rate_status = small_data_rate_status;
    status_notification_local_var->apn_rate_status = apn_rate_status;
    status_notification_local_var->target_dnai_info = target_dnai_info;
    status_notification_local_var->old_pdu_session_ref = old_pdu_session_ref;
    status_notification_local_var->new_smf_id = new_smf_id;
    status_notification_local_var->eps_pdn_cnx_info = eps_pdn_cnx_info;
    status_notification_local_var->inter_plmn_api_root = inter_plmn_api_root;
    status_notification_local_var->intra_plmn_api_root = intra_plmn_api_root;

    return status_notification_local_var;
}

void OpenAPI_status_notification_free(OpenAPI_status_notification_t *status_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == status_notification) {
        return;
    }
    if (status_notification->status_info) {
        OpenAPI_status_info_free(status_notification->status_info);
        status_notification->status_info = NULL;
    }
    if (status_notification->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(status_notification->small_data_rate_status);
        status_notification->small_data_rate_status = NULL;
    }
    if (status_notification->apn_rate_status) {
        OpenAPI_apn_rate_status_free(status_notification->apn_rate_status);
        status_notification->apn_rate_status = NULL;
    }
    if (status_notification->target_dnai_info) {
        OpenAPI_target_dnai_info_free(status_notification->target_dnai_info);
        status_notification->target_dnai_info = NULL;
    }
    if (status_notification->old_pdu_session_ref) {
        ogs_free(status_notification->old_pdu_session_ref);
        status_notification->old_pdu_session_ref = NULL;
    }
    if (status_notification->new_smf_id) {
        ogs_free(status_notification->new_smf_id);
        status_notification->new_smf_id = NULL;
    }
    if (status_notification->eps_pdn_cnx_info) {
        OpenAPI_eps_pdn_cnx_info_free(status_notification->eps_pdn_cnx_info);
        status_notification->eps_pdn_cnx_info = NULL;
    }
    if (status_notification->inter_plmn_api_root) {
        ogs_free(status_notification->inter_plmn_api_root);
        status_notification->inter_plmn_api_root = NULL;
    }
    if (status_notification->intra_plmn_api_root) {
        ogs_free(status_notification->intra_plmn_api_root);
        status_notification->intra_plmn_api_root = NULL;
    }
    ogs_free(status_notification);
}

cJSON *OpenAPI_status_notification_convertToJSON(OpenAPI_status_notification_t *status_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (status_notification == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [StatusNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!status_notification->status_info) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [status_info]");
        return NULL;
    }
    cJSON *status_info_local_JSON = OpenAPI_status_info_convertToJSON(status_notification->status_info);
    if (status_info_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "statusInfo", status_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }

    if (status_notification->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(status_notification->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (status_notification->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(status_notification->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

    if (status_notification->target_dnai_info) {
    cJSON *target_dnai_info_local_JSON = OpenAPI_target_dnai_info_convertToJSON(status_notification->target_dnai_info);
    if (target_dnai_info_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [target_dnai_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetDnaiInfo", target_dnai_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [target_dnai_info]");
        goto end;
    }
    }

    if (status_notification->old_pdu_session_ref) {
    if (cJSON_AddStringToObject(item, "oldPduSessionRef", status_notification->old_pdu_session_ref) == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    if (status_notification->new_smf_id) {
    if (cJSON_AddStringToObject(item, "newSmfId", status_notification->new_smf_id) == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [new_smf_id]");
        goto end;
    }
    }

    if (status_notification->eps_pdn_cnx_info) {
    cJSON *eps_pdn_cnx_info_local_JSON = OpenAPI_eps_pdn_cnx_info_convertToJSON(status_notification->eps_pdn_cnx_info);
    if (eps_pdn_cnx_info_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [eps_pdn_cnx_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "epsPdnCnxInfo", eps_pdn_cnx_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [eps_pdn_cnx_info]");
        goto end;
    }
    }

    if (status_notification->inter_plmn_api_root) {
    if (cJSON_AddStringToObject(item, "interPlmnApiRoot", status_notification->inter_plmn_api_root) == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    if (status_notification->intra_plmn_api_root) {
    if (cJSON_AddStringToObject(item, "intraPlmnApiRoot", status_notification->intra_plmn_api_root) == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [intra_plmn_api_root]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_status_notification_t *OpenAPI_status_notification_parseFromJSON(cJSON *status_notificationJSON)
{
    OpenAPI_status_notification_t *status_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *status_info = NULL;
    OpenAPI_status_info_t *status_info_local_nonprim = NULL;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    cJSON *apn_rate_status = NULL;
    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    cJSON *target_dnai_info = NULL;
    OpenAPI_target_dnai_info_t *target_dnai_info_local_nonprim = NULL;
    cJSON *old_pdu_session_ref = NULL;
    cJSON *new_smf_id = NULL;
    cJSON *eps_pdn_cnx_info = NULL;
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info_local_nonprim = NULL;
    cJSON *inter_plmn_api_root = NULL;
    cJSON *intra_plmn_api_root = NULL;
    status_info = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "statusInfo");
    if (!status_info) {
        ogs_error("OpenAPI_status_notification_parseFromJSON() failed [status_info]");
        goto end;
    }
    status_info_local_nonprim = OpenAPI_status_info_parseFromJSON(status_info);
    if (!status_info_local_nonprim) {
        ogs_error("OpenAPI_status_info_parseFromJSON failed [status_info]");
        goto end;
    }

    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "smallDataRateStatus");
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }
    }

    apn_rate_status = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "apnRateStatus");
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    if (!apn_rate_status_local_nonprim) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON failed [apn_rate_status]");
        goto end;
    }
    }

    target_dnai_info = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "targetDnaiInfo");
    if (target_dnai_info) {
    target_dnai_info_local_nonprim = OpenAPI_target_dnai_info_parseFromJSON(target_dnai_info);
    if (!target_dnai_info_local_nonprim) {
        ogs_error("OpenAPI_target_dnai_info_parseFromJSON failed [target_dnai_info]");
        goto end;
    }
    }

    old_pdu_session_ref = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "oldPduSessionRef");
    if (old_pdu_session_ref) {
    if (!cJSON_IsString(old_pdu_session_ref) && !cJSON_IsNull(old_pdu_session_ref)) {
        ogs_error("OpenAPI_status_notification_parseFromJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    new_smf_id = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "newSmfId");
    if (new_smf_id) {
    if (!cJSON_IsString(new_smf_id) && !cJSON_IsNull(new_smf_id)) {
        ogs_error("OpenAPI_status_notification_parseFromJSON() failed [new_smf_id]");
        goto end;
    }
    }

    eps_pdn_cnx_info = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "epsPdnCnxInfo");
    if (eps_pdn_cnx_info) {
    eps_pdn_cnx_info_local_nonprim = OpenAPI_eps_pdn_cnx_info_parseFromJSON(eps_pdn_cnx_info);
    if (!eps_pdn_cnx_info_local_nonprim) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON failed [eps_pdn_cnx_info]");
        goto end;
    }
    }

    inter_plmn_api_root = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "interPlmnApiRoot");
    if (inter_plmn_api_root) {
    if (!cJSON_IsString(inter_plmn_api_root) && !cJSON_IsNull(inter_plmn_api_root)) {
        ogs_error("OpenAPI_status_notification_parseFromJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    intra_plmn_api_root = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "intraPlmnApiRoot");
    if (intra_plmn_api_root) {
    if (!cJSON_IsString(intra_plmn_api_root) && !cJSON_IsNull(intra_plmn_api_root)) {
        ogs_error("OpenAPI_status_notification_parseFromJSON() failed [intra_plmn_api_root]");
        goto end;
    }
    }

    status_notification_local_var = OpenAPI_status_notification_create (
        status_info_local_nonprim,
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL,
        target_dnai_info ? target_dnai_info_local_nonprim : NULL,
        old_pdu_session_ref && !cJSON_IsNull(old_pdu_session_ref) ? ogs_strdup(old_pdu_session_ref->valuestring) : NULL,
        new_smf_id && !cJSON_IsNull(new_smf_id) ? ogs_strdup(new_smf_id->valuestring) : NULL,
        eps_pdn_cnx_info ? eps_pdn_cnx_info_local_nonprim : NULL,
        inter_plmn_api_root && !cJSON_IsNull(inter_plmn_api_root) ? ogs_strdup(inter_plmn_api_root->valuestring) : NULL,
        intra_plmn_api_root && !cJSON_IsNull(intra_plmn_api_root) ? ogs_strdup(intra_plmn_api_root->valuestring) : NULL
    );

    return status_notification_local_var;
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
    if (target_dnai_info_local_nonprim) {
        OpenAPI_target_dnai_info_free(target_dnai_info_local_nonprim);
        target_dnai_info_local_nonprim = NULL;
    }
    if (eps_pdn_cnx_info_local_nonprim) {
        OpenAPI_eps_pdn_cnx_info_free(eps_pdn_cnx_info_local_nonprim);
        eps_pdn_cnx_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_status_notification_t *OpenAPI_status_notification_copy(OpenAPI_status_notification_t *dst, OpenAPI_status_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_status_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed");
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

    OpenAPI_status_notification_free(dst);
    dst = OpenAPI_status_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

