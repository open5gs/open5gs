/*
 * slice_repl_outcome_info.h
 *
 * Represents the outcome of the AF requested Network Slice replacement. 
 */

#ifndef _OpenAPI_slice_repl_outcome_info_H_
#define _OpenAPI_slice_repl_outcome_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_repl_outcome_info_s OpenAPI_slice_repl_outcome_info_t;
#include "slice_repl_failure_cause.h"
#include "slice_repl_outcome.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_repl_outcome_info_s {
    OpenAPI_slice_repl_outcome_e result;
    OpenAPI_slice_repl_failure_cause_e fail_cause;
};

OpenAPI_slice_repl_outcome_info_t *OpenAPI_slice_repl_outcome_info_create(
    OpenAPI_slice_repl_outcome_e result,
    OpenAPI_slice_repl_failure_cause_e fail_cause
);
void OpenAPI_slice_repl_outcome_info_free(OpenAPI_slice_repl_outcome_info_t *slice_repl_outcome_info);
OpenAPI_slice_repl_outcome_info_t *OpenAPI_slice_repl_outcome_info_parseFromJSON(cJSON *slice_repl_outcome_infoJSON);
cJSON *OpenAPI_slice_repl_outcome_info_convertToJSON(OpenAPI_slice_repl_outcome_info_t *slice_repl_outcome_info);
OpenAPI_slice_repl_outcome_info_t *OpenAPI_slice_repl_outcome_info_copy(OpenAPI_slice_repl_outcome_info_t *dst, OpenAPI_slice_repl_outcome_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_repl_outcome_info_H_ */

