
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_information.h"

OpenAPI_pdu_session_information_t *OpenAPI_pdu_session_information_create(
    OpenAPI_network_slicing_info_t *network_slicing_info,
    int pdu_session_id,
    OpenAPI_pdu_session_type_e pdu_type,
    OpenAPI_ssc_mode_e ssc_mode,
    OpenAPI_plmn_id_t *h_plmn_id,
    OpenAPI_serving_network_function_id_t *serving_network_function_id,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_rat_type_e m_apdu_non3_gpprat_type,
    char *dnn_id,
    OpenAPI_dnn_selection_mode_e dnn_selection_mode,
    char *charging_characteristics,
    OpenAPI_charging_characteristics_selection_mode_e charging_characteristics_selection_mode,
    char *start_time,
    char *stop_time,
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_status,
    bool is_session_stop_indicator,
    int session_stop_indicator,
    OpenAPI_pdu_address_t *pdu_address,
    bool is_diagnostics,
    int diagnostics,
    OpenAPI_authorized_default_qos_t *authorized_qo_s_information,
    OpenAPI_subscribed_default_qos_t *subscribed_qo_s_information,
    OpenAPI_ambr_t *authorized_session_ambr,
    OpenAPI_ambr_t *subscribed_session_ambr,
    OpenAPI_plmn_id_t *serving_cn_plmn_id,
    OpenAPI_mapdu_session_information_t *m_apdu_session_information,
    OpenAPI_list_t *enhanced_diagnostics,
    OpenAPI_redundant_transmission_type_e redundant_transmission_type,
    bool is_p_du_session_pair_id,
    int p_du_session_pair_id,
    bool is_cp_cio_t_optimisation_indicator,
    int cp_cio_t_optimisation_indicator,
    bool is__5_gs_control_plane_only_indicator,
    int _5_gs_control_plane_only_indicator,
    bool is_small_data_rate_control_indicator,
    int small_data_rate_control_indicator,
    OpenAPI_model_5_glan_type_service_t *_5_glan_type_service,
    OpenAPI_snpn_information_t *s_npn_information,
    OpenAPI_model_5_g_multicast_service_t *_5_g_multicast_service,
    OpenAPI_model_5_gs_bridge_information_t *_5_gs_bridge_information,
    bool is_satellite_access_indicator,
    int satellite_access_indicator,
    OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information,
    char *service_level_aa
)
{
    OpenAPI_pdu_session_information_t *pdu_session_information_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_information_t));
    ogs_assert(pdu_session_information_local_var);

    pdu_session_information_local_var->network_slicing_info = network_slicing_info;
    pdu_session_information_local_var->pdu_session_id = pdu_session_id;
    pdu_session_information_local_var->pdu_type = pdu_type;
    pdu_session_information_local_var->ssc_mode = ssc_mode;
    pdu_session_information_local_var->h_plmn_id = h_plmn_id;
    pdu_session_information_local_var->serving_network_function_id = serving_network_function_id;
    pdu_session_information_local_var->rat_type = rat_type;
    pdu_session_information_local_var->m_apdu_non3_gpprat_type = m_apdu_non3_gpprat_type;
    pdu_session_information_local_var->dnn_id = dnn_id;
    pdu_session_information_local_var->dnn_selection_mode = dnn_selection_mode;
    pdu_session_information_local_var->charging_characteristics = charging_characteristics;
    pdu_session_information_local_var->charging_characteristics_selection_mode = charging_characteristics_selection_mode;
    pdu_session_information_local_var->start_time = start_time;
    pdu_session_information_local_var->stop_time = stop_time;
    pdu_session_information_local_var->_3gpp_ps_data_off_status = _3gpp_ps_data_off_status;
    pdu_session_information_local_var->is_session_stop_indicator = is_session_stop_indicator;
    pdu_session_information_local_var->session_stop_indicator = session_stop_indicator;
    pdu_session_information_local_var->pdu_address = pdu_address;
    pdu_session_information_local_var->is_diagnostics = is_diagnostics;
    pdu_session_information_local_var->diagnostics = diagnostics;
    pdu_session_information_local_var->authorized_qo_s_information = authorized_qo_s_information;
    pdu_session_information_local_var->subscribed_qo_s_information = subscribed_qo_s_information;
    pdu_session_information_local_var->authorized_session_ambr = authorized_session_ambr;
    pdu_session_information_local_var->subscribed_session_ambr = subscribed_session_ambr;
    pdu_session_information_local_var->serving_cn_plmn_id = serving_cn_plmn_id;
    pdu_session_information_local_var->m_apdu_session_information = m_apdu_session_information;
    pdu_session_information_local_var->enhanced_diagnostics = enhanced_diagnostics;
    pdu_session_information_local_var->redundant_transmission_type = redundant_transmission_type;
    pdu_session_information_local_var->is_p_du_session_pair_id = is_p_du_session_pair_id;
    pdu_session_information_local_var->p_du_session_pair_id = p_du_session_pair_id;
    pdu_session_information_local_var->is_cp_cio_t_optimisation_indicator = is_cp_cio_t_optimisation_indicator;
    pdu_session_information_local_var->cp_cio_t_optimisation_indicator = cp_cio_t_optimisation_indicator;
    pdu_session_information_local_var->is__5_gs_control_plane_only_indicator = is__5_gs_control_plane_only_indicator;
    pdu_session_information_local_var->_5_gs_control_plane_only_indicator = _5_gs_control_plane_only_indicator;
    pdu_session_information_local_var->is_small_data_rate_control_indicator = is_small_data_rate_control_indicator;
    pdu_session_information_local_var->small_data_rate_control_indicator = small_data_rate_control_indicator;
    pdu_session_information_local_var->_5_glan_type_service = _5_glan_type_service;
    pdu_session_information_local_var->s_npn_information = s_npn_information;
    pdu_session_information_local_var->_5_g_multicast_service = _5_g_multicast_service;
    pdu_session_information_local_var->_5_gs_bridge_information = _5_gs_bridge_information;
    pdu_session_information_local_var->is_satellite_access_indicator = is_satellite_access_indicator;
    pdu_session_information_local_var->satellite_access_indicator = satellite_access_indicator;
    pdu_session_information_local_var->satellite_backhaul_information = satellite_backhaul_information;
    pdu_session_information_local_var->service_level_aa = service_level_aa;

    return pdu_session_information_local_var;
}

