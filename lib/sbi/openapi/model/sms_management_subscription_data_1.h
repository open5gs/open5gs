/*
 * sms_management_subscription_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_sms_management_subscription_data_1_H_
#define _OpenAPI_sms_management_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "trace_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_management_subscription_data_1_s OpenAPI_sms_management_subscription_data_1_t;
typedef struct OpenAPI_sms_management_subscription_data_1_s {
    char *supported_features;
    bool is_mt_sms_subscribed;
    int mt_sms_subscribed;
    bool is_mt_sms_barring_all;
    int mt_sms_barring_all;
    bool is_mt_sms_barring_roaming;
    int mt_sms_barring_roaming;
    bool is_mo_sms_subscribed;
    int mo_sms_subscribed;
    bool is_mo_sms_barring_all;
    int mo_sms_barring_all;
    bool is_mo_sms_barring_roaming;
    int mo_sms_barring_roaming;
    OpenAPI_list_t *shared_sms_mng_data_ids;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
} OpenAPI_sms_management_subscription_data_1_t;

OpenAPI_sms_management_subscription_data_1_t *OpenAPI_sms_management_subscription_data_1_create(
    char *supported_features,
    bool is_mt_sms_subscribed,
    int mt_sms_subscribed,
    bool is_mt_sms_barring_all,
    int mt_sms_barring_all,
    bool is_mt_sms_barring_roaming,
    int mt_sms_barring_roaming,
    bool is_mo_sms_subscribed,
    int mo_sms_subscribed,
    bool is_mo_sms_barring_all,
    int mo_sms_barring_all,
    bool is_mo_sms_barring_roaming,
    int mo_sms_barring_roaming,
    OpenAPI_list_t *shared_sms_mng_data_ids,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data
);
void OpenAPI_sms_management_subscription_data_1_free(OpenAPI_sms_management_subscription_data_1_t *sms_management_subscription_data_1);
OpenAPI_sms_management_subscription_data_1_t *OpenAPI_sms_management_subscription_data_1_parseFromJSON(cJSON *sms_management_subscription_data_1JSON);
cJSON *OpenAPI_sms_management_subscription_data_1_convertToJSON(OpenAPI_sms_management_subscription_data_1_t *sms_management_subscription_data_1);
OpenAPI_sms_management_subscription_data_1_t *OpenAPI_sms_management_subscription_data_1_copy(OpenAPI_sms_management_subscription_data_1_t *dst, OpenAPI_sms_management_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_management_subscription_data_1_H_ */

