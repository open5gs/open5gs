/*
 * subscribed_default_qos_1.h
 *
 * Provides the subsribed 5QI and the ARP, it may contain the priority level.
 */

#ifndef _OpenAPI_subscribed_default_qos_1_H_
#define _OpenAPI_subscribed_default_qos_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscribed_default_qos_1_s OpenAPI_subscribed_default_qos_1_t;
typedef struct OpenAPI_subscribed_default_qos_1_s {
    int _5qi;
    struct OpenAPI_arp_1_s *arp;
    bool is_priority_level;
    int priority_level;
} OpenAPI_subscribed_default_qos_1_t;

OpenAPI_subscribed_default_qos_1_t *OpenAPI_subscribed_default_qos_1_create(
    int _5qi,
    OpenAPI_arp_1_t *arp,
    bool is_priority_level,
    int priority_level
);
void OpenAPI_subscribed_default_qos_1_free(OpenAPI_subscribed_default_qos_1_t *subscribed_default_qos_1);
OpenAPI_subscribed_default_qos_1_t *OpenAPI_subscribed_default_qos_1_parseFromJSON(cJSON *subscribed_default_qos_1JSON);
cJSON *OpenAPI_subscribed_default_qos_1_convertToJSON(OpenAPI_subscribed_default_qos_1_t *subscribed_default_qos_1);
OpenAPI_subscribed_default_qos_1_t *OpenAPI_subscribed_default_qos_1_copy(OpenAPI_subscribed_default_qos_1_t *dst, OpenAPI_subscribed_default_qos_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscribed_default_qos_1_H_ */

