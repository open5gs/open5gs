/*
 * _chargingdata_post_400_response_1.h
 *
 * 
 */

#ifndef _OpenAPI__chargingdata_post_400_response_1_H_
#define _OpenAPI__chargingdata_post_400_response_1_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI__chargingdata_post_400_response_1_s OpenAPI__chargingdata_post_400_response_1_t;
#include "access_token_err.h"
#include "access_token_req.h"
#include "invalid_param.h"
#include "invocation_result.h"
#include "no_profile_match_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI__chargingdata_post_400_response_1_s {
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
    struct OpenAPI_invocation_result_s *invocation_result;
};

OpenAPI__chargingdata_post_400_response_1_t *OpenAPI__chargingdata_post_400_response_1_create(
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
    OpenAPI_invocation_result_t *invocation_result
);
void OpenAPI__chargingdata_post_400_response_1_free(OpenAPI__chargingdata_post_400_response_1_t *_chargingdata_post_400_response_1);
OpenAPI__chargingdata_post_400_response_1_t *OpenAPI__chargingdata_post_400_response_1_parseFromJSON(cJSON *_chargingdata_post_400_response_1JSON);
cJSON *OpenAPI__chargingdata_post_400_response_1_convertToJSON(OpenAPI__chargingdata_post_400_response_1_t *_chargingdata_post_400_response_1);
OpenAPI__chargingdata_post_400_response_1_t *OpenAPI__chargingdata_post_400_response_1_copy(OpenAPI__chargingdata_post_400_response_1_t *dst, OpenAPI__chargingdata_post_400_response_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI__chargingdata_post_400_response_1_H_ */

