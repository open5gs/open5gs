/*
 * ext_problem_details.h
 *
 * Extended Problem Details
 */

#ifndef _OpenAPI_ext_problem_details_H_
#define _OpenAPI_ext_problem_details_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_token_err.h"
#include "access_token_req.h"
#include "invalid_param.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ext_problem_details_s OpenAPI_ext_problem_details_t;
typedef struct OpenAPI_ext_problem_details_s {
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
    struct OpenAPI_access_token_req_s *access_token_request;
    char *nrf_id;
    bool is_remote_error;
    int remote_error;
} OpenAPI_ext_problem_details_t;

OpenAPI_ext_problem_details_t *OpenAPI_ext_problem_details_create(
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
    OpenAPI_access_token_req_t *access_token_request,
    char *nrf_id,
    bool is_remote_error,
    int remote_error
);
void OpenAPI_ext_problem_details_free(OpenAPI_ext_problem_details_t *ext_problem_details);
OpenAPI_ext_problem_details_t *OpenAPI_ext_problem_details_parseFromJSON(cJSON *ext_problem_detailsJSON);
cJSON *OpenAPI_ext_problem_details_convertToJSON(OpenAPI_ext_problem_details_t *ext_problem_details);
OpenAPI_ext_problem_details_t *OpenAPI_ext_problem_details_copy(OpenAPI_ext_problem_details_t *dst, OpenAPI_ext_problem_details_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ext_problem_details_H_ */

