
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event.h"

OpenAPI_amf_event_t *OpenAPI_amf_event_create(
    OpenAPI_amf_event_type_e type,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_list_t *area_list,
    OpenAPI_list_t *location_filter_list,
    OpenAPI_list_t *location_trends_filter_list,
    bool is_ref_id,
    int ref_id,
    OpenAPI_list_t *traffic_descriptor_list,
    bool is_report_ue_reachable,
    int report_ue_reachable,
    OpenAPI_reachability_filter_e reachability_filter,
    bool is_udm_detect_ind,
    int udm_detect_ind,
    bool is_max_reports,
    int max_reports,
    OpenAPI_list_t* presence_info_list,
    bool is_max_response_time,
    int max_response_time,
    OpenAPI_target_area_t *target_area,
    OpenAPI_list_t *snssai_filter,
    OpenAPI_ue_in_area_filter_t *ue_in_area_filter,
    bool is_min_interval,
    int min_interval,
    char *next_report,
    bool is_idle_status_ind,
    int idle_status_ind,
    OpenAPI_dispersion_area_t *dispersion_area,
    char *next_periodic_report_time,
    bool is_adjust_ao_ion_ra,
    int adjust_ao_ion_ra,
    bool is_ran_timing_synchro_status_change,
    int ran_timing_synchro_status_change,
    OpenAPI_list_t *notify_for_supi_list,
    OpenAPI_list_t *notify_for_group_list,
    OpenAPI_list_t *notify_for_snssai_dnn_list,
    bool is_reporting_threshold,
    int reporting_threshold,
    OpenAPI_trajectory_t *assign_trajectory,
    OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter,
    bool is_ue_pos_cap_requested_ind,
    int ue_pos_cap_requested_ind,
    OpenAPI_list_t *tw_list,
    OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config
)
{
    OpenAPI_amf_event_t *amf_event_local_var = ogs_malloc(sizeof(OpenAPI_amf_event_t));
    ogs_assert(amf_event_local_var);

    amf_event_local_var->type = type;
    amf_event_local_var->is_immediate_flag = is_immediate_flag;
    amf_event_local_var->immediate_flag = immediate_flag;
    amf_event_local_var->area_list = area_list;
    amf_event_local_var->location_filter_list = location_filter_list;
    amf_event_local_var->location_trends_filter_list = location_trends_filter_list;
    amf_event_local_var->is_ref_id = is_ref_id;
    amf_event_local_var->ref_id = ref_id;
    amf_event_local_var->traffic_descriptor_list = traffic_descriptor_list;
    amf_event_local_var->is_report_ue_reachable = is_report_ue_reachable;
    amf_event_local_var->report_ue_reachable = report_ue_reachable;
    amf_event_local_var->reachability_filter = reachability_filter;
    amf_event_local_var->is_udm_detect_ind = is_udm_detect_ind;
    amf_event_local_var->udm_detect_ind = udm_detect_ind;
    amf_event_local_var->is_max_reports = is_max_reports;
    amf_event_local_var->max_reports = max_reports;
    amf_event_local_var->presence_info_list = presence_info_list;
    amf_event_local_var->is_max_response_time = is_max_response_time;
    amf_event_local_var->max_response_time = max_response_time;
    amf_event_local_var->target_area = target_area;
    amf_event_local_var->snssai_filter = snssai_filter;
    amf_event_local_var->ue_in_area_filter = ue_in_area_filter;
    amf_event_local_var->is_min_interval = is_min_interval;
    amf_event_local_var->min_interval = min_interval;
    amf_event_local_var->next_report = next_report;
    amf_event_local_var->is_idle_status_ind = is_idle_status_ind;
    amf_event_local_var->idle_status_ind = idle_status_ind;
    amf_event_local_var->dispersion_area = dispersion_area;
    amf_event_local_var->next_periodic_report_time = next_periodic_report_time;
    amf_event_local_var->is_adjust_ao_ion_ra = is_adjust_ao_ion_ra;
    amf_event_local_var->adjust_ao_ion_ra = adjust_ao_ion_ra;
    amf_event_local_var->is_ran_timing_synchro_status_change = is_ran_timing_synchro_status_change;
    amf_event_local_var->ran_timing_synchro_status_change = ran_timing_synchro_status_change;
    amf_event_local_var->notify_for_supi_list = notify_for_supi_list;
    amf_event_local_var->notify_for_group_list = notify_for_group_list;
    amf_event_local_var->notify_for_snssai_dnn_list = notify_for_snssai_dnn_list;
    amf_event_local_var->is_reporting_threshold = is_reporting_threshold;
    amf_event_local_var->reporting_threshold = reporting_threshold;
    amf_event_local_var->assign_trajectory = assign_trajectory;
    amf_event_local_var->sm_comm_failure_filter = sm_comm_failure_filter;
    amf_event_local_var->is_ue_pos_cap_requested_ind = is_ue_pos_cap_requested_ind;
    amf_event_local_var->ue_pos_cap_requested_ind = ue_pos_cap_requested_ind;
    amf_event_local_var->tw_list = tw_list;
    amf_event_local_var->uav_altitude_reporting_config = uav_altitude_reporting_config;

    return amf_event_local_var;
}

