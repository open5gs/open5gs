
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_and_mobility_subscription_data.h"

OpenAPI_access_and_mobility_subscription_data_t *OpenAPI_access_and_mobility_subscription_data_create(
    char *supported_features,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* vn_group_info,
    OpenAPI_list_t* shared_vn_group_data_ids,
    OpenAPI_ambr_rm_t *subscribed_ue_ambr,
    OpenAPI_nssai_t *nssai,
    OpenAPI_list_t *rat_restrictions,
    OpenAPI_list_t *forbidden_areas,
    OpenAPI_service_area_restriction_t *service_area_restriction,
    OpenAPI_list_t *core_network_type_restrictions,
    int rfsp_index,
    int subs_reg_timer,
    int ue_usage_type,
    int mps_priority,
    int mcs_priority,
    int active_time,
    int dl_packet_count,
    OpenAPI_sor_info_t *sor_info,
    int sor_info_expect_ind,
    int soraf_retrieval,
    OpenAPI_list_t *sor_update_indicator_list,
    OpenAPI_upu_info_t *upu_info,
    int mico_allowed,
    OpenAPI_list_t *shared_am_data_ids,
    OpenAPI_odb_packet_services_t *odb_packet_services,
    OpenAPI_list_t *subscribed_dnn_list,
    int service_gap_time,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_cag_data_t *cag_data,
    char *stn_sr,
    char *c_msisdn,
    int nb_io_t_ue_priority,
    int nssai_inclusion_allowed,
    char rg_wireline_characteristics,
    OpenAPI_tmbr_t *rg_tmbr,
    OpenAPI_ec_restriction_data_t *ec_restriction_data,
    OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_list,
    OpenAPI_list_t *maximum_response_time_list,
    OpenAPI_list_t *maximum_latency_list,
    OpenAPI_list_t *primary_rat_restrictions,
    OpenAPI_list_t *secondary_rat_restrictions,
    OpenAPI_list_t *edrx_parameters_list,
    OpenAPI_list_t *ptw_parameters_list,
    int iab_operation_allowed,
    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth,
    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth,
    char *nr_ue_pc5_ambr,
    char *lte_pc5_ambr
    )
{
    OpenAPI_access_and_mobility_subscription_data_t *access_and_mobility_subscription_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_access_and_mobility_subscription_data_t));
    if (!access_and_mobility_subscription_data_local_var) {
        return NULL;
    }
    access_and_mobility_subscription_data_local_var->supported_features = supported_features;
    access_and_mobility_subscription_data_local_var->gpsis = gpsis;
    access_and_mobility_subscription_data_local_var->internal_group_ids = internal_group_ids;
    access_and_mobility_subscription_data_local_var->vn_group_info = vn_group_info;
    access_and_mobility_subscription_data_local_var->shared_vn_group_data_ids = shared_vn_group_data_ids;
    access_and_mobility_subscription_data_local_var->subscribed_ue_ambr = subscribed_ue_ambr;
    access_and_mobility_subscription_data_local_var->nssai = nssai;
    access_and_mobility_subscription_data_local_var->rat_restrictions = rat_restrictions;
    access_and_mobility_subscription_data_local_var->forbidden_areas = forbidden_areas;
    access_and_mobility_subscription_data_local_var->service_area_restriction = service_area_restriction;
    access_and_mobility_subscription_data_local_var->core_network_type_restrictions = core_network_type_restrictions;
    access_and_mobility_subscription_data_local_var->rfsp_index = rfsp_index;
    access_and_mobility_subscription_data_local_var->subs_reg_timer = subs_reg_timer;
    access_and_mobility_subscription_data_local_var->ue_usage_type = ue_usage_type;
    access_and_mobility_subscription_data_local_var->mps_priority = mps_priority;
    access_and_mobility_subscription_data_local_var->mcs_priority = mcs_priority;
    access_and_mobility_subscription_data_local_var->active_time = active_time;
    access_and_mobility_subscription_data_local_var->dl_packet_count = dl_packet_count;
    access_and_mobility_subscription_data_local_var->sor_info = sor_info;
    access_and_mobility_subscription_data_local_var->sor_info_expect_ind = sor_info_expect_ind;
    access_and_mobility_subscription_data_local_var->soraf_retrieval = soraf_retrieval;
    access_and_mobility_subscription_data_local_var->sor_update_indicator_list = sor_update_indicator_list;
    access_and_mobility_subscription_data_local_var->upu_info = upu_info;
    access_and_mobility_subscription_data_local_var->mico_allowed = mico_allowed;
    access_and_mobility_subscription_data_local_var->shared_am_data_ids = shared_am_data_ids;
    access_and_mobility_subscription_data_local_var->odb_packet_services = odb_packet_services;
    access_and_mobility_subscription_data_local_var->subscribed_dnn_list = subscribed_dnn_list;
    access_and_mobility_subscription_data_local_var->service_gap_time = service_gap_time;
    access_and_mobility_subscription_data_local_var->trace_data = trace_data;
    access_and_mobility_subscription_data_local_var->cag_data = cag_data;
    access_and_mobility_subscription_data_local_var->stn_sr = stn_sr;
    access_and_mobility_subscription_data_local_var->c_msisdn = c_msisdn;
    access_and_mobility_subscription_data_local_var->nb_io_t_ue_priority = nb_io_t_ue_priority;
    access_and_mobility_subscription_data_local_var->nssai_inclusion_allowed = nssai_inclusion_allowed;
    access_and_mobility_subscription_data_local_var->rg_wireline_characteristics = rg_wireline_characteristics;
    access_and_mobility_subscription_data_local_var->rg_tmbr = rg_tmbr;
    access_and_mobility_subscription_data_local_var->ec_restriction_data = ec_restriction_data;
    access_and_mobility_subscription_data_local_var->expected_ue_behaviour_list = expected_ue_behaviour_list;
    access_and_mobility_subscription_data_local_var->maximum_response_time_list = maximum_response_time_list;
    access_and_mobility_subscription_data_local_var->maximum_latency_list = maximum_latency_list;
    access_and_mobility_subscription_data_local_var->primary_rat_restrictions = primary_rat_restrictions;
    access_and_mobility_subscription_data_local_var->secondary_rat_restrictions = secondary_rat_restrictions;
    access_and_mobility_subscription_data_local_var->edrx_parameters_list = edrx_parameters_list;
    access_and_mobility_subscription_data_local_var->ptw_parameters_list = ptw_parameters_list;
    access_and_mobility_subscription_data_local_var->iab_operation_allowed = iab_operation_allowed;
    access_and_mobility_subscription_data_local_var->nr_v2x_services_auth = nr_v2x_services_auth;
    access_and_mobility_subscription_data_local_var->lte_v2x_services_auth = lte_v2x_services_auth;
    access_and_mobility_subscription_data_local_var->nr_ue_pc5_ambr = nr_ue_pc5_ambr;
    access_and_mobility_subscription_data_local_var->lte_pc5_ambr = lte_pc5_ambr;

    return access_and_mobility_subscription_data_local_var;
}