void OpenAPI_pdu_session_information_free(OpenAPI_pdu_session_information_t *pdu_session_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_information) {
        return;
    }
    if (pdu_session_information->network_slicing_info) {
        OpenAPI_network_slicing_info_free(pdu_session_information->network_slicing_info);
        pdu_session_information->network_slicing_info = NULL;
    }
    if (pdu_session_information->h_plmn_id) {
        OpenAPI_plmn_id_free(pdu_session_information->h_plmn_id);
        pdu_session_information->h_plmn_id = NULL;
    }
    if (pdu_session_information->serving_network_function_id) {
        OpenAPI_serving_network_function_id_free(pdu_session_information->serving_network_function_id);
        pdu_session_information->serving_network_function_id = NULL;
    }
    if (pdu_session_information->dnn_id) {
        ogs_free(pdu_session_information->dnn_id);
        pdu_session_information->dnn_id = NULL;
    }
    if (pdu_session_information->charging_characteristics) {
        ogs_free(pdu_session_information->charging_characteristics);
        pdu_session_information->charging_characteristics = NULL;
    }
    if (pdu_session_information->start_time) {
        ogs_free(pdu_session_information->start_time);
        pdu_session_information->start_time = NULL;
    }
    if (pdu_session_information->stop_time) {
        ogs_free(pdu_session_information->stop_time);
        pdu_session_information->stop_time = NULL;
    }
    if (pdu_session_information->pdu_address) {
        OpenAPI_pdu_address_free(pdu_session_information->pdu_address);
        pdu_session_information->pdu_address = NULL;
    }
    if (pdu_session_information->authorized_qo_s_information) {
        OpenAPI_authorized_default_qos_free(pdu_session_information->authorized_qo_s_information);
        pdu_session_information->authorized_qo_s_information = NULL;
    }
    if (pdu_session_information->subscribed_qo_s_information) {
        OpenAPI_subscribed_default_qos_free(pdu_session_information->subscribed_qo_s_information);
        pdu_session_information->subscribed_qo_s_information = NULL;
    }
    if (pdu_session_information->authorized_session_ambr) {
        OpenAPI_ambr_free(pdu_session_information->authorized_session_ambr);
        pdu_session_information->authorized_session_ambr = NULL;
    }
    if (pdu_session_information->subscribed_session_ambr) {
        OpenAPI_ambr_free(pdu_session_information->subscribed_session_ambr);
        pdu_session_information->subscribed_session_ambr = NULL;
    }
    if (pdu_session_information->serving_cn_plmn_id) {
        OpenAPI_plmn_id_free(pdu_session_information->serving_cn_plmn_id);
        pdu_session_information->serving_cn_plmn_id = NULL;
    }
    if (pdu_session_information->m_apdu_session_information) {
        OpenAPI_mapdu_session_information_free(pdu_session_information->m_apdu_session_information);
        pdu_session_information->m_apdu_session_information = NULL;
    }
    if (pdu_session_information->enhanced_diagnostics) {
        OpenAPI_list_for_each(pdu_session_information->enhanced_diagnostics, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(pdu_session_information->enhanced_diagnostics);
        pdu_session_information->enhanced_diagnostics = NULL;
    }
    if (pdu_session_information->_5_glan_type_service) {
        OpenAPI_model_5_glan_type_service_free(pdu_session_information->_5_glan_type_service);
        pdu_session_information->_5_glan_type_service = NULL;
    }
    if (pdu_session_information->s_npn_information) {
        OpenAPI_snpn_information_free(pdu_session_information->s_npn_information);
        pdu_session_information->s_npn_information = NULL;
    }
    if (pdu_session_information->_5_g_multicast_service) {
        OpenAPI_model_5_g_multicast_service_free(pdu_session_information->_5_g_multicast_service);
        pdu_session_information->_5_g_multicast_service = NULL;
    }
    if (pdu_session_information->_5_gs_bridge_information) {
        OpenAPI_model_5_gs_bridge_information_free(pdu_session_information->_5_gs_bridge_information);
        pdu_session_information->_5_gs_bridge_information = NULL;
    }
    if (pdu_session_information->satellite_backhaul_information) {
        OpenAPI_satellite_backhaul_information_free(pdu_session_information->satellite_backhaul_information);
        pdu_session_information->satellite_backhaul_information = NULL;
    }
    if (pdu_session_information->service_level_aa) {
        ogs_free(pdu_session_information->service_level_aa);
        pdu_session_information->service_level_aa = NULL;
    }
    ogs_free(pdu_session_information);
}

