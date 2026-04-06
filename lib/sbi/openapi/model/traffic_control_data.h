/*
 * traffic_control_data.h
 *
 * Contains parameters determining how flows associated with a PCC Rule are treated (e.g.  blocked, redirected, etc). 
 */

#ifndef _OpenAPI_traffic_control_data_H_
#define _OpenAPI_traffic_control_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_control_data_s OpenAPI_traffic_control_data_t;
#include "af_header_handling_control_info.h"
#include "eas_ip_replacement_info.h"
#include "flow_status.h"
#include "npcf_multicast_access_control.h"
#include "on_path_n6_sig_info.h"
#include "redirect_information.h"
#include "route_to_location.h"
#include "sim_conn_fail_event.h"
#include "steering_functionality.h"
#include "steering_mode.h"
#include "traff_route_req_outcome_event.h"
#include "traffic_correlation_info.h"
#include "transport_mode.h"
#include "up_path_chg_event.h"
#include "uplink_downlink_support.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_control_data_s {
    char *tc_id;
    OpenAPI_uplink_downlink_support_e l4s_ind;
    OpenAPI_flow_status_e flow_status;
    struct OpenAPI_redirect_information_s *redirect_info;
    OpenAPI_list_t *add_redirect_info;
    bool is_mute_notif;
    int mute_notif;
    bool is_traffic_steering_pol_id_dl_null;
    char *traffic_steering_pol_id_dl;
    bool is_traffic_steering_pol_id_ul_null;
    char *traffic_steering_pol_id_ul;
    bool is_metadata_null;
    char *metadata;
    bool is_route_to_locs_null;
    OpenAPI_list_t *route_to_locs;
    bool is_max_allowed_up_lat_null;
    bool is_max_allowed_up_lat;
    int max_allowed_up_lat;
    bool is_eas_ip_replace_infos_null;
    OpenAPI_list_t *eas_ip_replace_infos;
    bool is_traff_corre_ind;
    int traff_corre_ind;
    bool is_tfc_corre_info_null;
    struct OpenAPI_traffic_correlation_info_s *tfc_corre_info;
    bool is_sim_conn_ind;
    int sim_conn_ind;
    bool is_sim_conn_term;
    int sim_conn_term;
    bool is_n6_delay_ind;
    int n6_delay_ind;
    bool is_up_path_chg_event_null;
    struct OpenAPI_up_path_chg_event_s *up_path_chg_event;
    bool is_outcome_event_null;
    struct OpenAPI_traff_route_req_outcome_event_s *outcome_event;
    bool is_sim_conn_fail_event_null;
    struct OpenAPI_sim_conn_fail_event_s *sim_conn_fail_event;
    OpenAPI_steering_functionality_e steer_fun;
    OpenAPI_transport_mode_e trans_mode;
    struct OpenAPI_steering_mode_s *steer_mode_dl;
    struct OpenAPI_steering_mode_s *steer_mode_ul;
    OpenAPI_npcf_multicast_access_control_e mul_acc_ctrl;
    bool is_cand_dnai_ind;
    int cand_dnai_ind;
    bool is_dat_end_mark_ind;
    int dat_end_mark_ind;
    bool is_dat_burst_size_ind;
    int dat_burst_size_ind;
    bool is_timeto_next_burst_ind;
    int timeto_next_burst_ind;
    bool is_payload_hdr_req_null;
    struct OpenAPI_af_header_handling_control_info_s *payload_hdr_req;
    bool is_on_path_n6_sig_info_null;
    struct OpenAPI_on_path_n6_sig_info_s *on_path_n6_sig_info;
};

OpenAPI_traffic_control_data_t *OpenAPI_traffic_control_data_create(
    char *tc_id,
    OpenAPI_uplink_downlink_support_e l4s_ind,
    OpenAPI_flow_status_e flow_status,
    OpenAPI_redirect_information_t *redirect_info,
    OpenAPI_list_t *add_redirect_info,
    bool is_mute_notif,
    int mute_notif,
    bool is_traffic_steering_pol_id_dl_null,
    char *traffic_steering_pol_id_dl,
    bool is_traffic_steering_pol_id_ul_null,
    char *traffic_steering_pol_id_ul,
    bool is_metadata_null,
    char *metadata,
    bool is_route_to_locs_null,
    OpenAPI_list_t *route_to_locs,
    bool is_max_allowed_up_lat_null,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat,
    bool is_eas_ip_replace_infos_null,
    OpenAPI_list_t *eas_ip_replace_infos,
    bool is_traff_corre_ind,
    int traff_corre_ind,
    bool is_tfc_corre_info_null,
    OpenAPI_traffic_correlation_info_t *tfc_corre_info,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term,
    int sim_conn_term,
    bool is_n6_delay_ind,
    int n6_delay_ind,
    bool is_up_path_chg_event_null,
    OpenAPI_up_path_chg_event_t *up_path_chg_event,
    bool is_outcome_event_null,
    OpenAPI_traff_route_req_outcome_event_t *outcome_event,
    bool is_sim_conn_fail_event_null,
    OpenAPI_sim_conn_fail_event_t *sim_conn_fail_event,
    OpenAPI_steering_functionality_e steer_fun,
    OpenAPI_transport_mode_e trans_mode,
    OpenAPI_steering_mode_t *steer_mode_dl,
    OpenAPI_steering_mode_t *steer_mode_ul,
    OpenAPI_npcf_multicast_access_control_e mul_acc_ctrl,
    bool is_cand_dnai_ind,
    int cand_dnai_ind,
    bool is_dat_end_mark_ind,
    int dat_end_mark_ind,
    bool is_dat_burst_size_ind,
    int dat_burst_size_ind,
    bool is_timeto_next_burst_ind,
    int timeto_next_burst_ind,
    bool is_payload_hdr_req_null,
    OpenAPI_af_header_handling_control_info_t *payload_hdr_req,
    bool is_on_path_n6_sig_info_null,
    OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info
);
void OpenAPI_traffic_control_data_free(OpenAPI_traffic_control_data_t *traffic_control_data);
OpenAPI_traffic_control_data_t *OpenAPI_traffic_control_data_parseFromJSON(cJSON *traffic_control_dataJSON);
cJSON *OpenAPI_traffic_control_data_convertToJSON(OpenAPI_traffic_control_data_t *traffic_control_data);
OpenAPI_traffic_control_data_t *OpenAPI_traffic_control_data_copy(OpenAPI_traffic_control_data_t *dst, OpenAPI_traffic_control_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_control_data_H_ */

