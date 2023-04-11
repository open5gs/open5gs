
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mm_context.h"

OpenAPI_mm_context_t *OpenAPI_mm_context_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_nas_security_mode_t *nas_security_mode,
    OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode,
    bool is_nas_downlink_count,
    int nas_downlink_count,
    bool is_nas_uplink_count,
    int nas_uplink_count,
    char *ue_security_capability,
    char *s1_ue_network_capability,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *nssai_mapping_list,
    OpenAPI_list_t *allowed_home_nssai,
    OpenAPI_list_t *ns_instance_list,
    OpenAPI_expected_ue_behavior_t *expected_u_ebehavior,
    OpenAPI_ue_differentiation_info_t *ue_differentiation_info,
    char *plmn_assi_ue_radio_cap_id,
    char *man_assi_ue_radio_cap_id,
    char *ucmf_dic_entry_id,
    OpenAPI_global_ran_node_id_t *n3_iwf_id,
    OpenAPI_global_ran_node_id_t *wagf_id,
    OpenAPI_global_ran_node_id_t *tngf_id,
    bool is_an_n2_ap_id,
    int an_n2_ap_id,
    OpenAPI_list_t *nssaa_status_list,
    OpenAPI_list_t *pending_nssai_mapping_list,
    OpenAPI_uuaa_mm_status_e uuaa_mm_status
)
{
    OpenAPI_mm_context_t *mm_context_local_var = ogs_malloc(sizeof(OpenAPI_mm_context_t));
    ogs_assert(mm_context_local_var);

    mm_context_local_var->access_type = access_type;
    mm_context_local_var->nas_security_mode = nas_security_mode;
    mm_context_local_var->eps_nas_security_mode = eps_nas_security_mode;
    mm_context_local_var->is_nas_downlink_count = is_nas_downlink_count;
    mm_context_local_var->nas_downlink_count = nas_downlink_count;
    mm_context_local_var->is_nas_uplink_count = is_nas_uplink_count;
    mm_context_local_var->nas_uplink_count = nas_uplink_count;
    mm_context_local_var->ue_security_capability = ue_security_capability;
    mm_context_local_var->s1_ue_network_capability = s1_ue_network_capability;
    mm_context_local_var->allowed_nssai = allowed_nssai;
    mm_context_local_var->nssai_mapping_list = nssai_mapping_list;
    mm_context_local_var->allowed_home_nssai = allowed_home_nssai;
    mm_context_local_var->ns_instance_list = ns_instance_list;
    mm_context_local_var->expected_u_ebehavior = expected_u_ebehavior;
    mm_context_local_var->ue_differentiation_info = ue_differentiation_info;
    mm_context_local_var->plmn_assi_ue_radio_cap_id = plmn_assi_ue_radio_cap_id;
    mm_context_local_var->man_assi_ue_radio_cap_id = man_assi_ue_radio_cap_id;
    mm_context_local_var->ucmf_dic_entry_id = ucmf_dic_entry_id;
    mm_context_local_var->n3_iwf_id = n3_iwf_id;
    mm_context_local_var->wagf_id = wagf_id;
    mm_context_local_var->tngf_id = tngf_id;
    mm_context_local_var->is_an_n2_ap_id = is_an_n2_ap_id;
    mm_context_local_var->an_n2_ap_id = an_n2_ap_id;
    mm_context_local_var->nssaa_status_list = nssaa_status_list;
    mm_context_local_var->pending_nssai_mapping_list = pending_nssai_mapping_list;
    mm_context_local_var->uuaa_mm_status = uuaa_mm_status;

    return mm_context_local_var;
}

