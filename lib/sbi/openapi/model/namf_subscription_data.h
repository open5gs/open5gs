/*
 * namf_subscription_data.h
 *
 * Data within an AMF Status Change Subscription request and response
 */

#ifndef _OpenAPI_namf_subscription_data_H_
#define _OpenAPI_namf_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_namf_subscription_data_s OpenAPI_namf_subscription_data_t;
typedef struct OpenAPI_namf_subscription_data_s {
    char *amf_status_uri;
    OpenAPI_list_t *guami_list;
} OpenAPI_namf_subscription_data_t;

OpenAPI_namf_subscription_data_t *OpenAPI_namf_subscription_data_create(
    char *amf_status_uri,
    OpenAPI_list_t *guami_list
);
void OpenAPI_namf_subscription_data_free(OpenAPI_namf_subscription_data_t *namf_subscription_data);
OpenAPI_namf_subscription_data_t *OpenAPI_namf_subscription_data_parseFromJSON(cJSON *namf_subscription_dataJSON);
cJSON *OpenAPI_namf_subscription_data_convertToJSON(OpenAPI_namf_subscription_data_t *namf_subscription_data);
OpenAPI_namf_subscription_data_t *OpenAPI_namf_subscription_data_copy(OpenAPI_namf_subscription_data_t *dst, OpenAPI_namf_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_namf_subscription_data_H_ */

