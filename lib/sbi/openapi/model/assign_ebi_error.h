/*
 * assign_ebi_error.h
 *
 * 
 */

#ifndef _OpenAPI_assign_ebi_error_H_
#define _OpenAPI_assign_ebi_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "assign_ebi_failed.h"
#include "problem_details.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_assign_ebi_error_s OpenAPI_assign_ebi_error_t;
typedef struct OpenAPI_assign_ebi_error_s {
    struct OpenAPI_problem_details_s *error;
    struct OpenAPI_assign_ebi_failed_s *failure_details;
} OpenAPI_assign_ebi_error_t;

OpenAPI_assign_ebi_error_t *OpenAPI_assign_ebi_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_assign_ebi_failed_t *failure_details
);
void OpenAPI_assign_ebi_error_free(OpenAPI_assign_ebi_error_t *assign_ebi_error);
OpenAPI_assign_ebi_error_t *OpenAPI_assign_ebi_error_parseFromJSON(cJSON *assign_ebi_errorJSON);
cJSON *OpenAPI_assign_ebi_error_convertToJSON(OpenAPI_assign_ebi_error_t *assign_ebi_error);
OpenAPI_assign_ebi_error_t *OpenAPI_assign_ebi_error_copy(OpenAPI_assign_ebi_error_t *dst, OpenAPI_assign_ebi_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_assign_ebi_error_H_ */