void OpenAPI_mm_context_free(OpenAPI_mm_context_t *mm_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mm_context) {
        return;
    }
    if (mm_context->nas_security_mode) {
        OpenAPI_nas_security_mode_free(mm_context->nas_security_mode);
        mm_context->nas_security_mode = NULL;
    }
    if (mm_context->eps_nas_security_mode) {
        OpenAPI_eps_nas_security_mode_free(mm_context->eps_nas_security_mode);
        mm_context->eps_nas_security_mode = NULL;
    }
    if (mm_context->ue_security_capability) {
        ogs_free(mm_context->ue_security_capability);
        mm_context->ue_security_capability = NULL;
    }
    if (mm_context->s1_ue_network_capability) {
        ogs_free(mm_context->s1_ue_network_capability);
        mm_context->s1_ue_network_capability = NULL;
    }
    if (mm_context->allowed_nssai) {
        OpenAPI_list_for_each(mm_context->allowed_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(mm_context->allowed_nssai);
        mm_context->allowed_nssai = NULL;
    }
    if (mm_context->nssai_mapping_list) {
        OpenAPI_list_for_each(mm_context->nssai_mapping_list, node) {
            OpenAPI_nssai_mapping_free(node->data);
        }
        OpenAPI_list_free(mm_context->nssai_mapping_list);
        mm_context->nssai_mapping_list = NULL;
    }
    if (mm_context->allowed_home_nssai) {
        OpenAPI_list_for_each(mm_context->allowed_home_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(mm_context->allowed_home_nssai);
        mm_context->allowed_home_nssai = NULL;
    }
    if (mm_context->ns_instance_list) {
        OpenAPI_list_for_each(mm_context->ns_instance_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mm_context->ns_instance_list);
        mm_context->ns_instance_list = NULL;
    }
    if (mm_context->expected_u_ebehavior) {
        OpenAPI_expected_ue_behavior_free(mm_context->expected_u_ebehavior);
        mm_context->expected_u_ebehavior = NULL;
    }
    if (mm_context->ue_differentiation_info) {
        OpenAPI_ue_differentiation_info_free(mm_context->ue_differentiation_info);
        mm_context->ue_differentiation_info = NULL;
    }
    if (mm_context->plmn_assi_ue_radio_cap_id) {
        ogs_free(mm_context->plmn_assi_ue_radio_cap_id);
        mm_context->plmn_assi_ue_radio_cap_id = NULL;
    }
    if (mm_context->man_assi_ue_radio_cap_id) {
        ogs_free(mm_context->man_assi_ue_radio_cap_id);
        mm_context->man_assi_ue_radio_cap_id = NULL;
    }
    if (mm_context->ucmf_dic_entry_id) {
        ogs_free(mm_context->ucmf_dic_entry_id);
        mm_context->ucmf_dic_entry_id = NULL;
    }
    if (mm_context->n3_iwf_id) {
        OpenAPI_global_ran_node_id_free(mm_context->n3_iwf_id);
        mm_context->n3_iwf_id = NULL;
    }
    if (mm_context->wagf_id) {
        OpenAPI_global_ran_node_id_free(mm_context->wagf_id);
        mm_context->wagf_id = NULL;
    }
    if (mm_context->tngf_id) {
        OpenAPI_global_ran_node_id_free(mm_context->tngf_id);
        mm_context->tngf_id = NULL;
    }
    if (mm_context->nssaa_status_list) {
        OpenAPI_list_for_each(mm_context->nssaa_status_list, node) {
            OpenAPI_nssaa_status_free(node->data);
        }
        OpenAPI_list_free(mm_context->nssaa_status_list);
        mm_context->nssaa_status_list = NULL;
    }
    if (mm_context->pending_nssai_mapping_list) {
        OpenAPI_list_for_each(mm_context->pending_nssai_mapping_list, node) {
            OpenAPI_nssai_mapping_free(node->data);
        }
        OpenAPI_list_free(mm_context->pending_nssai_mapping_list);
        mm_context->pending_nssai_mapping_list = NULL;
    }
    ogs_free(mm_context);
}

cJSON *OpenAPI_mm_context_convertToJSON(OpenAPI_mm_context_t *mm_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mm_context == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [MmContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mm_context->access_type == OpenAPI_access_type_NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [access_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(mm_context->access_type)) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [access_type]");
        goto end;
    }

    if (mm_context->nas_security_mode) {
    cJSON *nas_security_mode_local_JSON = OpenAPI_nas_security_mode_convertToJSON(mm_context->nas_security_mode);
    if (nas_security_mode_local_JSON == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_security_mode]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nasSecurityMode", nas_security_mode_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_security_mode]");
        goto end;
    }
    }

    if (mm_context->eps_nas_security_mode) {
    cJSON *eps_nas_security_mode_local_JSON = OpenAPI_eps_nas_security_mode_convertToJSON(mm_context->eps_nas_security_mode);
    if (eps_nas_security_mode_local_JSON == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [eps_nas_security_mode]");
        goto end;
    }
    cJSON_AddItemToObject(item, "epsNasSecurityMode", eps_nas_security_mode_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [eps_nas_security_mode]");
        goto end;
    }
    }

    if (mm_context->is_nas_downlink_count) {
    if (cJSON_AddNumberToObject(item, "nasDownlinkCount", mm_context->nas_downlink_count) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_downlink_count]");
        goto end;
    }
    }

    if (mm_context->is_nas_uplink_count) {
    if (cJSON_AddNumberToObject(item, "nasUplinkCount", mm_context->nas_uplink_count) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_uplink_count]");
        goto end;
    }
    }

    if (mm_context->ue_security_capability) {
    if (cJSON_AddStringToObject(item, "ueSecurityCapability", mm_context->ue_security_capability) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [ue_security_capability]");
        goto end;
    }
    }

    if (mm_context->s1_ue_network_capability) {
    if (cJSON_AddStringToObject(item, "s1UeNetworkCapability", mm_context->s1_ue_network_capability) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [s1_ue_network_capability]");
        goto end;
    }
    }

    if (mm_context->allowed_nssai) {
    cJSON *allowed_nssaiList = cJSON_AddArrayToObject(item, "allowedNssai");
    if (allowed_nssaiList == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [allowed_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(mm_context->allowed_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [allowed_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_nssaiList, itemLocal);
    }
    }

    if (mm_context->nssai_mapping_list) {
    cJSON *nssai_mapping_listList = cJSON_AddArrayToObject(item, "nssaiMappingList");
    if (nssai_mapping_listList == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [nssai_mapping_list]");
        goto end;
    }
    OpenAPI_list_for_each(mm_context->nssai_mapping_list, node) {
        cJSON *itemLocal = OpenAPI_nssai_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [nssai_mapping_list]");
            goto end;
        }
        cJSON_AddItemToArray(nssai_mapping_listList, itemLocal);
    }
    }

    if (mm_context->allowed_home_nssai) {
    cJSON *allowed_home_nssaiList = cJSON_AddArrayToObject(item, "allowedHomeNssai");
    if (allowed_home_nssaiList == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [allowed_home_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(mm_context->allowed_home_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [allowed_home_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_home_nssaiList, itemLocal);
    }
    }

    if (mm_context->ns_instance_list) {
    cJSON *ns_instance_listList = cJSON_AddArrayToObject(item, "nsInstanceList");
    if (ns_instance_listList == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [ns_instance_list]");
        goto end;
    }
    OpenAPI_list_for_each(mm_context->ns_instance_list, node) {
        if (cJSON_AddStringToObject(ns_instance_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [ns_instance_list]");
            goto end;
        }
    }
    }

    if (mm_context->expected_u_ebehavior) {
    cJSON *expected_u_ebehavior_local_JSON = OpenAPI_expected_ue_behavior_convertToJSON(mm_context->expected_u_ebehavior);
    if (expected_u_ebehavior_local_JSON == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [expected_u_ebehavior]");
        goto end;
    }
    cJSON_AddItemToObject(item, "expectedUEbehavior", expected_u_ebehavior_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [expected_u_ebehavior]");
        goto end;
    }
    }

    if (mm_context->ue_differentiation_info) {
    cJSON *ue_differentiation_info_local_JSON = OpenAPI_ue_differentiation_info_convertToJSON(mm_context->ue_differentiation_info);
    if (ue_differentiation_info_local_JSON == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [ue_differentiation_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueDifferentiationInfo", ue_differentiation_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [ue_differentiation_info]");
        goto end;
    }
    }

    if (mm_context->plmn_assi_ue_radio_cap_id) {
    if (cJSON_AddStringToObject(item, "plmnAssiUeRadioCapId", mm_context->plmn_assi_ue_radio_cap_id) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [plmn_assi_ue_radio_cap_id]");
        goto end;
    }
    }

    if (mm_context->man_assi_ue_radio_cap_id) {
    if (cJSON_AddStringToObject(item, "manAssiUeRadioCapId", mm_context->man_assi_ue_radio_cap_id) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [man_assi_ue_radio_cap_id]");
        goto end;
    }
    }

    if (mm_context->ucmf_dic_entry_id) {
    if (cJSON_AddStringToObject(item, "ucmfDicEntryId", mm_context->ucmf_dic_entry_id) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [ucmf_dic_entry_id]");
        goto end;
    }
    }

    if (mm_context->n3_iwf_id) {
    cJSON *n3_iwf_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(mm_context->n3_iwf_id);
    if (n3_iwf_id_local_JSON == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [n3_iwf_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n3IwfId", n3_iwf_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [n3_iwf_id]");
        goto end;
    }
    }

    if (mm_context->wagf_id) {
    cJSON *wagf_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(mm_context->wagf_id);
    if (wagf_id_local_JSON == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [wagf_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wagfId", wagf_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [wagf_id]");
        goto end;
    }
    }

    if (mm_context->tngf_id) {
    cJSON *tngf_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(mm_context->tngf_id);
    if (tngf_id_local_JSON == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [tngf_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tngfId", tngf_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [tngf_id]");
        goto end;
    }
    }

    if (mm_context->is_an_n2_ap_id) {
    if (cJSON_AddNumberToObject(item, "anN2ApId", mm_context->an_n2_ap_id) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [an_n2_ap_id]");
        goto end;
    }
    }

    if (mm_context->nssaa_status_list) {
    cJSON *nssaa_status_listList = cJSON_AddArrayToObject(item, "nssaaStatusList");
    if (nssaa_status_listList == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [nssaa_status_list]");
        goto end;
    }
    OpenAPI_list_for_each(mm_context->nssaa_status_list, node) {
        cJSON *itemLocal = OpenAPI_nssaa_status_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [nssaa_status_list]");
            goto end;
        }
        cJSON_AddItemToArray(nssaa_status_listList, itemLocal);
    }
    }

    if (mm_context->pending_nssai_mapping_list) {
    cJSON *pending_nssai_mapping_listList = cJSON_AddArrayToObject(item, "pendingNssaiMappingList");
    if (pending_nssai_mapping_listList == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [pending_nssai_mapping_list]");
        goto end;
    }
    OpenAPI_list_for_each(mm_context->pending_nssai_mapping_list, node) {
        cJSON *itemLocal = OpenAPI_nssai_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [pending_nssai_mapping_list]");
            goto end;
        }
        cJSON_AddItemToArray(pending_nssai_mapping_listList, itemLocal);
    }
    }

    if (mm_context->uuaa_mm_status != OpenAPI_uuaa_mm_status_NULL) {
    if (cJSON_AddStringToObject(item, "uuaaMmStatus", OpenAPI_uuaa_mm_status_ToString(mm_context->uuaa_mm_status)) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [uuaa_mm_status]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mm_context_t *OpenAPI_mm_context_parseFromJSON(cJSON *mm_contextJSON)
{
    OpenAPI_mm_context_t *mm_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *nas_security_mode = NULL;
    OpenAPI_nas_security_mode_t *nas_security_mode_local_nonprim = NULL;
    cJSON *eps_nas_security_mode = NULL;
    OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode_local_nonprim = NULL;
    cJSON *nas_downlink_count = NULL;
    cJSON *nas_uplink_count = NULL;
    cJSON *ue_security_capability = NULL;
    cJSON *s1_ue_network_capability = NULL;
    cJSON *allowed_nssai = NULL;
    OpenAPI_list_t *allowed_nssaiList = NULL;
    cJSON *nssai_mapping_list = NULL;
    OpenAPI_list_t *nssai_mapping_listList = NULL;
    cJSON *allowed_home_nssai = NULL;
    OpenAPI_list_t *allowed_home_nssaiList = NULL;
    cJSON *ns_instance_list = NULL;
    OpenAPI_list_t *ns_instance_listList = NULL;
    cJSON *expected_u_ebehavior = NULL;
    OpenAPI_expected_ue_behavior_t *expected_u_ebehavior_local_nonprim = NULL;
    cJSON *ue_differentiation_info = NULL;
    OpenAPI_ue_differentiation_info_t *ue_differentiation_info_local_nonprim = NULL;
    cJSON *plmn_assi_ue_radio_cap_id = NULL;
    cJSON *man_assi_ue_radio_cap_id = NULL;
    cJSON *ucmf_dic_entry_id = NULL;
    cJSON *n3_iwf_id = NULL;
    OpenAPI_global_ran_node_id_t *n3_iwf_id_local_nonprim = NULL;
    cJSON *wagf_id = NULL;
    OpenAPI_global_ran_node_id_t *wagf_id_local_nonprim = NULL;
    cJSON *tngf_id = NULL;
    OpenAPI_global_ran_node_id_t *tngf_id_local_nonprim = NULL;
    cJSON *an_n2_ap_id = NULL;
    cJSON *nssaa_status_list = NULL;
    OpenAPI_list_t *nssaa_status_listList = NULL;
    cJSON *pending_nssai_mapping_list = NULL;
    OpenAPI_list_t *pending_nssai_mapping_listList = NULL;
    cJSON *uuaa_mm_status = NULL;
    OpenAPI_uuaa_mm_status_e uuaa_mm_statusVariable = 0;
    access_type = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [access_type]");
        goto end;
    }
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    nas_security_mode = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nasSecurityMode");
    if (nas_security_mode) {
    nas_security_mode_local_nonprim = OpenAPI_nas_security_mode_parseFromJSON(nas_security_mode);
    if (!nas_security_mode_local_nonprim) {
        ogs_error("OpenAPI_nas_security_mode_parseFromJSON failed [nas_security_mode]");
        goto end;
    }
    }

    eps_nas_security_mode = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "epsNasSecurityMode");
    if (eps_nas_security_mode) {
    eps_nas_security_mode_local_nonprim = OpenAPI_eps_nas_security_mode_parseFromJSON(eps_nas_security_mode);
    if (!eps_nas_security_mode_local_nonprim) {
        ogs_error("OpenAPI_eps_nas_security_mode_parseFromJSON failed [eps_nas_security_mode]");
        goto end;
    }
    }

    nas_downlink_count = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nasDownlinkCount");
    if (nas_downlink_count) {
    if (!cJSON_IsNumber(nas_downlink_count)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nas_downlink_count]");
        goto end;
    }
    }

    nas_uplink_count = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nasUplinkCount");
    if (nas_uplink_count) {
    if (!cJSON_IsNumber(nas_uplink_count)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nas_uplink_count]");
        goto end;
    }
    }

    ue_security_capability = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "ueSecurityCapability");
    if (ue_security_capability) {
    if (!cJSON_IsString(ue_security_capability) && !cJSON_IsNull(ue_security_capability)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ue_security_capability]");
        goto end;
    }
    }

    s1_ue_network_capability = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "s1UeNetworkCapability");
    if (s1_ue_network_capability) {
    if (!cJSON_IsString(s1_ue_network_capability) && !cJSON_IsNull(s1_ue_network_capability)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [s1_ue_network_capability]");
        goto end;
    }
    }

    allowed_nssai = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "allowedNssai");
    if (allowed_nssai) {
        cJSON *allowed_nssai_local = NULL;
        if (!cJSON_IsArray(allowed_nssai)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [allowed_nssai]");
            goto end;
        }

        allowed_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssai_local, allowed_nssai) {
            if (!cJSON_IsObject(allowed_nssai_local)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [allowed_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_nssaiItem = OpenAPI_snssai_parseFromJSON(allowed_nssai_local);
            if (!allowed_nssaiItem) {
                ogs_error("No allowed_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(allowed_nssaiList, allowed_nssaiItem);
        }
    }

    nssai_mapping_list = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nssaiMappingList");
    if (nssai_mapping_list) {
        cJSON *nssai_mapping_list_local = NULL;
        if (!cJSON_IsArray(nssai_mapping_list)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nssai_mapping_list]");
            goto end;
        }

        nssai_mapping_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nssai_mapping_list_local, nssai_mapping_list) {
            if (!cJSON_IsObject(nssai_mapping_list_local)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nssai_mapping_list]");
                goto end;
            }
            OpenAPI_nssai_mapping_t *nssai_mapping_listItem = OpenAPI_nssai_mapping_parseFromJSON(nssai_mapping_list_local);
            if (!nssai_mapping_listItem) {
                ogs_error("No nssai_mapping_listItem");
                goto end;
            }
            OpenAPI_list_add(nssai_mapping_listList, nssai_mapping_listItem);
        }
    }

    allowed_home_nssai = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "allowedHomeNssai");
    if (allowed_home_nssai) {
        cJSON *allowed_home_nssai_local = NULL;
        if (!cJSON_IsArray(allowed_home_nssai)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [allowed_home_nssai]");
            goto end;
        }

        allowed_home_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_home_nssai_local, allowed_home_nssai) {
            if (!cJSON_IsObject(allowed_home_nssai_local)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [allowed_home_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_home_nssaiItem = OpenAPI_snssai_parseFromJSON(allowed_home_nssai_local);
            if (!allowed_home_nssaiItem) {
                ogs_error("No allowed_home_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(allowed_home_nssaiList, allowed_home_nssaiItem);
        }
    }

    ns_instance_list = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nsInstanceList");
    if (ns_instance_list) {
        cJSON *ns_instance_list_local = NULL;
        if (!cJSON_IsArray(ns_instance_list)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ns_instance_list]");
            goto end;
        }

        ns_instance_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ns_instance_list_local, ns_instance_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ns_instance_list_local)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ns_instance_list]");
                goto end;
            }
            OpenAPI_list_add(ns_instance_listList, ogs_strdup(ns_instance_list_local->valuestring));
        }
    }

    expected_u_ebehavior = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "expectedUEbehavior");
    if (expected_u_ebehavior) {
    expected_u_ebehavior_local_nonprim = OpenAPI_expected_ue_behavior_parseFromJSON(expected_u_ebehavior);
    if (!expected_u_ebehavior_local_nonprim) {
        ogs_error("OpenAPI_expected_ue_behavior_parseFromJSON failed [expected_u_ebehavior]");
        goto end;
    }
    }

    ue_differentiation_info = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "ueDifferentiationInfo");
    if (ue_differentiation_info) {
    ue_differentiation_info_local_nonprim = OpenAPI_ue_differentiation_info_parseFromJSON(ue_differentiation_info);
    if (!ue_differentiation_info_local_nonprim) {
        ogs_error("OpenAPI_ue_differentiation_info_parseFromJSON failed [ue_differentiation_info]");
        goto end;
    }
    }

    plmn_assi_ue_radio_cap_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "plmnAssiUeRadioCapId");
    if (plmn_assi_ue_radio_cap_id) {
    if (!cJSON_IsString(plmn_assi_ue_radio_cap_id) && !cJSON_IsNull(plmn_assi_ue_radio_cap_id)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [plmn_assi_ue_radio_cap_id]");
        goto end;
    }
    }

    man_assi_ue_radio_cap_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "manAssiUeRadioCapId");
    if (man_assi_ue_radio_cap_id) {
    if (!cJSON_IsString(man_assi_ue_radio_cap_id) && !cJSON_IsNull(man_assi_ue_radio_cap_id)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [man_assi_ue_radio_cap_id]");
        goto end;
    }
    }

    ucmf_dic_entry_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "ucmfDicEntryId");
    if (ucmf_dic_entry_id) {
    if (!cJSON_IsString(ucmf_dic_entry_id) && !cJSON_IsNull(ucmf_dic_entry_id)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ucmf_dic_entry_id]");
        goto end;
    }
    }

    n3_iwf_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "n3IwfId");
    if (n3_iwf_id) {
    n3_iwf_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(n3_iwf_id);
    if (!n3_iwf_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [n3_iwf_id]");
        goto end;
    }
    }

    wagf_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "wagfId");
    if (wagf_id) {
    wagf_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(wagf_id);
    if (!wagf_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [wagf_id]");
        goto end;
    }
    }

    tngf_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "tngfId");
    if (tngf_id) {
    tngf_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(tngf_id);
    if (!tngf_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [tngf_id]");
        goto end;
    }
    }

    an_n2_ap_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "anN2ApId");
    if (an_n2_ap_id) {
    if (!cJSON_IsNumber(an_n2_ap_id)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [an_n2_ap_id]");
        goto end;
    }
    }

    nssaa_status_list = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nssaaStatusList");
    if (nssaa_status_list) {
        cJSON *nssaa_status_list_local = NULL;
        if (!cJSON_IsArray(nssaa_status_list)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nssaa_status_list]");
            goto end;
        }

        nssaa_status_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nssaa_status_list_local, nssaa_status_list) {
            if (!cJSON_IsObject(nssaa_status_list_local)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nssaa_status_list]");
                goto end;
            }
            OpenAPI_nssaa_status_t *nssaa_status_listItem = OpenAPI_nssaa_status_parseFromJSON(nssaa_status_list_local);
            if (!nssaa_status_listItem) {
                ogs_error("No nssaa_status_listItem");
                goto end;
            }
            OpenAPI_list_add(nssaa_status_listList, nssaa_status_listItem);
        }
    }

    pending_nssai_mapping_list = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "pendingNssaiMappingList");
    if (pending_nssai_mapping_list) {
        cJSON *pending_nssai_mapping_list_local = NULL;
        if (!cJSON_IsArray(pending_nssai_mapping_list)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [pending_nssai_mapping_list]");
            goto end;
        }

        pending_nssai_mapping_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pending_nssai_mapping_list_local, pending_nssai_mapping_list) {
            if (!cJSON_IsObject(pending_nssai_mapping_list_local)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [pending_nssai_mapping_list]");
                goto end;
            }
            OpenAPI_nssai_mapping_t *pending_nssai_mapping_listItem = OpenAPI_nssai_mapping_parseFromJSON(pending_nssai_mapping_list_local);
            if (!pending_nssai_mapping_listItem) {
                ogs_error("No pending_nssai_mapping_listItem");
                goto end;
            }
            OpenAPI_list_add(pending_nssai_mapping_listList, pending_nssai_mapping_listItem);
        }
    }

    uuaa_mm_status = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "uuaaMmStatus");
    if (uuaa_mm_status) {
    if (!cJSON_IsString(uuaa_mm_status)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [uuaa_mm_status]");
        goto end;
    }
    uuaa_mm_statusVariable = OpenAPI_uuaa_mm_status_FromString(uuaa_mm_status->valuestring);
    }

    mm_context_local_var = OpenAPI_mm_context_create (
        access_typeVariable,
        nas_security_mode ? nas_security_mode_local_nonprim : NULL,
        eps_nas_security_mode ? eps_nas_security_mode_local_nonprim : NULL,
        nas_downlink_count ? true : false,
        nas_downlink_count ? nas_downlink_count->valuedouble : 0,
        nas_uplink_count ? true : false,
        nas_uplink_count ? nas_uplink_count->valuedouble : 0,
        ue_security_capability && !cJSON_IsNull(ue_security_capability) ? ogs_strdup(ue_security_capability->valuestring) : NULL,
        s1_ue_network_capability && !cJSON_IsNull(s1_ue_network_capability) ? ogs_strdup(s1_ue_network_capability->valuestring) : NULL,
        allowed_nssai ? allowed_nssaiList : NULL,
        nssai_mapping_list ? nssai_mapping_listList : NULL,
        allowed_home_nssai ? allowed_home_nssaiList : NULL,
        ns_instance_list ? ns_instance_listList : NULL,
        expected_u_ebehavior ? expected_u_ebehavior_local_nonprim : NULL,
        ue_differentiation_info ? ue_differentiation_info_local_nonprim : NULL,
        plmn_assi_ue_radio_cap_id && !cJSON_IsNull(plmn_assi_ue_radio_cap_id) ? ogs_strdup(plmn_assi_ue_radio_cap_id->valuestring) : NULL,
        man_assi_ue_radio_cap_id && !cJSON_IsNull(man_assi_ue_radio_cap_id) ? ogs_strdup(man_assi_ue_radio_cap_id->valuestring) : NULL,
        ucmf_dic_entry_id && !cJSON_IsNull(ucmf_dic_entry_id) ? ogs_strdup(ucmf_dic_entry_id->valuestring) : NULL,
        n3_iwf_id ? n3_iwf_id_local_nonprim : NULL,
        wagf_id ? wagf_id_local_nonprim : NULL,
        tngf_id ? tngf_id_local_nonprim : NULL,
        an_n2_ap_id ? true : false,
        an_n2_ap_id ? an_n2_ap_id->valuedouble : 0,
        nssaa_status_list ? nssaa_status_listList : NULL,
        pending_nssai_mapping_list ? pending_nssai_mapping_listList : NULL,
        uuaa_mm_status ? uuaa_mm_statusVariable : 0
    );

    return mm_context_local_var;
