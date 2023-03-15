
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
    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para,
    char *nrf_management_uri,
    char *nrf_discovery_uri,
    char *nrf_access_token_uri,
    char *smf_binding_info,
    char *vsmf_binding_info,
    char *ismf_binding_info,
    OpenAPI_snssai_t *additional_snssai,
    char *inter_plmn_api_root,
    char *pgw_fqdn,
    OpenAPI_ip_address_t *pgw_ip_addr,
    OpenAPI_plmn_id_t *plmn_id,
    char *anchor_smf_supported_features,
    bool is_anchor_smf_oauth2_required,
    int anchor_smf_oauth2_required
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
    pdu_session_context_local_var->nrf_management_uri = nrf_management_uri;
    pdu_session_context_local_var->nrf_discovery_uri = nrf_discovery_uri;
    pdu_session_context_local_var->nrf_access_token_uri = nrf_access_token_uri;
    pdu_session_context_local_var->smf_binding_info = smf_binding_info;
    pdu_session_context_local_var->vsmf_binding_info = vsmf_binding_info;
    pdu_session_context_local_var->ismf_binding_info = ismf_binding_info;
    pdu_session_context_local_var->additional_snssai = additional_snssai;
    pdu_session_context_local_var->inter_plmn_api_root = inter_plmn_api_root;
    pdu_session_context_local_var->pgw_fqdn = pgw_fqdn;
    pdu_session_context_local_var->pgw_ip_addr = pgw_ip_addr;
    pdu_session_context_local_var->plmn_id = plmn_id;
    pdu_session_context_local_var->anchor_smf_supported_features = anchor_smf_supported_features;
    pdu_session_context_local_var->is_anchor_smf_oauth2_required = is_anchor_smf_oauth2_required;
    pdu_session_context_local_var->anchor_smf_oauth2_required = anchor_smf_oauth2_required;

    return pdu_session_context_local_var;
}

