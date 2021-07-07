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
#include "access_and_mobility_subscription_data_1.h"
#include "dnn_configuration_1.h"
#include "sms_management_subscription_data_1.h"
#include "sms_subscription_data_1.h"
#include "snssai_info.h"
#include "trace_data.h"
#include "vn_group_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_shared_data_s OpenAPI_shared_data_t;
typedef struct OpenAPI_shared_data_s {
    char *shared_data_id;
    struct OpenAPI_access_and_mobility_subscription_data_1_s *shared_am_data;
    struct OpenAPI_sms_subscription_data_1_s *shared_sms_subs_data;
    struct OpenAPI_sms_management_subscription_data_1_s *shared_sms_mng_subs_data;
    OpenAPI_list_t* shared_dnn_configurations;
    struct OpenAPI_trace_data_s *shared_trace_data;
    OpenAPI_list_t* shared_snssai_infos;
    OpenAPI_list_t* shared_vn_group_datas;
} OpenAPI_shared_data_t;

OpenAPI_shared_data_t *OpenAPI_shared_data_create(
    char *shared_data_id,
    OpenAPI_access_and_mobility_subscription_data_1_t *shared_am_data,
    OpenAPI_sms_subscription_data_1_t *shared_sms_subs_data,
    OpenAPI_sms_management_subscription_data_1_t *shared_sms_mng_subs_data,
    OpenAPI_list_t* shared_dnn_configurations,
    OpenAPI_trace_data_t *shared_trace_data,
    OpenAPI_list_t* shared_snssai_infos,
    OpenAPI_list_t* shared_vn_group_datas
);
void OpenAPI_shared_data_free(OpenAPI_shared_data_t *shared_data);
OpenAPI_shared_data_t *OpenAPI_shared_data_parseFromJSON(cJSON *shared_dataJSON);
cJSON *OpenAPI_shared_data_convertToJSON(OpenAPI_shared_data_t *shared_data);
OpenAPI_shared_data_t *OpenAPI_shared_data_copy(OpenAPI_shared_data_t *dst, OpenAPI_shared_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_shared_data_H_ */

