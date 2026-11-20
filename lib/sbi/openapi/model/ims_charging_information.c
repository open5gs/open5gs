
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ims_charging_information.h"

OpenAPI_ims_charging_information_t *OpenAPI_ims_charging_information_create(
    OpenAPI_sip_event_type_t *event_type,
    OpenAPI_ims_node_functionality_e i_ms_node_functionality,
    OpenAPI_role_of_ims_node_e role_of_node,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_location_info,
    char *ue_time_zone,
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_status,
    OpenAPI_isup_cause_t *isup_cause,
    bool is_control_plane_address_null,
    OpenAPI_ims_address_t *control_plane_address,
    char *vlr_number,
    char *msc_address,
    char *user_session_id,
    char *outgoing_session_id,
    OpenAPI_ims_session_priority_e session_priority,
    OpenAPI_list_t *calling_party_addresses,
    char *called_party_address,
    char *number_portability_routinginformation,
    char *carrier_select_routing_information,
    char *alternate_charged_party_address,
    OpenAPI_list_t *requested_party_address,
    OpenAPI_list_t *called_asserted_identities,
    OpenAPI_list_t *called_identity_changes,
    OpenAPI_list_t *associated_uri,
    char *time_stamps,
    OpenAPI_list_t *application_server_information,
    OpenAPI_list_t *inter_operator_identifier,
    char *ims_charging_identifier,
    char *related_icid,
    char *related_icid_generation_node,
    OpenAPI_list_t *transit_ioi_list,
    OpenAPI_list_t *early_media_description,
    OpenAPI_list_t *sdp_session_description,
    OpenAPI_list_t *sdp_media_component,
    bool is_served_party_ip_address_null,
    OpenAPI_ims_address_t *served_party_ip_address,
    OpenAPI_server_capabilities_t *server_capabilities,
    OpenAPI_trunk_group_id_t *trunk_group_id,
    char *bearer_service,
    char *ims_service_id,
    OpenAPI_list_t *message_bodies,
    OpenAPI_list_t *access_network_information,
    char *additional_access_network_information,
    char *cellular_network_information,
    OpenAPI_list_t *access_transfer_information,
    OpenAPI_list_t *access_network_info_change,
    char *ims_communication_service_id,
    char *ims_application_reference_id,
    bool is_cause_code,
    int cause_code,
    OpenAPI_list_t *reason_header,
    char *initial_ims_charging_identifier,
    OpenAPI_list_t *nni_information,
    char *from_address,
    bool is_ims_emergency_indication,
    int ims_emergency_indication,
    char *ims_visited_network_identifier,
    char *sip_route_header_received,
    char *sip_route_header_transmitted,
    OpenAPI_tad_identifier_e tad_identifier,
    char *fe_identifier_list,
    OpenAPI_imsdc_app_info_t *ims_dc_app_info,
    OpenAPI_list_t *satellite_id_list,
    OpenAPI_media_resource_t *media_resource
)
{
    OpenAPI_ims_charging_information_t *ims_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_ims_charging_information_t));
    ogs_assert(ims_charging_information_local_var);

    ims_charging_information_local_var->event_type = event_type;
    ims_charging_information_local_var->i_ms_node_functionality = i_ms_node_functionality;
    ims_charging_information_local_var->role_of_node = role_of_node;
    ims_charging_information_local_var->user_information = user_information;
    ims_charging_information_local_var->user_location_info = user_location_info;
    ims_charging_information_local_var->ue_time_zone = ue_time_zone;
    ims_charging_information_local_var->_3gpp_ps_data_off_status = _3gpp_ps_data_off_status;
    ims_charging_information_local_var->isup_cause = isup_cause;
    ims_charging_information_local_var->is_control_plane_address_null = is_control_plane_address_null;
    ims_charging_information_local_var->control_plane_address = control_plane_address;
    ims_charging_information_local_var->vlr_number = vlr_number;
    ims_charging_information_local_var->msc_address = msc_address;
    ims_charging_information_local_var->user_session_id = user_session_id;
    ims_charging_information_local_var->outgoing_session_id = outgoing_session_id;
    ims_charging_information_local_var->session_priority = session_priority;
    ims_charging_information_local_var->calling_party_addresses = calling_party_addresses;
    ims_charging_information_local_var->called_party_address = called_party_address;
    ims_charging_information_local_var->number_portability_routinginformation = number_portability_routinginformation;
    ims_charging_information_local_var->carrier_select_routing_information = carrier_select_routing_information;
    ims_charging_information_local_var->alternate_charged_party_address = alternate_charged_party_address;
    ims_charging_information_local_var->requested_party_address = requested_party_address;
    ims_charging_information_local_var->called_asserted_identities = called_asserted_identities;
    ims_charging_information_local_var->called_identity_changes = called_identity_changes;
    ims_charging_information_local_var->associated_uri = associated_uri;
    ims_charging_information_local_var->time_stamps = time_stamps;
    ims_charging_information_local_var->application_server_information = application_server_information;
    ims_charging_information_local_var->inter_operator_identifier = inter_operator_identifier;
    ims_charging_information_local_var->ims_charging_identifier = ims_charging_identifier;
    ims_charging_information_local_var->related_icid = related_icid;
    ims_charging_information_local_var->related_icid_generation_node = related_icid_generation_node;
    ims_charging_information_local_var->transit_ioi_list = transit_ioi_list;
    ims_charging_information_local_var->early_media_description = early_media_description;
    ims_charging_information_local_var->sdp_session_description = sdp_session_description;
    ims_charging_information_local_var->sdp_media_component = sdp_media_component;
    ims_charging_information_local_var->is_served_party_ip_address_null = is_served_party_ip_address_null;
    ims_charging_information_local_var->served_party_ip_address = served_party_ip_address;
    ims_charging_information_local_var->server_capabilities = server_capabilities;
    ims_charging_information_local_var->trunk_group_id = trunk_group_id;
    ims_charging_information_local_var->bearer_service = bearer_service;
    ims_charging_information_local_var->ims_service_id = ims_service_id;
    ims_charging_information_local_var->message_bodies = message_bodies;
    ims_charging_information_local_var->access_network_information = access_network_information;
    ims_charging_information_local_var->additional_access_network_information = additional_access_network_information;
    ims_charging_information_local_var->cellular_network_information = cellular_network_information;
    ims_charging_information_local_var->access_transfer_information = access_transfer_information;
    ims_charging_information_local_var->access_network_info_change = access_network_info_change;
    ims_charging_information_local_var->ims_communication_service_id = ims_communication_service_id;
    ims_charging_information_local_var->ims_application_reference_id = ims_application_reference_id;
    ims_charging_information_local_var->is_cause_code = is_cause_code;
    ims_charging_information_local_var->cause_code = cause_code;
    ims_charging_information_local_var->reason_header = reason_header;
    ims_charging_information_local_var->initial_ims_charging_identifier = initial_ims_charging_identifier;
    ims_charging_information_local_var->nni_information = nni_information;
    ims_charging_information_local_var->from_address = from_address;
    ims_charging_information_local_var->is_ims_emergency_indication = is_ims_emergency_indication;
    ims_charging_information_local_var->ims_emergency_indication = ims_emergency_indication;
    ims_charging_information_local_var->ims_visited_network_identifier = ims_visited_network_identifier;
    ims_charging_information_local_var->sip_route_header_received = sip_route_header_received;
    ims_charging_information_local_var->sip_route_header_transmitted = sip_route_header_transmitted;
    ims_charging_information_local_var->tad_identifier = tad_identifier;
    ims_charging_information_local_var->fe_identifier_list = fe_identifier_list;
    ims_charging_information_local_var->ims_dc_app_info = ims_dc_app_info;
    ims_charging_information_local_var->satellite_id_list = satellite_id_list;
    ims_charging_information_local_var->media_resource = media_resource;

    return ims_charging_information_local_var;
}

