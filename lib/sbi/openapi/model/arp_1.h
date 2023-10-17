/*
 * arp_1.h
 *
 * Contains Allocation and Retention Priority information.
 */

#ifndef _OpenAPI_arp_1_H_
#define _OpenAPI_arp_1_H_

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

typedef struct OpenAPI_arp_1_s OpenAPI_arp_1_t;
typedef struct OpenAPI_arp_1_s {
    bool is_priority_level_null;
    int priority_level;
    OpenAPI_preemption_capability_e preempt_cap;
    OpenAPI_preemption_vulnerability_e preempt_vuln;
} OpenAPI_arp_1_t;

OpenAPI_arp_1_t *OpenAPI_arp_1_create(
    bool is_priority_level_null,
    int priority_level,
    OpenAPI_preemption_capability_e preempt_cap,
    OpenAPI_preemption_vulnerability_e preempt_vuln
);
void OpenAPI_arp_1_free(OpenAPI_arp_1_t *arp_1);
OpenAPI_arp_1_t *OpenAPI_arp_1_parseFromJSON(cJSON *arp_1JSON);
cJSON *OpenAPI_arp_1_convertToJSON(OpenAPI_arp_1_t *arp_1);
OpenAPI_arp_1_t *OpenAPI_arp_1_copy(OpenAPI_arp_1_t *dst, OpenAPI_arp_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_arp_1_H_ */

