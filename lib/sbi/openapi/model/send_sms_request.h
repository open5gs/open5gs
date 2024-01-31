/*
 * send_sms_request.h
 *
 * 
 */

#ifndef _OpenAPI_send_sms_request_H_
#define _OpenAPI_send_sms_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sms_record_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_send_sms_request_s OpenAPI_send_sms_request_t;
typedef struct OpenAPI_send_sms_request_s {
    struct OpenAPI_sms_record_data_s *json_data;
    OpenAPI_binary_t* binary_payload;
} OpenAPI_send_sms_request_t;

OpenAPI_send_sms_request_t *OpenAPI_send_sms_request_create(
    OpenAPI_sms_record_data_t *json_data,
    OpenAPI_binary_t* binary_payload
);
void OpenAPI_send_sms_request_free(OpenAPI_send_sms_request_t *send_sms_request);
OpenAPI_send_sms_request_t *OpenAPI_send_sms_request_parseFromJSON(cJSON *send_sms_requestJSON);
cJSON *OpenAPI_send_sms_request_convertToJSON(OpenAPI_send_sms_request_t *send_sms_request);
OpenAPI_send_sms_request_t *OpenAPI_send_sms_request_copy(OpenAPI_send_sms_request_t *dst, OpenAPI_send_sms_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_send_sms_request_H_ */

