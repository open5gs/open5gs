/*
 * sm_context_update_error.h
 *
 * 
 */

#ifndef _OpenAPI_sm_context_update_error_H_
#define _OpenAPI_sm_context_update_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ext_problem_details.h"
#include "n2_sm_info_type.h"
#include "ref_to_binary_data.h"
#include "up_cnx_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_update_error_s OpenAPI_sm_context_update_error_t;
typedef struct OpenAPI_sm_context_update_error_s {
    struct OpenAPI_ext_problem_details_s *error;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_msg;
    struct OpenAPI_ref_to_binary_data_s *n2_sm_info;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type;
    OpenAPI_up_cnx_state_e up_cnx_state;
    char *recovery_time;
} OpenAPI_sm_context_update_error_t;

OpenAPI_sm_context_update_error_t *OpenAPI_sm_context_update_error_create(
    OpenAPI_ext_problem_details_t *error,
    OpenAPI_ref_to_binary_data_t *n1_sm_msg,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    OpenAPI_up_cnx_state_e up_cnx_state,
    char *recovery_time
);
void OpenAPI_sm_context_update_error_free(OpenAPI_sm_context_update_error_t *sm_context_update_error);
OpenAPI_sm_context_update_error_t *OpenAPI_sm_context_update_error_parseFromJSON(cJSON *sm_context_update_errorJSON);
cJSON *OpenAPI_sm_context_update_error_convertToJSON(OpenAPI_sm_context_update_error_t *sm_context_update_error);
OpenAPI_sm_context_update_error_t *OpenAPI_sm_context_update_error_copy(OpenAPI_sm_context_update_error_t *dst, OpenAPI_sm_context_update_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_update_error_H_ */

