/*
 * event_subscription.h
 *
 * Represents a subscription to a single event.
 */

#ifndef _OpenAPI_event_subscription_H_
#define _OpenAPI_event_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "addr_fqdn.h"
#include "analytics_subset.h"
#include "bw_requirement.h"
#include "dispersion_requirement.h"
#include "dn_performance_req.h"
#include "event_reporting_requirement.h"
#include "exception.h"
#include "expected_analytics_type.h"
#include "expected_ue_behaviour_data.h"
#include "matching_direction.h"
#include "network_area_info.h"
#include "network_perf_requirement.h"
#include "nf_type.h"
#include "notification_method.h"
#include "nsi_id_info.h"
#include "nwdaf_event.h"
#include "qos_requirement.h"
#include "rat_freq_information.h"
#include "redundant_transmission_exp_req.h"
#include "retainability_threshold.h"
#include "snssai.h"
#include "target_ue_information.h"
#include "threshold_level.h"
#include "upf_information.h"
#include "wlan_performance_req.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_subscription_s OpenAPI_event_subscription_t;
typedef struct OpenAPI_event_subscription_s {
    bool is_any_slice;
    int any_slice;
    OpenAPI_list_t *app_ids;
    OpenAPI_list_t *dnns;
    OpenAPI_list_t *dnais;
    struct OpenAPI_nwdaf_event_s *event;
    struct OpenAPI_event_reporting_requirement_s *extra_report_req;
    OpenAPI_list_t *ladn_dnns;
    bool is_load_level_threshold;
    int load_level_threshold;
    struct OpenAPI_notification_method_s *notification_method;
    struct OpenAPI_matching_direction_s *matching_dir;
    OpenAPI_list_t *nf_load_lvl_thds;
    OpenAPI_list_t *nf_instance_ids;
    OpenAPI_list_t *nf_set_ids;
    OpenAPI_list_t *nf_types;
    struct OpenAPI_network_area_info_s *network_area;
    OpenAPI_list_t *visited_areas;
    bool is_max_top_app_ul_nbr;
    int max_top_app_ul_nbr;
    bool is_max_top_app_dl_nbr;
    int max_top_app_dl_nbr;
    OpenAPI_list_t *nsi_id_infos;
    OpenAPI_list_t *nsi_level_thrds;
    struct OpenAPI_qos_requirement_s *qos_requ;
    OpenAPI_list_t *qos_flow_ret_thds;
    OpenAPI_list_t *ran_ue_throu_thds;
    bool is_repetition_period;
    int repetition_period;
    OpenAPI_list_t *snssaia;
    struct OpenAPI_target_ue_information_s *tgt_ue;
    OpenAPI_list_t *cong_thresholds;
    OpenAPI_list_t *nw_perf_requs;
    OpenAPI_list_t *bw_requs;
    OpenAPI_list_t *excep_requs;
    struct OpenAPI_expected_analytics_type_s *expt_ana_type;
    struct OpenAPI_expected_ue_behaviour_data_s *expt_ue_behav;
    OpenAPI_list_t *rat_freqs;
    OpenAPI_list_t *list_of_ana_subsets;
    OpenAPI_list_t *disper_reqs;
    OpenAPI_list_t *red_trans_reqs;
    OpenAPI_list_t *wlan_reqs;
    struct OpenAPI_upf_information_s *upf_info;
    OpenAPI_list_t *app_server_addrs;
    OpenAPI_list_t *dn_perf_reqs;
} OpenAPI_event_subscription_t;

OpenAPI_event_subscription_t *OpenAPI_event_subscription_create(
    bool is_any_slice,
    int any_slice,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *dnais,
    OpenAPI_nwdaf_event_t *event,
    OpenAPI_event_reporting_requirement_t *extra_report_req,
    OpenAPI_list_t *ladn_dnns,
    bool is_load_level_threshold,
    int load_level_threshold,
    OpenAPI_notification_method_t *notification_method,
    OpenAPI_matching_direction_t *matching_dir,
    OpenAPI_list_t *nf_load_lvl_thds,
    OpenAPI_list_t *nf_instance_ids,
    OpenAPI_list_t *nf_set_ids,
    OpenAPI_list_t *nf_types,
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_list_t *visited_areas,
    bool is_max_top_app_ul_nbr,
    int max_top_app_ul_nbr,
    bool is_max_top_app_dl_nbr,
    int max_top_app_dl_nbr,
    OpenAPI_list_t *nsi_id_infos,
    OpenAPI_list_t *nsi_level_thrds,
    OpenAPI_qos_requirement_t *qos_requ,
    OpenAPI_list_t *qos_flow_ret_thds,
    OpenAPI_list_t *ran_ue_throu_thds,
    bool is_repetition_period,
    int repetition_period,
    OpenAPI_list_t *snssaia,
    OpenAPI_target_ue_information_t *tgt_ue,
    OpenAPI_list_t *cong_thresholds,
    OpenAPI_list_t *nw_perf_requs,
    OpenAPI_list_t *bw_requs,
    OpenAPI_list_t *excep_requs,
    OpenAPI_expected_analytics_type_t *expt_ana_type,
    OpenAPI_expected_ue_behaviour_data_t *expt_ue_behav,
    OpenAPI_list_t *rat_freqs,
    OpenAPI_list_t *list_of_ana_subsets,
    OpenAPI_list_t *disper_reqs,
    OpenAPI_list_t *red_trans_reqs,
    OpenAPI_list_t *wlan_reqs,
    OpenAPI_upf_information_t *upf_info,
    OpenAPI_list_t *app_server_addrs,
    OpenAPI_list_t *dn_perf_reqs
);
void OpenAPI_event_subscription_free(OpenAPI_event_subscription_t *event_subscription);
OpenAPI_event_subscription_t *OpenAPI_event_subscription_parseFromJSON(cJSON *event_subscriptionJSON);
cJSON *OpenAPI_event_subscription_convertToJSON(OpenAPI_event_subscription_t *event_subscription);
OpenAPI_event_subscription_t *OpenAPI_event_subscription_copy(OpenAPI_event_subscription_t *dst, OpenAPI_event_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_subscription_H_ */

