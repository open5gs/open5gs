
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_configuration_1.h"

OpenAPI_dnn_configuration_1_t *OpenAPI_dnn_configuration_1_create(
    OpenAPI_pdu_session_types_1_t *pdu_session_types,
    OpenAPI_ssc_modes_1_t *ssc_modes,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    OpenAPI_subscribed_default_qos_1_t *_5g_qos_profile,
    OpenAPI_ambr_1_t *session_ambr,
    char *_3gpp_charging_characteristics,
    OpenAPI_list_t *static_ip_address,
    OpenAPI_up_security_1_t *up_security,
    OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_ind,
    char *nidd_nef_id,
    OpenAPI_nidd_information_1_t *nidd_info,
    bool is_redundant_session_allowed,
    int redundant_session_allowed,
    OpenAPI_acs_info_1_t *acs_info,
    OpenAPI_list_t *ipv4_frame_route_list,
    OpenAPI_list_t *ipv6_frame_route_list,
    bool is_atsss_allowed,
    int atsss_allowed,
    bool is_secondary_auth,
    int secondary_auth,
    bool is_dn_aaa_ip_address_allocation,
    int dn_aaa_ip_address_allocation,
    OpenAPI_ip_address_1_t *dn_aaa_address,
    char *iptv_acc_ctrl_info
)
{
    OpenAPI_dnn_configuration_1_t *dnn_configuration_1_local_var = ogs_malloc(sizeof(OpenAPI_dnn_configuration_1_t));
    ogs_assert(dnn_configuration_1_local_var);

    dnn_configuration_1_local_var->pdu_session_types = pdu_session_types;
    dnn_configuration_1_local_var->ssc_modes = ssc_modes;
    dnn_configuration_1_local_var->is_iwk_eps_ind = is_iwk_eps_ind;
    dnn_configuration_1_local_var->iwk_eps_ind = iwk_eps_ind;
    dnn_configuration_1_local_var->_5g_qos_profile = _5g_qos_profile;
    dnn_configuration_1_local_var->session_ambr = session_ambr;
    dnn_configuration_1_local_var->_3gpp_charging_characteristics = _3gpp_charging_characteristics;
    dnn_configuration_1_local_var->static_ip_address = static_ip_address;
    dnn_configuration_1_local_var->up_security = up_security;
    dnn_configuration_1_local_var->pdu_session_continuity_ind = pdu_session_continuity_ind;
    dnn_configuration_1_local_var->nidd_nef_id = nidd_nef_id;
    dnn_configuration_1_local_var->nidd_info = nidd_info;
    dnn_configuration_1_local_var->is_redundant_session_allowed = is_redundant_session_allowed;
    dnn_configuration_1_local_var->redundant_session_allowed = redundant_session_allowed;
    dnn_configuration_1_local_var->acs_info = acs_info;
    dnn_configuration_1_local_var->ipv4_frame_route_list = ipv4_frame_route_list;
    dnn_configuration_1_local_var->ipv6_frame_route_list = ipv6_frame_route_list;
    dnn_configuration_1_local_var->is_atsss_allowed = is_atsss_allowed;
    dnn_configuration_1_local_var->atsss_allowed = atsss_allowed;
    dnn_configuration_1_local_var->is_secondary_auth = is_secondary_auth;
    dnn_configuration_1_local_var->secondary_auth = secondary_auth;
    dnn_configuration_1_local_var->is_dn_aaa_ip_address_allocation = is_dn_aaa_ip_address_allocation;
    dnn_configuration_1_local_var->dn_aaa_ip_address_allocation = dn_aaa_ip_address_allocation;
    dnn_configuration_1_local_var->dn_aaa_address = dn_aaa_address;
    dnn_configuration_1_local_var->iptv_acc_ctrl_info = iptv_acc_ctrl_info;

    return dnn_configuration_1_local_var;
}

