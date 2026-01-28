/*
 * termination_cause_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_termination_cause_any_of_H_
#define _OpenAPI_termination_cause_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_termination_cause_any_of_NULL = 0, OpenAPI_termination_cause_any_of_TERMINATION_BY_UE, OpenAPI_termination_cause_any_of_TERMINATION_BY_NETWORK, OpenAPI_termination_cause_any_of_NORMAL_TERMINATION } OpenAPI_termination_cause_any_of_e;

char* OpenAPI_termination_cause_any_of_ToString(OpenAPI_termination_cause_any_of_e termination_cause_any_of);

OpenAPI_termination_cause_any_of_e OpenAPI_termination_cause_any_of_FromString(char* termination_cause_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_termination_cause_any_of_H_ */

