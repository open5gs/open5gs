/*
 * pdu_session_create_error.h
 *
 * Error within Create Response
 */

#ifndef _OpenAPI_pdu_session_create_error_H_
#define _OpenAPI_pdu_session_create_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "problem_details.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_create_error_s OpenAPI_pdu_session_create_error_t;
typedef struct OpenAPI_pdu_session_create_error_s {
    struct OpenAPI_problem_details_s *error;
    char *n1sm_cause;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_to_ue;
    bool is_back_off_timer;
    int back_off_timer;
    char *recovery_time;
} OpenAPI_pdu_session_create_error_t;

OpenAPI_pdu_session_create_error_t *OpenAPI_pdu_session_create_error_create(
    OpenAPI_problem_details_t *error,
    char *n1sm_cause,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    bool is_back_off_timer,
    int back_off_timer,
    char *recovery_time
);
void OpenAPI_pdu_session_create_error_free(OpenAPI_pdu_session_create_error_t *pdu_session_create_error);
OpenAPI_pdu_session_create_error_t *OpenAPI_pdu_session_create_error_parseFromJSON(cJSON *pdu_session_create_errorJSON);
cJSON *OpenAPI_pdu_session_create_error_convertToJSON(OpenAPI_pdu_session_create_error_t *pdu_session_create_error);
OpenAPI_pdu_session_create_error_t *OpenAPI_pdu_session_create_error_copy(OpenAPI_pdu_session_create_error_t *dst, OpenAPI_pdu_session_create_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_create_error_H_ */

