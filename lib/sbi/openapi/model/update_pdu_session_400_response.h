/*
 * update_pdu_session_400_response.h
 *
 * 
 */

#ifndef _OpenAPI_update_pdu_session_400_response_H_
#define _OpenAPI_update_pdu_session_400_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hsmf_update_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_update_pdu_session_400_response_s OpenAPI_update_pdu_session_400_response_t;
typedef struct OpenAPI_update_pdu_session_400_response_s {
    struct OpenAPI_hsmf_update_error_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue;
} OpenAPI_update_pdu_session_400_response_t;

OpenAPI_update_pdu_session_400_response_t *OpenAPI_update_pdu_session_400_response_create(
    OpenAPI_hsmf_update_error_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue
);
void OpenAPI_update_pdu_session_400_response_free(OpenAPI_update_pdu_session_400_response_t *update_pdu_session_400_response);
OpenAPI_update_pdu_session_400_response_t *OpenAPI_update_pdu_session_400_response_parseFromJSON(cJSON *update_pdu_session_400_responseJSON);
cJSON *OpenAPI_update_pdu_session_400_response_convertToJSON(OpenAPI_update_pdu_session_400_response_t *update_pdu_session_400_response);
OpenAPI_update_pdu_session_400_response_t *OpenAPI_update_pdu_session_400_response_copy(OpenAPI_update_pdu_session_400_response_t *dst, OpenAPI_update_pdu_session_400_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_update_pdu_session_400_response_H_ */

