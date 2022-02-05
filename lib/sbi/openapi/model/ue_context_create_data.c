
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_create_data.h"

OpenAPI_ue_context_create_data_t *OpenAPI_ue_context_create_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_ng_ran_target_id_t *target_id,
    OpenAPI_n2_info_content_t *source_to_target_data,
    OpenAPI_list_t *pdu_session_list,
    char *n2_notify_uri,
    OpenAPI_n2_info_content_t *ue_radio_capability,
    OpenAPI_ng_ap_cause_t *ngap_cause,
    char *supported_features,
    OpenAPI_plmn_id_nid_t *serving_network
)
{
    OpenAPI_ue_context_create_data_t *ue_context_create_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_create_data_t));
    ogs_assert(ue_context_create_data_local_var);

    ue_context_create_data_local_var->ue_context = ue_context;
    ue_context_create_data_local_var->target_id = target_id;
    ue_context_create_data_local_var->source_to_target_data = source_to_target_data;
    ue_context_create_data_local_var->pdu_session_list = pdu_session_list;
    ue_context_create_data_local_var->n2_notify_uri = n2_notify_uri;
    ue_context_create_data_local_var->ue_radio_capability = ue_radio_capability;
    ue_context_create_data_local_var->ngap_cause = ngap_cause;
    ue_context_create_data_local_var->supported_features = supported_features;
    ue_context_create_data_local_var->serving_network = serving_network;

    return ue_context_create_data_local_var;
}

