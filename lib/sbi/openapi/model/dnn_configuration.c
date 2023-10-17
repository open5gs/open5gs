
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_configuration.h"

OpenAPI_dnn_configuration_t *OpenAPI_dnn_configuration_create(
    OpenAPI_pdu_session_types_t *pdu_session_types,
    OpenAPI_ssc_modes_t *ssc_modes,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    OpenAPI_subscribed_default_qos_t *_5g_qos_profile,
    OpenAPI_ambr_t *session_ambr,
    char *_3gpp_charging_characteristics,
    OpenAPI_list_t *static_ip_address,
    OpenAPI_up_security_t *up_security,
    OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_ind,
    char *nidd_nef_id,
    OpenAPI_nidd_information_t *nidd_info,
    bool is_redundant_session_allowed,
    int redundant_session_allowed,
    OpenAPI_acs_info_t *acs_info,
    OpenAPI_list_t *ipv4_frame_route_list,
    OpenAPI_list_t *ipv6_frame_route_list,
    bool is_atsss_allowed,
    int atsss_allowed,
    bool is_secondary_auth,
    int secondary_auth,
    bool is_uav_secondary_auth,
    int uav_secondary_auth,
    bool is_dn_aaa_ip_address_allocation,
    int dn_aaa_ip_address_allocation,
    OpenAPI_ip_address_t *dn_aaa_address,
    OpenAPI_list_t *additional_dn_aaa_addresses,
    char *dn_aaa_fqdn,
    char *iptv_acc_ctrl_info,
    OpenAPI_ip_index_t *ipv4_index,
    OpenAPI_ip_index_t *ipv6_index,
    bool is_ecs_addr_config_info_null,
    OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info,
    OpenAPI_list_t *additional_ecs_addr_config_infos,
    char *shared_ecs_addr_config_info,
    OpenAPI_list_t *additional_shared_ecs_addr_config_info_ids,
    bool is_eas_discovery_authorized,
    int eas_discovery_authorized,
    bool is_onboarding_ind,
    int onboarding_ind,
    OpenAPI_aerial_ue_indication_e aerial_ue_ind,
    bool is_subscribed_max_ipv6_prefix_size,
    int subscribed_max_ipv6_prefix_size
)
{
    OpenAPI_dnn_configuration_t *dnn_configuration_local_var = ogs_malloc(sizeof(OpenAPI_dnn_configuration_t));
    ogs_assert(dnn_configuration_local_var);

    dnn_configuration_local_var->pdu_session_types = pdu_session_types;
    dnn_configuration_local_var->ssc_modes = ssc_modes;
    dnn_configuration_local_var->is_iwk_eps_ind = is_iwk_eps_ind;
    dnn_configuration_local_var->iwk_eps_ind = iwk_eps_ind;
    dnn_configuration_local_var->_5g_qos_profile = _5g_qos_profile;
    dnn_configuration_local_var->session_ambr = session_ambr;
    dnn_configuration_local_var->_3gpp_charging_characteristics = _3gpp_charging_characteristics;
    dnn_configuration_local_var->static_ip_address = static_ip_address;
    dnn_configuration_local_var->up_security = up_security;
    dnn_configuration_local_var->pdu_session_continuity_ind = pdu_session_continuity_ind;
    dnn_configuration_local_var->nidd_nef_id = nidd_nef_id;
    dnn_configuration_local_var->nidd_info = nidd_info;
    dnn_configuration_local_var->is_redundant_session_allowed = is_redundant_session_allowed;
    dnn_configuration_local_var->redundant_session_allowed = redundant_session_allowed;
    dnn_configuration_local_var->acs_info = acs_info;
    dnn_configuration_local_var->ipv4_frame_route_list = ipv4_frame_route_list;
    dnn_configuration_local_var->ipv6_frame_route_list = ipv6_frame_route_list;
    dnn_configuration_local_var->is_atsss_allowed = is_atsss_allowed;
    dnn_configuration_local_var->atsss_allowed = atsss_allowed;
    dnn_configuration_local_var->is_secondary_auth = is_secondary_auth;
    dnn_configuration_local_var->secondary_auth = secondary_auth;
    dnn_configuration_local_var->is_uav_secondary_auth = is_uav_secondary_auth;
    dnn_configuration_local_var->uav_secondary_auth = uav_secondary_auth;
    dnn_configuration_local_var->is_dn_aaa_ip_address_allocation = is_dn_aaa_ip_address_allocation;
    dnn_configuration_local_var->dn_aaa_ip_address_allocation = dn_aaa_ip_address_allocation;
    dnn_configuration_local_var->dn_aaa_address = dn_aaa_address;
    dnn_configuration_local_var->additional_dn_aaa_addresses = additional_dn_aaa_addresses;
    dnn_configuration_local_var->dn_aaa_fqdn = dn_aaa_fqdn;
    dnn_configuration_local_var->iptv_acc_ctrl_info = iptv_acc_ctrl_info;
    dnn_configuration_local_var->ipv4_index = ipv4_index;
    dnn_configuration_local_var->ipv6_index = ipv6_index;
    dnn_configuration_local_var->is_ecs_addr_config_info_null = is_ecs_addr_config_info_null;
    dnn_configuration_local_var->ecs_addr_config_info = ecs_addr_config_info;
    dnn_configuration_local_var->additional_ecs_addr_config_infos = additional_ecs_addr_config_infos;
    dnn_configuration_local_var->shared_ecs_addr_config_info = shared_ecs_addr_config_info;
    dnn_configuration_local_var->additional_shared_ecs_addr_config_info_ids = additional_shared_ecs_addr_config_info_ids;
    dnn_configuration_local_var->is_eas_discovery_authorized = is_eas_discovery_authorized;
    dnn_configuration_local_var->eas_discovery_authorized = eas_discovery_authorized;
    dnn_configuration_local_var->is_onboarding_ind = is_onboarding_ind;
    dnn_configuration_local_var->onboarding_ind = onboarding_ind;
    dnn_configuration_local_var->aerial_ue_ind = aerial_ue_ind;
    dnn_configuration_local_var->is_subscribed_max_ipv6_prefix_size = is_subscribed_max_ipv6_prefix_size;
    dnn_configuration_local_var->subscribed_max_ipv6_prefix_size = subscribed_max_ipv6_prefix_size;

    return dnn_configuration_local_var;
}