cJSON *OpenAPI_pdu_session_information_convertToJSON(OpenAPI_pdu_session_information_t *pdu_session_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_information == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [PDUSessionInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_information->network_slicing_info) {
    cJSON *network_slicing_info_local_JSON = OpenAPI_network_slicing_info_convertToJSON(pdu_session_information->network_slicing_info);
    if (network_slicing_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [network_slicing_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkSlicingInfo", network_slicing_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [network_slicing_info]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "pduSessionID", pdu_session_information->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (pdu_session_information->pdu_type != OpenAPI_pdu_session_type_NULL) {
    if (cJSON_AddStringToObject(item, "pduType", OpenAPI_pdu_session_type_ToString(pdu_session_information->pdu_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [pdu_type]");
        goto end;
    }
    }

    if (pdu_session_information->ssc_mode != OpenAPI_ssc_mode_NULL) {
    if (cJSON_AddStringToObject(item, "sscMode", OpenAPI_ssc_mode_ToString(pdu_session_information->ssc_mode)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [ssc_mode]");
        goto end;
    }
    }

    if (pdu_session_information->h_plmn_id) {
    cJSON *h_plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(pdu_session_information->h_plmn_id);
    if (h_plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [h_plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hPlmnId", h_plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [h_plmn_id]");
        goto end;
    }
    }

    if (pdu_session_information->serving_network_function_id) {
    cJSON *serving_network_function_id_local_JSON = OpenAPI_serving_network_function_id_convertToJSON(pdu_session_information->serving_network_function_id);
    if (serving_network_function_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [serving_network_function_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetworkFunctionID", serving_network_function_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [serving_network_function_id]");
        goto end;
    }
    }

    if (pdu_session_information->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(pdu_session_information->rat_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (pdu_session_information->m_apdu_non3_gpprat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "mAPDUNon3GPPRATType", OpenAPI_rat_type_ToString(pdu_session_information->m_apdu_non3_gpprat_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [m_apdu_non3_gpprat_type]");
        goto end;
    }
    }

    if (!pdu_session_information->dnn_id) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [dnn_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnnId", pdu_session_information->dnn_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [dnn_id]");
        goto end;
    }

    if (pdu_session_information->dnn_selection_mode != OpenAPI_dnn_selection_mode_NULL) {
    if (cJSON_AddStringToObject(item, "dnnSelectionMode", OpenAPI_dnn_selection_mode_ToString(pdu_session_information->dnn_selection_mode)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [dnn_selection_mode]");
        goto end;
    }
    }

    if (pdu_session_information->charging_characteristics) {
    if (cJSON_AddStringToObject(item, "chargingCharacteristics", pdu_session_information->charging_characteristics) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [charging_characteristics]");
        goto end;
    }
    }

    if (pdu_session_information->charging_characteristics_selection_mode != OpenAPI_charging_characteristics_selection_mode_NULL) {
    if (cJSON_AddStringToObject(item, "chargingCharacteristicsSelectionMode", OpenAPI_charging_characteristics_selection_mode_ToString(pdu_session_information->charging_characteristics_selection_mode)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [charging_characteristics_selection_mode]");
        goto end;
    }
    }

    if (pdu_session_information->start_time) {
    if (cJSON_AddStringToObject(item, "startTime", pdu_session_information->start_time) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [start_time]");
        goto end;
    }
    }

    if (pdu_session_information->stop_time) {
    if (cJSON_AddStringToObject(item, "stopTime", pdu_session_information->stop_time) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [stop_time]");
        goto end;
    }
    }

    if (pdu_session_information->_3gpp_ps_data_off_status != OpenAPI_model_3_gppps_data_off_status_NULL) {
    if (cJSON_AddStringToObject(item, "3gppPSDataOffStatus", OpenAPI_model_3_gppps_data_off_status_ToString(pdu_session_information->_3gpp_ps_data_off_status)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    }

    if (pdu_session_information->is_session_stop_indicator) {
    if (cJSON_AddBoolToObject(item, "sessionStopIndicator", pdu_session_information->session_stop_indicator) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [session_stop_indicator]");
        goto end;
    }
    }

    if (pdu_session_information->pdu_address) {
    cJSON *pdu_address_local_JSON = OpenAPI_pdu_address_convertToJSON(pdu_session_information->pdu_address);
    if (pdu_address_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [pdu_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduAddress", pdu_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [pdu_address]");
        goto end;
    }
    }

    if (pdu_session_information->is_diagnostics) {
    if (cJSON_AddNumberToObject(item, "diagnostics", pdu_session_information->diagnostics) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [diagnostics]");
        goto end;
    }
    }

    if (pdu_session_information->authorized_qo_s_information) {
    cJSON *authorized_qo_s_information_local_JSON = OpenAPI_authorized_default_qos_convertToJSON(pdu_session_information->authorized_qo_s_information);
    if (authorized_qo_s_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [authorized_qo_s_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "authorizedQoSInformation", authorized_qo_s_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [authorized_qo_s_information]");
        goto end;
    }
    }

    if (pdu_session_information->subscribed_qo_s_information) {
    cJSON *subscribed_qo_s_information_local_JSON = OpenAPI_subscribed_default_qos_convertToJSON(pdu_session_information->subscribed_qo_s_information);
    if (subscribed_qo_s_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [subscribed_qo_s_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscribedQoSInformation", subscribed_qo_s_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [subscribed_qo_s_information]");
        goto end;
    }
    }

    if (pdu_session_information->authorized_session_ambr) {
    cJSON *authorized_session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(pdu_session_information->authorized_session_ambr);
    if (authorized_session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [authorized_session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "authorizedSessionAMBR", authorized_session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [authorized_session_ambr]");
        goto end;
    }
    }

    if (pdu_session_information->subscribed_session_ambr) {
    cJSON *subscribed_session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(pdu_session_information->subscribed_session_ambr);
    if (subscribed_session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [subscribed_session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscribedSessionAMBR", subscribed_session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [subscribed_session_ambr]");
        goto end;
    }
    }

    if (pdu_session_information->serving_cn_plmn_id) {
    cJSON *serving_cn_plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(pdu_session_information->serving_cn_plmn_id);
    if (serving_cn_plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [serving_cn_plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingCNPlmnId", serving_cn_plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [serving_cn_plmn_id]");
        goto end;
    }
    }

    if (pdu_session_information->m_apdu_session_information) {
    cJSON *m_apdu_session_information_local_JSON = OpenAPI_mapdu_session_information_convertToJSON(pdu_session_information->m_apdu_session_information);
    if (m_apdu_session_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [m_apdu_session_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mAPDUSessionInformation", m_apdu_session_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [m_apdu_session_information]");
        goto end;
    }
    }

    if (pdu_session_information->enhanced_diagnostics) {
    cJSON *enhanced_diagnosticsList = cJSON_AddArrayToObject(item, "enhancedDiagnostics");
    if (enhanced_diagnosticsList == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [enhanced_diagnostics]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_information->enhanced_diagnostics, node) {
        cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [enhanced_diagnostics]");
            goto end;
        }
        cJSON_AddItemToArray(enhanced_diagnosticsList, itemLocal);
    }
    }

    if (pdu_session_information->redundant_transmission_type != OpenAPI_redundant_transmission_type_NULL) {
    if (cJSON_AddStringToObject(item, "redundantTransmissionType", OpenAPI_redundant_transmission_type_ToString(pdu_session_information->redundant_transmission_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [redundant_transmission_type]");
        goto end;
    }
    }

    if (pdu_session_information->is_p_du_session_pair_id) {
    if (cJSON_AddNumberToObject(item, "pDUSessionPairID", pdu_session_information->p_du_session_pair_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [p_du_session_pair_id]");
        goto end;
    }
    }

    if (pdu_session_information->is_cp_cio_t_optimisation_indicator) {
    if (cJSON_AddBoolToObject(item, "cpCIoTOptimisationIndicator", pdu_session_information->cp_cio_t_optimisation_indicator) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [cp_cio_t_optimisation_indicator]");
        goto end;
    }
    }

    if (pdu_session_information->is__5_gs_control_plane_only_indicator) {
    if (cJSON_AddBoolToObject(item, "5GSControlPlaneOnlyIndicator", pdu_session_information->_5_gs_control_plane_only_indicator) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_5_gs_control_plane_only_indicator]");
        goto end;
    }
    }

    if (pdu_session_information->is_small_data_rate_control_indicator) {
    if (cJSON_AddBoolToObject(item, "smallDataRateControlIndicator", pdu_session_information->small_data_rate_control_indicator) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [small_data_rate_control_indicator]");
        goto end;
    }
    }

    if (pdu_session_information->_5_glan_type_service) {
    cJSON *_5_glan_type_service_local_JSON = OpenAPI_model_5_glan_type_service_convertToJSON(pdu_session_information->_5_glan_type_service);
    if (_5_glan_type_service_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_5_glan_type_service]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5GLANTypeService", _5_glan_type_service_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_5_glan_type_service]");
        goto end;
    }
    }

    if (pdu_session_information->s_npn_information) {
    cJSON *s_npn_information_local_JSON = OpenAPI_snpn_information_convertToJSON(pdu_session_information->s_npn_information);
    if (s_npn_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [s_npn_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNPNInformation", s_npn_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [s_npn_information]");
        goto end;
    }
    }

    if (pdu_session_information->_5_g_multicast_service) {
    cJSON *_5_g_multicast_service_local_JSON = OpenAPI_model_5_g_multicast_service_convertToJSON(pdu_session_information->_5_g_multicast_service);
    if (_5_g_multicast_service_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_5_g_multicast_service]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5GMulticastService", _5_g_multicast_service_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_5_g_multicast_service]");
        goto end;
    }
    }

    if (pdu_session_information->_5_gs_bridge_information) {
    cJSON *_5_gs_bridge_information_local_JSON = OpenAPI_model_5_gs_bridge_information_convertToJSON(pdu_session_information->_5_gs_bridge_information);
    if (_5_gs_bridge_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_5_gs_bridge_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5GSBridgeInformation", _5_gs_bridge_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [_5_gs_bridge_information]");
        goto end;
    }
    }

    if (pdu_session_information->is_satellite_access_indicator) {
    if (cJSON_AddBoolToObject(item, "satelliteAccessIndicator", pdu_session_information->satellite_access_indicator) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

    if (pdu_session_information->satellite_backhaul_information) {
    cJSON *satellite_backhaul_information_local_JSON = OpenAPI_satellite_backhaul_information_convertToJSON(pdu_session_information->satellite_backhaul_information);
    if (satellite_backhaul_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [satellite_backhaul_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "satelliteBackhaulInformation", satellite_backhaul_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [satellite_backhaul_information]");
        goto end;
    }
    }

    if (pdu_session_information->service_level_aa) {
    if (cJSON_AddStringToObject(item, "serviceLevelAA", pdu_session_information->service_level_aa) == NULL) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed [service_level_aa]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_information_t *OpenAPI_pdu_session_information_parseFromJSON(cJSON *pdu_session_informationJSON)
{
    OpenAPI_pdu_session_information_t *pdu_session_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *network_slicing_info = NULL;
    OpenAPI_network_slicing_info_t *network_slicing_info_local_nonprim = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *pdu_type = NULL;
    OpenAPI_pdu_session_type_e pdu_typeVariable = 0;
    cJSON *ssc_mode = NULL;
    OpenAPI_ssc_mode_e ssc_modeVariable = 0;
    cJSON *h_plmn_id = NULL;
    OpenAPI_plmn_id_t *h_plmn_id_local_nonprim = NULL;
    cJSON *serving_network_function_id = NULL;
    OpenAPI_serving_network_function_id_t *serving_network_function_id_local_nonprim = NULL;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *m_apdu_non3_gpprat_type = NULL;
    OpenAPI_rat_type_e m_apdu_non3_gpprat_typeVariable = 0;
    cJSON *dnn_id = NULL;
    cJSON *dnn_selection_mode = NULL;
    OpenAPI_dnn_selection_mode_e dnn_selection_modeVariable = 0;
    cJSON *charging_characteristics = NULL;
    cJSON *charging_characteristics_selection_mode = NULL;
    OpenAPI_charging_characteristics_selection_mode_e charging_characteristics_selection_modeVariable = 0;
    cJSON *start_time = NULL;
    cJSON *stop_time = NULL;
    cJSON *_3gpp_ps_data_off_status = NULL;
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_statusVariable = 0;
    cJSON *session_stop_indicator = NULL;
    cJSON *pdu_address = NULL;
    OpenAPI_pdu_address_t *pdu_address_local_nonprim = NULL;
    cJSON *diagnostics = NULL;
    cJSON *authorized_qo_s_information = NULL;
    OpenAPI_authorized_default_qos_t *authorized_qo_s_information_local_nonprim = NULL;
    cJSON *subscribed_qo_s_information = NULL;
    OpenAPI_subscribed_default_qos_t *subscribed_qo_s_information_local_nonprim = NULL;
    cJSON *authorized_session_ambr = NULL;
    OpenAPI_ambr_t *authorized_session_ambr_local_nonprim = NULL;
    cJSON *subscribed_session_ambr = NULL;
    OpenAPI_ambr_t *subscribed_session_ambr_local_nonprim = NULL;
    cJSON *serving_cn_plmn_id = NULL;
    OpenAPI_plmn_id_t *serving_cn_plmn_id_local_nonprim = NULL;
    cJSON *m_apdu_session_information = NULL;
    OpenAPI_mapdu_session_information_t *m_apdu_session_information_local_nonprim = NULL;
    cJSON *enhanced_diagnostics = NULL;
    OpenAPI_list_t *enhanced_diagnosticsList = NULL;
    cJSON *redundant_transmission_type = NULL;
    OpenAPI_redundant_transmission_type_e redundant_transmission_typeVariable = 0;
    cJSON *p_du_session_pair_id = NULL;
    cJSON *cp_cio_t_optimisation_indicator = NULL;
    cJSON *_5_gs_control_plane_only_indicator = NULL;
    cJSON *small_data_rate_control_indicator = NULL;
    cJSON *_5_glan_type_service = NULL;
    OpenAPI_model_5_glan_type_service_t *_5_glan_type_service_local_nonprim = NULL;
    cJSON *s_npn_information = NULL;
    OpenAPI_snpn_information_t *s_npn_information_local_nonprim = NULL;
    cJSON *_5_g_multicast_service = NULL;
    OpenAPI_model_5_g_multicast_service_t *_5_g_multicast_service_local_nonprim = NULL;
    cJSON *_5_gs_bridge_information = NULL;
    OpenAPI_model_5_gs_bridge_information_t *_5_gs_bridge_information_local_nonprim = NULL;
    cJSON *satellite_access_indicator = NULL;
    cJSON *satellite_backhaul_information = NULL;
    OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information_local_nonprim = NULL;
    cJSON *service_level_aa = NULL;
    network_slicing_info = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "networkSlicingInfo");
    if (network_slicing_info) {
    network_slicing_info_local_nonprim = OpenAPI_network_slicing_info_parseFromJSON(network_slicing_info);
    if (!network_slicing_info_local_nonprim) {
        ogs_error("OpenAPI_network_slicing_info_parseFromJSON failed [network_slicing_info]");
        goto end;
    }
    }

    pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "pduSessionID");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    pdu_type = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "pduType");
    if (pdu_type) {
    if (!cJSON_IsString(pdu_type)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [pdu_type]");
        goto end;
    }
    pdu_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_type->valuestring);
    }

    ssc_mode = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "sscMode");
    if (ssc_mode) {
    if (!cJSON_IsString(ssc_mode)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [ssc_mode]");
        goto end;
    }
    ssc_modeVariable = OpenAPI_ssc_mode_FromString(ssc_mode->valuestring);
    }

    h_plmn_id = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "hPlmnId");
    if (h_plmn_id) {
    h_plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(h_plmn_id);
    if (!h_plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [h_plmn_id]");
        goto end;
    }
    }

    serving_network_function_id = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "servingNetworkFunctionID");
    if (serving_network_function_id) {
    serving_network_function_id_local_nonprim = OpenAPI_serving_network_function_id_parseFromJSON(serving_network_function_id);
    if (!serving_network_function_id_local_nonprim) {
        ogs_error("OpenAPI_serving_network_function_id_parseFromJSON failed [serving_network_function_id]");
        goto end;
    }
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    m_apdu_non3_gpprat_type = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "mAPDUNon3GPPRATType");
    if (m_apdu_non3_gpprat_type) {
    if (!cJSON_IsString(m_apdu_non3_gpprat_type)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [m_apdu_non3_gpprat_type]");
        goto end;
    }
    m_apdu_non3_gpprat_typeVariable = OpenAPI_rat_type_FromString(m_apdu_non3_gpprat_type->valuestring);
    }

    dnn_id = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "dnnId");
    if (!dnn_id) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [dnn_id]");
        goto end;
    }
    if (!cJSON_IsString(dnn_id)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [dnn_id]");
        goto end;
    }

    dnn_selection_mode = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "dnnSelectionMode");
    if (dnn_selection_mode) {
    if (!cJSON_IsString(dnn_selection_mode)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [dnn_selection_mode]");
        goto end;
    }
    dnn_selection_modeVariable = OpenAPI_dnn_selection_mode_FromString(dnn_selection_mode->valuestring);
    }

    charging_characteristics = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "chargingCharacteristics");
    if (charging_characteristics) {
    if (!cJSON_IsString(charging_characteristics) && !cJSON_IsNull(charging_characteristics)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [charging_characteristics]");
        goto end;
    }
    }

    charging_characteristics_selection_mode = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "chargingCharacteristicsSelectionMode");
    if (charging_characteristics_selection_mode) {
    if (!cJSON_IsString(charging_characteristics_selection_mode)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [charging_characteristics_selection_mode]");
        goto end;
    }
    charging_characteristics_selection_modeVariable = OpenAPI_charging_characteristics_selection_mode_FromString(charging_characteristics_selection_mode->valuestring);
    }

    start_time = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "startTime");
    if (start_time) {
    if (!cJSON_IsString(start_time) && !cJSON_IsNull(start_time)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [start_time]");
        goto end;
    }
    }

    stop_time = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "stopTime");
    if (stop_time) {
    if (!cJSON_IsString(stop_time) && !cJSON_IsNull(stop_time)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [stop_time]");
        goto end;
    }
    }

    _3gpp_ps_data_off_status = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "3gppPSDataOffStatus");
    if (_3gpp_ps_data_off_status) {
    if (!cJSON_IsString(_3gpp_ps_data_off_status)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    _3gpp_ps_data_off_statusVariable = OpenAPI_model_3_gppps_data_off_status_FromString(_3gpp_ps_data_off_status->valuestring);
    }

    session_stop_indicator = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "sessionStopIndicator");
    if (session_stop_indicator) {
    if (!cJSON_IsBool(session_stop_indicator)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [session_stop_indicator]");
        goto end;
    }
    }

    pdu_address = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "pduAddress");
    if (pdu_address) {
    pdu_address_local_nonprim = OpenAPI_pdu_address_parseFromJSON(pdu_address);
    if (!pdu_address_local_nonprim) {
        ogs_error("OpenAPI_pdu_address_parseFromJSON failed [pdu_address]");
        goto end;
    }
    }

    diagnostics = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "diagnostics");
    if (diagnostics) {
    if (!cJSON_IsNumber(diagnostics)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [diagnostics]");
        goto end;
    }
    }

    authorized_qo_s_information = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "authorizedQoSInformation");
    if (authorized_qo_s_information) {
    authorized_qo_s_information_local_nonprim = OpenAPI_authorized_default_qos_parseFromJSON(authorized_qo_s_information);
    if (!authorized_qo_s_information_local_nonprim) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON failed [authorized_qo_s_information]");
        goto end;
    }
    }

    subscribed_qo_s_information = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "subscribedQoSInformation");
    if (subscribed_qo_s_information) {
    subscribed_qo_s_information_local_nonprim = OpenAPI_subscribed_default_qos_parseFromJSON(subscribed_qo_s_information);
    if (!subscribed_qo_s_information_local_nonprim) {
        ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON failed [subscribed_qo_s_information]");
        goto end;
    }
    }

    authorized_session_ambr = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "authorizedSessionAMBR");
    if (authorized_session_ambr) {
    authorized_session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(authorized_session_ambr);
    if (!authorized_session_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [authorized_session_ambr]");
        goto end;
    }
    }

    subscribed_session_ambr = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "subscribedSessionAMBR");
    if (subscribed_session_ambr) {
    subscribed_session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(subscribed_session_ambr);
    if (!subscribed_session_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [subscribed_session_ambr]");
        goto end;
    }
    }

    serving_cn_plmn_id = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "servingCNPlmnId");
    if (serving_cn_plmn_id) {
    serving_cn_plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(serving_cn_plmn_id);
    if (!serving_cn_plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [serving_cn_plmn_id]");
        goto end;
    }
    }

    m_apdu_session_information = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "mAPDUSessionInformation");
    if (m_apdu_session_information) {
    m_apdu_session_information_local_nonprim = OpenAPI_mapdu_session_information_parseFromJSON(m_apdu_session_information);
    if (!m_apdu_session_information_local_nonprim) {
        ogs_error("OpenAPI_mapdu_session_information_parseFromJSON failed [m_apdu_session_information]");
        goto end;
    }
    }

    enhanced_diagnostics = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "enhancedDiagnostics");
    if (enhanced_diagnostics) {
        cJSON *enhanced_diagnostics_local = NULL;
        if (!cJSON_IsArray(enhanced_diagnostics)) {
            ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [enhanced_diagnostics]");
            goto end;
        }

        enhanced_diagnosticsList = OpenAPI_list_create();

        cJSON_ArrayForEach(enhanced_diagnostics_local, enhanced_diagnostics) {
            if (!cJSON_IsObject(enhanced_diagnostics_local)) {
                ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [enhanced_diagnostics]");
                goto end;
            }
            OpenAPI_ran_nas_rel_cause_t *enhanced_diagnosticsItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(enhanced_diagnostics_local);
            if (!enhanced_diagnosticsItem) {
                ogs_error("No enhanced_diagnosticsItem");
                goto end;
            }
            OpenAPI_list_add(enhanced_diagnosticsList, enhanced_diagnosticsItem);
        }
    }

    redundant_transmission_type = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "redundantTransmissionType");
    if (redundant_transmission_type) {
    if (!cJSON_IsString(redundant_transmission_type)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [redundant_transmission_type]");
        goto end;
    }
    redundant_transmission_typeVariable = OpenAPI_redundant_transmission_type_FromString(redundant_transmission_type->valuestring);
    }

    p_du_session_pair_id = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "pDUSessionPairID");
    if (p_du_session_pair_id) {
    if (!cJSON_IsNumber(p_du_session_pair_id)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [p_du_session_pair_id]");
        goto end;
    }
    }

    cp_cio_t_optimisation_indicator = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "cpCIoTOptimisationIndicator");
    if (cp_cio_t_optimisation_indicator) {
    if (!cJSON_IsBool(cp_cio_t_optimisation_indicator)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [cp_cio_t_optimisation_indicator]");
        goto end;
    }
    }

    _5_gs_control_plane_only_indicator = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "5GSControlPlaneOnlyIndicator");
    if (_5_gs_control_plane_only_indicator) {
    if (!cJSON_IsBool(_5_gs_control_plane_only_indicator)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [_5_gs_control_plane_only_indicator]");
        goto end;
    }
    }

    small_data_rate_control_indicator = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "smallDataRateControlIndicator");
    if (small_data_rate_control_indicator) {
    if (!cJSON_IsBool(small_data_rate_control_indicator)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [small_data_rate_control_indicator]");
        goto end;
    }
    }

    _5_glan_type_service = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "5GLANTypeService");
    if (_5_glan_type_service) {
    _5_glan_type_service_local_nonprim = OpenAPI_model_5_glan_type_service_parseFromJSON(_5_glan_type_service);
    if (!_5_glan_type_service_local_nonprim) {
        ogs_error("OpenAPI_model_5_glan_type_service_parseFromJSON failed [_5_glan_type_service]");
        goto end;
    }
    }

    s_npn_information = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "sNPNInformation");
    if (s_npn_information) {
    s_npn_information_local_nonprim = OpenAPI_snpn_information_parseFromJSON(s_npn_information);
    if (!s_npn_information_local_nonprim) {
        ogs_error("OpenAPI_snpn_information_parseFromJSON failed [s_npn_information]");
        goto end;
    }
    }

    _5_g_multicast_service = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "5GMulticastService");
    if (_5_g_multicast_service) {
    _5_g_multicast_service_local_nonprim = OpenAPI_model_5_g_multicast_service_parseFromJSON(_5_g_multicast_service);
    if (!_5_g_multicast_service_local_nonprim) {
        ogs_error("OpenAPI_model_5_g_multicast_service_parseFromJSON failed [_5_g_multicast_service]");
        goto end;
    }
    }

    _5_gs_bridge_information = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "5GSBridgeInformation");
    if (_5_gs_bridge_information) {
    _5_gs_bridge_information_local_nonprim = OpenAPI_model_5_gs_bridge_information_parseFromJSON(_5_gs_bridge_information);
    if (!_5_gs_bridge_information_local_nonprim) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_parseFromJSON failed [_5_gs_bridge_information]");
        goto end;
    }
    }

    satellite_access_indicator = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "satelliteAccessIndicator");
    if (satellite_access_indicator) {
    if (!cJSON_IsBool(satellite_access_indicator)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

    satellite_backhaul_information = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "satelliteBackhaulInformation");
    if (satellite_backhaul_information) {
    satellite_backhaul_information_local_nonprim = OpenAPI_satellite_backhaul_information_parseFromJSON(satellite_backhaul_information);
    if (!satellite_backhaul_information_local_nonprim) {
        ogs_error("OpenAPI_satellite_backhaul_information_parseFromJSON failed [satellite_backhaul_information]");
        goto end;
    }
    }

    service_level_aa = cJSON_GetObjectItemCaseSensitive(pdu_session_informationJSON, "serviceLevelAA");
    if (service_level_aa) {
    if (!cJSON_IsString(service_level_aa) && !cJSON_IsNull(service_level_aa)) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON() failed [service_level_aa]");
        goto end;
    }
    }

    pdu_session_information_local_var = OpenAPI_pdu_session_information_create (
        network_slicing_info ? network_slicing_info_local_nonprim : NULL,
        
        pdu_session_id->valuedouble,
        pdu_type ? pdu_typeVariable : 0,
        ssc_mode ? ssc_modeVariable : 0,
        h_plmn_id ? h_plmn_id_local_nonprim : NULL,
        serving_network_function_id ? serving_network_function_id_local_nonprim : NULL,
        rat_type ? rat_typeVariable : 0,
        m_apdu_non3_gpprat_type ? m_apdu_non3_gpprat_typeVariable : 0,
        ogs_strdup(dnn_id->valuestring),
        dnn_selection_mode ? dnn_selection_modeVariable : 0,
        charging_characteristics && !cJSON_IsNull(charging_characteristics) ? ogs_strdup(charging_characteristics->valuestring) : NULL,
        charging_characteristics_selection_mode ? charging_characteristics_selection_modeVariable : 0,
        start_time && !cJSON_IsNull(start_time) ? ogs_strdup(start_time->valuestring) : NULL,
        stop_time && !cJSON_IsNull(stop_time) ? ogs_strdup(stop_time->valuestring) : NULL,
        _3gpp_ps_data_off_status ? _3gpp_ps_data_off_statusVariable : 0,
        session_stop_indicator ? true : false,
        session_stop_indicator ? session_stop_indicator->valueint : 0,
        pdu_address ? pdu_address_local_nonprim : NULL,
        diagnostics ? true : false,
        diagnostics ? diagnostics->valuedouble : 0,
        authorized_qo_s_information ? authorized_qo_s_information_local_nonprim : NULL,
        subscribed_qo_s_information ? subscribed_qo_s_information_local_nonprim : NULL,
        authorized_session_ambr ? authorized_session_ambr_local_nonprim : NULL,
        subscribed_session_ambr ? subscribed_session_ambr_local_nonprim : NULL,
        serving_cn_plmn_id ? serving_cn_plmn_id_local_nonprim : NULL,
        m_apdu_session_information ? m_apdu_session_information_local_nonprim : NULL,
        enhanced_diagnostics ? enhanced_diagnosticsList : NULL,
        redundant_transmission_type ? redundant_transmission_typeVariable : 0,
        p_du_session_pair_id ? true : false,
        p_du_session_pair_id ? p_du_session_pair_id->valuedouble : 0,
        cp_cio_t_optimisation_indicator ? true : false,
        cp_cio_t_optimisation_indicator ? cp_cio_t_optimisation_indicator->valueint : 0,
        _5_gs_control_plane_only_indicator ? true : false,
        _5_gs_control_plane_only_indicator ? _5_gs_control_plane_only_indicator->valueint : 0,
        small_data_rate_control_indicator ? true : false,
        small_data_rate_control_indicator ? small_data_rate_control_indicator->valueint : 0,
        _5_glan_type_service ? _5_glan_type_service_local_nonprim : NULL,
        s_npn_information ? s_npn_information_local_nonprim : NULL,
        _5_g_multicast_service ? _5_g_multicast_service_local_nonprim : NULL,
        _5_gs_bridge_information ? _5_gs_bridge_information_local_nonprim : NULL,
        satellite_access_indicator ? true : false,
        satellite_access_indicator ? satellite_access_indicator->valueint : 0,
        satellite_backhaul_information ? satellite_backhaul_information_local_nonprim : NULL,
        service_level_aa && !cJSON_IsNull(service_level_aa) ? ogs_strdup(service_level_aa->valuestring) : NULL
    );

    return pdu_session_information_local_var;
