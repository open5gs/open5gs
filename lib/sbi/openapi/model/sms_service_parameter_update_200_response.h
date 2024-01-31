/*
 * sms_service_parameter_update_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_sms_service_parameter_update_200_response_H_
#define _OpenAPI_sms_service_parameter_update_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "backup_amf_info.h"
#include "guami.h"
#include "patch_result.h"
#include "rat_type.h"
#include "report_item.h"
#include "trace_data.h"
#include "ue_sms_context_data.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_service_parameter_update_200_response_s OpenAPI_sms_service_parameter_update_200_response_t;
typedef struct OpenAPI_sms_service_parameter_update_200_response_s {
    OpenAPI_list_t *report;
    char *supi;
    char *pei;
    char *amf_id;
    OpenAPI_list_t *guamis;
    OpenAPI_access_type_e access_type;
    OpenAPI_access_type_e additional_access_type;
    char *gpsi;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    OpenAPI_list_t *backup_amf_info;
    char *udm_group_id;
    char *routing_indicator;
    bool is_h_nw_pub_key_id;
    int h_nw_pub_key_id;
    OpenAPI_rat_type_e rat_type;
    OpenAPI_rat_type_e additional_rat_type;
    char *supported_features;
} OpenAPI_sms_service_parameter_update_200_response_t;

OpenAPI_sms_service_parameter_update_200_response_t *OpenAPI_sms_service_parameter_update_200_response_create(
    OpenAPI_list_t *report,
    char *supi,
    char *pei,
    char *amf_id,
    OpenAPI_list_t *guamis,
    OpenAPI_access_type_e access_type,
    OpenAPI_access_type_e additional_access_type,
    char *gpsi,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_list_t *backup_amf_info,
    char *udm_group_id,
    char *routing_indicator,
    bool is_h_nw_pub_key_id,
    int h_nw_pub_key_id,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_rat_type_e additional_rat_type,
    char *supported_features
);
void OpenAPI_sms_service_parameter_update_200_response_free(OpenAPI_sms_service_parameter_update_200_response_t *sms_service_parameter_update_200_response);
OpenAPI_sms_service_parameter_update_200_response_t *OpenAPI_sms_service_parameter_update_200_response_parseFromJSON(cJSON *sms_service_parameter_update_200_responseJSON);
cJSON *OpenAPI_sms_service_parameter_update_200_response_convertToJSON(OpenAPI_sms_service_parameter_update_200_response_t *sms_service_parameter_update_200_response);
OpenAPI_sms_service_parameter_update_200_response_t *OpenAPI_sms_service_parameter_update_200_response_copy(OpenAPI_sms_service_parameter_update_200_response_t *dst, OpenAPI_sms_service_parameter_update_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_service_parameter_update_200_response_H_ */

