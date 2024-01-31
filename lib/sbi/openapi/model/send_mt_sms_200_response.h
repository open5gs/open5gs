/*
 * send_mt_sms_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_send_mt_sms_200_response_H_
#define _OpenAPI_send_mt_sms_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sms_delivery_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_send_mt_sms_200_response_s OpenAPI_send_mt_sms_200_response_t;
typedef struct OpenAPI_send_mt_sms_200_response_s {
    struct OpenAPI_sms_delivery_data_s *json_data;
    OpenAPI_binary_t* binary_payload;
} OpenAPI_send_mt_sms_200_response_t;

OpenAPI_send_mt_sms_200_response_t *OpenAPI_send_mt_sms_200_response_create(
    OpenAPI_sms_delivery_data_t *json_data,
    OpenAPI_binary_t* binary_payload
);
void OpenAPI_send_mt_sms_200_response_free(OpenAPI_send_mt_sms_200_response_t *send_mt_sms_200_response);
OpenAPI_send_mt_sms_200_response_t *OpenAPI_send_mt_sms_200_response_parseFromJSON(cJSON *send_mt_sms_200_responseJSON);
cJSON *OpenAPI_send_mt_sms_200_response_convertToJSON(OpenAPI_send_mt_sms_200_response_t *send_mt_sms_200_response);
OpenAPI_send_mt_sms_200_response_t *OpenAPI_send_mt_sms_200_response_copy(OpenAPI_send_mt_sms_200_response_t *dst, OpenAPI_send_mt_sms_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_send_mt_sms_200_response_H_ */

