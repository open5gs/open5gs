/*
 * post_sm_contexts_201_response.h
 *
 * 
 */

#ifndef _OpenAPI_post_sm_contexts_201_response_H_
#define _OpenAPI_post_sm_contexts_201_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_post_sm_contexts_201_response_s OpenAPI_post_sm_contexts_201_response_t;
#include "sm_context_created_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_post_sm_contexts_201_response_s {
    struct OpenAPI_sm_context_created_data_s *json_data;
    OpenAPI_binary_t* binary_data_n2_sm_information;
};

OpenAPI_post_sm_contexts_201_response_t *OpenAPI_post_sm_contexts_201_response_create(
    OpenAPI_sm_context_created_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_sm_information
);
void OpenAPI_post_sm_contexts_201_response_free(OpenAPI_post_sm_contexts_201_response_t *post_sm_contexts_201_response);
OpenAPI_post_sm_contexts_201_response_t *OpenAPI_post_sm_contexts_201_response_parseFromJSON(cJSON *post_sm_contexts_201_responseJSON);
cJSON *OpenAPI_post_sm_contexts_201_response_convertToJSON(OpenAPI_post_sm_contexts_201_response_t *post_sm_contexts_201_response);
OpenAPI_post_sm_contexts_201_response_t *OpenAPI_post_sm_contexts_201_response_copy(OpenAPI_post_sm_contexts_201_response_t *dst, OpenAPI_post_sm_contexts_201_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_post_sm_contexts_201_response_H_ */