void OpenAPI_ims_charging_information_free(OpenAPI_ims_charging_information_t *ims_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ims_charging_information) {
        return;
    }
    if (ims_charging_information->event_type) {
        OpenAPI_sip_event_type_free(ims_charging_information->event_type);
        ims_charging_information->event_type = NULL;
    }
    if (ims_charging_information->user_information) {
        OpenAPI_user_information_free(ims_charging_information->user_information);
        ims_charging_information->user_information = NULL;
    }
    if (ims_charging_information->user_location_info) {
        OpenAPI_charging_user_location_free(ims_charging_information->user_location_info);
        ims_charging_information->user_location_info = NULL;
    }
    if (ims_charging_information->ue_time_zone) {
        ogs_free(ims_charging_information->ue_time_zone);
        ims_charging_information->ue_time_zone = NULL;
    }
    if (ims_charging_information->isup_cause) {
        OpenAPI_isup_cause_free(ims_charging_information->isup_cause);
        ims_charging_information->isup_cause = NULL;
    }
    if (ims_charging_information->control_plane_address) {
        OpenAPI_ims_address_free(ims_charging_information->control_plane_address);
        ims_charging_information->control_plane_address = NULL;
    }
    if (ims_charging_information->vlr_number) {
        ogs_free(ims_charging_information->vlr_number);
        ims_charging_information->vlr_number = NULL;
    }
    if (ims_charging_information->msc_address) {
        ogs_free(ims_charging_information->msc_address);
        ims_charging_information->msc_address = NULL;
    }
    if (ims_charging_information->user_session_id) {
        ogs_free(ims_charging_information->user_session_id);
        ims_charging_information->user_session_id = NULL;
    }
    if (ims_charging_information->outgoing_session_id) {
        ogs_free(ims_charging_information->outgoing_session_id);
        ims_charging_information->outgoing_session_id = NULL;
    }
    if (ims_charging_information->calling_party_addresses) {
        OpenAPI_list_for_each(ims_charging_information->calling_party_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->calling_party_addresses);
        ims_charging_information->calling_party_addresses = NULL;
    }
    if (ims_charging_information->called_party_address) {
        ogs_free(ims_charging_information->called_party_address);
        ims_charging_information->called_party_address = NULL;
    }
    if (ims_charging_information->number_portability_routinginformation) {
        ogs_free(ims_charging_information->number_portability_routinginformation);
        ims_charging_information->number_portability_routinginformation = NULL;
    }
    if (ims_charging_information->carrier_select_routing_information) {
        ogs_free(ims_charging_information->carrier_select_routing_information);
        ims_charging_information->carrier_select_routing_information = NULL;
    }
    if (ims_charging_information->alternate_charged_party_address) {
        ogs_free(ims_charging_information->alternate_charged_party_address);
        ims_charging_information->alternate_charged_party_address = NULL;
    }
    if (ims_charging_information->requested_party_address) {
        OpenAPI_list_for_each(ims_charging_information->requested_party_address, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->requested_party_address);
        ims_charging_information->requested_party_address = NULL;
    }
    if (ims_charging_information->called_asserted_identities) {
        OpenAPI_list_for_each(ims_charging_information->called_asserted_identities, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->called_asserted_identities);
        ims_charging_information->called_asserted_identities = NULL;
    }
    if (ims_charging_information->called_identity_changes) {
        OpenAPI_list_for_each(ims_charging_information->called_identity_changes, node) {
            OpenAPI_called_identity_change_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->called_identity_changes);
        ims_charging_information->called_identity_changes = NULL;
    }
    if (ims_charging_information->associated_uri) {
        OpenAPI_list_for_each(ims_charging_information->associated_uri, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->associated_uri);
        ims_charging_information->associated_uri = NULL;
    }
    if (ims_charging_information->time_stamps) {
        ogs_free(ims_charging_information->time_stamps);
        ims_charging_information->time_stamps = NULL;
    }
    if (ims_charging_information->application_server_information) {
        OpenAPI_list_for_each(ims_charging_information->application_server_information, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->application_server_information);
        ims_charging_information->application_server_information = NULL;
    }
    if (ims_charging_information->inter_operator_identifier) {
        OpenAPI_list_for_each(ims_charging_information->inter_operator_identifier, node) {
            OpenAPI_inter_operator_identifier_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->inter_operator_identifier);
        ims_charging_information->inter_operator_identifier = NULL;
    }
    if (ims_charging_information->ims_charging_identifier) {
        ogs_free(ims_charging_information->ims_charging_identifier);
        ims_charging_information->ims_charging_identifier = NULL;
    }
    if (ims_charging_information->related_icid) {
        ogs_free(ims_charging_information->related_icid);
        ims_charging_information->related_icid = NULL;
    }
    if (ims_charging_information->related_icid_generation_node) {
        ogs_free(ims_charging_information->related_icid_generation_node);
        ims_charging_information->related_icid_generation_node = NULL;
    }
    if (ims_charging_information->transit_ioi_list) {
        OpenAPI_list_for_each(ims_charging_information->transit_ioi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->transit_ioi_list);
        ims_charging_information->transit_ioi_list = NULL;
    }
    if (ims_charging_information->early_media_description) {
        OpenAPI_list_for_each(ims_charging_information->early_media_description, node) {
            OpenAPI_early_media_description_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->early_media_description);
        ims_charging_information->early_media_description = NULL;
    }
    if (ims_charging_information->sdp_session_description) {
        OpenAPI_list_for_each(ims_charging_information->sdp_session_description, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->sdp_session_description);
        ims_charging_information->sdp_session_description = NULL;
    }
    if (ims_charging_information->sdp_media_component) {
        OpenAPI_list_for_each(ims_charging_information->sdp_media_component, node) {
            OpenAPI_sdp_media_component_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->sdp_media_component);
        ims_charging_information->sdp_media_component = NULL;
    }
    if (ims_charging_information->served_party_ip_address) {
        OpenAPI_ims_address_free(ims_charging_information->served_party_ip_address);
        ims_charging_information->served_party_ip_address = NULL;
    }
    if (ims_charging_information->server_capabilities) {
        OpenAPI_server_capabilities_free(ims_charging_information->server_capabilities);
        ims_charging_information->server_capabilities = NULL;
    }
    if (ims_charging_information->trunk_group_id) {
        OpenAPI_trunk_group_id_free(ims_charging_information->trunk_group_id);
        ims_charging_information->trunk_group_id = NULL;
    }
    if (ims_charging_information->bearer_service) {
        ogs_free(ims_charging_information->bearer_service);
        ims_charging_information->bearer_service = NULL;
    }
    if (ims_charging_information->ims_service_id) {
        ogs_free(ims_charging_information->ims_service_id);
        ims_charging_information->ims_service_id = NULL;
    }
    if (ims_charging_information->message_bodies) {
        OpenAPI_list_for_each(ims_charging_information->message_bodies, node) {
            OpenAPI_message_body_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->message_bodies);
        ims_charging_information->message_bodies = NULL;
    }
    if (ims_charging_information->access_network_information) {
        OpenAPI_list_for_each(ims_charging_information->access_network_information, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->access_network_information);
        ims_charging_information->access_network_information = NULL;
    }
    if (ims_charging_information->additional_access_network_information) {
        ogs_free(ims_charging_information->additional_access_network_information);
        ims_charging_information->additional_access_network_information = NULL;
    }
    if (ims_charging_information->cellular_network_information) {
        ogs_free(ims_charging_information->cellular_network_information);
        ims_charging_information->cellular_network_information = NULL;
    }
    if (ims_charging_information->access_transfer_information) {
        OpenAPI_list_for_each(ims_charging_information->access_transfer_information, node) {
            OpenAPI_access_transfer_information_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->access_transfer_information);
        ims_charging_information->access_transfer_information = NULL;
    }
    if (ims_charging_information->access_network_info_change) {
        OpenAPI_list_for_each(ims_charging_information->access_network_info_change, node) {
            OpenAPI_access_network_info_change_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->access_network_info_change);
        ims_charging_information->access_network_info_change = NULL;
    }
    if (ims_charging_information->ims_communication_service_id) {
        ogs_free(ims_charging_information->ims_communication_service_id);
        ims_charging_information->ims_communication_service_id = NULL;
    }
    if (ims_charging_information->ims_application_reference_id) {
        ogs_free(ims_charging_information->ims_application_reference_id);
        ims_charging_information->ims_application_reference_id = NULL;
    }
    if (ims_charging_information->reason_header) {
        OpenAPI_list_for_each(ims_charging_information->reason_header, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->reason_header);
        ims_charging_information->reason_header = NULL;
    }
    if (ims_charging_information->initial_ims_charging_identifier) {
        ogs_free(ims_charging_information->initial_ims_charging_identifier);
        ims_charging_information->initial_ims_charging_identifier = NULL;
    }
    if (ims_charging_information->nni_information) {
        OpenAPI_list_for_each(ims_charging_information->nni_information, node) {
            OpenAPI_nni_information_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->nni_information);
        ims_charging_information->nni_information = NULL;
    }
    if (ims_charging_information->from_address) {
        ogs_free(ims_charging_information->from_address);
        ims_charging_information->from_address = NULL;
    }
    if (ims_charging_information->ims_visited_network_identifier) {
        ogs_free(ims_charging_information->ims_visited_network_identifier);
        ims_charging_information->ims_visited_network_identifier = NULL;
    }
    if (ims_charging_information->sip_route_header_received) {
        ogs_free(ims_charging_information->sip_route_header_received);
        ims_charging_information->sip_route_header_received = NULL;
    }
    if (ims_charging_information->sip_route_header_transmitted) {
        ogs_free(ims_charging_information->sip_route_header_transmitted);
        ims_charging_information->sip_route_header_transmitted = NULL;
    }
    if (ims_charging_information->fe_identifier_list) {
        ogs_free(ims_charging_information->fe_identifier_list);
        ims_charging_information->fe_identifier_list = NULL;
    }
    if (ims_charging_information->ims_dc_app_info) {
        OpenAPI_imsdc_app_info_free(ims_charging_information->ims_dc_app_info);
        ims_charging_information->ims_dc_app_info = NULL;
    }
    if (ims_charging_information->satellite_id_list) {
        OpenAPI_list_for_each(ims_charging_information->satellite_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_charging_information->satellite_id_list);
        ims_charging_information->satellite_id_list = NULL;
    }
    if (ims_charging_information->media_resource) {
        OpenAPI_media_resource_free(ims_charging_information->media_resource);
        ims_charging_information->media_resource = NULL;
    }
    ogs_free(ims_charging_information);
}

