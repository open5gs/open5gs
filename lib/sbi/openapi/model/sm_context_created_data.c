
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_created_data.h"

OpenAPI_sm_context_created_data_t *OpenAPI_sm_context_created_data_create(
    char *h_smf_uri,
    char *smf_uri,
    bool is_pdu_session_id,
    int pdu_session_id,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_up_cnx_state_e up_cnx_state,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    OpenAPI_list_t *allocated_ebi_list,
    OpenAPI_ho_state_e ho_state,
    char *gpsi,
    char *smf_service_instance_id,
    char *recovery_time,
    char *supported_features,
    char *selected_smf_id,
    char *selected_old_smf_id
)
{
    OpenAPI_sm_context_created_data_t *sm_context_created_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_created_data_t));
    ogs_assert(sm_context_created_data_local_var);

    sm_context_created_data_local_var->h_smf_uri = h_smf_uri;
    sm_context_created_data_local_var->smf_uri = smf_uri;
    sm_context_created_data_local_var->is_pdu_session_id = is_pdu_session_id;
    sm_context_created_data_local_var->pdu_session_id = pdu_session_id;
    sm_context_created_data_local_var->s_nssai = s_nssai;
    sm_context_created_data_local_var->up_cnx_state = up_cnx_state;
    sm_context_created_data_local_var->n2_sm_info = n2_sm_info;
    sm_context_created_data_local_var->n2_sm_info_type = n2_sm_info_type;
    sm_context_created_data_local_var->allocated_ebi_list = allocated_ebi_list;
    sm_context_created_data_local_var->ho_state = ho_state;
    sm_context_created_data_local_var->gpsi = gpsi;
    sm_context_created_data_local_var->smf_service_instance_id = smf_service_instance_id;
    sm_context_created_data_local_var->recovery_time = recovery_time;
    sm_context_created_data_local_var->supported_features = supported_features;
    sm_context_created_data_local_var->selected_smf_id = selected_smf_id;
    sm_context_created_data_local_var->selected_old_smf_id = selected_old_smf_id;

    return sm_context_created_data_local_var;
}

void OpenAPI_sm_context_created_data_free(OpenAPI_sm_context_created_data_t *sm_context_created_data)
{
    if (NULL == sm_context_created_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_context_created_data->h_smf_uri);
    ogs_free(sm_context_created_data->smf_uri);
    OpenAPI_snssai_free(sm_context_created_data->s_nssai);
    OpenAPI_ref_to_binary_data_free(sm_context_created_data->n2_sm_info);
    OpenAPI_list_for_each(sm_context_created_data->allocated_ebi_list, node) {
        OpenAPI_ebi_arp_mapping_free(node->data);
    }
    OpenAPI_list_free(sm_context_created_data->allocated_ebi_list);
    ogs_free(sm_context_created_data->gpsi);
    ogs_free(sm_context_created_data->smf_service_instance_id);
    ogs_free(sm_context_created_data->recovery_time);
    ogs_free(sm_context_created_data->supported_features);
    ogs_free(sm_context_created_data->selected_smf_id);
    ogs_free(sm_context_created_data->selected_old_smf_id);
    ogs_free(sm_context_created_data);
}

