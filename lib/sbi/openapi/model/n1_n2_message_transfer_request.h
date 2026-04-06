/*
 * n1_n2_message_transfer_request.h
 *
 * 
 */

#ifndef _OpenAPI_n1_n2_message_transfer_request_H_
#define _OpenAPI_n1_n2_message_transfer_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_n1_n2_message_transfer_request_s OpenAPI_n1_n2_message_transfer_request_t;
#include "n1_n2_message_transfer_req_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_n1_n2_message_transfer_request_s {
    struct OpenAPI_n1_n2_message_transfer_req_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_message;
    OpenAPI_binary_t* binary_data_n2_information;
    OpenAPI_binary_t* binary_mt_data;
};

OpenAPI_n1_n2_message_transfer_request_t *OpenAPI_n1_n2_message_transfer_request_create(
    OpenAPI_n1_n2_message_transfer_req_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message,
    OpenAPI_binary_t* binary_data_n2_information,
    OpenAPI_binary_t* binary_mt_data
);
void OpenAPI_n1_n2_message_transfer_request_free(OpenAPI_n1_n2_message_transfer_request_t *n1_n2_message_transfer_request);
OpenAPI_n1_n2_message_transfer_request_t *OpenAPI_n1_n2_message_transfer_request_parseFromJSON(cJSON *n1_n2_message_transfer_requestJSON);
cJSON *OpenAPI_n1_n2_message_transfer_request_convertToJSON(OpenAPI_n1_n2_message_transfer_request_t *n1_n2_message_transfer_request);
OpenAPI_n1_n2_message_transfer_request_t *OpenAPI_n1_n2_message_transfer_request_copy(OpenAPI_n1_n2_message_transfer_request_t *dst, OpenAPI_n1_n2_message_transfer_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_n2_message_transfer_request_H_ */

