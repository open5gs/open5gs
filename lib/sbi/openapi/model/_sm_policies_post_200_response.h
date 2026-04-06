/*
 * _sm_policies_post_200_response.h
 *
 * 
 */

#ifndef _OpenAPI__sm_policies_post_200_response_H_
#define _OpenAPI__sm_policies_post_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI__sm_policies_post_200_response_s OpenAPI__sm_policies_post_200_response_t;
#include "partial_success_report.h"
#include "policy_decision_failure_code.h"
#include "ue_camping_rep.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI__sm_policies_post_200_response_s {
    struct OpenAPI_ue_camping_rep_s *ue_camping_rep;
    OpenAPI_list_t *partial_success_report_list;
    OpenAPI_list_t *policy_decision_failure_code_list;
};

OpenAPI__sm_policies_post_200_response_t *OpenAPI__sm_policies_post_200_response_create(
    OpenAPI_ue_camping_rep_t *ue_camping_rep,
    OpenAPI_list_t *partial_success_report_list,
    OpenAPI_list_t *policy_decision_failure_code_list
);
void OpenAPI__sm_policies_post_200_response_free(OpenAPI__sm_policies_post_200_response_t *_sm_policies_post_200_response);
OpenAPI__sm_policies_post_200_response_t *OpenAPI__sm_policies_post_200_response_parseFromJSON(cJSON *_sm_policies_post_200_responseJSON);
cJSON *OpenAPI__sm_policies_post_200_response_convertToJSON(OpenAPI__sm_policies_post_200_response_t *_sm_policies_post_200_response);
OpenAPI__sm_policies_post_200_response_t *OpenAPI__sm_policies_post_200_response_copy(OpenAPI__sm_policies_post_200_response_t *dst, OpenAPI__sm_policies_post_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI__sm_policies_post_200_response_H_ */