cJSON *OpenAPI_sm_context_created_data_convertToJSON(OpenAPI_sm_context_created_data_t *sm_context_created_data)
{
    cJSON *item = NULL;

    if (sm_context_created_data == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [SmContextCreatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_context_created_data->h_smf_uri) {
    if (cJSON_AddStringToObject(item, "hSmfUri", sm_context_created_data->h_smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [h_smf_uri]");
        goto end;
    }
    }

    if (sm_context_created_data->smf_uri) {
    if (cJSON_AddStringToObject(item, "smfUri", sm_context_created_data->smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [smf_uri]");
        goto end;
    }
    }

    if (sm_context_created_data->is_pdu_session_id) {
    if (cJSON_AddNumberToObject(item, "pduSessionId", sm_context_created_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    if (sm_context_created_data->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context_created_data->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (sm_context_created_data->up_cnx_state) {
    if (cJSON_AddStringToObject(item, "upCnxState", OpenAPI_up_cnx_state_ToString(sm_context_created_data->up_cnx_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [up_cnx_state]");
        goto end;
    }
    }

    if (sm_context_created_data->n2_sm_info) {
    cJSON *n2_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_created_data->n2_sm_info);
    if (n2_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2SmInfo", n2_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    }

    if (sm_context_created_data->n2_sm_info_type) {
    if (cJSON_AddStringToObject(item, "n2SmInfoType", OpenAPI_n2_sm_info_type_ToString(sm_context_created_data->n2_sm_info_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [n2_sm_info_type]");
        goto end;
    }
    }

    if (sm_context_created_data->allocated_ebi_list) {
    cJSON *allocated_ebi_listList = cJSON_AddArrayToObject(item, "allocatedEbiList");
    if (allocated_ebi_listList == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [allocated_ebi_list]");
        goto end;
    }

    OpenAPI_lnode_t *allocated_ebi_list_node;
    if (sm_context_created_data->allocated_ebi_list) {
        OpenAPI_list_for_each(sm_context_created_data->allocated_ebi_list, allocated_ebi_list_node) {
            cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(allocated_ebi_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [allocated_ebi_list]");
                goto end;
            }
            cJSON_AddItemToArray(allocated_ebi_listList, itemLocal);
        }
    }
    }

    if (sm_context_created_data->ho_state) {
    if (cJSON_AddStringToObject(item, "hoState", OpenAPI_ho_state_ToString(sm_context_created_data->ho_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [ho_state]");
        goto end;
    }
    }

    if (sm_context_created_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", sm_context_created_data->gpsi) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (sm_context_created_data->smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "smfServiceInstanceId", sm_context_created_data->smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [smf_service_instance_id]");
        goto end;
    }
    }

    if (sm_context_created_data->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", sm_context_created_data->recovery_time) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

    if (sm_context_created_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sm_context_created_data->supported_features) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (sm_context_created_data->selected_smf_id) {
    if (cJSON_AddStringToObject(item, "selectedSmfId", sm_context_created_data->selected_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [selected_smf_id]");
        goto end;
    }
    }

    if (sm_context_created_data->selected_old_smf_id) {
    if (cJSON_AddStringToObject(item, "selectedOldSmfId", sm_context_created_data->selected_old_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed [selected_old_smf_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_created_data_t *OpenAPI_sm_context_created_data_parseFromJSON(cJSON *sm_context_created_dataJSON)
{
    OpenAPI_sm_context_created_data_t *sm_context_created_data_local_var = NULL;
    cJSON *h_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "hSmfUri");

    if (h_smf_uri) {
    if (!cJSON_IsString(h_smf_uri)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [h_smf_uri]");
        goto end;
    }
    }

    cJSON *smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "smfUri");

    if (smf_uri) {
    if (!cJSON_IsString(smf_uri)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [smf_uri]");
        goto end;
    }
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "pduSessionId");

    if (pdu_session_id) {
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "sNssai");

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    }

    cJSON *up_cnx_state = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "upCnxState");

    OpenAPI_up_cnx_state_e up_cnx_stateVariable;
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    cJSON *n2_sm_info = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "n2SmInfo");

    OpenAPI_ref_to_binary_data_t *n2_sm_info_local_nonprim = NULL;
    if (n2_sm_info) {
    n2_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info);
    }

    cJSON *n2_sm_info_type = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "n2SmInfoType");

    OpenAPI_n2_sm_info_type_e n2_sm_info_typeVariable;
    if (n2_sm_info_type) {
    if (!cJSON_IsString(n2_sm_info_type)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [n2_sm_info_type]");
        goto end;
    }
    n2_sm_info_typeVariable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type->valuestring);
    }

    cJSON *allocated_ebi_list = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "allocatedEbiList");

    OpenAPI_list_t *allocated_ebi_listList;
    if (allocated_ebi_list) {
    cJSON *allocated_ebi_list_local_nonprimitive;
    if (!cJSON_IsArray(allocated_ebi_list)){
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [allocated_ebi_list]");
        goto end;
    }

    allocated_ebi_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(allocated_ebi_list_local_nonprimitive, allocated_ebi_list ) {
        if (!cJSON_IsObject(allocated_ebi_list_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [allocated_ebi_list]");
            goto end;
        }
        OpenAPI_ebi_arp_mapping_t *allocated_ebi_listItem = OpenAPI_ebi_arp_mapping_parseFromJSON(allocated_ebi_list_local_nonprimitive);

        if (!allocated_ebi_listItem) {
            ogs_error("No allocated_ebi_listItem");
            OpenAPI_list_free(allocated_ebi_listList);
            goto end;
        }

        OpenAPI_list_add(allocated_ebi_listList, allocated_ebi_listItem);
    }
    }

    cJSON *ho_state = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "hoState");

    OpenAPI_ho_state_e ho_stateVariable;
    if (ho_state) {
    if (!cJSON_IsString(ho_state)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [ho_state]");
        goto end;
    }
    ho_stateVariable = OpenAPI_ho_state_FromString(ho_state->valuestring);
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "gpsi");

    if (gpsi) {
    if (!cJSON_IsString(gpsi)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    cJSON *smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "smfServiceInstanceId");

    if (smf_service_instance_id) {
    if (!cJSON_IsString(smf_service_instance_id)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [smf_service_instance_id]");
        goto end;
    }
    }

    cJSON *recovery_time = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "recoveryTime");

    if (recovery_time) {
    if (!cJSON_IsString(recovery_time)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *selected_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "selectedSmfId");

    if (selected_smf_id) {
    if (!cJSON_IsString(selected_smf_id)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [selected_smf_id]");
        goto end;
    }
    }

    cJSON *selected_old_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_created_dataJSON, "selectedOldSmfId");

    if (selected_old_smf_id) {
    if (!cJSON_IsString(selected_old_smf_id)) {
        ogs_error("OpenAPI_sm_context_created_data_parseFromJSON() failed [selected_old_smf_id]");
        goto end;
    }
    }

    sm_context_created_data_local_var = OpenAPI_sm_context_created_data_create (
        h_smf_uri ? ogs_strdup(h_smf_uri->valuestring) : NULL,
        smf_uri ? ogs_strdup(smf_uri->valuestring) : NULL,
        pdu_session_id ? true : false,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        s_nssai ? s_nssai_local_nonprim : NULL,
        up_cnx_state ? up_cnx_stateVariable : 0,
        n2_sm_info ? n2_sm_info_local_nonprim : NULL,
        n2_sm_info_type ? n2_sm_info_typeVariable : 0,
        allocated_ebi_list ? allocated_ebi_listList : NULL,
        ho_state ? ho_stateVariable : 0,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        smf_service_instance_id ? ogs_strdup(smf_service_instance_id->valuestring) : NULL,
        recovery_time ? ogs_strdup(recovery_time->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        selected_smf_id ? ogs_strdup(selected_smf_id->valuestring) : NULL,
        selected_old_smf_id ? ogs_strdup(selected_old_smf_id->valuestring) : NULL
    );

    return sm_context_created_data_local_var;
end:
    return NULL;
}

OpenAPI_sm_context_created_data_t *OpenAPI_sm_context_created_data_copy(OpenAPI_sm_context_created_data_t *dst, OpenAPI_sm_context_created_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_created_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_created_data_convertToJSON() failed");
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

    OpenAPI_sm_context_created_data_free(dst);
    dst = OpenAPI_sm_context_created_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

