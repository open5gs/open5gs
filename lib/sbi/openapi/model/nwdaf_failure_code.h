/*
 * nwdaf_failure_code.h
 *
 * 
 */

#ifndef _OpenAPI_nwdaf_failure_code_H_
#define _OpenAPI_nwdaf_failure_code_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nwdaf_failure_code_NULL = 0, OpenAPI_nwdaf_failure_code_UNAVAILABLE_DATA, OpenAPI_nwdaf_failure_code_BOTH_STAT_PRED_NOT_ALLOWED, OpenAPI_nwdaf_failure_code_PREDICTION_NOT_ALLOWED, OpenAPI_nwdaf_failure_code_UNSATISFIED_REQUESTED_ANALYTICS_TIME, OpenAPI_nwdaf_failure_code_NO_ROAMING_SUPPORT, OpenAPI_nwdaf_failure_code_OTHER } OpenAPI_nwdaf_failure_code_e;

char* OpenAPI_nwdaf_failure_code_ToString(OpenAPI_nwdaf_failure_code_e nwdaf_failure_code);

OpenAPI_nwdaf_failure_code_e OpenAPI_nwdaf_failure_code_FromString(char* nwdaf_failure_code);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_failure_code_H_ */

