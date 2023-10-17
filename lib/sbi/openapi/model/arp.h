/*
 * arp.h
 *
 * Contains Allocation and Retention Priority information.
 */

#ifndef _OpenAPI_arp_H_
#define _OpenAPI_arp_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "preemption_capability.h"
#include "preemption_vulnerability.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_arp_s OpenAPI_arp_t;
typedef struct OpenAPI_arp_s {
    bool is_priority_level_null;
    int priority_level;
    OpenAPI_preemption_capability_e preempt_cap;
    OpenAPI_preemption_vulnerability_e preempt_vuln;
} OpenAPI_arp_t;

OpenAPI_arp_t *OpenAPI_arp_create(
    bool is_priority_level_null,
    int priority_level,
    OpenAPI_preemption_capability_e preempt_cap,
    OpenAPI_preemption_vulnerability_e preempt_vuln
);
void OpenAPI_arp_free(OpenAPI_arp_t *arp);
OpenAPI_arp_t *OpenAPI_arp_parseFromJSON(cJSON *arpJSON);
cJSON *OpenAPI_arp_convertToJSON(OpenAPI_arp_t *arp);
OpenAPI_arp_t *OpenAPI_arp_copy(OpenAPI_arp_t *dst, OpenAPI_arp_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_arp_H_ */

