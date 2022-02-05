
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_context_container.h"

OpenAPI_registration_context_container_t *OpenAPI_registration_context_container_create(
    OpenAPI_ue_context_t *ue_context,
    char *local_time_zone,
    OpenAPI_access_type_e an_type,
    int an_n2_ap_id,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    char *initial_amf_name,
    OpenAPI_user_location_t *user_location,
    char *rrc_est_cause,
    bool is_ue_context_request,
    int ue_context_request,
    bool is_initial_amf_n2_ap_id,
    int initial_amf_n2_ap_id,
    char *an_n2_ipv4_addr,
    char *an_n2_ipv6_addr,
    OpenAPI_allowed_nssai_t *allowed_nssai,
    OpenAPI_list_t *configured_nssai,
    OpenAPI_list_t *rejected_nssai_in_plmn,
    OpenAPI_list_t *rejected_nssai_in_ta,
    OpenAPI_plmn_id_t *selected_plmn_id,
    bool is_iab_node_ind,
    int iab_node_ind,
    OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind,
    OpenAPI_lte_m_ind_t *lte_m_ind,
    bool is_authenticated_ind,
    int authenticated_ind,
    OpenAPI_npn_access_info_t *npn_access_info
)
{
    OpenAPI_registration_context_container_t *registration_context_container_local_var = ogs_malloc(sizeof(OpenAPI_registration_context_container_t));
    ogs_assert(registration_context_container_local_var);

    registration_context_container_local_var->ue_context = ue_context;
    registration_context_container_local_var->local_time_zone = local_time_zone;
    registration_context_container_local_var->an_type = an_type;
    registration_context_container_local_var->an_n2_ap_id = an_n2_ap_id;
    registration_context_container_local_var->ran_node_id = ran_node_id;
    registration_context_container_local_var->initial_amf_name = initial_amf_name;
    registration_context_container_local_var->user_location = user_location;
    registration_context_container_local_var->rrc_est_cause = rrc_est_cause;
    registration_context_container_local_var->is_ue_context_request = is_ue_context_request;
    registration_context_container_local_var->ue_context_request = ue_context_request;
    registration_context_container_local_var->is_initial_amf_n2_ap_id = is_initial_amf_n2_ap_id;
    registration_context_container_local_var->initial_amf_n2_ap_id = initial_amf_n2_ap_id;
    registration_context_container_local_var->an_n2_ipv4_addr = an_n2_ipv4_addr;
    registration_context_container_local_var->an_n2_ipv6_addr = an_n2_ipv6_addr;
    registration_context_container_local_var->allowed_nssai = allowed_nssai;
    registration_context_container_local_var->configured_nssai = configured_nssai;
    registration_context_container_local_var->rejected_nssai_in_plmn = rejected_nssai_in_plmn;
    registration_context_container_local_var->rejected_nssai_in_ta = rejected_nssai_in_ta;
    registration_context_container_local_var->selected_plmn_id = selected_plmn_id;
    registration_context_container_local_var->is_iab_node_ind = is_iab_node_ind;
    registration_context_container_local_var->iab_node_ind = iab_node_ind;
    registration_context_container_local_var->ce_mode_b_ind = ce_mode_b_ind;
    registration_context_container_local_var->lte_m_ind = lte_m_ind;
    registration_context_container_local_var->is_authenticated_ind = is_authenticated_ind;
    registration_context_container_local_var->authenticated_ind = authenticated_ind;
    registration_context_container_local_var->npn_access_info = npn_access_info;

    return registration_context_container_local_var;
}