void OpenAPI_dnn_configuration_free(OpenAPI_dnn_configuration_t *dnn_configuration)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_configuration) {
        return;
    }
    if (dnn_configuration->pdu_session_types) {
        OpenAPI_pdu_session_types_free(dnn_configuration->pdu_session_types);
        dnn_configuration->pdu_session_types = NULL;
    }
    if (dnn_configuration->ssc_modes) {
        OpenAPI_ssc_modes_free(dnn_configuration->ssc_modes);
        dnn_configuration->ssc_modes = NULL;
    }
    if (dnn_configuration->_5g_qos_profile) {
        OpenAPI_subscribed_default_qos_free(dnn_configuration->_5g_qos_profile);
        dnn_configuration->_5g_qos_profile = NULL;
    }
    if (dnn_configuration->session_ambr) {
        OpenAPI_ambr_free(dnn_configuration->session_ambr);
        dnn_configuration->session_ambr = NULL;
    }
    if (dnn_configuration->_3gpp_charging_characteristics) {
        ogs_free(dnn_configuration->_3gpp_charging_characteristics);
        dnn_configuration->_3gpp_charging_characteristics = NULL;
    }
    if (dnn_configuration->static_ip_address) {
        OpenAPI_list_for_each(dnn_configuration->static_ip_address, node) {
            OpenAPI_ip_address_free(node->data);
        }
        OpenAPI_list_free(dnn_configuration->static_ip_address);
        dnn_configuration->static_ip_address = NULL;
    }
    if (dnn_configuration->up_security) {
        OpenAPI_up_security_free(dnn_configuration->up_security);
        dnn_configuration->up_security = NULL;
    }
    if (dnn_configuration->nidd_nef_id) {
        ogs_free(dnn_configuration->nidd_nef_id);
        dnn_configuration->nidd_nef_id = NULL;
    }
    if (dnn_configuration->nidd_info) {
        OpenAPI_nidd_information_free(dnn_configuration->nidd_info);
        dnn_configuration->nidd_info = NULL;
    }
    if (dnn_configuration->acs_info) {
        OpenAPI_acs_info_free(dnn_configuration->acs_info);
        dnn_configuration->acs_info = NULL;
    }
    if (dnn_configuration->ipv4_frame_route_list) {
        OpenAPI_list_for_each(dnn_configuration->ipv4_frame_route_list, node) {
            OpenAPI_frame_route_info_free(node->data);
        }
        OpenAPI_list_free(dnn_configuration->ipv4_frame_route_list);
        dnn_configuration->ipv4_frame_route_list = NULL;
    }
    if (dnn_configuration->ipv6_frame_route_list) {
        OpenAPI_list_for_each(dnn_configuration->ipv6_frame_route_list, node) {
            OpenAPI_frame_route_info_free(node->data);
        }
        OpenAPI_list_free(dnn_configuration->ipv6_frame_route_list);
        dnn_configuration->ipv6_frame_route_list = NULL;
    }
    if (dnn_configuration->dn_aaa_address) {
        OpenAPI_ip_address_free(dnn_configuration->dn_aaa_address);
        dnn_configuration->dn_aaa_address = NULL;
    }
    if (dnn_configuration->additional_dn_aaa_addresses) {
        OpenAPI_list_for_each(dnn_configuration->additional_dn_aaa_addresses, node) {
            OpenAPI_ip_address_free(node->data);
        }
        OpenAPI_list_free(dnn_configuration->additional_dn_aaa_addresses);
        dnn_configuration->additional_dn_aaa_addresses = NULL;
    }
    if (dnn_configuration->dn_aaa_fqdn) {
        ogs_free(dnn_configuration->dn_aaa_fqdn);
        dnn_configuration->dn_aaa_fqdn = NULL;
    }
    if (dnn_configuration->iptv_acc_ctrl_info) {
        ogs_free(dnn_configuration->iptv_acc_ctrl_info);
        dnn_configuration->iptv_acc_ctrl_info = NULL;
    }
    if (dnn_configuration->ipv4_index) {
        OpenAPI_ip_index_free(dnn_configuration->ipv4_index);
        dnn_configuration->ipv4_index = NULL;
    }
    if (dnn_configuration->ipv6_index) {
        OpenAPI_ip_index_free(dnn_configuration->ipv6_index);
        dnn_configuration->ipv6_index = NULL;
    }
    if (dnn_configuration->ecs_addr_config_info) {
        OpenAPI_ecs_addr_config_info_free(dnn_configuration->ecs_addr_config_info);
        dnn_configuration->ecs_addr_config_info = NULL;
    }
    if (dnn_configuration->additional_ecs_addr_config_infos) {
        OpenAPI_list_for_each(dnn_configuration->additional_ecs_addr_config_infos, node) {
            OpenAPI_ecs_addr_config_info_free(node->data);
        }
        OpenAPI_list_free(dnn_configuration->additional_ecs_addr_config_infos);
        dnn_configuration->additional_ecs_addr_config_infos = NULL;
    }
    if (dnn_configuration->shared_ecs_addr_config_info) {
        ogs_free(dnn_configuration->shared_ecs_addr_config_info);
        dnn_configuration->shared_ecs_addr_config_info = NULL;
    }
    if (dnn_configuration->additional_shared_ecs_addr_config_info_ids) {
        OpenAPI_list_for_each(dnn_configuration->additional_shared_ecs_addr_config_info_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_configuration->additional_shared_ecs_addr_config_info_ids);
        dnn_configuration->additional_shared_ecs_addr_config_info_ids = NULL;
    }
    ogs_free(dnn_configuration);
}