cJSON *OpenAPI_ims_charging_information_convertToJSON(OpenAPI_ims_charging_information_t *ims_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ims_charging_information == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [IMSChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ims_charging_information->event_type) {
    cJSON *event_type_local_JSON = OpenAPI_sip_event_type_convertToJSON(ims_charging_information->event_type);
    if (event_type_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [event_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eventType", event_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [event_type]");
        goto end;
    }
    }

    if (ims_charging_information->i_ms_node_functionality != OpenAPI_ims_node_functionality_NULL) {
    if (cJSON_AddStringToObject(item, "iMSNodeFunctionality", OpenAPI_ims_node_functionality_ToString(ims_charging_information->i_ms_node_functionality)) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [i_ms_node_functionality]");
        goto end;
    }
    }

    if (ims_charging_information->role_of_node != OpenAPI_role_of_ims_node_NULL) {
    if (cJSON_AddStringToObject(item, "roleOfNode", OpenAPI_role_of_ims_node_ToString(ims_charging_information->role_of_node)) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [role_of_node]");
        goto end;
    }
    }

    if (ims_charging_information->user_information) {
    cJSON *user_information_local_JSON = OpenAPI_user_information_convertToJSON(ims_charging_information->user_information);
    if (user_information_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userInformation", user_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    }

    if (ims_charging_information->user_location_info) {
    cJSON *user_location_info_local_JSON = OpenAPI_charging_user_location_convertToJSON(ims_charging_information->user_location_info);
    if (user_location_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [user_location_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInfo", user_location_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [user_location_info]");
        goto end;
    }
    }

    if (ims_charging_information->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", ims_charging_information->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (ims_charging_information->_3gpp_ps_data_off_status != OpenAPI_model_3_gppps_data_off_status_NULL) {
    if (cJSON_AddStringToObject(item, "3gppPSDataOffStatus", OpenAPI_model_3_gppps_data_off_status_ToString(ims_charging_information->_3gpp_ps_data_off_status)) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    }

    if (ims_charging_information->isup_cause) {
    cJSON *isup_cause_local_JSON = OpenAPI_isup_cause_convertToJSON(ims_charging_information->isup_cause);
    if (isup_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [isup_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "isupCause", isup_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [isup_cause]");
        goto end;
    }
    }

    if (ims_charging_information->control_plane_address) {
    cJSON *control_plane_address_local_JSON = OpenAPI_ims_address_convertToJSON(ims_charging_information->control_plane_address);
    if (control_plane_address_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [control_plane_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "controlPlaneAddress", control_plane_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [control_plane_address]");
        goto end;
    }
    } else if (ims_charging_information->is_control_plane_address_null) {
        if (cJSON_AddNullToObject(item, "controlPlaneAddress") == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [control_plane_address]");
            goto end;
        }
    }

    if (ims_charging_information->vlr_number) {
    if (cJSON_AddStringToObject(item, "vlrNumber", ims_charging_information->vlr_number) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [vlr_number]");
        goto end;
    }
    }

    if (ims_charging_information->msc_address) {
    if (cJSON_AddStringToObject(item, "mscAddress", ims_charging_information->msc_address) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [msc_address]");
        goto end;
    }
    }

    if (ims_charging_information->user_session_id) {
    if (cJSON_AddStringToObject(item, "userSessionID", ims_charging_information->user_session_id) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [user_session_id]");
        goto end;
    }
    }

    if (ims_charging_information->outgoing_session_id) {
    if (cJSON_AddStringToObject(item, "outgoingSessionID", ims_charging_information->outgoing_session_id) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [outgoing_session_id]");
        goto end;
    }
    }

    if (ims_charging_information->session_priority != OpenAPI_ims_session_priority_NULL) {
    if (cJSON_AddStringToObject(item, "sessionPriority", OpenAPI_ims_session_priority_ToString(ims_charging_information->session_priority)) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [session_priority]");
        goto end;
    }
    }

    if (ims_charging_information->calling_party_addresses) {
    cJSON *calling_party_addressesList = cJSON_AddArrayToObject(item, "callingPartyAddresses");
    if (calling_party_addressesList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [calling_party_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->calling_party_addresses, node) {
        if (cJSON_AddStringToObject(calling_party_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [calling_party_addresses]");
            goto end;
        }
    }
    }

    if (ims_charging_information->called_party_address) {
    if (cJSON_AddStringToObject(item, "calledPartyAddress", ims_charging_information->called_party_address) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [called_party_address]");
        goto end;
    }
    }

    if (ims_charging_information->number_portability_routinginformation) {
    if (cJSON_AddStringToObject(item, "numberPortabilityRoutinginformation", ims_charging_information->number_portability_routinginformation) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [number_portability_routinginformation]");
        goto end;
    }
    }

    if (ims_charging_information->carrier_select_routing_information) {
    if (cJSON_AddStringToObject(item, "carrierSelectRoutingInformation", ims_charging_information->carrier_select_routing_information) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [carrier_select_routing_information]");
        goto end;
    }
    }

    if (ims_charging_information->alternate_charged_party_address) {
    if (cJSON_AddStringToObject(item, "alternateChargedPartyAddress", ims_charging_information->alternate_charged_party_address) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [alternate_charged_party_address]");
        goto end;
    }
    }

    if (ims_charging_information->requested_party_address) {
    cJSON *requested_party_addressList = cJSON_AddArrayToObject(item, "requestedPartyAddress");
    if (requested_party_addressList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [requested_party_address]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->requested_party_address, node) {
        if (cJSON_AddStringToObject(requested_party_addressList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [requested_party_address]");
            goto end;
        }
    }
    }

    if (ims_charging_information->called_asserted_identities) {
    cJSON *called_asserted_identitiesList = cJSON_AddArrayToObject(item, "calledAssertedIdentities");
    if (called_asserted_identitiesList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [called_asserted_identities]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->called_asserted_identities, node) {
        if (cJSON_AddStringToObject(called_asserted_identitiesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [called_asserted_identities]");
            goto end;
        }
    }
    }

    if (ims_charging_information->called_identity_changes) {
    cJSON *called_identity_changesList = cJSON_AddArrayToObject(item, "calledIdentityChanges");
    if (called_identity_changesList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [called_identity_changes]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->called_identity_changes, node) {
        cJSON *itemLocal = OpenAPI_called_identity_change_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [called_identity_changes]");
            goto end;
        }
        cJSON_AddItemToArray(called_identity_changesList, itemLocal);
    }
    }

    if (ims_charging_information->associated_uri) {
    cJSON *associated_uriList = cJSON_AddArrayToObject(item, "associatedURI");
    if (associated_uriList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [associated_uri]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->associated_uri, node) {
        if (cJSON_AddStringToObject(associated_uriList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [associated_uri]");
            goto end;
        }
    }
    }

    if (ims_charging_information->time_stamps) {
    if (cJSON_AddStringToObject(item, "timeStamps", ims_charging_information->time_stamps) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [time_stamps]");
        goto end;
    }
    }

    if (ims_charging_information->application_server_information) {
    cJSON *application_server_informationList = cJSON_AddArrayToObject(item, "applicationServerInformation");
    if (application_server_informationList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [application_server_information]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->application_server_information, node) {
        if (cJSON_AddStringToObject(application_server_informationList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [application_server_information]");
            goto end;
        }
    }
    }

    if (ims_charging_information->inter_operator_identifier) {
    cJSON *inter_operator_identifierList = cJSON_AddArrayToObject(item, "interOperatorIdentifier");
    if (inter_operator_identifierList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [inter_operator_identifier]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->inter_operator_identifier, node) {
        cJSON *itemLocal = OpenAPI_inter_operator_identifier_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [inter_operator_identifier]");
            goto end;
        }
        cJSON_AddItemToArray(inter_operator_identifierList, itemLocal);
    }
    }

    if (ims_charging_information->ims_charging_identifier) {
    if (cJSON_AddStringToObject(item, "imsChargingIdentifier", ims_charging_information->ims_charging_identifier) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_charging_identifier]");
        goto end;
    }
    }

    if (ims_charging_information->related_icid) {
    if (cJSON_AddStringToObject(item, "relatedICID", ims_charging_information->related_icid) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [related_icid]");
        goto end;
    }
    }

    if (ims_charging_information->related_icid_generation_node) {
    if (cJSON_AddStringToObject(item, "relatedICIDGenerationNode", ims_charging_information->related_icid_generation_node) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [related_icid_generation_node]");
        goto end;
    }
    }

    if (ims_charging_information->transit_ioi_list) {
    cJSON *transit_ioi_listList = cJSON_AddArrayToObject(item, "transitIOIList");
    if (transit_ioi_listList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [transit_ioi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->transit_ioi_list, node) {
        if (cJSON_AddStringToObject(transit_ioi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [transit_ioi_list]");
            goto end;
        }
    }
    }

    if (ims_charging_information->early_media_description) {
    cJSON *early_media_descriptionList = cJSON_AddArrayToObject(item, "earlyMediaDescription");
    if (early_media_descriptionList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [early_media_description]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->early_media_description, node) {
        cJSON *itemLocal = OpenAPI_early_media_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [early_media_description]");
            goto end;
        }
        cJSON_AddItemToArray(early_media_descriptionList, itemLocal);
    }
    }

    if (ims_charging_information->sdp_session_description) {
    cJSON *sdp_session_descriptionList = cJSON_AddArrayToObject(item, "sdpSessionDescription");
    if (sdp_session_descriptionList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [sdp_session_description]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->sdp_session_description, node) {
        if (cJSON_AddStringToObject(sdp_session_descriptionList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [sdp_session_description]");
            goto end;
        }
    }
    }

    if (ims_charging_information->sdp_media_component) {
    cJSON *sdp_media_componentList = cJSON_AddArrayToObject(item, "sdpMediaComponent");
    if (sdp_media_componentList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [sdp_media_component]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->sdp_media_component, node) {
        cJSON *itemLocal = OpenAPI_sdp_media_component_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [sdp_media_component]");
            goto end;
        }
        cJSON_AddItemToArray(sdp_media_componentList, itemLocal);
    }
    }

    if (ims_charging_information->served_party_ip_address) {
    cJSON *served_party_ip_address_local_JSON = OpenAPI_ims_address_convertToJSON(ims_charging_information->served_party_ip_address);
    if (served_party_ip_address_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [served_party_ip_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servedPartyIPAddress", served_party_ip_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [served_party_ip_address]");
        goto end;
    }
    } else if (ims_charging_information->is_served_party_ip_address_null) {
        if (cJSON_AddNullToObject(item, "servedPartyIPAddress") == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [served_party_ip_address]");
            goto end;
        }
    }

    if (ims_charging_information->server_capabilities) {
    cJSON *server_capabilities_local_JSON = OpenAPI_server_capabilities_convertToJSON(ims_charging_information->server_capabilities);
    if (server_capabilities_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [server_capabilities]");
        goto end;
    }
    cJSON_AddItemToObject(item, "serverCapabilities", server_capabilities_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [server_capabilities]");
        goto end;
    }
    }

    if (ims_charging_information->trunk_group_id) {
    cJSON *trunk_group_id_local_JSON = OpenAPI_trunk_group_id_convertToJSON(ims_charging_information->trunk_group_id);
    if (trunk_group_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [trunk_group_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trunkGroupID", trunk_group_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [trunk_group_id]");
        goto end;
    }
    }

    if (ims_charging_information->bearer_service) {
    if (cJSON_AddStringToObject(item, "bearerService", ims_charging_information->bearer_service) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [bearer_service]");
        goto end;
    }
    }

    if (ims_charging_information->ims_service_id) {
    if (cJSON_AddStringToObject(item, "imsServiceId", ims_charging_information->ims_service_id) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_service_id]");
        goto end;
    }
    }

    if (ims_charging_information->message_bodies) {
    cJSON *message_bodiesList = cJSON_AddArrayToObject(item, "messageBodies");
    if (message_bodiesList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [message_bodies]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->message_bodies, node) {
        cJSON *itemLocal = OpenAPI_message_body_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [message_bodies]");
            goto end;
        }
        cJSON_AddItemToArray(message_bodiesList, itemLocal);
    }
    }

    if (ims_charging_information->access_network_information) {
    cJSON *access_network_informationList = cJSON_AddArrayToObject(item, "accessNetworkInformation");
    if (access_network_informationList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [access_network_information]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->access_network_information, node) {
        if (cJSON_AddStringToObject(access_network_informationList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [access_network_information]");
            goto end;
        }
    }
    }

    if (ims_charging_information->additional_access_network_information) {
    if (cJSON_AddStringToObject(item, "additionalAccessNetworkInformation", ims_charging_information->additional_access_network_information) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [additional_access_network_information]");
        goto end;
    }
    }

    if (ims_charging_information->cellular_network_information) {
    if (cJSON_AddStringToObject(item, "cellularNetworkInformation", ims_charging_information->cellular_network_information) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [cellular_network_information]");
        goto end;
    }
    }

    if (ims_charging_information->access_transfer_information) {
    cJSON *access_transfer_informationList = cJSON_AddArrayToObject(item, "accessTransferInformation");
    if (access_transfer_informationList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [access_transfer_information]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->access_transfer_information, node) {
        cJSON *itemLocal = OpenAPI_access_transfer_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [access_transfer_information]");
            goto end;
        }
        cJSON_AddItemToArray(access_transfer_informationList, itemLocal);
    }
    }

    if (ims_charging_information->access_network_info_change) {
    cJSON *access_network_info_changeList = cJSON_AddArrayToObject(item, "accessNetworkInfoChange");
    if (access_network_info_changeList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [access_network_info_change]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->access_network_info_change, node) {
        cJSON *itemLocal = OpenAPI_access_network_info_change_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [access_network_info_change]");
            goto end;
        }
        cJSON_AddItemToArray(access_network_info_changeList, itemLocal);
    }
    }

    if (ims_charging_information->ims_communication_service_id) {
    if (cJSON_AddStringToObject(item, "imsCommunicationServiceID", ims_charging_information->ims_communication_service_id) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_communication_service_id]");
        goto end;
    }
    }

    if (ims_charging_information->ims_application_reference_id) {
    if (cJSON_AddStringToObject(item, "imsApplicationReferenceID", ims_charging_information->ims_application_reference_id) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_application_reference_id]");
        goto end;
    }
    }

    if (ims_charging_information->is_cause_code) {
    if (cJSON_AddNumberToObject(item, "causeCode", ims_charging_information->cause_code) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [cause_code]");
        goto end;
    }
    }

    if (ims_charging_information->reason_header) {
    cJSON *reason_headerList = cJSON_AddArrayToObject(item, "reasonHeader");
    if (reason_headerList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [reason_header]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->reason_header, node) {
        if (cJSON_AddStringToObject(reason_headerList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [reason_header]");
            goto end;
        }
    }
    }

    if (ims_charging_information->initial_ims_charging_identifier) {
    if (cJSON_AddStringToObject(item, "initialIMSChargingIdentifier", ims_charging_information->initial_ims_charging_identifier) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [initial_ims_charging_identifier]");
        goto end;
    }
    }

    if (ims_charging_information->nni_information) {
    cJSON *nni_informationList = cJSON_AddArrayToObject(item, "nniInformation");
    if (nni_informationList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [nni_information]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->nni_information, node) {
        cJSON *itemLocal = OpenAPI_nni_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [nni_information]");
            goto end;
        }
        cJSON_AddItemToArray(nni_informationList, itemLocal);
    }
    }

    if (ims_charging_information->from_address) {
    if (cJSON_AddStringToObject(item, "fromAddress", ims_charging_information->from_address) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [from_address]");
        goto end;
    }
    }

    if (ims_charging_information->is_ims_emergency_indication) {
    if (cJSON_AddBoolToObject(item, "imsEmergencyIndication", ims_charging_information->ims_emergency_indication) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_emergency_indication]");
        goto end;
    }
    }

    if (ims_charging_information->ims_visited_network_identifier) {
    if (cJSON_AddStringToObject(item, "imsVisitedNetworkIdentifier", ims_charging_information->ims_visited_network_identifier) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_visited_network_identifier]");
        goto end;
    }
    }

    if (ims_charging_information->sip_route_header_received) {
    if (cJSON_AddStringToObject(item, "sipRouteHeaderReceived", ims_charging_information->sip_route_header_received) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [sip_route_header_received]");
        goto end;
    }
    }

    if (ims_charging_information->sip_route_header_transmitted) {
    if (cJSON_AddStringToObject(item, "sipRouteHeaderTransmitted", ims_charging_information->sip_route_header_transmitted) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [sip_route_header_transmitted]");
        goto end;
    }
    }

    if (ims_charging_information->tad_identifier != OpenAPI_tad_identifier_NULL) {
    if (cJSON_AddStringToObject(item, "tadIdentifier", OpenAPI_tad_identifier_ToString(ims_charging_information->tad_identifier)) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [tad_identifier]");
        goto end;
    }
    }

    if (ims_charging_information->fe_identifier_list) {
    if (cJSON_AddStringToObject(item, "feIdentifierList", ims_charging_information->fe_identifier_list) == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [fe_identifier_list]");
        goto end;
    }
    }

    if (ims_charging_information->ims_dc_app_info) {
    cJSON *ims_dc_app_info_local_JSON = OpenAPI_imsdc_app_info_convertToJSON(ims_charging_information->ims_dc_app_info);
    if (ims_dc_app_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_dc_app_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "imsDCAppInfo", ims_dc_app_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [ims_dc_app_info]");
        goto end;
    }
    }

    if (ims_charging_information->satellite_id_list) {
    cJSON *satellite_id_listList = cJSON_AddArrayToObject(item, "satelliteIdList");
    if (satellite_id_listList == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [satellite_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(ims_charging_information->satellite_id_list, node) {
        if (cJSON_AddStringToObject(satellite_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [satellite_id_list]");
            goto end;
        }
    }
    }

    if (ims_charging_information->media_resource) {
    cJSON *media_resource_local_JSON = OpenAPI_media_resource_convertToJSON(ims_charging_information->media_resource);
    if (media_resource_local_JSON == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [media_resource]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mediaResource", media_resource_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed [media_resource]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ims_charging_information_t *OpenAPI_ims_charging_information_parseFromJSON(cJSON *ims_charging_informationJSON)
{
    OpenAPI_ims_charging_information_t *ims_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_type = NULL;
    OpenAPI_sip_event_type_t *event_type_local_nonprim = NULL;
    cJSON *i_ms_node_functionality = NULL;
    OpenAPI_ims_node_functionality_e i_ms_node_functionalityVariable = 0;
    cJSON *role_of_node = NULL;
    OpenAPI_role_of_ims_node_e role_of_nodeVariable = 0;
    cJSON *user_information = NULL;
    OpenAPI_user_information_t *user_information_local_nonprim = NULL;
    cJSON *user_location_info = NULL;
    OpenAPI_charging_user_location_t *user_location_info_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *_3gpp_ps_data_off_status = NULL;
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_statusVariable = 0;
    cJSON *isup_cause = NULL;
    OpenAPI_isup_cause_t *isup_cause_local_nonprim = NULL;
    cJSON *control_plane_address = NULL;
    OpenAPI_ims_address_t *control_plane_address_local_nonprim = NULL;
    cJSON *vlr_number = NULL;
    cJSON *msc_address = NULL;
    cJSON *user_session_id = NULL;
    cJSON *outgoing_session_id = NULL;
    cJSON *session_priority = NULL;
    OpenAPI_ims_session_priority_e session_priorityVariable = 0;
    cJSON *calling_party_addresses = NULL;
    OpenAPI_list_t *calling_party_addressesList = NULL;
    cJSON *called_party_address = NULL;
    cJSON *number_portability_routinginformation = NULL;
    cJSON *carrier_select_routing_information = NULL;
    cJSON *alternate_charged_party_address = NULL;
    cJSON *requested_party_address = NULL;
    OpenAPI_list_t *requested_party_addressList = NULL;
    cJSON *called_asserted_identities = NULL;
    OpenAPI_list_t *called_asserted_identitiesList = NULL;
    cJSON *called_identity_changes = NULL;
    OpenAPI_list_t *called_identity_changesList = NULL;
    cJSON *associated_uri = NULL;
    OpenAPI_list_t *associated_uriList = NULL;
    cJSON *time_stamps = NULL;
    cJSON *application_server_information = NULL;
    OpenAPI_list_t *application_server_informationList = NULL;
    cJSON *inter_operator_identifier = NULL;
    OpenAPI_list_t *inter_operator_identifierList = NULL;
    cJSON *ims_charging_identifier = NULL;
    cJSON *related_icid = NULL;
    cJSON *related_icid_generation_node = NULL;
    cJSON *transit_ioi_list = NULL;
    OpenAPI_list_t *transit_ioi_listList = NULL;
    cJSON *early_media_description = NULL;
    OpenAPI_list_t *early_media_descriptionList = NULL;
    cJSON *sdp_session_description = NULL;
    OpenAPI_list_t *sdp_session_descriptionList = NULL;
    cJSON *sdp_media_component = NULL;
    OpenAPI_list_t *sdp_media_componentList = NULL;
    cJSON *served_party_ip_address = NULL;
    OpenAPI_ims_address_t *served_party_ip_address_local_nonprim = NULL;
    cJSON *server_capabilities = NULL;
    OpenAPI_server_capabilities_t *server_capabilities_local_nonprim = NULL;
    cJSON *trunk_group_id = NULL;
    OpenAPI_trunk_group_id_t *trunk_group_id_local_nonprim = NULL;
    cJSON *bearer_service = NULL;
    cJSON *ims_service_id = NULL;
    cJSON *message_bodies = NULL;
    OpenAPI_list_t *message_bodiesList = NULL;
    cJSON *access_network_information = NULL;
    OpenAPI_list_t *access_network_informationList = NULL;
    cJSON *additional_access_network_information = NULL;
    cJSON *cellular_network_information = NULL;
    cJSON *access_transfer_information = NULL;
    OpenAPI_list_t *access_transfer_informationList = NULL;
    cJSON *access_network_info_change = NULL;
    OpenAPI_list_t *access_network_info_changeList = NULL;
    cJSON *ims_communication_service_id = NULL;
    cJSON *ims_application_reference_id = NULL;
    cJSON *cause_code = NULL;
    cJSON *reason_header = NULL;
    OpenAPI_list_t *reason_headerList = NULL;
    cJSON *initial_ims_charging_identifier = NULL;
    cJSON *nni_information = NULL;
    OpenAPI_list_t *nni_informationList = NULL;
    cJSON *from_address = NULL;
    cJSON *ims_emergency_indication = NULL;
    cJSON *ims_visited_network_identifier = NULL;
    cJSON *sip_route_header_received = NULL;
    cJSON *sip_route_header_transmitted = NULL;
    cJSON *tad_identifier = NULL;
    OpenAPI_tad_identifier_e tad_identifierVariable = 0;
    cJSON *fe_identifier_list = NULL;
    cJSON *ims_dc_app_info = NULL;
    OpenAPI_imsdc_app_info_t *ims_dc_app_info_local_nonprim = NULL;
    cJSON *satellite_id_list = NULL;
    OpenAPI_list_t *satellite_id_listList = NULL;
    cJSON *media_resource = NULL;
    OpenAPI_media_resource_t *media_resource_local_nonprim = NULL;
    event_type = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "eventType");
    if (event_type) {
    event_type_local_nonprim = OpenAPI_sip_event_type_parseFromJSON(event_type);
    if (!event_type_local_nonprim) {
        ogs_error("OpenAPI_sip_event_type_parseFromJSON failed [event_type]");
        goto end;
    }
    }

    i_ms_node_functionality = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "iMSNodeFunctionality");
    if (i_ms_node_functionality) {
    if (!cJSON_IsString(i_ms_node_functionality)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [i_ms_node_functionality]");
        goto end;
    }
    i_ms_node_functionalityVariable = OpenAPI_ims_node_functionality_FromString(i_ms_node_functionality->valuestring);
    }

    role_of_node = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "roleOfNode");
    if (role_of_node) {
    if (!cJSON_IsString(role_of_node)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [role_of_node]");
        goto end;
    }
    role_of_nodeVariable = OpenAPI_role_of_ims_node_FromString(role_of_node->valuestring);
    }

    user_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "userInformation");
    if (user_information) {
    user_information_local_nonprim = OpenAPI_user_information_parseFromJSON(user_information);
    if (!user_information_local_nonprim) {
        ogs_error("OpenAPI_user_information_parseFromJSON failed [user_information]");
        goto end;
    }
    }

    user_location_info = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "userLocationInfo");
    if (user_location_info) {
    user_location_info_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_location_info);
    if (!user_location_info_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_location_info]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    _3gpp_ps_data_off_status = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "3gppPSDataOffStatus");
    if (_3gpp_ps_data_off_status) {
    if (!cJSON_IsString(_3gpp_ps_data_off_status)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    _3gpp_ps_data_off_statusVariable = OpenAPI_model_3_gppps_data_off_status_FromString(_3gpp_ps_data_off_status->valuestring);
    }

    isup_cause = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "isupCause");
    if (isup_cause) {
    isup_cause_local_nonprim = OpenAPI_isup_cause_parseFromJSON(isup_cause);
    if (!isup_cause_local_nonprim) {
        ogs_error("OpenAPI_isup_cause_parseFromJSON failed [isup_cause]");
        goto end;
    }
    }

    control_plane_address = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "controlPlaneAddress");
    if (control_plane_address) {
    if (!cJSON_IsNull(control_plane_address)) {
    control_plane_address_local_nonprim = OpenAPI_ims_address_parseFromJSON(control_plane_address);
    if (!control_plane_address_local_nonprim) {
        ogs_error("OpenAPI_ims_address_parseFromJSON failed [control_plane_address]");
        goto end;
    }
    }
    }

    vlr_number = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "vlrNumber");
    if (vlr_number) {
    if (!cJSON_IsString(vlr_number) && !cJSON_IsNull(vlr_number)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [vlr_number]");
        goto end;
    }
    }

    msc_address = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "mscAddress");
    if (msc_address) {
    if (!cJSON_IsString(msc_address) && !cJSON_IsNull(msc_address)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [msc_address]");
        goto end;
    }
    }

    user_session_id = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "userSessionID");
    if (user_session_id) {
    if (!cJSON_IsString(user_session_id) && !cJSON_IsNull(user_session_id)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [user_session_id]");
        goto end;
    }
    }

    outgoing_session_id = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "outgoingSessionID");
    if (outgoing_session_id) {
    if (!cJSON_IsString(outgoing_session_id) && !cJSON_IsNull(outgoing_session_id)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [outgoing_session_id]");
        goto end;
    }
    }

    session_priority = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "sessionPriority");
    if (session_priority) {
    if (!cJSON_IsString(session_priority)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [session_priority]");
        goto end;
    }
    session_priorityVariable = OpenAPI_ims_session_priority_FromString(session_priority->valuestring);
    }

    calling_party_addresses = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "callingPartyAddresses");
    if (calling_party_addresses) {
        cJSON *calling_party_addresses_local = NULL;
        if (!cJSON_IsArray(calling_party_addresses)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [calling_party_addresses]");
            goto end;
        }

        calling_party_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(calling_party_addresses_local, calling_party_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(calling_party_addresses_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [calling_party_addresses]");
                goto end;
            }
            OpenAPI_list_add(calling_party_addressesList, ogs_strdup(calling_party_addresses_local->valuestring));
        }
    }

    called_party_address = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "calledPartyAddress");
    if (called_party_address) {
    if (!cJSON_IsString(called_party_address) && !cJSON_IsNull(called_party_address)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [called_party_address]");
        goto end;
    }
    }

    number_portability_routinginformation = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "numberPortabilityRoutinginformation");
    if (number_portability_routinginformation) {
    if (!cJSON_IsString(number_portability_routinginformation) && !cJSON_IsNull(number_portability_routinginformation)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [number_portability_routinginformation]");
        goto end;
    }
    }

    carrier_select_routing_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "carrierSelectRoutingInformation");
    if (carrier_select_routing_information) {
    if (!cJSON_IsString(carrier_select_routing_information) && !cJSON_IsNull(carrier_select_routing_information)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [carrier_select_routing_information]");
        goto end;
    }
    }

    alternate_charged_party_address = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "alternateChargedPartyAddress");
    if (alternate_charged_party_address) {
    if (!cJSON_IsString(alternate_charged_party_address) && !cJSON_IsNull(alternate_charged_party_address)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [alternate_charged_party_address]");
        goto end;
    }
    }

    requested_party_address = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "requestedPartyAddress");
    if (requested_party_address) {
        cJSON *requested_party_address_local = NULL;
        if (!cJSON_IsArray(requested_party_address)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [requested_party_address]");
            goto end;
        }

        requested_party_addressList = OpenAPI_list_create();

        cJSON_ArrayForEach(requested_party_address_local, requested_party_address) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(requested_party_address_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [requested_party_address]");
                goto end;
            }
            OpenAPI_list_add(requested_party_addressList, ogs_strdup(requested_party_address_local->valuestring));
        }
    }

    called_asserted_identities = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "calledAssertedIdentities");
    if (called_asserted_identities) {
        cJSON *called_asserted_identities_local = NULL;
        if (!cJSON_IsArray(called_asserted_identities)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [called_asserted_identities]");
            goto end;
        }

        called_asserted_identitiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(called_asserted_identities_local, called_asserted_identities) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(called_asserted_identities_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [called_asserted_identities]");
                goto end;
            }
            OpenAPI_list_add(called_asserted_identitiesList, ogs_strdup(called_asserted_identities_local->valuestring));
        }
    }

    called_identity_changes = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "calledIdentityChanges");
    if (called_identity_changes) {
        cJSON *called_identity_changes_local = NULL;
        if (!cJSON_IsArray(called_identity_changes)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [called_identity_changes]");
            goto end;
        }

        called_identity_changesList = OpenAPI_list_create();

        cJSON_ArrayForEach(called_identity_changes_local, called_identity_changes) {
            if (!cJSON_IsObject(called_identity_changes_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [called_identity_changes]");
                goto end;
            }
            OpenAPI_called_identity_change_t *called_identity_changesItem = OpenAPI_called_identity_change_parseFromJSON(called_identity_changes_local);
            if (!called_identity_changesItem) {
                ogs_error("No called_identity_changesItem");
                goto end;
            }
            OpenAPI_list_add(called_identity_changesList, called_identity_changesItem);
        }
    }

    associated_uri = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "associatedURI");
    if (associated_uri) {
        cJSON *associated_uri_local = NULL;
        if (!cJSON_IsArray(associated_uri)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [associated_uri]");
            goto end;
        }

        associated_uriList = OpenAPI_list_create();

        cJSON_ArrayForEach(associated_uri_local, associated_uri) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(associated_uri_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [associated_uri]");
                goto end;
            }
            OpenAPI_list_add(associated_uriList, ogs_strdup(associated_uri_local->valuestring));
        }
    }

    time_stamps = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "timeStamps");
    if (time_stamps) {
    if (!cJSON_IsString(time_stamps) && !cJSON_IsNull(time_stamps)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [time_stamps]");
        goto end;
    }
    }

    application_server_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "applicationServerInformation");
    if (application_server_information) {
        cJSON *application_server_information_local = NULL;
        if (!cJSON_IsArray(application_server_information)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [application_server_information]");
            goto end;
        }

        application_server_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(application_server_information_local, application_server_information) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(application_server_information_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [application_server_information]");
                goto end;
            }
            OpenAPI_list_add(application_server_informationList, ogs_strdup(application_server_information_local->valuestring));
        }
    }

    inter_operator_identifier = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "interOperatorIdentifier");
    if (inter_operator_identifier) {
        cJSON *inter_operator_identifier_local = NULL;
        if (!cJSON_IsArray(inter_operator_identifier)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [inter_operator_identifier]");
            goto end;
        }

        inter_operator_identifierList = OpenAPI_list_create();

        cJSON_ArrayForEach(inter_operator_identifier_local, inter_operator_identifier) {
            if (!cJSON_IsObject(inter_operator_identifier_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [inter_operator_identifier]");
                goto end;
            }
            OpenAPI_inter_operator_identifier_t *inter_operator_identifierItem = OpenAPI_inter_operator_identifier_parseFromJSON(inter_operator_identifier_local);
            if (!inter_operator_identifierItem) {
                ogs_error("No inter_operator_identifierItem");
                goto end;
            }
            OpenAPI_list_add(inter_operator_identifierList, inter_operator_identifierItem);
        }
    }

    ims_charging_identifier = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "imsChargingIdentifier");
    if (ims_charging_identifier) {
    if (!cJSON_IsString(ims_charging_identifier) && !cJSON_IsNull(ims_charging_identifier)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [ims_charging_identifier]");
        goto end;
    }
    }

    related_icid = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "relatedICID");
    if (related_icid) {
    if (!cJSON_IsString(related_icid) && !cJSON_IsNull(related_icid)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [related_icid]");
        goto end;
    }
    }

    related_icid_generation_node = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "relatedICIDGenerationNode");
    if (related_icid_generation_node) {
    if (!cJSON_IsString(related_icid_generation_node) && !cJSON_IsNull(related_icid_generation_node)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [related_icid_generation_node]");
        goto end;
    }
    }

    transit_ioi_list = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "transitIOIList");
    if (transit_ioi_list) {
        cJSON *transit_ioi_list_local = NULL;
        if (!cJSON_IsArray(transit_ioi_list)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [transit_ioi_list]");
            goto end;
        }

        transit_ioi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(transit_ioi_list_local, transit_ioi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(transit_ioi_list_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [transit_ioi_list]");
                goto end;
            }
            OpenAPI_list_add(transit_ioi_listList, ogs_strdup(transit_ioi_list_local->valuestring));
        }
    }

    early_media_description = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "earlyMediaDescription");
    if (early_media_description) {
        cJSON *early_media_description_local = NULL;
        if (!cJSON_IsArray(early_media_description)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [early_media_description]");
            goto end;
        }

        early_media_descriptionList = OpenAPI_list_create();

        cJSON_ArrayForEach(early_media_description_local, early_media_description) {
            if (!cJSON_IsObject(early_media_description_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [early_media_description]");
                goto end;
            }
            OpenAPI_early_media_description_t *early_media_descriptionItem = OpenAPI_early_media_description_parseFromJSON(early_media_description_local);
            if (!early_media_descriptionItem) {
                ogs_error("No early_media_descriptionItem");
                goto end;
            }
            OpenAPI_list_add(early_media_descriptionList, early_media_descriptionItem);
        }
    }

    sdp_session_description = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "sdpSessionDescription");
    if (sdp_session_description) {
        cJSON *sdp_session_description_local = NULL;
        if (!cJSON_IsArray(sdp_session_description)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [sdp_session_description]");
            goto end;
        }

        sdp_session_descriptionList = OpenAPI_list_create();

        cJSON_ArrayForEach(sdp_session_description_local, sdp_session_description) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(sdp_session_description_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [sdp_session_description]");
                goto end;
            }
            OpenAPI_list_add(sdp_session_descriptionList, ogs_strdup(sdp_session_description_local->valuestring));
        }
    }

    sdp_media_component = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "sdpMediaComponent");
    if (sdp_media_component) {
        cJSON *sdp_media_component_local = NULL;
        if (!cJSON_IsArray(sdp_media_component)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [sdp_media_component]");
            goto end;
        }

        sdp_media_componentList = OpenAPI_list_create();

        cJSON_ArrayForEach(sdp_media_component_local, sdp_media_component) {
            if (!cJSON_IsObject(sdp_media_component_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [sdp_media_component]");
                goto end;
            }
            OpenAPI_sdp_media_component_t *sdp_media_componentItem = OpenAPI_sdp_media_component_parseFromJSON(sdp_media_component_local);
            if (!sdp_media_componentItem) {
                ogs_error("No sdp_media_componentItem");
                goto end;
            }
            OpenAPI_list_add(sdp_media_componentList, sdp_media_componentItem);
        }
    }

    served_party_ip_address = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "servedPartyIPAddress");
    if (served_party_ip_address) {
    if (!cJSON_IsNull(served_party_ip_address)) {
    served_party_ip_address_local_nonprim = OpenAPI_ims_address_parseFromJSON(served_party_ip_address);
    if (!served_party_ip_address_local_nonprim) {
        ogs_error("OpenAPI_ims_address_parseFromJSON failed [served_party_ip_address]");
        goto end;
    }
    }
    }

    server_capabilities = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "serverCapabilities");
    if (server_capabilities) {
    server_capabilities_local_nonprim = OpenAPI_server_capabilities_parseFromJSON(server_capabilities);
    if (!server_capabilities_local_nonprim) {
        ogs_error("OpenAPI_server_capabilities_parseFromJSON failed [server_capabilities]");
        goto end;
    }
    }

    trunk_group_id = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "trunkGroupID");
    if (trunk_group_id) {
    trunk_group_id_local_nonprim = OpenAPI_trunk_group_id_parseFromJSON(trunk_group_id);
    if (!trunk_group_id_local_nonprim) {
        ogs_error("OpenAPI_trunk_group_id_parseFromJSON failed [trunk_group_id]");
        goto end;
    }
    }

    bearer_service = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "bearerService");
    if (bearer_service) {
    if (!cJSON_IsString(bearer_service) && !cJSON_IsNull(bearer_service)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [bearer_service]");
        goto end;
    }
    }

    ims_service_id = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "imsServiceId");
    if (ims_service_id) {
    if (!cJSON_IsString(ims_service_id) && !cJSON_IsNull(ims_service_id)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [ims_service_id]");
        goto end;
    }
    }

    message_bodies = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "messageBodies");
    if (message_bodies) {
        cJSON *message_bodies_local = NULL;
        if (!cJSON_IsArray(message_bodies)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [message_bodies]");
            goto end;
        }

        message_bodiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(message_bodies_local, message_bodies) {
            if (!cJSON_IsObject(message_bodies_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [message_bodies]");
                goto end;
            }
            OpenAPI_message_body_t *message_bodiesItem = OpenAPI_message_body_parseFromJSON(message_bodies_local);
            if (!message_bodiesItem) {
                ogs_error("No message_bodiesItem");
                goto end;
            }
            OpenAPI_list_add(message_bodiesList, message_bodiesItem);
        }
    }

    access_network_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "accessNetworkInformation");
    if (access_network_information) {
        cJSON *access_network_information_local = NULL;
        if (!cJSON_IsArray(access_network_information)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [access_network_information]");
            goto end;
        }

        access_network_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_network_information_local, access_network_information) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(access_network_information_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [access_network_information]");
                goto end;
            }
            OpenAPI_list_add(access_network_informationList, ogs_strdup(access_network_information_local->valuestring));
        }
    }

    additional_access_network_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "additionalAccessNetworkInformation");
    if (additional_access_network_information) {
    if (!cJSON_IsString(additional_access_network_information) && !cJSON_IsNull(additional_access_network_information)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [additional_access_network_information]");
        goto end;
    }
    }

    cellular_network_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "cellularNetworkInformation");
    if (cellular_network_information) {
    if (!cJSON_IsString(cellular_network_information) && !cJSON_IsNull(cellular_network_information)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [cellular_network_information]");
        goto end;
    }
    }

    access_transfer_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "accessTransferInformation");
    if (access_transfer_information) {
        cJSON *access_transfer_information_local = NULL;
        if (!cJSON_IsArray(access_transfer_information)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [access_transfer_information]");
            goto end;
        }

        access_transfer_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_transfer_information_local, access_transfer_information) {
            if (!cJSON_IsObject(access_transfer_information_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [access_transfer_information]");
                goto end;
            }
            OpenAPI_access_transfer_information_t *access_transfer_informationItem = OpenAPI_access_transfer_information_parseFromJSON(access_transfer_information_local);
            if (!access_transfer_informationItem) {
                ogs_error("No access_transfer_informationItem");
                goto end;
            }
            OpenAPI_list_add(access_transfer_informationList, access_transfer_informationItem);
        }
    }

    access_network_info_change = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "accessNetworkInfoChange");
    if (access_network_info_change) {
        cJSON *access_network_info_change_local = NULL;
        if (!cJSON_IsArray(access_network_info_change)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [access_network_info_change]");
            goto end;
        }

        access_network_info_changeList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_network_info_change_local, access_network_info_change) {
            if (!cJSON_IsObject(access_network_info_change_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [access_network_info_change]");
                goto end;
            }
            OpenAPI_access_network_info_change_t *access_network_info_changeItem = OpenAPI_access_network_info_change_parseFromJSON(access_network_info_change_local);
            if (!access_network_info_changeItem) {
                ogs_error("No access_network_info_changeItem");
                goto end;
            }
            OpenAPI_list_add(access_network_info_changeList, access_network_info_changeItem);
        }
    }

    ims_communication_service_id = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "imsCommunicationServiceID");
    if (ims_communication_service_id) {
    if (!cJSON_IsString(ims_communication_service_id) && !cJSON_IsNull(ims_communication_service_id)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [ims_communication_service_id]");
        goto end;
    }
    }

    ims_application_reference_id = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "imsApplicationReferenceID");
    if (ims_application_reference_id) {
    if (!cJSON_IsString(ims_application_reference_id) && !cJSON_IsNull(ims_application_reference_id)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [ims_application_reference_id]");
        goto end;
    }
    }

    cause_code = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "causeCode");
    if (cause_code) {
    if (!cJSON_IsNumber(cause_code)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [cause_code]");
        goto end;
    }
    }

    reason_header = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "reasonHeader");
    if (reason_header) {
        cJSON *reason_header_local = NULL;
        if (!cJSON_IsArray(reason_header)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [reason_header]");
            goto end;
        }

        reason_headerList = OpenAPI_list_create();

        cJSON_ArrayForEach(reason_header_local, reason_header) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reason_header_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [reason_header]");
                goto end;
            }
            OpenAPI_list_add(reason_headerList, ogs_strdup(reason_header_local->valuestring));
        }
    }

    initial_ims_charging_identifier = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "initialIMSChargingIdentifier");
    if (initial_ims_charging_identifier) {
    if (!cJSON_IsString(initial_ims_charging_identifier) && !cJSON_IsNull(initial_ims_charging_identifier)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [initial_ims_charging_identifier]");
        goto end;
    }
    }

    nni_information = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "nniInformation");
    if (nni_information) {
        cJSON *nni_information_local = NULL;
        if (!cJSON_IsArray(nni_information)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [nni_information]");
            goto end;
        }

        nni_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(nni_information_local, nni_information) {
            if (!cJSON_IsObject(nni_information_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [nni_information]");
                goto end;
            }
            OpenAPI_nni_information_t *nni_informationItem = OpenAPI_nni_information_parseFromJSON(nni_information_local);
            if (!nni_informationItem) {
                ogs_error("No nni_informationItem");
                goto end;
            }
            OpenAPI_list_add(nni_informationList, nni_informationItem);
        }
    }

    from_address = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "fromAddress");
    if (from_address) {
    if (!cJSON_IsString(from_address) && !cJSON_IsNull(from_address)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [from_address]");
        goto end;
    }
    }

    ims_emergency_indication = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "imsEmergencyIndication");
    if (ims_emergency_indication) {
    if (!cJSON_IsBool(ims_emergency_indication)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [ims_emergency_indication]");
        goto end;
    }
    }

    ims_visited_network_identifier = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "imsVisitedNetworkIdentifier");
    if (ims_visited_network_identifier) {
    if (!cJSON_IsString(ims_visited_network_identifier) && !cJSON_IsNull(ims_visited_network_identifier)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [ims_visited_network_identifier]");
        goto end;
    }
    }

    sip_route_header_received = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "sipRouteHeaderReceived");
    if (sip_route_header_received) {
    if (!cJSON_IsString(sip_route_header_received) && !cJSON_IsNull(sip_route_header_received)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [sip_route_header_received]");
        goto end;
    }
    }

    sip_route_header_transmitted = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "sipRouteHeaderTransmitted");
    if (sip_route_header_transmitted) {
    if (!cJSON_IsString(sip_route_header_transmitted) && !cJSON_IsNull(sip_route_header_transmitted)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [sip_route_header_transmitted]");
        goto end;
    }
    }

    tad_identifier = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "tadIdentifier");
    if (tad_identifier) {
    if (!cJSON_IsString(tad_identifier)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [tad_identifier]");
        goto end;
    }
    tad_identifierVariable = OpenAPI_tad_identifier_FromString(tad_identifier->valuestring);
    }

    fe_identifier_list = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "feIdentifierList");
    if (fe_identifier_list) {
    if (!cJSON_IsString(fe_identifier_list) && !cJSON_IsNull(fe_identifier_list)) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [fe_identifier_list]");
        goto end;
    }
    }

    ims_dc_app_info = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "imsDCAppInfo");
    if (ims_dc_app_info) {
    ims_dc_app_info_local_nonprim = OpenAPI_imsdc_app_info_parseFromJSON(ims_dc_app_info);
    if (!ims_dc_app_info_local_nonprim) {
        ogs_error("OpenAPI_imsdc_app_info_parseFromJSON failed [ims_dc_app_info]");
        goto end;
    }
    }

    satellite_id_list = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "satelliteIdList");
    if (satellite_id_list) {
        cJSON *satellite_id_list_local = NULL;
        if (!cJSON_IsArray(satellite_id_list)) {
            ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [satellite_id_list]");
            goto end;
        }

        satellite_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(satellite_id_list_local, satellite_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(satellite_id_list_local)) {
                ogs_error("OpenAPI_ims_charging_information_parseFromJSON() failed [satellite_id_list]");
                goto end;
            }
            OpenAPI_list_add(satellite_id_listList, ogs_strdup(satellite_id_list_local->valuestring));
        }
    }

    media_resource = cJSON_GetObjectItemCaseSensitive(ims_charging_informationJSON, "mediaResource");
    if (media_resource) {
    media_resource_local_nonprim = OpenAPI_media_resource_parseFromJSON(media_resource);
    if (!media_resource_local_nonprim) {
        ogs_error("OpenAPI_media_resource_parseFromJSON failed [media_resource]");
        goto end;
    }
    }

    ims_charging_information_local_var = OpenAPI_ims_charging_information_create (
        event_type ? event_type_local_nonprim : NULL,
        i_ms_node_functionality ? i_ms_node_functionalityVariable : 0,
        role_of_node ? role_of_nodeVariable : 0,
        user_information ? user_information_local_nonprim : NULL,
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        _3gpp_ps_data_off_status ? _3gpp_ps_data_off_statusVariable : 0,
        isup_cause ? isup_cause_local_nonprim : NULL,
        control_plane_address && cJSON_IsNull(control_plane_address) ? true : false,
        control_plane_address ? control_plane_address_local_nonprim : NULL,
        vlr_number && !cJSON_IsNull(vlr_number) ? ogs_strdup(vlr_number->valuestring) : NULL,
        msc_address && !cJSON_IsNull(msc_address) ? ogs_strdup(msc_address->valuestring) : NULL,
        user_session_id && !cJSON_IsNull(user_session_id) ? ogs_strdup(user_session_id->valuestring) : NULL,
        outgoing_session_id && !cJSON_IsNull(outgoing_session_id) ? ogs_strdup(outgoing_session_id->valuestring) : NULL,
        session_priority ? session_priorityVariable : 0,
        calling_party_addresses ? calling_party_addressesList : NULL,
        called_party_address && !cJSON_IsNull(called_party_address) ? ogs_strdup(called_party_address->valuestring) : NULL,
        number_portability_routinginformation && !cJSON_IsNull(number_portability_routinginformation) ? ogs_strdup(number_portability_routinginformation->valuestring) : NULL,
        carrier_select_routing_information && !cJSON_IsNull(carrier_select_routing_information) ? ogs_strdup(carrier_select_routing_information->valuestring) : NULL,
        alternate_charged_party_address && !cJSON_IsNull(alternate_charged_party_address) ? ogs_strdup(alternate_charged_party_address->valuestring) : NULL,
        requested_party_address ? requested_party_addressList : NULL,
        called_asserted_identities ? called_asserted_identitiesList : NULL,
        called_identity_changes ? called_identity_changesList : NULL,
        associated_uri ? associated_uriList : NULL,
        time_stamps && !cJSON_IsNull(time_stamps) ? ogs_strdup(time_stamps->valuestring) : NULL,
        application_server_information ? application_server_informationList : NULL,
        inter_operator_identifier ? inter_operator_identifierList : NULL,
        ims_charging_identifier && !cJSON_IsNull(ims_charging_identifier) ? ogs_strdup(ims_charging_identifier->valuestring) : NULL,
        related_icid && !cJSON_IsNull(related_icid) ? ogs_strdup(related_icid->valuestring) : NULL,
        related_icid_generation_node && !cJSON_IsNull(related_icid_generation_node) ? ogs_strdup(related_icid_generation_node->valuestring) : NULL,
        transit_ioi_list ? transit_ioi_listList : NULL,
        early_media_description ? early_media_descriptionList : NULL,
        sdp_session_description ? sdp_session_descriptionList : NULL,
        sdp_media_component ? sdp_media_componentList : NULL,
        served_party_ip_address && cJSON_IsNull(served_party_ip_address) ? true : false,
        served_party_ip_address ? served_party_ip_address_local_nonprim : NULL,
        server_capabilities ? server_capabilities_local_nonprim : NULL,
        trunk_group_id ? trunk_group_id_local_nonprim : NULL,
        bearer_service && !cJSON_IsNull(bearer_service) ? ogs_strdup(bearer_service->valuestring) : NULL,
        ims_service_id && !cJSON_IsNull(ims_service_id) ? ogs_strdup(ims_service_id->valuestring) : NULL,
        message_bodies ? message_bodiesList : NULL,
        access_network_information ? access_network_informationList : NULL,
        additional_access_network_information && !cJSON_IsNull(additional_access_network_information) ? ogs_strdup(additional_access_network_information->valuestring) : NULL,
        cellular_network_information && !cJSON_IsNull(cellular_network_information) ? ogs_strdup(cellular_network_information->valuestring) : NULL,
        access_transfer_information ? access_transfer_informationList : NULL,
        access_network_info_change ? access_network_info_changeList : NULL,
        ims_communication_service_id && !cJSON_IsNull(ims_communication_service_id) ? ogs_strdup(ims_communication_service_id->valuestring) : NULL,
        ims_application_reference_id && !cJSON_IsNull(ims_application_reference_id) ? ogs_strdup(ims_application_reference_id->valuestring) : NULL,
        cause_code ? true : false,
        cause_code ? cause_code->valuedouble : 0,
        reason_header ? reason_headerList : NULL,
        initial_ims_charging_identifier && !cJSON_IsNull(initial_ims_charging_identifier) ? ogs_strdup(initial_ims_charging_identifier->valuestring) : NULL,
        nni_information ? nni_informationList : NULL,
        from_address && !cJSON_IsNull(from_address) ? ogs_strdup(from_address->valuestring) : NULL,
        ims_emergency_indication ? true : false,
        ims_emergency_indication ? ims_emergency_indication->valueint : 0,
        ims_visited_network_identifier && !cJSON_IsNull(ims_visited_network_identifier) ? ogs_strdup(ims_visited_network_identifier->valuestring) : NULL,
        sip_route_header_received && !cJSON_IsNull(sip_route_header_received) ? ogs_strdup(sip_route_header_received->valuestring) : NULL,
        sip_route_header_transmitted && !cJSON_IsNull(sip_route_header_transmitted) ? ogs_strdup(sip_route_header_transmitted->valuestring) : NULL,
        tad_identifier ? tad_identifierVariable : 0,
        fe_identifier_list && !cJSON_IsNull(fe_identifier_list) ? ogs_strdup(fe_identifier_list->valuestring) : NULL,
        ims_dc_app_info ? ims_dc_app_info_local_nonprim : NULL,
        satellite_id_list ? satellite_id_listList : NULL,
        media_resource ? media_resource_local_nonprim : NULL
    );

    return ims_charging_information_local_var;
