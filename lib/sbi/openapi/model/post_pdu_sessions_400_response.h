/*
 * post_pdu_sessions_400_response.h
 *
 * 
 */

#ifndef _OpenAPI_post_pdu_sessions_400_response_H_
#define _OpenAPI_post_pdu_sessions_400_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_post_pdu_sessions_400_response_s OpenAPI_post_pdu_sessions_400_response_t;
#include "pdu_session_create_error.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_post_pdu_sessions_400_response_s {
    struct OpenAPI_pdu_session_create_error_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue;
};

OpenAPI_post_pdu_sessions_400_response_t *OpenAPI_post_pdu_sessions_400_response_create(
    OpenAPI_pdu_session_create_error_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue
);
void OpenAPI_post_pdu_sessions_400_response_free(OpenAPI_post_pdu_sessions_400_response_t *post_pdu_sessions_400_response);
OpenAPI_post_pdu_sessions_400_response_t *OpenAPI_post_pdu_sessions_400_response_parseFromJSON(cJSON *post_pdu_sessions_400_responseJSON);
cJSON *OpenAPI_post_pdu_sessions_400_response_convertToJSON(OpenAPI_post_pdu_sessions_400_response_t *post_pdu_sessions_400_response);
OpenAPI_post_pdu_sessions_400_response_t *OpenAPI_post_pdu_sessions_400_response_copy(OpenAPI_post_pdu_sessions_400_response_t *dst, OpenAPI_post_pdu_sessions_400_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_post_pdu_sessions_400_response_H_ */

