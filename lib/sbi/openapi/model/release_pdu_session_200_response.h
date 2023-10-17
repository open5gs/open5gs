/*
 * release_pdu_session_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_release_pdu_session_200_response_H_
#define _OpenAPI_release_pdu_session_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "released_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_release_pdu_session_200_response_s OpenAPI_release_pdu_session_200_response_t;
typedef struct OpenAPI_release_pdu_session_200_response_s {
    struct OpenAPI_released_data_s *json_data;
    OpenAPI_binary_t* binary_data_n4_information;
    OpenAPI_binary_t* binary_data_n4_information_ext1;
    OpenAPI_binary_t* binary_data_n4_information_ext2;
} OpenAPI_release_pdu_session_200_response_t;

OpenAPI_release_pdu_session_200_response_t *OpenAPI_release_pdu_session_200_response_create(
    OpenAPI_released_data_t *json_data,
    OpenAPI_binary_t* binary_data_n4_information,
    OpenAPI_binary_t* binary_data_n4_information_ext1,
    OpenAPI_binary_t* binary_data_n4_information_ext2
);
void OpenAPI_release_pdu_session_200_response_free(OpenAPI_release_pdu_session_200_response_t *release_pdu_session_200_response);
OpenAPI_release_pdu_session_200_response_t *OpenAPI_release_pdu_session_200_response_parseFromJSON(cJSON *release_pdu_session_200_responseJSON);
cJSON *OpenAPI_release_pdu_session_200_response_convertToJSON(OpenAPI_release_pdu_session_200_response_t *release_pdu_session_200_response);
OpenAPI_release_pdu_session_200_response_t *OpenAPI_release_pdu_session_200_response_copy(OpenAPI_release_pdu_session_200_response_t *dst, OpenAPI_release_pdu_session_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_release_pdu_session_200_response_H_ */

