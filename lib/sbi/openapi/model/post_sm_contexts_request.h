/*
 * post_sm_contexts_request.h
 *
 * 
 */

#ifndef _OpenAPI_post_sm_contexts_request_H_
#define _OpenAPI_post_sm_contexts_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_post_sm_contexts_request_s OpenAPI_post_sm_contexts_request_t;
#include "sm_context_create_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_post_sm_contexts_request_s {
    struct OpenAPI_sm_context_create_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_message;
    OpenAPI_binary_t* binary_data_n2_sm_information;
    OpenAPI_binary_t* binary_data_n2_sm_information_ext1;
};

OpenAPI_post_sm_contexts_request_t *OpenAPI_post_sm_contexts_request_create(
    OpenAPI_sm_context_create_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_message,
    OpenAPI_binary_t* binary_data_n2_sm_information,
    OpenAPI_binary_t* binary_data_n2_sm_information_ext1
);
void OpenAPI_post_sm_contexts_request_free(OpenAPI_post_sm_contexts_request_t *post_sm_contexts_request);
OpenAPI_post_sm_contexts_request_t *OpenAPI_post_sm_contexts_request_parseFromJSON(cJSON *post_sm_contexts_requestJSON);
cJSON *OpenAPI_post_sm_contexts_request_convertToJSON(OpenAPI_post_sm_contexts_request_t *post_sm_contexts_request);
OpenAPI_post_sm_contexts_request_t *OpenAPI_post_sm_contexts_request_copy(OpenAPI_post_sm_contexts_request_t *dst, OpenAPI_post_sm_contexts_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_post_sm_contexts_request_H_ */

