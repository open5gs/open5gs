/*
 * update_pdu_session_request.h
 *
 * 
 */

#ifndef _OpenAPI_update_pdu_session_request_H_
#define _OpenAPI_update_pdu_session_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hsmf_update_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_update_pdu_session_request_s OpenAPI_update_pdu_session_request_t;
typedef struct OpenAPI_update_pdu_session_request_s {
    struct OpenAPI_hsmf_update_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_info_from_ue;
    OpenAPI_binary_t* binary_data_unknown_n1_sm_info;
    OpenAPI_binary_t* binary_data_n4_information;
    OpenAPI_binary_t* binary_data_n4_information_ext1;
    OpenAPI_binary_t* binary_data_n4_information_ext2;
} OpenAPI_update_pdu_session_request_t;

OpenAPI_update_pdu_session_request_t *OpenAPI_update_pdu_session_request_create(
    OpenAPI_hsmf_update_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_from_ue,
    OpenAPI_binary_t* binary_data_unknown_n1_sm_info,
    OpenAPI_binary_t* binary_data_n4_information,
    OpenAPI_binary_t* binary_data_n4_information_ext1,
    OpenAPI_binary_t* binary_data_n4_information_ext2
);
void OpenAPI_update_pdu_session_request_free(OpenAPI_update_pdu_session_request_t *update_pdu_session_request);
OpenAPI_update_pdu_session_request_t *OpenAPI_update_pdu_session_request_parseFromJSON(cJSON *update_pdu_session_requestJSON);
cJSON *OpenAPI_update_pdu_session_request_convertToJSON(OpenAPI_update_pdu_session_request_t *update_pdu_session_request);
OpenAPI_update_pdu_session_request_t *OpenAPI_update_pdu_session_request_copy(OpenAPI_update_pdu_session_request_t *dst, OpenAPI_update_pdu_session_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_update_pdu_session_request_H_ */

