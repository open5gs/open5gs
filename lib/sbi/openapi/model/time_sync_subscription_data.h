/*
 * time_sync_subscription_data.h
 *
 * UE Time Synchronization Subscription Data
 */

#ifndef _OpenAPI_time_sync_subscription_data_H_
#define _OpenAPI_time_sync_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_time_sync_subscription_data_s OpenAPI_time_sync_subscription_data_t;
#include "af_request_authorization.h"
#include "time_sync_service_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_time_sync_subscription_data_s {
    struct OpenAPI_af_request_authorization_s *af_req_authorizations;
    OpenAPI_list_t *service_ids;
};

OpenAPI_time_sync_subscription_data_t *OpenAPI_time_sync_subscription_data_create(
    OpenAPI_af_request_authorization_t *af_req_authorizations,
    OpenAPI_list_t *service_ids
);
void OpenAPI_time_sync_subscription_data_free(OpenAPI_time_sync_subscription_data_t *time_sync_subscription_data);
OpenAPI_time_sync_subscription_data_t *OpenAPI_time_sync_subscription_data_parseFromJSON(cJSON *time_sync_subscription_dataJSON);
cJSON *OpenAPI_time_sync_subscription_data_convertToJSON(OpenAPI_time_sync_subscription_data_t *time_sync_subscription_data);
OpenAPI_time_sync_subscription_data_t *OpenAPI_time_sync_subscription_data_copy(OpenAPI_time_sync_subscription_data_t *dst, OpenAPI_time_sync_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_sync_subscription_data_H_ */

