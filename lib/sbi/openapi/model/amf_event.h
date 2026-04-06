/*
 * amf_event.h
 *
 * Describes an event to be subscribed
 */

#ifndef _OpenAPI_amf_event_H_
#define _OpenAPI_amf_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_amf_event_s OpenAPI_amf_event_t;
#include "amf_event_area.h"
#include "amf_event_type.h"
#include "dispersion_area.h"
#include "ext_snssai.h"
#include "location_filter.h"
#include "presence_info.h"
#include "reachability_filter.h"
#include "sm_comm_failure_filter.h"
#include "snssai_dnn_item.h"
#include "target_area.h"
#include "time_window.h"
#include "traffic_descriptor.h"
#include "trajectory.h"
#include "uav_altitude_reporting_config.h"
#include "ue_in_area_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_amf_event_s {
    OpenAPI_amf_event_type_e type;
    bool is_immediate_flag;
    int immediate_flag;
    OpenAPI_list_t *area_list;
    OpenAPI_list_t *location_filter_list;
    OpenAPI_list_t *location_trends_filter_list;
    bool is_ref_id;
    int ref_id;
    OpenAPI_list_t *traffic_descriptor_list;
    bool is_report_ue_reachable;
    int report_ue_reachable;
    OpenAPI_reachability_filter_e reachability_filter;
    bool is_udm_detect_ind;
    int udm_detect_ind;
    bool is_max_reports;
    int max_reports;
    OpenAPI_list_t* presence_info_list;
    bool is_max_response_time;
    int max_response_time;
    struct OpenAPI_target_area_s *target_area;
    OpenAPI_list_t *snssai_filter;
    struct OpenAPI_ue_in_area_filter_s *ue_in_area_filter;
    bool is_min_interval;
    int min_interval;
    char *next_report;
    bool is_idle_status_ind;
    int idle_status_ind;
    struct OpenAPI_dispersion_area_s *dispersion_area;
    char *next_periodic_report_time;
    bool is_adjust_ao_ion_ra;
    int adjust_ao_ion_ra;
    bool is_ran_timing_synchro_status_change;
    int ran_timing_synchro_status_change;
    OpenAPI_list_t *notify_for_supi_list;
    OpenAPI_list_t *notify_for_group_list;
    OpenAPI_list_t *notify_for_snssai_dnn_list;
    bool is_reporting_threshold;
    int reporting_threshold;
    struct OpenAPI_trajectory_s *assign_trajectory;
    struct OpenAPI_sm_comm_failure_filter_s *sm_comm_failure_filter;
    bool is_ue_pos_cap_requested_ind;
    int ue_pos_cap_requested_ind;
    OpenAPI_list_t *tw_list;
    struct OpenAPI_uav_altitude_reporting_config_s *uav_altitude_reporting_config;
};

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
);
void OpenAPI_amf_event_free(OpenAPI_amf_event_t *amf_event);
OpenAPI_amf_event_t *OpenAPI_amf_event_parseFromJSON(cJSON *amf_eventJSON);
cJSON *OpenAPI_amf_event_convertToJSON(OpenAPI_amf_event_t *amf_event);
OpenAPI_amf_event_t *OpenAPI_amf_event_copy(OpenAPI_amf_event_t *dst, OpenAPI_amf_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_H_ */

