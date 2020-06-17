
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_n2_message_transfer_req_data.h"

OpenAPI_n1_n2_message_transfer_req_data_t *OpenAPI_n1_n2_message_transfer_req_data_create(
    OpenAPI_n1_message_container_t *n1_message_container,
    OpenAPI_n2_info_container_t *n2_info_container,
    OpenAPI_ref_to_binary_data_t *mt_data,
    int skip_ind,
    int last_msg_indication,
    int pdu_session_id,
    char *lcs_correlation_id,
    int ppi,
    OpenAPI_arp_t *arp,
    int _5qi,
    char *n1n2_failure_txf_notif_uri,
    int smf_reallocation_ind,
    OpenAPI_area_of_validity_t *area_of_validity,
    char *supported_features,
    OpenAPI_guami_t *old_guami,
    int ma_accepted_ind,
    int ext_buf_support,
    OpenAPI_access_type_e target_access
    )
{
    OpenAPI_n1_n2_message_transfer_req_data_t *n1_n2_message_transfer_req_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_n1_n2_message_transfer_req_data_t));
    if (!n1_n2_message_transfer_req_data_local_var) {
        return NULL;
    }
    n1_n2_message_transfer_req_data_local_var->n1_message_container = n1_message_container;
    n1_n2_message_transfer_req_data_local_var->n2_info_container = n2_info_container;
    n1_n2_message_transfer_req_data_local_var->mt_data = mt_data;
    n1_n2_message_transfer_req_data_local_var->skip_ind = skip_ind;
    n1_n2_message_transfer_req_data_local_var->last_msg_indication = last_msg_indication;
    n1_n2_message_transfer_req_data_local_var->pdu_session_id = pdu_session_id;
    n1_n2_message_transfer_req_data_local_var->lcs_correlation_id = lcs_correlation_id;
    n1_n2_message_transfer_req_data_local_var->ppi = ppi;
    n1_n2_message_transfer_req_data_local_var->arp = arp;
    n1_n2_message_transfer_req_data_local_var->_5qi = _5qi;
    n1_n2_message_transfer_req_data_local_var->n1n2_failure_txf_notif_uri = n1n2_failure_txf_notif_uri;
    n1_n2_message_transfer_req_data_local_var->smf_reallocation_ind = smf_reallocation_ind;
    n1_n2_message_transfer_req_data_local_var->area_of_validity = area_of_validity;
    n1_n2_message_transfer_req_data_local_var->supported_features = supported_features;
    n1_n2_message_transfer_req_data_local_var->old_guami = old_guami;
    n1_n2_message_transfer_req_data_local_var->ma_accepted_ind = ma_accepted_ind;
    n1_n2_message_transfer_req_data_local_var->ext_buf_support = ext_buf_support;
    n1_n2_message_transfer_req_data_local_var->target_access = target_access;

    return n1_n2_message_transfer_req_data_local_var;
}

