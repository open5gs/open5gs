/*
 * slice_repl_outcome.h
 *
 * 
 */

#ifndef _OpenAPI_slice_repl_outcome_H_
#define _OpenAPI_slice_repl_outcome_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_slice_repl_outcome_NULL = 0, OpenAPI_slice_repl_outcome_SUCCESSFUL, OpenAPI_slice_repl_outcome_UNSUCCESSFUL } OpenAPI_slice_repl_outcome_e;

char* OpenAPI_slice_repl_outcome_ToString(OpenAPI_slice_repl_outcome_e slice_repl_outcome);

OpenAPI_slice_repl_outcome_e OpenAPI_slice_repl_outcome_FromString(char* slice_repl_outcome);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_repl_outcome_H_ */