end:
    if (nas_security_mode_local_nonprim) {
        OpenAPI_nas_security_mode_free(nas_security_mode_local_nonprim);
        nas_security_mode_local_nonprim = NULL;
    }
    if (eps_nas_security_mode_local_nonprim) {
        OpenAPI_eps_nas_security_mode_free(eps_nas_security_mode_local_nonprim);
        eps_nas_security_mode_local_nonprim = NULL;
    }
    if (allowed_nssaiList) {
        OpenAPI_list_for_each(allowed_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_nssaiList);
        allowed_nssaiList = NULL;
    }
    if (nssai_mapping_listList) {
        OpenAPI_list_for_each(nssai_mapping_listList, node) {
            OpenAPI_nssai_mapping_free(node->data);
        }
        OpenAPI_list_free(nssai_mapping_listList);
        nssai_mapping_listList = NULL;
    }
    if (allowed_home_nssaiList) {
        OpenAPI_list_for_each(allowed_home_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_home_nssaiList);
        allowed_home_nssaiList = NULL;
    }
    if (ns_instance_listList) {
        OpenAPI_list_for_each(ns_instance_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ns_instance_listList);
        ns_instance_listList = NULL;
    }
    if (expected_u_ebehavior_local_nonprim) {
        OpenAPI_expected_ue_behavior_free(expected_u_ebehavior_local_nonprim);
        expected_u_ebehavior_local_nonprim = NULL;
    }
    if (ue_differentiation_info_local_nonprim) {
        OpenAPI_ue_differentiation_info_free(ue_differentiation_info_local_nonprim);
        ue_differentiation_info_local_nonprim = NULL;
    }
    if (n3_iwf_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(n3_iwf_id_local_nonprim);
        n3_iwf_id_local_nonprim = NULL;
    }
    if (wagf_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(wagf_id_local_nonprim);
        wagf_id_local_nonprim = NULL;
    }
    if (tngf_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(tngf_id_local_nonprim);
        tngf_id_local_nonprim = NULL;
    }
    if (nssaa_status_listList) {
        OpenAPI_list_for_each(nssaa_status_listList, node) {
            OpenAPI_nssaa_status_free(node->data);
        }
        OpenAPI_list_free(nssaa_status_listList);
        nssaa_status_listList = NULL;
    }
    if (pending_nssai_mapping_listList) {
        OpenAPI_list_for_each(pending_nssai_mapping_listList, node) {
            OpenAPI_nssai_mapping_free(node->data);
        }
        OpenAPI_list_free(pending_nssai_mapping_listList);
        pending_nssai_mapping_listList = NULL;
    }
    return NULL;
}

OpenAPI_mm_context_t *OpenAPI_mm_context_copy(OpenAPI_mm_context_t *dst, OpenAPI_mm_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mm_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed");
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

    OpenAPI_mm_context_free(dst);
    dst = OpenAPI_mm_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