cJSON *OpenAPI_dnn_configuration_convertToJSON(OpenAPI_dnn_configuration_t *dnn_configuration)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_configuration == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [DnnConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_configuration->pdu_session_types) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [pdu_session_types]");
        return NULL;
    }
    cJSON *pdu_session_types_local_JSON = OpenAPI_pdu_session_types_convertToJSON(dnn_configuration->pdu_session_types);
    if (pdu_session_types_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSessionTypes", pdu_session_types_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [pdu_session_types]");
        goto end;
    }

    if (!dnn_configuration->ssc_modes) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ssc_modes]");
        return NULL;
    }
    cJSON *ssc_modes_local_JSON = OpenAPI_ssc_modes_convertToJSON(dnn_configuration->ssc_modes);
    if (ssc_modes_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ssc_modes]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sscModes", ssc_modes_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ssc_modes]");
        goto end;
    }

    if (dnn_configuration->is_iwk_eps_ind) {
    if (cJSON_AddBoolToObject(item, "iwkEpsInd", dnn_configuration->iwk_eps_ind) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    if (dnn_configuration->_5g_qos_profile) {
    cJSON *_5g_qos_profile_local_JSON = OpenAPI_subscribed_default_qos_convertToJSON(dnn_configuration->_5g_qos_profile);
    if (_5g_qos_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [_5g_qos_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5gQosProfile", _5g_qos_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [_5g_qos_profile]");
        goto end;
    }
    }

    if (dnn_configuration->session_ambr) {
    cJSON *session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(dnn_configuration->session_ambr);
    if (session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessionAmbr", session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [session_ambr]");
        goto end;
    }
    }

    if (dnn_configuration->_3gpp_charging_characteristics) {
    if (cJSON_AddStringToObject(item, "3gppChargingCharacteristics", dnn_configuration->_3gpp_charging_characteristics) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

    if (dnn_configuration->static_ip_address) {
    cJSON *static_ip_addressList = cJSON_AddArrayToObject(item, "staticIpAddress");
    if (static_ip_addressList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [static_ip_address]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_configuration->static_ip_address, node) {
        cJSON *itemLocal = OpenAPI_ip_address_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [static_ip_address]");
            goto end;
        }
        cJSON_AddItemToArray(static_ip_addressList, itemLocal);
    }
    }

    if (dnn_configuration->up_security) {
    cJSON *up_security_local_JSON = OpenAPI_up_security_convertToJSON(dnn_configuration->up_security);
    if (up_security_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [up_security]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upSecurity", up_security_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [up_security]");
        goto end;
    }
    }

    if (dnn_configuration->pdu_session_continuity_ind != OpenAPI_pdu_session_continuity_ind_NULL) {
    if (cJSON_AddStringToObject(item, "pduSessionContinuityInd", OpenAPI_pdu_session_continuity_ind_ToString(dnn_configuration->pdu_session_continuity_ind)) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [pdu_session_continuity_ind]");
        goto end;
    }
    }

    if (dnn_configuration->nidd_nef_id) {
    if (cJSON_AddStringToObject(item, "niddNefId", dnn_configuration->nidd_nef_id) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [nidd_nef_id]");
        goto end;
    }
    }

    if (dnn_configuration->nidd_info) {
    cJSON *nidd_info_local_JSON = OpenAPI_nidd_information_convertToJSON(dnn_configuration->nidd_info);
    if (nidd_info_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [nidd_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "niddInfo", nidd_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [nidd_info]");
        goto end;
    }
    }

    if (dnn_configuration->is_redundant_session_allowed) {
    if (cJSON_AddBoolToObject(item, "redundantSessionAllowed", dnn_configuration->redundant_session_allowed) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [redundant_session_allowed]");
        goto end;
    }
    }

    if (dnn_configuration->acs_info) {
    cJSON *acs_info_local_JSON = OpenAPI_acs_info_convertToJSON(dnn_configuration->acs_info);
    if (acs_info_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [acs_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "acsInfo", acs_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [acs_info]");
        goto end;
    }
    }

    if (dnn_configuration->ipv4_frame_route_list) {
    cJSON *ipv4_frame_route_listList = cJSON_AddArrayToObject(item, "ipv4FrameRouteList");
    if (ipv4_frame_route_listList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv4_frame_route_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_configuration->ipv4_frame_route_list, node) {
        cJSON *itemLocal = OpenAPI_frame_route_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv4_frame_route_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_frame_route_listList, itemLocal);
    }
    }

    if (dnn_configuration->ipv6_frame_route_list) {
    cJSON *ipv6_frame_route_listList = cJSON_AddArrayToObject(item, "ipv6FrameRouteList");
    if (ipv6_frame_route_listList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv6_frame_route_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_configuration->ipv6_frame_route_list, node) {
        cJSON *itemLocal = OpenAPI_frame_route_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv6_frame_route_list]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_frame_route_listList, itemLocal);
    }
    }

    if (dnn_configuration->is_atsss_allowed) {
    if (cJSON_AddBoolToObject(item, "atsssAllowed", dnn_configuration->atsss_allowed) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [atsss_allowed]");
        goto end;
    }
    }

    if (dnn_configuration->is_secondary_auth) {
    if (cJSON_AddBoolToObject(item, "secondaryAuth", dnn_configuration->secondary_auth) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [secondary_auth]");
        goto end;
    }
    }

    if (dnn_configuration->is_uav_secondary_auth) {
    if (cJSON_AddBoolToObject(item, "uavSecondaryAuth", dnn_configuration->uav_secondary_auth) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [uav_secondary_auth]");
        goto end;
    }
    }

    if (dnn_configuration->is_dn_aaa_ip_address_allocation) {
    if (cJSON_AddBoolToObject(item, "dnAaaIpAddressAllocation", dnn_configuration->dn_aaa_ip_address_allocation) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    if (dnn_configuration->dn_aaa_address) {
    cJSON *dn_aaa_address_local_JSON = OpenAPI_ip_address_convertToJSON(dnn_configuration->dn_aaa_address);
    if (dn_aaa_address_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnAaaAddress", dn_aaa_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    }

    if (dnn_configuration->additional_dn_aaa_addresses) {
    cJSON *additional_dn_aaa_addressesList = cJSON_AddArrayToObject(item, "additionalDnAaaAddresses");
    if (additional_dn_aaa_addressesList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [additional_dn_aaa_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_configuration->additional_dn_aaa_addresses, node) {
        cJSON *itemLocal = OpenAPI_ip_address_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [additional_dn_aaa_addresses]");
            goto end;
        }
        cJSON_AddItemToArray(additional_dn_aaa_addressesList, itemLocal);
    }
    }

    if (dnn_configuration->dn_aaa_fqdn) {
    if (cJSON_AddStringToObject(item, "dnAaaFqdn", dnn_configuration->dn_aaa_fqdn) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [dn_aaa_fqdn]");
        goto end;
    }
    }

    if (dnn_configuration->iptv_acc_ctrl_info) {
    if (cJSON_AddStringToObject(item, "iptvAccCtrlInfo", dnn_configuration->iptv_acc_ctrl_info) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [iptv_acc_ctrl_info]");
        goto end;
    }
    }

    if (dnn_configuration->ipv4_index) {
    cJSON *ipv4_index_local_JSON = OpenAPI_ip_index_convertToJSON(dnn_configuration->ipv4_index);
    if (ipv4_index_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv4_index]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipv4Index", ipv4_index_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv4_index]");
        goto end;
    }
    }

    if (dnn_configuration->ipv6_index) {
    cJSON *ipv6_index_local_JSON = OpenAPI_ip_index_convertToJSON(dnn_configuration->ipv6_index);
    if (ipv6_index_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv6_index]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipv6Index", ipv6_index_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ipv6_index]");
        goto end;
    }
    }

    if (dnn_configuration->ecs_addr_config_info) {
    cJSON *ecs_addr_config_info_local_JSON = OpenAPI_ecs_addr_config_info_convertToJSON(dnn_configuration->ecs_addr_config_info);
    if (ecs_addr_config_info_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ecs_addr_config_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecsAddrConfigInfo", ecs_addr_config_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ecs_addr_config_info]");
        goto end;
    }
    } else if (dnn_configuration->is_ecs_addr_config_info_null) {
        if (cJSON_AddNullToObject(item, "ecsAddrConfigInfo") == NULL) {
            ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [ecs_addr_config_info]");
            goto end;
        }
    }

    if (dnn_configuration->additional_ecs_addr_config_infos) {
    cJSON *additional_ecs_addr_config_infosList = cJSON_AddArrayToObject(item, "additionalEcsAddrConfigInfos");
    if (additional_ecs_addr_config_infosList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [additional_ecs_addr_config_infos]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_configuration->additional_ecs_addr_config_infos, node) {
        cJSON *itemLocal = OpenAPI_ecs_addr_config_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [additional_ecs_addr_config_infos]");
            goto end;
        }
        cJSON_AddItemToArray(additional_ecs_addr_config_infosList, itemLocal);
    }
    }

    if (dnn_configuration->shared_ecs_addr_config_info) {
    if (cJSON_AddStringToObject(item, "sharedEcsAddrConfigInfo", dnn_configuration->shared_ecs_addr_config_info) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [shared_ecs_addr_config_info]");
        goto end;
    }
    }

    if (dnn_configuration->additional_shared_ecs_addr_config_info_ids) {
    cJSON *additional_shared_ecs_addr_config_info_idsList = cJSON_AddArrayToObject(item, "additionalSharedEcsAddrConfigInfoIds");
    if (additional_shared_ecs_addr_config_info_idsList == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [additional_shared_ecs_addr_config_info_ids]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_configuration->additional_shared_ecs_addr_config_info_ids, node) {
        if (cJSON_AddStringToObject(additional_shared_ecs_addr_config_info_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [additional_shared_ecs_addr_config_info_ids]");
            goto end;
        }
    }
    }

    if (dnn_configuration->is_eas_discovery_authorized) {
    if (cJSON_AddBoolToObject(item, "easDiscoveryAuthorized", dnn_configuration->eas_discovery_authorized) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [eas_discovery_authorized]");
        goto end;
    }
    }

    if (dnn_configuration->is_onboarding_ind) {
    if (cJSON_AddBoolToObject(item, "onboardingInd", dnn_configuration->onboarding_ind) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [onboarding_ind]");
        goto end;
    }
    }

    if (dnn_configuration->aerial_ue_ind != OpenAPI_aerial_ue_indication_NULL) {
    if (cJSON_AddStringToObject(item, "aerialUeInd", OpenAPI_aerial_ue_indication_ToString(dnn_configuration->aerial_ue_ind)) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [aerial_ue_ind]");
        goto end;
    }
    }

    if (dnn_configuration->is_subscribed_max_ipv6_prefix_size) {
    if (cJSON_AddNumberToObject(item, "subscribedMaxIpv6PrefixSize", dnn_configuration->subscribed_max_ipv6_prefix_size) == NULL) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed [subscribed_max_ipv6_prefix_size]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dnn_configuration_t *OpenAPI_dnn_configuration_parseFromJSON(cJSON *dnn_configurationJSON)
{
    OpenAPI_dnn_configuration_t *dnn_configuration_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_types = NULL;
    OpenAPI_pdu_session_types_t *pdu_session_types_local_nonprim = NULL;
    cJSON *ssc_modes = NULL;
    OpenAPI_ssc_modes_t *ssc_modes_local_nonprim = NULL;
    cJSON *iwk_eps_ind = NULL;
    cJSON *_5g_qos_profile = NULL;
    OpenAPI_subscribed_default_qos_t *_5g_qos_profile_local_nonprim = NULL;
    cJSON *session_ambr = NULL;
    OpenAPI_ambr_t *session_ambr_local_nonprim = NULL;
    cJSON *_3gpp_charging_characteristics = NULL;
    cJSON *static_ip_address = NULL;
    OpenAPI_list_t *static_ip_addressList = NULL;
    cJSON *up_security = NULL;
    OpenAPI_up_security_t *up_security_local_nonprim = NULL;
    cJSON *pdu_session_continuity_ind = NULL;
    OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_indVariable = 0;
    cJSON *nidd_nef_id = NULL;
    cJSON *nidd_info = NULL;
    OpenAPI_nidd_information_t *nidd_info_local_nonprim = NULL;
    cJSON *redundant_session_allowed = NULL;
    cJSON *acs_info = NULL;
    OpenAPI_acs_info_t *acs_info_local_nonprim = NULL;
    cJSON *ipv4_frame_route_list = NULL;
    OpenAPI_list_t *ipv4_frame_route_listList = NULL;
    cJSON *ipv6_frame_route_list = NULL;
    OpenAPI_list_t *ipv6_frame_route_listList = NULL;
    cJSON *atsss_allowed = NULL;
    cJSON *secondary_auth = NULL;
    cJSON *uav_secondary_auth = NULL;
    cJSON *dn_aaa_ip_address_allocation = NULL;
    cJSON *dn_aaa_address = NULL;
    OpenAPI_ip_address_t *dn_aaa_address_local_nonprim = NULL;
    cJSON *additional_dn_aaa_addresses = NULL;
    OpenAPI_list_t *additional_dn_aaa_addressesList = NULL;
    cJSON *dn_aaa_fqdn = NULL;
    cJSON *iptv_acc_ctrl_info = NULL;
    cJSON *ipv4_index = NULL;
    OpenAPI_ip_index_t *ipv4_index_local_nonprim = NULL;
    cJSON *ipv6_index = NULL;
    OpenAPI_ip_index_t *ipv6_index_local_nonprim = NULL;
    cJSON *ecs_addr_config_info = NULL;
    OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info_local_nonprim = NULL;
    cJSON *additional_ecs_addr_config_infos = NULL;
    OpenAPI_list_t *additional_ecs_addr_config_infosList = NULL;
    cJSON *shared_ecs_addr_config_info = NULL;
    cJSON *additional_shared_ecs_addr_config_info_ids = NULL;
    OpenAPI_list_t *additional_shared_ecs_addr_config_info_idsList = NULL;
    cJSON *eas_discovery_authorized = NULL;
    cJSON *onboarding_ind = NULL;
    cJSON *aerial_ue_ind = NULL;
    OpenAPI_aerial_ue_indication_e aerial_ue_indVariable = 0;
    cJSON *subscribed_max_ipv6_prefix_size = NULL;
    pdu_session_types = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "pduSessionTypes");
    if (!pdu_session_types) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [pdu_session_types]");
        goto end;
    }
    pdu_session_types_local_nonprim = OpenAPI_pdu_session_types_parseFromJSON(pdu_session_types);
    if (!pdu_session_types_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_types_parseFromJSON failed [pdu_session_types]");
        goto end;
    }

    ssc_modes = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "sscModes");
    if (!ssc_modes) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [ssc_modes]");
        goto end;
    }
    ssc_modes_local_nonprim = OpenAPI_ssc_modes_parseFromJSON(ssc_modes);
    if (!ssc_modes_local_nonprim) {
        ogs_error("OpenAPI_ssc_modes_parseFromJSON failed [ssc_modes]");
        goto end;
    }

    iwk_eps_ind = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "iwkEpsInd");
    if (iwk_eps_ind) {
    if (!cJSON_IsBool(iwk_eps_ind)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    _5g_qos_profile = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "5gQosProfile");
    if (_5g_qos_profile) {
    _5g_qos_profile_local_nonprim = OpenAPI_subscribed_default_qos_parseFromJSON(_5g_qos_profile);
    if (!_5g_qos_profile_local_nonprim) {
        ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON failed [_5g_qos_profile]");
        goto end;
    }
    }

    session_ambr = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "sessionAmbr");
    if (session_ambr) {
    session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(session_ambr);
    if (!session_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [session_ambr]");
        goto end;
    }
    }

    _3gpp_charging_characteristics = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "3gppChargingCharacteristics");
    if (_3gpp_charging_characteristics) {
    if (!cJSON_IsString(_3gpp_charging_characteristics) && !cJSON_IsNull(_3gpp_charging_characteristics)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

    static_ip_address = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "staticIpAddress");
    if (static_ip_address) {
        cJSON *static_ip_address_local = NULL;
        if (!cJSON_IsArray(static_ip_address)) {
            ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [static_ip_address]");
            goto end;
        }

        static_ip_addressList = OpenAPI_list_create();

        cJSON_ArrayForEach(static_ip_address_local, static_ip_address) {
            if (!cJSON_IsObject(static_ip_address_local)) {
                ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [static_ip_address]");
                goto end;
            }
            OpenAPI_ip_address_t *static_ip_addressItem = OpenAPI_ip_address_parseFromJSON(static_ip_address_local);
            if (!static_ip_addressItem) {
                ogs_error("No static_ip_addressItem");
                goto end;
            }
            OpenAPI_list_add(static_ip_addressList, static_ip_addressItem);
        }
    }

    up_security = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "upSecurity");
    if (up_security) {
    up_security_local_nonprim = OpenAPI_up_security_parseFromJSON(up_security);
    if (!up_security_local_nonprim) {
        ogs_error("OpenAPI_up_security_parseFromJSON failed [up_security]");
        goto end;
    }
    }

    pdu_session_continuity_ind = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "pduSessionContinuityInd");
    if (pdu_session_continuity_ind) {
    if (!cJSON_IsString(pdu_session_continuity_ind)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [pdu_session_continuity_ind]");
        goto end;
    }
    pdu_session_continuity_indVariable = OpenAPI_pdu_session_continuity_ind_FromString(pdu_session_continuity_ind->valuestring);
    }

    nidd_nef_id = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "niddNefId");
    if (nidd_nef_id) {
    if (!cJSON_IsString(nidd_nef_id) && !cJSON_IsNull(nidd_nef_id)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [nidd_nef_id]");
        goto end;
    }
    }

    nidd_info = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "niddInfo");
    if (nidd_info) {
    nidd_info_local_nonprim = OpenAPI_nidd_information_parseFromJSON(nidd_info);
    if (!nidd_info_local_nonprim) {
        ogs_error("OpenAPI_nidd_information_parseFromJSON failed [nidd_info]");
        goto end;
    }
    }

    redundant_session_allowed = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "redundantSessionAllowed");
    if (redundant_session_allowed) {
    if (!cJSON_IsBool(redundant_session_allowed)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [redundant_session_allowed]");
        goto end;
    }
    }

    acs_info = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "acsInfo");
    if (acs_info) {
    acs_info_local_nonprim = OpenAPI_acs_info_parseFromJSON(acs_info);
    if (!acs_info_local_nonprim) {
        ogs_error("OpenAPI_acs_info_parseFromJSON failed [acs_info]");
        goto end;
    }
    }

    ipv4_frame_route_list = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "ipv4FrameRouteList");
    if (ipv4_frame_route_list) {
        cJSON *ipv4_frame_route_list_local = NULL;
        if (!cJSON_IsArray(ipv4_frame_route_list)) {
            ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [ipv4_frame_route_list]");
            goto end;
        }

        ipv4_frame_route_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_frame_route_list_local, ipv4_frame_route_list) {
            if (!cJSON_IsObject(ipv4_frame_route_list_local)) {
                ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [ipv4_frame_route_list]");
                goto end;
            }
            OpenAPI_frame_route_info_t *ipv4_frame_route_listItem = OpenAPI_frame_route_info_parseFromJSON(ipv4_frame_route_list_local);
            if (!ipv4_frame_route_listItem) {
                ogs_error("No ipv4_frame_route_listItem");
                goto end;
            }
            OpenAPI_list_add(ipv4_frame_route_listList, ipv4_frame_route_listItem);
        }
    }

    ipv6_frame_route_list = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "ipv6FrameRouteList");
    if (ipv6_frame_route_list) {
        cJSON *ipv6_frame_route_list_local = NULL;
        if (!cJSON_IsArray(ipv6_frame_route_list)) {
            ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [ipv6_frame_route_list]");
            goto end;
        }

        ipv6_frame_route_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_frame_route_list_local, ipv6_frame_route_list) {
            if (!cJSON_IsObject(ipv6_frame_route_list_local)) {
                ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [ipv6_frame_route_list]");
                goto end;
            }
            OpenAPI_frame_route_info_t *ipv6_frame_route_listItem = OpenAPI_frame_route_info_parseFromJSON(ipv6_frame_route_list_local);
            if (!ipv6_frame_route_listItem) {
                ogs_error("No ipv6_frame_route_listItem");
                goto end;
            }
            OpenAPI_list_add(ipv6_frame_route_listList, ipv6_frame_route_listItem);
        }
    }

    atsss_allowed = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "atsssAllowed");
    if (atsss_allowed) {
    if (!cJSON_IsBool(atsss_allowed)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [atsss_allowed]");
        goto end;
    }
    }

    secondary_auth = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "secondaryAuth");
    if (secondary_auth) {
    if (!cJSON_IsBool(secondary_auth)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [secondary_auth]");
        goto end;
    }
    }

    uav_secondary_auth = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "uavSecondaryAuth");
    if (uav_secondary_auth) {
    if (!cJSON_IsBool(uav_secondary_auth)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [uav_secondary_auth]");
        goto end;
    }
    }

    dn_aaa_ip_address_allocation = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "dnAaaIpAddressAllocation");
    if (dn_aaa_ip_address_allocation) {
    if (!cJSON_IsBool(dn_aaa_ip_address_allocation)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    dn_aaa_address = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "dnAaaAddress");
    if (dn_aaa_address) {
    dn_aaa_address_local_nonprim = OpenAPI_ip_address_parseFromJSON(dn_aaa_address);
    if (!dn_aaa_address_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [dn_aaa_address]");
        goto end;
    }
    }

    additional_dn_aaa_addresses = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "additionalDnAaaAddresses");
    if (additional_dn_aaa_addresses) {
        cJSON *additional_dn_aaa_addresses_local = NULL;
        if (!cJSON_IsArray(additional_dn_aaa_addresses)) {
            ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [additional_dn_aaa_addresses]");
            goto end;
        }

        additional_dn_aaa_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_dn_aaa_addresses_local, additional_dn_aaa_addresses) {
            if (!cJSON_IsObject(additional_dn_aaa_addresses_local)) {
                ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [additional_dn_aaa_addresses]");
                goto end;
            }
            OpenAPI_ip_address_t *additional_dn_aaa_addressesItem = OpenAPI_ip_address_parseFromJSON(additional_dn_aaa_addresses_local);
            if (!additional_dn_aaa_addressesItem) {
                ogs_error("No additional_dn_aaa_addressesItem");
                goto end;
            }
            OpenAPI_list_add(additional_dn_aaa_addressesList, additional_dn_aaa_addressesItem);
        }
    }

    dn_aaa_fqdn = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "dnAaaFqdn");
    if (dn_aaa_fqdn) {
    if (!cJSON_IsString(dn_aaa_fqdn) && !cJSON_IsNull(dn_aaa_fqdn)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [dn_aaa_fqdn]");
        goto end;
    }
    }

    iptv_acc_ctrl_info = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "iptvAccCtrlInfo");
    if (iptv_acc_ctrl_info) {
    if (!cJSON_IsString(iptv_acc_ctrl_info) && !cJSON_IsNull(iptv_acc_ctrl_info)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [iptv_acc_ctrl_info]");
        goto end;
    }
    }

    ipv4_index = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "ipv4Index");
    if (ipv4_index) {
    ipv4_index_local_nonprim = OpenAPI_ip_index_parseFromJSON(ipv4_index);
    if (!ipv4_index_local_nonprim) {
        ogs_error("OpenAPI_ip_index_parseFromJSON failed [ipv4_index]");
        goto end;
    }
    }

    ipv6_index = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "ipv6Index");
    if (ipv6_index) {
    ipv6_index_local_nonprim = OpenAPI_ip_index_parseFromJSON(ipv6_index);
    if (!ipv6_index_local_nonprim) {
        ogs_error("OpenAPI_ip_index_parseFromJSON failed [ipv6_index]");
        goto end;
    }
    }

    ecs_addr_config_info = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "ecsAddrConfigInfo");
    if (ecs_addr_config_info) {
    if (!cJSON_IsNull(ecs_addr_config_info)) {
    ecs_addr_config_info_local_nonprim = OpenAPI_ecs_addr_config_info_parseFromJSON(ecs_addr_config_info);
    if (!ecs_addr_config_info_local_nonprim) {
        ogs_error("OpenAPI_ecs_addr_config_info_parseFromJSON failed [ecs_addr_config_info]");
        goto end;
    }
    }
    }

    additional_ecs_addr_config_infos = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "additionalEcsAddrConfigInfos");
    if (additional_ecs_addr_config_infos) {
        cJSON *additional_ecs_addr_config_infos_local = NULL;
        if (!cJSON_IsArray(additional_ecs_addr_config_infos)) {
            ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [additional_ecs_addr_config_infos]");
            goto end;
        }

        additional_ecs_addr_config_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_ecs_addr_config_infos_local, additional_ecs_addr_config_infos) {
            if (!cJSON_IsObject(additional_ecs_addr_config_infos_local)) {
                ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [additional_ecs_addr_config_infos]");
                goto end;
            }
            OpenAPI_ecs_addr_config_info_t *additional_ecs_addr_config_infosItem = OpenAPI_ecs_addr_config_info_parseFromJSON(additional_ecs_addr_config_infos_local);
            if (!additional_ecs_addr_config_infosItem) {
                ogs_error("No additional_ecs_addr_config_infosItem");
                goto end;
            }
            OpenAPI_list_add(additional_ecs_addr_config_infosList, additional_ecs_addr_config_infosItem);
        }
    }

    shared_ecs_addr_config_info = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "sharedEcsAddrConfigInfo");
    if (shared_ecs_addr_config_info) {
    if (!cJSON_IsString(shared_ecs_addr_config_info) && !cJSON_IsNull(shared_ecs_addr_config_info)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [shared_ecs_addr_config_info]");
        goto end;
    }
    }

    additional_shared_ecs_addr_config_info_ids = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "additionalSharedEcsAddrConfigInfoIds");
    if (additional_shared_ecs_addr_config_info_ids) {
        cJSON *additional_shared_ecs_addr_config_info_ids_local = NULL;
        if (!cJSON_IsArray(additional_shared_ecs_addr_config_info_ids)) {
            ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [additional_shared_ecs_addr_config_info_ids]");
            goto end;
        }

        additional_shared_ecs_addr_config_info_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_shared_ecs_addr_config_info_ids_local, additional_shared_ecs_addr_config_info_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_shared_ecs_addr_config_info_ids_local)) {
                ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [additional_shared_ecs_addr_config_info_ids]");
                goto end;
            }
            OpenAPI_list_add(additional_shared_ecs_addr_config_info_idsList, ogs_strdup(additional_shared_ecs_addr_config_info_ids_local->valuestring));
        }
    }

    eas_discovery_authorized = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "easDiscoveryAuthorized");
    if (eas_discovery_authorized) {
    if (!cJSON_IsBool(eas_discovery_authorized)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [eas_discovery_authorized]");
        goto end;
    }
    }

    onboarding_ind = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "onboardingInd");
    if (onboarding_ind) {
    if (!cJSON_IsBool(onboarding_ind)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [onboarding_ind]");
        goto end;
    }
    }

    aerial_ue_ind = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "aerialUeInd");
    if (aerial_ue_ind) {
    if (!cJSON_IsString(aerial_ue_ind)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [aerial_ue_ind]");
        goto end;
    }
    aerial_ue_indVariable = OpenAPI_aerial_ue_indication_FromString(aerial_ue_ind->valuestring);
    }

    subscribed_max_ipv6_prefix_size = cJSON_GetObjectItemCaseSensitive(dnn_configurationJSON, "subscribedMaxIpv6PrefixSize");
    if (subscribed_max_ipv6_prefix_size) {
    if (!cJSON_IsNumber(subscribed_max_ipv6_prefix_size)) {
        ogs_error("OpenAPI_dnn_configuration_parseFromJSON() failed [subscribed_max_ipv6_prefix_size]");
        goto end;
    }
    }

    dnn_configuration_local_var = OpenAPI_dnn_configuration_create (
        pdu_session_types_local_nonprim,
        ssc_modes_local_nonprim,
        iwk_eps_ind ? true : false,
        iwk_eps_ind ? iwk_eps_ind->valueint : 0,
        _5g_qos_profile ? _5g_qos_profile_local_nonprim : NULL,
        session_ambr ? session_ambr_local_nonprim : NULL,
        _3gpp_charging_characteristics && !cJSON_IsNull(_3gpp_charging_characteristics) ? ogs_strdup(_3gpp_charging_characteristics->valuestring) : NULL,
        static_ip_address ? static_ip_addressList : NULL,
        up_security ? up_security_local_nonprim : NULL,
        pdu_session_continuity_ind ? pdu_session_continuity_indVariable : 0,
        nidd_nef_id && !cJSON_IsNull(nidd_nef_id) ? ogs_strdup(nidd_nef_id->valuestring) : NULL,
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
        uav_secondary_auth ? true : false,
        uav_secondary_auth ? uav_secondary_auth->valueint : 0,
        dn_aaa_ip_address_allocation ? true : false,
        dn_aaa_ip_address_allocation ? dn_aaa_ip_address_allocation->valueint : 0,
        dn_aaa_address ? dn_aaa_address_local_nonprim : NULL,
        additional_dn_aaa_addresses ? additional_dn_aaa_addressesList : NULL,
        dn_aaa_fqdn && !cJSON_IsNull(dn_aaa_fqdn) ? ogs_strdup(dn_aaa_fqdn->valuestring) : NULL,
        iptv_acc_ctrl_info && !cJSON_IsNull(iptv_acc_ctrl_info) ? ogs_strdup(iptv_acc_ctrl_info->valuestring) : NULL,
        ipv4_index ? ipv4_index_local_nonprim : NULL,
        ipv6_index ? ipv6_index_local_nonprim : NULL,
        ecs_addr_config_info && cJSON_IsNull(ecs_addr_config_info) ? true : false,
        ecs_addr_config_info ? ecs_addr_config_info_local_nonprim : NULL,
        additional_ecs_addr_config_infos ? additional_ecs_addr_config_infosList : NULL,
        shared_ecs_addr_config_info && !cJSON_IsNull(shared_ecs_addr_config_info) ? ogs_strdup(shared_ecs_addr_config_info->valuestring) : NULL,
        additional_shared_ecs_addr_config_info_ids ? additional_shared_ecs_addr_config_info_idsList : NULL,
        eas_discovery_authorized ? true : false,
        eas_discovery_authorized ? eas_discovery_authorized->valueint : 0,
        onboarding_ind ? true : false,
        onboarding_ind ? onboarding_ind->valueint : 0,
        aerial_ue_ind ? aerial_ue_indVariable : 0,
        subscribed_max_ipv6_prefix_size ? true : false,
        subscribed_max_ipv6_prefix_size ? subscribed_max_ipv6_prefix_size->valuedouble : 0
    );

    return dnn_configuration_local_var;
