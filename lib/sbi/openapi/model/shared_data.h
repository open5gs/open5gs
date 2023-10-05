/*
 * shared_data.h
 *
 * 
 */

#ifndef _OpenAPI_shared_data_H_
#define _OpenAPI_shared_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_and_mobility_subscription_data.h"
#include "dnn_configuration.h"
#include "ecs_addr_config_info.h"
#include "session_management_subscription_data.h"
#include "shared_data_treatment_instruction.h"
#include "sms_management_subscription_data.h"
#include "sms_subscription_data.h"
#include "snssai_info.h"
#include "trace_data.h"
#include "vn_group_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_shared_data_s OpenAPI_shared_data_t;
typedef struct OpenAPI_shared_data_s {
    char *shared_data_id;
    struct OpenAPI_access_and_mobility_subscription_data_s *shared_am_data;
    struct OpenAPI_sms_subscription_data_s *shared_sms_subs_data;
    struct OpenAPI_sms_management_subscription_data_s *shared_sms_mng_subs_data;
    OpenAPI_list_t* shared_dnn_configurations;
    bool is_shared_trace_data_null;
    struct OpenAPI_trace_data_s *shared_trace_data;
    OpenAPI_list_t* shared_snssai_infos;
    OpenAPI_list_t* shared_vn_group_datas;
    OpenAPI_list_t* treatment_instructions;
    struct OpenAPI_session_management_subscription_data_s *shared_sm_subs_data;
    bool is_shared_ecs_addr_config_info_null;
    struct OpenAPI_ecs_addr_config_info_s *shared_ecs_addr_config_info;
} OpenAPI_shared_data_t;

OpenAPI_shared_data_t *OpenAPI_shared_data_create(
    char *shared_data_id,
    OpenAPI_access_and_mobility_subscription_data_t *shared_am_data,
    OpenAPI_sms_subscription_data_t *shared_sms_subs_data,
    OpenAPI_sms_management_subscription_data_t *shared_sms_mng_subs_data,
    OpenAPI_list_t* shared_dnn_configurations,
    bool is_shared_trace_data_null,
    OpenAPI_trace_data_t *shared_trace_data,
    OpenAPI_list_t* shared_snssai_infos,
    OpenAPI_list_t* shared_vn_group_datas,
    OpenAPI_list_t* treatment_instructions,
    OpenAPI_session_management_subscription_data_t *shared_sm_subs_data,
    bool is_shared_ecs_addr_config_info_null,
    OpenAPI_ecs_addr_config_info_t *shared_ecs_addr_config_info
);
void OpenAPI_shared_data_free(OpenAPI_shared_data_t *shared_data);
OpenAPI_shared_data_t *OpenAPI_shared_data_parseFromJSON(cJSON *shared_dataJSON);
cJSON *OpenAPI_shared_data_convertToJSON(OpenAPI_shared_data_t *shared_data);
OpenAPI_shared_data_t *OpenAPI_shared_data_copy(OpenAPI_shared_data_t *dst, OpenAPI_shared_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_shared_data_H_ */