void OpenAPI_registration_context_container_free(OpenAPI_registration_context_container_t *registration_context_container)
{
    if (NULL == registration_context_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ue_context_free(registration_context_container->ue_context);
    ogs_free(registration_context_container->local_time_zone);
    OpenAPI_global_ran_node_id_free(registration_context_container->ran_node_id);
    ogs_free(registration_context_container->initial_amf_name);
    OpenAPI_user_location_free(registration_context_container->user_location);
    ogs_free(registration_context_container->rrc_est_cause);
    ogs_free(registration_context_container->an_n2_ipv4_addr);
    ogs_free(registration_context_container->an_n2_ipv6_addr);
    OpenAPI_allowed_nssai_free(registration_context_container->allowed_nssai);
    OpenAPI_list_for_each(registration_context_container->configured_nssai, node) {
        OpenAPI_configured_snssai_free(node->data);
    }
    OpenAPI_list_free(registration_context_container->configured_nssai);
    OpenAPI_list_for_each(registration_context_container->rejected_nssai_in_plmn, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(registration_context_container->rejected_nssai_in_plmn);
    OpenAPI_list_for_each(registration_context_container->rejected_nssai_in_ta, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(registration_context_container->rejected_nssai_in_ta);
    OpenAPI_plmn_id_free(registration_context_container->selected_plmn_id);
    OpenAPI_ce_mode_b_ind_free(registration_context_container->ce_mode_b_ind);
    OpenAPI_lte_m_ind_free(registration_context_container->lte_m_ind);
    OpenAPI_npn_access_info_free(registration_context_container->npn_access_info);
    ogs_free(registration_context_container);
}

cJSON *OpenAPI_registration_context_container_convertToJSON(OpenAPI_registration_context_container_t *registration_context_container)
{
    cJSON *item = NULL;

    if (registration_context_container == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [RegistrationContextContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *ue_context_local_JSON = OpenAPI_ue_context_convertToJSON(registration_context_container->ue_context);
    if (ue_context_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [ue_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueContext", ue_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [ue_context]");
        goto end;
    }

    if (registration_context_container->local_time_zone) {
    if (cJSON_AddStringToObject(item, "localTimeZone", registration_context_container->local_time_zone) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [local_time_zone]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(registration_context_container->an_type)) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [an_type]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "anN2ApId", registration_context_container->an_n2_ap_id) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [an_n2_ap_id]");
        goto end;
    }

    cJSON *ran_node_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(registration_context_container->ran_node_id);
    if (ran_node_id_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranNodeId", ran_node_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [ran_node_id]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "initialAmfName", registration_context_container->initial_amf_name) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [initial_amf_name]");
        goto end;
    }

    cJSON *user_location_local_JSON = OpenAPI_user_location_convertToJSON(registration_context_container->user_location);
    if (user_location_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [user_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocation", user_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [user_location]");
        goto end;
    }

    if (registration_context_container->rrc_est_cause) {
    if (cJSON_AddStringToObject(item, "rrcEstCause", registration_context_container->rrc_est_cause) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [rrc_est_cause]");
        goto end;
    }
    }

    if (registration_context_container->is_ue_context_request) {
    if (cJSON_AddBoolToObject(item, "ueContextRequest", registration_context_container->ue_context_request) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [ue_context_request]");
        goto end;
    }
    }

    if (registration_context_container->is_initial_amf_n2_ap_id) {
    if (cJSON_AddNumberToObject(item, "initialAmfN2ApId", registration_context_container->initial_amf_n2_ap_id) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [initial_amf_n2_ap_id]");
        goto end;
    }
    }

    if (registration_context_container->an_n2_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "anN2IPv4Addr", registration_context_container->an_n2_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [an_n2_ipv4_addr]");
        goto end;
    }
    }

    if (registration_context_container->an_n2_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "anN2IPv6Addr", registration_context_container->an_n2_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [an_n2_ipv6_addr]");
        goto end;
    }
    }

    if (registration_context_container->allowed_nssai) {
    cJSON *allowed_nssai_local_JSON = OpenAPI_allowed_nssai_convertToJSON(registration_context_container->allowed_nssai);
    if (allowed_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [allowed_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allowedNssai", allowed_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [allowed_nssai]");
        goto end;
    }
    }

    if (registration_context_container->configured_nssai) {
    cJSON *configured_nssaiList = cJSON_AddArrayToObject(item, "configuredNssai");
    if (configured_nssaiList == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [configured_nssai]");
        goto end;
    }

    OpenAPI_lnode_t *configured_nssai_node;
    if (registration_context_container->configured_nssai) {
        OpenAPI_list_for_each(registration_context_container->configured_nssai, configured_nssai_node) {
            cJSON *itemLocal = OpenAPI_configured_snssai_convertToJSON(configured_nssai_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [configured_nssai]");
                goto end;
            }
            cJSON_AddItemToArray(configured_nssaiList, itemLocal);
        }
    }
    }

    if (registration_context_container->rejected_nssai_in_plmn) {
    cJSON *rejected_nssai_in_plmnList = cJSON_AddArrayToObject(item, "rejectedNssaiInPlmn");
    if (rejected_nssai_in_plmnList == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [rejected_nssai_in_plmn]");
        goto end;
    }

    OpenAPI_lnode_t *rejected_nssai_in_plmn_node;
    if (registration_context_container->rejected_nssai_in_plmn) {
        OpenAPI_list_for_each(registration_context_container->rejected_nssai_in_plmn, rejected_nssai_in_plmn_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(rejected_nssai_in_plmn_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [rejected_nssai_in_plmn]");
                goto end;
            }
            cJSON_AddItemToArray(rejected_nssai_in_plmnList, itemLocal);
        }
    }
    }

    if (registration_context_container->rejected_nssai_in_ta) {
    cJSON *rejected_nssai_in_taList = cJSON_AddArrayToObject(item, "rejectedNssaiInTa");
    if (rejected_nssai_in_taList == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [rejected_nssai_in_ta]");
        goto end;
    }

    OpenAPI_lnode_t *rejected_nssai_in_ta_node;
    if (registration_context_container->rejected_nssai_in_ta) {
        OpenAPI_list_for_each(registration_context_container->rejected_nssai_in_ta, rejected_nssai_in_ta_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(rejected_nssai_in_ta_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [rejected_nssai_in_ta]");
                goto end;
            }
            cJSON_AddItemToArray(rejected_nssai_in_taList, itemLocal);
        }
    }
    }

    if (registration_context_container->selected_plmn_id) {
    cJSON *selected_plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(registration_context_container->selected_plmn_id);
    if (selected_plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [selected_plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "selectedPlmnId", selected_plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [selected_plmn_id]");
        goto end;
    }
    }

    if (registration_context_container->is_iab_node_ind) {
    if (cJSON_AddBoolToObject(item, "iabNodeInd", registration_context_container->iab_node_ind) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [iab_node_ind]");
        goto end;
    }
    }

    if (registration_context_container->ce_mode_b_ind) {
    cJSON *ce_mode_b_ind_local_JSON = OpenAPI_ce_mode_b_ind_convertToJSON(registration_context_container->ce_mode_b_ind);
    if (ce_mode_b_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [ce_mode_b_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ceModeBInd", ce_mode_b_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [ce_mode_b_ind]");
        goto end;
    }
    }

    if (registration_context_container->lte_m_ind) {
    cJSON *lte_m_ind_local_JSON = OpenAPI_lte_m_ind_convertToJSON(registration_context_container->lte_m_ind);
    if (lte_m_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [lte_m_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lteMInd", lte_m_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [lte_m_ind]");
        goto end;
    }
    }

    if (registration_context_container->is_authenticated_ind) {
    if (cJSON_AddBoolToObject(item, "authenticatedInd", registration_context_container->authenticated_ind) == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [authenticated_ind]");
        goto end;
    }
    }

    if (registration_context_container->npn_access_info) {
    cJSON *npn_access_info_local_JSON = OpenAPI_npn_access_info_convertToJSON(registration_context_container->npn_access_info);
    if (npn_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [npn_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "npnAccessInfo", npn_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed [npn_access_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_registration_context_container_t *OpenAPI_registration_context_container_parseFromJSON(cJSON *registration_context_containerJSON)
{
    OpenAPI_registration_context_container_t *registration_context_container_local_var = NULL;
    cJSON *ue_context = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "ueContext");
    if (!ue_context) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [ue_context]");
        goto end;
    }

    OpenAPI_ue_context_t *ue_context_local_nonprim = NULL;
    ue_context_local_nonprim = OpenAPI_ue_context_parseFromJSON(ue_context);

    cJSON *local_time_zone = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "localTimeZone");

    if (local_time_zone) {
    if (!cJSON_IsString(local_time_zone)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [local_time_zone]");
        goto end;
    }
    }

    cJSON *an_type = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "anType");
    if (!an_type) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [an_type]");
        goto end;
    }

    OpenAPI_access_type_e an_typeVariable;
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);

    cJSON *an_n2_ap_id = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "anN2ApId");
    if (!an_n2_ap_id) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [an_n2_ap_id]");
        goto end;
    }

    if (!cJSON_IsNumber(an_n2_ap_id)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [an_n2_ap_id]");
        goto end;
    }

    cJSON *ran_node_id = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "ranNodeId");
    if (!ran_node_id) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [ran_node_id]");
        goto end;
    }

    OpenAPI_global_ran_node_id_t *ran_node_id_local_nonprim = NULL;
    ran_node_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id);

    cJSON *initial_amf_name = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "initialAmfName");
    if (!initial_amf_name) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [initial_amf_name]");
        goto end;
    }

    if (!cJSON_IsString(initial_amf_name)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [initial_amf_name]");
        goto end;
    }

    cJSON *user_location = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "userLocation");
    if (!user_location) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [user_location]");
        goto end;
    }

    OpenAPI_user_location_t *user_location_local_nonprim = NULL;
    user_location_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location);

    cJSON *rrc_est_cause = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "rrcEstCause");

    if (rrc_est_cause) {
    if (!cJSON_IsString(rrc_est_cause)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [rrc_est_cause]");
        goto end;
    }
    }

    cJSON *ue_context_request = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "ueContextRequest");

    if (ue_context_request) {
    if (!cJSON_IsBool(ue_context_request)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [ue_context_request]");
        goto end;
    }
    }

    cJSON *initial_amf_n2_ap_id = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "initialAmfN2ApId");

    if (initial_amf_n2_ap_id) {
    if (!cJSON_IsNumber(initial_amf_n2_ap_id)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [initial_amf_n2_ap_id]");
        goto end;
    }
    }

    cJSON *an_n2_ipv4_addr = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "anN2IPv4Addr");

    if (an_n2_ipv4_addr) {
    if (!cJSON_IsString(an_n2_ipv4_addr)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [an_n2_ipv4_addr]");
        goto end;
    }
    }

    cJSON *an_n2_ipv6_addr = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "anN2IPv6Addr");

    if (an_n2_ipv6_addr) {
    if (!cJSON_IsString(an_n2_ipv6_addr)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [an_n2_ipv6_addr]");
        goto end;
    }
    }

    cJSON *allowed_nssai = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "allowedNssai");

    OpenAPI_allowed_nssai_t *allowed_nssai_local_nonprim = NULL;
    if (allowed_nssai) {
    allowed_nssai_local_nonprim = OpenAPI_allowed_nssai_parseFromJSON(allowed_nssai);
    }

    cJSON *configured_nssai = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "configuredNssai");

    OpenAPI_list_t *configured_nssaiList;
    if (configured_nssai) {
    cJSON *configured_nssai_local_nonprimitive;
    if (!cJSON_IsArray(configured_nssai)){
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [configured_nssai]");
        goto end;
    }

    configured_nssaiList = OpenAPI_list_create();

    cJSON_ArrayForEach(configured_nssai_local_nonprimitive, configured_nssai ) {
        if (!cJSON_IsObject(configured_nssai_local_nonprimitive)) {
            ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [configured_nssai]");
            goto end;
        }
        OpenAPI_configured_snssai_t *configured_nssaiItem = OpenAPI_configured_snssai_parseFromJSON(configured_nssai_local_nonprimitive);

        if (!configured_nssaiItem) {
            ogs_error("No configured_nssaiItem");
            OpenAPI_list_free(configured_nssaiList);
            goto end;
        }

        OpenAPI_list_add(configured_nssaiList, configured_nssaiItem);
    }
    }

    cJSON *rejected_nssai_in_plmn = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "rejectedNssaiInPlmn");

    OpenAPI_list_t *rejected_nssai_in_plmnList;
    if (rejected_nssai_in_plmn) {
    cJSON *rejected_nssai_in_plmn_local_nonprimitive;
    if (!cJSON_IsArray(rejected_nssai_in_plmn)){
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [rejected_nssai_in_plmn]");
        goto end;
    }

    rejected_nssai_in_plmnList = OpenAPI_list_create();

    cJSON_ArrayForEach(rejected_nssai_in_plmn_local_nonprimitive, rejected_nssai_in_plmn ) {
        if (!cJSON_IsObject(rejected_nssai_in_plmn_local_nonprimitive)) {
            ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [rejected_nssai_in_plmn]");
            goto end;
        }
        OpenAPI_snssai_t *rejected_nssai_in_plmnItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_in_plmn_local_nonprimitive);

        if (!rejected_nssai_in_plmnItem) {
            ogs_error("No rejected_nssai_in_plmnItem");
            OpenAPI_list_free(rejected_nssai_in_plmnList);
            goto end;
        }

        OpenAPI_list_add(rejected_nssai_in_plmnList, rejected_nssai_in_plmnItem);
    }
    }

    cJSON *rejected_nssai_in_ta = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "rejectedNssaiInTa");

    OpenAPI_list_t *rejected_nssai_in_taList;
    if (rejected_nssai_in_ta) {
    cJSON *rejected_nssai_in_ta_local_nonprimitive;
    if (!cJSON_IsArray(rejected_nssai_in_ta)){
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [rejected_nssai_in_ta]");
        goto end;
    }

    rejected_nssai_in_taList = OpenAPI_list_create();

    cJSON_ArrayForEach(rejected_nssai_in_ta_local_nonprimitive, rejected_nssai_in_ta ) {
        if (!cJSON_IsObject(rejected_nssai_in_ta_local_nonprimitive)) {
            ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [rejected_nssai_in_ta]");
            goto end;
        }
        OpenAPI_snssai_t *rejected_nssai_in_taItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_in_ta_local_nonprimitive);

        if (!rejected_nssai_in_taItem) {
            ogs_error("No rejected_nssai_in_taItem");
            OpenAPI_list_free(rejected_nssai_in_taList);
            goto end;
        }

        OpenAPI_list_add(rejected_nssai_in_taList, rejected_nssai_in_taItem);
    }
    }

    cJSON *selected_plmn_id = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "selectedPlmnId");

    OpenAPI_plmn_id_t *selected_plmn_id_local_nonprim = NULL;
    if (selected_plmn_id) {
    selected_plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(selected_plmn_id);
    }

    cJSON *iab_node_ind = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "iabNodeInd");

    if (iab_node_ind) {
    if (!cJSON_IsBool(iab_node_ind)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [iab_node_ind]");
        goto end;
    }
    }

    cJSON *ce_mode_b_ind = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "ceModeBInd");

    OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind_local_nonprim = NULL;
    if (ce_mode_b_ind) {
    ce_mode_b_ind_local_nonprim = OpenAPI_ce_mode_b_ind_parseFromJSON(ce_mode_b_ind);
    }

    cJSON *lte_m_ind = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "lteMInd");

    OpenAPI_lte_m_ind_t *lte_m_ind_local_nonprim = NULL;
    if (lte_m_ind) {
    lte_m_ind_local_nonprim = OpenAPI_lte_m_ind_parseFromJSON(lte_m_ind);
    }

    cJSON *authenticated_ind = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "authenticatedInd");

    if (authenticated_ind) {
    if (!cJSON_IsBool(authenticated_ind)) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON() failed [authenticated_ind]");
        goto end;
    }
    }

    cJSON *npn_access_info = cJSON_GetObjectItemCaseSensitive(registration_context_containerJSON, "npnAccessInfo");

    OpenAPI_npn_access_info_t *npn_access_info_local_nonprim = NULL;
    if (npn_access_info) {
    npn_access_info_local_nonprim = OpenAPI_npn_access_info_parseFromJSON(npn_access_info);
    }

    registration_context_container_local_var = OpenAPI_registration_context_container_create (
        ue_context_local_nonprim,
        local_time_zone ? ogs_strdup(local_time_zone->valuestring) : NULL,
        an_typeVariable,
        
        an_n2_ap_id->valuedouble,
        ran_node_id_local_nonprim,
        ogs_strdup(initial_amf_name->valuestring),
        user_location_local_nonprim,
        rrc_est_cause ? ogs_strdup(rrc_est_cause->valuestring) : NULL,
        ue_context_request ? true : false,
        ue_context_request ? ue_context_request->valueint : 0,
        initial_amf_n2_ap_id ? true : false,
        initial_amf_n2_ap_id ? initial_amf_n2_ap_id->valuedouble : 0,
        an_n2_ipv4_addr ? ogs_strdup(an_n2_ipv4_addr->valuestring) : NULL,
        an_n2_ipv6_addr ? ogs_strdup(an_n2_ipv6_addr->valuestring) : NULL,
        allowed_nssai ? allowed_nssai_local_nonprim : NULL,
        configured_nssai ? configured_nssaiList : NULL,
        rejected_nssai_in_plmn ? rejected_nssai_in_plmnList : NULL,
        rejected_nssai_in_ta ? rejected_nssai_in_taList : NULL,
        selected_plmn_id ? selected_plmn_id_local_nonprim : NULL,
        iab_node_ind ? true : false,
        iab_node_ind ? iab_node_ind->valueint : 0,
        ce_mode_b_ind ? ce_mode_b_ind_local_nonprim : NULL,
        lte_m_ind ? lte_m_ind_local_nonprim : NULL,
        authenticated_ind ? true : false,
        authenticated_ind ? authenticated_ind->valueint : 0,
        npn_access_info ? npn_access_info_local_nonprim : NULL
    );

    return registration_context_container_local_var;
end:
    return NULL;
}

OpenAPI_registration_context_container_t *OpenAPI_registration_context_container_copy(OpenAPI_registration_context_container_t *dst, OpenAPI_registration_context_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_registration_context_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_registration_context_container_convertToJSON() failed");
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

    OpenAPI_registration_context_container_free(dst);
    dst = OpenAPI_registration_context_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

