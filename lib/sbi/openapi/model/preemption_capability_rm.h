/*
 * preemption_capability_rm.h
 *
 * This enumeration is defined in the same way as the &#39;PreemptionCapability&#39; enumeration, but with the OpenAPI &#39;nullable: true&#39; property. 
 */

#ifndef _OpenAPI_preemption_capability_rm_H_
#define _OpenAPI_preemption_capability_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"
#include "preemption_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_preemption_capability_rm_s OpenAPI_preemption_capability_rm_t;
typedef struct OpenAPI_preemption_capability_rm_s {
} OpenAPI_preemption_capability_rm_t;

OpenAPI_preemption_capability_rm_t *OpenAPI_preemption_capability_rm_create(
);
void OpenAPI_preemption_capability_rm_free(OpenAPI_preemption_capability_rm_t *preemption_capability_rm);
OpenAPI_preemption_capability_rm_t *OpenAPI_preemption_capability_rm_parseFromJSON(cJSON *preemption_capability_rmJSON);
cJSON *OpenAPI_preemption_capability_rm_convertToJSON(OpenAPI_preemption_capability_rm_t *preemption_capability_rm);
OpenAPI_preemption_capability_rm_t *OpenAPI_preemption_capability_rm_copy(OpenAPI_preemption_capability_rm_t *dst, OpenAPI_preemption_capability_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_preemption_capability_rm_H_ */

