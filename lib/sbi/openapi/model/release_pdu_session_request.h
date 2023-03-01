/*
 * release_pdu_session_request.h
 *
 * 
 */

#ifndef _OpenAPI_release_pdu_session_request_H_
#define _OpenAPI_release_pdu_session_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "release_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_release_pdu_session_request_s OpenAPI_release_pdu_session_request_t;
typedef struct OpenAPI_release_pdu_session_request_s {
    struct OpenAPI_release_data_s *json_data;
    OpenAPI_binary_t* binary_data_n4_information;
    OpenAPI_binary_t* binary_data_n4_information_ext1;
    OpenAPI_binary_t* binary_data_n4_information_ext2;
} OpenAPI_release_pdu_session_request_t;

OpenAPI_release_pdu_session_request_t *OpenAPI_release_pdu_session_request_create(
    OpenAPI_release_data_t *json_data,
    OpenAPI_binary_t* binary_data_n4_information,
    OpenAPI_binary_t* binary_data_n4_information_ext1,
    OpenAPI_binary_t* binary_data_n4_information_ext2
);
void OpenAPI_release_pdu_session_request_free(OpenAPI_release_pdu_session_request_t *release_pdu_session_request);
OpenAPI_release_pdu_session_request_t *OpenAPI_release_pdu_session_request_parseFromJSON(cJSON *release_pdu_session_requestJSON);
cJSON *OpenAPI_release_pdu_session_request_convertToJSON(OpenAPI_release_pdu_session_request_t *release_pdu_session_request);
OpenAPI_release_pdu_session_request_t *OpenAPI_release_pdu_session_request_copy(OpenAPI_release_pdu_session_request_t *dst, OpenAPI_release_pdu_session_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_release_pdu_session_request_H_ */

