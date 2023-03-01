/*
 * mbs_ext_problem_details.h
 *
 * Contains the FQDN or IP endpoints of the existing PCF and the cause value if there is an  existing PCF binding information for the MBS session. 
 */

#ifndef _OpenAPI_mbs_ext_problem_details_H_
#define _OpenAPI_mbs_ext_problem_details_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_token_err.h"
#include "access_token_req.h"
#include "invalid_param.h"
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_ext_problem_details_s OpenAPI_mbs_ext_problem_details_t;
typedef struct OpenAPI_mbs_ext_problem_details_s {
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
    char *pcf_fqdn;
    OpenAPI_list_t *pcf_ip_end_points;
} OpenAPI_mbs_ext_problem_details_t;

OpenAPI_mbs_ext_problem_details_t *OpenAPI_mbs_ext_problem_details_create(
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
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points
);
void OpenAPI_mbs_ext_problem_details_free(OpenAPI_mbs_ext_problem_details_t *mbs_ext_problem_details);
OpenAPI_mbs_ext_problem_details_t *OpenAPI_mbs_ext_problem_details_parseFromJSON(cJSON *mbs_ext_problem_detailsJSON);
cJSON *OpenAPI_mbs_ext_problem_details_convertToJSON(OpenAPI_mbs_ext_problem_details_t *mbs_ext_problem_details);
OpenAPI_mbs_ext_problem_details_t *OpenAPI_mbs_ext_problem_details_copy(OpenAPI_mbs_ext_problem_details_t *dst, OpenAPI_mbs_ext_problem_details_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_ext_problem_details_H_ */

