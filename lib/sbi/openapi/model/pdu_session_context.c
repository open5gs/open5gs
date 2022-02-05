
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_context.h"

OpenAPI_pdu_session_context_t *OpenAPI_pdu_session_context_create(
    int pdu_session_id,
    char *sm_context_ref,
    OpenAPI_snssai_t *s_nssai,
    char *dnn,
    char *selected_dnn,
    OpenAPI_access_type_e access_type,
    OpenAPI_access_type_e additional_access_type,
    OpenAPI_list_t *allocated_ebi_list,
    char *hsmf_id,
    char *hsmf_set_id,
    char *hsmf_service_set_id,
    OpenAPI_sbi_binding_level_e smf_binding,
    char *vsmf_id,
    char *vsmf_set_id,
    char *vsmf_service_set_id,
    OpenAPI_sbi_binding_level_e vsmf_binding,
    char *ismf_id,
    char *ismf_set_id,
    char *ismf_service_set_id,
    OpenAPI_sbi_binding_level_e ismf_binding,
    char *ns_instance,
    char *smf_service_instance_id,
    bool is_ma_pdu_session,
    int ma_pdu_session,
    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para
)
{
    OpenAPI_pdu_session_context_t *pdu_session_context_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_context_t));
    ogs_assert(pdu_session_context_local_var);

    pdu_session_context_local_var->pdu_session_id = pdu_session_id;
    pdu_session_context_local_var->sm_context_ref = sm_context_ref;
    pdu_session_context_local_var->s_nssai = s_nssai;
    pdu_session_context_local_var->dnn = dnn;
    pdu_session_context_local_var->selected_dnn = selected_dnn;
    pdu_session_context_local_var->access_type = access_type;
    pdu_session_context_local_var->additional_access_type = additional_access_type;
    pdu_session_context_local_var->allocated_ebi_list = allocated_ebi_list;
    pdu_session_context_local_var->hsmf_id = hsmf_id;
    pdu_session_context_local_var->hsmf_set_id = hsmf_set_id;
    pdu_session_context_local_var->hsmf_service_set_id = hsmf_service_set_id;
    pdu_session_context_local_var->smf_binding = smf_binding;
    pdu_session_context_local_var->vsmf_id = vsmf_id;
    pdu_session_context_local_var->vsmf_set_id = vsmf_set_id;
    pdu_session_context_local_var->vsmf_service_set_id = vsmf_service_set_id;
    pdu_session_context_local_var->vsmf_binding = vsmf_binding;
    pdu_session_context_local_var->ismf_id = ismf_id;
    pdu_session_context_local_var->ismf_set_id = ismf_set_id;
    pdu_session_context_local_var->ismf_service_set_id = ismf_service_set_id;
    pdu_session_context_local_var->ismf_binding = ismf_binding;
    pdu_session_context_local_var->ns_instance = ns_instance;
    pdu_session_context_local_var->smf_service_instance_id = smf_service_instance_id;
    pdu_session_context_local_var->is_ma_pdu_session = is_ma_pdu_session;
    pdu_session_context_local_var->ma_pdu_session = ma_pdu_session;
    pdu_session_context_local_var->cn_assisted_ran_para = cn_assisted_ran_para;

    return pdu_session_context_local_var;
}

void OpenAPI_pdu_session_context_free(OpenAPI_pdu_session_context_t *pdu_session_context)
{
    if (NULL == pdu_session_context) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pdu_session_context->sm_context_ref);
    OpenAPI_snssai_free(pdu_session_context->s_nssai);
    ogs_free(pdu_session_context->dnn);
    ogs_free(pdu_session_context->selected_dnn);
    OpenAPI_list_for_each(pdu_session_context->allocated_ebi_list, node) {
        OpenAPI_ebi_arp_mapping_free(node->data);
    }
    OpenAPI_list_free(pdu_session_context->allocated_ebi_list);
    ogs_free(pdu_session_context->hsmf_id);
    ogs_free(pdu_session_context->hsmf_set_id);
    ogs_free(pdu_session_context->hsmf_service_set_id);
    ogs_free(pdu_session_context->vsmf_id);
    ogs_free(pdu_session_context->vsmf_set_id);
    ogs_free(pdu_session_context->vsmf_service_set_id);
    ogs_free(pdu_session_context->ismf_id);
    ogs_free(pdu_session_context->ismf_set_id);
    ogs_free(pdu_session_context->ismf_service_set_id);
    ogs_free(pdu_session_context->ns_instance);
    ogs_free(pdu_session_context->smf_service_instance_id);
    OpenAPI_cn_assisted_ran_para_free(pdu_session_context->cn_assisted_ran_para);
    ogs_free(pdu_session_context);
}

