/*
 * release_sm_context_request.h
 *
 * 
 */

#ifndef _OpenAPI_release_sm_context_request_H_
#define _OpenAPI_release_sm_context_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sm_context_release_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_release_sm_context_request_s OpenAPI_release_sm_context_request_t;
typedef struct OpenAPI_release_sm_context_request_s {
    struct OpenAPI_sm_context_release_data_s *json_data;
    OpenAPI_binary_t* binary_data_n2_sm_information;
} OpenAPI_release_sm_context_request_t;

OpenAPI_release_sm_context_request_t *OpenAPI_release_sm_context_request_create(
    OpenAPI_sm_context_release_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_sm_information
);
void OpenAPI_release_sm_context_request_free(OpenAPI_release_sm_context_request_t *release_sm_context_request);
OpenAPI_release_sm_context_request_t *OpenAPI_release_sm_context_request_parseFromJSON(cJSON *release_sm_context_requestJSON);
cJSON *OpenAPI_release_sm_context_request_convertToJSON(OpenAPI_release_sm_context_request_t *release_sm_context_request);
OpenAPI_release_sm_context_request_t *OpenAPI_release_sm_context_request_copy(OpenAPI_release_sm_context_request_t *dst, OpenAPI_release_sm_context_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_release_sm_context_request_H_ */

