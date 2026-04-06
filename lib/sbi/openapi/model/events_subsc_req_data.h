/*
 * events_subsc_req_data.h
 *
 * Identifies the events the application subscribes to.
 */

#ifndef _OpenAPI_events_subsc_req_data_H_
#define _OpenAPI_events_subsc_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_events_subsc_req_data_s OpenAPI_events_subsc_req_data_t;
#include "af_event_subscription.h"
#include "qos_monitoring_information.h"
#include "requested_qos_monitoring_parameter.h"
#include "required_access_info.h"
#include "rtt_flow_reference.h"
#include "usage_threshold.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_events_subsc_req_data_s {
    OpenAPI_list_t *events;
    char *notif_uri;
    OpenAPI_list_t *req_qos_mon_params;
    struct OpenAPI_qos_monitoring_information_s *qos_mon;
    struct OpenAPI_qos_monitoring_information_s *qos_mon_dat_rate;
    OpenAPI_list_t *pdv_req_mon_params;
    struct OpenAPI_qos_monitoring_information_s *pdv_mon;
    struct OpenAPI_qos_monitoring_information_s *congest_mon;
    struct OpenAPI_qos_monitoring_information_s *rtt_mon;
    struct OpenAPI_qos_monitoring_information_s *avl_bit_rate_mon;
    struct OpenAPI_rtt_flow_reference_s *rtt_flow_ref;
    OpenAPI_list_t *req_anis;
    struct OpenAPI_usage_threshold_s *usg_thres;
    char *notif_corre_id;
    OpenAPI_list_t *af_app_ids;
    bool is_direct_notif_ind;
    int direct_notif_ind;
    bool is_avrg_wndw;
    int avrg_wndw;
};

OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_qos_monitoring_information_t *qos_mon,
    OpenAPI_qos_monitoring_information_t *qos_mon_dat_rate,
    OpenAPI_list_t *pdv_req_mon_params,
    OpenAPI_qos_monitoring_information_t *pdv_mon,
    OpenAPI_qos_monitoring_information_t *congest_mon,
    OpenAPI_qos_monitoring_information_t *rtt_mon,
    OpenAPI_qos_monitoring_information_t *avl_bit_rate_mon,
    OpenAPI_rtt_flow_reference_t *rtt_flow_ref,
    OpenAPI_list_t *req_anis,
    OpenAPI_usage_threshold_t *usg_thres,
    char *notif_corre_id,
    OpenAPI_list_t *af_app_ids,
    bool is_direct_notif_ind,
    int direct_notif_ind,
    bool is_avrg_wndw,
    int avrg_wndw
);
void OpenAPI_events_subsc_req_data_free(OpenAPI_events_subsc_req_data_t *events_subsc_req_data);
OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_parseFromJSON(cJSON *events_subsc_req_dataJSON);
cJSON *OpenAPI_events_subsc_req_data_convertToJSON(OpenAPI_events_subsc_req_data_t *events_subsc_req_data);
OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_copy(OpenAPI_events_subsc_req_data_t *dst, OpenAPI_events_subsc_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_events_subsc_req_data_H_ */

