/*
 * slice_repl_failure_cause.h
 *
 * 
 */

#ifndef _OpenAPI_slice_repl_failure_cause_H_
#define _OpenAPI_slice_repl_failure_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_slice_repl_failure_cause_NULL = 0, OpenAPI_slice_repl_failure_cause_UNSPECIFIED, OpenAPI_slice_repl_failure_cause_NOT_WITHIN_UE_SUBSCRIPTION, OpenAPI_slice_repl_failure_cause_NOT_SUPPORTED_IN_RA, OpenAPI_slice_repl_failure_cause_PARTIAL_NET_SLICE_NOT_SUPPORTED, OpenAPI_slice_repl_failure_cause_INSUFFICIENT_RES } OpenAPI_slice_repl_failure_cause_e;

char* OpenAPI_slice_repl_failure_cause_ToString(OpenAPI_slice_repl_failure_cause_e slice_repl_failure_cause);

OpenAPI_slice_repl_failure_cause_e OpenAPI_slice_repl_failure_cause_FromString(char* slice_repl_failure_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_repl_failure_cause_H_ */

