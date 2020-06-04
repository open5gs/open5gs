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

typedef struct OpenAPI_preemption_capability_s OpenAPI_preemption_capability_t;
typedef struct OpenAPI_preemption_capability_s {
} OpenAPI_preemption_capability_t;

OpenAPI_preemption_capability_t *OpenAPI_preemption_capability_create(
    );
void OpenAPI_preemption_capability_free(OpenAPI_preemption_capability_t *preemption_capability);
OpenAPI_preemption_capability_t *OpenAPI_preemption_capability_parseFromJSON(cJSON *preemption_capabilityJSON);
cJSON *OpenAPI_preemption_capability_convertToJSON(OpenAPI_preemption_capability_t *preemption_capability);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_preemption_capability_H_ */

