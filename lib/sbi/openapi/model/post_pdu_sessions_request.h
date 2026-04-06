/*
 * post_pdu_sessions_request.h
 *
 * 
 */

#ifndef _OpenAPI_post_pdu_sessions_request_H_
#define _OpenAPI_post_pdu_sessions_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_post_pdu_sessions_request_s OpenAPI_post_pdu_sessions_request_t;
#include "pdu_session_create_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_post_pdu_sessions_request_s {
    struct OpenAPI_pdu_session_create_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_info_from_ue;
    OpenAPI_binary_t* binary_data_unknown_n1_sm_info;
};

OpenAPI_post_pdu_sessions_request_t *OpenAPI_post_pdu_sessions_request_create(
    OpenAPI_pdu_session_create_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_from_ue,
    OpenAPI_binary_t* binary_data_unknown_n1_sm_info
);
void OpenAPI_post_pdu_sessions_request_free(OpenAPI_post_pdu_sessions_request_t *post_pdu_sessions_request);
OpenAPI_post_pdu_sessions_request_t *OpenAPI_post_pdu_sessions_request_parseFromJSON(cJSON *post_pdu_sessions_requestJSON);
cJSON *OpenAPI_post_pdu_sessions_request_convertToJSON(OpenAPI_post_pdu_sessions_request_t *post_pdu_sessions_request);
OpenAPI_post_pdu_sessions_request_t *OpenAPI_post_pdu_sessions_request_copy(OpenAPI_post_pdu_sessions_request_t *dst, OpenAPI_post_pdu_sessions_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_post_pdu_sessions_request_H_ */

