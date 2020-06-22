
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context.h"

OpenAPI_sm_context_t *OpenAPI_sm_context_create(
    int pdu_session_id,
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *hplmn_snssai,
    OpenAPI_pdu_session_type_e pdu_session_type,
    char *gpsi,
    char *h_smf_uri,
    char *smf_uri,
    char *pdu_session_ref,
    char *pcf_id,
    char *pcf_group_id,
    char *pcf_set_id,
    OpenAPI_dnn_selection_mode_e sel_mode,
    char *udm_group_id,
    char *routing_indicator,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_list_t *qos_flows_list,
    char *h_smf_instance_id,
    char *smf_instance_id,
    int enable_pause_charging,
    char *ue_ipv4_address,
    char *ue_ipv6_prefix,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    OpenAPI_list_t *eps_bearer_info,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate,
    int always_on_granted,
    OpenAPI_up_security_t *up_security,
    char *h_smf_service_instance_id,
    char *smf_service_instance_id,
    char *recovery_time,
    int forwarding_ind,
    OpenAPI_tunnel_info_t *psa_tunnel_info,
    char *home_provided_charging_id,
    OpenAPI_charging_information_t *charging_info,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    int nef_ext_buf_support_ind
    )
{
    OpenAPI_sm_context_t *sm_context_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_context_t));
    if (!sm_context_local_var) {
        return NULL;
    }
    sm_context_local_var->pdu_session_id = pdu_session_id;
    sm_context_local_var->dnn = dnn;
    sm_context_local_var->s_nssai = s_nssai;
    sm_context_local_var->hplmn_snssai = hplmn_snssai;
    sm_context_local_var->pdu_session_type = pdu_session_type;
    sm_context_local_var->gpsi = gpsi;
    sm_context_local_var->h_smf_uri = h_smf_uri;
    sm_context_local_var->smf_uri = smf_uri;
    sm_context_local_var->pdu_session_ref = pdu_session_ref;
    sm_context_local_var->pcf_id = pcf_id;
    sm_context_local_var->pcf_group_id = pcf_group_id;
    sm_context_local_var->pcf_set_id = pcf_set_id;
    sm_context_local_var->sel_mode = sel_mode;
    sm_context_local_var->udm_group_id = udm_group_id;
    sm_context_local_var->routing_indicator = routing_indicator;
    sm_context_local_var->session_ambr = session_ambr;
    sm_context_local_var->qos_flows_list = qos_flows_list;
    sm_context_local_var->h_smf_instance_id = h_smf_instance_id;
    sm_context_local_var->smf_instance_id = smf_instance_id;
    sm_context_local_var->enable_pause_charging = enable_pause_charging;
    sm_context_local_var->ue_ipv4_address = ue_ipv4_address;
    sm_context_local_var->ue_ipv6_prefix = ue_ipv6_prefix;
    sm_context_local_var->eps_pdn_cnx_info = eps_pdn_cnx_info;
    sm_context_local_var->eps_bearer_info = eps_bearer_info;
    sm_context_local_var->max_integrity_protected_data_rate = max_integrity_protected_data_rate;
    sm_context_local_var->always_on_granted = always_on_granted;
    sm_context_local_var->up_security = up_security;
    sm_context_local_var->h_smf_service_instance_id = h_smf_service_instance_id;
    sm_context_local_var->smf_service_instance_id = smf_service_instance_id;
    sm_context_local_var->recovery_time = recovery_time;
    sm_context_local_var->forwarding_ind = forwarding_ind;
    sm_context_local_var->psa_tunnel_info = psa_tunnel_info;
    sm_context_local_var->home_provided_charging_id = home_provided_charging_id;
    sm_context_local_var->charging_info = charging_info;
    sm_context_local_var->roaming_charging_profile = roaming_charging_profile;
    sm_context_local_var->nef_ext_buf_support_ind = nef_ext_buf_support_ind;

    return sm_context_local_var;
}

