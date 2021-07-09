/*
 * session_management_subscription_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_session_management_subscription_data_1_H_
#define _OpenAPI_session_management_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnn_configuration_1.h"
#include "expected_ue_behaviour_data_1.h"
#include "odb_packet_services.h"
#include "snssai.h"
#include "suggested_packet_num_dl_1.h"
#include "trace_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_session_management_subscription_data_1_s OpenAPI_session_management_subscription_data_1_t;
typedef struct OpenAPI_session_management_subscription_data_1_s {
    struct OpenAPI_snssai_s *single_nssai;
    OpenAPI_list_t* dnn_configurations;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t* shared_vn_group_data_ids;
    char *shared_dnn_configurations_id;
    OpenAPI_odb_packet_services_e odb_packet_services;
    struct OpenAPI_trace_data_s *trace_data;
    char *shared_trace_data_id;
    OpenAPI_list_t* expected_ue_behaviours_list;
    OpenAPI_list_t* suggested_packet_num_dl_list;
    char *_3gpp_charging_characteristics;
} OpenAPI_session_management_subscription_data_1_t;

OpenAPI_session_management_subscription_data_1_t *OpenAPI_session_management_subscription_data_1_create(
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t* dnn_configurations,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* shared_vn_group_data_ids,
    char *shared_dnn_configurations_id,
    OpenAPI_odb_packet_services_e odb_packet_services,
    OpenAPI_trace_data_t *trace_data,
    char *shared_trace_data_id,
    OpenAPI_list_t* expected_ue_behaviours_list,
    OpenAPI_list_t* suggested_packet_num_dl_list,
    char *_3gpp_charging_characteristics
);
void OpenAPI_session_management_subscription_data_1_free(OpenAPI_session_management_subscription_data_1_t *session_management_subscription_data_1);
OpenAPI_session_management_subscription_data_1_t *OpenAPI_session_management_subscription_data_1_parseFromJSON(cJSON *session_management_subscription_data_1JSON);
cJSON *OpenAPI_session_management_subscription_data_1_convertToJSON(OpenAPI_session_management_subscription_data_1_t *session_management_subscription_data_1);
OpenAPI_session_management_subscription_data_1_t *OpenAPI_session_management_subscription_data_1_copy(OpenAPI_session_management_subscription_data_1_t *dst, OpenAPI_session_management_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_management_subscription_data_1_H_ */

