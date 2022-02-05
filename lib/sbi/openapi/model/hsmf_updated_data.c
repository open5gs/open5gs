
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hsmf_updated_data.h"

OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_create(
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    OpenAPI_list_t *dnai_list,
    char *supported_features,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    OpenAPI_up_security_t *up_security,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    bool is_ipv6_multi_homing_ind,
    int ipv6_multi_homing_ind,
    OpenAPI_list_t *qos_flows_setup_list,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    OpenAPI_list_t *eps_bearer_info,
    bool is_pti,
    int pti
)
{
    OpenAPI_hsmf_updated_data_t *hsmf_updated_data_local_var = ogs_malloc(sizeof(OpenAPI_hsmf_updated_data_t));
    ogs_assert(hsmf_updated_data_local_var);

    hsmf_updated_data_local_var->n1_sm_info_to_ue = n1_sm_info_to_ue;
    hsmf_updated_data_local_var->n4_info = n4_info;
    hsmf_updated_data_local_var->n4_info_ext1 = n4_info_ext1;
    hsmf_updated_data_local_var->n4_info_ext2 = n4_info_ext2;
    hsmf_updated_data_local_var->dnai_list = dnai_list;
    hsmf_updated_data_local_var->supported_features = supported_features;
    hsmf_updated_data_local_var->roaming_charging_profile = roaming_charging_profile;
    hsmf_updated_data_local_var->up_security = up_security;
    hsmf_updated_data_local_var->max_integrity_protected_data_rate_ul = max_integrity_protected_data_rate_ul;
    hsmf_updated_data_local_var->max_integrity_protected_data_rate_dl = max_integrity_protected_data_rate_dl;
    hsmf_updated_data_local_var->is_ipv6_multi_homing_ind = is_ipv6_multi_homing_ind;
    hsmf_updated_data_local_var->ipv6_multi_homing_ind = ipv6_multi_homing_ind;
    hsmf_updated_data_local_var->qos_flows_setup_list = qos_flows_setup_list;
    hsmf_updated_data_local_var->session_ambr = session_ambr;
    hsmf_updated_data_local_var->eps_pdn_cnx_info = eps_pdn_cnx_info;
    hsmf_updated_data_local_var->eps_bearer_info = eps_bearer_info;
    hsmf_updated_data_local_var->is_pti = is_pti;
    hsmf_updated_data_local_var->pti = pti;

    return hsmf_updated_data_local_var;
}

