/*
 * n1_message_notify_request.h
 *
 * 
 */

#ifndef _OpenAPI_n1_message_notify_request_H_
#define _OpenAPI_n1_message_notify_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_n1_message_notify_request_s OpenAPI_n1_message_notify_request_t;
#include "n1_message_notification.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_n1_message_notify_request_s {
    struct OpenAPI_n1_message_notification_s *json_data;
    OpenAPI_binary_t* binary_data_n1_message;
};

OpenAPI_n1_message_notify_request_t *OpenAPI_n1_message_notify_request_create(
    OpenAPI_n1_message_notification_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message
);
void OpenAPI_n1_message_notify_request_free(OpenAPI_n1_message_notify_request_t *n1_message_notify_request);
OpenAPI_n1_message_notify_request_t *OpenAPI_n1_message_notify_request_parseFromJSON(cJSON *n1_message_notify_requestJSON);
cJSON *OpenAPI_n1_message_notify_request_convertToJSON(OpenAPI_n1_message_notify_request_t *n1_message_notify_request);
OpenAPI_n1_message_notify_request_t *OpenAPI_n1_message_notify_request_copy(OpenAPI_n1_message_notify_request_t *dst, OpenAPI_n1_message_notify_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_message_notify_request_H_ */

