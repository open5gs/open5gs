/*
 * failure_cause.h
 *
 *
 */

#ifndef _OpenAPI_failure_cause_H_
#define _OpenAPI_failure_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_failure_cause_s OpenAPI_failure_cause_t;
typedef struct OpenAPI_failure_cause_s {
} OpenAPI_failure_cause_t;

OpenAPI_failure_cause_t *OpenAPI_failure_cause_create(
    );
void OpenAPI_failure_cause_free(OpenAPI_failure_cause_t *failure_cause);
OpenAPI_failure_cause_t *OpenAPI_failure_cause_parseFromJSON(cJSON *failure_causeJSON);
cJSON *OpenAPI_failure_cause_convertToJSON(OpenAPI_failure_cause_t *failure_cause);
OpenAPI_failure_cause_t *OpenAPI_failure_cause_copy(OpenAPI_failure_cause_t *dst, OpenAPI_failure_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_cause_H_ */

