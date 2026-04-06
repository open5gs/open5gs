/*
 * modify_pdu_session_request.h
 *
 * 
 */

#ifndef _OpenAPI_modify_pdu_session_request_H_
#define _OpenAPI_modify_pdu_session_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_modify_pdu_session_request_s OpenAPI_modify_pdu_session_request_t;
#include "vsmf_update_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_modify_pdu_session_request_s {
    struct OpenAPI_vsmf_update_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue;
    OpenAPI_binary_t* binary_data_n4_information;
    OpenAPI_binary_t* binary_data_n4_information_ext1;
    OpenAPI_binary_t* binary_data_n4_information_ext2;
    OpenAPI_binary_t* binary_data_n4_information_ext3;
};

OpenAPI_modify_pdu_session_request_t *OpenAPI_modify_pdu_session_request_create(
    OpenAPI_vsmf_update_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue,
    OpenAPI_binary_t* binary_data_n4_information,
    OpenAPI_binary_t* binary_data_n4_information_ext1,
    OpenAPI_binary_t* binary_data_n4_information_ext2,
    OpenAPI_binary_t* binary_data_n4_information_ext3
);
void OpenAPI_modify_pdu_session_request_free(OpenAPI_modify_pdu_session_request_t *modify_pdu_session_request);
OpenAPI_modify_pdu_session_request_t *OpenAPI_modify_pdu_session_request_parseFromJSON(cJSON *modify_pdu_session_requestJSON);
cJSON *OpenAPI_modify_pdu_session_request_convertToJSON(OpenAPI_modify_pdu_session_request_t *modify_pdu_session_request);
OpenAPI_modify_pdu_session_request_t *OpenAPI_modify_pdu_session_request_copy(OpenAPI_modify_pdu_session_request_t *dst, OpenAPI_modify_pdu_session_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_modify_pdu_session_request_H_ */

