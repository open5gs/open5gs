/*
 * event_notification.h
 *
 * Represents a notification on events that occurred.
 */

#ifndef _OpenAPI_event_notification_H_
#define _OpenAPI_event_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "abnormal_behaviour.h"
#include "analytics_metadata_info.h"
#include "dispersion_info.h"
#include "dn_perf_info.h"
#include "network_perf_info.h"
#include "nf_load_level_information.h"
#include "nsi_load_level_info.h"
#include "nwdaf_event.h"
#include "nwdaf_failure_code.h"
#include "qos_sustainability_info.h"
#include "redundant_transmission_exp_info.h"
#include "service_experience_info.h"
#include "slice_load_level_information.h"
#include "smcce_info.h"
#include "ue_communication.h"
#include "ue_mobility.h"
#include "user_data_congestion_info.h"
#include "wlan_performance_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_notification_s OpenAPI_event_notification_t;
typedef struct OpenAPI_event_notification_s {
    struct OpenAPI_nwdaf_event_s *event;
    char *start;
    char *expiry;
    char *time_stamp_gen;
    struct OpenAPI_nwdaf_failure_code_s *fail_notify_code;
    bool is_rv_wait_time;
    int rv_wait_time;
    struct OpenAPI_analytics_metadata_info_s *ana_meta_info;
    OpenAPI_list_t *nf_load_level_infos;
    OpenAPI_list_t *nsi_load_level_infos;
    struct OpenAPI_slice_load_level_information_s *slice_load_level_info;
    OpenAPI_list_t *svc_exps;
    OpenAPI_list_t *qos_sustain_infos;
    OpenAPI_list_t *ue_comms;
    OpenAPI_list_t *ue_mobs;
    OpenAPI_list_t *user_data_cong_infos;
    OpenAPI_list_t *abnor_behavrs;
    OpenAPI_list_t *nw_perfs;
    OpenAPI_list_t *dn_perf_infos;
    OpenAPI_list_t *disper_infos;
    OpenAPI_list_t *red_trans_infos;
    OpenAPI_list_t *wlan_infos;
    OpenAPI_list_t *smcc_exps;
} OpenAPI_event_notification_t;

OpenAPI_event_notification_t *OpenAPI_event_notification_create(
    OpenAPI_nwdaf_event_t *event,
    char *start,
    char *expiry,
    char *time_stamp_gen,
    OpenAPI_nwdaf_failure_code_t *fail_notify_code,
    bool is_rv_wait_time,
    int rv_wait_time,
    OpenAPI_analytics_metadata_info_t *ana_meta_info,
    OpenAPI_list_t *nf_load_level_infos,
    OpenAPI_list_t *nsi_load_level_infos,
    OpenAPI_slice_load_level_information_t *slice_load_level_info,
    OpenAPI_list_t *svc_exps,
    OpenAPI_list_t *qos_sustain_infos,
    OpenAPI_list_t *ue_comms,
    OpenAPI_list_t *ue_mobs,
    OpenAPI_list_t *user_data_cong_infos,
    OpenAPI_list_t *abnor_behavrs,
    OpenAPI_list_t *nw_perfs,
    OpenAPI_list_t *dn_perf_infos,
    OpenAPI_list_t *disper_infos,
    OpenAPI_list_t *red_trans_infos,
    OpenAPI_list_t *wlan_infos,
    OpenAPI_list_t *smcc_exps
);
void OpenAPI_event_notification_free(OpenAPI_event_notification_t *event_notification);
OpenAPI_event_notification_t *OpenAPI_event_notification_parseFromJSON(cJSON *event_notificationJSON);
cJSON *OpenAPI_event_notification_convertToJSON(OpenAPI_event_notification_t *event_notification);
OpenAPI_event_notification_t *OpenAPI_event_notification_copy(OpenAPI_event_notification_t *dst, OpenAPI_event_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_notification_H_ */

