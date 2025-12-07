
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "input_data.h"

OpenAPI_input_data_t *OpenAPI_input_data_create(
    OpenAPI_external_client_type_t *external_client_type,
    char *correlation_id,
    char *amf_id,
    OpenAPI_location_qo_s_t *location_qo_s,
    OpenAPI_list_t *supported_gad_shapes,
    char *supi,
    char *pei,
    char *gpsi,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_ecgi_t *ecgi_on_second_node,
    OpenAPI_ncgi_t *ncgi,
    OpenAPI_ncgi_t *ncgi_on_second_node,
    OpenAPI_lcs_priority_t *priority,
    OpenAPI_velocity_requested_t *velocity_requested,
    OpenAPI_ue_lcs_capability_t *ue_lcs_cap,
    bool is_lcs_service_type,
    int lcs_service_type,
    OpenAPI_ldr_type_t *ldr_type,
    char *hgmlc_call_back_uri,
    char *vgmlc_address,
    char *ldr_reference,
    OpenAPI_periodic_event_info_t *periodic_event_info,
    OpenAPI_area_event_info_t *area_event_info,
    OpenAPI_motion_event_info_t *motion_event_info,
    OpenAPI_list_t *reporting_access_types,
    OpenAPI_ue_connectivity_state_t *ue_connectivity_states,
    OpenAPI_ue_location_service_ind_t *ue_location_service_ind,
    OpenAPI_lcs_broadcast_assistance_types_data_t *mo_assistance_data_types,
    OpenAPI_ref_to_binary_data_t *lpp_message,
    OpenAPI_list_t *lpp_message_ext,
    char *supported_features,
    char *ue_positioning_cap,
    OpenAPI_tnap_id_t *tnap_id,
    OpenAPI_twap_id_t *twap_id,
    bool is_ue_country_det_ind,
    int ue_country_det_ind,
    char *scheduled_loc_time,
    bool is_reliable_loc_req,
    int reliable_loc_req
)
{
    OpenAPI_input_data_t *input_data_local_var = ogs_malloc(sizeof(OpenAPI_input_data_t));
    ogs_assert(input_data_local_var);

    input_data_local_var->external_client_type = external_client_type;
    input_data_local_var->correlation_id = correlation_id;
    input_data_local_var->amf_id = amf_id;
    input_data_local_var->location_qo_s = location_qo_s;
    input_data_local_var->supported_gad_shapes = supported_gad_shapes;
    input_data_local_var->supi = supi;
    input_data_local_var->pei = pei;
    input_data_local_var->gpsi = gpsi;
    input_data_local_var->ecgi = ecgi;
    input_data_local_var->ecgi_on_second_node = ecgi_on_second_node;
    input_data_local_var->ncgi = ncgi;
    input_data_local_var->ncgi_on_second_node = ncgi_on_second_node;
    input_data_local_var->priority = priority;
    input_data_local_var->velocity_requested = velocity_requested;
    input_data_local_var->ue_lcs_cap = ue_lcs_cap;
    input_data_local_var->is_lcs_service_type = is_lcs_service_type;
    input_data_local_var->lcs_service_type = lcs_service_type;
    input_data_local_var->ldr_type = ldr_type;
    input_data_local_var->hgmlc_call_back_uri = hgmlc_call_back_uri;
    input_data_local_var->vgmlc_address = vgmlc_address;
    input_data_local_var->ldr_reference = ldr_reference;
    input_data_local_var->periodic_event_info = periodic_event_info;
    input_data_local_var->area_event_info = area_event_info;
    input_data_local_var->motion_event_info = motion_event_info;
    input_data_local_var->reporting_access_types = reporting_access_types;
    input_data_local_var->ue_connectivity_states = ue_connectivity_states;
    input_data_local_var->ue_location_service_ind = ue_location_service_ind;
    input_data_local_var->mo_assistance_data_types = mo_assistance_data_types;
    input_data_local_var->lpp_message = lpp_message;
    input_data_local_var->lpp_message_ext = lpp_message_ext;
    input_data_local_var->supported_features = supported_features;
    input_data_local_var->ue_positioning_cap = ue_positioning_cap;
    input_data_local_var->tnap_id = tnap_id;
    input_data_local_var->twap_id = twap_id;
    input_data_local_var->is_ue_country_det_ind = is_ue_country_det_ind;
    input_data_local_var->ue_country_det_ind = ue_country_det_ind;
    input_data_local_var->scheduled_loc_time = scheduled_loc_time;
    input_data_local_var->is_reliable_loc_req = is_reliable_loc_req;
    input_data_local_var->reliable_loc_req = reliable_loc_req;

    return input_data_local_var;
}