void OpenAPI_amf_event_free(OpenAPI_amf_event_t *amf_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_event) {
        return;
    }
    if (amf_event->area_list) {
        OpenAPI_list_for_each(amf_event->area_list, node) {
            OpenAPI_amf_event_area_free(node->data);
        }
        OpenAPI_list_free(amf_event->area_list);
        amf_event->area_list = NULL;
    }
    if (amf_event->location_filter_list) {
        OpenAPI_list_free(amf_event->location_filter_list);
        amf_event->location_filter_list = NULL;
    }
    if (amf_event->location_trends_filter_list) {
        OpenAPI_list_free(amf_event->location_trends_filter_list);
        amf_event->location_trends_filter_list = NULL;
    }
    if (amf_event->traffic_descriptor_list) {
        OpenAPI_list_for_each(amf_event->traffic_descriptor_list, node) {
            OpenAPI_traffic_descriptor_free(node->data);
        }
        OpenAPI_list_free(amf_event->traffic_descriptor_list);
        amf_event->traffic_descriptor_list = NULL;
    }
    if (amf_event->presence_info_list) {
        OpenAPI_list_for_each(amf_event->presence_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(amf_event->presence_info_list);
        amf_event->presence_info_list = NULL;
    }
    if (amf_event->target_area) {
        OpenAPI_target_area_free(amf_event->target_area);
        amf_event->target_area = NULL;
    }
    if (amf_event->snssai_filter) {
        OpenAPI_list_for_each(amf_event->snssai_filter, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(amf_event->snssai_filter);
        amf_event->snssai_filter = NULL;
    }
    if (amf_event->ue_in_area_filter) {
        OpenAPI_ue_in_area_filter_free(amf_event->ue_in_area_filter);
        amf_event->ue_in_area_filter = NULL;
    }
    if (amf_event->next_report) {
        ogs_free(amf_event->next_report);
        amf_event->next_report = NULL;
    }
    if (amf_event->dispersion_area) {
        OpenAPI_dispersion_area_free(amf_event->dispersion_area);
        amf_event->dispersion_area = NULL;
    }
    if (amf_event->next_periodic_report_time) {
        ogs_free(amf_event->next_periodic_report_time);
        amf_event->next_periodic_report_time = NULL;
    }
    if (amf_event->notify_for_supi_list) {
        OpenAPI_list_for_each(amf_event->notify_for_supi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(amf_event->notify_for_supi_list);
        amf_event->notify_for_supi_list = NULL;
    }
    if (amf_event->notify_for_group_list) {
        OpenAPI_list_for_each(amf_event->notify_for_group_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(amf_event->notify_for_group_list);
        amf_event->notify_for_group_list = NULL;
    }
    if (amf_event->notify_for_snssai_dnn_list) {
        OpenAPI_list_for_each(amf_event->notify_for_snssai_dnn_list, node) {
            OpenAPI_snssai_dnn_item_free(node->data);
        }
        OpenAPI_list_free(amf_event->notify_for_snssai_dnn_list);
        amf_event->notify_for_snssai_dnn_list = NULL;
    }
    if (amf_event->assign_trajectory) {
        OpenAPI_trajectory_free(amf_event->assign_trajectory);
        amf_event->assign_trajectory = NULL;
    }
    if (amf_event->sm_comm_failure_filter) {
        OpenAPI_sm_comm_failure_filter_free(amf_event->sm_comm_failure_filter);
        amf_event->sm_comm_failure_filter = NULL;
    }
    if (amf_event->tw_list) {
        OpenAPI_list_for_each(amf_event->tw_list, node) {
            OpenAPI_time_window_free(node->data);
        }
        OpenAPI_list_free(amf_event->tw_list);
        amf_event->tw_list = NULL;
    }
    if (amf_event->uav_altitude_reporting_config) {
        OpenAPI_uav_altitude_reporting_config_free(amf_event->uav_altitude_reporting_config);
        amf_event->uav_altitude_reporting_config = NULL;
    }
    ogs_free(amf_event);
}

cJSON *OpenAPI_amf_event_convertToJSON(OpenAPI_amf_event_t *amf_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_event == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [AmfEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (amf_event->type == OpenAPI_amf_event_type_NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "type", OpenAPI_amf_event_type_ToString(amf_event->type)) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [type]");
        goto end;
    }

    if (amf_event->is_immediate_flag) {
    if (cJSON_AddBoolToObject(item, "immediateFlag", amf_event->immediate_flag) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [immediate_flag]");
        goto end;
    }
    }

    if (amf_event->area_list) {
    cJSON *area_listList = cJSON_AddArrayToObject(item, "areaList");
    if (area_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [area_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->area_list, node) {
        cJSON *itemLocal = OpenAPI_amf_event_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [area_list]");
            goto end;
        }
        cJSON_AddItemToArray(area_listList, itemLocal);
    }
    }

    if (amf_event->location_filter_list != OpenAPI_location_filter_NULL) {
    cJSON *location_filter_listList = cJSON_AddArrayToObject(item, "locationFilterList");
    if (location_filter_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [location_filter_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->location_filter_list, node) {
        if (cJSON_AddStringToObject(location_filter_listList, "", OpenAPI_location_filter_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [location_filter_list]");
            goto end;
        }
    }
    }

    if (amf_event->location_trends_filter_list != OpenAPI_location_filter_NULL) {
    cJSON *location_trends_filter_listList = cJSON_AddArrayToObject(item, "locationTrendsFilterList");
    if (location_trends_filter_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [location_trends_filter_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->location_trends_filter_list, node) {
        if (cJSON_AddStringToObject(location_trends_filter_listList, "", OpenAPI_location_filter_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [location_trends_filter_list]");
            goto end;
        }
    }
    }

    if (amf_event->is_ref_id) {
    if (cJSON_AddNumberToObject(item, "refId", amf_event->ref_id) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [ref_id]");
        goto end;
    }
    }

    if (amf_event->traffic_descriptor_list) {
    cJSON *traffic_descriptor_listList = cJSON_AddArrayToObject(item, "trafficDescriptorList");
    if (traffic_descriptor_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [traffic_descriptor_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->traffic_descriptor_list, node) {
        cJSON *itemLocal = OpenAPI_traffic_descriptor_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [traffic_descriptor_list]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_descriptor_listList, itemLocal);
    }
    }

    if (amf_event->is_report_ue_reachable) {
    if (cJSON_AddBoolToObject(item, "reportUeReachable", amf_event->report_ue_reachable) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [report_ue_reachable]");
        goto end;
    }
    }

    if (amf_event->reachability_filter != OpenAPI_reachability_filter_NULL) {
    if (cJSON_AddStringToObject(item, "reachabilityFilter", OpenAPI_reachability_filter_ToString(amf_event->reachability_filter)) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [reachability_filter]");
        goto end;
    }
    }

    if (amf_event->is_udm_detect_ind) {
    if (cJSON_AddBoolToObject(item, "udmDetectInd", amf_event->udm_detect_ind) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [udm_detect_ind]");
        goto end;
    }
    }

    if (amf_event->is_max_reports) {
    if (cJSON_AddNumberToObject(item, "maxReports", amf_event->max_reports) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [max_reports]");
        goto end;
    }
    }

    if (amf_event->presence_info_list) {
    cJSON *presence_info_list = cJSON_AddObjectToObject(item, "presenceInfoList");
    if (presence_info_list == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [presence_info_list]");
        goto end;
    }
    cJSON *localMapObject = presence_info_list;
    if (amf_event->presence_info_list) {
        OpenAPI_list_for_each(amf_event->presence_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_amf_event_convertToJSON() failed [presence_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_amf_event_convertToJSON() failed [presence_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_event_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (amf_event->is_max_response_time) {
    if (cJSON_AddNumberToObject(item, "maxResponseTime", amf_event->max_response_time) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [max_response_time]");
        goto end;
    }
    }

    if (amf_event->target_area) {
    cJSON *target_area_local_JSON = OpenAPI_target_area_convertToJSON(amf_event->target_area);
    if (target_area_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [target_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetArea", target_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [target_area]");
        goto end;
    }
    }

    if (amf_event->snssai_filter) {
    cJSON *snssai_filterList = cJSON_AddArrayToObject(item, "snssaiFilter");
    if (snssai_filterList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [snssai_filter]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->snssai_filter, node) {
        cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [snssai_filter]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_filterList, itemLocal);
    }
    }

    if (amf_event->ue_in_area_filter) {
    cJSON *ue_in_area_filter_local_JSON = OpenAPI_ue_in_area_filter_convertToJSON(amf_event->ue_in_area_filter);
    if (ue_in_area_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [ue_in_area_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueInAreaFilter", ue_in_area_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [ue_in_area_filter]");
        goto end;
    }
    }

    if (amf_event->is_min_interval) {
    if (cJSON_AddNumberToObject(item, "minInterval", amf_event->min_interval) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [min_interval]");
        goto end;
    }
    }

    if (amf_event->next_report) {
    if (cJSON_AddStringToObject(item, "nextReport", amf_event->next_report) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [next_report]");
        goto end;
    }
    }

    if (amf_event->is_idle_status_ind) {
    if (cJSON_AddBoolToObject(item, "idleStatusInd", amf_event->idle_status_ind) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [idle_status_ind]");
        goto end;
    }
    }

    if (amf_event->dispersion_area) {
    cJSON *dispersion_area_local_JSON = OpenAPI_dispersion_area_convertToJSON(amf_event->dispersion_area);
    if (dispersion_area_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [dispersion_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dispersionArea", dispersion_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [dispersion_area]");
        goto end;
    }
    }

    if (amf_event->next_periodic_report_time) {
    if (cJSON_AddStringToObject(item, "nextPeriodicReportTime", amf_event->next_periodic_report_time) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [next_periodic_report_time]");
        goto end;
    }
    }

    if (amf_event->is_adjust_ao_ion_ra) {
    if (cJSON_AddBoolToObject(item, "adjustAoIOnRa", amf_event->adjust_ao_ion_ra) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [adjust_ao_ion_ra]");
        goto end;
    }
    }

    if (amf_event->is_ran_timing_synchro_status_change) {
    if (cJSON_AddBoolToObject(item, "ranTimingSynchroStatusChange", amf_event->ran_timing_synchro_status_change) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [ran_timing_synchro_status_change]");
        goto end;
    }
    }

    if (amf_event->notify_for_supi_list) {
    cJSON *notify_for_supi_listList = cJSON_AddArrayToObject(item, "notifyForSupiList");
    if (notify_for_supi_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [notify_for_supi_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->notify_for_supi_list, node) {
        if (cJSON_AddStringToObject(notify_for_supi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [notify_for_supi_list]");
            goto end;
        }
    }
    }

    if (amf_event->notify_for_group_list) {
    cJSON *notify_for_group_listList = cJSON_AddArrayToObject(item, "notifyForGroupList");
    if (notify_for_group_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [notify_for_group_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->notify_for_group_list, node) {
        if (cJSON_AddStringToObject(notify_for_group_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [notify_for_group_list]");
            goto end;
        }
    }
    }

    if (amf_event->notify_for_snssai_dnn_list) {
    cJSON *notify_for_snssai_dnn_listList = cJSON_AddArrayToObject(item, "notifyForSnssaiDnnList");
    if (notify_for_snssai_dnn_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [notify_for_snssai_dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->notify_for_snssai_dnn_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_dnn_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [notify_for_snssai_dnn_list]");
            goto end;
        }
        cJSON_AddItemToArray(notify_for_snssai_dnn_listList, itemLocal);
    }
    }

    if (amf_event->is_reporting_threshold) {
    if (cJSON_AddNumberToObject(item, "reportingThreshold", amf_event->reporting_threshold) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [reporting_threshold]");
        goto end;
    }
    }

    if (amf_event->assign_trajectory) {
    cJSON *assign_trajectory_local_JSON = OpenAPI_trajectory_convertToJSON(amf_event->assign_trajectory);
    if (assign_trajectory_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [assign_trajectory]");
        goto end;
    }
    cJSON_AddItemToObject(item, "assignTrajectory", assign_trajectory_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [assign_trajectory]");
        goto end;
    }
    }

    if (amf_event->sm_comm_failure_filter) {
    cJSON *sm_comm_failure_filter_local_JSON = OpenAPI_sm_comm_failure_filter_convertToJSON(amf_event->sm_comm_failure_filter);
    if (sm_comm_failure_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [sm_comm_failure_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smCommFailureFilter", sm_comm_failure_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [sm_comm_failure_filter]");
        goto end;
    }
    }

    if (amf_event->is_ue_pos_cap_requested_ind) {
    if (cJSON_AddBoolToObject(item, "uePosCapRequestedInd", amf_event->ue_pos_cap_requested_ind) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [ue_pos_cap_requested_ind]");
        goto end;
    }
    }

    if (amf_event->tw_list) {
    cJSON *tw_listList = cJSON_AddArrayToObject(item, "twList");
    if (tw_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [tw_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event->tw_list, node) {
        cJSON *itemLocal = OpenAPI_time_window_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_event_convertToJSON() failed [tw_list]");
            goto end;
        }
        cJSON_AddItemToArray(tw_listList, itemLocal);
    }
    }

    if (amf_event->uav_altitude_reporting_config) {
    cJSON *uav_altitude_reporting_config_local_JSON = OpenAPI_uav_altitude_reporting_config_convertToJSON(amf_event->uav_altitude_reporting_config);
    if (uav_altitude_reporting_config_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [uav_altitude_reporting_config]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uavAltitudeReportingConfig", uav_altitude_reporting_config_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [uav_altitude_reporting_config]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_event_t *OpenAPI_amf_event_parseFromJSON(cJSON *amf_eventJSON)
{
    OpenAPI_amf_event_t *amf_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *type = NULL;
    OpenAPI_amf_event_type_e typeVariable = 0;
    cJSON *immediate_flag = NULL;
    cJSON *area_list = NULL;
    OpenAPI_list_t *area_listList = NULL;
    cJSON *location_filter_list = NULL;
    OpenAPI_list_t *location_filter_listList = NULL;
    cJSON *location_trends_filter_list = NULL;
    OpenAPI_list_t *location_trends_filter_listList = NULL;
    cJSON *ref_id = NULL;
    cJSON *traffic_descriptor_list = NULL;
    OpenAPI_list_t *traffic_descriptor_listList = NULL;
    cJSON *report_ue_reachable = NULL;
    cJSON *reachability_filter = NULL;
    OpenAPI_reachability_filter_e reachability_filterVariable = 0;
    cJSON *udm_detect_ind = NULL;
    cJSON *max_reports = NULL;
    cJSON *presence_info_list = NULL;
    OpenAPI_list_t *presence_info_listList = NULL;
    cJSON *max_response_time = NULL;
    cJSON *target_area = NULL;
    OpenAPI_target_area_t *target_area_local_nonprim = NULL;
    cJSON *snssai_filter = NULL;
    OpenAPI_list_t *snssai_filterList = NULL;
    cJSON *ue_in_area_filter = NULL;
    OpenAPI_ue_in_area_filter_t *ue_in_area_filter_local_nonprim = NULL;
    cJSON *min_interval = NULL;
    cJSON *next_report = NULL;
    cJSON *idle_status_ind = NULL;
    cJSON *dispersion_area = NULL;
    OpenAPI_dispersion_area_t *dispersion_area_local_nonprim = NULL;
    cJSON *next_periodic_report_time = NULL;
    cJSON *adjust_ao_ion_ra = NULL;
    cJSON *ran_timing_synchro_status_change = NULL;
    cJSON *notify_for_supi_list = NULL;
    OpenAPI_list_t *notify_for_supi_listList = NULL;
    cJSON *notify_for_group_list = NULL;
    OpenAPI_list_t *notify_for_group_listList = NULL;
    cJSON *notify_for_snssai_dnn_list = NULL;
    OpenAPI_list_t *notify_for_snssai_dnn_listList = NULL;
    cJSON *reporting_threshold = NULL;
    cJSON *assign_trajectory = NULL;
    OpenAPI_trajectory_t *assign_trajectory_local_nonprim = NULL;
    cJSON *sm_comm_failure_filter = NULL;
    OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter_local_nonprim = NULL;
    cJSON *ue_pos_cap_requested_ind = NULL;
    cJSON *tw_list = NULL;
    OpenAPI_list_t *tw_listList = NULL;
    cJSON *uav_altitude_reporting_config = NULL;
    OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config_local_nonprim = NULL;
    type = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "type");
    if (!type) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [type]");
        goto end;
    }
    if (!cJSON_IsString(type)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [type]");
        goto end;
    }
    typeVariable = OpenAPI_amf_event_type_FromString(type->valuestring);

    immediate_flag = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "immediateFlag");
    if (immediate_flag) {
    if (!cJSON_IsBool(immediate_flag)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [immediate_flag]");
        goto end;
    }
    }

    area_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "areaList");
    if (area_list) {
        cJSON *area_list_local = NULL;
        if (!cJSON_IsArray(area_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [area_list]");
            goto end;
        }

        area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(area_list_local, area_list) {
            if (!cJSON_IsObject(area_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [area_list]");
                goto end;
            }
            OpenAPI_amf_event_area_t *area_listItem = OpenAPI_amf_event_area_parseFromJSON(area_list_local);
            if (!area_listItem) {
                ogs_error("No area_listItem");
                goto end;
            }
            OpenAPI_list_add(area_listList, area_listItem);
        }
    }

    location_filter_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "locationFilterList");
    if (location_filter_list) {
        cJSON *location_filter_list_local = NULL;
        if (!cJSON_IsArray(location_filter_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [location_filter_list]");
            goto end;
        }

        location_filter_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(location_filter_list_local, location_filter_list) {
            OpenAPI_location_filter_e localEnum = OpenAPI_location_filter_NULL;
            if (!cJSON_IsString(location_filter_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [location_filter_list]");
                goto end;
            }
            localEnum = OpenAPI_location_filter_FromString(location_filter_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"location_filter_list\" is not supported. Ignoring it ...",
                         location_filter_list_local->valuestring);
            } else {
                OpenAPI_list_add(location_filter_listList, (void *)localEnum);
            }
        }
        if (location_filter_listList->count == 0) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed: Expected location_filter_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    location_trends_filter_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "locationTrendsFilterList");
    if (location_trends_filter_list) {
        cJSON *location_trends_filter_list_local = NULL;
        if (!cJSON_IsArray(location_trends_filter_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [location_trends_filter_list]");
            goto end;
        }

        location_trends_filter_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(location_trends_filter_list_local, location_trends_filter_list) {
            OpenAPI_location_filter_e localEnum = OpenAPI_location_filter_NULL;
            if (!cJSON_IsString(location_trends_filter_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [location_trends_filter_list]");
                goto end;
            }
            localEnum = OpenAPI_location_filter_FromString(location_trends_filter_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"location_trends_filter_list\" is not supported. Ignoring it ...",
                         location_trends_filter_list_local->valuestring);
            } else {
                OpenAPI_list_add(location_trends_filter_listList, (void *)localEnum);
            }
        }
        if (location_trends_filter_listList->count == 0) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed: Expected location_trends_filter_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    ref_id = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "refId");
    if (ref_id) {
    if (!cJSON_IsNumber(ref_id)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [ref_id]");
        goto end;
    }
    }

    traffic_descriptor_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "trafficDescriptorList");
    if (traffic_descriptor_list) {
        cJSON *traffic_descriptor_list_local = NULL;
        if (!cJSON_IsArray(traffic_descriptor_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [traffic_descriptor_list]");
            goto end;
        }

        traffic_descriptor_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_descriptor_list_local, traffic_descriptor_list) {
            if (!cJSON_IsObject(traffic_descriptor_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [traffic_descriptor_list]");
                goto end;
            }
            OpenAPI_traffic_descriptor_t *traffic_descriptor_listItem = OpenAPI_traffic_descriptor_parseFromJSON(traffic_descriptor_list_local);
            if (!traffic_descriptor_listItem) {
                ogs_error("No traffic_descriptor_listItem");
                goto end;
            }
            OpenAPI_list_add(traffic_descriptor_listList, traffic_descriptor_listItem);
        }
    }

    report_ue_reachable = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "reportUeReachable");
    if (report_ue_reachable) {
    if (!cJSON_IsBool(report_ue_reachable)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [report_ue_reachable]");
        goto end;
    }
    }

    reachability_filter = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "reachabilityFilter");
    if (reachability_filter) {
    if (!cJSON_IsString(reachability_filter)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [reachability_filter]");
        goto end;
    }
    reachability_filterVariable = OpenAPI_reachability_filter_FromString(reachability_filter->valuestring);
    }

    udm_detect_ind = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "udmDetectInd");
    if (udm_detect_ind) {
    if (!cJSON_IsBool(udm_detect_ind)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [udm_detect_ind]");
        goto end;
    }
    }

    max_reports = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "maxReports");
    if (max_reports) {
    if (!cJSON_IsNumber(max_reports)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [max_reports]");
        goto end;
    }
    }

    presence_info_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "presenceInfoList");
    if (presence_info_list) {
        cJSON *presence_info_list_local_map = NULL;
        if (!cJSON_IsObject(presence_info_list) && !cJSON_IsNull(presence_info_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [presence_info_list]");
            goto end;
        }
        if (cJSON_IsObject(presence_info_list)) {
            presence_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(presence_info_list_local_map, presence_info_list) {
                cJSON *localMapObject = presence_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_amf_event_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(presence_info_listList, localMapKeyPair);
            }
        }
    }

    max_response_time = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "maxResponseTime");
    if (max_response_time) {
    if (!cJSON_IsNumber(max_response_time)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [max_response_time]");
        goto end;
    }
    }

    target_area = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "targetArea");
    if (target_area) {
    target_area_local_nonprim = OpenAPI_target_area_parseFromJSON(target_area);
    if (!target_area_local_nonprim) {
        ogs_error("OpenAPI_target_area_parseFromJSON failed [target_area]");
        goto end;
    }
    }

    snssai_filter = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "snssaiFilter");
    if (snssai_filter) {
        cJSON *snssai_filter_local = NULL;
        if (!cJSON_IsArray(snssai_filter)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [snssai_filter]");
            goto end;
        }

        snssai_filterList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_filter_local, snssai_filter) {
            if (!cJSON_IsObject(snssai_filter_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [snssai_filter]");
                goto end;
            }
            OpenAPI_ext_snssai_t *snssai_filterItem = OpenAPI_ext_snssai_parseFromJSON(snssai_filter_local);
            if (!snssai_filterItem) {
                ogs_error("No snssai_filterItem");
                goto end;
            }
            OpenAPI_list_add(snssai_filterList, snssai_filterItem);
        }
    }

    ue_in_area_filter = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "ueInAreaFilter");
    if (ue_in_area_filter) {
    ue_in_area_filter_local_nonprim = OpenAPI_ue_in_area_filter_parseFromJSON(ue_in_area_filter);
    if (!ue_in_area_filter_local_nonprim) {
        ogs_error("OpenAPI_ue_in_area_filter_parseFromJSON failed [ue_in_area_filter]");
        goto end;
    }
    }

    min_interval = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "minInterval");
    if (min_interval) {
    if (!cJSON_IsNumber(min_interval)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [min_interval]");
        goto end;
    }
    }

    next_report = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "nextReport");
    if (next_report) {
    if (!cJSON_IsString(next_report) && !cJSON_IsNull(next_report)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [next_report]");
        goto end;
    }
    }

    idle_status_ind = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "idleStatusInd");
    if (idle_status_ind) {
    if (!cJSON_IsBool(idle_status_ind)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [idle_status_ind]");
        goto end;
    }
    }

    dispersion_area = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "dispersionArea");
    if (dispersion_area) {
    dispersion_area_local_nonprim = OpenAPI_dispersion_area_parseFromJSON(dispersion_area);
    if (!dispersion_area_local_nonprim) {
        ogs_error("OpenAPI_dispersion_area_parseFromJSON failed [dispersion_area]");
        goto end;
    }
    }

    next_periodic_report_time = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "nextPeriodicReportTime");
    if (next_periodic_report_time) {
    if (!cJSON_IsString(next_periodic_report_time) && !cJSON_IsNull(next_periodic_report_time)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [next_periodic_report_time]");
        goto end;
    }
    }

    adjust_ao_ion_ra = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "adjustAoIOnRa");
    if (adjust_ao_ion_ra) {
    if (!cJSON_IsBool(adjust_ao_ion_ra)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [adjust_ao_ion_ra]");
        goto end;
    }
    }

    ran_timing_synchro_status_change = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "ranTimingSynchroStatusChange");
    if (ran_timing_synchro_status_change) {
    if (!cJSON_IsBool(ran_timing_synchro_status_change)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [ran_timing_synchro_status_change]");
        goto end;
    }
    }

    notify_for_supi_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "notifyForSupiList");
    if (notify_for_supi_list) {
        cJSON *notify_for_supi_list_local = NULL;
        if (!cJSON_IsArray(notify_for_supi_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [notify_for_supi_list]");
            goto end;
        }

        notify_for_supi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_for_supi_list_local, notify_for_supi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(notify_for_supi_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [notify_for_supi_list]");
                goto end;
            }
            OpenAPI_list_add(notify_for_supi_listList, ogs_strdup(notify_for_supi_list_local->valuestring));
        }
    }

    notify_for_group_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "notifyForGroupList");
    if (notify_for_group_list) {
        cJSON *notify_for_group_list_local = NULL;
        if (!cJSON_IsArray(notify_for_group_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [notify_for_group_list]");
            goto end;
        }

        notify_for_group_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_for_group_list_local, notify_for_group_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(notify_for_group_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [notify_for_group_list]");
                goto end;
            }
            OpenAPI_list_add(notify_for_group_listList, ogs_strdup(notify_for_group_list_local->valuestring));
        }
    }

    notify_for_snssai_dnn_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "notifyForSnssaiDnnList");
    if (notify_for_snssai_dnn_list) {
        cJSON *notify_for_snssai_dnn_list_local = NULL;
        if (!cJSON_IsArray(notify_for_snssai_dnn_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [notify_for_snssai_dnn_list]");
            goto end;
        }

        notify_for_snssai_dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_for_snssai_dnn_list_local, notify_for_snssai_dnn_list) {
            if (!cJSON_IsObject(notify_for_snssai_dnn_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [notify_for_snssai_dnn_list]");
                goto end;
            }
            OpenAPI_snssai_dnn_item_t *notify_for_snssai_dnn_listItem = OpenAPI_snssai_dnn_item_parseFromJSON(notify_for_snssai_dnn_list_local);
            if (!notify_for_snssai_dnn_listItem) {
                ogs_error("No notify_for_snssai_dnn_listItem");
                goto end;
            }
            OpenAPI_list_add(notify_for_snssai_dnn_listList, notify_for_snssai_dnn_listItem);
        }
    }

    reporting_threshold = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "reportingThreshold");
    if (reporting_threshold) {
    if (!cJSON_IsNumber(reporting_threshold)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [reporting_threshold]");
        goto end;
    }
    }

    assign_trajectory = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "assignTrajectory");
    if (assign_trajectory) {
    assign_trajectory_local_nonprim = OpenAPI_trajectory_parseFromJSON(assign_trajectory);
    if (!assign_trajectory_local_nonprim) {
        ogs_error("OpenAPI_trajectory_parseFromJSON failed [assign_trajectory]");
        goto end;
    }
    }

    sm_comm_failure_filter = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "smCommFailureFilter");
    if (sm_comm_failure_filter) {
    sm_comm_failure_filter_local_nonprim = OpenAPI_sm_comm_failure_filter_parseFromJSON(sm_comm_failure_filter);
    if (!sm_comm_failure_filter_local_nonprim) {
        ogs_error("OpenAPI_sm_comm_failure_filter_parseFromJSON failed [sm_comm_failure_filter]");
        goto end;
    }
    }

    ue_pos_cap_requested_ind = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "uePosCapRequestedInd");
    if (ue_pos_cap_requested_ind) {
    if (!cJSON_IsBool(ue_pos_cap_requested_ind)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [ue_pos_cap_requested_ind]");
        goto end;
    }
    }

    tw_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "twList");
    if (tw_list) {
        cJSON *tw_list_local = NULL;
        if (!cJSON_IsArray(tw_list)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [tw_list]");
            goto end;
        }

        tw_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tw_list_local, tw_list) {
            if (!cJSON_IsObject(tw_list_local)) {
                ogs_error("OpenAPI_amf_event_parseFromJSON() failed [tw_list]");
                goto end;
            }
            OpenAPI_time_window_t *tw_listItem = OpenAPI_time_window_parseFromJSON(tw_list_local);
            if (!tw_listItem) {
                ogs_error("No tw_listItem");
                goto end;
            }
            OpenAPI_list_add(tw_listList, tw_listItem);
        }
    }

    uav_altitude_reporting_config = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "uavAltitudeReportingConfig");
    if (uav_altitude_reporting_config) {
    uav_altitude_reporting_config_local_nonprim = OpenAPI_uav_altitude_reporting_config_parseFromJSON(uav_altitude_reporting_config);
    if (!uav_altitude_reporting_config_local_nonprim) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON failed [uav_altitude_reporting_config]");
        goto end;
    }
    }

    amf_event_local_var = OpenAPI_amf_event_create (
        typeVariable,
        immediate_flag ? true : false,
        immediate_flag ? immediate_flag->valueint : 0,
        area_list ? area_listList : NULL,
        location_filter_list ? location_filter_listList : NULL,
        location_trends_filter_list ? location_trends_filter_listList : NULL,
        ref_id ? true : false,
        ref_id ? ref_id->valuedouble : 0,
        traffic_descriptor_list ? traffic_descriptor_listList : NULL,
        report_ue_reachable ? true : false,
        report_ue_reachable ? report_ue_reachable->valueint : 0,
        reachability_filter ? reachability_filterVariable : 0,
        udm_detect_ind ? true : false,
        udm_detect_ind ? udm_detect_ind->valueint : 0,
        max_reports ? true : false,
        max_reports ? max_reports->valuedouble : 0,
        presence_info_list ? presence_info_listList : NULL,
        max_response_time ? true : false,
        max_response_time ? max_response_time->valuedouble : 0,
        target_area ? target_area_local_nonprim : NULL,
        snssai_filter ? snssai_filterList : NULL,
        ue_in_area_filter ? ue_in_area_filter_local_nonprim : NULL,
        min_interval ? true : false,
        min_interval ? min_interval->valuedouble : 0,
        next_report && !cJSON_IsNull(next_report) ? ogs_strdup(next_report->valuestring) : NULL,
        idle_status_ind ? true : false,
        idle_status_ind ? idle_status_ind->valueint : 0,
        dispersion_area ? dispersion_area_local_nonprim : NULL,
        next_periodic_report_time && !cJSON_IsNull(next_periodic_report_time) ? ogs_strdup(next_periodic_report_time->valuestring) : NULL,
        adjust_ao_ion_ra ? true : false,
        adjust_ao_ion_ra ? adjust_ao_ion_ra->valueint : 0,
        ran_timing_synchro_status_change ? true : false,
        ran_timing_synchro_status_change ? ran_timing_synchro_status_change->valueint : 0,
        notify_for_supi_list ? notify_for_supi_listList : NULL,
        notify_for_group_list ? notify_for_group_listList : NULL,
        notify_for_snssai_dnn_list ? notify_for_snssai_dnn_listList : NULL,
        reporting_threshold ? true : false,
        reporting_threshold ? reporting_threshold->valuedouble : 0,
        assign_trajectory ? assign_trajectory_local_nonprim : NULL,
        sm_comm_failure_filter ? sm_comm_failure_filter_local_nonprim : NULL,
        ue_pos_cap_requested_ind ? true : false,
        ue_pos_cap_requested_ind ? ue_pos_cap_requested_ind->valueint : 0,
        tw_list ? tw_listList : NULL,
        uav_altitude_reporting_config ? uav_altitude_reporting_config_local_nonprim : NULL
    );

    return amf_event_local_var;
