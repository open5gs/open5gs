
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_relocate_data.h"

OpenAPI_ue_context_relocate_data_t *OpenAPI_ue_context_relocate_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_ng_ran_target_id_t *target_id,
    OpenAPI_n2_info_content_t *source_to_target_data,
    OpenAPI_ref_to_binary_data_t *forward_relocation_request,
    OpenAPI_list_t *pdu_session_list,
    OpenAPI_n2_info_content_t *ue_radio_capability,
    OpenAPI_ng_ap_cause_t *ngap_cause,
    char *supported_features
)
{
    OpenAPI_ue_context_relocate_data_t *ue_context_relocate_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_relocate_data_t));
    ogs_assert(ue_context_relocate_data_local_var);

    ue_context_relocate_data_local_var->ue_context = ue_context;
    ue_context_relocate_data_local_var->target_id = target_id;
    ue_context_relocate_data_local_var->source_to_target_data = source_to_target_data;
    ue_context_relocate_data_local_var->forward_relocation_request = forward_relocation_request;
    ue_context_relocate_data_local_var->pdu_session_list = pdu_session_list;
    ue_context_relocate_data_local_var->ue_radio_capability = ue_radio_capability;
    ue_context_relocate_data_local_var->ngap_cause = ngap_cause;
    ue_context_relocate_data_local_var->supported_features = supported_features;

    return ue_context_relocate_data_local_var;
}

void OpenAPI_ue_context_relocate_data_free(OpenAPI_ue_context_relocate_data_t *ue_context_relocate_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_relocate_data) {
        return;
    }
    if (ue_context_relocate_data->ue_context) {
        OpenAPI_ue_context_free(ue_context_relocate_data->ue_context);
        ue_context_relocate_data->ue_context = NULL;
    }
    if (ue_context_relocate_data->target_id) {
        OpenAPI_ng_ran_target_id_free(ue_context_relocate_data->target_id);
        ue_context_relocate_data->target_id = NULL;
    }
    if (ue_context_relocate_data->source_to_target_data) {
        OpenAPI_n2_info_content_free(ue_context_relocate_data->source_to_target_data);
        ue_context_relocate_data->source_to_target_data = NULL;
    }
    if (ue_context_relocate_data->forward_relocation_request) {
        OpenAPI_ref_to_binary_data_free(ue_context_relocate_data->forward_relocation_request);
        ue_context_relocate_data->forward_relocation_request = NULL;
    }
    if (ue_context_relocate_data->pdu_session_list) {
        OpenAPI_list_for_each(ue_context_relocate_data->pdu_session_list, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(ue_context_relocate_data->pdu_session_list);
        ue_context_relocate_data->pdu_session_list = NULL;
    }
    if (ue_context_relocate_data->ue_radio_capability) {
        OpenAPI_n2_info_content_free(ue_context_relocate_data->ue_radio_capability);
        ue_context_relocate_data->ue_radio_capability = NULL;
    }
    if (ue_context_relocate_data->ngap_cause) {
        OpenAPI_ng_ap_cause_free(ue_context_relocate_data->ngap_cause);
        ue_context_relocate_data->ngap_cause = NULL;
    }
    if (ue_context_relocate_data->supported_features) {
        ogs_free(ue_context_relocate_data->supported_features);
        ue_context_relocate_data->supported_features = NULL;
    }
    ogs_free(ue_context_relocate_data);
}

