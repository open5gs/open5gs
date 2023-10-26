
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_and_mobility_subscription_data_1.h"

OpenAPI_access_and_mobility_subscription_data_1_t *OpenAPI_access_and_mobility_subscription_data_1_create(
    char *supported_features,
    OpenAPI_list_t *gpsis,
    char *hss_group_id,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* shared_vn_group_data_ids,
    OpenAPI_ambr_rm_t *subscribed_ue_ambr,
    bool is_nssai_null,
    OpenAPI_nssai_1_t *nssai,
    OpenAPI_set_t *rat_restrictions,
    OpenAPI_list_t *forbidden_areas,
    OpenAPI_service_area_restriction_1_t *service_area_restriction,
    OpenAPI_list_t *core_network_type_restrictions,
    bool is_rfsp_index_null,
    bool is_rfsp_index,
    int rfsp_index,
    bool is_subs_reg_timer_null,
    bool is_subs_reg_timer,
    int subs_reg_timer,
    bool is_ue_usage_type,
    int ue_usage_type,
    bool is_mps_priority,
    int mps_priority,
    bool is_mcs_priority,
    int mcs_priority,
    bool is_active_time_null,
    bool is_active_time,
    int active_time,
    OpenAPI_sor_info_1_t *sor_info,
    bool is_sor_info_expect_ind,
    int sor_info_expect_ind,
    bool is_soraf_retrieval,
    int soraf_retrieval,
    OpenAPI_list_t *sor_update_indicator_list,
    OpenAPI_upu_info_1_t *upu_info,
    char *routing_indicator,
    bool is_mico_allowed,
    int mico_allowed,
    OpenAPI_list_t *shared_am_data_ids,
    OpenAPI_odb_packet_services_e odb_packet_services,
    OpenAPI_list_t *subscribed_dnn_list,
    bool is_service_gap_time,
    int service_gap_time,
    OpenAPI_mdt_user_consent_e mdt_user_consent,
    OpenAPI_mdt_configuration_1_t *mdt_configuration,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_cag_data_1_t *cag_data,
    char *stn_sr,
    char *c_msisdn,
    bool is_nb_io_tue_priority,
    int nb_io_tue_priority,
    bool is_nssai_inclusion_allowed,
    int nssai_inclusion_allowed,
    char *rg_wireline_characteristics,
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb,
    bool is_ec_restriction_data_nb,
    int ec_restriction_data_nb,
    OpenAPI_expected_ue_behaviour_data_1_t *expected_ue_behaviour_list,
    OpenAPI_set_t *primary_rat_restrictions,
    OpenAPI_set_t *secondary_rat_restrictions,
    OpenAPI_list_t *edrx_parameters_list,
    OpenAPI_list_t *ptw_parameters_list,
    bool is_iab_operation_allowed,
    int iab_operation_allowed,
    OpenAPI_list_t* adjacent_plmn_restrictions,
    OpenAPI_list_t *wireline_forbidden_areas,
    OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction,
    OpenAPI_list_t *pcf_selection_assistance_infos,
    OpenAPI_aerial_ue_subscription_info_1_t *aerial_ue_sub_info,
    OpenAPI_roaming_restrictions_t *roaming_restrictions,
    bool is_remote_prov_ind,
    int remote_prov_ind
)
{
    OpenAPI_access_and_mobility_subscription_data_1_t *access_and_mobility_subscription_data_1_local_var = ogs_malloc(sizeof(OpenAPI_access_and_mobility_subscription_data_1_t));
    ogs_assert(access_and_mobility_subscription_data_1_local_var);

    access_and_mobility_subscription_data_1_local_var->supported_features = supported_features;
    access_and_mobility_subscription_data_1_local_var->gpsis = gpsis;
    access_and_mobility_subscription_data_1_local_var->hss_group_id = hss_group_id;
    access_and_mobility_subscription_data_1_local_var->internal_group_ids = internal_group_ids;
    access_and_mobility_subscription_data_1_local_var->shared_vn_group_data_ids = shared_vn_group_data_ids;
    access_and_mobility_subscription_data_1_local_var->subscribed_ue_ambr = subscribed_ue_ambr;
    access_and_mobility_subscription_data_1_local_var->is_nssai_null = is_nssai_null;
    access_and_mobility_subscription_data_1_local_var->nssai = nssai;
    access_and_mobility_subscription_data_1_local_var->rat_restrictions = rat_restrictions;
    access_and_mobility_subscription_data_1_local_var->forbidden_areas = forbidden_areas;
    access_and_mobility_subscription_data_1_local_var->service_area_restriction = service_area_restriction;
    access_and_mobility_subscription_data_1_local_var->core_network_type_restrictions = core_network_type_restrictions;
    access_and_mobility_subscription_data_1_local_var->is_rfsp_index_null = is_rfsp_index_null;
    access_and_mobility_subscription_data_1_local_var->is_rfsp_index = is_rfsp_index;
    access_and_mobility_subscription_data_1_local_var->rfsp_index = rfsp_index;
    access_and_mobility_subscription_data_1_local_var->is_subs_reg_timer_null = is_subs_reg_timer_null;
    access_and_mobility_subscription_data_1_local_var->is_subs_reg_timer = is_subs_reg_timer;
    access_and_mobility_subscription_data_1_local_var->subs_reg_timer = subs_reg_timer;
    access_and_mobility_subscription_data_1_local_var->is_ue_usage_type = is_ue_usage_type;
    access_and_mobility_subscription_data_1_local_var->ue_usage_type = ue_usage_type;
    access_and_mobility_subscription_data_1_local_var->is_mps_priority = is_mps_priority;
    access_and_mobility_subscription_data_1_local_var->mps_priority = mps_priority;
    access_and_mobility_subscription_data_1_local_var->is_mcs_priority = is_mcs_priority;
    access_and_mobility_subscription_data_1_local_var->mcs_priority = mcs_priority;
    access_and_mobility_subscription_data_1_local_var->is_active_time_null = is_active_time_null;
    access_and_mobility_subscription_data_1_local_var->is_active_time = is_active_time;
    access_and_mobility_subscription_data_1_local_var->active_time = active_time;
    access_and_mobility_subscription_data_1_local_var->sor_info = sor_info;
    access_and_mobility_subscription_data_1_local_var->is_sor_info_expect_ind = is_sor_info_expect_ind;
    access_and_mobility_subscription_data_1_local_var->sor_info_expect_ind = sor_info_expect_ind;
    access_and_mobility_subscription_data_1_local_var->is_soraf_retrieval = is_soraf_retrieval;
    access_and_mobility_subscription_data_1_local_var->soraf_retrieval = soraf_retrieval;
    access_and_mobility_subscription_data_1_local_var->sor_update_indicator_list = sor_update_indicator_list;
    access_and_mobility_subscription_data_1_local_var->upu_info = upu_info;
    access_and_mobility_subscription_data_1_local_var->routing_indicator = routing_indicator;
    access_and_mobility_subscription_data_1_local_var->is_mico_allowed = is_mico_allowed;
    access_and_mobility_subscription_data_1_local_var->mico_allowed = mico_allowed;
    access_and_mobility_subscription_data_1_local_var->shared_am_data_ids = shared_am_data_ids;
    access_and_mobility_subscription_data_1_local_var->odb_packet_services = odb_packet_services;
    access_and_mobility_subscription_data_1_local_var->subscribed_dnn_list = subscribed_dnn_list;
    access_and_mobility_subscription_data_1_local_var->is_service_gap_time = is_service_gap_time;
    access_and_mobility_subscription_data_1_local_var->service_gap_time = service_gap_time;
    access_and_mobility_subscription_data_1_local_var->mdt_user_consent = mdt_user_consent;
    access_and_mobility_subscription_data_1_local_var->mdt_configuration = mdt_configuration;
    access_and_mobility_subscription_data_1_local_var->is_trace_data_null = is_trace_data_null;
    access_and_mobility_subscription_data_1_local_var->trace_data = trace_data;
    access_and_mobility_subscription_data_1_local_var->cag_data = cag_data;
    access_and_mobility_subscription_data_1_local_var->stn_sr = stn_sr;
    access_and_mobility_subscription_data_1_local_var->c_msisdn = c_msisdn;
    access_and_mobility_subscription_data_1_local_var->is_nb_io_tue_priority = is_nb_io_tue_priority;
    access_and_mobility_subscription_data_1_local_var->nb_io_tue_priority = nb_io_tue_priority;
    access_and_mobility_subscription_data_1_local_var->is_nssai_inclusion_allowed = is_nssai_inclusion_allowed;
    access_and_mobility_subscription_data_1_local_var->nssai_inclusion_allowed = nssai_inclusion_allowed;
    access_and_mobility_subscription_data_1_local_var->rg_wireline_characteristics = rg_wireline_characteristics;
    access_and_mobility_subscription_data_1_local_var->ec_restriction_data_wb = ec_restriction_data_wb;
    access_and_mobility_subscription_data_1_local_var->is_ec_restriction_data_nb = is_ec_restriction_data_nb;
    access_and_mobility_subscription_data_1_local_var->ec_restriction_data_nb = ec_restriction_data_nb;
    access_and_mobility_subscription_data_1_local_var->expected_ue_behaviour_list = expected_ue_behaviour_list;
    access_and_mobility_subscription_data_1_local_var->primary_rat_restrictions = primary_rat_restrictions;
    access_and_mobility_subscription_data_1_local_var->secondary_rat_restrictions = secondary_rat_restrictions;
    access_and_mobility_subscription_data_1_local_var->edrx_parameters_list = edrx_parameters_list;
    access_and_mobility_subscription_data_1_local_var->ptw_parameters_list = ptw_parameters_list;
    access_and_mobility_subscription_data_1_local_var->is_iab_operation_allowed = is_iab_operation_allowed;
    access_and_mobility_subscription_data_1_local_var->iab_operation_allowed = iab_operation_allowed;
    access_and_mobility_subscription_data_1_local_var->adjacent_plmn_restrictions = adjacent_plmn_restrictions;
    access_and_mobility_subscription_data_1_local_var->wireline_forbidden_areas = wireline_forbidden_areas;
    access_and_mobility_subscription_data_1_local_var->wireline_service_area_restriction = wireline_service_area_restriction;
    access_and_mobility_subscription_data_1_local_var->pcf_selection_assistance_infos = pcf_selection_assistance_infos;
    access_and_mobility_subscription_data_1_local_var->aerial_ue_sub_info = aerial_ue_sub_info;
    access_and_mobility_subscription_data_1_local_var->roaming_restrictions = roaming_restrictions;
    access_and_mobility_subscription_data_1_local_var->is_remote_prov_ind = is_remote_prov_ind;
    access_and_mobility_subscription_data_1_local_var->remote_prov_ind = remote_prov_ind;

    return access_and_mobility_subscription_data_1_local_var;
}

