/*
 * problem_details_1.h
 *
 * Provides additional information in an error response.
 */

#ifndef _OpenAPI_problem_details_1_H_
#define _OpenAPI_problem_details_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_problem_details_1_s OpenAPI_problem_details_1_t;
#include "access_token_err.h"
#include "access_token_req_1.h"
#include "invalid_param.h"
#include "no_profile_match_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_problem_details_1_s {
    char *type;
    char *title;
    bool is_status;
    int status;
    char *detail;
    char *instance;
    char *cause;
    OpenAPI_list_t *invalid_params;
    char *supported_features;
    struct OpenAPI_access_token_err_s *access_token_error;
    struct OpenAPI_access_token_req_1_s *access_token_request;
    char *nrf_id;
    OpenAPI_list_t *supported_api_versions;
    struct OpenAPI_no_profile_match_info_1_s *no_profile_match_info;
};

OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_create(
    char *type,
    char *title,
    bool is_status,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params,
    char *supported_features,
    OpenAPI_access_token_err_t *access_token_error,
    OpenAPI_access_token_req_1_t *access_token_request,
    char *nrf_id,
    OpenAPI_list_t *supported_api_versions,
    OpenAPI_no_profile_match_info_1_t *no_profile_match_info
);
void OpenAPI_problem_details_1_free(OpenAPI_problem_details_1_t *problem_details_1);
OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_parseFromJSON(cJSON *problem_details_1JSON);
cJSON *OpenAPI_problem_details_1_convertToJSON(OpenAPI_problem_details_1_t *problem_details_1);
OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_copy(OpenAPI_problem_details_1_t *dst, OpenAPI_problem_details_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_problem_details_1_H_ */