void OpenAPI_pdu_session_context_free(OpenAPI_pdu_session_context_t *pdu_session_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_context) {
        return;
    }
    if (pdu_session_context->sm_context_ref) {
        ogs_free(pdu_session_context->sm_context_ref);
        pdu_session_context->sm_context_ref = NULL;
    }
    if (pdu_session_context->s_nssai) {
        OpenAPI_snssai_free(pdu_session_context->s_nssai);
        pdu_session_context->s_nssai = NULL;
    }
    if (pdu_session_context->dnn) {
        ogs_free(pdu_session_context->dnn);
        pdu_session_context->dnn = NULL;
    }
    if (pdu_session_context->selected_dnn) {
        ogs_free(pdu_session_context->selected_dnn);
        pdu_session_context->selected_dnn = NULL;
    }
    if (pdu_session_context->allocated_ebi_list) {
        OpenAPI_list_for_each(pdu_session_context->allocated_ebi_list, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(pdu_session_context->allocated_ebi_list);
        pdu_session_context->allocated_ebi_list = NULL;
    }
    if (pdu_session_context->hsmf_id) {
        ogs_free(pdu_session_context->hsmf_id);
        pdu_session_context->hsmf_id = NULL;
    }
    if (pdu_session_context->hsmf_set_id) {
        ogs_free(pdu_session_context->hsmf_set_id);
        pdu_session_context->hsmf_set_id = NULL;
    }
    if (pdu_session_context->hsmf_service_set_id) {
        ogs_free(pdu_session_context->hsmf_service_set_id);
        pdu_session_context->hsmf_service_set_id = NULL;
    }
    if (pdu_session_context->vsmf_id) {
        ogs_free(pdu_session_context->vsmf_id);
        pdu_session_context->vsmf_id = NULL;
    }
    if (pdu_session_context->vsmf_set_id) {
        ogs_free(pdu_session_context->vsmf_set_id);
        pdu_session_context->vsmf_set_id = NULL;
    }
    if (pdu_session_context->vsmf_service_set_id) {
        ogs_free(pdu_session_context->vsmf_service_set_id);
        pdu_session_context->vsmf_service_set_id = NULL;
    }
    if (pdu_session_context->ismf_id) {
        ogs_free(pdu_session_context->ismf_id);
        pdu_session_context->ismf_id = NULL;
    }
    if (pdu_session_context->ismf_set_id) {
        ogs_free(pdu_session_context->ismf_set_id);
        pdu_session_context->ismf_set_id = NULL;
    }
    if (pdu_session_context->ismf_service_set_id) {
        ogs_free(pdu_session_context->ismf_service_set_id);
        pdu_session_context->ismf_service_set_id = NULL;
    }
    if (pdu_session_context->ns_instance) {
        ogs_free(pdu_session_context->ns_instance);
        pdu_session_context->ns_instance = NULL;
    }
    if (pdu_session_context->smf_service_instance_id) {
        ogs_free(pdu_session_context->smf_service_instance_id);
        pdu_session_context->smf_service_instance_id = NULL;
    }
    if (pdu_session_context->cn_assisted_ran_para) {
        OpenAPI_cn_assisted_ran_para_free(pdu_session_context->cn_assisted_ran_para);
        pdu_session_context->cn_assisted_ran_para = NULL;
    }
    if (pdu_session_context->nrf_management_uri) {
        ogs_free(pdu_session_context->nrf_management_uri);
        pdu_session_context->nrf_management_uri = NULL;
    }
    if (pdu_session_context->nrf_discovery_uri) {
        ogs_free(pdu_session_context->nrf_discovery_uri);
        pdu_session_context->nrf_discovery_uri = NULL;
    }
    if (pdu_session_context->nrf_access_token_uri) {
        ogs_free(pdu_session_context->nrf_access_token_uri);
        pdu_session_context->nrf_access_token_uri = NULL;
    }
    if (pdu_session_context->smf_binding_info) {
        ogs_free(pdu_session_context->smf_binding_info);
        pdu_session_context->smf_binding_info = NULL;
    }
    if (pdu_session_context->vsmf_binding_info) {
        ogs_free(pdu_session_context->vsmf_binding_info);
        pdu_session_context->vsmf_binding_info = NULL;
    }
    if (pdu_session_context->ismf_binding_info) {
        ogs_free(pdu_session_context->ismf_binding_info);
        pdu_session_context->ismf_binding_info = NULL;
    }
    if (pdu_session_context->additional_snssai) {
        OpenAPI_snssai_free(pdu_session_context->additional_snssai);
        pdu_session_context->additional_snssai = NULL;
    }
    if (pdu_session_context->inter_plmn_api_root) {
        ogs_free(pdu_session_context->inter_plmn_api_root);
        pdu_session_context->inter_plmn_api_root = NULL;
    }
    if (pdu_session_context->pgw_fqdn) {
        ogs_free(pdu_session_context->pgw_fqdn);
        pdu_session_context->pgw_fqdn = NULL;
    }
    if (pdu_session_context->pgw_ip_addr) {
        OpenAPI_ip_address_free(pdu_session_context->pgw_ip_addr);
        pdu_session_context->pgw_ip_addr = NULL;
    }
    if (pdu_session_context->plmn_id) {
        OpenAPI_plmn_id_free(pdu_session_context->plmn_id);
        pdu_session_context->plmn_id = NULL;
    }
    if (pdu_session_context->anchor_smf_supported_features) {
        ogs_free(pdu_session_context->anchor_smf_supported_features);
        pdu_session_context->anchor_smf_supported_features = NULL;
    }
    ogs_free(pdu_session_context);
}

