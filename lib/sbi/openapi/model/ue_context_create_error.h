/*
 * ue_context_create_error.h
 *
 * Data within a failure response for creating a UE context
 */

#ifndef _OpenAPI_ue_context_create_error_H_
#define _OpenAPI_ue_context_create_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"
#include "ng_ap_cause.h"
#include "problem_details.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_create_error_s OpenAPI_ue_context_create_error_t;
typedef struct OpenAPI_ue_context_create_error_s {
    struct OpenAPI_problem_details_s *error;
    struct OpenAPI_ng_ap_cause_s *ngap_cause;
    struct OpenAPI_n2_info_content_s *target_to_source_failure_data;
} OpenAPI_ue_context_create_error_t;

OpenAPI_ue_context_create_error_t *OpenAPI_ue_context_create_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_ng_ap_cause_t *ngap_cause,
    OpenAPI_n2_info_content_t *target_to_source_failure_data
);
void OpenAPI_ue_context_create_error_free(OpenAPI_ue_context_create_error_t *ue_context_create_error);
OpenAPI_ue_context_create_error_t *OpenAPI_ue_context_create_error_parseFromJSON(cJSON *ue_context_create_errorJSON);
cJSON *OpenAPI_ue_context_create_error_convertToJSON(OpenAPI_ue_context_create_error_t *ue_context_create_error);
OpenAPI_ue_context_create_error_t *OpenAPI_ue_context_create_error_copy(OpenAPI_ue_context_create_error_t *dst, OpenAPI_ue_context_create_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_create_error_H_ */