void OpenAPI_access_and_mobility_subscription_data_1_free(OpenAPI_access_and_mobility_subscription_data_1_t *access_and_mobility_subscription_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_and_mobility_subscription_data_1) {
        return;
    }
    if (access_and_mobility_subscription_data_1->supported_features) {
        ogs_free(access_and_mobility_subscription_data_1->supported_features);
        access_and_mobility_subscription_data_1->supported_features = NULL;
    }
    if (access_and_mobility_subscription_data_1->gpsis) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->gpsis);
        access_and_mobility_subscription_data_1->gpsis = NULL;
    }
    if (access_and_mobility_subscription_data_1->hss_group_id) {
        ogs_free(access_and_mobility_subscription_data_1->hss_group_id);
        access_and_mobility_subscription_data_1->hss_group_id = NULL;
    }
    if (access_and_mobility_subscription_data_1->internal_group_ids) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->internal_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->internal_group_ids);
        access_and_mobility_subscription_data_1->internal_group_ids = NULL;
    }
    if (access_and_mobility_subscription_data_1->shared_vn_group_data_ids) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->shared_vn_group_data_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->shared_vn_group_data_ids);
        access_and_mobility_subscription_data_1->shared_vn_group_data_ids = NULL;
    }
    if (access_and_mobility_subscription_data_1->subscribed_ue_ambr) {
        OpenAPI_ambr_rm_free(access_and_mobility_subscription_data_1->subscribed_ue_ambr);
        access_and_mobility_subscription_data_1->subscribed_ue_ambr = NULL;
    }
    if (access_and_mobility_subscription_data_1->nssai) {
        OpenAPI_nssai_1_free(access_and_mobility_subscription_data_1->nssai);
        access_and_mobility_subscription_data_1->nssai = NULL;
    }
    if (access_and_mobility_subscription_data_1->rat_restrictions) {
        OpenAPI_list_free(access_and_mobility_subscription_data_1->rat_restrictions);
        access_and_mobility_subscription_data_1->rat_restrictions = NULL;
    }
    if (access_and_mobility_subscription_data_1->forbidden_areas) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->forbidden_areas, node) {
            OpenAPI_area_1_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->forbidden_areas);
        access_and_mobility_subscription_data_1->forbidden_areas = NULL;
    }
    if (access_and_mobility_subscription_data_1->service_area_restriction) {
        OpenAPI_service_area_restriction_1_free(access_and_mobility_subscription_data_1->service_area_restriction);
        access_and_mobility_subscription_data_1->service_area_restriction = NULL;
    }
    if (access_and_mobility_subscription_data_1->core_network_type_restrictions) {
        OpenAPI_list_free(access_and_mobility_subscription_data_1->core_network_type_restrictions);
        access_and_mobility_subscription_data_1->core_network_type_restrictions = NULL;
    }
    if (access_and_mobility_subscription_data_1->sor_info) {
        OpenAPI_sor_info_1_free(access_and_mobility_subscription_data_1->sor_info);
        access_and_mobility_subscription_data_1->sor_info = NULL;
    }
    if (access_and_mobility_subscription_data_1->sor_update_indicator_list) {
        OpenAPI_list_free(access_and_mobility_subscription_data_1->sor_update_indicator_list);
        access_and_mobility_subscription_data_1->sor_update_indicator_list = NULL;
    }
    if (access_and_mobility_subscription_data_1->upu_info) {
        OpenAPI_upu_info_1_free(access_and_mobility_subscription_data_1->upu_info);
        access_and_mobility_subscription_data_1->upu_info = NULL;
    }
    if (access_and_mobility_subscription_data_1->routing_indicator) {
        ogs_free(access_and_mobility_subscription_data_1->routing_indicator);
        access_and_mobility_subscription_data_1->routing_indicator = NULL;
    }
    if (access_and_mobility_subscription_data_1->shared_am_data_ids) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->shared_am_data_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->shared_am_data_ids);
        access_and_mobility_subscription_data_1->shared_am_data_ids = NULL;
    }
    if (access_and_mobility_subscription_data_1->subscribed_dnn_list) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->subscribed_dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->subscribed_dnn_list);
        access_and_mobility_subscription_data_1->subscribed_dnn_list = NULL;
    }
    if (access_and_mobility_subscription_data_1->mdt_configuration) {
        OpenAPI_mdt_configuration_1_free(access_and_mobility_subscription_data_1->mdt_configuration);
        access_and_mobility_subscription_data_1->mdt_configuration = NULL;
    }
    if (access_and_mobility_subscription_data_1->trace_data) {
        OpenAPI_trace_data_free(access_and_mobility_subscription_data_1->trace_data);
        access_and_mobility_subscription_data_1->trace_data = NULL;
    }
    if (access_and_mobility_subscription_data_1->cag_data) {
        OpenAPI_cag_data_1_free(access_and_mobility_subscription_data_1->cag_data);
        access_and_mobility_subscription_data_1->cag_data = NULL;
    }
    if (access_and_mobility_subscription_data_1->stn_sr) {
        ogs_free(access_and_mobility_subscription_data_1->stn_sr);
        access_and_mobility_subscription_data_1->stn_sr = NULL;
    }
    if (access_and_mobility_subscription_data_1->c_msisdn) {
        ogs_free(access_and_mobility_subscription_data_1->c_msisdn);
        access_and_mobility_subscription_data_1->c_msisdn = NULL;
    }
    if (access_and_mobility_subscription_data_1->rg_wireline_characteristics) {
        ogs_free(access_and_mobility_subscription_data_1->rg_wireline_characteristics);
        access_and_mobility_subscription_data_1->rg_wireline_characteristics = NULL;
    }
    if (access_and_mobility_subscription_data_1->ec_restriction_data_wb) {
        OpenAPI_ec_restriction_data_wb_free(access_and_mobility_subscription_data_1->ec_restriction_data_wb);
        access_and_mobility_subscription_data_1->ec_restriction_data_wb = NULL;
    }
    if (access_and_mobility_subscription_data_1->expected_ue_behaviour_list) {
        OpenAPI_expected_ue_behaviour_data_1_free(access_and_mobility_subscription_data_1->expected_ue_behaviour_list);
        access_and_mobility_subscription_data_1->expected_ue_behaviour_list = NULL;
    }
    if (access_and_mobility_subscription_data_1->primary_rat_restrictions) {
        OpenAPI_list_free(access_and_mobility_subscription_data_1->primary_rat_restrictions);
        access_and_mobility_subscription_data_1->primary_rat_restrictions = NULL;
    }
    if (access_and_mobility_subscription_data_1->secondary_rat_restrictions) {
        OpenAPI_list_free(access_and_mobility_subscription_data_1->secondary_rat_restrictions);
        access_and_mobility_subscription_data_1->secondary_rat_restrictions = NULL;
    }
    if (access_and_mobility_subscription_data_1->edrx_parameters_list) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->edrx_parameters_list, node) {
            OpenAPI_edrx_parameters_1_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->edrx_parameters_list);
        access_and_mobility_subscription_data_1->edrx_parameters_list = NULL;
    }
    if (access_and_mobility_subscription_data_1->ptw_parameters_list) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->ptw_parameters_list, node) {
            OpenAPI_ptw_parameters_1_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->ptw_parameters_list);
        access_and_mobility_subscription_data_1->ptw_parameters_list = NULL;
    }
    if (access_and_mobility_subscription_data_1->adjacent_plmn_restrictions) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->adjacent_plmn_restrictions, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_plmn_restriction_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->adjacent_plmn_restrictions);
        access_and_mobility_subscription_data_1->adjacent_plmn_restrictions = NULL;
    }
    if (access_and_mobility_subscription_data_1->wireline_forbidden_areas) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->wireline_forbidden_areas, node) {
            OpenAPI_wireline_area_1_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->wireline_forbidden_areas);
        access_and_mobility_subscription_data_1->wireline_forbidden_areas = NULL;
    }
    if (access_and_mobility_subscription_data_1->wireline_service_area_restriction) {
        OpenAPI_wireline_service_area_restriction_1_free(access_and_mobility_subscription_data_1->wireline_service_area_restriction);
        access_and_mobility_subscription_data_1->wireline_service_area_restriction = NULL;
    }
    if (access_and_mobility_subscription_data_1->pcf_selection_assistance_infos) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->pcf_selection_assistance_infos, node) {
            OpenAPI_pcf_selection_assistance_info_1_free(node->data);
        }
        OpenAPI_list_free(access_and_mobility_subscription_data_1->pcf_selection_assistance_infos);
        access_and_mobility_subscription_data_1->pcf_selection_assistance_infos = NULL;
    }
    if (access_and_mobility_subscription_data_1->aerial_ue_sub_info) {
        OpenAPI_aerial_ue_subscription_info_1_free(access_and_mobility_subscription_data_1->aerial_ue_sub_info);
        access_and_mobility_subscription_data_1->aerial_ue_sub_info = NULL;
    }
    if (access_and_mobility_subscription_data_1->roaming_restrictions) {
        OpenAPI_roaming_restrictions_free(access_and_mobility_subscription_data_1->roaming_restrictions);
        access_and_mobility_subscription_data_1->roaming_restrictions = NULL;
    }
    ogs_free(access_and_mobility_subscription_data_1);
}

