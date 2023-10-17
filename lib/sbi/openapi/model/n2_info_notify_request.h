/*
 * n2_info_notify_request.h
 *
 * 
 */

#ifndef _OpenAPI_n2_info_notify_request_H_
#define _OpenAPI_n2_info_notify_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_information_notification.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_info_notify_request_s OpenAPI_n2_info_notify_request_t;
typedef struct OpenAPI_n2_info_notify_request_s {
    struct OpenAPI_n2_information_notification_s *json_data;
    OpenAPI_binary_t* binary_data_n1_message;
    OpenAPI_binary_t* binary_data_n2_information;
} OpenAPI_n2_info_notify_request_t;

OpenAPI_n2_info_notify_request_t *OpenAPI_n2_info_notify_request_create(
    OpenAPI_n2_information_notification_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message,
    OpenAPI_binary_t* binary_data_n2_information
);
void OpenAPI_n2_info_notify_request_free(OpenAPI_n2_info_notify_request_t *n2_info_notify_request);
OpenAPI_n2_info_notify_request_t *OpenAPI_n2_info_notify_request_parseFromJSON(cJSON *n2_info_notify_requestJSON);
cJSON *OpenAPI_n2_info_notify_request_convertToJSON(OpenAPI_n2_info_notify_request_t *n2_info_notify_request);
OpenAPI_n2_info_notify_request_t *OpenAPI_n2_info_notify_request_copy(OpenAPI_n2_info_notify_request_t *dst, OpenAPI_n2_info_notify_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_info_notify_request_H_ */

