/*
 * hsmf_update_error.h
 *
 *
 */

#ifndef _OpenAPI_hsmf_update_error_H_
#define _OpenAPI_hsmf_update_error_H_

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

typedef struct OpenAPI_hsmf_update_error_s OpenAPI_hsmf_update_error_t;
typedef struct OpenAPI_hsmf_update_error_s {
    struct OpenAPI_problem_details_s *error;
    int pti;
    char *n1sm_cause;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_to_ue;
    int back_off_timer;
    char *recovery_time;
} OpenAPI_hsmf_update_error_t;

OpenAPI_hsmf_update_error_t *OpenAPI_hsmf_update_error_create(
    OpenAPI_problem_details_t *error,
    int pti,
    char *n1sm_cause,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    int back_off_timer,
    char *recovery_time
    );
void OpenAPI_hsmf_update_error_free(OpenAPI_hsmf_update_error_t *hsmf_update_error);
OpenAPI_hsmf_update_error_t *OpenAPI_hsmf_update_error_parseFromJSON(cJSON *hsmf_update_errorJSON);
cJSON *OpenAPI_hsmf_update_error_convertToJSON(OpenAPI_hsmf_update_error_t *hsmf_update_error);
OpenAPI_hsmf_update_error_t *OpenAPI_hsmf_update_error_copy(OpenAPI_hsmf_update_error_t *dst, OpenAPI_hsmf_update_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hsmf_update_error_H_ */