void OpenAPI_access_and_mobility_subscription_data_free(OpenAPI_access_and_mobility_subscription_data_t *access_and_mobility_subscription_data)
{
    if (NULL == access_and_mobility_subscription_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(access_and_mobility_subscription_data->supported_features);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->gpsis, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->gpsis);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->internal_group_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->internal_group_ids);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->vn_group_info, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_vn_group_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->vn_group_info);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->shared_vn_group_data_ids, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->shared_vn_group_data_ids);
    OpenAPI_ambr_rm_free(access_and_mobility_subscription_data->subscribed_ue_ambr);
    OpenAPI_nssai_free(access_and_mobility_subscription_data->nssai);
    OpenAPI_list_free(access_and_mobility_subscription_data->rat_restrictions);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->forbidden_areas, node) {
        OpenAPI_area_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->forbidden_areas);
    OpenAPI_service_area_restriction_free(access_and_mobility_subscription_data->service_area_restriction);
    OpenAPI_list_free(access_and_mobility_subscription_data->core_network_type_restrictions);
    OpenAPI_sor_info_free(access_and_mobility_subscription_data->sor_info);
    OpenAPI_list_free(access_and_mobility_subscription_data->sor_update_indicator_list);
    OpenAPI_upu_info_free(access_and_mobility_subscription_data->upu_info);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->shared_am_data_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->shared_am_data_ids);
    OpenAPI_odb_packet_services_free(access_and_mobility_subscription_data->odb_packet_services);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->subscribed_dnn_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->subscribed_dnn_list);
    OpenAPI_trace_data_free(access_and_mobility_subscription_data->trace_data);
    OpenAPI_cag_data_free(access_and_mobility_subscription_data->cag_data);
    ogs_free(access_and_mobility_subscription_data->stn_sr);
    ogs_free(access_and_mobility_subscription_data->c_msisdn);
    OpenAPI_tmbr_free(access_and_mobility_subscription_data->rg_tmbr);
    OpenAPI_ec_restriction_data_free(access_and_mobility_subscription_data->ec_restriction_data);
    OpenAPI_expected_ue_behaviour_data_free(access_and_mobility_subscription_data->expected_ue_behaviour_list);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->maximum_response_time_list, node) {
        OpenAPI_maximum_response_time_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->maximum_response_time_list);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->maximum_latency_list, node) {
        OpenAPI_maximum_latency_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->maximum_latency_list);
    OpenAPI_list_free(access_and_mobility_subscription_data->primary_rat_restrictions);
    OpenAPI_list_free(access_and_mobility_subscription_data->secondary_rat_restrictions);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->edrx_parameters_list, node) {
        OpenAPI_edrx_parameters_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->edrx_parameters_list);
    OpenAPI_list_for_each(access_and_mobility_subscription_data->ptw_parameters_list, node) {
        OpenAPI_ptw_parameters_free(node->data);
    }
    OpenAPI_list_free(access_and_mobility_subscription_data->ptw_parameters_list);
    OpenAPI_nr_v2x_auth_free(access_and_mobility_subscription_data->nr_v2x_services_auth);
    OpenAPI_lte_v2x_auth_free(access_and_mobility_subscription_data->lte_v2x_services_auth);
    ogs_free(access_and_mobility_subscription_data->nr_ue_pc5_ambr);
    ogs_free(access_and_mobility_subscription_data->lte_pc5_ambr);
    ogs_free(access_and_mobility_subscription_data);
}