cJSON *OpenAPI_ue_context_relocate_data_convertToJSON(OpenAPI_ue_context_relocate_data_t *ue_context_relocate_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_relocate_data == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [UeContextRelocateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_context_relocate_data->ue_context) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [ue_context]");
        return NULL;
    }
    cJSON *ue_context_local_JSON = OpenAPI_ue_context_convertToJSON(ue_context_relocate_data->ue_context);
    if (ue_context_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [ue_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueContext", ue_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [ue_context]");
        goto end;
    }

    if (!ue_context_relocate_data->target_id) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [target_id]");
        return NULL;
    }
    cJSON *target_id_local_JSON = OpenAPI_ng_ran_target_id_convertToJSON(ue_context_relocate_data->target_id);
    if (target_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [target_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetId", target_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [target_id]");
        goto end;
    }

    if (!ue_context_relocate_data->source_to_target_data) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [source_to_target_data]");
        return NULL;
    }
    cJSON *source_to_target_data_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_relocate_data->source_to_target_data);
    if (source_to_target_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [source_to_target_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sourceToTargetData", source_to_target_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [source_to_target_data]");
        goto end;
    }

    if (!ue_context_relocate_data->forward_relocation_request) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [forward_relocation_request]");
        return NULL;
    }
    cJSON *forward_relocation_request_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(ue_context_relocate_data->forward_relocation_request);
    if (forward_relocation_request_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [forward_relocation_request]");
        goto end;
    }
    cJSON_AddItemToObject(item, "forwardRelocationRequest", forward_relocation_request_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [forward_relocation_request]");
        goto end;
    }

    if (ue_context_relocate_data->pdu_session_list) {
    cJSON *pdu_session_listList = cJSON_AddArrayToObject(item, "pduSessionList");
    if (pdu_session_listList == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [pdu_session_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context_relocate_data->pdu_session_list, node) {
        cJSON *itemLocal = OpenAPI_n2_sm_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [pdu_session_list]");
            goto end;
        }
        cJSON_AddItemToArray(pdu_session_listList, itemLocal);
    }
    }

    if (ue_context_relocate_data->ue_radio_capability) {
    cJSON *ue_radio_capability_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_relocate_data->ue_radio_capability);
    if (ue_radio_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [ue_radio_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueRadioCapability", ue_radio_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [ue_radio_capability]");
        goto end;
    }
    }

    if (ue_context_relocate_data->ngap_cause) {
    cJSON *ngap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(ue_context_relocate_data->ngap_cause);
    if (ngap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [ngap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngapCause", ngap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [ngap_cause]");
        goto end;
    }
    }

    if (ue_context_relocate_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ue_context_relocate_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_relocate_data_t *OpenAPI_ue_context_relocate_data_parseFromJSON(cJSON *ue_context_relocate_dataJSON)
{
    OpenAPI_ue_context_relocate_data_t *ue_context_relocate_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_context = NULL;
    OpenAPI_ue_context_t *ue_context_local_nonprim = NULL;
    cJSON *target_id = NULL;
    OpenAPI_ng_ran_target_id_t *target_id_local_nonprim = NULL;
    cJSON *source_to_target_data = NULL;
    OpenAPI_n2_info_content_t *source_to_target_data_local_nonprim = NULL;
    cJSON *forward_relocation_request = NULL;
    OpenAPI_ref_to_binary_data_t *forward_relocation_request_local_nonprim = NULL;
    cJSON *pdu_session_list = NULL;
    OpenAPI_list_t *pdu_session_listList = NULL;
    cJSON *ue_radio_capability = NULL;
    OpenAPI_n2_info_content_t *ue_radio_capability_local_nonprim = NULL;
    cJSON *ngap_cause = NULL;
    OpenAPI_ng_ap_cause_t *ngap_cause_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    ue_context = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "ueContext");
    if (!ue_context) {
        ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON() failed [ue_context]");
        goto end;
    }
    ue_context_local_nonprim = OpenAPI_ue_context_parseFromJSON(ue_context);
    if (!ue_context_local_nonprim) {
        ogs_error("OpenAPI_ue_context_parseFromJSON failed [ue_context]");
        goto end;
    }

    target_id = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "targetId");
    if (!target_id) {
        ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON() failed [target_id]");
        goto end;
    }
    target_id_local_nonprim = OpenAPI_ng_ran_target_id_parseFromJSON(target_id);
    if (!target_id_local_nonprim) {
        ogs_error("OpenAPI_ng_ran_target_id_parseFromJSON failed [target_id]");
        goto end;
    }

    source_to_target_data = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "sourceToTargetData");
    if (!source_to_target_data) {
        ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON() failed [source_to_target_data]");
        goto end;
    }
    source_to_target_data_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(source_to_target_data);
    if (!source_to_target_data_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [source_to_target_data]");
        goto end;
    }

    forward_relocation_request = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "forwardRelocationRequest");
    if (!forward_relocation_request) {
        ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON() failed [forward_relocation_request]");
        goto end;
    }
    forward_relocation_request_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(forward_relocation_request);
    if (!forward_relocation_request_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [forward_relocation_request]");
        goto end;
    }

    pdu_session_list = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "pduSessionList");
    if (pdu_session_list) {
        cJSON *pdu_session_list_local = NULL;
        if (!cJSON_IsArray(pdu_session_list)) {
            ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON() failed [pdu_session_list]");
            goto end;
        }

        pdu_session_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_list_local, pdu_session_list) {
            if (!cJSON_IsObject(pdu_session_list_local)) {
                ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON() failed [pdu_session_list]");
                goto end;
            }
            OpenAPI_n2_sm_information_t *pdu_session_listItem = OpenAPI_n2_sm_information_parseFromJSON(pdu_session_list_local);
            if (!pdu_session_listItem) {
                ogs_error("No pdu_session_listItem");
                goto end;
            }
            OpenAPI_list_add(pdu_session_listList, pdu_session_listItem);
        }
    }

    ue_radio_capability = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "ueRadioCapability");
    if (ue_radio_capability) {
    ue_radio_capability_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(ue_radio_capability);
    if (!ue_radio_capability_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [ue_radio_capability]");
        goto end;
    }
    }

    ngap_cause = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "ngapCause");
    if (ngap_cause) {
    ngap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ngap_cause);
    if (!ngap_cause_local_nonprim) {
        ogs_error("OpenAPI_ng_ap_cause_parseFromJSON failed [ngap_cause]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(ue_context_relocate_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    ue_context_relocate_data_local_var = OpenAPI_ue_context_relocate_data_create (
        ue_context_local_nonprim,
        target_id_local_nonprim,
        source_to_target_data_local_nonprim,
        forward_relocation_request_local_nonprim,
        pdu_session_list ? pdu_session_listList : NULL,
        ue_radio_capability ? ue_radio_capability_local_nonprim : NULL,
        ngap_cause ? ngap_cause_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return ue_context_relocate_data_local_var;
end:
    if (ue_context_local_nonprim) {
        OpenAPI_ue_context_free(ue_context_local_nonprim);
        ue_context_local_nonprim = NULL;
    }
    if (target_id_local_nonprim) {
        OpenAPI_ng_ran_target_id_free(target_id_local_nonprim);
        target_id_local_nonprim = NULL;
    }
    if (source_to_target_data_local_nonprim) {
        OpenAPI_n2_info_content_free(source_to_target_data_local_nonprim);
        source_to_target_data_local_nonprim = NULL;
    }
    if (forward_relocation_request_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(forward_relocation_request_local_nonprim);
        forward_relocation_request_local_nonprim = NULL;
    }
    if (pdu_session_listList) {
        OpenAPI_list_for_each(pdu_session_listList, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(pdu_session_listList);
        pdu_session_listList = NULL;
    }
    if (ue_radio_capability_local_nonprim) {
        OpenAPI_n2_info_content_free(ue_radio_capability_local_nonprim);
        ue_radio_capability_local_nonprim = NULL;
    }
    if (ngap_cause_local_nonprim) {
        OpenAPI_ng_ap_cause_free(ngap_cause_local_nonprim);
        ngap_cause_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_relocate_data_t *OpenAPI_ue_context_relocate_data_copy(OpenAPI_ue_context_relocate_data_t *dst, OpenAPI_ue_context_relocate_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_relocate_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_relocate_data_convertToJSON() failed");
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

    OpenAPI_ue_context_relocate_data_free(dst);
    dst = OpenAPI_ue_context_relocate_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