end:
    if (pdu_session_types_local_nonprim) {
        OpenAPI_pdu_session_types_free(pdu_session_types_local_nonprim);
        pdu_session_types_local_nonprim = NULL;
    }
    if (ssc_modes_local_nonprim) {
        OpenAPI_ssc_modes_free(ssc_modes_local_nonprim);
        ssc_modes_local_nonprim = NULL;
    }
    if (_5g_qos_profile_local_nonprim) {
        OpenAPI_subscribed_default_qos_free(_5g_qos_profile_local_nonprim);
        _5g_qos_profile_local_nonprim = NULL;
    }
    if (session_ambr_local_nonprim) {
        OpenAPI_ambr_free(session_ambr_local_nonprim);
        session_ambr_local_nonprim = NULL;
    }
    if (static_ip_addressList) {
        OpenAPI_list_for_each(static_ip_addressList, node) {
            OpenAPI_ip_address_free(node->data);
        }
        OpenAPI_list_free(static_ip_addressList);
        static_ip_addressList = NULL;
    }
    if (up_security_local_nonprim) {
        OpenAPI_up_security_free(up_security_local_nonprim);
        up_security_local_nonprim = NULL;
    }
    if (nidd_info_local_nonprim) {
        OpenAPI_nidd_information_free(nidd_info_local_nonprim);
        nidd_info_local_nonprim = NULL;
    }
    if (acs_info_local_nonprim) {
        OpenAPI_acs_info_free(acs_info_local_nonprim);
        acs_info_local_nonprim = NULL;
    }
    if (ipv4_frame_route_listList) {
        OpenAPI_list_for_each(ipv4_frame_route_listList, node) {
            OpenAPI_frame_route_info_free(node->data);
        }
        OpenAPI_list_free(ipv4_frame_route_listList);
        ipv4_frame_route_listList = NULL;
    }
    if (ipv6_frame_route_listList) {
        OpenAPI_list_for_each(ipv6_frame_route_listList, node) {
            OpenAPI_frame_route_info_free(node->data);
        }
        OpenAPI_list_free(ipv6_frame_route_listList);
        ipv6_frame_route_listList = NULL;
    }
    if (dn_aaa_address_local_nonprim) {
        OpenAPI_ip_address_free(dn_aaa_address_local_nonprim);
        dn_aaa_address_local_nonprim = NULL;
    }
    if (additional_dn_aaa_addressesList) {
        OpenAPI_list_for_each(additional_dn_aaa_addressesList, node) {
            OpenAPI_ip_address_free(node->data);
        }
        OpenAPI_list_free(additional_dn_aaa_addressesList);
        additional_dn_aaa_addressesList = NULL;
    }
    if (ipv4_index_local_nonprim) {
        OpenAPI_ip_index_free(ipv4_index_local_nonprim);
        ipv4_index_local_nonprim = NULL;
    }
    if (ipv6_index_local_nonprim) {
        OpenAPI_ip_index_free(ipv6_index_local_nonprim);
        ipv6_index_local_nonprim = NULL;
    }
    if (ecs_addr_config_info_local_nonprim) {
        OpenAPI_ecs_addr_config_info_free(ecs_addr_config_info_local_nonprim);
        ecs_addr_config_info_local_nonprim = NULL;
    }
    if (additional_ecs_addr_config_infosList) {
        OpenAPI_list_for_each(additional_ecs_addr_config_infosList, node) {
            OpenAPI_ecs_addr_config_info_free(node->data);
        }
        OpenAPI_list_free(additional_ecs_addr_config_infosList);
        additional_ecs_addr_config_infosList = NULL;
    }
    if (additional_shared_ecs_addr_config_info_idsList) {
        OpenAPI_list_for_each(additional_shared_ecs_addr_config_info_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_shared_ecs_addr_config_info_idsList);
        additional_shared_ecs_addr_config_info_idsList = NULL;
    }
    return NULL;
}

OpenAPI_dnn_configuration_t *OpenAPI_dnn_configuration_copy(OpenAPI_dnn_configuration_t *dst, OpenAPI_dnn_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_configuration_convertToJSON() failed");
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

    OpenAPI_dnn_configuration_free(dst);
    dst = OpenAPI_dnn_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