cJSON *OpenAPI_pdu_session_context_convertToJSON(OpenAPI_pdu_session_context_t *pdu_session_context)
{
    cJSON *item = NULL;

    if (pdu_session_context == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [PduSessionContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pduSessionId", pdu_session_context->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "smContextRef", pdu_session_context->sm_context_ref) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [sm_context_ref]");
        goto end;
    }

    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_context->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "dnn", pdu_session_context->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [dnn]");
        goto end;
    }

    if (pdu_session_context->selected_dnn) {
    if (cJSON_AddStringToObject(item, "selectedDnn", pdu_session_context->selected_dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [selected_dnn]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(pdu_session_context->access_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [access_type]");
        goto end;
    }

    if (pdu_session_context->additional_access_type) {
    if (cJSON_AddStringToObject(item, "additionalAccessType", OpenAPI_access_type_ToString(pdu_session_context->additional_access_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [additional_access_type]");
        goto end;
    }
    }

    if (pdu_session_context->allocated_ebi_list) {
    cJSON *allocated_ebi_listList = cJSON_AddArrayToObject(item, "allocatedEbiList");
    if (allocated_ebi_listList == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [allocated_ebi_list]");
        goto end;
    }

    OpenAPI_lnode_t *allocated_ebi_list_node;
    if (pdu_session_context->allocated_ebi_list) {
        OpenAPI_list_for_each(pdu_session_context->allocated_ebi_list, allocated_ebi_list_node) {
            cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(allocated_ebi_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [allocated_ebi_list]");
                goto end;
            }
            cJSON_AddItemToArray(allocated_ebi_listList, itemLocal);
        }
    }
    }

    if (pdu_session_context->hsmf_id) {
    if (cJSON_AddStringToObject(item, "hsmfId", pdu_session_context->hsmf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [hsmf_id]");
        goto end;
    }
    }

    if (pdu_session_context->hsmf_set_id) {
    if (cJSON_AddStringToObject(item, "hsmfSetId", pdu_session_context->hsmf_set_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [hsmf_set_id]");
        goto end;
    }
    }

    if (pdu_session_context->hsmf_service_set_id) {
    if (cJSON_AddStringToObject(item, "hsmfServiceSetId", pdu_session_context->hsmf_service_set_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [hsmf_service_set_id]");
        goto end;
    }
    }

    if (pdu_session_context->smf_binding) {
    if (cJSON_AddStringToObject(item, "smfBinding", OpenAPI_sbi_binding_level_ToString(pdu_session_context->smf_binding)) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [smf_binding]");
        goto end;
    }
    }

    if (pdu_session_context->vsmf_id) {
    if (cJSON_AddStringToObject(item, "vsmfId", pdu_session_context->vsmf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [vsmf_id]");
        goto end;
    }
    }

    if (pdu_session_context->vsmf_set_id) {
    if (cJSON_AddStringToObject(item, "vsmfSetId", pdu_session_context->vsmf_set_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [vsmf_set_id]");
        goto end;
    }
    }

    if (pdu_session_context->vsmf_service_set_id) {
    if (cJSON_AddStringToObject(item, "vsmfServiceSetId", pdu_session_context->vsmf_service_set_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [vsmf_service_set_id]");
        goto end;
    }
    }

    if (pdu_session_context->vsmf_binding) {
    if (cJSON_AddStringToObject(item, "vsmfBinding", OpenAPI_sbi_binding_level_ToString(pdu_session_context->vsmf_binding)) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [vsmf_binding]");
        goto end;
    }
    }

    if (pdu_session_context->ismf_id) {
    if (cJSON_AddStringToObject(item, "ismfId", pdu_session_context->ismf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [ismf_id]");
        goto end;
    }
    }

    if (pdu_session_context->ismf_set_id) {
    if (cJSON_AddStringToObject(item, "ismfSetId", pdu_session_context->ismf_set_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [ismf_set_id]");
        goto end;
    }
    }

    if (pdu_session_context->ismf_service_set_id) {
    if (cJSON_AddStringToObject(item, "ismfServiceSetId", pdu_session_context->ismf_service_set_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [ismf_service_set_id]");
        goto end;
    }
    }

    if (pdu_session_context->ismf_binding) {
    if (cJSON_AddStringToObject(item, "ismfBinding", OpenAPI_sbi_binding_level_ToString(pdu_session_context->ismf_binding)) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [ismf_binding]");
        goto end;
    }
    }

    if (pdu_session_context->ns_instance) {
    if (cJSON_AddStringToObject(item, "nsInstance", pdu_session_context->ns_instance) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [ns_instance]");
        goto end;
    }
    }

    if (pdu_session_context->smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "smfServiceInstanceId", pdu_session_context->smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [smf_service_instance_id]");
        goto end;
    }
    }

    if (pdu_session_context->is_ma_pdu_session) {
    if (cJSON_AddBoolToObject(item, "maPduSession", pdu_session_context->ma_pdu_session) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [ma_pdu_session]");
        goto end;
    }
    }

    if (pdu_session_context->cn_assisted_ran_para) {
    cJSON *cn_assisted_ran_para_local_JSON = OpenAPI_cn_assisted_ran_para_convertToJSON(pdu_session_context->cn_assisted_ran_para);
    if (cn_assisted_ran_para_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [cn_assisted_ran_para]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cnAssistedRanPara", cn_assisted_ran_para_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [cn_assisted_ran_para]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_context_t *OpenAPI_pdu_session_context_parseFromJSON(cJSON *pdu_session_contextJSON)
{
    OpenAPI_pdu_session_context_t *pdu_session_context_local_var = NULL;
    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    cJSON *sm_context_ref = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "smContextRef");
    if (!sm_context_ref) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [sm_context_ref]");
        goto end;
    }

    if (!cJSON_IsString(sm_context_ref)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [sm_context_ref]");
        goto end;
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [s_nssai]");
        goto end;
    }

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [dnn]");
        goto end;
    }

    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *selected_dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "selectedDnn");

    if (selected_dnn) {
    if (!cJSON_IsString(selected_dnn)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [selected_dnn]");
        goto end;
    }
    }

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [access_type]");
        goto end;
    }

    OpenAPI_access_type_e access_typeVariable;
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    cJSON *additional_access_type = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "additionalAccessType");

    OpenAPI_access_type_e additional_access_typeVariable;
    if (additional_access_type) {
    if (!cJSON_IsString(additional_access_type)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [additional_access_type]");
        goto end;
    }
    additional_access_typeVariable = OpenAPI_access_type_FromString(additional_access_type->valuestring);
    }

    cJSON *allocated_ebi_list = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "allocatedEbiList");

    OpenAPI_list_t *allocated_ebi_listList;
    if (allocated_ebi_list) {
    cJSON *allocated_ebi_list_local_nonprimitive;
    if (!cJSON_IsArray(allocated_ebi_list)){
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [allocated_ebi_list]");
        goto end;
    }

    allocated_ebi_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(allocated_ebi_list_local_nonprimitive, allocated_ebi_list ) {
        if (!cJSON_IsObject(allocated_ebi_list_local_nonprimitive)) {
            ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [allocated_ebi_list]");
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

    cJSON *hsmf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "hsmfId");

    if (hsmf_id) {
    if (!cJSON_IsString(hsmf_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [hsmf_id]");
        goto end;
    }
    }

    cJSON *hsmf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "hsmfSetId");

    if (hsmf_set_id) {
    if (!cJSON_IsString(hsmf_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [hsmf_set_id]");
        goto end;
    }
    }

    cJSON *hsmf_service_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "hsmfServiceSetId");

    if (hsmf_service_set_id) {
    if (!cJSON_IsString(hsmf_service_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [hsmf_service_set_id]");
        goto end;
    }
    }

    cJSON *smf_binding = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "smfBinding");

    OpenAPI_sbi_binding_level_e smf_bindingVariable;
    if (smf_binding) {
    if (!cJSON_IsString(smf_binding)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [smf_binding]");
        goto end;
    }
    smf_bindingVariable = OpenAPI_sbi_binding_level_FromString(smf_binding->valuestring);
    }

    cJSON *vsmf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfId");

    if (vsmf_id) {
    if (!cJSON_IsString(vsmf_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_id]");
        goto end;
    }
    }

    cJSON *vsmf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfSetId");

    if (vsmf_set_id) {
    if (!cJSON_IsString(vsmf_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_set_id]");
        goto end;
    }
    }

    cJSON *vsmf_service_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfServiceSetId");

    if (vsmf_service_set_id) {
    if (!cJSON_IsString(vsmf_service_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_service_set_id]");
        goto end;
    }
    }

    cJSON *vsmf_binding = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfBinding");

    OpenAPI_sbi_binding_level_e vsmf_bindingVariable;
    if (vsmf_binding) {
    if (!cJSON_IsString(vsmf_binding)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_binding]");
        goto end;
    }
    vsmf_bindingVariable = OpenAPI_sbi_binding_level_FromString(vsmf_binding->valuestring);
    }

    cJSON *ismf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfId");

    if (ismf_id) {
    if (!cJSON_IsString(ismf_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_id]");
        goto end;
    }
    }

    cJSON *ismf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfSetId");

    if (ismf_set_id) {
    if (!cJSON_IsString(ismf_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_set_id]");
        goto end;
    }
    }

    cJSON *ismf_service_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfServiceSetId");

    if (ismf_service_set_id) {
    if (!cJSON_IsString(ismf_service_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_service_set_id]");
        goto end;
    }
    }

    cJSON *ismf_binding = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfBinding");

    OpenAPI_sbi_binding_level_e ismf_bindingVariable;
    if (ismf_binding) {
    if (!cJSON_IsString(ismf_binding)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_binding]");
        goto end;
    }
    ismf_bindingVariable = OpenAPI_sbi_binding_level_FromString(ismf_binding->valuestring);
    }

    cJSON *ns_instance = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "nsInstance");

    if (ns_instance) {
    if (!cJSON_IsString(ns_instance)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ns_instance]");
        goto end;
    }
    }

    cJSON *smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "smfServiceInstanceId");

    if (smf_service_instance_id) {
    if (!cJSON_IsString(smf_service_instance_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [smf_service_instance_id]");
        goto end;
    }
    }

    cJSON *ma_pdu_session = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "maPduSession");

    if (ma_pdu_session) {
    if (!cJSON_IsBool(ma_pdu_session)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ma_pdu_session]");
        goto end;
    }
    }

    cJSON *cn_assisted_ran_para = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "cnAssistedRanPara");

    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para_local_nonprim = NULL;
    if (cn_assisted_ran_para) {
    cn_assisted_ran_para_local_nonprim = OpenAPI_cn_assisted_ran_para_parseFromJSON(cn_assisted_ran_para);
    }

    pdu_session_context_local_var = OpenAPI_pdu_session_context_create (
        
        pdu_session_id->valuedouble,
        ogs_strdup(sm_context_ref->valuestring),
        s_nssai_local_nonprim,
        ogs_strdup(dnn->valuestring),
        selected_dnn ? ogs_strdup(selected_dnn->valuestring) : NULL,
        access_typeVariable,
        additional_access_type ? additional_access_typeVariable : 0,
        allocated_ebi_list ? allocated_ebi_listList : NULL,
        hsmf_id ? ogs_strdup(hsmf_id->valuestring) : NULL,
        hsmf_set_id ? ogs_strdup(hsmf_set_id->valuestring) : NULL,
        hsmf_service_set_id ? ogs_strdup(hsmf_service_set_id->valuestring) : NULL,
        smf_binding ? smf_bindingVariable : 0,
        vsmf_id ? ogs_strdup(vsmf_id->valuestring) : NULL,
        vsmf_set_id ? ogs_strdup(vsmf_set_id->valuestring) : NULL,
        vsmf_service_set_id ? ogs_strdup(vsmf_service_set_id->valuestring) : NULL,
        vsmf_binding ? vsmf_bindingVariable : 0,
        ismf_id ? ogs_strdup(ismf_id->valuestring) : NULL,
        ismf_set_id ? ogs_strdup(ismf_set_id->valuestring) : NULL,
        ismf_service_set_id ? ogs_strdup(ismf_service_set_id->valuestring) : NULL,
        ismf_binding ? ismf_bindingVariable : 0,
        ns_instance ? ogs_strdup(ns_instance->valuestring) : NULL,
        smf_service_instance_id ? ogs_strdup(smf_service_instance_id->valuestring) : NULL,
        ma_pdu_session ? true : false,
        ma_pdu_session ? ma_pdu_session->valueint : 0,
        cn_assisted_ran_para ? cn_assisted_ran_para_local_nonprim : NULL
    );

    return pdu_session_context_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_context_t *OpenAPI_pdu_session_context_copy(OpenAPI_pdu_session_context_t *dst, OpenAPI_pdu_session_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed");
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

    OpenAPI_pdu_session_context_free(dst);
    dst = OpenAPI_pdu_session_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

