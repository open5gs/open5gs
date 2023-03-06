/*
 * update_sm_context_request.h
 *
 * 
 */

#ifndef _OpenAPI_update_sm_context_request_H_
#define _OpenAPI_update_sm_context_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sm_context_update_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_update_sm_context_request_s OpenAPI_update_sm_context_request_t;
typedef struct OpenAPI_update_sm_context_request_s {
    struct OpenAPI_sm_context_update_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_message;
    OpenAPI_binary_t* binary_data_n2_sm_information;
    OpenAPI_binary_t* binary_data_n2_sm_information_ext1;
} OpenAPI_update_sm_context_request_t;

OpenAPI_update_sm_context_request_t *OpenAPI_update_sm_context_request_create(
    OpenAPI_sm_context_update_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_message,
    OpenAPI_binary_t* binary_data_n2_sm_information,
    OpenAPI_binary_t* binary_data_n2_sm_information_ext1
);
void OpenAPI_update_sm_context_request_free(OpenAPI_update_sm_context_request_t *update_sm_context_request);
OpenAPI_update_sm_context_request_t *OpenAPI_update_sm_context_request_parseFromJSON(cJSON *update_sm_context_requestJSON);
cJSON *OpenAPI_update_sm_context_request_convertToJSON(OpenAPI_update_sm_context_request_t *update_sm_context_request);
OpenAPI_update_sm_context_request_t *OpenAPI_update_sm_context_request_copy(OpenAPI_update_sm_context_request_t *dst, OpenAPI_update_sm_context_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_update_sm_context_request_H_ */

