/*
 * send_mo_data_request.h
 *
 * 
 */

#ifndef _OpenAPI_send_mo_data_request_H_
#define _OpenAPI_send_mo_data_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "send_mo_data_req_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_send_mo_data_request_s OpenAPI_send_mo_data_request_t;
typedef struct OpenAPI_send_mo_data_request_s {
    struct OpenAPI_send_mo_data_req_data_s *json_data;
    OpenAPI_binary_t* binary_mo_data;
} OpenAPI_send_mo_data_request_t;

OpenAPI_send_mo_data_request_t *OpenAPI_send_mo_data_request_create(
    OpenAPI_send_mo_data_req_data_t *json_data,
    OpenAPI_binary_t* binary_mo_data
);
void OpenAPI_send_mo_data_request_free(OpenAPI_send_mo_data_request_t *send_mo_data_request);
OpenAPI_send_mo_data_request_t *OpenAPI_send_mo_data_request_parseFromJSON(cJSON *send_mo_data_requestJSON);
cJSON *OpenAPI_send_mo_data_request_convertToJSON(OpenAPI_send_mo_data_request_t *send_mo_data_request);
OpenAPI_send_mo_data_request_t *OpenAPI_send_mo_data_request_copy(OpenAPI_send_mo_data_request_t *dst, OpenAPI_send_mo_data_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_send_mo_data_request_H_ */