void OpenAPI_input_data_free(OpenAPI_input_data_t *input_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == input_data) {
        return;
    }
    if (input_data->external_client_type) {
        OpenAPI_external_client_type_free(input_data->external_client_type);
        input_data->external_client_type = NULL;
    }
    if (input_data->correlation_id) {
        ogs_free(input_data->correlation_id);
        input_data->correlation_id = NULL;
    }
    if (input_data->amf_id) {
        ogs_free(input_data->amf_id);
        input_data->amf_id = NULL;
    }
    if (input_data->location_qo_s) {
        OpenAPI_location_qo_s_free(input_data->location_qo_s);
        input_data->location_qo_s = NULL;
    }
    if (input_data->supported_gad_shapes) {
        OpenAPI_list_for_each(input_data->supported_gad_shapes, node) {
            OpenAPI_supported_gad_shapes_free(node->data);
        }
        OpenAPI_list_free(input_data->supported_gad_shapes);
        input_data->supported_gad_shapes = NULL;
    }
    if (input_data->supi) {
        ogs_free(input_data->supi);
        input_data->supi = NULL;
    }
    if (input_data->pei) {
        ogs_free(input_data->pei);
        input_data->pei = NULL;
    }
    if (input_data->gpsi) {
        ogs_free(input_data->gpsi);
        input_data->gpsi = NULL;
    }
    if (input_data->ecgi) {
        OpenAPI_ecgi_free(input_data->ecgi);
        input_data->ecgi = NULL;
    }
    if (input_data->ecgi_on_second_node) {
        OpenAPI_ecgi_free(input_data->ecgi_on_second_node);
        input_data->ecgi_on_second_node = NULL;
    }
    if (input_data->ncgi) {
        OpenAPI_ncgi_free(input_data->ncgi);
        input_data->ncgi = NULL;
    }
    if (input_data->ncgi_on_second_node) {
        OpenAPI_ncgi_free(input_data->ncgi_on_second_node);
        input_data->ncgi_on_second_node = NULL;
    }
    if (input_data->priority) {
        OpenAPI_lcs_priority_free(input_data->priority);
        input_data->priority = NULL;
    }
    if (input_data->velocity_requested) {
        OpenAPI_velocity_requested_free(input_data->velocity_requested);
        input_data->velocity_requested = NULL;
    }
    if (input_data->ue_lcs_cap) {
        OpenAPI_ue_lcs_capability_free(input_data->ue_lcs_cap);
        input_data->ue_lcs_cap = NULL;
    }
    if (input_data->ldr_type) {
        OpenAPI_ldr_type_free(input_data->ldr_type);
        input_data->ldr_type = NULL;
    }
    if (input_data->hgmlc_call_back_uri) {
        ogs_free(input_data->hgmlc_call_back_uri);
        input_data->hgmlc_call_back_uri = NULL;
    }
    if (input_data->vgmlc_address) {
        ogs_free(input_data->vgmlc_address);
        input_data->vgmlc_address = NULL;
    }
    if (input_data->ldr_reference) {
        ogs_free(input_data->ldr_reference);
        input_data->ldr_reference = NULL;
    }
    if (input_data->periodic_event_info) {
        OpenAPI_periodic_event_info_free(input_data->periodic_event_info);
        input_data->periodic_event_info = NULL;
    }
    if (input_data->area_event_info) {
        OpenAPI_area_event_info_free(input_data->area_event_info);
        input_data->area_event_info = NULL;
    }
    if (input_data->motion_event_info) {
        OpenAPI_motion_event_info_free(input_data->motion_event_info);
        input_data->motion_event_info = NULL;
    }
    if (input_data->reporting_access_types) {
        OpenAPI_list_for_each(input_data->reporting_access_types, node) {
            OpenAPI_reporting_access_type_free(node->data);
        }
        OpenAPI_list_free(input_data->reporting_access_types);
        input_data->reporting_access_types = NULL;
    }
    if (input_data->ue_connectivity_states) {
        OpenAPI_ue_connectivity_state_free(input_data->ue_connectivity_states);
        input_data->ue_connectivity_states = NULL;
    }
    if (input_data->ue_location_service_ind) {
        OpenAPI_ue_location_service_ind_free(input_data->ue_location_service_ind);
        input_data->ue_location_service_ind = NULL;
    }
    if (input_data->mo_assistance_data_types) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(input_data->mo_assistance_data_types);
        input_data->mo_assistance_data_types = NULL;
    }
    if (input_data->lpp_message) {
        OpenAPI_ref_to_binary_data_free(input_data->lpp_message);
        input_data->lpp_message = NULL;
    }
    if (input_data->lpp_message_ext) {
        OpenAPI_list_for_each(input_data->lpp_message_ext, node) {
            OpenAPI_ref_to_binary_data_free(node->data);
        }
        OpenAPI_list_free(input_data->lpp_message_ext);
        input_data->lpp_message_ext = NULL;
    }
    if (input_data->supported_features) {
        ogs_free(input_data->supported_features);
        input_data->supported_features = NULL;
    }
    if (input_data->ue_positioning_cap) {
        ogs_free(input_data->ue_positioning_cap);
        input_data->ue_positioning_cap = NULL;
    }
    if (input_data->tnap_id) {
        OpenAPI_tnap_id_free(input_data->tnap_id);
        input_data->tnap_id = NULL;
    }
    if (input_data->twap_id) {
        OpenAPI_twap_id_free(input_data->twap_id);
        input_data->twap_id = NULL;
    }
    if (input_data->scheduled_loc_time) {
        ogs_free(input_data->scheduled_loc_time);
        input_data->scheduled_loc_time = NULL;
    }
    ogs_free(input_data);
}

