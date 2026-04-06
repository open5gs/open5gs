/*
 * qos_policy_assist_set.h
 *
 * The QoS and policy assistance parameter set.
 */

#ifndef _OpenAPI_qos_policy_assist_set_H_
#define _OpenAPI_qos_policy_assist_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_policy_assist_set_s OpenAPI_qos_policy_assist_set_t;
#include "ip_eth_flow_description.h"
#include "network_area_info.h"
#include "qos_para.h"
#include "rat_type.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_policy_assist_set_s {
    struct OpenAPI_qos_para_s *qos_param_set;
    char *dnn;
    char *app_id;
    OpenAPI_list_t *f_descs;
    bool is_app_duration;
    int app_duration;
    bool is_predicted_avg_qoe;
    float predicted_avg_qoe;
    bool is_predicted_max_qoe;
    float predicted_max_qoe;
    bool is_predicted_min_qoe;
    float predicted_min_qoe;
    bool is_pred_qoe_variance;
    float pred_qoe_variance;
    struct OpenAPI_time_window_s *qos_pol_time_win;
    OpenAPI_list_t *freqs;
    OpenAPI_list_t *rat_types;
    struct OpenAPI_time_window_s *validity_period;
    struct OpenAPI_network_area_info_s *spatial_validity;
    bool is_max_qo_s_flow_usg_dur;
    int max_qo_s_flow_usg_dur;
    bool is_min_qo_s_flow_usg_dur;
    int min_qo_s_flow_usg_dur;
    bool is_avg_qo_s_flow_usg_dur;
    int avg_qo_s_flow_usg_dur;
    bool is_qos_flow_usg_number;
    int qos_flow_usg_number;
};

OpenAPI_qos_policy_assist_set_t *OpenAPI_qos_policy_assist_set_create(
    OpenAPI_qos_para_t *qos_param_set,
    char *dnn,
    char *app_id,
    OpenAPI_list_t *f_descs,
    bool is_app_duration,
    int app_duration,
    bool is_predicted_avg_qoe,
    float predicted_avg_qoe,
    bool is_predicted_max_qoe,
    float predicted_max_qoe,
    bool is_predicted_min_qoe,
    float predicted_min_qoe,
    bool is_pred_qoe_variance,
    float pred_qoe_variance,
    OpenAPI_time_window_t *qos_pol_time_win,
    OpenAPI_list_t *freqs,
    OpenAPI_list_t *rat_types,
    OpenAPI_time_window_t *validity_period,
    OpenAPI_network_area_info_t *spatial_validity,
    bool is_max_qo_s_flow_usg_dur,
    int max_qo_s_flow_usg_dur,
    bool is_min_qo_s_flow_usg_dur,
    int min_qo_s_flow_usg_dur,
    bool is_avg_qo_s_flow_usg_dur,
    int avg_qo_s_flow_usg_dur,
    bool is_qos_flow_usg_number,
    int qos_flow_usg_number
);
void OpenAPI_qos_policy_assist_set_free(OpenAPI_qos_policy_assist_set_t *qos_policy_assist_set);
OpenAPI_qos_policy_assist_set_t *OpenAPI_qos_policy_assist_set_parseFromJSON(cJSON *qos_policy_assist_setJSON);
cJSON *OpenAPI_qos_policy_assist_set_convertToJSON(OpenAPI_qos_policy_assist_set_t *qos_policy_assist_set);
OpenAPI_qos_policy_assist_set_t *OpenAPI_qos_policy_assist_set_copy(OpenAPI_qos_policy_assist_set_t *dst, OpenAPI_qos_policy_assist_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_policy_assist_set_H_ */

