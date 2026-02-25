/*
 * termination_cause.h
 *
 * Specifies causes of event reporting termination.
 */

#ifndef _OpenAPI_termination_cause_H_
#define _OpenAPI_termination_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "termination_cause_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_termination_cause_s OpenAPI_termination_cause_t;
typedef struct OpenAPI_termination_cause_s {
} OpenAPI_termination_cause_t;

OpenAPI_termination_cause_t *OpenAPI_termination_cause_create(
);
void OpenAPI_termination_cause_free(OpenAPI_termination_cause_t *termination_cause);
OpenAPI_termination_cause_t *OpenAPI_termination_cause_parseFromJSON(cJSON *termination_causeJSON);
cJSON *OpenAPI_termination_cause_convertToJSON(OpenAPI_termination_cause_t *termination_cause);
OpenAPI_termination_cause_t *OpenAPI_termination_cause_copy(OpenAPI_termination_cause_t *dst, OpenAPI_termination_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_termination_cause_H_ */