cJSON *OpenAPI_input_data_convertToJSON(OpenAPI_input_data_t *input_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (input_data == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [InputData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (input_data->external_client_type) {
    cJSON *external_client_type_local_JSON = OpenAPI_external_client_type_convertToJSON(input_data->external_client_type);
    if (external_client_type_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [external_client_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "externalClientType", external_client_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [external_client_type]");
        goto end;
    }
    }

    if (input_data->correlation_id) {
    if (cJSON_AddStringToObject(item, "correlationID", input_data->correlation_id) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [correlation_id]");
        goto end;
    }
    }

    if (input_data->amf_id) {
    if (cJSON_AddStringToObject(item, "amfId", input_data->amf_id) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [amf_id]");
        goto end;
    }
    }

    if (input_data->location_qo_s) {
    cJSON *location_qo_s_local_JSON = OpenAPI_location_qo_s_convertToJSON(input_data->location_qo_s);
    if (location_qo_s_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [location_qo_s]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationQoS", location_qo_s_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [location_qo_s]");
        goto end;
    }
    }

    if (input_data->supported_gad_shapes) {
    cJSON *supported_gad_shapesList = cJSON_AddArrayToObject(item, "supportedGADShapes");
    if (supported_gad_shapesList == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [supported_gad_shapes]");
        goto end;
    }
    OpenAPI_list_for_each(input_data->supported_gad_shapes, node) {
        cJSON *itemLocal = OpenAPI_supported_gad_shapes_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_input_data_convertToJSON() failed [supported_gad_shapes]");
            goto end;
        }
        cJSON_AddItemToArray(supported_gad_shapesList, itemLocal);
    }
    }

    if (input_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", input_data->supi) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (input_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", input_data->pei) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (input_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", input_data->gpsi) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (input_data->ecgi) {
    cJSON *ecgi_local_JSON = OpenAPI_ecgi_convertToJSON(input_data->ecgi);
    if (ecgi_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ecgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgi", ecgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ecgi]");
        goto end;
    }
    }

    if (input_data->ecgi_on_second_node) {
    cJSON *ecgi_on_second_node_local_JSON = OpenAPI_ecgi_convertToJSON(input_data->ecgi_on_second_node);
    if (ecgi_on_second_node_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ecgi_on_second_node]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgiOnSecondNode", ecgi_on_second_node_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ecgi_on_second_node]");
        goto end;
    }
    }

    if (input_data->ncgi) {
    cJSON *ncgi_local_JSON = OpenAPI_ncgi_convertToJSON(input_data->ncgi);
    if (ncgi_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ncgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ncgi", ncgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ncgi]");
        goto end;
    }
    }

    if (input_data->ncgi_on_second_node) {
    cJSON *ncgi_on_second_node_local_JSON = OpenAPI_ncgi_convertToJSON(input_data->ncgi_on_second_node);
    if (ncgi_on_second_node_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ncgi_on_second_node]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ncgiOnSecondNode", ncgi_on_second_node_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ncgi_on_second_node]");
        goto end;
    }
    }

    if (input_data->priority) {
    cJSON *priority_local_JSON = OpenAPI_lcs_priority_convertToJSON(input_data->priority);
    if (priority_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [priority]");
        goto end;
    }
    cJSON_AddItemToObject(item, "priority", priority_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (input_data->velocity_requested) {
    cJSON *velocity_requested_local_JSON = OpenAPI_velocity_requested_convertToJSON(input_data->velocity_requested);
    if (velocity_requested_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [velocity_requested]");
        goto end;
    }
    cJSON_AddItemToObject(item, "velocityRequested", velocity_requested_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [velocity_requested]");
        goto end;
    }
    }

    if (input_data->ue_lcs_cap) {
    cJSON *ue_lcs_cap_local_JSON = OpenAPI_ue_lcs_capability_convertToJSON(input_data->ue_lcs_cap);
    if (ue_lcs_cap_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_lcs_cap]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLcsCap", ue_lcs_cap_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_lcs_cap]");
        goto end;
    }
    }

    if (input_data->is_lcs_service_type) {
    if (cJSON_AddNumberToObject(item, "lcsServiceType", input_data->lcs_service_type) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [lcs_service_type]");
        goto end;
    }
    }

    if (input_data->ldr_type) {
    cJSON *ldr_type_local_JSON = OpenAPI_ldr_type_convertToJSON(input_data->ldr_type);
    if (ldr_type_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ldr_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ldrType", ldr_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ldr_type]");
        goto end;
    }
    }

    if (input_data->hgmlc_call_back_uri) {
    if (cJSON_AddStringToObject(item, "hgmlcCallBackURI", input_data->hgmlc_call_back_uri) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [hgmlc_call_back_uri]");
        goto end;
    }
    }

    if (input_data->vgmlc_address) {
    if (cJSON_AddStringToObject(item, "vgmlcAddress", input_data->vgmlc_address) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [vgmlc_address]");
        goto end;
    }
    }

    if (input_data->ldr_reference) {
    if (cJSON_AddStringToObject(item, "ldrReference", input_data->ldr_reference) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ldr_reference]");
        goto end;
    }
    }

    if (input_data->periodic_event_info) {
    cJSON *periodic_event_info_local_JSON = OpenAPI_periodic_event_info_convertToJSON(input_data->periodic_event_info);
    if (periodic_event_info_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [periodic_event_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "periodicEventInfo", periodic_event_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [periodic_event_info]");
        goto end;
    }
    }

    if (input_data->area_event_info) {
    cJSON *area_event_info_local_JSON = OpenAPI_area_event_info_convertToJSON(input_data->area_event_info);
    if (area_event_info_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [area_event_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "areaEventInfo", area_event_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [area_event_info]");
        goto end;
    }
    }

    if (input_data->motion_event_info) {
    cJSON *motion_event_info_local_JSON = OpenAPI_motion_event_info_convertToJSON(input_data->motion_event_info);
    if (motion_event_info_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [motion_event_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "motionEventInfo", motion_event_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [motion_event_info]");
        goto end;
    }
    }

    if (input_data->reporting_access_types) {
    cJSON *reporting_access_typesList = cJSON_AddArrayToObject(item, "reportingAccessTypes");
    if (reporting_access_typesList == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [reporting_access_types]");
        goto end;
    }
    OpenAPI_list_for_each(input_data->reporting_access_types, node) {
        cJSON *itemLocal = OpenAPI_reporting_access_type_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_input_data_convertToJSON() failed [reporting_access_types]");
            goto end;
        }
        cJSON_AddItemToArray(reporting_access_typesList, itemLocal);
    }
    }

    if (input_data->ue_connectivity_states) {
    cJSON *ue_connectivity_states_local_JSON = OpenAPI_ue_connectivity_state_convertToJSON(input_data->ue_connectivity_states);
    if (ue_connectivity_states_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_connectivity_states]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueConnectivityStates", ue_connectivity_states_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_connectivity_states]");
        goto end;
    }
    }

    if (input_data->ue_location_service_ind) {
    cJSON *ue_location_service_ind_local_JSON = OpenAPI_ue_location_service_ind_convertToJSON(input_data->ue_location_service_ind);
    if (ue_location_service_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_location_service_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocationServiceInd", ue_location_service_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_location_service_ind]");
        goto end;
    }
    }

    if (input_data->mo_assistance_data_types) {
    cJSON *mo_assistance_data_types_local_JSON = OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(input_data->mo_assistance_data_types);
    if (mo_assistance_data_types_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [mo_assistance_data_types]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moAssistanceDataTypes", mo_assistance_data_types_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [mo_assistance_data_types]");
        goto end;
    }
    }

    if (input_data->lpp_message) {
    cJSON *lpp_message_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(input_data->lpp_message);
    if (lpp_message_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [lpp_message]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lppMessage", lpp_message_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [lpp_message]");
        goto end;
    }
    }

    if (input_data->lpp_message_ext) {
    cJSON *lpp_message_extList = cJSON_AddArrayToObject(item, "lppMessageExt");
    if (lpp_message_extList == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [lpp_message_ext]");
        goto end;
    }
    OpenAPI_list_for_each(input_data->lpp_message_ext, node) {
        cJSON *itemLocal = OpenAPI_ref_to_binary_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_input_data_convertToJSON() failed [lpp_message_ext]");
            goto end;
        }
        cJSON_AddItemToArray(lpp_message_extList, itemLocal);
    }
    }

    if (input_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", input_data->supported_features) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (input_data->ue_positioning_cap) {
    if (cJSON_AddStringToObject(item, "uePositioningCap", input_data->ue_positioning_cap) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_positioning_cap]");
        goto end;
    }
    }

    if (input_data->tnap_id) {
    cJSON *tnap_id_local_JSON = OpenAPI_tnap_id_convertToJSON(input_data->tnap_id);
    if (tnap_id_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [tnap_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tnapId", tnap_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [tnap_id]");
        goto end;
    }
    }

    if (input_data->twap_id) {
    cJSON *twap_id_local_JSON = OpenAPI_twap_id_convertToJSON(input_data->twap_id);
    if (twap_id_local_JSON == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [twap_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "twapId", twap_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [twap_id]");
        goto end;
    }
    }

    if (input_data->is_ue_country_det_ind) {
    if (cJSON_AddBoolToObject(item, "ueCountryDetInd", input_data->ue_country_det_ind) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [ue_country_det_ind]");
        goto end;
    }
    }

    if (input_data->scheduled_loc_time) {
    if (cJSON_AddStringToObject(item, "scheduledLocTime", input_data->scheduled_loc_time) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [scheduled_loc_time]");
        goto end;
    }
    }

    if (input_data->is_reliable_loc_req) {
    if (cJSON_AddBoolToObject(item, "reliableLocReq", input_data->reliable_loc_req) == NULL) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed [reliable_loc_req]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_input_data_t *OpenAPI_input_data_parseFromJSON(cJSON *input_dataJSON)
{
    OpenAPI_input_data_t *input_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *external_client_type = NULL;
    OpenAPI_external_client_type_t *external_client_type_local_nonprim = NULL;
    cJSON *correlation_id = NULL;
    cJSON *amf_id = NULL;
    cJSON *location_qo_s = NULL;
    OpenAPI_location_qo_s_t *location_qo_s_local_nonprim = NULL;
    cJSON *supported_gad_shapes = NULL;
    OpenAPI_list_t *supported_gad_shapesList = NULL;
    cJSON *supi = NULL;
    cJSON *pei = NULL;
    cJSON *gpsi = NULL;
    cJSON *ecgi = NULL;
    OpenAPI_ecgi_t *ecgi_local_nonprim = NULL;
    cJSON *ecgi_on_second_node = NULL;
    OpenAPI_ecgi_t *ecgi_on_second_node_local_nonprim = NULL;
    cJSON *ncgi = NULL;
    OpenAPI_ncgi_t *ncgi_local_nonprim = NULL;
    cJSON *ncgi_on_second_node = NULL;
    OpenAPI_ncgi_t *ncgi_on_second_node_local_nonprim = NULL;
    cJSON *priority = NULL;
    OpenAPI_lcs_priority_t *priority_local_nonprim = NULL;
    cJSON *velocity_requested = NULL;
    OpenAPI_velocity_requested_t *velocity_requested_local_nonprim = NULL;
    cJSON *ue_lcs_cap = NULL;
    OpenAPI_ue_lcs_capability_t *ue_lcs_cap_local_nonprim = NULL;
    cJSON *lcs_service_type = NULL;
    cJSON *ldr_type = NULL;
    OpenAPI_ldr_type_t *ldr_type_local_nonprim = NULL;
    cJSON *hgmlc_call_back_uri = NULL;
    cJSON *vgmlc_address = NULL;
    cJSON *ldr_reference = NULL;
    cJSON *periodic_event_info = NULL;
    OpenAPI_periodic_event_info_t *periodic_event_info_local_nonprim = NULL;
    cJSON *area_event_info = NULL;
    OpenAPI_area_event_info_t *area_event_info_local_nonprim = NULL;
    cJSON *motion_event_info = NULL;
    OpenAPI_motion_event_info_t *motion_event_info_local_nonprim = NULL;
    cJSON *reporting_access_types = NULL;
    OpenAPI_list_t *reporting_access_typesList = NULL;
    cJSON *ue_connectivity_states = NULL;
    OpenAPI_ue_connectivity_state_t *ue_connectivity_states_local_nonprim = NULL;
    cJSON *ue_location_service_ind = NULL;
    OpenAPI_ue_location_service_ind_t *ue_location_service_ind_local_nonprim = NULL;
    cJSON *mo_assistance_data_types = NULL;
    OpenAPI_lcs_broadcast_assistance_types_data_t *mo_assistance_data_types_local_nonprim = NULL;
    cJSON *lpp_message = NULL;
    OpenAPI_ref_to_binary_data_t *lpp_message_local_nonprim = NULL;
    cJSON *lpp_message_ext = NULL;
    OpenAPI_list_t *lpp_message_extList = NULL;
    cJSON *supported_features = NULL;
    cJSON *ue_positioning_cap = NULL;
    cJSON *tnap_id = NULL;
    OpenAPI_tnap_id_t *tnap_id_local_nonprim = NULL;
    cJSON *twap_id = NULL;
    OpenAPI_twap_id_t *twap_id_local_nonprim = NULL;
    cJSON *ue_country_det_ind = NULL;
    cJSON *scheduled_loc_time = NULL;
    cJSON *reliable_loc_req = NULL;
    external_client_type = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "externalClientType");
    if (external_client_type) {
    external_client_type_local_nonprim = OpenAPI_external_client_type_parseFromJSON(external_client_type);
    if (!external_client_type_local_nonprim) {
        ogs_error("OpenAPI_external_client_type_parseFromJSON failed [external_client_type]");
        goto end;
    }
    }

    correlation_id = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "correlationID");
    if (correlation_id) {
    if (!cJSON_IsString(correlation_id) && !cJSON_IsNull(correlation_id)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [correlation_id]");
        goto end;
    }
    }

    amf_id = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "amfId");
    if (amf_id) {
    if (!cJSON_IsString(amf_id) && !cJSON_IsNull(amf_id)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [amf_id]");
        goto end;
    }
    }

    location_qo_s = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "locationQoS");
    if (location_qo_s) {
    location_qo_s_local_nonprim = OpenAPI_location_qo_s_parseFromJSON(location_qo_s);
    if (!location_qo_s_local_nonprim) {
        ogs_error("OpenAPI_location_qo_s_parseFromJSON failed [location_qo_s]");
        goto end;
    }
    }

    supported_gad_shapes = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "supportedGADShapes");
    if (supported_gad_shapes) {
        cJSON *supported_gad_shapes_local = NULL;
        if (!cJSON_IsArray(supported_gad_shapes)) {
            ogs_error("OpenAPI_input_data_parseFromJSON() failed [supported_gad_shapes]");
            goto end;
        }

        supported_gad_shapesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_gad_shapes_local, supported_gad_shapes) {
            if (!cJSON_IsObject(supported_gad_shapes_local)) {
                ogs_error("OpenAPI_input_data_parseFromJSON() failed [supported_gad_shapes]");
                goto end;
            }
            OpenAPI_supported_gad_shapes_t *supported_gad_shapesItem = OpenAPI_supported_gad_shapes_parseFromJSON(supported_gad_shapes_local);
            if (!supported_gad_shapesItem) {
                ogs_error("No supported_gad_shapesItem");
                goto end;
            }
            OpenAPI_list_add(supported_gad_shapesList, supported_gad_shapesItem);
        }
    }

    supi = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    ecgi = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ecgi");
    if (ecgi) {
    ecgi_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi);
    if (!ecgi_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi]");
        goto end;
    }
    }

    ecgi_on_second_node = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ecgiOnSecondNode");
    if (ecgi_on_second_node) {
    ecgi_on_second_node_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi_on_second_node);
    if (!ecgi_on_second_node_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi_on_second_node]");
        goto end;
    }
    }

    ncgi = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ncgi");
    if (ncgi) {
    ncgi_local_nonprim = OpenAPI_ncgi_parseFromJSON(ncgi);
    if (!ncgi_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [ncgi]");
        goto end;
    }
    }

    ncgi_on_second_node = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ncgiOnSecondNode");
    if (ncgi_on_second_node) {
    ncgi_on_second_node_local_nonprim = OpenAPI_ncgi_parseFromJSON(ncgi_on_second_node);
    if (!ncgi_on_second_node_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [ncgi_on_second_node]");
        goto end;
    }
    }

    priority = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "priority");
    if (priority) {
    priority_local_nonprim = OpenAPI_lcs_priority_parseFromJSON(priority);
    if (!priority_local_nonprim) {
        ogs_error("OpenAPI_lcs_priority_parseFromJSON failed [priority]");
        goto end;
    }
    }

    velocity_requested = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "velocityRequested");
    if (velocity_requested) {
    velocity_requested_local_nonprim = OpenAPI_velocity_requested_parseFromJSON(velocity_requested);
    if (!velocity_requested_local_nonprim) {
        ogs_error("OpenAPI_velocity_requested_parseFromJSON failed [velocity_requested]");
        goto end;
    }
    }

    ue_lcs_cap = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ueLcsCap");
    if (ue_lcs_cap) {
    ue_lcs_cap_local_nonprim = OpenAPI_ue_lcs_capability_parseFromJSON(ue_lcs_cap);
    if (!ue_lcs_cap_local_nonprim) {
        ogs_error("OpenAPI_ue_lcs_capability_parseFromJSON failed [ue_lcs_cap]");
        goto end;
    }
    }

    lcs_service_type = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "lcsServiceType");
    if (lcs_service_type) {
    if (!cJSON_IsNumber(lcs_service_type)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [lcs_service_type]");
        goto end;
    }
    }

    ldr_type = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ldrType");
    if (ldr_type) {
    ldr_type_local_nonprim = OpenAPI_ldr_type_parseFromJSON(ldr_type);
    if (!ldr_type_local_nonprim) {
        ogs_error("OpenAPI_ldr_type_parseFromJSON failed [ldr_type]");
        goto end;
    }
    }

    hgmlc_call_back_uri = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "hgmlcCallBackURI");
    if (hgmlc_call_back_uri) {
    if (!cJSON_IsString(hgmlc_call_back_uri) && !cJSON_IsNull(hgmlc_call_back_uri)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [hgmlc_call_back_uri]");
        goto end;
    }
    }

    vgmlc_address = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "vgmlcAddress");
    if (vgmlc_address) {
    if (!cJSON_IsString(vgmlc_address) && !cJSON_IsNull(vgmlc_address)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [vgmlc_address]");
        goto end;
    }
    }

    ldr_reference = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ldrReference");
    if (ldr_reference) {
    if (!cJSON_IsString(ldr_reference) && !cJSON_IsNull(ldr_reference)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [ldr_reference]");
        goto end;
    }
    }

    periodic_event_info = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "periodicEventInfo");
    if (periodic_event_info) {
    periodic_event_info_local_nonprim = OpenAPI_periodic_event_info_parseFromJSON(periodic_event_info);
    if (!periodic_event_info_local_nonprim) {
        ogs_error("OpenAPI_periodic_event_info_parseFromJSON failed [periodic_event_info]");
        goto end;
    }
    }

    area_event_info = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "areaEventInfo");
    if (area_event_info) {
    area_event_info_local_nonprim = OpenAPI_area_event_info_parseFromJSON(area_event_info);
    if (!area_event_info_local_nonprim) {
        ogs_error("OpenAPI_area_event_info_parseFromJSON failed [area_event_info]");
        goto end;
    }
    }

    motion_event_info = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "motionEventInfo");
    if (motion_event_info) {
    motion_event_info_local_nonprim = OpenAPI_motion_event_info_parseFromJSON(motion_event_info);
    if (!motion_event_info_local_nonprim) {
        ogs_error("OpenAPI_motion_event_info_parseFromJSON failed [motion_event_info]");
        goto end;
    }
    }

    reporting_access_types = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "reportingAccessTypes");
    if (reporting_access_types) {
        cJSON *reporting_access_types_local = NULL;
        if (!cJSON_IsArray(reporting_access_types)) {
            ogs_error("OpenAPI_input_data_parseFromJSON() failed [reporting_access_types]");
            goto end;
        }

        reporting_access_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(reporting_access_types_local, reporting_access_types) {
            if (!cJSON_IsObject(reporting_access_types_local)) {
                ogs_error("OpenAPI_input_data_parseFromJSON() failed [reporting_access_types]");
                goto end;
            }
            OpenAPI_reporting_access_type_t *reporting_access_typesItem = OpenAPI_reporting_access_type_parseFromJSON(reporting_access_types_local);
            if (!reporting_access_typesItem) {
                ogs_error("No reporting_access_typesItem");
                goto end;
            }
            OpenAPI_list_add(reporting_access_typesList, reporting_access_typesItem);
        }
    }

    ue_connectivity_states = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ueConnectivityStates");
    if (ue_connectivity_states) {
    ue_connectivity_states_local_nonprim = OpenAPI_ue_connectivity_state_parseFromJSON(ue_connectivity_states);
    if (!ue_connectivity_states_local_nonprim) {
        ogs_error("OpenAPI_ue_connectivity_state_parseFromJSON failed [ue_connectivity_states]");
        goto end;
    }
    }

    ue_location_service_ind = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ueLocationServiceInd");
    if (ue_location_service_ind) {
    ue_location_service_ind_local_nonprim = OpenAPI_ue_location_service_ind_parseFromJSON(ue_location_service_ind);
    if (!ue_location_service_ind_local_nonprim) {
        ogs_error("OpenAPI_ue_location_service_ind_parseFromJSON failed [ue_location_service_ind]");
        goto end;
    }
    }

    mo_assistance_data_types = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "moAssistanceDataTypes");
    if (mo_assistance_data_types) {
    mo_assistance_data_types_local_nonprim = OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(mo_assistance_data_types);
    if (!mo_assistance_data_types_local_nonprim) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON failed [mo_assistance_data_types]");
        goto end;
    }
    }

    lpp_message = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "lppMessage");
    if (lpp_message) {
    lpp_message_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(lpp_message);
    if (!lpp_message_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [lpp_message]");
        goto end;
    }
    }

    lpp_message_ext = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "lppMessageExt");
    if (lpp_message_ext) {
        cJSON *lpp_message_ext_local = NULL;
        if (!cJSON_IsArray(lpp_message_ext)) {
            ogs_error("OpenAPI_input_data_parseFromJSON() failed [lpp_message_ext]");
            goto end;
        }

        lpp_message_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(lpp_message_ext_local, lpp_message_ext) {
            if (!cJSON_IsObject(lpp_message_ext_local)) {
                ogs_error("OpenAPI_input_data_parseFromJSON() failed [lpp_message_ext]");
                goto end;
            }
            OpenAPI_ref_to_binary_data_t *lpp_message_extItem = OpenAPI_ref_to_binary_data_parseFromJSON(lpp_message_ext_local);
            if (!lpp_message_extItem) {
                ogs_error("No lpp_message_extItem");
                goto end;
            }
            OpenAPI_list_add(lpp_message_extList, lpp_message_extItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    ue_positioning_cap = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "uePositioningCap");
    if (ue_positioning_cap) {
    if (!cJSON_IsString(ue_positioning_cap) && !cJSON_IsNull(ue_positioning_cap)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [ue_positioning_cap]");
        goto end;
    }
    }

    tnap_id = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "tnapId");
    if (tnap_id) {
    tnap_id_local_nonprim = OpenAPI_tnap_id_parseFromJSON(tnap_id);
    if (!tnap_id_local_nonprim) {
        ogs_error("OpenAPI_tnap_id_parseFromJSON failed [tnap_id]");
        goto end;
    }
    }

    twap_id = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "twapId");
    if (twap_id) {
    twap_id_local_nonprim = OpenAPI_twap_id_parseFromJSON(twap_id);
    if (!twap_id_local_nonprim) {
        ogs_error("OpenAPI_twap_id_parseFromJSON failed [twap_id]");
        goto end;
    }
    }

    ue_country_det_ind = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "ueCountryDetInd");
    if (ue_country_det_ind) {
    if (!cJSON_IsBool(ue_country_det_ind)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [ue_country_det_ind]");
        goto end;
    }
    }

    scheduled_loc_time = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "scheduledLocTime");
    if (scheduled_loc_time) {
    if (!cJSON_IsString(scheduled_loc_time) && !cJSON_IsNull(scheduled_loc_time)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [scheduled_loc_time]");
        goto end;
    }
    }

    reliable_loc_req = cJSON_GetObjectItemCaseSensitive(input_dataJSON, "reliableLocReq");
    if (reliable_loc_req) {
    if (!cJSON_IsBool(reliable_loc_req)) {
        ogs_error("OpenAPI_input_data_parseFromJSON() failed [reliable_loc_req]");
        goto end;
    }
    }

    input_data_local_var = OpenAPI_input_data_create (
        external_client_type ? external_client_type_local_nonprim : NULL,
        correlation_id && !cJSON_IsNull(correlation_id) ? ogs_strdup(correlation_id->valuestring) : NULL,
        amf_id && !cJSON_IsNull(amf_id) ? ogs_strdup(amf_id->valuestring) : NULL,
        location_qo_s ? location_qo_s_local_nonprim : NULL,
        supported_gad_shapes ? supported_gad_shapesList : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        ecgi ? ecgi_local_nonprim : NULL,
        ecgi_on_second_node ? ecgi_on_second_node_local_nonprim : NULL,
        ncgi ? ncgi_local_nonprim : NULL,
        ncgi_on_second_node ? ncgi_on_second_node_local_nonprim : NULL,
        priority ? priority_local_nonprim : NULL,
        velocity_requested ? velocity_requested_local_nonprim : NULL,
        ue_lcs_cap ? ue_lcs_cap_local_nonprim : NULL,
        lcs_service_type ? true : false,
        lcs_service_type ? lcs_service_type->valuedouble : 0,
        ldr_type ? ldr_type_local_nonprim : NULL,
        hgmlc_call_back_uri && !cJSON_IsNull(hgmlc_call_back_uri) ? ogs_strdup(hgmlc_call_back_uri->valuestring) : NULL,
        vgmlc_address && !cJSON_IsNull(vgmlc_address) ? ogs_strdup(vgmlc_address->valuestring) : NULL,
        ldr_reference && !cJSON_IsNull(ldr_reference) ? ogs_strdup(ldr_reference->valuestring) : NULL,
        periodic_event_info ? periodic_event_info_local_nonprim : NULL,
        area_event_info ? area_event_info_local_nonprim : NULL,
        motion_event_info ? motion_event_info_local_nonprim : NULL,
        reporting_access_types ? reporting_access_typesList : NULL,
        ue_connectivity_states ? ue_connectivity_states_local_nonprim : NULL,
        ue_location_service_ind ? ue_location_service_ind_local_nonprim : NULL,
        mo_assistance_data_types ? mo_assistance_data_types_local_nonprim : NULL,
        lpp_message ? lpp_message_local_nonprim : NULL,
        lpp_message_ext ? lpp_message_extList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        ue_positioning_cap && !cJSON_IsNull(ue_positioning_cap) ? ogs_strdup(ue_positioning_cap->valuestring) : NULL,
        tnap_id ? tnap_id_local_nonprim : NULL,
        twap_id ? twap_id_local_nonprim : NULL,
        ue_country_det_ind ? true : false,
        ue_country_det_ind ? ue_country_det_ind->valueint : 0,
        scheduled_loc_time && !cJSON_IsNull(scheduled_loc_time) ? ogs_strdup(scheduled_loc_time->valuestring) : NULL,
        reliable_loc_req ? true : false,
        reliable_loc_req ? reliable_loc_req->valueint : 0
    );

    return input_data_local_var;