void OpenAPI_ue_context_create_data_free(OpenAPI_ue_context_create_data_t *ue_context_create_data)
{
    if (NULL == ue_context_create_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ue_context_free(ue_context_create_data->ue_context);
    OpenAPI_ng_ran_target_id_free(ue_context_create_data->target_id);
    OpenAPI_n2_info_content_free(ue_context_create_data->source_to_target_data);
    OpenAPI_list_for_each(ue_context_create_data->pdu_session_list, node) {
        OpenAPI_n2_sm_information_free(node->data);
    }
    OpenAPI_list_free(ue_context_create_data->pdu_session_list);
    ogs_free(ue_context_create_data->n2_notify_uri);
    OpenAPI_n2_info_content_free(ue_context_create_data->ue_radio_capability);
    OpenAPI_ng_ap_cause_free(ue_context_create_data->ngap_cause);
    ogs_free(ue_context_create_data->supported_features);
    OpenAPI_plmn_id_nid_free(ue_context_create_data->serving_network);
    ogs_free(ue_context_create_data);
}

cJSON *OpenAPI_ue_context_create_data_convertToJSON(OpenAPI_ue_context_create_data_t *ue_context_create_data)
{
    cJSON *item = NULL;

    if (ue_context_create_data == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [UeContextCreateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *ue_context_local_JSON = OpenAPI_ue_context_convertToJSON(ue_context_create_data->ue_context);
    if (ue_context_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [ue_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueContext", ue_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [ue_context]");
        goto end;
    }

    cJSON *target_id_local_JSON = OpenAPI_ng_ran_target_id_convertToJSON(ue_context_create_data->target_id);
    if (target_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [target_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetId", target_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [target_id]");
        goto end;
    }

    cJSON *source_to_target_data_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_create_data->source_to_target_data);
    if (source_to_target_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [source_to_target_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sourceToTargetData", source_to_target_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [source_to_target_data]");
        goto end;
    }

    cJSON *pdu_session_listList = cJSON_AddArrayToObject(item, "pduSessionList");
    if (pdu_session_listList == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [pdu_session_list]");
        goto end;
    }

    OpenAPI_lnode_t *pdu_session_list_node;
    if (ue_context_create_data->pdu_session_list) {
        OpenAPI_list_for_each(ue_context_create_data->pdu_session_list, pdu_session_list_node) {
            cJSON *itemLocal = OpenAPI_n2_sm_information_convertToJSON(pdu_session_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [pdu_session_list]");
                goto end;
            }
            cJSON_AddItemToArray(pdu_session_listList, itemLocal);
        }
    }

    if (ue_context_create_data->n2_notify_uri) {
    if (cJSON_AddStringToObject(item, "n2NotifyUri", ue_context_create_data->n2_notify_uri) == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [n2_notify_uri]");
        goto end;
    }
    }

    if (ue_context_create_data->ue_radio_capability) {
    cJSON *ue_radio_capability_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_create_data->ue_radio_capability);
    if (ue_radio_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [ue_radio_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueRadioCapability", ue_radio_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [ue_radio_capability]");
        goto end;
    }
    }

    if (ue_context_create_data->ngap_cause) {
    cJSON *ngap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(ue_context_create_data->ngap_cause);
    if (ngap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [ngap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngapCause", ngap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [ngap_cause]");
        goto end;
    }
    }

    if (ue_context_create_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ue_context_create_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (ue_context_create_data->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(ue_context_create_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_create_data_t *OpenAPI_ue_context_create_data_parseFromJSON(cJSON *ue_context_create_dataJSON)
{
    OpenAPI_ue_context_create_data_t *ue_context_create_data_local_var = NULL;
    cJSON *ue_context = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "ueContext");
    if (!ue_context) {
        ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [ue_context]");
        goto end;
    }

    OpenAPI_ue_context_t *ue_context_local_nonprim = NULL;
    ue_context_local_nonprim = OpenAPI_ue_context_parseFromJSON(ue_context);

    cJSON *target_id = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "targetId");
    if (!target_id) {
        ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [target_id]");
        goto end;
    }

    OpenAPI_ng_ran_target_id_t *target_id_local_nonprim = NULL;
    target_id_local_nonprim = OpenAPI_ng_ran_target_id_parseFromJSON(target_id);

    cJSON *source_to_target_data = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "sourceToTargetData");
    if (!source_to_target_data) {
        ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [source_to_target_data]");
        goto end;
    }

    OpenAPI_n2_info_content_t *source_to_target_data_local_nonprim = NULL;
    source_to_target_data_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(source_to_target_data);

    cJSON *pdu_session_list = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "pduSessionList");
    if (!pdu_session_list) {
        ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [pdu_session_list]");
        goto end;
    }

    OpenAPI_list_t *pdu_session_listList;
    cJSON *pdu_session_list_local_nonprimitive;
    if (!cJSON_IsArray(pdu_session_list)){
        ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [pdu_session_list]");
        goto end;
    }

    pdu_session_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(pdu_session_list_local_nonprimitive, pdu_session_list ) {
        if (!cJSON_IsObject(pdu_session_list_local_nonprimitive)) {
            ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [pdu_session_list]");
            goto end;
        }
        OpenAPI_n2_sm_information_t *pdu_session_listItem = OpenAPI_n2_sm_information_parseFromJSON(pdu_session_list_local_nonprimitive);

        if (!pdu_session_listItem) {
            ogs_error("No pdu_session_listItem");
            OpenAPI_list_free(pdu_session_listList);
            goto end;
        }

        OpenAPI_list_add(pdu_session_listList, pdu_session_listItem);
    }

    cJSON *n2_notify_uri = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "n2NotifyUri");

    if (n2_notify_uri) {
    if (!cJSON_IsString(n2_notify_uri)) {
        ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [n2_notify_uri]");
        goto end;
    }
    }

    cJSON *ue_radio_capability = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "ueRadioCapability");

    OpenAPI_n2_info_content_t *ue_radio_capability_local_nonprim = NULL;
    if (ue_radio_capability) {
    ue_radio_capability_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(ue_radio_capability);
    }

    cJSON *ngap_cause = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "ngapCause");

    OpenAPI_ng_ap_cause_t *ngap_cause_local_nonprim = NULL;
    if (ngap_cause) {
    ngap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ngap_cause);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_ue_context_create_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(ue_context_create_dataJSON, "servingNetwork");

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    }

    ue_context_create_data_local_var = OpenAPI_ue_context_create_data_create (
        ue_context_local_nonprim,
        target_id_local_nonprim,
        source_to_target_data_local_nonprim,
        pdu_session_listList,
        n2_notify_uri ? ogs_strdup(n2_notify_uri->valuestring) : NULL,
        ue_radio_capability ? ue_radio_capability_local_nonprim : NULL,
        ngap_cause ? ngap_cause_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        serving_network ? serving_network_local_nonprim : NULL
    );

    return ue_context_create_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_create_data_t *OpenAPI_ue_context_create_data_copy(OpenAPI_ue_context_create_data_t *dst, OpenAPI_ue_context_create_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_create_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_create_data_convertToJSON() failed");
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

    OpenAPI_ue_context_create_data_free(dst);
    dst = OpenAPI_ue_context_create_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

