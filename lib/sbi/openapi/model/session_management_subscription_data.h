/*
 * session_management_subscription_data.h
 *
 *
 */

#ifndef _OpenAPI_session_management_subscription_data_H_
#define _OpenAPI_session_management_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnn_configuration.h"
#include "expected_ue_behaviour_data.h"
#include "odb_packet_services.h"
#include "snssai.h"
#include "suggested_packet_num_dl.h"
#include "trace_data.h"
#include "vn_group_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_session_management_subscription_data_s OpenAPI_session_management_subscription_data_t;
typedef struct OpenAPI_session_management_subscription_data_s {
    struct OpenAPI_snssai_s *single_nssai;
    OpenAPI_list_t* dnn_configurations;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t* vn_group_info;
    OpenAPI_list_t* shared_vn_group_data_ids;
    char *shared_dnn_configurations_id;
    struct OpenAPI_odb_packet_services_s *odb_packet_services;
    struct OpenAPI_trace_data_s *trace_data;
    char *shared_trace_data_id;
    OpenAPI_list_t* expected_ue_behaviours_list;
    OpenAPI_list_t* suggested_packet_num_dl_list;
    char *_3gpp_charging_characteristics;
} OpenAPI_session_management_subscription_data_t;

OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_create(
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t* dnn_configurations,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* vn_group_info,
    OpenAPI_list_t* shared_vn_group_data_ids,
    char *shared_dnn_configurations_id,
    OpenAPI_odb_packet_services_t *odb_packet_services,
    OpenAPI_trace_data_t *trace_data,
    char *shared_trace_data_id,
    OpenAPI_list_t* expected_ue_behaviours_list,
    OpenAPI_list_t* suggested_packet_num_dl_list,
    char *_3gpp_charging_characteristics
    );
void OpenAPI_session_management_subscription_data_free(OpenAPI_session_management_subscription_data_t *session_management_subscription_data);
OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_parseFromJSON(cJSON *session_management_subscription_dataJSON);
cJSON *OpenAPI_session_management_subscription_data_convertToJSON(OpenAPI_session_management_subscription_data_t *session_management_subscription_data);
OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_copy(OpenAPI_session_management_subscription_data_t *dst, OpenAPI_session_management_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_management_subscription_data_H_ */