end:
    if (external_client_type_local_nonprim) {
        OpenAPI_external_client_type_free(external_client_type_local_nonprim);
        external_client_type_local_nonprim = NULL;
    }
    if (location_qo_s_local_nonprim) {
        OpenAPI_location_qo_s_free(location_qo_s_local_nonprim);
        location_qo_s_local_nonprim = NULL;
    }
    if (supported_gad_shapesList) {
        OpenAPI_list_for_each(supported_gad_shapesList, node) {
            OpenAPI_supported_gad_shapes_free(node->data);
        }
        OpenAPI_list_free(supported_gad_shapesList);
        supported_gad_shapesList = NULL;
    }
    if (ecgi_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_local_nonprim);
        ecgi_local_nonprim = NULL;
    }
    if (ecgi_on_second_node_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_on_second_node_local_nonprim);
        ecgi_on_second_node_local_nonprim = NULL;
    }
    if (ncgi_local_nonprim) {
        OpenAPI_ncgi_free(ncgi_local_nonprim);
        ncgi_local_nonprim = NULL;
    }
    if (ncgi_on_second_node_local_nonprim) {
        OpenAPI_ncgi_free(ncgi_on_second_node_local_nonprim);
        ncgi_on_second_node_local_nonprim = NULL;
    }
    if (priority_local_nonprim) {
        OpenAPI_lcs_priority_free(priority_local_nonprim);
        priority_local_nonprim = NULL;
    }
    if (velocity_requested_local_nonprim) {
        OpenAPI_velocity_requested_free(velocity_requested_local_nonprim);
        velocity_requested_local_nonprim = NULL;
    }
    if (ue_lcs_cap_local_nonprim) {
        OpenAPI_ue_lcs_capability_free(ue_lcs_cap_local_nonprim);
        ue_lcs_cap_local_nonprim = NULL;
    }
    if (ldr_type_local_nonprim) {
        OpenAPI_ldr_type_free(ldr_type_local_nonprim);
        ldr_type_local_nonprim = NULL;
    }
    if (periodic_event_info_local_nonprim) {
        OpenAPI_periodic_event_info_free(periodic_event_info_local_nonprim);
        periodic_event_info_local_nonprim = NULL;
    }
    if (area_event_info_local_nonprim) {
        OpenAPI_area_event_info_free(area_event_info_local_nonprim);
        area_event_info_local_nonprim = NULL;
    }
    if (motion_event_info_local_nonprim) {
        OpenAPI_motion_event_info_free(motion_event_info_local_nonprim);
        motion_event_info_local_nonprim = NULL;
    }
    if (reporting_access_typesList) {
        OpenAPI_list_for_each(reporting_access_typesList, node) {
            OpenAPI_reporting_access_type_free(node->data);
        }
        OpenAPI_list_free(reporting_access_typesList);
        reporting_access_typesList = NULL;
    }
    if (ue_connectivity_states_local_nonprim) {
        OpenAPI_ue_connectivity_state_free(ue_connectivity_states_local_nonprim);
        ue_connectivity_states_local_nonprim = NULL;
    }
    if (ue_location_service_ind_local_nonprim) {
        OpenAPI_ue_location_service_ind_free(ue_location_service_ind_local_nonprim);
        ue_location_service_ind_local_nonprim = NULL;
    }
    if (mo_assistance_data_types_local_nonprim) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(mo_assistance_data_types_local_nonprim);
        mo_assistance_data_types_local_nonprim = NULL;
    }
    if (lpp_message_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(lpp_message_local_nonprim);
        lpp_message_local_nonprim = NULL;
    }
    if (lpp_message_extList) {
        OpenAPI_list_for_each(lpp_message_extList, node) {
            OpenAPI_ref_to_binary_data_free(node->data);
        }
        OpenAPI_list_free(lpp_message_extList);
        lpp_message_extList = NULL;
    }
    if (tnap_id_local_nonprim) {
        OpenAPI_tnap_id_free(tnap_id_local_nonprim);
        tnap_id_local_nonprim = NULL;
    }
    if (twap_id_local_nonprim) {
        OpenAPI_twap_id_free(twap_id_local_nonprim);
        twap_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_input_data_t *OpenAPI_input_data_copy(OpenAPI_input_data_t *dst, OpenAPI_input_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_input_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_input_data_convertToJSON() failed");
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

    OpenAPI_input_data_free(dst);
    dst = OpenAPI_input_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