cJSON *OpenAPI_access_and_mobility_subscription_data_convertToJSON(OpenAPI_access_and_mobility_subscription_data_t *access_and_mobility_subscription_data)
{
    cJSON *item = NULL;

    if (access_and_mobility_subscription_data == NULL) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [AccessAndMobilitySubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (access_and_mobility_subscription_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", access_and_mobility_subscription_data->supported_features) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->gpsis) {
        cJSON *gpsis = cJSON_AddArrayToObject(item, "gpsis");
        if (gpsis == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [gpsis]");
            goto end;
        }

        OpenAPI_lnode_t *gpsis_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->gpsis, gpsis_node)  {
            if (cJSON_AddStringToObject(gpsis, "", (char*)gpsis_node->data) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [gpsis]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->internal_group_ids) {
        cJSON *internal_group_ids = cJSON_AddArrayToObject(item, "internalGroupIds");
        if (internal_group_ids == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [internal_group_ids]");
            goto end;
        }

        OpenAPI_lnode_t *internal_group_ids_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->internal_group_ids, internal_group_ids_node)  {
            if (cJSON_AddStringToObject(internal_group_ids, "", (char*)internal_group_ids_node->data) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [internal_group_ids]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->vn_group_info) {
        cJSON *vn_group_info = cJSON_AddObjectToObject(item, "vnGroupInfo");
        if (vn_group_info == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [vn_group_info]");
            goto end;
        }
        cJSON *localMapObject = vn_group_info;
        OpenAPI_lnode_t *vn_group_info_node;
        if (access_and_mobility_subscription_data->vn_group_info) {
            OpenAPI_list_for_each(access_and_mobility_subscription_data->vn_group_info, vn_group_info_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)vn_group_info_node->data;
                cJSON *itemLocal = OpenAPI_vn_group_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [vn_group_info]");
                    goto end;
                }
                cJSON_AddItemToObject(vn_group_info, localKeyValue->key, itemLocal);
            }
        }
    }

    if (access_and_mobility_subscription_data->shared_vn_group_data_ids) {
        cJSON *shared_vn_group_data_ids = cJSON_AddObjectToObject(item, "sharedVnGroupDataIds");
        if (shared_vn_group_data_ids == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [shared_vn_group_data_ids]");
            goto end;
        }
        cJSON *localMapObject = shared_vn_group_data_ids;
        OpenAPI_lnode_t *shared_vn_group_data_ids_node;
        if (access_and_mobility_subscription_data->shared_vn_group_data_ids) {
            OpenAPI_list_for_each(access_and_mobility_subscription_data->shared_vn_group_data_ids, shared_vn_group_data_ids_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)shared_vn_group_data_ids_node->data;
            }
        }
    }

    if (access_and_mobility_subscription_data->subscribed_ue_ambr) {
        cJSON *subscribed_ue_ambr_local_JSON = OpenAPI_ambr_rm_convertToJSON(access_and_mobility_subscription_data->subscribed_ue_ambr);
        if (subscribed_ue_ambr_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [subscribed_ue_ambr]");
            goto end;
        }
        cJSON_AddItemToObject(item, "subscribedUeAmbr", subscribed_ue_ambr_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [subscribed_ue_ambr]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->nssai) {
        cJSON *nssai_local_JSON = OpenAPI_nssai_convertToJSON(access_and_mobility_subscription_data->nssai);
        if (nssai_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [nssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nssai", nssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [nssai]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->rat_restrictions) {
        cJSON *rat_restrictions = cJSON_AddArrayToObject(item, "ratRestrictions");
        if (rat_restrictions == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [rat_restrictions]");
            goto end;
        }
        OpenAPI_lnode_t *rat_restrictions_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->rat_restrictions, rat_restrictions_node) {
            if (cJSON_AddStringToObject(rat_restrictions, "", OpenAPI_rat_type_ToString((OpenAPI_rat_type_e)rat_restrictions_node->data)) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [rat_restrictions]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->forbidden_areas) {
        cJSON *forbidden_areasList = cJSON_AddArrayToObject(item, "forbiddenAreas");
        if (forbidden_areasList == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [forbidden_areas]");
            goto end;
        }

        OpenAPI_lnode_t *forbidden_areas_node;
        if (access_and_mobility_subscription_data->forbidden_areas) {
            OpenAPI_list_for_each(access_and_mobility_subscription_data->forbidden_areas, forbidden_areas_node) {
                cJSON *itemLocal = OpenAPI_area_convertToJSON(forbidden_areas_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [forbidden_areas]");
                    goto end;
                }
                cJSON_AddItemToArray(forbidden_areasList, itemLocal);
            }
        }
    }

    if (access_and_mobility_subscription_data->service_area_restriction) {
        cJSON *service_area_restriction_local_JSON = OpenAPI_service_area_restriction_convertToJSON(access_and_mobility_subscription_data->service_area_restriction);
        if (service_area_restriction_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [service_area_restriction]");
            goto end;
        }
        cJSON_AddItemToObject(item, "serviceAreaRestriction", service_area_restriction_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [service_area_restriction]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->core_network_type_restrictions) {
        cJSON *core_network_type_restrictions = cJSON_AddArrayToObject(item, "coreNetworkTypeRestrictions");
        if (core_network_type_restrictions == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [core_network_type_restrictions]");
            goto end;
        }
        OpenAPI_lnode_t *core_network_type_restrictions_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->core_network_type_restrictions, core_network_type_restrictions_node) {
            if (cJSON_AddStringToObject(core_network_type_restrictions, "", OpenAPI_core_network_type_ToString((OpenAPI_core_network_type_e)core_network_type_restrictions_node->data)) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [core_network_type_restrictions]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->rfsp_index) {
        if (cJSON_AddNumberToObject(item, "rfspIndex", access_and_mobility_subscription_data->rfsp_index) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [rfsp_index]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->subs_reg_timer) {
        if (cJSON_AddNumberToObject(item, "subsRegTimer", access_and_mobility_subscription_data->subs_reg_timer) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [subs_reg_timer]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->ue_usage_type) {
        if (cJSON_AddNumberToObject(item, "ueUsageType", access_and_mobility_subscription_data->ue_usage_type) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [ue_usage_type]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->mps_priority) {
        if (cJSON_AddBoolToObject(item, "mpsPriority", access_and_mobility_subscription_data->mps_priority) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [mps_priority]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->mcs_priority) {
        if (cJSON_AddBoolToObject(item, "mcsPriority", access_and_mobility_subscription_data->mcs_priority) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [mcs_priority]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->active_time) {
        if (cJSON_AddNumberToObject(item, "activeTime", access_and_mobility_subscription_data->active_time) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [active_time]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->dl_packet_count) {
        if (cJSON_AddNumberToObject(item, "dlPacketCount", access_and_mobility_subscription_data->dl_packet_count) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [dl_packet_count]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->sor_info) {
        cJSON *sor_info_local_JSON = OpenAPI_sor_info_convertToJSON(access_and_mobility_subscription_data->sor_info);
        if (sor_info_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [sor_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "sorInfo", sor_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [sor_info]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->sor_info_expect_ind) {
        if (cJSON_AddBoolToObject(item, "sorInfoExpectInd", access_and_mobility_subscription_data->sor_info_expect_ind) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [sor_info_expect_ind]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->soraf_retrieval) {
        if (cJSON_AddBoolToObject(item, "sorafRetrieval", access_and_mobility_subscription_data->soraf_retrieval) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [soraf_retrieval]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->sor_update_indicator_list) {
        cJSON *sor_update_indicator_list = cJSON_AddArrayToObject(item, "sorUpdateIndicatorList");
        if (sor_update_indicator_list == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [sor_update_indicator_list]");
            goto end;
        }
        OpenAPI_lnode_t *sor_update_indicator_list_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->sor_update_indicator_list, sor_update_indicator_list_node) {
            if (cJSON_AddStringToObject(sor_update_indicator_list, "", OpenAPI_sor_update_indicator_ToString((OpenAPI_sor_update_indicator_e)sor_update_indicator_list_node->data)) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [sor_update_indicator_list]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->upu_info) {
        cJSON *upu_info_local_JSON = OpenAPI_upu_info_convertToJSON(access_and_mobility_subscription_data->upu_info);
        if (upu_info_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [upu_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "upuInfo", upu_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [upu_info]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->mico_allowed) {
        if (cJSON_AddBoolToObject(item, "micoAllowed", access_and_mobility_subscription_data->mico_allowed) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [mico_allowed]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->shared_am_data_ids) {
        cJSON *shared_am_data_ids = cJSON_AddArrayToObject(item, "sharedAmDataIds");
        if (shared_am_data_ids == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [shared_am_data_ids]");
            goto end;
        }

        OpenAPI_lnode_t *shared_am_data_ids_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->shared_am_data_ids, shared_am_data_ids_node)  {
            if (cJSON_AddStringToObject(shared_am_data_ids, "", (char*)shared_am_data_ids_node->data) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [shared_am_data_ids]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->odb_packet_services) {
        cJSON *odb_packet_services_local_JSON = OpenAPI_odb_packet_services_convertToJSON(access_and_mobility_subscription_data->odb_packet_services);
        if (odb_packet_services_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [odb_packet_services]");
            goto end;
        }
        cJSON_AddItemToObject(item, "odbPacketServices", odb_packet_services_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [odb_packet_services]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->subscribed_dnn_list) {
        cJSON *subscribed_dnn_list = cJSON_AddArrayToObject(item, "subscribedDnnList");
        if (subscribed_dnn_list == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [subscribed_dnn_list]");
            goto end;
        }

        OpenAPI_lnode_t *subscribed_dnn_list_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->subscribed_dnn_list, subscribed_dnn_list_node)  {
            if (cJSON_AddStringToObject(subscribed_dnn_list, "", (char*)subscribed_dnn_list_node->data) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [subscribed_dnn_list]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->service_gap_time) {
        if (cJSON_AddNumberToObject(item, "serviceGapTime", access_and_mobility_subscription_data->service_gap_time) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [service_gap_time]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->trace_data) {
        cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(access_and_mobility_subscription_data->trace_data);
        if (trace_data_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [trace_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->cag_data) {
        cJSON *cag_data_local_JSON = OpenAPI_cag_data_convertToJSON(access_and_mobility_subscription_data->cag_data);
        if (cag_data_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [cag_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "cagData", cag_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [cag_data]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->stn_sr) {
        if (cJSON_AddStringToObject(item, "stnSr", access_and_mobility_subscription_data->stn_sr) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [stn_sr]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->c_msisdn) {
        if (cJSON_AddStringToObject(item, "cMsisdn", access_and_mobility_subscription_data->c_msisdn) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [c_msisdn]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->nb_io_t_ue_priority) {
        if (cJSON_AddNumberToObject(item, "nbIoTUePriority", access_and_mobility_subscription_data->nb_io_t_ue_priority) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [nb_io_t_ue_priority]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->nssai_inclusion_allowed) {
        if (cJSON_AddBoolToObject(item, "nssaiInclusionAllowed", access_and_mobility_subscription_data->nssai_inclusion_allowed) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [nssai_inclusion_allowed]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->rg_wireline_characteristics) {
        if (cJSON_AddNumberToObject(item, "rgWirelineCharacteristics", access_and_mobility_subscription_data->rg_wireline_characteristics) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [rg_wireline_characteristics]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->rg_tmbr) {
        cJSON *rg_tmbr_local_JSON = OpenAPI_tmbr_convertToJSON(access_and_mobility_subscription_data->rg_tmbr);
        if (rg_tmbr_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [rg_tmbr]");
            goto end;
        }
        cJSON_AddItemToObject(item, "rgTMBR", rg_tmbr_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [rg_tmbr]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->ec_restriction_data) {
        cJSON *ec_restriction_data_local_JSON = OpenAPI_ec_restriction_data_convertToJSON(access_and_mobility_subscription_data->ec_restriction_data);
        if (ec_restriction_data_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [ec_restriction_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "ecRestrictionData", ec_restriction_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [ec_restriction_data]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->expected_ue_behaviour_list) {
        cJSON *expected_ue_behaviour_list_local_JSON = OpenAPI_expected_ue_behaviour_data_convertToJSON(access_and_mobility_subscription_data->expected_ue_behaviour_list);
        if (expected_ue_behaviour_list_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [expected_ue_behaviour_list]");
            goto end;
        }
        cJSON_AddItemToObject(item, "expectedUeBehaviourList", expected_ue_behaviour_list_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [expected_ue_behaviour_list]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->maximum_response_time_list) {
        cJSON *maximum_response_time_listList = cJSON_AddArrayToObject(item, "maximumResponseTimeList");
        if (maximum_response_time_listList == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [maximum_response_time_list]");
            goto end;
        }

        OpenAPI_lnode_t *maximum_response_time_list_node;
        if (access_and_mobility_subscription_data->maximum_response_time_list) {
            OpenAPI_list_for_each(access_and_mobility_subscription_data->maximum_response_time_list, maximum_response_time_list_node) {
                cJSON *itemLocal = OpenAPI_maximum_response_time_convertToJSON(maximum_response_time_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [maximum_response_time_list]");
                    goto end;
                }
                cJSON_AddItemToArray(maximum_response_time_listList, itemLocal);
            }
        }
    }

    if (access_and_mobility_subscription_data->maximum_latency_list) {
        cJSON *maximum_latency_listList = cJSON_AddArrayToObject(item, "maximumLatencyList");
        if (maximum_latency_listList == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [maximum_latency_list]");
            goto end;
        }

        OpenAPI_lnode_t *maximum_latency_list_node;
        if (access_and_mobility_subscription_data->maximum_latency_list) {
            OpenAPI_list_for_each(access_and_mobility_subscription_data->maximum_latency_list, maximum_latency_list_node) {
                cJSON *itemLocal = OpenAPI_maximum_latency_convertToJSON(maximum_latency_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [maximum_latency_list]");
                    goto end;
                }
                cJSON_AddItemToArray(maximum_latency_listList, itemLocal);
            }
        }
    }

    if (access_and_mobility_subscription_data->primary_rat_restrictions) {
        cJSON *primary_rat_restrictions = cJSON_AddArrayToObject(item, "primaryRatRestrictions");
        if (primary_rat_restrictions == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [primary_rat_restrictions]");
            goto end;
        }
        OpenAPI_lnode_t *primary_rat_restrictions_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->primary_rat_restrictions, primary_rat_restrictions_node) {
            if (cJSON_AddStringToObject(primary_rat_restrictions, "", OpenAPI_rat_type_ToString((OpenAPI_rat_type_e)primary_rat_restrictions_node->data)) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [primary_rat_restrictions]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->secondary_rat_restrictions) {
        cJSON *secondary_rat_restrictions = cJSON_AddArrayToObject(item, "secondaryRatRestrictions");
        if (secondary_rat_restrictions == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [secondary_rat_restrictions]");
            goto end;
        }
        OpenAPI_lnode_t *secondary_rat_restrictions_node;
        OpenAPI_list_for_each(access_and_mobility_subscription_data->secondary_rat_restrictions, secondary_rat_restrictions_node) {
            if (cJSON_AddStringToObject(secondary_rat_restrictions, "", OpenAPI_rat_type_ToString((OpenAPI_rat_type_e)secondary_rat_restrictions_node->data)) == NULL) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [secondary_rat_restrictions]");
                goto end;
            }
        }
    }

    if (access_and_mobility_subscription_data->edrx_parameters_list) {
        cJSON *edrx_parameters_listList = cJSON_AddArrayToObject(item, "edrxParametersList");
        if (edrx_parameters_listList == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [edrx_parameters_list]");
            goto end;
        }

        OpenAPI_lnode_t *edrx_parameters_list_node;
        if (access_and_mobility_subscription_data->edrx_parameters_list) {
            OpenAPI_list_for_each(access_and_mobility_subscription_data->edrx_parameters_list, edrx_parameters_list_node) {
                cJSON *itemLocal = OpenAPI_edrx_parameters_convertToJSON(edrx_parameters_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [edrx_parameters_list]");
                    goto end;
                }
                cJSON_AddItemToArray(edrx_parameters_listList, itemLocal);
            }
        }
    }

    if (access_and_mobility_subscription_data->ptw_parameters_list) {
        cJSON *ptw_parameters_listList = cJSON_AddArrayToObject(item, "ptwParametersList");
        if (ptw_parameters_listList == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [ptw_parameters_list]");
            goto end;
        }

        OpenAPI_lnode_t *ptw_parameters_list_node;
        if (access_and_mobility_subscription_data->ptw_parameters_list) {
            OpenAPI_list_for_each(access_and_mobility_subscription_data->ptw_parameters_list, ptw_parameters_list_node) {
                cJSON *itemLocal = OpenAPI_ptw_parameters_convertToJSON(ptw_parameters_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [ptw_parameters_list]");
                    goto end;
                }
                cJSON_AddItemToArray(ptw_parameters_listList, itemLocal);
            }
        }
    }

    if (access_and_mobility_subscription_data->iab_operation_allowed) {
        if (cJSON_AddBoolToObject(item, "iabOperationAllowed", access_and_mobility_subscription_data->iab_operation_allowed) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [iab_operation_allowed]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->nr_v2x_services_auth) {
        cJSON *nr_v2x_services_auth_local_JSON = OpenAPI_nr_v2x_auth_convertToJSON(access_and_mobility_subscription_data->nr_v2x_services_auth);
        if (nr_v2x_services_auth_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [nr_v2x_services_auth]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nrV2xServicesAuth", nr_v2x_services_auth_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [nr_v2x_services_auth]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->lte_v2x_services_auth) {
        cJSON *lte_v2x_services_auth_local_JSON = OpenAPI_lte_v2x_auth_convertToJSON(access_and_mobility_subscription_data->lte_v2x_services_auth);
        if (lte_v2x_services_auth_local_JSON == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [lte_v2x_services_auth]");
            goto end;
        }
        cJSON_AddItemToObject(item, "lteV2xServicesAuth", lte_v2x_services_auth_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [lte_v2x_services_auth]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->nr_ue_pc5_ambr) {
        if (cJSON_AddStringToObject(item, "nrUePc5Ambr", access_and_mobility_subscription_data->nr_ue_pc5_ambr) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [nr_ue_pc5_ambr]");
            goto end;
        }
    }

    if (access_and_mobility_subscription_data->lte_pc5_ambr) {
        if (cJSON_AddStringToObject(item, "ltePc5Ambr", access_and_mobility_subscription_data->lte_pc5_ambr) == NULL) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed [lte_pc5_ambr]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_access_and_mobility_subscription_data_t *OpenAPI_access_and_mobility_subscription_data_parseFromJSON(cJSON *access_and_mobility_subscription_dataJSON)
{
    OpenAPI_access_and_mobility_subscription_data_t *access_and_mobility_subscription_data_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *gpsis = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "gpsis");

    OpenAPI_list_t *gpsisList;
    if (gpsis) {
        cJSON *gpsis_local;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [gpsis]");
            goto end;
        }
        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    cJSON *internal_group_ids = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "internalGroupIds");

    OpenAPI_list_t *internal_group_idsList;
    if (internal_group_ids) {
        cJSON *internal_group_ids_local;
        if (!cJSON_IsArray(internal_group_ids)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [internal_group_ids]");
            goto end;
        }
        internal_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
            if (!cJSON_IsString(internal_group_ids_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [internal_group_ids]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idsList, ogs_strdup(internal_group_ids_local->valuestring));
        }
    }

    cJSON *vn_group_info = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "vnGroupInfo");

    OpenAPI_list_t *vn_group_infoList;
    if (vn_group_info) {
        cJSON *vn_group_info_local_map;
        if (!cJSON_IsObject(vn_group_info)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [vn_group_info]");
            goto end;
        }
        vn_group_infoList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(vn_group_info_local_map, vn_group_info) {
            cJSON *localMapObject = vn_group_info_local_map;
            if (!cJSON_IsObject(vn_group_info_local_map)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [vn_group_info]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_vn_group_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(vn_group_infoList, localMapKeyPair);
        }
    }

    cJSON *shared_vn_group_data_ids = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "sharedVnGroupDataIds");

    OpenAPI_list_t *shared_vn_group_data_idsList;
    if (shared_vn_group_data_ids) {
        cJSON *shared_vn_group_data_ids_local_map;
        if (!cJSON_IsObject(shared_vn_group_data_ids)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [shared_vn_group_data_ids]");
            goto end;
        }
        shared_vn_group_data_idsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(shared_vn_group_data_ids_local_map, shared_vn_group_data_ids) {
            cJSON *localMapObject = shared_vn_group_data_ids_local_map;
            OpenAPI_list_add(shared_vn_group_data_idsList, localMapKeyPair);
        }
    }

    cJSON *subscribed_ue_ambr = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "subscribedUeAmbr");

    OpenAPI_ambr_rm_t *subscribed_ue_ambr_local_nonprim = NULL;
    if (subscribed_ue_ambr) {
        subscribed_ue_ambr_local_nonprim = OpenAPI_ambr_rm_parseFromJSON(subscribed_ue_ambr);
    }

    cJSON *nssai = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "nssai");

    OpenAPI_nssai_t *nssai_local_nonprim = NULL;
    if (nssai) {
        nssai_local_nonprim = OpenAPI_nssai_parseFromJSON(nssai);
    }

    cJSON *rat_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "ratRestrictions");

    OpenAPI_list_t *rat_restrictionsList;
    if (rat_restrictions) {
        cJSON *rat_restrictions_local_nonprimitive;
        if (!cJSON_IsArray(rat_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [rat_restrictions]");
            goto end;
        }

        rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_restrictions_local_nonprimitive, rat_restrictions ) {
            if (!cJSON_IsString(rat_restrictions_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [rat_restrictions]");
                goto end;
            }

            OpenAPI_list_add(rat_restrictionsList, (void *)OpenAPI_rat_type_FromString(rat_restrictions_local_nonprimitive->valuestring));
        }
    }

    cJSON *forbidden_areas = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "forbiddenAreas");

    OpenAPI_list_t *forbidden_areasList;
    if (forbidden_areas) {
        cJSON *forbidden_areas_local_nonprimitive;
        if (!cJSON_IsArray(forbidden_areas)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [forbidden_areas]");
            goto end;
        }

        forbidden_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(forbidden_areas_local_nonprimitive, forbidden_areas ) {
            if (!cJSON_IsObject(forbidden_areas_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [forbidden_areas]");
                goto end;
            }
            OpenAPI_area_t *forbidden_areasItem = OpenAPI_area_parseFromJSON(forbidden_areas_local_nonprimitive);

            OpenAPI_list_add(forbidden_areasList, forbidden_areasItem);
        }
    }

    cJSON *service_area_restriction = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "serviceAreaRestriction");

    OpenAPI_service_area_restriction_t *service_area_restriction_local_nonprim = NULL;
    if (service_area_restriction) {
        service_area_restriction_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(service_area_restriction);
    }

    cJSON *core_network_type_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "coreNetworkTypeRestrictions");

    OpenAPI_list_t *core_network_type_restrictionsList;
    if (core_network_type_restrictions) {
        cJSON *core_network_type_restrictions_local_nonprimitive;
        if (!cJSON_IsArray(core_network_type_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [core_network_type_restrictions]");
            goto end;
        }

        core_network_type_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(core_network_type_restrictions_local_nonprimitive, core_network_type_restrictions ) {
            if (!cJSON_IsString(core_network_type_restrictions_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [core_network_type_restrictions]");
                goto end;
            }

            OpenAPI_list_add(core_network_type_restrictionsList, (void *)OpenAPI_core_network_type_FromString(core_network_type_restrictions_local_nonprimitive->valuestring));
        }
    }

    cJSON *rfsp_index = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "rfspIndex");

    if (rfsp_index) {
        if (!cJSON_IsNumber(rfsp_index)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [rfsp_index]");
            goto end;
        }
    }

    cJSON *subs_reg_timer = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "subsRegTimer");

    if (subs_reg_timer) {
        if (!cJSON_IsNumber(subs_reg_timer)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [subs_reg_timer]");
            goto end;
        }
    }

    cJSON *ue_usage_type = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "ueUsageType");

    if (ue_usage_type) {
        if (!cJSON_IsNumber(ue_usage_type)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [ue_usage_type]");
            goto end;
        }
    }

    cJSON *mps_priority = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "mpsPriority");

    if (mps_priority) {
        if (!cJSON_IsBool(mps_priority)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [mps_priority]");
            goto end;
        }
    }

    cJSON *mcs_priority = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "mcsPriority");

    if (mcs_priority) {
        if (!cJSON_IsBool(mcs_priority)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [mcs_priority]");
            goto end;
        }
    }

    cJSON *active_time = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "activeTime");

    if (active_time) {
        if (!cJSON_IsNumber(active_time)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [active_time]");
            goto end;
        }
    }

    cJSON *dl_packet_count = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "dlPacketCount");

    if (dl_packet_count) {
        if (!cJSON_IsNumber(dl_packet_count)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [dl_packet_count]");
            goto end;
        }
    }

    cJSON *sor_info = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "sorInfo");

    OpenAPI_sor_info_t *sor_info_local_nonprim = NULL;
    if (sor_info) {
        sor_info_local_nonprim = OpenAPI_sor_info_parseFromJSON(sor_info);
    }

    cJSON *sor_info_expect_ind = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "sorInfoExpectInd");

    if (sor_info_expect_ind) {
        if (!cJSON_IsBool(sor_info_expect_ind)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [sor_info_expect_ind]");
            goto end;
        }
    }

    cJSON *soraf_retrieval = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "sorafRetrieval");

    if (soraf_retrieval) {
        if (!cJSON_IsBool(soraf_retrieval)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [soraf_retrieval]");
            goto end;
        }
    }

    cJSON *sor_update_indicator_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "sorUpdateIndicatorList");

    OpenAPI_list_t *sor_update_indicator_listList;
    if (sor_update_indicator_list) {
        cJSON *sor_update_indicator_list_local_nonprimitive;
        if (!cJSON_IsArray(sor_update_indicator_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [sor_update_indicator_list]");
            goto end;
        }

        sor_update_indicator_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(sor_update_indicator_list_local_nonprimitive, sor_update_indicator_list ) {
            if (!cJSON_IsString(sor_update_indicator_list_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [sor_update_indicator_list]");
                goto end;
            }

            OpenAPI_list_add(sor_update_indicator_listList, (void *)OpenAPI_sor_update_indicator_FromString(sor_update_indicator_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *upu_info = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "upuInfo");

    OpenAPI_upu_info_t *upu_info_local_nonprim = NULL;
    if (upu_info) {
        upu_info_local_nonprim = OpenAPI_upu_info_parseFromJSON(upu_info);
    }

    cJSON *mico_allowed = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "micoAllowed");

    if (mico_allowed) {
        if (!cJSON_IsBool(mico_allowed)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [mico_allowed]");
            goto end;
        }
    }

    cJSON *shared_am_data_ids = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "sharedAmDataIds");

    OpenAPI_list_t *shared_am_data_idsList;
    if (shared_am_data_ids) {
        cJSON *shared_am_data_ids_local;
        if (!cJSON_IsArray(shared_am_data_ids)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [shared_am_data_ids]");
            goto end;
        }
        shared_am_data_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_am_data_ids_local, shared_am_data_ids) {
            if (!cJSON_IsString(shared_am_data_ids_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [shared_am_data_ids]");
                goto end;
            }
            OpenAPI_list_add(shared_am_data_idsList, ogs_strdup(shared_am_data_ids_local->valuestring));
        }
    }

    cJSON *odb_packet_services = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "odbPacketServices");

    OpenAPI_odb_packet_services_t *odb_packet_services_local_nonprim = NULL;
    if (odb_packet_services) {
        odb_packet_services_local_nonprim = OpenAPI_odb_packet_services_parseFromJSON(odb_packet_services);
    }

    cJSON *subscribed_dnn_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "subscribedDnnList");

    OpenAPI_list_t *subscribed_dnn_listList;
    if (subscribed_dnn_list) {
        cJSON *subscribed_dnn_list_local;
        if (!cJSON_IsArray(subscribed_dnn_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [subscribed_dnn_list]");
            goto end;
        }
        subscribed_dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_dnn_list_local, subscribed_dnn_list) {
            if (!cJSON_IsString(subscribed_dnn_list_local)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [subscribed_dnn_list]");
                goto end;
            }
            OpenAPI_list_add(subscribed_dnn_listList, ogs_strdup(subscribed_dnn_list_local->valuestring));
        }
    }

    cJSON *service_gap_time = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "serviceGapTime");

    if (service_gap_time) {
        if (!cJSON_IsNumber(service_gap_time)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [service_gap_time]");
            goto end;
        }
    }

    cJSON *trace_data = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "traceData");

    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    if (trace_data) {
        trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    }

    cJSON *cag_data = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "cagData");

    OpenAPI_cag_data_t *cag_data_local_nonprim = NULL;
    if (cag_data) {
        cag_data_local_nonprim = OpenAPI_cag_data_parseFromJSON(cag_data);
    }

    cJSON *stn_sr = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "stnSr");

    if (stn_sr) {
        if (!cJSON_IsString(stn_sr)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [stn_sr]");
            goto end;
        }
    }

    cJSON *c_msisdn = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "cMsisdn");

    if (c_msisdn) {
        if (!cJSON_IsString(c_msisdn)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [c_msisdn]");
            goto end;
        }
    }

    cJSON *nb_io_t_ue_priority = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "nbIoTUePriority");

    if (nb_io_t_ue_priority) {
        if (!cJSON_IsNumber(nb_io_t_ue_priority)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [nb_io_t_ue_priority]");
            goto end;
        }
    }

    cJSON *nssai_inclusion_allowed = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "nssaiInclusionAllowed");

    if (nssai_inclusion_allowed) {
        if (!cJSON_IsBool(nssai_inclusion_allowed)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [nssai_inclusion_allowed]");
            goto end;
        }
    }

    cJSON *rg_wireline_characteristics = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "rgWirelineCharacteristics");

    if (rg_wireline_characteristics) {
        if (!cJSON_IsNumber(rg_wireline_characteristics)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [rg_wireline_characteristics]");
            goto end;
        }
    }

    cJSON *rg_tmbr = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "rgTMBR");

    OpenAPI_tmbr_t *rg_tmbr_local_nonprim = NULL;
    if (rg_tmbr) {
        rg_tmbr_local_nonprim = OpenAPI_tmbr_parseFromJSON(rg_tmbr);
    }

    cJSON *ec_restriction_data = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "ecRestrictionData");

    OpenAPI_ec_restriction_data_t *ec_restriction_data_local_nonprim = NULL;
    if (ec_restriction_data) {
        ec_restriction_data_local_nonprim = OpenAPI_ec_restriction_data_parseFromJSON(ec_restriction_data);
    }

    cJSON *expected_ue_behaviour_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "expectedUeBehaviourList");

    OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_list_local_nonprim = NULL;
    if (expected_ue_behaviour_list) {
        expected_ue_behaviour_list_local_nonprim = OpenAPI_expected_ue_behaviour_data_parseFromJSON(expected_ue_behaviour_list);
    }

    cJSON *maximum_response_time_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "maximumResponseTimeList");

    OpenAPI_list_t *maximum_response_time_listList;
    if (maximum_response_time_list) {
        cJSON *maximum_response_time_list_local_nonprimitive;
        if (!cJSON_IsArray(maximum_response_time_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [maximum_response_time_list]");
            goto end;
        }

        maximum_response_time_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(maximum_response_time_list_local_nonprimitive, maximum_response_time_list ) {
            if (!cJSON_IsObject(maximum_response_time_list_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [maximum_response_time_list]");
                goto end;
            }
            OpenAPI_maximum_response_time_t *maximum_response_time_listItem = OpenAPI_maximum_response_time_parseFromJSON(maximum_response_time_list_local_nonprimitive);

            OpenAPI_list_add(maximum_response_time_listList, maximum_response_time_listItem);
        }
    }

    cJSON *maximum_latency_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "maximumLatencyList");

    OpenAPI_list_t *maximum_latency_listList;
    if (maximum_latency_list) {
        cJSON *maximum_latency_list_local_nonprimitive;
        if (!cJSON_IsArray(maximum_latency_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [maximum_latency_list]");
            goto end;
        }

        maximum_latency_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(maximum_latency_list_local_nonprimitive, maximum_latency_list ) {
            if (!cJSON_IsObject(maximum_latency_list_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [maximum_latency_list]");
                goto end;
            }
            OpenAPI_maximum_latency_t *maximum_latency_listItem = OpenAPI_maximum_latency_parseFromJSON(maximum_latency_list_local_nonprimitive);

            OpenAPI_list_add(maximum_latency_listList, maximum_latency_listItem);
        }
    }

    cJSON *primary_rat_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "primaryRatRestrictions");

    OpenAPI_list_t *primary_rat_restrictionsList;
    if (primary_rat_restrictions) {
        cJSON *primary_rat_restrictions_local_nonprimitive;
        if (!cJSON_IsArray(primary_rat_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [primary_rat_restrictions]");
            goto end;
        }

        primary_rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(primary_rat_restrictions_local_nonprimitive, primary_rat_restrictions ) {
            if (!cJSON_IsString(primary_rat_restrictions_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [primary_rat_restrictions]");
                goto end;
            }

            OpenAPI_list_add(primary_rat_restrictionsList, (void *)OpenAPI_rat_type_FromString(primary_rat_restrictions_local_nonprimitive->valuestring));
        }
    }

    cJSON *secondary_rat_restrictions = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "secondaryRatRestrictions");

    OpenAPI_list_t *secondary_rat_restrictionsList;
    if (secondary_rat_restrictions) {
        cJSON *secondary_rat_restrictions_local_nonprimitive;
        if (!cJSON_IsArray(secondary_rat_restrictions)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [secondary_rat_restrictions]");
            goto end;
        }

        secondary_rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_restrictions_local_nonprimitive, secondary_rat_restrictions ) {
            if (!cJSON_IsString(secondary_rat_restrictions_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [secondary_rat_restrictions]");
                goto end;
            }

            OpenAPI_list_add(secondary_rat_restrictionsList, (void *)OpenAPI_rat_type_FromString(secondary_rat_restrictions_local_nonprimitive->valuestring));
        }
    }

    cJSON *edrx_parameters_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "edrxParametersList");

    OpenAPI_list_t *edrx_parameters_listList;
    if (edrx_parameters_list) {
        cJSON *edrx_parameters_list_local_nonprimitive;
        if (!cJSON_IsArray(edrx_parameters_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [edrx_parameters_list]");
            goto end;
        }

        edrx_parameters_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(edrx_parameters_list_local_nonprimitive, edrx_parameters_list ) {
            if (!cJSON_IsObject(edrx_parameters_list_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [edrx_parameters_list]");
                goto end;
            }
            OpenAPI_edrx_parameters_t *edrx_parameters_listItem = OpenAPI_edrx_parameters_parseFromJSON(edrx_parameters_list_local_nonprimitive);

            OpenAPI_list_add(edrx_parameters_listList, edrx_parameters_listItem);
        }
    }

    cJSON *ptw_parameters_list = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "ptwParametersList");

    OpenAPI_list_t *ptw_parameters_listList;
    if (ptw_parameters_list) {
        cJSON *ptw_parameters_list_local_nonprimitive;
        if (!cJSON_IsArray(ptw_parameters_list)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [ptw_parameters_list]");
            goto end;
        }

        ptw_parameters_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ptw_parameters_list_local_nonprimitive, ptw_parameters_list ) {
            if (!cJSON_IsObject(ptw_parameters_list_local_nonprimitive)) {
                ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [ptw_parameters_list]");
                goto end;
            }
            OpenAPI_ptw_parameters_t *ptw_parameters_listItem = OpenAPI_ptw_parameters_parseFromJSON(ptw_parameters_list_local_nonprimitive);

            OpenAPI_list_add(ptw_parameters_listList, ptw_parameters_listItem);
        }
    }

    cJSON *iab_operation_allowed = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "iabOperationAllowed");

    if (iab_operation_allowed) {
        if (!cJSON_IsBool(iab_operation_allowed)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [iab_operation_allowed]");
            goto end;
        }
    }

    cJSON *nr_v2x_services_auth = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "nrV2xServicesAuth");

    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth_local_nonprim = NULL;
    if (nr_v2x_services_auth) {
        nr_v2x_services_auth_local_nonprim = OpenAPI_nr_v2x_auth_parseFromJSON(nr_v2x_services_auth);
    }

    cJSON *lte_v2x_services_auth = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "lteV2xServicesAuth");

    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth_local_nonprim = NULL;
    if (lte_v2x_services_auth) {
        lte_v2x_services_auth_local_nonprim = OpenAPI_lte_v2x_auth_parseFromJSON(lte_v2x_services_auth);
    }

    cJSON *nr_ue_pc5_ambr = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "nrUePc5Ambr");

    if (nr_ue_pc5_ambr) {
        if (!cJSON_IsString(nr_ue_pc5_ambr)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [nr_ue_pc5_ambr]");
            goto end;
        }
    }

    cJSON *lte_pc5_ambr = cJSON_GetObjectItemCaseSensitive(access_and_mobility_subscription_dataJSON, "ltePc5Ambr");

    if (lte_pc5_ambr) {
        if (!cJSON_IsString(lte_pc5_ambr)) {
            ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON() failed [lte_pc5_ambr]");
            goto end;
        }
    }

    access_and_mobility_subscription_data_local_var = OpenAPI_access_and_mobility_subscription_data_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        gpsis ? gpsisList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        vn_group_info ? vn_group_infoList : NULL,
        shared_vn_group_data_ids ? shared_vn_group_data_idsList : NULL,
        subscribed_ue_ambr ? subscribed_ue_ambr_local_nonprim : NULL,
        nssai ? nssai_local_nonprim : NULL,
        rat_restrictions ? rat_restrictionsList : NULL,
        forbidden_areas ? forbidden_areasList : NULL,
        service_area_restriction ? service_area_restriction_local_nonprim : NULL,
        core_network_type_restrictions ? core_network_type_restrictionsList : NULL,
        rfsp_index ? rfsp_index->valuedouble : 0,
        subs_reg_timer ? subs_reg_timer->valuedouble : 0,
        ue_usage_type ? ue_usage_type->valuedouble : 0,
        mps_priority ? mps_priority->valueint : 0,
        mcs_priority ? mcs_priority->valueint : 0,
        active_time ? active_time->valuedouble : 0,
        dl_packet_count ? dl_packet_count->valuedouble : 0,
        sor_info ? sor_info_local_nonprim : NULL,
        sor_info_expect_ind ? sor_info_expect_ind->valueint : 0,
        soraf_retrieval ? soraf_retrieval->valueint : 0,
        sor_update_indicator_list ? sor_update_indicator_listList : NULL,
        upu_info ? upu_info_local_nonprim : NULL,
        mico_allowed ? mico_allowed->valueint : 0,
        shared_am_data_ids ? shared_am_data_idsList : NULL,
        odb_packet_services ? odb_packet_services_local_nonprim : NULL,
        subscribed_dnn_list ? subscribed_dnn_listList : NULL,
        service_gap_time ? service_gap_time->valuedouble : 0,
        trace_data ? trace_data_local_nonprim : NULL,
        cag_data ? cag_data_local_nonprim : NULL,
        stn_sr ? ogs_strdup(stn_sr->valuestring) : NULL,
        c_msisdn ? ogs_strdup(c_msisdn->valuestring) : NULL,
        nb_io_t_ue_priority ? nb_io_t_ue_priority->valuedouble : 0,
        nssai_inclusion_allowed ? nssai_inclusion_allowed->valueint : 0,
        rg_wireline_characteristics ? rg_wireline_characteristics->valueint : 0,
        rg_tmbr ? rg_tmbr_local_nonprim : NULL,
        ec_restriction_data ? ec_restriction_data_local_nonprim : NULL,
        expected_ue_behaviour_list ? expected_ue_behaviour_list_local_nonprim : NULL,
        maximum_response_time_list ? maximum_response_time_listList : NULL,
        maximum_latency_list ? maximum_latency_listList : NULL,
        primary_rat_restrictions ? primary_rat_restrictionsList : NULL,
        secondary_rat_restrictions ? secondary_rat_restrictionsList : NULL,
        edrx_parameters_list ? edrx_parameters_listList : NULL,
        ptw_parameters_list ? ptw_parameters_listList : NULL,
        iab_operation_allowed ? iab_operation_allowed->valueint : 0,
        nr_v2x_services_auth ? nr_v2x_services_auth_local_nonprim : NULL,
        lte_v2x_services_auth ? lte_v2x_services_auth_local_nonprim : NULL,
        nr_ue_pc5_ambr ? ogs_strdup(nr_ue_pc5_ambr->valuestring) : NULL,
        lte_pc5_ambr ? ogs_strdup(lte_pc5_ambr->valuestring) : NULL
        );

    return access_and_mobility_subscription_data_local_var;
end:
    return NULL;
}

OpenAPI_access_and_mobility_subscription_data_t *OpenAPI_access_and_mobility_subscription_data_copy(OpenAPI_access_and_mobility_subscription_data_t *dst, OpenAPI_access_and_mobility_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_and_mobility_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_convertToJSON() failed");
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

    OpenAPI_access_and_mobility_subscription_data_free(dst);
    dst = OpenAPI_access_and_mobility_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