cJSON *OpenAPI_pdu_session_context_convertToJSON(OpenAPI_pdu_session_context_t *pdu_session_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_context == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [PduSessionContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pduSessionId", pdu_session_context->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!pdu_session_context->sm_context_ref) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [sm_context_ref]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smContextRef", pdu_session_context->sm_context_ref) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [sm_context_ref]");
        goto end;
    }

    if (!pdu_session_context->s_nssai) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [s_nssai]");
        return NULL;
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

    if (!pdu_session_context->dnn) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [dnn]");
        return NULL;
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

    if (pdu_session_context->access_type == OpenAPI_access_type_NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [access_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(pdu_session_context->access_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [access_type]");
        goto end;
    }

    if (pdu_session_context->additional_access_type != OpenAPI_access_type_NULL) {
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
    OpenAPI_list_for_each(pdu_session_context->allocated_ebi_list, node) {
        cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [allocated_ebi_list]");
            goto end;
        }
        cJSON_AddItemToArray(allocated_ebi_listList, itemLocal);
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

    if (pdu_session_context->smf_binding != OpenAPI_sbi_binding_level_NULL) {
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

    if (pdu_session_context->vsmf_binding != OpenAPI_sbi_binding_level_NULL) {
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

    if (pdu_session_context->ismf_binding != OpenAPI_sbi_binding_level_NULL) {
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

    if (pdu_session_context->nrf_management_uri) {
    if (cJSON_AddStringToObject(item, "nrfManagementUri", pdu_session_context->nrf_management_uri) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [nrf_management_uri]");
        goto end;
    }
    }

    if (pdu_session_context->nrf_discovery_uri) {
    if (cJSON_AddStringToObject(item, "nrfDiscoveryUri", pdu_session_context->nrf_discovery_uri) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [nrf_discovery_uri]");
        goto end;
    }
    }

    if (pdu_session_context->nrf_access_token_uri) {
    if (cJSON_AddStringToObject(item, "nrfAccessTokenUri", pdu_session_context->nrf_access_token_uri) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [nrf_access_token_uri]");
        goto end;
    }
    }

    if (pdu_session_context->smf_binding_info) {
    if (cJSON_AddStringToObject(item, "smfBindingInfo", pdu_session_context->smf_binding_info) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [smf_binding_info]");
        goto end;
    }
    }

    if (pdu_session_context->vsmf_binding_info) {
    if (cJSON_AddStringToObject(item, "vsmfBindingInfo", pdu_session_context->vsmf_binding_info) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [vsmf_binding_info]");
        goto end;
    }
    }

    if (pdu_session_context->ismf_binding_info) {
    if (cJSON_AddStringToObject(item, "ismfBindingInfo", pdu_session_context->ismf_binding_info) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [ismf_binding_info]");
        goto end;
    }
    }

    if (pdu_session_context->additional_snssai) {
    cJSON *additional_snssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_context->additional_snssai);
    if (additional_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [additional_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "additionalSnssai", additional_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [additional_snssai]");
        goto end;
    }
    }

    if (pdu_session_context->inter_plmn_api_root) {
    if (cJSON_AddStringToObject(item, "interPlmnApiRoot", pdu_session_context->inter_plmn_api_root) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    if (pdu_session_context->pgw_fqdn) {
    if (cJSON_AddStringToObject(item, "pgwFqdn", pdu_session_context->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    if (pdu_session_context->pgw_ip_addr) {
    cJSON *pgw_ip_addr_local_JSON = OpenAPI_ip_address_convertToJSON(pdu_session_context->pgw_ip_addr);
    if (pgw_ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [pgw_ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pgwIpAddr", pgw_ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [pgw_ip_addr]");
        goto end;
    }
    }

    if (pdu_session_context->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(pdu_session_context->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (pdu_session_context->anchor_smf_supported_features) {
    if (cJSON_AddStringToObject(item, "anchorSmfSupportedFeatures", pdu_session_context->anchor_smf_supported_features) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [anchor_smf_supported_features]");
        goto end;
    }
    }

    if (pdu_session_context->is_anchor_smf_oauth2_required) {
    if (cJSON_AddBoolToObject(item, "anchorSmfOauth2Required", pdu_session_context->anchor_smf_oauth2_required) == NULL) {
        ogs_error("OpenAPI_pdu_session_context_convertToJSON() failed [anchor_smf_oauth2_required]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_context_t *OpenAPI_pdu_session_context_parseFromJSON(cJSON *pdu_session_contextJSON)
{
    OpenAPI_pdu_session_context_t *pdu_session_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *sm_context_ref = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *selected_dnn = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *additional_access_type = NULL;
    OpenAPI_access_type_e additional_access_typeVariable = 0;
    cJSON *allocated_ebi_list = NULL;
    OpenAPI_list_t *allocated_ebi_listList = NULL;
    cJSON *hsmf_id = NULL;
    cJSON *hsmf_set_id = NULL;
    cJSON *hsmf_service_set_id = NULL;
    cJSON *smf_binding = NULL;
    OpenAPI_sbi_binding_level_e smf_bindingVariable = 0;
    cJSON *vsmf_id = NULL;
    cJSON *vsmf_set_id = NULL;
    cJSON *vsmf_service_set_id = NULL;
    cJSON *vsmf_binding = NULL;
    OpenAPI_sbi_binding_level_e vsmf_bindingVariable = 0;
    cJSON *ismf_id = NULL;
    cJSON *ismf_set_id = NULL;
    cJSON *ismf_service_set_id = NULL;
    cJSON *ismf_binding = NULL;
    OpenAPI_sbi_binding_level_e ismf_bindingVariable = 0;
    cJSON *ns_instance = NULL;
    cJSON *smf_service_instance_id = NULL;
    cJSON *ma_pdu_session = NULL;
    cJSON *cn_assisted_ran_para = NULL;
    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para_local_nonprim = NULL;
    cJSON *nrf_management_uri = NULL;
    cJSON *nrf_discovery_uri = NULL;
    cJSON *nrf_access_token_uri = NULL;
    cJSON *smf_binding_info = NULL;
    cJSON *vsmf_binding_info = NULL;
    cJSON *ismf_binding_info = NULL;
    cJSON *additional_snssai = NULL;
    OpenAPI_snssai_t *additional_snssai_local_nonprim = NULL;
    cJSON *inter_plmn_api_root = NULL;
    cJSON *pgw_fqdn = NULL;
    cJSON *pgw_ip_addr = NULL;
    OpenAPI_ip_address_t *pgw_ip_addr_local_nonprim = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *anchor_smf_supported_features = NULL;
    cJSON *anchor_smf_oauth2_required = NULL;
    pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    sm_context_ref = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "smContextRef");
    if (!sm_context_ref) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [sm_context_ref]");
        goto end;
    }
    if (!cJSON_IsString(sm_context_ref)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [sm_context_ref]");
        goto end;
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [s_nssai]");
        goto end;
    }
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [dnn]");
        goto end;
    }

    selected_dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "selectedDnn");
    if (selected_dnn) {
    if (!cJSON_IsString(selected_dnn) && !cJSON_IsNull(selected_dnn)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [selected_dnn]");
        goto end;
    }
    }

    access_type = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [access_type]");
        goto end;
    }
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    additional_access_type = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "additionalAccessType");
    if (additional_access_type) {
    if (!cJSON_IsString(additional_access_type)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [additional_access_type]");
        goto end;
    }
    additional_access_typeVariable = OpenAPI_access_type_FromString(additional_access_type->valuestring);
    }

    allocated_ebi_list = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "allocatedEbiList");
    if (allocated_ebi_list) {
        cJSON *allocated_ebi_list_local = NULL;
        if (!cJSON_IsArray(allocated_ebi_list)) {
            ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [allocated_ebi_list]");
            goto end;
        }

        allocated_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allocated_ebi_list_local, allocated_ebi_list) {
            if (!cJSON_IsObject(allocated_ebi_list_local)) {
                ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [allocated_ebi_list]");
                goto end;
            }
            OpenAPI_ebi_arp_mapping_t *allocated_ebi_listItem = OpenAPI_ebi_arp_mapping_parseFromJSON(allocated_ebi_list_local);
            if (!allocated_ebi_listItem) {
                ogs_error("No allocated_ebi_listItem");
                goto end;
            }
            OpenAPI_list_add(allocated_ebi_listList, allocated_ebi_listItem);
        }
    }

    hsmf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "hsmfId");
    if (hsmf_id) {
    if (!cJSON_IsString(hsmf_id) && !cJSON_IsNull(hsmf_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [hsmf_id]");
        goto end;
    }
    }

    hsmf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "hsmfSetId");
    if (hsmf_set_id) {
    if (!cJSON_IsString(hsmf_set_id) && !cJSON_IsNull(hsmf_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [hsmf_set_id]");
        goto end;
    }
    }

    hsmf_service_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "hsmfServiceSetId");
    if (hsmf_service_set_id) {
    if (!cJSON_IsString(hsmf_service_set_id) && !cJSON_IsNull(hsmf_service_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [hsmf_service_set_id]");
        goto end;
    }
    }

    smf_binding = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "smfBinding");
    if (smf_binding) {
    if (!cJSON_IsString(smf_binding)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [smf_binding]");
        goto end;
    }
    smf_bindingVariable = OpenAPI_sbi_binding_level_FromString(smf_binding->valuestring);
    }

    vsmf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfId");
    if (vsmf_id) {
    if (!cJSON_IsString(vsmf_id) && !cJSON_IsNull(vsmf_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_id]");
        goto end;
    }
    }

    vsmf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfSetId");
    if (vsmf_set_id) {
    if (!cJSON_IsString(vsmf_set_id) && !cJSON_IsNull(vsmf_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_set_id]");
        goto end;
    }
    }

    vsmf_service_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfServiceSetId");
    if (vsmf_service_set_id) {
    if (!cJSON_IsString(vsmf_service_set_id) && !cJSON_IsNull(vsmf_service_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_service_set_id]");
        goto end;
    }
    }

    vsmf_binding = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfBinding");
    if (vsmf_binding) {
    if (!cJSON_IsString(vsmf_binding)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_binding]");
        goto end;
    }
    vsmf_bindingVariable = OpenAPI_sbi_binding_level_FromString(vsmf_binding->valuestring);
    }

    ismf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfId");
    if (ismf_id) {
    if (!cJSON_IsString(ismf_id) && !cJSON_IsNull(ismf_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_id]");
        goto end;
    }
    }

    ismf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfSetId");
    if (ismf_set_id) {
    if (!cJSON_IsString(ismf_set_id) && !cJSON_IsNull(ismf_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_set_id]");
        goto end;
    }
    }

    ismf_service_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfServiceSetId");
    if (ismf_service_set_id) {
    if (!cJSON_IsString(ismf_service_set_id) && !cJSON_IsNull(ismf_service_set_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_service_set_id]");
        goto end;
    }
    }

    ismf_binding = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfBinding");
    if (ismf_binding) {
    if (!cJSON_IsString(ismf_binding)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_binding]");
        goto end;
    }
    ismf_bindingVariable = OpenAPI_sbi_binding_level_FromString(ismf_binding->valuestring);
    }

    ns_instance = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "nsInstance");
    if (ns_instance) {
    if (!cJSON_IsString(ns_instance) && !cJSON_IsNull(ns_instance)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ns_instance]");
        goto end;
    }
    }

    smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "smfServiceInstanceId");
    if (smf_service_instance_id) {
    if (!cJSON_IsString(smf_service_instance_id) && !cJSON_IsNull(smf_service_instance_id)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [smf_service_instance_id]");
        goto end;
    }
    }

    ma_pdu_session = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "maPduSession");
    if (ma_pdu_session) {
    if (!cJSON_IsBool(ma_pdu_session)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ma_pdu_session]");
        goto end;
    }
    }

    cn_assisted_ran_para = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "cnAssistedRanPara");
    if (cn_assisted_ran_para) {
    cn_assisted_ran_para_local_nonprim = OpenAPI_cn_assisted_ran_para_parseFromJSON(cn_assisted_ran_para);
    if (!cn_assisted_ran_para_local_nonprim) {
        ogs_error("OpenAPI_cn_assisted_ran_para_parseFromJSON failed [cn_assisted_ran_para]");
        goto end;
    }
    }

    nrf_management_uri = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "nrfManagementUri");
    if (nrf_management_uri) {
    if (!cJSON_IsString(nrf_management_uri) && !cJSON_IsNull(nrf_management_uri)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [nrf_management_uri]");
        goto end;
    }
    }

    nrf_discovery_uri = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "nrfDiscoveryUri");
    if (nrf_discovery_uri) {
    if (!cJSON_IsString(nrf_discovery_uri) && !cJSON_IsNull(nrf_discovery_uri)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [nrf_discovery_uri]");
        goto end;
    }
    }

    nrf_access_token_uri = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "nrfAccessTokenUri");
    if (nrf_access_token_uri) {
    if (!cJSON_IsString(nrf_access_token_uri) && !cJSON_IsNull(nrf_access_token_uri)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [nrf_access_token_uri]");
        goto end;
    }
    }

    smf_binding_info = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "smfBindingInfo");
    if (smf_binding_info) {
    if (!cJSON_IsString(smf_binding_info) && !cJSON_IsNull(smf_binding_info)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [smf_binding_info]");
        goto end;
    }
    }

    vsmf_binding_info = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "vsmfBindingInfo");
    if (vsmf_binding_info) {
    if (!cJSON_IsString(vsmf_binding_info) && !cJSON_IsNull(vsmf_binding_info)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [vsmf_binding_info]");
        goto end;
    }
    }

    ismf_binding_info = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "ismfBindingInfo");
    if (ismf_binding_info) {
    if (!cJSON_IsString(ismf_binding_info) && !cJSON_IsNull(ismf_binding_info)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [ismf_binding_info]");
        goto end;
    }
    }

    additional_snssai = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "additionalSnssai");
    if (additional_snssai) {
    additional_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(additional_snssai);
    if (!additional_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [additional_snssai]");
        goto end;
    }
    }

    inter_plmn_api_root = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "interPlmnApiRoot");
    if (inter_plmn_api_root) {
    if (!cJSON_IsString(inter_plmn_api_root) && !cJSON_IsNull(inter_plmn_api_root)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    pgw_fqdn = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "pgwFqdn");
    if (pgw_fqdn) {
    if (!cJSON_IsString(pgw_fqdn) && !cJSON_IsNull(pgw_fqdn)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    pgw_ip_addr = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "pgwIpAddr");
    if (pgw_ip_addr) {
    pgw_ip_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(pgw_ip_addr);
    if (!pgw_ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [pgw_ip_addr]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    anchor_smf_supported_features = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "anchorSmfSupportedFeatures");
    if (anchor_smf_supported_features) {
    if (!cJSON_IsString(anchor_smf_supported_features) && !cJSON_IsNull(anchor_smf_supported_features)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [anchor_smf_supported_features]");
        goto end;
    }
    }

    anchor_smf_oauth2_required = cJSON_GetObjectItemCaseSensitive(pdu_session_contextJSON, "anchorSmfOauth2Required");
    if (anchor_smf_oauth2_required) {
    if (!cJSON_IsBool(anchor_smf_oauth2_required)) {
        ogs_error("OpenAPI_pdu_session_context_parseFromJSON() failed [anchor_smf_oauth2_required]");
        goto end;
    }
    }

    pdu_session_context_local_var = OpenAPI_pdu_session_context_create (
        
        pdu_session_id->valuedouble,
        ogs_strdup(sm_context_ref->valuestring),
        s_nssai_local_nonprim,
        ogs_strdup(dnn->valuestring),
        selected_dnn && !cJSON_IsNull(selected_dnn) ? ogs_strdup(selected_dnn->valuestring) : NULL,
        access_typeVariable,
        additional_access_type ? additional_access_typeVariable : 0,
        allocated_ebi_list ? allocated_ebi_listList : NULL,
        hsmf_id && !cJSON_IsNull(hsmf_id) ? ogs_strdup(hsmf_id->valuestring) : NULL,
        hsmf_set_id && !cJSON_IsNull(hsmf_set_id) ? ogs_strdup(hsmf_set_id->valuestring) : NULL,
        hsmf_service_set_id && !cJSON_IsNull(hsmf_service_set_id) ? ogs_strdup(hsmf_service_set_id->valuestring) : NULL,
        smf_binding ? smf_bindingVariable : 0,
        vsmf_id && !cJSON_IsNull(vsmf_id) ? ogs_strdup(vsmf_id->valuestring) : NULL,
        vsmf_set_id && !cJSON_IsNull(vsmf_set_id) ? ogs_strdup(vsmf_set_id->valuestring) : NULL,
        vsmf_service_set_id && !cJSON_IsNull(vsmf_service_set_id) ? ogs_strdup(vsmf_service_set_id->valuestring) : NULL,
        vsmf_binding ? vsmf_bindingVariable : 0,
        ismf_id && !cJSON_IsNull(ismf_id) ? ogs_strdup(ismf_id->valuestring) : NULL,
        ismf_set_id && !cJSON_IsNull(ismf_set_id) ? ogs_strdup(ismf_set_id->valuestring) : NULL,
        ismf_service_set_id && !cJSON_IsNull(ismf_service_set_id) ? ogs_strdup(ismf_service_set_id->valuestring) : NULL,
        ismf_binding ? ismf_bindingVariable : 0,
        ns_instance && !cJSON_IsNull(ns_instance) ? ogs_strdup(ns_instance->valuestring) : NULL,
        smf_service_instance_id && !cJSON_IsNull(smf_service_instance_id) ? ogs_strdup(smf_service_instance_id->valuestring) : NULL,
        ma_pdu_session ? true : false,
        ma_pdu_session ? ma_pdu_session->valueint : 0,
        cn_assisted_ran_para ? cn_assisted_ran_para_local_nonprim : NULL,
        nrf_management_uri && !cJSON_IsNull(nrf_management_uri) ? ogs_strdup(nrf_management_uri->valuestring) : NULL,
        nrf_discovery_uri && !cJSON_IsNull(nrf_discovery_uri) ? ogs_strdup(nrf_discovery_uri->valuestring) : NULL,
        nrf_access_token_uri && !cJSON_IsNull(nrf_access_token_uri) ? ogs_strdup(nrf_access_token_uri->valuestring) : NULL,
        smf_binding_info && !cJSON_IsNull(smf_binding_info) ? ogs_strdup(smf_binding_info->valuestring) : NULL,
        vsmf_binding_info && !cJSON_IsNull(vsmf_binding_info) ? ogs_strdup(vsmf_binding_info->valuestring) : NULL,
        ismf_binding_info && !cJSON_IsNull(ismf_binding_info) ? ogs_strdup(ismf_binding_info->valuestring) : NULL,
        additional_snssai ? additional_snssai_local_nonprim : NULL,
        inter_plmn_api_root && !cJSON_IsNull(inter_plmn_api_root) ? ogs_strdup(inter_plmn_api_root->valuestring) : NULL,
        pgw_fqdn && !cJSON_IsNull(pgw_fqdn) ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        pgw_ip_addr ? pgw_ip_addr_local_nonprim : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        anchor_smf_supported_features && !cJSON_IsNull(anchor_smf_supported_features) ? ogs_strdup(anchor_smf_supported_features->valuestring) : NULL,
        anchor_smf_oauth2_required ? true : false,
        anchor_smf_oauth2_required ? anchor_smf_oauth2_required->valueint : 0
    );

    return pdu_session_context_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (allocated_ebi_listList) {
        OpenAPI_list_for_each(allocated_ebi_listList, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(allocated_ebi_listList);
        allocated_ebi_listList = NULL;
    }
    if (cn_assisted_ran_para_local_nonprim) {
        OpenAPI_cn_assisted_ran_para_free(cn_assisted_ran_para_local_nonprim);
        cn_assisted_ran_para_local_nonprim = NULL;
    }
    if (additional_snssai_local_nonprim) {
        OpenAPI_snssai_free(additional_snssai_local_nonprim);
        additional_snssai_local_nonprim = NULL;
    }
    if (pgw_ip_addr_local_nonprim) {
        OpenAPI_ip_address_free(pgw_ip_addr_local_nonprim);
        pgw_ip_addr_local_nonprim = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
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