void OpenAPI_n1_n2_message_transfer_req_data_free(OpenAPI_n1_n2_message_transfer_req_data_t *n1_n2_message_transfer_req_data)
{
    if (NULL == n1_n2_message_transfer_req_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_n1_message_container_free(n1_n2_message_transfer_req_data->n1_message_container);
    OpenAPI_n2_info_container_free(n1_n2_message_transfer_req_data->n2_info_container);
    OpenAPI_ref_to_binary_data_free(n1_n2_message_transfer_req_data->mt_data);
    ogs_free(n1_n2_message_transfer_req_data->lcs_correlation_id);
    OpenAPI_arp_free(n1_n2_message_transfer_req_data->arp);
    ogs_free(n1_n2_message_transfer_req_data->n1n2_failure_txf_notif_uri);
    OpenAPI_area_of_validity_free(n1_n2_message_transfer_req_data->area_of_validity);
    ogs_free(n1_n2_message_transfer_req_data->supported_features);
    OpenAPI_guami_free(n1_n2_message_transfer_req_data->old_guami);
    ogs_free(n1_n2_message_transfer_req_data);
}

cJSON *OpenAPI_n1_n2_message_transfer_req_data_convertToJSON(OpenAPI_n1_n2_message_transfer_req_data_t *n1_n2_message_transfer_req_data)
{
    cJSON *item = NULL;

    if (n1_n2_message_transfer_req_data == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [N1N2MessageTransferReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n1_n2_message_transfer_req_data->n1_message_container) {
        cJSON *n1_message_container_local_JSON = OpenAPI_n1_message_container_convertToJSON(n1_n2_message_transfer_req_data->n1_message_container);
        if (n1_message_container_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [n1_message_container]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n1MessageContainer", n1_message_container_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [n1_message_container]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->n2_info_container) {
        cJSON *n2_info_container_local_JSON = OpenAPI_n2_info_container_convertToJSON(n1_n2_message_transfer_req_data->n2_info_container);
        if (n2_info_container_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [n2_info_container]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n2InfoContainer", n2_info_container_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [n2_info_container]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->mt_data) {
        cJSON *mt_data_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(n1_n2_message_transfer_req_data->mt_data);
        if (mt_data_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [mt_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "mtData", mt_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [mt_data]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->skip_ind) {
        if (cJSON_AddBoolToObject(item, "skipInd", n1_n2_message_transfer_req_data->skip_ind) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [skip_ind]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->last_msg_indication) {
        if (cJSON_AddBoolToObject(item, "lastMsgIndication", n1_n2_message_transfer_req_data->last_msg_indication) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [last_msg_indication]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->pdu_session_id) {
        if (cJSON_AddNumberToObject(item, "pduSessionId", n1_n2_message_transfer_req_data->pdu_session_id) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [pdu_session_id]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->lcs_correlation_id) {
        if (cJSON_AddStringToObject(item, "lcsCorrelationId", n1_n2_message_transfer_req_data->lcs_correlation_id) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [lcs_correlation_id]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->ppi) {
        if (cJSON_AddNumberToObject(item, "ppi", n1_n2_message_transfer_req_data->ppi) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [ppi]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->arp) {
        cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(n1_n2_message_transfer_req_data->arp);
        if (arp_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [arp]");
            goto end;
        }
        cJSON_AddItemToObject(item, "arp", arp_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [arp]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->_5qi) {
        if (cJSON_AddNumberToObject(item, "5qi", n1_n2_message_transfer_req_data->_5qi) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [_5qi]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->n1n2_failure_txf_notif_uri) {
        if (cJSON_AddStringToObject(item, "n1n2FailureTxfNotifURI", n1_n2_message_transfer_req_data->n1n2_failure_txf_notif_uri) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [n1n2_failure_txf_notif_uri]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->smf_reallocation_ind) {
        if (cJSON_AddBoolToObject(item, "smfReallocationInd", n1_n2_message_transfer_req_data->smf_reallocation_ind) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [smf_reallocation_ind]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->area_of_validity) {
        cJSON *area_of_validity_local_JSON = OpenAPI_area_of_validity_convertToJSON(n1_n2_message_transfer_req_data->area_of_validity);
        if (area_of_validity_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [area_of_validity]");
            goto end;
        }
        cJSON_AddItemToObject(item, "areaOfValidity", area_of_validity_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [area_of_validity]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", n1_n2_message_transfer_req_data->supported_features) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->old_guami) {
        cJSON *old_guami_local_JSON = OpenAPI_guami_convertToJSON(n1_n2_message_transfer_req_data->old_guami);
        if (old_guami_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [old_guami]");
            goto end;
        }
        cJSON_AddItemToObject(item, "oldGuami", old_guami_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [old_guami]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->ma_accepted_ind) {
        if (cJSON_AddBoolToObject(item, "maAcceptedInd", n1_n2_message_transfer_req_data->ma_accepted_ind) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [ma_accepted_ind]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->ext_buf_support) {
        if (cJSON_AddBoolToObject(item, "extBufSupport", n1_n2_message_transfer_req_data->ext_buf_support) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [ext_buf_support]");
            goto end;
        }
    }

    if (n1_n2_message_transfer_req_data->target_access) {
        if (cJSON_AddStringToObject(item, "targetAccess", OpenAPI_access_type_ToString(n1_n2_message_transfer_req_data->target_access)) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed [target_access]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n1_n2_message_transfer_req_data_t *OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON(cJSON *n1_n2_message_transfer_req_dataJSON)
{
    OpenAPI_n1_n2_message_transfer_req_data_t *n1_n2_message_transfer_req_data_local_var = NULL;
    cJSON *n1_message_container = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "n1MessageContainer");

    OpenAPI_n1_message_container_t *n1_message_container_local_nonprim = NULL;
    if (n1_message_container) {
        n1_message_container_local_nonprim = OpenAPI_n1_message_container_parseFromJSON(n1_message_container);
    }

    cJSON *n2_info_container = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "n2InfoContainer");

    OpenAPI_n2_info_container_t *n2_info_container_local_nonprim = NULL;
    if (n2_info_container) {
        n2_info_container_local_nonprim = OpenAPI_n2_info_container_parseFromJSON(n2_info_container);
    }

    cJSON *mt_data = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "mtData");

    OpenAPI_ref_to_binary_data_t *mt_data_local_nonprim = NULL;
    if (mt_data) {
        mt_data_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(mt_data);
    }

    cJSON *skip_ind = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "skipInd");

    if (skip_ind) {
        if (!cJSON_IsBool(skip_ind)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [skip_ind]");
            goto end;
        }
    }

    cJSON *last_msg_indication = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "lastMsgIndication");

    if (last_msg_indication) {
        if (!cJSON_IsBool(last_msg_indication)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [last_msg_indication]");
            goto end;
        }
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "pduSessionId");

    if (pdu_session_id) {
        if (!cJSON_IsNumber(pdu_session_id)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [pdu_session_id]");
            goto end;
        }
    }

    cJSON *lcs_correlation_id = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "lcsCorrelationId");

    if (lcs_correlation_id) {
        if (!cJSON_IsString(lcs_correlation_id)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [lcs_correlation_id]");
            goto end;
        }
    }

    cJSON *ppi = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "ppi");

    if (ppi) {
        if (!cJSON_IsNumber(ppi)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [ppi]");
            goto end;
        }
    }

    cJSON *arp = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "arp");

    OpenAPI_arp_t *arp_local_nonprim = NULL;
    if (arp) {
        arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);
    }

    cJSON *_5qi = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "5qi");

    if (_5qi) {
        if (!cJSON_IsNumber(_5qi)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [_5qi]");
            goto end;
        }
    }

    cJSON *n1n2_failure_txf_notif_uri = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "n1n2FailureTxfNotifURI");

    if (n1n2_failure_txf_notif_uri) {
        if (!cJSON_IsString(n1n2_failure_txf_notif_uri)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [n1n2_failure_txf_notif_uri]");
            goto end;
        }
    }

    cJSON *smf_reallocation_ind = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "smfReallocationInd");

    if (smf_reallocation_ind) {
        if (!cJSON_IsBool(smf_reallocation_ind)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [smf_reallocation_ind]");
            goto end;
        }
    }

    cJSON *area_of_validity = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "areaOfValidity");

    OpenAPI_area_of_validity_t *area_of_validity_local_nonprim = NULL;
    if (area_of_validity) {
        area_of_validity_local_nonprim = OpenAPI_area_of_validity_parseFromJSON(area_of_validity);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *old_guami = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "oldGuami");

    OpenAPI_guami_t *old_guami_local_nonprim = NULL;
    if (old_guami) {
        old_guami_local_nonprim = OpenAPI_guami_parseFromJSON(old_guami);
    }

    cJSON *ma_accepted_ind = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "maAcceptedInd");

    if (ma_accepted_ind) {
        if (!cJSON_IsBool(ma_accepted_ind)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [ma_accepted_ind]");
            goto end;
        }
    }

    cJSON *ext_buf_support = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "extBufSupport");

    if (ext_buf_support) {
        if (!cJSON_IsBool(ext_buf_support)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [ext_buf_support]");
            goto end;
        }
    }

    cJSON *target_access = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_req_dataJSON, "targetAccess");

    OpenAPI_access_type_e target_accessVariable;
    if (target_access) {
        if (!cJSON_IsString(target_access)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON() failed [target_access]");
            goto end;
        }
        target_accessVariable = OpenAPI_access_type_FromString(target_access->valuestring);
    }

    n1_n2_message_transfer_req_data_local_var = OpenAPI_n1_n2_message_transfer_req_data_create (
        n1_message_container ? n1_message_container_local_nonprim : NULL,
        n2_info_container ? n2_info_container_local_nonprim : NULL,
        mt_data ? mt_data_local_nonprim : NULL,
        skip_ind ? skip_ind->valueint : 0,
        last_msg_indication ? last_msg_indication->valueint : 0,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        lcs_correlation_id ? ogs_strdup(lcs_correlation_id->valuestring) : NULL,
        ppi ? ppi->valuedouble : 0,
        arp ? arp_local_nonprim : NULL,
        _5qi ? _5qi->valuedouble : 0,
        n1n2_failure_txf_notif_uri ? ogs_strdup(n1n2_failure_txf_notif_uri->valuestring) : NULL,
        smf_reallocation_ind ? smf_reallocation_ind->valueint : 0,
        area_of_validity ? area_of_validity_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        old_guami ? old_guami_local_nonprim : NULL,
        ma_accepted_ind ? ma_accepted_ind->valueint : 0,
        ext_buf_support ? ext_buf_support->valueint : 0,
        target_access ? target_accessVariable : 0
        );

    return n1_n2_message_transfer_req_data_local_var;
end:
    return NULL;
}

OpenAPI_n1_n2_message_transfer_req_data_t *OpenAPI_n1_n2_message_transfer_req_data_copy(OpenAPI_n1_n2_message_transfer_req_data_t *dst, OpenAPI_n1_n2_message_transfer_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_n2_message_transfer_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_n2_message_transfer_req_data_convertToJSON() failed");
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

    OpenAPI_n1_n2_message_transfer_req_data_free(dst);
    dst = OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