void OpenAPI_sm_context_free(OpenAPI_sm_context_t *sm_context)
{
    if (NULL == sm_context) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_context->dnn);
    OpenAPI_snssai_free(sm_context->s_nssai);
    OpenAPI_snssai_free(sm_context->hplmn_snssai);
    ogs_free(sm_context->gpsi);
    ogs_free(sm_context->h_smf_uri);
    ogs_free(sm_context->smf_uri);
    ogs_free(sm_context->pdu_session_ref);
    ogs_free(sm_context->pcf_id);
    ogs_free(sm_context->pcf_group_id);
    ogs_free(sm_context->pcf_set_id);
    ogs_free(sm_context->udm_group_id);
    ogs_free(sm_context->routing_indicator);
    OpenAPI_ambr_free(sm_context->session_ambr);
    OpenAPI_list_for_each(sm_context->qos_flows_list, node) {
        OpenAPI_qos_flow_setup_item_free(node->data);
    }
    OpenAPI_list_free(sm_context->qos_flows_list);
    ogs_free(sm_context->h_smf_instance_id);
    ogs_free(sm_context->smf_instance_id);
    ogs_free(sm_context->ue_ipv4_address);
    ogs_free(sm_context->ue_ipv6_prefix);
    OpenAPI_eps_pdn_cnx_info_free(sm_context->eps_pdn_cnx_info);
    OpenAPI_list_for_each(sm_context->eps_bearer_info, node) {
        OpenAPI_eps_bearer_info_free(node->data);
    }
    OpenAPI_list_free(sm_context->eps_bearer_info);
    OpenAPI_up_security_free(sm_context->up_security);
    ogs_free(sm_context->h_smf_service_instance_id);
    ogs_free(sm_context->smf_service_instance_id);
    ogs_free(sm_context->recovery_time);
    OpenAPI_tunnel_info_free(sm_context->psa_tunnel_info);
    ogs_free(sm_context->home_provided_charging_id);
    OpenAPI_charging_information_free(sm_context->charging_info);
    OpenAPI_roaming_charging_profile_free(sm_context->roaming_charging_profile);
    ogs_free(sm_context);
}

