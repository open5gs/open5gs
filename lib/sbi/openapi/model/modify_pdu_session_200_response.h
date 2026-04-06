/*
 * modify_pdu_session_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_modify_pdu_session_200_response_H_
#define _OpenAPI_modify_pdu_session_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_modify_pdu_session_200_response_s OpenAPI_modify_pdu_session_200_response_t;
#include "vsmf_updated_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_modify_pdu_session_200_response_s {
    struct OpenAPI_vsmf_updated_data_s *json_data;
    OpenAPI_binary_t* binary_data_n1_sm_info_from_ue;
    OpenAPI_binary_t* binary_data_unknown_n1_sm_info;
    OpenAPI_binary_t* binary_data_n4_information;
    OpenAPI_binary_t* binary_data_n4_information_ext1;
    OpenAPI_binary_t* binary_data_n4_information_ext2;
    OpenAPI_binary_t* binary_data_n4_information_ext3;
};

OpenAPI_modify_pdu_session_200_response_t *OpenAPI_modify_pdu_session_200_response_create(
    OpenAPI_vsmf_updated_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_from_ue,
    OpenAPI_binary_t* binary_data_unknown_n1_sm_info,
    OpenAPI_binary_t* binary_data_n4_information,
    OpenAPI_binary_t* binary_data_n4_information_ext1,
    OpenAPI_binary_t* binary_data_n4_information_ext2,
    OpenAPI_binary_t* binary_data_n4_information_ext3
);
void OpenAPI_modify_pdu_session_200_response_free(OpenAPI_modify_pdu_session_200_response_t *modify_pdu_session_200_response);
OpenAPI_modify_pdu_session_200_response_t *OpenAPI_modify_pdu_session_200_response_parseFromJSON(cJSON *modify_pdu_session_200_responseJSON);
cJSON *OpenAPI_modify_pdu_session_200_response_convertToJSON(OpenAPI_modify_pdu_session_200_response_t *modify_pdu_session_200_response);
OpenAPI_modify_pdu_session_200_response_t *OpenAPI_modify_pdu_session_200_response_copy(OpenAPI_modify_pdu_session_200_response_t *dst, OpenAPI_modify_pdu_session_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_modify_pdu_session_200_response_H_ */

