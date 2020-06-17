/*
 * preemption_capability.h
 *
 *
 */

#ifndef _OpenAPI_preemption_capability_H_
#define _OpenAPI_preemption_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_preemption_capability_NULL = 0, OpenAPI_preemption_capability_NOT_PREEMPT, OpenAPI_preemption_capability_MAY_PREEMPT } OpenAPI_preemption_capability_e;

char* OpenAPI_preemption_capability_ToString(OpenAPI_preemption_capability_e preemption_capability);

OpenAPI_preemption_capability_e OpenAPI_preemption_capability_FromString(char* preemption_capability);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_preemption_capability_H_ */