cJSON *OpenAPI_sm_context_convertToJSON(OpenAPI_sm_context_t *sm_context)
{
    cJSON *item = NULL;

    if (sm_context == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [SmContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_context->pdu_session_id) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pduSessionId", sm_context->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!sm_context->dnn) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", sm_context->dnn) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!sm_context->s_nssai) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (sm_context->hplmn_snssai) {
        cJSON *hplmn_snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context->hplmn_snssai);
        if (hplmn_snssai_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [hplmn_snssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "hplmnSnssai", hplmn_snssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [hplmn_snssai]");
            goto end;
        }
    }

    if (!sm_context->pdu_session_type) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "pduSessionType", OpenAPI_pdu_session_type_ToString(sm_context->pdu_session_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_type]");
        goto end;
    }

    if (sm_context->gpsi) {
        if (cJSON_AddStringToObject(item, "gpsi", sm_context->gpsi) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [gpsi]");
            goto end;
        }
    }

    if (sm_context->h_smf_uri) {
        if (cJSON_AddStringToObject(item, "hSmfUri", sm_context->h_smf_uri) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_smf_uri]");
            goto end;
        }
    }

    if (sm_context->smf_uri) {
        if (cJSON_AddStringToObject(item, "smfUri", sm_context->smf_uri) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_uri]");
            goto end;
        }
    }

    if (sm_context->pdu_session_ref) {
        if (cJSON_AddStringToObject(item, "pduSessionRef", sm_context->pdu_session_ref) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_ref]");
            goto end;
        }
    }

    if (sm_context->pcf_id) {
        if (cJSON_AddStringToObject(item, "pcfId", sm_context->pcf_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [pcf_id]");
            goto end;
        }
    }

    if (sm_context->pcf_group_id) {
        if (cJSON_AddStringToObject(item, "pcfGroupId", sm_context->pcf_group_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [pcf_group_id]");
            goto end;
        }
    }

    if (sm_context->pcf_set_id) {
        if (cJSON_AddStringToObject(item, "pcfSetId", sm_context->pcf_set_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [pcf_set_id]");
            goto end;
        }
    }

    if (sm_context->sel_mode) {
        if (cJSON_AddStringToObject(item, "selMode", OpenAPI_dnn_selection_mode_ToString(sm_context->sel_mode)) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [sel_mode]");
            goto end;
        }
    }

    if (sm_context->udm_group_id) {
        if (cJSON_AddStringToObject(item, "udmGroupId", sm_context->udm_group_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [udm_group_id]");
            goto end;
        }
    }

    if (sm_context->routing_indicator) {
        if (cJSON_AddStringToObject(item, "routingIndicator", sm_context->routing_indicator) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [routing_indicator]");
            goto end;
        }
    }

    if (!sm_context->session_ambr) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON *session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(sm_context->session_ambr);
    if (session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessionAmbr", session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [session_ambr]");
        goto end;
    }

    if (!sm_context->qos_flows_list) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_flows_list]");
        goto end;
    }
    cJSON *qos_flows_listList = cJSON_AddArrayToObject(item, "qosFlowsList");
    if (qos_flows_listList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_flows_list]");
        goto end;
    }

    OpenAPI_lnode_t *qos_flows_list_node;
    if (sm_context->qos_flows_list) {
        OpenAPI_list_for_each(sm_context->qos_flows_list, qos_flows_list_node) {
            cJSON *itemLocal = OpenAPI_qos_flow_setup_item_convertToJSON(qos_flows_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_flows_list]");
                goto end;
            }
            cJSON_AddItemToArray(qos_flows_listList, itemLocal);
        }
    }

    if (sm_context->h_smf_instance_id) {
        if (cJSON_AddStringToObject(item, "hSmfInstanceId", sm_context->h_smf_instance_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_smf_instance_id]");
            goto end;
        }
    }

    if (sm_context->smf_instance_id) {
        if (cJSON_AddStringToObject(item, "smfInstanceId", sm_context->smf_instance_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_instance_id]");
            goto end;
        }
    }

    if (sm_context->enable_pause_charging) {
        if (cJSON_AddBoolToObject(item, "enablePauseCharging", sm_context->enable_pause_charging) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [enable_pause_charging]");
            goto end;
        }
    }

    if (sm_context->ue_ipv4_address) {
        if (cJSON_AddStringToObject(item, "ueIpv4Address", sm_context->ue_ipv4_address) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [ue_ipv4_address]");
            goto end;
        }
    }

    if (sm_context->ue_ipv6_prefix) {
        if (cJSON_AddStringToObject(item, "ueIpv6Prefix", sm_context->ue_ipv6_prefix) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [ue_ipv6_prefix]");
            goto end;
        }
    }

    if (sm_context->eps_pdn_cnx_info) {
        cJSON *eps_pdn_cnx_info_local_JSON = OpenAPI_eps_pdn_cnx_info_convertToJSON(sm_context->eps_pdn_cnx_info);
        if (eps_pdn_cnx_info_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_pdn_cnx_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "epsPdnCnxInfo", eps_pdn_cnx_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_pdn_cnx_info]");
            goto end;
        }
    }

    if (sm_context->eps_bearer_info) {
        cJSON *eps_bearer_infoList = cJSON_AddArrayToObject(item, "epsBearerInfo");
        if (eps_bearer_infoList == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_bearer_info]");
            goto end;
        }

        OpenAPI_lnode_t *eps_bearer_info_node;
        if (sm_context->eps_bearer_info) {
            OpenAPI_list_for_each(sm_context->eps_bearer_info, eps_bearer_info_node) {
                cJSON *itemLocal = OpenAPI_eps_bearer_info_convertToJSON(eps_bearer_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_bearer_info]");
                    goto end;
                }
                cJSON_AddItemToArray(eps_bearer_infoList, itemLocal);
            }
        }
    }

    if (sm_context->max_integrity_protected_data_rate) {
        if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRate", OpenAPI_max_integrity_protected_data_rate_ToString(sm_context->max_integrity_protected_data_rate)) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [max_integrity_protected_data_rate]");
            goto end;
        }
    }

    if (sm_context->always_on_granted) {
        if (cJSON_AddBoolToObject(item, "alwaysOnGranted", sm_context->always_on_granted) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [always_on_granted]");
            goto end;
        }
    }

    if (sm_context->up_security) {
        cJSON *up_security_local_JSON = OpenAPI_up_security_convertToJSON(sm_context->up_security);
        if (up_security_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [up_security]");
            goto end;
        }
        cJSON_AddItemToObject(item, "upSecurity", up_security_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [up_security]");
            goto end;
        }
    }

    if (sm_context->h_smf_service_instance_id) {
        if (cJSON_AddStringToObject(item, "hSmfServiceInstanceId", sm_context->h_smf_service_instance_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_smf_service_instance_id]");
            goto end;
        }
    }

    if (sm_context->smf_service_instance_id) {
        if (cJSON_AddStringToObject(item, "smfServiceInstanceId", sm_context->smf_service_instance_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_service_instance_id]");
            goto end;
        }
    }

    if (sm_context->recovery_time) {
        if (cJSON_AddStringToObject(item, "recoveryTime", sm_context->recovery_time) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [recovery_time]");
            goto end;
        }
    }

    if (sm_context->forwarding_ind) {
        if (cJSON_AddBoolToObject(item, "forwardingInd", sm_context->forwarding_ind) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [forwarding_ind]");
            goto end;
        }
    }

    if (sm_context->psa_tunnel_info) {
        cJSON *psa_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(sm_context->psa_tunnel_info);
        if (psa_tunnel_info_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [psa_tunnel_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "psaTunnelInfo", psa_tunnel_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [psa_tunnel_info]");
            goto end;
        }
    }

    if (sm_context->home_provided_charging_id) {
        if (cJSON_AddStringToObject(item, "homeProvidedChargingId", sm_context->home_provided_charging_id) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [home_provided_charging_id]");
            goto end;
        }
    }

    if (sm_context->charging_info) {
        cJSON *charging_info_local_JSON = OpenAPI_charging_information_convertToJSON(sm_context->charging_info);
        if (charging_info_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [charging_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "chargingInfo", charging_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [charging_info]");
            goto end;
        }
    }

    if (sm_context->roaming_charging_profile) {
        cJSON *roaming_charging_profile_local_JSON = OpenAPI_roaming_charging_profile_convertToJSON(sm_context->roaming_charging_profile);
        if (roaming_charging_profile_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [roaming_charging_profile]");
            goto end;
        }
        cJSON_AddItemToObject(item, "roamingChargingProfile", roaming_charging_profile_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [roaming_charging_profile]");
            goto end;
        }
    }

    if (sm_context->nef_ext_buf_support_ind) {
        if (cJSON_AddBoolToObject(item, "nefExtBufSupportInd", sm_context->nef_ext_buf_support_ind) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [nef_ext_buf_support_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sm_context_t *OpenAPI_sm_context_parseFromJSON(cJSON *sm_contextJSON)
{
    OpenAPI_sm_context_t *sm_context_local_var = NULL;
    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }


    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [s_nssai]");
        goto end;
    }

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;

    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);

    cJSON *hplmn_snssai = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hplmnSnssai");

    OpenAPI_snssai_t *hplmn_snssai_local_nonprim = NULL;
    if (hplmn_snssai) {
        hplmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(hplmn_snssai);
    }

    cJSON *pdu_session_type = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionType");
    if (!pdu_session_type) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }

    OpenAPI_pdu_session_type_e pdu_session_typeVariable;

    if (!cJSON_IsString(pdu_session_type)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }
    pdu_session_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_session_type->valuestring);

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "gpsi");

    if (gpsi) {
        if (!cJSON_IsString(gpsi)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [gpsi]");
            goto end;
        }
    }

    cJSON *h_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hSmfUri");

    if (h_smf_uri) {
        if (!cJSON_IsString(h_smf_uri)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [h_smf_uri]");
            goto end;
        }
    }

    cJSON *smf_uri = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfUri");

    if (smf_uri) {
        if (!cJSON_IsString(smf_uri)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_uri]");
            goto end;
        }
    }

    cJSON *pdu_session_ref = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionRef");

    if (pdu_session_ref) {
        if (!cJSON_IsString(pdu_session_ref)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_ref]");
            goto end;
        }
    }

    cJSON *pcf_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pcfId");

    if (pcf_id) {
        if (!cJSON_IsString(pcf_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pcf_id]");
            goto end;
        }
    }

    cJSON *pcf_group_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pcfGroupId");

    if (pcf_group_id) {
        if (!cJSON_IsString(pcf_group_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pcf_group_id]");
            goto end;
        }
    }

    cJSON *pcf_set_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pcfSetId");

    if (pcf_set_id) {
        if (!cJSON_IsString(pcf_set_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pcf_set_id]");
            goto end;
        }
    }

    cJSON *sel_mode = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "selMode");

    OpenAPI_dnn_selection_mode_e sel_modeVariable;
    if (sel_mode) {
        if (!cJSON_IsString(sel_mode)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [sel_mode]");
            goto end;
        }
        sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(sel_mode->valuestring);
    }

    cJSON *udm_group_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "udmGroupId");

    if (udm_group_id) {
        if (!cJSON_IsString(udm_group_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [udm_group_id]");
            goto end;
        }
    }

    cJSON *routing_indicator = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "routingIndicator");

    if (routing_indicator) {
        if (!cJSON_IsString(routing_indicator)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [routing_indicator]");
            goto end;
        }
    }

    cJSON *session_ambr = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "sessionAmbr");
    if (!session_ambr) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [session_ambr]");
        goto end;
    }

    OpenAPI_ambr_t *session_ambr_local_nonprim = NULL;

    session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(session_ambr);

    cJSON *qos_flows_list = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "qosFlowsList");
    if (!qos_flows_list) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [qos_flows_list]");
        goto end;
    }

    OpenAPI_list_t *qos_flows_listList;

    cJSON *qos_flows_list_local_nonprimitive;
    if (!cJSON_IsArray(qos_flows_list)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [qos_flows_list]");
        goto end;
    }

    qos_flows_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(qos_flows_list_local_nonprimitive, qos_flows_list ) {
        if (!cJSON_IsObject(qos_flows_list_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [qos_flows_list]");
            goto end;
        }
        OpenAPI_qos_flow_setup_item_t *qos_flows_listItem = OpenAPI_qos_flow_setup_item_parseFromJSON(qos_flows_list_local_nonprimitive);

        OpenAPI_list_add(qos_flows_listList, qos_flows_listItem);
    }

    cJSON *h_smf_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hSmfInstanceId");

    if (h_smf_instance_id) {
        if (!cJSON_IsString(h_smf_instance_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [h_smf_instance_id]");
            goto end;
        }
    }

    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfInstanceId");

    if (smf_instance_id) {
        if (!cJSON_IsString(smf_instance_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_instance_id]");
            goto end;
        }
    }

    cJSON *enable_pause_charging = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "enablePauseCharging");

    if (enable_pause_charging) {
        if (!cJSON_IsBool(enable_pause_charging)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [enable_pause_charging]");
            goto end;
        }
    }

    cJSON *ue_ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "ueIpv4Address");

    if (ue_ipv4_address) {
        if (!cJSON_IsString(ue_ipv4_address)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [ue_ipv4_address]");
            goto end;
        }
    }

    cJSON *ue_ipv6_prefix = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "ueIpv6Prefix");

    if (ue_ipv6_prefix) {
        if (!cJSON_IsString(ue_ipv6_prefix)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [ue_ipv6_prefix]");
            goto end;
        }
    }

    cJSON *eps_pdn_cnx_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "epsPdnCnxInfo");

    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info_local_nonprim = NULL;
    if (eps_pdn_cnx_info) {
        eps_pdn_cnx_info_local_nonprim = OpenAPI_eps_pdn_cnx_info_parseFromJSON(eps_pdn_cnx_info);
    }

    cJSON *eps_bearer_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "epsBearerInfo");

    OpenAPI_list_t *eps_bearer_infoList;
    if (eps_bearer_info) {
        cJSON *eps_bearer_info_local_nonprimitive;
        if (!cJSON_IsArray(eps_bearer_info)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [eps_bearer_info]");
            goto end;
        }

        eps_bearer_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(eps_bearer_info_local_nonprimitive, eps_bearer_info ) {
            if (!cJSON_IsObject(eps_bearer_info_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [eps_bearer_info]");
                goto end;
            }
            OpenAPI_eps_bearer_info_t *eps_bearer_infoItem = OpenAPI_eps_bearer_info_parseFromJSON(eps_bearer_info_local_nonprimitive);

            OpenAPI_list_add(eps_bearer_infoList, eps_bearer_infoItem);
        }
    }

    cJSON *max_integrity_protected_data_rate = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "maxIntegrityProtectedDataRate");

    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rateVariable;
    if (max_integrity_protected_data_rate) {
        if (!cJSON_IsString(max_integrity_protected_data_rate)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [max_integrity_protected_data_rate]");
            goto end;
        }
        max_integrity_protected_data_rateVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate->valuestring);
    }

    cJSON *always_on_granted = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "alwaysOnGranted");

    if (always_on_granted) {
        if (!cJSON_IsBool(always_on_granted)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [always_on_granted]");
            goto end;
        }
    }

    cJSON *up_security = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "upSecurity");

    OpenAPI_up_security_t *up_security_local_nonprim = NULL;
    if (up_security) {
        up_security_local_nonprim = OpenAPI_up_security_parseFromJSON(up_security);
    }

    cJSON *h_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hSmfServiceInstanceId");

    if (h_smf_service_instance_id) {
        if (!cJSON_IsString(h_smf_service_instance_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [h_smf_service_instance_id]");
            goto end;
        }
    }

    cJSON *smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfServiceInstanceId");

    if (smf_service_instance_id) {
        if (!cJSON_IsString(smf_service_instance_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_service_instance_id]");
            goto end;
        }
    }

    cJSON *recovery_time = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "recoveryTime");

    if (recovery_time) {
        if (!cJSON_IsString(recovery_time)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [recovery_time]");
            goto end;
        }
    }

    cJSON *forwarding_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "forwardingInd");

    if (forwarding_ind) {
        if (!cJSON_IsBool(forwarding_ind)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [forwarding_ind]");
            goto end;
        }
    }

    cJSON *psa_tunnel_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "psaTunnelInfo");

    OpenAPI_tunnel_info_t *psa_tunnel_info_local_nonprim = NULL;
    if (psa_tunnel_info) {
        psa_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(psa_tunnel_info);
    }

    cJSON *home_provided_charging_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "homeProvidedChargingId");

    if (home_provided_charging_id) {
        if (!cJSON_IsString(home_provided_charging_id)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [home_provided_charging_id]");
            goto end;
        }
    }

    cJSON *charging_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "chargingInfo");

    OpenAPI_charging_information_t *charging_info_local_nonprim = NULL;
    if (charging_info) {
        charging_info_local_nonprim = OpenAPI_charging_information_parseFromJSON(charging_info);
    }

    cJSON *roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "roamingChargingProfile");

    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    if (roaming_charging_profile) {
        roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    }

    cJSON *nef_ext_buf_support_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "nefExtBufSupportInd");

    if (nef_ext_buf_support_ind) {
        if (!cJSON_IsBool(nef_ext_buf_support_ind)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [nef_ext_buf_support_ind]");
            goto end;
        }
    }

    sm_context_local_var = OpenAPI_sm_context_create (
        pdu_session_id->valuedouble,
        ogs_strdup(dnn->valuestring),
        s_nssai_local_nonprim,
        hplmn_snssai ? hplmn_snssai_local_nonprim : NULL,
        pdu_session_typeVariable,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        h_smf_uri ? ogs_strdup(h_smf_uri->valuestring) : NULL,
        smf_uri ? ogs_strdup(smf_uri->valuestring) : NULL,
        pdu_session_ref ? ogs_strdup(pdu_session_ref->valuestring) : NULL,
        pcf_id ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_group_id ? ogs_strdup(pcf_group_id->valuestring) : NULL,
        pcf_set_id ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        sel_mode ? sel_modeVariable : 0,
        udm_group_id ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator ? ogs_strdup(routing_indicator->valuestring) : NULL,
        session_ambr_local_nonprim,
        qos_flows_listList,
        h_smf_instance_id ? ogs_strdup(h_smf_instance_id->valuestring) : NULL,
        smf_instance_id ? ogs_strdup(smf_instance_id->valuestring) : NULL,
        enable_pause_charging ? enable_pause_charging->valueint : 0,
        ue_ipv4_address ? ogs_strdup(ue_ipv4_address->valuestring) : NULL,
        ue_ipv6_prefix ? ogs_strdup(ue_ipv6_prefix->valuestring) : NULL,
        eps_pdn_cnx_info ? eps_pdn_cnx_info_local_nonprim : NULL,
        eps_bearer_info ? eps_bearer_infoList : NULL,
        max_integrity_protected_data_rate ? max_integrity_protected_data_rateVariable : 0,
        always_on_granted ? always_on_granted->valueint : 0,
        up_security ? up_security_local_nonprim : NULL,
        h_smf_service_instance_id ? ogs_strdup(h_smf_service_instance_id->valuestring) : NULL,
        smf_service_instance_id ? ogs_strdup(smf_service_instance_id->valuestring) : NULL,
        recovery_time ? ogs_strdup(recovery_time->valuestring) : NULL,
        forwarding_ind ? forwarding_ind->valueint : 0,
        psa_tunnel_info ? psa_tunnel_info_local_nonprim : NULL,
        home_provided_charging_id ? ogs_strdup(home_provided_charging_id->valuestring) : NULL,
        charging_info ? charging_info_local_nonprim : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        nef_ext_buf_support_ind ? nef_ext_buf_support_ind->valueint : 0
        );

    return sm_context_local_var;
end:
    return NULL;
}

OpenAPI_sm_context_t *OpenAPI_sm_context_copy(OpenAPI_sm_context_t *dst, OpenAPI_sm_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed");
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

    OpenAPI_sm_context_free(dst);
    dst = OpenAPI_sm_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