void OpenAPI_dnn_configuration_1_free(OpenAPI_dnn_configuration_1_t *dnn_configuration_1)
{
    if (NULL == dnn_configuration_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_pdu_session_types_1_free(dnn_configuration_1->pdu_session_types);
    OpenAPI_ssc_modes_1_free(dnn_configuration_1->ssc_modes);
    OpenAPI_subscribed_default_qos_1_free(dnn_configuration_1->_5g_qos_profile);
    OpenAPI_ambr_1_free(dnn_configuration_1->session_ambr);
    ogs_free(dnn_configuration_1->_3gpp_charging_characteristics);
    OpenAPI_list_for_each(dnn_configuration_1->static_ip_address, node) {
        OpenAPI_ip_address_1_free(node->data);
    }
    OpenAPI_list_free(dnn_configuration_1->static_ip_address);
    OpenAPI_up_security_1_free(dnn_configuration_1->up_security);
    ogs_free(dnn_configuration_1->nidd_nef_id);
    OpenAPI_nidd_information_1_free(dnn_configuration_1->nidd_info);
    OpenAPI_acs_info_1_free(dnn_configuration_1->acs_info);
    OpenAPI_list_for_each(dnn_configuration_1->ipv4_frame_route_list, node) {
        OpenAPI_frame_route_info_1_free(node->data);
    }
    OpenAPI_list_free(dnn_configuration_1->ipv4_frame_route_list);
    OpenAPI_list_for_each(dnn_configuration_1->ipv6_frame_route_list, node) {
        OpenAPI_frame_route_info_1_free(node->data);
    }
    OpenAPI_list_free(dnn_configuration_1->ipv6_frame_route_list);
    OpenAPI_ip_address_1_free(dnn_configuration_1->dn_aaa_address);
    ogs_free(dnn_configuration_1->iptv_acc_ctrl_info);
    ogs_free(dnn_configuration_1);
}

cJSON *OpenAPI_dnn_configuration_1_convertToJSON(OpenAPI_dnn_configuration_1_t *dnn_configuration_1)
{
    cJSON *item = NULL;

    if (dnn_configuration_1 == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [DnnConfiguration_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *pdu_session_types_local_JSON = OpenAPI_pdu_session_types_1_convertToJSON(dnn_configuration_1->pdu_session_types);
    if (pdu_session_types_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSessionTypes", pdu_session_types_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [pdu_session_types]");
        goto end;
    }

    cJSON *ssc_modes_local_JSON = OpenAPI_ssc_modes_1_convertToJSON(dnn_configuration_1->ssc_modes);
    if (ssc_modes_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [ssc_modes]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sscModes", ssc_modes_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [ssc_modes]");
        goto end;
    }

    if (dnn_configuration_1->is_iwk_eps_ind) {
    if (cJSON_AddBoolToObject(item, "iwkEpsInd", dnn_configuration_1->iwk_eps_ind) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    if (dnn_configuration_1->_5g_qos_profile) {
    cJSON *_5g_qos_profile_local_JSON = OpenAPI_subscribed_default_qos_1_convertToJSON(dnn_configuration_1->_5g_qos_profile);
    if (_5g_qos_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [_5g_qos_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5gQosProfile", _5g_qos_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [_5g_qos_profile]");
        goto end;
    }
    }

    if (dnn_configuration_1->session_ambr) {
    cJSON *session_ambr_local_JSON = OpenAPI_ambr_1_convertToJSON(dnn_configuration_1->session_ambr);
    if (session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessionAmbr", session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [session_ambr]");
        goto end;
    }
    }

    if (dnn_configuration_1->_3gpp_charging_characteristics) {
    if (cJSON_AddStringToObject(item, "3gppChargingCharacteristics", dnn_configuration_1->_3gpp_charging_characteristics) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

    if (dnn_configuration_1->static_ip_address) {
    cJSON *static_ip_addressList = cJSON_AddArrayToObject(item, "staticIpAddress");
    if (static_ip_addressList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [static_ip_address]");
        goto end;
    }

    OpenAPI_lnode_t *static_ip_address_node;
    if (dnn_configuration_1->static_ip_address) {
        OpenAPI_list_for_each(dnn_configuration_1->static_ip_address, static_ip_address_node) {
            cJSON *itemLocal = OpenAPI_ip_address_1_convertToJSON(static_ip_address_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [static_ip_address]");
                goto end;
            }
            cJSON_AddItemToArray(static_ip_addressList, itemLocal);
        }
    }
    }

    if (dnn_configuration_1->up_security) {
    cJSON *up_security_local_JSON = OpenAPI_up_security_1_convertToJSON(dnn_configuration_1->up_security);
    if (up_security_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [up_security]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upSecurity", up_security_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [up_security]");
        goto end;
    }
    }

    if (dnn_configuration_1->pdu_session_continuity_ind) {
    if (cJSON_AddStringToObject(item, "pduSessionContinuityInd", OpenAPI_pdu_session_continuity_ind_ToString(dnn_configuration_1->pdu_session_continuity_ind)) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [pdu_session_continuity_ind]");
        goto end;
    }
    }

    if (dnn_configuration_1->nidd_nef_id) {
    if (cJSON_AddStringToObject(item, "niddNefId", dnn_configuration_1->nidd_nef_id) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [nidd_nef_id]");
        goto end;
    }
    }

    if (dnn_configuration_1->nidd_info) {
    cJSON *nidd_info_local_JSON = OpenAPI_nidd_information_1_convertToJSON(dnn_configuration_1->nidd_info);
    if (nidd_info_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [nidd_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "niddInfo", nidd_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [nidd_info]");
        goto end;
    }
    }

    if (dnn_configuration_1->is_redundant_session_allowed) {
    if (cJSON_AddBoolToObject(item, "redundantSessionAllowed", dnn_configuration_1->redundant_session_allowed) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [redundant_session_allowed]");
        goto end;
    }
    }

    if (dnn_configuration_1->acs_info) {
    cJSON *acs_info_local_JSON = OpenAPI_acs_info_1_convertToJSON(dnn_configuration_1->acs_info);
    if (acs_info_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [acs_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "acsInfo", acs_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [acs_info]");
        goto end;
    }
    }

    if (dnn_configuration_1->ipv4_frame_route_list) {
    cJSON *ipv4_frame_route_listList = cJSON_AddArrayToObject(item, "ipv4FrameRouteList");
    if (ipv4_frame_route_listList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [ipv4_frame_route_list]");
        goto end;
    }

    OpenAPI_lnode_t *ipv4_frame_route_list_node;
    if (dnn_configuration_1->ipv4_frame_route_list) {
        OpenAPI_list_for_each(dnn_configuration_1->ipv4_frame_route_list, ipv4_frame_route_list_node) {
            cJSON *itemLocal = OpenAPI_frame_route_info_1_convertToJSON(ipv4_frame_route_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [ipv4_frame_route_list]");
                goto end;
            }
            cJSON_AddItemToArray(ipv4_frame_route_listList, itemLocal);
        }
    }
    }

    if (dnn_configuration_1->ipv6_frame_route_list) {
    cJSON *ipv6_frame_route_listList = cJSON_AddArrayToObject(item, "ipv6FrameRouteList");
    if (ipv6_frame_route_listList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [ipv6_frame_route_list]");
        goto end;
    }

    OpenAPI_lnode_t *ipv6_frame_route_list_node;
    if (dnn_configuration_1->ipv6_frame_route_list) {
        OpenAPI_list_for_each(dnn_configuration_1->ipv6_frame_route_list, ipv6_frame_route_list_node) {
            cJSON *itemLocal = OpenAPI_frame_route_info_1_convertToJSON(ipv6_frame_route_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [ipv6_frame_route_list]");
                goto end;
            }
            cJSON_AddItemToArray(ipv6_frame_route_listList, itemLocal);
        }
    }
    }

    if (dnn_configuration_1->is_atsss_allowed) {
    if (cJSON_AddBoolToObject(item, "atsssAllowed", dnn_configuration_1->atsss_allowed) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [atsss_allowed]");
        goto end;
    }
    }

    if (dnn_configuration_1->is_secondary_auth) {
    if (cJSON_AddBoolToObject(item, "secondaryAuth", dnn_configuration_1->secondary_auth) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [secondary_auth]");
        goto end;
    }
    }

    if (dnn_configuration_1->is_dn_aaa_ip_address_allocation) {
    if (cJSON_AddBoolToObject(item, "dnAaaIpAddressAllocation", dnn_configuration_1->dn_aaa_ip_address_allocation) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    if (dnn_configuration_1->dn_aaa_address) {
    cJSON *dn_aaa_address_local_JSON = OpenAPI_ip_address_1_convertToJSON(dnn_configuration_1->dn_aaa_address);
    if (dn_aaa_address_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnAaaAddress", dn_aaa_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    }

    if (dnn_configuration_1->iptv_acc_ctrl_info) {
    if (cJSON_AddStringToObject(item, "iptvAccCtrlInfo", dnn_configuration_1->iptv_acc_ctrl_info) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed [iptv_acc_ctrl_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dnn_configuration_1_t *OpenAPI_dnn_configuration_1_parseFromJSON(cJSON *dnn_configuration_1JSON)
{
    OpenAPI_dnn_configuration_1_t *dnn_configuration_1_local_var = NULL;
    cJSON *pdu_session_types = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "pduSessionTypes");
    if (!pdu_session_types) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [pdu_session_types]");
        goto end;
    }

    OpenAPI_pdu_session_types_1_t *pdu_session_types_local_nonprim = NULL;
    pdu_session_types_local_nonprim = OpenAPI_pdu_session_types_1_parseFromJSON(pdu_session_types);

    cJSON *ssc_modes = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "sscModes");
    if (!ssc_modes) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [ssc_modes]");
        goto end;
    }

    OpenAPI_ssc_modes_1_t *ssc_modes_local_nonprim = NULL;
    ssc_modes_local_nonprim = OpenAPI_ssc_modes_1_parseFromJSON(ssc_modes);

    cJSON *iwk_eps_ind = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "iwkEpsInd");

    if (iwk_eps_ind) {
    if (!cJSON_IsBool(iwk_eps_ind)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    cJSON *_5g_qos_profile = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "5gQosProfile");

    OpenAPI_subscribed_default_qos_1_t *_5g_qos_profile_local_nonprim = NULL;
    if (_5g_qos_profile) {
    _5g_qos_profile_local_nonprim = OpenAPI_subscribed_default_qos_1_parseFromJSON(_5g_qos_profile);
    }

    cJSON *session_ambr = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "sessionAmbr");

    OpenAPI_ambr_1_t *session_ambr_local_nonprim = NULL;
    if (session_ambr) {
    session_ambr_local_nonprim = OpenAPI_ambr_1_parseFromJSON(session_ambr);
    }

    cJSON *_3gpp_charging_characteristics = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "3gppChargingCharacteristics");

    if (_3gpp_charging_characteristics) {
    if (!cJSON_IsString(_3gpp_charging_characteristics)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

    cJSON *static_ip_address = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "staticIpAddress");

    OpenAPI_list_t *static_ip_addressList;
    if (static_ip_address) {
    cJSON *static_ip_address_local_nonprimitive;
    if (!cJSON_IsArray(static_ip_address)){
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [static_ip_address]");
        goto end;
    }

    static_ip_addressList = OpenAPI_list_create();

    cJSON_ArrayForEach(static_ip_address_local_nonprimitive, static_ip_address ) {
        if (!cJSON_IsObject(static_ip_address_local_nonprimitive)) {
            ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [static_ip_address]");
            goto end;
        }
        OpenAPI_ip_address_1_t *static_ip_addressItem = OpenAPI_ip_address_1_parseFromJSON(static_ip_address_local_nonprimitive);

        if (!static_ip_addressItem) {
            ogs_error("No static_ip_addressItem");
            OpenAPI_list_free(static_ip_addressList);
            goto end;
        }

        OpenAPI_list_add(static_ip_addressList, static_ip_addressItem);
    }
    }

    cJSON *up_security = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "upSecurity");

    OpenAPI_up_security_1_t *up_security_local_nonprim = NULL;
    if (up_security) {
    up_security_local_nonprim = OpenAPI_up_security_1_parseFromJSON(up_security);
    }

    cJSON *pdu_session_continuity_ind = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "pduSessionContinuityInd");

    OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_indVariable;
    if (pdu_session_continuity_ind) {
    if (!cJSON_IsString(pdu_session_continuity_ind)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [pdu_session_continuity_ind]");
        goto end;
    }
    pdu_session_continuity_indVariable = OpenAPI_pdu_session_continuity_ind_FromString(pdu_session_continuity_ind->valuestring);
    }

    cJSON *nidd_nef_id = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "niddNefId");

    if (nidd_nef_id) {
    if (!cJSON_IsString(nidd_nef_id)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [nidd_nef_id]");
        goto end;
    }
    }

    cJSON *nidd_info = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "niddInfo");

    OpenAPI_nidd_information_1_t *nidd_info_local_nonprim = NULL;
    if (nidd_info) {
    nidd_info_local_nonprim = OpenAPI_nidd_information_1_parseFromJSON(nidd_info);
    }

    cJSON *redundant_session_allowed = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "redundantSessionAllowed");

    if (redundant_session_allowed) {
    if (!cJSON_IsBool(redundant_session_allowed)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [redundant_session_allowed]");
        goto end;
    }
    }

    cJSON *acs_info = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "acsInfo");

    OpenAPI_acs_info_1_t *acs_info_local_nonprim = NULL;
    if (acs_info) {
    acs_info_local_nonprim = OpenAPI_acs_info_1_parseFromJSON(acs_info);
    }

    cJSON *ipv4_frame_route_list = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "ipv4FrameRouteList");

    OpenAPI_list_t *ipv4_frame_route_listList;
    if (ipv4_frame_route_list) {
    cJSON *ipv4_frame_route_list_local_nonprimitive;
    if (!cJSON_IsArray(ipv4_frame_route_list)){
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [ipv4_frame_route_list]");
        goto end;
    }

    ipv4_frame_route_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv4_frame_route_list_local_nonprimitive, ipv4_frame_route_list ) {
        if (!cJSON_IsObject(ipv4_frame_route_list_local_nonprimitive)) {
            ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [ipv4_frame_route_list]");
            goto end;
        }
        OpenAPI_frame_route_info_1_t *ipv4_frame_route_listItem = OpenAPI_frame_route_info_1_parseFromJSON(ipv4_frame_route_list_local_nonprimitive);

        if (!ipv4_frame_route_listItem) {
            ogs_error("No ipv4_frame_route_listItem");
            OpenAPI_list_free(ipv4_frame_route_listList);
            goto end;
        }

        OpenAPI_list_add(ipv4_frame_route_listList, ipv4_frame_route_listItem);
    }
    }

    cJSON *ipv6_frame_route_list = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "ipv6FrameRouteList");

    OpenAPI_list_t *ipv6_frame_route_listList;
    if (ipv6_frame_route_list) {
    cJSON *ipv6_frame_route_list_local_nonprimitive;
    if (!cJSON_IsArray(ipv6_frame_route_list)){
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [ipv6_frame_route_list]");
        goto end;
    }

    ipv6_frame_route_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(ipv6_frame_route_list_local_nonprimitive, ipv6_frame_route_list ) {
        if (!cJSON_IsObject(ipv6_frame_route_list_local_nonprimitive)) {
            ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [ipv6_frame_route_list]");
            goto end;
        }
        OpenAPI_frame_route_info_1_t *ipv6_frame_route_listItem = OpenAPI_frame_route_info_1_parseFromJSON(ipv6_frame_route_list_local_nonprimitive);

        if (!ipv6_frame_route_listItem) {
            ogs_error("No ipv6_frame_route_listItem");
            OpenAPI_list_free(ipv6_frame_route_listList);
            goto end;
        }

        OpenAPI_list_add(ipv6_frame_route_listList, ipv6_frame_route_listItem);
    }
    }

    cJSON *atsss_allowed = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "atsssAllowed");

    if (atsss_allowed) {
    if (!cJSON_IsBool(atsss_allowed)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [atsss_allowed]");
        goto end;
    }
    }

    cJSON *secondary_auth = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "secondaryAuth");

    if (secondary_auth) {
    if (!cJSON_IsBool(secondary_auth)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [secondary_auth]");
        goto end;
    }
    }

    cJSON *dn_aaa_ip_address_allocation = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "dnAaaIpAddressAllocation");

    if (dn_aaa_ip_address_allocation) {
    if (!cJSON_IsBool(dn_aaa_ip_address_allocation)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    cJSON *dn_aaa_address = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "dnAaaAddress");

    OpenAPI_ip_address_1_t *dn_aaa_address_local_nonprim = NULL;
    if (dn_aaa_address) {
    dn_aaa_address_local_nonprim = OpenAPI_ip_address_1_parseFromJSON(dn_aaa_address);
    }

    cJSON *iptv_acc_ctrl_info = cJSON_GetObjectItemCaseSensitive(dnn_configuration_1JSON, "iptvAccCtrlInfo");

    if (iptv_acc_ctrl_info) {
    if (!cJSON_IsString(iptv_acc_ctrl_info)) {
        ogs_error("OpenAPI_dnn_configuration_1_parseFromJSON() failed [iptv_acc_ctrl_info]");
        goto end;
    }
    }

    dnn_configuration_1_local_var = OpenAPI_dnn_configuration_1_create (
        pdu_session_types_local_nonprim,
        ssc_modes_local_nonprim,
        iwk_eps_ind ? true : false,
        iwk_eps_ind ? iwk_eps_ind->valueint : 0,
        _5g_qos_profile ? _5g_qos_profile_local_nonprim : NULL,
        session_ambr ? session_ambr_local_nonprim : NULL,
        _3gpp_charging_characteristics ? ogs_strdup(_3gpp_charging_characteristics->valuestring) : NULL,
        static_ip_address ? static_ip_addressList : NULL,
        up_security ? up_security_local_nonprim : NULL,
        pdu_session_continuity_ind ? pdu_session_continuity_indVariable : 0,
        nidd_nef_id ? ogs_strdup(nidd_nef_id->valuestring) : NULL,
        nidd_info ? nidd_info_local_nonprim : NULL,
        redundant_session_allowed ? true : false,
        redundant_session_allowed ? redundant_session_allowed->valueint : 0,
        acs_info ? acs_info_local_nonprim : NULL,
        ipv4_frame_route_list ? ipv4_frame_route_listList : NULL,
        ipv6_frame_route_list ? ipv6_frame_route_listList : NULL,
        atsss_allowed ? true : false,
        atsss_allowed ? atsss_allowed->valueint : 0,
        secondary_auth ? true : false,
        secondary_auth ? secondary_auth->valueint : 0,
        dn_aaa_ip_address_allocation ? true : false,
        dn_aaa_ip_address_allocation ? dn_aaa_ip_address_allocation->valueint : 0,
        dn_aaa_address ? dn_aaa_address_local_nonprim : NULL,
        iptv_acc_ctrl_info ? ogs_strdup(iptv_acc_ctrl_info->valuestring) : NULL
    );

    return dnn_configuration_1_local_var;
end:
    return NULL;
}

OpenAPI_dnn_configuration_1_t *OpenAPI_dnn_configuration_1_copy(OpenAPI_dnn_configuration_1_t *dst, OpenAPI_dnn_configuration_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_configuration_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_configuration_1_convertToJSON() failed");
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

    OpenAPI_dnn_configuration_1_free(dst);
    dst = OpenAPI_dnn_configuration_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

