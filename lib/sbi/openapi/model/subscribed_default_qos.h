/*
 * subscribed_default_qos.h
 *
 * Provides the subsribed 5QI and the ARP, it may contain the priority level.
 */

#ifndef _OpenAPI_subscribed_default_qos_H_
#define _OpenAPI_subscribed_default_qos_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscribed_default_qos_s OpenAPI_subscribed_default_qos_t;
typedef struct OpenAPI_subscribed_default_qos_s {
    int _5qi;
    struct OpenAPI_arp_s *arp;
    bool is_priority_level;
    int priority_level;
} OpenAPI_subscribed_default_qos_t;

OpenAPI_subscribed_default_qos_t *OpenAPI_subscribed_default_qos_create(
    int _5qi,
    OpenAPI_arp_t *arp,
    bool is_priority_level,
    int priority_level
);
void OpenAPI_subscribed_default_qos_free(OpenAPI_subscribed_default_qos_t *subscribed_default_qos);
OpenAPI_subscribed_default_qos_t *OpenAPI_subscribed_default_qos_parseFromJSON(cJSON *subscribed_default_qosJSON);
cJSON *OpenAPI_subscribed_default_qos_convertToJSON(OpenAPI_subscribed_default_qos_t *subscribed_default_qos);
OpenAPI_subscribed_default_qos_t *OpenAPI_subscribed_default_qos_copy(OpenAPI_subscribed_default_qos_t *dst, OpenAPI_subscribed_default_qos_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscribed_default_qos_H_ */

