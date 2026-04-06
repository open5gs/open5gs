/*
 * session_management_subscription_data.h
 *
 * Contains Session Management Subscription Data
 */

#ifndef _OpenAPI_session_management_subscription_data_H_
#define _OpenAPI_session_management_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_session_management_subscription_data_s OpenAPI_session_management_subscription_data_t;
#include "app_specific_expected_ue_behaviour_data.h"
#include "dnn_configuration.h"
#include "expected_ue_behaviour_data.h"
#include "nsac_admission_mode.h"
#include "odb_exempted_dnn_info.h"
#include "odb_packet_services.h"
#include "snssai.h"
#include "suggested_packet_num_dl.h"
#include "trace_data.h"
#include "ue_level_measurements_configuration.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_session_management_subscription_data_s {
    struct OpenAPI_snssai_s *single_nssai;
    OpenAPI_list_t* dnn_configurations;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t* shared_vn_group_data_ids;
    char *shared_dnn_configurations_id;
    OpenAPI_odb_packet_services_e odb_packet_services;
    OpenAPI_list_t* odb_exempted_dnn_data;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    char *shared_trace_data_id;
    OpenAPI_list_t* expected_ue_behaviours_list;
    OpenAPI_list_t* expected_ue_behaviour_data;
    OpenAPI_list_t* app_specific_expected_ue_behaviour_data;
    OpenAPI_list_t* suggested_packet_num_dl_list;
    char *_3gpp_charging_characteristics;
    char *chf_group_id;
    OpenAPI_nsac_admission_mode_e nsac_mode;
    bool is_sess_inact_timer;
    int sess_inact_timer;
    char *supported_features;
    OpenAPI_list_t *additional_shared_dnn_configurations_ids;
    struct OpenAPI_ue_level_measurements_configuration_s *ue_level_meas_config;
};

OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_create(
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t* dnn_configurations,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* shared_vn_group_data_ids,
    char *shared_dnn_configurations_id,
    OpenAPI_odb_packet_services_e odb_packet_services,
    OpenAPI_list_t* odb_exempted_dnn_data,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *shared_trace_data_id,
    OpenAPI_list_t* expected_ue_behaviours_list,
    OpenAPI_list_t* expected_ue_behaviour_data,
    OpenAPI_list_t* app_specific_expected_ue_behaviour_data,
    OpenAPI_list_t* suggested_packet_num_dl_list,
    char *_3gpp_charging_characteristics,
    char *chf_group_id,
    OpenAPI_nsac_admission_mode_e nsac_mode,
    bool is_sess_inact_timer,
    int sess_inact_timer,
    char *supported_features,
    OpenAPI_list_t *additional_shared_dnn_configurations_ids,
    OpenAPI_ue_level_measurements_configuration_t *ue_level_meas_config
);
void OpenAPI_session_management_subscription_data_free(OpenAPI_session_management_subscription_data_t *session_management_subscription_data);
OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_parseFromJSON(cJSON *session_management_subscription_dataJSON);
cJSON *OpenAPI_session_management_subscription_data_convertToJSON(OpenAPI_session_management_subscription_data_t *session_management_subscription_data);
OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_copy(OpenAPI_session_management_subscription_data_t *dst, OpenAPI_session_management_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_management_subscription_data_H_ */