end:
    if (network_slicing_info_local_nonprim) {
        OpenAPI_network_slicing_info_free(network_slicing_info_local_nonprim);
        network_slicing_info_local_nonprim = NULL;
    }
    if (h_plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(h_plmn_id_local_nonprim);
        h_plmn_id_local_nonprim = NULL;
    }
    if (serving_network_function_id_local_nonprim) {
        OpenAPI_serving_network_function_id_free(serving_network_function_id_local_nonprim);
        serving_network_function_id_local_nonprim = NULL;
    }
    if (pdu_address_local_nonprim) {
        OpenAPI_pdu_address_free(pdu_address_local_nonprim);
        pdu_address_local_nonprim = NULL;
    }
    if (authorized_qo_s_information_local_nonprim) {
        OpenAPI_authorized_default_qos_free(authorized_qo_s_information_local_nonprim);
        authorized_qo_s_information_local_nonprim = NULL;
    }
    if (subscribed_qo_s_information_local_nonprim) {
        OpenAPI_subscribed_default_qos_free(subscribed_qo_s_information_local_nonprim);
        subscribed_qo_s_information_local_nonprim = NULL;
    }
    if (authorized_session_ambr_local_nonprim) {
        OpenAPI_ambr_free(authorized_session_ambr_local_nonprim);
        authorized_session_ambr_local_nonprim = NULL;
    }
    if (subscribed_session_ambr_local_nonprim) {
        OpenAPI_ambr_free(subscribed_session_ambr_local_nonprim);
        subscribed_session_ambr_local_nonprim = NULL;
    }
    if (serving_cn_plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(serving_cn_plmn_id_local_nonprim);
        serving_cn_plmn_id_local_nonprim = NULL;
    }
    if (m_apdu_session_information_local_nonprim) {
        OpenAPI_mapdu_session_information_free(m_apdu_session_information_local_nonprim);
        m_apdu_session_information_local_nonprim = NULL;
    }
    if (enhanced_diagnosticsList) {
        OpenAPI_list_for_each(enhanced_diagnosticsList, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(enhanced_diagnosticsList);
        enhanced_diagnosticsList = NULL;
    }
    if (_5_glan_type_service_local_nonprim) {
        OpenAPI_model_5_glan_type_service_free(_5_glan_type_service_local_nonprim);
        _5_glan_type_service_local_nonprim = NULL;
    }
    if (s_npn_information_local_nonprim) {
        OpenAPI_snpn_information_free(s_npn_information_local_nonprim);
        s_npn_information_local_nonprim = NULL;
    }
    if (_5_g_multicast_service_local_nonprim) {
        OpenAPI_model_5_g_multicast_service_free(_5_g_multicast_service_local_nonprim);
        _5_g_multicast_service_local_nonprim = NULL;
    }
    if (_5_gs_bridge_information_local_nonprim) {
        OpenAPI_model_5_gs_bridge_information_free(_5_gs_bridge_information_local_nonprim);
        _5_gs_bridge_information_local_nonprim = NULL;
    }
    if (satellite_backhaul_information_local_nonprim) {
        OpenAPI_satellite_backhaul_information_free(satellite_backhaul_information_local_nonprim);
        satellite_backhaul_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_information_t *OpenAPI_pdu_session_information_copy(OpenAPI_pdu_session_information_t *dst, OpenAPI_pdu_session_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_information_convertToJSON() failed");
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

    OpenAPI_pdu_session_information_free(dst);
    dst = OpenAPI_pdu_session_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

