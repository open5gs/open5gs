/*
 * ue_context_transfer_request.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_transfer_request_H_
#define _OpenAPI_ue_context_transfer_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_context_transfer_req_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_transfer_request_s OpenAPI_ue_context_transfer_request_t;
typedef struct OpenAPI_ue_context_transfer_request_s {
    struct OpenAPI_ue_context_transfer_req_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_message;
} OpenAPI_ue_context_transfer_request_t;

OpenAPI_ue_context_transfer_request_t *OpenAPI_ue_context_transfer_request_create(
    OpenAPI_ue_context_transfer_req_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message
);
void OpenAPI_ue_context_transfer_request_free(OpenAPI_ue_context_transfer_request_t *ue_context_transfer_request);
OpenAPI_ue_context_transfer_request_t *OpenAPI_ue_context_transfer_request_parseFromJSON(cJSON *ue_context_transfer_requestJSON);
cJSON *OpenAPI_ue_context_transfer_request_convertToJSON(OpenAPI_ue_context_transfer_request_t *ue_context_transfer_request);
OpenAPI_ue_context_transfer_request_t *OpenAPI_ue_context_transfer_request_copy(OpenAPI_ue_context_transfer_request_t *dst, OpenAPI_ue_context_transfer_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_transfer_request_H_ */