end:
    if (event_type_local_nonprim) {
        OpenAPI_sip_event_type_free(event_type_local_nonprim);
        event_type_local_nonprim = NULL;
    }
    if (user_information_local_nonprim) {
        OpenAPI_user_information_free(user_information_local_nonprim);
        user_information_local_nonprim = NULL;
    }
    if (user_location_info_local_nonprim) {
        OpenAPI_charging_user_location_free(user_location_info_local_nonprim);
        user_location_info_local_nonprim = NULL;
    }
    if (isup_cause_local_nonprim) {
        OpenAPI_isup_cause_free(isup_cause_local_nonprim);
        isup_cause_local_nonprim = NULL;
    }
    if (control_plane_address_local_nonprim) {
        OpenAPI_ims_address_free(control_plane_address_local_nonprim);
        control_plane_address_local_nonprim = NULL;
    }
    if (calling_party_addressesList) {
        OpenAPI_list_for_each(calling_party_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(calling_party_addressesList);
        calling_party_addressesList = NULL;
    }
    if (requested_party_addressList) {
        OpenAPI_list_for_each(requested_party_addressList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(requested_party_addressList);
        requested_party_addressList = NULL;
    }
    if (called_asserted_identitiesList) {
        OpenAPI_list_for_each(called_asserted_identitiesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(called_asserted_identitiesList);
        called_asserted_identitiesList = NULL;
    }
    if (called_identity_changesList) {
        OpenAPI_list_for_each(called_identity_changesList, node) {
            OpenAPI_called_identity_change_free(node->data);
        }
        OpenAPI_list_free(called_identity_changesList);
        called_identity_changesList = NULL;
    }
    if (associated_uriList) {
        OpenAPI_list_for_each(associated_uriList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(associated_uriList);
        associated_uriList = NULL;
    }
    if (application_server_informationList) {
        OpenAPI_list_for_each(application_server_informationList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(application_server_informationList);
        application_server_informationList = NULL;
    }
    if (inter_operator_identifierList) {
        OpenAPI_list_for_each(inter_operator_identifierList, node) {
            OpenAPI_inter_operator_identifier_free(node->data);
        }
        OpenAPI_list_free(inter_operator_identifierList);
        inter_operator_identifierList = NULL;
    }
    if (transit_ioi_listList) {
        OpenAPI_list_for_each(transit_ioi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(transit_ioi_listList);
        transit_ioi_listList = NULL;
    }
    if (early_media_descriptionList) {
        OpenAPI_list_for_each(early_media_descriptionList, node) {
            OpenAPI_early_media_description_free(node->data);
        }
        OpenAPI_list_free(early_media_descriptionList);
        early_media_descriptionList = NULL;
    }
    if (sdp_session_descriptionList) {
        OpenAPI_list_for_each(sdp_session_descriptionList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sdp_session_descriptionList);
        sdp_session_descriptionList = NULL;
    }
    if (sdp_media_componentList) {
        OpenAPI_list_for_each(sdp_media_componentList, node) {
            OpenAPI_sdp_media_component_free(node->data);
        }
        OpenAPI_list_free(sdp_media_componentList);
        sdp_media_componentList = NULL;
    }
    if (served_party_ip_address_local_nonprim) {
        OpenAPI_ims_address_free(served_party_ip_address_local_nonprim);
        served_party_ip_address_local_nonprim = NULL;
    }
    if (server_capabilities_local_nonprim) {
        OpenAPI_server_capabilities_free(server_capabilities_local_nonprim);
        server_capabilities_local_nonprim = NULL;
    }
    if (trunk_group_id_local_nonprim) {
        OpenAPI_trunk_group_id_free(trunk_group_id_local_nonprim);
        trunk_group_id_local_nonprim = NULL;
    }
    if (message_bodiesList) {
        OpenAPI_list_for_each(message_bodiesList, node) {
            OpenAPI_message_body_free(node->data);
        }
        OpenAPI_list_free(message_bodiesList);
        message_bodiesList = NULL;
    }
    if (access_network_informationList) {
        OpenAPI_list_for_each(access_network_informationList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_network_informationList);
        access_network_informationList = NULL;
    }
    if (access_transfer_informationList) {
        OpenAPI_list_for_each(access_transfer_informationList, node) {
            OpenAPI_access_transfer_information_free(node->data);
        }
        OpenAPI_list_free(access_transfer_informationList);
        access_transfer_informationList = NULL;
    }
    if (access_network_info_changeList) {
        OpenAPI_list_for_each(access_network_info_changeList, node) {
            OpenAPI_access_network_info_change_free(node->data);
        }
        OpenAPI_list_free(access_network_info_changeList);
        access_network_info_changeList = NULL;
    }
    if (reason_headerList) {
        OpenAPI_list_for_each(reason_headerList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reason_headerList);
        reason_headerList = NULL;
    }
    if (nni_informationList) {
        OpenAPI_list_for_each(nni_informationList, node) {
            OpenAPI_nni_information_free(node->data);
        }
        OpenAPI_list_free(nni_informationList);
        nni_informationList = NULL;
    }
    if (ims_dc_app_info_local_nonprim) {
        OpenAPI_imsdc_app_info_free(ims_dc_app_info_local_nonprim);
        ims_dc_app_info_local_nonprim = NULL;
    }
    if (satellite_id_listList) {
        OpenAPI_list_for_each(satellite_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(satellite_id_listList);
        satellite_id_listList = NULL;
    }
    if (media_resource_local_nonprim) {
        OpenAPI_media_resource_free(media_resource_local_nonprim);
        media_resource_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ims_charging_information_t *OpenAPI_ims_charging_information_copy(OpenAPI_ims_charging_information_t *dst, OpenAPI_ims_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ims_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ims_charging_information_convertToJSON() failed");
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

    OpenAPI_ims_charging_information_free(dst);
    dst = OpenAPI_ims_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

