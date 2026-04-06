/*
 * events_subsc_req_data_rm.h
 *
 * This data type is defined in the same way as the EventsSubscReqData data type, but with the OpenAPI nullable property set to true. 
 */

#ifndef _OpenAPI_events_subsc_req_data_rm_H_
#define _OpenAPI_events_subsc_req_data_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_events_subsc_req_data_rm_s OpenAPI_events_subsc_req_data_rm_t;
#include "af_event_subscription.h"
#include "qos_monitoring_information_rm.h"
#include "requested_qos_monitoring_parameter.h"
#include "required_access_info.h"
#include "rtt_flow_reference_rm.h"
#include "usage_threshold_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_events_subsc_req_data_rm_s {
    OpenAPI_list_t *events;
    char *notif_uri;
    bool is_req_qos_mon_params_null;
    OpenAPI_list_t *req_qos_mon_params;
    bool is_qos_mon_null;
    struct OpenAPI_qos_monitoring_information_rm_s *qos_mon;
    bool is_qos_mon_dat_rate_null;
    struct OpenAPI_qos_monitoring_information_rm_s *qos_mon_dat_rate;
    bool is_pdv_req_mon_params_null;
    OpenAPI_list_t *pdv_req_mon_params;
    bool is_pdv_mon_null;
    struct OpenAPI_qos_monitoring_information_rm_s *pdv_mon;
    bool is_congest_mon_null;
    struct OpenAPI_qos_monitoring_information_rm_s *congest_mon;
    bool is_rtt_mon_null;
    struct OpenAPI_qos_monitoring_information_rm_s *rtt_mon;
    bool is_rtt_flow_ref_null;
    struct OpenAPI_rtt_flow_reference_rm_s *rtt_flow_ref;
    bool is_avl_bit_rate_mon_null;
    struct OpenAPI_qos_monitoring_information_rm_s *avl_bit_rate_mon;
    OpenAPI_list_t *req_anis;
    bool is_usg_thres_null;
    struct OpenAPI_usage_threshold_rm_s *usg_thres;
    char *notif_corre_id;
    bool is_direct_notif_ind_null;
    bool is_direct_notif_ind;
    int direct_notif_ind;
    bool is_avrg_wndw_null;
    bool is_avrg_wndw;
    int avrg_wndw;
};

OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    bool is_req_qos_mon_params_null,
    OpenAPI_list_t *req_qos_mon_params,
    bool is_qos_mon_null,
    OpenAPI_qos_monitoring_information_rm_t *qos_mon,
    bool is_qos_mon_dat_rate_null,
    OpenAPI_qos_monitoring_information_rm_t *qos_mon_dat_rate,
    bool is_pdv_req_mon_params_null,
    OpenAPI_list_t *pdv_req_mon_params,
    bool is_pdv_mon_null,
    OpenAPI_qos_monitoring_information_rm_t *pdv_mon,
    bool is_congest_mon_null,
    OpenAPI_qos_monitoring_information_rm_t *congest_mon,
    bool is_rtt_mon_null,
    OpenAPI_qos_monitoring_information_rm_t *rtt_mon,
    bool is_rtt_flow_ref_null,
    OpenAPI_rtt_flow_reference_rm_t *rtt_flow_ref,
    bool is_avl_bit_rate_mon_null,
    OpenAPI_qos_monitoring_information_rm_t *avl_bit_rate_mon,
    OpenAPI_list_t *req_anis,
    bool is_usg_thres_null,
    OpenAPI_usage_threshold_rm_t *usg_thres,
    char *notif_corre_id,
    bool is_direct_notif_ind_null,
    bool is_direct_notif_ind,
    int direct_notif_ind,
    bool is_avrg_wndw_null,
    bool is_avrg_wndw,
    int avrg_wndw
);
void OpenAPI_events_subsc_req_data_rm_free(OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm);
OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_parseFromJSON(cJSON *events_subsc_req_data_rmJSON);
cJSON *OpenAPI_events_subsc_req_data_rm_convertToJSON(OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm);
OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_copy(OpenAPI_events_subsc_req_data_rm_t *dst, OpenAPI_events_subsc_req_data_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_events_subsc_req_data_rm_H_ */

