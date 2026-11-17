/*
 * charging_notify_request.h
 *
 * 
 */

#ifndef _OpenAPI_charging_notify_request_H_
#define _OpenAPI_charging_notify_request_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_notify_request_s OpenAPI_charging_notify_request_t;
#include "charging_notification_type.h"
#include "reauthorization_details.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_notify_request_s {
    OpenAPI_charging_notification_type_e notification_type;
    OpenAPI_list_t *reauthorization_details;
};

OpenAPI_charging_notify_request_t *OpenAPI_charging_notify_request_create(
    OpenAPI_charging_notification_type_e notification_type,
    OpenAPI_list_t *reauthorization_details
);
void OpenAPI_charging_notify_request_free(OpenAPI_charging_notify_request_t *charging_notify_request);
OpenAPI_charging_notify_request_t *OpenAPI_charging_notify_request_parseFromJSON(cJSON *charging_notify_requestJSON);
cJSON *OpenAPI_charging_notify_request_convertToJSON(OpenAPI_charging_notify_request_t *charging_notify_request);
OpenAPI_charging_notify_request_t *OpenAPI_charging_notify_request_copy(OpenAPI_charging_notify_request_t *dst, OpenAPI_charging_notify_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_notify_request_H_ */