cJSON *OpenAPI_access_and_mobility_subscription_data_1_convertToJSON(OpenAPI_access_and_mobility_subscription_data_1_t *access_and_mobility_subscription_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_and_mobility_subscription_data_1 == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [AccessAndMobilitySubscriptionData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (access_and_mobility_subscription_data_1->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", access_and_mobility_subscription_data_1->supported_features) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->gpsis) {
    cJSON *gpsisList = cJSON_AddArrayToObject(item, "gpsis");
    if (gpsisList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->gpsis, node) {
        if (cJSON_AddStringToObject(gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [gpsis]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->hss_group_id) {
    if (cJSON_AddStringToObject(item, "hssGroupId", access_and_mobility_subscription_data_1->hss_group_id) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [hss_group_id]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->internal_group_ids) {
    cJSON *internal_group_idsList = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_idsList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->internal_group_ids, node) {
        if (cJSON_AddStringToObject(internal_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [internal_group_ids]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->shared_vn_group_data_ids) {
    cJSON *shared_vn_group_data_ids = cJSON_AddObjectToObject(item, "sharedVnGroupDataIds");
    if (shared_vn_group_data_ids == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [shared_vn_group_data_ids]");
        goto end;
    }
    cJSON *localMapObject = shared_vn_group_data_ids;
    if (access_and_mobility_subscription_data_1->shared_vn_group_data_ids) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->shared_vn_group_data_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [shared_vn_group_data_ids]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [shared_vn_group_data_ids]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, (char*)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (access_and_mobility_subscription_data_1->subscribed_ue_ambr) {
    cJSON *subscribed_ue_ambr_local_JSON = OpenAPI_ambr_rm_convertToJSON(access_and_mobility_subscription_data_1->subscribed_ue_ambr);
    if (subscribed_ue_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [subscribed_ue_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscribedUeAmbr", subscribed_ue_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [subscribed_ue_ambr]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->nssai) {
    cJSON *nssai_local_JSON = OpenAPI_nssai_1_convertToJSON(access_and_mobility_subscription_data_1->nssai);
    if (nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nssai", nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [nssai]");
        goto end;
    }
    } else if (access_and_mobility_subscription_data_1->is_nssai_null) {
        if (cJSON_AddNullToObject(item, "nssai") == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [nssai]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data_1->rat_restrictions != OpenAPI_rat_type_NULL) {
    cJSON *rat_restrictionsList = cJSON_AddArrayToObject(item, "ratRestrictions");
    if (rat_restrictionsList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [rat_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->rat_restrictions, node) {
        if (cJSON_AddStringToObject(rat_restrictionsList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [rat_restrictions]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->forbidden_areas) {
    cJSON *forbidden_areasList = cJSON_AddArrayToObject(item, "forbiddenAreas");
    if (forbidden_areasList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [forbidden_areas]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->forbidden_areas, node) {
        cJSON *itemLocal = OpenAPI_area_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [forbidden_areas]");
            goto end;
        }
        cJSON_AddItemToArray(forbidden_areasList, itemLocal);
    }
    }

    if (access_and_mobility_subscription_data_1->service_area_restriction) {
    cJSON *service_area_restriction_local_JSON = OpenAPI_service_area_restriction_1_convertToJSON(access_and_mobility_subscription_data_1->service_area_restriction);
    if (service_area_restriction_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "serviceAreaRestriction", service_area_restriction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->core_network_type_restrictions != OpenAPI_core_network_type_NULL) {
    cJSON *core_network_type_restrictionsList = cJSON_AddArrayToObject(item, "coreNetworkTypeRestrictions");
    if (core_network_type_restrictionsList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [core_network_type_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->core_network_type_restrictions, node) {
        if (cJSON_AddStringToObject(core_network_type_restrictionsList, "", OpenAPI_core_network_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [core_network_type_restrictions]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->is_rfsp_index) {
    if (cJSON_AddNumberToObject(item, "rfspIndex", access_and_mobility_subscription_data_1->rfsp_index) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [rfsp_index]");
        goto end;
    }
    } else if (access_and_mobility_subscription_data_1->is_rfsp_index_null) {
        if (cJSON_AddNullToObject(item, "rfspIndex") == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [rfsp_index]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data_1->is_subs_reg_timer) {
    if (cJSON_AddNumberToObject(item, "subsRegTimer", access_and_mobility_subscription_data_1->subs_reg_timer) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [subs_reg_timer]");
        goto end;
    }
    } else if (access_and_mobility_subscription_data_1->is_subs_reg_timer_null) {
        if (cJSON_AddNullToObject(item, "subsRegTimer") == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [subs_reg_timer]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data_1->is_ue_usage_type) {
    if (cJSON_AddNumberToObject(item, "ueUsageType", access_and_mobility_subscription_data_1->ue_usage_type) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [ue_usage_type]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_mps_priority) {
    if (cJSON_AddBoolToObject(item, "mpsPriority", access_and_mobility_subscription_data_1->mps_priority) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [mps_priority]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_mcs_priority) {
    if (cJSON_AddBoolToObject(item, "mcsPriority", access_and_mobility_subscription_data_1->mcs_priority) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [mcs_priority]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_active_time) {
    if (cJSON_AddNumberToObject(item, "activeTime", access_and_mobility_subscription_data_1->active_time) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [active_time]");
        goto end;
    }
    } else if (access_and_mobility_subscription_data_1->is_active_time_null) {
        if (cJSON_AddNullToObject(item, "activeTime") == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [active_time]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data_1->sor_info) {
    cJSON *sor_info_local_JSON = OpenAPI_sor_info_1_convertToJSON(access_and_mobility_subscription_data_1->sor_info);
    if (sor_info_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [sor_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sorInfo", sor_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [sor_info]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_sor_info_expect_ind) {
    if (cJSON_AddBoolToObject(item, "sorInfoExpectInd", access_and_mobility_subscription_data_1->sor_info_expect_ind) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [sor_info_expect_ind]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_soraf_retrieval) {
    if (cJSON_AddBoolToObject(item, "sorafRetrieval", access_and_mobility_subscription_data_1->soraf_retrieval) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [soraf_retrieval]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->sor_update_indicator_list != OpenAPI_sor_update_indicator_NULL) {
    cJSON *sor_update_indicator_listList = cJSON_AddArrayToObject(item, "sorUpdateIndicatorList");
    if (sor_update_indicator_listList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [sor_update_indicator_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->sor_update_indicator_list, node) {
        if (cJSON_AddStringToObject(sor_update_indicator_listList, "", OpenAPI_sor_update_indicator_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [sor_update_indicator_list]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->upu_info) {
    cJSON *upu_info_local_JSON = OpenAPI_upu_info_1_convertToJSON(access_and_mobility_subscription_data_1->upu_info);
    if (upu_info_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [upu_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upuInfo", upu_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [upu_info]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", access_and_mobility_subscription_data_1->routing_indicator) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_mico_allowed) {
    if (cJSON_AddBoolToObject(item, "micoAllowed", access_and_mobility_subscription_data_1->mico_allowed) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [mico_allowed]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->shared_am_data_ids) {
    cJSON *shared_am_data_idsList = cJSON_AddArrayToObject(item, "sharedAmDataIds");
    if (shared_am_data_idsList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [shared_am_data_ids]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->shared_am_data_ids, node) {
        if (cJSON_AddStringToObject(shared_am_data_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [shared_am_data_ids]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->odb_packet_services != OpenAPI_odb_packet_services_NULL) {
    if (cJSON_AddStringToObject(item, "odbPacketServices", OpenAPI_odb_packet_services_ToString(access_and_mobility_subscription_data_1->odb_packet_services)) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [odb_packet_services]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->subscribed_dnn_list) {
    cJSON *subscribed_dnn_listList = cJSON_AddArrayToObject(item, "subscribedDnnList");
    if (subscribed_dnn_listList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [subscribed_dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->subscribed_dnn_list, node) {
        if (cJSON_AddStringToObject(subscribed_dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [subscribed_dnn_list]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->is_service_gap_time) {
    if (cJSON_AddNumberToObject(item, "serviceGapTime", access_and_mobility_subscription_data_1->service_gap_time) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [service_gap_time]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->mdt_user_consent != OpenAPI_mdt_user_consent_NULL) {
    if (cJSON_AddStringToObject(item, "mdtUserConsent", OpenAPI_mdt_user_consent_ToString(access_and_mobility_subscription_data_1->mdt_user_consent)) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [mdt_user_consent]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->mdt_configuration) {
    cJSON *mdt_configuration_local_JSON = OpenAPI_mdt_configuration_1_convertToJSON(access_and_mobility_subscription_data_1->mdt_configuration);
    if (mdt_configuration_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [mdt_configuration]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mdtConfiguration", mdt_configuration_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [mdt_configuration]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(access_and_mobility_subscription_data_1->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (access_and_mobility_subscription_data_1->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data_1->cag_data) {
    cJSON *cag_data_local_JSON = OpenAPI_cag_data_1_convertToJSON(access_and_mobility_subscription_data_1->cag_data);
    if (cag_data_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [cag_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cagData", cag_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [cag_data]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->stn_sr) {
    if (cJSON_AddStringToObject(item, "stnSr", access_and_mobility_subscription_data_1->stn_sr) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [stn_sr]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->c_msisdn) {
    if (cJSON_AddStringToObject(item, "cMsisdn", access_and_mobility_subscription_data_1->c_msisdn) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [c_msisdn]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_nb_io_tue_priority) {
    if (cJSON_AddNumberToObject(item, "nbIoTUePriority", access_and_mobility_subscription_data_1->nb_io_tue_priority) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [nb_io_tue_priority]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_nssai_inclusion_allowed) {
    if (cJSON_AddBoolToObject(item, "nssaiInclusionAllowed", access_and_mobility_subscription_data_1->nssai_inclusion_allowed) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [nssai_inclusion_allowed]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->rg_wireline_characteristics) {
    if (cJSON_AddStringToObject(item, "rgWirelineCharacteristics", access_and_mobility_subscription_data_1->rg_wireline_characteristics) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [rg_wireline_characteristics]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->ec_restriction_data_wb) {
    cJSON *ec_restriction_data_wb_local_JSON = OpenAPI_ec_restriction_data_wb_convertToJSON(access_and_mobility_subscription_data_1->ec_restriction_data_wb);
    if (ec_restriction_data_wb_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [ec_restriction_data_wb]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecRestrictionDataWb", ec_restriction_data_wb_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [ec_restriction_data_wb]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_ec_restriction_data_nb) {
    if (cJSON_AddBoolToObject(item, "ecRestrictionDataNb", access_and_mobility_subscription_data_1->ec_restriction_data_nb) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [ec_restriction_data_nb]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->expected_ue_behaviour_list) {
    cJSON *expected_ue_behaviour_list_local_JSON = OpenAPI_expected_ue_behaviour_data_1_convertToJSON(access_and_mobility_subscription_data_1->expected_ue_behaviour_list);
    if (expected_ue_behaviour_list_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [expected_ue_behaviour_list]");
        goto end;
    }
    cJSON_AddItemToObject(item, "expectedUeBehaviourList", expected_ue_behaviour_list_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [expected_ue_behaviour_list]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->primary_rat_restrictions != OpenAPI_rat_type_NULL) {
    cJSON *primary_rat_restrictionsList = cJSON_AddArrayToObject(item, "primaryRatRestrictions");
    if (primary_rat_restrictionsList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [primary_rat_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->primary_rat_restrictions, node) {
        if (cJSON_AddStringToObject(primary_rat_restrictionsList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [primary_rat_restrictions]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->secondary_rat_restrictions != OpenAPI_rat_type_NULL) {
    cJSON *secondary_rat_restrictionsList = cJSON_AddArrayToObject(item, "secondaryRatRestrictions");
    if (secondary_rat_restrictionsList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [secondary_rat_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->secondary_rat_restrictions, node) {
        if (cJSON_AddStringToObject(secondary_rat_restrictionsList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [secondary_rat_restrictions]");
            goto end;
        }
    }
    }

    if (access_and_mobility_subscription_data_1->edrx_parameters_list) {
    cJSON *edrx_parameters_listList = cJSON_AddArrayToObject(item, "edrxParametersList");
    if (edrx_parameters_listList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [edrx_parameters_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->edrx_parameters_list, node) {
        cJSON *itemLocal = OpenAPI_edrx_parameters_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [edrx_parameters_list]");
            goto end;
        }
        cJSON_AddItemToArray(edrx_parameters_listList, itemLocal);
    }
    }

    if (access_and_mobility_subscription_data_1->ptw_parameters_list) {
    cJSON *ptw_parameters_listList = cJSON_AddArrayToObject(item, "ptwParametersList");
    if (ptw_parameters_listList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [ptw_parameters_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->ptw_parameters_list, node) {
        cJSON *itemLocal = OpenAPI_ptw_parameters_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [ptw_parameters_list]");
            goto end;
        }
        cJSON_AddItemToArray(ptw_parameters_listList, itemLocal);
    }
    }

    if (access_and_mobility_subscription_data_1->is_iab_operation_allowed) {
    if (cJSON_AddBoolToObject(item, "iabOperationAllowed", access_and_mobility_subscription_data_1->iab_operation_allowed) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [iab_operation_allowed]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->adjacent_plmn_restrictions) {
    cJSON *adjacent_plmn_restrictions = cJSON_AddObjectToObject(item, "adjacentPlmnRestrictions");
    if (adjacent_plmn_restrictions == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [adjacent_plmn_restrictions]");
        goto end;
    }
    cJSON *localMapObject = adjacent_plmn_restrictions;
    if (access_and_mobility_subscription_data_1->adjacent_plmn_restrictions) {
        OpenAPI_list_for_each(access_and_mobility_subscription_data_1->adjacent_plmn_restrictions, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [adjacent_plmn_restrictions]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [adjacent_plmn_restrictions]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_plmn_restriction_1_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (access_and_mobility_subscription_data_1->wireline_forbidden_areas) {
    cJSON *wireline_forbidden_areasList = cJSON_AddArrayToObject(item, "wirelineForbiddenAreas");
    if (wireline_forbidden_areasList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [wireline_forbidden_areas]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->wireline_forbidden_areas, node) {
        cJSON *itemLocal = OpenAPI_wireline_area_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [wireline_forbidden_areas]");
            goto end;
        }
        cJSON_AddItemToArray(wireline_forbidden_areasList, itemLocal);
    }
    }

    if (access_and_mobility_subscription_data_1->wireline_service_area_restriction) {
    cJSON *wireline_service_area_restriction_local_JSON = OpenAPI_wireline_service_area_restriction_1_convertToJSON(access_and_mobility_subscription_data_1->wireline_service_area_restriction);
    if (wireline_service_area_restriction_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [wireline_service_area_restriction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wirelineServiceAreaRestriction", wireline_service_area_restriction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [wireline_service_area_restriction]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->pcf_selection_assistance_infos) {
    cJSON *pcf_selection_assistance_infosList = cJSON_AddArrayToObject(item, "pcfSelectionAssistanceInfos");
    if (pcf_selection_assistance_infosList == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [pcf_selection_assistance_infos]");
        goto end;
    }
    OpenAPI_list_for_each(access_and_mobility_subscription_data_1->pcf_selection_assistance_infos, node) {
        cJSON *itemLocal = OpenAPI_pcf_selection_assistance_info_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [pcf_selection_assistance_infos]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_selection_assistance_infosList, itemLocal);
    }
    }

    if (access_and_mobility_subscription_data_1->aerial_ue_sub_info) {
    cJSON *aerial_ue_sub_info_local_JSON = OpenAPI_aerial_ue_subscription_info_1_convertToJSON(access_and_mobility_subscription_data_1->aerial_ue_sub_info);
    if (aerial_ue_sub_info_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [aerial_ue_sub_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "aerialUeSubInfo", aerial_ue_sub_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [aerial_ue_sub_info]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->roaming_restrictions) {
    cJSON *roaming_restrictions_local_JSON = OpenAPI_roaming_restrictions_convertToJSON(access_and_mobility_subscription_data_1->roaming_restrictions);
    if (roaming_restrictions_local_JSON == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [roaming_restrictions]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingRestrictions", roaming_restrictions_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [roaming_restrictions]");
        goto end;
    }
    }

    if (access_and_mobility_subscription_data_1->is_remote_prov_ind) {
    if (cJSON_AddBoolToObject(item, "remoteProvInd", access_and_mobility_subscription_data_1->remote_prov_ind) == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed [remote_prov_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_access_and_mobility_subscription_data_1_t *OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON(cJSON *access_and_mobility_subscription_data_1JSON)
{
    OpenAPI_access_and_mobility_subscription_data_1_t *access_and_mobility_subscription_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supported_features = NULL;
    cJSON *gpsis = NULL;
    OpenAPI_list_t *gpsisList = NULL;
    cJSON *hss_group_id = NULL;
    cJSON *internal_group_ids = NULL;
    OpenAPI_list_t *internal_group_idsList = NULL;
    cJSON *shared_vn_group_data_ids = NULL;
    OpenAPI_list_t *shared_vn_group_data_idsList = NULL;
    cJSON *subscribed_ue_ambr = NULL;
    OpenAPI_ambr_rm_t *subscribed_ue_ambr_local_nonprim = NULL;
    cJSON *nssai = NULL;
    OpenAPI_nssai_1_t *nssai_local_nonprim = NULL;
    cJSON *rat_restrictions = NULL;
    OpenAPI_list_t *rat_restrictionsList = NULL;
    cJSON *forbidden_areas = NULL;
    OpenAPI_list_t *forbidden_areasList = NULL;
    cJSON *service_area_restriction = NULL;
    OpenAPI_service_area_restriction_1_t *service_area_restriction_local_nonprim = NULL;
    cJSON *core_network_type_restrictions = NULL;
    OpenAPI_list_t *core_network_type_restrictionsList = NULL;
    cJSON *rfsp_index = NULL;
    cJSON *subs_reg_timer = NULL;
    cJSON *ue_usage_type = NULL;
    cJSON *mps_priority = NULL;
    cJSON *mcs_priority = NULL;
    cJSON *active_time = NULL;
    cJSON *sor_info = NULL;
    OpenAPI_sor_info_1_t *sor_info_local_nonprim = NULL;
    cJSON *sor_info_expect_ind = NULL;
    cJSON *soraf_retrieval = NULL;
    cJSON *sor_update_indicator_list = NULL;
    OpenAPI_list_t *sor_update_indicator_listList = NULL;
    cJSON *upu_info = NULL;
    OpenAPI_upu_info_1_t *upu_info_local_nonprim = NULL;
    cJSON *routing_indicator = NULL;
    cJSON *mico_allowed = NULL;
    cJSON *shared_am_data_ids = NULL;
    OpenAPI_list_t *shared_am_data_idsList = NULL;
    cJSON *odb_packet_services = NULL;
    OpenAPI_odb_packet_services_e odb_packet_servicesVariable = 0;
    cJSON *subscribed_dnn_list = NULL;
    OpenAPI_list_t *subscribed_dnn_listList = NULL;
    cJSON *service_gap_time = NULL;
    cJSON *mdt_user_consent = NULL;
    OpenAPI_mdt_user_consent_e mdt_user_consentVariable = 0;
    cJSON *mdt_configuration = NULL;
    OpenAPI_mdt_configuration_1_t *mdt_configuration_local_nonprim = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *cag_data = NULL;
    OpenAPI_cag_data_1_t *cag_data_local_nonprim = NULL;
    cJSON *stn_sr = NULL;
    cJSON *c_msisdn = NULL;
    cJSON *nb_io_tue_priority = NULL;
    cJSON *nssai_inclusion_allowed = NULL;
    cJSON *rg_wireline_characteristics = NULL;
    cJSON *ec_restriction_data_wb = NULL;
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb_local_nonprim = NULL;
    cJSON *ec_restriction_data_nb = NULL;
    cJSON *expected_ue_behaviour_list = NULL;
    OpenAPI_expected_ue_behaviour_data_1_t *expected_ue_behaviour_list_local_nonprim = NULL;
    cJSON *primary_rat_restrictions = NULL;
    OpenAPI_list_t *primary_rat_restrictionsList = NULL;
    cJSON *secondary_rat_restrictions = NULL;
    OpenAPI_list_t *secondary_rat_restrictionsList = NULL;
    cJSON *edrx_parameters_list = NULL;
    OpenAPI_list_t *edrx_parameters_listList = NULL;
    cJSON *ptw_parameters_list = NULL;
    OpenAPI_list_t *ptw_parameters_listList = NULL;
    cJSON *iab_operation_allowed = NULL;
    cJSON *adjacent_plmn_restrictions = NULL;
    OpenAPI_list_t *adjacent_plmn_restrictionsList = NULL;
    cJSON *wireline_forbidden_areas = NULL;
    OpenAPI_list_t *wireline_forbidden_areasList = NULL;
    cJSON *wireline_service_area_restriction = NULL;
    OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction_local_nonprim = NULL;
    cJSON *pcf_selection_assistance_infos = NULL;
    OpenAPI_list_t *pcf_selection_assistance_infosList = NULL;
    cJSON *aerial_ue_sub_info = NULL;
    OpenAPI_aerial_ue_subscription_info_1_t *aerial_ue_sub_info_local_nonprim = NULL;
    cJSON *roaming_restrictions = NULL;
    OpenAPI_roaming_restrictions_t *roaming_restrictions_local_nonprim = NULL;
    cJSON *remote_prov_ind = NULL;
    supported_features = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    gpsis = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "gpsis");
    if (gpsis) {
        cJSON *gpsis_local = NULL;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [gpsis]");
            goto end;
        }

        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    hss_group_id = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "hssGroupId");
    if (hss_group_id) {
    if (!cJSON_IsString(hss_group_id) && !cJSON_IsNull(hss_group_id)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [hss_group_id]");
        goto end;
    }
    }

    internal_group_ids = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "internalGroupIds");
    if (internal_group_ids) {
        cJSON *internal_group_ids_local = NULL;
        if (!cJSON_IsArray(internal_group_ids)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [internal_group_ids]");
            goto end;
        }

        internal_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_ids_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [internal_group_ids]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idsList, ogs_strdup(internal_group_ids_local->valuestring));
        }
    }

    shared_vn_group_data_ids = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "sharedVnGroupDataIds");
    if (shared_vn_group_data_ids) {
        cJSON *shared_vn_group_data_ids_local_map = NULL;
        if (!cJSON_IsObject(shared_vn_group_data_ids) && !cJSON_IsNull(shared_vn_group_data_ids)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [shared_vn_group_data_ids]");
            goto end;
        }
        if (cJSON_IsObject(shared_vn_group_data_ids)) {
            shared_vn_group_data_idsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(shared_vn_group_data_ids_local_map, shared_vn_group_data_ids) {
                cJSON *localMapObject = shared_vn_group_data_ids_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), ogs_strdup(localMapObject->valuestring));
                OpenAPI_list_add(shared_vn_group_data_idsList, localMapKeyPair);
            }
        }
    }

    subscribed_ue_ambr = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "subscribedUeAmbr");
    if (subscribed_ue_ambr) {
    subscribed_ue_ambr_local_nonprim = OpenAPI_ambr_rm_parseFromJSON(subscribed_ue_ambr);
    if (!subscribed_ue_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_rm_parseFromJSON failed [subscribed_ue_ambr]");
        goto end;
    }
    }

    nssai = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "nssai");
    if (nssai) {
    if (!cJSON_IsNull(nssai)) {
    nssai_local_nonprim = OpenAPI_nssai_1_parseFromJSON(nssai);
    if (!nssai_local_nonprim) {
        ogs_error("OpenAPI_nssai_1_parseFromJSON failed [nssai]");
        goto end;
    }
    }
    }

    rat_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "ratRestrictions");
    if (rat_restrictions) {
        cJSON *rat_restrictions_local = NULL;
        if (!cJSON_IsArray(rat_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [rat_restrictions]");
            goto end;
        }

        rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_restrictions_local, rat_restrictions) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_restrictions_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [rat_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_restrictions\" is not supported. Ignoring it ...",
                         rat_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(rat_restrictionsList, (void *)localEnum);
            }
        }
        if (rat_restrictionsList->count == 0) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed: Expected rat_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    forbidden_areas = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "forbiddenAreas");
    if (forbidden_areas) {
        cJSON *forbidden_areas_local = NULL;
        if (!cJSON_IsArray(forbidden_areas)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [forbidden_areas]");
            goto end;
        }

        forbidden_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(forbidden_areas_local, forbidden_areas) {
            if (!cJSON_IsObject(forbidden_areas_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [forbidden_areas]");
                goto end;
            }
            OpenAPI_area_1_t *forbidden_areasItem = OpenAPI_area_1_parseFromJSON(forbidden_areas_local);
            if (!forbidden_areasItem) {
                ogs_error("No forbidden_areasItem");
                goto end;
            }
            OpenAPI_list_add(forbidden_areasList, forbidden_areasItem);
        }
    }

    service_area_restriction = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "serviceAreaRestriction");
    if (service_area_restriction) {
    service_area_restriction_local_nonprim = OpenAPI_service_area_restriction_1_parseFromJSON(service_area_restriction);
    if (!service_area_restriction_local_nonprim) {
        ogs_error("OpenAPI_service_area_restriction_1_parseFromJSON failed [service_area_restriction]");
        goto end;
    }
    }

    core_network_type_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "coreNetworkTypeRestrictions");
    if (core_network_type_restrictions) {
        cJSON *core_network_type_restrictions_local = NULL;
        if (!cJSON_IsArray(core_network_type_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [core_network_type_restrictions]");
            goto end;
        }

        core_network_type_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(core_network_type_restrictions_local, core_network_type_restrictions) {
            OpenAPI_core_network_type_e localEnum = OpenAPI_core_network_type_NULL;
            if (!cJSON_IsString(core_network_type_restrictions_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [core_network_type_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_core_network_type_FromString(core_network_type_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"core_network_type_restrictions\" is not supported. Ignoring it ...",
                         core_network_type_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(core_network_type_restrictionsList, (void *)localEnum);
            }
        }
        if (core_network_type_restrictionsList->count == 0) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed: Expected core_network_type_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    rfsp_index = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "rfspIndex");
    if (rfsp_index) {
    if (!cJSON_IsNull(rfsp_index)) {
    if (!cJSON_IsNumber(rfsp_index)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [rfsp_index]");
        goto end;
    }
    }
    }

    subs_reg_timer = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "subsRegTimer");
    if (subs_reg_timer) {
    if (!cJSON_IsNull(subs_reg_timer)) {
    if (!cJSON_IsNumber(subs_reg_timer)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [subs_reg_timer]");
        goto end;
    }
    }
    }

    ue_usage_type = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "ueUsageType");
    if (ue_usage_type) {
    if (!cJSON_IsNumber(ue_usage_type)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [ue_usage_type]");
        goto end;
    }
    }

    mps_priority = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "mpsPriority");
    if (mps_priority) {
    if (!cJSON_IsBool(mps_priority)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [mps_priority]");
        goto end;
    }
    }

    mcs_priority = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "mcsPriority");
    if (mcs_priority) {
    if (!cJSON_IsBool(mcs_priority)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [mcs_priority]");
        goto end;
    }
    }

    active_time = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "activeTime");
    if (active_time) {
    if (!cJSON_IsNull(active_time)) {
    if (!cJSON_IsNumber(active_time)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [active_time]");
        goto end;
    }
    }
    }

    sor_info = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "sorInfo");
    if (sor_info) {
    sor_info_local_nonprim = OpenAPI_sor_info_1_parseFromJSON(sor_info);
    if (!sor_info_local_nonprim) {
        ogs_error("OpenAPI_sor_info_1_parseFromJSON failed [sor_info]");
        goto end;
    }
    }

    sor_info_expect_ind = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "sorInfoExpectInd");
    if (sor_info_expect_ind) {
    if (!cJSON_IsBool(sor_info_expect_ind)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [sor_info_expect_ind]");
        goto end;
    }
    }

    soraf_retrieval = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "sorafRetrieval");
    if (soraf_retrieval) {
    if (!cJSON_IsBool(soraf_retrieval)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [soraf_retrieval]");
        goto end;
    }
    }

    sor_update_indicator_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "sorUpdateIndicatorList");
    if (sor_update_indicator_list) {
        cJSON *sor_update_indicator_list_local = NULL;
        if (!cJSON_IsArray(sor_update_indicator_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [sor_update_indicator_list]");
            goto end;
        }

        sor_update_indicator_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(sor_update_indicator_list_local, sor_update_indicator_list) {
            OpenAPI_sor_update_indicator_e localEnum = OpenAPI_sor_update_indicator_NULL;
            if (!cJSON_IsString(sor_update_indicator_list_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [sor_update_indicator_list]");
                goto end;
            }
            localEnum = OpenAPI_sor_update_indicator_FromString(sor_update_indicator_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"sor_update_indicator_list\" is not supported. Ignoring it ...",
                         sor_update_indicator_list_local->valuestring);
            } else {
                OpenAPI_list_add(sor_update_indicator_listList, (void *)localEnum);
            }
        }
        if (sor_update_indicator_listList->count == 0) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed: Expected sor_update_indicator_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    upu_info = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "upuInfo");
    if (upu_info) {
    upu_info_local_nonprim = OpenAPI_upu_info_1_parseFromJSON(upu_info);
    if (!upu_info_local_nonprim) {
        ogs_error("OpenAPI_upu_info_1_parseFromJSON failed [upu_info]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    mico_allowed = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "micoAllowed");
    if (mico_allowed) {
    if (!cJSON_IsBool(mico_allowed)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [mico_allowed]");
        goto end;
    }
    }

    shared_am_data_ids = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "sharedAmDataIds");
    if (shared_am_data_ids) {
        cJSON *shared_am_data_ids_local = NULL;
        if (!cJSON_IsArray(shared_am_data_ids)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [shared_am_data_ids]");
            goto end;
        }

        shared_am_data_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_am_data_ids_local, shared_am_data_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(shared_am_data_ids_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [shared_am_data_ids]");
                goto end;
            }
            OpenAPI_list_add(shared_am_data_idsList, ogs_strdup(shared_am_data_ids_local->valuestring));
        }
    }

    odb_packet_services = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "odbPacketServices");
    if (odb_packet_services) {
    if (!cJSON_IsString(odb_packet_services)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [odb_packet_services]");
        goto end;
    }
    odb_packet_servicesVariable = OpenAPI_odb_packet_services_FromString(odb_packet_services->valuestring);
    }

    subscribed_dnn_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "subscribedDnnList");
    if (subscribed_dnn_list) {
        cJSON *subscribed_dnn_list_local = NULL;
        if (!cJSON_IsArray(subscribed_dnn_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [subscribed_dnn_list]");
            goto end;
        }

        subscribed_dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_dnn_list_local, subscribed_dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subscribed_dnn_list_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [subscribed_dnn_list]");
                goto end;
            }
            OpenAPI_list_add(subscribed_dnn_listList, ogs_strdup(subscribed_dnn_list_local->valuestring));
        }
    }

    service_gap_time = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "serviceGapTime");
    if (service_gap_time) {
    if (!cJSON_IsNumber(service_gap_time)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [service_gap_time]");
        goto end;
    }
    }

    mdt_user_consent = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "mdtUserConsent");
    if (mdt_user_consent) {
    if (!cJSON_IsString(mdt_user_consent)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [mdt_user_consent]");
        goto end;
    }
    mdt_user_consentVariable = OpenAPI_mdt_user_consent_FromString(mdt_user_consent->valuestring);
    }

    mdt_configuration = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "mdtConfiguration");
    if (mdt_configuration) {
    mdt_configuration_local_nonprim = OpenAPI_mdt_configuration_1_parseFromJSON(mdt_configuration);
    if (!mdt_configuration_local_nonprim) {
        ogs_error("OpenAPI_mdt_configuration_1_parseFromJSON failed [mdt_configuration]");
        goto end;
    }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    cag_data = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "cagData");
    if (cag_data) {
    cag_data_local_nonprim = OpenAPI_cag_data_1_parseFromJSON(cag_data);
    if (!cag_data_local_nonprim) {
        ogs_error("OpenAPI_cag_data_1_parseFromJSON failed [cag_data]");
        goto end;
    }
    }

    stn_sr = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "stnSr");
    if (stn_sr) {
    if (!cJSON_IsString(stn_sr) && !cJSON_IsNull(stn_sr)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [stn_sr]");
        goto end;
    }
    }

    c_msisdn = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "cMsisdn");
    if (c_msisdn) {
    if (!cJSON_IsString(c_msisdn) && !cJSON_IsNull(c_msisdn)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [c_msisdn]");
        goto end;
    }
    }

    nb_io_tue_priority = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "nbIoTUePriority");
    if (nb_io_tue_priority) {
    if (!cJSON_IsNumber(nb_io_tue_priority)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [nb_io_tue_priority]");
        goto end;
    }
    }

    nssai_inclusion_allowed = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "nssaiInclusionAllowed");
    if (nssai_inclusion_allowed) {
    if (!cJSON_IsBool(nssai_inclusion_allowed)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [nssai_inclusion_allowed]");
        goto end;
    }
    }

    rg_wireline_characteristics = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "rgWirelineCharacteristics");
    if (rg_wireline_characteristics) {
    if (!cJSON_IsString(rg_wireline_characteristics) && !cJSON_IsNull(rg_wireline_characteristics)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [rg_wireline_characteristics]");
        goto end;
    }
    }

    ec_restriction_data_wb = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "ecRestrictionDataWb");
    if (ec_restriction_data_wb) {
    ec_restriction_data_wb_local_nonprim = OpenAPI_ec_restriction_data_wb_parseFromJSON(ec_restriction_data_wb);
    if (!ec_restriction_data_wb_local_nonprim) {
        ogs_error("OpenAPI_ec_restriction_data_wb_parseFromJSON failed [ec_restriction_data_wb]");
        goto end;
    }
    }

    ec_restriction_data_nb = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "ecRestrictionDataNb");
    if (ec_restriction_data_nb) {
    if (!cJSON_IsBool(ec_restriction_data_nb)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [ec_restriction_data_nb]");
        goto end;
    }
    }

    expected_ue_behaviour_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "expectedUeBehaviourList");
    if (expected_ue_behaviour_list) {
    expected_ue_behaviour_list_local_nonprim = OpenAPI_expected_ue_behaviour_data_1_parseFromJSON(expected_ue_behaviour_list);
    if (!expected_ue_behaviour_list_local_nonprim) {
        ogs_error("OpenAPI_expected_ue_behaviour_data_1_parseFromJSON failed [expected_ue_behaviour_list]");
        goto end;
    }
    }

    primary_rat_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "primaryRatRestrictions");
    if (primary_rat_restrictions) {
        cJSON *primary_rat_restrictions_local = NULL;
        if (!cJSON_IsArray(primary_rat_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [primary_rat_restrictions]");
            goto end;
        }

        primary_rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(primary_rat_restrictions_local, primary_rat_restrictions) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(primary_rat_restrictions_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [primary_rat_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(primary_rat_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"primary_rat_restrictions\" is not supported. Ignoring it ...",
                         primary_rat_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(primary_rat_restrictionsList, (void *)localEnum);
            }
        }
        if (primary_rat_restrictionsList->count == 0) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed: Expected primary_rat_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    secondary_rat_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "secondaryRatRestrictions");
    if (secondary_rat_restrictions) {
        cJSON *secondary_rat_restrictions_local = NULL;
        if (!cJSON_IsArray(secondary_rat_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [secondary_rat_restrictions]");
            goto end;
        }

        secondary_rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_restrictions_local, secondary_rat_restrictions) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(secondary_rat_restrictions_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [secondary_rat_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(secondary_rat_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"secondary_rat_restrictions\" is not supported. Ignoring it ...",
                         secondary_rat_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(secondary_rat_restrictionsList, (void *)localEnum);
            }
        }
        if (secondary_rat_restrictionsList->count == 0) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed: Expected secondary_rat_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    edrx_parameters_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "edrxParametersList");
    if (edrx_parameters_list) {
        cJSON *edrx_parameters_list_local = NULL;
        if (!cJSON_IsArray(edrx_parameters_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [edrx_parameters_list]");
            goto end;
        }

        edrx_parameters_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(edrx_parameters_list_local, edrx_parameters_list) {
            if (!cJSON_IsObject(edrx_parameters_list_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [edrx_parameters_list]");
                goto end;
            }
            OpenAPI_edrx_parameters_1_t *edrx_parameters_listItem = OpenAPI_edrx_parameters_1_parseFromJSON(edrx_parameters_list_local);
            if (!edrx_parameters_listItem) {
                ogs_error("No edrx_parameters_listItem");
                goto end;
            }
            OpenAPI_list_add(edrx_parameters_listList, edrx_parameters_listItem);
        }
    }

    ptw_parameters_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "ptwParametersList");
    if (ptw_parameters_list) {
        cJSON *ptw_parameters_list_local = NULL;
        if (!cJSON_IsArray(ptw_parameters_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [ptw_parameters_list]");
            goto end;
        }

        ptw_parameters_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ptw_parameters_list_local, ptw_parameters_list) {
            if (!cJSON_IsObject(ptw_parameters_list_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [ptw_parameters_list]");
                goto end;
            }
            OpenAPI_ptw_parameters_1_t *ptw_parameters_listItem = OpenAPI_ptw_parameters_1_parseFromJSON(ptw_parameters_list_local);
            if (!ptw_parameters_listItem) {
                ogs_error("No ptw_parameters_listItem");
                goto end;
            }
            OpenAPI_list_add(ptw_parameters_listList, ptw_parameters_listItem);
        }
    }

    iab_operation_allowed = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "iabOperationAllowed");
    if (iab_operation_allowed) {
    if (!cJSON_IsBool(iab_operation_allowed)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [iab_operation_allowed]");
        goto end;
    }
    }

    adjacent_plmn_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "adjacentPlmnRestrictions");
    if (adjacent_plmn_restrictions) {
        cJSON *adjacent_plmn_restrictions_local_map = NULL;
        if (!cJSON_IsObject(adjacent_plmn_restrictions) && !cJSON_IsNull(adjacent_plmn_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [adjacent_plmn_restrictions]");
            goto end;
        }
        if (cJSON_IsObject(adjacent_plmn_restrictions)) {
            adjacent_plmn_restrictionsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(adjacent_plmn_restrictions_local_map, adjacent_plmn_restrictions) {
                cJSON *localMapObject = adjacent_plmn_restrictions_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_plmn_restriction_1_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(adjacent_plmn_restrictionsList, localMapKeyPair);
            }
        }
    }

    wireline_forbidden_areas = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "wirelineForbiddenAreas");
    if (wireline_forbidden_areas) {
        cJSON *wireline_forbidden_areas_local = NULL;
        if (!cJSON_IsArray(wireline_forbidden_areas)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [wireline_forbidden_areas]");
            goto end;
        }

        wireline_forbidden_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(wireline_forbidden_areas_local, wireline_forbidden_areas) {
            if (!cJSON_IsObject(wireline_forbidden_areas_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [wireline_forbidden_areas]");
                goto end;
            }
            OpenAPI_wireline_area_1_t *wireline_forbidden_areasItem = OpenAPI_wireline_area_1_parseFromJSON(wireline_forbidden_areas_local);
            if (!wireline_forbidden_areasItem) {
                ogs_error("No wireline_forbidden_areasItem");
                goto end;
            }
            OpenAPI_list_add(wireline_forbidden_areasList, wireline_forbidden_areasItem);
        }
    }

    wireline_service_area_restriction = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "wirelineServiceAreaRestriction");
    if (wireline_service_area_restriction) {
    wireline_service_area_restriction_local_nonprim = OpenAPI_wireline_service_area_restriction_1_parseFromJSON(wireline_service_area_restriction);
    if (!wireline_service_area_restriction_local_nonprim) {
        ogs_error("OpenAPI_wireline_service_area_restriction_1_parseFromJSON failed [wireline_service_area_restriction]");
        goto end;
    }
    }

    pcf_selection_assistance_infos = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "pcfSelectionAssistanceInfos");
    if (pcf_selection_assistance_infos) {
        cJSON *pcf_selection_assistance_infos_local = NULL;
        if (!cJSON_IsArray(pcf_selection_assistance_infos)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [pcf_selection_assistance_infos]");
            goto end;
        }

        pcf_selection_assistance_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_selection_assistance_infos_local, pcf_selection_assistance_infos) {
            if (!cJSON_IsObject(pcf_selection_assistance_infos_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [pcf_selection_assistance_infos]");
                goto end;
            }
            OpenAPI_pcf_selection_assistance_info_1_t *pcf_selection_assistance_infosItem = OpenAPI_pcf_selection_assistance_info_1_parseFromJSON(pcf_selection_assistance_infos_local);
            if (!pcf_selection_assistance_infosItem) {
                ogs_error("No pcf_selection_assistance_infosItem");
                goto end;
            }
            OpenAPI_list_add(pcf_selection_assistance_infosList, pcf_selection_assistance_infosItem);
        }
    }

    aerial_ue_sub_info = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "aerialUeSubInfo");
    if (aerial_ue_sub_info) {
    aerial_ue_sub_info_local_nonprim = OpenAPI_aerial_ue_subscription_info_1_parseFromJSON(aerial_ue_sub_info);
    if (!aerial_ue_sub_info_local_nonprim) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_1_parseFromJSON failed [aerial_ue_sub_info]");
        goto end;
    }
    }

    roaming_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "roamingRestrictions");
    if (roaming_restrictions) {
    roaming_restrictions_local_nonprim = OpenAPI_roaming_restrictions_parseFromJSON(roaming_restrictions);
    if (!roaming_restrictions_local_nonprim) {
        ogs_error("OpenAPI_roaming_restrictions_parseFromJSON failed [roaming_restrictions]");
        goto end;
    }
    }

    remote_prov_ind = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_data_1JSON, "remoteProvInd");
    if (remote_prov_ind) {
    if (!cJSON_IsBool(remote_prov_ind)) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON() failed [remote_prov_ind]");
        goto end;
    }
    }

    access_and_mobility_subscription_data_1_local_var = OpenAPI_access_and_mobility_subscription_data_1_create (
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        gpsis ? gpsisList : NULL,
        hss_group_id && !cJSON_IsNull(hss_group_id) ? ogs_strdup(hss_group_id->valuestring) : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        shared_vn_group_data_ids ? shared_vn_group_data_idsList : NULL,
        subscribed_ue_ambr ? subscribed_ue_ambr_local_nonprim : NULL,
        nssai && cJSON_IsNull(nssai) ? true : false,
        nssai ? nssai_local_nonprim : NULL,
        rat_restrictions ? rat_restrictionsList : NULL,
        forbidden_areas ? forbidden_areasList : NULL,
        service_area_restriction ? service_area_restriction_local_nonprim : NULL,
        core_network_type_restrictions ? core_network_type_restrictionsList : NULL,
        rfsp_index && cJSON_IsNull(rfsp_index) ? true : false,
        rfsp_index ? true : false,
        rfsp_index ? rfsp_index->valuedouble : 0,
        subs_reg_timer && cJSON_IsNull(subs_reg_timer) ? true : false,
        subs_reg_timer ? true : false,
        subs_reg_timer ? subs_reg_timer->valuedouble : 0,
        ue_usage_type ? true : false,
        ue_usage_type ? ue_usage_type->valuedouble : 0,
        mps_priority ? true : false,
        mps_priority ? mps_priority->valueint : 0,
        mcs_priority ? true : false,
        mcs_priority ? mcs_priority->valueint : 0,
        active_time && cJSON_IsNull(active_time) ? true : false,
        active_time ? true : false,
        active_time ? active_time->valuedouble : 0,
        sor_info ? sor_info_local_nonprim : NULL,
        sor_info_expect_ind ? true : false,
        sor_info_expect_ind ? sor_info_expect_ind->valueint : 0,
        soraf_retrieval ? true : false,
        soraf_retrieval ? soraf_retrieval->valueint : 0,
        sor_update_indicator_list ? sor_update_indicator_listList : NULL,
        upu_info ? upu_info_local_nonprim : NULL,
        routing_indicator && !cJSON_IsNull(routing_indicator) ? ogs_strdup(routing_indicator->valuestring) : NULL,
        mico_allowed ? true : false,
        mico_allowed ? mico_allowed->valueint : 0,
        shared_am_data_ids ? shared_am_data_idsList : NULL,
        odb_packet_services ? odb_packet_servicesVariable : 0,
        subscribed_dnn_list ? subscribed_dnn_listList : NULL,
        service_gap_time ? true : false,
        service_gap_time ? service_gap_time->valuedouble : 0,
        mdt_user_consent ? mdt_user_consentVariable : 0,
        mdt_configuration ? mdt_configuration_local_nonprim : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        cag_data ? cag_data_local_nonprim : NULL,
        stn_sr && !cJSON_IsNull(stn_sr) ? ogs_strdup(stn_sr->valuestring) : NULL,
        c_msisdn && !cJSON_IsNull(c_msisdn) ? ogs_strdup(c_msisdn->valuestring) : NULL,
        nb_io_tue_priority ? true : false,
        nb_io_tue_priority ? nb_io_tue_priority->valuedouble : 0,
        nssai_inclusion_allowed ? true : false,
        nssai_inclusion_allowed ? nssai_inclusion_allowed->valueint : 0,
        rg_wireline_characteristics && !cJSON_IsNull(rg_wireline_characteristics) ? ogs_strdup(rg_wireline_characteristics->valuestring) : NULL,
        ec_restriction_data_wb ? ec_restriction_data_wb_local_nonprim : NULL,
        ec_restriction_data_nb ? true : false,
        ec_restriction_data_nb ? ec_restriction_data_nb->valueint : 0,
        expected_ue_behaviour_list ? expected_ue_behaviour_list_local_nonprim : NULL,
        primary_rat_restrictions ? primary_rat_restrictionsList : NULL,
        secondary_rat_restrictions ? secondary_rat_restrictionsList : NULL,
        edrx_parameters_list ? edrx_parameters_listList : NULL,
        ptw_parameters_list ? ptw_parameters_listList : NULL,
        iab_operation_allowed ? true : false,
        iab_operation_allowed ? iab_operation_allowed->valueint : 0,
        adjacent_plmn_restrictions ? adjacent_plmn_restrictionsList : NULL,
        wireline_forbidden_areas ? wireline_forbidden_areasList : NULL,
        wireline_service_area_restriction ? wireline_service_area_restriction_local_nonprim : NULL,
        pcf_selection_assistance_infos ? pcf_selection_assistance_infosList : NULL,
        aerial_ue_sub_info ? aerial_ue_sub_info_local_nonprim : NULL,
        roaming_restrictions ? roaming_restrictions_local_nonprim : NULL,
        remote_prov_ind ? true : false,
        remote_prov_ind ? remote_prov_ind->valueint : 0
    );

    return access_and_mobility_subscription_data_1_local_var;
end:
    if (gpsisList) {
        OpenAPI_list_for_each(gpsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsisList);
        gpsisList = NULL;
    }
    if (internal_group_idsList) {
        OpenAPI_list_for_each(internal_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(internal_group_idsList);
        internal_group_idsList = NULL;
    }
    if (shared_vn_group_data_idsList) {
        OpenAPI_list_for_each(shared_vn_group_data_idsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_vn_group_data_idsList);
        shared_vn_group_data_idsList = NULL;
    }
    if (subscribed_ue_ambr_local_nonprim) {
        OpenAPI_ambr_rm_free(subscribed_ue_ambr_local_nonprim);
        subscribed_ue_ambr_local_nonprim = NULL;
    }
    if (nssai_local_nonprim) {
        OpenAPI_nssai_1_free(nssai_local_nonprim);
        nssai_local_nonprim = NULL;
    }
    if (rat_restrictionsList) {
        OpenAPI_list_free(rat_restrictionsList);
        rat_restrictionsList = NULL;
    }
    if (forbidden_areasList) {
        OpenAPI_list_for_each(forbidden_areasList, node) {
            OpenAPI_area_1_free(node->data);
        }
        OpenAPI_list_free(forbidden_areasList);
        forbidden_areasList = NULL;
    }
    if (service_area_restriction_local_nonprim) {
        OpenAPI_service_area_restriction_1_free(service_area_restriction_local_nonprim);
        service_area_restriction_local_nonprim = NULL;
    }
    if (core_network_type_restrictionsList) {
        OpenAPI_list_free(core_network_type_restrictionsList);
        core_network_type_restrictionsList = NULL;
    }
    if (sor_info_local_nonprim) {
        OpenAPI_sor_info_1_free(sor_info_local_nonprim);
        sor_info_local_nonprim = NULL;
    }
    if (sor_update_indicator_listList) {
        OpenAPI_list_free(sor_update_indicator_listList);
        sor_update_indicator_listList = NULL;
    }
    if (upu_info_local_nonprim) {
        OpenAPI_upu_info_1_free(upu_info_local_nonprim);
        upu_info_local_nonprim = NULL;
    }
    if (shared_am_data_idsList) {
        OpenAPI_list_for_each(shared_am_data_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_am_data_idsList);
        shared_am_data_idsList = NULL;
    }
    if (subscribed_dnn_listList) {
        OpenAPI_list_for_each(subscribed_dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscribed_dnn_listList);
        subscribed_dnn_listList = NULL;
    }
    if (mdt_configuration_local_nonprim) {
        OpenAPI_mdt_configuration_1_free(mdt_configuration_local_nonprim);
        mdt_configuration_local_nonprim = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (cag_data_local_nonprim) {
        OpenAPI_cag_data_1_free(cag_data_local_nonprim);
        cag_data_local_nonprim = NULL;
    }
    if (ec_restriction_data_wb_local_nonprim) {
        OpenAPI_ec_restriction_data_wb_free(ec_restriction_data_wb_local_nonprim);
        ec_restriction_data_wb_local_nonprim = NULL;
    }
    if (expected_ue_behaviour_list_local_nonprim) {
        OpenAPI_expected_ue_behaviour_data_1_free(expected_ue_behaviour_list_local_nonprim);
        expected_ue_behaviour_list_local_nonprim = NULL;
    }
    if (primary_rat_restrictionsList) {
        OpenAPI_list_free(primary_rat_restrictionsList);
        primary_rat_restrictionsList = NULL;
    }
    if (secondary_rat_restrictionsList) {
        OpenAPI_list_free(secondary_rat_restrictionsList);
        secondary_rat_restrictionsList = NULL;
    }
    if (edrx_parameters_listList) {
        OpenAPI_list_for_each(edrx_parameters_listList, node) {
            OpenAPI_edrx_parameters_1_free(node->data);
        }
        OpenAPI_list_free(edrx_parameters_listList);
        edrx_parameters_listList = NULL;
    }
    if (ptw_parameters_listList) {
        OpenAPI_list_for_each(ptw_parameters_listList, node) {
            OpenAPI_ptw_parameters_1_free(node->data);
        }
        OpenAPI_list_free(ptw_parameters_listList);
        ptw_parameters_listList = NULL;
    }
    if (adjacent_plmn_restrictionsList) {
        OpenAPI_list_for_each(adjacent_plmn_restrictionsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_plmn_restriction_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(adjacent_plmn_restrictionsList);
        adjacent_plmn_restrictionsList = NULL;
    }
    if (wireline_forbidden_areasList) {
        OpenAPI_list_for_each(wireline_forbidden_areasList, node) {
            OpenAPI_wireline_area_1_free(node->data);
        }
        OpenAPI_list_free(wireline_forbidden_areasList);
        wireline_forbidden_areasList = NULL;
    }
    if (wireline_service_area_restriction_local_nonprim) {
        OpenAPI_wireline_service_area_restriction_1_free(wireline_service_area_restriction_local_nonprim);
        wireline_service_area_restriction_local_nonprim = NULL;
    }
    if (pcf_selection_assistance_infosList) {
        OpenAPI_list_for_each(pcf_selection_assistance_infosList, node) {
            OpenAPI_pcf_selection_assistance_info_1_free(node->data);
        }
        OpenAPI_list_free(pcf_selection_assistance_infosList);
        pcf_selection_assistance_infosList = NULL;
    }
    if (aerial_ue_sub_info_local_nonprim) {
        OpenAPI_aerial_ue_subscription_info_1_free(aerial_ue_sub_info_local_nonprim);
        aerial_ue_sub_info_local_nonprim = NULL;
    }
    if (roaming_restrictions_local_nonprim) {
        OpenAPI_roaming_restrictions_free(roaming_restrictions_local_nonprim);
        roaming_restrictions_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_access_and_mobility_subscription_data_1_t *OpenAPI_access_and_mobility_subscription_data_1_copy(OpenAPI_access_and_mobility_subscription_data_1_t *dst, OpenAPI_access_and_mobility_subscription_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_and_mobility_subscription_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_convertToJSON() failed");
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

    OpenAPI_access_and_mobility_subscription_data_1_free(dst);
    dst = OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

