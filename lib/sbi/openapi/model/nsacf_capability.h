/*
 * nsacf_capability.h
 *
 * NSACF service capabilities (e.g. to monitor and control the number of registered UEs or established PDU sessions per network slice) 
 */

#ifndef _OpenAPI_nsacf_capability_H_
#define _OpenAPI_nsacf_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nsacf_capability_s OpenAPI_nsacf_capability_t;
typedef struct OpenAPI_nsacf_capability_s {
    bool is_support_ue_sac;
    int support_ue_sac;
    bool is_support_pdu_sac;
    int support_pdu_sac;
} OpenAPI_nsacf_capability_t;

OpenAPI_nsacf_capability_t *OpenAPI_nsacf_capability_create(
    bool is_support_ue_sac,
    int support_ue_sac,
    bool is_support_pdu_sac,
    int support_pdu_sac
);
void OpenAPI_nsacf_capability_free(OpenAPI_nsacf_capability_t *nsacf_capability);
OpenAPI_nsacf_capability_t *OpenAPI_nsacf_capability_parseFromJSON(cJSON *nsacf_capabilityJSON);
cJSON *OpenAPI_nsacf_capability_convertToJSON(OpenAPI_nsacf_capability_t *nsacf_capability);
OpenAPI_nsacf_capability_t *OpenAPI_nsacf_capability_copy(OpenAPI_nsacf_capability_t *dst, OpenAPI_nsacf_capability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsacf_capability_H_ */

