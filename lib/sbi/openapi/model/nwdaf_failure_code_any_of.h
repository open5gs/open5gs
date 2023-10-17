/*
 * nwdaf_failure_code_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_nwdaf_failure_code_any_of_H_
#define _OpenAPI_nwdaf_failure_code_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nwdaf_failure_code_any_of_NULL = 0, OpenAPI_nwdaf_failure_code_any_of_UNAVAILABLE_DATA, OpenAPI_nwdaf_failure_code_any_of_BOTH_STAT_PRED_NOT_ALLOWED, OpenAPI_nwdaf_failure_code_any_of_UNSATISFIED_REQUESTED_ANALYTICS_TIME, OpenAPI_nwdaf_failure_code_any_of_OTHER } OpenAPI_nwdaf_failure_code_any_of_e;

char* OpenAPI_nwdaf_failure_code_any_of_ToString(OpenAPI_nwdaf_failure_code_any_of_e nwdaf_failure_code_any_of);

OpenAPI_nwdaf_failure_code_any_of_e OpenAPI_nwdaf_failure_code_any_of_FromString(char* nwdaf_failure_code_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_failure_code_any_of_H_ */