void OpenAPI_hsmf_updated_data_free(OpenAPI_hsmf_updated_data_t *hsmf_updated_data)
{
    if (NULL == hsmf_updated_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ref_to_binary_data_free(hsmf_updated_data->n1_sm_info_to_ue);
    OpenAPI_n4_information_free(hsmf_updated_data->n4_info);
    OpenAPI_n4_information_free(hsmf_updated_data->n4_info_ext1);
    OpenAPI_n4_information_free(hsmf_updated_data->n4_info_ext2);
    OpenAPI_list_for_each(hsmf_updated_data->dnai_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(hsmf_updated_data->dnai_list);
    ogs_free(hsmf_updated_data->supported_features);
    OpenAPI_roaming_charging_profile_free(hsmf_updated_data->roaming_charging_profile);
    OpenAPI_up_security_free(hsmf_updated_data->up_security);
    OpenAPI_list_for_each(hsmf_updated_data->qos_flows_setup_list, node) {
        OpenAPI_qos_flow_setup_item_free(node->data);
    }
    OpenAPI_list_free(hsmf_updated_data->qos_flows_setup_list);
    OpenAPI_ambr_free(hsmf_updated_data->session_ambr);
    OpenAPI_eps_pdn_cnx_info_free(hsmf_updated_data->eps_pdn_cnx_info);
    OpenAPI_list_for_each(hsmf_updated_data->eps_bearer_info, node) {
        OpenAPI_eps_bearer_info_free(node->data);
    }
    OpenAPI_list_free(hsmf_updated_data->eps_bearer_info);
    ogs_free(hsmf_updated_data);
}

cJSON *OpenAPI_hsmf_updated_data_convertToJSON(OpenAPI_hsmf_updated_data_t *hsmf_updated_data)
{
    cJSON *item = NULL;

    if (hsmf_updated_data == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [HsmfUpdatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hsmf_updated_data->n1_sm_info_to_ue) {
    cJSON *n1_sm_info_to_ue_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(hsmf_updated_data->n1_sm_info_to_ue);
    if (n1_sm_info_to_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n1_sm_info_to_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmInfoToUe", n1_sm_info_to_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n1_sm_info_to_ue]");
        goto end;
    }
    }

    if (hsmf_updated_data->n4_info) {
    cJSON *n4_info_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_updated_data->n4_info);
    if (n4_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4Info", n4_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info]");
        goto end;
    }
    }

    if (hsmf_updated_data->n4_info_ext1) {
    cJSON *n4_info_ext1_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_updated_data->n4_info_ext1);
    if (n4_info_ext1_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext1]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4InfoExt1", n4_info_ext1_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext1]");
        goto end;
    }
    }

    if (hsmf_updated_data->n4_info_ext2) {
    cJSON *n4_info_ext2_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_updated_data->n4_info_ext2);
    if (n4_info_ext2_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext2]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4InfoExt2", n4_info_ext2_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext2]");
        goto end;
    }
    }

    if (hsmf_updated_data->dnai_list) {
    cJSON *dnai_list = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_list == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [dnai_list]");
        goto end;
    }

    OpenAPI_lnode_t *dnai_list_node;
    OpenAPI_list_for_each(hsmf_updated_data->dnai_list, dnai_list_node)  {
    if (cJSON_AddStringToObject(dnai_list, "", (char*)dnai_list_node->data) == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [dnai_list]");
        goto end;
    }
                    }
    }

    if (hsmf_updated_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", hsmf_updated_data->supported_features) == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (hsmf_updated_data->roaming_charging_profile) {
    cJSON *roaming_charging_profile_local_JSON = OpenAPI_roaming_charging_profile_convertToJSON(hsmf_updated_data->roaming_charging_profile);
    if (roaming_charging_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingChargingProfile", roaming_charging_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    }

    if (hsmf_updated_data->up_security) {
    cJSON *up_security_local_JSON = OpenAPI_up_security_convertToJSON(hsmf_updated_data->up_security);
    if (up_security_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [up_security]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upSecurity", up_security_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [up_security]");
        goto end;
    }
    }

    if (hsmf_updated_data->max_integrity_protected_data_rate_ul) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateUl", OpenAPI_max_integrity_protected_data_rate_ToString(hsmf_updated_data->max_integrity_protected_data_rate_ul)) == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    }

    if (hsmf_updated_data->max_integrity_protected_data_rate_dl) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateDl", OpenAPI_max_integrity_protected_data_rate_ToString(hsmf_updated_data->max_integrity_protected_data_rate_dl)) == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    }

    if (hsmf_updated_data->is_ipv6_multi_homing_ind) {
    if (cJSON_AddBoolToObject(item, "ipv6MultiHomingInd", hsmf_updated_data->ipv6_multi_homing_ind) == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [ipv6_multi_homing_ind]");
        goto end;
    }
    }

    if (hsmf_updated_data->qos_flows_setup_list) {
    cJSON *qos_flows_setup_listList = cJSON_AddArrayToObject(item, "qosFlowsSetupList");
    if (qos_flows_setup_listList == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [qos_flows_setup_list]");
        goto end;
    }

    OpenAPI_lnode_t *qos_flows_setup_list_node;
    if (hsmf_updated_data->qos_flows_setup_list) {
        OpenAPI_list_for_each(hsmf_updated_data->qos_flows_setup_list, qos_flows_setup_list_node) {
            cJSON *itemLocal = OpenAPI_qos_flow_setup_item_convertToJSON(qos_flows_setup_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [qos_flows_setup_list]");
                goto end;
            }
            cJSON_AddItemToArray(qos_flows_setup_listList, itemLocal);
        }
    }
    }

    if (hsmf_updated_data->session_ambr) {
    cJSON *session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(hsmf_updated_data->session_ambr);
    if (session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessionAmbr", session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [session_ambr]");
        goto end;
    }
    }

    if (hsmf_updated_data->eps_pdn_cnx_info) {
    cJSON *eps_pdn_cnx_info_local_JSON = OpenAPI_eps_pdn_cnx_info_convertToJSON(hsmf_updated_data->eps_pdn_cnx_info);
    if (eps_pdn_cnx_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [eps_pdn_cnx_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "epsPdnCnxInfo", eps_pdn_cnx_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [eps_pdn_cnx_info]");
        goto end;
    }
    }

    if (hsmf_updated_data->eps_bearer_info) {
    cJSON *eps_bearer_infoList = cJSON_AddArrayToObject(item, "epsBearerInfo");
    if (eps_bearer_infoList == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [eps_bearer_info]");
        goto end;
    }

    OpenAPI_lnode_t *eps_bearer_info_node;
    if (hsmf_updated_data->eps_bearer_info) {
        OpenAPI_list_for_each(hsmf_updated_data->eps_bearer_info, eps_bearer_info_node) {
            cJSON *itemLocal = OpenAPI_eps_bearer_info_convertToJSON(eps_bearer_info_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [eps_bearer_info]");
                goto end;
            }
            cJSON_AddItemToArray(eps_bearer_infoList, itemLocal);
        }
    }
    }

    if (hsmf_updated_data->is_pti) {
    if (cJSON_AddNumberToObject(item, "pti", hsmf_updated_data->pti) == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [pti]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_parseFromJSON(cJSON *hsmf_updated_dataJSON)
{
    OpenAPI_hsmf_updated_data_t *hsmf_updated_data_local_var = NULL;
    cJSON *n1_sm_info_to_ue = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n1SmInfoToUe");

    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue_local_nonprim = NULL;
    if (n1_sm_info_to_ue) {
    n1_sm_info_to_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_to_ue);
    }

    cJSON *n4_info = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n4Info");

    OpenAPI_n4_information_t *n4_info_local_nonprim = NULL;
    if (n4_info) {
    n4_info_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info);
    }

    cJSON *n4_info_ext1 = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n4InfoExt1");

    OpenAPI_n4_information_t *n4_info_ext1_local_nonprim = NULL;
    if (n4_info_ext1) {
    n4_info_ext1_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext1);
    }

    cJSON *n4_info_ext2 = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n4InfoExt2");

    OpenAPI_n4_information_t *n4_info_ext2_local_nonprim = NULL;
    if (n4_info_ext2) {
    n4_info_ext2_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext2);
    }

    cJSON *dnai_list = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "dnaiList");

    OpenAPI_list_t *dnai_listList;
    if (dnai_list) {
    cJSON *dnai_list_local;
    if (!cJSON_IsArray(dnai_list)) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [dnai_list]");
        goto end;
    }
    dnai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnai_list_local, dnai_list) {
    if (!cJSON_IsString(dnai_list_local)) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_add(dnai_listList , ogs_strdup(dnai_list_local->valuestring));
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "roamingChargingProfile");

    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    if (roaming_charging_profile) {
    roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    }

    cJSON *up_security = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "upSecurity");

    OpenAPI_up_security_t *up_security_local_nonprim = NULL;
    if (up_security) {
    up_security_local_nonprim = OpenAPI_up_security_parseFromJSON(up_security);
    }

    cJSON *max_integrity_protected_data_rate_ul = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "maxIntegrityProtectedDataRateUl");

    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ulVariable;
    if (max_integrity_protected_data_rate_ul) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_ul)) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    max_integrity_protected_data_rate_ulVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_ul->valuestring);
    }

    cJSON *max_integrity_protected_data_rate_dl = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "maxIntegrityProtectedDataRateDl");

    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dlVariable;
    if (max_integrity_protected_data_rate_dl) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_dl)) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    max_integrity_protected_data_rate_dlVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_dl->valuestring);
    }

    cJSON *ipv6_multi_homing_ind = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "ipv6MultiHomingInd");

    if (ipv6_multi_homing_ind) {
    if (!cJSON_IsBool(ipv6_multi_homing_ind)) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [ipv6_multi_homing_ind]");
        goto end;
    }
    }

    cJSON *qos_flows_setup_list = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "qosFlowsSetupList");

    OpenAPI_list_t *qos_flows_setup_listList;
    if (qos_flows_setup_list) {
    cJSON *qos_flows_setup_list_local_nonprimitive;
    if (!cJSON_IsArray(qos_flows_setup_list)){
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [qos_flows_setup_list]");
        goto end;
    }

    qos_flows_setup_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(qos_flows_setup_list_local_nonprimitive, qos_flows_setup_list ) {
        if (!cJSON_IsObject(qos_flows_setup_list_local_nonprimitive)) {
            ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [qos_flows_setup_list]");
            goto end;
        }
        OpenAPI_qos_flow_setup_item_t *qos_flows_setup_listItem = OpenAPI_qos_flow_setup_item_parseFromJSON(qos_flows_setup_list_local_nonprimitive);

        if (!qos_flows_setup_listItem) {
            ogs_error("No qos_flows_setup_listItem");
            OpenAPI_list_free(qos_flows_setup_listList);
            goto end;
        }

        OpenAPI_list_add(qos_flows_setup_listList, qos_flows_setup_listItem);
    }
    }

    cJSON *session_ambr = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "sessionAmbr");

    OpenAPI_ambr_t *session_ambr_local_nonprim = NULL;
    if (session_ambr) {
    session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(session_ambr);
    }

    cJSON *eps_pdn_cnx_info = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "epsPdnCnxInfo");

    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info_local_nonprim = NULL;
    if (eps_pdn_cnx_info) {
    eps_pdn_cnx_info_local_nonprim = OpenAPI_eps_pdn_cnx_info_parseFromJSON(eps_pdn_cnx_info);
    }

    cJSON *eps_bearer_info = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "epsBearerInfo");

    OpenAPI_list_t *eps_bearer_infoList;
    if (eps_bearer_info) {
    cJSON *eps_bearer_info_local_nonprimitive;
    if (!cJSON_IsArray(eps_bearer_info)){
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [eps_bearer_info]");
        goto end;
    }

    eps_bearer_infoList = OpenAPI_list_create();

    cJSON_ArrayForEach(eps_bearer_info_local_nonprimitive, eps_bearer_info ) {
        if (!cJSON_IsObject(eps_bearer_info_local_nonprimitive)) {
            ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [eps_bearer_info]");
            goto end;
        }
        OpenAPI_eps_bearer_info_t *eps_bearer_infoItem = OpenAPI_eps_bearer_info_parseFromJSON(eps_bearer_info_local_nonprimitive);

        if (!eps_bearer_infoItem) {
            ogs_error("No eps_bearer_infoItem");
            OpenAPI_list_free(eps_bearer_infoList);
            goto end;
        }

        OpenAPI_list_add(eps_bearer_infoList, eps_bearer_infoItem);
    }
    }

    cJSON *pti = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "pti");

    if (pti) {
    if (!cJSON_IsNumber(pti)) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [pti]");
        goto end;
    }
    }

    hsmf_updated_data_local_var = OpenAPI_hsmf_updated_data_create (
        n1_sm_info_to_ue ? n1_sm_info_to_ue_local_nonprim : NULL,
        n4_info ? n4_info_local_nonprim : NULL,
        n4_info_ext1 ? n4_info_ext1_local_nonprim : NULL,
        n4_info_ext2 ? n4_info_ext2_local_nonprim : NULL,
        dnai_list ? dnai_listList : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        up_security ? up_security_local_nonprim : NULL,
        max_integrity_protected_data_rate_ul ? max_integrity_protected_data_rate_ulVariable : 0,
        max_integrity_protected_data_rate_dl ? max_integrity_protected_data_rate_dlVariable : 0,
        ipv6_multi_homing_ind ? true : false,
        ipv6_multi_homing_ind ? ipv6_multi_homing_ind->valueint : 0,
        qos_flows_setup_list ? qos_flows_setup_listList : NULL,
        session_ambr ? session_ambr_local_nonprim : NULL,
        eps_pdn_cnx_info ? eps_pdn_cnx_info_local_nonprim : NULL,
        eps_bearer_info ? eps_bearer_infoList : NULL,
        pti ? true : false,
        pti ? pti->valuedouble : 0
    );

    return hsmf_updated_data_local_var;
end:
    return NULL;
}

OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_copy(OpenAPI_hsmf_updated_data_t *dst, OpenAPI_hsmf_updated_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hsmf_updated_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed");
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

    OpenAPI_hsmf_updated_data_free(dst);
    dst = OpenAPI_hsmf_updated_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

