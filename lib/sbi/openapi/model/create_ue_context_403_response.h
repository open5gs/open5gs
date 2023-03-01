/*
 * create_ue_context_403_response.h
 *
 * 
 */

#ifndef _OpenAPI_create_ue_context_403_response_H_
#define _OpenAPI_create_ue_context_403_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_context_create_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_create_ue_context_403_response_s OpenAPI_create_ue_context_403_response_t;
typedef struct OpenAPI_create_ue_context_403_response_s {
    struct OpenAPI_ue_context_create_error_s *json_data;
    OpenAPI_binary_t* binary_data_n2_information;
} OpenAPI_create_ue_context_403_response_t;

OpenAPI_create_ue_context_403_response_t *OpenAPI_create_ue_context_403_response_create(
    OpenAPI_ue_context_create_error_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information
);
void OpenAPI_create_ue_context_403_response_free(OpenAPI_create_ue_context_403_response_t *create_ue_context_403_response);
OpenAPI_create_ue_context_403_response_t *OpenAPI_create_ue_context_403_response_parseFromJSON(cJSON *create_ue_context_403_responseJSON);
cJSON *OpenAPI_create_ue_context_403_response_convertToJSON(OpenAPI_create_ue_context_403_response_t *create_ue_context_403_response);
OpenAPI_create_ue_context_403_response_t *OpenAPI_create_ue_context_403_response_copy(OpenAPI_create_ue_context_403_response_t *dst, OpenAPI_create_ue_context_403_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_create_ue_context_403_response_H_ */

