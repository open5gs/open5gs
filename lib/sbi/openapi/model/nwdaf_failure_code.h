/*
 * nwdaf_failure_code.h
 *
 * Possible values are: - UNAVAILABLE_DATA: Indicates the requested statistics information for the event is rejected since necessary data to perform the service is unavailable. - BOTH_STAT_PRED_NOT_ALLOWED: Indicates the requested analysis information for the event is rejected since the start time is in the past and the end time is in the future, which means the NF service consumer requested both statistics and prediction for the analytics. - UNSATISFIED_REQUESTED_ANALYTICS_TIME: Indicates that the requested event is rejected since the analytics information is not ready when the time indicated by the \&quot;timeAnaNeeded\&quot; attribute (as provided during the creation or modification of subscription) is reached. - OTHER: Indicates the requested analysis information for the event is rejected due to other reasons.  
 */

#ifndef _OpenAPI_nwdaf_failure_code_H_
#define _OpenAPI_nwdaf_failure_code_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_failure_code_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_failure_code_s OpenAPI_nwdaf_failure_code_t;
typedef struct OpenAPI_nwdaf_failure_code_s {
} OpenAPI_nwdaf_failure_code_t;

OpenAPI_nwdaf_failure_code_t *OpenAPI_nwdaf_failure_code_create(
);
void OpenAPI_nwdaf_failure_code_free(OpenAPI_nwdaf_failure_code_t *nwdaf_failure_code);
OpenAPI_nwdaf_failure_code_t *OpenAPI_nwdaf_failure_code_parseFromJSON(cJSON *nwdaf_failure_codeJSON);
cJSON *OpenAPI_nwdaf_failure_code_convertToJSON(OpenAPI_nwdaf_failure_code_t *nwdaf_failure_code);
OpenAPI_nwdaf_failure_code_t *OpenAPI_nwdaf_failure_code_copy(OpenAPI_nwdaf_failure_code_t *dst, OpenAPI_nwdaf_failure_code_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_failure_code_H_ */

