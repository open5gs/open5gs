/*
 * ue_sms_context_data.h
 *
 * Represents the information used for activating the SMS service for a service user, or updating the parameters of the SMS service.
 */

#ifndef _OpenAPI_ue_sms_context_data_H_
#define _OpenAPI_ue_sms_context_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "backup_amf_info.h"
#include "guami.h"
#include "rat_type.h"
#include "trace_data.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_sms_context_data_s OpenAPI_ue_sms_context_data_t;
typedef struct OpenAPI_ue_sms_context_data_s {
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
} OpenAPI_ue_sms_context_data_t;

OpenAPI_ue_sms_context_data_t *OpenAPI_ue_sms_context_data_create(
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
void OpenAPI_ue_sms_context_data_free(OpenAPI_ue_sms_context_data_t *ue_sms_context_data);
OpenAPI_ue_sms_context_data_t *OpenAPI_ue_sms_context_data_parseFromJSON(cJSON *ue_sms_context_dataJSON);
cJSON *OpenAPI_ue_sms_context_data_convertToJSON(OpenAPI_ue_sms_context_data_t *ue_sms_context_data);
OpenAPI_ue_sms_context_data_t *OpenAPI_ue_sms_context_data_copy(OpenAPI_ue_sms_context_data_t *dst, OpenAPI_ue_sms_context_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_sms_context_data_H_ */