end:
    if (area_listList) {
        OpenAPI_list_for_each(area_listList, node) {
            OpenAPI_amf_event_area_free(node->data);
        }
        OpenAPI_list_free(area_listList);
        area_listList = NULL;
    }
    if (location_filter_listList) {
        OpenAPI_list_free(location_filter_listList);
        location_filter_listList = NULL;
    }
    if (location_trends_filter_listList) {
        OpenAPI_list_free(location_trends_filter_listList);
        location_trends_filter_listList = NULL;
    }
    if (traffic_descriptor_listList) {
        OpenAPI_list_for_each(traffic_descriptor_listList, node) {
            OpenAPI_traffic_descriptor_free(node->data);
        }
        OpenAPI_list_free(traffic_descriptor_listList);
        traffic_descriptor_listList = NULL;
    }
    if (presence_info_listList) {
        OpenAPI_list_for_each(presence_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(presence_info_listList);
        presence_info_listList = NULL;
    }
    if (target_area_local_nonprim) {
        OpenAPI_target_area_free(target_area_local_nonprim);
        target_area_local_nonprim = NULL;
    }
    if (snssai_filterList) {
        OpenAPI_list_for_each(snssai_filterList, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_filterList);
        snssai_filterList = NULL;
    }
    if (ue_in_area_filter_local_nonprim) {
        OpenAPI_ue_in_area_filter_free(ue_in_area_filter_local_nonprim);
        ue_in_area_filter_local_nonprim = NULL;
    }
    if (dispersion_area_local_nonprim) {
        OpenAPI_dispersion_area_free(dispersion_area_local_nonprim);
        dispersion_area_local_nonprim = NULL;
    }
    if (notify_for_supi_listList) {
        OpenAPI_list_for_each(notify_for_supi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(notify_for_supi_listList);
        notify_for_supi_listList = NULL;
    }
    if (notify_for_group_listList) {
        OpenAPI_list_for_each(notify_for_group_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(notify_for_group_listList);
        notify_for_group_listList = NULL;
    }
    if (notify_for_snssai_dnn_listList) {
        OpenAPI_list_for_each(notify_for_snssai_dnn_listList, node) {
            OpenAPI_snssai_dnn_item_free(node->data);
        }
        OpenAPI_list_free(notify_for_snssai_dnn_listList);
        notify_for_snssai_dnn_listList = NULL;
    }
    if (assign_trajectory_local_nonprim) {
        OpenAPI_trajectory_free(assign_trajectory_local_nonprim);
        assign_trajectory_local_nonprim = NULL;
    }
    if (sm_comm_failure_filter_local_nonprim) {
        OpenAPI_sm_comm_failure_filter_free(sm_comm_failure_filter_local_nonprim);
        sm_comm_failure_filter_local_nonprim = NULL;
    }
    if (tw_listList) {
        OpenAPI_list_for_each(tw_listList, node) {
            OpenAPI_time_window_free(node->data);
        }
        OpenAPI_list_free(tw_listList);
        tw_listList = NULL;
    }
    if (uav_altitude_reporting_config_local_nonprim) {
        OpenAPI_uav_altitude_reporting_config_free(uav_altitude_reporting_config_local_nonprim);
        uav_altitude_reporting_config_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_amf_event_t *OpenAPI_amf_event_copy(OpenAPI_amf_event_t *dst, OpenAPI_amf_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed");
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

    OpenAPI_amf_event_free(dst);
    dst = OpenAPI_amf_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

