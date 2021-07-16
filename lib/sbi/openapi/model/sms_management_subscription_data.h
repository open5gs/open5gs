/*
 * sms_management_subscription_data.h
 *
 * 
 */

#ifndef _OpenAPI_sms_management_subscription_data_H_
#define _OpenAPI_sms_management_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "trace_data_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_management_subscription_data_s OpenAPI_sms_management_subscription_data_t;
typedef struct OpenAPI_sms_management_subscription_data_s {
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
    struct OpenAPI_trace_data_1_s *trace_data;
} OpenAPI_sms_management_subscription_data_t;

OpenAPI_sms_management_subscription_data_t *OpenAPI_sms_management_subscription_data_create(
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
    OpenAPI_trace_data_1_t *trace_data
);
void OpenAPI_sms_management_subscription_data_free(OpenAPI_sms_management_subscription_data_t *sms_management_subscription_data);
OpenAPI_sms_management_subscription_data_t *OpenAPI_sms_management_subscription_data_parseFromJSON(cJSON *sms_management_subscription_dataJSON);
cJSON *OpenAPI_sms_management_subscription_data_convertToJSON(OpenAPI_sms_management_subscription_data_t *sms_management_subscription_data);
OpenAPI_sms_management_subscription_data_t *OpenAPI_sms_management_subscription_data_copy(OpenAPI_sms_management_subscription_data_t *dst, OpenAPI_sms_management_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_management_subscription_data_H_ */

