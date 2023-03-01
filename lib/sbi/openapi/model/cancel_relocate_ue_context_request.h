/*
 * cancel_relocate_ue_context_request.h
 *
 * 
 */

#ifndef _OpenAPI_cancel_relocate_ue_context_request_H_
#define _OpenAPI_cancel_relocate_ue_context_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_context_cancel_relocate_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cancel_relocate_ue_context_request_s OpenAPI_cancel_relocate_ue_context_request_t;
typedef struct OpenAPI_cancel_relocate_ue_context_request_s {
    struct OpenAPI_ue_context_cancel_relocate_data_s *json_data;
    OpenAPI_binary_t* binary_data_gtpc_message;
} OpenAPI_cancel_relocate_ue_context_request_t;

OpenAPI_cancel_relocate_ue_context_request_t *OpenAPI_cancel_relocate_ue_context_request_create(
    OpenAPI_ue_context_cancel_relocate_data_t *json_data,
    OpenAPI_binary_t* binary_data_gtpc_message
);
void OpenAPI_cancel_relocate_ue_context_request_free(OpenAPI_cancel_relocate_ue_context_request_t *cancel_relocate_ue_context_request);
OpenAPI_cancel_relocate_ue_context_request_t *OpenAPI_cancel_relocate_ue_context_request_parseFromJSON(cJSON *cancel_relocate_ue_context_requestJSON);
cJSON *OpenAPI_cancel_relocate_ue_context_request_convertToJSON(OpenAPI_cancel_relocate_ue_context_request_t *cancel_relocate_ue_context_request);
OpenAPI_cancel_relocate_ue_context_request_t *OpenAPI_cancel_relocate_ue_context_request_copy(OpenAPI_cancel_relocate_ue_context_request_t *dst, OpenAPI_cancel_relocate_ue_context_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cancel_relocate_ue_context_request_H_ */

