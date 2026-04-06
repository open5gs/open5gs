/*
 * nf_profile_registration_error.h
 *
 * NF Profile Registration Error.
 */

#ifndef _OpenAPI_nf_profile_registration_error_H_
#define _OpenAPI_nf_profile_registration_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nf_profile_registration_error_s OpenAPI_nf_profile_registration_error_t;
#include "access_token_err.h"
#include "access_token_req.h"
#include "invalid_param.h"
#include "no_profile_match_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nf_profile_registration_error_s {
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
    OpenAPI_list_t *supported_api_versions;
    struct OpenAPI_no_profile_match_info_s *no_profile_match_info;
    OpenAPI_list_t *shared_data_ids;
};

OpenAPI_nf_profile_registration_error_t *OpenAPI_nf_profile_registration_error_create(
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
    OpenAPI_list_t *supported_api_versions,
    OpenAPI_no_profile_match_info_t *no_profile_match_info,
    OpenAPI_list_t *shared_data_ids
);
void OpenAPI_nf_profile_registration_error_free(OpenAPI_nf_profile_registration_error_t *nf_profile_registration_error);
OpenAPI_nf_profile_registration_error_t *OpenAPI_nf_profile_registration_error_parseFromJSON(cJSON *nf_profile_registration_errorJSON);
cJSON *OpenAPI_nf_profile_registration_error_convertToJSON(OpenAPI_nf_profile_registration_error_t *nf_profile_registration_error);
OpenAPI_nf_profile_registration_error_t *OpenAPI_nf_profile_registration_error_copy(OpenAPI_nf_profile_registration_error_t *dst, OpenAPI_nf_profile_registration_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_profile_registration_error_H_ */

